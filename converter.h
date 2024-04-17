#ifndef CONVERTER_H
#define CONVERTER_H

#endif // CONVERTER_H

#include "transcoder.h"
#include "encode_parameter.h"
#include <QString>

class Converter
{
public:
    Converter();
    Converter(ProcessParameter *processParamter, EncodeParameter *encodeParamter);
    ~Converter();

private:
    Transcoder *transcoder;

    //encoder's parameters
    bool copyVideo;
    bool copyAudio;



public:
    //void set_Encode_Param(EncodeParameter *ep);

    EncodeParameter *encodeParameter = NULL;

    bool convert_Format(QString src, QString dst);

    bool transcode(char *src, char *dst);

    ProcessParameter *get_Process_Parameter();

};
