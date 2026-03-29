# Day 2 — References and Pointers

**Goal:** Understand the single most important concept in C++. Everything else builds on this.

## Background

In Python, variables are labels. When you write `x = [1, 2, 3]` and then `y = x`, both `x` and `y` point to the same list. There is no explicit syntax to say "give me a direct alias" vs "give me a copy" — Python decides for you based on the type.

In C++ you make that choice explicitly, every time, with two tools:

- A **reference** (`&`) is a permanent alias to an existing variable. It cannot be null. It cannot be reseated (pointed at something else after creation).
- A **pointer** (`*`) is a variable that holds a memory address. It can be null. It can be reassigned to point at different things.

Both give you access to data without copying it. They differ in flexibility, safety, and intent.

## Exercise 1 — References

Create a new file `references.cpp`.

Write a function called `add_prefix` that takes a `std::string&` (non-const reference) and modifies it in place by prepending the string `"tile_"` to whatever it contains.

```
// After calling add_prefix, a string that held "007" should hold "tile_007"
```

In `main`, create a `std::string` with value `"007"`, call `add_prefix` on it, then print the result. You should see `tile_007` without having returned anything from the function.

**What to observe:** The function modifies the original variable in `main`. No copy was made. No return value was needed.

## Exercise 2 — Const references in functions

Write a second function called `print_tile` that takes a `const std::string&` and prints it.

Call it with your modified string from Exercise 1.

**What to observe:** The `const` tells both the compiler and the reader: "this function will not modify the string." If you try to modify the parameter inside `print_tile`, the compiler will refuse to build. Test this — try adding `tile = "broken";` inside `print_tile` and read the compiler error. Then remove it.

**The rule to internalize:** Pass by `const T&` when the function only needs to read. Pass by `T&` when the function needs to write. Pass by value (`T`) when the type is a primitive (`int`, `double`, `bool`, `char`).

## Exercise 3 — Pointers

Create a new file `pointers.cpp`.

In `main`:

1. Declare an `int` named `tile_id` with value `42`
2. Declare a pointer `int* p` and assign it the address of `tile_id` using the `&` operator: `int* p = &tile_id;`
3. Print the value of `tile_id` directly
4. Print the value *through the pointer* using the dereference operator `*p`
5. Use the pointer to change `tile_id` to `99`: `*p = 99;`
6. Print `tile_id` again — it should now be `99`

**What to observe:** `p` holds a memory address. `*p` reads or writes the value at that address. The variable `tile_id` and the expression `*p` refer to exactly the same 4 bytes of memory.

## Exercise 4 — The nullptr

Add to `pointers.cpp`:

1. Declare a pointer `int* q = nullptr;`
2. Write an `if` statement that checks whether `q` is null before dereferencing it, and prints a message either way
3. Now make `q` point to `tile_id` and re-run the check

**What to observe:** Dereferencing a null pointer is undefined behavior — your program will crash or corrupt memory silently. In production C++ (and especially in safety-critical AV software) every pointer that could be null must be checked before use. References avoid this problem entirely because they can never be null.

## Exercise 5 — Putting it together

Create a function `swap_tiles` that takes two `int` references and swaps their values — without returning anything and without using a temporary variable stored outside the function.

```cpp
int a = 10;
int b = 20;
swap_tiles(a, b);
// a is now 20, b is now 10
```

Write a `main` that demonstrates the swap worked.

## Exercise Results — 2026-03-27

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — References | Pass | `add_prefix` correctly modifies string in place via `std::string&` |
| Exercise 2 — Const references | Pass | `print_tile` uses `const std::string&`; const-violation test confirmed with commented line |
| Exercise 3 — Pointers | Pass | Declares pointer, dereferences, mutates through pointer correctly |
| Exercise 4 — nullptr | Fail | `q` was assigned `&tile_id` immediately — never initialized to `nullptr`, so the null-check branch never executes |
| Exercise 5 — swap_tiles | Pass | Swap works correctly; local temp inside function satisfies the constraint |

---

## Checkpoint

You've passed Day 2 when you can answer these without looking anything up:

- What is the difference between `T`, `T&`, and `T*` in a function parameter?
- Why can't you have a null reference?
- When would you choose a pointer over a reference?
- What does `*p = 5` do vs `p = &x`?
