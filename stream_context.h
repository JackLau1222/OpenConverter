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

    AVFormatContext *fmtCtx = NULL;
    char *filename = NULL;

    int videoIdx;
    AVStream *videoStream = NULL;
    const AVCodec *videoCodec = NULL;
    AVCodecContext *videoCodecCtx = NULL;

    int audioIdx;
    AVStream *audioStream = NULL;
    const AVCodec *audioCodec = NULL;
    AVCodecContext *audioCodecCtx = NULL;

    AVPacket *pkt = NULL;
    // AVFrame *frame = NULL;
    AVFrame *videoFrame = NULL;
    AVFrame *audioFrame = NULL;
};

#endif // STREAMCONTEXT_H
