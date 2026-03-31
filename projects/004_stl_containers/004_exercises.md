# Day 4 — STL Containers

**Goal:** Use C++'s standard containers to organise and query map data efficiently — the same building blocks used in map tile pipelines at the core of AV systems like NVIDIA DRIVE.

## Active Recall Warm-Up

*From Day 2/3 — nullptr verify*

Before starting: compile and run `projects/002_ref_and_pointers/nullptr_check.cpp`. Confirm both the null branch and the non-null branch print output. This verifies your fix from last session.

---

## Background

In Python you have `dict`, `list`, and `set` — all runtime-typed containers you rarely think twice about. In C++, the STL (Standard Template Library) gives you a family of typed, high-performance containers. The three you'll use most in AV code:

- **`std::vector<T>`** — ordered, dynamic array. O(1) random access, O(n) search. Use when order matters and you iterate more than you look up.
- **`std::map<K, V>`** — sorted key-value store (red-black tree under the hood). O(log n) lookup. Keys are always in sorted order. Use when you need ordered iteration or range queries.
- **`std::unordered_map<K, V>`** — hash map. O(1) average lookup. No ordering guarantee. Use when you need fast lookup by key and don't care about order.

In AV mapping, tile IDs are looked up constantly. A `std::unordered_map<int, MapTile>` is the natural home for a tile cache — O(1) lookup by ID. A `std::map<int, MapTile>` makes sense when you need tiles in ID order (e.g. sequential processing or debugging). A `std::vector<int>` of tile IDs models a route — ordered, allows repeats.

The Python analogy: `std::unordered_map` ≈ `dict`, `std::map` ≈ `dict` but always sorted, `std::vector` ≈ `list`.

## Exercise 1 — std::map

**File:** Create `projects/004_stl_containers/tile_map.cpp` (new file).

Copy your `MapTile` struct (with constructor) from Day 3 into this file. Build a `std::map<int, MapTile>` and insert at least four tiles with different IDs (try inserting them out of order: e.g. IDs 5, 2, 8, 1). Iterate over the map and print each tile's ID and label.

```cpp
#include <map>
std::map<int, MapTile> store;
store.insert({42, MapTile(42, 37.77, -122.41, "San Francisco")});
```

**What to observe:** Iteration always yields tiles in ascending key order — regardless of insertion order. This is the red-black tree at work. Useful when you need sequential tile processing or want deterministic output.

## Exercise 2 — std::unordered_map

**File:** Update `tile_map.cpp` (add to existing file).

Add a second store: `std::unordered_map<int, MapTile>`. Insert the same four tiles. Iterate and print. Observe the order.

```cpp
#include <unordered_map>
std::unordered_map<int, MapTile> cache;
cache.insert({42, MapTile(42, 37.77, -122.41, "San Francisco")});
```

**What to observe:** Iteration order is arbitrary and may change between runs. For a real-time tile cache where you're looking up by ID on every frame, `unordered_map` is the right call — O(1) beats O(log n) at scale. For a pipeline that processes tiles sequentially, `map` wins.

## Exercise 3 — Iterators and find()

**File:** Update `tile_map.cpp` (add to existing file).

Use `find()` on your `std::map` to search for a tile that exists and one that doesn't. Check the result against `end()` before accessing it.

```cpp
auto it = store.find(99);  // 99 doesn't exist
if (it != store.end()) {
    std::cout << "found: " << it->second.label << "\n";
} else {
    std::cout << "tile 99 not found\n";
}
```

**What to observe:** `find()` returns an iterator — a pointer-like object to the key-value pair. `it->first` is the key, `it->second` is the value. `end()` is a sentinel past the last element — never a valid element. Always check before dereferencing. This is the safe search pattern across all STL containers.

## Exercise 4 — Erase and update

**File:** Update `tile_map.cpp` (add to existing file).

Erase one tile from your `std::map` by key. Then update an existing tile's label using `operator[]`. Print the map before and after both operations.

```cpp
store.erase(2);             // removes tile with ID 2
store[5].label = "updated"; // modifies tile with ID 5 in place
```

**What to observe:** `store[key]` gives a direct reference to the value — you can assign to it. But if the key doesn't exist, `operator[]` silently inserts a default-constructed `MapTile`. This can create ghost tiles. `find()` + check is safer when you're not certain the key exists.

## Exercise 5 — Choosing the right container

**File:** Create `projects/004_stl_containers/tile_router.cpp` (new file).

Model a route as a `std::vector<int>` of tile IDs (ordered, may repeat). Model a tile store as a `std::unordered_map<int, MapTile>`. Write a function:

```cpp
void print_route(const std::vector<int>& route,
                 const std::unordered_map<int, MapTile>& store);
```

That iterates the route in order, looks up each tile in the store, and prints it. If a tile ID isn't in the store, print `"[missing tile NNN]"`.

In `main`, create a route with at least 5 IDs (including one that doesn't exist in the store) and call the function.

**What to observe:** This is the real AV pattern — a route is a sequence of IDs, a tile store is a lookup table. The vector preserves order and allows repeats; the unordered_map provides O(1) lookup. Together they're the foundation of how map providers serve tiles to navigation and planning modules.

---

## Exercise Results — 2026-03-31

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — std::map | Pass | 4 tiles inserted out of order, iterated and printed correctly |
| Exercise 2 — std::unordered_map | Pass | Cache built and iterated; arbitrary order demonstrated |
| Exercise 3 — Iterators and find() | Pass | find() used with end() check; iterator used to read and modify value |
| Exercise 4 — Erase and update | Pass | erase() used correctly; update done via find()+iterator (safer than prescribed operator[]) |
| Exercise 5 — Choosing the right container | Pass | Correct signature, vector+unordered_map pattern, missing tile handled |

---

## Checkpoint

You've passed Day 4 when you can:

- Explain when to choose `vector`, `map`, and `unordered_map` — and the cost of each
- Use `find()` safely and always check against `end()` before dereferencing
- Explain why `map` iteration is ordered but `unordered_map` iteration is not
- Explain the silent insertion danger of `operator[]` on a map
- Model the route + tile store pattern using the right containers for each role
