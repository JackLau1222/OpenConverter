#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTimer>
#include <QMessageBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //main
    QVBoxLayout *mainbox=new QVBoxLayout;
    //first line
    username=new QLineEdit;
    number=new QLineEdit;
    rest=new QLCDNumber;
    usr=new QLabel("name:");
    num=new QLabel("num:");
    QHBoxLayout *firstline=new QHBoxLayout;
    firstline->addWidget(usr);
    firstline->addWidget(username);
    firstline->addWidget(num);
    firstline->addWidget(number);
    firstline->addWidget(rest);


    //middle line
    QGroupBox *maingb=new QGroupBox;
    //1
    QGroupBox *gb1=new QGroupBox(tr("test1"));
    QRadioButton *rb11=new QRadioButton(tr("A"));
    QRadioButton *rb12=new QRadioButton(tr("B"));
    QRadioButton *rb13=new QRadioButton(tr("C"));
    QRadioButton *rb14=new QRadioButton(tr("D"));
    //rb11->setChecked(true);
    QVBoxLayout *vbox1=new QVBoxLayout;
    //vbox1->addWidget(gb1);
    vbox1->addWidget(rb11);
    vbox1->addWidget(rb12);
    vbox1->addWidget(rb13);
    vbox1->addWidget(rb14);
    vbox1->addStretch(1);
    gb1->setLayout(vbox1);
    //2
    QGroupBox *gb2=new QGroupBox(tr("test2"));
    QRadioButton *rb21=new QRadioButton(tr("A"));
    QRadioButton *rb22=new QRadioButton(tr("B"));
    QRadioButton *rb23=new QRadioButton(tr("C"));
    QRadioButton *rb24=new QRadioButton(tr("D"));
    //rb21->setChecked(true);
    QVBoxLayout *vbox2=new QVBoxLayout;

    vbox2->addWidget(rb21);
    vbox2->addWidget(rb22);
    vbox2->addWidget(rb23);
    vbox2->addWidget(rb24);
    vbox2->addStretch(1);
    gb2->setLayout(vbox2);

    //3
    QGroupBox *gb3=new QGroupBox(tr("test3"));
    QRadioButton *rb31=new QRadioButton(tr("A"));
    QRadioButton *rb32=new QRadioButton(tr("B"));
    QRadioButton *rb33=new QRadioButton(tr("C"));
    QRadioButton *rb34=new QRadioButton(tr("D"));
    //rb31->setChecked(true);
    QVBoxLayout *vbox3=new QVBoxLayout;

    vbox3->addWidget(rb31);
    vbox3->addWidget(rb32);
    vbox3->addWidget(rb33);
    vbox3->addWidget(rb34);
    vbox3->addStretch(1);
    gb3->setLayout(vbox3);
    //4
    QGroupBox *gb4=new QGroupBox(tr("test4"));
    QRadioButton *rb41=new QRadioButton(tr("A"));
    QRadioButton *rb42=new QRadioButton(tr("B"));
    QRadioButton *rb43=new QRadioButton(tr("C"));
    QRadioButton *rb44=new QRadioButton(tr("D"));
    //rb41->setChecked(true);
    QVBoxLayout *vbox4=new QVBoxLayout;

    vbox4->addWidget(rb41);
    vbox4->addWidget(rb42);
    vbox4->addWidget(rb43);
    vbox4->addWidget(rb44);
    vbox4->addStretch(1);
    gb4->setLayout(vbox4);


    QVBoxLayout *mainvbox=new QVBoxLayout;
    mainvbox->addWidget(gb1);
    mainvbox->addWidget(gb2);
    mainvbox->addWidget(gb3);
    mainvbox->addWidget(gb4);
    maingb->setLayout(mainvbox);

    QScrollArea *sl=new QScrollArea;
    sl->setFixedSize(640,480);
    sl->setWidget(maingb);

    QVBoxLayout *middlebox=new QVBoxLayout;
    middlebox->addWidget(sl);
    //setLayout(middlebox);

    //final line
    pbr=new QProgressBar;
    pbr->setValue(60);
    push=new QPushButton("push");
    QHBoxLayout *finaline=new QHBoxLayout;
    finaline->addWidget(pbr);
    finaline->addWidget(push);

    mainbox->addLayout(firstline);
    mainbox->addLayout(middlebox);
    mainbox->addLayout(finaline);
    setLayout(mainbox);

    QTimer *t=new QTimer;
    t->start(1000);
    connect(t,&QTimer::timeout,[&](){
        static int x=5;
        //pbr->setValue(x--);
            rest->display(x--);
        if(x==0)
            endTest();
    });


  }

void Widget::endTest()
{
    QMessageBox::information(this,"test",QString("info"));
}


void Widget::progress_display()
{


}


Widget::~Widget()
{

}
