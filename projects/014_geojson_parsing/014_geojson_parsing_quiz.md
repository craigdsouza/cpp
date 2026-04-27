# Quiz — GeoJSON Road Parsing

---

## Warm-Up (From Day 12 — Review)

**QR1:** Your BFS implementation uses `std::queue`. A colleague replaces it with `std::stack`. Why does this substitution produce Depth-First Search instead of Breadth-First Search? Describe in your own words what changes about the exploration order.

## Answer QR1

---

## Question 1

A GeoJSON LineString for Market Street has 3 coordinate points: 4th/Market, 5th/Market, 6th/Market. When you parse this into a RoadGraph, how many edges are created (assuming bidirectional), and why? What would go wrong if you created a single edge directly between the first and last coordinate instead?

## Answer 1

---

## Question 2

nlohmann/json offers two ways to read a field: `j["key"]` and `j.at("key")`. What is the difference in behavior when the key is absent, and why does this matter when parsing real OSM GeoJSON exports where some features may be missing the `"oneway"` or `"name"` property?

## Answer 2

---

## Question 3

The Haversine formula computes great-circle distance between two (lat, lon) pairs. For routing within a single city where all edges are under 1 km, would it make a meaningful difference if you used a simple Euclidean distance computed directly from the raw degree values (`sqrt(Δlat² + Δlon²)`) instead? Explain when Haversine accuracy becomes genuinely important in the DRIVE context.

## Answer 3

---

## Question 4

When building a RoadGraph from a one-way GeoJSON feature, you call `add_edge` only in the LineString direction. What routing error would Dijkstra's algorithm produce if you accidentally added the reverse edge for a one-way road — and would the algorithm itself detect the mistake?

## Answer 4

---

## Grade Log
