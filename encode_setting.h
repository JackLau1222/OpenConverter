#ifndef ENCODE_SETTING_H
#define ENCODE_SETTING_H

#include <QMainWindow>
#include "info.h"

namespace Ui {
class EncodeSetting;
}

class EncodeSetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit EncodeSetting(QWidget *parent = nullptr);
    ~EncodeSetting();

public slots:
    void cancel_Pushed();

    void apply_Pushed();

private:
    Ui::EncodeSetting *ui;

    EncodeInfo *encodeInfo;
};

#endif // ENCODE_SETTING_H
