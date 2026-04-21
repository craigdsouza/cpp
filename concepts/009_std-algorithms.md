# Concepts: std::algorithm

> Reference for the algorithm functions used in Day 9 onwards. All require `#include <algorithm>`. Read this when an algorithm doesn't compile, returns unexpected results, or you're unsure which one to reach for.

---

## std::for_each

**What it does:** Calls a function once for each element. Like a range-for loop but the body is a callable.

```cpp
std::for_each(v.begin(), v.end(), fn);
// fn is called as fn(element) for each element in order
```

**When to use:** When you want to do something *with* every element but don't need a return value. Printing, logging, accumulating into a captured variable.

```cpp
int count = 0;
std::for_each(scan.begin(), scan.end(),
    [&count] (const LidarPoint& p) {
        if (p.intensity > 0.5f) count++;
    });
```

**vs range-for:** Functionally equivalent. `std::for_each` is preferred when the operation is meaningful enough to name, or when you want to swap in `std::execution::par_unseq` later for parallelism.

---

## std::sort

**What it does:** Sorts elements in place. Default: ascending (`operator<`). Custom: provide a comparator lambda.

```cpp
std::sort(v.begin(), v.end());                          // ascending by operator<
std::sort(v.begin(), v.end(), comparator);              // custom order
```

**Comparator contract — strict weak ordering:**
- Must return `true` if `a` should come *before* `b`
- Must return `false` when `a == b` (i.e. use `<` or `>`, never `<=` or `>=`)
- Violating this is **undefined behavior** — the sort may loop infinitely or corrupt memory

```cpp
// Ascending by intensity
std::sort(scan.begin(), scan.end(),
    [] (const LidarPoint& a, const LidarPoint& b) {
        return a.intensity < b.intensity;   // correct: strict <
    });

// Descending
std::sort(scan.begin(), scan.end(),
    [] (const LidarPoint& a, const LidarPoint& b) {
        return a.intensity > b.intensity;   // correct: strict >
    });

// WRONG — undefined behavior
std::sort(scan.begin(), scan.end(),
    [] (const LidarPoint& a, const LidarPoint& b) {
        return a.intensity >= b.intensity;  // returns true when equal — violates strict ordering
    });
```

**Complexity:** O(n log n) average.

---

## std::find_if

**What it does:** Returns an iterator to the **first** element satisfying the predicate. Returns `end()` if none found.

```cpp
auto it = std::find_if(v.begin(), v.end(), predicate);
```

**Always check the result against `end()` before using it:**

```cpp
auto it = std::find_if(scan.begin(), scan.end(),
    [] (const LidarPoint& p) { return p.intensity > 0.9f; });

if (it != scan.end()) {
    std::cout << "Found: " << it->intensity << "\n";
} else {
    std::cout << "Not found\n";
}
```

This is the same sentinel pattern as `std::map::find` from Day 4. The iterator abstraction is consistent across the entire STL — `end()` always means "not found" or "past the end."

**Complexity:** O(n) — scans linearly until a match is found.

**Related:**
- `std::find` — same but matches by value equality (requires `operator==`)
- `std::find_if_not` — returns first element that does NOT satisfy predicate

---

## std::transform

**What it does:** Applies a function to each element and writes the result to an output range. Does not resize the output — caller must ensure output is large enough.

```cpp
// Unary form (one input range → one output range)
std::transform(in.begin(), in.end(), out.begin(), fn);
// fn is called as fn(element), result written to out
```

**In-place transform (input = output):**

```cpp
std::transform(v.begin(), v.end(), v.begin(),
    [max_val] (LidarPoint p) {
        p.intensity /= max_val;
        return p;
    });
```

In-place is safe when the operation is element-wise (output[i] depends only on input[i]).

**Output range sizing — common mistake:**

```cpp
std::vector<float> input = {1.0f, 2.0f, 3.0f};
std::vector<float> output;   // empty — size 0!
std::transform(input.begin(), input.end(), output.begin(), fn);
// UNDEFINED BEHAVIOR — writing past end of empty output vector
```

Fix: pre-size the output, or use `std::back_inserter`:

```cpp
output.resize(input.size());                                      // pre-size
std::transform(input.begin(), input.end(), output.begin(), fn);  // safe

// Or:
std::transform(input.begin(), input.end(), std::back_inserter(output), fn);
// back_inserter calls push_back — output grows automatically
```

**vs Python `map`:** Python's `map` is lazy (returns an iterator). `std::transform` is eager — it writes immediately.

**Complexity:** O(n).

---

## std::count_if

**What it does:** Returns the number of elements satisfying a predicate.

```cpp
int n = std::count_if(v.begin(), v.end(), predicate);
```

```cpp
int hot = std::count_if(scan.begin(), scan.end(),
    [] (const LidarPoint& p) { return p.intensity > 0.8f; });
std::cout << "High-intensity points: " << hot << "\n";
```

**vs Python `sum(1 for x in v if pred(x))`** — same idea, different syntax.

**Complexity:** O(n).

---

## std::remove_if + erase (erase-remove idiom)

**What it does:** `std::remove_if` shuffles matching elements to the end and returns an iterator to the new logical end. The elements after that iterator are unspecified — you must call `erase` to actually remove them.

```cpp
auto new_end = std::remove_if(v.begin(), v.end(), predicate);
v.erase(new_end, v.end());

// One-liner:
v.erase(std::remove_if(v.begin(), v.end(), predicate), v.end());
```

```cpp
// Remove all points below confidence threshold
scan.erase(
    std::remove_if(scan.begin(), scan.end(),
        [] (const LidarPoint& p) { return p.intensity < 0.1f; }),
    scan.end());
```

**Why two steps?** `std::remove_if` doesn't know the container — it only works with iterators. Erasing from the container requires calling a method on it. This separation is intentional in the STL design.

**Complexity:** O(n).

---

## Choosing the right algorithm

| Goal | Algorithm |
|------|-----------|
| Do something with every element | `std::for_each` |
| Sort elements | `std::sort` |
| Find the first matching element | `std::find_if` |
| Apply a function to every element | `std::transform` |
| Count matching elements | `std::count_if` |
| Remove matching elements | `std::remove_if` + `erase` |
| Check if any element matches | `std::any_of` |
| Check if all elements match | `std::all_of` |
| Find min/max element | `std::min_element` / `std::max_element` |

---

## Common mistakes

| Mistake | Consequence | Fix |
|---------|-------------|-----|
| Using `>=` in sort comparator | Undefined behavior | Use `<` or `>` only |
| Not checking `find_if` result against `end()` | Undefined behavior (dereference invalid iterator) | Always `if (it != v.end())` |
| Passing empty output to `transform` | Undefined behavior (buffer overflow) | `resize` output first or use `back_inserter` |
| Forgetting `erase` after `remove_if` | Elements still in vector (just moved to end) | Always pair with `.erase()` |
| Modifying a container while iterating it | Undefined behavior | Never push_back/erase inside a for_each lambda on the same container |
