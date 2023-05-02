#include "widget.h"
//#include <QLabel>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{


   image=new QLabel;
    image->setPixmap(QPixmap("test.JPG"));
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(image);
    setLayout(vbox);
}


Widget::~Widget()
{

}
