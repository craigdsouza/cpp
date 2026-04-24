// Exercise 1 — Road Node and Edge Types
// Compile: make ex1.exe   OR   cmake -B build && cmake --build build
//
// Goal: Declare Edge and RoadNode in separate header/source files (same pattern as Day 11).
// The header declares, the source defines, this file only includes the header.
//
// Step 1: Fill in include/road_types.h  (declarations + #pragma once)
// Step 2: Fill in src/road_types.cpp    (constructor definitions using ClassName:: + initializer list)
// Step 3: Write main() below

// include road_types.h (user header — quotes, not angle brackets)
#include "road_types.h"
#include <iostream>
#include <vector>
#include <algorithm>   // std::for_each

int main() {
    // Create three RoadNode objects:
    RoadNode rn1("MARKET_4TH",  37.7749f, -122.4194f);
    RoadNode rn2 ("MARKET_5TH",  37.7749f, -122.4128f);
    RoadNode rn3("MISSION_4TH", 37.7729f, -122.4194f);

    std::vector<RoadNode> roadnodes;
    roadnodes.push_back(rn1);
    roadnodes.push_back(rn2);
    roadnodes.push_back(rn3);

    // Create two Edge objects leaving MARKET_4TH:
    Edge e1("MARKET_5TH",  0.15f);
    Edge e2("MISSION_4TH", 0.12f);

    std::vector<Edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);

    // Print each node using std::for_each + lambda
    // Format: "[id] lat=X lon=Y"
    std::for_each(roadnodes.begin(),roadnodes.end(),[](const RoadNode& rn){
        std::cout << "[" << rn.id_ << "] lat=" << rn.lat_ << " lon=" << rn.lon_ << std::endl;
    });

    // Print each edge using std::for_each + lambda
    // Format: "  -> [to_id] dist=X km"
    std::for_each(edges.begin(),edges.end(),[](const Edge& edge){
        std::cout << "[" << edge.to_id_ << "] dist=" << edge.distance_km_ << " km" << std::endl;
    });

    return 0;
}
