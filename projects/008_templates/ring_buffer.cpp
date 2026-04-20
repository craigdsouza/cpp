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
        data_[tail_] = value;
        tail_ = (tail_ + 1) % capacity_;
        size_ += 1;
        if (size_>capacity_){
            head_ = (head_ + 1) % capacity_;
            size_=capacity_;
        }        
     }

    // pop: remove and return oldest value (advance head) ; ASSUME buffer is non-empty , as per exercise
    T pop() {
        T last = data_[head_];
        head_ = (head_ + 1) % capacity_;
        size_ = size_ -1;
        return last;
    }

    // size, full, empty
    int size() const { return size_; }
    bool full() const { return size_==capacity_;}
    bool empty() const { return size_==0;}
};


int main() {
    // --- RingBuffer<float>: capacity 4, lidar intensity readings ---
    // RingBuffer<float> intensity_buf(4);
    // push 5 values: 0.1f, 0.3f, 0.5f, 0.7f, 0.9f  (5th overwrites 0.1f)
    // pop 2 values and print them
    // print size after pops
    RingBuffer<float> intensity_buf(4);
    std::vector<float> intensity_raw= {0.1f,0.3f,0.5f,0.7f,0.9f};
    for (const float& x:intensity_raw){
        intensity_buf.push(x);
    }
    float pop1 = intensity_buf.pop();
    float pop2 = intensity_buf.pop();
    std::cout << pop1 << std::endl;
    std::cout << pop2 << std::endl;
    std::cout << intensity_buf.size() << std::endl;


    // --- RingBuffer<int>: capacity 3, ring channel indices ---
    // RingBuffer<int> ring_buf(3);
    // push 4 values: 10, 20, 30, 40  (40 overwrites 10)
    // pop all remaining values one by one and print each
    // verify the oldest value was correctly overwritten (10 should not appear)
    RingBuffer<int> ring_buf(3);
    std::vector<int> channel_indices= {10,20,30,40};
    for (const int& x:channel_indices){
        ring_buf.push(x);
    }
    int size = ring_buf.size();
    while (size >0){
        std::cout << ring_buf.pop() << std::endl;
        size = ring_buf.size();
    }
    
    return 0;
}
