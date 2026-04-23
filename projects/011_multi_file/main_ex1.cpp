// Exercise 1 — Header/Source Split
// Compile: g++ -std=c++17 -Wall -Wextra -I include main_ex1.cpp src/gps_waypoint.cpp -o main_ex1.exe
// Or:      make main_ex1.exe

// include the user header (quotes, not angle brackets)
#include "gps_waypoint.h"
#include <iostream>
#include <vector>
#include <algorithm>   // std::for_each

int main() {
    // create a std::vector<GPSWaypoint> containing three waypoints:
    //   (37.4219f, -122.0840f, "Googleplex")
    //   (37.4275f, -122.1697f, "Stanford")
    //   (37.6213f, -122.3790f, "SFO")
    // Hint: use push_back or initializer list construction
    std::vector<GPSWaypoint> waypoints;
    waypoints.push_back({37.4219f, -122.0840f, "Googleplex"});
    waypoints.push_back({37.4275f, -122.1697f, "Stanford"});
    waypoints.push_back({37.6213f, -122.3790f, "SFO"});

    // use std::for_each + lambda to print each waypoint
    // format: "[name] lat=X lon=Y"
    // lambda signature: [](const GPSWaypoint& wp) { ... }
    std::for_each(waypoints.begin(),waypoints.end(),[](const GPSWaypoint& wp){
        std::cout << "[" << wp.name_ << "] " << "lat=" << wp.lat_ << " " << "lon=" << wp.lon_ <<std::endl;
    });

    return 0;
}
