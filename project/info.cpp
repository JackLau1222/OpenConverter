#include "info.h"


Info::Info()
{

}

QString Info::enumToString(AVColorSpace e)
{
    static std::map<AVColorSpace, QString> colorSpaceMap;

    // Initialize the map on first call
    if (colorSpaceMap.empty())
    {
        colorSpaceMap[AVCOL_SPC_BT709] = "AVCOL_SPC_BT709";
        colorSpaceMap[AVCOL_SPC_UNSPECIFIED] = "AVCOL_SPC_UNSPECIFIED";
        colorSpaceMap[AVCOL_SPC_RESERVED] = "AVCOL_SPC_RESERVED";
        // Add more enum-value pairs as needed
    }

    auto it = colorSpaceMap.find(e);
    if (it != colorSpaceMap.end()) {
        return it->second;
    } else {
        return "Unknown";
    }
}

QString Info::enumToString(AVCodecID e)
{
    static std::map<AVCodecID, QString> colorSpaceMap;

    if(colorSpaceMap.empty())
    {
        colorSpaceMap[AV_CODEC_ID_AAC] = "AAC";
        colorSpaceMap[AV_CODEC_ID_H264] = "H264";
    }

    auto it = colorSpaceMap.find(e);
    if (it != colorSpaceMap.end()) {
        return it->second;
    } else {
        return "Unknown";
    }
}

QString Info::enumToString(AVSampleFormat e)
{
    static std::map<AVSampleFormat, QString> colorSpaceMap;

    if(colorSpaceMap.empty())
    {
        colorSpaceMap[AV_SAMPLE_FMT_U8] = "u8";
        colorSpaceMap[AV_SAMPLE_FMT_S16] = "s16be";
        colorSpaceMap[AV_SAMPLE_FMT_S32] = "s32be";
        colorSpaceMap[AV_SAMPLE_FMT_FLT] = "f32be";
        colorSpaceMap[AV_SAMPLE_FMT_DBL] = "f64be";
        colorSpaceMap[AV_SAMPLE_FMT_FLTP] = "fltp";
    }

    auto it = colorSpaceMap.find(e);
    if (it != colorSpaceMap.end()) {
        return it->second;
    } else {
        return "Unknown";
    }
}

void Info::send_info(char *src, QuickInfo *quickInfo)
{

    int ret = 0;
    av_log_set_level(AV_LOG_DEBUG);
    ret = avformat_open_input(&avCtx, src, NULL, NULL);
    if(ret < 0)
    {
        av_log(avCtx, AV_LOG_ERROR, "open failed");
        goto end;
    }

    //find the video stream from container
    if((quickInfo->videoIdx = av_find_best_stream(avCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0)) < 0){
        av_log(avCtx, AV_LOG_ERROR, "There is no video stream!\n");
        goto end;
    }

    quickInfo->height = avCtx->streams[quickInfo->videoIdx]->codecpar->height;
    quickInfo->width = avCtx->streams[quickInfo->videoIdx]->codecpar->width;

    quickInfo->colorSpace = enumToString(avCtx->streams[quickInfo->videoIdx]->codecpar->color_space);

    quickInfo->videoCodec = enumToString(avCtx->streams[quickInfo->videoIdx]->codecpar->codec_id);



    quickInfo->videoBitRate = avCtx->streams[quickInfo->videoIdx]->codecpar->bit_rate;
    quickInfo->frameRate = avCtx->streams[quickInfo->videoIdx]->r_frame_rate.num/avCtx->streams[quickInfo->videoIdx]->r_frame_rate.den;

    //find the audio stream from container
    if((quickInfo->audioIdx = av_find_best_stream(avCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0)) < 0){
        av_log(avCtx, AV_LOG_ERROR, "There is no audio stream!\n");
        goto end;
    }
    audioCodec = avcodec_find_decoder(avCtx->streams[quickInfo->audioIdx]->codecpar->codec_id);
    if (!audioCodec) {
        av_log(NULL, AV_LOG_ERROR, "Codec not found\n");
        goto end;
    }
    audioCtx = avcodec_alloc_context3(audioCodec);
    if (!audioCtx) {
        av_log(audioCtx, AV_LOG_ERROR, "Could not allocate audio codec context\n");
        goto end;
    }
    /* open it */
    if (avcodec_open2(audioCtx, audioCodec, NULL) < 0) {
        av_log(audioCtx, AV_LOG_ERROR, "Could not open codec\n");
        goto end;
    }

    quickInfo->audioCodec = enumToString(avCtx->streams[quickInfo->audioIdx]->codecpar->codec_id);
    quickInfo->audioBitRate = avCtx->streams[quickInfo->audioIdx]->codecpar->bit_rate;
    quickInfo->channels = avCtx->streams[quickInfo->audioIdx]->codecpar->channels;
    quickInfo->sampleFmt = enumToString(audioCtx->sample_fmt);
    quickInfo->sampleRate = avCtx->streams[quickInfo->audioIdx]->codecpar->sample_rate;





end:
    avformat_close_input(&avCtx);
}

Info::~Info()
{

}
