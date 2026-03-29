# Day 3 — Classes and Structs

**Goal:** Model real-world data in C++. This is the foundation for everything that follows — every map tile, sensor reading, and vehicle state in the NVIDIA stack is a struct or class.

## Background

In Python you'd model a map tile as a dict or a dataclass:
```python
@dataclass
class MapTile:
    tile_id: int
    lat: float
    lon: float
    label: str
```

C++ has two nearly identical constructs for this: `struct` and `class`. The only difference is the default access level — `struct` members are `public` by default, `class` members are `private`. In practice, use `struct` for plain data containers (no behaviour, just fields), and `class` when you have private state and methods that enforce invariants.

## Exercise 1 — Define a struct

**File:** Create `projects/003_classes_and_structs/maptile.cpp` (new file).

Define a `struct MapTile` with four fields:
```
int id
double lat
double lon
std::string label
```

In `main`, create two `MapTile` variables, assign values to their fields directly, and print each one.

**What to observe:** Struct fields are accessed with `.` (dot notation), exactly like Python attributes. There's no magic yet — you're just grouping data.

## Exercise 2 — Functions that take structs

**File:** Update `maptile.cpp` (add to existing file).

Write two free functions (outside the struct):

- `void print_tile(const MapTile& tile)` — prints id, lat, lon, label on one line
- `void tag_tile(MapTile& tile, const std::string& tag)` — appends `" [" + tag + "]"` to the tile's label

In `main`, create a tile, tag it, then print it.

**What to observe:** You're applying the Day 2 rules directly — `const MapTile&` for read-only, `MapTile&` for mutation. The struct is just another type.

## Exercise 3 — Constructor

**File:** Update `maptile.cpp` (add to existing file).

Add a constructor to `MapTile` so you can initialise it in one line:

```cpp
MapTile t(42, 37.7749, -122.4194, "San Francisco");
```

Without a constructor, you have to set each field separately. A constructor is a special function with the same name as the struct, no return type, that runs automatically when the object is created.

Update `main` to use the constructor. Both tiles should be created with one-line initialisations.

## Exercise 4 — A vector of structs

**File:** Update `maptile.cpp` (add to existing file).

Create a `std::vector<MapTile>` with at least three tiles (use your constructor). Write a loop that calls `print_tile` on each one. Then write a second loop that tags every tile whose `id` is even with the string `"even"`.

**What to observe:** `std::vector<MapTile>` works identically to `std::vector<int>` — the vector doesn't care what type it holds. And your range-for loop should use `MapTile&` (not `const MapTile&`) in the tagging loop, because you're mutating.

## Exercise 5 — A member function

**File:** Update `maptile.cpp` (add to existing file).

Move `print_tile` from a free function into the struct itself as a member function called `print()`. Call it as `tile.print()`.

A member function declared `const` after its parameter list promises not to modify the object:
```cpp
void print() const { ... }
```

Make `print()` const. Then try to modify a field inside it — read the compiler error, then remove the modification.

**What to observe:** Member functions have automatic access to all fields via an implicit `this` pointer. `const` on a member function is the same contract as `const T&` on a parameter — it tells the compiler and the reader that this operation is read-only.

## Repeat Exercise (from Day 2)

**File:** Create `projects/002_ref_and_pointers/nullptr_check.cpp` (new file).

**Exercise 4 — nullptr (redo)**

In your previous attempt, `q` was immediately pointed at `tile_id` and never started as `nullptr`, so the null-check branch never executed.

Redo this in `pointers.cpp` (or a new `nullptr_check.cpp`):

1. Declare `int* q = nullptr;` — start it null
2. Write an `if/else` that checks `q` before dereferencing, and prints a different message for each case — verify the null branch actually runs
3. Now reassign `q = &tile_id;` and run the check again — verify the non-null branch runs

**What to observe:** The safety pattern in AV code is always: check before dereference. The only way to test the null branch is to actually start with `nullptr`.

---

## Exercise Results — 2026-03-29

| Exercise | Result | Note |
|----------|--------|------|
| Exercise 1 — Define a struct | Pass | Struct with all four fields, values assigned and printed |
| Exercise 2 — Functions that take structs | Pass | `print_tile` (const ref) and `tag_tile` (mutable ref) both correct |
| Exercise 3 — Constructor | Pass | Constructor with `this->` assignments, correct signature |
| Exercise 4 — A vector of structs | Pass | Vector, print loop, and even-tag loop all confirmed working |
| Exercise 5 — A member function | Pass | `print() const` as member function, const-violation test present |
| Repeat Exercise — nullptr | Fail | `nullptr_check.cpp` redeclares `int* q` on line 20 — should be `q = &tile_id` (no type) |

---

## Checkpoint

You've passed Day 3 when you can:

- Explain the difference between `struct` and `class` in C++
- Write a struct with a constructor, member functions, and `const` correctness
- Explain why the tagging loop needs `MapTile&` but the printing loop can use `const MapTile&`
- Answer: what does `const` mean on a member function?
