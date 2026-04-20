// Exercise 4: Integration — LidarScanProcessor
// No scaffold. Write everything from scratch.
//
// Implement a LidarScanProcessor class that models a single scan's processing
// stage in the DRIVE perception pipeline.
//
// Requirements:
//   struct LidarPoint { float x, y, z, intensity; ... };
//
//   class LidarScanProcessor {
//     - owns std::vector<LidarPoint> points_
//     - void add(LidarPoint p)
//     - void sort_by_intensity()             — ascending, std::sort + lambda
//     - const LidarPoint* find_above(float threshold) const
//                                            — std::find_if, return nullptr if none
//     - void normalize()                     — std::transform + captured max, no-op if empty
//     - int count_above(float threshold) const  — std::count_if
//     - void print_all(std::string label) const — std::for_each
//   };
//
// In main:
//   1. Add 8 points with intensities in [0.3f, 1.8f]. Print ("raw").
//   2. Sort, print ("sorted").
//   3. Find first point above 1.0f, print it.
//   4. Normalize, print ("normalized"). Verify all intensities in [0,1].
//   5. Count normalized points above 0.6f, print count.
//   6. Second processor with 3 points all below 0.5f —
//      verify find_above(0.8f) returns nullptr and prints not-found message.
