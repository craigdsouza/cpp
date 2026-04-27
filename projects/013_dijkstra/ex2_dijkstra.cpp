// Exercise 2 — Dijkstra's Algorithm
// Compile: cmake --build build
//
// Goal: Implement dijkstra() in src/road_graph.cpp.
// Open src/road_graph.cpp now — the hint-comments are there.
//
// After implementing, write main() here to test it.

#include "road_graph.h"
#include <iostream>
#include <iomanip>   // std::setprecision
#include <algorithm> // std::for_each

int main() {
    // Load the road graph from data/road_edges.csv
    // Run from the project root (not build/) so the relative path resolves.
    RoadGraph g;
    g.load("data/road_edges.csv");
    std::cout << "Nodes: " << g.node_count() << "  Edges: " << g.edge_count() << "\n\n";


    // Run dijkstra from "MARKET_4TH"
    auto dist = g.dijkstra("MARKET_4TH");


    // Print all distances sorted by value (smallest first).
    // Hint: copy dist into a std::vector<std::pair<float,std::string>>,
    //       sort by .first using a lambda, then print each.
    //
    // Expected output (approximate):
    //   MARKET_4TH   : 0.00 km
    //   MISSION_4TH  : 0.12 km
    //   MARKET_5TH   : 0.15 km
    //   HOWARD_4TH   : 0.24 km
    //   MISSION_5TH  : 0.27 km
    //   MARKET_6TH   : 0.30 km
    //   HOWARD_5TH   : 0.39 km
    //   MISSION_6TH  : 0.42 km
    std::vector<std::pair<float,std::string>> dist_vector;
    for(const auto& [id,d]:dist){
        dist_vector.push_back({d,id});
    }
    std::sort(dist_vector.begin(),dist_vector.end(),[](const auto& a, const auto& b){
        return(a.first<b.first);
    });

    for(const auto& [d,id]:dist_vector){
        std::cout <<std::left <<std::setw(14) << id << ": " << std::fixed << std::setprecision(2) << d << " km" << std::endl;
    }

    // Test with a non-existent start node — dist map should be empty or all-infinity.
    auto dist_fake = g.dijkstra("FAKE_NODE");
    std::cout << "fake node dijkstra result: " << dist_fake.size() << std::endl;

    // TODO: call dijkstra("FAKE_NODE") and print the result size

    return 0;
}
