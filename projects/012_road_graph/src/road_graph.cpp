// Exercise 2 + 3 — RoadGraph Source
// Implement all RoadGraph methods using RoadGraph:: prefix.
//
// Includes needed:
#include "road_graph.h"
#include "road_types.h"
#include <cstddef>
#include <iostream>
#include <fstream>      // Exercise 3: load()
#include <sstream>      // Exercise 3: load()
#include <stdexcept>    // Exercise 3: try/catch
#include <algorithm>    // std::for_each
#include <map>

// --- Exercise 2: implement these four methods ---

void RoadGraph::add_node(const RoadNode& node){
    nodes_.emplace(node.id_,node);
    adj_.emplace(node.id_,std::vector<Edge>{});  // could also use .try_emplace()
}

void RoadGraph::add_edge(const std::string& from, const std::string& to, float dist){
    adj_[from].push_back(Edge(to,dist));
}

std::size_t RoadGraph::node_count() const{
    return nodes_.size();
}

std::size_t RoadGraph::edge_count() const {
    std::size_t count = 0;
    std::for_each(adj_.begin(),adj_.end(),[&count](const auto& pair){
        count += pair.second.size();
    });
    return count;
}

std::pair<std::string,size_t> RoadGraph::most_connected() const {
    std::string most_connected_id;
    std::size_t most_connected_count=0;
    for (const auto& pair:adj_){
        if(pair.second.size() > most_connected_count){
            most_connected_count = pair.second.size();
            most_connected_id = pair.first;
        } else {}
    }
    return {most_connected_id,most_connected_count};
}

std::size_t RoadGraph::count_nodes_with_degree(std::size_t degree) const{
    return std::count_if(adj_.begin(),adj_.end(),[degree](const auto& pair){
        return pair.second.size() == degree;
    });
}

void RoadGraph::print() const{
    std::for_each(adj_.begin(),adj_.end(),[](const auto& pair){
        std::cout << "NODE_ID:" << pair.first << std::endl;
        std::for_each(pair.second.begin(),pair.second.end(),[](const auto& edge){
            std::cout << "  -> " << edge.to_id_ << " (" << edge.distance_km_ << " km)" << std::endl;
        });
    });
}

// --- Exercise 3: implement these two methods ---

void RoadGraph::load(const std::string& path){
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "Error - could not open file" << path << std::endl;
        return;
    }
    std::string line, header;

    std::getline(file, header);
    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string from_id, to_id, distance_km_str;
        std::getline(ss,from_id,',');
        std::getline(ss,to_id,',');
        std::getline(ss,distance_km_str);
        try{
            float distance_km = std::stof(distance_km_str);
            if(nodes_.find(from_id)==nodes_.end()){
                add_node(RoadNode(from_id,0.0f,0.0f));
            }
            add_edge(from_id, to_id, distance_km);
        } catch(const std::invalid_argument& error){
            std::cerr << "Invalid distance found, record skipped" << std::endl;
        }
    }
}

std::vector<std::string> RoadGraph::bfs(const std::string& start_id) const{
//   push start → mark visited
//   while queue not empty:
//       current = front; pop
//       add current to result
//       for each neighbor:
//           if not visited: push + mark visited

//  Return the BFS visit order from start_id as a vector of node IDs
    std::vector<std::string> result;
//  Use std::queue<std::string> for the frontier
    std::queue<std::string> frontier;
//  Use std::unordered_set<std::string> for visited nodes
    std::unordered_set<std::string> visited;
//  Standard BFS loop:
//  push start_id to frontier, mark visited
    if(adj_.find(start_id) == adj_.end()) return {};
    frontier.push(start_id);
    visited.insert(start_id);
//  while frontier (queue) not empty:
    while (!frontier.empty()){
//    current = front; pop        
      std::string current = frontier.front(); 
      frontier.pop();
//    add current to result
      result.push_back(current);
      const std::vector<Edge>& current_edges = adj_.at(current);
//    for each neighbor:
      for (auto& edge: current_edges){
//      if not visited: push + mark visited
        if (visited.find(edge.to_id_)== visited.end()){
            visited.insert(edge.to_id_);
            frontier.push(edge.to_id_);
        }
      }
    }
    return result;
}
