#ifndef INFO_H
#define INFO_H

#endif // INFO_H
extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
};
#include <QWidget>
#include <map>


typedef struct QuickInfo
{
    //video
    int videoIdx;
    int width;
    int height;

    QString colorSpace;
    QString videoCodec;

    int64_t videoBitRate;
    double frameRate;
    //audio
    int audioIdx;
    QString audioCodec;
    int64_t audioBitRate;
    int channels;
    QString sampleFmt;
    int sampleRate;

    //sub
    QString codec;

}QuickInfo;

class Info
{

public:
    Info();
    ~Info();

private:
    AVFormatContext *avCtx = NULL;
    const AVCodec *audioCodec = NULL;
    AVCodecContext *audioCtx = NULL;

public:
    //send the info to front-end
    void send_info(char *src, QuickInfo *info);

    //convert video color space into string
    QString enumToString(AVColorSpace e);

    //convert audio and video codec enum into string
    QString enumToString(AVCodecID e);

    //convert audio sample format enum into string
    QString enumToString(AVSampleFormat sample_fmt);
};



