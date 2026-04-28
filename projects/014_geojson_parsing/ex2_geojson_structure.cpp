// Exercise 2 — GeoJSON Structure Exploration
// Compile: cmake --build build
//
// Goal: Load the real GeoJSON file and audit its structure.
// This is the "sanity check" step a map engineer runs before trusting new data.

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <nlohmann/json.hpp>

int main() {
    // TODO: open data/sf_roads_sample.geojson with std::ifstream
    //       check is_open() — print error and return 1 if it fails
    std::ifstream f("data/sf_roads_sample.geojson");
    if (!f.is_open()){
        std::cerr << "Error while opening geojson file" << std::endl;
        return 1;
    }
    // TODO: parse the file into a json object
    //   nlohmann::json data = nlohmann::json::parse(file);
    //   wrap in try/catch for nlohmann::json::parse_error
    using json = nlohmann::json;
    json data;
    try{
        data = json::parse(f);
    } catch (std::exception& e){
        std::cerr << "Error while parsing file" << std::endl;
        return 1;
    }

    // TODO: print total feature count
    //   data.at("features").size()
    std::cout << "Features: " << data.at("features").size()<< std::endl;;

    // TODO: iterate features and for each print:
    //   - name: feature.at("properties").at("name").get<std::string>()
    //   - geometry type: feature.at("geometry").at("type").get<std::string>()
    //   - coordinate count: feature.at("geometry").at("coordinates").size()
    //   - "[one-way]" tag if properties contains "oneway" and value == "yes"
    int count_ow=0;
    double min_lat = std::numeric_limits<double>::max();        
    double max_lat = std::numeric_limits<double>::lowest();   
    double min_lon = std::numeric_limits<double>::max();      
    double max_lon = std::numeric_limits<double>::lowest();

    for (const auto& feature:data.at("features")){
        std::string name = feature.at("properties").at("name").get<std::string>();
        std::string geom_type = feature.at("geometry").at("type").get<std::string>();
        size_t no_coords = feature.at("geometry").at("coordinates").size();
        std:: cout << name << " - " << geom_type << " - " << no_coords << " points"; 

        if (feature.at("properties").contains("oneway")){
            std::string oneway = feature.at("properties").at("oneway").get<std::string>();
            if (oneway=="yes") {
                count_ow++;
                std::cout << " [one-way]";
            } 
        } std::cout << "\n";
        
        
        auto coords = feature.at("geometry").at("coordinates");
        for(size_t i=0; i<coords.size(); i++){
            if(coords[i][0].get<double>()>max_lon){
                max_lon = coords[i][0];
            }
            if(coords[i][1].get<double>()>max_lat){
                max_lat = coords[i][1];
            }
            if(coords[i][0].get<double>()<min_lon){
                min_lon = coords[i][0];
            }
            if(coords[i][1].get<double>()<min_lat){
                min_lat = coords[i][1];
            }
        }
    }

    // TODO: count and print how many features have "oneway": "yes"
    std::cout << "One-way features: " << count_ow<< std::endl;

    // TODO: compute and print the bounding box:
    //   track min/max lat and lon across ALL coordinates in ALL features
    //   initialise: double min_lat = std::numeric_limits<double>::max(), etc.
    //   for each feature → each coord pair → update min/max
    std::cout << "Bounding box: lat [" << min_lat << "," << max_lat << "] lon [" << min_lon << "," << max_lon << "]" << std::endl;
    // Expected output: 
    //   Features: 6
    //     Market Street — LineString — 3 points
    //     Mission Street — LineString — 3 points
    //     Howard Street — LineString — 2 points
    //     4th Street — LineString — 3 points
    //     5th Street — LineString — 3 points
    //     6th Street — LineString — 2 points  [one-way]
    //   One-way features: 1
    //   Bounding box: lat [37.7823, 37.7856]  lon [-122.4076, -122.4035]

    return 0;
}
