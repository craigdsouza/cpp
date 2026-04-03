# Quiz — STL Containers

---

## Question 1

You're building a tile cache for a localization module. Each frame, it looks up ~50 tiles by their integer ID. Two colleagues disagree on the container:

- Colleague A says `std::map<int, MapTile>`
- Colleague B says `std::unordered_map<int, MapTile>`

Who is right, and why? What does the losing option give you that the winner doesn't?

## Answer 1
It should be `std::unordered_map<int,MapTile>` , because search on unordered_map is extremely fast and is the same regardless of the size of the cache, which is important when search has to happen very frequently and latency matters. In contrast `std::map<int, MapTile>` would be much slower to produce a result. 

---

## Question 2

A teammate writes this code to check whether a tile exists before using it:

```cpp
std::unordered_map<int, MapTile> store;
// ... store is populated with tiles ...

MapTile& t = store[999];  // tile 999 was never inserted
std::cout << t.label;
```

The code compiles and runs without crashing. What is actually happening, and why is it dangerous? What should be written instead?

## Answer 2
The store doesn't actually have a tile with id 999, but when the reference `t` is created, a ghost MapTile with id 999 and no other values is created because store initializes. It runs, so thus over time ghost tiles accumulate. Instead of directly creating the reference, a lookup should be performed first. 

For instance
```cpp
std::unordered_map<int, MapTile> store;
// ... store is populated with tiles ...

auto item = store.find(999)
if(item != store.end()){
    std::cout << item->second.label;
} else {
    std::cout << "id 999 not available"
}

```

---

## Question 3

You iterate over a `std::map<int, MapTile>` and a `std::unordered_map<int, MapTile>` containing the same four tiles (IDs: 5, 2, 8, 1). What order do you get from each, and why?

## Answer 3
With map, you get 1,2,5,8 and with unordered_map you get no fixed order, perhaps changing on every run.
This is because map is based on a Binary search tree approach and IDs are stored in an ordered fashion.
With unordered map there is no particular order since iteration doesn't matter, only lookup does.

---

## Question 4

You need to store the sequence of tile IDs along a planned route. The route is ordered (tile 3 comes before tile 7), and the same tile can appear multiple times (the route loops back through an intersection). Which STL container do you use, and why does `std::map` not work here even though it also stores tile IDs?

## Answer 4 - 2026-03-31
I think I would use a std::vector<int> because it meets both criteria, it allows repeated entries and second it is ordered. But then I'm not sure, because, if it is ordered and the sequence of tiles is {1,2,3,4,5,6,7,3} // i.e. 3 repeats, isn't this now out of order?

## Answer 4 - 2026-04-01
I would use std::vector<int> because it meets both criteria, it maintains the order of insertion, thus the fixed order of the route is maintained. It also allows repetition, thus you can "revisit the intersection" . "std::map" wouldn't work because it changes insertion order to be sorted in ascending order, and it also doesn't allow repetition of tile IDs, since keys have to be unique.

---

## Question 5

You call `store.find(42)` on a `std::map<int, MapTile>`. Walk through what `it->first` and `it->second` give you. What happens if you call `it->second` when `it == store.end()`? Connect this to the pointer/reference safety rules from Day 2.

## Answer 5
`it->first` gives me the key and `it->second` gives me the value if store.find(42) is actually successful at finding the element in the map. If `it == store.end()` it means the element wasn't found, and thus calling `it->second` would return an error and the program would crash. In day 2, we similarly talked about how dereferencing a pointer should only happen after verifying that the address is not `nullptr`. here `store.end()` similarly helps verify that the pointer to the key, value actually exists, i.e. the key was found.

---

## Grade Log

### 2026-03-31

**Total: 4.0 / 5.0**

| Q | Score | Note |
|---|-------|------|
| Q1 | 0.75 | Correct pick + O(1) reasoning; missed what `map` gives you that `unordered_map` doesn't (sorted iteration, range queries) |
| Q2 | 1.0 | Ghost tile identified, danger explained clearly, correct fix with code |
| Q3 | 0.75 | Correct outputs for both containers; BST reasoning solid for map, but didn't explain hash buckets as the reason for unordered_map's arbitrary order |
| Q4 | 0.5 | Correct container chosen and both criteria cited, but then confused insertion-order (vector) with sorted-order (map) — undermined own reasoning |
| Q5 | 1.0 | Complete: first/second explained, undefined behavior on end() called out, Day 2 nullptr analogy made cleanly |
