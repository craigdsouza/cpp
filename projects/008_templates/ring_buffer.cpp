// Exercise 2: Class Template — RingBuffer<T>
// Goal: implement a ring buffer backed by std::vector<T>.
// A ring buffer overwrites the oldest element when full — used in AV to
// buffer sensor readings between a fast producer and a slower consumer.

#include <iostream>
#include <string>
#include <vector>

// Write the class template below.
// Declare the template parameter, then provide the class opening and members.

template<typename T>
class RingBuffer {

    // --- member variables ---
    std::vector<T> data_;       // internal storage
    int capacity_;              // max elements
    int head_;                  // index of oldest element
    int tail_;                  // index where next push goes
    int size_;                  // current element count

public:
    // Constructor: takes int capacity, reserves space in data_, sets head/tail/size to 0
    RingBuffer(int capacity): capacity_(capacity),head_(0),tail_(0),size_(0){ 
        data_.resize(capacity);
    }

    // push: add value; if full, overwrite oldest (advance head past it)
    void push(T value) {
        if (size_ == capacity_){
            data_[head_] = value;
            head_+=1;
        } else {
            data_[tail_] = value;
            tail_+=1;
        }
     }

    // pop: remove and return oldest value (advance head)
    T pop() {
        T last = std::move(data_[head_]);
        data_.erase(head_);
        return last;
    }

    // size, full, empty
    int size() const { return data_.size(); }
    bool full() const { if (data_.size() == capacity_): return true, false ;}
    bool empty() const { return data_.empty(); }
};


int main() {
    // --- RingBuffer<float>: capacity 4, lidar intensity readings ---
    // RingBuffer<float> intensity_buf(4);
    // push 5 values: 0.1f, 0.3f, 0.5f, 0.7f, 0.9f  (5th overwrites 0.1f)
    // pop 2 values and print them
    // print size after pops


    // --- RingBuffer<int>: capacity 3, ring channel indices ---
    // RingBuffer<int> ring_buf(3);
    // push 4 values: 10, 20, 30, 40  (40 overwrites 10)
    // pop all remaining values one by one and print each
    // verify the oldest value was correctly overwritten (10 should not appear)


    return 0;
}
