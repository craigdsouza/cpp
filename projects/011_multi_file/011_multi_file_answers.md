# Answers — Multi-File Projects and CMake

---

## Question QR1 (From Day 10)

**`std::optional<GPSWaypoint>` is preferred — it lives on the stack, requires no heap allocation, and the "maybe nothing" contract is explicit in the return type.**

`std::optional<T>` (C++17, `#include <optional>`) is a wrapper that either contains a value of type `T` or contains nothing (`std::nullopt`). The caller checks before using it:

```cpp
std::optional<GPSWaypoint> wp = parse_waypoint(line);
if (wp) {                       // or wp.has_value()
    std::cout << wp->name;      // dereference like a pointer
}
```

vs the raw pointer version:
```cpp
GPSWaypoint* wp = parse_waypoint(line);
if (wp) {
    std::cout << wp->name;
    delete wp;   // caller must remember this — easy to forget
}
```

Three reasons `std::optional` wins:
1. **No heap allocation** — the value lives inside the optional, on the stack. No `new`/`delete`.
2. **Ownership is clear** — no question of who deletes the pointer. The optional owns its value.
3. **The "maybe" contract is in the type** — a function returning `std::optional<T>` self-documents that it might not produce a value. A function returning `T*` might mean "nullable result" or "I just prefer pointers" — ambiguous.

Python's equivalent: returning `None` on failure. `std::optional` is C++'s principled version of that.

---

## Question QR2 (From Day 8)

**Templates are not copy-paste — they give type safety per instantiation, a single source of truth, and zero cost for unused types.**

Two benefits copy-paste cannot provide:

1. **Single source of truth:** With copy-paste, fixing a bug in `sort_int` requires finding and fixing the same bug in `sort_float`, `sort_double`, etc. With a template, there is one source — fix it once, all instantiations get the fix automatically.

2. **Type safety per instantiation:** The compiler generates a concrete version for each type and type-checks each one independently. A `sort_float` that accidentally compares as int would be a silent bug. The template generates `sort<float>` and `sort<int>` as distinct verified versions.

Bonus: **Zero cost for unused types** — the compiler only generates code for types that are actually used. Copy-paste generates functions regardless.

---

## Question 1

**`GPSWaypoint::` is the scope resolution operator — it tells the compiler that this constructor definition belongs to the `GPSWaypoint` type, not to the global scope.**

Without the `::` prefix:

```cpp
// gps_waypoint.cpp (WRONG)
GPSWaypoint(float lat, float lon, const std::string& name) {
    this->lat = lat; ...
}
```

The compiler sees a free function named `GPSWaypoint` (like a factory function), not the constructor of the struct. It doesn't match the declaration in the header, so you'd get a linker error: "undefined reference to `GPSWaypoint::GPSWaypoint`" — the struct's constructor was declared but never defined.

The `ClassName::` prefix is required for any member function defined outside its class body. This is how the compiler knows which class the function belongs to. Inside the class body (`struct { ... }`), the scope is implicit — outside it, you must be explicit.

---

## Question 2

**Two bugs: angle brackets instead of quotes, and missing `-I include` flag (or wrong path).**

1. **`<gps_waypoint.h>` → `"gps_waypoint.h"`**

   Angle brackets (`<...>`) tell the compiler to look in standard library and system include paths. Your user header isn't there. Quotes (`"..."`) tell the compiler to look in the current directory first, then in paths added with `-I`. Angle brackets are for `<string>`, `<vector>`, etc. Quotes are for your own headers.

2. **Missing `-I include` in the compile command.**

   Even with quotes, the compiler only finds `"gps_waypoint.h"` if it's in the current directory or a directory added with `-I`. Since the header lives in `include/`, the compile command needs `-I include` to add that directory to the search path:

   ```bash
   g++ -std=c++17 -I include main.cpp src/gps_waypoint.cpp -o program.exe
   ```

---

## Question 3

**Without `#pragma once`, including `gps_waypoint.h` from two headers that are both included in `main.cpp` causes `GPSWaypoint` to be defined twice in the same translation unit — a "redefinition of struct" compile error.**

When the preprocessor processes `main.cpp`, it expands every `#include` by copy-pasting the file's contents in-place. The chain is:

```
main.cpp includes waypoint_loader.h  → which includes gps_waypoint.h → struct GPSWaypoint defined (first time)
main.cpp includes tile_index.h       → which includes gps_waypoint.h → struct GPSWaypoint defined (AGAIN)
```

The compiler sees two definitions of `GPSWaypoint` in the same file and rejects it.

`#pragma once` tells the compiler: "the first time you process this file in a translation unit, include it; every subsequent time, skip it." It's a guard — the struct is defined exactly once no matter how many headers include it.

The older equivalent is the include guard idiom:
```cpp
#ifndef GPS_WAYPOINT_H
#define GPS_WAYPOINT_H
// ... content ...
#endif
```
`#pragma once` is simpler and supported by all modern compilers.

---

## Question 4

**A linker error means the compiler found a *call* to a function but couldn't find its *definition* to link in. It appeared after the call was added because before that, no code needed the missing function.**

Compilation is two phases:

1. **Compile phase:** Each `.cpp` is compiled independently. `main_ex1.cpp` is compiled; `src/waypoint_validator.cpp` is compiled. Neither needs to know about the other at this stage.

2. **Link phase:** The linker combines all compiled object files into one executable. It resolves every function call to a function definition. If `main_ex1.cpp` calls `validate_waypoint()` but `waypoint_validator.cpp` was never compiled and linked in, the linker can't find the definition and fails with "undefined reference to `validate_waypoint`."

Before the function call was added, no code in `main_ex1` referenced anything from `waypoint_validator.cpp`, so the missing file had no consequence. The moment a call was added, the linker looked for the definition — and couldn't find it.

The fix: add `src/waypoint_validator.cpp` to the `add_executable` in `CMakeLists.txt`.

---

## Question 5

**`#include` is a preprocessor text substitution — it literally copy-pastes the contents of the header into the source file before compilation. It is not selective and it does not execute module-level code.**

When the preprocessor encounters `#include "waypoint_loader.h"`, it opens the file and replaces the `#include` line with the entire text of `waypoint_loader.h`. The result is a single large source text that the compiler then processes.

This means:
- **Every declaration in the header is included** — you can't selectively include one function like Python's `from module import fn`.
- **No code runs** — a header is declarations only. There's no "module-level initialization" equivalent from Python.
- **It's purely textual** — the preprocessor has no concept of modules, namespaces, or scope at this stage. It's a find-and-replace.

In Python, `from waypoint_loader import load_waypoints` imports the symbol from an already-compiled module and runs module-level code once (cached after the first import). C++'s `#include` is closer to "paste this text here" than to Python's import — which is why `#pragma once` exists: without it, pasting the same text twice causes redefinition errors.

C++20 introduced **modules** (`import waypoint_loader;`) as a proper selective import mechanism — but they're not yet universally adopted, and DriveWorks-era codebases still use the `#include` model.

---
