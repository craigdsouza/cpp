#include "road_graph.h"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <limits>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <unordered_map>

// ---------------------------------------------------------------------------
// Day 12 methods — pre-implemented
// ---------------------------------------------------------------------------

void RoadGraph::add_node(const RoadNode& node) {
    nodes_.emplace(node.id_, node);
    adj_.emplace(node.id_, std::vector<Edge>{});
}

void RoadGraph::add_edge(const std::string& from, const std::string& to, float dist) {
    adj_[from].push_back(Edge(to, dist));
}

std::size_t RoadGraph::node_count() const {
    return nodes_.size();
}

std::size_t RoadGraph::edge_count() const {
    std::size_t count = 0;
    std::for_each(adj_.begin(), adj_.end(), [&count](const auto& pair) {
        count += pair.second.size();
    });
    return count;
}

void RoadGraph::print() const {
    std::for_each(adj_.begin(), adj_.end(), [](const auto& pair) {
        std::cout << pair.first << ":\n";
        std::for_each(pair.second.begin(), pair.second.end(), [](const auto& edge) {
            std::cout << "  -> " << edge.to_id_ << " (" << edge.distance_km_ << " km)\n";
        });
    });
}

void RoadGraph::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << path << "\n";
        return;
    }
    std::string line, header;
    std::getline(file, header);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string from_id, to_id, dist_str;
        std::getline(ss, from_id, ',');
        std::getline(ss, to_id, ',');
        std::getline(ss, dist_str);
        try {
            float dist = std::stof(dist_str);
            if (nodes_.find(from_id) == nodes_.end())
                add_node(RoadNode(from_id, 0.0f, 0.0f));
            if (nodes_.find(to_id) == nodes_.end())
                add_node(RoadNode(to_id, 0.0f, 0.0f));
            add_edge(from_id, to_id, dist);
        } catch (const std::invalid_argument&) {
            std::cerr << "Skipping malformed line: " << line << "\n";
        }
    }
}

std::vector<std::string> RoadGraph::bfs(const std::string& start_id) const {
    if (adj_.find(start_id) == adj_.end()) return {};
    std::vector<std::string> result;
    std::queue<std::string> frontier;
    std::unordered_set<std::string> visited;
    frontier.push(start_id);
    visited.insert(start_id);
    while (!frontier.empty()) {
        std::string current = frontier.front();
        frontier.pop();
        result.push_back(current);
        for (const auto& edge : adj_.at(current)) {
            if (visited.find(edge.to_id_) == visited.end()) {
                visited.insert(edge.to_id_);
                frontier.push(edge.to_id_);
            }
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Day 13 methods — implement these
// ---------------------------------------------------------------------------

std::unordered_map<std::string, float> RoadGraph::dijkstra(const std::string& start_id) const {
  // Returns shortest distances from start_id to all reachable nodes.

  // Step 1: initialise a dist map — for every node in adj_, set dist[id] = infinity
  // use std::numeric_limits<float>::infinity()
  std::unordered_map<std::string, float> dist;

  for(const auto& [node, edges]: adj_) {
    dist[node] = std::numeric_limits<float>::infinity(); 
  }
//   Step 2: set dist[start_id] = 0.0f
        //   return empty map if start_id not in adj_
  dist[start_id] = 0.0f;
  if(adj_.find(start_id) == adj_.end()) return{};
    
//   Step 3: declare a min-heap priority queue of (distance, node_id) pairs
        //   type alias: using Entry = std::pair<float, std::string>;
        //   use std::greater<Entry> as the comparator to get a min-heap
        //   push {0.0f, start_id}
  using Entry = std::pair<float, std::string>;
  std::priority_queue<Entry,std::vector<Entry>,std::greater<Entry>> pq;
  pq.push({0.0f,start_id});

  while(!pq.empty()){   // begins with 0.0f,start_id
    float d = pq.top().first;
    std::string current = pq.top().second;
    pq.pop();
    if (d> dist[current]) continue;   // initial value is simply 0.0f > 0.0f, thus continue.
    for(const auto& edge:adj_.at(current)){
        float new_dist = dist[current] + edge.distance_km_; // distance to new node
        if (new_dist < dist[edge.to_id_]){                  // check if distance to new node is less than current distance to that node, initially inf
            dist[edge.to_id_] = new_dist;
            pq.push({new_dist,edge.to_id_});
        }

    }
  }
  return dist;
}

// shortest_path(start_id, end_id)
//   Returns the ordered list of node IDs on the shortest path, or {} if unreachable.
//
//   Same structure as dijkstra, plus a prev map:
//     std::unordered_map<std::string, std::string> prev;
//   When you relax an edge (new_dist < dist[to]):
//     prev[edge.to_id_] = current;
//
//   After the loop, reconstruct:
//     if dist[end_id] is infinity → return {}
//     walk back: start at end_id, follow prev[node] until you reach start_id
//     push each node onto a path vector, then std::reverse it
//     #include <algorithm> is already included

std::vector<std::string> RoadGraph::shortest_path(const std::string& start_id, const std::string& end_id) const {
    // Returns shortest distances from start_id to all reachable nodes.

  std::unordered_map<std::string, float> dist;
  std::unordered_map<std::string, std::string> prev;

  for(const auto& [node, edges]: adj_) {
    dist[node] = std::numeric_limits<float>::infinity(); 
  }

  dist[start_id] = 0.0f;
  if(adj_.find(start_id) == adj_.end()) return{};
    
  using Entry = std::pair<float, std::string>;
  std::priority_queue<Entry,std::vector<Entry>,std::greater<Entry>> pq;
  pq.push({0.0f,start_id});

  while(!pq.empty()){   // begins with 0.0f,start_id
    float d = pq.top().first;
    std::string current = pq.top().second;
    pq.pop();
    if (d> dist[current]) continue;   // initial value is simply 0.0f > 0.0f, thus continue.
    for(const auto& edge:adj_.at(current)){
        float new_dist = dist[current] + edge.distance_km_; // distance to new node
        if (new_dist < dist[edge.to_id_]){                  // check if distance to new node is less than current distance to that node, initially inf
            dist[edge.to_id_] = new_dist;
            pq.push({new_dist,edge.to_id_});
            prev[edge.to_id_] = current;
        }
    }
}
    if (dist[end_id]==std::numeric_limits<float>::infinity()) return{};

    std::vector<std::string> path;
    for (std::string at = end_id; at != start_id; at = prev[at])
        path.push_back(at);
    path.push_back(start_id);
    std::reverse(path.begin(), path.end());  // #include <algorithm>
    return path;
    
}


// path_distance(path)
//   Sum the edge weights along the path.
//   For consecutive nodes path[i] → path[i+1], find the edge in adj_[path[i]]
//   whose to_id_ matches path[i+1] and add its distance_km_.
//   Return 0.0f if path has fewer than 2 nodes.
float RoadGraph::path_distance(const std::vector<std::string>& path) const {
    float total=0;
    for(size_t i=0;i<path.size()-1;i++){
        for (const auto& edge:adj_.at(path[i])){
            if(edge.to_id_==path[i+1]){
                total+=edge.distance_km_;
                break;
            }
        }
    }
    return total;
}
