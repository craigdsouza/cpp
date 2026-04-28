# Quiz — A* Search

---

## Warm-Up (From Day 12 — Review)

**QR1:** Your BFS implementation uses `std::queue<std::string>`. A colleague suggests replacing it with `std::stack<std::string>` to "make it faster." What traversal would result, and how would the visit order differ from BFS on a simple graph — say A connected to B and C, B connected to D and E?

## Answer QR1

---

## Warm-Up (From Day 14 — Review)

**QR2:** The Haversine formula computes great-circle distance between two (lat, lon) pairs. For routing within a single city where all edges are under 1 km, would it make a meaningful difference if you used a simple Euclidean distance computed directly from the raw degree values (`sqrt(Δlat² + Δlon²)`) instead? Explain when Haversine accuracy becomes genuinely important in the DRIVE context.

## Answer QR2

---

## Question 1

A* uses `f = g + h` as its priority instead of just `g` (as Dijkstra's does). Suppose the heuristic `h` always returns 0. What algorithm does A* degenerate into, and why? What does this tell you about the relationship between A* and Dijkstra's?

## Answer 1

---

## Question 2

An admissible heuristic never overestimates the true remaining cost. Explain concretely what goes wrong if the heuristic *overestimates* — that is, why would A* return a suboptimal path? Use a two-node example to illustrate.

## Answer 2

---

## Question 3

A* terminates as soon as the goal node is *popped* from the priority queue, not when it is first *pushed*. Why is the pop — not the push — the correct termination point? What property of the priority queue guarantees that the g-value is final at pop time?

## Answer 3

---

## Question 4

Both A* and Dijkstra's accumulate stale heap entries when a node's g-value is improved. In your A* implementation, the stale skip condition is `if (f > g[current] + h(current, goal)) continue`. Explain in plain English what this check is detecting and why it correctly identifies entries that are no longer relevant.

## Answer 4

---

## Question 5

NVIDIA DRIVE's route planner re-queries paths continuously as the vehicle moves. A route query using A* takes 5 ms; the equivalent Dijkstra's query takes 25 ms on the same city graph. Beyond the obvious "it's 5× faster," explain *why* A* expands fewer nodes than Dijkstra's, and describe two specific scenarios in the DRIVE pipeline where this speedup directly improves vehicle behaviour.

## Answer 5

---

## Grade Log
