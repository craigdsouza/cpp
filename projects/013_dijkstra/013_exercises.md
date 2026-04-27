# Day 13 — Dijkstra's Shortest Path

**Goal:** Implement Dijkstra's algorithm on the road graph from Day 12, reconstruct shortest paths between intersections, and build a route planner — the same core algorithm that powers NVIDIA DRIVE's navigation engine.

> Before starting: answer the warm-up questions in `013_dijkstra_quiz.md`.

---

## Background

**Why BFS isn't enough**

BFS finds the path with the fewest hops. But on a real road network, a two-hop route through long roads can be much farther than a three-hop route through short ones. BFS treats all edges as equal. Dijkstra's treats edges as having a cost (distance, time) and finds the minimum-cost path.

**The core idea: always process the cheapest unvisited node**

Dijkstra's is a greedy algorithm. It maintains a running estimate of the shortest known distance from the start to every node. At each step it picks the unvisited node with the smallest known distance, updates its neighbors' estimates, and marks it done. Because edges have non-negative weights, once a node is popped from the min-heap its distance is final.

```
dist = {all nodes: ∞, start: 0}   # ∞ means "not yet reached"; start costs 0 to reach itself
pq = min-heap, push (0, start)     # seed the heap with the start node at distance 0

while pq not empty:
    d, current = pop minimum from pq          # greedily pick the cheapest unvisited node
    if d > dist[current]: skip  ← stale entry # a cheaper path was already found; discard this old heap entry
    for each neighbor of current:
        new_dist = dist[current] + edge_weight          # candidate distance through current
        if new_dist < dist[neighbor]:                   # only update if this is a genuine improvement
            dist[neighbor] = new_dist                   # record the new best distance
            push (new_dist, neighbor) to pq             # schedule neighbor for processing at its new cost

return dist   # dist[node] now holds the shortest distance from start to every reachable node
```

**`std::priority_queue` — the min-heap in C++**

The standard queue you used in Day 12 is FIFO. A priority queue serves the element with the highest priority first. By default C++ gives you a **max-heap** (largest on top). For Dijkstra's you need a **min-heap** (smallest distance on top).

```cpp
#include <queue>
using Entry = std::pair<float, std::string>;  // (distance, node_id)

// Min-heap: std::greater<Entry> flips the order so smallest comes first
std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

pq.push({0.12f, "MISSION_4TH"});
pq.push({0.30f, "MARKET_6TH"});

float d        = pq.top().first;   // 0.12 — smallest distance
std::string id = pq.top().second;  // "MISSION_4TH"
pq.pop();                          // remove it
```

`pq.top()` peeks without removing. `pq.pop()` removes without returning. Same split as `std::queue::front()` + `pop()` from Day 12.

**`std::numeric_limits<float>::infinity()`**

Initial distances are set to infinity — any real path will be shorter. C++ provides this via:

```cpp
#include <limits>
float inf = std::numeric_limits<float>::infinity();
// inf + any_positive == inf
// any_real < inf == true
```

**Stale entries**

When you update `dist[node]` and push a new entry, the old entry is still in the heap. When you pop it later, `d > dist[current]` — it's stale. Just `continue`. This is correct and efficient; it avoids a more complex "decrease-key" operation.

**Path reconstruction**

The `dijkstra()` method returns distances only. To get the actual path, you add a `prev` map that records how each node was reached:

```cpp
std::unordered_map<std::string, std::string> prev;
// When relaxing edge current → to_id:
prev[to_id] = current;
```

To reconstruct from `start` to `end`:

```cpp
std::vector<std::string> path;
for (std::string at = end; at != start; at = prev[at])
    path.push_back(at);
path.push_back(start);
std::reverse(path.begin(), path.end());  // #include <algorithm>
```

**Why this matters for DRIVE**

NVIDIA DRIVE's route planning module runs Dijkstra's (or A*) on the road graph to produce the planned route. The route is then handed to the prediction and planning modules to constrain the vehicle's decision space. The `dist` and `prev` maps you implement today are the exact data structures at the core of that pipeline.

---

## Exercise 1 — Priority Queue Basics (~30 min)

**File:** `ex1_priority_queue.cpp` (scaffold provided)

No class needed. Explore `std::priority_queue` with `std::greater<>` as a standalone program:

