#include <iostream>
#include <string>

void print_tile(const std::string& tile_w_prefix){
    // tile_w_prefix = "broken";
    std::cout << tile_w_prefix << "\n";
}

void add_prefix(std::string& tile) {
    tile = "tile_" + tile;
    print_tile(tile);
}

int main() {
    std::string tile_id = "007";
    add_prefix(tile_id);
}





