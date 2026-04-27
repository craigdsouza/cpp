# Answers — Dijkstra's Shortest Path

---

## Question QR1 (From Day 12 — DFS visit order)

**Replacing `std::queue` with `std::stack` produces Depth-First Search (DFS). Visit order: A, C, E, D, B.**

A stack is LIFO — the last element pushed is the first popped. When A is processed, its neighbors B and C are pushed in order. C was pushed last, so C is popped first. C's neighbors D and E are pushed; E is popped first (last pushed). E has no neighbors. Then D is popped. Then B is popped (it was pushed first and sat at the bottom). Then D is attempted again but already visited.

The key difference from BFS: DFS dives as deep as possible along one branch before backtracking, because the stack keeps the most recently discovered node on top. BFS keeps the oldest discovered node on top (queue) which produces level-by-level expansion.

Neither is "faster" — both are O(V+E). The choice depends on the problem: BFS for shortest paths (hop count), DFS for cycle detection, topological sort, connected components.

---

## Question QR2 (From Day 10 — std::optional)

```cpp
// Old signature
TileNode* find_tile(const std::string& id);

// New signature
std::optional<TileNode> find_tile(const std::string& id);

// Returning a found result
return tile;           // implicitly wraps in optional

// Returning not found
return std::nullopt;   // the "nothing" sentinel

// Caller checks and accesses
auto result = find_tile("T42");
if (result.has_value()) {
    std::cout << result->lat;   // arrow operator on optional
    // or: (*result).lat
}
```

`std::optional<T>` makes the "maybe" contract explicit in the type. Unlike a raw pointer it lives on the stack (no heap allocation), there is no risk of a null dereference if you check `has_value()` first, and the caller knows from the signature alone that the function may return nothing.

---

## Question 1

**BFS reports hop count, not distance. On weighted roads, fewer hops ≠ shorter distance.**

On the road graph, consider two routes from `MARKET_4TH` to `MARKET_6TH`:
- Route A: MARKET_4TH → MARKET_5TH → MARKET_6TH (2 hops, 0.15 + 0.15 = 0.30 km)
- Route B: MARKET_4TH → MISSION_4TH → MISSION_5TH → MISSION_6TH → MARKET_6TH (4 hops, but all edges are 0.12 km = 0.48 km)

BFS reports Route A as "2 hops" and Route B as "4 hops." It correctly identifies Route A as the shorter-hop path — and in this case that also happens to be the shorter-distance path.

But imagine edge MARKET_4TH→MARKET_5TH had distance 10.0 km and MARKET_5TH→MARKET_6TH had distance 10.0 km (20.0 km total). BFS would still report 2 hops and call it "closer" than the 4-hop route that is actually only 0.48 km. BFS has no way to distinguish: it treats every edge as cost 1. Dijkstra's uses the actual weights and would correctly return Route B as the shorter path.

---

## Question 2

**The missing line is the stale-entry skip: `if (d > dist[current]) continue;`**

It belongs immediately after `pq.pop()`, before the neighbor loop.

Without it, on a bidirectional graph like this one: when node B is relaxed via two different paths, two entries for B appear in the heap — say (0.12, B) and (0.27, B). After (0.12, B) is popped and B's neighbors are correctly relaxed, the stale (0.27, B) will eventually be popped. Without the skip, the neighbors of B are relaxed again using `d = 0.27`, potentially overwriting correct shorter distances with longer ones and pushing redundant entries. On large graphs this causes incorrect final distances and exponential heap growth.

---

## Question 3

**Trace starting from A:**

Initial: `dist = {A:0, B:∞, C:∞, D:∞}` | `pq = [(0,A)]`

Pop (0, A):
- Relax A→B: 0+4=4 < ∞ → `dist[B]=4`, push (4,B)
- Relax A→C: 0+1=1 < ∞ → `dist[C]=1`, push (1,C)
`dist = {A:0, B:4, C:1, D:∞}` | `pq = [(1,C),(4,B)]`

Pop (1, C):
- Relax C→B: 1+2=3 < 4 → `dist[B]=3`, push (3,B)
- Relax C→D: 1+5=6 < ∞ → `dist[D]=6`, push (6,D)
`dist = {A:0, B:3, C:1, D:6}` | `pq = [(3,B),(4,B),(6,D)]`

Pop (3, B):
- Relax B→D: 3+1=4 < 6 → `dist[D]=4`, push (4,D)
`dist = {A:0, B:3, C:1, D:4}` | `pq = [(4,B),(4,D),(6,D)]`

Pop (4, B): stale (4 > dist[B]=3) → skip

Pop (4, D): no neighbors.

Pop (6, D): stale → skip

**Final: `dist = {A:0, B:3, C:1, D:4}`**

---

## Question 4

**(a) Yes — inserting at the front gives the same final path. (b) `std::reverse` is O(N); repeated front-insertion on `std::vector` is O(N²).**

`push_front` on a `std::vector` (or `insert` at `begin()`) requires shifting every existing element one position to the right — that's O(N) per insertion. Inserting N elements this way is O(N²) total.

`push_back` N times is O(N) amortized, then one `std::reverse` is O(N) — total O(N). On a short city-block path this makes no practical difference, but on a graph with thousands of nodes the quadratic cost would be significant.

The colleague is conceptually right (same result) but algorithmically wrong about "one fewer operation." The preferred C++ idiom is always: push_back + reverse.

If you want to avoid the reverse entirely, use `std::deque` which supports O(1) `push_front`, or `std::list`. But `std::vector` + `std::reverse` is the idiomatic standard pattern.

---

## Question 5

**Dijkstra's fails on negative weights because the stale-entry skip assumes a popped node's distance is final — negative edges can produce a shorter path later.**

Example:
```
A → B: 5
A → C: 3
C → B: -10   ← negative edge
```

Dijkstra's processes A first. Distances: A=0, B=5, C=3. C is popped next (smaller). When C is processed, C→B gives 3 + (-10) = -7, which is less than 5, so dist[B] is updated to -7 and (-7, B) is pushed. So far so good.

But consider a more subtle case:
```
A → B: 2
A → C: 4
B → C: -3   ← negative edge
```
After popping A: dist[B]=2, dist[C]=4. B is popped next. B→C gives 2+(-3)=-1, updating dist[C]=-1. C is eventually popped.

The issue arises when a shorter path to an *already-popped node* exists via a negative edge discovered later. Dijkstra's considers popped nodes finalized. Bellman-Ford handles this by repeatedly relaxing all edges, not relying on a greedy ordering — but it costs O(VE) instead of O((V+E)logV).

For road networks with non-negative distances (you can't have a -0.3 km road), Dijkstra's is correct and efficient.

---
