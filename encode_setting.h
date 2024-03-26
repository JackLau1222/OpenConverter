#ifndef ENCODE_SETTING_H
#define ENCODE_SETTING_H

#include <QMainWindow>

namespace Ui {
class EncodeSetting;
}

class EncodeSetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit EncodeSetting(QWidget *parent = nullptr);
    void set_Info(QString in, QString out);
    ~EncodeSetting();

public slots:
    void cancel_Pushed();

    void apply_Pushed();

private:
    Ui::EncodeSetting *ui;

    QString input;
    QString output;
};

#endif // ENCODE_SETTING_H
