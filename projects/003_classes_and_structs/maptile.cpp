#include <iostream>
#include <vector>

struct MapTile {
    int id;
    double lat;
    double lon;
    std::string label;

    MapTile(int id, double lat, double lon, const std::string& label) {
        this->id = id;
        this->lat = lat;
        this->lon = lon;
        this->label = label;
    }

    // member function - for exercise 5
    void print() const {
        // attempt to modify id
        // this->id =10;

        std::cout << "\n" << this->id << " " << this->lat << " " << this->lon << " " << this->label << "\n";
    }
};

// free function - for exercises 1-4
// void print_tile(const MapTile& tile) {
//     std::cout << "\n" << tile.id << " " << tile.lat << " " << tile.lon << " " << tile.label;
// }

void tag_tile(MapTile& tile, const std::string& tag) {
    tile.label += " [" + tag + "]";
}

int main () {
    // for exercise 1 - day 3
    // MapTile a = {1,20,80,"a"};
    // MapTile b = {2,30,70,"b"};    
    // std::cout << "\n" << a.id << " " << a.lat << " " << a.lon << " " << a.label;
    // std::cout << "\n" << b.id << " " << b.lat << " " << b.lon << " " << b.label << "\n";

    // for exercise 2 - day 3
    // MapTile a = {1,20,80,"a"};
    // tag_tile(a,"1");
    // print_tile(a);

    // for exercise 3 - day 3
    // MapTile t(42, 37.7749, -122.4194, "San Francisco");
    // MapTile v(43, 47.7749, -132.4194, "Not San Francisco");

    // for exercise 4 - day 4
    // MapTile t(42, 37.7749, -122.4194, "San Francisco");
    // MapTile v(43, 47.7749, -132.4194, "Not San Francisco");
    // MapTile u(44, 57.7749, -142.4194, "also not San Francisco");
    
    // std::vector<MapTile> tiles = {t,v,u};
    // for (const MapTile& tile:tiles){
    //     print_tile(tile);
    // }
    // for (MapTile& tile:tiles){
    //     if(tile.id %2 ==0){
    //         tag_tile(tile,"even");
    //         print_tile(tile);
    //     }
    // }

    // for exercise 5 - day 5
    MapTile tile = {1,20,80,"a"};
    tile.print();
}