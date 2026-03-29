#include <iostream>


void swap_tiles(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
    
}

int main() {
    int a = 10;
    int b = 20;
    std::cout << "Before swap, a: " << a << " and b: " << b << "\n";

    swap_tiles(a,b);
    std::cout << "After swap, a: " << a << " and b: " << b << "\n";

}