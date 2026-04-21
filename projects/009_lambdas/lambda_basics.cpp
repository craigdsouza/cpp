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

struct LidarPoint{
    float x_;
    float y_;
    float z_;
    float intensity_;

    LidarPoint(float x, float y, float z, float intensity): x_(x),y_(y),z_(z),intensity_(intensity){}
};

int main() {
    // --- Create a vector of 6 LidarPoints ---
    // Mix of intensities: some below 0.5f, some above
    // e.g. { {0,0,0,0.2f}, {1,0,0,0.9f}, ... }
    std::vector<LidarPoint> scan = { 
        {0,1,0,0.5f},
        {1,0,0,0.3f},
        {0,1,1,0.4f},
        {1,1,1,0.8f},
        {1,0,1,0.9f},
        {1,1,0,1.0f},
    };


    // --- Use std::for_each to print each point ---
    std::for_each(scan.begin(), scan.end(), [](const LidarPoint& p) {
        std::cout << "(" << p.x_ << ", " << p.y_ << ", " << p.z_
                  << ") intensity: " << p.intensity_ << "\n";
    });


    // --- Use std::for_each with a captured counter ---
    // Declare: int count = 0;
    // Lambda: [&count](const LidarPoint& p) { if (p.intensity > 0.5f) count++; }
    // After: print count
    int count = 0;
    std::for_each(scan.begin(),scan.end(), [&count] (const LidarPoint& p) {
        if (p.intensity_ > 0.5f){count++;}
    });


    // --- Write the same count with a plain range-for loop ---
    // int count2 = 0;
    // for (const LidarPoint& p : scan) { ... }
    // Verify count == count2
    int count2 = 0;
    for (const LidarPoint& p: scan) {
        if (p.intensity_ > 0.5f){
            count2++;
        }
    }

    std::cout << "count1: " << count << std::endl;
    std::cout << "count2: " << count2 << std::endl;

    return 0;
}
