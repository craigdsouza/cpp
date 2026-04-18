# Concepts: std::vector Methods

> Reference for the vector operations used across Day 4 onwards. Read this when a vector method behaves unexpectedly or you're unsure which method to reach for.

---

## Construction

```cpp
std::vector<float> a;           // empty vector, size = 0, capacity = 0
std::vector<float> b(4);        // 4 elements, all 0.0f — slots exist, data_[0..3] valid
std::vector<float> c(4, 1.5f);  // 4 elements, all 1.5f
```

---

## `reserve` vs `resize` — the most important distinction

These look similar but do completely different things:

```cpp
std::vector<float> v;

v.reserve(4);   // capacity = 4, size = 0
                // NO elements created — data_[0] is undefined behavior
                // use when you know how many push_backs you'll do

v.resize(4);    // capacity >= 4, size = 4
                // 4 elements created (default-initialized to 0.0f)
                // data_[0..3] are valid and safe to read/write
```

| | `reserve(n)` | `resize(n)` |
|---|---|---|
| `size()` after call | unchanged | `n` |
| `capacity()` after call | at least `n` | at least `n` |
| Elements created? | No | Yes (zero-initialized) |
| `data_[i]` safe? | No | Yes |
| Use when | many `push_back`s coming | need `[]` access immediately |

**Rule of thumb:** if you're going to use `[]` to access elements, use `resize`. If you're going to use `push_back`, use `reserve`.

In the `RingBuffer` exercise: the bug was `reserve` followed by `data_[tail_] = value`. Since no elements were created, the `[]` access was undefined behavior. Fix: `resize`.

---

## Adding and removing elements

```cpp
std::vector<int> v;

v.push_back(10);    // append 10 — size grows by 1
v.push_back(20);
v.push_back(30);    // v = [10, 20, 30]

v.pop_back();       // remove last element — size shrinks by 1
                    // v = [10, 20]
```

**`push_back` vs `[]`:**
- `push_back` creates a new element and appends it — always safe
- `v[i] = x` overwrites an existing element at index `i` — only safe if `i < size()`

---

## Accessing elements

```cpp
v[0]        // no bounds check — fast, undefined behavior if out of range
v.at(0)     // bounds-checked — throws std::out_of_range if invalid
v.front()   // first element (same as v[0])
v.back()    // last element (same as v[v.size()-1])
```

---

## Size and capacity

```cpp
v.size()      // number of elements currently held
v.capacity()  // number of elements allocated (>= size)
v.empty()     // true if size == 0
```

These are often confused. A vector of capacity 8 and size 3 has 3 real elements and 5 unused slots. `size()` is what you almost always want.

---

## Erasing elements

`erase` takes an **iterator**, not an index:

```cpp
v.erase(v.begin());         // remove first element — iterator to first
v.erase(v.begin() + 2);     // remove element at index 2
v.erase(v.begin(), v.end()); // remove all (same as clear())
```

This is why `data_.erase(head_)` in the ring buffer exercise failed — `head_` is an `int` index, not an iterator. The fix is either `data_.erase(data_.begin() + head_)`, or (better for a ring buffer) don't erase at all — just advance the head index and let the slot be overwritten on the next push.

**Erasing from the front is O(n)** — the vector shifts every element left to fill the gap. For a ring buffer this defeats the purpose. Using an index and modulo wrapping avoids this entirely.

---

## Iterators

```cpp
v.begin()   // iterator pointing to first element
v.end()     // iterator pointing one past the last element (sentinel — don't dereference)

// Range-for uses these internally:
for (float x : v) { ... }          // copy
for (const float& x : v) { ... }   // read-only reference (preferred)
for (float& x : v) { ... }         // mutable reference
```

---

## Clearing

```cpp
v.clear();    // size = 0, capacity unchanged, all elements destroyed
```

---

## Modulo wrapping in a ring buffer

The `%` operator gives the remainder after division. In a ring buffer with `capacity_ = 4`:

```
Indices:   0   1   2   3   (then wrap back to 0)
Data:    [ _   _   _   _ ]
```

Each time you push, `tail_` advances by 1. Without modulo, it would run off the end. With modulo:

```
tail_ = (tail_ + 1) % 4

tail_ = 0 → push → tail_ = (0+1) % 4 = 1
tail_ = 1 → push → tail_ = (1+1) % 4 = 2
tail_ = 2 → push → tail_ = (2+1) % 4 = 3
tail_ = 3 → push → tail_ = (3+1) % 4 = 0  ← wraps back to start
tail_ = 0 → push → tail_ = (0+1) % 4 = 1  ← overwrites index 0
```

A concrete walkthrough — `RingBuffer<int>` with capacity 4, pushing 6 values:

```
Push 10: data_=[10, _, _, _]  head_=0  tail_=1  size_=1
Push 20: data_=[10,20, _, _]  head_=0  tail_=2  size_=2
Push 30: data_=[10,20,30, _]  head_=0  tail_=3  size_=3
Push 40: data_=[10,20,30,40]  head_=0  tail_=0  size_=4  ← full
Push 50: data_=[50,20,30,40]  head_=1  tail_=1  size_=4  ← overwrites oldest (10)
Push 60: data_=[50,60,30,40]  head_=2  tail_=2  size_=4  ← overwrites 20

Pop:  returns data_[head_=2] = 30,  head_=3,  size_=3
Pop:  returns data_[head_=3] = 40,  head_=0,  size_=2
Pop:  returns data_[head_=0] = 50,  head_=1,  size_=1
```

The key insight: `head_` and `tail_` chase each other around the fixed array in a circle. `size_` tells you how many real elements are between them. The array itself never grows or shrinks.
