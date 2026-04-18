# Quiz — Move Semantics

---

## Question 1

A colleague reads your code and says: "I see you wrote `std::move(frame)` — so that's where the move happens, right?" Correct their understanding. What does `std::move` actually do, and where does the actual resource transfer occur?

## Answer 1

std::move is where frame is cast from an lvalue to an xvalue. Only when std::move(frame) is assigned to a new or existing variable does the move actually happen. It happens by using the move constructor of the frame object.

---

## Question 2

You write a `LidarFrame` class with a raw `float* points` array and implement a move constructor:

```cpp
LidarFrame(LidarFrame&& other) {
    points = other.points;
    count  = other.count;
}
```

A colleague reviews it and says something is missing. What is it, why does it matter, and what specific failure will occur at runtime if you ship this as-is?


## Answer 2
I'm assuming this is meant to be a move by construction, not a move assignment operator.
In this case, what's missing is that other.points has not been set to nullptr and count hasn't been set to 0.
Since these steps haven't been implemented, now both this->points and other.points are both pointing to the same memory address where the array is stored. if any one of their destructors runs first the other one is left dangling and this risks a `double free`, i.e. an attempt to destroy a reference to memory that has already been destroyed , which could cause the program to crash.
---

## Question 3

Your teammate writes this factory function:

```cpp
LidarFrame make_frame(int n) {
    LidarFrame f(n);
    return std::move(f);
}
```

They added `std::move` thinking it makes the return more efficient. Does it? Explain whether this helps, has no effect, or actively hurts — and why.

## Answer 3
This would still work, but it actively hurts by increasing one copy operation where it wouldn't have been necessary.
Given that the LidarFrame f is created only to be returned, the NRVO (no return value optimization) principle would have kicked in and f would have been created in the caller's memory slot itself, which meant that a simple return statement wouldn't have required a copy, from the functions' slot to the caller's slot. However, with std::move(f) in place, while the intent might be to remove the requirement for copying the data in f, the actual result is that a single copy operation is still required, that of the reference to f, which wasn't required with NRVO.

---

## Question 4

The Rule of Five states: if you define any one of destructor, copy constructor, copy assignment operator, move constructor, or move assignment operator — you should define all five. Explain *why* this rule exists. What breaks if you define a destructor and a move constructor but leave copy constructor and copy assignment as compiler-generated defaults?

## Answer 4
I am totally guessing here. If you define only one, or some, not all, your intent might not be compatible with the compiler-generated defaults, this will lead to inconsistent behaviour if for example, you define the copy constructor, but the copy assignment is compiler generated. 

---

## Question 5

In Day 6 you learned that `unique_ptr` cannot be copied but can be moved. Now that you understand move semantics at the language level, explain *how* `unique_ptr` enforces this. What specific C++ mechanism prevents copying, and what does the move constructor do internally?

## Answer 5
delete. The copy constructor on unique_ptr has internally been set to delete, thus any attempt to copy throws an error. 
---

## Grade Log

### 2026-04-17 — 4.0 / 5.0

| Q | Score | Note |
|---|-------|------|
| Q1 | 1.0 | Correct: cast lvalue→xvalue, move happens in constructor — complete reasoning |
| Q2 | 1.0 | Correct: both nullification steps identified, double-free mechanism explained cleanly |
| Q3 | 0.75 | Correct that it hurts and NRVO is suppressed; says "copy operation" where it's actually a move — small precision gap |
| Q4 | 0.5 | Admits guessing; gets "inconsistency" intuition but misses the key: compiler-generated copy does shallow copy, so destructor + default copy = double-free |
| Q5 | 0.75 | Correctly identifies `= delete`; doesn't explain move constructor internals (steal raw pointer + nullify); "throws an error" is a compile error, not runtime exception |

