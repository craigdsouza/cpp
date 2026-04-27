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
    void load(const std::string& path);          // load CSV (Day 12)
    std::vector<std::string> bfs(const std::string& start_id) const;

    // Day 13 methods — pre-implemented
    std::unordered_map<std::string, float> dijkstra(const std::string& start_id) const;
    std::vector<std::string> shortest_path(const std::string& start_id, const std::string& end_id) const;
    float path_distance(const std::vector<std::string>& path) const;

    // Day 14 — implement this in src/road_graph.cpp
    // Parses a GeoJSON FeatureCollection of LineString road features.
    // Creates nodes from coordinate pairs, computes Haversine edge weights,
    // and respects "oneway": "yes" in feature properties.
    void load_geojson(const std::string& path);

    // Expose nodes_ for coordinate lookups in integration exercises
    const std::unordered_map<std::string, RoadNode>& nodes() const { return nodes_; }
    const std::unordered_map<std::string, std::vector<Edge>>& adj() const { return adj_; }
};
