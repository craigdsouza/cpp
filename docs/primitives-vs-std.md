# Primitive Types vs `std` Namespace Types in C++

## Primitive (Built-in) Types

These are baked directly into the C++ language itself — the compiler understands them without any includes:

- `int`, `float`, `double`, `char`, `bool`, `long`, etc.

They map directly to raw memory representations (e.g., `int` is typically 4 bytes on your CPU). No library needed.

## `std` Namespace Types

These are **classes** defined in the C++ Standard Library — they don't exist until you `#include` the right header:

| Type | Header needed | What it actually is |
|---|---|---|
| `std::string` | `<string>` | A class managing a heap-allocated char array |
| `std::vector<T>` | `<vector>` | A class managing a dynamic array |
| `std::map<K,V>` | `<map>` | A class implementing a red-black tree |

`std::` is the **namespace** — it's just a prefix to avoid name collisions. It tells the compiler "look for `string` inside the standard library's namespace."

## Why `std::string` and not just `string`?

`string` is not primitive because:
1. It has **methods** (`.size()`, `.substr()`, etc.)
2. It manages **heap memory** automatically
3. It was **designed later** and added to the library, not the language core

## The Short Version

- **Primitive** = the language itself knows it, no include needed
- **`std::` types** = library code someone wrote, you must include it

This is also why `std::vector<int>` works — `int` is the primitive element type, and `vector` is the library wrapper that manages a collection of them.
