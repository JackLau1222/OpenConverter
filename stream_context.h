#ifndef STREAMCONTEXT_H
#define STREAMCONTEXT_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
};

class StreamContext
{

public:
    StreamContext();
    ~StreamContext();

    AVFormatContext *fmtCtx;
    char *filename;

    int videoIdx;
    AVStream *videoStream;
    const AVCodec *videoCodec;
    AVCodecContext *videoCodecCtx;

    int audioIdx;
    AVStream *audioStream;
    const AVCodec *audioCodec;
    AVCodecContext *audioCodecCtx;

    AVPacket *pkt;
    AVFrame *frame;
};

#endif // STREAMCONTEXT_H
