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
std::optional<T> is a variable that is created on the stack, not heap. Its main advantage is that it indicates to the user that the function that populates the variable may or may not return a value. thus with a simple if (result) {...} , assuming result is of type std::optional<GPSWaypoint> , the caller can be sure that if the function doesn't return a value to result the code is still safe. It also avoids the question of ownership and responsibility of deletion that comes with heap variables. for these reasons it's preferred over the raw pointer in modern c++

---

## Warm-Up (From Day 8 — Review)

**QR2:** A colleague says: "Function templates are just a fancy way of copy-pasting the same function for each type — you might as well write `sort_int`, `sort_float`, etc. manually." What are the concrete benefits of using a template instead of literal copy-paste? Name at least two that a single copy-paste cannot give you.

## Answer QR2
Two concrete benefits
- maintenance is simpler with template functions. if any part of the function changes I don't need to change it in multiple places
- no non-usage penalty - with templates the compiler creates the function only if its actually needed in the code. whereas without templates I might write many functions, one for each type and then later not use one or more functions , which lead to bloating of my source code.

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
The prefix on the constructor tells the compiler where to look for the class, i.e within which imported header file. it is similar to the prefix std:: that accompanies standard template library objects.  If it was omitted, the function you wrote would become a non member function (i.e. a general function that doesn't belong to the GPSWaypoint class.)

---

## Question 2

You have three files: `gps_waypoint.h`, `gps_waypoint.cpp`, and `main.cpp`. Your `main.cpp` contains:

```cpp
#include <gps_waypoint.h>
```

The build fails with: `fatal error: gps_waypoint.h: No such file or directory`. Identify the two things wrong with this `#include` line and explain what each fix does.

## Answer 2
the include line should use quotes, not <>
```cpp
#include "gps_waypoint.h"
```
Besides this, the build fails because the directory which contains the header file has probably not been shared using the -I param.

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
The compiler error that results is a "redefinition error". In pragma once is omitted, both waypoint_loader and tile_index header files include the definition of the gps waypoint class . this means the class is defined twice when both header files are includes in main.cpp. which is faulty behaviour.

---

## Question 4

You have a working `CMakeLists.txt` that builds `main_ex1` from two source files. A new colleague adds a third source file `src/waypoint_validator.cpp` to the project but forgets to add it to `CMakeLists.txt`. They compile and run successfully. Two weeks later, they add a function to `waypoint_validator.cpp` and call it from `main_ex1.cpp` — now the build fails with a linker error. Explain what a linker error is and why it only appeared after the function call was added.

## Answer 4
a linker error is an error that arises when a piece of code calls a function that doesn't know where to find the function definition. when source files are compiled, object files are created, which contain references to pieces of code that reside in other files. the linker then connects these object files together and creates an executable.  in the case described above, when the executable was created, the new function wasn't in there. later when it was updated with the new function, the build step was run again, but the object files that existed had not been updated yet and this resulted in a linker error.

---

## Question 5

In Python, you can do this:

```python
from waypoint_loader import load_waypoints
```

And only the `load_waypoints` function is imported — nothing else from that module runs. In C++, when `main.cpp` does `#include "waypoint_loader.h"`, what exactly happens, and what does the preprocessor do with it? How is this different from Python's `import`?

## Answer 5

When main.cpp does #include "waypoint_loader.h" the contents of entire header file are copied to main.cpp, appended above the other content. In Python you have a way to import only a specific function, whereas in c++ you don't

---

## Grade Log

### 2026-04-23

**Total: 5.5 / 7.0** — Solid across the board; all key concepts present, gaps in precision and mechanism depth.

| Q | Score | Note |
|---|-------|------|
| QR1 | 0.75 | Stack vs heap, ownership, "maybe" contract all present; missing `std::nullopt` syntax and `->` access pattern |
| QR2 | 0.75 | Two valid benefits named; still missing type safety per instantiation (carry-forward from Day 9) |
| Q1 | 0.75 | Free function consequence correct; `::` described as "finding the header" rather than scope resolution operator |
| Q2 | 1.0 | Both bugs identified (quotes, -I flag) with correct reasoning |
| Q3 | 0.75 | Error and cause correct; didn't explain the pragma once mechanism ("skip on subsequent includes") |
| Q4 | 0.75 | Two-phase pipeline explained well; attributed late error to stale object files rather than file absent from CMakeLists.txt |
| Q5 | 0.75 | Text substitution and selectivity correct; missing "no code runs" distinction from Python import |

