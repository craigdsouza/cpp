// Exercise 3: Build a Map Tile Index
// Goal: parse data/map_tiles.csv into a std::map<string,MapTile> index.
// Format: tile_id,lat_min,lon_min,lat_max,lon_max,road_count (header row)
// New mechanics: std::stoi for integer fields; std::map keyed by parsed string

#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>   // std::sort

// --- MapTile struct ---
// Fields: std::string id, float lat_min, lon_min, lat_max, lon_max, int road_count
struct MapTile {
    std::string id_;
    float lat_min_, lon_min_, lat_max_, lon_max_;
    int road_count_;

    MapTile(std::string id, float lat_min, float lon_min, float lat_max, float lon_max, int road_count){
        id_ = id;
        lat_min_ = lat_min;
        lon_min_ = lon_min;
        lat_max_ = lat_max;
        lon_max_ = lon_max;
        road_count_ = road_count;
    }
};


// --- load_tile_index ---
// Signature: std::map<std::string, MapTile> load_tile_index(const std::string& path)
// skip header, parse each line into a MapTile, insert into map with tile_id as key
// use std::stoi for road_count
std::map<std::string, MapTile> load_tile_index(const std::string& path){
    std::ifstream file(path);
    std::string line,header;
    std::map<std::string, MapTile> result;

    std::getline(file,header);
    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string id_str, lat_min_str, lon_min_str, lat_max_str, lon_max_str, road_count_str;
        std::getline(ss,id_str,',');
        std::getline(ss,lat_min_str,',');
        std::getline(ss,lon_min_str,',');
        std::getline(ss,lat_max_str,',');
        std::getline(ss,lon_max_str,',');
        std::getline(ss,road_count_str);
        float lat_min = std::stof(lat_min_str);
        float lon_min = std::stof(lon_min_str);
        float lat_max = std::stof(lat_max_str);
        float lon_max = std::stof(lon_max_str);
        int road_count = std::stoi(road_count_str); 
        
        // insert each tile into a std::map, with tile_id as key, then to be returned
        result.insert({id_str, MapTile(id_str,lat_min, lon_min, lat_max, lon_max,road_count)});        
    }
    return result;
}

// --- find_tile ---
// Signature: const MapTile* find_tile(const std::map<std::string, MapTile>& index, const std::string& id)
// use map::find + end() sentinel — return &it->second if found, nullptr if not

const MapTile* find_tile(const std::map<std::string,MapTile>& index, const std::string& id){
    auto it = index.find(id);
    if(it==index.end()){
        return nullptr;
    } else {
        return &it->second;
    }
}



int main() {
    // --- load and print all tiles ---
    std::map<std::string, MapTile> maptile_index = load_tile_index("data/map_tiles.csv");

    // --- find "tile_003" — print its road count ---
    auto tile1 = find_tile(maptile_index,"tile_003");
    if (tile1!= nullptr){
        std::cout << "No of roads in tile_003: " << tile1->road_count_ << std::endl;
    } else {
        std::cout << "tile_003 not found" << std::endl;
    }
    
    // --- find "tile_999" — print "not found" ---
    auto tile2 = find_tile(maptile_index,"tile_999");
    if (tile2!= nullptr){
        std::cout << "No of roads in tile_003: " << tile2->road_count_ << std::endl;
    } else {
        std::cout << "tile_999 not found" << std::endl;
    }

    // --- copy tiles into std::vector<MapTile> ---
    std::vector<MapTile> maptile_vector;
    for (const auto& pair: maptile_index){
        maptile_vector.push_back(pair.second);
    }
    // sort descending by road_count using std::sort + lambda
    std::sort(maptile_vector.begin(),maptile_vector.end(),[](const MapTile& a, const MapTile& b){
        return (a.road_count_>b.road_count_);
    });
    // print the sorted order
    std::for_each(maptile_vector.begin(),maptile_vector.end(),[](const MapTile& tile){
        std::cout << tile.id_ << " road_count=" << tile.road_count_ << std::endl;
    });

    return 0;
}
