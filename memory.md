# Student C++ Understanding Snapshot

Last updated: 2026-04-22 (after Day 10)

This file documents the student's current C++ understanding — what is solid, what has gaps, and what patterns have emerged in how they learn. It is intended to inform the creation of new project days.

---

## Background

- **Primary language:** Python
- **Goal:** NVIDIA DRIVE AV stack — C++ for localization, perception, sensor pipelines
- **Days completed:** 1–10 (Hello Map, References & Pointers, Classes & Structs, STL Containers, RAII & Destructors, Smart Pointers, Move Semantics, Templates, Lambdas + std::algorithm, File Parsing)

---

## What is solid

### Syntax and mechanics

- Writes correct constructors, destructors, member functions without prompting
- Correctly uses `const T&` for read-only parameters, `T&` for mutation, `T`* for optional/nullable
- Understands `this->` and member initialization
- Understands range-for with and without `&` (copy vs reference)
- Correctly uses `std::map`, `std::unordered_map`, `std::vector` — insert, find, erase, iterate
- Writes RAII classes correctly: constructor acquires, destructor releases
- Uses `std::ofstream` / `std::ifstream`, `std::getline` correctly
- Correctly uses `try` / `catch` / `throw` and understands when to apply them
- Uses `std::unique_ptr` and `std::shared_ptr` correctly — `make_unique`, `make_shared`, `std::move`, `.get()`, `*ptr`
- Writes factory functions returning `unique_ptr` correctly
- Implements move constructor and move assignment operator from scratch (steal-and-nullify pattern, self-assignment guard, free existing before steal)
- Implements full Rule of Five for a raw-pointer-owning class (`LidarFrame`)
- Writes `PipelineStage` with deleted copy, move constructor, move assignment, ingest/extract methods — from scratch, no scaffold
- Writes function templates (`template<typename T>`) and class templates with member functions in class body
- Writes full template specialization (`template<>`) — correct syntax and firing verified
- Uses non-type template parameters (`template<typename T, int N>`) with `constexpr` arguments
- Implements `FixedBuffer<T,N>` — stack-allocated array, head/tail/size tracking, modulo wrapping, zero heap allocation
- Implements `RingBuffer<T>` — same pattern backed by `std::vector<T>`, overwrite-on-full behavior
- Implements `SensorPipeline<T,N>` from scratch — owns FixedBuffer, feed/read/apply/print_all, `peek` for const access, function pointer parameter
- Writes lambdas with `[]`, `[x]`, `[&x]`, `[=]`, `[&]` captures — correct intuition for which to use
- Uses `std::for_each`, `std::sort`, `std::find_if`, `std::transform`, `std::count_if` correctly with lambdas
- Implements `LidarScanProcessor` class from scratch — owns vector, all five methods using algorithm + lambda pattern
- Understands iterator vs pointer distinction (`&(*it)` to convert iterator to pointer)
- Understands when `std::transform` in-place is safe (same input/output vector, element-wise)
- `std::stringstream(line)` + `std::getline(ss, field, ',')` — applies the CSV field-splitting pattern fluently across multiple exercises without prompting
- `try/catch std::invalid_argument` around `stof` in a parse loop — implemented correctly and independently
- `std::map::insert({key, value})` — understood the operator[] default-construction trap when the compiler surfaced it and applied the fix correctly
- `std::accumulate` with a binary lambda for summing a struct field — reached for the right tool independently

### Conceptual understanding

- RAII: can define it, give a concrete example, explain composability
- Destructor timing: understands scope exit, return, and exception as trigger paths
- Stack unwinding: understands that a `catch` somewhere up the call stack is required for unwinding to occur
- Heap vs stack distinction: understands that heap requires explicit `delete[]`, that RAII wraps this
- `std::ofstream` and `std::ifstream` are themselves RAII types
- Python `with` vs RAII: understands opt-in vs automatic, understands composability
- Iterator safety: correctly identifies `end()` as a sentinel, avoids dereferencing it
- Ghost tile danger of `operator[]` on `std::map`
- `unique_ptr` exception safety: understands that raw `delete` is skipped on exception or early return; `unique_ptr` destructor fires regardless
- `shared_ptr` reference counting: can trace count step by step across copies, resets, and scope exits
- Partially constructed objects: destructor does not run for partially constructed objects (carry-forward from Day 5 — fully answered in Day 6 QR2)
- NRVO: understands that the compiler builds the returned object directly in the caller's slot; understands `std::move` on return value suppresses NRVO (anti-pattern)
- `std::move` is a cast (lvalue→xvalue), not a transfer — actual resource move happens in move constructor or move assignment
- Steal-and-nullify: both pointer and count must be nullified after a move; `delete[] nullptr` is safe no-op
- `= delete` on copy constructor/assignment enforces single-owner invariant at compile time
- Value categories: lvalue (has name, persistent), prvalue (pure temporary), xvalue (about to expire — result of `std::move`); `&&` binds to rvalues only

---

## Known gaps

### CS fundamentals (most important)

- Stack/heap memory model was not known going into Day 5 — learned reactively mid-exercise
- Does not yet have fluent mental model of stack frames, call stack, heap allocator
- → **Mitigation:** `concepts/005_memory-model.md` written before Day 6

### Depth gaps on "why" questions

