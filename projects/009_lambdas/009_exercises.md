# Day 9 — Lambdas + std::algorithm

**Goal:** Replace hand-written loops with expressive, one-line operations using C++ lambdas and the standard algorithm library — the same tools NVIDIA engineers use to process lidar point clouds, filter map features, and transform sensor data pipelines without a loop in sight.

> Before starting: answer the warm-up questions in `009_lambdas_quiz.md`.

---

## Background

In Python you've been writing things like this for years:

```python
points = sorted(scan, key=lambda p: p.intensity)
hot    = [p for p in scan if p.intensity > 0.8]
norms  = list(map(lambda p: p.intensity / max_val, scan))
```

C++ has the exact same idea — just with different syntax. A **lambda** is an anonymous function defined inline, and `std::algorithm` is the standard library of operations that accept them.

```cpp
// Python: sorted(scan, key=lambda p: p.intensity)
std::sort(scan.begin(), scan.end(),
    [](const LidarPoint& a, const LidarPoint& b) {
        return a.intensity < b.intensity;
    });

// Python: next(p for p in scan if p.intensity > 0.8)
auto it = std::find_if(scan.begin(), scan.end(),
    [](const LidarPoint& p) { return p.intensity > 0.8f; });
```

The `[]` at the start is the **capture list** — it controls which variables from the surrounding scope the lambda can see. `[]` means capture nothing; `[=]` captures everything by value; `[&]` captures everything by reference; `[max_val]` captures only `max_val` by value.

```cpp
float threshold = 0.5f;
auto above = std::count_if(scan.begin(), scan.end(),
    [threshold](const LidarPoint& p) { return p.intensity > threshold; });
//   ^^^^^^^
//   captures 'threshold' from the enclosing scope — the lambda can now use it
```

In the DRIVE stack, lambdas appear everywhere sensor data is processed: sorting points by distance for nearest-neighbour queries, filtering out points below a confidence threshold before feeding them to the object detector, normalizing intensity across a scan before writing to a map tile. The algorithms (`std::sort`, `std::find_if`, `std::transform`, `std::for_each`, `std::count_if`) are the workhorse — lambdas are how you tell them what to do.

> **Real-world note:** DriveWorks and NVIDIA's sensor processing libraries use `std::transform` and `std::for_each` extensively for per-point operations because they express intent clearly, are easy to parallelize (swap to `std::execution::par_unseq` with one keyword in C++17), and avoid the subtle off-by-one errors that hand-written loops accumulate.

---

## Exercise 1 — Lambda Basics and std::for_each (~45 min)

**File:** Create `projects/009_lambdas/lambda_basics.cpp` (new file).

You are given a lidar scan as a `std::vector<LidarPoint>`. Each point has `float x, y, z, intensity`.

Tasks:
- Define the `LidarPoint` struct with those four fields and a constructor
- Create a vector of 6 points with varying intensities (some below 0.5f, some above)
- Use `std::for_each` with a lambda to print each point: `"(x, y, z) intensity: val"`
- Use `std::for_each` with a lambda that captures a counter `int count` by reference — count how many points have `intensity > 0.5f`, then print the count
- Write the same count using a plain range-for loop to compare — observe that both produce identical output

**What to observe:** The lambda passed to `std::for_each` is called once per element, exactly like a range-for body — but it's a first-class value you can pass around, store, or compose. The capture `[&count]` is how the lambda "sees" a variable from the outer scope. Without the `&`, the lambda gets a copy and your counter stays at zero.

---

## Exercise 2 — Sorting and Finding (~60 min)

**File:** Create `projects/009_lambdas/sort_find.cpp` (new file).

The perception pipeline needs to sort a lidar scan by intensity before feeding it to the object detector, and quickly locate the first point above a confidence threshold.

Tasks:
- Reuse the `LidarPoint` struct (redefine it here)
- Create a `std::vector<LidarPoint>` of 8 points with mixed intensities
- Use `std::sort` with a lambda comparator to sort ascending by intensity — print the sorted order
- Use `std::sort` again to sort descending — print
- Use `std::find_if` with a lambda to find the **first** point with `intensity > 0.7f`
  - Check the returned iterator against `end()` before using it (same pattern as `std::map::find`)
  - If found, print the point; if not found, print `"No point above threshold"`
- Try a threshold that guarantees no match — verify the not-found path fires

