#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextToSpeech>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTextToSpeech *x;
};

#endif // WIDGET_H
