# Concepts: Multi-File Projects and CMake

> Reference for the header/source split, `#pragma once`, `ClassName::` syntax, the multi-file compilation pipeline, and CMake basics. Read this when you get a "redefinition" error, a linker error about missing symbols, or you're unsure how to structure a `CMakeLists.txt`.

---

## The header/source split — declaration vs definition

Every C++ type or function has two parts:

- **Declaration** — says that something *exists*. Goes in the `.h` file.
- **Definition** — says what it *does*. Goes in the `.cpp` file.

```cpp
// include/gps_waypoint.h — declaration only
#pragma once
#include <string>

struct GPSWaypoint {
    float lat, lon;
    std::string name;
    GPSWaypoint(float lat, float lon, const std::string& name);  // declared, not defined
};
```

```cpp
// src/gps_waypoint.cpp — definition
#include "gps_waypoint.h"

GPSWaypoint::GPSWaypoint(float lat, float lon, const std::string& name)
    : lat(lat), lon(lon), name(name) {}
```

Any `.cpp` that wants to use `GPSWaypoint` includes the header. It never sees `gps_waypoint.cpp` directly — the linker handles that later.

**vs Python:** A Python module is both declaration and definition in one file. C++ splits them so that many translation units can share the same declaration without duplicating the compiled code.

---

## `#pragma once` — preventing double-inclusion

If `gps_waypoint.h` is included by both `main.cpp` and `waypoint_loader.cpp`, the preprocessor would paste its contents into both. If both `.cpp` files also include each other (or a common header), the struct declaration would appear twice in one translation unit — causing a "redefinition" compile error.

`#pragma once` tells the preprocessor: only process this file once per translation unit, no matter how many times it's `#include`d.

```cpp
#pragma once   // always the first line of every header you write
```

The older alternative is an include guard:

```cpp
#ifndef GPS_WAYPOINT_H
#define GPS_WAYPOINT_H
// ... header content ...
#endif
```

`#pragma once` does the same job in one line and is supported by every modern compiler (GCC, Clang, MSVC). Use `#pragma once`.

---

## `ClassName::` — scope resolution in the .cpp

When a member function is defined outside its class body (which it always is in a `.cpp`), you must prefix its name with `ClassName::` to tell the compiler which class it belongs to.

```cpp
// Correct — ties this definition to GPSWaypoint
GPSWaypoint::GPSWaypoint(float lat, float lon, const std::string& name)
    : lat(lat), lon(lon), name(name) {}

void TileIndex::load(const std::string& path) { ... }
const MapTile* TileIndex::find(const std::string& id) const { ... }
```

Without `ClassName::`, the compiler would treat the function as a free (global) function, not a member — and the linker would fail to connect it to the class.

**Rule of thumb:** Every function body in a `.cpp` that belongs to a class must start with `ClassName::`.

---

## Angle brackets vs quotes

```cpp
#include <string>          // standard library — compiler searches its own known paths
#include <vector>
#include "gps_waypoint.h"  // your own header — compiler searches current dir and -I paths
#include "waypoint_loader.h"
```

Using angle brackets for your own headers will fail unless you've explicitly added your directory to the system include paths. Use quotes for everything you wrote.

---

## The multi-file compilation pipeline

