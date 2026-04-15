#include "../005_raii_and_destructors/map_tile.h"
#include <memory>
#include <iostream>

std::unique_ptr<MapTile> load_tile(){
    return std::make_unique<MapTile>(42,37.77,-122.41,"San Fransisco");
}

// for exercise 5, day 6
std::unique_ptr<MapTile> make_tile(const std::string& city) {
    if (city == "SF")     return std::make_unique<MapTile>(1, 37.77, -122.41, "San Francisco");
    if (city == "Delhi")  return std::make_unique<MapTile>(2, 28.61, 77.20,   "Delhi");
    return nullptr;
}

int main(){
    std::unique_ptr<MapTile> owner = load_tile();
    owner->print_tile();

    // transfer ownership
    std::unique_ptr<MapTile> new_owner = std::move(owner);

    // attempted copy of unique ptr
    // std::unique_ptr<MapTile> copy = owner; 

    // what is owner now?
    if (owner == nullptr){
        std::cout << "owner is now null" << std::endl;
    }
    new_owner->print_tile();

    // for exercise 5, day 6
    {
        auto tile = make_tile("SF");
        if (tile) {
            tile->print_tile();
        }
        auto unknown = make_tile("Tokyo");
        if (!unknown) {
            std::cout << "no tile for Tokyo\n";
        }
    }
}

