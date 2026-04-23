// Exercise 4 — Source file for TileIndex
// No scaffold provided. Write from scratch.
//
// Include tile_index.h, <fstream>, <sstream>, <stdexcept>, <algorithm>, <iostream>
//
// Implement all four methods using TileIndex:: prefix:
//   TileIndex::load    — parse CSV, try/catch stof/stoi, map::insert
//   TileIndex::find    — map::find + end() sentinel, return &it->second or nullptr
//   TileIndex::print_all  — std::for_each + lambda over tiles_
//   TileIndex::count_above — std::count_if + lambda on tiles_ values
//
// Note: iterating std::map gives std::pair<key, value> — use pair.second for the MapTile

#include "tile_index.h"
#include "map_tile.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>

TileIndex::TileIndex(){}

void TileIndex::load(const std::string& path){
    std::ifstream file(path);
    std::string line, header;

    std::getline(file,header);
    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string tile_id, lat_min_str, lon_min_str, lat_max_str, lon_max_str, road_count_str;
        std::getline(ss,tile_id,',');
        std::getline(ss,lat_min_str,',');
        std::getline(ss,lon_min_str,',');
        std::getline(ss,lat_max_str,',');
        std::getline(ss,lon_max_str,',');
        std::getline(ss,road_count_str);
        try{
            float lat_min = std::stof(lat_min_str);
            float lon_min = std::stof(lon_min_str);
            float lat_max = std::stof(lat_max_str);
            float lon_max = std::stof(lon_max_str);
            int road_count = std::stoi(road_count_str);
            tiles_.emplace(tile_id,MapTile(tile_id,lat_min,lon_min,lat_max,lon_max,road_count));
        } catch (const std::invalid_argument& error){
            std::cerr << "Invalid tile found and skipped" << std::endl;
        }
    }
}

const MapTile* TileIndex::find(const std::string& id) const{
    auto it = tiles_.find(id);
    if(it!=tiles_.end()){
        return &it->second;
    } else {
        return nullptr;
    }
}

void TileIndex::print_all() const{
    std::for_each(tiles_.begin(),tiles_.end(),[](const std::pair<const std::string, MapTile>& tile){
        std::cout << tile.first << ": road count - " << tile.second.road_count_ << std::endl;
    });
}

int TileIndex::count_above(int min_roads) const{
    int count = std::count_if(tiles_.begin(),tiles_.end(),[min_roads](const auto& tile){
        return(tile.second.road_count_>min_roads);
    });
    return count;
}