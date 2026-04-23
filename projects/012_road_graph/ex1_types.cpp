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
// #include "road_types.h"
#include <iostream>
#include <vector>
#include <algorithm>   // std::for_each

int main() {
    // Create three RoadNode objects:
    //   ("MARKET_4TH",  37.7749f, -122.4194f)
    //   ("MARKET_5TH",  37.7749f, -122.4128f)
    //   ("MISSION_4TH", 37.7729f, -122.4194f)

    // Create two Edge objects leaving MARKET_4TH:
    //   ("MARKET_5TH",  0.15f)
    //   ("MISSION_4TH", 0.12f)

    // Print each node using std::for_each + lambda
    // Format: "[id] lat=X lon=Y"

    // Print each edge using std::for_each + lambda
    // Format: "  -> [to_id] dist=X km"

    return 0;
}
