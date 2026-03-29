# Answers — STL Containers

---

## Question 1

**Colleague B is right — `std::unordered_map<int, MapTile>`.**

For 50 lookups per frame at real-time rates (60+ Hz), O(1) average hash lookup beats O(log n) tree lookup decisively. At n=50, log₂(50) ≈ 6 comparisons per lookup vs ~1 for a hash map. Multiply by 50 lookups × 60 frames = 3000 comparisons per second saved.

What `std::map` gives you that `unordered_map` doesn't: **sorted iteration**. If you ever need to process tiles in ID order — e.g. for deterministic logging, range queries ("give me all tiles with ID between 100 and 200"), or sequential pipeline stages — `map` wins. For a pure lookup cache, it's the wrong tool.

---

## Question 2

**`operator[]` silently inserts a default-constructed `MapTile` when the key doesn't exist.**

`store[999]` doesn't find tile 999, so it creates one — a `MapTile` with zeroed numeric fields and an empty `label` string — and inserts it into the store. The code then prints an empty label and leaves a ghost tile in the store. No crash, no warning, silent corruption.

The correct pattern:

```cpp
auto it = store.find(999);
if (it != store.end()) {
    std::cout << it->second.label;
} else {
    std::cout << "[tile 999 not found]\n";
}
```

`find()` never modifies the container. Always prefer it when you're not certain the key exists.

---

## Question 3

**`std::map` yields: 1, 2, 5, 8 (ascending key order, always).**
**`std::unordered_map` yields: arbitrary order, may vary between runs.**

`std::map` is a red-black tree — a self-balancing binary search tree that keeps keys sorted at all times. Every insertion maintains the order. Iteration is an in-order traversal.

`std::unordered_map` is a hash table. Each key is hashed to a bucket. Iteration walks the buckets, not the keys in any logical order. The order depends on the hash function and load factor — it's implementation-defined and unstable.

---

## Question 4

**`std::vector<int>` — a vector of tile IDs.**

Two reasons `std::map` fails:
1. **No duplicates** — map keys are unique. If tile 3 appears twice in the route, `map` can only store one entry for key 3. The repetition is lost.
2. **Wrong ordering model** — `map` sorts by key. A route ordered as [3, 7, 3, 12] would become [3, 7, 12] — the traversal sequence is destroyed.

`std::vector<int>` preserves insertion order and allows duplicates — exactly what a route requires. It models *sequence*, not *lookup*. Paired with an `unordered_map` for the tile store, you get the best of both.

---

## Question 5

**`it->first` is the key (the `int` ID), `it->second` is the value (the `MapTile`).**

When `store.find(42)` succeeds, `it` points to a `std::pair<const int, MapTile>`. `it->first` gives `42`. `it->second` gives the `MapTile` object — you can read its fields or (if the iterator is non-const) modify them.

If `it == store.end()`, dereferencing `it` is **undefined behavior** — exactly like dereferencing a null pointer from Day 2. `end()` is a sentinel, not a valid element. There's no `MapTile` there. The program may crash, corrupt memory, or silently produce garbage.

The connection to Day 2: an iterator past `end()` is the STL equivalent of a null pointer. The rule is the same — always check before dereferencing.

```cpp
auto it = store.find(42);
if (it != store.end()) {       // check, just like nullptr check
    std::cout << it->second.label;  // safe to dereference
}
```

---
