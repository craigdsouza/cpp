# Answers — Smart Pointers

---

## Warm-Up QR1 (From Day 4)

**`std::vector<int>` — a vector of tile IDs.**

`std::map` fails on two counts: it cannot store duplicate keys (a route that revisits a tile would silently drop the duplicate), and it reorders entries by key — destroying the traversal sequence. `std::vector` is the right tool because a route is a *sequence*, not a lookup table. The distinction: `vector` is **insertion-ordered** (preserves the order you added elements), while `map` is **sort-ordered** (always keeps keys in ascending order). These are different meanings of "ordered."

---

## Warm-Up QR2 (From Day 5)

**`a` is properly closed. `b` was never fully constructed. `c` was never constructed.**

Construction runs in order: `a` → `b` → `c`. `a` completes successfully — its destructor will run during stack unwinding. `b`'s constructor throws before completing — because the object was never fully constructed, C++ does not call `b`'s destructor (there is nothing to destroy). `c` is never reached. So `log_a.txt` is properly closed; `log_b.txt` was never opened (the throw happened inside the constructor, before `file.open()` succeeded); `log_c.txt` was never opened. The rule: only fully constructed objects have their destructors run.

---

## Question 1

**Two reasons `unique_ptr` is safer than raw `new`/`delete`:**

**1. Exception safety.** If `process(frame)` throws, execution leaves the function without reaching `delete`. The heap allocation leaks. With `unique_ptr`, the destructor fires regardless — scope exit via exception is still scope exit.

**2. No `delete` required.** With raw `new`, you must remember to write `delete` on every exit path. With `unique_ptr`, the cleanup is in the type itself. There is no "forget to delete" — the language enforces it.

A secondary benefit: `make_unique` is atomic — it allocates and constructs in one step, so there is no intermediate state where memory is allocated but the object isn't yet constructed. Raw `new` in complex expressions can leak in edge cases that `make_unique` eliminates.

---

## Question 2

**`unique_ptr` cannot be copied because it models *exclusive ownership* — there can only be one owner.**

If a copy were allowed, two `unique_ptr`s would both believe they owned the same heap object and both would call `delete` on destruction — a double-free, which is undefined behavior. The compiler prevents this at compile time by deleting the copy constructor.

The fix is `std::move`:

```cpp
std::unique_ptr<MapTile> tile2 = std::move(tile);
```

After the move, `tile` is null — it gave up ownership. `tile2` is the new exclusive owner. The heap object was not copied; only the pointer was transferred. This is why it's called a *move*: the resource moves from one owner to another, not duplicated.

---

## Question 3

**No — the `MapTile` is not destroyed after `tile.reset()`.**

Step by step:
1. `make_shared` creates the tile. Reference count = 1 (`tile` owns it).
2. `localization.set_tile(tile)` copies the `shared_ptr`. Reference count = 2.
3. `planning.set_tile(tile)` copies again. Reference count = 3.
4. `tile.reset()` releases the original. Reference count drops to 2.

The tile is still alive — `localization` and `planning` each hold a `shared_ptr` to it. The `MapTile` is destroyed only when **both** of those `shared_ptr`s are released (go out of scope, are reset, or the objects that hold them are destroyed). At that point the count hits 0 and `delete` is called automatically.

This is the same semantics as Python's reference counting — but in C++, the timing is deterministic. You know exactly when the count reaches 0.

---

## Question 4

**Component A (`LidarBuffer`): `unique_ptr`.**

There is exactly one owner — the downstream processor. Ownership is exclusive. When the processor is done, the buffer should be freed immediately. `unique_ptr` expresses this directly: one owner, deterministic destruction, no overhead.

**Component B (`MapTile`): `shared_ptr`.**

Three modules need simultaneous access and none of them owns the tile exclusively. The tile must stay alive as long as any of them is using it, and be destroyed when all of them are done. This is shared ownership — `shared_ptr` with reference counting is the right model.

The broader rule: **start with `unique_ptr` as the default.** Reach for `shared_ptr` only when you genuinely need shared ownership. `shared_ptr` has non-zero overhead (atomic reference count increment/decrement) and can mask ownership design problems if overused.

---

## Question 5

**Two problems:**

1. **Exception leak.** If `process(t)` throws, `delete t` is never reached. The `MapTile` leaks.
2. **Ownership is implicit.** The caller of `make_tile` receives a raw pointer with no indication that they own it and must delete it. A second caller might call `make_tile` again and forget to delete — or delete the same pointer twice.

**Fixed version:**

```cpp
std::unique_ptr<MapTile> make_tile(int id) {
    return std::make_unique<MapTile>(id, 0, 0, "unknown");
}

void run() {
    std::unique_ptr<MapTile> t = make_tile(1);
    process(*t);   // or process(t.get()) if process takes a raw pointer
}  // t destroyed here, even if process threw
```

The factory's return type now communicates ownership explicitly: "caller, you own this." No `delete` is needed in `run`. If `process` throws, `t`'s destructor fires during stack unwinding and the tile is freed. Both problems solved by one type change.

---
