// Exercise 1: Parse GPS Waypoints
// Goal: read data/waypoints.csv into a vector of GPSWaypoint structs.
// New mechanics: std::stringstream, std::stof
// Format: lat,lon,name  (no header)

#include <iostream>
#include <fstream>     // std::ifstream
#include <sstream>     // std::stringstream
#include <string>
#include <vector>
#include <algorithm>   // std::for_each

// --- GPSWaypoint struct ---
// Fields: float lat, float lon, std::string name
// Add a constructor that takes all three values
struct GPSWaypoint { 
    float lat_;
    float lon_;
    std::string name_;

    GPSWaypoint(float lat,float lon,std::string name):lat_(lat),lon_(lon),name_(name){}
 };


// --- load_waypoints function ---
// Signature: std::vector<GPSWaypoint> load_waypoints(const std::string& path)
// Steps:
//   open the file with std::ifstream
//   read line by line with std::getline(file, line)
//   for each line:
//     wrap it in std::stringstream ss(line)
//     extract lat_str with: std::getline(ss, lat_str, ',')
//     extract lon_str with: std::getline(ss, lon_str, ',')
//     extract name with:    std::getline(ss, name)
//     convert: float lat = std::stof(lat_str);
//     push a GPSWaypoint into the result vector
//   return the vector
std::vector<GPSWaypoint> load_waypoints(const std::string& path){
    std::vector<GPSWaypoint> result;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file,line)){
        std::stringstream ss(line);
        std::string lat_str,lon_str,name;
        std::getline(ss,lat_str,',');
        std::getline(ss,lon_str,',');
        std::getline(ss,name);
        float lat = std::stof(lat_str);
        float lon = std::stof(lon_str);
        result.push_back({lat,lon,name});
    }
    return result;
}

int main() {
    // --- load and print all waypoints ---
    std::vector<GPSWaypoint> waypoints = load_waypoints("data/waypoints.csv");

    // --- use std::for_each + lambda to print each waypoint ---
    // format: "[name] lat=X lon=Y"
    std::for_each(waypoints.begin(),waypoints.end(),[](const GPSWaypoint& p){
        std::cout << p.lat_ << "," << p.lon_ << "," << p.name_ << std::endl; 
    });

    return 0;
}
