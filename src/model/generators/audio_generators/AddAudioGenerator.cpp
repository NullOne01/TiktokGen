#include "AddAudioGenerator.h"

extern "C" {
#include <libavformat/avio.h>
#include <libavformat/avformat.h>
#include <libavutil/file.h>
};
#include <utility>

void AddAudioGenerator::addAudio() {

    AVFormatContext *fmt_ctx = NULL;
    AVIOContext *avio_ctx = NULL;
    uint8_t *buffer = NULL, *avio_ctx_buffer = NULL;
    size_t buffer_size, avio_ctx_buffer_size = 4096;
    char *input_filename = NULL;
    int ret = 0;

    input_filename = "asd";

    /* slurp file content into buffer */
    ret = av_file_map(input_filename, &buffer, &buffer_size, 0, NULL);
}

AddAudioGenerator::~AddAudioGenerator() {

}

AddAudioGenerator::AddAudioGenerator(const std::string &path_to_video, std::string path_to_audio)
        : AudioGenerator(
        path_to_video), path_to_audio_(std::move(path_to_audio)) {}
