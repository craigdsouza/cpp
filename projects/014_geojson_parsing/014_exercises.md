# Day 14 — GeoJSON Road Parsing

**Goal:** Parse real OpenStreetMap GeoJSON road data into the RoadGraph from Days 12–13, calculate edge distances with the Haversine formula, and run Dijkstra's routing on a real city street network — the same data ingestion pipeline that feeds NVIDIA DRIVE's mapping stack.

> Before starting: answer the warm-up questions in `014_geojson_parsing_quiz.md`.

**Setup — do this before Exercise 1:**
```bash
sudo apt install nlohmann-json3-dev
```

---

## Background

**GeoJSON and OpenStreetMap**

OpenStreetMap (OSM) exports road data as GeoJSON. A GeoJSON `FeatureCollection` contains an array of `Feature` objects; each Feature has `properties` (road name, highway class, one-way flag) and `geometry` (the shape). For roads the geometry is a `LineString`: an ordered sequence of `[longitude, latitude]` coordinate pairs tracing the road centerline. A LineString with 3 points represents 2 consecutive road segments.

```json
{
  "type": "FeatureCollection",
  "features": [
    {
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
    }
  ]
}
```

Note: GeoJSON coordinate order is always `[longitude, latitude]`, not `[latitude, longitude]`. This is a common source of bugs.

**nlohmann/json — the standard C++ JSON library**

nlohmann/json is a header-only library that makes JSON feel natural in C++. After installing via apt, include it with `#include <nlohmann/json.hpp>`.

```cpp
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::ifstream f("data/sf_roads_sample.geojson");
json data = json::parse(f);

// Access a field — throws json::out_of_range if key is absent
std::string type = data.at("type").get<std::string>();

// Check before access
if (data.at("properties").contains("oneway"))
    std::string ow = data.at("properties").at("oneway").get<std::string>();

// Iterate an array
for (const auto& feature : data.at("features")) {
    auto coords = feature.at("geometry").at("coordinates");
    double lon = coords[0][0].get<double>();
    double lat = coords[0][1].get<double>();
}
```

Prefer `.at("key")` over `["key"]` — `["key"]` silently inserts a null entry if the key is missing (ghost-tile problem, same as `std::map::operator[]`). `.at("key")` throws `json::out_of_range` instead, which you can catch.

**Converting coordinates to node IDs**

Each coordinate pair needs a stable string ID for the adjacency list key. Use fixed-precision formatting:

```cpp
#include <sstream>
#include <iomanip>

std::string coord_to_id(double lat, double lon) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4) << lat << "_" << lon;
    return oss.str();
}
```

The same (lat, lon) values always produce the same string, so nodes at shared intersections snap together correctly.

**Haversine distance**

The great-circle distance between two GPS coordinates accounts for Earth's curvature:

```
a = sin²(Δlat/2) + cos(lat1)·cos(lat2)·sin²(Δlon/2)
distance = 2·R·atan2(√a, √(1−a))     R = 6371 km
```

In C++ this uses `<cmath>` functions: `sin`, `cos`, `atan2`, `sqrt`. Angles must be converted to radians (`degrees * M_PI / 180.0`).

**One-way roads**

OSM marks one-way roads with `"oneway": "yes"` in properties. When building the adjacency list:
- No `"oneway"` property, or value is not `"yes"` → call `add_edge` in both directions
- `"oneway": "yes"` → call `add_edge` only in the LineString direction (index 0 → 1 → 2 → ...)

**Why this matters for DRIVE**

The NVIDIA DRIVE map ingestion pipeline begins exactly here: raw GeoJSON (or protobuf/NDS.Live) road features → adjacency list → routing queries. The `load_geojson()` method you implement today is the ingestion layer. Correct one-way handling and node deduplication are safety-critical — a reversed one-way edge would let the route planner navigate against traffic.

---

## Exercise 1 — JSON Basics with nlohmann (~45 min)

**File:** `ex1_json_basics.cpp` (scaffold provided)

No road graph. Explore the nlohmann/json API in isolation using a hardcoded JSON string:

- Parse a JSON string that represents a single GeoJSON feature (market street, 3 coordinate points)
- Print the `name` from `properties`
- Print the geometry `type`
- Iterate `coordinates` and print each `[lon, lat]` pair with its index
- Wrap all field access in a try/catch for `nlohmann::json::exception`

