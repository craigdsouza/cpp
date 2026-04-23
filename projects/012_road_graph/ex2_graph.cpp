// Exercise 2 — RoadGraph Class
// Compile: make ex2.exe   OR   cmake --build build (after cmake -B build)
//
// Goal: Implement RoadGraph with an adjacency list — the core data structure
// behind every road network in production AV systems.
//
// Step 1: Fill in include/road_graph.h  (class declaration)
// Step 2: Fill in src/road_graph.cpp    (add_node, add_edge, node_count, edge_count, print)
// Step 3: Write main() below

#include "road_graph.h"
#include <iostream>

int main() {
    // Create a RoadGraph
    // RoadGraph g;

    // Add five nodes:
    //   MARKET_4TH  (37.7749f, -122.4194f)
    //   MARKET_5TH  (37.7749f, -122.4128f)
    //   MARKET_6TH  (37.7749f, -122.4062f)
    //   MISSION_4TH (37.7729f, -122.4194f)
    //   MISSION_5TH (37.7729f, -122.4128f)

    // Add directed edges:
    //   MARKET_4TH  → MARKET_5TH  (0.15f)
    //   MARKET_5TH  → MARKET_4TH  (0.15f)
    //   MARKET_5TH  → MARKET_6TH  (0.15f)
    //   MARKET_4TH  → MISSION_4TH (0.12f)
    //   MISSION_4TH → MISSION_5TH (0.15f)
    //   MISSION_5TH → MARKET_5TH  (0.12f)

    // Print node count and edge count
    // std::cout << "Nodes: " << g.node_count() << "\n";
    // std::cout << "Edges: " << g.edge_count() << "\n";

    // Call g.print() to show the full adjacency list

    return 0;
}
