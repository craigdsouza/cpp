# Day 11 — Multi-File Projects and CMake

**Goal:** Split a C++ program across header and source files and build it with CMake — the same build system used throughout the NVIDIA DRIVE stack — so you can read, navigate, and contribute to real multi-module C++ codebases.

> Before starting: answer the warm-up questions in `011_multi_file_quiz.md`.

---

## Background

In Python you import anything from anywhere with a single line. C++ has a different model — one that's been part of the language since its inception and that you'll encounter in every production codebase.

**The header/source split**

In Python, a file is both a module (what others import) and an implementation (where the code lives). In C++, these are separated:

- **Header file (`.h`):** declares that something *exists* — a struct, a class, a function signature. It's the public interface. Any file that wants to use your `GPSWaypoint` struct includes this file.
- **Source file (`.cpp`):** defines what things *do* — constructor implementations, function bodies. Compiled once.

```cpp
// gps_waypoint.h — declaration (the "what exists")
#pragma once
#include <string>

struct GPSWaypoint {
    float lat, lon;
    std::string name;
    GPSWaypoint(float lat, float lon, const std::string& name);
};
```

```cpp
// gps_waypoint.cpp — definition (the "what it does")
#include "gps_waypoint.h"

GPSWaypoint::GPSWaypoint(float lat, float lon, const std::string& name)
    : lat(lat), lon(lon), name(name) {}
```

The `::` here is the scope resolution operator — `GPSWaypoint::GPSWaypoint` means "the constructor that belongs to GPSWaypoint." When a function body is defined outside its class in a `.cpp` file, you must prefix its name with `ClassName::`.

`#pragma once` tells the compiler: "only process this header once per compilation unit, even if it's `#include`d from multiple places." Without it, including a header from two different `.cpp` files would cause "struct redefinition" compile errors.

**User headers vs library headers**

```cpp
#include <string>          // angle brackets — standard library (compiler knows where to find it)
#include "gps_waypoint.h"  // quotes — your own header (compiler looks in current dir and -I paths)
```

**Compiling multiple files**

```bash
g++ -std=c++17 -Wall -Wextra -I include main.cpp src/gps_waypoint.cpp -o program.exe
```

`-I include` adds the `include/` folder to the header search path. Each `.cpp` is compiled separately into an object file, then the linker joins them into one executable.

**CMake**

In real projects you don't call `g++` manually — you use a build system. CMake is the standard across C++, embedded, and AV engineering. A minimal `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.15)
project(day11)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

add_executable(main_ex1
    main_ex1.cpp
    src/gps_waypoint.cpp
)
target_include_directories(main_ex1 PRIVATE include)
```

To build:
```bash
cmake -B build          # configure — generates platform-specific build files
cmake --build build     # compile
./build/main_ex1.exe    # run
```

**Why this matters for DRIVE**

The NVIDIA DRIVE codebase contains thousands of files across hundreds of modules. Every sensor driver, perception algorithm, and map service is split into headers and sources. CMake builds the whole system. Understanding how headers, sources, and build targets relate is the foundation for reading and contributing to any production C++ codebase.

---

## Exercise 1 — Header/Source Split (~45 min)

**Files:** `include/gps_waypoint.h`, `src/gps_waypoint.cpp`, `main_ex1.cpp`

Take the `GPSWaypoint` struct from Day 10 and split it across a header and source file:

- `include/gps_waypoint.h` — declare the struct with `float lat`, `float lon`, `std::string name`, and a constructor declaration. Add `#pragma once` at the very top.
- `src/gps_waypoint.cpp` — implement the constructor using an initializer list. Include the header with `#include "gps_waypoint.h"` (quotes, not angle brackets).
- `main_ex1.cpp` — include only the header, create three `GPSWaypoint` objects (Googleplex, Stanford, SFO), print them with `std::for_each` + lambda in the format `"[name] lat=X lon=Y"`.

Compile with:
```bash
g++ -std=c++17 -Wall -Wextra -I include main_ex1.cpp src/gps_waypoint.cpp -o main_ex1.exe
```

Or use the Makefile: `make main_ex1.exe`

**What to observe:** `main_ex1.cpp` never sees the `.cpp` file — it only sees the header. The header is the contract; the source is the implementation. This is the same pattern every DRIVE module header follows.

---

## Exercise 2 — Separating a Function (~60 min)

**Files:** `include/waypoint_loader.h`, `src/waypoint_loader.cpp`, `main_ex2.cpp`

Move the `load_waypoints` function from Day 10 into its own header/source pair:

