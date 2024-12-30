#include "transcoder_bmf.h"

/* Receive pointers from converter */
TranscoderBMF::TranscoderBMF(ProcessParameter *processParameter,
                       EncodeParameter *encodeParameter)
    : processParameter(processParameter), encodeParameter(encodeParameter) {
    frameTotalNumber = 0;
}

bool TranscoderBMF::prepare_info(std::string input_path, std::string output_path) {
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

    nlohmann::json video_codec = {
        "video_codec", ""
    };
    nlohmann::json audio_codec = {
        "audio_codec", ""
    };

    if (copyVideo) {
        video_codec = {
            "video_codec", "copy"
        };
    }
    if (copyAudio) {
        audio_codec = {
            "audio_codec", "copy"
        };
    }

    decoder_para = {
        {"input_path", input_path},
        video_codec,
        audio_codec,
    };

    encoder_para = {
        {"output_path", output_path},
    };

}

bool TranscoderBMF::transcode(std::string input_path, std::string output_path) {

    prepare_info(input_path, output_path);
    int scheduler_cnt = 0;

    auto graph = bmf::builder::Graph(bmf::builder::NormalMode);
    // decoder init

//    nlohmann::json decode_para = {
//        {"input_path", input_path}
//    };
    auto video = graph.Decode(bmf_sdk::JsonParam(decoder_para), "", scheduler_cnt++);

    // encoder init
//    nlohmann::json encode_para = {
//                                  {"output_path", output_path},
//                                  };
    graph.Encode(video["video"], video["audio"], bmf_sdk::JsonParam(encoder_para), "", scheduler_cnt++);

    nlohmann::json graph_para = {{"dump_graph", 1}};
    graph.SetOption(bmf_sdk::JsonParam(graph_para));

    if (graph.Run() == 0) {
        return true;
    } else {
        return false;
    }

}
