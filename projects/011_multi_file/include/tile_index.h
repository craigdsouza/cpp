// Exercise 4 — Header file for TileIndex class
// No scaffold provided. Write from scratch.
//
// Requirements:
// - #pragma once
// - includes: "map_tile.h", <map>, <string>
// - class TileIndex with private std::map<std::string, MapTile> tiles_
// - public method declarations:
//     void load(const std::string& path)
//     const MapTile* find(const std::string& id) const
//     void print_all() const
//     int count_above(int min_roads) const
