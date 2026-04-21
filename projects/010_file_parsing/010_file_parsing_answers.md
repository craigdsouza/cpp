# Answers — File Parsing + Real Data

---

## Question QR1 (From Day 9)

**Undefined behavior — not a compile error. `std::transform` writes past the end of the shorter output vector.**

`std::transform` does not know or check the size of the output range — it simply advances the output iterator once per input element. If the output vector has 4 elements and the input has 8, `std::transform` writes elements 5 through 8 past the end of the output vector's allocated memory. This is a buffer overflow — undefined behavior. In practice it silently corrupts memory or crashes at a later point.

The compiler cannot catch this because iterators are just pointers under the hood — they carry no size information. The fix is to ensure the output range is at least as large as the input before calling `std::transform`, or use `std::back_inserter(output)` if you want the output vector to grow automatically.

---

## Question 1

**`std::stringstream` wraps a string so you can use stream operations on it — it's a `std::string` that behaves like a file.**

Line by line:
1. `std::stringstream ss(line)` — wraps the string `"37.4268,-122.1733,Palo_Alto_Station"` in a stream object. Now `ss` can be read from like a file.
2. `std::getline(ss, lat_str, ',')` — reads from `ss` up to the first `','`, storing `"37.4268"` in `lat_str`. The `','` is consumed but not stored.
3. `std::getline(ss, lon_str, ',')` — reads from where the stream left off, up to the next `','`, storing `"-122.1733"` in `lon_str`.
4. `std::getline(ss, name)` — reads the remainder of the stream (no delimiter = read to end), storing `"Palo_Alto_Station"` in `name`.
5. `std::stof(lat_str)` — converts the string `"37.4268"` to the `float` value `37.4268f`.

Why not `file >> lat >> lon >> name`? The `>>` operator splits on *whitespace* (spaces, tabs, newlines), not commas. It would fail to split `"37.4268,-122.1733,Palo_Alto_Station"` correctly — it would try to parse the entire string as a float and fail. `std::getline` with a delimiter is how you split on non-whitespace characters.

---

## Question 2

**Bug: the header line `"lat,lon,name"` is processed as a data row. `std::stof("lat")` throws `std::invalid_argument` and the program crashes.**

The fix — read and discard the header before the loop:
```cpp
std::string header;
std::getline(file, header);   // consume the header line, then enter the loop
```

One line, placed immediately after opening the file.

---

## Question 3

**Wrap the `std::stof` calls in `try/catch std::invalid_argument`, skip the line on failure.**

```cpp
while (std::getline(file, line)) {
    try {
        std::stringstream ss(line);
        std::string ts_str, x_str, y_str, z_str, i_str;
        std::getline(ss, ts_str, ',');
        std::getline(ss, x_str,  ',');
        std::getline(ss, y_str,  ',');
        std::getline(ss, z_str,  ',');
        std::getline(ss, i_str);
        points.push_back({
            std::stof(ts_str), std::stof(x_str), std::stof(y_str),
            std::stof(z_str),  std::stof(i_str)
        });
    } catch (const std::invalid_argument&) {
        std::cerr << "Warning: skipping malformed line: " << line << "\n";
    }
}
```

The whole body of the parse attempt is inside `try`. If any `stof` call fails, execution jumps to `catch`, prints a warning, and the `while` loop moves to the next line. You've been using `try/catch` since Day 5 — this is the same mechanism, now applied to a parsing boundary rather than a resource operation.

---

## Question 4

**`std::optional<GPSWaypoint>` is preferred in modern C++ over returning a raw pointer.**

Raw pointer approach:
```cpp
GPSWaypoint* parse_waypoint(const std::string& line) {
    // ...
    return new GPSWaypoint(lat, lon, name);   // caller must delete — easy to leak
    // or return nullptr on failure
}
```
This works but the caller must manage the heap allocation manually — easy to forget `delete`, and ownership is unclear from the signature.

`std::optional<T>` approach (C++17):
```cpp
#include <optional>
std::optional<GPSWaypoint> parse_waypoint(const std::string& line) {
    try {
        // ... parse ...
        return GPSWaypoint{lat, lon, name};   // success — wraps the value
    } catch (...) {
        return std::nullopt;                  // failure — empty optional
    }
}
// Caller:
auto wp = parse_waypoint(line);
if (wp) { use(*wp); }   // wp.has_value() is true
```

`std::optional<T>` lives on the stack, requires no heap allocation, and the "maybe nothing" contract is explicit in the return type — the caller cannot forget to check. This is the idiomatic modern C++ way to express "this function might not produce a value." Python's `None` is its closest analogue.

---

## Question 5

**Use `std::stringstream` + `std::getline(ss, field, ',')` — but you extract fields sequentially, not by index.**

```cpp
std::string line = "37.4268,-122.1733,Palo_Alto_Station";
std::stringstream ss(line);
std::string f1, f2, f3;
std::getline(ss, f1, ',');   // "37.4268"
std::getline(ss, f2, ',');   // "-122.1733"
std::getline(ss, f3);        // "Palo_Alto_Station"
```

In Python, `line.split(',')` returns a list you can index in any order: `parts[0]`, `parts[2]`. In C++, `std::getline` is stateful — it advances through the stream left-to-right. You must extract fields in the order they appear; you cannot jump to field 2 without first consuming fields 0 and 1. This is the key difference: Python gives you random access to all fields at once; C++ gives you sequential access one field at a time.

If you genuinely need random access to fields, extract them all into a `std::vector<std::string>` first, then index the vector.

---
