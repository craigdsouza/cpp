# Quiz — Smart Pointers

---

## Warm-Up (From Day 4 — Review)

**QR1:**

You need to store the sequence of tile IDs along a planned route. The route is ordered (tile 3 comes before tile 7), and the same tile can appear multiple times (the route loops back through an intersection). Which STL container do you use, and why does `std::map` not work here? Be specific about what "ordered" means for `std::vector` vs `std::map` — they are ordered in *different* senses.

## Answer QR1

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

---

## Question 4

You are designing two components for the DRIVE stack:

- **Component A:** A `LidarBuffer` that holds a single scan's point cloud data. It is created at the start of a pipeline tick and consumed by exactly one downstream processor.
- **Component B:** A `MapTile` that is read simultaneously by the localization module, the planning module, and the HMI display.

For each component, should you use `unique_ptr` or `shared_ptr`? Explain your reasoning in terms of ownership, not just "one vs many."

## Answer 4

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

---

## Grade Log
