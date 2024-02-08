#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QLCDNumber>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QLabel *usr,*num;
    QLineEdit *username,*number;
    QPushButton *push;
    QProgressBar *pbr;
    QLCDNumber *rest;
    void progress_display();
    void endTest();
};

#endif // WIDGET_H
