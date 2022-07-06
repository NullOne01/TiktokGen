#include "AddAudioGenerator.h"
#include "plog/Log.h"
#include "../../../view_model/utils/StringFunctions.h"
#include <utility>
#include <iostream>
#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
};

void AddAudioGenerator::addAudio() {
    // Writing in C is not funny :/

    int ret;
    if ((ret = open_input_file(path_to_video_.c_str())) < 0) {
        PLOGD << "AddAudio: Couldn't open input video file";
        free_resources();
        return;
    }
    if ((ret = open_input_audio(path_to_audio_.c_str())) < 0) {
        PLOGD << "AddAudio: Couldn't open input audio file";
        free_resources();
        return;
    }
    if ((ret = open_output_file(path_to_output_.c_str())) < 0) {
        PLOGD << "AddAudio: Couldn't open output video file";
        free_resources();
        return;
    }

    ofmt_ctx->duration = ifmt_ctx->duration;

    if ((ret = write_all_frames(ifmt_ctx, 0)) < 0) {
        PLOGD << "AddAudio: Couldn't write video frames";
        free_resources();
        return;
    }

    if ((ret = write_all_frames(ifmt_audio_ctx, 1)) < 0) {
        PLOGD << "AddAudio: Couldn't write audio frames";
        free_resources();
        return;
    }

    av_write_trailer(ofmt_ctx);
    free_resources();
}

AddAudioGenerator::~AddAudioGenerator() {

}

AddAudioGenerator::AddAudioGenerator(const std::string &path_to_video, const std::string &path_to_output,
                                     std::string path_to_audio)
        : AudioGenerator(path_to_video, path_to_output), path_to_audio_(std::move(path_to_audio)) {}


int AddAudioGenerator::open_input_file(const char *filename) {
    int ret;
    unsigned int i;

    ifmt_ctx = nullptr;
    if ((ret = avformat_open_input(&ifmt_ctx, filename, nullptr, nullptr)) < 0) {
        PLOGD << "AddAudio: Cannot open input file";
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, nullptr)) < 0) {
        PLOGD << "AddAudio: Cannot find stream information";
        return ret;
    }

    stream_ctx = static_cast<StreamContext *>(av_calloc(ifmt_ctx->nb_streams, sizeof(*stream_ctx)));
    if (!stream_ctx) {
        return AVERROR(ENOMEM);
    }

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *stream = ifmt_ctx->streams[i];
        const AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        AVCodecContext *codec_ctx;
        if (!dec) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Failed to find decoder for stream #%u", i);
            return AVERROR_DECODER_NOT_FOUND;
        }
        codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Failed to allocate the decoder context for stream #%u",
                                                   i);
            return AVERROR(ENOMEM);
        }
        ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
        if (ret < 0) {
            PLOGD << StringFunctions::stringFormat(
                        "AddAudio: Failed to copy decoder parameters to input decoder context for stream #%u", i);
            return ret;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, nullptr);
            }
            /* Open decoder */
            ret = avcodec_open2(codec_ctx, dec, nullptr);
            if (ret < 0) {
                PLOGD << StringFunctions::stringFormat("AddAudio: Failed to open decoder for stream #%u", i);
                return ret;
            }
        }

        stream_ctx[i].dec_ctx = codec_ctx;

        stream_ctx[i].dec_frame = av_frame_alloc();
        if (!stream_ctx[i].dec_frame) {
            return AVERROR(ENOMEM);
        }
    }

    av_dump_format(ifmt_ctx, 0, filename, 0);
    return 0;
}


int AddAudioGenerator::open_output_file(const char *filename) {
    AVStream *out_stream;
    AVStream *in_stream;
    AVCodecContext *dec_ctx, *enc_ctx;
    const AVCodec *encoder;
    int ret;
    unsigned int i;

    ofmt_ctx = nullptr;
    avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, filename);
    if (!ofmt_ctx) {
        PLOGD << "AddAudio: Could not create output context";
        return AVERROR_UNKNOWN;
    }

    // Video
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        out_stream = avformat_new_stream(ofmt_ctx, nullptr);
        if (!out_stream) {
            PLOGD << "AddAudio: Failed allocating output stream";
            return AVERROR_UNKNOWN;
        }

        in_stream = ifmt_ctx->streams[i];
        dec_ctx = stream_ctx[i].dec_ctx;

        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* remuxing */
            encoder = avcodec_find_encoder(dec_ctx->codec_id);
            if (!encoder) {
                PLOGD << "AddAudio: Necessary encoder not found";
                return AVERROR_INVALIDDATA;
            }
            enc_ctx = avcodec_alloc_context3(encoder);
            if (!enc_ctx) {
                PLOGD << "AddAudio: Failed to allocate the encoder context";
                return AVERROR(ENOMEM);
            }

            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                PLOGD << StringFunctions::stringFormat("AddAudio: Copying parameters for stream #%u failed", i);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
            stream_ctx[i].enc_ctx = enc_ctx;
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Elementary stream #%d is of unknown type, cannot proceed",
                                                   i);
            return AVERROR_INVALIDDATA;
        }
    }
    // Audio
    for (i = 0; i < ifmt_audio_ctx->nb_streams; i++) {
        out_stream = avformat_new_stream(ofmt_ctx, nullptr);
        if (!out_stream) {
            PLOGD << "AddAudio: Failed allocating output stream";
            return AVERROR_UNKNOWN;
        }

        in_stream = ifmt_audio_ctx->streams[i];
        dec_ctx = stream_audio_ctx[i].dec_ctx;

        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* remuxing */
            encoder = avcodec_find_encoder(dec_ctx->codec_id);
            if (!encoder) {
                PLOGD << "AddAudio: Necessary encoder not found";
                return AVERROR_INVALIDDATA;
            }
            enc_ctx = avcodec_alloc_context3(encoder);
            if (!enc_ctx) {
                PLOGD << "AddAudio: Failed to allocate the encoder context";
                return AVERROR(ENOMEM);
            }

            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                PLOGD << StringFunctions::stringFormat("AddAudio: Copying parameters for stream #%u failed", i);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
            stream_audio_ctx[i].enc_ctx = enc_ctx;
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Elementary stream #%d is of unknown type, cannot proceed",
                                                   i);
            return AVERROR_INVALIDDATA;
        }
    }
    av_dump_format(ofmt_ctx, 0, filename, 1);

    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Could not open output file '%s'", filename);
            return ret;
        }
    }

    /* init muxer, write output file header */
    ret = avformat_write_header(ofmt_ctx, nullptr);
    if (ret < 0) {
        PLOGD << "AddAudio: Error occurred when opening output file";
        return ret;
    }

    return 0;
}

