// Exercise 2 — Separating a Function
// Compile: g++ -std=c++17 -Wall -Wextra -I include main_ex2.cpp src/gps_waypoint.cpp src/waypoint_loader.cpp -o main_ex2.exe
// Or:      make main_ex2.exe

// include only the loader header — it already includes gps_waypoint.h transitively
// #include "include/gps_waypoint.h"
#include "waypoint_loader.h"
#include <iostream>
#include <algorithm>   // std::for_each
#include <vector>

int main() {
    // load waypoints from the Day 10 data file
    // path: "../010_file_parsing/data/waypoints.csv"
    std::vector<GPSWaypoint> waypoints = load_waypoints("../010_file_parsing/data/waypoints.csv");

    // use std::for_each + lambda to print each waypoint
    // format: "[name] lat=X lon=Y"
    std::for_each(waypoints.begin(),waypoints.end(),[](const GPSWaypoint& wp){
        std::cout << "[" << wp.name_ << "] lat: " << wp.lat_ << " lon: " << wp.lon_ << std::endl;  
    });

    return 0;
}
