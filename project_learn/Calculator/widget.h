#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QPushButton *bt_num[10];
    QPushButton *bt_add,*bt_cut,*bt_calc,*bt_div,*bt_multi;
    QPushButton *bt_chop;
    QLineEdit *le_lcd;
    QString op;
    int data1;
    int data2;
    int sum;

public slots:
    void num_Pushed();

    void op_Pushed();

    void cal_Pushed();

    void chop_Pushed();

    void Print()
    {
        QPushButton *xbt = static_cast<QPushButton *>(sender());
        qDebug()<<xbt->text();
    }

};

#endif // WIDGET_H
