# Answers — Lambdas + std::algorithm

---

## Question QR1 (From Day 8)

**The sequence: shallow copy → shared pointer → first destructor frees → second destructor double-frees → undefined behavior.**

1. Compiler-generated copy constructor does memberwise copy: `copy.data_ = original.data_`. Now two `SensorFrame` objects point to the same heap address.
2. When the first `SensorFrame` is destroyed (e.g. goes out of scope), its destructor calls `delete[] data_`. The memory is freed.
3. When the second `SensorFrame` is destroyed, its destructor calls `delete[]` on the same pointer — which no longer owns valid memory. This is a **double-free** — undefined behavior that manifests as a crash, heap corruption, or silent data corruption depending on the allocator.

The Rule of Five says: if you define *any* one of the five special functions, the others must also be defined — because owning a resource (a destructor exists to manage it) means copy and move must be ownership-aware too. The fix is either define all five, or replace the raw array with `std::vector<float>` and define none (Rule of Zero).

---

## Question QR2 (From Day 8)

**Partially accurate — but misses the real value: one source, propagated fixes, and type safety.**

The colleague is right that the compiler generates a separate concrete class for each instantiation (`RingBuffer<float>`, `RingBuffer<int>` are two distinct types in the binary). This is **template instantiation** and can cause **code bloat** if many types are instantiated.

Three concrete benefits over copy-paste:

1. **One fix propagates everywhere.** Fix a bug in `RingBuffer<T>::push` and every instantiation — float, int, double, struct — gets the fix automatically. With copy-paste you'd need to track down and fix each separately, and inevitably miss one.
2. **Type safety per instantiation.** The compiler checks each instantiation independently. `RingBuffer<MyType>` won't compile if your push logic tries an operation `MyType` doesn't support — you catch the error at compile time, not at runtime.
3. **Zero cost for unused types.** If you define `RingBuffer<T>` but only ever use `RingBuffer<float>`, no code is generated for `RingBuffer<int>`. Literal copy-paste leaves dead code in the binary.

The entire STL (`std::vector`, `std::map`, `std::unique_ptr`) is templates. The alternative — `VectorOfFloat`, `VectorOfInt`, `VectorOfLidarFrame` — would be unworkable.

---

## Question 1

**Prints `1` (true). Capture by value takes a snapshot at lambda creation time.**

When `[threshold]` is used, the lambda captures the value of `threshold` at the moment the lambda is created — which is `5`. Changing `threshold = 100` afterward has no effect on the lambda's internal copy. So `fn(10)` evaluates `10 > 5` → `true` → prints `1`.

To make the lambda see the updated value, capture by reference:
```cpp
auto fn = [&threshold](int x) { return x > threshold; };
```
Now the lambda holds a reference to `threshold`, and `fn(10)` after `threshold = 100` evaluates `10 > 100` → `false` → prints `0`.

---

## Question 2

**Bug: no check against `end()` before dereferencing the iterator.**

`std::find_if` returns `scan.end()` when no element matches. Dereferencing `end()` is undefined behavior — same as dereferencing a null pointer.

Correct fix:
```cpp
auto it = std::find_if(scan.begin(), scan.end(),
    [](const LidarPoint& p) { return p.intensity > 0.9f; });
if (it != scan.end()) {
    std::cout << it->intensity;
} else {
    std::cout << "No point found\n";
}
```

This is identical to the `std::map::find` pattern from Day 4: `find` returns an iterator, `end()` is the sentinel meaning "not found", and you must check before using. The iterator abstraction is the same across the entire STL.

---

## Question 3

**`fn3` (`[max_val]`) is safest for normalization. `fn2` (`[&]`) can dangle if the lambda outlives the local scope.**

- `fn1` (`[=]`): captures *everything* in scope by value — correct but captures more than needed; fine here.
- `fn2` (`[&]`): captures everything by reference — if `max_val` goes out of scope before the lambda runs (e.g. lambda is stored and called later), the reference dangles and dereferencing it is undefined behavior. For in-place `std::transform` called immediately, it's safe in practice but fragile.
- `fn3` (`[max_val]`): captures only `max_val` by value — minimal, explicit, and safe regardless of when the lambda runs. This is the best choice for normalization: you want the fixed maximum at the time of capture, not a live reference that could change.

For normalization, `fn3` is safest: you want the max computed once, frozen, and used for every element.

---

## Question 4

**Bug: using `>=` instead of `>` violates the strict weak ordering requirement.**

`std::sort` requires the comparator to be a **strict weak ordering**: it must return `false` when both arguments are equal (`comp(x, x)` must be `false`). With `>=`, `comp(a, a)` returns `true` — the sort algorithm assumes `a` comes before itself, which creates a logical contradiction that leads to undefined behavior (infinite loops or memory corruption in some implementations).

Correct fix — use `>` for descending:
```cpp
std::sort(scan.begin(), scan.end(),
    [](const LidarPoint& a, const LidarPoint& b) {
        return a.intensity > b.intensity;   // strict: false when equal
    });
```

The rule: comparators for `std::sort` must use `<` or `>`, never `<=` or `>=`.

---

## Question 5

**In-place transform is safe here because the input and output ranges are identical and the operation is element-wise with no dependencies between elements.**

`std::transform` processes elements one at a time left-to-right. For in-place operation (`output = begin`), it reads `intensities[i]`, computes the result, and writes it back to `intensities[i]` before moving to `i+1`. Since each output element depends only on the corresponding input element (no `output[i]` depends on `input[i+1]`), there's no issue.

If the output iterator pointed to a *shorter* vector, `std::transform` would write past the end of the output vector — undefined behavior (buffer overflow). `std::transform` does not resize the output; it assumes the output range is at least as long as the input range. Always ensure `output.size() >= input.size()` before calling, or use `std::back_inserter(output)` if you want the output vector to grow.

---
