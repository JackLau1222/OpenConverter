#include "transcoder.h"

Transcoder::Transcoder()
{
}

int Transcoder::frameNumber = 0;

/* Receive pointers from converter */
Transcoder::Transcoder(ProcessParameter *processParameter, EncodeParameter *encodeParamter)
    : processParameter(processParameter)
    , encodeParamter(encodeParamter)
{
    frameTotalNumber = 0;
}


bool Transcoder::open_Media(StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;
    /* set the frameNumber to zero to avoid some bugs */
    frameNumber = 0;
    //open the multimedia file
    if( (ret = avformat_open_input(&decoder->fmtCtx, decoder->filename, NULL, NULL)) < 0 )
    {
        //av_log(NULL, AV_LOG_ERROR, " %s \n", av_err2str(ret));
        return -1;
    }

    ret = avformat_find_stream_info(decoder->fmtCtx, NULL);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed to retrieve input stream information\n");
        return -1;
    }

    ret = avformat_alloc_output_context2(&encoder->fmtCtx, NULL, NULL, encoder->filename);
    if (!encoder->fmtCtx)
    {
        av_log(NULL, AV_LOG_ERROR, "Could not create output context\n");
        return -1;
    }

    return true;
}

bool Transcoder::copyFrame(AVFrame* oldFrame, AVFrame* newFrame)
{
    int response;
    newFrame->pts = oldFrame->pts;
    // newFrame->format = oldFrame->format;
    newFrame->width = oldFrame->width;
    newFrame->height = oldFrame->height;
    //newFrame->channels = oldFrame->channels;
    //newFrame->channel_layout = oldFrame->channel_layout;
    // newFrame->ch_layout = oldFrame->ch_layout;
    // newFrame->nb_samples = oldFrame->nb_samples;
    response = av_frame_get_buffer(newFrame, 32);
    if (response != 0)
    {
        return false;
    }
    response = av_frame_copy(newFrame, oldFrame);
    if (response >= 0)
    {
        return false;
    }
    response = av_frame_copy_props(newFrame, oldFrame);
    if (response == 0)
    {
        return false;
    }
    return true;
}

bool Transcoder::encode_Video(AVStream *inStream, StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;
    // encoder->videoFrame = decoder->videoFrame;
    // AVPacket *enc_pkt = av_packet_alloc();

    //send frame to encoder
    ret = avcodec_send_frame(encoder->videoCodecCtx, decoder->videoFrame);
    if(ret < 0)
    {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        ret = av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        // av_packet_free(&enc_pkt);
        av_log(NULL, AV_LOG_ERROR, "Failed to send video frame to encoder! %s\n", errbuf);
        goto end;
    }


    while (ret >= 0)
    {
        ret = avcodec_receive_packet(encoder->videoCodecCtx, encoder->pkt);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return true;
        }else if(ret < 0){

            return false;
        }
        /* set the frameNumber of processParameter */
        //frameNumber = encoder->frame->pts/(inStream->time_base.den/inStream->r_frame_rate.num);

        av_log(NULL, AV_LOG_DEBUG, "calculator frame = %d\n", frameNumber);
        processParameter->set_Process_Number(frameNumber++, frameTotalNumber);

        encoder->pkt->stream_index = encoder->videoStream->index;
        encoder->pkt->duration = encoder->videoStream->time_base.den / encoder->videoStream->time_base.num / inStream->avg_frame_rate.num * inStream->avg_frame_rate.den;

        // av_packet_rescale_ts(enc_pkt, inStream->time_base, encoder->videoStream->time_base);
        av_packet_rescale_ts(encoder->pkt, encoder->videoCodecCtx->time_base, encoder->videoStream->time_base);


        ret = av_interleaved_write_frame(encoder->fmtCtx, encoder->pkt);
        if(ret < 0)
        {
            //fprintf(stderr, "Error while writing output packet: %s\n", av_err2str(ret));
        }

        av_packet_unref(encoder->pkt);
    }


end:
    return true;
}

