# Quiz — Move Semantics

---

## Question 1

A colleague reads your code and says: "I see you wrote `std::move(frame)` — so that's where the move happens, right?" Correct their understanding. What does `std::move` actually do, and where does the actual resource transfer occur?

## Answer 1

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

---

## Question 4

The Rule of Five states: if you define any one of destructor, copy constructor, copy assignment operator, move constructor, or move assignment operator — you should define all five. Explain *why* this rule exists. What breaks if you define a destructor and a move constructor but leave copy constructor and copy assignment as compiler-generated defaults?

## Answer 4

---

## Question 5

In Day 6 you learned that `unique_ptr` cannot be copied but can be moved. Now that you understand move semantics at the language level, explain *how* `unique_ptr` enforces this. What specific C++ mechanism prevents copying, and what does the move constructor do internally?

## Answer 5

---

## Grade Log
