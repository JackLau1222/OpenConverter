#ifndef CONVERTER_H
#define CONVERTER_H

#endif // CONVERTER_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
};

class Converter
{
public:
    Converter();
    ~Converter();

private:
    AVFormatContext *pFmtCtx = NULL;

    AVFormatContext *oFmtCtx = NULL;

    const AVOutputFormat *outFmt = NULL;

    AVPacket *pkt = NULL;

    int ret = -1;
    int idx = -1;
    int out_flag = -1;
    enum AVMediaType avMt = AVMEDIA_TYPE_UNKNOWN;
    int out_streams = 0;
    int stream_index = 0;
    int *stream_map = NULL;

    AVStream *inStream = NULL, *outStream = NULL;

public:
    bool convert_Format(char *src, char *dst);

    bool remux();

    bool transcode();

    bool extract();

};