int AddAudioGenerator::open_input_audio(const char *filename) {
    int ret;
    unsigned int i;

    ifmt_audio_ctx = nullptr;
    if ((ret = avformat_open_input(&ifmt_audio_ctx, path_to_audio_.c_str(), nullptr, nullptr)) < 0) {
        PLOGD << "AddAudio: Cannot open input audio file";
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_audio_ctx, nullptr)) < 0) {
        PLOGD << "AddAudio: Cannot find audio stream information";
        return ret;
    }

    stream_audio_ctx = static_cast<StreamContext *>(av_calloc(ifmt_audio_ctx->nb_streams, sizeof(*stream_audio_ctx)));
    if (!stream_audio_ctx) {
        return AVERROR(ENOMEM);
    }

    for (i = 0; i < ifmt_audio_ctx->nb_streams; i++) {
        AVStream *stream = ifmt_audio_ctx->streams[i];
        const AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        AVCodecContext *codec_ctx;
        if (!dec) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Failed to find decoder for stream #%u", i);
            return AVERROR_DECODER_NOT_FOUND;
        }
        codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            PLOGD << StringFunctions::stringFormat("AddAudio: Failed to allocate the decoder context for stream #%u",
                                                   i);
            return AVERROR(ENOMEM);
        }
        ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
        if (ret < 0) {
            PLOGD << StringFunctions::stringFormat(
                        "AddAudio: Failed to copy decoder parameters to input decoder context for stream #%u",
                        i);
            return ret;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* Open decoder */
            ret = avcodec_open2(codec_ctx, dec, nullptr);
            if (ret < 0) {
                PLOGD << StringFunctions::stringFormat("AddAudio: Failed to open decoder for stream #%u", i);
                return ret;
            }
        }
        stream_audio_ctx[i].dec_ctx = codec_ctx;

        stream_audio_ctx[i].dec_frame = av_frame_alloc();
        if (!stream_audio_ctx[i].dec_frame) {
            return AVERROR(ENOMEM);
        }
    }

    av_dump_format(ifmt_audio_ctx, 0, path_to_audio_.c_str(), 0);
    return 0;
}

/**
 * Write all frames from given context (stream 0) to output file (stream num given).
 * @param ctx Given context to take frames from
 * @param stream_num stream of output file to write into
 * @return error code (0 if success).
 */
int AddAudioGenerator::write_all_frames(AVFormatContext *ctx, int stream_num) {
    int ret;
    AVPacket *packet = nullptr;

    if (!(packet = av_packet_alloc())) {
        PLOGD << "AddAudio: Couldn't allocate packet";
        return -1;
    }

    while (true) {
        if ((ret = av_read_frame(ctx, packet)) < 0) {
            break;
        }
        packet->stream_index = stream_num;

        // remux this frame without reencoding
        // taking the first stream of source file
        av_packet_rescale_ts(packet,
                             ctx->streams[0]->time_base,
                             ofmt_ctx->streams[stream_num]->time_base);
        // Trimming end of video.
        if (stream_num != 0) {
            AVRational r = {1, 1};
            auto cmp = av_compare_ts(packet->pts, ofmt_ctx->streams[stream_num]->time_base, ofmt_ctx->duration / AV_TIME_BASE, r);
            if (cmp >= 0) {
                break;
            }
        }

        ret = av_interleaved_write_frame(ofmt_ctx, packet);
        if (ret < 0) {
            PLOGD << "av_interleaved_write_frame failed";
            av_packet_free(&packet);
            return -1;
        }

        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    return 0;
}

void AddAudioGenerator::free_resources() {
    for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
        avcodec_free_context(&stream_ctx[i].dec_ctx);
        if (ofmt_ctx && ofmt_ctx->nb_streams > i && ofmt_ctx->streams[i] && stream_ctx[i].enc_ctx) {
            avcodec_free_context(&stream_ctx[i].enc_ctx);
        }

        av_frame_free(&stream_ctx[i].dec_frame);
    }
    for (int i = 0; i < ifmt_audio_ctx->nb_streams; i++) {
        avcodec_free_context(&stream_audio_ctx[i].dec_ctx);
        if (ofmt_ctx && ofmt_ctx->nb_streams > i && ofmt_ctx->streams[i] && stream_audio_ctx[i].enc_ctx) {
            avcodec_free_context(&stream_audio_ctx[i].enc_ctx);
        }

        av_frame_free(&stream_audio_ctx[i].dec_frame);
    }
    av_free(stream_ctx);
    av_free(stream_audio_ctx);
    avformat_close_input(&ifmt_ctx);
    avformat_close_input(&ifmt_audio_ctx);
    if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&ofmt_ctx->pb);
    }

    avformat_free_context(ofmt_ctx);
}
