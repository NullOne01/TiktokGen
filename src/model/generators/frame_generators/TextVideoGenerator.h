#ifndef TIKTOKGEN_TEXTVIDEOGENERATOR_H
#define TIKTOKGEN_TEXTVIDEOGENERATOR_H


#include "FrameGenerator.h"
#include "utils/GeneratorRequirement.h"

class TextVideoGenerator : public FrameGenerator {
public:
    TextVideoGenerator(std::string pathToVideo, std::string text);

    ~TextVideoGenerator() override;

    Generator<cv::Mat> frameGenerator() override;

    inline const static std::vector<GeneratorRequirement> requirements = {
            GeneratorRequirement("MainText", NEED_TEXT),
            GeneratorRequirement("BackgroundVideo", NEED_VIDEO),
    };

    inline const static std::string name = "TextVideoGenerator";

private:
    std::string path_to_video_;
    std::string text_;
};


#endif //TIKTOKGEN_TEXTVIDEOGENERATOR_H
