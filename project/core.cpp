#include "core.h"


Core::Core()
{

}

QString Core::enumToString(AVColorSpace e)
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

QString Core::enumToString(AVCodecID e)
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

QString Core::enumToString(AVSampleFormat e)
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

void Core::send_info(char *src, QuickInfo *info)
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
    if((info->videoIdx = av_find_best_stream(avCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0)) < 0){
        av_log(avCtx, AV_LOG_ERROR, "There is no video stream!\n");
        goto end;
    }

    info->height = avCtx->streams[info->videoIdx]->codecpar->height;
    info->width = avCtx->streams[info->videoIdx]->codecpar->width;

    info->colorSpace = enumToString(avCtx->streams[info->videoIdx]->codecpar->color_space);

    info->videoCodec = enumToString(avCtx->streams[info->videoIdx]->codecpar->codec_id);



    info->videoBitRate = avCtx->streams[info->videoIdx]->codecpar->bit_rate;
    info->frameRate = avCtx->streams[info->videoIdx]->r_frame_rate.num/avCtx->streams[info->videoIdx]->r_frame_rate.den;

    //find the audio stream from container
    if((info->audioIdx = av_find_best_stream(avCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0)) < 0){
        av_log(avCtx, AV_LOG_ERROR, "There is no audio stream!\n");
        goto end;
    }
    audioCodec = avcodec_find_decoder(avCtx->streams[info->audioIdx]->codecpar->codec_id);
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

    info->audioCodec = enumToString(avCtx->streams[info->audioIdx]->codecpar->codec_id);
    info->audioBitRate = avCtx->streams[info->audioIdx]->codecpar->bit_rate;
    info->channels = avCtx->streams[info->audioIdx]->codecpar->channels;
    info->sampleFmt = enumToString(audioCtx->sample_fmt);
    info->sampleRate = avCtx->streams[info->audioIdx]->codecpar->sample_rate;





end:
    avformat_close_input(&avCtx);
}

Core::~Core()
{

}
