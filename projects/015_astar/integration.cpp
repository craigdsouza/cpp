// Exercise 4 — Integration: A* Route Planner + JSON Export
// Compile: cmake --build build  (run from project root, not build/)
//
// Spec: Write a complete A*-powered route planner that exports results for
// Python visualization (Day 16). No scaffold — write from scratch.
//
// 1. Load data/sf_roads_sample.geojson into a RoadGraph.
//    Print: "Graph: N nodes, M edges"
//
// 2. Find the northernmost, southernmost, easternmost, and westernmost nodes.
//    Print each node's ID and (lat, lon).
//
// 3. Run A* for two routes:
//    a) northernmost → southernmost
//    b) easternmost  → westernmost
//    For each: print the path chain, total distance, and time taken (µs).
//
// 4. Export both routes to data/routes.json.
//    Format — a JSON array of route objects, each with a name and coordinate list:
//    [
//      {
//        "name": "north_to_south",
//        "distance_km": 0.46,
//        "coords": [[37.7856, -122.4076], [37.7841, -122.4069], ...]
//      },
//      {
//        "name": "east_to_west",
//        "distance_km": ...,
//        "coords": [...]
//      }
//    ]
//    Use nlohmann/json to build and write the document:
//      nlohmann::json doc = nlohmann::json::array();
//      // build each route object and push to doc
//      std::ofstream out("data/routes.json");
//      out << doc.dump(2);
//    Coordinates in the export are [lat, lon] (Python/Folium convention).
//    Look up each node ID in g.nodes() to get lat_ and lon_.
//
// 5. Print a benchmark summary comparing A* vs Dijkstra's on both routes.
//    Time each with std::chrono and print the speedup.

#include "road_graph.h"
#include <nlohmann/json.hpp>