bool Transcoder::encode_Audio(AVStream *inStream, StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;
    // encoder->audioFrame = decoder->audioFrame;
    // AVPacket *enc_pkt = av_packet_alloc();

    ret = avcodec_send_frame(encoder->audioCodecCtx, decoder->audioFrame);
    if(ret < 0)
    {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        ret = av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        // av_packet_free(&enc_pkt);
        av_log(NULL, AV_LOG_ERROR, "Failed to send audio frame to encoder! %s\n", errbuf);
        goto end;
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_packet(encoder->audioCodecCtx, encoder->pkt);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return true;
        }else if(ret < 0){

            return false;
        }
        /* set the frameNumber of processParameter */
        //frameNumber = encoder->frame->pts/(inStream->time_base.den/inStream->r_frame_rate.num);

        av_log(NULL, AV_LOG_DEBUG, "calculator frame = %d\n", frameNumber);
        processParameter->set_Process_Number(frameNumber++, frameTotalNumber);

        encoder->pkt->stream_index = encoder->audioStream->index;
        encoder->pkt->duration = encoder->audioStream->time_base.den / encoder->audioStream->time_base.num / inStream->avg_frame_rate.num * inStream->avg_frame_rate.den;

        av_packet_rescale_ts(encoder->pkt, encoder->audioCodecCtx->time_base, encoder->audioStream->time_base);

        ret = av_interleaved_write_frame(encoder->fmtCtx, encoder->pkt);
        if(ret < 0)
        {
            //fprintf(stderr, "Error while writing output packet: %s\n", av_err2str(ret));
        }

        av_packet_unref(encoder->pkt);
    }


end:
    return true;
}

bool Transcoder::transcode_Video(StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;

    //send packet to decoder
    ret = avcodec_send_packet(decoder->videoCodecCtx, decoder->pkt);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed to send video frame to decoder!\n");
        goto end;
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_frame(decoder->videoCodecCtx, decoder->videoFrame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return 0;
        }else if(ret < 0)
        {

            return -1;
        }

        // copyFrame(decoder->frame, encoder->frame);

        encode_Video(decoder->videoStream, decoder, encoder);

        if (decoder->pkt)
        {
            av_packet_unref(decoder->pkt);
        }

        av_frame_unref(decoder->videoFrame);
    }


end:
    return 0;
}

bool Transcoder::transcode_Audio(StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;

    // send packet to decoder
    ret = avcodec_send_packet(decoder->audioCodecCtx, decoder->pkt);
    if(ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed to send audio frame to decoder!\n");
        goto end;
    }

    while(ret >= 0)
    {
        ret = avcodec_receive_frame(decoder->audioCodecCtx, decoder->audioFrame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            return 0;
        }else if(ret < 0)
        {

            return -1;
        }

        //copyFrame(decoder->frame, encoder->frame);

        encode_Audio(decoder->audioStream, decoder, encoder);

        if(decoder->pkt)
        {
            av_packet_unref(decoder->pkt);
        }

        av_frame_unref(decoder->audioFrame);
    }

end:
    return 0;
}

