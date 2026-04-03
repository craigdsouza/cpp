#include <iostream>

class TileBuffer {
    int* data;
    int  size;
public:
    TileBuffer(int size) : size(size) {
        data = new int[size];
        std::cout << "TileBuffer allocated " << size << " ints\n";
    }

    ~TileBuffer(){
        delete[] data;
        std::cout << "TileBuffer freed " << std::endl;
    }

    void fill(int value) {
        for (int i = 0; i < size; i++) data[i] = value;
    }
};

int main(){
    for (int i=0; i<5; i++){
        TileBuffer buffer(10);
        buffer.fill(42);
    }
    return 0;
}