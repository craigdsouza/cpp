#include <iostream>
#include <vector>
#include <string>

// for exercise 1, day 5
struct MapTile {
    int id;
    double lat;
    double lon;
    std::string label;

    MapTile(int id, double lat, double lon, const std::string& label){
        this->id = id;
        this->lat = lat;
        this->lon = lon;
        this->label = label;

        std::cout << "MapTile " << this->id << " created\n";
    }

    ~MapTile(){
        std::cout << "MapTile " << this->id << " destroyed\n";
    }
};

// for exercise 3, day 5
void vector_test() {
    std::vector<MapTile> tiles;
    tiles.push_back(MapTile(10, 0, 0, "Alpha"));
    tiles.push_back(MapTile(20, 0, 0, "Beta"));
    std::cout << "--- vector going out of scope ---\n";
}

int main(){
    // {
    //     // for exercise 1, day 5
    //     MapTile a(1, 37.77, -122.41, "San Francisco");
    //     MapTile b(2, 28.61, 77.20, "Delhi");
    //     MapTile c(3, 52.52, 13.40, "Berlin");
    //     std::cout << "--- end of block ---\n";
    // }
    // std::cout << "--- after block ---\n";

    // for exercise 3, day 5
    vector_test();

    return 0;
}
