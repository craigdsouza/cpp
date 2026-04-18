# Concepts: Constructors and Special Member Functions

> Useful background for Day 7 (Move Semantics) and Day 8 (Templates). Read this when the syntax of constructors, destructors, or operator overloads feels unclear.

---

## What This Is About

Every time an object is created, copied, moved, or destroyed in C++++, a **special member function** fires. These are not called by name — the compiler selects and calls them automatically based on context. Understanding which one fires when, and why, is essential for writing correct C++++ code.

The six special member functions are:


| Function                  | When it fires                                                      |
| ------------------------- | ------------------------------------------------------------------ |
| Default constructor       | `ClassName obj;` — object created with no arguments                |
| Parameterized constructor | `ClassName obj(x, y);` — object created with arguments             |
| Destructor                | Object goes out of scope or is deleted                             |
| Copy constructor          | `ClassName b = a;` — new object initialized from existing one      |
| Move constructor          | `ClassName b = std::move(a);` — new object initialized by stealing |
| Copy assignment           | `b = a;` — existing object overwritten by copying                  |
| Move assignment           | `b = std::move(a);` — existing object overwritten by stealing      |


---

## The Running Example

All examples below use `LidarFrame` from Day 7 Exercise 3 — a class that owns a heap-allocated `float` array:

```cpp
class LidarFrame {
    float* points;  // pointer to heap array
    int count;      // number of floats

public:
    // ... special member functions go here
};
```

---

## 1. Default Constructor

**Fires when:** object created with no arguments.

```cpp
LidarFrame a;         // default constructor
PipeLineStage stage1; // default constructor — vector initializes itself to empty
```

**Syntax:**

```cpp
ClassName() {
    // initialize members to safe defaults
}
```

**For `LidarFrame`**, a default constructor would set `points = nullptr` and `count = 0` — a valid empty state that the destructor can safely handle.

**For `PipeLineStage`** (Day 7 Exercise 5), the body is blank — `std::vector` initializes itself automatically:

```cpp
PipeLineStage() {}
```

---

## 2. Parameterized Constructor

**Fires when:** object created with arguments.

```cpp
LidarFrame a(1024);        // allocate 1024 floats
SensorBuffer s("lidar", 256); // name and size
```

**Syntax — with initializer list (preferred):**

```cpp
LidarFrame(int count) : count(count) {
    points = new float[count];
    for (int i = 0; i < count; i++) points[i] = 0.0f;
    std::cout << "LidarFrame constructed (count=" << count << ")" << std::endl;
}
```

The `: count(count)` part is the **initializer list** — it initializes members before the body runs. The left `count` is the member, the right `count` is the parameter. It runs before the `{` body.

**Why use initializer lists?** More efficient than assigning inside the body — the member is constructed directly rather than default-constructed and then overwritten.

---

## 3. Destructor

**Fires when:** object goes out of scope (stack unwinds) or is explicitly deleted (heap).

```cpp
{
    LidarFrame a(1024);
}  // destructor fires here — a goes out of scope
```

**Syntax:**

```cpp
~LidarFrame() {
    delete[] points;  // free heap memory
    std::cout << "LidarFrame destroyed" << std::endl;
}
```

**Rules:**

- No parameters, no return type
- Only one destructor per class
- `delete[] nullptr` is safe — a no-op. This is why nullifying pointers after a move is essential: the moved-from object's destructor still runs, and it must not free memory that was stolen.

**Python analogy:** Python's `__del__` — but called deterministically when the variable goes out of scope, not whenever the GC decides.

---

## 4. Copy Constructor

**Fires when:** a new object is initialized from an existing one.

```cpp
LidarFrame a(1024);
LidarFrame b = a;      // copy constructor
LidarFrame c(a);       // also copy constructor

validateBuffer(a);     // copy constructor fires — a is copied into the parameter
```

**Syntax:**

```cpp
LidarFrame(const LidarFrame& other) {
    count = other.count;
    points = new float[count];          // allocate fresh array for this object
    for (int i = 0; i < count; i++) {
        points[i] = other.points[i];    // copy values across
    }
    std::cout << "LidarFrame copied" << std::endl;
}
```

**Key rule — deep copy:** You must allocate a new array and copy the values. Just copying the pointer (`points = other.points`) is a shallow copy — both objects would point to the same heap array, and when either destructor runs, the other is left with a dangling pointer (double-free crash).

```
Deep copy:                          Shallow copy (WRONG):
a.points → [0.0, 0.0, 0.0, 0.0]   a.points ──→ [0.0, 0.0, 0.0, 0.0]
b.points → [0.0, 0.0, 0.0, 0.0]               ↗
(separate arrays, safe)             b.points ──/
                                    (shared array — double-free when either destructs)
```

---

## 5. Move Constructor

**Fires when:** a new object is initialized by stealing from an existing rvalue.

```cpp
LidarFrame a(1024);
LidarFrame b = std::move(a);   // move constructor — b steals a's array
LidarFrame c = factory_nrvo(); // move constructor (or NRVO — see value-categories.md)
```