Compile: `cmake --build build`

**What to observe:** `.at("key").get<T>()` is the safe access pattern. The explicit `.get<double>()` call is the only real difference from Python's `j["key"]` — C++ needs to know the target type at compile time.

---

## Exercise 2 — GeoJSON Structure Exploration (~60 min)

**File:** `ex2_geojson_structure.cpp` (scaffold provided)

Load `data/sf_roads_sample.geojson` and audit its structure:

- Print total feature count
- For each feature: print `name` (from properties), geometry type, number of coordinate pairs, and `[one-way]` tag if `"oneway": "yes"`
- Count and print total one-way features
- Print the bounding box: min/max lat and lon across all coordinates in all features

Compile: `cmake --build build`

**Expected output (approximate):**
```
Features: 6
  Market Street — LineString — 3 points
  Mission Street — LineString — 3 points
  Howard Street — LineString — 2 points
  4th Street — LineString — 3 points
  5th Street — LineString — 3 points
  6th Street — LineString — 2 points  [one-way]
One-way features: 1
Bounding box: lat [37.7823, 37.7856]  lon [-122.4076, -122.4035]
```

**What to observe:** Auditing a new data feed before loading it into a graph is standard practice. Feature count, geometry types, and bounding box are the first sanity checks a map engineer runs.

---

## Exercise 3 — Build RoadGraph from GeoJSON (~75 min)

**Files:** `src/road_graph.cpp` (implement `load_geojson()` + two helper functions), `ex3_build_road_graph.cpp`

Open `src/road_graph.cpp`. Three things to implement:

1. **`coord_to_id(lat, lon)`** — static helper that formats a lat/lon pair to a 4-decimal-place string ID. Hint-comments are in the file.

2. **`haversine_km(lat1, lon1, lat2, lon2)`** — static helper for great-circle distance. Hint-comments are in the file. You need `<cmath>` and `M_PI`.

3. **`load_geojson(path)`** — the main method. Hint-comments walk you through the steps:
   - Open and parse the file
   - For each feature, extract coordinates
   - For each consecutive coordinate pair: create node IDs, register new nodes with `add_node`, compute distance, call `add_edge` (twice for bidirectional, once for one-way)

In `ex3_build_road_graph.cpp`:
- Load `data/sf_roads_sample.geojson` and print node and edge counts
- Run `dijkstra()` from the northernmost node (highest latitude — you'll need to find it)
- Print all distances sorted ascending

Compile: `cmake --build build`

**Expected (approximate):**
```
Nodes: 8   Edges: 19
Distances from 37.7856_-122.4076:
  37.7856_-122.4076 : 0.00 km
  37.7855_-122.4059 : 0.14 km
  ...
```

**What to observe:** The `dijkstra()` and `shortest_path()` code from Days 12–13 runs unchanged on real GPS coordinates. The parser is the only new code — the graph algorithms are fully reusable.

---

## Exercise 4 — Integration: Route Planner on Real OSM Data (~90 min)

**File:** `integration.cpp` (no scaffold — write from scratch)

Write a complete GeoJSON route planner. No hints — just a spec:

1. Load `data/sf_roads_sample.geojson` into a `RoadGraph`. Print node count and edge count.
2. Find the northernmost node (highest latitude) and the southernmost node (lowest latitude). Print their IDs and coordinates.
3. Find the shortest path between those two nodes. Print the path chain and total distance.
4. Find and print the node farthest from the northernmost node (highest finite Dijkstra distance).
5. Detect and print all one-way edges: for each directed edge A→B in `adj_`, check whether the reverse B→A exists. If not, it is one-way. Print `A → B (X.XX km)` for each.

Build with `cmake --build build`. Run from the project root directory.

---

## Checkpoint

You've passed Day 14 when you can:
- Parse a GeoJSON FeatureCollection with nlohmann/json, accessing nested objects and arrays
- Explain why `.at("key")` is preferred over `["key"]` for untrusted JSON data
- Convert (lat, lon) pairs to stable node ID strings using fixed-precision formatting
- Implement the Haversine formula in C++ using `<cmath>` functions
- Handle one-way vs bidirectional roads by reading the `"oneway"` property
- Load a real OSM GeoJSON file into a RoadGraph and run Dijkstra routing on the result
