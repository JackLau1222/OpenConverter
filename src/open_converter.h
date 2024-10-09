#ifndef OPEN_CONVERTER_H
#define OPEN_CONVERTER_H

#include <QMainWindow>
#include "info.h"
#include "converter.h"
#include "encode_setting.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class OpenConverter; }
QT_END_NAMESPACE

class OpenConverter : public QMainWindow
{
    Q_OBJECT
protected:
    // this event is called, when a new translator is loaded or the system language is changed
    void changeEvent(QEvent*);
signals:
    void activateConverterThread(QString src, QString dst);
protected slots:
    void slotLanguageChanged(QAction *);
public slots:
    void apply_Pushed();

    void convert_Pushed();

    void encode_Setting_Pushed();

    void info_Display(QuickInfo *info);

    void update_Process_Bar(double result);

    void handle_Converter_Result(bool flag);
public:
    OpenConverter(QWidget *parent = nullptr);
    ~OpenConverter();

private:
    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString& rLanguage);

    QTranslator m_translator; // contains the translations for this application
//    QTranslator m_translatorQt; // contains the translations for qt
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.

    Ui::OpenConverter *ui;

    Info *info = NULL;

    EncodeParameter *encodeParameter = NULL;

    EncodeSetting *encodeSetting = NULL;

    ProcessParameter *processParameter = NULL;

    Converter *converter = NULL;

    double processNumber = 0;

    QMessageBox *displayResult = NULL;

    QThread converterThread;

};
#endif // OPEN_CONVERTER_H
