// Exercise 4: Non-Type Template Parameters — FixedBuffer<T, N>
// Goal: implement a fixed-capacity stack-allocated buffer.
// N is a compile-time constant — the array lives on the stack, zero heap allocation.
// This is the pattern used in real-time AV code where dynamic allocation is forbidden.

#include <iostream>

// Write the class template below.
// Two template parameters: typename T (element type) and int N (compile-time capacity).
// The internal array T data[N] lives entirely on the stack.

// template<typename T, int N>
// class FixedBuffer {
//
//     // T data[N];    // stack-allocated array — size known at compile time
//     // int size_;    // current number of elements
//
// public:
//     // Constructor: set size_ to 0
//
//     // void push(T value): add value at tail; do nothing if full
//     // T pop(): remove and return oldest (head) element; assumes non-empty
//     // int size() const
//     // bool full() const
//     // bool empty() const
// };


int main() {
    // --- FixedBuffer<float, 8>: lidar intensity readings ---
    // push 6 floats, pop 3, print size (should be 3)
    // print remaining elements

    // --- FixedBuffer<double, 4>: GPS latitude slots ---
    // push 4 doubles to fill to capacity
    // verify full() returns true, print it

    // --- Named constexpr as non-type argument ---
    // constexpr int CAP = 16;
    // FixedBuffer<int, CAP> ring_buf;
    // push a few values, print size

    return 0;
}