bool Transcoder::prepare_Decoder(StreamContext *decoder)
{
    int ret = -1;

    for (int i = 0; i < decoder->fmtCtx->nb_streams; i++)
    {
        if (decoder->fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            decoder->videoStream = decoder->fmtCtx->streams[i];
            decoder->videoIdx = i;
        }else if(decoder->fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            decoder->audioStream = decoder->fmtCtx->streams[i];
            decoder->audioIdx = i;
        }

    }

    //find the decoder
    //decoder->videoCodec = avcodec_find_encoder_by_name(codec);
    //find the decoder by ID
    decoder->videoCodec = avcodec_find_decoder(decoder->videoStream->codecpar->codec_id);
    if(!decoder->videoCodec)
    {
        av_log(NULL, AV_LOG_ERROR, "Couldn't find codec: %s \n", avcodec_get_name(decoder->videoStream->codecpar->codec_id));
        //return -1;
    }
    av_log(NULL, AV_LOG_INFO, "Decoder: %s\n", avcodec_get_name(decoder->videoStream->codecpar->codec_id));

    decoder->audioCodec = avcodec_find_decoder(decoder->audioStream->codecpar->codec_id);
    if(!decoder->audioCodec)
    {
        av_log(NULL, AV_LOG_ERROR, "Couldn't find codec: %s \n", avcodec_get_name(decoder->audioStream->codecpar->codec_id));
    }
//    av_log(NULL, AV_LOG_INFO, "Decoder: %s\n", avcodec_get_name(decoder->audioStream->codecpar->codec_id));

    //init decoder context
    decoder->videoCodecCtx = avcodec_alloc_context3(decoder->videoCodec);
    if(!decoder->videoCodec)
    {
        av_log(decoder->videoCodecCtx, AV_LOG_ERROR, "No memory!\n");
        //return -1;
    }
    av_log(NULL, AV_LOG_INFO, "Successfully allocated video decoder context\n");

    decoder->audioCodecCtx = avcodec_alloc_context3(decoder->audioCodec);
    if(!decoder->audioCodec)
    {
        av_log(decoder->audioCodecCtx, AV_LOG_ERROR, "No Memory!");
    }
    av_log(NULL, AV_LOG_INFO, "Successfully allocated audio decoder context\n");

    avcodec_parameters_to_context(decoder->videoCodecCtx, decoder->videoStream->codecpar);
    avcodec_parameters_to_context(decoder->audioCodecCtx, decoder->audioStream->codecpar);

    if(decoder->videoCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        decoder->videoCodecCtx->framerate = av_guess_frame_rate(decoder->fmtCtx, decoder->videoStream, NULL);
    }

    //bind decoder and decoder context
    ret = avcodec_open2(decoder->videoCodecCtx, decoder->videoCodec, NULL);
    if(ret < 0){
        //av_log(NULL, AV_LOG_ERROR, "Couldn't open the codec: %s\n", av_err2str(ret));
        //return -1;
    }

    ret = avcodec_open2(decoder->audioCodecCtx, decoder->audioCodec, NULL);
    if(ret < 0){
        //av_log(NULL, AV_LOG_ERROR, "Couldn't open the codec: %s\n", av_err2str(ret));
        //return -1;
    }

    //create AVFrame
    decoder->videoFrame = av_frame_alloc();
    if(!decoder->videoFrame)
    {
        av_log(NULL, AV_LOG_ERROR, "No Memory!\n");
        return false;
    }

    decoder->audioFrame = av_frame_alloc();
    if(!decoder->audioFrame)
    {
        av_log(NULL, AV_LOG_ERROR, "No Memory!\n");
        return false;
    }

    //create AVPacket
    decoder->pkt = av_packet_alloc();
    if(!decoder->pkt)
    {
        av_log(NULL, AV_LOG_ERROR, "NO Memory!\n");
        return false;
    }

    return true;

    av_dump_format(decoder->fmtCtx, 0, decoder->filename, 0);
}