Each `.cpp` file is compiled separately into an **object file** (`.o` or `.obj`). Object files contain compiled machine code with unresolved references (e.g., `gps_waypoint.cpp` references `std::string` but doesn't have it yet). The **linker** then combines all object files into one executable, resolving those references.

```
main_ex1.cpp  ──compile──▶  main_ex1.o  ─┐
gps_waypoint.cpp ─compile──▶ gps_waypoint.o ─┤─ link ──▶ main_ex1.exe
                                             │
                            (std lib .o files)─┘
```

**What this means for errors:-**

- **Compile error** (e.g., "expected ';'", "redefinition of struct") — found by the compiler in a single `.cpp`. Fix the source file.
- **Linker error** (e.g., "undefined reference to `GPSWaypoint::GPSWaypoint`") — the declaration was found in a header but no `.cpp` provided the definition, or you forgot to list a `.cpp` in the compile command. Fix: add the missing source file.

The `-I include` flag tells the compiler where to search for user headers. Without it, `#include "gps_waypoint.h"` fails even if the file exists.

```bash
g++ -std=c++17 -Wall -Wextra -I include main_ex1.cpp src/gps_waypoint.cpp -o main_ex1.exe
#                              ^^^^^^^^  list every .cpp that contains definitions you use
```

---

## CMake — build system basics

In real projects you don't call `g++` manually. CMake reads a `CMakeLists.txt` and generates platform-specific build files (Makefiles on Linux/Mac, Visual Studio projects on Windows, Ninja files in DRIVE CI).

**Minimal `CMakeLists.txt`:**

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

**Key vocabulary:**


| CMake construct                                  | What it does                                                                                                                          |
| ------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------- |
| `add_executable(name files...)`                  | Declares a build target — one executable. Lists every `.cpp` that contributes to it.                                                  |
| `target_include_directories(target PRIVATE dir)` | Adds `dir` to the header search path for that target. Equivalent to `-I dir` in g++.                                                  |
| `PRIVATE`                                        | Means "only this target needs these include dirs, don't propagate to targets that link against me." The right choice for executables. |
| `cmake -B build`                                 | Configures — reads `CMakeLists.txt`, generates build files into `build/`. Safe to re-run.                                             |
| `cmake --build build`                            | Compiles — runs the generated build files. Only recompiles files that changed.                                                        |


**Build workflow:**

```bash
cmake -B build          # run once (or after changing CMakeLists.txt)
cmake --build build     # run every time you change source files
./build/main_ex1.exe    # run the output
```

**Common mistake:** Editing `CMakeLists.txt` but only running `cmake --build build`. Always re-run `cmake -B build` after changing the CMake file itself — otherwise the build system is stale.

---

## Member functions vs free functions — when to use `ClassName::`

This is the key question when splitting code across files: does this function *belong to* a class, or is it a standalone utility?

**Member function** — has access to the object's private fields because it lives inside the class. Requires `ClassName::` when defined outside the class body:

```cpp
// TileIndex owns this — it reads/writes tiles_ directly
void TileIndex::load(const std::string& path) {
    tiles_.emplace(...);   // accesses private member tiles_
}
```

**Free function** (non-member) — standalone function in the global scope. Takes objects as parameters if it needs them, but doesn't live inside any class. No `ClassName::` prefix:

```cpp
// load_waypoints owns nothing — it constructs GPSWaypoints from outside
std::vector<GPSWaypoint> load_waypoints(const std::string& path) {
    // creates GPSWaypoint objects, but doesn't belong to GPSWaypoint
}
```

**The ownership test:** ask "does this function need direct access to an object's private fields?"
- **Yes** → member function, requires `ClassName::` in the `.cpp`
- **No** → free function, no prefix, lives in the global scope

In Day 11: `TileIndex::load` is a member (modifies `tiles_`). `load_waypoints` is a free function (only creates `GPSWaypoint` objects from outside). Both live in their own `.cpp` — the difference is only in the prefix and whether they're declared inside a class.

---

## CMake vs Make — the two-level model

`make` is a build tool — it reads a `Makefile` and runs compiler commands directly. A `Makefile` is written for one platform; the `g++` flags, file paths, and output names are hardcoded.

CMake sits one level above. It is a **build system generator** — it reads `CMakeLists.txt` and *generates* a platform-appropriate build system:

```
CMakeLists.txt
      │
   cmake -B build
      │
      ├── on Linux/Mac  → generates a Makefile   → cmake --build runs make
      ├── on Windows    → generates VS project   → cmake --build runs MSBuild
      └── with -G Ninja → generates Ninja files  → cmake --build runs ninja
```

`cmake --build build` then calls whatever was generated — you never need to know which. This is why the DRIVE stack uses CMake: thousands of modules, built on Linux CI, Windows dev machines, and ARM targets — one `CMakeLists.txt` works everywhere.

**The Makefile you wrote by hand this week** is the low-level tool. A CMake-generated Makefile does the same job but is produced automatically from a higher-level description. In production you write `CMakeLists.txt`, not `Makefile`.

---

## Why this matters for DRIVE

The NVIDIA DRIVE stack contains hundreds of modules — sensor drivers, lidar preprocessing, HD map services, path planning — each split into headers and sources and built with CMake. Every DriveWorks API you'll call is declared in a `.h` and defined in a compiled `.so`. When you add a new perception module, you add an `add_executable` or `add_library` target to a `CMakeLists.txt`. Understanding the header/source split and how CMake connects targets is the foundation for navigating and contributing to any production C++ codebase.

---

## Common mistakes


| Mistake                                                          | Error you'll see                                                                     | Fix                                                                  |
| ---------------------------------------------------------------- | ------------------------------------------------------------------------------------ | -------------------------------------------------------------------- |
| Missing `#pragma once` in a header included from multiple places | "redefinition of struct X"                                                           | Add `#pragma once` as the first line                                 |
| Forgetting `ClassName::` in the `.cpp`                           | Function compiles but linker can't connect it to the class                           | Prefix every member function definition with `ClassName::`           |
| Using angle brackets for your own header                         | "No such file or directory"                                                          | Use quotes: `#include "my_header.h"`                                 |
| Leaving a `.cpp` out of the compile command                      | "undefined reference to `ClassName::method`" (linker error)                          | Add the missing `.cpp` to `g++` args or to `add_executable` in CMake |
| Changing `CMakeLists.txt` but not re-running `cmake -B build`    | Build uses stale configuration, new targets missing                                  | Always `cmake -B build` after touching `CMakeLists.txt`              |
| Putting function definitions in a header (no `inline`)           | "multiple definition" linker error when the header is included from two `.cpp` files | Move definitions to a `.cpp`; keep only declarations in the header   |


