#include "converter.h"

Converter::Converter()
{
}
/* Receive pointers from widget */
Converter::Converter(ProcessParameter *processParamter, EncodeParameter *encodeParamter)
    : processParameter(processParamter)
    , encodeParameter(encodeParamter)
{
    transcoder = new Transcoder(this->processParameter, this->encodeParameter);
    this->encodeParameter = encodeParamter;
}

void Converter::convert_Format(QString src, QString dst)
{
    if(encodeParameter->get_Video_Codec_Name() == "")
    {
        copyVideo = false;
    }else
    {
        copyVideo = false;
    }

    if(encodeParameter->get_Audio_Codec_Name() == "")
    {
        copyAudio = false;
    }else
    {
        copyAudio = false;
    }
    QByteArray ba = src.toLocal8Bit();
    char *sourceFileName = ba.data();

    QByteArray bb = dst.toLocal8Bit();
    char *destinationFileName = bb.data();
    emit return_Value_Converter(transcode(sourceFileName, destinationFileName));
}

bool Converter::transcode(char *src, char *dst)
{
    bool flag = true;
    int ret = -1;
    int stream_index = -1;
    //deal with arguments

    StreamContext *decoder = new StreamContext;
    StreamContext *encoder = new StreamContext;

    av_log_set_level(AV_LOG_DEBUG);


    decoder->filename = src;
    encoder->filename = dst;

    transcoder->open_Media(decoder, encoder);

    if(!transcoder->prepare_Decoder(decoder))
    {
        flag = false;
        goto end;
    }
    
    if(!copyVideo)
    // if(1)
    {
        if(!transcoder->prepare_Encoder_Video(decoder, encoder))
        {
            flag = false;
            goto end;
        }
    }else
    {
        transcoder->prepare_Copy(encoder->fmtCtx, &encoder->videoStream, decoder->videoStream->codecpar);
    }
    // transcoder->prepare_Encoder_Video(decoder, encoder);
    // transcoder->prepare_Encoder_Audio(decoder, encoder);

    av_log(NULL, AV_LOG_INFO, "prepare_Encoder_Audio\n");
    if(!copyAudio)
    // if(1)
    {
        if(!transcoder->prepare_Encoder_Audio(decoder, encoder))
        {
            flag = false;
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
        //av_log(encoder->fmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        flag = false;
        goto end;
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(encoder->fmtCtx, NULL);
    if (ret < 0)
    {
//        fprintf(stderr, "Error occurred when opening output file: %s\n", av_err2str(ret));
        flag = false;
        goto end;
    }

    av_log(NULL, AV_LOG_INFO, "start transcode\n");
    av_dump_format(encoder->fmtCtx, 0, encoder->filename, 1);

    //read video data from multimedia files to write into destination file
    while(av_read_frame(decoder->fmtCtx, decoder->pkt) >= 0)
    {
        stream_index = decoder->pkt->stream_index;
        // decoder->videoIdx = decoder->pkt->stream_index;
        // decoder->audioIdx = decoder->pkt->stream_index;
        if(decoder->pkt->stream_index == decoder->videoIdx )
        {
            // set packet pts
            av_packet_rescale_ts(decoder->pkt, decoder->fmtCtx->streams[stream_index]->time_base, decoder->videoCodecCtx->time_base);
            if(!copyVideo)
            {
                av_log(NULL, AV_LOG_INFO, "decoder->videoIdx:%d", decoder->videoIdx);
                transcoder->transcode_Video(decoder, encoder);
            }else
            {
                transcoder->remux(decoder->pkt, encoder->fmtCtx, decoder->videoStream, encoder->videoStream);
            }

            //encode(oFmtCtx, outCodecCtx, outFrame, outPkt, inStream, outStream);
        }else if(decoder->pkt->stream_index == decoder->audioIdx)
        {
            // set packet pts
            av_packet_rescale_ts(decoder->pkt, decoder->fmtCtx->streams[stream_index]->time_base, decoder->audioCodecCtx->time_base);
            if(!copyAudio)
            {
                av_log(NULL, AV_LOG_INFO, "decoder->audioIdx:%d", decoder->audioIdx);
                transcoder->transcode_Audio(decoder, encoder);
            }else
            {
                transcoder->remux(decoder->pkt, encoder->fmtCtx, decoder->audioStream, encoder->audioStream);
            }
        }
    }
    if(!copyVideo)
    {
        // encoder->videoFrame = NULL;
        decoder->videoFrame = NULL;
        //write the buffered frame
        transcoder->encode_Video(decoder->videoStream, decoder, encoder);
    }
    if(!copyAudio)
    {
        // encoder->audioFrame = NULL;
        decoder->audioFrame = NULL;
        //write the buffered frame
        transcoder->encode_Audio(decoder->audioStream, decoder, encoder);
    }

    processParameter->set_Process_Number(1, 1);

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
    if(decoder->audioCodecCtx)
    {
        avcodec_free_context(&decoder->audioCodecCtx);
        decoder->audioCodecCtx = NULL;
    }
    if (decoder->videoFrame)
    {
        av_frame_free(&decoder->videoFrame);
        decoder->videoFrame = NULL;
    }
    if (decoder->audioFrame)
    {
        av_frame_free(&decoder->audioFrame);
        decoder->audioFrame = NULL;
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
    if(encoder->audioCodecCtx)
    {
        avcodec_free_context(&encoder->audioCodecCtx);
        encoder->audioCodecCtx = NULL;
    }
    if(encoder->videoFrame)
    {
        av_frame_free(&encoder->videoFrame);
        encoder->videoFrame = NULL;
    }
    if(encoder->audioFrame)
    {
        av_frame_free(&encoder->audioFrame);
        encoder->audioFrame = NULL;
    }
    if(encoder->pkt)
    {
        av_packet_free(&encoder->pkt);
        encoder->pkt = NULL;
    }
    return flag;
}

Converter::~Converter()
{

}

