#ifndef CONVERTER_H
#define CONVERTER_H

#endif // CONVERTER_H

#include "transcoder.h"
#include "encode_parameter.h"
#include <QString>
#include <QObject>

class Converter : public QObject
{
    Q_OBJECT;
public:
    Converter();
    Converter(ProcessParameter *processParamter, EncodeParameter *encodeParamter);
    ~Converter();

private:
    Transcoder *transcoder;

    //encoder's parameters
    bool copyVideo;
    bool copyAudio;

public slots:
    void convert_Format(QString src, QString dst);

signals:
    void return_Value_Converter(bool flag);

public:
    ProcessParameter *processParameter = NULL;

    EncodeParameter *encodeParameter = NULL;

    bool transcode(char *src, char *dst);

};
