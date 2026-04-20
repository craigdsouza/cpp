# Quiz — Lambdas + std::algorithm

---

## Warm-Up (From Day 8 — Review)

**QR1:** A colleague writes a `SensorFrame` class that owns a heap-allocated array of floats and defines a destructor that calls `delete[]`. They leave copy constructor and copy assignment as compiler-generated defaults. Explain exactly what breaks — trace the sequence of events step by step that leads to undefined behavior.

## Answer QR1

---

**QR2:** Your colleague says: *"Templates are just a fancy copy-paste — the compiler duplicates the class definition for each type. You're not actually saving any work at runtime."* Evaluate this claim. What actually happens at compile time, and what are three concrete benefits of templates over literal copy-paste that your colleague is missing?

## Answer QR2

---

## Question 1

You write this lambda:

```cpp
int threshold = 5;
auto fn = [threshold](int x) { return x > threshold; };
threshold = 100;
std::cout << fn(10);   // prints 1 (true) or 0 (false)?
```

What does it print, and why? What would you change to make the lambda see the updated value of `threshold`?

## Answer 1

---

## Question 2

A colleague writes this to find the most intense lidar point:

```cpp
std::vector<LidarPoint> scan = get_scan();
auto it = std::find_if(scan.begin(), scan.end(),
    [](const LidarPoint& p) { return p.intensity > 0.9f; });
std::cout << it->intensity;
```

Identify the bug. What is the correct fix, and why is this the same pattern you used with `std::map::find` on Day 4?

## Answer 2

---

## Question 3

Explain the difference between these three captures for a lambda used inside a function:

```cpp
float max_val = compute_max(scan);

auto fn1 = [=](LidarPoint p) { return p.intensity / max_val; };
auto fn2 = [&](LidarPoint p) { return p.intensity / max_val; };
auto fn3 = [max_val](LidarPoint p) { return p.intensity / max_val; };
```

For normalizing a scan, which is safest and why? Under what circumstance would `fn2` produce a dangling reference?

## Answer 3

---

## Question 4

`std::sort` requires a comparator that returns `true` if the first argument should come *before* the second. A colleague writes this comparator to sort lidar points by intensity descending:

```cpp
std::sort(scan.begin(), scan.end(),
    [](const LidarPoint& a, const LidarPoint& b) {
        return a.intensity >= b.intensity;
    });
```

This compiles but has undefined behavior. Why? What is the correct fix?

## Answer 4

---

## Question 5

In Python, `map(fn, list)` returns a lazy iterator. In C++, `std::transform` writes results to an output range immediately. Given this difference, explain what happens when you call:

```cpp
std::vector<float> intensities = {0.2f, 0.8f, 1.4f};
std::transform(intensities.begin(), intensities.end(),
               intensities.begin(),    // output = same vector
               [](float x) { return x * 0.5f; });
```

Is in-place transform safe here? Why or why not? What would break if the output iterator pointed to a *shorter* vector?

## Answer 5

---

## Grade Log
