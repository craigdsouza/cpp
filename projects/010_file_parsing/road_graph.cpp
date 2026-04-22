// Exercise 4: Integration — Road Segment Parser
// No scaffold. Write this program from scratch.
//
// Data file: data/road_segments.csv
// Header: segment_id,start_lat,start_lon,end_lat,end_lon,road_class,speed_limit
//
// Implement:
//   struct RoadSegment — all seven fields

#include<iostream>
#include <stdexcept>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<numeric>

struct RoadSegment{
    std::string segment_id_, road_class_;
    float start_lat_, start_lon_, end_lat_, end_lon_, speed_limit_;

    RoadSegment(std::string segment_id,
        float start_lat, float start_lon, float end_lat, float end_lon,
        std::string road_class, float speed_limit){
            segment_id_=segment_id;
            start_lat_=start_lat;
            start_lon_=start_lon;
            end_lat_=end_lat;
            end_lon_=end_lon;
            road_class_ = road_class;
            speed_limit_=speed_limit;
    }
};

class RoadGraph{
    std::vector<RoadSegment> segments_;
    public:
        void load(const std::string& path){
            std::ifstream file(path);
            std::string line, header;
            
            std::getline(file,header);
            while(std::getline(file,line)){
                std::stringstream ss(line);

                std::string segment_id, start_lat_str,start_lon_str, end_lat_str,end_lon_str, road_class, speed_limit_str;
                std::getline(ss,segment_id,',');
                std::getline(ss,start_lat_str,',');
                std::getline(ss,start_lon_str,',');
                std::getline(ss,end_lat_str,',');
                std::getline(ss,end_lon_str,',');
                std::getline(ss,road_class,',');
                std::getline(ss,speed_limit_str);
                try{
                    float start_lat = std::stof(start_lat_str);
                    float start_lon = std::stof(start_lon_str);
                    float end_lat = std::stof(end_lat_str);
                    float end_lon = std::stof(end_lon_str);
                    float speed_limit = std::stof(speed_limit_str);
                    segments_.push_back(RoadSegment(segment_id,start_lat,start_lon,end_lat,end_lon,road_class,speed_limit));
                } catch (const std::invalid_argument& error) {
                    std::cerr << "One invalid argument found, skipping" << std::endl;
                }

                
            }
        }

        void print_all(){
            std::for_each(segments_.begin(),segments_.end(),[](const RoadSegment& segment){
                std::cout << segment.segment_id_ << ", " << segment.road_class_ << ", " << segment.speed_limit_ <<std::endl;
            });
        }

        int count_by_class(const std::string& road_class) const{
            return std::count_if(segments_.begin(),segments_.end(),[road_class](const RoadSegment& segment){
                return (segment.road_class_==road_class);
            });

        }

        const RoadSegment* fastest() const {
            if(segments_.empty()) return nullptr;
            auto it = std::max_element(segments_.begin(),segments_.end(),[](const RoadSegment& a, const RoadSegment& b){
                return a.speed_limit_ < b.speed_limit_;
            });
            return &(*it);
        }

        void normalize_speeds(){
            if (segments_.empty()) return;
            float max_speed = std::max_element(segments_.begin(),segments_.end(),[](const RoadSegment& a, const RoadSegment& b){
                return a.speed_limit_ < b.speed_limit_;
            })->speed_limit_;
            std::transform(segments_.begin(),segments_.end(),segments_.begin(),[max_speed](RoadSegment s)-> RoadSegment{
                s.speed_limit_ /= max_speed;
                return s;
            });
        }

        float average_speed() const {
            if(segments_.empty()) return 0.0f;
            float total = std::accumulate(segments_.begin(),segments_.end(),0.0f,[](float sum, const RoadSegment& s){
                return sum + s.speed_limit_;
            });
            return (total/segments_.size());
        }


};


int main(){
    RoadGraph rg;
    rg.load("data/road_segments.csv");

    // print all segments
    rg.print_all();

    // print count of motorway segments and primary segments
    std::cout << "Motorway segments: " << rg.count_by_class("motorway") << 
  "\n";
  std::cout << "Primary segments: "  << rg.count_by_class("primary")  << 
  "\n";

    // find and print fastest segment
    const RoadSegment* fastest = rg.fastest();
    if(fastest){
    std::cout << "\nSegment with fastest speed: " << fastest->segment_id_ << " Speed: " << fastest->speed_limit_ << std::endl;
    };

    // print average speed before normalization
    std::cout << "\nAverage speed on all segments: " << rg.average_speed() << "\n";

    // normalize speeds
    rg.normalize_speeds();

    // print all again - verify speeds in [0,1]
    std::cout << "\nSpeeds after normalizing: " << std::endl;
    rg.print_all();

    return 0;
}