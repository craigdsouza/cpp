// Exercise 4: Integration — LidarScanProcessor
// No scaffold. Write everything from scratch.
//
// Implement a LidarScanProcessor class that models a single scan's processing
// stage in the DRIVE perception pipeline.
//
// Requirements:
//   struct LidarPoint { float x, y, z, intensity; ... };

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct LidarPoint{
    float x_,y_,z_,intensity_;
    LidarPoint(float x,float y, float z, float intensity):x_(x),y_(y),z_(z),intensity_(intensity){};
};


//   class LidarScanProcessor {
//     - owns std::vector<LidarPoint> points_
//     - void add(LidarPoint p)
//     - void sort_by_intensity()             — ascending, std::sort + lambda
//     - const LidarPoint* find_above(float threshold) const
//                                            — std::find_if, return nullptr if none
//     - void normalize()                     — std::transform + captured max, no-op if empty
//     - int count_above(float threshold) const  — std::count_if
//     - void print_all(std::string label) const — std::for_each
//   };

class LidarScanProcessor{

public:
    std::vector<LidarPoint> points_;

    void add(LidarPoint p){
        points_.push_back(p);
    }

    void sort_by_intensity(){

        std::sort(points_.begin(), points_.end(),[](const LidarPoint& a, const LidarPoint& b)-> bool{
            return a.intensity_<b.intensity_;
        });
    }

    const LidarPoint* find_above(float threshold) const {
        auto it = std::find_if(points_.begin(),points_.end(),[threshold](const LidarPoint& p) {
            return p.intensity_ > threshold;
            });
        if (it != points_.end()){
            return &(*it);  // it is an iterator, use * to dereference it first, then & to get its address and return
        } else{
            return nullptr;
        };
    }

    void normalize(){
        if (points_.empty()) return;
        float max_val = points_[0].intensity_;
        for (const LidarPoint& p : points_) { if (p.intensity_ > max_val) max_val = p.intensity_; }

        std::transform(points_.begin(),points_.end(),points_.begin(),[max_val](LidarPoint p)-> LidarPoint{
            p.intensity_ /= max_val;
            return p;
        });
    }

    int count_above(float threshold) const {
        int n=0;
        n = std::count_if(points_.begin(),points_.end(),[threshold](const LidarPoint& p){
            return p.intensity_>threshold;
        });
        return n;
    }

    void print_all(std::string label) const {
        std::for_each(points_.begin(),points_.end(),[label](const LidarPoint& p){
            std::cout << "[" << label << "]: " << "(" << p.x_ << ", " << p.y_ << ", " << p.z_
            << ") intensity: " << p.intensity_ << "\n";
        });
    }

};


// In main:
//   1. Add 8 points with intensities in [0.3f, 1.8f]. Print ("raw").
//   2. Sort, print ("sorted").
//   3. Find first point above 1.0f, print it.
//   4. Normalize, print ("normalized"). Verify all intensities in [0,1].
//   5. Count normalized points above 0.6f, print count.
//   6. Second processor with 3 points all below 0.5f —
//      verify find_above(0.8f) returns nullptr and prints not-found message.

int main(){
    LidarScanProcessor lsp;
    lsp.add({1,1,1,0.3f});
    lsp.add({2,2,2,0.5f});
    lsp.add({3,3,3,0.7f});
    lsp.add({4,4,4,0.9f});
    lsp.add({5,5,5,1.1f});
    lsp.add({6,6,6,1.3f});
    lsp.add({7,7,7,1.5f});
    lsp.add({8,8,8,1.7f});
    lsp.print_all("raw");
    lsp.sort_by_intensity();
    lsp.print_all("sorted");
    auto first = lsp.find_above(1.0f);
    if (first) std::cout << "first above 1.0f:  intensity = " << first->intensity_ << "\n";       
    else std::cout << "No point above threshold\n"; 
    lsp.normalize();
    lsp.print_all("normalized");
    int count = lsp.count_above(0.6f);
    std::cout << "count of intensity values above 0.6f threshold: " << count << std::endl;
    LidarScanProcessor lsp2;
    lsp2.add({1,1,1,0.3f});
    lsp2.add({2,2,2,0.35f});
    lsp2.add({3,3,3,0.4f});
    auto first2 = lsp2.find_above(0.8f);
    if (first2) std::cout << "first above 0.6f:  intensity = " << first2->intensity_ << "\n";       
    else std::cout << "No point above threshold\n"; 
    return 0;
}