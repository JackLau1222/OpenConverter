#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QToolBox>
#include <QComboBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QGroupBox *gb1;
    QGroupBox *gb2;

    QLabel *lb_pix;

    QTextEdit *te_test1;
    QTextEdit *te_test2;
    QComboBox *cb_test;
};

#endif // WIDGET_H
