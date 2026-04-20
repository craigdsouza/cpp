# Day 6 — Smart Pointers

**Goal:** Replace raw `new`/`delete` with `std::unique_ptr` and `std::shared_ptr`, understand ownership semantics, and write C++where heap memory is never manually freed — the foundation of safe, modern C++ in the NVIDIA DRIVE stack.

---

## Active Recall Warm-Up

Craig - questions posed to me were the repeated in 006_smart_pointers_quiz.md, so I deleted the questions here and answered them in the quiz.

---

## Background

> Before reading: make sure you have read `concepts/005_memory-model.md`. This day assumes you understand the stack/heap distinction and what `new` does under the hood.

In Day 5 you wrote `TileBuffer` — a class that allocated a `int* data` array with `new int[n]` and freed it with `delete[] data` in the destructor. That worked, but it required you to remember to write the destructor. Miss it once and you have a leak. Make a mistake in the destructor (wrong size, double-free) and you have undefined behavior.

Modern C++ solves this with **smart pointers**: wrapper objects that own a heap allocation and free it automatically when they go out of scope. They are RAII applied to heap memory — you get the flexibility of the heap with the safety of the stack.

There are two you will use constantly:

`**std::unique_ptr<T>`** — *exclusive ownership*. One and only one `unique_ptr` can own a given allocation at a time. It cannot be copied, only *moved* (ownership transferred). When the `unique_ptr` goes out of scope, it calls `delete` on what it owns. Think of it as a single owner — like a file opened by one process.

`**std::shared_ptr<T>`** — *shared ownership*. Multiple `shared_ptr`s can point to the same object. Internally, a reference count tracks how many owners exist. When the last owner's `shared_ptr` goes out of scope, the count drops to zero and the object is deleted. Think of it like Python object references — the object lives as long as someone is using it.

In the NVIDIA AV stack, ownership matters for the same reason it matters in all real-time systems: resources (sensor buffers, map tiles, lidar frames) must be freed exactly once, at the right time. `unique_ptr` is the default — most resources have one clear owner. `shared_ptr` is used when multiple subsystems (localization, planning, HMI) need to read the same tile simultaneously.

The Python analogy: Python objects are like `shared_ptr` — reference-counted, freed when no references remain. C++ just makes the ownership model explicit and gives you control over it.

---

## Exercise 1 — unique_ptr for a single tile

**File:** Create `projects/006_smart_pointers/unique_tile.cpp` (new file).

Include `<memory>` (where smart pointers live). Create a `MapTile` with a destructor that prints `"MapTile NNN destroyed"` (reuse from Day 5 or include `map_tile.h`). Then:

```cpp
#include <memory>

int main() {
    {
        std::unique_ptr<MapTile> tile = std::make_unique<MapTile>(1, 37.77, -122.41, "San Francisco");
        tile->print_tile();
    } // tile destroyed here — no delete needed
    std::cout << "after block\n";
}
```

**What to observe:** `make_unique<T>(args...)` is the preferred way to create a `unique_ptr` — it allocates on the heap and wraps it in one step. The tile is destroyed at the closing brace, just like a stack variable, even though it lives on the heap. You wrote no destructor for the owning class and no `delete`. This is RAII for heap memory.

---

## Exercise 2 — Ownership transfer with std::move

**File:** Create `projects/006_smart_pointers/tile_ownership.cpp` (new file).

A tile is loaded by one function and transferred to another. `unique_ptr` cannot be copied — ownership must be *moved* explicitly.

```cpp
std::unique_ptr<MapTile> load_tile() {
    return std::make_unique<MapTile>(42, 37.77, -122.41, "San Francisco");
}

int main() {
    std::unique_ptr<MapTile> owner = load_tile();
    owner->print_tile();

    // Transfer ownership
    std::unique_ptr<MapTile> new_owner = std::move(owner);

    // What is owner now?
    if (owner == nullptr) {
        std::cout << "owner is now null\n";
    }
    new_owner->print_tile();
}
```

Try also adding this line after the `std::move` and observe the compiler error:

```cpp
// std::unique_ptr<MapTile> copy = owner;  // ERROR: unique_ptr cannot be copied
```

**What to observe:** After `std::move`, the original `unique_ptr` is null — it gave up ownership. The heap allocation wasn't copied; ownership was transferred. This is the core guarantee of `unique_ptr`: the object has exactly one owner at all times. The compiler enforces this — the copy line is a compile error, not a runtime mistake.

---

## Exercise 3 — shared_ptr: a tile shared between cache and route

**File:** Create `projects/006_smart_pointers/shared_tile.cpp` (new file).

A map tile is loaded into a tile cache *and* added to an active route plan simultaneously. Both need to keep the tile alive.

