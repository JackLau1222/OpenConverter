#ifndef INFO_H
#define INFO_H


extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
};
#include <QWidget>
#include <map>


//store some info of video and audio
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

    //subtitle
    int subIdx;
    QString subCodec;
    QString subFmt;
    QString displayTime;
    int position;
    QSize subSize;
    QString subColor;
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

public:
    //send the info to front-end
    void send_info(char *src, QuickInfo *QuickInfo);

    //convert video color space into string
    QString enum_To_String(AVColorSpace e);

    //convert audio and video codec enum into string
    QString enum_To_String(AVCodecID e);

    //convert audio sample format enum into string
    QString enum_To_String(AVSampleFormat sample_fmt);
};

#endif // INFO_H


