# Answers — Move Semantics

---

## Question 1

**`std::move` is a cast, not a transfer. The actual move happens in the move constructor or move assignment operator.**

`std::move(x)` unconditionally casts `x` to an rvalue reference (`T&&`). That's all it does — it changes the type category of the expression so the compiler selects the move overload instead of the copy overload when constructing or assigning. The resource transfer (stealing the pointer, nullifying the source) happens inside the move constructor or move assignment operator that gets invoked as a result of that cast. If no move constructor exists, the compiler falls back to the copy constructor. The name `std::move` is arguably misleading — `std::make_movable` or `std::rvalue_cast` would be more accurate.

---

## Question 2

**Missing: `other.points = nullptr; other.count = 0;` — nullifying the source.**

After `points = other.points`, both `this->points` and `other.points` point to the same heap allocation. When `other` is eventually destroyed, its destructor calls `delete[] points`. If `other.points` is still non-null, it frees memory that `this` now owns — a double-free, which is undefined behavior (typically a crash or silent heap corruption). Setting `other.points = nullptr` makes `other`'s destructor call `delete[] nullptr`, which is a safe no-op. This "steal and nullify" pattern is the invariant every move constructor must preserve: after the move, the source is in a valid but empty state, and its destructor is harmless.

---

## Question 3

**It hurts — `std::move` on a return value disables NRVO.**

When a function returns a named local variable by value, the compiler can apply Named Return Value Optimization (NRVO): it constructs the object directly in the caller's memory slot, eliminating any copy or move entirely. This is a zero-cost return. By writing `return std::move(f)`, the programmer converts `f` to an rvalue before the return. This prevents the compiler from recognising it as a NRVO candidate, so NRVO is suppressed. The result is that a move *does* happen — which is better than a copy, but worse than the zero-cost NRVO that would have happened without `std::move`. The rule: never `std::move` a local variable in a return statement. Let the compiler decide.

---

## Question 4

**Because compiler-generated copy operations do shallow copies, which become dangerous the moment you have a destructor managing a raw resource.**

If you define a destructor (say, to `delete[] points`) but leave copy constructor and copy assignment as compiler-generated defaults, the compiler generates them as *memberwise shallow copies*: `copy.points = original.points`. Now two objects share the same pointer. When both are destroyed, both call `delete[]` on the same address — a double-free. The Rule of Five exists because the need to customize *any one* of the five special functions almost always signals that the class is managing a raw resource — and raw resource management requires all five to be correct. The safe default when in doubt: delete copy, implement move, or switch to smart pointers so the Rule of Five doesn't apply at all.

---

## Question 5

**`unique_ptr` deletes its copy constructor and copy assignment operator, and explicitly implements move constructor and move assignment.**

In the standard library, `unique_ptr` is declared with `unique_ptr(const unique_ptr&) = delete` and `operator=(const unique_ptr&) = delete`. These `= delete` declarations tell the compiler that these operations do not exist — any attempt to copy a `unique_ptr` is a compile error, not a runtime failure. The move constructor is explicitly implemented: it assigns the raw pointer from the source to the destination (`this->ptr = other.ptr`), then sets `other.ptr = nullptr`. This is exactly the steal-and-nullify pattern. The result is that the invariant "exactly one `unique_ptr` owns this allocation at all times" is enforced by the type system at compile time — the compiler will reject any code that would violate it.

---
