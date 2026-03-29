#include <iostream>

int main() {
    int tile_id = 42;
    int* p = &tile_id;
    std::cout << "tile_id: " << tile_id << "\n";
    std::cout << "p: " << p << "\n";
    std::cout << "*p: " << *p << "\n";
    *p = 100;
    std::cout << "tile_id: " << tile_id << "\n";

    int* q = &tile_id;
    if (q == nullptr) {
        std::cout << "q is nullptr\n";
    } else {
        std::cout << "q is not nullptr\n";
        std::cout << "q: " << q << "\n";
        std::cout << "*q: " << *q << "\n";
    }
}