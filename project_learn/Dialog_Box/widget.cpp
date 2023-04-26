#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QProgressDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    bt_filename=new QPushButton("get filename");
    bt_getcolor=new QPushButton("get color");
    bt_getfront=new QPushButton("get front");
    bt_getinput=new QPushButton("get input");
    bt_message=new QPushButton("message box");
    bt_error=new QPushButton("error box");
    bt_progress=new QPushButton("progress box");

    te_test=new QTextEdit;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(bt_filename);
    vbox->addWidget(bt_getcolor);
    vbox->addWidget(bt_getfront);
    vbox->addWidget(bt_getinput);
    vbox->addWidget(bt_message);
    vbox->addWidget(bt_error);
    vbox->addWidget(bt_progress);

    QHBoxLayout *hbox=new QHBoxLayout;
    hbox->addLayout(vbox);
    hbox->addWidget(te_test);
    setLayout(hbox);

    connect(bt_filename,&QPushButton::clicked,[&](){
        QString filename=QFileDialog::getOpenFileName();
        te_test->append(filename);
    });

    connect(bt_getcolor,&QPushButton::clicked,[&](){
       QColor color = QColorDialog::getColor();
       te_test->setTextColor(color);
    });

    connect(bt_getfront,&QPushButton::clicked,[&](){
        bool ok;
        QFont font=QFontDialog::getFont(&ok);
        if(ok)
            te_test->setCurrentFont(font);
    });

    connect(bt_getinput,&QPushButton::clicked,[&](){
       QString str=QInputDialog::getText(this,"xxxx","yyyy");
       te_test->setText(str);
    });

    connect(bt_message,&QPushButton::clicked,[&](){
       QMessageBox::warning(this,"xxxx","yyyy",QMessageBox::Open,QMessageBox::Cancel);
    });

    connect(bt_error,&QPushButton::clicked,[&](){
        QErrorMessage *x=new QErrorMessage;
        x->showMessage("xxxxxxxxxxxxxxxxxxx");
    });

    connect(bt_progress,&QPushButton::clicked,[&](){
       QProgressDialog x;
       x.setValue(88);
       x.exec();
    });

}

Widget::~Widget()
{

}
