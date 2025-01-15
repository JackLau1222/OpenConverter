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

#ifndef TRANSCODER_BMF_H
#define TRANSCODER_BMF_H

#include "transcoder.h"

#include "builder.hpp"
#include "nlohmann/json.hpp"

#include <regex>

class TranscoderBMF : public Transcoder {
public:
    TranscoderBMF(ProcessParameter *processParameter,
                  EncodeParameter *encodeParamter);

    bool prepare_info(std::string input_path, std::string output_path);

    bool transcode(std::string input_path, std::string output_path);

    double compute_smooth_duration(double new_duration);

    bmf_sdk::CBytes decoder_callback(bmf_sdk::CBytes input);

    bmf_sdk::CBytes encoder_callback(bmf_sdk::CBytes input);

private:

    static int frameNumber;

    int64_t frameTotalNumber;

    // encoder's parameters
    bool copyVideo;
    bool copyAudio;

    nlohmann::json decoder_para;
    nlohmann::json encoder_para;

    // Global variable or pass-by-reference as needed
    int frame_number_total = 0;
    int frame_number_global = 0;

    int process_number = 0;
    double rest_time = 0;

    std::chrono::system_clock::time_point last_encoder_call; // Track last call time
    bool first_encoder_call = true; // Flag for first call

    std::vector<double> duration_history;  // Store recent durations for averaging
    static constexpr size_t max_history_size = 20;  // Limit for the number of durations tracked
    static constexpr double min_duration_threshold = 10.0;  // Ignore durations < 10 ms
};

#endif // TRANSCODER_BMF_H
