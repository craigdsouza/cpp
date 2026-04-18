# Day 7 — Move Semantics

**Goal:** Understand what `std::move` actually does under the hood, implement move constructors and move assignment operators, and write C++ where resources are transferred — never accidentally copied — laying the foundation for efficient sensor data pipelines in the NVIDIA DRIVE stack.

> Before starting: answer the warm-up questions in `007_move_semantics_quiz.md`.

---

## Background

In Day 6 you used `std::move` to transfer a `unique_ptr`. It worked — but you used it as a black box. Today you open the box.

**Lvalues, rvalues, and references.** Every expression in C++ is either an *lvalue* (has a name, persists beyond the expression — you can take its address) or an *rvalue* (a temporary, no persistent identity). `int x = 5` — `x` is an lvalue, `5` is an rvalue. A function return value is an rvalue. An rvalue reference (`T&&`) binds to rvalues — it says "I can steal from this, it won't be used again."

**What `std::move` actually does.** `std::move` is not a function that moves anything. It is a cast — it unconditionally converts its argument to an rvalue reference (`T&&`), telling the compiler "treat this as a temporary whose resources can be stolen." The actual transfer of resources happens in the *move constructor* or *move assignment operator* that gets called as a result. The name is misleading to beginners: `std::move(x)` does not move `x`. It makes `x` *movable*.

**The move constructor pattern.** A move constructor takes a `T&&` parameter, *steals* the source's resources (pointers, file handles, etc.), then *nullifies* the source so its destructor is harmless. This is the "steal and nullify" pattern. If you forget to nullify, the source's destructor frees memory the destination now owns — a double-free and undefined behavior.

**Why this matters for AV.** In the NVIDIA DRIVE stack, lidar frames, camera buffers, and map tiles are large. Copying them is expensive and wrong — a lidar frame has one owner at a time, and ownership must transfer as the frame moves down the pipeline (capture → preprocessing → inference → disposal). Move semantics is what makes this zero-copy ownership transfer possible without raw pointers.

**Python analogy.** Python has no move semantics — everything is a reference, and the GC handles lifetime. The closest manual equivalent is `a = b; b = None` — you've manually done what a C++ move constructor does automatically. C++ formalizes and optimizes this so the compiler can elide copies across function boundaries.

---

## Exercise 1 — Observe copy vs move

**File:** Create `projects/007_move_semantics/copy_vs_move.cpp` (new file).

Define a `SensorBuffer` class with a `std::string name` and an `int size`. Give it:
- A copy constructor that prints `"SensorBuffer copied: <name>"`
- A move constructor that prints `"SensorBuffer moved: <name>"`

Then in `main`, trigger each and observe which fires:

```cpp
// construct an original buffer
// trigger a copy (pass by value to a function, or copy-construct)
// trigger a move (use std::move or return from a function)
// observe which constructor fires in each case
```

**What to observe:** The copy constructor fires when you copy-construct. The move constructor fires when you `std::move` or return by value (if NRVO doesn't apply). After the move, access the original — what is `name`? What does this tell you about what the move constructor should do to the source?

---

## Exercise 2 — Moving standard containers

**File:** Create `projects/007_move_semantics/move_containers.cpp` (new file).

Work with `std::vector<std::string>` representing a list of map tile labels. Write a `process_tiles` function that takes a vector by value (a "sink" parameter — it accepts ownership), then in `main`:

- Create a vector of 5 tile labels and print its size
- Move it into a second vector using `std::move` — print both sizes after
- Call `process_tiles` once by copying, once by moving — observe via size checks what happened to the source each time

**What to observe:** After `std::move(v)`, the source vector is in a valid but unspecified state — typically empty. Moving a `std::vector` is O(1) regardless of element count. Copying is O(n). For a lidar point cloud with 100,000 points this difference is significant.

---

## Exercise 3 — Implement a move constructor and move assignment

**File:** Update `projects/007_move_semantics/lidar_frame.cpp`.

Define a `LidarFrame` class that owns a raw `float*` point cloud array. Complete the move assignment operator and `main`:

- Move assignment operator: guard against self-assignment, free existing `this->points`, steal from `other`, nullify `other`, return `*this`
- In `main`: demonstrate construction, copy, and move — verify the source is nullified after a move and that the destructor on a moved-from object is safe (no double-free)

**What to observe:** The move assignment operator differs from the move constructor in one key way — `this` already owns an array that must be freed before stealing. The "steal and nullify" pattern is the entire substance of move semantics. Every move constructor and move assignment operator you ever write follows this shape.

---

## Exercise 4 — Move semantics in return values

**File:** Create `projects/007_move_semantics/return_move.cpp` (new file).

Write two factory functions for `LidarFrame` (reuse from Exercise 2 or redefine inline):

```cpp
// factory_nrvo: creates a named local LidarFrame and returns it by value
// factory_move: creates a named local LidarFrame, wraps it in std::move before returning

// in main:
//   call factory_nrvo — does "LidarFrame moved" print?
//   call factory_move — does "LidarFrame moved" print?
//   which approach is correct and why?
```

**What to observe:** `factory_nrvo` triggers NRVO — the compiler constructs the frame directly in the caller's slot, no move occurs. `factory_move` *disables* NRVO by converting the named variable to an rvalue before the return — you've told the compiler "this is a temporary" which prevents the copy elision optimisation. `std::move` on a return value is an anti-pattern. Never do it.

---

## Exercise 5 — Integration

**File:** Create `projects/007_move_semantics/integration.cpp` (new file).
**No scaffold provided.** Write this program from scratch.

Design and implement a `PipelineStage` class that models one stage in the DRIVE sensor pipeline. The class must:

- Own a `std::vector<LidarFrame>` (use your `LidarFrame` from Exercise 2, or redefine it here)
- Be move-constructible and move-assignable
- Have its copy constructor and copy assignment **explicitly deleted** — pipeline stages are never duplicated
- Have a method `void ingest(LidarFrame&& frame)` that takes a frame by move and stores it
- Have a method `LidarFrame extract()` that removes and returns the oldest frame by move (transfers ownership to the caller)
- Have a method `int frame_count() const`

In `main`:
- Create two `PipelineStage` objects
- Ingest 3 frames into the first stage
- Move the first stage into the second (transfer all ownership)
- Extract all frames from the second stage one by one, printing each frame's count
- Verify at the end: first stage has 0 frames, all `LidarFrame` destructors have fired exactly once

Correct output should make clear that no frame was ever copied — only constructed, moved, and destroyed.

---

## Exercise Results — 2026-04-17

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — copy_vs_move.cpp | Pass | SensorBuffer copy and move constructors correct, main demonstrates both |
| Exercise 2 — move_containers.cpp | Pass | process_tiles sink parameter, copy vs move observed via size checks |
| Exercise 3 — lidar_frame.cpp | Pass | Full Rule of Five including move assignment with self-assignment guard |
| Exercise 4 — return_move.cpp | Pass | Both factories, NRVO vs anti-pattern clearly demonstrated |
| Exercise 5 — integration.cpp | Pass | PipelineStage from scratch, copy deleted, ingest/extract/frame_count all correct |

---

## Checkpoint

You've passed Day 7 when you can:
- Explain what `std::move` actually does (a cast, not a transfer) and why it's needed
- Implement a move constructor following the steal-and-nullify pattern and explain why nullifying is essential
- Explain why `std::move` on a return value is an anti-pattern that disables NRVO
- Delete copy operations explicitly and explain when that's the right design choice
- Explain why move semantics matters for AV sensor pipelines specifically (large buffers, single ownership, zero-copy transfer)
