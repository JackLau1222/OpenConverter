#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    lb_test=new QLabel("first",this);
    lb_pix=new QLabel("second",this);
    lb_gif=new QLabel("third",this);
    tb_test=new QTextBrowser;
    lcd_test=new QLCDNumber;
    lcd_test->setDigitCount(20);
    lcd_test->setMinimumSize(400,100);
    lcd_test->display(1234);
    pbr_test=new QProgressBar;
    pbr_test->setValue(60);
    QTimer *t=new QTimer;
    connect(t,&QTimer::timeout,[&](){
        static int x;
        pbr_test->setValue(x++);
        lcd_test->display(x);
    });
    t->start(100);
    cl_test=new QCalendarWidget;
    connect(cl_test,&QCalendarWidget::clicked,[&](QDate x){
        qDebug()<<x;
        lcd_test->display(x.toString());
    });



    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(lb_test);
    vbox->addWidget(lb_pix);
    vbox->addWidget(lb_gif);
    vbox->addWidget(tb_test);
    vbox->addWidget(cl_test);
    //setLayout(vbox);
    QVBoxLayout *vbox1=new QVBoxLayout;
    vbox1->addWidget(lcd_test);
    vbox1->addWidget(pbr_test);
    //setLayout(vbox1);
    QHBoxLayout *mainbox=new QHBoxLayout;
    mainbox->addLayout(vbox);
    mainbox->addLayout(vbox1);
    setLayout(mainbox);
}

Widget::~Widget()
{

}
