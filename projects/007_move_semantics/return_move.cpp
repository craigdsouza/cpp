#include <iostream>
#include <vector>
// Exercise 4: Move Semantics in Return Values
// Goal: understand why std::move on a return value is an anti-pattern

// define a minimal LidarFrame with:
//   - int count member
//   - constructor that prints "LidarFrame constructed (count=N)"
//   - move constructor that prints "LidarFrame moved"
//   - destructor that prints "LidarFrame destroyed"

class LidarFrame{
    int count;
    public:
        LidarFrame(int count): count(count){
            std::cout << "LidarFrame constructed (count="<< count <<")" << std::endl;
        }
        ~LidarFrame(){
            std::cout << "LidarFrame destroyed" << std::endl;
        }

        LidarFrame(LidarFrame&& other): count(other.count){
            other.count = 0;
            std::cout << "LidarFrame moved" << std::endl;
        }
};

// factory_nrvo: creates a named local LidarFrame and returns it by value (no std::move)
LidarFrame factory_nrvo(int count) { 
    LidarFrame a(count);
    return a;
 };

// factory_move: creates a named local LidarFrame, wraps it in std::move before returning
LidarFrame factory_move(int count) { 
    LidarFrame b(count);
    return (std::move(b));
 }

int main() {
    // call factory_nrvo — does "LidarFrame moved" print? why or why not?
    LidarFrame x = factory_nrvo(10);
    // call factory_move — does "LidarFrame moved" print? why or why not?
    LidarFrame y = factory_move(10);
    // add a comment here explaining which factory is correct and why
    std::cout << "factory_nrvo is correct, because it performs no extra operations";
    return 0;
}
