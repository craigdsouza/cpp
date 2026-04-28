# Answers — A* Search

---

## QR1 (From Day 12)

**Replacing `std::queue` with `std::stack` produces Depth-First Search (DFS), not BFS.**

With a queue (FIFO), BFS processes nodes level by level — on graph A→[B,C], B→[D,E], BFS visits A, then B and C (level 1), then D and E (level 2). Visit order: A, B, C, D, E.

With a stack (LIFO), DFS dives deep before backtracking. Nodes pushed last are processed first. If B and C are pushed in order [B, C], C sits on top and is processed next — diving into C's subtree before B. Visit order: A, C, B, D, E (exact order depends on push sequence).

DFS is not "faster" in general — it may find a path quicker when the goal happens to be in the deeper branch first explored, but it gives no shortest-path guarantees. BFS guarantees the shortest path in terms of hop count.

---

## QR2 (From Day 14)

**Euclidean distance on raw degree values is wrong even at city scale — not just at intercontinental range.**

The key issue is *degree non-equivalence*: 1° of latitude ≈ 111 km everywhere, but 1° of longitude ≈ 111 × cos(lat) km. At San Francisco's latitude (38°N), 1° of longitude ≈ 87 km — about 22% shorter than a degree of latitude. A Euclidean formula treating both as equal shrinks all east-west distances by 22%, which distorts route comparisons and heuristic estimates.

This is separate from Earth's curvature (Haversine handles both). In the DRIVE context, Haversine becomes critical when computing A* heuristics: an Euclidean heuristic on raw degrees will overestimate east-west distances, making the heuristic inadmissible and potentially causing A* to return suboptimal paths.

---

## Question 1

**A* with `h = 0` everywhere degenerates exactly into Dijkstra's algorithm.**

When `h(n) = 0` for all nodes, the priority `f = g + 0 = g` — A* orders the heap by cost-so-far only, which is precisely what Dijkstra's does. There is no heuristic guidance toward the goal; the algorithm explores all directions equally.

This reveals that Dijkstra's is a special case of A* with a zero heuristic. The heuristic is the only thing distinguishing A* — it biases exploration toward the goal without changing the underlying priority-queue mechanism. A maximally informed heuristic (h = exact remaining cost) would expand only nodes on the optimal path; h = 0 expands everything.

---

## Question 2

**An overestimating heuristic can cause A* to skip the optimal path in favour of a path that looks cheaper because the heuristic lied about the remaining cost.**

Simple example: two routes from S to G.
- Route 1: cost-so-far g=3, heuristic says h=1, so f=4.
- Route 2: cost-so-far g=2, heuristic says h=3 (overestimate — actual remaining cost is 1), so f=5.

A* pops Route 1 first (f=4). But Route 2 actually has total cost 2+1=3, which is cheaper. Because the heuristic overestimated Route 2's remaining cost, A* never gave it priority — it expanded Route 1's path and terminated with a suboptimal result.

Admissibility (h ≤ true cost) prevents this: if the heuristic never overpromises, no node on the optimal path can be pushed behind a worse node in the heap.

---

## Question 3

**The pop — not the push — is the correct termination point because only at pop time does the priority queue guarantee finality.**

When a node is first *pushed*, its g-value is the best estimate at that moment. But a better path to the same node may be discovered later, causing a new (lower-f) entry to be pushed. The old entry becomes stale but is still in the heap.

When a node is *popped*, it has the smallest f-value in the heap. Because the heuristic is admissible (never overestimates), no node remaining in the heap can offer a cheaper total-cost route to the goal than what has already been expanded. In particular, the goal node's g-value at pop time is the true shortest-path distance — no better path can exist behind it. Terminating at push would use a potentially non-final g-value.

---

## Question 4

**The stale skip condition detects heap entries whose g-value has since been improved.**

`f > g[current] + h(current, goal)` breaks down as: "Is the f-value stored in this heap entry (`f`) greater than what we would compute for this node right now (`g[current] + h(current, goal)`)?"

If yes, it means that after this entry was pushed, we found a cheaper path to `current` — updating `g[current]` to a lower value. The entry in the heap represents the old, worse path. Since the better path has already been recorded (or will be processed from a lower-f entry), we can safely skip this one.

This is the same mechanism as in Dijkstra's: we never delete stale entries, we just skip them when they surface.

---

## Question 5

**A* expands fewer nodes because the heuristic `h(n)` orders nodes by estimated total cost — biasing expansion toward nodes that are geographically close to the goal.**

Dijkstra's has no information about where the goal is. On a city graph, it expands nodes radially outward from the start in all directions until it reaches the goal. A* concentrates expansion along the corridor between start and goal: nodes "behind" the start or far off to the side have high `h` values and are deprioritised.

Two specific DRIVE scenarios where the speedup matters:

1. **Obstacle detection at highway speed**: When a sensor detects a blocked road at 100 km/h, the planner must re-route within milliseconds. A 5-ms A* query allows 200 re-routes per second vs 40 for Dijkstra's — enough to recalculate before the vehicle commits to the blocked segment.

2. **Continuous replanning in dense traffic**: DRIVE replans the route every few seconds as the map changes (construction zones, traffic signals, new sensor observations). With A*, the planner can use the remaining budget to run more queries with fresher sensor data, rather than spending it on exhaustive Dijkstra's searches.
