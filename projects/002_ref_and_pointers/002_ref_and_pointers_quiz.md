# Quiz — References vs Pointers

---

## Question 1

You're writing a function that takes a `std::vector` of map tiles and just prints them to the screen. It doesn't modify anything.

What do you use for the parameter: `T`, `T&`, `const T&`, or `T*`?

## Answer 1
`const T&` . that's the safest, it allows reads and forbids edits to variables.

---

## Question 2

You're writing a game loop. Each frame, a function updates a tile's position. It receives the tile and moves it. Tiles always exist — there's no scenario where a tile is missing.

What do you use: `T`, `T&`, `const T&`, or `T*`?

## Answer 2
I'd use `T&`, because I want to be able to edit the tile's position, so `const T&` doesn't make sense. 
I know that tiles always exist, so no need for `T*`. 
I also want to edit the tile's position itself, not a copy of the tile, thus `T` doesn't make sense.

---

## Question 3

You're building a scene graph — a tree structure where each `TileNode` has a field pointing to its parent node. The root node has no parent.

What type do you use for the parent field, and why?

## Answer 3
Because the `TileNode` in some cases needs to have a null value, I would use `T*` since pointers allow `nullptr`

---

## Question 4

You're writing a function that searches a `std::vector<TileNode>` for a tile with a given ID. If found, the caller needs to be able to modify that tile directly. If not found, the caller needs to know nothing was found.

What do you return: `TileNode`, `TileNode&`, `const TileNode&`, or `TileNode*`?

## Answer 4
`TileNode*` , because it allows me to return nullptr if nothing was found.

---

## Question 5

You write this function:

```cpp
TileNode& get_default_tile() {
    TileNode t;
    t.tile_id = 0;
    return t;
}
```

The caller does:

```cpp
TileNode& tile = get_default_tile();
std::cout << tile.tile_id;
```

This compiles. What's wrong with it, and what should you do instead?

The issue is that TileNode t is a local variable and when the main function tries to call it with `tile.tile_id` the variable isn't available anymore, it has been destroyed.
One solution is to use `static TileNode t;` instead, which persists the variable `t` after the function has been called once, for the life of the program.
This is somewhere between a local and global variable. 
However if multiple functions call get_default_tile its possible that their values of `t` are out of sync with each other.
Thus a better solution is perhaps return by value `TileNode` , instead of return by alias `TileNode&`.
In modern C++, the difference between return by value and return by reference isn't as large because of return value optimization.
without RVO, two copies = two slots in memory used for the same variable, which if large is undesirable.
with RVO, the compiler recognises when a value is simply going to be returned by value and thus, no function
copy is ever created, only the caller's copy is created.

---

## Grade Log

### 2026-03-28 — 4.25 / 5.0

| Q | Score | Note |
|---|-------|------|
| Q1 | 0.75 | Correct + reason, but missed the no-copy efficiency benefit of `&` |
| Q2 | 1.0  | Excellent — ruled out all three alternatives with clear reasoning |
| Q3 | 0.75 | Correct + reason, but didn't mention why a reference can't work here |
| Q4 | 0.75 | Correct + reason, but didn't explain why the other three fail |
| Q5 | 1.0  | Outstanding — diagnosed the bug, gave two solutions, explained the trade-off, and covered RVO |

