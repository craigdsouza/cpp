// Exercise 2 — Separating a Function
// Compile: g++ -std=c++17 -Wall -Wextra -I include main_ex2.cpp src/gps_waypoint.cpp src/waypoint_loader.cpp -o main_ex2.exe
// Or:      make main_ex2.exe

// include only the loader header — it already includes gps_waypoint.h transitively
// #include "waypoint_loader.h"
#include <iostream>
#include <algorithm>   // std::for_each

int main() {
    // load waypoints from the Day 10 data file
    // path: "../010_file_parsing/data/waypoints.csv"

    // use std::for_each + lambda to print each waypoint
    // format: "[name] lat=X lon=Y"

    return 0;
}
