#include "transcoder_bmf.h"

/* Receive pointers from converter */
TranscoderBMF::TranscoderBMF(ProcessParameter *processParameter,
                       EncodeParameter *encodeParameter)
    : processParameter(processParameter), encodeParameter(encodeParameter) {
    frameTotalNumber = 0;
}

double TranscoderBMF::compute_smooth_duration(double new_duration) {
    if (new_duration >= min_duration_threshold) {
        duration_history.push_back(new_duration);
        if (duration_history.size() > max_history_size) {
            duration_history.erase(duration_history.begin());
        }
    }
    return duration_history.empty() ? 0.0 :
               std::accumulate(duration_history.begin(), duration_history.end(), 0.0) / duration_history.size();
}

bmf_sdk::CBytes TranscoderBMF::decoder_callback(bmf_sdk::CBytes input) {
    std::string strInfo;
    strInfo.assign(reinterpret_cast<const char*>(input.buffer), input.size);
    // BMFLOG(BMF_INFO) << "====Callback==== " << strInfo;


    std::regex frame_regex(R"(\btotal frame number:\s*(\d+))");
    std::smatch match;

    if (std::regex_search(strInfo, match, frame_regex) && match.size() > 1) {
        std::istringstream(match[1]) >> frame_number_total; // Convert to int
        BMFLOG(BMF_DEBUG) << "Extracted Frame Number: " << frame_number_total;
    } else {
        BMFLOG(BMF_WARNING) << "Failed to extract frame number";
    }

    uint8_t bytes[] = {97, 98, 99, 100, 101, 0};
    return bmf_sdk::CBytes{bytes, 6};
}

bmf_sdk::CBytes TranscoderBMF::encoder_callback(bmf_sdk::CBytes input) {
    std::string strInfo;
    strInfo.assign(reinterpret_cast<const char*>(input.buffer), input.size);
    // BMFLOG(BMF_INFO) << "====Callback==== " << strInfo;

    std::regex frame_regex(R"(\bframe number:\s*(\d+))");
    std::smatch match;

    if (std::regex_search(strInfo, match, frame_regex) && match.size() > 1) {
        std::istringstream(match[1]) >> frame_number_global; // Convert to int
        BMFLOG(BMF_DEBUG) << "Extracted Total Frame Number: " << frame_number_global;
        process_number = frame_number_global * 100 / frame_number_total;

        processParameter->set_Process_Number(process_number);

        static auto last_encoder_call_time = std::chrono::system_clock::now();
        auto now = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_encoder_call_time).count();
        last_encoder_call_time = now;

        double smooth_duration = compute_smooth_duration(duration);
        if (frame_number_global > 0 && frame_number_total > 0) {
            rest_time = smooth_duration * (frame_number_total-frame_number_global) / 1000;
            processParameter->set_Time_Required(rest_time);
        }

        BMFLOG(BMF_INFO) << "Process Number (percentage): " << process_number << "%\t"
                         << "Current duration (milliseconds): " << duration << "\t"
                         << "Smoothed Duration: " << smooth_duration << " ms\t"
                         << "Estimated Rest Time (seconds): " << rest_time;



        if (frame_number_global == frame_number_total) {
            BMFLOG(BMF_INFO) << "====Callback==== Finish";
        }

    } else {
        BMFLOG(BMF_WARNING) << "Failed to extract frame number";
    }

    uint8_t bytes[] = {97, 98, 99, 100, 101, 0};
    return bmf_sdk::CBytes{bytes, 6};
}


bool TranscoderBMF::prepare_info(std::string input_path, std::string output_path) {
    // decoder init
    if (encodeParameter->get_Video_Codec_Name() == "") {
        copyVideo = true;
    } else {
        copyVideo = false;
    }

    if (encodeParameter->get_Audio_Codec_Name() == "") {
        copyAudio = true;
    } else {
        copyAudio = false;
    }

    nlohmann::json de_video_codec = {
        "video_codec", ""
    };
    nlohmann::json de_audio_codec = {
        "audio_codec", ""
    };

    if (copyVideo) {
        de_video_codec = {
            "video_codec", "copy"
        };
    }
    if (copyAudio) {
        de_audio_codec = {
            "audio_codec", "copy"
        };
    }

    decoder_para = {
        {"input_path", input_path},
        de_video_codec,
        de_audio_codec,
    };

    // encoder init
    nlohmann::json en_video_codec = {
        "codec", encodeParameter->get_Video_Codec_Name()
    };
    nlohmann::json en_audio_codec = {
        "codec", encodeParameter->get_Audio_Codec_Name()
    };
    nlohmann::json en_video_bitrate = {
        "bit_rate", encodeParameter->get_Video_Bit_Rate()
    };
    nlohmann::json en_audio_bitrate = {
        "bit_rate", encodeParameter->get_Audio_Bit_Rate()
    };

    encoder_para = {
        {"output_path", output_path},
        {
            "video_params", {
                en_video_codec,
                en_video_bitrate
            }
        },
        {
             "audio_params", {
                 en_audio_codec,
                 en_audio_bitrate
             }
        }
    };

}

bool TranscoderBMF::transcode(std::string input_path, std::string output_path) {

    prepare_info(input_path, output_path);
    int scheduler_cnt = 0;

    auto graph = bmf::builder::Graph(bmf::builder::NormalMode);

    auto decoder = graph.Decode(bmf_sdk::JsonParam(decoder_para), "", scheduler_cnt++);

    auto encoder = graph.Encode(decoder["video"], decoder["audio"], bmf_sdk::JsonParam(encoder_para), "", scheduler_cnt++);

    auto de_callback = std::bind(&TranscoderBMF::decoder_callback, this, std::placeholders::_1);
    auto en_callback = std::bind(&TranscoderBMF::encoder_callback, this, std::placeholders::_1);

    decoder.AddCallback(0, std::function<bmf_sdk::CBytes(bmf_sdk::CBytes)>(de_callback));
    encoder.AddCallback(0, std::function<bmf_sdk::CBytes(bmf_sdk::CBytes)>(en_callback));

    nlohmann::json graph_para = {{"dump_graph", 1}};
    graph.SetOption(bmf_sdk::JsonParam(graph_para));

    if (graph.Run() == 0) {
        return true;
    } else {
        return false;
    }

}
