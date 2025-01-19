#include "../include/converter.h"

#if defined(USE_BMF)
    #include "../../transcoder/include/transcoder_bmf.h"
#elif defined(USE_FFMPEG)
    #include "../../transcoder/include/transcoder_ffmpeg.h"
#elif defined(USE_FFTOOL)
    #include "../../transcoder/include/transcoder_fftool.h"
#else
    #error "No transcoder selected. Please define USE_BMF or USE_FFMPEG."
#endif

Converter::Converter() {}
/* Receive pointers from widget */
Converter::Converter(ProcessParameter *processParamter,
                     EncodeParameter *encodeParamter)
    : processParameter(processParamter), encodeParameter(encodeParamter) {
    #if defined(USE_BMF)
        transcoder = new TranscoderBMF(this->processParameter, this->encodeParameter);
    #elif defined(USE_FFMPEG)
        transcoder = new TranscoderFFmpeg(this->processParameter, this->encodeParameter);
    #elif defined(USE_FFTOOL)
        transcoder = new TranscoderFFTool(this->processParameter, this->encodeParameter);
    #endif
    
    this->encodeParameter = encodeParamter;
}

void Converter::convert_Format(QString src, QString dst) {
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
    QByteArray ba = src.toLocal8Bit();
    char *sourceFileName = ba.data();

    QByteArray bb = dst.toLocal8Bit();
    char *destinationFileName = bb.data();
    //emit return_Value_Converter(transcode(sourceFileName, destinationFileName));
    emit return_Value_Converter(transcoder->transcode(src.toStdString(), dst.toStdString()));
}

Converter::~Converter() {}
