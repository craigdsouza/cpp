// Exercise 3: Template Specialization
// Goal: write a generic SensorLogger<T> and a specialization for bool.
// The compiler picks the most specific match — the bool specialization fires
// for booleans; the generic fires for float, int, etc.

#include <iostream>
#include <string>

// --- Generic SensorLogger<T> ---
// Method: void log(std::string label, T value)
// Prints: "[label] reading: value"
//
// template<typename T>
// class SensorLogger {
// public:
//     // void log(std::string label, T value) { ... }
// };


// --- Specialization for bool ---
// Method: void log(std::string label, bool value)
// Prints: "[label] ALERT" if value is true
//         "[label] OK"    if value is false
// The specialization syntax starts with: template<>
// class SensorLogger<bool> {
//
//     // void log(std::string label, bool value) { ... }
// };


int main() {
    // SensorLogger<float> float_logger;
    // float_logger.log("intensity", 0.73f);           // expected: "intensity reading: 0.73"

    // SensorLogger<int> int_logger;
    // int_logger.log("ring_channel", 42);             // expected: "ring_channel reading: 42"

    // SensorLogger<bool> bool_logger;
    // bool_logger.log("collision_alert", true);       // expected: "collision_alert ALERT"
    // bool_logger.log("lane_clear", false);           // expected: "lane_clear OK"

    return 0;
}
