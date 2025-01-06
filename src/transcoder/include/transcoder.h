/*
 * Copyright 2024 Jack Lau
 * Email: jacklau1222gm@gmail.com
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

#ifndef TRANSCODER_H
#define TRANSCODER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
};

#include "../../common/include/process_parameter.h"
#include "../../common/include/encode_parameter.h"
#include "../../common/include/stream_context.h"

#define ENCODE_BIT_RATE 5000000

class Transcoder {
  public:
    Transcoder();
    Transcoder(ProcessParameter *processParameter,
               EncodeParameter *encodeParamter);
    ~Transcoder();

    bool open_Media(StreamContext *decoder, StreamContext *encoder);

    bool copyFrame(AVFrame *oldFrame, AVFrame *newFrame);

    bool encode_Video(AVStream *inStream, StreamContext *encoder);

    bool transcode_Video(StreamContext *decoder, StreamContext *encoder);

    bool prepare_Decoder(StreamContext *decoder);

    bool prepare_Encoder_Video(StreamContext *decoder, StreamContext *encoder);

    bool prepare_Encoder_Audio(StreamContext *decoder, StreamContext *encoder);

    bool prepare_Copy(AVFormatContext *avCtx, AVStream **stream,
                      AVCodecParameters *codecParam);

    bool remux(AVPacket *pkt, AVFormatContext *avCtx, AVStream *inStream,
               AVStream *outStream);

  private:
    ProcessParameter *processParameter = NULL;

    EncodeParameter *encodeParamter = NULL;

    static int frameNumber;

    int64_t frameTotalNumber;
};

#endif // TRANSCODER_H
