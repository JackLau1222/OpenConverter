#ifndef CONVERTER_H
#define CONVERTER_H

#endif // CONVERTER_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
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

    AVPacket pkt;

    int ret = -1;
    int idx = -1;
    int stream_index = 0;
    int *stream_map = NULL;

public:
    void convert_Format(char *src, char *dst);

};
