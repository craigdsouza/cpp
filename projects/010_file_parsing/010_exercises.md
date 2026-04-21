# Day 10 — File Parsing + Real Data

**Goal:** Replace hand-written inline data with real CSV files — read GPS waypoints, lidar scan logs, and map tile metadata off disk and parse them into structured C++ types, exactly as the DRIVE mapping pipeline ingests sensor and map data from disk.

> Before starting: answer the warm-up questions in `010_file_parsing_quiz.md`.

---

## Background

In Python you've been parsing CSVs since day one:

```python
import csv
with open("waypoints.csv") as f:
    for row in csv.reader(f):
        lat, lon, name = float(row[0]), float(row[1]), row[2]
```

In C++ the equivalent uses three components working together:

```cpp
#include <fstream>    // std::ifstream — opens and reads a file
#include <sstream>    // std::stringstream — treats a string as a stream
#include <string>     // std::getline, std::stof, std::stoi

std::ifstream file("waypoints.csv");
std::string line;
while (std::getline(file, line)) {         // read one line at a time
    std::stringstream ss(line);            // wrap the line in a stream
    std::string lat_str, lon_str, name;
    std::getline(ss, lat_str, ',');        // read up to first ','
    std::getline(ss, lon_str, ',');        // read up to second ','
    std::getline(ss, name);                // read the rest
    float lat = std::stof(lat_str);        // string → float
    float lon = std::stof(lon_str);
}
```

`std::stringstream` is the key: it wraps a `std::string` so you can use stream operations on it. `std::getline(ss, field, ',')` reads up to the next comma into `field` — the same idea as Python's `str.split(',')`, but one field at a time. `std::stof` converts a string like `"37.42"` to `float`; `std::stoi` does the same for integers.

**Two things to watch out for:**

1. **Headers.** If your file has a header row (`lat,lon,name`), read and discard it before the loop:
   ```cpp
   std::string header;
   std::getline(file, header);   // consume the header line
   ```

2. **Malformed lines.** `std::stof` throws `std::invalid_argument` if the string isn't a valid number. In a real pipeline, malformed sensor data is normal — wrap the parse in `try/catch` and skip bad lines rather than crashing.

**Why this matters for DRIVE:** The NVIDIA mapping pipeline ingests HD map tiles, GPS traces, and lidar scan logs from disk at every stage. Before you build the graph algorithms (Day 13) and the tile server (Project 3), you need to be able to read data off disk reliably. Day 10 is the inflection point — from here every exercise works with real files.

---

## Exercise 1 — Parse GPS Waypoints (~45 min)

**File:** Create `projects/010_file_parsing/parse_waypoints.cpp` (new file).

Data file: `data/waypoints.csv` — five Bay Area GPS waypoints, no header, format `lat,lon,name`.

Tasks:
- Define a `GPSWaypoint` struct with `float lat`, `float lon`, `std::string name`, and a constructor
- Write a free function `std::vector<GPSWaypoint> load_waypoints(const std::string& path)` that opens the file, parses each line using `std::stringstream` and `std::stof`, and returns the parsed waypoints
- In `main`: call `load_waypoints`, print all waypoints in the format `"[name] lat=X lon=Y"`
- Use `std::for_each` with a lambda to do the printing

**What to observe:** `std::stringstream` + `std::getline(ss, field, ',')` is the C++ idiom for splitting a delimited string. Once you have the string fields, `std::stof` does what `float(s)` does in Python. This pattern appears everywhere in the DRIVE mapping stack — GPS traces, tile manifests, calibration files.

---

## Exercise 2 — Parse a LiDAR Scan Log (~60 min)

**File:** Create `projects/010_file_parsing/parse_lidar_log.cpp` (new file).

Data file: `data/lidar_log.csv` — ten scan points plus a corrupt line, format:
`timestamp,x,y,z,intensity` (first row is a header).

Tasks:
- Define a `LidarPoint` struct with `float timestamp, x, y, z, intensity`
- Write `std::vector<LidarPoint> load_lidar_log(const std::string& path)`:
  - Skip the header line
  - For each remaining line, wrap `std::stof` calls in `try/catch std::invalid_argument` — if any field fails to parse, print a warning and skip the line
