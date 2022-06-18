#ifndef TIKTOKGEN_FRAMEGENERATOR_H
#define TIKTOKGEN_FRAMEGENERATOR_H

#include <experimental/coroutine>
#include "../Generator.h"
#include "opencv2/core/mat.hpp"
#include "utils/GeneratorRequirementType.h"

class FrameGenerator {
public:
    virtual Generator<cv::Mat> frameGenerator() = 0;

    virtual ~FrameGenerator();

private:
};


#endif //TIKTOKGEN_FRAMEGENERATOR_H
