# Day 12 — Graphs, BFS, and Route Planning

---

## 1. Graph Fundamentals

A **graph** is a set of **nodes** (also called vertices) connected by **edges**. That's it. The concept is simple; the power comes from how many real-world systems map onto it naturally.

```
Nodes:  intersections, cities, web pages, people
Edges:  roads, flight routes, hyperlinks, friendships
```

### Directed vs Undirected

A **directed** edge has a direction: A → B means you can travel from A to B, but not necessarily B → A. One-way streets are directed edges.

An **undirected** edge goes both ways. In practice, an undirected road is represented as *two* directed edges: A → B and B → A. The CSV in Day 12 uses this convention — each row is one directed edge, so two rows represent a two-way road.

### Weighted vs Unweighted

A **weighted** edge carries a value — in a road graph, this is distance (km) or travel time. An **unweighted** edge just says "connected." BFS works on unweighted graphs (or ignores weights). Dijkstra's algorithm uses weights to find the shortest path.

### Sparse vs Dense

A graph is **sparse** when most nodes connect to only a few others. A **dense** graph has most nodes connected to most others.

Road networks are extremely sparse: a city intersection has 2–4 roads. This matters because it determines which data structure to use (see Section 2).

---

## 2. Adjacency List vs Adjacency Matrix

These are the two standard ways to store a graph's edges.

### Adjacency Matrix

A 2D array where `adj[i][j]` stores the edge weight from node `i` to node `j` (or 0 / infinity if no edge exists).

```cpp
float adj[N][N];  // N×N entries regardless of how many edges exist
```

**Problem:** For N nodes, this always uses N² space. A city with 10,000 intersections needs 100,000,000 entries — almost all of them zero. This is wasteful for sparse graphs.

**When to use it:** Dense graphs where most pairs of nodes are connected (e.g., social networks, complete graphs in algorithms problems).

### Adjacency List

A map from each node to the list of edges leaving it. Only stores edges that actually exist.

```cpp
std::unordered_map<std::string, std::vector<Edge>> adj_;
// adj_["MARKET_4TH"] = [ Edge{to="MISSION_4TH", dist=0.3},
//                        Edge{to="MARKET_5TH",  dist=0.15} ]
```

**Space:** proportional to the number of edges, not N². For a road network with 10,000 nodes and ~30,000 edges, this is vastly smaller than a matrix.

**Lookup:** "What roads leave MARKET_4TH?" → one hash lookup, then iterate the small vector. Fast.

**When to use it:** Sparse graphs — the standard choice for road networks, dependency graphs, and most real-world graphs.

| | Adjacency Matrix | Adjacency List |
|---|---|---|
| Space | O(N²) | O(N + E) |
| "Does edge A→B exist?" | O(1) | O(degree of A) |
| "All neighbors of A?" | O(N) | O(degree of A) |
| Best for | Dense graphs | Sparse graphs (road networks) |

---

## 3. Breadth-First Search (BFS)

BFS answers: **starting from node X, which nodes can I reach, and in what order?**

It explores the graph level by level — first all nodes 1 hop away, then all nodes 2 hops away, then 3, and so on. This gives BFS a useful property: the first time BFS visits a node, it has found the shortest path to it in terms of hop count.

### The Algorithm

```
1. Put the start node in a queue. Mark it visited.
2. While the queue is not empty:
   a. Take the front node off the queue (call it current).
   b. Record it (add to result).
   c. For each neighbor of current:
      - If not yet visited: mark visited, add to queue.
3. Return the result list.
```

### Worked Example

Graph: A→B, A→C, B→D, C→D, C→E

```
Start: A

Queue: [A]          Visited: {A}            Result: []
Pop A → neighbors B, C
Queue: [B, C]       Visited: {A, B, C}      Result: [A]

Pop B → neighbor D
Queue: [C, D]       Visited: {A, B, C, D}   Result: [A, B]

Pop C → neighbor D (already visited), E
Queue: [D, E]       Visited: {A, B, C, D, E} Result: [A, B, C]

Pop D → no unvisited neighbors
Queue: [E]                                  Result: [A, B, C, D]

Pop E → no unvisited neighbors
Queue: []                                   Result: [A, B, C, D, E]
```

### Why the Visited Set is Required

Every undirected road creates a two-cycle: A→B and B→A. Without a visited set:
- Pop A, push B
- Pop B, push A (again)
- Pop A, push B (again)
- ...infinite loop

The visited set ensures each node is processed exactly once.

