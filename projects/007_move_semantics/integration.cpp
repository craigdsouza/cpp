// Exercise 5: Integration — DRIVE Sensor Pipeline
// No scaffold. Write everything from scratch.
// Design and implement a PipelineStage class that models one stage in the
// NVIDIA DRIVE sensor pipeline.

#include <iostream>
#include <string>
#include <vector>

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
        std::cout << "LidarFrame copied by construction" << std::endl;
    }

    // move constructor: steal other's array, nullify other
    LidarFrame(LidarFrame&& other) {
        points = other.points;
        other.points = nullptr;
        count = other.count;
        other.count = 0;
        std::cout << "LidarFrame moved by construction" << std::endl;
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
        std::cout << "LidarFrame moved by assignment" << std::endl;
        return *this;
    }

    int getCount() const { return count; }
};

class PipeLineStage{
    // owns a std::vector<LidarFrame> (use your LidarFrame from Exercise 3)
    std::vector<LidarFrame> frames;

    //   Constructors / operators:
    //   - move constructor: transfer the vector to the new stage
    //   - move assignment operator: transfer the vector to an existing stage
    //   - copy constructor: explicitly deleted — pipeline stages are never duplicated
    //   - copy assignment operator: explicitly deleted
    public:
        // constructor, do nothing for now
        PipeLineStage(){

        }

        // move constructor: steal other's array, nullify other in one step with std::move
        PipeLineStage(PipeLineStage&& other){
            frames = std::move(other.frames);
            std::cout << "PipeLineStage moved by construction" << std::endl;
        }

        // move assignment 
        // difference from move constructor: `this` already owns an array that must be freed first
        PipeLineStage& operator=(PipeLineStage&& other){
            if(this== &other) return *this;   // self assignment guard
            
            frames = std::move(other.frames);            // free other's existing Lidar Frame array
            std::cout << "PipeLineStage moved by assignment" << std::endl;
            return *this;
        }
        
        // copy constructor deleted
        PipeLineStage(const PipeLineStage& other) = delete;
        
        // copy assignment deleted
        PipeLineStage& operator=(const PipeLineStage& other) = delete;

        void ingest(LidarFrame&& frame){
            // accepts a frame by move and appends it to the internal vector
            frames.push_back(std::move(frame));
        }
        // LidarFrame extract()
        // removes and returns the oldest frame by move (transfers ownership to caller)
        LidarFrame extract(){
            LidarFrame frame = std::move(frames.front());
            frames.erase(frames.begin());
            return frame;
        }

        //returns the number of frames currently held
        int frame_count() const{
            return frames.size();
        };

};


int main(){
    // create two PipelineStage objects (stage1, stage2)
    std::cout << "-----Creating PipeLineStages 1 and 2-----" <<std::endl;
    PipeLineStage stage1;
    PipeLineStage stage2;
    // ingest 3 LidarFrames into stage1
    std::cout << "\n-----Creating 3 LidarFrames with 1,2,3 frames each -----" <<std::endl;
    LidarFrame a(1);
    LidarFrame b(2);
    LidarFrame c(3);
    std::cout << "\n-----Ingesting 3 LidarFrames into stage1 -----" <<std::endl;
    stage1.ingest(std::move(a));
    stage1.ingest(std::move(b));
    stage1.ingest(std::move(c));

    std::cout << "\n-----Moving PS1 to PS2 -----" <<std::endl;
    // print stage1.frame_count() — should be 3
    std::cout << "Before move - Stage 1 frame count:" << stage1.frame_count() << std::endl;
    // move stage1 into stage2
    stage2 = std::move(stage1);
    // print stage1.frame_count() — should be 0
    std::cout << "Post move - Stage 1 frame count:" << stage1.frame_count() << std::endl;
    // print stage2.frame_count() — should be 3
    std::cout << "Post move - Stage 2 frame count:" << stage2.frame_count() << std::endl;

    std::cout << "\n-----Extracting 3 LidarFrames from PS2 and verifying count -----" <<std::endl;
    // extract all 3 frames from stage2 one by one, printing each frame's count
    LidarFrame f1 = stage2.extract();
    std::cout << "Extracted frame count: " << f1.getCount() << std::endl << std::endl; 
    LidarFrame f2 = stage2.extract();
    std::cout << "Extracted frame count: " << f2.getCount() << std::endl << std::endl; 
    LidarFrame f3 = stage2.extract();
    std::cout << "Extracted frame count: " << f3.getCount() << std::endl << std::endl; 
    // at program end verify all LidarFrame destructors fired exactly once
    // (no "LidarFrame destroyed" should appear more than once per frame)
    //   Correct output shows: no frame was ever copied — only constructed, moved, destroyed.
}