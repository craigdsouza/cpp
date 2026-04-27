// Exercise 1 — Priority Queue Basics
// Compile: cmake --build build (after cmake -B build)
//
// Goal: Learn std::priority_queue before using it in Dijkstra's.
// A priority queue is like a queue, but instead of FIFO it always serves
// the element with the highest (or lowest) priority first.
// For Dijkstra's, we need a MIN-heap — the node with the smallest distance
// is processed first.
//
// New mechanic: std::priority_queue<T, Container, Compare>
//   Default is a MAX-heap. To get a MIN-heap, use std::greater<T>.

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <utility>   // std::pair
#include <algorithm>

int main() {
    // A "entry" is a (distance, node_id) pair.
    // std::pair compares lexicographically — first by .first (distance), then by .second.
    using Entry = std::pair<float, std::string>;

    // Declare a min-heap priority queue of Entry.
    // Template arguments: <element type, underlying container, comparator>
    // std::greater<Entry> flips the ordering so the SMALLEST distance is on top.
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    // TODO: push these five entries onto pq:
    pq.push({0.42f, "MISSION_5TH"});
    pq.push({0.12f, "MARKET_4TH"});
    pq.push({0.30f, "MISSION_4TH"});
    pq.push({0.27f, "MARKET_5TH"});
    pq.push({0.54f, "HOWARD_4TH"});

    // TODO: pop all entries from pq one by one and print each:
    //   Format: "dist=0.12 node=MARKET_4TH"
    //   Access: pq.top().first (distance), pq.top().second (node_id)
    //   Don't forget: pq.top() peeks, pq.pop() removes (same as std::queue)
    //
    // What to observe: entries come out in ascending distance order (smallest first).
    // This is the property Dijkstra's needs — always process the cheapest node next.
    while(!pq.empty()){
        auto[dist,id] = pq.top();
        pq.pop();
        std::cout << "dist=" << dist << " node=" << id  << std::endl;
    }

    // TODO: declare a second min-heap and demonstrate that equal distances
    // are broken by node_id alphabetically (std::pair compares .second when .first ties).
    // Push: {0.15f, "MARKET_6TH"} and {0.15f, "HOWARD_5TH"}
    // Pop both and print. Which comes first?
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq_min;

    pq_min.push({0.15f,"MARKET_6TH"});
    pq_min.push({0.15f,"HOWARD_5TH"});

    while(!pq_min.empty()){
        auto[dist,id] = pq_min.top();
        pq_min.pop();
        std::cout << "dist=" <<dist << " node=" << id << std::endl;
    }

    return 0;
}