### BFS vs DFS

| | BFS | DFS |
|---|---|---|
| Data structure | Queue (FIFO) | Stack (LIFO) — or recursion |
| Order | Level by level (nearest first) | Deep first, backtracks |
| Shortest path (unweighted)? | Yes — first visit = shortest | No |
| Memory | Wider frontier stored in queue | Deeper path stored on stack |
| Use in AV | Reachability, tile pre-loading | Connected components, cycle detection |

Replace `std::queue` with `std::stack` in the BFS code and you get DFS. The visited-set logic stays identical.

---

## 4. C++ Data Structures for BFS

### `std::queue` — FIFO frontier

```cpp
#include <queue>
std::queue<std::string> q;

q.push("A");              // add to back
std::string front = q.front();  // peek at front — does NOT remove
q.pop();                  // remove front — does NOT return value
bool empty = q.empty();
```

**Gotcha:** `front()` and `pop()` are separate. You must call `front()` first to read the value, then `pop()` to remove it. Calling `pop()` alone discards the value silently.

Compare with `std::vector`: `pop_back()` also discards without returning — same pattern.

### `std::unordered_set` — O(1) visited tracking

```cpp
#include <unordered_set>
std::unordered_set<std::string> visited;

visited.insert("A");                          // mark as visited
bool seen = visited.count("A") > 0;          // check — returns 0 or 1
// or:
bool seen2 = visited.find("A") != visited.end();
```

**Why not `std::vector`?** Checking `std::find` on a vector is O(N) — you scan every element. For a graph with thousands of nodes, this makes BFS O(N²) instead of O(N + E). `unordered_set` gives O(1) average lookup via hashing, same as `unordered_map`.

**Why not `std::set`?** `std::set` is O(log N) per lookup (sorted BST). Fine, but `unordered_set` is faster on average when you don't need sorted order — and BFS doesn't.

---

## 5. Route Planning Theory

BFS is the foundation. Real routing algorithms build on top of it by adding weights.

### BFS — "Can I reach it? How many hops?"

Finds reachable nodes. Guarantees minimum hop count. Ignores edge weights (treats all roads as equal length).

**Use in AV:** reachability queries, connected-component analysis, map tile pre-loading.

### Dijkstra's Algorithm — "What is the shortest weighted path?"

Like BFS, but uses a **priority queue** (min-heap) instead of a plain queue. Always processes the closest unvisited node next. Guarantees the shortest total distance path.

```
Priority queue ordered by: cumulative distance from start
Pop cheapest node → update neighbors' distances if shorter path found
```

**Use in AV:** route planning, ETA calculation, finding the fastest path through a road network.

### A* (A-star) — "What is the shortest path, found faster?"

Like Dijkstra, but adds a **heuristic** — an estimate of remaining distance to the goal (e.g., straight-line / Euclidean distance). The heuristic guides the search toward the goal, so A* explores fewer nodes than Dijkstra on large graphs.

```
Priority = actual distance so far + estimated distance remaining
```

**Use in AV:** real-time in-vehicle navigation, mission planning. The standard algorithm behind Google Maps-style routing.

### The Progression

```
BFS          → reachability (hop count, unweighted)
Dijkstra     → shortest path (weighted, no heuristic)
A*           → shortest path (weighted + geographic heuristic, faster in practice)
```

Day 12 implements BFS. Dijkstra and A* are direct extensions — swap `std::queue` for `std::priority_queue` and add distance tracking.

---

## 6. Road Graphs in NVIDIA DRIVE

The DRIVE mapping stack maintains a **road graph** of the operational design domain — every mapped road segment, intersection, and lane connection the vehicle might encounter.

**Reachability queries:** "Given the vehicle is at junction J, which junctions are reachable within 500m?" BFS on the road graph, stopping at a depth/distance threshold. Used to determine which map tiles to stream into memory ahead of time.

**Connected components:** During map ingestion, the road graph is analysed for isolated subgraphs (disconnected road islands). These represent data errors or genuinely separate road networks. BFS from every unvisited node builds the component structure.

**Route planning:** Dijkstra / A* on the weighted road graph produces the planned route. The route is then used by the prediction and planning modules to anticipate relevant junctions and constrain the search space.

**Map tile pre-loading:** The planned route is a path through the road graph. The tiles intersecting that path are queued for pre-load. BFS outward from the route adds a buffer of surrounding tiles for handling deviations.

The adjacency list + BFS you build in Day 12 is the data structure and traversal primitive that all of the above builds on.
