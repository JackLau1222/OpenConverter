#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QErrorMessage>


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QPushButton *bt_filename;
    QPushButton *bt_getcolor;
    QPushButton *bt_getfront;
    QPushButton *bt_getinput;
    QPushButton *bt_message;
    QPushButton *bt_error;
    QPushButton *bt_progress;

    QTextEdit *te_test;


};

#endif // WIDGET_H
