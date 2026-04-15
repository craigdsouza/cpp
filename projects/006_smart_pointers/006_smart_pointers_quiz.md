# Quiz — Smart Pointers

---

## Warm-Up (From Day 4 — Review)

**QR1:**

You need to store the sequence of tile IDs along a planned route. The route is ordered (tile 3 comes before tile 7), and the same tile can appear multiple times (the route loops back through an intersection). Which STL container do you use, and why does `std::map` not work here? Be specific about what "ordered" means for `std::vector` vs `std::map` — they are ordered in *different* senses.

## Answer QR1
I would use std::vector , because it preserves insertion order. thus if I know the order of tiles along the planned route, I can insert them in that particular order, including insertion of the same tile multiple times. std::map preserves sorted order, i.e. alphanumerical sorting of the keys. thus this wouldn't work because it would not preserve my insertion order for the tiles. 

---

## Warm-Up (From Day 5 — Review)

**QR2:**

You construct three `TileLogger` objects in sequence:

```cpp
TileLogger a("log_a.txt");
TileLogger b("log_b.txt");
TileLogger c("log_c.txt");
```

`b`'s constructor throws because the file can't be opened. Which loggers are properly closed, which aren't, and why? Connect this to what "fully constructed" means for an object.

## Answer QR2
c wasn't constructed and thus log_c.txt was never opened after b's constructor threw an exception. log_b.txt isn't properly closed because its constructor ran partially, thus its destructor doesn't run. a was fully constructed, thus in unwinding, its destructor runs and so log_a.txt is closed properly. 

---

## Question 1

You have this code:

```cpp
void load_frame() {
    std::unique_ptr<SensorFrame> frame = std::make_unique<SensorFrame>(42);
    process(frame);
} // frame destroyed here
```

A colleague asks: "Why not just use `new SensorFrame(42)` and delete it at the end of the function?" Give two concrete reasons why `unique_ptr` is safer, referencing what you know about how scope exit and exceptions work.

## Answer 1
Because 1) in the event of an exception, the delete doesn't execute and memory associated with the frame leaks. 2) in the event of early scope exits, e.g. return statements , you would need one delete statement for every return statement, easy to forget. if you do forget it memory leaks. 

---

## Question 2

A teammate writes this code to pass a tile to a processing function:

```cpp
std::unique_ptr<MapTile> tile = std::make_unique<MapTile>(1, 37.77, -122.41, "SF");
std::unique_ptr<MapTile> tile2 = tile;  // (A)
process_tile(tile2);
```

Line (A) causes a compile error. Explain *why* `unique_ptr` cannot be copied, what you would write instead, and what state `tile` is in after the fix.

## Answer 2
The compile error occurs because `tile` is a unique_ptr, which cannot be copied. its purpose is to maintain exclusivity of ownership. If a copy was possible it would mean that multiple objects, `tile` and `tile2` own the MapTile. But it isn't. Instead transfer of ownership is possible with `std::unique_ptr<MapTile> tile2 = std::move(tile);` . after the fix, `tile` is a nullptr

---

## Question 3

A localization module and a planning module both need access to the same `MapTile`. A colleague proposes:

```cpp
std::shared_ptr<MapTile> tile = std::make_shared<MapTile>(99, 51.5, -0.1, "London");
localization.set_tile(tile);   // localization stores a shared_ptr copy
planning.set_tile(tile);       // planning stores a shared_ptr copy
tile.reset();                  // original released
```

After `tile.reset()`, is the `MapTile` destroyed? Explain reference counting step by step. When is the `MapTile` actually destroyed?

## Answer 3
No, after reset, the `MapTile` isn't destroyed. this is because a shared_ptr to the tile is stored in localization and another in planning. A reference counter keeps count of the number of copies of the MapTile stored in various owners and only after the last copy is destroyed is the MapTile actually destroyed (released). In this case, after tile is created the counter is set to 1, after localization makes a copy it is set to 2 , after planning makes a copy it set to 3. after tile reset happens count drops to 2 and finally when scope ends and localization and planning are destroyed the count goes to 0. 

---

## Question 4

You are designing two components for the DRIVE stack:

- **Component A:** A `LidarBuffer` that holds a single scan's point cloud data. It is created at the start of a pipeline tick and consumed by exactly one downstream processor.
- **Component B:** A `MapTile` that is read simultaneously by the localization module, the planning module, and the HMI display.

For each component, should you use `unique_ptr` or `shared_ptr`? Explain your reasoning in terms of ownership, not just "one vs many."

## Answer 4
for the `LidarBuffer` , unique_ptr is appropriate. this is because, the buffer is used by one entity and when that entity is destroyed the buffer is no longer needed and thus unique_ptr takes care to destroy it. it ensures that no other object can make a copy of the Lidar buffer and accidentally delete it. the one downstream processor owns the unique_ptr and can destroy it or transfer its ownership at any time.
for the `MapTile`, shared_ptr is appropriate, since multiple modules require the MapTile and only when all modules are destroyed does the shared_ptr release the MapTile. this ensures that the MapTile is kept alive for all modules to use as long as needed. No single module owns the shared_ptr, its genuinely a shared resource.


---

## Question 5

A colleague writes this factory function:

```cpp
MapTile* make_tile(int id) {
    return new MapTile(id, 0, 0, "unknown");
}

void run() {
    MapTile* t = make_tile(1);
    process(t);  // process might throw
    delete t;
}
```

There are two problems here. Identify both, then rewrite `make_tile` and `run` using smart pointers to fix them.

## Answer 5
The first issue is that release of the MapTile is not guaranteed, since any exception in `process(t)` means that the delete is not executed. The second issue is that `t` which is the caller of make_tile might not know that it receives a pointer to an object in heap memory and that it has an obligation to delete the pointer.

here is the same code re-written with smart pointers instead

```cpp
std::unique_ptr<MapTile> make_tile(int id){
    std::unique_ptr<MapTile> tile = std::make_unique<MapTile>(id, 0, 0, "unknown");
    return tile;
}

void run() {
    std::unique_ptr<MapTile> t = make_tile(1);
    process(*t);  // process might throw
}

```


---

## Grade Log

### 2026-04-15

**Total: 6.75 / 7.0**

| Q   | Score | Note |
|-----|-------|------|
| QR1 | 1.0   | Correct + complete — insertion-order vs sorted-order distinction nailed |
| QR2 | 1.0   | Full carry-forward recovery — partially constructed object semantics correct |
| Q1  | 1.0   | Both reasons solid: exception path + early return paths |
| Q2  | 1.0   | Why, fix, and resulting state all correct |
| Q3  | 1.0   | Step-by-step reference count trace correct |
| Q4  | 0.75  | Ownership framing present but LidarBuffer mixes usage count with ownership reasoning |
| Q5  | 1.0   | Both problems identified, rewrite correct including process(*t) dereference |
