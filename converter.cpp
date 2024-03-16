#include "converter.h"

Converter::Converter()
{

}

bool Converter::convert_Format(char *src, char *dst)
{  
    bool flag = false;

    av_log_set_level(AV_LOG_ERROR);

    pkt = av_packet_alloc();
    if (!pkt) {
        fprintf(stderr, "Could not allocate AVPacket\n");
        goto end;
    }
    //open the multimedia file
    if( (ret = avformat_open_input(&pFmtCtx, src, NULL, NULL)) < 0 ){
        av_log(NULL, AV_LOG_ERROR, " %s \n", av_err2str(ret));
        goto end;
    }


    avformat_alloc_output_context2(&oFmtCtx, NULL, NULL, dst);
    if(!oFmtCtx){
        av_log(NULL, AV_LOG_ERROR, "No Memory\n");
        goto end;
    }

    stream_map = (int *)av_calloc(pFmtCtx->nb_streams, sizeof(int));
    if(!stream_map){
        av_log(NULL, AV_LOG_ERROR, "No Memory\n");
        goto end;
    }

    for (int i = 0; i < pFmtCtx->nb_streams; i++)
    {
        AVStream *outStream = NULL;
        AVStream *inStream = pFmtCtx->streams[i];
        AVCodecParameters *inCodecPar = inStream->codecpar;
        if(inCodecPar->codec_type != AVMEDIA_TYPE_AUDIO &&
            inCodecPar->codec_type != AVMEDIA_TYPE_VIDEO &&
            inCodecPar->codec_type != AVMEDIA_TYPE_SUBTITLE){
            stream_map[i] = -1;
            continue;
        }
        stream_map[i] = stream_index++;
        //create a new stream
        outStream = avformat_new_stream(oFmtCtx, NULL);
        if(!outStream){
            av_log(oFmtCtx, AV_LOG_ERROR, "No Memory!\n");
        }

        //set the arguments of output Stream
        avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
        outStream->codecpar->codec_tag = 0;
    }


    //binding
    ret = avio_open2(&oFmtCtx->pb, dst, AVIO_FLAG_WRITE, NULL, NULL);
    if(ret < 0){
        av_log(oFmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        goto end;
    }

    //write the head file of multimedia to destination file
    ret = avformat_write_header(oFmtCtx, NULL);
    if(ret < 0){
        av_log(oFmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        goto end;
    }

    //read data from multimedia files to write into destination file
    while(av_read_frame(pFmtCtx, pkt) >= 0){
        AVStream *inStream, *outStream;

        inStream = pFmtCtx->streams[pkt->stream_index];
        if(stream_map[stream_index] < 0){
            av_packet_unref(pkt);
            continue;
        }
        pkt->stream_index = stream_map[pkt->stream_index];

        outStream = oFmtCtx->streams[pkt->stream_index];
        av_packet_rescale_ts(pkt, inStream->time_base, outStream->time_base);


        pkt->pos = -1;
        av_interleaved_write_frame(oFmtCtx, pkt);
        av_packet_unref(pkt);

    }

    //write end of file
    av_write_trailer(oFmtCtx);

    flag = true;
end:
    if(pFmtCtx)
    {
        avformat_close_input(&pFmtCtx);
    }

    if(oFmtCtx)
    {
        avio_close(oFmtCtx->pb);

        avformat_free_context(oFmtCtx);

    }
    if(stream_map)
    {
        av_freep(&stream_map);
    }
    if(pkt)
    {
        av_packet_free(&pkt);
    }

    return flag;
}


Converter::~Converter()
{

}

