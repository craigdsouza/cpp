// Exercise 1 — Road Types Source
// Implement the Edge and RoadNode constructors using ClassName:: and initializer lists.
//
// #include "road_types.h"
//
// Edge::Edge(const std::string& to_id, float distance_km)
//     : ...
//
// RoadNode::RoadNode(const std::string& id, float lat, float lon)
//     : ...

#include "road_types.h"

Edge::Edge(const std::string& to_id, float distance_km):to_id_(to_id),distance_km_(distance_km){}

RoadNode::RoadNode(const std::string& id, float lat, float lon):id_(id),lat_(lat),lon_(lon){}