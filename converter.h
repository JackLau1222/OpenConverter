#ifndef CONVERTER_H
#define CONVERTER_H

#endif // CONVERTER_H

#include "transcoder.h"
#include "encode_parameter.h"

class Converter
{
public:
    Converter();
    ~Converter();

private:
    //old code (remux only)
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

    //encoder's parameters
    bool copyVideo;
    bool copyAudio;



public:
    //void set_Encode_Param(EncodeParameter *ep);
    EncodeParameter *encodeParameter = new EncodeParameter;

    bool convert_Format(char *src, char *dst);

    bool transcode(char *src, char *dst);

};
