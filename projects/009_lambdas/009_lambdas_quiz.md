# Quiz — Lambdas + std::algorithm

---

## Warm-Up (From Day 8 — Review)

**QR1:** A colleague writes a `SensorFrame` class that owns a heap-allocated array of floats and defines a destructor that calls `delete[]`. They leave copy constructor and copy assignment as compiler-generated defaults. Explain exactly what breaks — trace the sequence of events step by step that leads to undefined behavior.

## Answer QR1
I understand this only superficially, from memory, not from practice.  If destructor calls delete[] it means that a heap object is being deleted. If copy constructor and copy assignment are compiler generated defaults, these are shallow copies, ie. the copy and original point to the same heap memory address, rather than a deep copy, meaning each gets its own memory allocation. If any one, the copy or original are destroyed, the common memory allocation is freed and if the other is now destroyed, a double free occurs, which means a crash could occur. Thus its important to define all five constructors , this way you can ensure that copy is a deep copy if destructor calls delete[].

---

**QR2:** Your colleague says: *"Templates are just a fancy copy-paste — the compiler duplicates the class definition for each type. You're not actually saving any work at runtime."* Evaluate this claim. What actually happens at compile time, and what are three concrete benefits of templates over literal copy-paste that your colleague is missing?

## Answer QR2
In a way yes, it is simply copy paste, but the advantages over copy paste are real.
1) maintainability - updating or fixing bugs in one template function is easier than maintaining several similar functions
2) no cost for unused types - if a particular type isn't used , with copy-paste the excess code still exists, whereas with templates, if its unused the code isn't generated at all.
3) with several custom types, the number of copy-paste becomes unmanageable.

---

## Question 1

You write this lambda:

```cpp
int threshold = 5;
auto fn = [threshold] (int x) { return x > threshold; };
threshold = 100;
std::cout << fn(10);   // prints 1 (true) or 0 (false)?
```

What does it print, and why? What would you change to make the lambda see the updated value of `threshold`?

## Answer 1
It prints true because the threshold captured is a copy rather than a reference to the original. So changing threshold to 100 doesn't make a difference. The captured value is still 5 and thus it returns true.

---

## Question 2

A colleague writes this to find the most intense lidar point:

```cpp
std::vector<LidarPoint> scan = get_scan();
auto it = std::find_if(scan.begin(), scan.end(),
    [] (const LidarPoint& p) { return p.intensity > 0.9f; });
std::cout << it->intensity;
```

Identify the bug. What is the correct fix, and why is this the same pattern you used with `std::map::find` on Day 4?

## Answer 2
I don't remember map from Day 4, but here, I believe the issue is that a check is required to see whether any valid value is returned or not. this can be done with `if (it != scan.end()) { std::cout << it->intensity_ << std::endl; }`

---

## Question 3

Explain the difference between these three captures for a lambda used inside a function:

```cpp
float max_val = compute_max(scan);

auto fn1 = [=] (LidarPoint p) { return p.intensity / max_val; };
auto fn2 = [&] (LidarPoint p) { return p.intensity / max_val; };
auto fn3 = [max_val] (LidarPoint p) { return p.intensity / max_val; };
```

For normalizing a scan, which is safest and why? Under what circumstance would `fn2` produce a dangling reference?

## Answer 3
fn1 captures all variables in scope, by value. this is excessive, since values ofmany variables might be made, whereas only the values needed for the function should be copied.
fn2 captures all variables in scope by reference. this is excessive too, and dangerous since multiple variables can now potentially be altered accidentally using the reference. 
The last, fn3 is safest, because it minimizes the variables captured and ensures that the one needed is available.

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
because we should use simply > instead of >= , If the sequence contains values of the same magnitude this might not work.

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
its safe , because the input and output vectors are the same, they're thus the same length, which is the condition required. If the output iterator were a different length it would lead to a compile error.

---

## Grade Log

## 2026-04-21 — Day 9

**Total: 5.5 / 7.0**

| Q | Score | Note |
|---|-------|------|
| QR1 | 1.0 | Full chain finally nailed: shallow copy → shared pointer → double-free → crash |
| QR2 | 0.75 | Propagation + zero-cost-unused correct; missing type safety per instantiation |
| Q1 | 1.0 | Correct answer + complete reasoning; [&threshold] fix named correctly |
| Q2 | 0.75 | Bug and fix correct; didn't connect to Day 4 map sentinel pattern |
| Q3 | 0.75 | fn3 correctly chosen; dangling ref risk described as accidental mutation, not lambda outliving scope |
| Q4 | 0.75 | Correct fix; missing "strict weak ordering" term and why comp(x,x)=true is the contradiction |
| Q5 | 0.5 | In-place safety correct; shorter vector said "compile error" — it's undefined behavior (buffer overflow) |
