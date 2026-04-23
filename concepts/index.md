# Concepts Index

> Quick reference for future sessions. Each entry states what's covered and when to reach for it.

---

## Day 1 — Program Structure and Compilation

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `001_compilation.md` | The three compilation steps: preprocessing, compiling, linking | You need to recall the pipeline or explain a linker error |
| `001_program-structure.md` | Preprocessor directives, `::` scope resolution, `using namespace` | You're confused about `#include`, namespaces, or comment conventions |
| `001_stream.md` | (stub — not yet written) | — |

---

## Day 2 — Headers and Namespaces

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `002_headers-namespaces.md` | Header files as modules, namespace collision risk, prefer `using std::cout` over `using namespace std` | You're deciding how to import names or hitting ambiguous symbol errors |

---

## Day 3 — Constructors and Special Members

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `003_constructors-and-special-members.md` | All six special member functions: default/parameterized constructor, destructor, copy constructor, copy assignment, move constructor, move assignment. Deleted constructors. Quick-reference table. | You're unsure which constructor fires, why a copy compiles when it shouldn't, or how to implement a safe move |

---

## Day 4 — STL Containers

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `004_vector-methods.md` | `reserve` vs `resize`, `push_back`, `pop_back`, `[]` vs `at()`, `erase` (takes iterator not index), `clear`, modulo ring-buffer wrapping | A vector method crashes, you're mixing up `reserve`/`resize`, or building a ring buffer |

---

## Day 5 — Memory Model and RAII

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `005_memory-model.md` | Stack vs heap (speed, size, lifetime), stack frames and unwinding, how RAII bridges the two, why smart pointers exist | Smart pointer or RAII motivation is unclear; you're confused about where data lives |

---

## Day 7 — Move Semantics and AV Context

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `007_value-categories.md` | lvalue / prvalue / xvalue, what `std::move` actually is (a `static_cast`), reference binding rules (`T&` vs `T&&`), forwarding references in templates, NRVO anti-pattern | `std::move` behaves unexpectedly, a move constructor isn't firing, or you're writing template code with `T&&` |
| `007_lidar-data-and-av-pipeline.md` | What real lidar data looks like (point structure, frame rates, data volume), the full DriveWorks pipeline (capture → preprocessing → detection → fusion → perception → planning), why move semantics matters for 36MB frames, producer/consumer threading | You want real-world context for why the patterns in Day 7 exist |

---

## Day 8 — Templates

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `008_class-invariants.md` | What a class invariant is, how to reason about `RingBuffer` correctness using invariants, `assert_invariants()` pattern, invariants vs preconditions | Designing a class with state and you need to verify correctness, or debugging a ring buffer |
| `008_constexpr.md` | `constexpr` vs `const`, when compile-time constants are required (template arguments, array sizes), DriveWorks buffer-size pattern | Template argument fails to compile, or you need a named compile-time constant |

---

## Day 9 — Lambdas and STL Algorithms

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `009_lambdas.md` | Lambda syntax, all capture modes (`[]`, `[=]`, `[&]`, `[x]`, `[&x]`), dangling reference risk, `mutable`, `std::function`, generic lambdas | A capture doesn't compile or gives wrong values, or you're unsure which capture mode to use |
| `009_std-algorithms.md` | `for_each`, `sort` (strict weak ordering), `find_if`, `transform` (output sizing), `count_if`, `remove_if` + erase idiom. Choosing the right algorithm table. Common mistakes. | You're unsure which algorithm to reach for, a sort comparator is wrong, or `transform` writes garbage |

---

## Day 10 — File Parsing

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `010_file-io-and-parsing.md` | `std::ifstream`, checking `is_open()`, line-by-line reading with `std::getline`, skipping headers, `std::stringstream` for splitting CSV fields by delimiter, `std::stof` / `std::stoi` / `std::stod`. Full parsing pattern. | File won't open, `getline` isn't splitting correctly, `stof` throws, or you're unsure how to read a CSV |
| `010_exceptions.md` | `try/catch` syntax, `std::invalid_argument` and `std::out_of_range` (thrown by `stof`/`stoi`), guarding a parse loop to skip bad lines, catching by `const&`, when NOT to use exceptions | You need to make a parser production-safe, a `stof` call is crashing on corrupt data, or you're unsure whether exceptions are the right tool |

---

## Day 11 — Multi-File Projects and CMake

| File | What it covers | Reach for it when |
|------|---------------|-------------------|
| `011_multi-file-and-cmake.md` | Declaration vs definition, `#pragma once` vs include guards, `ClassName::` scope resolution in `.cpp` files, angle brackets vs quotes, the compile → object → link pipeline, CMake vocabulary (`add_executable`, `target_include_directories`, `PRIVATE`), build workflow | You get a "redefinition" or "undefined reference" error, a header isn't found, or you're writing/debugging a `CMakeLists.txt` |
