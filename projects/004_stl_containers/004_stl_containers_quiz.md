# Quiz — STL Containers

---

## Question 1

You're building a tile cache for a localization module. Each frame, it looks up ~50 tiles by their integer ID. Two colleagues disagree on the container:

- Colleague A says `std::map<int, MapTile>`
- Colleague B says `std::unordered_map<int, MapTile>`

Who is right, and why? What does the losing option give you that the winner doesn't?

## Answer 1

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

---

## Question 3

You iterate over a `std::map<int, MapTile>` and a `std::unordered_map<int, MapTile>` containing the same four tiles (IDs: 5, 2, 8, 1). What order do you get from each, and why?

## Answer 3

---

## Question 4

You need to store the sequence of tile IDs along a planned route. The route is ordered (tile 3 comes before tile 7), and the same tile can appear multiple times (the route loops back through an intersection). Which STL container do you use, and why does `std::map` not work here even though it also stores tile IDs?

## Answer 4

---

## Question 5

You call `store.find(42)` on a `std::map<int, MapTile>`. Walk through what `it->first` and `it->second` give you. What happens if you call `it->second` when `it == store.end()`? Connect this to the pointer/reference safety rules from Day 2.

## Answer 5

---

## Grade Log
