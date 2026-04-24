# Quiz — Road Graph Fundamentals

---

## Warm-Up (From Day 8/9/11 — Review)

**QR1:** Function templates generate one concrete version per type at compile time. When both `sort<int>` and `sort<float>` are instantiated in your program, how does the compiler ensure each is type-correct? What specific guarantee does this give you that copy-pasting `sort_int` and `sort_float` by hand cannot?

## Answer QR1
When both sort<int> and sort<float> are manually declared and instantiated in the program by a developer, the independent type checking is lost. With the use of templates you can write the function only once and whichever type you call it with will get checked for compiler errors. If you write them manually the errors will still be found out, but they're found out separately for each function and this creates difficulty in maintenance. you have multiple functions to debug for compile erors whereas with templates you only have one. 

so templates doesn't give you errors that manually writing the functions wouldn't show you. their advantage is more about reducing code repetition and therefore improving maintainability. The other benefit is zero cost for unused types. which I have already mentioned in previous quizzes. if you don't use a template function with a particular type the compiler never writes that function, whereas with manually writing functions you end up writing each instance and it all remains in your source code even if unused. removing it is manual. upto you.

---

## Question 1

A road network has 500 nodes and 1,200 edges. Your colleague suggests representing it as a 500×500 matrix of floats (`float adj[500][500]`), where `adj[i][j]` holds the distance or 0 if no edge exists.

Give two concrete disadvantages of this approach compared to `std::unordered_map<std::string, std::vector<Edge>>`.

## Answer 1
The first disadvantage is that storage space taken up is vast for a network where of 500*500= 250,000 potential connections, only 1200 exist. Thus the vast majority of the matrix is filled up with 0s which is space inefficient. Space required grows with O(N^2) for a matrix, whereas it grows as O(N+E) for an adjacency list.

The second disadvantage is that a lookup for all neighbors of a given node takes O(N) because all 500 elements in that node's row must be searched, whereas with an adjacency list it only takes O(degree of node). Degree of node because it takes O(1) to find the node itself in the unordered_map and then `degree of node` to search for the connected neighbors in the std::vector linked with the node.

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
The visit order is A,B,C,D,E. The std::queue (FIFO) is responsible for giving BFS its level by level property because any node that is closest to the top of the queue is necessarily closer to the starting point. Sorry, tracing it step by step feel too onerous, and time consuming for a quiz question.

---

## Question 3

Your BFS implementation uses `std::queue<std::string>`. A colleague suggests replacing it with `std::stack<std::string>` to "make it faster." What traversal would result, and how would the visit order differ from BFS on the graph in Question 2?

## Answer 3
replacing queue with stack would result in depth first search (dfs) instead of bfs. the visit order would i'm not sure of.

---

## Question 4

The road edge CSV contains both `A,B,0.5` and `B,A,0.5` to represent a two-way road. Your `load()` function calls `add_edge` for each row. Without a `visited` set in BFS, what would happen when you call `bfs("A")` on this graph? Be specific about what goes wrong and why.

## Answer 4
without a visited set the bfs function would infinitely loop between node A and B. This is because when we visit A, B is the frontier and we push it onto the queue. thus later when B is processed, it's frontier is A and A is pushed onto the queue, because the `visited` mechanism doesn't exist to prevent this. thus an infinite loop is set in place.

---

## Question 5

Your `road_edges.csv` has a corrupt line: `MARKET_5TH,,abc`. Your `load()` function uses `try/catch std::invalid_argument` around the `stof` call.

(a) Which specific call throws, and what is the value of `distance_km_str` when it does?  
(b) What happens to the rest of the CSV after the exception is caught — does loading continue or stop?  
(c) Does the node `"MARKET_5TH"` end up in the graph? Explain.

## Answer 5
the call to std::stof(distance_km_str) throws an std::invalid_argument error because it expects a value that can be cast to float , but instead its value is a string "abc". The rest of the CSV is processed after skipping this erroneous record altogether. MARKET_5TH may end up in the graph. It depends on whether the node is added to the node registry as soon as its found or after the cast to float for distance completes without error.


---

## Grade Log

### 2026-04-24

**Total: 5.0 / 6.0**

| Q | Score | Note |
|---|-------|------|
| QR1 | 1.0 | Maintainability + zero-cost named correctly; correctly challenged "type safety per instantiation" framing — assistant validated the pushback |
| Q1 | 1.0 | Both disadvantages correct with complexity notation; string ID limitation not mentioned (bonus point only) |
| Q2 | 0.75 | Visit order and queue/FIFO reasoning correct; explicitly skipped step-by-step trace |
| Q3 | 0.5 | DFS named correctly; visit order left blank |
| Q4 | 1.0 | Infinite loop mechanism explained completely and correctly |
| Q5 | 0.75 | (a) stof + "abc" correct, missed that to_id is empty (""); (b) and (c) both correct including implementation-order caveat |
