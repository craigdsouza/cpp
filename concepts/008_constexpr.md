# Concepts: constexpr

> `constexpr` tells the compiler: "this value is known at compile time." It is the bridge between runtime variables and template arguments.

---

## The problem it solves

Template arguments must be compile-time constants. This fails:

```cpp
int cap = 16;
FixedBuffer<float, cap> buf;  // ❌ cap is a runtime variable
```

Even though `cap` never changes, the compiler doesn't know that — it's a regular variable that *could* be modified. This also fails:

```cpp
const int cap = 16;
FixedBuffer<float, cap> buf;  // ✅ actually works for simple integer constants
```

`const` on a simple integer initialized with a literal is a special case the compiler handles. But for anything more complex — expressions, function return values — you need `constexpr`:

```cpp
constexpr int cap = 16;
FixedBuffer<float, cap> buf;  // ✅ unambiguously a compile-time constant
```

---

## What constexpr guarantees

`constexpr` makes a promise to the compiler: *this value can be fully computed before the program runs*. The compiler verifies this — if the initializer isn't computable at compile time, it's a compile error, not a silent bug.

```cpp
constexpr int CAP = 8 * 2;        // ✅ arithmetic on literals
constexpr int CAP = some_func();   // ✅ only if some_func is also constexpr
int n = 16;
constexpr int CAP = n;             // ❌ n is a runtime variable
```

---

## constexpr vs const

| | `const` | `constexpr` |
|---|---|---|
| Value can change? | No | No |
| Guaranteed compile-time? | Not always | Yes |
| Usable as template argument? | Sometimes (integer literals only) | Always |
| Applies to functions? | No | Yes |

Use `constexpr` when you need a value in a template argument or array size. Use `const` for runtime values that simply shouldn't be modified.

---

## In the AV context

DriveWorks and similar real-time frameworks define buffer sizes as `constexpr` so they can be used as template arguments across the codebase:

```cpp
constexpr int LIDAR_RING_COUNT   = 64;
constexpr int INTENSITY_BUF_SIZE = 512;

FixedBuffer<float, LIDAR_RING_COUNT>   ring_buf;
FixedBuffer<float, INTENSITY_BUF_SIZE> intensity_buf;
```

This means buffer sizes are named, documented, and verified at compile time — a runtime misconfiguration is impossible.
