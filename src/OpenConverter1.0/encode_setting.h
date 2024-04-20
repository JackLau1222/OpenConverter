#ifndef ENCODE_SETTING_H
#define ENCODE_SETTING_H

#include <QMainWindow>
#include "encode_parameter.h"

namespace Ui {
class EncodeSetting;
}

class EncodeSetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit EncodeSetting(QWidget *parent = nullptr, EncodeParameter *encodeParamter = NULL);

    ~EncodeSetting();

    bool get_Encode_Parameter(EncodeParameter *ep);

    bool get_Available();

public slots:
    void cancel_Pushed();

    void apply_Pushed();

private:

    EncodeParameter *encodeParameter = NULL;

    Ui::EncodeSetting *ui;

};

#endif // ENCODE_SETTING_H
