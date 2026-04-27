// Exercise 2 — GeoJSON Structure Exploration
// Compile: cmake --build build
//
// Goal: Load the real GeoJSON file and audit its structure.
// This is the "sanity check" step a map engineer runs before trusting new data.

#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <nlohmann/json.hpp>

int main() {
    // TODO: open data/sf_roads_sample.geojson with std::ifstream
    //       check is_open() — print error and return 1 if it fails

    // TODO: parse the file into a json object
    //   nlohmann::json data = nlohmann::json::parse(file);
    //   wrap in try/catch for nlohmann::json::parse_error

    // TODO: print total feature count
    //   data.at("features").size()

    // TODO: iterate features and for each print:
    //   - name: feature.at("properties").at("name").get<std::string>()
    //   - geometry type: feature.at("geometry").at("type").get<std::string>()
    //   - coordinate count: feature.at("geometry").at("coordinates").size()
    //   - "[one-way]" tag if properties contains "oneway" and value == "yes"

    // TODO: count and print how many features have "oneway": "yes"

    // TODO: compute and print the bounding box:
    //   track min/max lat and lon across ALL coordinates in ALL features
    //   initialise: double min_lat = std::numeric_limits<double>::max(), etc.
    //   for each feature → each coord pair → update min/max

    // Expected output:
    //   Features: 6
    //     Market Street — LineString — 3 points
    //     Mission Street — LineString — 3 points
    //     Howard Street — LineString — 2 points
    //     4th Street — LineString — 3 points
    //     5th Street — LineString — 3 points
    //     6th Street — LineString — 2 points  [one-way]
    //   One-way features: 1
    //   Bounding box: lat [37.7823, 37.7856]  lon [-122.4076, -122.4035]

    return 0;
}