```cpp
#include <memory>
#include <vector>

int main() {
    std::shared_ptr<MapTile> tile = std::make_shared<MapTile>(99, 51.5, -0.1, "London");

    std::vector<std::shared_ptr<MapTile>> cache;
    std::vector<std::shared_ptr<MapTile>> route;

    cache.push_back(tile);
    route.push_back(tile);

    std::cout << "use count: " << tile.use_count() << "\n"; // should be 3

    cache.clear();
    std::cout << "after cache clear, use count: " << tile.use_count() << "\n"; // should be 2

    route.clear();
    std::cout << "after route clear, use count: " << tile.use_count() << "\n"; // should be 1

} // tile destroyed here — last owner goes out of scope
```

**What to observe:** `use_count()` shows the reference count. The tile is not destroyed until the last `shared_ptr` to it goes out of scope — even after `cache` and `route` are cleared, `tile` itself still holds a reference. This mirrors Python's garbage collector, but the timing is deterministic: you can predict exactly when the count hits zero.

---

## Exercise 4 — Rewrite TileBuffer without a destructor

**File:** Create `projects/006_smart_pointers/smart_buffer.cpp` (new file).

Take your `TileBuffer` from Day 5 and rewrite it using `unique_ptr<int[]>` instead of `int* data` + `delete[]`. The goal: the destructor disappears entirely.

```cpp
#include <memory>
#include <iostream>

class TileBuffer {
    std::unique_ptr<int[]> data;
    int size;
public:
    TileBuffer(int n) : size(n), data(std::make_unique<int[]>(n)) {
        std::cout << "TileBuffer allocated " << n << " ints\n";
    }
    // No destructor needed — unique_ptr frees the array automatically

    void fill(int value) {
        for (int i = 0; i < size; i++) data[i] = value;
    }
};

int main() {
    for (int i = 0; i < 5; i++) {
        TileBuffer buf(10);
        buf.fill(42);
    }
}
```

**What to observe:** No `~TileBuffer()`. No `delete[]`. The `unique_ptr<int[]>` member owns the array and frees it when `TileBuffer` is destroyed — because destroying an object destroys all its members, and `unique_ptr`'s destructor calls `delete[]`. This is composability of RAII: one RAII type (`TileBuffer`) containing another (`unique_ptr`) — cleanup propagates automatically down the ownership chain.

---

## Exercise 5 — Factory function

**File:** Update `tile_ownership.cpp` (add below `main`).

In real AV code, objects are often created by factory functions that hide construction details. Write a factory for different tile types:

```cpp
std::unique_ptr<MapTile> make_tile(const std::string& city) {
    if (city == "SF")     return std::make_unique<MapTile>(1, 37.77, -122.41, "San Francisco");
    if (city == "Delhi")  return std::make_unique<MapTile>(2, 28.61, 77.20,   "Delhi");
    return nullptr;
}

// In main (add a second block):
{
    auto tile = make_tile("SF");
    if (tile) {
        tile->print_tile();
    }
    auto unknown = make_tile("Tokyo");
    if (!unknown) {
        std::cout << "no tile for Tokyo\n";
    }
}
```

**What to observe:** Returning `unique_ptr` by value from a function is safe and efficient — the compiler elides the copy (NRVO) or moves it. The caller receives exclusive ownership of the heap object. Returning `nullptr` is the clean way to signal "no tile" — just like `T`* returning `nullptr` from Day 2, but now ownership is explicit and the caller can't forget to delete it.

---

## Checkpoint

You've passed Day 6 when you can:

- Explain what `unique_ptr` owns and what happens when it goes out of scope
- Explain why `unique_ptr` cannot be copied but can be moved
- Explain `shared_ptr` reference counting and when the object is destroyed
- Rewrite a class with raw `new`/`delete` using smart pointers and remove the destructor
- Write a factory function that returns `unique_ptr` and explain why it's safe
- Explain when to choose `unique_ptr` vs `shared_ptr` in an AV context
- Explain what `make_unique` and `make_shared` do and why they're preferred over `new`

---

## Exercise Results — 2026-04-15

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — unique_ptr for a single tile | Pass | unique_tile.cpp correct, destructor fires at block close |
| Exercise 2 — Ownership transfer with std::move | Pass | tile_ownership.cpp correct, nullptr check present |
| Exercise 3 — shared_ptr: cache and route | Pass | shared_tile.cpp correct, use_count trace accurate |
| Exercise 4 — Rewrite TileBuffer without a destructor | Pass | smart_buffer.cpp correct (minor: shared_ptr declared, unique_ptr used — discussed) |
| Exercise 5 — Factory function | Pass | make_tile factory in tile_ownership.cpp, nullptr return for unknown city handled |

