// Exercise 2 — RoadGraph Header
// Declare the RoadGraph class. No implementation goes here.
//
// Step 1: #pragma once
// Step 2: includes — "road_types.h", <unordered_map>, <vector>, <string>, <queue>, <unordered_set>
//
// class RoadGraph {
// private:
//     std::unordered_map<std::string, std::vector<Edge>> adj_;   // adjacency list
//     std::unordered_map<std::string, RoadNode> nodes_;          // node registry
//
// public:
//     void add_node(const RoadNode& node);
//     void add_edge(const std::string& from, const std::string& to, float dist);
//     std::size_t node_count() const;
//     std::size_t edge_count() const;
//     void print() const;
//     void load(const std::string& path);                         // Exercise 3
//     std::vector<std::string> bfs(const std::string& start_id) const;  // Exercise 3
// };