**What to observe:** `std::sort` takes a comparator — a callable that returns `true` if the first argument should come before the second. `std::find_if` returns an iterator — exactly like `std::map::find` from Day 4. The `it != v.end()` guard is the same sentinel pattern you've been using since Day 4; it just appears in a new context here.

---

## Exercise 3 — Transform and Count (~75 min)

**File:** Create `projects/009_lambdas/transform_count.cpp` (new file).

Before writing intensity values into a map tile, the pipeline normalizes them to [0.0, 1.0] relative to the scan's maximum. You'll implement this using `std::transform` with a captured value.

Tasks:
- Redefine `LidarPoint` with at least `float intensity`
- Create a vector of 7 points
- Find the maximum intensity manually using a range-for loop (store as `float max_val`)
- Use `std::transform` with a lambda that **captures `max_val` by value** to normalize every point's intensity in place: `p.intensity = p.intensity / max_val`
  - `std::transform` signature: `std::transform(begin, end, output_begin, fn)` — use the same vector as both input and output (in-place)
  - The lambda takes a `LidarPoint` and returns a `LidarPoint` with the modified intensity
- After normalizing, use `std::count_if` to count how many points have `intensity > 0.5f`
- Print the normalized intensities and the count
- Also use `std::count_if` with a lambda that captures a `std::string label` — just to demonstrate non-numeric captures work too (e.g., count points whose intensity rounds to a specific bucket)

**What to observe:** `[max_val]` captures by value — the lambda gets a snapshot of `max_val` at the moment it's created. If `max_val` changed after the lambda was created, the lambda wouldn't see the change. `[&max_val]` would see live updates. For normalization, by-value is correct — you want the fixed maximum, not a potentially-changing reference.

---

## Exercise 4 — Integration (~90 min)

**File:** Create `projects/009_lambdas/integration.cpp` (new file).
**No scaffold provided.** Write this program from scratch.

Design and implement a `LidarScanProcessor` class that models a single scan's processing stage in the DRIVE perception pipeline. The class must:

- Own a `std::vector<LidarPoint>` of points (where `LidarPoint` has `float x, y, z, intensity`)
- `void add(LidarPoint p)` — adds a point to the internal vector
- `void sort_by_intensity()` — sorts points ascending by intensity using `std::sort` + lambda
- `const LidarPoint* find_above(float threshold) const` — returns a pointer to the first point above threshold using `std::find_if`; returns `nullptr` if none found
- `void normalize()` — normalizes all intensities to [0,1] relative to the scan maximum using `std::transform` with a captured max value; does nothing if the vector is empty
- `int count_above(float threshold) const` — returns count of points above threshold using `std::count_if`
- `void print_all(std::string label) const` — prints all points using `std::for_each`

In `main`:
1. Create a `LidarScanProcessor`, add 8 lidar points with intensities spread across [0.3f, 1.8f]
2. Print all points (label: `"raw"`)
3. Sort by intensity, print again (label: `"sorted"`)
4. Find the first point above 1.0f and print it
5. Normalize — print all (label: `"normalized"`) and verify all intensities are in [0,1]
6. Count how many normalized points are above 0.6f, print the count
7. Create a second `LidarScanProcessor` with only 3 points all below 0.5f — verify `find_above(0.8f)` returns nullptr and prints the not-found message

Correct output must clearly show: the sort order, the normalization bringing intensities into [0,1], and the nullptr path being handled safely.

---

## Exercise Results — 2026-04-21

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — Lambda Basics and std::for_each | Pass | All tasks complete; count vs count2 bug self-caught |
| Exercise 2 — Sorting and Finding | Pass | Sort + find_if correct; no-match threshold left as comment |
| Exercise 3 — Transform and Count | Pass | All tasks complete including non-numeric string capture |
| Exercise 4 — Integration | Pass | Full class from scratch; find_above pointer deref flagged but not fully fixed |

---

## Checkpoint

You've passed Day 9 when you can:
- Write a lambda with `[]`, `[=]`, `[&]`, and specific captures — and explain what each does
- Use `std::for_each`, `std::sort`, `std::find_if`, `std::transform`, and `std::count_if` correctly
- Handle the iterator returned by `std::find_if` safely using the `end()` sentinel check
- Explain why `[max_val]` (by value) is correct for normalization but `[&max_val]` could be dangerous
- Connect lambdas to Python's `lambda`, `filter`, `map`, and `sorted` — same idea, different syntax
