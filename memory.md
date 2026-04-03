# Student C++ Understanding Snapshot

Last updated: 2026-04-03 (after Day 5)

This file documents the student's current C++ understanding — what is solid, what has gaps, and what patterns have emerged in how they learn. It is intended to inform the creation of new project days.

---

## Background

- **Primary language:** Python
- **Goal:** NVIDIA DRIVE AV stack — C++ for localization, perception, sensor pipelines
- **Days completed:** 1–5 (Hello Map, References & Pointers, Classes & Structs, STL Containers, RAII & Destructors)

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

### Conceptual understanding

- RAII: can define it, give a concrete example, explain composability
- Destructor timing: understands scope exit, return, and exception as trigger paths
- Stack unwinding: understands that a `catch` somewhere up the call stack is required for unwinding to occur
- Heap vs stack distinction: understands that heap requires explicit `delete[]`, that RAII wraps this
- `std::ofstream` and `std::ifstream` are themselves RAII types
- Python `with` vs RAII: understands opt-in vs automatic, understands composability
- Iterator safety: correctly identifies `end()` as a sentinel, avoids dereferencing it
- Ghost tile danger of `operator[]` on `std::map`

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
- **Partially constructed objects (Q5 Day 5 — unanswered):** if `b`'s constructor throws in a sequence `a`, `b`, `c` — `b`'s destructor does NOT run (never fully constructed), `a`'s does. Carry-forward to Day 6.

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

- **"Why" reasoning lags "what" application** — quiz scores are consistently 0.75 on reasoning questions that require knowing CS internals
- **Benefits from Python analogies** — `getline` loop clicked immediately when compared to `for line in f`; stack unwinding clicked when compared to LIFO
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
| 6   | Smart Pointers            | **Next** |
| 7   | Move Semantics            | Upcoming |


**Coming into Day 6:** Student has solid RAII mechanics. Memory model explainer (`concepts/memory-model.md`) written to pre-load heap/stack intuition. Carry-forward: partially constructed objects (Q5 Day 5).

---

## Recommendations for Day 6 and beyond

- **Assume memory model is pre-read** — reference `concepts/memory-model.md` in the Day 6 background
- **Pre-empt the partially constructed object question** — it will come up naturally with `unique_ptr` construction
- **Use `TileBuffer` as the bridge** — student already wrote it with `new`/`delete[]`; `unique_ptr<int[]>` is the natural next step
- **Keep Python analogies available** — they land well with this student
- **Lean into "why" questions in quiz** — student needs reps on reasoning from CS fundamentals, not just applying syntax
- **Avoid assuming CS background knowledge** — introduce heap allocator, hash buckets, etc. explicitly before they are needed

