#include "widget.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //构造所有需要的控件
    le_lcd = new QLineEdit;
    le_lcd->setAlignment(Qt::AlignRight);
    bt_cut = new QPushButton("<");

    for(int i=0; i<10; i++)
    {
        bt_num[i] = new QPushButton(QString::number(i));
    }

    bt_add = new QPushButton("+");
    bt_sub = new QPushButton("-");
    bt_mul = new QPushButton("*");
    bt_div = new QPushButton("/");
    bt_calc = new QPushButton("=");
    bt_calc->setMinimumHeight(63);

    //布局
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(le_lcd);
    //hbox->addStretch();
    hbox->addWidget(bt_cut);
    QGridLayout *gbox = new QGridLayout;
    int j = 0;
    for(int i=1; i<10; i++)
    {
       gbox->addWidget(bt_num[i], j, (i-1)%3, 1, 1);
       if(i%3==0)
           j++;
    }
    gbox->addWidget(bt_add, 0, 3, 1, 1);
    gbox->addWidget(bt_sub, 1, 3, 1, 1);
    gbox->addWidget(bt_mul, 3, 0, 1, 1);
    gbox->addWidget(bt_div, 3, 2, 1, 1);
    gbox->addWidget(bt_num[0], 3, 1, 1, 1);
    gbox->addWidget(bt_calc, 2, 3, 2, 1);
    QVBoxLayout *mainbox = new QVBoxLayout;
    mainbox->addLayout(hbox);
    mainbox->addLayout(gbox);
    setLayout(mainbox);

    //信号与槽的连接
    connect(bt_cut, SIGNAL(clicked(bool)), this, SLOT(chop_onedata()));
    for(int i = 0; i<10; i++)
        connect(bt_num[i], SIGNAL(clicked(bool)), this, SLOT(num_bt_pushed()));
    connect(bt_add, SIGNAL(clicked(bool)), this, SLOT(op_bt_pushed()));
    connect(bt_sub, SIGNAL(clicked(bool)), this, SLOT(op_bt_pushed()));
    connect(bt_mul, SIGNAL(clicked(bool)), this, SLOT(op_bt_pushed()));
    connect(bt_div, SIGNAL(clicked(bool)), this, SLOT(op_bt_pushed()));
    connect(bt_calc, SIGNAL(clicked(bool)), this, SLOT(calc_bt_pushed()));
}

/*将运算符键捕获*/
void Widget::op_bt_pushed()
{
    //1.提取按键
    QPushButton *btx = static_cast<QPushButton *>(sender());
    //2.提取文字
    op = btx->text();
    //3.保存数据1
    data1 = le_lcd->text().toInt();
    //4.清除输入框
    le_lcd->clear();
}
/*将计算键捕获*/
void Widget::calc_bt_pushed()
{
    //1.提取第二操作数
    data2 = le_lcd->text().toInt();
    //2.运算
    switch(op.toStdString().c_str()[0])
    {
    case '+': sum = data1+data2; break;
    case '-': sum = data1-data2; break;
    case '*': sum = data1*data2; break;
    case '/': sum = data1/data2; break;
    }
    //3.显示结果
    le_lcd->setText(QString::number(sum));
}

void Widget::num_bt_pushed()
{
    //1.提取按键
    QPushButton *btx = static_cast<QPushButton *>(sender());
    //2.提取文字
    QString c = btx->text();
    //3.追加显示在编辑框
    QString data = le_lcd->text();
    le_lcd->setText(data+c);
}

void Widget::chop_onedata()
{
    //1.提取文字
    QString data = le_lcd->text();
    //2.将尾部删除
    data.chop(1);
    //3.放回去
    le_lcd->setText(data);
}

Widget::~Widget()
{

}
