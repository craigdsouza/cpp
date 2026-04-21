// Exercise 3: Transform and Count
// Goal: use std::transform with a captured value to normalize data in place,
// and std::count_if to query the result.
//
// std::transform signature (in-place):
//   std::transform(v.begin(), v.end(), v.begin(), fn);
//   fn takes a LidarPoint and returns a LidarPoint with modified intensity.
//
// Capture by value [max_val]: lambda gets a snapshot — correct for normalization.
// Capture by reference [&max_val]: lambda sees live changes — dangerous if max_val changes.

#include <iostream>
#include <vector>
#include <algorithm>   // std::transform, std::count_if
#include <string>

// --- Redefine LidarPoint (float intensity is the minimum required field) ---
struct LidarPoint { 
    float x_, y_, z_, intensity_; 
    LidarPoint(float x, float y, float z, float intensity):x_(x),y_(y),z_(z),intensity_(intensity){};
};


int main() {
    // --- Create 7 LidarPoints with intensities spread across different ranges ---
    std::vector<LidarPoint> scan = {
        {1,3,4,0.4f},
        {2,5,1,0.5f},
        {2,6,2,0.2f},
        {8,8,2,0.6f},
        {5,8,5,0.9f},
        {8,2,4,0.1f},
        {2,8,3,0.6f},
    };


    // --- Find maximum intensity using a range-for loop ---
    float max_val = scan[0].intensity_;
    for (const LidarPoint& p : scan) { if (p.intensity_ > max_val) max_val = p.intensity_; }
    std::cout << "max value found using range-for loop: " << max_val;


    // --- Normalize in place using std::transform + captured max_val ---
    // uses a copy, transforms it and returns. less memory efficient than range-for loop for inplace mutations
    std::transform(scan.begin(), scan.end(), scan.begin(),
        [max_val](LidarPoint p) {
            p.intensity_ /= max_val;
            return p;
        });


    // --- Print normalized intensities ---
    std::for_each(scan.begin(),scan.end(),[](const LidarPoint& p)-> void{
        std::cout << "(" << p.x_ << ", "<< p.y_ << ", "<< p.z_ << ")" << " intensity: " << p.intensity_ << std::endl;
    });

    // --- Count points above 0.5f using std::count_if ---
    int above = std::count_if(scan.begin(), scan.end(),
        [](const LidarPoint& p) { return p.intensity_ > 0.5f; });
    std::cout << "Points above 0.5f after normalization: " << above << "\n";


    // --- Demonstrate non-numeric capture ---
    // Capture a std::string label and print it inside the lambda to show
    // that captures aren't limited to numbers
    std::string label = "normalized_scan";
    int total = std::count_if(scan.begin(), scan.end(),
        [label](const LidarPoint& p) {
            (void)label;  // suppress unused warning if you don't print it
            return p.intensity_ > 0.0f;
        });
    std::cout << label << " total valid points: " << total << "\n";


    return 0;
}
