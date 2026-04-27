# Quiz — Dijkstra's Shortest Path

---

## Question 1

You run BFS and Dijkstra's on the same road graph, both starting from `"MARKET_4TH"`. BFS reports that `"HOWARD_5TH"` is 3 hops away. Dijkstra's reports a distance of 0.39 km. A colleague says "BFS already told us how far it is — why run Dijkstra's?" Give a concrete example using the road graph data where BFS gives a *wrong* answer to the question "what is the shortest route?" and explain why Dijkstra's is needed. 

## Answer 1
BFS only told us how many hops, but number of hops is a bad proxy for total distance since each hop has a weight of 1. 3 hops by one route could in fact be much shorter than 2 hops by another route. In this case BFS would give a wrong answer to the question what is the shortest route. Dijkstra is needed to unambiguously tell us which is the shortest route.


---

## Question 2

Your Dijkstra's implementation contains this bug:

```cpp
while (!pq.empty()) {
    float d        = pq.top().first;
    std::string current = pq.top().second;
    pq.pop();
    for (const Edge& edge : adj_.at(current)) {
        float new_dist = d + edge.distance_km_;
        if (new_dist < dist[edge.to_id_]) {
            dist[edge.to_id_] = new_dist;
            pq.push({new_dist, edge.to_id_});
        }
    }
}
```

One line is missing. What is it, where does it go, and what goes wrong without it on the road graph (which has bidirectional edges)?

## Answer 2
if (d> dist[current]) continue; 
It goes immediately before the for loop. 
This line skips stale node entries. Stale entries occur for example when the same node can be visited from two other nodes and each has a different distance. If the one with the shorter distance is saved first, and later the one with the greater distance is observed, it becomes a case where d > dist[current], because dist[current] is the lower distance to the node already discovered on a previous pass and d is the new distance. in this case the right thing to do is simply continue.
In the case of bi-directional edges, if you forgot the stale entry check you might push the stale entry anyway. and the "relaxation" continues forever.

---

## Question 4

Your `shortest_path()` implementation reconstructs the path by walking the `prev` map backwards and then calling `std::reverse`. A colleague says: "just push to the front of the vector instead of reversing — same result, one fewer operation."

(a) Is the colleague correct that inserting at the front gives the same result?
(b) Why is `std::reverse` on a `std::vector` generally preferred over repeated front-insertion? What is the time complexity of each approach?

## Answer 4
a) in theory yes, it would give the same result
b) repeated front insertion would mean that each time a new entry is added all the previously added entries must move back one spot. this means many more operations that wouldn't be needed if push_back was used instead. reverse at the end would still mean fewer operations than the repeated slide by one spot.

---

## Grade Log

### 2026-04-27 — 2.25 / 3.0

| Q | Score | Note |
|---|-------|------|
| Q1 | 0.75 | Correct reasoning; missing the concrete road graph example the question asked for |
| Q2 | 0.75 | Correct line, placement, and stale entry explanation; "continues forever" right but re-relaxation cascade not fully articulated |
| Q4 | 0.75 | Correct + O(N²) intuition present; missing explicit complexity notation (O(N²) vs O(N) amortized) |
