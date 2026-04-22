# Concepts: File I/O and String Parsing

> Reference for `std::ifstream`, `std::stringstream`, and string-to-number conversion. Read this when your file won't open, `getline` isn't splitting correctly, or `stof` is throwing.

---

## Opening a file with std::ifstream

`std::ifstream` opens a file for reading. It lives in `<fstream>`.

```cpp
#include <fstream>
#include <string>

std::ifstream file("data/waypoints.csv");
if (!file.is_open()) {
    std::cerr << "Failed to open file\n";
    return;
}
```

**Always check `is_open()`.** If the path is wrong or the file doesn't exist, `file` will be in a failed state — subsequent reads silently return nothing rather than crashing.

The file closes automatically when `file` goes out of scope (RAII, same as Day 5). You can also call `file.close()` explicitly.

---

## Reading lines with std::getline

`std::getline(stream, str)` reads one line into `str`, consuming (but not storing) the newline. Returns the stream itself — falsy when the stream is exhausted.

```cpp
std::string line;
while (std::getline(file, line)) {
    // process line
}
// loop exits when EOF or read error
```

This is the standard C++ idiom for line-by-line file reading. The equivalent Python pattern is `for line in f:`.

**Skipping a header row:** Call `getline` once before the loop.

```cpp
std::string header;
std::getline(file, header);   // consume header, discard it
while (std::getline(file, line)) {
    // now only data rows
}
```

---

## Splitting a line with std::stringstream

`std::stringstream` (from `<sstream>`) wraps a `std::string` so you can use stream operations on it — including `std::getline` with a delimiter.

```cpp
#include <sstream>

std::string line = "37.42,-122.08,Googleplex";
std::stringstream ss(line);

std::string lat_str, lon_str, name;
std::getline(ss, lat_str, ',');   // reads up to first ','
std::getline(ss, lon_str, ',');   // reads up to second ','
std::getline(ss, name);           // reads the rest (no delimiter = read to end)
```

This is the C++ equivalent of Python's `line.split(',')`, but field-by-field. The delimiter is consumed and not stored in the output string.

**Common mistake:** Calling `std::getline(ss, last_field, ',')` on the last field. Without a delimiter, `std::getline` reads to the end of the string — which is what you want for the last field.

---

## String-to-number conversion

| Function | Header | Converts |
|----------|--------|----------|
| `std::stof(s)` | `<string>` | `std::string` → `float` |
| `std::stod(s)` | `<string>` | `std::string` → `double` |
| `std::stoi(s)` | `<string>` | `std::string` → `int` |

```cpp
float lat = std::stof("37.42");     // → 37.42f
int n     = std::stoi("42");        // → 42
```

**These throw on bad input.** If `s` is not a valid number (empty string, `"N/A"`, `"abc"`), they throw `std::invalid_argument`. In sensor pipelines, bad data is normal — always wrap in `try/catch` (see `010_exceptions.md`).

**vs Python:** `float("37.42")` and `int("42")` — same idea, same throwing behavior on bad input.

---

## Full parsing pattern

Putting it together — the complete CSV parsing loop for a file with a header:

```cpp
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct GPSWaypoint {
    float lat, lon;
    std::string name;
};

std::vector<GPSWaypoint> load_waypoints(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return {};

    std::string header;
    std::getline(file, header);   // skip header row

    std::vector<GPSWaypoint> result;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string lat_str, lon_str, name;
        std::getline(ss, lat_str, ',');
        std::getline(ss, lon_str, ',');
        std::getline(ss, name);
        result.push_back({std::stof(lat_str), std::stof(lon_str), name});
    }
    return result;
}
```

---

## Why this matters for DRIVE

The NVIDIA DRIVE mapping pipeline ingests HD map tiles, GPS traces, lidar scan logs, and calibration files from disk at every stage. The same `ifstream` + `stringstream` pattern appears throughout DriveWorks sensor ingest code — reading binary sensor headers, parsing YAML config fragments, loading tile manifests. Before you can build the graph algorithms (Day 13) or the tile server (Project 3), parsing real data off disk reliably is the foundation.

---

## Common mistakes

| Mistake | Consequence | Fix |
|---------|-------------|-----|
| Not checking `is_open()` | Silent empty parse — no error, no data | Always check after opening |
| Wrong relative path | File not found, `is_open()` returns false | Run the binary from the project directory, or use an absolute path |
| Using delimiter on last field | Reads nothing (waits for another delimiter at EOF) | Omit delimiter on the last `getline` call |
| Not skipping the header | First "data" row is the header string; `stof` throws | `getline(file, header)` before the loop |
| Calling `stof` without `try/catch` on real data | Crashes on any malformed line | See `010_exceptions.md` |
