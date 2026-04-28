# Day 15 — A* Search on Real SF Road Data

**Goal:** Implement A* search on the real SF road graph loaded from GeoJSON, benchmark it against Dijkstra's to quantify the speedup, and understand why A* is the algorithm of choice in NVIDIA DRIVE's time-critical route planning pipeline.

> Before starting: answer the warm-up questions in `015_astar_quiz.md`.

---

## Background

**Why A* instead of Dijkstra's?**

Dijkstra's expands every reachable node in order of cost from the start. It has no information about where the goal is — it searches in all directions equally. On a large city road network with tens of thousands of nodes, this is wasteful when you only care about one specific destination.

A* fixes this by adding a *heuristic* — an estimate of the remaining cost from any node to the goal. Instead of prioritizing nodes by cost-so-far (`g`), A* prioritizes by *total estimated cost* (`f = g + h`):

- `g(n)` = actual cost from start to node n (same as Dijkstra's `dist[n]`)
- `h(n)` = heuristic estimate of cost from n to the goal

The effect: A* biases its search toward the goal, expanding fewer nodes and finding the path faster — while still producing the optimal result, provided the heuristic is *admissible*.

```
Dijkstra's:  priority = g(n)           ← cost so far only
A*:          priority = g(n) + h(n)    ← cost so far + estimated remaining
```

**Admissible heuristic**

A heuristic is admissible if it *never overestimates* the true remaining cost. Haversine distance (straight-line between GPS coordinates) qualifies: roads curve, detour through intersections, and follow a street grid — the actual road distance is always ≥ the straight-line distance. An admissible heuristic guarantees A* returns the optimal path.

**Early termination**

Once the goal node is popped from the priority queue, its g-value is final — stop immediately. Dijkstra's must drain the heap (or check every pop). This alone can halve A*'s runtime when the goal is geographically close.

**Stale entries — unchanged from Dijkstra's**

A* generates stale heap entries for the same reason Dijkstra's does: when a node's g-value is updated, the old entry remains in the heap. Skip it with `if (f > g[current] + h(current, goal)) continue`.

**Why this matters for DRIVE**

NVIDIA DRIVE's route planner re-queries paths continuously as the vehicle moves and obstacles are detected. A query that takes 5ms instead of 25ms means the planner can re-route 5× more frequently — critical at highway speed. A* is the production algorithm for single-pair routing precisely because it scales to large graphs where Dijkstra's exhaustive search becomes a bottleneck.

---

## Exercise 1 — The Heuristic (~30 min)

**File:** `ex1_heuristic.cpp` (scaffold provided)

Implement `heuristic(from_id, to_id)` in `src/road_graph.cpp`:
- Look up the two `RoadNode` structs from `nodes_`
- Return `haversine_km(a.lat_, a.lon_, b.lat_, b.lon_)`

Then in `ex1_heuristic.cpp`:
- Load the GeoJSON and find the northernmost and southernmost nodes
- Print both nodes' IDs and coordinates
- Print the heuristic estimate (straight-line) and the actual shortest-path distance
- Verify admissibility: heuristic ≤ actual (print `"Admissible: yes"` or `"NO — bug!"`)
- Print the circuity ratio: `actual / heuristic`

Compile: `cmake --build build`

**Expected output (approximate):**
```
North: 37.7856_-122.4076  (lat=37.7856, lon=-122.4076)
South: 37.7823_-122.4045  (lat=37.7823, lon=-122.4045)
Heuristic (straight line): 0.38 km
Actual shortest path:      0.46 km
Admissible: yes
Circuity ratio: 1.21x
```

**What to observe:** The circuity ratio — how much longer real roads are than the straight line — is typically 1.2–1.5 for urban grids. This gap is exactly what A* exploits: the heuristic guides search toward the goal without ever overpromising how short the remaining path will be.

---

## Exercise 2 — A* Algorithm (~75 min)

**Files:** `src/road_graph.cpp` (implement `a_star()`), `ex2_astar.cpp`

Implement `a_star(start_id, end_id)` in `src/road_graph.cpp`. The hint-comments in the file walk through each step. Key differences from `shortest_path()`:

1. Use a `g` map (cost so far) instead of `dist` — same concept, clearer name for A*
2. Push `{heuristic(start_id, end_id), start_id}` as the first heap entry (f = 0 + h)
3. For each neighbor: compute `tentative_g = g[current] + edge.distance_km_`; if it improves `g[neighbor]`, push `{tentative_g + heuristic(neighbor, end_id), neighbor}`
4. Early termination: `if (current == end_id) break`
5. Stale skip: `if (f > g[current] + heuristic(current, end_id)) continue`
6. Path reconstruction: identical to `shortest_path()` — back-walk `prev`, `std::reverse`

In `ex2_astar.cpp`:
- Run A* between northernmost and southernmost nodes; print path and distance
- Run Dijkstra's `shortest_path()` on the same pair; verify paths match
- Test edge cases: same start/end, unreachable node

Compile: `cmake --build build`

**What to observe:** A* and Dijkstra's return the same optimal path — the heuristic changes the *order* of exploration, not the *result*. On this small graph the difference in nodes examined is modest; on a real city-scale graph (50K+ nodes) it's dramatic.

---

## Exercise 3 — Benchmarking with std::chrono (~60 min)

**File:** `ex3_benchmark.cpp` (scaffold provided)

New mechanic — `std::chrono` for timing:

```cpp
#include <chrono>

auto t0 = std::chrono::high_resolution_clock::now();
// ... code to time ...
auto t1 = std::chrono::high_resolution_clock::now();
long long us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
std::cout << us << " µs\n";
```

`high_resolution_clock` gives the finest timer available. `duration_cast<microseconds>` converts the raw duration to microseconds. `.count()` extracts the integer value.

Define 5 query pairs from the node set, time both algorithms on each, and print:

```
Query                                Dijkstra      A*            Speedup
----------------------------------------------------------------------
37.7856_-122.4076 → 37.7823_...     42 µs         18 µs         2.3x
...
```

Compile: `cmake --build build`

**What to observe:** On this small sample graph, both algorithms are fast and timing is noisy at µs resolution — run each query a few times if needed. The speedup *pattern* matters more than absolute numbers. Note in a comment which queries show the most A* benefit. Hypothesis to test: does A* help more when the goal is far from the start?

---

## Exercise 4 — Integration: A* Route Planner + JSON Export (~90 min)

**File:** `integration.cpp` (no scaffold — write from scratch)

Write a complete A*-powered route planner that exports results for Python visualization in Day 16. No hints — just a spec:

1. Load `data/sf_roads_sample.geojson`. Print node and edge counts.
2. Find the northernmost, southernmost, easternmost, and westernmost nodes. Print each.
3. Run A* for two routes: north→south and east→west. Print each path chain, distance, and time (µs).
4. Export both routes to `data/routes.json` as a JSON array:
   ```json
   [
     { "name": "north_to_south", "distance_km": 0.46,
       "coords": [[37.7856, -122.4076], [37.7841, -122.4069], ...] },
     { "name": "east_to_west",   "distance_km": ..., "coords": [...] }
   ]
   ```
   Use `nlohmann::json` to build and write: `doc.dump(2)` to a `std::ofstream`.
   Coordinates are `[lat, lon]` (Python/Folium convention — note the swap from GeoJSON's `[lon, lat]`).
5. Print a benchmark summary: A* vs Dijkstra's on both routes, with speedup.

---

## Checkpoint

You've passed Day 15 when you can:
- Explain the difference between Dijkstra's and A* in terms of the priority function (`g` vs `g + h`)
- Explain what "admissible heuristic" means and why Haversine qualifies for road routing
- Implement A* from scratch using the `g + h` priority pattern, early termination, and stale skip
- Use `std::chrono` to time code and compute a speedup ratio
- Write a JSON file using `nlohmann::json`'s `.dump()` method
