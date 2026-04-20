# Day 8 — Templates

**Goal:** Write generic C++ code using function and class templates, so that one implementation of a sensor buffer, processor, or pipeline stage works cleanly with any data type — exactly as the STL containers you've been using since Day 4 work with any type.

> Before starting: answer the warm-up questions in `008_templates_quiz.md`.

---

## Background

In Python, you write one function and it works with any type — duck typing means the interpreter doesn't care. In C++, every type must be known at compile time. Without templates, you'd need to write `int_max`, `float_max`, and `double_max` as three separate functions even though the logic is identical.

**Templates** are C++'s answer: write the logic once, parameterized on a type. The compiler generates the concrete version for each type you actually use.

```cpp
// Without templates — three identical functions
int   max_int(int a, int b)     { return a > b ? a : b; }
float max_float(float a, float b) { return a > b ? a : b; }

// With a function template — one definition, any type
template<typename T>
T sensor_max(T a, T b) { return a > b ? a : b; }

// Compiler instantiates the right version automatically:
sensor_max(3.5f, 2.1f);   // T = float — compiler writes float version
sensor_max(100, 200);      // T = int   — compiler writes int version
```

The `template<typename T>` line declares T as a **type parameter** — a placeholder that the compiler replaces with the actual type at compile time. This is called **instantiation**. If you write `sensor_max<float>` and `sensor_max<int>` in the same program, the compiler generates two separate functions in the compiled binary.

**Class templates** do the same for classes. You've been using them since Day 4:

```cpp
std::vector<float>       // vector is a class template, T = float
std::vector<LidarFrame>  // same template, T = LidarFrame
```

You can write your own. In the DRIVE stack, a typed `RingBuffer<T>` holds sensor readings between producer and consumer threads — one definition works for lidar intensities (`float`), GPS coordinates (`double`), or full point structs.

**Template specialization** lets you override the generic implementation for one specific type. `std::vector<bool>` is the famous example — it uses bit-packing instead of one byte per bool. The standard library provides the generic `vector<T>` and a separate full specialization just for `bool`.

**Non-type template parameters** let you pass compile-time values (like sizes) as template arguments:

```cpp
template<typename T, int N>
class FixedBuffer {
    T data[N];  // N is known at compile time — no heap allocation
};

FixedBuffer<float, 128> intensity_buf;   // 128 floats on the stack
FixedBuffer<float, 512> point_buf;       // 512 floats on the stack
```

In real-time AV code, `FixedBuffer<T, N>` is preferred over `std::vector<T>` in hot paths because it has zero heap allocation, zero dynamic resizing, and predictable memory layout. NVIDIA's DriveWorks uses this pattern extensively.

> **Hot path:** A section of code that runs so frequently (e.g., processing every lidar point at 20 scans/sec × 64,000 points/scan) that even microsecond slowdowns compound into missed safety deadlines.
>
> **Dynamic resizing:** When a `std::vector` exceeds its capacity, it allocates a new block of memory (typically 2×), copies all elements over, and frees the old block. This works fine in general-purpose code, but the time it takes depends on what the OS is doing right now — it's *non-deterministic*. In a hot path, one unlucky push that triggers a resize can stall the pipeline far longer than the timing budget allows. `FixedBuffer<T, N>` bakes the size in at compile time so the array lives on the stack — there is nothing to allocate at runtime, ever.

---

## Exercise 1 — Function templates

**File:** Create `projects/008_templates/function_templates.cpp` (new file).

Write two function templates:
- `sensor_clamp<T>(T value, T lo, T hi)` — returns `value` clamped to the range `[lo, hi]`
- `print_reading<T>(std::string label, T value)` — prints `"[label]: value"` to stdout

Then in `main`:
- Call `sensor_clamp` with floats (lidar intensity: clamp 1.5f to [0.0f, 1.0f]), ints (ring channel: clamp 150 to [0, 127]), and doubles (GPS latitude: clamp 91.0 to [-90.0, 90.0])
- Call `print_reading` with each result — label them clearly
- Observe: the same template works for all three numeric types

