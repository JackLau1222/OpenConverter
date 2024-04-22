#include "encode_parameter.h"

EncodeParameter::EncodeParameter()
{
    videoCodec = "";
    audioCodec = "";

    videoBitRate = 0;
    audioBitRate = 0;

    available = false;
}

bool EncodeParameter::get_Available()
{
    return available;
}

void EncodeParameter::set_Video_Codec_Name(QString vc)
{
    if(vc == "")
    {
        return;
    }
    videoCodec = vc;
    available = true;
}

void EncodeParameter::set_Audio_Codec_Name(QString ac)
{
    if(ac == "")
    {
        return;
    }
    audioCodec = ac;
    available = true;
}

void EncodeParameter::set_Video_Bit_Rate(int64_t vbr)
{
    if(vbr == 0)
    {
        return;
    }
    videoBitRate = vbr;
    available = true;
}

void EncodeParameter::set_Audio_Bit_Rate(int64_t abr)
{
    if(abr == 0)
    {
        return;
    }
    audioBitRate = abr;
    available = true;
}

QString EncodeParameter::get_Video_Codec_Name()
{
    return videoCodec;
}

QString EncodeParameter::get_Audio_Codec_Name()
{
    return audioCodec;
}

int64_t EncodeParameter::get_Video_Bit_Rate()
{
    return videoBitRate;
}

int64_t EncodeParameter::get_Audio_Bit_Rate()
{
    return audioBitRate;
}


EncodeParameter::~EncodeParameter()
{

}
