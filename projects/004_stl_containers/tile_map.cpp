#include <map>
#include <unordered_map>

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

    void print_tile(){
        std::cout << "tile id " << this->id << ": " << "lat:" << this->lat << " lon:" << this->lon << " place: " << this->label << "\n";
    }
};

int main() {
    // for exercises 1,3, 4
    std::map<int,MapTile> store;
    store.insert({42, MapTile(42, 37.77, -122.41, "San Francisco")});
    store.insert({12, MapTile(12, 37.77, -122.41, "Delhi")});
    store.insert({92, MapTile(92, 37.77, -122.41, "New York")});
    store.insert({62, MapTile(62, 37.77, -122.41, "Berlin")});
    std::cout << "\n" << "store tiles" << "\n";
    for (auto& item : store){
        item.second.print_tile();
    }

    auto it = store.find(92);
    if (it !=store.end()){
        std::cout << "searched for id 92 and found: " << it->second.label << "\n";
        it->second.label = "Moscow";
        std::cout << "modified 92, place is now: " << it->second.label << "\n";
    } else {
        std::cout << "tile 99 not found\n";
    }

    store.erase(62);
    
    std::cout << "\n" << "updated store tiles" << "\n";
    for (auto& item: store) {
        item.second.print_tile();
    }

    
    // for exercise 2
    std::unordered_map<int,MapTile> cache;
    cache.insert({42, MapTile(42, 37.77, -122.41, "San Francisco")});
    cache.insert({12, MapTile(12, 37.77, -122.41, "Delhi")});
    cache.insert({92, MapTile(62, 37.77, -122.41, "New York")});
    cache.insert({62, MapTile(92, 37.77, -122.41, "Berlin")});
    std::cout << "\n" << "cache tiles" << "\n";
    for(auto& item : cache){
        item.second.print_tile();
    }
    
}