#ifndef TRANSCODER_H
#define TRANSCODER_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
};

#include "process_parameter.h"
#include "encode_parameter.h"

#define ENCODE_BIT_RATE 5000000

typedef struct StreamContext
{
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
}StreamContext;

class Transcoder
{
public:
    Transcoder();
    Transcoder(ProcessParameter *processParameter, EncodeParameter *encodeParamter);
    ~Transcoder();

    bool open_Media(StreamContext *decoder, StreamContext *encoder);

    bool copyFrame(AVFrame *oldFrame, AVFrame *newFrame);

    bool encode_Video(AVStream *inStream, StreamContext *encoder);

    bool transcode_Video(StreamContext *decoder, StreamContext *encoder);

    bool prepare_Decoder(StreamContext *decoder);

    bool prepare_Encoder_Video(StreamContext *decoder, StreamContext *encoder);

    bool prepare_Encoder_Audio(StreamContext *decoder, StreamContext *encoder);

    bool prepare_Copy(AVFormatContext *avCtx, AVStream **stream, AVCodecParameters *codecParam);

    bool remux(AVPacket *pkt, AVFormatContext *avCtx, AVStream *inStream, AVStream *outStream);

private:
    ProcessParameter *processParameter = NULL;

    EncodeParameter *encodeParamter = NULL;

    static int frameNumber;

    int64_t frameTotalNumber;

};

#endif // TRANSCODER_H