- **Reverse destruction order:** knows it happens, hasn't connected it to dependency preservation (b may depend on a, so destroy b first)
- **Hash internals:** knows `unordered_map` has O(1) lookup, doesn't know hash buckets explain the arbitrary iteration order
- **Leak quantification:** correct on identifying leaks, struggles to reason about severity at scale (100Hz × 60s = 6,000 calls)
- **unique_ptr ownership precision:** understands exclusive ownership but occasionally frames it as "only one entity uses it" rather than "one entity is solely responsible for cleanup." The distinction matters when designing components — exclusive ownership is about cleanup responsibility, not just access count.
- **Strict weak ordering:** Knows to use `>` not `>=` in sort comparators but doesn't yet use the term "strict weak ordering" or explain that `comp(x,x)` must return `false`. Surface in Day 10 warm-up.
- **std::transform output safety:** Fully resolved in Day 10 QR1 — correctly named UB, explained buffer overflow, confirmed compiler cannot catch it.
- **Dangling reference framing:** Described `[&]` danger as "accidental mutation of many variables" rather than "lambda outliving the scope of captured variables." The lifetime angle is the real risk. Persists.
- **std::optional:** Q4 unanswered — no exposure to `std::optional<T>` yet. Needs introduction: `std::nullopt` instead of `nullptr`, stack-allocated, "maybe" contract explicit in return type, preferred over raw pointer for "value or nothing" returns.
- **try/catch placement in a parse loop:** Structure of try/catch is understood (Q3: 0.75), but placement nuance not yet solid — the `try` must be inside the `while(getline)` loop, not wrapping it. Placing it outside causes the loop to exit on the first bad line.
- **Template conceptual vocabulary:** Can use templates correctly but doesn't yet use terms "type deduction," "instantiation," "most specific match" fluently in explanations. Mechanics ahead of vocabulary — typical pattern for this student.

### Vocabulary precision

- Used "try/catch in C" as the answer for what C requires without RAII — C has no exceptions; the correct answer is manual cleanup on every exit path
- Occasionally imprecise on "destroyed immediately" vs "destroyed when out of scope"

---

## Learning patterns

### Strengths

- **Strong applied learner:** exercises all pass; code is correct before fully understanding the why
- **Asks good follow-up questions:** goes beyond the exercise to probe the underlying concept (e.g. "why does `<<` not work stream-to-stream?", "so `>>` can't handle stream to stream but `<<` can?")
- **Self-corrects:** caught the `with` vs RAII distinction on their own after a hint
- **Notes composability unprompted** — named it before it was introduced (Day 5 Q4)

### Areas to support

- **"Why" reasoning was lagging early (Days 2–5) but is closing** — Day 6 quiz was 6.75/7.0; conceptual questions in session were sharp and unprompted (NRVO, implicit unique_ptr→shared_ptr conversion, push_back internals). Continue giving CS fundamentals before they are needed rather than reactively.
- **Benefits from Python analogies** — `getline` loop clicked immediately when compared to `for line in f`; stack unwinding clicked when compared to LIFO; shared_ptr reference counting clicked via Python GC comparison
- **CS fundamentals need to be given before they are needed**, not explained reactively mid-exercise

### Preferred learning style

- Works through exercises first, then asks conceptual questions
- Responds well to step-by-step breakdowns (e.g. push_back resize walkthrough)
- Appreciates when gaps are named explicitly rather than glossed over

---

## Curriculum position


| Day | Topic                     | Status   |
| --- | ------------------------- | -------- |
| 1   | Hello Map, types, vectors | Complete |
| 2   | References and Pointers   | Complete |
| 3   | Classes and Structs       | Complete |
| 4   | STL Containers            | Complete |
| 5   | RAII and Destructors      | Complete |
| 6   | Smart Pointers            | Complete |
| 7   | Move Semantics            | Complete |
| 8   | Templates                 | Complete |
| 9   | Lambdas + std::algorithm  | Complete |
| 10  | File Parsing              | Complete |
| 11  | TBD                       | **Next** |


**Coming into Day 11:** File parsing mechanics fully solid — ifstream, stringstream, stof/stoi, header skip, try/catch guard in parse loop. All four exercises passed. Carry-forward: `std::optional<T>` (unanswered Q4 — introduce early in Day 11), try/catch placement nuance (must be inside the while loop, not wrapping it), dangling reference lifetime framing still imprecise.

---

## Recommendations for Day 10 and beyond

- **std::optional warm-up** — introduce `std::optional<T>` and `std::nullopt` early in Day 11. Student left Q4 blank — no prior exposure. Frame as the modern alternative to returning a raw pointer for "value or nothing."
- **try/catch placement** — reinforce that the `try` block belongs inside the `while(getline)` loop, not outside it. The loop must continue past a bad line, not exit.
- **Dangling reference lifetime framing** — when reviewing `[&]` captures, ask "what happens if the lambda is stored and called after the enclosing function returns?" rather than "what can go wrong?"
- **Real-world context docs are highly motivating** — create a domain context doc for each new topic. Student explicitly stated this keeps energy up through syntax difficulty.
- **Keep Python analogies available** — they land well
- **4 exercises per day** — New default is 4. Time estimates added to each exercise.
- **Mechanics before new concepts** — student's bottleneck is unfamiliar C++ mechanics, not conceptual difficulty. Ensure each exercise introduces at most 2 new mechanics; check Practiced Mechanics in glossary.md before designing exercises.

