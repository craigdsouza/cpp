# Quiz — File Parsing + Real Data

---

## Warm-Up (From Day 9 — Review)

**QR1:** In `std::transform`, you are normalizing a vector of 8 lidar points in place — the input and output iterators both point to the same vector. A colleague asks: *"What would happen if the output iterator instead pointed to a different, shorter vector of only 4 elements — would the compiler catch that and reject it, or would something else happen?"* What actually occurs, and why?

## Answer QR1

---

## Question 1

You have a CSV line `"37.4268,-122.1733,Palo_Alto_Station"` and you write:

```cpp
std::stringstream ss(line);
std::string lat_str, lon_str, name;
std::getline(ss, lat_str, ',');
std::getline(ss, lon_str, ',');
std::getline(ss, name);
float lat = std::stof(lat_str);
float lon = std::stof(lon_str);
```

Walk through what each of the first five lines does. What is `std::stringstream` and why is it used here instead of `file >> lat >> lon >> name`?

## Answer 1

---

## Question 2

A colleague writes this parser for `waypoints.csv`, which has a header row `"lat,lon,name"` followed by data rows:

```cpp
std::ifstream file("waypoints.csv");
std::string line;
std::vector<GPSWaypoint> points;
while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string lat_str, lon_str, name;
    std::getline(ss, lat_str, ',');
    std::getline(ss, lon_str, ',');
    std::getline(ss, name);
    points.push_back({std::stof(lat_str), std::stof(lon_str), name});
}
```

What happens when this runs? Identify the bug and write the one-line fix.

## Answer 2

---

## Question 3

`std::stof("CORRUPT_LINE_IGNORE_THIS")` throws `std::invalid_argument`. In a real sensor pipeline, corrupt lines in a lidar log are normal — the parser must not crash. Write the structure of the defensive guard you would add around the `std::stof` calls inside the parse loop.

## Answer 3

---

## Question 4

You want to write a function `parse_waypoint(const std::string& line)` that returns a `GPSWaypoint` if the line is valid, or "nothing" if it is malformed. In Python you'd return `None` on failure. In C++ you have two options: return a raw pointer, or return `std::optional<GPSWaypoint>`. Explain the tradeoff. Which is preferred in modern C++ and why?

## Answer 4

---

## Question 5

In Python you split a comma-separated string with `line.split(',')`, which returns a list you can index freely. In C++ there is no built-in `split`. Describe the idiomatic C++ approach using `std::stringstream` and `std::getline`. What is the key difference in how you access each field compared to Python?

## Answer 5

---

## Grade Log
