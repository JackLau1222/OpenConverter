#include "widget.h"
#include <QTabWidget>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDebug>
#include <QStackedWidget>
#include <QMdiArea>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *maingb=new QGroupBox;

    gb1=new QGroupBox(tr("test1"));
    QRadioButton *rd11=new QRadioButton(tr("A"));
    QRadioButton *rd12=new QRadioButton(tr("B"));
    QRadioButton *rd13=new QRadioButton(tr("C"));
    QRadioButton *rd14=new QRadioButton(tr("D"));
    rd11->setChecked(true);
    QVBoxLayout *vbox1=new QVBoxLayout;
    vbox1->addWidget(rd11);
    vbox1->addWidget(rd12);
    vbox1->addWidget(rd13);
    vbox1->addWidget(rd14);
    vbox1->addStretch(1);
    gb1->setLayout(vbox1);

    gb2=new QGroupBox(tr("test2"));
    QRadioButton *rd21=new QRadioButton(tr("A"));
    QRadioButton *rd22=new QRadioButton(tr("B"));
    QRadioButton *rd23=new QRadioButton(tr("C"));
    QRadioButton *rd24=new QRadioButton(tr("D"));
    rd21->setChecked(true);
    QVBoxLayout *vbox2=new QVBoxLayout;
    vbox2->addWidget(rd21);
    vbox2->addWidget(rd22);
    vbox2->addWidget(rd23);
    vbox2->addWidget(rd24);
    vbox2->addStretch(1);
    gb2->setLayout(vbox2);

    QVBoxLayout *mainvbox=new QVBoxLayout;
    mainvbox->addWidget(gb1);
    mainvbox->addWidget(gb2);
    maingb->setLayout(mainvbox);

    QScrollArea *sl=new QScrollArea;
    sl->setFixedSize(640,480);
    sl->setWidget(maingb);

    QVBoxLayout *mainbox=new QVBoxLayout;
    mainbox->addWidget(sl);
    setLayout(mainbox);



    /*lb_pix=new QLabel;
    lb_pix->setPixmap(QPixmap("test.JPG"));
    QScrollArea *sl=new QScrollArea;
    sl->setFixedSize(640,480);
    sl->setWidget(lb_pix);
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(sl);
    setLayout(vbox);*/

    /*QLabel *image_test=new QLabel;
    image_test->setPixmap(QPixmap("test.JPG"));
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(image_test);
    setLayout(vbox);*/

    /*te_test1=new QTextEdit("xxxxxxxxxx\nxxxxxxxx\nxxxxxxxxxxxx");
    te_test2=new QTextEdit("yyyyyyyyyy\nyyyyy\nyyyyyyy");
    QToolBox *tb=new QToolBox;
    tb->addItem(te_test1,"xxxxxx1");
    tb->addItem(te_test2,"yyyyyy2");
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(tb);
    setLayout(vbox);*/

    /*te_test1=new QTextEdit("xxxxxxxxxx\nxxxxxxxx\nxxxxxxxxxxxx");
    te_test2=new QTextEdit("yyyyyyyyyy\nyyyyy\nyyyyyyy");
    QStackedWidget *sw=new QStackedWidget;
    sw->addWidget(te_test1);
    sw->addWidget(te_test2);

    cb_test=new QComboBox;
    cb_test->addItem("1.c");
    cb_test->addItem("2.c");

    connect(cb_test,SIGNAL(activated(int)),sw,SLOT(setCurrentIndex(int)));

    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(cb_test);
    vbox->addWidget(sw);

    setLayout(vbox);*/

    /*te_test1=new QTextEdit("xxxxxxxxxx\nxxxxxxxx\nxxxxxxxxxxxx");
    te_test2=new QTextEdit("yyyyyyyyyy\nyyyyy\nyyyyyyy");
    QMdiArea *qm=new QMdiArea;
    qm->addSubWindow(te_test1);
    qm->addSubWindow(te_test2);

    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(qm);
    setLayout(vbox);*/

    /*QLabel *image=new QLabel;
    image->setPixmap(QPixmap("test.JPG"));
    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(image);
    setLayout(vbox);*/



}

Widget::~Widget()
{

}
