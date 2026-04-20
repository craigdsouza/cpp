// Exercise 1: Lambda Basics and std::for_each
// Goal: write lambdas inline and pass them to std::for_each.
// A lambda is an anonymous function — the [] is the capture list,
// () is the parameter list, and {} is the body.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>   // std::for_each

// --- LidarPoint struct ---
// Fields: float x, y, z, intensity
// Add a constructor that takes all four values
// struct LidarPoint { ... };


int main() {
    // --- Create a vector of 6 LidarPoints ---
    // Mix of intensities: some below 0.5f, some above
    // e.g. { {0,0,0,0.2f}, {1,0,0,0.9f}, ... }
    // std::vector<LidarPoint> scan = { ... };


    // --- Use std::for_each to print each point ---
    // std::for_each(scan.begin(), scan.end(), [](const LidarPoint& p) {
    //     std::cout << "(" << p.x << ", " << p.y << ", " << p.z
    //               << ") intensity: " << p.intensity << "\n";
    // });


    // --- Use std::for_each with a captured counter ---
    // Declare: int count = 0;
    // Lambda: [&count](const LidarPoint& p) { if (p.intensity > 0.5f) count++; }
    // After: print count


    // --- Write the same count with a plain range-for loop ---
    // int count2 = 0;
    // for (const LidarPoint& p : scan) { ... }
    // Verify count == count2


    return 0;
}
