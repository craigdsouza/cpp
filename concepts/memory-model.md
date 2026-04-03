# The Memory Model — How a Running Program Uses Memory

Read this before Day 6. Everything about smart pointers, RAII, and ownership makes more sense with this picture in your head.

---

## The four regions of a running program

When your C++ program runs, the OS gives it a block of memory divided into four regions:

```
High addresses
┌─────────────────┐
│      Stack      │  ← grows downward
├─────────────────┤
│                 │
│   (free space)  │
│                 │
├─────────────────┤
│      Heap       │  ← grows upward
├─────────────────┤
│  Global / BSS   │  ← static and global variables
├─────────────────┤
│      Code       │  ← your compiled instructions
└─────────────────┘
Low addresses
```

You interact with two of these constantly: the **stack** and the **heap**.

---

## The Stack

The stack is where local variables live. Every time you call a function, the program carves out a chunk of stack memory called a **stack frame** — it holds that function's local variables, its parameters, and the return address (where to go when the function finishes).

```cpp
void process_frame() {
    SensorHandle lidar;   // lives on the stack, inside this frame
    int count = 0;        // also on the stack
}
// frame is popped here — lidar and count are gone
```

When `process_frame` returns, its entire frame is discarded in one operation — the stack pointer just moves back. This is why stack allocation is **instant**: no searching, no bookkeeping, just move a pointer.

**The call stack** is the sequence of active frames stacked on top of each other:

```
┌──────────────────┐  ← top of stack (currently executing)
│  process_frame   │    lidar, count
├──────────────────┤
│      main        │    argc, argv
└──────────────────┘
```

When `process_frame` throws an exception, C++ walks down this stack, running each frame's destructors as it goes — that's **stack unwinding**. This is why destructors fire automatically: stack frame cleanup is baked into the language.

**Limits of the stack:**
- Fixed size (typically 1–8 MB depending on OS)
- Variables must have a size known at compile time
- Lifetime is tied to the function — you can't return a pointer to a local variable

---

## The Heap

The heap is a large pool of memory managed at runtime. Unlike the stack, nothing is automatically allocated or freed — you ask for memory explicitly and return it explicitly.

```cpp
int* data = new int[1000];   // asks the OS for space for 1000 ints
// ... use data ...
delete[] data;               // returns it
```

Under the hood, `new` calls a memory allocator (like `malloc`) which searches the heap for a free block of the right size, marks it as used, and returns a pointer. This is slower than stack allocation — it requires bookkeeping.

**What a pointer actually is:** just an integer that stores a memory address. `data` above is a variable on the *stack* that holds the *address* of the heap allocation. The data lives on the heap; the pointer to it lives on the stack.

```
Stack                     Heap
┌──────────┐              ┌──────────────────────┐
│  data    │─────────────▶│  [int][int][int]...  │
│ (address)│              └──────────────────────┘
└──────────┘
```

When `process_frame` returns, `data` (the pointer on the stack) is destroyed — but the heap allocation it pointed to is **not**. That memory just sits there, unreachable, until the process exits. That's a memory leak.

---

## Stack vs Heap — the practical tradeoffs

|                          | Stack                    | Heap                     |
|--------------------------|--------------------------|--------------------------|
| Speed                    | Instant (move a pointer) | Slower (allocator search)|
| Size                     | Small (~1–8 MB)          | Large (GBs, limited by RAM) |
| Lifetime                 | Tied to scope            | Until you call `delete`  |
| Size known at compile time? | Required              | Not required             |
| Cleanup                  | Automatic (scope exit)   | Manual (`delete`)        |

---

## Why RAII bridges the gap

The core tension: heap memory is necessary (large buffers, runtime sizes, shared ownership) but its cleanup is manual and error-prone. Stack cleanup is automatic but limited.

RAII's trick: **put a stack object in charge of a heap allocation**.

```cpp
class TileBuffer {
    int* data;
public:
    TileBuffer(int n) {
        data = new int[n];   // heap: manual allocation
    }
    ~TileBuffer() {
        delete[] data;       // heap: freed via destructor
    }
};

void tick() {
    TileBuffer buf(1000);    // buf lives on the stack
}                            // buf's destructor fires → heap freed automatically
```

`buf` is on the stack so its destructor fires automatically on scope exit. The destructor calls `delete[]` on the heap. You get heap memory with stack-like cleanup guarantees.

---

## Why smart pointers exist (preview for Day 6)

Writing `delete[]` in every destructor is still error-prone — and what if an object needs to be shared between two owners? `std::unique_ptr` and `std::shared_ptr` are pre-built RAII wrappers around heap allocations so you never write `new`/`delete` manually at all. They handle the destructor for you.

With that picture in mind, Day 6 should make immediate sense.
