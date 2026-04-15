#include <memory>
#include <vector>
#include "../005_raii_and_destructors/map_tile.h"

int main() {
    std::shared_ptr<MapTile> tile = std::make_shared<MapTile>(99, 51.5, -0.1, "London");

    std::vector<std::shared_ptr<MapTile>> cache;
    std::vector<std::shared_ptr<MapTile>> route;

    std::cout << "use count: " << tile.use_count() << std::endl;
    cache.push_back(tile);
    std::cout << "use count: " << tile.use_count() << std::endl;
    route.push_back(tile);

    std::cout << "use count: " << tile.use_count() << "\n"; // should be 3

    cache.clear();
    std::cout << "after cache clear, use count: " << tile.use_count() << "\n"; // should be 2

    route.clear();
    std::cout << "after route clear, use count: " << tile.use_count() << "\n"; // should be 1

} // tile destroyed here — last owner goes out of scope