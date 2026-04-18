# Quiz — Templates

---

## Warm-Up (From Day 7 — Review)

**QR1:** The Rule of Five says: if you define any one of destructor, copy constructor, copy assignment, move constructor, or move assignment — you should define all five. Explain *why* this rule exists from first principles. Specifically: what breaks if you define a destructor (to `delete[]` a raw array) but leave copy constructor and copy assignment as compiler-generated defaults?

## Answer QR1
This rule exists because compiler generated defaults do a shallow copy

---

## Question 1

You write this function template:

```cpp
template<typename T>
T sensor_max(T a, T b) { return a > b ? a : b; }
```

A colleague calls it two ways:

```cpp
float x = sensor_max(3.5f, 2.1f);   // compiles fine
float y = sensor_max(3, 2.5f);       // compile error
```

Why does the second call fail? What is the compiler trying to do, and what two ways could your colleague fix it?

## Answer 1
The second one fails because my colleague is passing a mix of int and float, whereas the function expects the same type for both, based on the function declaration. 
He could 1) cast 3 to float, 3.0f OR 2) add a second type in the function declaration, instead of just `typename T` so then the compiler wouldn't expect the two params to have the same type. I'm not sure how the syntax here would look.


---

## Question 2

You define a class template `RingBuffer<T>` and a colleague tries to use it:

```cpp
RingBuffer buf(10);   // colleague writes this
```

The compiler rejects it. Why? What is the correct syntax, and why does this differ from calling a function template like `sensor_max(3.5f, 2.1f)` where you don't need to write the type explicitly?

## Answer 2
The compiler rejects it because the type isnt specified. Class templates require specification of type, for instance `RingBuffer<int> buf(10)`;


---

## Question 3

`std::vector<bool>` is famously different from `std::vector<int>` — it stores bits instead of one byte per bool, and `operator[]` returns a proxy object, not a real `bool&`. What C++ mechanism makes this possible? Give a one-sentence explanation of how the compiler decides which implementation to use when you write `std::vector<bool> v;`.

## Answer 3



---

## Question 4

You define `template<typename T, int N> class FixedBuffer`. A colleague wants a buffer of 100 floats but writes:

```cpp
int capacity = 100;
FixedBuffer<float, capacity> buf;    // compile error
```

They try again:

```cpp
constexpr int capacity = 100;
FixedBuffer<float, capacity> buf;    // compiles fine
```

Why does the first fail and the second succeed? What is the fundamental requirement for non-type template arguments?

## Answer 4



---

## Question 5

Your colleague says: *"Templates are just a fancy copy-paste — the compiler copies the class definition once for each type you use it with. You're not actually saving any work at runtime."*

Evaluate this claim. Is it accurate? What actually happens in the compiler when you write `RingBuffer<float>` and `RingBuffer<int>` in the same program, and what is the real benefit of templates over literal copy-paste?

## Answer 5



---

## Grade Log
