#include "TextVideoGenerator.h"
#include "plog/Log.h"
#include "../../../view_model/utils/StringFunctions.h"
#include "../../../view/utils/TextFunctions.h"

#include <utility>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

TextVideoGenerator::TextVideoGenerator(std::string path_to_video, std::string text) : path_to_video_(std::move(
        path_to_video)), text_(std::move(text)) {}

TextVideoGenerator::~TextVideoGenerator() = default;

Generator<cv::Mat> TextVideoGenerator::frameGenerator() {
    cv::VideoCapture background_video_;
    background_video_.open(path_to_video_);
    if (!background_video_.isOpened()) {
        LOGD << "Video cannot be opened. Path: " + path_to_video_;
    }

    for (int i = 0; i < 600; ++i) {
        cv::Mat frame(1920, 1080, CV_8UC4, cv::Scalar(255, 255, 255, 255));

        // Loading background video.
        cv::Mat background_video_frame;
        background_video_.read(background_video_frame);
        cv::resize(background_video_frame, frame, {1080, 1920}, cv::INTER_CUBIC);

        // Replace with cv namespace?
        TextFunctions::putTextMultiline(frame, text_, cv::Point(10, frame.rows / 2), cv::FONT_HERSHEY_DUPLEX,
                                        1.0, CV_RGB(0, 0, 0), 300);
//        cv::putText(frame, //target image
//                    lines, //text
//                    cv::Point(10, frame.rows / 2), //top-left position
//                    cv::FONT_HERSHEY_DUPLEX,
//                    1.0,
//                    CV_RGB(0, 0, 0), //font color
//                    2);

        co_yield frame;
    }

    co_return;
}
