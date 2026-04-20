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
// struct LidarPoint { float x, y, z, intensity; LidarPoint(...) ... };


int main() {
    // --- Create 7 LidarPoints with intensities spread across different ranges ---
    // std::vector<LidarPoint> scan = { ... };


    // --- Find maximum intensity using a range-for loop ---
    // float max_val = scan[0].intensity;
    // for (const LidarPoint& p : scan) { if (p.intensity > max_val) max_val = p.intensity; }


    // --- Normalize in place using std::transform + captured max_val ---
    // std::transform(scan.begin(), scan.end(), scan.begin(),
    //     [max_val](LidarPoint p) {
    //         p.intensity = p.intensity / max_val;
    //         return p;
    //     });


    // --- Print normalized intensities ---


    // --- Count points above 0.5f using std::count_if ---
    // int above = std::count_if(scan.begin(), scan.end(),
    //     [](const LidarPoint& p) { return p.intensity > 0.5f; });
    // std::cout << "Points above 0.5f after normalization: " << above << "\n";


    // --- Demonstrate non-numeric capture ---
    // Capture a std::string label and print it inside the lambda to show
    // that captures aren't limited to numbers
    // std::string label = "normalized_scan";
    // int total = std::count_if(scan.begin(), scan.end(),
    //     [label](const LidarPoint& p) {
    //         (void)label;  // suppress unused warning if you don't print it
    //         return p.intensity > 0.0f;
    //     });
    // std::cout << label << " total valid points: " << total << "\n";


    return 0;
}
