// Exercise 3 — Path Reconstruction
// Compile: cmake --build build
//
// Goal: Implement shortest_path() and path_distance() in src/road_graph.cpp.
// Open src/road_graph.cpp — the hint-comments are there.
//
// Key new mechanic: prev map + reverse walk
//   std::unordered_map<std::string, std::string> prev;
//   When relaxing edge to_id: prev[to_id] = current;
//   After the loop, walk back: end → prev[end] → prev[prev[end]] → ... → start
//   Then std::reverse the path vector so it reads start → ... → end.

#include "road_graph.h"
#include <algorithm>
#include <iostream>

int main() {
    // TODO: declare g, load "data/road_edges.csv", print node/edge counts
    RoadGraph g;
    g.load("data/road_edges.csv");
    std::cout << "Node count in graph : " << g.node_count() << std::endl;
    std::cout << "Edge count in graph : " << g.edge_count() << std::endl;

    // TODO: find and print the shortest path from "MARKET_4TH" to "HOWARD_5TH"
    // Format: "MARKET_4TH -> MISSION_4TH -> HOWARD_4TH -> HOWARD_5TH"
    // Expected path: MARKET_4TH → MISSION_4TH → HOWARD_4TH → HOWARD_5TH  (0.39 km)
    std::vector<std::string> shortest_path = g.shortest_path("MARKET_4TH", "HOWARD_5TH");
    std::for_each(shortest_path.begin(),shortest_path.end(),[](const auto& node){
        std::cout << node << "->";
    });
    // TODO: print the total distance of the path using path_distance()
    // Format: "Total distance: 0.39 km"
    std::cout << "\nTotal distance: " << g.path_distance(shortest_path) <<std::endl;

    // TODO: find and print the shortest path from "MISSION_6TH" to "HOWARD_4TH"
    // Then print its total distance.
    // Expected: MISSION_6TH → MISSION_5TH → HOWARD_5TH → HOWARD_4TH  (0.42 km)
    //        or MISSION_6TH → MARKET_6TH → MARKET_5TH → MISSION_5TH → HOWARD_5TH → HOWARD_4TH
    //        (Dijkstra's will find the minimum — verify which is actually shorter)
    std::vector<std::string> shortest_path_2 = g.shortest_path("MISSION_6TH", "HOWARD_4TH");
    std::for_each(shortest_path_2.begin(),shortest_path_2.end(),[](const auto& node){
        std::cout << node << "->";
    });
    // TODO: print the total distance of the path using path_distance()
    // Format: "Total distance: 0.39 km"
    std::cout << "\nTotal distance: " << g.path_distance(shortest_path_2) <<std::endl;

    // TODO: test with an unreachable pair. Add an isolated node manually:
    g.add_node(RoadNode("ISOLATED", 0.0f, 0.0f));
    auto nopath = g.shortest_path("MARKET_4TH", "ISOLATED");
    if(nopath.empty()) {
        std::cout << "no path found";
    }

    return 0;
}
