Great context. Geospatial is actually a stronger foundation than you might think — mapping, sensor fusion, coordinate systems, and spatial reasoning are all core to AV work.

Since you're not sure which layer yet, let me give you one solid project per layer, calibrated to your intermediate level. This way you can taste each area and see what excites you.

---

**Hardware — DRIVE Hyperion**
*Project: Build a mini sensor fusion pipeline on a Raspberry Pi*
Connect a camera + IMU (cheap MPU-6050), fuse their data streams to track orientation and detect obstacles. Very hands-on with embedded constraints.
**Difficulty: Medium** — your geospatial background helps with coordinate frames

---

**OS / Safety — Halos**
*Project: Implement a watchdog safety monitor for a simulated AV*
Build a process that monitors a "driving system," detects failures, and triggers safe fallback states. Explore ROS 2 (Robot Operating System), which is the real-world foundation here.
**Difficulty: Medium-Hard** — new concepts but well-documented

---

**Models / Perception — Alpamayo**
*Project: Train a real-time object detection + depth estimation model on dashcam footage*
Use something like YOLO + MiDaS (monocular depth), run inference on a video stream. Your CV experience applies directly.
**Difficulty: Medium** — closest to your existing skillset

---

**AV Software Stack — DRIVE AV**
*Project: Build a lane-following + obstacle avoidance agent in CARLA simulator*
CARLA is a free, open-source AV simulator. Write the perception → planning → control loop yourself. This is the most "full-stack AV engineering" project on this list.
**Difficulty: Hard** — but very impressive on a portfolio

---

**Simulation / Infra — Omniverse**
*Project: Generate a synthetic driving dataset using CARLA + augment with your own geospatial data*
Use your geospatial skills to create realistic map-based scenarios, generate annotated training data, and document the pipeline. Directly mirrors what Omniverse/NuRec does.
**Difficulty: Medium** — your strongest natural fit given your background

---

**My honest recommendation**: Start with the **Omniverse/infra project** since it leverages your geospatial edge immediately and produces a dataset you can then use for the **perception project**. Those two together tell a coherent story: *"I built synthetic training data and trained a model on it."* That's a real NVIDIA workflow.

Want me to go deeper on any of these — tools, resources, or a rough project roadmap?