**What to observe:** The `template<typename T>` syntax is the only difference between this and writing three separate functions. The compiler generates a separate concrete function for each type you call it with — but you only wrote the logic once. Notice that T must be consistent within a single call: `sensor_clamp(1.5f, 0, 1.0f)` won't compile (T deduces to `float` from first arg, `int` from second — conflict).

---

## Exercise 2 — Class template: RingBuffer\<T\>

**File:** Create `projects/008_templates/ring_buffer.cpp` (new file).

A ring buffer is the classic data structure for buffering sensor data between a fast producer (the sensor hardware) and a slower consumer (the processing pipeline). Write a `RingBuffer<T>` class template backed by `std::vector<T>`:

- Constructor takes a `int capacity` and reserves space in the internal vector
- `void push(T value)` — add a value; if full, overwrite the oldest (wrap around)
- `T pop()` — remove and return the oldest value; assume buffer is non-empty
- `int size() const` — number of elements currently held
- `bool full() const` — true when at capacity
- `bool empty() const` — true when size is 0

In `main`:
- Create `RingBuffer<float>` of capacity 4 — push 5 intensity readings, pop 2, print size
- Create `RingBuffer<int>` of capacity 3 — push ring channel indices, verify wrap-around by checking which values remain after overflow
- Print enough to confirm the ring behaviour is correct

**What to observe:** The template class compiles for any T that supports the operations you use inside the template body. You wrote the buffer logic once — the compiler generates separate `RingBuffer<float>` and `RingBuffer<int>` classes. The member function bodies live in the header (or same file as the class definition) — unlike regular classes, template method implementations can't be split into a separate `.cpp`.

> **RingBuffer vs FixedBuffer — where does the memory live?**
> `RingBuffer<T>` is backed by `std::vector<T>`, which allocates memory on the **heap** at runtime when the constructor runs. This initial allocation is *non-deterministic* — calling `new` (which `std::vector` does internally) asks the OS for memory, and the OS response time depends on what else is running: other processes, fragmentation, kernel scheduling. In most runs it's fast, but you cannot *guarantee* how long it takes. For a soft real-time system (bounded latency) this is acceptable. For hard real-time (guaranteed latency, e.g. an airbag controller) it is not.
>
> The size is also decided at runtime — `RingBuffer<float> buf(n)` works even if `n` is a variable. This flexibility is the tradeoff: you get a runtime-sized buffer, but at the cost of a heap allocation you can't reason about statically.
>
> Compare this to `FixedBuffer<T, N>` in Exercise 4, where `N` is a compile-time constant and the array lives entirely on the stack — zero runtime allocation, guaranteed.

---

## Exercise 3 — Template specialization

**File:** Create `projects/008_templates/template_specialization.cpp` (new file).

The DRIVE pipeline generates both numeric readings and alert flags. Write a `SensorLogger<T>` class template that logs any reading, then specialize it for `bool` to change the output format:

- Generic `SensorLogger<T>`: method `void log(std::string label, T value)` prints `"[label] reading: value"`
- Specialization `SensorLogger<bool>`: same method, but prints `"[label] ALERT"` when true and `"[label] OK"` when false

In `main`:
- `SensorLogger<float>` logs a lidar intensity: `"intensity reading: 0.73"`
- `SensorLogger<int>` logs a ring channel: `"ring_channel reading: 42"`
- `SensorLogger<bool>` logs a collision alert: `"collision_alert ALERT"` or `"collision_alert OK"`
- Verify the specialization fires for bool and the generic fires for float/int

**What to observe:** The specialization syntax uses `template<>` with an empty parameter list. The compiler always prefers the most specific match — if you instantiate `SensorLogger<bool>`, it picks the specialization over the generic. This is the same mechanism the standard library uses for `std::vector<bool>`.

---

## Exercise 4 — Non-type template parameters: FixedBuffer\<T, N\>

**File:** Create `projects/008_templates/fixed_buffer.cpp` (new file).

In real-time AV code, dynamic allocation (`new`, `std::vector` resizing) is avoided in hot paths — it's non-deterministic. A `FixedBuffer<T, N>` uses a compile-time size so the memory sits on the stack with no heap involvement:

