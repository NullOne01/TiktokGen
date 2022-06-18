#include "TextVideoGenerator.h"

#include <utility>
#include <iostream>

TextVideoGenerator::TextVideoGenerator(std::string pathToVideo, std::string text) : path_to_video_(std::move(
        pathToVideo)), text_(std::move(text)) {}

TextVideoGenerator::~TextVideoGenerator() = default;

Generator<cv::Mat> TextVideoGenerator::frameGenerator() {
//    return return;
}
