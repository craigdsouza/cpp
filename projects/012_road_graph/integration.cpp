// Exercise 4 — Integration: Road Graph Analysis
// No scaffold provided. Write this program from scratch.
//
// Build with CMake (add_executable already in CMakeLists.txt).
//
// Spec:

#include "include/road_graph.h"
#include <cstddef>
#include <iostream>
#include <algorithm>

int main(){
    RoadGraph rg;
    // 1. Load data/road_edges.csv into a RoadGraph.
    rg.load("data/road_edges.csv");

// 2. Print a summary header:
//      "Road graph loaded: N nodes, M edges"
    std::cout << "Road graph loaded: " << rg.node_count() << " nodes, " << rg.edge_count() << " edges" << std::endl; 

// 3. Run BFS from "MARKET_4TH". Print the visit order on one line:
//      "BFS from MARKET_4TH: A -> B -> C -> ..."
//    Print the total count of reachable nodes.
    std::vector<std::string> visit_order = rg.bfs("MARKET_4TH");
    std::cout << "BFS from MARKET_4TH: ";
    for(std::size_t i=0;i<visit_order.size();i++){
        if(i<visit_order.size()-1){
            std::cout << visit_order[i] << " -> ";
        } else {
            std::cout << visit_order[i] << std::endl;
        }
    }

// 4. Find the node with the most outgoing edges (most connections).
//    Print: "Most connected: [id] with N outgoing edges"
//    Hint: iterate adj_ — you'll need to expose adj_ via a const getter,
//          or add a method `std::string most_connected() const` to RoadGraph.
    std::pair<std::string,size_t> most_connected = rg.most_connected();
    std::cout << "Most connected: [" << most_connected.first << "] with " << most_connected.second << " outgoing edges";


// 5. Count and print how many nodes have exactly 2 outgoing edges.
//    Use std::count_if with a lambda.
//    Hint: same as above — you'll need access to the adjacency list entries.
    std::size_t two_edges_count = rg.count_nodes_with_degree(2);
    std::cout << "No of nodes with two edges: " << two_edges_count << std::endl;

// 6. Run BFS from "MISSION_6TH" and print whether "HOWARD_4TH" appears in the result.
//    Print: "HOWARD_4TH reachable from MISSION_6TH: yes/no"
    std::vector<std::string> visit_order_m6 = rg.bfs("MISSION_6TH");
    bool reachable = std::find(visit_order_m6.begin(),visit_order_m6.end(), "HOWARD_4TH") != visit_order_m6.end();
    std::cout << "HOWARD_4TH reachable from MISSION_6TH: " << (reachable ? "yes":"no") <<std::endl;

// Correct output (approximate — exact counts depend on your load implementation):
//   Road graph loaded: 7 nodes, 20 edges
//   BFS from MARKET_4TH: MARKET_4TH -> MARKET_5TH -> MISSION_4TH -> ...
//   Reachable from MARKET_4TH: 7 nodes
//   Most connected: MISSION_5TH with 4 outgoing edges
//   Nodes with exactly 2 outgoing edges: 2
//   HOWARD_4TH reachable from MISSION_6TH: yes
    return 0;
}