#include "AddAudioGenerator.h"

#include <utility>

void AddAudioGenerator::addAudio() {

}

AddAudioGenerator::~AddAudioGenerator() {

}

AddAudioGenerator::AddAudioGenerator(const std::string &path_to_video, std::string path_to_audio)
        : AudioGenerator(
        path_to_video), path_to_audio_(std::move(path_to_audio)) {}
