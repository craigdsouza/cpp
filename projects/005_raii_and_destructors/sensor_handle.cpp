#include <iostream>
#include <stdexcept>

class SensorHandle {
    public:
      SensorHandle(){
        std::cout << "LiDAR sensor acquired" << std::endl;
      }
      ~SensorHandle(){
        std::cout << "LiDAR sensor released" << std::endl;
      };

      void read(){
        std::cout << "Reading sensor data" << std::endl;
      };
};

void run_localization(){
    SensorHandle sensor;
    sensor.read();
    sensor.read();
    sensor.read();
    return;
}

void run_localization_with_error(){
    SensorHandle sensor;
    sensor.read();
    sensor.read();
    throw std::runtime_error("localization failed");
    sensor.read(); // never reached
    return;
}

int main(){
    run_localization();
    try{
        run_localization_with_error();
    } catch(const std::runtime_error& e){
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}