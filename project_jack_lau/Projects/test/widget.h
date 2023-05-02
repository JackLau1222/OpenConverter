#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

#include <QLineEdit>
#include <QDebug>
#include <QTextToSpeech>

class Widget : public QWidget
{
    Q_OBJECT

public slots:
    void xxx()
    {
        QPushButton *xbt = static_cast<QPushButton *>(sender());
        qDebug()<<xbt->text();
        x->say(le->text());

    }
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QPushButton *bt;
    QPushButton *bt1;
    QLineEdit *le;
    QTextToSpeech *x;
};

#endif // WIDGET_H
