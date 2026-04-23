// Exercise 3 — Load from CSV + BFS
// Compile: make ex3.exe   OR   cmake --build build
//
// Goal: Load a real road edge CSV and traverse the graph with BFS —
// the same traversal used for reachability queries in the DRIVE mapping stack.
//
// Step 1: Add load() to src/road_graph.cpp   (CSV parsing + add_edge)
// Step 2: Add bfs() to src/road_graph.cpp    (queue + unordered_set traversal)
// Step 3: Write main() below

#include "road_graph.h"
#include <iostream>
#include <algorithm>   // std::for_each

int main() {
    // Create a RoadGraph and load from "data/road_edges.csv"
    // RoadGraph g;
    // g.load("data/road_edges.csv");

    // Print node count and edge count

    // Run BFS from "MARKET_4TH"
    // auto visit_order = g.bfs("MARKET_4TH");

    // Print the BFS visit order using std::for_each + lambda
    // Format: "BFS from MARKET_4TH: A -> B -> C -> ..."
    // Hint: print each element with a separator, handle the last element specially
    //       or just print each one followed by " -> "

    // Try BFS from a node that doesn't exist ("FAKE_NODE")
    // print "not found" if the result is empty

    return 0;
}
