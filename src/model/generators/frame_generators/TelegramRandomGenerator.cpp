#include <opencv2/videoio.hpp>
#include <opencv2/freetype.hpp>
#include <opencv2/imgproc.hpp>
#include "TelegramRandomGenerator.h"
#include "../requirements/VideoChooseRequirement.h"
#include "../requirements/TelegramRandomRequirement.h"
#include "plog/Log.h"
#include "../../../view/utils/TextFunctions.h"

TelegramRandomGenerator::TelegramRandomGenerator() {
    requirements.push_back(std::make_unique<VideoChooseRequirement>("BackgroundVideo"));
    requirements.push_back(std::make_unique<TelegramRandomRequirement>("TelegramRandom"));
}

TelegramRandomGenerator::~TelegramRandomGenerator() {

}

Generator<cv::Mat> TelegramRandomGenerator::frameGenerator() {
    cv::VideoCapture background_video_;
    background_video_.open(path_to_video_);
    if (!background_video_.isOpened()) {
        LOGD << "Video cannot be opened. Path: " + path_to_video_;
    }

    cv::Ptr<cv::freetype::FreeType2> ft2;
    ft2 = cv::freetype::createFreeType2();
    ft2->loadFontData("resources/DejaVuSansCondensed.ttf", 0);

    for (int i = 0; i < 600; ++i) {
        cv::Mat frame(1920, 1080, CV_8UC4, cv::Scalar(255, 255, 255, 255));

        // Loading background video.
        cv::Mat background_video_frame;
        background_video_.read(background_video_frame);
        cv::resize(background_video_frame, frame, {1080, 1920}, cv::INTER_CUBIC);

        double text_width = 500;
        double text_height = 400;
        int thickness = 1;
        cv::Scalar text_color = cv::Scalar(255, 255, 255, 255);
        int font_height = TextFunctions::getBestFontHeight(ft2, frame, telegram_text_, text_height, text_width,
                                                           thickness);

        cv::Size multiline_size = TextFunctions::getSizeOfLines(ft2, telegram_text_, text_width,
                                                                font_height, text_color, thickness);
        cv::Point text_center((frame.cols - multiline_size.width) / 2, (frame.rows - multiline_size.height) / 2);

        TextFunctions::putTextMultiline(ft2, frame,
                                        telegram_text_,
                                        text_center, font_height,
                                        text_color, text_width,
                                        thickness);

        co_yield frame;
    }

    co_return;
}

void TelegramRandomGenerator::loadData() {
    path_to_video_ = requirements[0]->getValue();
    telegram_text_ = requirements[1]->getValue();
}
