#ifndef MAP_TILE_H
#define MAP_TILE_H

#include <iostream>
#include <string>

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

    void print_tile() {
        std::cout << "tile id " << this->id << ": " << "lat:" << this->lat << " lon:" << this->lon << " place: " << this->label << "\n";
    }
};

#endif
