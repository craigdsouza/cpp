// Exercise 1: Function Templates
// Goal: write two function templates that work for any numeric type.
// The compiler will instantiate a separate concrete function for each type you call it with.

#include <iostream>
#include <string>

// --- sensor_clamp<T> ---
// Returns value clamped to the range [lo, hi].
// If value < lo, return lo. If value > hi, return hi. Otherwise return value.
// Write the template declaration and function body below:

template<typename T>
T sensor_clamp(T value, T lo, T hi) {
    if(value < lo){
        return lo;
    }
    else if (value > hi) {
        return hi;
    }
    else{
        return value;
    }
}


// --- print_reading<T> ---
// Prints "[label]: value" to stdout followed by a newline.
// Write the template declaration and function body below:

// something goes in the template parameter list only if it affects the type of the function or class. 
// T affects the type (float version vs int version). 
// label is just data  passed at runtime — it stays as a regular parameter.
// std::string, as a heap object, built at runtime (not a compile time constant) 
// cannot be a non type template param.
template<typename T>  // label only varies between calls, doesn't vary between types, thus only T is needed
void print_reading(std::string label, T value) {
    std::cout << "[" << label << "]: " << value << std::endl;
}


int main() {
    // --- lidar intensity (float): clamp 1.5f to [0.0f, 1.0f] ---
    float intensity = sensor_clamp(1.5f,0.0f,1.0f);
    print_reading("intensity", intensity);

    // --- ring channel (int): clamp 150 to [0, 127] ---
    int ring = sensor_clamp(150,0,127);
    print_reading("ring_channel", ring);

    // --- GPS latitude (double): clamp 91.0 to [-90.0, 90.0] ---
    double lat = sensor_clamp(91.0,-90.0,90.0);
    print_reading("latitude", lat);

    return 0;
}
