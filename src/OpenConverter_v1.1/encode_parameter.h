#ifndef ENCODEPARAMETER_H
#define ENCODEPARAMETER_H

#include <QWidget>

class EncodeParameter
{
private:
    bool available;

    QString videoCodec;
    int64_t videoBitRate;

    QString audioCodec;
    int64_t audioBitRate;

public:
    EncodeParameter();
    ~EncodeParameter();

    bool get_Available();

    void set_Video_Codec_Name(QString vc);

    void set_Audio_Codec_Name(QString ac);

    void set_Video_Bit_Rate(int64_t vbr);

    void set_Audio_Bit_Rate(int64_t abr);

    QString get_Video_Codec_Name();

    QString get_Audio_Codec_Name();

    int64_t get_Video_Bit_Rate();

    int64_t get_Audio_Bit_Rate();
};

#endif // ENCODEPARAMETER_H
