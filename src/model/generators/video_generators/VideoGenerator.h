#ifndef TIKTOKGEN_VIDEOGENERATOR_H
#define TIKTOKGEN_VIDEOGENERATOR_H


#include <memory>
#include "../frame_generators/FrameGenerator.h"

class VideoGenerator {
public:
    VideoGenerator(FrameGenerator &frameGenerator);

    void generate();

private:
    FrameGenerator &frame_generator_;
};


#endif //TIKTOKGEN_VIDEOGENERATOR_H
