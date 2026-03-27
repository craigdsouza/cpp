# Copy vs Reference

This note explains the `for` loop patterns discussed in `hello_map.cpp`, with simple C++ and Python comparisons.

## Keep the original array unchanged, but change the loop variable before using it

In C++, this means making a copy for each loop iteration.

```cpp
for (std::string tile : tiles) {
    // Example placeholder: change the copy before using it.
    std::cout << tile << "\n";
}
```

Python does not have a direct equivalent where a normal `for` loop automatically copies each string first.

```python
# No direct equivalent in a normal Python for loop.
for tile in tiles:
    print(tile)
```

In Python, reassigning `tile` only changes the loop variable, not the original list item.

## Change some or all elements of the array

In C++, use a non-const reference so `tile` refers to the real element in the vector.

```cpp
for (std::string& tile : tiles) {
    // Replace the real element with a changed version.
    tile = "changed";
}
```

In Python, the closest equivalent is assigning back through the list index.

```python
for i in range(len(tiles)):
    tiles[i] = tiles[i].upper()
```

## Do not change anything, only use the loop variable as-is

This is the pattern used in `hello_map.cpp`.

```cpp
for (const std::string& tile : tiles) {
    std::cout << tile << "\n";
}
```

In Python, the closest equivalent is a normal `for` loop over the list.

```python
for tile in tiles:
    print(tile)
```

This is only a close match, not an exact one. Python does not have C++ `const`, but strings are immutable, so you cannot modify the string itself in place.
