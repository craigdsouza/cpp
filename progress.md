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