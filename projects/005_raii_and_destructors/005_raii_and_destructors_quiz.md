# Quiz — RAII and Destructors

---

## Warm-Up (From Day 4 — Review)

**QR1:**

You need to store the sequence of tile IDs along a planned route. The route is ordered (tile 3 comes before tile 7), and the same tile can appear multiple times (the route loops back through an intersection). Which STL container do you use, and why does `std::map` not work here?

Focus specifically on clarifying what "ordered" means for `std::vector` vs `std::map` — they are ordered in *different* senses.

## Answer QR1
std::map wouldn't work because it has to have unique keys, thus a route with a tile that appears twice would have duplicate IDs which makes it not compatible with this use case. Similarly map would not preserve insertion order of tiles which is required for a planned route.
A vector on the other hand, would take care of preservation of insertion order and it also allows duplicate tile IDs.

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
The LiDAR sensor is released, because the SensorHandle class' destructor function releases it. Thus when the exception is thrown the destructor function runs and releases the LIDAR sensor. 
To get the same guarantee in C, you would need a try/catch block to explicitly release the sensor if data is empty. 

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
The destructors are called in reverse order, Gamma, then Beta , then Alpha. 
This is similar to how a call stack unwinds, the last variable in the stack is pushed out first.

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
On each call to `localization_tick()` heap memory is allocated for 50 tiles, but because there is no destructor that memory isn't released after the scope ends, thus after 60 seconds, the code would have run 60*100Hz = 6000 times. This means an enormous amount of memory (6000*50 tiles) is locked away and unusable.

A simple destructor is sufficient to ensure the memory is released each time the cache goes out of scope.
```
~TileCache() {
    delete[] tiles;
}
```

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
Yes my colleague is right. With RAII I don't need to specify a with statement anywhere.
The RAII pattern automatically takes care of releasing any resource (object or file or connection) on the stack when not used anymore.  
The RAII patterns also has the property of composability, ie nested objects within an object that is destroyed are also destroyed.
In Python, explicit destruction is needed and with statements are necessary at the call site everywhere.

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
No clue.

---

## Grade Log

### 2026-04-03

| Q | Score | Note |
|---|-------|------|
| QR1 | 1.0 | Correct on both counts — unique keys and insertion-order vs sort-order clearly distinguished |
| Q1 | 0.75 | Correct + destructor reasoning; said "try/catch" for C but C has no exceptions — should be manual cleanup on every exit path |
| Q2 | 0.75 | Correct order and LIFO connection; missed why reverse order matters (dependency preservation) |
| Q3 | 1 | Correct leak identified, correct destructor written|
| Q4 | 0.75 | Correct + composability noted (impressive); missed that `with` is opt-in — RAII protects even if caller doesn't know about it |
| Q5 | 0.0 | Unanswered |

**Total: 4.25 / 6.0**
