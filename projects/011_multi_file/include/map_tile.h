// Exercise 4 — Header file for MapTile
// No scaffold provided for Exercise 4. Write this file from scratch.
//
// Requirements:
// - #pragma once
// - include <string>
// - struct MapTile with fields: std::string id, float lat_min/lon_min/lat_max/lon_max, int road_count
// - constructor declaration

#pragma once
#include <string>

struct MapTile{
    std::string id_;
    float lat_min_, lon_min_, lat_max_, lon_max_;
    int road_count_;

    MapTile(std::string id, float lat_min,  float lon_min, float lat_max, float lon_max, int road_count);
};
