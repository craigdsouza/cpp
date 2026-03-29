# Quiz — Classes and Structs

---

## Question 1

In C++, what is the only technical difference between `struct` and `class`? When would you choose one over the other in practice? Give a concrete example of each from an AV mapping context.

## Answer 1
The only technical difference between the two is that `struct` is public by default. It allows changes to its variables without going through sanctioned methods. `class` is private by default. changes are only allowed via sanctioned methods.

In AV mapping , 
- I might use a class `Policy` when I want to validate a variable `acceleration` to ensure that a Policy doesn't accelerate beyond an accepted threshold.
- I might use a struct if I want to represent raw sensor data.

---

## Question 2

You have this struct:

```cpp
struct VehiclePose {
    double x, y, heading;
    void print() {
        std::cout << x << " " << y << " " << heading << "\n";
    }
};
```

The compiler rejects this code:

```cpp
void log_pose(const VehiclePose& pose) {
    pose.print();
}
```

What is missing from the `print` declaration, and why does C++ require it?

## Answer 2
a `const` is missing after `void print()`, this is required, because the caller function `log_pose` passes along a read only reference to struct `pose` to be printed, and thus the member function `print` must promise not to change it.  

---

## Question 3

You write this loop to tag every tile in a vector:

```cpp
for (MapTile tile : tiles) {
    tile.label = "visited";
}
```

After the loop finishes, all tiles in `tiles` still have their original labels. What went wrong, and what is the one-word fix?

## Answer 3
What went wrong was that copies of each tile in the vector were passed to the for loop instead of references to the actual tiles. Thus in each pass of the loop the copy was altered , not the original. 

The one word fix is to use `MapTile&` instead of `MapTile`

---

## Question 4

What is a constructor in C++? How is it different from a regular member function? Write a constructor for this struct so it can be initialised in one line:

```cpp
struct SensorReading {
    int sensor_id;
    float value;
    std::string unit;
};

// Goal: SensorReading r(3, 9.81, "m/s2");
```

## Answer 4
```cpp
struct SensorReading {
    int sensor_id;
    float value;
    std::string unit;

    SensorReading (int sensor_id, float value, std::string& unit) {
        this->sensor_id = sensor_id;
        this->value = value;
        this->unit = unit;
    }
};
```
A constructor is declared within a struct or class and it helps initialize the struct in one go, without the user having to declare variables individually. A constructor doesn't have to specify any return type whereas all other member functions must.

---

## Question 5

You have this function:

```cpp
void apply_offset(MapTile t, double lat_offset, double lon_offset) {
    t.lat += lat_offset;
    t.lon += lon_offset;
}
```

After calling it, the original tile is unchanged. Why? Write the corrected signature — just the signature, not the body.

## Answer 5
the correct signature is `MapTile& t` which passes a reference to the tile `t` rather than a copy. this ensures that the offset is applied to the original.

---

## Grade Log

### 2026-03-29 — 4.75 / 5.0

| Q | Score | Note |
|---|-------|------|
| Q1 | 1.0 | Correct technical difference + strong AV examples (Policy class vs raw sensor struct) |
| Q2 | 1.0 | Identified missing `const`, explained why compiler requires it |
| Q3 | 1.0 | Correct diagnosis (copy not reference) and correct one-word fix |
| Q4 | 0.75 | Constructor code correct, but missed that it's called implicitly/automatically; `std::string& unit` should be `const std::string&` |
| Q5 | 1.0 | Correct signature and clear reasoning |
