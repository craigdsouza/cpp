// Exercise 4 — Integration: Route Planner on Real OSM Data
// Compile: cmake --build build  (run from project root, not build/)
//
// Spec: Write a complete GeoJSON route planner. No scaffold — write from scratch.
//
// 1. Load data/sf_roads_sample.geojson into a RoadGraph.
//    Print: "Graph: N nodes, M edges"
//
// 2. Find the northernmost node (highest latitude) and the southernmost node
//    (lowest latitude). Print each node's ID and its (lat, lon) coordinates.
//
// 3. Find the shortest path between the northernmost and southernmost nodes.
//    Print the path chain: "A -> B -> C -> ..."
//    Print the total distance: "Distance: X.XX km"
//
// 4. Find and print the node farthest from the northernmost node
//    (highest finite Dijkstra distance from it).
//    Print: "Farthest from north: [id] at X.XX km"
//
// 5. Detect and print all one-way edges.
//    For each directed edge A→B in adj(), if the reverse B→A does not exist
//    in adj(), it is a one-way edge. Print: "One-way: A → B (X.XX km)"
//    Hint: to check if B→A exists, look for A in the edge list of B.

#include "road_graph.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <limits>

int main() {
    // Write your implementation here.

    return 0;
}
