# Day 12 — Road Graph Fundamentals

**Goal:** Build a road network graph in C++ — loading real edge data, representing it as an adjacency list, and traversing it with BFS — the same data structure and traversal patterns used in NVIDIA DRIVE's routing and reachability queries.

> Before starting: answer the warm-up questions in `012_road_graph_quiz.md`.

---

## Background

A road network is a **graph**: intersections are nodes, roads are edges. Every routing algorithm — Dijkstra's, A*, the algorithms that power Google Maps and the DRIVE stack's route planning — operates on this structure.

**Adjacency list — the standard graph representation**

The most important design decision in a graph implementation is how to store the edges. The two main choices:

```cpp
// Option A — adjacency matrix (2D array)
float adj[N][N];   // adj[i][j] = distance, 0 if no edge
                   // Problem: N×N space even when edges are sparse

// Option B — adjacency list (map of edge lists)
std::unordered_map<std::string, std::vector<Edge>> adj_;
// Only stores edges that exist — efficient for sparse graphs like road networks
```

Real road networks are extremely sparse: a city intersection connects to 2–4 roads, not to every other intersection in the city. An adjacency list uses memory proportional to the number of edges, not the square of the number of nodes.

**Directed vs undirected**

A **directed** edge from A to B means you can travel A→B but not necessarily B→A (one-way streets). An **undirected** road is represented as two directed edges: A→B and B→A. The CSV in this project includes both directions explicitly — each row is one directed edge.

**BFS — Breadth-First Search**

BFS answers the question: "starting from node X, which nodes can I reach, and in what order?" It processes nodes level by level (all nodes 1 hop away, then 2 hops, then 3…). This gives the property that BFS visit order is also shortest-path order in terms of hop count.

```
Queue: [A]       Visited: {A}         Result: []
Pop A → neighbors B, C  →  Queue: [B, C]  Visited: {A,B,C}  Result: [A]
Pop B → neighbor D      →  Queue: [C, D]  Visited: {A,B,C,D} Result: [A,B]
Pop C → neighbor D (already visited), E → Queue: [D,E] …
```

The `std::queue` (FIFO) is what gives BFS its level-by-level property. Replace it with a stack (LIFO) and you get DFS.

**`std::queue` basics**

```cpp
#include <queue>
std::queue<std::string> q;
q.push("A");           // add to back
std::string front = q.front();  // peek at front (does not remove)
q.pop();               // remove front (does not return value)
bool empty = q.empty();
```

Note: `front()` and `pop()` are separate operations — unlike `std::vector::back()` + `pop_back()` which can be combined.

**`std::unordered_set` for visited tracking**

```cpp
#include <unordered_set>
std::unordered_set<std::string> visited;
visited.insert("A");
bool seen = visited.count("A") > 0;   // or visited.find("A") != visited.end()
```

O(1) average insert and lookup — much faster than scanning a `std::vector<std::string>` for the same element.

**Why this matters for DRIVE**

The DRIVE mapping stack runs reachability queries to answer questions like "is this junction reachable from the vehicle's current position?", "which map tiles need to be loaded given this route?". BFS is also used to build the connected-component structure of the road network during map ingestion. The same `adjacency list + BFS` pattern you'll implement today is at the core of Project 1's road graph engine.

---

## Exercise 1 — Road Node and Edge Types (~30 min)

**Files:** `include/road_types.h`, `src/road_types.cpp`, `ex1_types.cpp`

Declare and implement `Edge` and `RoadNode` using the header/source pattern from Day 11:

- `include/road_types.h` — `#pragma once`, declare both structs with constructor signatures only (no bodies)
- `src/road_types.cpp` — implement both constructors with `ClassName::` prefix and initializer lists
- `ex1_types.cpp` — include only the header, create three `RoadNode` objects and two `Edge` objects, print them using `std::for_each` + lambda

Compile: `make ex1.exe`

**What to observe:** This is the same declaration/definition split as Day 11 — just applying it to the building blocks of a graph. The rest of the exercises add to these types incrementally.

---

