#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "info.h"
#include "converter.h"
#include "encode_setting.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT


public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //void set_Process_Bar();



public slots:
    void apply_Pushed();

    void convert_Pushed();

    void encode_Setting_Pushed();

    void info_Display(QuickInfo *info);

    void update_Process_Bar();

private:
    Ui::Widget *ui;

    QuickInfo *quickInfo = new QuickInfo;

    Info *info = new Info;

    EncodeParameter *encodeParamter = new EncodeParameter;

    EncodeSetting *encodeSetting = new EncodeSetting(nullptr, encodeParamter);

    ProcessParameter *processParameter = new ProcessParameter;

    Converter *converter = new Converter(processParameter, encodeParamter);

    double processNumber = 0;

};
#endif // WIDGET_H
