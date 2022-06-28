#ifndef TIKTOKGEN_AUDIOGENERATOR_H
#define TIKTOKGEN_AUDIOGENERATOR_H

#include <string>

class AudioGenerator {
public:
    explicit AudioGenerator(const std::string &path_to_video);

    virtual void addAudio() = 0;

    virtual ~AudioGenerator() = 0;

private:
    std::string path_to_video_;
};


#endif //TIKTOKGEN_AUDIOGENERATOR_H
