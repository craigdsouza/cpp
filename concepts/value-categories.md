# Concepts: Value Categories

> Recommended reading before Day 8 (Templates). Useful background for Day 7 (Move Semantics) if you want to understand *why* `std::move` produces a `T&&`.

---

## What This Is About

Every expression in C++ belongs to a *value category*. This determines whether the expression can be moved from, whether you can take its address, and which overload the compiler selects. Value categories are why `std::move` works the way it does, why `T&&` in a template means something different from `T&&` in a regular function, and why the compiler sometimes moves automatically and sometimes copies.

---

## The Two Properties That Define Everything

Each expression has two independent boolean properties:

- **Has identity** — does the expression refer to a named, persistent object with a memory address you can take?
- **Can be moved from** — is it safe for the compiler to steal the resources of this expression?

From these two properties, all value categories fall out:

| Category | Has identity | Can be moved from | In plain English |
|----------|-------------|-------------------|------------------|
| **lvalue** | Yes | No | A named variable or reference — it persists |
| **prvalue** | No | Yes | A pure temporary — never had a name |
| **xvalue** | Yes | Yes | A named object explicitly cast as movable |
| **glvalue** | Yes | Either | lvalue or xvalue (generalised lvalue) |
| **rvalue** | Either | Yes | prvalue or xvalue (anything movable) |

You'll mostly reason about three: **lvalue**, **prvalue**, and **xvalue**. The other two (glvalue, rvalue) are umbrella terms.

---

## Intuition for Each

### lvalue — "has a name, lives on"

```cpp
int x = 5;        // x is an lvalue
MapTile tile;     // tile is an lvalue
tile.id;          // member access of an lvalue is an lvalue
```

You can take the address of an lvalue (`&x` works). It persists past the expression. It cannot be moved from automatically — the compiler won't steal from something that still has a name and might be used again.

### prvalue — "pure temporary, never named"

```cpp
5                         // integer literal — prvalue
MapTile(1, 0.0, 0.0, "")  // temporary constructed inline — prvalue
x + 1                     // arithmetic result — prvalue
```

A prvalue has no identity — it's born and consumed in the same expression. You can't take its address. The compiler is free to construct it directly in its final destination (this is what enables copy elision / NRVO).

### xvalue — "has a name but you've declared it expendable"

```cpp
std::move(tile)           // tile is an lvalue; std::move casts it to xvalue (T&&)
std::move(v)[0]           // element of a moved-from vector — xvalue
```

An xvalue has identity (it came from a named object) but has been explicitly marked as safe to steal from. This is exactly what `std::move` does: it casts an lvalue to an xvalue by returning `T&&`. The object still exists at its address — you just told the compiler "I'm done with it, take its resources."

---

## What `std::move` Is, Precisely

```cpp
template<typename T>
std::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}
```

It is a `static_cast` to `T&&`. Nothing more. The expression `std::move(x)` produces an xvalue — an expression that has identity but is marked movable. The actual resource transfer happens in whatever move constructor or move assignment operator gets called on that xvalue.

This is why `std::move` on a return value is an anti-pattern: a named local variable about to be returned is already treated as an xvalue by the compiler (implicit move on return). Calling `std::move` explicitly converts it to an xvalue *before* the return analysis, which prevents NRVO.

---

## Reference Binding Rules

These rules determine which references bind to which value categories:

| Reference type | Binds to |
|---------------|----------|
| `T&` (lvalue ref) | lvalues only |
| `const T&` (const lvalue ref) | lvalues and rvalues (prvalues and xvalues) |
| `T&&` (rvalue ref) | rvalues only (prvalues and xvalues) |

This is why:
- `void f(T&)` — only called with named variables
- `void f(const T&)` — called with anything; the "universal read-only" signature
- `void f(T&&)` — only called with temporaries or `std::move`d objects; the move overload

When both `f(T&)` and `f(T&&)` exist, the compiler picks the best match: lvalues go to `T&`, rvalues go to `T&&`.

---

## `T&&` in Templates Is Different

In a regular function, `T&&` is an rvalue reference:

```cpp
void f(LidarFrame&& frame);  // only accepts rvalues
```

In a template, `T&&` is a *forwarding reference* (also called universal reference):

```cpp
template<typename T>
void f(T&& arg);  // accepts ANYTHING — lvalue or rvalue
```

When `T` is deduced, the compiler applies *reference collapsing*:
- If you pass an lvalue of type `X`, `T` deduces as `X&`, and `T&&` becomes `X& &&` → collapses to `X&`
- If you pass an rvalue of type `X`, `T` deduces as `X`, and `T&&` becomes `X&&`

This is the foundation of `std::forward<T>(arg)` — it preserves the value category of the original argument when passing it along. Day 8 will cover this directly.

---

## Common Pitfalls

- **Calling `std::move` on a return value.** You're converting the local variable to an xvalue before the compiler's implicit-move-on-return logic, disabling NRVO. Write `return frame;` not `return std::move(frame);`.
- **Assuming a moved-from object is destroyed.** After a move, the source is in a valid but unspecified state — typically empty/null, but still alive. Its destructor will still run. Always nullify raw pointers in move constructors.
- **Confusing `T&&` in a template with an rvalue reference.** In a template, `T&&` is a forwarding reference that binds to anything. Outside a template, `T&&` only binds to rvalues.
- **Taking the address of an rvalue.** `&std::move(x)` is a compile error. Rvalues (prvalues and xvalues) don't have a stable address you can take.

---

## Connection to NVIDIA DRIVE / Mapping

The DRIVE sensor pipeline passes large objects — lidar frames (millions of floats), camera buffers, map tile payloads — between modules. Value categories are what makes this zero-copy: when a buffer moves from the capture module to the preprocessing module, it's an xvalue transfer. No bytes are copied; only the pointer moves. The receiving module's move constructor steals the pointer and nullifies the source.

At the infrastructure level, `std::forward` (built on forwarding references) is used extensively in DriveWorks-style middleware to pass sensor data through dispatch layers without unnecessary copies. Understanding that `T&&` in a template is not the same as `T&&` in a regular function is essential for reading and writing that kind of code.
