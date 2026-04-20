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


| Q   | Score | Note                                                                                         |
| --- | ----- | -------------------------------------------------------------------------------------------- |
| Q1  | 0.75  | Correct + reason, but missed the no-copy efficiency benefit of `&`                           |
| Q2  | 1.0   | Excellent — ruled out all three alternatives with clear reasoning                            |
| Q3  | 0.75  | Correct + reason, but didn't explain why a reference can't represent a missing parent        |
| Q4  | 0.75  | Correct + reason, but didn't explain why `TileNode`, `TileNode&`, `const TileNode&` all fail |
| Q5  | 1.0   | Outstanding — dangling reference diagnosed, two solutions given, RVO explained correctly     |


### Exercises


| Exercise                      | Result |
| ----------------------------- | ------ |
| Exercise 1 — References       | Pass   |
| Exercise 2 — Const references | Pass   |
| Exercise 3 — Pointers         | Pass   |
| Exercise 4 — nullptr          | Fail   |
| Exercise 5 — swap_tiles       | Pass   |


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


| Q   | Score | Note                                                                                           |
| --- | ----- | ---------------------------------------------------------------------------------------------- |
| Q1  | 1.0   | Correct technical difference + strong AV examples                                              |
| Q2  | 1.0   | Identified missing `const`, explained why compiler requires it                                 |
| Q3  | 1.0   | Correct diagnosis (copy not reference) and correct one-word fix                                |
| Q4  | 0.75  | Constructor code correct, but missed implicit invocation and `const std::string&` on parameter |
| Q5  | 1.0   | Correct signature and clear reasoning                                                          |


### Exercises


| Exercise                                 | Result |
| ---------------------------------------- | ------ |
| Exercise 1 — Define a struct             | Pass   |
| Exercise 2 — Functions that take structs | Pass   |
| Exercise 3 — Constructor                 | Pass   |
| Exercise 4 — A vector of structs         | Pass   |
| Exercise 5 — A member function           | Pass   |
| Repeat Exercise — nullptr                | Fail   |


### Concepts Confirmed

- Understands the only technical difference between `struct` and `class` (default access level) and when to choose each
- Understands `const` on a member function — same contract as `const T&`: promises no modification, required when called on a const reference
- Understands that range-for without `&` makes copies, and mutation requires `MapTile&`
- Understands constructors: same name as struct, no return type, parameters assigned to fields via `this->`

### Carry-Forward

- **Repeat Exercise — nullptr (Fail):** `nullptr_check.cpp` redeclares `int* q` on line 20 — fix to `q = &tile_id` (drop the type)

---

## 2026-03-31 — Day 4: STL Containers

### Quiz Score

4.0 / 5.0 — Strong on lookup mechanics and safe iterator patterns; gap in explaining the internals of unordered_map and confusion on what "ordered" means for vector.


| Q   | Score | Note                                                                                                               |
| --- | ----- | ------------------------------------------------------------------------------------------------------------------ |
| Q1  | 0.75  | Correct pick + O(1) reasoning; missed what `map` gives you (sorted iteration, range queries)                       |
| Q2  | 1.0   | Ghost tile identified, danger explained, correct fix with code                                                     |
| Q3  | 0.75  | Correct outputs for both; didn't explain hash buckets as the reason for unordered_map's arbitrary order            |
| Q4  | 0.5   | Correct container chosen, but confused insertion-order (vector) with sorted-order (map) — undermined own reasoning |
| Q5  | 1.0   | Complete: first/second explained, undefined behavior on end() called out, Day 2 nullptr analogy made cleanly       |


### Exercises


| Exercise                                  | Result |
| ----------------------------------------- | ------ |
| Exercise 1 — std::map                     | Pass   |
| Exercise 2 — std::unordered_map           | Pass   |
| Exercise 3 — Iterators and find()         | Pass   |
| Exercise 4 — Erase and update             | Pass   |
| Exercise 5 — Choosing the right container | Pass   |


### Concepts Confirmed

- Understands O(1) vs O(log n) and when unordered_map wins over map for real-time lookup
- Understands the ghost tile danger of `operator[]` and can write the correct find()+end() alternative
- Understands that map yields sorted iteration (BST) and unordered_map yields arbitrary order
- Understands iterators as pointer-like objects: `it->first`/`it->second`, and that end() is a sentinel analogous to nullptr

