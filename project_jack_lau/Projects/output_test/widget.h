#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QCalendarWidget>
#include <QLCDNumber>
#include <QProgressBar>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QLabel *lb_test;
    QLabel *lb_pix;
    QLabel *lb_gif;

    QTextBrowser *tb_test;

    QCalendarWidget *cl_test;
    QLCDNumber *lcd_test;
    QProgressBar *pbr_test;
};

#endif // WIDGET_H