**Syntax:**

```cpp
LidarFrame(LidarFrame&& other) {
    points = other.points;      // steal the pointer
    other.points = nullptr;     // nullify source — essential for safe destruction
    count = other.count;        // steal the count
    other.count = 0;            // nullify source count
    std::cout << "LidarFrame moved" << std::endl;
}
```

**The steal-and-nullify pattern:**

1. Copy the pointer from `other` to `this` — no heap data moves
2. Set `other.points = nullptr` — so `other`'s destructor calls `delete[] nullptr` (safe no-op)

**Why `&&`?** The parameter type `LidarFrame&&` is an rvalue reference — it only binds to rvalues (temporaries or `std::move`d objects). This prevents the move constructor from firing accidentally on lvalues.

**The naming trap:** Inside the move constructor, `other` has a name — so it's an lvalue. To move `other`'s members (if they're non-trivial types like `std::string` or `std::vector`) you need `std::move(other.member)`. For raw pointers, simple assignment suffices.

**NVIDIA relevance:** Moving a `LidarFrame` with 1 million floats costs 8 bytes (the pointer) + 4 bytes (the count) = 12 bytes. Copying costs 4MB. This is why move semantics exists.

---

## 6. Copy Assignment Operator

**Fires when:** an already-existing object is overwritten by copying from another.

```cpp
LidarFrame a(1024);
LidarFrame b(512);
b = a;              // copy assignment — b already exists, gets overwritten
```

**Syntax:**

```cpp
LidarFrame& operator=(const LidarFrame& other) {
    if (this == &other) return *this;  // guard against self-assignment

    delete[] points;                   // free b's existing array

    count = other.count;
    points = new float[count];         // allocate fresh array
    for (int i = 0; i < count; i++) {
        points[i] = other.points[i];
    }
    return *this;                      // return reference to this object
}
```

**Key difference from copy constructor:** The object already exists and owns resources. You must free them before overwriting.

`**return *this`:** `this` is a pointer to the current object. `*this` dereferences it to get the object, which is returned as a `LidarFrame&`. This enables chaining: `a = b = c`.

---

## 7. Move Assignment Operator

**Fires when:** an already-existing object is overwritten by stealing from an rvalue.

```cpp
LidarFrame a(1024);
LidarFrame b(512);
b = std::move(a);   // move assignment — b already exists, steals a's resources
```

**Syntax:**

```cpp
LidarFrame& operator=(LidarFrame&& other) {
    if (this == &other) return *this;  // guard: b = std::move(b) would self-destruct

    delete[] points;                   // free b's existing array

    points = other.points;             // steal
    count = other.count;
    other.points = nullptr;            // nullify source
    other.count = 0;
    return *this;
}
```

**Why the self-assignment guard?** `b = std::move(b)` would: free `b`'s array, then try to steal from `b` — reading a freed pointer. The guard `if (this == &other)` catches this: `this` is `b`'s address, `&other` is also `b`'s address — same object, skip everything.

**Key difference from move constructor:** The object already exists and owns a heap array. Free it first, then steal.

---

## 8. Deleted Constructors

Sometimes you want to explicitly forbid copying or moving. In Day 7 Exercise 5, `PipeLineStage` must never be copied — two stages owning the same frames would cause double-processing and double-free:

```cpp
class PipeLineStage {
public:
    PipeLineStage(const PipeLineStage&) = delete;             // no copy constructor
    PipeLineStage& operator=(const PipeLineStage&) = delete;  // no copy assignment

    PipeLineStage(PipeLineStage&& other) { ... }              // move is allowed
    PipeLineStage& operator=(PipeLineStage&& other) { ... }   // move assignment allowed
};
```

Attempting to copy will produce a compile error — the bug is caught before the program runs.

`**unique_ptr` (Day 6)** uses the same pattern: copy is deleted, move is allowed. This enforces single ownership — exactly what a pipeline stage needs.

---

## When Each Fires — Quick Reference

```cpp
LidarFrame a(1024);          // parameterized constructor
LidarFrame b;                // default constructor
LidarFrame c = a;            // copy constructor   (c is new)
LidarFrame d = std::move(a); // move constructor   (d is new, a nullified)
b = c;                       // copy assignment    (b already existed)
b = std::move(d);            // move assignment    (b already existed, d nullified)
                             // destructors fire for a, b, c, d as they go out of scope
```

---

## Connection to NVIDIA DRIVE

A `PipelineStage` in the DRIVE stack owns its current batch of `LidarFrame` objects. When the stage finishes processing:

```cpp
// capture stage hands off to preprocessing stage
preprocessing = std::move(capture);
// move assignment fires:
//   preprocessing's old frames freed
//   capture's frames vector pointer stolen (O(1), no data copied)
//   capture.frames is now empty
```

One pointer swap. Zero bytes of lidar data copied. The move assignment operator is what makes zero-copy pipeline handoffs possible.