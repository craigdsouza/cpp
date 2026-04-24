// Exercise 1 — Road Types Header
// Declare Edge and RoadNode structs. No implementation goes here.
//
// Step 1: #pragma once
// Step 2: #include <string>
//
// struct Edge {
//     std::string to_id;      // destination node ID
//     float distance_km;
//     Edge(const std::string& to_id, float distance_km);
// };
//
// struct RoadNode {
//     std::string id;
//     float lat, lon;
//     RoadNode(const std::string& id, float lat, float lon);
// };

#pragma once
#include <string>

struct Edge{
    std::string to_id_;
    float distance_km_;
    Edge(const std::string& to_id, float distance_km);
};

struct RoadNode{
    std::string id_;
    float lat_,lon_;
    RoadNode(const std::string& id, float lat, float lon);
};