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

#include "../../common/include/process_parameter.h"
#include "../../common/include/encode_parameter.h"
#include "../../common/include/stream_context.h"

class Transcoder {
public:
    Transcoder(ProcessParameter *processParameter, EncodeParameter *encodeParameter)
                : processParameter(processParameter), encodeParameter(encodeParameter) {}
    
    virtual ~Transcoder() = default; 

    virtual bool transcode(std::string input_path, std::string output_path) = 0;

    ProcessParameter *processParameter = NULL;

    EncodeParameter *encodeParameter = NULL;
};

#endif