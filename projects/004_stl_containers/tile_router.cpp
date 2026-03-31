


#include <vector>
#include <unordered_map>
#include <iostream>
#include <vector>

struct MapTile{
    int id;
    double lat;
    double lon;
    std::string label;

    MapTile(int id, double lat, double lon, const std::string& label){
        this->id = id;
        this->lat = lat;
        this->lon = lon;
        this->label = label;
    };
};

void print_route(const std::vector<int>& route,
                 const std::unordered_map<int,MapTile>& store){
                    for(const int& id :route){
                        auto item = store.find(id);
                        if (item != store.end()){
                            std::cout << "tile found: " << item->second.label << "\n";
                        }
                        else{
                            std::cout << "[missing tile " << id << "]";
                        }
                    }
                 };

int main () {
    std::unordered_map<int, MapTile> store;
    std::vector<int> route = {42,12,62,90,92};
    store.insert({42, MapTile(42, 37.77, -122.41, "San Francisco")});
    store.insert({12, MapTile(12, 37.77, -122.41, "Delhi")});
    store.insert({92, MapTile(92, 37.77, -122.41, "New York")});
    store.insert({62, MapTile(62, 37.77, -122.41, "Berlin")});
    print_route(route,store);
}