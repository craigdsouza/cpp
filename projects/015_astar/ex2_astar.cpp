// Exercise 2 — A* Algorithm
// Compile: cmake --build build
//
// Goal: Implement a_star() in src/road_graph.cpp, then verify it finds the same
// optimal path as shortest_path() (Dijkstra's). Same result, potentially fewer
// nodes examined.

#include "road_graph.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

int main() {
    // TODO: load graph from data/sf_roads_sample.geojson

    // TODO: find northernmost and southernmost nodes (same pattern as ex1)

    // --- Run A* ---
    // TODO: call g.a_star(north_id, south_id) → std::vector<std::string> astar_path
    // TODO: print the path chain:  "A* path: id0 -> id1 -> id2 -> ..."
    // TODO: print the path distance using g.path_distance(astar_path)

    // --- Run Dijkstra's for comparison ---
    // TODO: call g.shortest_path(north_id, south_id) → dijkstra_path
    // TODO: print the Dijkstra path and distance

    // --- Verify they match ---
    // TODO: compare astar_path == dijkstra_path
    //   std::cout << "Paths match: " << (astar_path == dijkstra_path ? "yes" : "NO") << "\n";

    // --- Test edge cases ---
    // TODO: call g.a_star(north_id, north_id) — same start and end
    //   should return a path of length 1 (just the node itself) or {}
    // TODO: add an isolated node manually and call g.a_star(north_id, "isolated")
    //   should return {} (no path)

    // Expected output (approximate):
    //   A* path: 37.7856_-122.4076 -> 37.7841_-122.4069 -> 37.7823_-122.4062  (0.46 km)
    //   Dijkstra: 37.7856_-122.4076 -> 37.7841_-122.4069 -> 37.7823_-122.4062  (0.46 km)
    //   Paths match: yes

    return 0;
}
