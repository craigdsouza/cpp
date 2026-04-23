// Exercise 1 — Source file: implements what was declared in gps_waypoint.h
//
// Step 1: include the header — use QUOTES for user headers, angle brackets for std library
// #include "gps_waypoint.h"
//
// Step 2: implement the constructor
// Syntax for defining a member function outside its class:
//   ReturnType ClassName::MethodName(params) { ... }
// For a constructor:
//   ClassName::ClassName(params) : member(param), member2(param2) {}
//
// GPSWaypoint::GPSWaypoint(float lat, float lon, const std::string& name)
//     : lat(lat), lon(lon), name(name) {}
//
// The colon syntax is the initializer list — same as Day 3.
// Left side of each pair: the member variable name.
// Right side: the parameter value to initialize it with.

#include "gps_waypoint.h"

GPSWaypoint::GPSWaypoint(float lat, float lon, const std::string& name): lat_(lat),lon_(lon),name_(name){}