- `include/waypoint_loader.h` — declare the function: `std::vector<GPSWaypoint> load_waypoints(const std::string& path)`. Include `"gps_waypoint.h"`, `<vector>`, and `<string>` in this header.
- `src/waypoint_loader.cpp` — implement the function (same logic as Day 10: `std::ifstream`, `std::stringstream`, `std::stof`). Include `"waypoint_loader.h"`, `<fstream>`, `<sstream>`.
- `main_ex2.cpp` — include only `"waypoint_loader.h"`, load from `"../010_file_parsing/data/waypoints.csv"`, print results with `std::for_each`.

Compile with:
```bash
g++ -std=c++17 -Wall -Wextra -I include main_ex2.cpp src/gps_waypoint.cpp src/waypoint_loader.cpp -o main_ex2.exe
```

Or: `make main_ex2.exe`

**What to observe:** `main_ex2.cpp` doesn't know *how* `load_waypoints` works — just what it takes and returns. Changing the implementation (e.g. swapping `std::stringstream` for a different parser) requires no changes to `main_ex2.cpp`. This is the principle of interface/implementation separation.

---

## Exercise 3 — CMake (~60 min)

**File:** `CMakeLists.txt`

Write a `CMakeLists.txt` that builds both `main_ex1` and `main_ex2`:

- `cmake_minimum_required(VERSION 3.15)` and `project(day11)`
- `set(CMAKE_CXX_STANDARD 17)` and `add_compile_options(-Wall -Wextra)`
- Two `add_executable` targets: `main_ex1` (listing `main_ex1.cpp` and `src/gps_waypoint.cpp`) and `main_ex2` (listing its three source files)
- `target_include_directories(target PRIVATE include)` for each target

Build and verify both executables work:
```bash
cmake -B build
cmake --build build
./build/main_ex1.exe
./build/main_ex2.exe
```

**What to observe:** CMake reads `CMakeLists.txt` and generates platform-specific build files in the `build/` directory. The NVIDIA DRIVE stack uses CMake throughout — every DriveWorks module and every third-party dependency is declared this way.

---

## Exercise 4 — Integration: Multi-File Tile System (~90 min)

**Files:** `include/map_tile.h`, `src/map_tile.cpp`, `include/tile_index.h`, `src/tile_index.cpp`, `integration.cpp`

**No scaffold provided.** Write all files from scratch.

Design and implement a multi-file map tile system. Each type gets its own header/source pair.

**`MapTile`** (`include/map_tile.h` + `src/map_tile.cpp`):
- Fields: `std::string id`, `float lat_min, lon_min, lat_max, lon_max`, `int road_count`
- Constructor implemented in the `.cpp` using `ClassName::` syntax and an initializer list

**`TileIndex`** (`include/tile_index.h` + `src/tile_index.cpp`):
- Owns a private `std::map<std::string, MapTile> tiles_`
- Methods (all implemented in `tile_index.cpp`, declared in `tile_index.h`):
  - `void load(const std::string& path)` — parses `map_tiles.csv` (same CSV format as Day 10: `tile_id,lat_min,lon_min,lat_max,lon_max,road_count`, header row). Use `std::ifstream` + `std::stringstream` + `std::stoi`/`std::stof`. Use `try/catch std::invalid_argument` around the conversions.
  - `const MapTile* find(const std::string& id) const` — uses `map::find` + `end()` sentinel, returns `nullptr` if not found
  - `void print_all() const` — uses `std::for_each` + lambda over the map
  - `int count_above(int min_roads) const` — uses `std::count_if` + lambda

**`integration.cpp`** (main):
1. Create a `TileIndex`, call `load("../010_file_parsing/data/map_tiles.csv")`
2. Call `print_all()`
3. Find `"tile_003"` and print its road count; find `"tile_999"` and print "not found"
4. Print the count of tiles with `road_count > 5`

Add `integration` as a third `add_executable` in your `CMakeLists.txt` and build with CMake.

Correct output: all tiles printed, tile_003 found, tile_999 not found, count of high-road tiles correct.

---

## Exercise Results — 2026-04-23

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — Header/Source Split | Pass | `#pragma once`, declaration vs definition, initializer list, `for_each` + lambda all correct |
| Exercise 2 — Separating a Function | Pass | Free function header/source split, transitive include pattern correct |
| Exercise 3 — CMake | Pass | Both targets with `add_executable` + `target_include_directories(PRIVATE include)` |
| Exercise 4 — Integration: Multi-File Tile System | Pass | All five files written from scratch; `emplace` fix, `map::find` + `end()` sentinel, `count_if` lambda |

---

## Checkpoint

You've passed Day 11 when you can:
- Declare a struct or class in a `.h` file with `#pragma once`
- Implement member functions in a `.cpp` file using `ClassName::method()` syntax
- Include a user header with `#include "header.h"` (quotes, not angle brackets)
- Compile a multi-file program with `g++ ... file1.cpp file2.cpp -I include -o program`
- Write a `CMakeLists.txt` with `add_executable` and `target_include_directories`
- Build with `cmake -B build && cmake --build build`
