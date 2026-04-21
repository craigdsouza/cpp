// Exercise 2: Sorting and Finding
// Goal: use std::sort with a lambda comparator and std::find_if to locate elements.
// std::sort comparator: returns true if first arg should come BEFORE second.
// std::find_if returns an iterator — check against end() before dereferencing (Day 4 pattern).

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>   // std::sort, std::find_if

// --- Redefine LidarPoint ---
struct LidarPoint { 
    float x_, y_, z_, intensity_; 
    LidarPoint(float x, float y, float z, float intensity):x_(x),y_(y),z_(z),intensity_(intensity){}; 
};


// --- helper: print a vector of LidarPoints with a label ---
void print_scan(const std::string& label, const std::vector<LidarPoint>& scan) {
    std::cout << "[" << label << "]" << std::endl;
    std::for_each(scan.begin(),scan.end(), [](const LidarPoint& p)-> void {
        std::cout << "(" << p.x_ << ", "<< p.y_ << ", "<< p.z_ << ", " << ")" << " intensity: " << p.intensity_ << std::endl;
    });
};


int main() {
    // --- Create 8 LidarPoints with mixed intensities ---
    std::vector<LidarPoint> scan = {
        {1,8,7,0.1f},
        {1,4,8,0.4f},
        {0,2,5,0.9f},
        {4,7,3,0.8f},
        {8,8,0,0.1f},
        {2,4,0,0.3f},
        {7,3,3,0.7f},
        {5,1,7,0.5f},
     };


    // --- Sort ascending by intensity ---
    std::sort(scan.begin(), scan.end(), [](const LidarPoint& a, const LidarPoint& b) {
        return a.intensity_ < b.intensity_;
    });
    print_scan("ascending", scan);


    // --- Sort descending by intensity ---
    std::sort(scan.begin(), scan.end(), [](const LidarPoint& a, const LidarPoint& b)-> bool{
        return a.intensity_ > b.intensity_;
    });
    print_scan("descending", scan);


    // --- Find first point with intensity > 0.7f ---
    auto it = std::find_if(scan.begin(), scan.end(),
        [](const LidarPoint& p) { return p.intensity_ > 0.7f; });
    if (it != scan.end()) { std::cout << it->intensity_ << std::endl; }
    else { std::cout << "No point above threshold\n"; }


    // --- Try a threshold with no match (e.g. > 2.0f) ---
    // verify the not-found path fires

    auto it2 = std::find_if(scan.begin(),scan.end(),[](const LidarPoint& p) -> bool {
        return p.intensity_ > 2.0f;
    });
    if (it2 != scan.end()) {std::cout << it2->intensity_ << std::endl;}
    else {std::cout << "No point above threshold\n";};


    return 0;
}
