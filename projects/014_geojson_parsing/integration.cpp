// Exercise 4 — Integration: Route Planner on Real OSM Data
// Compile: cmake --build build  (run from project root, not build/)
//
// Spec: Write a complete GeoJSON route planner. No scaffold — write from scratch.
//
// 1. Load data/sf_roads_sample.geojson into a RoadGraph.
//    Print: "Graph: N nodes, M edges"
//
// 2. Find the northernmost node (highest latitude) and the southernmost node
//    (lowest latitude). Print each node's ID and its (lat, lon) coordinates.
//
// 3. Find the shortest path between the northernmost and southernmost nodes.
//    Print the path chain: "A -> B -> C -> ..."
//    Print the total distance: "Distance: X.XX km"
//
// 4. Find and print the node farthest from the northernmost node
//    (highest finite Dijkstra distance from it).
//    Print: "Farthest from north: [id] at X.XX km"
//
// 5. Detect and print all one-way edges.
//    For each directed edge A→B in adj(), if the reverse B→A does not exist
//    in adj(), it is a one-way edge. Print: "One-way: A → B (X.XX km)"
//    Hint: to check if B→A exists, look for A in the edge list of B.

#include "road_graph.h"
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <limits>

int main() {
    // Write your implementation here.
    RoadGraph g;
    g.load_geojson("data/sf_roads_sample.geojson");

    std::cout << "Graph: " << g.node_count() << " nodes, " << g.edge_count() << " edges." << std::endl;

    std::string north_id;
    std::string south_id;
    double max_lat = std::numeric_limits<double>::lowest();
    double min_lat = std::numeric_limits<double>::max();

    for(const auto& [id,node]:g.nodes()){
        if(node.lat_>max_lat){
            max_lat = node.lat_;
            north_id = id;
        }
        if(node.lat_<min_lat){
            min_lat = node.lat_;
            south_id = id;
        }
    }
    std::cout << "Northernmost id: " << north_id << "\nLatitude: " << g.nodes().at(north_id).lat_ << " \nLongitude: " << g.nodes().at(north_id).lon_ << std::endl;
    std::cout << "Southernmost id: " << south_id << "\nLatitude: " << g.nodes().at(south_id).lat_ << " \nLongitude: " << g.nodes().at(south_id).lon_ << std::endl;
    
    std::vector<std::string> path = g.shortest_path(north_id,south_id);
    for(size_t i=0;i<path.size()-1;i++){
        std::cout << path[i] << " -> ";
    } std::cout << path.back() <<std::endl;
    float dist = g.path_distance(path);

    std::cout << "Distance: " << std::fixed << std::setprecision(2) << dist << " km" << std::endl;

    auto dijkstra = g.dijkstra(north_id);
    std::vector<std::pair<float,std::string>> dijkstra_vector;

    for(const auto& [id,d]:dijkstra){
        dijkstra_vector.push_back({d,id});
    }

    std::sort(dijkstra_vector.begin(),dijkstra_vector.end(),[](const auto& a, const auto& b){
        return(a.first<b.first);
    });

    std::cout << "Farthest from north: [" << dijkstra_vector.back().second << "] at " << dijkstra_vector.back().first  << " km" << std::endl;


    for (const auto& [from_id,edges]:g.adj()){
        // std::cout << "\nedge-from_id" << from_id << std::endl;
        for(const auto& edge:edges){
            // std::cout << "\n edge-to_id: " << edge.to_id_ << " distance: " << edge.distance_km_ << " km" << std::endl;
            const auto& reverse_edges = g.adj().at(edge.to_id_); 
            bool has_reverse = std::find_if(reverse_edges.begin(),reverse_edges.end(),[&](const Edge& e){
                return e.to_id_== from_id;
            }) != reverse_edges.end();
            if (!has_reverse) {
                std::cout << "One-way: " << from_id << "   → " << edge.to_id_ << " (" << std::fixed <<
                std::setprecision(2) << edge.distance_km_ << "  km)\n";
            }
        }
    }

    return 0;
}
