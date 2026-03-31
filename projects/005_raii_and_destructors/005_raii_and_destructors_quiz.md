# Quiz — RAII and Destructors

---

## Warm-Up (From Day 4 — Review)

**QR1:**

You need to store the sequence of tile IDs along a planned route. The route is ordered (tile 3 comes before tile 7), and the same tile can appear multiple times (the route loops back through an intersection). Which STL container do you use, and why does `std::map` not work here?

Focus specifically on clarifying what "ordered" means for `std::vector` vs `std::map` — they are ordered in *different* senses.

## Answer QR1

---

## Question 1

A `SensorHandle` class acquires a LiDAR sensor in its constructor and releases it in its destructor. A colleague writes this function:

```cpp
void process_frame() {
    SensorHandle lidar;
    auto data = lidar.read();
    if (data.empty()) {
        throw std::runtime_error("no data");
    }
    localize(data);
}
```

If `data.empty()` is true and the exception is thrown, is the LiDAR sensor released? Explain why. What would happen in a language without RAII (e.g. C) if you wanted the same guarantee?

## Answer 1

---

## Question 2

Given this code:

```cpp
{
    MapTile a(1, 0, 0, "Alpha");
    MapTile b(2, 0, 0, "Beta");
    MapTile c(3, 0, 0, "Gamma");
}
```

In what order are the destructors called, and why? Connect this to how a call stack unwinds.

## Answer 2

---

## Question 3

A teammate writes this `TileCache` class:

```cpp
class TileCache {
    MapTile* tiles;
    int count;
public:
    TileCache(int n) : count(n) {
        tiles = new MapTile[n]; // heap allocation
    }
};
```

There is no destructor. The class is used like this in a localization module running at 100Hz:

```cpp
void localization_tick() {
    TileCache cache(50);
    // ... use cache ...
} // cache goes out of scope here
```

What happens on each call to `localization_tick()`? How bad does this get after 60 seconds? Write the destructor that fixes it.

## Answer 3

---

## Question 4

In Python you write:

```python
with open("tiles.log", "w") as f:
    f.write("tile data")
# file is closed here
```

A C++ colleague says: "RAII is strictly more powerful than Python's `with` statement." Are they right? Explain the difference — specifically what RAII gives you that `with` does not.

## Answer 4

---

## Question 5

You have a `TileLogger` RAII class (opens a file in constructor, closes in destructor). You write:

```cpp
TileLogger a("log_a.txt");
TileLogger b("log_b.txt");
TileLogger c("log_c.txt");
```

Then the program exits normally. Then you test what happens if `b`'s constructor throws (the file can't be opened). Which logs are closed, and which aren't — and why? Connect this to the Day 2 concept of undefined behavior and why defensive programming matters at construction time.

## Answer 5

---

## Grade Log
