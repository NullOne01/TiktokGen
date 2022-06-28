#ifndef TIKTOKGEN_ADDAUDIOGENERATOR_H
#define TIKTOKGEN_ADDAUDIOGENERATOR_H


#include "AudioGenerator.h"

class AddAudioGenerator : public AudioGenerator {
public:
    void addAudio() override;

    ~AddAudioGenerator() override;

    AddAudioGenerator(const std::string &path_to_video, std::string path_to_audio);

private:
    std::string path_to_audio_;
};


#endif //TIKTOKGEN_ADDAUDIOGENERATOR_H
