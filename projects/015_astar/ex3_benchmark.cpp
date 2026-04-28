// Exercise 3 — Benchmarking A* vs Dijkstra's with std::chrono
// Compile: cmake --build build
//
// Goal: Measure and compare the runtime of A* and Dijkstra's across multiple
// query pairs using std::chrono. Observe which queries benefit most from A*.
//
// New mechanic — std::chrono timing:
//
//   #include <chrono>
//
//   auto t0 = std::chrono::high_resolution_clock::now();
//   // ... code to time ...
//   auto t1 = std::chrono::high_resolution_clock::now();
//   long long us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
//   std::cout << us << " µs\n";
//
// high_resolution_clock gives the finest timer available on the platform.
// duration_cast converts the raw duration to a named unit (microseconds here).
// .count() extracts the integer value.

#include "road_graph.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

int main() {
    // TODO: load graph from data/sf_roads_sample.geojson

    // TODO: collect all node IDs into a std::vector<std::string> node_ids
    //   iterate g.nodes(), push_back each id

    // TODO: define 5 query pairs as std::vector<std::pair<std::string,std::string>>
    //   pick pairs by index from node_ids — vary distances (adjacent, mid, far)

    // TODO: print table header:
    //   std::cout << std::left << std::setw(36) << "Query"
    //             << std::setw(14) << "Dijkstra"
    //             << std::setw(14) << "A*"
    //             << "Speedup\n";
    //   std::cout << std::string(70, '-') << "\n";

    // TODO: for each query pair (start, end):
    //   a) time Dijkstra's shortest_path():
    //        auto t0 = std::chrono::high_resolution_clock::now();
    //        auto path_d = g.shortest_path(start, end);
    //        auto t1 = std::chrono::high_resolution_clock::now();
    //        long long dijk_us = std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
    //
    //   b) time A* a_star():
    //        auto t2 = std::chrono::high_resolution_clock::now();
    //        auto path_a = g.a_star(start, end);
    //        auto t3 = std::chrono::high_resolution_clock::now();
    //        long long astar_us = std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count();
    //
    //   c) compute speedup = (float)dijk_us / (float)astar_us
    //      guard against division by zero: if astar_us == 0, speedup = 1.0f
    //
    //   d) print the row: query label, dijk_us µs, astar_us µs, speedupX

    // Expected output (approximate — times will vary by machine):
    //   Query                                Dijkstra      A*            Speedup
    //   ----------------------------------------------------------------------
    //   37.7856_... → 37.7823_...           38 µs         14 µs         2.7x
    //   ...
    //
    // What to observe: on this small graph times are noisy at µs resolution.
    // The speedup pattern matters more than absolute numbers — note in a comment
    // which queries show the most A* benefit and why.

    return 0;
}
