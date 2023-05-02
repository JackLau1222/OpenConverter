#include "widget.h"
#include "QGridLayout"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    le_lcd = new QLineEdit;
    for(int i=0;i<10;i++)
    {
        bt_num[i]=new QPushButton(QString::number(i));
    }
    bt_add=new QPushButton("+",this);
    bt_cut=new QPushButton("-",this);
    bt_calc=new QPushButton("=",this);
    bt_chop=new QPushButton(">",this);
    bt_multi=new QPushButton("*",this);
    bt_div=new QPushButton("/",this);
    QGridLayout *gbox=new QGridLayout;
    gbox->addWidget(le_lcd,0,0,1,4);
    for(int i=0;i<10;i++)
    {
        if(i==0)
            gbox->addWidget(bt_num[i],4,1,1,1);
        else if(i<4)
            gbox->addWidget(bt_num[i],1,i-1,1,1);
        else if(i<7)
            gbox->addWidget(bt_num[i],2,i-4,1,1);
        else if(i<10)
            gbox->addWidget(bt_num[i],3,i-7,1,1);
    }
    gbox->addWidget(bt_add,2,3,1,1);
    gbox->addWidget(bt_cut,3,3,1,1);
    gbox->addWidget(bt_calc,4,3,1,1);
    gbox->addWidget(bt_chop,1,3,1,1);
    gbox->addWidget(bt_multi,4,0,1,1);
    gbox->addWidget(bt_div,4,2,1,1);
    setLayout(gbox);
    this->setLayout(gbox);

    for(int i=0;i<10;i++)
        connect(bt_num[i], SIGNAL(clicked(bool)), this, SLOT(num_Pushed()));
    connect(bt_add, SIGNAL(clicked(bool)), this, SLOT(op_Pushed()));
    connect(bt_cut, SIGNAL(clicked(bool)), this, SLOT(op_Pushed()));
    connect(bt_multi, SIGNAL(clicked(bool)), this, SLOT(op_Pushed()));
    connect(bt_div, SIGNAL(clicked(bool)), this, SLOT(op_Pushed()));
    connect(bt_calc, SIGNAL(clicked(bool)), this, SLOT(cal_Pushed()));
    connect(bt_chop, SIGNAL(clicked(bool)), this, SLOT(chop_Pushed()));
}

void Widget::num_Pushed()
{

    QPushButton *btx = static_cast<QPushButton *>(sender());
    QString c = btx->text();
    QString data = le_lcd->text();
    le_lcd->setText(data+c);
}

void Widget::op_Pushed()
{
    QPushButton *btx = static_cast<QPushButton *>(sender());
    op=btx->text();
    data1=le_lcd->text().toInt();
    le_lcd->clear();
}

void Widget::chop_Pushed()
{
    le_lcd->clear();
}

void Widget::cal_Pushed()
{
    data2=le_lcd->text().toInt();
    switch(op.toStdString().c_str()[0])
    {
        case '+':sum=data1+data2;break;
        case '-':sum=data1-data2;break;
        case '*':sum=data1*data2;break;
        case '/':sum=data1/data2;break;
    };
    le_lcd->setText(QString::number(sum));
}

Widget::~Widget()
{

}
