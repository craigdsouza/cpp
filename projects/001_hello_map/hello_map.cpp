#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::ofstream outfile("output.txt");

    std::vector<std::string> tiles_str = {"tile_001","tile_002","tile_003"};
    std::vector<int> tiles_int = {1,2,3};

    std::string line1 = "Loading " + std::to_string(tiles_int.size()) + " map tiles...\n";
    std::cout << line1;
    outfile << line1;

    for(const int& tile : tiles_int) {
        std::string line = " -> integer " + std::to_string(tile) + "\n";
        std::cout << line;
        outfile << line;
    };

    std::string line2 = (tiles_int.size() % 2 == 0) ? "Even number of tiles\n" : "Odd number of tiles\n";
    std::cout << line2;
    outfile << line2;

    return 0;
}