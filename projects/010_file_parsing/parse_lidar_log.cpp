// Exercise 2: Parse a LiDAR Scan Log
// Goal: parse data/lidar_log.csv, skip the header, handle corrupt lines.
// New mechanics: skip header line, try/catch around std::stof for malformed lines
// Format: timestamp,x,y,z,intensity  (first row is a header)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>   // std::sort, std::find_if, std::count_if
#include <stdexcept>   // std::invalid_argument

// --- LidarPoint struct ---
// Fields: float timestamp, x, y, z, intensity
// struct LidarPoint { ... };


// --- load_lidar_log function ---
// Signature: std::vector<LidarPoint> load_lidar_log(const std::string& path)
// Steps:
//   open file, check it opened (if (!file) print error and return {})
//   skip the header: std::string header; std::getline(file, header);
//   while std::getline(file, line):
//     try {
//       extract all 5 fields with stringstream + getline(ss, field, ',')
//       convert each with std::stof
//       push_back to result
//     } catch (const std::invalid_argument&) {
//       print: "Warning: skipping malformed line: " << line
//     }
//   return result


int main() {
    // --- load the log ---
    // std::vector<LidarPoint> points = load_lidar_log("data/lidar_log.csv");
    // print how many points loaded

    // --- count high-intensity points (intensity > 0.7f) using std::count_if + lambda ---

    // --- find first point with intensity > 0.8f using std::find_if + lambda ---
    // check iterator against end() before using

    // --- sort by timestamp ascending using std::sort + lambda ---
    // print first and last points after sorting

    return 0;
}
