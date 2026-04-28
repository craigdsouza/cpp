// Exercise 3 — Build RoadGraph from GeoJSON
// Compile: cmake --build build
//
// Goal: Implement load_geojson() in src/road_graph.cpp, then test it here.
// Open src/road_graph.cpp now — implement coord_to_id(), haversine_km(),
// and load_geojson() following the hint-comments in each stub.

#include "road_graph.h"
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <vector>
#include <string>

int main() {
    // TODO: declare a RoadGraph g, call g.load_geojson("data/sf_roads_sample.geojson")
    RoadGraph g;
    g.load_geojson("data/sf_roads_sample.geojson");

    // TODO: print node count and edge count
    //   "Nodes: N   Edges: M"
    std::cout << "Nodes: " << g.node_count() << " Edges: " << g.edge_count() << std::endl;

    // TODO: find the northernmost node (highest latitude)
    //   iterate g.nodes() — each value is a RoadNode with .lat_ and .lon_
    //   track the node with the maximum .lat_
    //   std::string north_id = ...;
    std::string north_id;
    double max_lat = std::numeric_limits<double>::lowest();

    for(const auto& [id,node]:g.nodes()){
        if(node.lat_>max_lat){
            max_lat=node.lat_;
            north_id = id;
        }
    }

    // TODO: run g.dijkstra(north_id) to get distances from the northernmost node
    auto dist = g.dijkstra(north_id);
    std::vector<std::pair<float,std::string>> dist_vector;
    // TODO: copy distances into a vector<pair<float,string>>
    //   same pattern as ex2_dijkstra.cpp from Day 13
    for(const auto& [id,d]:dist){
        dist_vector.push_back({d,id});
    }
    // sort ascending, print all
    std::sort(dist_vector.begin(),dist_vector.end(),[](const auto& a, const auto& b){
        return (a.first<b.first);
    });

    // Expected output (approximate):
    //   Nodes: 8   Edges: 19
    //   Distances from 37.7856_-122.4076:
    //     37.7856_-122.4076 : 0.00 km
    //     37.7855_-122.4059 : 0.14 km
    //     37.7841_-122.4069 : 0.18 km
    //     ...
    for (const auto& [d,id]:dist_vector){
        std::cout << std::left << std::setw(14) << id << ": " << std::fixed << std::setprecision(2) << d << " km" << std::endl;
    }

    return 0;
}

