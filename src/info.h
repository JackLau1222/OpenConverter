#ifndef INFO_H
#define INFO_H


extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
};
#include <string>
#include <map>

//store some info of video and audio
typedef struct QuickInfo
{
    //video
    int videoIdx;
    int width;
    int height;

    std::string colorSpace;
    std::string videoCodec;

    int64_t videoBitRate;
    double frameRate;
    //audio
    int audioIdx;
    std::string audioCodec;
    int64_t audioBitRate;
    int channels;
    std::string sampleFmt;
    int sampleRate;

    //subtitle
    int subIdx;
    std::string subCodec;
    std::string subFmt;
    std::string displayTime;
    int position;
    //QSize subSize;
    std::string subColor;
}QuickInfo;


//deal with info of video and audio and stored as QuickInfo type
class Info
{

public:
    Info();
    ~Info();

private:
    AVFormatContext *avCtx = NULL;

    const AVCodec *audioCodec = NULL;

    AVCodecContext *audioCtx = NULL;

    QuickInfo *quickInfo = NULL;

public:
    // init quick info
    void init();
    // get qucik info reference
    QuickInfo* get_Quick_Info();
    //send the info to front-end
    void send_Info(char *src);

    //convert video color space into string
    std::string enum_To_String(AVColorSpace e);

    //convert audio and video codec enum into string
    std::string enum_To_String(AVCodecID e);

    //convert audio sample format enum into string
    std::string enum_To_String(AVSampleFormat sample_fmt);
};

#endif // INFO_H


