
/*
 * @file main.c
 * @author Akagi201
 * @date 2014/10/10
 *
 *
 */

#include <stdio.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#include "log.h"

// '1': Use H.264 Bitstream Filter
#define USE_H264BSF (1)

int main(int argc, char *argv[]) {
    AVFormatContext *ifmt_ctx = NULL;
    AVPacket pkt;
    int ret = 0;
    int i = 0;
    int videoindex = -1;
    int audioindex = -1;
    char *in_filename = "input.flv"; // Input File URL
    char *out_filename_v = "output.h264"; // Output File URL
    char *out_filename_a = "output.mp3";
    FILE *fp_audio = NULL;
    FILE *fp_video = NULL;

    av_register_all();

    // Input
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        LOG("Open input file failed.");
        return -1;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        LOG("Retrieve input stream info failed.");
        return -1;
    }

    videoindex = -1;

    for (i = 0; i < ifmt_ctx->nb_streams; ++i) {
        if (AVMEDIA_TYPE_VIDEO == ifmt_ctx->streams[i]->codec->codec) {
            videoindex = i;
        } else if (AVMEDIA_TYPE_AUDIO == ifmt_ctx->streams[i]->codec->codec_type) {
            audioindex = i;
        }
    }

    // Dump Format
    LOG("Input Video Start");
    av_dump_format(ifmt_ctx, 0, in_filename, 0);
    LOG("Input Video End");

    fp_audio = fopen(out_filename_a, "wb+");
    fp_video = fopen(out_filename_v, "wb+");

    /*
	FIX: H.264 in some container format (FLV, MP4, MKV etc.) need
	"h264_mp4toannexb" bitstream filter (BSF)
	  *Add SPS,PPS in front of IDR frame
	  *Add start code ("0,0,0,1") in front of NALU
	H.264 in some container (MPEG2TS) don't need this BSF.
	*/

#if USE_H264BSF
    AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
#endif

    while (av_read_frame(ifmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == videoindex) {
#if USE_H264BSF
            av_bitstream_filter_filter(h264bsfc, ifmt_ctx->streams[videoindex]->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif
            LOG("Write Video Packet. size: %d\t pts:%d\n", pkt.size, pkt.pts);
            fwrite(pkt.data, 1, pkt.size, fp_video);
        } else if (pkt.stream_index == audioindex) {
            /*
			AAC in some container format (FLV, MP4, MKV etc.) need to add 7 Bytes
			ADTS Header in front of AVPacket data manually.
			Other Audio Codec (MP3...) works well.
			*/
            LOG("Write Audio Packet. size:%d\t pts: %d\n", pkt.size, pkt.pts);
            fwrite(pkt.data, 1, pkt.size, fp_audio);
        }
        av_free_packet(&pkt);
    }

#if USE_H264BSF
    av_bitstream_filter_close(h264bsfc);
#endif

    fclose(fp_video);
    fclose(fp_audio);

    avformat_close_input(&ifmt_ctx);

    if (ret < 0 && ret != AVERROR_EOF) {
        LOG("Error occurred.");
        return -1;
    }

    return 0;
}