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
// Day 14 helpers — implement these (called by load_geojson below)
// ---------------------------------------------------------------------------

// Convert a (lat, lon) pair to a stable 4-decimal-place string node ID.
// Use std::ostringstream with std::fixed and std::setprecision(4).
// Format: "lat_lon"  e.g. "37.7856_-122.4076"
static std::string coord_to_id(double lat, double lon) {
    // TODO: implement using std::ostringstream, std::fixed, std::setprecision(4)
    (void)lat; (void)lon;
    return "";
}

// Great-circle distance between two (lat, lon) pairs, in kilometres.
// Formula (from exercises background):
//   a = sin²(Δlat/2) + cos(lat1)·cos(lat2)·sin²(Δlon/2)
//   distance = 2·R·atan2(√a, √(1−a))   where R = 6371.0 km
// Convert degrees to radians: degrees * M_PI / 180.0
// C++ functions: std::sin, std::cos, std::atan2, std::sqrt — all from <cmath>
static float haversine_km(double lat1, double lon1, double lat2, double lon2) {
    // TODO: implement
    (void)lat1; (void)lon1; (void)lat2; (void)lon2;
    return 0.0f;
}

// ---------------------------------------------------------------------------
// Day 14 method — implement this
// ---------------------------------------------------------------------------

void RoadGraph::load_geojson(const std::string& path) {
    // Step 1: open file — std::ifstream f(path); check f.is_open()
    //         parse JSON — nlohmann::json data = nlohmann::json::parse(f);
    //         wrap in try/catch for nlohmann::json::parse_error

    // Step 2: iterate features — for (const auto& feature : data.at("features"))

    // Step 3: for each feature:
    //   a. get one_way flag:
    //      bool one_way = feature.at("properties").contains("oneway")
    //                  && feature.at("properties").at("oneway").get<std::string>() == "yes";
    //
    //   b. get coordinates array:
    //      auto coords = feature.at("geometry").at("coordinates");
    //
    //   c. iterate consecutive pairs — for (size_t i = 0; i < coords.size() - 1; i++)
    //      - extract: double lon0 = coords[i][0].get<double>(), lat0 = coords[i][1].get<double>()
    //      - extract: double lon1 = coords[i+1][0].get<double>(), lat1 = coords[i+1][1].get<double>()
    //      - compute: std::string id0 = coord_to_id(lat0, lon0), id1 = coord_to_id(lat1, lon1)
    //      - register nodes if new:
    //          if (nodes_.find(id0) == nodes_.end()) add_node(RoadNode(id0, lat0, lon0))
    //          if (nodes_.find(id1) == nodes_.end()) add_node(RoadNode(id1, lat1, lon1))
    //      - compute: float dist = haversine_km(lat0, lon0, lat1, lon1)
    //      - add_edge(id0, id1, dist)         // always (LineString direction)
    //      - if (!one_way) add_edge(id1, id0, dist)   // only for bidirectional roads
}
