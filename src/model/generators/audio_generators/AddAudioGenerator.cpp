#include "AddAudioGenerator.h"
#include "plog/Log.h"
#include "../../../view_model/utils/StringFunctions.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>

};

#include <utility>
#include <string>

void AddAudioGenerator::addAudio() {
    // Writing in C is not funny :/

    int ret;
    AVPacket *packet = nullptr;
    unsigned int stream_index;
    unsigned int i;

    if ((ret = open_input_file(path_to_video_.c_str())) < 0) {
        PLOGD << "AddAudio: Couldn't open input video file";
        return;
    }
    if ((ret = open_input_audio(path_to_audio_.c_str())) < 0) {
        PLOGD << "AddAudio: Couldn't open input audio file";
        return;
    }
    if ((ret = open_output_file(path_to_output_.c_str())) < 0) {
        PLOGD << "AddAudio: Couldn't open output video file";
        return;
    }

    if (!(packet = av_packet_alloc())) {
        PLOGD << "AddAudio: Couldn't allocate packet";
        return;
    }

    while (true) {
        if ((ret = av_read_frame(ifmt_ctx, packet)) < 0) {
            break;
        }
        stream_index = packet->stream_index;
        PLOGD << StringFunctions::stringFormat("Demuxer gave frame of stream_index %u, num1: %d, num2: %d", stream_index,
                                               stream_ctx->enc_ctx->frame_number, stream_ctx->dec_ctx->frame_number);

        /* remux this frame without reencoding */
        av_packet_rescale_ts(packet,
                             ifmt_ctx->streams[stream_index]->time_base,
                             ofmt_ctx->streams[stream_index]->time_base);

        ret = av_interleaved_write_frame(ofmt_ctx, packet);
        if (ret < 0) {
            PLOGD << StringFunctions::stringFormat("Demuxer gave frame of stream_index %u", stream_index);
            av_log(NULL, AV_LOG_ERROR, "av_interleaved_write_frame failed\n");
//            goto end;
        }

        av_packet_unref(packet);
    }


    while (true) {
        if ((ret = av_read_frame(ifmt_audio_ctx, packet)) < 0) {
            break;
        }
        stream_index = packet->stream_index;
        packet->stream_index = 1;
        PLOGD << StringFunctions::stringFormat("Demuxer gave frame of stream_index %u, num1: %d, num2: %d", stream_index,
                                               stream_ctx->enc_ctx->frame_number, stream_ctx->dec_ctx->frame_number);

        /* remux this frame without reencoding */
        av_packet_rescale_ts(packet,
                             ifmt_audio_ctx->streams[stream_index]->time_base,
                             ofmt_ctx->streams[1]->time_base);

        ret = av_interleaved_write_frame(ofmt_ctx, packet);
        if (ret < 0) {
            PLOGD << StringFunctions::stringFormat("Demuxer gave frame of stream_index %u", stream_index);
            av_log(NULL, AV_LOG_ERROR, "av_interleaved_write_frame failed\n");
//            goto end;
        }

        av_packet_unref(packet);
    }

    av_write_trailer(ofmt_ctx);
}

AddAudioGenerator::~AddAudioGenerator() {

}

AddAudioGenerator::AddAudioGenerator(const std::string &path_to_video, const std::string &path_to_output,
                                     std::string path_to_audio)
        : AudioGenerator(path_to_video, path_to_output), path_to_audio_(std::move(path_to_audio)) {}


