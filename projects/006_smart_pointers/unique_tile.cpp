#include <string>
#include <iostream>
#include <memory>

struct MapTile{
    int id;
    double lat;
    double lon;
    std::string label;

    // cleaner way of initializing MapTile variables using member initializer lists
    MapTile(int id,double lat,double lon, const std::string& label) : id(id), lat(lat), lon(lon), label(label){
        std::cout << "MapTile " << id << " initialized" << std::endl;
    }

    ~MapTile(){
        std::cout << "MapTile " << id << " destroyed" << std::endl;
    }

    void print_tile(){
        std::cout << "tile id " << this->id << ": " << "lat:" << this->lat << " lon:" << this->lon << " place: " << this->label << std::endl;
    }

};

int main(){
    {
        std::unique_ptr<MapTile> tile = std::make_unique<MapTile>(1,37.77,-122.41,"San Fransisco");
        tile->print_tile();
    }
    std::cout << "after block\n";
    return 0;
}