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
struct LidarPoint {
    float timestamp_, x_, y_, z_, intensity_;

    LidarPoint(float timestamp, float x, float y, float z, float intensity){
        timestamp_ = timestamp;
        x_ = x;
        y_ = y;
        z_ = z;
        intensity_ = intensity;
    }
};


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
std::vector<LidarPoint> load_lidar_log(const std::string& path){
    std::vector<LidarPoint> result;
    std::ifstream file(path);
    if (!file.is_open()){
        std::cerr << "File could not be opened\n" << std::endl;
        return {};
    }
    std::string line, header;
    std::getline(file,header);
    while(std::getline(file,line)){
        std::stringstream ss(line);

        std::string timestamp_str,x_str,y_str,z_str,intensity_str;
        std::getline(ss,timestamp_str,',');
        std::getline(ss,x_str,',');
        std::getline(ss,y_str,',');
        std::getline(ss,z_str,',');
        std::getline(ss,intensity_str);

        try {
            float timestamp = std::stof(timestamp_str);
            float x = std::stof(x_str);
            float y = std::stof(y_str);
            float z = std::stof(z_str);
            float intensity = std::stof(intensity_str);
            result.push_back({timestamp,x,y,z,intensity});
        } catch (const std::invalid_argument& error) {
            std::cerr << "One invalid argument found, skipping" <<std::endl;
        };
    }
    return result;
}



int main() {
    // --- load the log ---
    std::vector<LidarPoint> points = load_lidar_log("data/lidar_log.csv");
    // print how many points loaded
    std::cout << points.size() << " valid points loaded." << std::endl;

    // --- count high-intensity points (intensity > 0.7f) using std::count_if + lambda ---
    int hi_intensity = std::count_if(points.begin(),points.end(),[](const LidarPoint& p){
        return p.intensity_>0.7f;
    });
    std::cout << "High intensity points: " << hi_intensity << std::endl;

    // --- find first point with intensity > 0.9f using std::find_if + lambda ---
    // check iterator against end() before using
    auto it = std::find_if(points.begin(),points.end(),[](const LidarPoint& p){
        return p.intensity_>0.9f;
    });
    if(it!=points.end()){
        std::cout << "TIMESTAMP of first point with intensity > 0.9f: " << it->timestamp_ << std::endl;
    } else {
        std::cout << "No point found with intensity > 0.9f." << std::endl;
    }

    // --- sort by timestamp descending using std::sort + lambda ---
    // print first and last points after sorting
    std::sort(points.begin(),points.end(),[](const LidarPoint& a, const LidarPoint& b){
        return (a.timestamp_>b.timestamp_);
    });
    std::cout << "First timestamp in reverse chronological order: " << points.front().timestamp_ << std::endl;
    std::cout << "last timestamp in reverse chronological order: " << points.back().timestamp_ <<  std::endl;
    


    return 0;
}
