# Quiz — Multi-File Projects and CMake

---

## Warm-Up (From Day 10 — Review)

**QR1:** A colleague writes a function `parse_waypoint(const std::string& line)` that should return a `GPSWaypoint` if the line is valid, or signal failure if it's malformed. They're deciding between two signatures:

```cpp
// Option A
GPSWaypoint* parse_waypoint(const std::string& line);

// Option B
std::optional<GPSWaypoint> parse_waypoint(const std::string& line);
```

Explain what `std::optional<T>` is, how the caller would use it, and why it is preferred over a raw pointer in modern C++.

## Answer QR1

---

## Warm-Up (From Day 8 — Review)

**QR2:** A colleague says: "Function templates are just a fancy way of copy-pasting the same function for each type — you might as well write `sort_int`, `sort_float`, etc. manually." What are the concrete benefits of using a template instead of literal copy-paste? Name at least two that a single copy-paste cannot give you.

## Answer QR2

---

## Question 1

You split `GPSWaypoint` into a header and source file:

```cpp
// gps_waypoint.h
struct GPSWaypoint {
    float lat, lon;
    std::string name;
    GPSWaypoint(float lat, float lon, const std::string& name);
};
```

```cpp
// gps_waypoint.cpp
#include "gps_waypoint.h"

GPSWaypoint::GPSWaypoint(float lat, float lon, const std::string& name)
    : lat(lat), lon(lon), name(name) {}
```

What does the `GPSWaypoint::` prefix on the constructor mean, and why is it required? What would happen if you omitted it and just wrote `GPSWaypoint(float lat, float lon, const std::string& name) { ... }` in the `.cpp` file?

## Answer 1

---

## Question 2

You have three files: `gps_waypoint.h`, `gps_waypoint.cpp`, and `main.cpp`. Your `main.cpp` contains:

```cpp
#include <gps_waypoint.h>
```

The build fails with: `fatal error: gps_waypoint.h: No such file or directory`. Identify the two things wrong with this `#include` line and explain what each fix does.

## Answer 2

---

## Question 3

A colleague omits `#pragma once` from `gps_waypoint.h`. Two other headers both include it:

```cpp
// waypoint_loader.h
#include "gps_waypoint.h"
...

// tile_index.h
#include "gps_waypoint.h"
...
```

And `main.cpp` includes both:

```cpp
#include "waypoint_loader.h"
#include "tile_index.h"
```

What compiler error results and why? How does `#pragma once` fix it?

## Answer 3

---

## Question 4

You have a working `CMakeLists.txt` that builds `main_ex1` from two source files. A new colleague adds a third source file `src/waypoint_validator.cpp` to the project but forgets to add it to `CMakeLists.txt`. They compile and run successfully. Two weeks later, they add a function to `waypoint_validator.cpp` and call it from `main_ex1.cpp` — now the build fails with a linker error. Explain what a linker error is and why it only appeared after the function call was added.

## Answer 4

---

## Question 5

In Python, you can do this:

```python
from waypoint_loader import load_waypoints
```

And only the `load_waypoints` function is imported — nothing else from that module runs. In C++, when `main.cpp` does `#include "waypoint_loader.h"`, what exactly happens, and what does the preprocessor do with it? How is this different from Python's `import`?

## Answer 5

---

## Grade Log
