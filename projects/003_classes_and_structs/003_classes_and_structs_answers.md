# Answers — Classes and Structs

---

## Question 1

**Use `struct` for plain data; use `class` for encapsulated behaviour.**

The only technical difference: `struct` members are `public` by default, `class` members are `private` by default. That's it — everything else (constructors, member functions, inheritance) works identically.

In practice:
- Use `struct` when the type is just a bundle of data with no invariants to protect. Example: `struct MapTile { int id; double lat, lon; std::string label; };` — all fields are always valid, anyone can read or write them.
- Use `class` when you have private state that must stay consistent. Example: a `TileCache` that manages a fixed-size LRU buffer — you don't want callers directly manipulating the internal array, so you make it private and expose only a `get(id)` method.

---

## Question 2

**The `print` function is missing `const` after its parameter list.**

```cpp
void print() const {
    std::cout << x << " " << y << " " << heading << "\n";
}
```

When you pass an object as `const VehiclePose&`, the compiler only permits calls to member functions that promise not to modify the object. That promise is made by marking the function `const`. Without it, the compiler assumes `print()` might modify `x`, `y`, or `heading` — and it won't allow that on a `const` reference.

This is the same contract as `const T&` on a parameter: it tells both the compiler and the reader that this operation is read-only.

---

## Question 3

**The loop variable `tile` is a copy, not a reference.**

```cpp
for (MapTile& tile : tiles) {  // add &
    tile.label = "visited";
}
```

Without `&`, each iteration creates a fresh copy of the element. Modifying `tile` changes the copy, which is discarded at the end of each iteration. The original vector element is never touched.

Adding `&` makes `tile` an alias for the actual element — the same rule as Day 2: `T&` when you need to write, `const T&` when you only need to read.

---

## Question 4

**A constructor is a special member function that runs automatically when an object is created. It has the same name as the struct/class and no return type.**

Unlike a regular member function (which you call explicitly), a constructor is called implicitly at the point of object creation. You can't skip it or call it twice.

```cpp
struct SensorReading {
    int sensor_id;
    float value;
    std::string unit;

    SensorReading(int id, float v, std::string u)
        : sensor_id(id), value(v), unit(u) {}
};

// Usage:
SensorReading r(3, 9.81, "m/s2");
```

The `: sensor_id(id), value(v), unit(u)` part is the *member initializer list* — the preferred way to initialize fields in a constructor. It initializes them directly rather than default-constructing them first and then assigning.

---

## Question 5

**`t` is passed by value — the function receives a copy of the tile.**

Corrected signature:
```cpp
void apply_offset(MapTile& t, double lat_offset, double lon_offset)
```

By value (`MapTile t`), C++ makes a local copy. All modifications inside the function affect that copy only. The caller's tile is untouched. Using `MapTile&` makes `t` an alias for the caller's tile — same rule as Day 2.