- Declare a min-heap of `std::pair<float, std::string>` entries
- Push five (distance, node_id) pairs with different distances
- Pop all entries and print them — observe ascending distance order
- Push two entries with equal distance and observe alphabetical tie-breaking

Compile: `cmake --build build` (ex1 has no shared sources)

**What to observe:** The min-heap always serves the smallest distance first. Equal distances are broken by node_id lexicographic order. This ordering property is what makes Dijkstra's greedy step correct.

---

## Exercise 2 — Dijkstra's Distance Map (~75 min)

**File:** `src/road_graph.cpp` (implement `dijkstra()`), `ex2_dijkstra.cpp`

Open `src/road_graph.cpp`. The `dijkstra()` method body has hint-comments — follow them step by step:

1. Initialize `dist` map: all nodes → `std::numeric_limits<float>::infinity()`, start → `0.0f`
2. Push `{0.0f, start_id}` onto a min-heap
3. While the heap is not empty: pop cheapest entry, skip if stale, relax neighbors
4. Return `dist`

In `ex2_dijkstra.cpp`:
- Load `data/road_edges.csv`
- Run `dijkstra("MARKET_4TH")`
- Copy the result into a `std::vector<std::pair<float,std::string>>`, sort by distance ascending, print each

Compile: `cmake --build build`

**Expected output (approximate):**
```
MARKET_4TH   : 0.00 km
MISSION_4TH  : 0.12 km
MARKET_5TH   : 0.15 km
HOWARD_4TH   : 0.24 km
MISSION_5TH  : 0.27 km
MARKET_6TH   : 0.30 km
HOWARD_5TH   : 0.39 km
MISSION_6TH  : 0.42 km
```

**What to observe:** Unlike BFS, Dijkstra's gives you the minimum total distance to each node, not just whether it's reachable. MARKET_5TH (0.15 km) is one hop away; HOWARD_5TH (0.39 km) is reached via a longer chain — Dijkstra's found the cheapest chain automatically.

---

## Exercise 3 — Path Reconstruction (~60 min)

**File:** `src/road_graph.cpp` (implement `shortest_path()` and `path_distance()`), `ex3_shortest_path.cpp`

Extend `dijkstra()`'s logic with a `prev` map:
- `shortest_path(start, end)` — returns the ordered list of node IDs from start to end
- `path_distance(path)` — sums the edge weights along a path vector

In `ex3_shortest_path.cpp`:
- Find and print `MARKET_4TH → HOWARD_5TH` with total distance
- Find and print `MISSION_6TH → HOWARD_4TH` with total distance
- Test the unreachable case with an isolated manually-added node

Compile: `cmake --build build`

**Expected:**
```
MARKET_4TH -> MISSION_4TH -> HOWARD_4TH -> HOWARD_5TH   (0.39 km)
MISSION_6TH -> MISSION_5TH -> HOWARD_5TH -> HOWARD_4TH  (0.42 km)
```

**What to observe:** `shortest_path()` reconstructs the path by walking the `prev` map backwards from the destination, then reversing. The `std::reverse` call is the only new mechanic here — everything else reuses the Dijkstra pattern from Exercise 2.

---

## Exercise 4 — Integration: Route Planner (~90 min)

**File:** `integration.cpp` (no scaffold — write from scratch)

Write a complete route planner. No hints in the file — just a spec. See the spec inside `integration.cpp`.

Build with CMake. Run from the project root directory.

---

## Exercise Results — 2026-04-27

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — Priority Queue Basics | Pass | Min-heap declared correctly, tie-breaking observed |
| Exercise 2 — Dijkstra's Distance Map | Pass | dijkstra() implemented correctly, output sorted and formatted |
| Exercise 3 — Path Reconstruction | Pass | shortest_path() and path_distance() complete, unreachable case tested |
| Exercise 4 — Integration: Route Planner | Pass | All five spec items implemented including farthest node |

---

## Checkpoint

You've passed Day 13 when you can:
- Explain why BFS gives wrong answers on a weighted graph
- Implement Dijkstra's algorithm using `std::priority_queue` with `std::greater<>`
- Explain what a stale heap entry is and why skipping it is correct
- Reconstruct a shortest path using a `prev` map and `std::reverse`
- Implement `path_distance()` by iterating a path vector and looking up edge weights
- Distinguish `pq.top()` (peek) from `pq.pop()` (remove) — same pattern as `std::queue`
