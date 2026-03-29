# Progress Log

## 2026-03-25 — C++ Environment Setup on Windows (Cursor IDE)

### Goal

Get C++ code compiling and running from within Cursor IDE on Windows.

### Steps Taken

1. **Verified no C++ compiler was available**
  - No `g++` or `clang++` found in PATH on Windows
  - No MinGW installed
2. **Confirmed WSL 2 was installed but had no distro**
  - WSL 2 version 2.6.3 was present
  - No Linux distribution was installed — WSL was an engine with no OS
3. **Installed Ubuntu on WSL**
  ```
   wsl --install -d Ubuntu
  ```
4. **Installed g++ and build tools inside Ubuntu**
  ```
   sudo apt update && sudo apt install -y g++ build-essential
  ```
  - Installed: g++ 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)
5. **Verified everything works**
  - Compiled and ran a Hello World C++ program via WSL
  - Output confirmed: `C++ is working!`
6. **Successfully ran hello_map.cpp**
  ```
   g++ -std=c++17 -Wall -Wextra -o hello_map hello_map.cpp
   ./hello_map
  ```
  - Used C++17 standard with full warnings enabled

### How to Compile & Run C++ Files

From the Cursor terminal:

```bash
g++ yourfile.cpp -o yourfile
./yourfile
```

### Understanding for loops

See [copy-vs-reference.md](./copy-vs-reference.md).

## 2026-03-26 — Debugging, Makefiles, and File Output

### Concepts Covered

- [Primitive types vs `std` namespace types](./primitives_vs_std.md) — why `int` needs no include but `std::string` does
- [Terminal and file output](./output-terminal-and-file.md) — using `std::cout` and `std::ofstream` together, and shell redirection

### Steps Taken

1. Debugged `hello_map.cpp` — removed invalid `#include <int>` and fixed `std::integer` → `int`
2. Created a `Makefile` so the project compiles with `make` instead of the full `g++` command
3. Added file output to `hello_map.cpp` using `std::ofstream` so output goes to both terminal and `output.txt`

---

## 2026-03-27 — Day 2: References and Pointers

### Quiz Score
4.25 / 5.0 — Strong intuition on all questions; gaps in fully ruling out the non-chosen alternatives.

| Q | Score | Note |
|---|-------|------|
| Q1 | 0.75 | Correct + reason, but missed the no-copy efficiency benefit of `&` |
| Q2 | 1.0  | Excellent — ruled out all three alternatives with clear reasoning |
| Q3 | 0.75 | Correct + reason, but didn't explain why a reference can't represent a missing parent |
| Q4 | 0.75 | Correct + reason, but didn't explain why `TileNode`, `TileNode&`, `const TileNode&` all fail |
| Q5 | 1.0  | Outstanding — dangling reference diagnosed, two solutions given, RVO explained correctly |

### Exercises
| Exercise | Result |
|----------|--------|
| Exercise 1 — References | Pass |
| Exercise 2 — Const references | Pass |
| Exercise 3 — Pointers | Pass |
| Exercise 4 — nullptr | Fail |
| Exercise 5 — swap_tiles | Pass |

### Concepts Confirmed
- Understands `const T&` for read-only parameters: no copy, no mutation
- Understands `T&` for in-place mutation and can correctly rule out `T`, `const T&`, and `T*` when tiles always exist
- Understands `T*` as the only type that can represent both "found" and "not found" (`nullptr`)
- Understands dangling references, the `static` trade-off, and return-by-value with RVO

### Carry-Forward
- **Exercise 4 (Fail):** `nullptr` — redo with `q` initialized to `nullptr` first, then re-pointed to `tile_id`

*(Q1, Q3, Q4 scored 0.75 — partial credit, not carried forward)*

---

## 2026-03-29 — Day 3: Classes and Structs

### Quiz Score
4.75 / 5.0 — Excellent across the board; only gap was a precision detail on constructor semantics.

| Q | Score | Note |
|---|-------|------|
| Q1 | 1.0 | Correct technical difference + strong AV examples |
| Q2 | 1.0 | Identified missing `const`, explained why compiler requires it |
| Q3 | 1.0 | Correct diagnosis (copy not reference) and correct one-word fix |
| Q4 | 0.75 | Constructor code correct, but missed implicit invocation and `const std::string&` on parameter |
| Q5 | 1.0 | Correct signature and clear reasoning |

### Exercises
| Exercise | Result |
|----------|--------|
| Exercise 1 — Define a struct | Pass |
| Exercise 2 — Functions that take structs | Pass |
| Exercise 3 — Constructor | Pass |
| Exercise 4 — A vector of structs | Pass |
| Exercise 5 — A member function | Pass |
| Repeat Exercise — nullptr | Fail |

### Concepts Confirmed
- Understands the only technical difference between `struct` and `class` (default access level) and when to choose each
- Understands `const` on a member function — same contract as `const T&`: promises no modification, required when called on a const reference
- Understands that range-for without `&` makes copies, and mutation requires `MapTile&`
- Understands constructors: same name as struct, no return type, parameters assigned to fields via `this->`

### Carry-Forward
- **Repeat Exercise — nullptr (Fail):** `nullptr_check.cpp` redeclares `int* q` on line 20 — fix to `q = &tile_id` (drop the type)