/*
 * Copyright 2024 Jack Lau
 * Email: jacklau1222gm@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONVERTER_H
#define CONVERTER_H

#include "transcoder.h"
#include "transcoder_bmf.h"
#include "encode_parameter.h"
#include <QString>
#include <QObject>

class Converter : public QObject {
    Q_OBJECT;

  public:
    Converter();
    Converter(ProcessParameter *processParamter,
              EncodeParameter *encodeParamter);
    ~Converter();

  private:
    Transcoder *transcoder = NULL;

    TranscoderBMF *transcoder_bmf = NULL;

    // encoder's parameters
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

#endif // CONVERTER_H
