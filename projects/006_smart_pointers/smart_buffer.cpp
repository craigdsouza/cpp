#include <memory>
#include <iostream>

class TileBuffer {
    int size;
    std::unique_ptr<int[]> data;
    

    public:
        TileBuffer(int n): size(n), data(std::make_unique<int[]>(n)){
            std::cout << "TileBuffer allocated " << n <<  " ints\n";
        }

        void fill(int value){
            for (int i=0;i<size;i++) data[i] = value;
        }
};

int main(){
    for(int i=0;i<5;i++){
        TileBuffer buf(10);
        buf.fill(42);
    }
}