- `template<typename T, int N> class FixedBuffer` — N is the compile-time capacity
- Backed by a plain C array: `T data[N]`
- Methods: `void push(T value)`, `T pop()`, `int size() const`, `bool full() const`, `bool empty() const`
- Constructor sets size to 0; push/pop update a head/tail index

In `main`:
- `FixedBuffer<float, 8>` — 8 lidar intensity slots on the stack; push 6, pop 3, verify size
- `FixedBuffer<double, 4>` — 4 GPS latitude slots; fill to capacity, verify `full()` returns true
- Try: `constexpr int CAP = 16; FixedBuffer<int, CAP> buf;` — demonstrate a named constexpr works as a non-type argument

**What to observe:** `N` is known at compile time — `T data[N]` is a valid stack array declaration because the size is fixed before the program runs. A runtime variable (`int cap = 16;`) would fail as a template argument even if its value is computable. The distinction: template arguments are a compile-time concept; `constexpr` makes a variable eligible.

> **FixedBuffer vs RingBuffer — the hard real-time difference**
> Unlike `RingBuffer<T>`, `FixedBuffer<T, N>` performs zero heap allocation — not just "no resizing", but nothing at runtime at all. The stack frame for the buffer is laid out by the compiler before the program runs. This makes it safe in contexts where heap allocation is outright forbidden: interrupt service routines, hard real-time threads, and safety-critical embedded systems.
>
> The tradeoff: `N` must be a compile-time constant. You cannot size a `FixedBuffer` from a runtime variable. If you need runtime-determined capacity, `RingBuffer<T>` is the right tool. If you need guaranteed latency and bounded memory, `FixedBuffer<T, N>` is.

---

## Exercise 5 — Integration

**File:** Create `projects/008_templates/integration.cpp` (new file).
**No scaffold provided.** Write this program from scratch.

Design and implement a `SensorPipeline<typename T, int Capacity>` class template that models a single stage in the DRIVE sensor pipeline. The class must:

- Own a `FixedBuffer<T, Capacity>` internally (copy the FixedBuffer class definition into this file, or redefine it)
- Have a method `void feed(T reading)` — adds a reading to the buffer; prints a warning and does nothing if the buffer is full
- Have a method `T read()` — removes and returns the oldest reading; behaviour when empty is undefined (assume caller checks first)
- Have a method `void apply(T (*fn)(T))` — applies a function to every element currently in the buffer, replacing each in place
- Have a method `void print_all(std::string label) const` — prints all buffered readings labeled with the stage name

In `main`:
- Create a `SensorPipeline<float, 8>` for lidar intensity readings. Feed 6 readings in the range [0.5f, 1.5f]. Apply a clamp function that brings any value above 1.0f down to 1.0f. Print before and after to verify the transform.
- Create a `SensorPipeline<int, 4>` for lidar ring channel indices. Feed 4 channels. Read them out one by one and print each.
- Demonstrate that the same `SensorPipeline` template works for both types with no code duplication.

Correct output should clearly show: the clamp transform firing on the float pipeline, and the ring channel readings extracted correctly from the int pipeline. No dynamic allocation anywhere.

---

## Exercise Results — 2026-04-20

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — Function Templates | Pass | Both templates correct, all three types demonstrated |
| Exercise 2 — RingBuffer\<T\> | Pass | Head/tail/modulo/size all correct after systematic debugging |
| Exercise 3 — Template Specialization | Pass | `template<>` syntax correct, bool specialization fires correctly |
| Exercise 4 — FixedBuffer\<T,N\> | Pass | Stack array, constexpr argument, no heap allocation |
| Exercise 5 — Integration | Pass | SensorPipeline complete — feed/read/apply/print_all all working |

---

## Checkpoint

You've passed Day 8 when you can:
- Write a function template and explain what "the compiler instantiates a concrete version per type" means
- Write a class template with member functions defined in the class body
- Write a full template specialization (`template<>`) and explain when the compiler chooses it
- Use a non-type template parameter and explain why it must be a compile-time constant
- Explain the trade-off between templates (compile-time polymorphism, potential code bloat) and virtual functions (runtime polymorphism, one copy of code)
