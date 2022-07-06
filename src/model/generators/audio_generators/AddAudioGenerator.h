#ifndef TIKTOKGEN_ADDAUDIOGENERATOR_H
#define TIKTOKGEN_ADDAUDIOGENERATOR_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
};

#include "AudioGenerator.h"


class AddAudioGenerator : public AudioGenerator {
public:
    void addAudio() override;

    ~AddAudioGenerator() override;

    AddAudioGenerator(const std::string &path_to_video, const std::string &path_to_output, std::string path_to_audio);

private:
    std::string path_to_audio_;

    AVFormatContext *ifmt_ctx;
    AVFormatContext *ifmt_audio_ctx;
    AVFormatContext *ofmt_ctx;

    typedef struct StreamContext {
        AVCodecContext *dec_ctx;
        AVCodecContext *enc_ctx;

        AVFrame *dec_frame;
    } StreamContext;
    StreamContext *stream_ctx;
    StreamContext *stream_audio_ctx;

    int open_input_file(const char *filename);

    int open_input_audio(const char *filename);

    int open_output_file(const char *filename);
};


#endif //TIKTOKGEN_ADDAUDIOGENERATOR_H
