#ifndef TIKTOKGEN_NONEGENERATOR_H
#define TIKTOKGEN_NONEGENERATOR_H


#include "FrameGenerator.h"

class NoneGenerator : public FrameGenerator {
public:
    Generator<cv::Mat> frameGenerator() override;

    ~NoneGenerator() override;

    inline const static std::string name = "NoneGenerator";
};


#endif //TIKTOKGEN_NONEGENERATOR_H
