#include "converter.h"

Converter::Converter()
{
    transcoder = new Transcoder;
}
/* Receive pointers from widget */
Converter::Converter(ProcessParameter *processParamter, EncodeParameter *encodeParamter)
{
    this->processParameter = processParamter;
    transcoder = new Transcoder(this->processParameter);
    this->encodeParameter = encodeParamter;
}

void Converter::convert_Format(QString src, QString dst)
{
    if(encodeParameter->get_Video_Codec_Name() == "")
    {
        copyVideo = true;
    }else
    {
        copyVideo = false;
    }

    if(encodeParameter->get_Audio_Codec_Name() == "")
    {
        copyAudio = true;
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
    //deal with arguments

    StreamContext *decoder = new StreamContext;
    StreamContext *encoder = new StreamContext;

    av_log_set_level(AV_LOG_DEBUG);


    decoder->filename = src;
    encoder->filename = dst;

    transcoder->open_Media(decoder, encoder);

    ret = transcoder->prepare_Decoder(decoder);
    if(ret < 0)
    {
        flag = false;
        goto end;
    }

    if(!copyVideo)
    {
        ret = transcoder->prepare_Encoder_Video(decoder, encoder);
        if(ret < 0)
        {
            flag = false;
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
        av_log(encoder->fmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        flag = false;
        goto end;
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(encoder->fmtCtx, NULL);
    if (ret < 0)
    {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        flag = false;
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
    return flag;
}

Converter::~Converter()
{

}

