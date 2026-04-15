# Student C++ Understanding Snapshot

Last updated: 2026-04-15 (after Day 6)

This file documents the student's current C++ understanding — what is solid, what has gaps, and what patterns have emerged in how they learn. It is intended to inform the creation of new project days.

---

## Background

- **Primary language:** Python
- **Goal:** NVIDIA DRIVE AV stack — C++ for localization, perception, sensor pipelines
- **Days completed:** 1–6 (Hello Map, References & Pointers, Classes & Structs, STL Containers, RAII & Destructors, Smart Pointers)

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
- NRVO: understands that the compiler builds the returned object directly in the caller's slot

---

## Known gaps

### CS fundamentals (most important)

- Stack/heap memory model was not known going into Day 5 — learned reactively mid-exercise
- Does not yet have fluent mental model of stack frames, call stack, heap allocator
- → **Mitigation:** `concepts/memory-model.md` written before Day 6

### Depth gaps on "why" questions

- **Reverse destruction order:** knows it happens, hasn't connected it to dependency preservation (b may depend on a, so destroy b first)
- **Hash internals:** knows `unordered_map` has O(1) lookup, doesn't know hash buckets explain the arbitrary iteration order
- **Leak quantification:** correct on identifying leaks, struggles to reason about severity at scale (100Hz × 60s = 6,000 calls)
- **unique_ptr ownership precision:** understands exclusive ownership but occasionally frames it as "only one entity uses it" rather than "one entity is solely responsible for cleanup." The distinction matters when designing components — exclusive ownership is about cleanup responsibility, not just access count.

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
| 7   | Move Semantics            | **Next** |


**Coming into Day 7:** Student has solid smart pointer mechanics — unique_ptr, shared_ptr, move, factories. Carry-forward: articulate exclusive ownership as cleanup responsibility (not just access count). Move semantics will build directly on std::move already encountered in Day 6.

---

## Recommendations for Day 7 and beyond

- **Move semantics will feel familiar** — student already used `std::move` in Day 6; Day 7 should build on that intuition and explain what's happening under the hood (move constructor, move assignment, rvalue references)
- **Connect move to unique_ptr** — student knows unique_ptr can't be copied but can be moved; Day 7 explains why that's possible at the language level
- **Ownership precision is the carry-forward theme** — use Day 7 to reinforce: exclusive ownership = sole cleanup responsibility, not just sole access
- **Keep Python analogies available** — they land well with this student
- **Lean into "why" questions in quiz** — student needs reps on reasoning from CS fundamentals, not just applying syntax
- **Avoid assuming CS background knowledge** — introduce concepts explicitly before they are needed

