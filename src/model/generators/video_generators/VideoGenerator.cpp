#include "VideoGenerator.h"
#include <opencv2/videoio.hpp>

VideoGenerator::VideoGenerator(FrameGenerator &frameGenerator) : frame_generator_(frameGenerator) {}

void VideoGenerator::generate() {
    cv::VideoWriter video("out1.mp4", cv::VideoWriter::fourcc('H','2','6','4'),
                          60,
                          cv::Size(1080, 1920));

    auto gen = frame_generator_.frameGenerator();
    for (int frame_num = 0; gen; frame_num++) {
        auto frame = gen();
        video.write(frame);
    }

    video.release();
}
