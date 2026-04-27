// Exercise 1 — JSON Basics with nlohmann
// Compile: cmake --build build  (after cmake -B build)
//
// Goal: Learn the nlohmann/json API before using it on real GeoJSON files.
// This exercise uses a hardcoded JSON string so you can focus on the API
// without file I/O getting in the way.
//
// Setup: sudo apt install nlohmann-json3-dev

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

int main() {
    // The JSON string to parse — a single GeoJSON Feature for Market Street.
    // We use a raw string literal R"(...)" to avoid escaping all the quotes.
    std::string raw = R"({
        "type": "Feature",
        "properties": { "name": "Market Street", "highway": "secondary" },
        "geometry": {
            "type": "LineString",
            "coordinates": [
                [-122.4076, 37.7856],
                [-122.4059, 37.7855],
                [-122.4042, 37.7854]
            ]
        }
    })";

    // TODO: wrap everything below in try/catch for nlohmann::json::exception
    //       so a bad field access prints an error message instead of crashing

    // TODO: parse the raw string into a json object
    //   nlohmann::json feature = nlohmann::json::parse(raw);
    //   (or use the alias: using json = nlohmann::json; then json feature = ...)

    // TODO: print the road name from properties
    //   access with .at("properties").at("name").get<std::string>()

    // TODO: print the geometry type
    //   access with .at("geometry").at("type").get<std::string>()

    // TODO: iterate coordinates and print each [lon, lat] pair with its index
    //   auto coords = feature.at("geometry").at("coordinates");
    //   for (size_t i = 0; i < coords.size(); i++) {
    //       double lon = coords[i][0].get<double>();
    //       double lat = coords[i][1].get<double>();
    //       std::cout << "  [" << i << "] lon=" << lon << " lat=" << lat << "\n";
    //   }

    // Expected output:
    //   Name: Market Street
    //   Geometry type: LineString
    //   Coordinates:
    //     [0] lon=-122.4076 lat=37.7856
    //     [1] lon=-122.4059 lat=37.7855
    //     [2] lon=-122.4042 lat=37.7854

    return 0;
}
