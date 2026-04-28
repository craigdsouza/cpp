# Quiz — GeoJSON Road Parsing

---

## Warm-Up (From Day 12 — Review)

**QR1:** Your BFS implementation uses `std::queue`. A colleague replaces it with `std::stack`. Why does this substitution produce Depth-First Search instead of Breadth-First Search? Describe in your own words what changes about the exploration order.

## Answer QR1
queue implements FIFO, whereas a stack implements LIFO. In Breadth first search our aim is to visit nodes from the start node until the end while sequentially visiting every sibling node . for instance if A is the start and it leads to B (leads to D)and C (leads to E). BFS would visit A -> B -> C -> D -> E. DFS on the other hand would visit in this order, A -> B -> D -> C -> E

---

## Question 1

A GeoJSON LineString for Market Street has 3 coordinate points: 4th/Market, 5th/Market, 6th/Market. When you parse this into a RoadGraph, how many edges are created (assuming bidirectional), and why? What would go wrong if you created a single edge directly between the first and last coordinate instead?

## Answer 1
Four edges are created. Two between 4th/Market & 5th/Market. and another two between 5th/Market & 6th/Market. if the first and last coordinate were joined directly that wouldn't be a LineString anymore, it would be a polygon. But in reality to get from 4th to 6th you have to pass 5th and can't skip it, since Market is a single street intersected by 4th, 5th and 6th.

---

## Question 2

nlohmann/json offers two ways to read a field: `j["key"]` and `j.at("key")`. What is the difference in behavior when the key is absent, and why does this matter when parsing real OSM GeoJSON exports where some features may be missing the `"oneway"` or `"name"` property?

## Answer 2
When the key is absent, j["key"] silently adds the missing key with empty value. this creates the issue of ghost keys we're not aware of. If for instance we use properties["oneway"] and the key wasn't there it would create the key with an empty value. this could lead to undefined errors further down in the workflow.

---

## Question 3

The Haversine formula computes great-circle distance between two (lat, lon) pairs. For routing within a single city where all edges are under 1 km, would it make a meaningful difference if you used a simple Euclidean distance computed directly from the raw degree values (`sqrt(Δlat² + Δlon²)`) instead? Explain when Haversine accuracy becomes genuinely important in the DRIVE context.

## Answer 3
No, in a single city it wouldn't make a meaningful difference if I used Euclidean distance. Haversine becomes genuinely important in the context of computing paths that are several hundreds of kms from start to finish where the difference would be meaningful.

---

## Question 4

When building a RoadGraph from a one-way GeoJSON feature, you call `add_edge` only in the LineString direction. What routing error would Dijkstra's algorithm produce if you accidentally added the reverse edge for a one-way road — and would the algorithm itself detect the mistake?

## Answer 4
Accidentally adding a reverse edge for a one way road produces no routing error. The only issue would be following that path in the real world wouldn't be possible. If I accidentally added the reverse edge for a one way road, the algorithm itself doesn't detect the mistake.

---

## Grade Log

### 2026-04-28

**Total: 3.5 / 5.0**

| Q | Score | Note |
|---|-------|------|
| QR1 | 0.75 | FIFO/LIFO and example correct; missing "last-pushed drives depth-first" mechanism and explicit backtracking framing |
| Q1 | 0.75 | Correct edge count (4 directed); "polygon" explanation wrong; missing "intermediate node absent from graph" as the key routing consequence |
| Q2 | 0.75 | Ghost-key behavior and vocabulary correct; missing specific exception type (`json::out_of_range`) and the failure mode of calling `.get<string>()` on null |
| Q3 | 0.5 | Correct that Haversine matters at long range; wrong that city-scale Euclidean is fine — degree non-equivalence (1° lon ≈ 87 km at 38°N, not 111 km) distorts distances regardless of path length |
| Q4 | 0.75 | All three key points present; thin on reasoning; missing explicit DRIVE safety-critical framing |

