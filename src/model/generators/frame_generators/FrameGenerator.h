#ifndef TIKTOKGEN_FRAMEGENERATOR_H
#define TIKTOKGEN_FRAMEGENERATOR_H

#include <experimental/coroutine>
#include "../Generator.h"
#include "opencv2/core/mat.hpp"
#include "../requirements/GeneratorRequirement.h"

class FrameGenerator {
public:
    virtual Generator<cv::Mat> frameGenerator() = 0;

    virtual ~FrameGenerator();

    virtual void loadData() {}

    std::vector<std::unique_ptr<GeneratorRequirement<std::string>>> requirements;

    void viewRequirements();

    static FrameGenerator *createGenerator(int index);
};


#endif //TIKTOKGEN_FRAMEGENERATOR_H
