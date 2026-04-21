# Concepts: Lambdas

> A lambda is an anonymous function defined inline. The `[]` is the capture list, `()` is the parameter list, and `{}` is the body. Read this when a lambda doesn't compile, a capture behaves unexpectedly, or you're unsure which capture mode to use.

---

## Basic syntax

```cpp
// Full form
auto fn = [capture] (parameters) -> return_type { body };

// Return type is almost always omitted — compiler deduces it
auto fn = [capture] (parameters) { body };

// No parameters — parentheses can be omitted too
auto fn = [capture] { body };
```

Python comparison:

```python
fn = lambda x: x > 0.5          # Python
auto fn = [](float x) { return x > 0.5f; };   // C++
```

---

## Capture modes

The capture list controls which variables from the surrounding scope the lambda can access.


| Syntax    | Meaning                                                         |
| --------- | --------------------------------------------------------------- |
| `[]`      | Capture nothing — lambda can only use its own parameters        |
| `[=]`     | Capture everything in scope by **value** (snapshot at creation) |
| `[&]`     | Capture everything in scope by **reference** (live access)      |
| `[x]`     | Capture only `x` by value                                       |
| `[&x]`    | Capture only `x` by reference                                   |
| `[x, &y]` | Capture `x` by value, `y` by reference                          |
| `[=, &x]` | Capture everything by value except `x` by reference             |


```cpp
float threshold = 0.5f;

auto by_val = [threshold] (float x) { return x > threshold; };
threshold = 99.0f;
by_val(1.0f);   // still compares against 0.5f — snapshot was taken at creation

auto by_ref = [&threshold] (float x) { return x > threshold; };
threshold = 99.0f;
by_ref(1.0f);   // now compares against 99.0f — sees the live value
```

**Rule of thumb:**

- Use `[x]` (by value) when the lambda is passed to an algorithm immediately — safe, explicit, minimal.
- Use `[&x]` (by reference) only when you need the lambda to *write back* to the outer variable (e.g. accumulating a counter).
- Avoid `[&]` for lambdas that outlive the current scope — the references will dangle.

---

## Dangling reference risk

```cpp
std::function<bool(float)> make_filter(float threshold) {
    return [&threshold] (float x) { return x > threshold; };
    //      ^^^^^^^^^^^ DANGER: threshold is a local variable
    //      It's destroyed when make_filter returns.
    //      The returned lambda holds a dangling reference.
}

// Safe version:
std::function<bool(float)> make_filter(float threshold) {
    return [threshold] (float x) { return x > threshold; };
    //      ^^^^^^^^^ captures by value — safe to return
}
```

**When is `[&]` safe?** When the lambda is created and used within the same scope — e.g. passed directly to `std::for_each` or `std::sort` in the same function. The lambda never outlives the variables it references.

---

## Capture with mutable

By default, captured-by-value variables are `const` inside the lambda — you can read them but not modify them. Use `mutable` to allow modification of the copy:

```cpp
int count = 0;
auto fn = [count]() mutable { count++; return count; };
// modifies the lambda's OWN copy of count — the outer count is unchanged
```

This is rarely needed. If you want to write back, capture by reference instead.

---

## Lambdas as algorithm arguments

Lambdas are the idiomatic way to pass behaviour to `std::algorithm` functions:

```cpp
// std::for_each — called once per element
std::for_each(v.begin(), v.end(), [](const T& x) { /* do something */ });

// std::sort — comparator: return true if a should come before b
std::sort(v.begin(), v.end(), [](const T& a, const T& b) { return a.val < b.val; });

// std::find_if — predicate: return true when element is found
auto it = std::find_if(v.begin(), v.end(), [](const T& x) { return x.val > 0.5f; });

// std::transform — unary function: maps input element to output element
std::transform(v.begin(), v.end(), out.begin(), [](T x) { x.val *= 2; return x; });

// std::count_if — predicate: count elements satisfying condition
int n = std::count_if(v.begin(), v.end(), [](const T& x) { return x.val > 0.5f; });
```

---

## Generic lambdas (C++14)

Use `auto` as a parameter type to make a lambda work with any type — like a function template:

```cpp
auto print = [] (const auto& x) { std::cout << x << "\n"; };
print(42);       // works
print(3.14f);    // works
print("hello");  // works
```

---

## Storing lambdas with std::function

If you need to store a lambda in a variable with a fixed type (e.g. as a class member or function return value), use `std::function`:

```cpp
#include <functional>

std::function<bool(float)> filter = [](float x) { return x > 0.5f; };
filter(0.3f);   // false
filter(0.8f);   // true
```

`std::function` has overhead (type erasure + heap allocation). For performance-critical paths, prefer passing lambdas as template parameters or `auto` — the compiler can then inline them completely.

---

## In the AV context

DriveWorks and NVIDIA's sensor processing pipelines use lambdas extensively:

```cpp
// Normalize intensity across a lidar scan
float max_val = compute_max(scan);
std::transform(scan.begin(), scan.end(), scan.begin(),
    [max_val] (LidarPoint p) {
        p.intensity /= max_val;
        return p;
    });

// Filter points below confidence threshold before object detection
auto low_conf = std::remove_if(scan.begin(), scan.end(),
    [] (const LidarPoint& p) { return p.intensity < 0.1f; });
scan.erase(low_conf, scan.end());

// Sort by distance for nearest-neighbour queries
std::sort(scan.begin(), scan.end(),
    [] (const LidarPoint& a, const LidarPoint& b) {
        return (a.x*a.x + a.y*a.y) < (b.x*b.x + b.y*b.y);
    });
```

C++17 adds `std::execution::par_unseq` — swap it in before `begin()` and the same lambda runs in parallel across CPU cores with no other changes. This is why the algorithm + lambda pattern is preferred over hand-written loops in performance-critical pipelines.