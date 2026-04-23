# Answers — Road Graph Fundamentals

---

## Question QR1 (From Day 8/9/11 — persistent carry-forward)

**The compiler generates a separate, independently type-checked version for each instantiation. This is type safety per instantiation — something copy-paste cannot give you.**

When you write `sort<int>` and `sort<float>`, the compiler emits two separate functions: one that operates on `int` values and one on `float` values. Each is type-checked independently against the concrete type. If your sort comparator accidentally compares `float` values using integer arithmetic, the compiler catches that error in the `sort<float>` instantiation specifically.

Copy-pasting `sort_int` and `sort_float` by hand gives you two functions, but it doesn't give you independent type checking — if `sort_int` has a type error that only manifests with `float` values, you'd never see it unless you actually create `sort_float` and trigger that path. Templates give you automatic, exhaustive per-type verification.

The other benefit copy-paste can't give: **zero cost for unused types** — the compiler only generates code for types that are actually instantiated. If you copy-paste `sort_float` but never call it, it still gets compiled. Templates skip it entirely.

---

## Question 1

**Two disadvantages of a 500×500 matrix for a sparse road network:**

1. **Wasted memory.** A 500×500 float matrix uses 500×500×4 = 1,000,000 bytes (~1 MB) regardless of how many edges actually exist. With 1,200 edges, only 1,200 of those 250,000 cells are non-zero. An adjacency list stores only the 1,200 edges that exist — proportional to actual data size.

2. **Node IDs must be integers in range 0–499.** The matrix representation requires mapping node IDs (e.g. `"MARKET_4TH"`) to integer indices. With `unordered_map<string, vector<Edge>>`, you use the real string IDs directly — no secondary mapping needed, and adding a new node doesn't require resizing the matrix.

Bonus: matrix iteration to find all edges of a node is O(N) per node (scan a whole row). Adjacency list iteration is O(degree) — proportional only to the actual number of edges at that node.

---

## Question 2

**BFS visit order: A, B, C, D, E. The `std::queue` (FIFO) is responsible for the level-by-level property.**

Trace:
```
Queue: [A]          Visited: {A}           Result: []
Pop A → push B, C   Queue: [B, C]          Visited: {A,B,C}    Result: [A]
Pop B → push D      Queue: [C, D]          Visited: {A,B,C,D}  Result: [A,B]
Pop C → D visited, push E  Queue: [D, E]   Visited: {A,B,C,D,E} Result: [A,B,C]
Pop D → no neighbors  Queue: [E]           Result: [A,B,C,D]
Pop E → no neighbors  Queue: []            Result: [A,B,C,D,E]
```

Because `std::queue` is FIFO, all nodes at distance 1 from the start are processed before any node at distance 2. This is what "level-by-level" means. If you used a `std::stack` (LIFO), you'd get depth-first order instead.

---

## Question 3

**Replacing `std::queue` with `std::stack` produces Depth-First Search (DFS), which visits as deep as possible before backtracking.**

On the same graph starting from A, DFS would visit: A, C, E, D, B (or A, B, D, C, E depending on push order — it goes deep on the last-pushed neighbor first).

The key difference: BFS processes all nodes at hop-distance 1 before hop-distance 2. DFS immediately dives as deep as possible along one branch before exploring others. Neither is "faster" in general — they have the same O(V+E) complexity. The choice depends on the problem: BFS finds shortest paths (hop count), DFS is used for cycle detection, topological sort, and connected components.

---

## Question 4

**Without a `visited` set, BFS on a bidirectional edge A↔B would loop infinitely.**

When we pop A and process its edges, we push B onto the queue. Later we pop B and process its edges — which include A. Since we have no visited tracking, we push A again. We pop A again, push B again… the queue never empties and the loop runs forever.

The `visited` set is what breaks the cycle. Before pushing any neighbor onto the queue, check if it's already in `visited`. If it is, skip it. This guarantees each node is processed at most once, making BFS O(V+E) and guaranteed to terminate.

---

## Question 5

**(a)** `std::stof("abc")` throws `std::invalid_argument`. The `distance_km_str` field contains `"abc"` — parsed from the third CSV field. The `to_id` field would be empty (`""`).

**(b)** Loading continues. The `try/catch` is inside the `while(getline)` loop, so catching the exception on this line only skips that one row. The loop then calls `getline` again and processes the next line normally.

**(c)** It depends on implementation order. If your `load()` registers nodes before the `try/catch` (i.e. you parse `from_id` first and call `add_node` before reaching `stof`), then `"MARKET_5TH"` ends up in the graph with no outgoing edge from that row. If you parse all fields first and only call `add_node`/`add_edge` after the successful conversion, it won't be added from this row (but may already exist from other rows). This is why the order of parsing vs registration matters — a good implementation only mutates state after all conversions succeed.

---
