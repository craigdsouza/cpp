#include <iostream>
#include <string>

class LidarFrame {
    float* points;
    int count;

public:
    // constructor: allocate count floats, fill with 0.0f
    LidarFrame(int count) : count(count) {
        points = new float[count];
        int i;
        for (i = 0; i < count; i++) {
            points[i] = 0.0f;
        }
        std::cout << "LidarFrame constructed (count=" << count << ")" << std::endl;
    }

    // destructor: free the heap array
    ~LidarFrame() {
        delete[] points;
        std::cout << "LidarFrame destroyed" << std::endl;
    }

    // copy constructor: deep-copy the array
    LidarFrame(const LidarFrame& other) {
        int i;
        count = other.count;
        points = new float[count];
        for (i = 0; i < count; i++) {
            points[i] = other.points[i];
        }
        std::cout << "LidarFrame copied" << std::endl;
    }

    // move constructor: steal other's array, nullify other
    LidarFrame(LidarFrame&& other) {
        points = other.points;
        other.points = nullptr;
        count = other.count;
        other.count = 0;
        std::cout << "LidarFrame moved" << std::endl;
    }

    // move assignment operator — complete this
    // difference from move constructor: `this` already owns an array that must be freed first
    LidarFrame& operator=(LidarFrame&& other) {
        // guard against self-assignment: if (this == &other) return *this;
        if (this == &other) return *this;

        // free this->points (it already owns heap memory)
        delete[] points;

        // steal other.points and other.count
        points = other.points;
        count = other.count;
        // nullify other.points and other.count
        other.points = nullptr;
        other.count = 0;
        return *this;
    }

    int getCount() const { return count; }
};

int main() {
    // construct two LidarFrames with different counts
    LidarFrame a(10);
    LidarFrame b(20);

    // demonstrate copy: copy-construct a third from the first
    // verify "LidarFrame copied" prints
    LidarFrame c = a;

    // demonstrate move constructor: move-construct a fourth using std::move
    // print the source's count after — verify it is 0
    LidarFrame d = std::move(a);
    std::cout << a.getCount() << std::endl;

    // demonstrate move assignment: assign one existing LidarFrame into another using std::move
    // print the source's count after — verify it is 0
    b = std::move(d);
    std::cout << d.getCount() << std::endl;

    // at program end all destructors fire — verify no crash (would indicate a double-free)

    return 0;
}