bool Transcoder::prepare_Encoder_Video(StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;

    /* set the total numbers of frame */
    frameTotalNumber = decoder->videoStream->nb_frames;
    /**
     * set the output file parameters
     */
    //find the encodec by Name
    // QByteArray ba = encodeParamter->get_Video_Codec_Name().toLocal8Bit();
    // const char *codec = encodeParamter->get_Video_Codec_Name().c_str();
    // encoder->videoCodec = avcodec_find_encoder_by_name(codec);
    encoder->videoCodec = avcodec_find_encoder(decoder->videoCodecCtx->codec_id);
    av_log(NULL, AV_LOG_INFO, "Encoder: %s\n", avcodec_get_name(encoder->videoCodec->id));

    //find the encodec by ID
    //encoder->videoCodec = avcodec_find_encoder(decoder->videoCodecCtx->codec_id);
    if(!encoder->videoCodec)
    {
        av_log(NULL, AV_LOG_ERROR, "Couldn't find codec: \n");
        return false;
    }
    av_log(NULL, AV_LOG_INFO, "Encoder: %s\n", avcodec_get_name(encoder->videoCodec->id));

    //init codec context
    encoder->videoCodecCtx = avcodec_alloc_context3(encoder->videoCodec);
    if(!encoder->videoCodecCtx)
    {
        av_log(NULL, AV_LOG_ERROR, "No memory!\n");
        return false;
    }
    av_log(NULL, AV_LOG_INFO, "Successfully allocated video encoder context\n");

    if(decoder->videoCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        encoder->videoCodecCtx->height = decoder->videoCodecCtx->height;
        encoder->videoCodecCtx->width = decoder->videoCodecCtx->width;
        // encoder->videoCodecCtx->bit_rate = ENCODE_BIT_RATE;
        encoder->videoCodecCtx->bit_rate = decoder->videoCodecCtx->bit_rate;
        encoder->videoCodecCtx->sample_aspect_ratio = decoder->videoCodecCtx->sample_aspect_ratio;
        //the AVCodecContext don't have framerate
        //outCodecCtx->time_base = av_inv_q(inCodecCtx->framerate);
        // encoder->videoCodecCtx->time_base = (AVRational){1, 60};
        // encoder->videoCodecCtx->framerate = (AVRational){60, 1};
        encoder->videoCodecCtx->time_base = av_inv_q(decoder->videoCodecCtx->framerate);
        encoder->videoCodecCtx->time_base = decoder->videoCodecCtx->time_base;
        // if(inCodecCtx->pix_fmt)
        //     outCodecCtx->pix_fmt = inCodecCtx->pix_fmt;
        // else
        // encoder->videoCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
//        if(encoder->videoCodecCtx->pix_fmt)
//            encoder->videoCodecCtx->pix_fmt = decoder->videoCodecCtx->pix_fmts[0];
//        else
         encoder->videoCodecCtx->pix_fmt = decoder->videoCodecCtx->pix_fmt;
        //encoder->videoCodecCtx->max_b_frames = 0;
    }

    //bind codec and codec context
    ret = avcodec_open2(encoder->videoCodecCtx, encoder->videoCodec, NULL);
    if(ret < 0)
    {
        //av_log(NULL, AV_LOG_ERROR, "Couldn't open the codec: %s\n", av_err2str(ret));
        return false;
    }

    //create AVFrame
    encoder->videoFrame = av_frame_alloc();
    if(!encoder->videoFrame)
    {
        av_log(NULL, AV_LOG_ERROR, "No Memory!\n");
        return false;
    }

    // encoder->frame->width = encoder->videoCodecCtx->width;
    // encoder->frame->height = encoder->videoCodecCtx->height;
    // encoder->frame->format = encoder->videoCodecCtx->pix_fmt;

//    ret = av_frame_get_buffer(encoder->videoFrame, 0);
//    if(ret < 0)
//    {
//        av_log(NULL, AV_LOG_ERROR, "Couldn't allocate the video frame\n");
//        return false;
//    }

    //create AVPacket
    encoder->pkt = av_packet_alloc();
    if(!encoder->pkt)
    {
        av_log(NULL, AV_LOG_ERROR, "NO Memory!\n");
        return false;
    }

    encoder->videoStream = avformat_new_stream(encoder->fmtCtx, NULL);
    if (!encoder->videoStream)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
        return false;
    }
    // encoder->videoStream->r_frame_rate = (AVRational){60, 1}; // For setting real frame rate
    // encoder->videoStream->avg_frame_rate = (AVRational){60, 1}; // For setting average frame rate
    //the input file's time_base is wrong
    encoder->videoStream->time_base = encoder->videoCodecCtx->time_base;

    ret = avcodec_parameters_from_context(encoder->videoStream->codecpar, encoder->videoCodecCtx);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream #\n");
        return false;
    }

    av_dump_format(encoder->fmtCtx, 0, encoder->filename, 1);

    return true;
}

