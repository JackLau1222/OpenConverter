#include "widget.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QGridLayout"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //this->setFixedSize(640,480);
    //bt=new QPushButton("login",this);
    //bt->setGeometry(300,400,100,50);
    //bt->setStyleSheet("QPushButton{background-color:#63B8FF;};");
    bt = new QPushButton("read",this);
    bt1 = new QPushButton("I'm here!",this);
    le = new QLineEdit;
    x=new QTextToSpeech;

    //QHBoxLayout *hbox = new QHBoxLayout;

    //hbox->addWidget(le);
    //hbox->addWidget(bt1);
    //hbox->addWidget(bt);
    QGridLayout *gbox=new QGridLayout;
    gbox->addWidget(le,0,0,1,2);
    gbox->addWidget(bt,1,1,1,1);
    gbox->addWidget(bt1,2,1,1,1);
    setLayout(gbox);
    this->setLayout(gbox);

    connect(bt, SIGNAL(clicked(bool)), this, SLOT(xxx()));
    connect(bt1, SIGNAL(clicked(bool)), this, SLOT(xxx()));
}

Widget::~Widget()
{

}
