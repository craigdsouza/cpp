# Quiz — File Parsing + Real Data

---

## Warm-Up (From Day 9 — Review)

**QR1:** In `std::transform`, you are normalizing a vector of 8 lidar points in place — the input and output iterators both point to the same vector. A colleague asks: *"What would happen if the output iterator instead pointed to a different, shorter vector of only 4 elements — would the compiler catch that and reject it, or would something else happen?"* What actually occurs, and why?

## Answer QR1
`std::transform` would simply write past the end of the output buffer. it does not automatically resize the output buffer since it assumes the output is at least as long as the input. the compiler doesn't catch this issue and this ends up beiing undefined behaviour.  

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
The first line casts `line` which is a std::string to a std::stringstream which means stream operations can be performed on it. The primary operation used in this example is std::getline, with the stringstream as the first input, the variable to be saved to as the second and the delimiter (,) as the third. This splits the line at the next comma and saves the text prior to it to the variable passed as the second parameter. This is what happens in lines 3 and 4. Line 5 is similar except that it doesn't require the comma delimiter since it's the end of the line. Line 2 simply declares the three variables as std::string to which the line's parts are saved to.

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
add std::getline(file,header) before the while loop and declare header along with line as a std::string.
What happens without this is that the header is interpreted as a data row and a conversion of the column names to float is attempted in the push_back step, which would throw error.

---

## Question 3

`std::stof("CORRUPT_LINE_IGNORE_THIS")` throws `std::invalid_argument`. In a real sensor pipeline, corrupt lines in a lidar log are normal — the parser must not crash. Write the structure of the defensive guard you would add around the `std::stof` calls inside the parse loop.

## Answer 3
try {
    float lat = std::stof("CORRUPT_LINE_IGNORE_THIS");
} catch (const std::invalid_argument& error){
    std::cerr << "Invalid line found and skipped" <<std::endl;
}


---

## Question 4

You want to write a function `parse_waypoint(const std::string& line)` that returns a `GPSWaypoint` if the line is valid, or "nothing" if it is malformed. In Python you'd return `None` on failure. In C++ you have two options: return a raw pointer, or return `std::optional<GPSWaypoint>`. Explain the tradeoff. Which is preferred in modern C++ and why?

## Answer 4

---

## Question 5

In Python you split a comma-separated string with `line.split(',')`, which returns a list you can index freely. In C++ there is no built-in `split`. Describe the idiomatic C++ approach using `std::stringstream` and `std::getline`. What is the key difference in how you access each field compared to Python?

## Answer 5
In c++ you use std::stringstream to enable stream operations, primarily std::getline , which allows splitting of the input on a given delimiter. It's perhaps unnecessary to repeat the example. Question 1 in this quiz shares an example and my answer to the same question is the idiomatic approach. First wrap the line in stringstream, then split one part of it at a time using the delimiter and save the component to a variable as a string. In Python when you use the split method you are returned a list of the parts of the string and to access one part you access it with a list index. In c++ you are returned only the part of the string that lies before the next delimiter.


---

## Grade Log

### 2026-04-22 — 4.25 / 6.0

| Q | Score | Note |
|---|-------|------|
| QR1 | 1.0 | Full carry-forward recovery — UB named explicitly, compiler can't catch it, buffer overflow mechanism correct |
| Q1 | 0.75 | stringstream + getline mechanics correct; missing explanation of why `>>` fails (splits on whitespace, not commas) |
| Q2 | 1.0 | Bug, consequence, and one-line fix all correct |
| Q3 | 0.75 | try/catch structure correct; missing placement inside the while loop and "loop continues on catch" semantics |
| Q4 | 0.0 | Unanswered — std::optional vs raw pointer tradeoff |
| Q5 | 0.75 | Sequential vs random access distinction captured; didn't name statefulness or the vector workaround |
