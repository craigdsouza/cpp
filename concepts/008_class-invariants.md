# Concepts: Class Invariants

> A class invariant is a condition that must be true before and after every public method call. It's the contract a class makes with its callers: "no matter what sequence of operations you perform, these facts will always hold."

---

## What is a class invariant?

When you design a class, you choose a set of member variables to represent the state. An invariant is a rule about those variables that is always true from the outside — even if individual methods temporarily break it internally while running.

If your invariant ever becomes false after a public method returns, your class has a bug.

---

## RingBuffer\<T\> — a concrete example

`RingBuffer<T>` has four member variables: `data_`, `head_`, `tail_`, `size_`, `capacity_`.

The invariants are:

1. `0 <= size_ <= capacity_` — size is always within bounds
2. `0 <= head_ < capacity_` — head always points to a valid slot
3. `0 <= tail_ < capacity_` — tail always points to a valid slot
4. `data_[head_]` is the oldest element whenever `size_ > 0`
5. `tail_` is the slot where the *next* push will write

These must hold after every `push` and every `pop`. You reason about correctness not by tracing every possible sequence of calls, but by asking: *does this method preserve all five invariants?*

---

## Why this matters in practice

Without invariants, you test by enumeration — push 3, pop 1, push 2, check. With invariants, you reason structurally:

- `push` when not full: `size_` grows by 1, `tail_` advances with `%`, `head_` unchanged → all invariants hold
- `push` when full: `tail_` overwrites oldest, `head_` advances with `%`, `size_` stays at `capacity_` → all invariants hold
- `pop`: oldest returned from `head_`, `head_` advances with `%`, `size_` shrinks by 1 → all invariants hold

Any sequence of pushes and pops is covered by those three cases. You don't need to test the combination — you trust the invariant.

---

## The pattern in professional C++

Senior engineers document invariants explicitly, either in comments or in a private `assert_invariants()` method used during testing:

```cpp
void assert_invariants() const {
    assert(size_ >= 0 && size_ <= capacity_);
    assert(head_ >= 0 && head_ < capacity_);
    assert(tail_ >= 0 && tail_ < capacity_);
}
```

Calling this at the start and end of every public method during debug builds catches violations the moment they happen, rather than when a symptom surfaces later.

---

## Invariants vs. preconditions

An **invariant** is always true (before and after any public call).  
A **precondition** is a requirement on the *caller* before a specific call.

`RingBuffer::pop()` has a precondition: the buffer must be non-empty. That's not an invariant — it's allowed for the buffer to be empty. But if you call `pop()` on an empty buffer, you've violated the precondition, and behaviour is undefined. The exercise states this explicitly: *"assume caller checks first."*
