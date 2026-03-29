# Answers — References vs Pointers

---

## Question 1

**`const T&`**

Two reasons:
- `const` — the function won't modify the vector, and the compiler enforces it
- `&` — no copy is made; just an alias, which matters when the vector is large

---

## Question 2

**`T&`**

Two reasons:
- `&` — the caller's variable needs to change, so you need an alias not a copy
- Not `T*` — tiles always exist, so there's no need for nullable indirection; a reference encodes that guarantee in the signature

---

## Question 3

**`TileNode*`**

A reference cannot be null, so it cannot represent "no parent". A pointer can be set to `nullptr`, which is the only way to express the root node's parentless state. Every node is the same type — the pointer field is what encodes whether a parent exists.

```cpp
struct TileNode {
    int tile_id;
    TileNode* parent;  // nullptr if this is the root
};
```

---

## Question 4

**`TileNode*`**

- You can't return `TileNode&` because when the tile isn't found, there's nothing valid to bind the reference to — a reference must always refer to a real object
- You can't return `TileNode` (by value) because the caller needs to modify the original in the vector, not a copy
- `TileNode*` lets you return `nullptr` when not found, and a pointer to the actual element when found — the caller can then modify it through the pointer

```cpp
TileNode* find_tile(std::vector<TileNode>& tiles, int id) {
    for (auto& tile : tiles) {
        if (tile.tile_id == id) return &tile;
    }
    return nullptr;
}
```

---

## Question 5

**Dangling reference — undefined behavior.**

`t` is a local variable. When `get_default_tile` returns, `t` is destroyed and its memory is freed. The returned reference now points to memory that no longer belongs to your program. Reading from it (or writing to it) is undefined behavior — it may print garbage, crash, or appear to work and corrupt something else silently.

This is one of the most dangerous bugs in C++ because it often isn't caught at compile time.

**What to do instead:** Return by value. Modern C++ (RVO — Return Value Optimization) will avoid the copy automatically:

```cpp
TileNode get_default_tile() {
    TileNode t;
    t.tile_id = 0;
    return t;  // compiler elides the copy
}
```
