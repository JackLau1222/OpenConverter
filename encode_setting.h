#ifndef ENCODE_SETTING_H
#define ENCODE_SETTING_H

#include <QMainWindow>


typedef struct EncodeInfo
{
    QString videoCodec;
    int64_t videoBitRate;

    QString audioCodec;
    int64_t audioBitRate;

}EncodeInfo;

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
