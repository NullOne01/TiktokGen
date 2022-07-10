#ifndef TIKTOKGEN_TEXTVIDEOGENERATOR_H
#define TIKTOKGEN_TEXTVIDEOGENERATOR_H


#include "FrameGenerator.h"
#include "../requirements/VideoChooseRequirement.h"

class TextVideoGenerator : public FrameGenerator {
public:
    TextVideoGenerator();

    ~TextVideoGenerator() override;

    Generator<cv::Mat> frameGenerator() override;

    void loadData() override;

    inline const static std::string name = "TextVideoGenerator";

private:
    std::string path_to_video_;
    std::string text_;
};


#endif //TIKTOKGEN_TEXTVIDEOGENERATOR_H