### Carry-Forward

- **Q4 (0.5):** Confusion between insertion-order (vector preserves sequence) vs sorted-order (map sorts by key) — revisit what "ordered" means for each container type

---

## 2026-04-03 — Day 5: RAII and Destructors

### Quiz Score

4.0 / 6.0 — Strong on core RAII concepts and practical application; Q5 unanswered, minor gaps in C analogy and quantifying leak severity.


| Q   | Score | Note                                                                                                                                 |
| --- | ----- | ------------------------------------------------------------------------------------------------------------------------------------ |
| QR1 | 1.0   | Both disqualifying reasons for map correctly identified                                                                              |
| Q1  | 0.75  | Correct destructor reasoning; said "try/catch" for C but C has no exceptions — manual cleanup on every exit path is the right answer |
| Q2  | 0.75  | Correct order and LIFO analogy; missed dependency preservation as the reason for reverse order                                       |
| Q3  | 0.75  | Correct leak + correct destructor; math error (wrote 60,000 instead of 6,000) and total leaked objects not quantified                |
| Q4  | 0.75  | Correct + composability noted unprompted; missed opt-in vs automatic distinction                                                     |
| Q5  | 0.0   | Unanswered — partially constructed object destructor semantics                                                                       |


### Exercises


| Exercise                                   | Result |
| ------------------------------------------ | ------ |
| Exercise 1 — Writing a destructor          | Pass   |
| Exercise 2 — RAII file handle              | Pass   |
| Exercise 3 — Destruction order in a vector | Pass   |
| Exercise 4 — RAII sensor handle            | Pass   |
| Exercise 5 — Spot the resource leak        | Pass   |
| Repeat Exercise — nullptr                  | Pass   |


### Concepts Confirmed

- Understands RAII: resource tied to object lifetime, constructor acquires, destructor releases
- Understands stack unwinding triggers destructors on exception — and that a catch is needed somewhere for unwinding to occur
- Understands heap vs stack memory: heap requires explicit `delete[]`, RAII wraps this in a destructor
- Understands why RAII beats Python's `with`: automatic at every call site, composable across nested objects

### Carry-Forward

- **Q5 (0.0):** Partially constructed object semantics — if `b`'s constructor throws, `b`'s destructor does NOT run, but `a`'s does. Revisit in Day 6 context.

---

## 2026-04-15 — Day 6: Smart Pointers

### Quiz Score
6.75 / 7.0 — Strongest quiz yet; solid ownership semantics throughout, minor gap in articulating exclusive ownership cleanly for unique_ptr.

| Q   | Score | Note |
|-----|-------|------|
| QR1 | 1.0   | Correct + complete — insertion-order vs sorted-order distinction nailed |
| QR2 | 1.0   | Full carry-forward recovery — partially constructed object semantics correct |
| Q1  | 1.0   | Both reasons solid: exception path + early return paths |
| Q2  | 1.0   | Why, fix, and resulting state all correct |
| Q3  | 1.0   | Step-by-step reference count trace correct |
| Q4  | 0.75  | Ownership framing present but LidarBuffer mixes usage count with ownership reasoning |
| Q5  | 1.0   | Both problems identified, rewrite correct including process(*t) dereference |

### Exercises

| Exercise | Result |
|----------|--------|
| Exercise 1 — unique_ptr for a single tile | Pass |
| Exercise 2 — Ownership transfer with std::move | Pass |
| Exercise 3 — shared_ptr: cache and route | Pass |
| Exercise 4 — Rewrite TileBuffer without a destructor | Pass |
| Exercise 5 — Factory function | Pass |

### Concepts Confirmed
- Understands unique_ptr exclusive ownership and what move semantics mean in practice (nullptr after move)
- Understands shared_ptr reference counting — can trace count step by step across push_back, clear, reset
- Understands exception safety argument for smart pointers over raw new/delete
- Understands factory functions returning unique_ptr: ownership explicit in the type, NRVO/move makes it safe and efficient

### Carry-Forward
- **Q4 (0.75):** unique_ptr ownership reasoning — "exclusive ownership" means one entity is solely responsible for cleanup, not just that only one entity uses it. Articulate this distinction explicitly.

---

## 2026-04-17 — Day 7: Move Semantics

### Quiz Score
4.0 / 5.0 — Strong on mechanics (cast vs transfer, steal-and-nullify); gaps in Rule of Five reasoning and unique_ptr move internals.