int AddAudioGenerator::open_input_file(const char *filename) {
    int ret;
    unsigned int i;

    ifmt_ctx = NULL;
    if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    stream_ctx = static_cast<StreamContext *>(av_calloc(ifmt_ctx->nb_streams, sizeof(*stream_ctx)));
    if (!stream_ctx)
        return AVERROR(ENOMEM);

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *stream = ifmt_ctx->streams[i];
        const AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        AVCodecContext *codec_ctx;
        if (!dec) {
            av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
            return AVERROR_DECODER_NOT_FOUND;
        }
        codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
            return AVERROR(ENOMEM);
        }
        ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
                                       "for stream #%u\n", i);
            return ret;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
                codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, NULL);
            /* Open decoder */
            ret = avcodec_open2(codec_ctx, dec, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
                return ret;
            }
        }
        stream_ctx[i].dec_ctx = codec_ctx;

        stream_ctx[i].dec_frame = av_frame_alloc();
        if (!stream_ctx[i].dec_frame)
            return AVERROR(ENOMEM);
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

    ofmt_ctx = NULL;
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return AVERROR_UNKNOWN;
    }

    // Video
    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            return AVERROR_UNKNOWN;
        }

        in_stream = ifmt_ctx->streams[i];
        dec_ctx = stream_ctx[i].dec_ctx;

        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* remuxing */
            encoder = avcodec_find_encoder(dec_ctx->codec_id);
            if (!encoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                return AVERROR_INVALIDDATA;
            }
            enc_ctx = avcodec_alloc_context3(encoder);
            if (!enc_ctx) {
                av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
                return AVERROR(ENOMEM);
            }

            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", i);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
            stream_ctx[i].enc_ctx = enc_ctx;
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
            return AVERROR_INVALIDDATA;
        }
    }
    // Audio
    for (i = 0; i < ifmt_audio_ctx->nb_streams; i++) {
        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
            return AVERROR_UNKNOWN;
        }

        in_stream = ifmt_audio_ctx->streams[i];
        dec_ctx = stream_audio_ctx[i].dec_ctx;

        if (dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO
            || dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* remuxing */
            encoder = avcodec_find_encoder(dec_ctx->codec_id);
            if (!encoder) {
                av_log(NULL, AV_LOG_FATAL, "Necessary encoder not found\n");
                return AVERROR_INVALIDDATA;
            }
            enc_ctx = avcodec_alloc_context3(encoder);
            if (!enc_ctx) {
                av_log(NULL, AV_LOG_FATAL, "Failed to allocate the encoder context\n");
                return AVERROR(ENOMEM);
            }

            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Copying parameters for stream #%u failed\n", i);
                return ret;
            }
            out_stream->time_base = in_stream->time_base;
            stream_audio_ctx[i].enc_ctx = enc_ctx;
        } else if (dec_ctx->codec_type == AVMEDIA_TYPE_UNKNOWN) {
            av_log(NULL, AV_LOG_FATAL, "Elementary stream #%d is of unknown type, cannot proceed\n", i);
            return AVERROR_INVALIDDATA;
        }
    }
    av_dump_format(ofmt_ctx, 0, filename, 1);

    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", filename);
            return ret;
        }
    }

    /* init muxer, write output file header */
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    return 0;
}

int AddAudioGenerator::open_input_audio(const char *filename) {
    int ret;
    unsigned int i;

    ifmt_audio_ctx = NULL;
    if ((ret = avformat_open_input(&ifmt_audio_ctx, path_to_audio_.c_str(), NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input audio file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_audio_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find audio stream information\n");
        return ret;
    }

    stream_audio_ctx = static_cast<StreamContext *>(av_calloc(ifmt_audio_ctx->nb_streams, sizeof(*stream_audio_ctx)));
    if (!stream_audio_ctx)
        return AVERROR(ENOMEM);

    for (i = 0; i < ifmt_audio_ctx->nb_streams; i++) {
        AVStream *stream = ifmt_audio_ctx->streams[i];
        const AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
        AVCodecContext *codec_ctx;
        if (!dec) {
            av_log(NULL, AV_LOG_ERROR, "Failed to find decoder for stream #%u\n", i);
            return AVERROR_DECODER_NOT_FOUND;
        }
        codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate the decoder context for stream #%u\n", i);
            return AVERROR(ENOMEM);
        }
        ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy decoder parameters to input decoder context "
                                       "for stream #%u\n", i);
            return ret;
        }
        /* Reencode video & audio and remux subtitles etc. */
        if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
            /* Open decoder */
            ret = avcodec_open2(codec_ctx, dec, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Failed to open decoder for stream #%u\n", i);
                return ret;
            }
        }
        stream_audio_ctx[i].dec_ctx = codec_ctx;

        stream_audio_ctx[i].dec_frame = av_frame_alloc();
        if (!stream_audio_ctx[i].dec_frame)
            return AVERROR(ENOMEM);
    }

    av_dump_format(ifmt_audio_ctx, 0, path_to_audio_.c_str(), 0);
    return 0;
}
