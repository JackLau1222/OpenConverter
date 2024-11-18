#ifndef ENCODEPARAMETER_H
#define ENCODEPARAMETER_H

#include <string>

class EncodeParameter {
  private:
    bool available;

    std::string videoCodec;
    int64_t videoBitRate;

    std::string audioCodec;
    int64_t audioBitRate;

  public:
    EncodeParameter();
    ~EncodeParameter();

    bool get_Available();

    void set_Video_Codec_Name(std::string vc);

    void set_Audio_Codec_Name(std::string ac);

    void set_Video_Bit_Rate(int64_t vbr);

    void set_Audio_Bit_Rate(int64_t abr);

    std::string get_Video_Codec_Name();

    std::string get_Audio_Codec_Name();

    int64_t get_Video_Bit_Rate();

    int64_t get_Audio_Bit_Rate();
};

#endif // ENCODEPARAMETER_H
