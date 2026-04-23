// Exercise 4 — Source file for MapTile
// No scaffold provided. Write from scratch.
//
// Include map_tile.h and implement the constructor using ClassName:: syntax and an initializer list.

#include "map_tile.h"

MapTile::MapTile(std::string id, float lat_min, float lon_min, float lat_max, float lon_max, int road_count)
    : id_(id),
    lat_min_(lat_min),
    lon_min_(lon_min),
    lat_max_(lat_max),
    lon_max_(lon_max),
    road_count_(road_count) {}