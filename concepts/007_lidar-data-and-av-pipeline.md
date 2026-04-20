# Concepts: Lidar Data and the AV Sensor Pipeline

> Real-world context for the `LidarFrame` and `PipelineStage` classes built in Day 7. Read this to understand what the data actually looks like and why the code patterns matter.

---

## Is Our `LidarFrame` Realistic?

Our Day 7 class was a simplified model:

```cpp
class LidarFrame {
    float* points;  // raw floats — no structure
    int count;      // just a number
};
```

Real lidar data is the same *shape* — a heap-allocated array owned by one object at a time — but each element is a structured point, not a raw float:

```cpp
struct LidarPoint {
    float x, y, z;        // 3D position in metres, relative to the vehicle
    float intensity;       // how reflective the surface was (0.0–1.0)
    uint16_t ring;         // which laser channel produced this point (0–127)
    double timestamp;      // when exactly this point was captured (nanoseconds)
};

class LidarFrame {
    LidarPoint* points;    // array of structured points
    int count;             // number of points in this frame
    uint64_t frame_id;     // which frame number this is
    double capture_time;   // when the scan started
};
```

The ownership semantics — one owner, move not copy — are identical. Our simplified version captured the right *pattern* while avoiding the complexity of the full struct.

---

## Yes — They Are Like Video Frames

The analogy is exact. Think of a lidar sensor like a depth-sensing camera:

| | Camera | Lidar |
|---|---|---|
| Rate | 30 fps | 10–20 Hz |
| One frame | 1920×1080 pixels | ~500K–1.5M points |
| Frame size | ~6MB uncompressed | ~12–36MB |
| What it captures | colour per pixel | 3D position + intensity per point |
| Produced by | image sensor | rotating laser array |

At 10 Hz, one lidar frame arrives every 100ms. The entire pipeline — capture, filter, detect, fuse, plan — must complete before the next frame arrives. This is a hard real-time deadline.

---

## What the Data Actually Looks Like

Imagine you're standing in a car park. The lidar spins 360° and fires laser pulses in all directions. Each pulse that hits a surface returns a point:

```
Top-down view of a single frame (schematic):

         . . . . . . . .          ← distant building (sparse points)
       .                 .
     .    [ car ]          .      ← parked car (dense rectangular cluster)
    .   . . . . .           .
   .  .           .          .
  .  .  [vehicle]  .          .   ← your car (sensor origin)
   .  .           .          .
    .   . . . . .           .
     .    [ ped ]          .      ← pedestrian (thin vertical cluster)
       .                 .
         . . . . . . . .
              ground              ← dense flat layer of points at z=0
```

Each dot is a `LidarPoint` with an (x, y, z) coordinate. The sensor produces this entire picture 10–20 times per second.

**What different objects look like in a point cloud:**
- **Ground:** Dense, flat layer of points — almost perfectly planar at z ≈ 0. Often the first thing filtered out ("ground removal").
- **Cars:** Rectangular clusters ~1.5m tall, 2m wide, 4m long. The top, sides, and hood return points; windows are transparent and return nothing.
- **Pedestrians:** Thin vertical clusters ~1.7m tall. Very sparse — small cross-section.
- **Trees:** Irregular, semi-random cloud. Leaves scatter the laser unpredictably.
- **Buildings:** Large flat vertical surfaces — dense uniform rows of points.
- **Road markings:** High-intensity returns (white paint is very reflective).

---

## Real Sensor Specs (NVIDIA DRIVE Context)

NVIDIA DRIVE-compatible sensors you'd encounter on a real AV programme:

| Sensor | Channels | Points/frame | Frame rate | Range |
|---|---|---|---|---|
| Velodyne HDL-64E | 64 | ~220,000 | 10 Hz | 120m |
| Hesai AT128 | 128 | ~1,500,000 | 10 Hz | 200m |
| Luminar Hydra | — | ~500,000 | 20 Hz | 300m |
| Innoviz One | — | ~1,200,000 | 25 Hz | 250m |

At 128 channels, 1.5M points, 24 bytes per point:
- One frame: **36MB**
- At 10 Hz: **360MB/second** of raw lidar data

Copying a frame between pipeline stages would mean copying 36MB per transfer. With 6 pipeline stages, that's 216MB per frame, 2.1GB/second. Move semantics reduces this to **24 bytes** per transfer (one pointer + one integer), regardless of frame size.

---

## The Real Pipeline

In NVIDIA DRIVE AV stack (DriveWorks), a frame moves through stages:

