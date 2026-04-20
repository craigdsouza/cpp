// Exercise 4: Non-Type Template Parameters — FixedBuffer<T, N>
// Goal: implement a fixed-capacity stack-allocated buffer.
// N is a compile-time constant — the array lives on the stack, zero heap allocation.
// This is the pattern used in real-time AV code where dynamic allocation is forbidden.

#include <array>
#include <iostream>
#include <vector>

// Write the class template below.
// Two template parameters: typename T (element type) and int N (compile-time capacity).
// The internal array T data[N] lives entirely on the stack.

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
    int size() const { return size_; }
    bool full() const { return size_==N;}
    bool empty() const { return size_==0;}
};


int main() {
    // --- FixedBuffer<float, 8>: lidar intensity readings ---
    // push 6 floats, pop 3, print size (should be 3)
    // print remaining elements
    FixedBuffer<float,8> intensity;
    std::vector<float> intensity_raw = {0.1f,0.2f,0.3f,0.4f,0.5f,0.6f};
    for (const float& x:intensity_raw){
        intensity.push(x);
    }

    for (int i=0; i<3; i++){
        intensity.pop();
    };
    std::cout << intensity.size() << std::endl;

    // --- FixedBuffer<double, 4>: GPS latitude slots ---
    // push 4 doubles to fill to capacity
    // verify full() returns true, print it
    FixedBuffer<double,4> gps_lat;
    std::vector<double> gps_lat_raw = {71.0,72.0,73.0,74.0};
    for (const double& x:gps_lat_raw){
        gps_lat.push(x);
    }
    if (gps_lat.full()){
        std::cout << "Array full" << std::endl;
    }
    
    // --- Named constexpr as non-type argument ---
    // constexpr int CAP = 16;
    // FixedBuffer<int, CAP> ring_buf;
    // push a few values, print size
    constexpr int CAP = 16;
    FixedBuffer<int, CAP> ring_buf;
    std::vector<int> ring_indices= {10,20,30,40};
    for (const int& x:ring_indices){
        ring_buf.push(x);
    }
    std::cout << ring_buf.size() << std::endl;
    return 0;
}