- In `main`:
  - Load the log and print how many points were loaded vs how many lines the file had
  - Use `std::count_if` with a lambda to count high-intensity points (`intensity > 0.7f`)
  - Use `std::find_if` to locate the first point with `intensity > 0.8f`; print it or "none found"
  - Use `std::sort` to sort points by timestamp ascending; print first and last

**What to observe:** Real sensor logs always contain corrupt entries. The `try/catch` guard makes your parser production-safe — the pipeline skips bad data rather than crashing. This is the same approach DriveWorks uses for sensor ingest.

---

## Exercise 3 — Build a Map Tile Index (~75 min)

**File:** Create `projects/010_file_parsing/tile_index.cpp` (new file).

Data file: `data/map_tiles.csv` — six map tiles with bounding boxes, format:
`tile_id,lat_min,lon_min,lat_max,lon_max,road_count` (first row is a header).

Tasks:
- Define a `MapTile` struct with `std::string id`, `float lat_min, lon_min, lat_max, lon_max`, `int road_count`
- Write `std::map<std::string, MapTile> load_tile_index(const std::string& path)`:
  - Parse each tile and insert it into a `std::map` keyed by `tile_id`
  - Use `std::stoi` to parse `road_count`
- Write a free function `const MapTile* find_tile(const std::map<std::string, MapTile>& index, const std::string& id)` that returns a pointer to the tile or `nullptr` if not found (use `map::find` + `end()` sentinel)
- In `main`:
  - Load the index and print all tiles
  - Find `"tile_003"` and print its road count; try `"tile_999"` and print "not found"
  - Copy the tiles into a `std::vector<MapTile>` and use `std::sort` + lambda to sort descending by `road_count`; print the sorted order

**What to observe:** `std::map` with a `std::string` key is the natural tile index structure — O(log n) lookup by tile ID. The `map::find` + `end()` sentinel pattern you've been using since Day 4 appears again here, now with parsed real data as the key.

---

## Exercise 4 — Integration: Road Segment Parser (~90 min)

**File:** Create `projects/010_file_parsing/road_graph.cpp` (new file).
**No scaffold provided.** Write this program from scratch.

Data file: `data/road_segments.csv` — eight Bay Area road segments, format:
`segment_id,start_lat,start_lon,end_lat,end_lon,road_class,speed_limit` (header row).

Design and implement a `RoadGraph` class that models a parsed road network. The class must:

- Own a `std::vector<RoadSegment>` of parsed segments (define `RoadSegment` struct with all seven fields)
- `void load(const std::string& path)` — parses the CSV, skips the header, skips malformed lines, populates the internal vector
- `void print_all() const` — prints every segment using `std::for_each` + lambda
- `int count_by_class(const std::string& road_class) const` — uses `std::count_if` to count segments of a given class
- `const RoadSegment* fastest() const` — returns a pointer to the segment with the highest speed limit using `std::find_if` or a manual max scan; returns `nullptr` if empty
- `void normalize_speeds()` — normalizes all speed limits to [0,1] relative to the maximum using `std::transform` with a captured max value; no-op if empty
- `float average_speed() const` — returns the average speed limit across all segments

In `main`:
1. Create a `RoadGraph`, call `load("data/road_segments.csv")`
2. Print all segments
3. Print count of `"motorway"` segments and count of `"primary"` segments
4. Find and print the fastest segment
5. Print the average speed before normalization
6. Normalize speeds, print all again — verify all speeds are in [0,1]

Correct output must show: all segments loaded, class counts correct, fastest segment identified, normalization bringing all values into [0,1].

---

## Checkpoint

You've passed Day 10 when you can:
- Open a CSV file with `std::ifstream` + `std::getline` and parse it field-by-field using `std::stringstream`
- Convert string fields to numbers with `std::stof` / `std::stoi`
- Skip a header line before the parse loop
- Handle malformed lines defensively with `try/catch` around `std::stof`
- Combine file parsing with the algorithm + lambda pattern from Day 9
