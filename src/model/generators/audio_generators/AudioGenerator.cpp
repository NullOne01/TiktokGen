#include "AudioGenerator.h"

#include <utility>

AudioGenerator::AudioGenerator(std::string path_to_video, std::string path_to_output) : path_to_video_(std::move(
        path_to_video)), path_to_output_(std::move(path_to_output)) {

}

AudioGenerator::~AudioGenerator() {

}
