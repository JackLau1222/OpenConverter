#include "transcoder_bmf.h"

/* Receive pointers from converter */
TranscoderBMF::TranscoderBMF(ProcessParameter *processParameter,
                       EncodeParameter *encodeParamter)
    : processParameter(processParameter), encodeParamter(encodeParamter) {
    frameTotalNumber = 0;
}
bool TranscoderBMF::transcode(std::string input_path, std::string output_path) {
    int scheduler_cnt = 0;

    auto graph = bmf::builder::Graph(bmf::builder::NormalMode);
    // decoder init
    nlohmann::json decode_para = {
        {"input_path", input_path}
    };
    auto video = graph.Decode(bmf_sdk::JsonParam(decode_para), "", scheduler_cnt++);

    // encoder init
    nlohmann::json encode_para = {
                                  {"output_path", output_path},
                                  };
    graph.Encode(video["video"], video["audio"], bmf_sdk::JsonParam(encode_para), "", scheduler_cnt++);

    nlohmann::json graph_para = {{"dump_graph", 1}};
    graph.SetOption(bmf_sdk::JsonParam(graph_para));
    graph.Run();

    return true;
}
