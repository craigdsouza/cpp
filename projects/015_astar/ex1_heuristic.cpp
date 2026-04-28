// Exercise 1 — The Heuristic
// Compile: cmake --build build
//
// Goal: Implement heuristic() in src/road_graph.cpp, then verify admissibility.
// A heuristic is admissible if it never overestimates the true remaining cost.
// For road routing: Haversine distance ≤ actual road distance (roads always detour).

#include "road_graph.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

int main() {
    // TODO: declare RoadGraph g, call g.load_geojson("data/sf_roads_sample.geojson")

    // TODO: find northernmost node (highest lat) and southernmost node (lowest lat)
    //   same pattern as Day 14 integration — iterate g.nodes(), track max/min lat

    // TODO: print both node IDs and their (lat, lon) coordinates

    // TODO: call g.heuristic(north_id, south_id) — straight-line estimate
    //   std::cout << "Heuristic (straight line): " << h << " km\n";

    // TODO: call g.shortest_path(north_id, south_id) then g.path_distance(path)
    //   std::cout << "Actual shortest path:      " << actual << " km\n";

    // TODO: verify admissibility — print whether heuristic <= actual
    //   std::cout << "Admissible: " << (h <= actual ? "yes" : "NO — bug!") << "\n";

    // TODO: print the circuity ratio: actual / heuristic
    //   std::cout << "Circuity ratio: " << std::fixed << std::setprecision(2)
    //             << (actual / h) << "x\n";

    // Expected output (approximate):
    //   North: 37.7856_-122.4076  (lat=37.7856, lon=-122.4076)
    //   South: 37.7823_-122.4045  (lat=37.7823, lon=-122.4045)
    //   Heuristic (straight line): 0.38 km
    //   Actual shortest path:      0.46 km
    //   Admissible: yes
    //   Circuity ratio: 1.21x

    return 0;
}
