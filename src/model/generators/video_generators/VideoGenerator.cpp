#include "VideoGenerator.h"
#include "../audio_generators/AudioGenerator.h"
#include "../audio_generators/AddAudioGenerator.h"
#include <opencv2/videoio.hpp>

VideoGenerator::VideoGenerator(FrameGenerator &frameGenerator) : frame_generator_(frameGenerator) {}

void VideoGenerator::generate() {
    std::string video_name = "out1.mp4";
    cv::VideoWriter video(video_name, cv::VideoWriter::fourcc('H', '2', '6', '4'),
                          60,
                          cv::Size(1080, 1920));

    auto gen = frame_generator_.frameGenerator();
    for (int frame_num = 0; gen; frame_num++) {
        auto frame = gen();
        video.write(frame);
    }

    std::string audio_name = "sample.mp3";
    std::unique_ptr<AudioGenerator> audio_gen = std::make_unique<AddAudioGenerator>(video_name, audio_name);
    audio_gen->addAudio();

    video.release();
}