| Q | Score | Note |
|---|-------|------|
| Q1 | 1.0 | Correct: cast lvalue→xvalue, move happens in constructor — complete reasoning |
| Q2 | 1.0 | Correct: both nullification steps identified, double-free mechanism explained cleanly |
| Q3 | 0.75 | Correct that it hurts and NRVO is suppressed; says "copy operation" where it's actually a move — small precision gap |
| Q4 | 0.5 | Admits guessing; gets "inconsistency" intuition but misses the key: compiler-generated copy does shallow copy, so destructor + default copy = double-free |
| Q5 | 0.75 | Correctly identifies `= delete`; doesn't explain move constructor internals (steal raw pointer + nullify); "throws an error" is a compile error, not runtime exception |

### Exercises
| Exercise | Result |
|----------|--------|
| Exercise 1 — copy_vs_move.cpp | Pass |
| Exercise 2 — move_containers.cpp | Pass |
| Exercise 3 — lidar_frame.cpp | Pass |
| Exercise 4 — return_move.cpp | Pass |
| Exercise 5 — integration.cpp | Pass |

### Concepts Confirmed
- `std::move` is a cast (lvalue→xvalue), not a transfer — the actual resource move happens in the move constructor or move assignment operator
- Steal-and-nullify: both pointer and count must be nullified in the source after a move, or double-free results
- NRVO: compiler builds return value directly in caller's slot — `std::move` on a named return variable suppresses this and forces a move instead of zero operations
- `= delete` on copy constructor/assignment enforces single-owner invariant at compile time rather than runtime

### Carry-Forward
- **Q4 (0.5):** Rule of Five rationale — when a class has a destructor managing raw memory, compiler-generated copy operations do shallow copies, leading to double-free. Defining destructor without copy = latent bug.
- **Q5 (0.75):** `unique_ptr` move constructor internals — it steals the raw pointer (`this->ptr = other.ptr`) and sets `other.ptr = nullptr`. The pattern is identical to what was implemented in Exercise 3.

---

## 2026-04-20 — Day 8: Templates

### Quiz Score
3.75 / 6.0 — Strong on mechanics (Q1–Q4), gap in conceptual articulation: couldn't describe template instantiation benefits over copy-paste (Q5), and Rule of Five carry-forward still incomplete (QR1).

| Q | Score | Note |
|---|-------|------|
| QR1 | 0.5 | Named shallow copy, missed double-free consequence |
| Q1 | 0.75 | Type conflict identified, two fixes correct; missing "type deduction" term and explicit template argument fix |
| Q2 | 0.75 | Correct fix; didn't explain why class templates can't deduce T from constructor args |
| Q3 | 0.75 | Template specialization named; missing "most specific match" principle |
| Q4 | 1.0 | Complete — compile-time requirement and constexpr distinction clear |
| Q5 | 0.0 | Unanswered — couldn't articulate benefits of templates over literal copy-paste |

### Exercises
| Exercise | Result |
|----------|--------|
| Exercise 1 — Function Templates | Pass |
| Exercise 2 — RingBuffer\<T\> | Pass |
| Exercise 3 — Template Specialization | Pass |
| Exercise 4 — FixedBuffer\<T,N\> | Pass |
| Exercise 5 — Integration (SensorPipeline) | Pass |

### Concepts Confirmed
- Understands function template syntax and type deduction — compiler infers T from arguments, conflicts cause compile errors
- Understands class template syntax — member functions defined in class body, type must be specified explicitly at instantiation
- Understands template specialization — `template<>` syntax, compiler picks most specific match
- Understands non-type template parameters — `int N` must be compile-time constant, `constexpr` is the bridge

### Carry-Forward
- **QR1 (0.5):** Rule of Five — named shallow copy but didn't complete the chain: shallow copy → two objects share same pointer → both destructors call `delete[]` → double-free → undefined behavior. Revisit in Day 9 warm-up.
- **Q5 (0.0):** Template instantiation vs copy-paste — compiler generates one concrete type per instantiation (code bloat tradeoff), but one source means one fix propagates to all types, type safety per instantiation, and zero cost for unused types. Student knew "it's more than copy-paste" but couldn't articulate why.

---

# 2026-04-13 Microsoft Coursera C++ Course

Installed C/C++ Extension pack

