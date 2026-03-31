# Day 5 — RAII and Destructors

**Goal:** Understand how C++ uses constructors and destructors to manage resources automatically — the pattern that makes C++ safe without a garbage collector, and the foundation of every resource-owning class in the NVIDIA DRIVE stack.

## Active Recall Warm-Up

*From Day 4 — Review*

Before starting: open `projects/004_stl_containers/004_stl_containers_quiz.md` and re-read Question 4. Write a corrected answer in the answer slot. Focus on this distinction:

- A `std::vector<int>` is **ordered** in the sense that it preserves the sequence you insert — `{3, 7, 3, 12}` stays `{3, 7, 3, 12}`.
- A `std::map<int, T>` is **sorted** — it reorders entries by key. It cannot store duplicates.

These are two different meanings of "ordered." Make sure you can explain both before moving on.

---

## Background

In Python, you rarely think about *when* objects are destroyed. The garbage collector handles it whenever it feels like it. You can hint with `del`, but there are no guarantees. For files, sockets, and other resources, Python gives you the `with` statement as a workaround.

C++ takes a fundamentally different approach: **every object has a precisely defined lifetime**, and when that lifetime ends, its **destructor** runs — guaranteed, immediately, no exceptions. This guarantee is the foundation of a pattern called **RAII**: *Resource Acquisition Is Initialization*.

The idea is simple: tie the lifetime of a resource (a file, a connection, a memory buffer) to the lifetime of an object. Acquire the resource in the constructor. Release it in the destructor. Because C++ destructor timing is deterministic, the resource is *always* cleaned up — even if an exception is thrown, even if you forget to call a cleanup function.

In AV systems this matters enormously. A localization module running at 100Hz that leaks a file handle or a sensor connection on every frame will exhaust system resources within seconds. RAII is what prevents this — not programmer discipline, but language mechanics.

The Python analogy: a C++ destructor is like `__exit__` in a context manager, except it runs automatically for *every* object on every exit path, with no `with` statement required.

---

## Exercise 1 — Writing a destructor

**File:** Create `projects/005_raii_and_destructors/tile_lifetime.cpp` (new file).

Copy your `MapTile` struct from Day 4. Add two things:
1. A print statement at the end of the constructor: `"MapTile NNN created"`
2. A destructor that prints: `"MapTile NNN destroyed"`

```cpp
struct MapTile {
    int id;
    // ... other fields ...

    MapTile(int id, double lat, double lon, const std::string& label) {
        // ... assign fields ...
        std::cout << "MapTile " << this->id << " created\n";
    }

    ~MapTile() {
        std::cout << "MapTile " << this->id << " destroyed\n";
    }
};
```

In `main`, create three MapTile objects in a block scope:

```cpp
{
    MapTile a(1, 37.77, -122.41, "San Francisco");
    MapTile b(2, 28.61, 77.20, "Delhi");
    MapTile c(3, 52.52, 13.40, "Berlin");
    std::cout << "--- end of block ---\n";
}
std::cout << "--- after block ---\n";
```

**What to observe:** Destructors run in reverse construction order (last created, first destroyed) at the closing brace. This is not a coincidence — it mirrors how a call stack unwinds. The "end of block" line prints *before* any destructor fires.

---

## Exercise 2 — RAII file handle

**File:** Create `projects/005_raii_and_destructors/tile_logger.cpp` (new file).

Build a `TileLogger` class that wraps a file. The constructor opens the file; the destructor closes and flushes it. The class has a `log(const MapTile&)` method that writes a tile's data to the file.

```cpp
#include <fstream>

class TileLogger {
    std::ofstream file;
public:
    TileLogger(const std::string& filename) {
        file.open(filename);
        std::cout << "TileLogger: file opened\n";
    }

    ~TileLogger() {
        file.close();
        std::cout << "TileLogger: file closed\n";
    }

    void log(const MapTile& tile) {
        file << tile.id << " " << tile.label << "\n";
    }
};
```

In `main`, create a `TileLogger` inside a block scope, log three tiles, then let it go out of scope. After the block, try to open and read the file back using `std::ifstream` and print its contents.

