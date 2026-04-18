// Exercise 5: Integration — Typed Sensor Pipeline
// No scaffold. Write everything from scratch.
//
// Design and implement a SensorPipeline<typename T, int Capacity> class template
// that models one stage in the DRIVE sensor pipeline using a fixed-size buffer.
//
// Requirements:
//   - Owns a FixedBuffer<T, Capacity> internally (redefine FixedBuffer here or copy it in)
//   - void feed(T reading)         — adds to buffer; prints warning + does nothing if full
//   - T read()                     — removes and returns oldest reading (assume caller checks first)
//   - void apply(T (*fn)(T))       — applies fn to every element in place
//   - void print_all(std::string label) const  — prints all buffered readings with the label
//
// In main:
//   1. SensorPipeline<float, 8> for lidar intensity:
//      - Feed 6 readings in range [0.5f, 1.5f] (some above 1.0f)
//      - Print all ("before clamp")
//      - Apply a clamp function that brings any value above 1.0f down to 1.0f
//      - Print all ("after clamp")
//
//   2. SensorPipeline<int, 4> for lidar ring channel indices:
//      - Feed 4 channel numbers (0, 32, 64, 96)
//      - Read them out one by one, printing each
//
//   Correct output must clearly show the clamp transform firing on the float pipeline
//   and the int readings extracted in FIFO order.
//   No dynamic allocation anywhere in the program.
