// Exercise 3 — Build RoadGraph from GeoJSON
// Compile: cmake --build build
//
// Goal: Implement load_geojson() in src/road_graph.cpp, then test it here.
// Open src/road_graph.cpp now — implement coord_to_id(), haversine_km(),
// and load_geojson() following the hint-comments in each stub.

#include "road_graph.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>

int main() {
    // TODO: declare a RoadGraph g, call g.load_geojson("data/sf_roads_sample.geojson")

    // TODO: print node count and edge count
    //   "Nodes: N   Edges: M"

    // TODO: find the northernmost node (highest latitude)
    //   iterate g.nodes() — each value is a RoadNode with .lat_ and .lon_
    //   track the node with the maximum .lat_
    //   std::string north_id = ...;

    // TODO: run g.dijkstra(north_id) to get distances from the northernmost node

    // TODO: copy distances into a vector<pair<float,string>>, sort ascending, print all
    //   same pattern as ex2_dijkstra.cpp from Day 13

    // Expected output (approximate):
    //   Nodes: 8   Edges: 19
    //   Distances from 37.7856_-122.4076:
    //     37.7856_-122.4076 : 0.00 km
    //     37.7855_-122.4059 : 0.14 km
    //     37.7841_-122.4069 : 0.18 km
    //     ...

    return 0;
}
