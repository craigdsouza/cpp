// Exercise 4 — Integration: Route Planner
// Compile: cmake --build build (run from project root, not build/)
//
// Spec: Write a route planner that answers five queries on the road graph.
// No scaffold. Write everything from scratch.
//
// 1. Load data/road_edges.csv. Print: "Graph: N nodes, M edges"
//
// 2. Run Dijkstra's from "MARKET_4TH". Print all reachable nodes and their
//    distances in ascending order:
//    "Distances from MARKET_4TH:"
//    "  MARKET_4TH    : 0.00 km"
//    "  MISSION_4TH   : 0.12 km"
//    "  ..."
//
// 3. Find and print the shortest path from "MARKET_4TH" to "HOWARD_5TH".
//    Print the path as a chain: "A -> B -> C -> ..."
//    Print the total distance: "Distance: X.XX km"
//
// 4. Find and print the shortest path from "MISSION_6TH" to "HOWARD_4TH".
//    Print path and total distance in the same format.
//
// 5. Find the node farthest from "MARKET_4TH" (highest finite distance).
//    Print: "Farthest node: [id] at X.XX km"
//    Hint: iterate the distance map, track the max finite distance.

#include "road_graph.h"
#include <cstddef>
#include <iostream>
#include <algorithm>

int main() {
    RoadGraph g;
    g.load("data/road_edges.csv");
    std::cout << "Graph: " << g.node_count() << " nodes, " << g.edge_count() << " edges" << std::endl;;

    std::unordered_map<std::string, float> dist = g.dijkstra("MARKET_4TH");
    std::vector<std::pair<std::string, float>> dist_vector;
    for(const auto& [id,d]:dist){
        dist_vector.push_back({id,d});
    };

    std::sort(dist_vector.begin(),dist_vector.end(),[](const auto& a, const auto& b){
        return(a.second<b.second);
    });

    std::cout << "Distances from MARKET_4TH:" << std::endl;
    for(const auto& pair:dist_vector){
        std::cout << pair.first << " : " << pair.second << std::endl;
    }

    std::vector<std::string> path_mkt_hwd = g.shortest_path("MARKET_4TH","HOWARD_5TH");
    for(size_t i=0;i<path_mkt_hwd.size();i++){
        std::cout << path_mkt_hwd[i] ;
        if(i<path_mkt_hwd.size()-1){
            std::cout << " -> ";
        } 
    }
    std::cout << "\n";
    float distance_mkt_hwd = g.path_distance(path_mkt_hwd);
    std::cout << "Total distance: " << distance_mkt_hwd << " km" << std::endl;

    std::vector<std::string> path_mis_hwd = g.shortest_path("MISSION_6TH","HOWARD_4TH");
    for(size_t i=0;i<path_mis_hwd.size();i++){
        std::cout << path_mis_hwd[i];
        if(i<path_mis_hwd.size()-1){
            std::cout << " -> ";
        } 
    }
    std::cout << "\n";
    float distance_mis_hwd = g.path_distance(path_mis_hwd);
    std::cout << "Total distance: " << distance_mis_hwd << " km" << std::endl;

    std::cout << "Farthest node: [" << dist_vector.back().first << "] at " << dist_vector.back().second << " km" <<std::endl;
    return 0;
}
