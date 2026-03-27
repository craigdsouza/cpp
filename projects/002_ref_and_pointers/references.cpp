#include <iostream>
#include <string>

int main() {
    void add_prefix(std::string& tile) {
        tile = "tile_" + tile;
        std::cout << tile << "\n";
    }
    std::string tile_id = "007";
    add_prefix(tile_id);
}



