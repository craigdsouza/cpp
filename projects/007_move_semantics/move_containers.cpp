#include <iostream>
#include <vector>
#include <string>

// Exercise 2: Moving Standard Containers
// Goal: observe the cost difference between copying and moving a std::vector

// write a function that takes a vector<string> by value (a "sink" — it accepts ownership)
// it should print how many tiles it received, then print each label
void process_tiles(std::vector<std::string> tiles) {
    std::cout<< "no of tiles: " << tiles.size() << std::endl;
    int size = tiles.size();
    int i;
    for (i=0;i<size;i++){
        std::cout << "tile label " << i << ": " << tiles[i] << std::endl;
    }
};

int main() {
    // create a vector of 5 map tile labels
    std::vector<std::string> tiles = {"tile_N1E1", "tile_N1E2", "tile_N2E1", "tile_N2E2", "tile_N3E1"};

    std::cout << "tiles size before move: " << tiles.size() << std::endl;

    // move tiles into a second vector using std::move
    std::vector<std::string> tiles2 = std::move(tiles);

    // print the size of both tiles and tiles2 after the move — what do you observe?
    std::cout << "tiles size after move:  " << tiles.size() << std::endl;
    std::cout << "tiles2 size:           " << tiles2.size() << std::endl;

    // call process_tiles once by copying tiles2 (pass it directly, no std::move)

    // print tiles2 size after the copy call — did it change?
    process_tiles(tiles2);
    std::cout << "tiles2 size after copy call: " << tiles2.size() << std::endl;

    // call process_tiles once by moving tiles2 (use std::move)
    process_tiles(std::move(tiles2));
    // print tiles2 size after the move call — what happened?
    std::cout << "tiles2 size after move call: " << tiles2.size() << std::endl;

    return 0;
}
