#include <iostream>
#include <string>

class SensorBuffer{
    std::string name; 
    int size;

    public:
        std::string getName() const {
            return name;
        }

        SensorBuffer(std::string name, int size){
            this->name = name;
            this->size = size;
        }

        SensorBuffer(const SensorBuffer& other){
            name = other.name; // direct copy
            size = other.size;
            std::cout <<"SensorBuffer copied: " << name << std::endl;
        }

        SensorBuffer(SensorBuffer&& other){
            name = std::move(other.name); // triggers string's move constructor
                                            // other.name is now empty, name has the value
            size = other.size;            // int is just 4 bytes — copy is fine, no std::move needed
            other.size = 0;               // nullify the int
            std::cout << "SensorBuffer moved: " << name << std::endl;
        }
};

int validateBuffer(SensorBuffer buf){
    std::cout << "Sensor Buffer name: " << buf.getName() << std::endl;
    return 1;
}

int main(){
    // construct an original buffer
    SensorBuffer a("LIDAR",256);

    // trigger a copy (pass by value to a function, or copy-construct)
    validateBuffer(a);

    // trigger a move (use std::move or return from a function)
    SensorBuffer c = std::move(a);
    // observe which constructor fires in each case

    std::cout << "Sensor buffer name after move: '" << a.getName() << "'" << std::endl;

}
