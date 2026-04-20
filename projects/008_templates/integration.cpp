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

#include <iostream>
#include <string>
#include <vector>

template<typename T, int N>
class FixedBuffer {
    T data[N];    // stack-allocated array — size known at compile time
    int head_;
    int tail_;
    int size_;    // current number of elements

public:
    // Constructor: set size_ to 0
    FixedBuffer(){
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }
    // void push(T value): add value at tail; do nothing if full
    void push (T value){
        if (full()) return;
        data[tail_] = value;
        tail_ = (tail_ + 1) % N;
        size_ += 1;
    }
    // T pop(): remove and return oldest (head) element; assumes non-empty
    T pop() {
        T last = data[head_];
        head_ = (head_ + 1) % N;
        size_ = size_ -1;
        return last;
    }

    T peek(int i) const{
        return data[(head_ + i) %N];
    }
    int size() const { return size_; }
    bool full() const { return size_==N;}
    bool empty() const { return size_==0;}
};

template<typename T, int N>
class SensorPipeline{
    FixedBuffer<T, N> buffer;

    public:
    SensorPipeline(){

    }

    void feed (T reading){
        if(buffer.full()){
            std::cout << "Warning: buffer full, dropping reading" << std::endl;
            return;
        }
        buffer.push(reading);

    };

    T read(){
        return buffer.pop();
    };

    void apply(T (*fn)(T)){
        std::vector<T> elements;
        int i;
        int size = buffer.size();
        for(i=0;i<size;i++){
            elements.push_back(buffer.pop());
        };
        for(T& x : elements){
            x = fn(x);
        }
        for(i=0;i<size;i++){
            buffer.push(elements[i]);
        };
    }

    void print_all(std::string label) const {
        std::vector<T> elements;
        int i;
        int size = buffer.size();
        for(i=0;i<size;i++){
            elements.push_back(buffer.peek(i));
        };
        for(T& x : elements){
            std::cout << "[" << label << "]" << x << std::endl;
        }
    }

    bool empty(){
        return buffer.empty();
    }

};

float clamp(float value) {
    if(value < 0.5){
        return 0.5;
    }
    else if (value > 1.0) {
        return 1.0;
    }
    else{
        return value;
    }
}

int main(){
//   1. SensorPipeline<float, 8> for lidar intensity:
//      - Feed 6 readings in range [0.5f, 1.5f] (some above 1.0f)
//      - Print all ("before clamp")
//      - Apply a clamp function that brings any value above 1.0f down to 1.0f
//      - Print all ("after clamp")
    SensorPipeline<float,8> lidar_intensity;
    std::vector<float> readings = {1.1f,0.9f,0.6f,0.8f,1.3f,1.5f};
    for (const float& r:readings){
        lidar_intensity.feed(r);
    };
    std::cout << "\nBefore clamp" << std::endl;
    lidar_intensity.print_all("lidar_intensity");
    lidar_intensity.apply(clamp);
    std::cout << "\nAfter clamp" << std::endl;
    lidar_intensity.print_all("lidar_intensity");
//   2. SensorPipeline<int, 4> for lidar ring channel indices:
//      - Feed 4 channel numbers (0, 32, 64, 96)
//      - Read them out one by one, printing each    
    SensorPipeline<int,4> ring_channel_indices;
    std::vector<int> indices = {0,32,64,96};
    for (const int& i:indices){
        ring_channel_indices.feed(i);
    };
    std::cout << "\nRing Channel Indices" << std::endl;
    while (!ring_channel_indices.empty()) {
        std::cout << ring_channel_indices.read() << std::endl;
    }
    return 0;
}
//   Correct output must clearly show the clamp transform firing on the float pipeline
//   and the int readings extracted in FIFO order.
//   No dynamic allocation anywhere in the program.