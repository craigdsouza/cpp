# Answers — GeoJSON Road Parsing

---

## Question QR1 (From Day 12 — DFS)

**Replacing `std::queue` with `std::stack` makes the algorithm process the most recently discovered node first (LIFO), which produces Depth-First Search.**

A queue is FIFO: BFS always processes the node that has been waiting longest — the shallowest node in the frontier. A stack is LIFO: DFS always processes the node that was just pushed, which is the deepest one discovered so far. The effect is that DFS dives as far as possible down one branch before backtracking, while BFS expands level by level. The visited-set logic and the overall structure of the loop are identical — only the frontier data structure changes.

---

## Question 1

**A 3-point LineString produces 2 edges (4 directed edges for a bidirectional road): point[0]↔point[1] and point[1]↔point[2].**

The LineString models the road centerline as a polyline — a sequence of connected segments, not a single straight line between endpoints. Processing consecutive pairs (index i to i+1) creates one edge per segment. If you connected only the first and last coordinate, you would create a single long edge that skips the intermediate intersection entirely. That intersection node would be missing from the graph, so no route could ever pass through it. More subtly, the edge distance would be the straight-line distance from 4th/Market to 6th/Market, ignoring the geometry of the actual road path — incorrect for routing.

---

## Question 2

**`j["key"]` silently inserts a null value when the key is absent; `j.at("key")` throws `nlohmann::json::out_of_range`.**

This is the exact same ghost-tile problem as `std::map::operator[]` — accessing a missing key with `["key"]` creates a new entry with a null/empty value, which then causes a crash or wrong result when you call `.get<std::string>()` on it. With `.at("key")`, the exception is immediate and descriptive. In a parse loop over real OSM data, `"oneway"` and `"name"` are optional properties — checking `.contains("oneway")` before accessing it with `.at()` is the correct pattern. Without the check, malformed features would silently produce wrong node IDs or incorrect edge directions.

---

## Question 3

**For city-scale routing (< 5 km), Euclidean distance on raw degree values gives meaningfully wrong results because one degree of longitude is not the same distance as one degree of latitude.**

At San Francisco's latitude (38°N), one degree of latitude ≈ 111 km but one degree of longitude ≈ 87 km — a 22% difference. Treating them as equal in a Euclidean formula distorts east-west distances relative to north-south distances. Haversine accounts for this by converting to radians and applying the spherical geometry formula. For sub-kilometer city blocks the absolute error from using raw-degree Euclidean is small but can meaningfully affect which path Dijkstra's picks when two candidate paths are close in distance. In the DRIVE context, Haversine accuracy becomes genuinely important for cross-city and highway routing where distances exceed 10–50 km and Earth's curvature introduces non-negligible error.

---

## Question 4

**Dijkstra's would find a route that travels the wrong way down a one-way road — and would not detect the mistake, because it has no concept of road direction rules.**

Dijkstra's algorithm is purely mechanical: it follows edges in the adjacency list and minimizes total distance. If a reverse edge exists for a one-way road, the algorithm will use it whenever it produces a shorter path, blissfully unaware that this means driving against traffic. The algorithm has no semantic understanding of one-way constraints — that knowledge is encoded entirely in which edges you add to the graph. The bug would appear at runtime as routes that are geometrically valid but legally incorrect. In NVIDIA DRIVE, a planner following such a route could direct the vehicle into oncoming traffic, making correct one-way handling a safety-critical requirement, not just a correctness nicety.

---
