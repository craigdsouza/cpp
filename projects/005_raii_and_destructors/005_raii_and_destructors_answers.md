# Answers — RAII and Destructors

---

## Warm-Up QR1 (From Day 4)

**`std::vector<int>` — a vector of tile IDs.**

The key is that "ordered" means two different things:

- `std::vector` is **sequence-ordered**: it preserves exactly the order you insert. `{3, 7, 3, 12}` stays `{3, 7, 3, 12}`. It allows duplicates. It models *sequence*.
- `std::map` is **sort-ordered**: it always keeps keys in ascending order, and keys must be unique. `{3, 7, 3, 12}` would become `{3, 7, 12}` — the duplicate is dropped and the sequence is destroyed.

`std::map` fails for a route on two counts: it cannot store the same tile ID twice (a route that loops is impossible to represent), and it would reorder the IDs by key (destroying the traversal sequence). `std::vector` is the right tool because a route is a *sequence*, not a *lookup table*.

---

## Question 1

**Yes — the LiDAR sensor is released, even when the exception is thrown.**

When an exception propagates out of a scope, C++ guarantees that all stack objects in that scope have their destructors called before the exception continues unwinding. This is called **stack unwinding**. `SensorHandle lidar` is a stack object, so its destructor fires when the exception exits `process_frame()` — regardless of whether the exit was via `return` or `throw`.

In C (no destructors), you'd have to write:

```c
SensorHandle lidar = acquire_lidar();
SensorData data = read(lidar);
if (data_empty(data)) {
    release_lidar(lidar);  // must remember this on every exit path
    // handle error...
}
release_lidar(lidar);      // and again on the normal path
```

Every new exit path requires a new `release_lidar()` call. Miss one and you have a resource leak. RAII removes this requirement entirely — there is one cleanup site (the destructor) and it fires on every exit path automatically.

---

## Question 2

**Destructors fire in reverse construction order: `c`, then `b`, then `a`.**

Construction order is a → b → c. Destruction is c → b → a. This mirrors how a call stack unwinds: the last frame pushed is the first popped (LIFO — Last In, First Out). C++ applies the same principle to local objects within a scope.

The rationale: `b` may depend on `a` (e.g., `b` was initialised using a reference to `a`). Destroying `a` before `b` could leave `b` with a dangling reference. Reverse order preserves the dependency chain safely.

---

## Question 3

**Every call to `localization_tick()` leaks 50 `MapTile`-sized heap allocations.**

`new MapTile[n]` allocates memory on the heap. Without a destructor calling `delete[]`, that memory is never returned to the OS. At 100Hz, after 60 seconds: 100 × 60 = 6,000 calls × 50 tiles = **300,000 leaked `MapTile` objects**. On a real DRIVE system with a `MapTile` containing strings and doubles, this would exhaust available memory within minutes and crash the process.

The fix:

```cpp
~TileCache() {
    delete[] tiles;
}
```

One destructor. One line. The leak is closed on every exit path — normal return, exception, or early termination.

---

## Question 4

**Yes — RAII is strictly more powerful than `with`, for one key reason: it requires no syntax at the call site.**

Python's `with` is opt-in. You must remember to write `with open(...) as f:` instead of `f = open(...)`. If you forget, or if a colleague doesn't know to use `with`, the resource leaks.

C++ RAII is automatic. The moment a `TileLogger` is constructed, its destructor is scheduled to run when it goes out of scope — with no special syntax required. There is no `with`-equivalent keyword. Any code that creates a `TileLogger` gets cleanup for free, whether the author knows about RAII or not.

The second difference: `with` only protects a specific, explicitly delimited block. RAII protects for the object's entire lifetime — which may span multiple function calls, container operations, or even other objects (a class member is destroyed when its owning object is destroyed).

---

## Question 5

**`a` is never opened, `b` throws before completion, `c` is never reached.**

Construction happens in order: `a` → `b` → `c`. If `b`'s constructor throws, `b` was never fully constructed — so `b`'s destructor does **not** run (there is nothing to destroy). However, `a` was fully constructed before `b` threw, so `a`'s destructor **does** run during stack unwinding. `c` was never constructed at all.

Result: `log_a.txt` is properly closed. `log_b.txt` was never opened (the throw happened inside the constructor). `log_c.txt` was never opened.

The Day 2 connection: this is analogous to dereferencing a pointer without checking for `nullptr` — if a constructor can fail (file doesn't exist, sensor not available), you must handle that at construction time, not assume success. A common pattern is to throw from the constructor on failure, which signals to callers that the object was never valid. The caller can then handle the error rather than receiving a silently broken object.

---
