#include "road_graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <queue>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <nlohmann/json.hpp>

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

std::size_t RoadGraph::node_count() const { return nodes_.size(); }

std::size_t RoadGraph::edge_count() const {
    std::size_t count = 0;
    for (const auto& [id, edges] : adj_) count += edges.size();
    return count;
}

void RoadGraph::print() const {
    for (const auto& [id, edges] : adj_) {
        std::cout << id << ":\n";
        for (const auto& e : edges)
            std::cout << "  -> " << e.to_id_ << " (" << e.distance_km_ << " km)\n";
    }
}

void RoadGraph::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) { std::cerr << "Error: cannot open " << path << "\n"; return; }
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
            if (nodes_.find(from_id) == nodes_.end()) add_node(RoadNode(from_id, 0.0f, 0.0f));
            if (nodes_.find(to_id)   == nodes_.end()) add_node(RoadNode(to_id,   0.0f, 0.0f));
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
        std::string current = frontier.front(); frontier.pop();
        result.push_back(current);
        for (const auto& edge : adj_.at(current)) {
            if (!visited.count(edge.to_id_)) {
                visited.insert(edge.to_id_);
                frontier.push(edge.to_id_);
            }
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Day 13 methods — pre-implemented
// ---------------------------------------------------------------------------

std::unordered_map<std::string, float> RoadGraph::dijkstra(const std::string& start_id) const {
    std::unordered_map<std::string, float> dist;
    for (const auto& [node, edges] : adj_) dist[node] = std::numeric_limits<float>::infinity();
    if (adj_.find(start_id) == adj_.end()) return {};
    dist[start_id] = 0.0f;
    using Entry = std::pair<float, std::string>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    pq.push({0.0f, start_id});
    while (!pq.empty()) {
        auto [d, current] = pq.top(); pq.pop();
        if (d > dist[current]) continue;
        for (const auto& edge : adj_.at(current)) {
            float new_dist = dist[current] + edge.distance_km_;
            if (new_dist < dist[edge.to_id_]) {
                dist[edge.to_id_] = new_dist;
                pq.push({new_dist, edge.to_id_});
            }
        }
    }
    return dist;
}

std::vector<std::string> RoadGraph::shortest_path(const std::string& start_id, const std::string& end_id) const {
    std::unordered_map<std::string, float> dist;
    std::unordered_map<std::string, std::string> prev;
    for (const auto& [node, edges] : adj_) dist[node] = std::numeric_limits<float>::infinity();
    if (adj_.find(start_id) == adj_.end()) return {};
    dist[start_id] = 0.0f;
    using Entry = std::pair<float, std::string>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    pq.push({0.0f, start_id});
    while (!pq.empty()) {
        auto [d, current] = pq.top(); pq.pop();
        if (d > dist[current]) continue;
        for (const auto& edge : adj_.at(current)) {
            float new_dist = dist[current] + edge.distance_km_;
            if (new_dist < dist[edge.to_id_]) {
                dist[edge.to_id_] = new_dist;
                pq.push({new_dist, edge.to_id_});
                prev[edge.to_id_] = current;
            }
        }
    }
    if (dist[end_id] == std::numeric_limits<float>::infinity()) return {};
    std::vector<std::string> path;
    for (std::string at = end_id; at != start_id; at = prev[at]) path.push_back(at);
    path.push_back(start_id);
    std::reverse(path.begin(), path.end());
    return path;
}

float RoadGraph::path_distance(const std::vector<std::string>& path) const {
    float total = 0.0f;
    for (std::size_t i = 0; i < path.size() - 1; i++) {
        for (const auto& edge : adj_.at(path[i])) {
            if (edge.to_id_ == path[i + 1]) { total += edge.distance_km_; break; }
        }
    }
    return total;
}

// ---------------------------------------------------------------------------
// Day 14 helpers + method — pre-implemented
// ---------------------------------------------------------------------------

static std::string coord_to_id(double lat, double lon) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4) << lat << "_" << lon;
    return oss.str();
}