```
Sensor hardware
      │
      ▼
1. Capture module
   Receives raw UDP packets from the sensor.
   Assembles them into a complete LidarFrame.
   Owns the frame briefly, then hands it off.
      │  std::move
      ▼
2. Preprocessing module
   Ground removal — filters the flat z≈0 layer.
   Noise filtering — removes isolated spurious points.
   Downsampling — reduces 1.5M points to ~200K for speed.
      │  std::move
      ▼
3. Object detection module
   Clusters remaining points into candidate objects.
   Each cluster becomes a bounding box: "something is here."
      │  std::move (detections, not raw frame)
      ▼
4. Sensor fusion module
   Combines lidar detections with camera and radar.
   Camera confirms: "that cluster is a pedestrian."
   Radar confirms: "that cluster is moving at 12 m/s."
      │  std::move
      ▼
5. Perception module
   Classifies and tracks objects across frames.
   "Pedestrian, 3.2m ahead, stationary, 95% confidence."
      │  std::move
      ▼
6. Planning module
   Decides what the car should do.
   "Brake. Pedestrian is entering the lane."
```

Each `std::move` between stages is exactly what you implemented in Exercise 5. `stage1.ingest(std::move(frame))` followed by `stage2 = std::move(stage1)` is the same pattern — just with real processing at each stage.

---

## Why `PipelineStage` Has No Copy Constructor

In the real stack, two pipeline stages never share the same frame. If two modules both believed they owned the same lidar frame:
- Both would try to run detection on it simultaneously (race condition)
- Both destructors would free the same memory (double-free, crash)
- Results from both would be passed downstream (duplicate detections)

Deleting the copy constructor makes this a **compile error** rather than a runtime crash. The invariant — one frame, one owner — is enforced by the type system, not by discipline.

This is the C++ philosophy: make incorrect states unrepresentable at compile time.

---

## Producer and Consumer Threads

A **thread** is an independent sequence of execution running inside your program. Multiple threads run concurrently — while one thread is waiting on I/O, another can be doing computation.

In the DRIVE pipeline, the sensor and the processing stages run on **separate threads** so neither blocks the other:

```
[Sensor hardware]
      │  fires at 10Hz — one lidar frame every 100ms
      ▼
[Producer thread]            ← writes frames into a shared buffer
   receives UDP packets
   assembles LidarFrames
   pushes to RingBuffer<LidarFrame>
      │
      ▼
[RingBuffer<LidarFrame>]     ← shared buffer between threads
      │
      ▼
[Consumer thread]            ← reads frames out of the shared buffer
   runs ground removal
   runs object detection
   pushes detections downstream
```

The producer doesn't wait for the consumer to finish before the next frame arrives. The `RingBuffer<T>` sits in between — the producer fills it, the consumer drains it at its own pace. If the consumer falls behind, the ring buffer absorbs the backlog, or (in your Day 8 Exercise 2 implementation) overwrites the oldest frame when full.

**Why `RingBuffer<T>` and not `std::vector<T>`:** `std::vector` resizes dynamically — it allocates heap memory when it grows. In a real-time producer-consumer loop running at 10Hz, non-deterministic allocation is a latency hazard. A `RingBuffer` has a fixed capacity set at construction; it never allocates after that. The `FixedBuffer<T, N>` from Day 8 Exercise 4 takes this further — the capacity is a compile-time constant and the memory lives on the stack with zero heap involvement.

**Thread safety comes later (Day 11):** In a real multi-threaded program, both threads touching the ring buffer simultaneously would corrupt it — you need a mutex or lock-free mechanism to protect shared state. For Day 8, `RingBuffer<T>` is the data structure concept; thread safety is added in the Concurrency day.

---

## What You Built vs. What's Real

| Aspect | Day 7 implementation | Real DriveWorks |
|---|---|---|
| Point structure | `float*` (raw bytes) | `LidarPoint` struct with x,y,z,intensity,ring,timestamp |
| Frame metadata | just `count` | frame_id, capture_time, sensor_id, vehicle_pose |
| Pipeline stages | dummy (no processing) | real modules with GPU kernels |
| Threading | single-threaded | each stage runs on its own thread or GPU stream |
| Move semantics | `std::move` between stages | same — plus lock-free queues between threads |
| Copy deleted | yes | yes |

The ownership model you implemented is not a simplification of the real thing — it **is** the real thing. The data inside and the processing at each stage are more complex, but the move-based ownership transfer is identical.
