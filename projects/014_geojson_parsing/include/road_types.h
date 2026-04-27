#pragma once
#include <string>

struct Edge {
    std::string to_id_;
    float distance_km_;
    Edge(const std::string& to_id, float distance_km);
};

struct RoadNode {
    std::string id_;
    float lat_, lon_;
    RoadNode(const std::string& id, float lat, float lon);
};
