// Exercise 2: Sorting and Finding
// Goal: use std::sort with a lambda comparator and std::find_if to locate elements.
// std::sort comparator: returns true if first arg should come BEFORE second.
// std::find_if returns an iterator — check against end() before dereferencing (Day 4 pattern).

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>   // std::sort, std::find_if

// --- Redefine LidarPoint ---
// struct LidarPoint { float x, y, z, intensity; LidarPoint(...) ... };


// --- helper: print a vector of LidarPoints with a label ---
// void print_scan(const std::string& label, const std::vector<LidarPoint>& scan) { ... }


int main() {
    // --- Create 8 LidarPoints with mixed intensities ---
    // std::vector<LidarPoint> scan = { ... };


    // --- Sort ascending by intensity ---
    // std::sort(scan.begin(), scan.end(), [](const LidarPoint& a, const LidarPoint& b) {
    //     return a.intensity < b.intensity;
    // });
    // print_scan("ascending", scan);


    // --- Sort descending by intensity ---
    // std::sort(scan.begin(), scan.end(), ... );
    // print_scan("descending", scan);


    // --- Find first point with intensity > 0.7f ---
    // auto it = std::find_if(scan.begin(), scan.end(),
    //     [](const LidarPoint& p) { return p.intensity > 0.7f; });
    // if (it != scan.end()) { ... print it ... }
    // else { std::cout << "No point above threshold\n"; }


    // --- Try a threshold with no match (e.g. > 2.0f) ---
    // verify the not-found path fires


    return 0;
}
