// Exercise 4 — Integration: Multi-File Tile System
// No scaffold provided. Write from scratch.
//
// Spec:
// 1. Include "tile_index.h" and <iostream>
// 2. In main():
//    a. Create a TileIndex, call load("../010_file_parsing/data/map_tiles.csv")
//    b. Call print_all()
//    c. Find "tile_003" — if found, print its road_count; otherwise print "not found"
//    d. Find "tile_999" — print "not found"
//    e. Print the count of tiles with road_count > 5
//
// Build this target using CMake (add_executable in CMakeLists.txt)

#include "tile_index.h"
#include <iostream>

int main(){
    TileIndex t;
    t.load("../010_file_parsing/data/map_tiles.csv");
    t.print_all();
    auto result = t.find("tile_003");
    if(result!=nullptr){
        std::cout << result->id_;
    } else {
        std::cout << "tile not found" << std::endl;
    }
    result = t.find("tile_999");
    if(result!=nullptr){
        std::cout << result->id_;
    }else {
        std::cout << "tile not found" << std::endl;
    }
    std::cout << t.count_above(5) << std::endl;

}