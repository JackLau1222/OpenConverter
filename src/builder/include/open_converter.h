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

#ifndef OPEN_CONVERTER_H
#define OPEN_CONVERTER_H

#include "../../common/include/info.h"
#include "../../engine/include/converter.h"
#include "encode_setting.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QThread>
#include <QTranslator>

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class OpenConverter;
}
QT_END_NAMESPACE

class OpenConverter : public QMainWindow {
    Q_OBJECT
  protected:
    // this event is called, when a new translator is loaded or the system
    // language is changed
    void changeEvent(QEvent *);

    // this event is called, when the user drags and drops a file onto the
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
  signals:
    void activateConverterThread(QString src, QString dst);
  protected slots:
    void slotLanguageChanged(QAction *);
    void slotTranscoderChanged(QAction *);
  public slots:
    void apply_Pushed();

    void convert_Pushed();

    void encode_Setting_Pushed();

    void info_Display(QuickInfo *info);

    void update_Process_Bar(double result);

    void update_Time_Required(double result);

    void handle_Converter_Result(bool flag);

  public:
    explicit OpenConverter(QWidget *parent = nullptr);
    ~OpenConverter();

  private:
    // intelligent conversion of bit rate units
    QString formatBitrate(int64_t bitsPerSec);

    // intelligent conversion of frequency units
    QString formatFrequency(int64_t hertz);

    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString &rLanguage);

    QTranslator m_translator; // contains the translations for this application
    //    QTranslator m_translatorQt; // contains the translations for qt
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to
                        // /languages.

    Ui::OpenConverter *ui;

    Info *info = NULL;

    EncodeParameter *encodeParameter = NULL;

    EncodeSetting *encodeSetting = NULL;

    ProcessParameter *processParameter = NULL;

    Converter *converter = NULL;

    double processNumber = 0;

    QMessageBox *displayResult = NULL;

    QThread converterThread;

    QString currentInputPath;

    QString currentOutputPath;
};
#endif // OPEN_CONVERTER_H
