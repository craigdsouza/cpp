// Exercise 2 — Implements load_waypoints declared in waypoint_loader.h
//
// includes:
// #include "waypoint_loader.h"   (user header — quotes)
// #include <fstream>             (std::ifstream)
// #include <sstream>             (std::stringstream)
//
// Implement: std::vector<GPSWaypoint> load_waypoints(const std::string& path)
// This is a free function (not a class member), so no ClassName:: prefix needed.
//
// Logic is identical to Day 10 parse_waypoints.cpp:
//   open file with std::ifstream
//   read line by line with std::getline(file, line)
//   for each line: wrap in std::stringstream, extract lat_str/lon_str/name with getline + ','
//   convert with std::stof, push_back a GPSWaypoint into the result vector
//   return the vector

#include "waypoint_loader.h"
#include "gps_waypoint.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<GPSWaypoint> load_waypoints(const std::string &path){
    std::vector<GPSWaypoint> result;
    std::string line, header;
    std::ifstream file(path);
    std::getline(file,header);

    while(std::getline(file,line)){
        std::string lat_str,lon_str,name;
        std::stringstream ss(line);
        std::getline(ss,lat_str,',');
        std::getline(ss,lon_str,',');
        std::getline(ss,name);
        float lat = std::stof(lat_str);
        float lon = std::stof(lon_str);
        result.push_back({lat,lon,name});
    };
    return result;
}