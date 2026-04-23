// Exercise 2 + 3 — RoadGraph Source
// Implement all RoadGraph methods using RoadGraph:: prefix.
//
// Includes needed:
// #include "road_graph.h"
// #include <iostream>
// #include <fstream>      // Exercise 3: load()
// #include <sstream>      // Exercise 3: load()
// #include <stdexcept>    // Exercise 3: try/catch
// #include <algorithm>    // std::for_each
//
// --- Exercise 2: implement these four methods ---
//
// void RoadGraph::add_node(const RoadNode& node)
//   Insert node into nodes_ map using node.id as the key
//   Also insert an empty vector into adj_ for this node (so node_count includes nodes with no outgoing edges)
//
// void RoadGraph::add_edge(const std::string& from, const std::string& to, float dist)
//   Push an Edge(to, dist) onto adj_[from]
//
// std::size_t RoadGraph::node_count() const
//   Return nodes_.size()
//
// std::size_t RoadGraph::edge_count() const
//   Iterate adj_ with for_each, sum up the size of each edge vector
//   Hint: capture a counter by reference in the lambda
//
// void RoadGraph::print() const
//   For each entry in adj_ (pair.first = node id, pair.second = vector<Edge>):
//     Print "NODE_ID:" then for each edge print "  -> TO_ID (X.XX km)"
//
// --- Exercise 3: implement these two methods ---
//
// void RoadGraph::load(const std::string& path)
//   Open with std::ifstream, skip header row with std::getline
//   For each line: parse from_id, to_id, distance_km using stringstream + getline(ss, field, ',')
//   Wrap stof in try/catch std::invalid_argument — skip malformed lines
//   Register nodes that don't exist yet: if nodes_.find(from_id) == nodes_.end(), add_node with dummy lat/lon (0,0)
//   Call add_edge(from_id, to_id, dist)
//
// std::vector<std::string> RoadGraph::bfs(const std::string& start_id) const
//   Return the BFS visit order from start_id as a vector of node IDs
//   Use std::queue<std::string> for the frontier
//   Use std::unordered_set<std::string> for visited nodes
//   Standard BFS loop:
//     push start_id to queue, mark visited
//     while queue not empty:
//       current = queue.front(); queue.pop()
//       add current to result vector
//       for each Edge in adj_.at(current): if not visited, push to queue and mark visited
//   Return result (empty if start_id not in graph)