## Exercise 2 — RoadGraph Class and Adjacency List (~60 min)

**Files:** `include/road_graph.h`, `src/road_graph.cpp`, `ex2_graph.cpp`

Implement the `RoadGraph` class:

- `include/road_graph.h` — declare the class (scaffold provided as comments; fill it in)
- `src/road_graph.cpp` — implement `add_node`, `add_edge`, `node_count`, `edge_count`, `print`
  - `add_node` inserts the node into `nodes_` and initialises an empty edge vector in `adj_`
  - `add_edge` pushes an `Edge` onto `adj_[from]`
  - `edge_count` iterates `adj_` with `std::for_each` + lambda, sums edge vector sizes
  - `print` prints each node ID followed by its outgoing edges
- `ex2_graph.cpp` — build a 5-node graph manually, print node/edge counts and the full adjacency list

Compile: `make ex2.exe`

**What to observe:** `adj_` only has entries for nodes with outgoing edges (or nodes explicitly added via `add_node`). Iterating it gives you `pair.first` = node ID, `pair.second` = `vector<Edge>`.

---

## Exercise 3 — Load from CSV + BFS (~75 min)

**Files:** `src/road_graph.cpp` (add two methods), `ex3_bfs.cpp`

Extend `RoadGraph` with two more methods:

- `load(const std::string& path)` — parses `data/road_edges.csv` (format: `from_id,to_id,distance_km`, header row). Use `std::ifstream` + `std::stringstream` + `std::stof`. Wrap `stof` in `try/catch std::invalid_argument`. For each row, register any new node IDs with dummy lat/lon `(0.0f, 0.0f)` if they haven't been seen before, then call `add_edge`.
- `bfs(const std::string& start_id) const` — returns BFS visit order as `std::vector<std::string>`. Use `std::queue<std::string>` for the frontier and `std::unordered_set<std::string>` for visited tracking. Return empty vector if `start_id` not in graph.

`ex3_bfs.cpp` — load from `data/road_edges.csv`, print node/edge counts, run BFS from `"MARKET_4TH"`, print the visit order. Also test BFS from a non-existent node.

Compile: `make ex3.exe`

**What to observe:** The `visited` set is what prevents infinite loops on cycles (and all undirected edges create 2-cycles: A→B and B→A). Without it, BFS would loop forever on any bidirectional road.

---

## Exercise 4 — Integration: Road Graph Analysis (~90 min)

**File:** `integration.cpp`

**No scaffold provided.** Write this program from scratch.

Load `data/road_edges.csv` and answer the following queries, printing each result:

1. Summary: node count and edge count
2. BFS visit order from `"MARKET_4TH"` + total reachable count
3. Most connected node (most outgoing edges) — you'll need to expose `adj_` via a const getter or add a `most_connected() const` method to `RoadGraph`
4. Count of nodes with exactly 2 outgoing edges — use `std::count_if` + lambda
5. Whether `"HOWARD_4TH"` is reachable from `"MISSION_6TH"` via BFS

Build with CMake. Expected output listed in the scaffold comments of `integration.cpp`.

---

## Exercise Results — 2026-04-24

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — Road Node and Edge Types | Pass | Header/source split correct; for_each + lambda print implemented |
| Exercise 2 — RoadGraph Class and Adjacency List | Pass | All five methods implemented and verified working |
| Exercise 3 — Load from CSV + BFS | Pass | load() and bfs() both correct; fake node path tested |
| Exercise 4 — Integration: Road Graph Analysis | Pass | All 6 queries implemented from scratch including most_connected() and count_nodes_with_degree() |

---

## Checkpoint

You've passed Day 12 when you can:
- Represent a road network as an adjacency list (`unordered_map<string, vector<Edge>>`)
- Explain why an adjacency list is preferred over a matrix for sparse graphs
- Implement BFS with `std::queue` + `std::unordered_set` and explain why visited tracking is required
- Distinguish `queue.front()` (peek) from `queue.pop()` (remove)
- Load a graph from a CSV file using the `ifstream` + `stringstream` pattern
- Build a multi-file graph project with CMake