static float haversine_km(double lat1, double lon1, double lat2, double lon2) {
    const double PI = M_PI;
    double to_rad = PI / 180.0;
    double lat1r = lat1 * to_rad;
    double lat2r = lat2 * to_rad;
    double dlat  = (lat2 - lat1) * to_rad;
    double dlon  = (lon2 - lon1) * to_rad;
    double a = std::sin(dlat/2) * std::sin(dlat/2)
             + std::cos(lat1r) * std::cos(lat2r)
             * std::sin(dlon/2) * std::sin(dlon/2);
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return static_cast<float>(6371.0 * c);
}

void RoadGraph::load_geojson(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) { std::cerr << "Error: cannot open " << path << "\n"; return; }
    using json = nlohmann::json;
    json data;
    try {
        data = json::parse(f);
    } catch (const json::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n"; return;
    }
    for (const auto& feature : data.at("features")) {
        bool one_way = feature.at("properties").contains("oneway")
                    && feature.at("properties").at("oneway").get<std::string>() == "yes";
        auto coords = feature.at("geometry").at("coordinates");
        for (std::size_t i = 0; i < coords.size() - 1; i++) {
            double lon0 = coords[i][0].get<double>();
            double lat0 = coords[i][1].get<double>();
            double lon1 = coords[i+1][0].get<double>();
            double lat1 = coords[i+1][1].get<double>();
            std::string id0 = coord_to_id(lat0, lon0);
            std::string id1 = coord_to_id(lat1, lon1);
            if (nodes_.find(id0) == nodes_.end()) add_node(RoadNode(id0, lat0, lon0));
            if (nodes_.find(id1) == nodes_.end()) add_node(RoadNode(id1, lat1, lon1));
            float dist = haversine_km(lat0, lon0, lat1, lon1);
            add_edge(id0, id1, dist);
            if (!one_way) add_edge(id1, id0, dist);
        }
    }
}

// ---------------------------------------------------------------------------
// Day 15 methods — implement these
// ---------------------------------------------------------------------------

// heuristic(from_id, to_id)
//   Returns the Haversine distance between the two nodes.
//   Steps:
//     1. Look up nodes_.at(from_id) and nodes_.at(to_id) to get their RoadNode structs
//     2. Call haversine_km(a.lat_, a.lon_, b.lat_, b.lon_) and return the result
float RoadGraph::heuristic(const std::string& from_id, const std::string& to_id) const {
    // TODO: implement
    (void)from_id; (void)to_id;
    return 0.0f;
}

// a_star(start_id, end_id)
//   A* search: same structure as shortest_path() but uses f = g + h as priority.
//
//   Step 1: initialise g map — all nodes → infinity, start_id → 0.0f
//           return {} if start_id not in adj_
//   Step 2: declare prev map (unordered_map<string,string>) for path reconstruction
//   Step 3: declare min-heap: using Entry = std::pair<float, std::string>;
//           push {heuristic(start_id, end_id), start_id}   ← f = 0 + h
//   Step 4: loop while pq not empty:
//             pop (f, current)
//             if current == end_id: break   ← early termination
//             if f > g[current] + heuristic(current, end_id): continue   ← stale skip
//             for each edge in adj_.at(current):
//               float tentative_g = g[current] + edge.distance_km_
//               if tentative_g < g[edge.to_id_]:
//                 g[edge.to_id_] = tentative_g
//                 prev[edge.to_id_] = current
//                 float new_f = tentative_g + heuristic(edge.to_id_, end_id)
//                 pq.push({new_f, edge.to_id_})
//   Step 5: reconstruct path — same as shortest_path():
//             if g[end_id] == infinity → return {}
//             back-walk prev from end_id to start_id, push_back each node, std::reverse
std::vector<std::string> RoadGraph::a_star(const std::string& start_id, const std::string& end_id) const {
    // TODO: implement
    (void)start_id; (void)end_id;
    return {};
}
