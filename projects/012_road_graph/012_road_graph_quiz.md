# Quiz — Road Graph Fundamentals

---

## Warm-Up (From Day 8/9/11 — Review)

**QR1:** Function templates generate one concrete version per type at compile time. When both `sort<int>` and `sort<float>` are instantiated in your program, how does the compiler ensure each is type-correct? What specific guarantee does this give you that copy-pasting `sort_int` and `sort_float` by hand cannot?

## Answer QR1

---

## Question 1

A road network has 500 nodes and 1,200 edges. Your colleague suggests representing it as a 500×500 matrix of floats (`float adj[500][500]`), where `adj[i][j]` holds the distance or 0 if no edge exists.

Give two concrete disadvantages of this approach compared to `std::unordered_map<std::string, std::vector<Edge>>`.

## Answer 1

---

## Question 2

Trace BFS on this adjacency list starting from `"A"`. Write the visit order and explain which data structure is responsible for giving BFS its level-by-level property.

```
A → [B, C]
B → [D]
C → [D, E]
D → []
E → []
```

## Answer 2

---

## Question 3

Your BFS implementation uses `std::queue<std::string>`. A colleague suggests replacing it with `std::stack<std::string>` to "make it faster." What traversal would result, and how would the visit order differ from BFS on the graph in Question 2?

## Answer 3

---

## Question 4

The road edge CSV contains both `A,B,0.5` and `B,A,0.5` to represent a two-way road. Your `load()` function calls `add_edge` for each row. Without a `visited` set in BFS, what would happen when you call `bfs("A")` on this graph? Be specific about what goes wrong and why.

## Answer 4

---

## Question 5

Your `road_edges.csv` has a corrupt line: `MARKET_5TH,,abc`. Your `load()` function uses `try/catch std::invalid_argument` around the `stof` call.

(a) Which specific call throws, and what is the value of `distance_km_str` when it does?  
(b) What happens to the rest of the CSV after the exception is caught — does loading continue or stop?  
(c) Does the node `"MARKET_5TH"` end up in the graph? Explain.

## Answer 5

---

## Grade Log
