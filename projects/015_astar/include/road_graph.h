#pragma once
#include "road_types.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <queue>
#include <limits>

class RoadGraph {
private:
    std::unordered_map<std::string, std::vector<Edge>> adj_;
    std::unordered_map<std::string, RoadNode> nodes_;

public:
    // Day 12 methods — pre-implemented
    void add_node(const RoadNode& node);
    void add_edge(const std::string& from, const std::string& to, float dist);
    std::size_t node_count() const;
    std::size_t edge_count() const;
    void print() const;
    void load(const std::string& path);
    std::vector<std::string> bfs(const std::string& start_id) const;

    // Day 13 methods — pre-implemented
    std::unordered_map<std::string, float> dijkstra(const std::string& start_id) const;
    std::vector<std::string> shortest_path(const std::string& start_id, const std::string& end_id) const;
    float path_distance(const std::vector<std::string>& path) const;

    // Day 14 method — pre-implemented
    void load_geojson(const std::string& path);

    // Day 15 — implement these in src/road_graph.cpp
    // Returns the Haversine distance between two nodes as a routing heuristic.
    // Admissible: Haversine ≤ actual road distance (straight line is never longer than a road).
    float heuristic(const std::string& from_id, const std::string& to_id) const;

    // A* search from start_id to end_id.
    // Uses f = g + h priority (g = cost so far, h = heuristic to goal).
    // Returns the shortest path as an ordered list of node IDs.
    // Returns an empty vector if no path exists.
    std::vector<std::string> a_star(const std::string& start_id, const std::string& end_id) const;

    // Expose internals for coordinate lookups and benchmarking
    const std::unordered_map<std::string, RoadNode>& nodes() const { return nodes_; }
    const std::unordered_map<std::string, std::vector<Edge>>& adj() const { return adj_; }
};
