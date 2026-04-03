#include <fstream>
#include "map_tile.h"

class TileLogger {
    std::ofstream file;

public:
    TileLogger(const std::string& filename) {
        file.open(filename);
        std::cout << "TileLogger opened" << std::endl;
    }

    ~TileLogger() {
        file.close();
        std::cout << "TileLogger closed" << std::endl;
    }

    void log(const MapTile& tile) {
        file << tile.id << " logged" << std::endl;
    }
};

int main() {
    {
        TileLogger logger("log.txt");
        logger.log({MapTile(42, 37.77, -122.41, "San Francisco")});
        logger.log({MapTile(12, 37.77, -122.41, "Delhi")});
        logger.log({MapTile(92, 37.77, -122.41, "New York")});
    }

    std::ifstream file;
    file.open("log.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    return 0;

}