#include "converter.h"

Converter::Converter()
{

}

//void Converter::set_Encode_Param(EncodeParameter *ep)
//{
//    encodeParameter = ep;

//    if(encodeParameter->get_Video_Codec_Name() != "")
//    {
//        copyVideo = true;
//    }

//    if(encodeParameter->get_Audio_Codec_Name() != "")
//    {
//        copyAudio = true;
//    }
//}

bool Converter::convert_Format(char *src, char *dst)
{
    if(encodeParameter->get_Video_Codec_Name() != "")
    {
        copyVideo = true;
    }

    if(encodeParameter->get_Audio_Codec_Name() != "")
    {
        copyAudio = true;
    }

    return transcode(src, dst);

    /* temporarily unavialable */
    bool flag = false;

    av_log_set_level(AV_LOG_ERROR);

    pkt = av_packet_alloc();
    if (!pkt)
    {
        fprintf(stderr, "Could not allocate AVPacket\n");
        goto end;
    }
    //open the multimedia file
    if( (ret = avformat_open_input(&pFmtCtx, src, NULL, NULL)) < 0 )
    {
        //av_log(NULL, AV_LOG_ERROR, " %s \n", av_err2str(ret));
        goto end;
    }


    avformat_alloc_output_context2(&oFmtCtx, NULL, NULL, dst);
    if(!oFmtCtx)
    {
        av_log(NULL, AV_LOG_ERROR, "No Memory\n");
        goto end;
    }

    //get the output format
    outFmt = av_guess_format(NULL, dst, NULL);
    if(!outFmt){
        av_log(NULL, AV_LOG_ERROR, "No Memory\n");
        goto end;
    }
    oFmtCtx->oformat = outFmt;

    if(oFmtCtx->oformat->video_codec != AV_CODEC_ID_NONE)
    {
        out_streams++;
        avMt = AVMEDIA_TYPE_VIDEO;
    }

    if(oFmtCtx->oformat->audio_codec != AV_CODEC_ID_NONE)
    {
        out_streams++;
        avMt = AVMEDIA_TYPE_AUDIO;
    }

    if(oFmtCtx->oformat->subtitle_codec != AV_CODEC_ID_NONE)
    {
        out_streams++;
        avMt = AVMEDIA_TYPE_SUBTITLE;
    }

    if(out_streams < 2)
    {
        out_flag = 1;
    }

    //if(oFmtCtx->oformat->audio_codec != AV_CODEC_ID_NONE)

    stream_map = (int *)av_calloc(pFmtCtx->nb_streams, sizeof(int));
    if(!stream_map)
    {
        av_log(NULL, AV_LOG_ERROR, "No Memory\n");
        goto end;
    }



    for (int i = 0; i < pFmtCtx->nb_streams; i++)
    {
        //outStream = NULL;
        inStream = pFmtCtx->streams[i];
        AVCodecParameters *inCodecPar = inStream->codecpar;
        if(inCodecPar->codec_type != AVMEDIA_TYPE_AUDIO &&
            inCodecPar->codec_type != AVMEDIA_TYPE_VIDEO &&
            inCodecPar->codec_type != AVMEDIA_TYPE_SUBTITLE
            )
        {

            stream_map[i] = -1;
            continue;
        }

        if (inCodecPar->codec_type != avMt && out_flag > 0 )
        {
            stream_map[i] = -1;
            continue;
        }

        stream_map[i] = stream_index++;

        idx = i;

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
    if(ret < 0)
    {
        //av_log(oFmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        goto end;
    }

    //write the head file of multimedia to destination file
    ret = avformat_write_header(oFmtCtx, NULL);
    if(ret < 0)
    {
        //av_log(oFmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        goto end;
    }

    if(oFmtCtx->oformat->audio_codec == AV_CODEC_ID_NONE && stream_index < 2)
    {
        while(av_read_frame(pFmtCtx, pkt) >= 0){
            if(pkt->stream_index == idx ){
                pkt->pts = av_rescale_q_rnd(pkt->pts, inStream->time_base, outStream->time_base, static_cast<enum AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                //the dts data maybe different with the the pts if the file is video
                pkt->dts = av_rescale_q_rnd(pkt->dts, inStream->time_base, outStream->time_base, static_cast<enum AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                pkt->duration = av_rescale_q(pkt->duration, inStream->time_base, outStream->time_base);
                pkt->stream_index = 0;
                pkt->pos = -1;
                av_interleaved_write_frame(oFmtCtx, pkt);
                av_packet_unref(pkt);
            }
        }
    }else if(oFmtCtx->oformat->video_codec == AV_CODEC_ID_NONE && stream_index < 2)
    {
        //read audio data from multimedia files to write into destination file
        while(av_read_frame(pFmtCtx, pkt) >= 0){
            if(pkt->stream_index == idx ){
                pkt->pts = av_rescale_q_rnd(pkt->pts, inStream->time_base, outStream->time_base, static_cast<enum AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                //the dts data is same as the the pts if the file is audio
                pkt->dts = pkt->pts;
                pkt->duration = av_rescale_q(pkt->duration, inStream->time_base, outStream->time_base);
                pkt->stream_index = 0;
                pkt->pos = -1;
                av_interleaved_write_frame(oFmtCtx, pkt);
                av_packet_unref(pkt);
            }
        }
    }else
    {
        //read data from multimedia files to write into destination file
        while(av_read_frame(pFmtCtx, pkt) >= 0){

            AVStream *inStreamTem, *outStreamTem;

            inStreamTem = pFmtCtx->streams[pkt->stream_index];
            if(stream_map[stream_index] < 0){
                av_packet_unref(pkt);
                continue;
            }
            pkt->stream_index = stream_map[pkt->stream_index];

            outStreamTem = oFmtCtx->streams[pkt->stream_index];
            av_packet_rescale_ts(pkt, inStreamTem->time_base, outStreamTem->time_base);


            pkt->pos = -1;
            av_interleaved_write_frame(oFmtCtx, pkt);
            av_packet_unref(pkt);

        }
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

bool Converter::transcode(char *src, char *dst)
{
    int ret = -1;
    //deal with arguments
    Transcoder *transcoder = new Transcoder;
    StreamContext *decoder = new StreamContext;
    StreamContext *encoder = new StreamContext;

    av_log_set_level(AV_LOG_DEBUG);


    decoder->filename = src;
    encoder->filename = dst;

    transcoder->open_Media(decoder, encoder);

    ret = transcoder->prepare_Decoder(decoder);
    if(ret < 0)
    {
        goto end;
    }

    if(!copyVideo)
    {
        ret = transcoder->prepare_Encoder_Video(decoder, encoder);
        if(ret < 0)
        {
            goto end;
        }
    }else
    {
        transcoder->prepare_Copy(encoder->fmtCtx, &encoder->videoStream, decoder->videoStream->codecpar);
    }

    if(!copyAudio)
    {
        ret = transcoder->prepare_Encoder_Audio(decoder, encoder);
        if(ret < 0)
        {
            goto end;
        }
    }else
    {
        transcoder->prepare_Copy(encoder->fmtCtx, &encoder->audioStream, decoder->audioStream->codecpar);
    }

    //binding
    ret = avio_open2(&encoder->fmtCtx->pb, encoder->filename, AVIO_FLAG_WRITE, NULL, NULL);
    if(ret < 0)
    {
        av_log(encoder->fmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        return -1;
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(encoder->fmtCtx, NULL);
    if (ret < 0)
    {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        goto end;
    }

    //read video data from multimedia files to write into destination file
    while(av_read_frame(decoder->fmtCtx, decoder->pkt) >= 0)
    {
        if(decoder->pkt->stream_index == decoder->videoIdx )
        {
            if(!copyVideo)
            {
                transcoder->transcode_Video(decoder, encoder);
            }else
            {
                transcoder->remux(decoder->pkt, encoder->fmtCtx, decoder->videoStream, encoder->videoStream);
            }

            //encode(oFmtCtx, outCodecCtx, outFrame, outPkt, inStream, outStream);
        }else if(decoder->pkt->stream_index == decoder->audioIdx)
        {
            if(!copyAudio)
            {

            }else
            {
                transcoder->remux(decoder->pkt, encoder->fmtCtx, decoder->audioStream, encoder->audioStream);
            }
        }
    }
    if(!copyVideo)
    {
        encoder->frame = NULL;
        //write the buffered frame
        transcoder->encode_Video(decoder->videoStream, encoder);

    }

    av_write_trailer(encoder->fmtCtx);

//free memory
end:
    if(decoder->fmtCtx)
    {
        avformat_close_input(&decoder->fmtCtx);
        decoder->fmtCtx = NULL;
    }
    if(decoder->videoCodecCtx)
    {
        avcodec_free_context(&decoder->videoCodecCtx);
        decoder->videoCodecCtx = NULL;
    }
    if (decoder->frame)
    {
        av_frame_free(&decoder->frame);
        decoder->frame = NULL;
    }
    if (decoder->pkt)
    {
        av_packet_free(&decoder->pkt);
        decoder->pkt = NULL;
    }

    if(encoder->fmtCtx && !(encoder->fmtCtx->oformat->flags & AVFMT_NOFILE))
    {
        avio_closep(&encoder->fmtCtx->pb);
    }
    if(encoder->fmtCtx)
    {
        avformat_free_context(encoder->fmtCtx);
        encoder->fmtCtx = NULL;
    }
    if(encoder->videoCodecCtx)
    {
        avcodec_free_context(&encoder->videoCodecCtx);
        encoder->videoCodecCtx = NULL;
    }
    if(encoder->frame)
    {
        av_frame_free(&encoder->frame);
        encoder->frame = NULL;
    }
    if(encoder->pkt)
    {
        av_packet_free(&encoder->pkt);
        encoder->pkt = NULL;
    }
    return 0;
}

Converter::~Converter()
{

}

