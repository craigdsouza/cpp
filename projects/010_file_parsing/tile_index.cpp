// Exercise 3: Build a Map Tile Index
// Goal: parse data/map_tiles.csv into a std::map<string,MapTile> index.
// Format: tile_id,lat_min,lon_min,lat_max,lon_max,road_count (header row)
// New mechanics: std::stoi for integer fields; std::map keyed by parsed string

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>   // std::sort

// --- MapTile struct ---
// Fields: std::string id, float lat_min, lon_min, lat_max, lon_max, int road_count
// struct MapTile { ... };


// --- load_tile_index ---
// Signature: std::map<std::string, MapTile> load_tile_index(const std::string& path)
// skip header, parse each line into a MapTile, insert into map with tile_id as key
// use std::stoi for road_count


// --- find_tile ---
// Signature: const MapTile* find_tile(const std::map<std::string, MapTile>& index, const std::string& id)
// use map::find + end() sentinel — return &it->second if found, nullptr if not


int main() {
    // --- load and print all tiles ---

    // --- find "tile_003" — print its road count ---
    // --- find "tile_999" — print "not found" ---

    // --- copy tiles into std::vector<MapTile> ---
    // sort descending by road_count using std::sort + lambda
    // print the sorted order

    return 0;
}