**What to observe:** The file is properly closed the moment `TileLogger` goes out of scope — before your read attempt. You never called `close()` explicitly. This is RAII in action: the resource lifetime is tied to the object lifetime.

---

## Exercise 3 — Destruction order in a vector

**File:** Update `tile_lifetime.cpp` (add to existing file).

Add a second `main` section (or a separate function `void vector_test()` called from main). Create a `std::vector<MapTile>` and push four tiles into it. Then let the vector go out of scope.

```cpp
void vector_test() {
    std::vector<MapTile> tiles;
    tiles.push_back(MapTile(10, 0, 0, "Alpha"));
    tiles.push_back(MapTile(20, 0, 0, "Beta"));
    tiles.push_back(MapTile(30, 0, 0, "Gamma"));
    std::cout << "--- vector going out of scope ---\n";
}
```

**What to observe:** When the vector is destroyed, it destroys each element in reverse order. Notice also the extra "created/destroyed" messages from copy operations during `push_back` — this previews why move semantics (Day 7) matter for performance.

---

## Exercise 4 — RAII sensor handle

**File:** Create `projects/005_raii_and_destructors/sensor_handle.cpp` (new file).

Model a sensor connection as a RAII class. The sensor is "acquired" (constructor prints `"LiDAR sensor acquired"`) and "released" (destructor prints `"LiDAR sensor released"`). Add a `read()` method that prints `"reading sensor data"`.

Write a function `void run_localization()` that:
1. Creates a `SensorHandle` on the stack
2. Calls `read()` three times
3. Returns normally

Then write a second function `void run_localization_with_error()` that does the same but throws a `std::runtime_error` after the second read.

Call both from `main`. Observe that `"LiDAR sensor released"` prints in *both* cases — even when an exception is thrown.

```cpp
#include <stdexcept>

void run_localization_with_error() {
    SensorHandle sensor;
    sensor.read();
    sensor.read();
    throw std::runtime_error("localization failed");
    sensor.read(); // never reached
}
```

**What to observe:** This is the core value proposition of RAII. In a language without it (C, early Java), you'd need `try/finally` blocks everywhere to guarantee cleanup. In C++, the destructor fires regardless of how the scope is exited — return, exception, or fall-off.

---

## Exercise 5 — Spot the resource leak

**File:** Create `projects/005_raii_and_destructors/leak_demo.cpp` (new file).

Write the following deliberately broken class — a `TileBuffer` that allocates a raw C-style array in its constructor but has **no destructor**:

```cpp
class TileBuffer {
    int* data;
    int  size;
public:
    TileBuffer(int size) : size(size) {
        data = new int[size];
        std::cout << "TileBuffer allocated " << size << " ints\n";
    }

    void fill(int value) {
        for (int i = 0; i < size; i++) data[i] = value;
    }
};
```

In `main`, create and destroy a `TileBuffer` inside a loop 5 times. Then write a corrected version with a proper destructor using `delete[]`.

**What to observe:** The broken version leaks memory on every iteration — `new` allocates heap memory that only `delete[]` can free. Without a destructor, the memory is never returned. In a real AV system running 24/7, this would grow until the process crashes. The fix is one destructor:

```cpp
~TileBuffer() {
    delete[] data;
    std::cout << "TileBuffer freed\n";
}
```

This is the problem that smart pointers (Day 6) solve automatically — but understanding the raw problem first is essential.

---

## Repeat Exercise — nullptr (from Day 2/3)

**File:** `projects/002_ref_and_pointers/nullptr_check.cpp`

This exercise has been incomplete since Day 2. Open the file and fix the bug: on the line where `q` is re-pointed to `tile_id`, you are redeclaring `q` with `int*` instead of just reassigning it. The fix is one character — drop the `int*` type prefix on that line.

Compile and run. Confirm both the null branch and the non-null branch print output.

---

## Checkpoint

You've passed Day 5 when you can:

- Write a destructor and explain exactly when it fires
- Explain RAII in one sentence and give a concrete example
- Explain why RAII is more powerful than Python's `with` statement
- Explain destruction order for stack objects and vector elements
- Explain what a resource leak is and write the destructor that prevents it
- Explain why exception safety comes "for free" with RAII
