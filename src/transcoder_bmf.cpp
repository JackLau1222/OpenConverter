#include "transcoder_bmf.h"

/* Receive pointers from converter */
TranscoderBMF::TranscoderBMF(ProcessParameter *processParameter,
                       EncodeParameter *encodeParameter)
    : processParameter(processParameter), encodeParameter(encodeParameter) {
    frameTotalNumber = 0;
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

    auto video = graph.Decode(bmf_sdk::JsonParam(decoder_para), "", scheduler_cnt++);

    graph.Encode(video["video"], video["audio"], bmf_sdk::JsonParam(encoder_para), "", scheduler_cnt++);

    nlohmann::json graph_para = {{"dump_graph", 1}};
    graph.SetOption(bmf_sdk::JsonParam(graph_para));

    if (graph.Run() == 0) {
        return true;
    } else {
        return false;
    }

}
