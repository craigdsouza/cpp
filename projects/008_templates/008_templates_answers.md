# Answers — Templates

---

## Question QR1 (From Day 7)

**Compiler-generated copy operations do memberwise shallow copies — so a class with a destructor managing raw memory gets a ticking time-bomb for free.**

When you define a destructor that calls `delete[] points`, you're signalling that your class owns a heap allocation. But if you leave copy constructor and copy assignment as compiler-generated defaults, the compiler generates them as memberwise copies: `copy.points = original.points`. Now two objects share the same heap address. When the first is destroyed, its destructor frees the allocation. When the second is destroyed, its destructor calls `delete[]` on a pointer that no longer owns that memory — a double-free, which is undefined behavior (crash, heap corruption, or silent data corruption).

The Rule of Five exists because defining any one of the five special functions almost always means the class owns a resource — and ownership requires all five to be correct. If you can't easily define all five, use a smart pointer or STL container and the Rule of Zero applies instead (you need to define none of them).

---

## Question 1

**Template type deduction resolves T independently per argument — conflicting deductions cause a compile error.**

For `sensor_max(3, 2.5f)`: the first argument `3` deduces `T = int`; the second argument `2.5f` deduces `T = float`. The compiler can't resolve T to both `int` and `float` simultaneously — it doesn't perform implicit conversions during type deduction. Two fixes:

1. **Explicit template argument:** `sensor_max<float>(3, 2.5f)` — locks T to `float`, then `3` is implicitly converted to `3.0f`.
2. **Cast one argument:** `sensor_max(static_cast<float>(3), 2.5f)` — both arguments now deduce `T = float`.

A third option is to write `template<typename T, typename U>` with two separate type parameters, but then the return type becomes ambiguous.

---

## Question 2

**Class templates require explicit template arguments — the compiler cannot deduce T from nothing.**

`RingBuffer buf(10)` fails because there's no argument for the compiler to deduce T from — `10` is the constructor argument (capacity), not a T. The correct syntax is `RingBuffer<float> buf(10)`.

The difference from `sensor_max(3.5f, 2.1f)`: function templates deduce T from their *function arguments*. `3.5f` is a value of type `float`, so the compiler infers T. A class template's type parameter appears in the *type*, not in a function call — the programmer must say what T is explicitly.

(Note: C++17 introduced **Class Template Argument Deduction (CTAD)**, which can deduce T from constructor arguments if a deduction guide is provided. But this is an advanced feature and requires explicit setup — it's not automatic for user-defined class templates.)

---

## Question 3

**Template specialization — `std::vector<bool>` is a full explicit specialization of the `std::vector<T>` template for `T = bool`.**

The compiler always prefers the most specific match. When you write `std::vector<bool> v;`, the compiler sees both the generic `vector<T>` and the explicit specialization `vector<bool>` — and picks the specialization. The specialization replaces the *entire class implementation* with a different one (bit-packing instead of one byte per element). From the outside the API looks the same; the internals are completely different. This is also why `std::vector<bool>` is considered a design mistake in hindsight — the surprise behavior from a specialization that changes semantics.

---

## Question 4

**Non-type template arguments must be compile-time constants — values the compiler can evaluate before generating any code.**

`int capacity = 100` is a regular variable. Even though its value is obviously 100 at that line, the compiler treats it as a runtime value — it could theoretically be modified before the template instantiation point (though it's obviously not here). Template instantiation happens at compile time, before any runtime code runs, so the compiler refuses.

`constexpr int capacity = 100` tells the compiler: this value is a compile-time constant, guaranteed never to change, evaluate it now. The compiler can substitute `100` into the template parameter directly. `constexpr` is the bridge between runtime variables and compile-time values.

---

## Question 5

**Partially accurate — but misses the key benefit: one source, one point of maintenance, compiler-enforced correctness per type.**

The colleague is right that the compiler generates a separate concrete class or function for each unique instantiation: `RingBuffer<float>` and `RingBuffer<int>` become two separate classes in the compiled binary. This is **template instantiation**, and it can lead to **code bloat** if many types are instantiated.

But the benefits over literal copy-paste are real:

1. **One definition to maintain:** Fix a bug in `RingBuffer<T>::push`, and the fix applies to every instantiation — float, int, double, struct types. With copy-paste, you'd need to fix each separately.
2. **Type safety:** The compiler checks each instantiation independently. If `RingBuffer<MyType>` tries to compare two `MyType` objects and `MyType` has no `operator<`, the compiler catches it.
3. **Zero cost for unused types:** If you write `RingBuffer<T>` but only ever use `RingBuffer<float>`, the compiler generates no code for `RingBuffer<int>`. With copy-paste, the unused code still exists.
4. **The STL is built on this:** Every `std::vector`, `std::map`, `std::unique_ptr` you've used since Day 4 is a template instantiation. The alternative — a separate VectorOfFloat, VectorOfInt, VectorOfLidarFrame — would have been unworkable.

---

