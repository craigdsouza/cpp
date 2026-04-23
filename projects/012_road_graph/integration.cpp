// Exercise 4 — Integration: Road Graph Analysis
// No scaffold provided. Write this program from scratch.
//
// Build with CMake (add_executable already in CMakeLists.txt).
//
// Spec:
//
// 1. Load data/road_edges.csv into a RoadGraph.
//
// 2. Print a summary header:
//      "Road graph loaded: N nodes, M edges"
//
// 3. Run BFS from "MARKET_4TH". Print the visit order on one line:
//      "BFS from MARKET_4TH: A -> B -> C -> ..."
//    Print the total count of reachable nodes.
//
// 4. Find the node with the most outgoing edges (most connections).
//    Print: "Most connected: [id] with N outgoing edges"
//    Hint: iterate adj_ — you'll need to expose adj_ via a const getter,
//          or add a method `std::string most_connected() const` to RoadGraph.
//
// 5. Count and print how many nodes have exactly 2 outgoing edges.
//    Use std::count_if with a lambda.
//    Hint: same as above — you'll need access to the adjacency list entries.
//
// 6. Run BFS from "MISSION_6TH" and print whether "HOWARD_4TH" appears in the result.
//    Print: "HOWARD_4TH reachable from MISSION_6TH: yes/no"
//
// Correct output (approximate — exact counts depend on your load implementation):
//   Road graph loaded: 7 nodes, 20 edges
//   BFS from MARKET_4TH: MARKET_4TH -> MARKET_5TH -> MISSION_4TH -> ...
//   Reachable from MARKET_4TH: 7 nodes
//   Most connected: MISSION_5TH with 4 outgoing edges
//   Nodes with exactly 2 outgoing edges: 2
//   HOWARD_4TH reachable from MISSION_6TH: yes
