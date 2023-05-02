#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class Widget : public QWidget
{
    Q_OBJECT
public slots:
    /*实现尾部字母的删除*/
    void chop_onedata();
    /*将数字键捕获*/
    void num_bt_pushed();
    /*将运算符键捕获*/
    void op_bt_pushed();
    /*将计算键捕获*/
    void calc_bt_pushed();
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QPushButton *bt_num[10];
    QPushButton *bt_cut;
    QPushButton *bt_add, *bt_sub, *bt_mul, *bt_div, *bt_calc;
    QLineEdit *le_lcd;

    QString op;
    int data1;
    int data2;
    int sum;
};

#endif // WIDGET_H
