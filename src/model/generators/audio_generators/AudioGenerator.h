#ifndef TIKTOKGEN_AUDIOGENERATOR_H
#define TIKTOKGEN_AUDIOGENERATOR_H

#include <string>

class AudioGenerator {
public:
    explicit AudioGenerator(std::string path_to_video, std::string path_to_output);

    virtual void addAudio() = 0;

    virtual ~AudioGenerator();

protected:
    std::string path_to_video_;

    std::string path_to_output_;
};


#endif //TIKTOKGEN_AUDIOGENERATOR_H
