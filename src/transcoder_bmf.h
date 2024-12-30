#ifndef TRANSCODER_BMF_H
#define TRANSCODER_BMF_H

#include "process_parameter.h"
#include "encode_parameter.h"
#include "stream_context.h"

#include "builder.hpp"
#include "nlohmann/json.hpp"

class TranscoderBMF {
public:
    TranscoderBMF(ProcessParameter *processParameter,
                  EncodeParameter *encodeParamter);

    bool prepare_info(std::string input_path, std::string output_path);

    bool transcode(std::string input_path, std::string output_path);

private:
    ProcessParameter *processParameter = NULL;

    EncodeParameter *encodeParameter = NULL;

    static int frameNumber;

    int64_t frameTotalNumber;

    // encoder's parameters
    bool copyVideo;
    bool copyAudio;

    nlohmann::json decoder_para;
    nlohmann::json encoder_para;
};

#endif // TRANSCODER_BMF_H
