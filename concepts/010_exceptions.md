# Concepts: Exception Handling

> Reference for `try/catch` in C++. Read this when you need to guard against bad sensor data, handle conversion failures from `stof`/`stoi`, or understand when exceptions are the right tool.

---

## The basics: try/catch

```cpp
try {
    // code that might throw
    float x = std::stof("not_a_number");
} catch (const std::invalid_argument& e) {
    std::cerr << "Parse error: " << e.what() << "\n";
}
// execution continues here whether or not the exception was caught
```

When code inside `try` throws, execution jumps immediately to the matching `catch` block. If no exception is thrown, the `catch` block is skipped entirely.

**vs Python:**

```python
try:
    x = float("not_a_number")
except ValueError as e:
    print(f"Parse error: {e}")
```

Same structure. C++ uses `catch` instead of `except`, and the exception type goes in the catch signature rather than after `as`.

---

## Common exception types in Day 10

| Exception | Thrown by | Meaning |
|-----------|-----------|---------|
| `std::invalid_argument` | `stof`, `stoi`, `stod` | String is not a valid number |
| `std::out_of_range` | `stof`, `stoi`, `stod` | Number is valid but too large for the type |
| `std::ios_base::failure` | `ifstream` (if exceptions enabled) | File I/O error |

For Day 10, `std::invalid_argument` is what you'll catch — it fires whenever a CSV field contains a missing value, a typo, or a corrupt sensor reading.

Both `std::invalid_argument` and `std::out_of_range` are in `<stdexcept>` — but `stof`/`stoi` throw them without you needing to include it explicitly.

---

## Guarding a parse loop

The typical pattern for production-safe CSV parsing: wrap the conversion calls inside the loop, skip the line on failure.

```cpp
while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string ts_str, x_str;
    std::getline(ss, ts_str, ',');
    std::getline(ss, x_str, ',');

    try {
        float ts = std::stof(ts_str);
        float x  = std::stof(x_str);
        points.push_back({ts, x});
    } catch (const std::invalid_argument&) {
        std::cerr << "Skipping malformed line: " << line << "\n";
        // continue to next iteration — loop keeps going
    }
}
```

**Scope matters:** the `try` block wraps all the `stof` calls for a single line. If any field fails, the whole line is skipped and the loop continues. Do not put the `while (getline(...))` inside the `try` — that would exit the loop on the first bad line.

---

## Catching by const reference

Always catch exceptions as `const Type&`:

```cpp
catch (const std::invalid_argument& e) { ... }   // correct
catch (std::invalid_argument e) { ... }           // works but copies the object — wasteful
catch (std::invalid_argument& e) { ... }          // works but non-const is a lie — you won't modify it
```

`e.what()` returns a `const char*` description of the error. Useful for logging, but in a sensor pipeline you often just log the raw line and skip it.

---

## Catching all exceptions

```cpp
catch (...) {
    // catches anything — use sparingly
}
```

This catches any thrown object. Useful as a last-resort safety net but hides the error type. Prefer catching specific types so you can log or handle them appropriately.

---

## When NOT to use exceptions

Exceptions are for *exceptional* conditions — unexpected failures outside the normal control flow. Do not use them for:
- Regular control flow ("did the user enter a valid number?" — validate with `isdigit` instead)
- Checking if a key exists in a map — use `map::find` and the `end()` sentinel
- Returning multiple values from a function — use a struct or `std::optional`

In Day 10, `stof` throwing on a corrupt sensor log line is the right use: the file *should* be well-formed, and a bad line is genuinely exceptional.

---

## Real-world context

DriveWorks sensor ingest code wraps field-level parsing in exception guards for exactly this reason: lidar scan logs from physical hardware occasionally contain bit-flipped fields or truncated packets. A parser that crashes on the first bad packet stops the entire mapping run. A parser that skips bad packets and logs a warning lets the run complete with 99.9% of the data intact — which is the production behavior.
