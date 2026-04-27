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
    // Day 12 methods — pre-implemented in src/road_graph.cpp
    void add_node(const RoadNode& node);
    void add_edge(const std::string& from, const std::string& to, float dist);
    std::size_t node_count() const;
    std::size_t edge_count() const;
    void print() const;
    void load(const std::string& path);
    std::vector<std::string> bfs(const std::string& start_id) const;

    // Day 13 methods — implement these in src/road_graph.cpp
    // Returns shortest distance from start_id to every reachable node.
    // Unreachable nodes map to std::numeric_limits<float>::infinity().
    std::unordered_map<std::string, float> dijkstra(const std::string& start_id) const;

    // Returns the shortest path from start_id to end_id as an ordered list of node IDs.
    // Returns an empty vector if no path exists.
    std::vector<std::string> shortest_path(const std::string& start_id, const std::string& end_id) const;

    // Returns the total distance of a path (sum of edge weights along the path).
    // Returns 0.0f for a path of length 0 or 1.
    float path_distance(const std::vector<std::string>& path) const;
};