bool Transcoder::prepare_Encoder_Audio(StreamContext *decoder, StreamContext *encoder)
{
    int ret = -1;

    frameTotalNumber = decoder->audioStream->nb_frames;

    // find the encodec by Name
    // const char *codec = encodeParamter->get_Audio_Codec_Name().c_str();
    // encoder->audioCodec = avcodec_find_encoder_by_name(codec);
    encoder->audioCodec = avcodec_find_encoder(decoder->audioCodecCtx->codec_id);
    if(!encoder->audioCodec)
    {
        // av_log(NULL, AV_LOG_ERROR, "Couldn't find codec: %s\n", codec);
        return false;
    }
    av_log(NULL, AV_LOG_INFO, "Audio encoder codec: %s\n", encoder->audioCodec->name);

    encoder->audioCodecCtx = avcodec_alloc_context3(encoder->audioCodec);
    if(!encoder->audioCodecCtx)
    {
        av_log(NULL, AV_LOG_ERROR, "No memory!\n");
        return false;
    }
    av_log(NULL, AV_LOG_INFO, "Successfully allocated audio encoder context\n");


    if(decoder->audioCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        encoder->audioCodecCtx->sample_rate = decoder->audioCodecCtx->sample_rate;
        encoder->audioCodecCtx->sample_fmt = decoder->audioCodecCtx->codec->sample_fmts[0];
        encoder->audioCodecCtx->ch_layout = decoder->audioCodecCtx->ch_layout;
        encoder->audioCodecCtx->time_base = decoder->audioCodecCtx->time_base;
    }

    ret = avcodec_open2(encoder->audioCodecCtx, encoder->audioCodec, NULL);
    if(ret < 0)
    {
        // av_log(NULL, AV_LOG_ERROR, "Couldn't open the codec: %s\n", av_err2str(ret));
        return false;
    }

    encoder->audioFrame = av_frame_alloc();
    if(!encoder->audioFrame)
    {
        av_log(NULL, AV_LOG_ERROR, "No Memory!\n");
        return false;
    }

    // encoder->frame->nb_samples = encoder->audioCodecCtx->frame_size;
    // encoder->frame->format = encoder->audioCodecCtx->sample_fmt;
    // encoder->frame->channel_layout = encoder->audioCodecCtx->ch_layout;
    
//    ret = av_frame_get_buffer(encoder->audioFrame, 0);
//    if(ret < 0)
//    {
//        av_log(NULL, AV_LOG_ERROR, "Couldn't allocate the audio frame\n");
//        return false;
//    }

    encoder->pkt = av_packet_alloc();
    if(!encoder->pkt)
    {
        av_log(NULL, AV_LOG_ERROR, "NO Memory!\n");
        return false;
    }

    encoder->audioStream = avformat_new_stream(encoder->fmtCtx, NULL);
    if (!encoder->audioStream)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed allocating output stream\n");
        return false;
    }

    ret = avcodec_parameters_from_context(encoder->audioStream->codecpar, encoder->audioCodecCtx);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Failed to copy encoder parameters to output stream #\n");
        return false;
    }


    av_dump_format(encoder->fmtCtx, 0, encoder->filename, 1);

    return true;
}

bool Transcoder::prepare_Copy(AVFormatContext *avCtx, AVStream **stream, AVCodecParameters *codecParam)
{
    av_log(NULL, AV_LOG_INFO, "prepare_Copy\n");
    *stream = avformat_new_stream(avCtx, NULL);
    avcodec_parameters_copy((*stream)->codecpar, codecParam);
    return true;
}

bool Transcoder::remux(AVPacket *pkt, AVFormatContext *avCtx, AVStream *inStream, AVStream *outStream)
{
    av_packet_rescale_ts(pkt, inStream->time_base, outStream->time_base);
    if(av_interleaved_write_frame(avCtx, pkt) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "write frame error!\n");
        return false;
    }
    return true;
}


Transcoder::~Transcoder()
{

}
