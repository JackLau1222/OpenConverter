#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    bt_test=new QCheckBox("show password",this);
    le_test=new QLineEdit;
    le_test->setEchoMode(QLineEdit::Password);
    cmb_test=new QComboBox;
    cmb_test->addItem("test1");
    cmb_test->addItem("test2");
    cmb_test->addItem("test3");
    fcb_test=new QFontComboBox;
    te_test = new QTextEdit;
    sp_test=new QSpinBox;
    sp_test->setRange(20,100);


    lcd_test=new QLCDNumber;
    lcd_test->setMinimumHeight(50);
    connect(sp_test,SIGNAL(valueChanged(int)),lcd_test,SLOT(display(int)));

    dl_test=new QDial;
    connect(dl_test,SIGNAL(valueChanged(int)),lcd_test,SLOT(display(int)));

    sbr_test=new QScrollBar;
    sbr_test->setOrientation(Qt::Horizontal);
    connect(sbr_test,SIGNAL(valueChanged(int)),lcd_test,SLOT(display(int)));

    sd_test=new QSlider;
    sd_test->setOrientation(Qt::Horizontal);
    connect(sd_test,SIGNAL(valueChanged(int)),lcd_test,SLOT(display(int)));

    /*bt=new QPushButton("read",this);
    bt_tool=new QToolButton;

    bt_radio1=new QRadioButton("trun left",this);
    bt_radio2=new QRadioButton("trun right",this);

    bt_check1=new QCheckBox("setting one");
    bt_check2=new QCheckBox("setting two");
    //bt->setMinimumSize(500,500);
    //bt->setFlat(true);
    //bt->setIconSize(QSize(80,80));
    bt->setIcon(QIcon("test.JPG"));
    bt_tool->setIcon(QIcon("test.JPG"));
    bt_tool->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M ));*/
    QVBoxLayout *vbox=new QVBoxLayout;

    /*vbox->addWidget(bt);
    vbox->addWidget(bt_tool);
    vbox->addWidget(bt_radio1);
    vbox->addWidget(bt_radio2);
    vbox->addWidget(bt_check1);
    vbox->addWidget(bt_check2);*/
    vbox->addWidget(le_test);
    vbox->addWidget(bt_test);
    vbox->addWidget(cmb_test);
    vbox->addWidget(fcb_test);
    vbox->addWidget(te_test);
    vbox->addWidget(sp_test);
    vbox->addWidget(lcd_test);
    vbox->addWidget(dl_test);
    vbox->addWidget(sbr_test);
    vbox->addWidget(sd_test);
    setLayout(vbox);

    connect(bt_test,&QCheckBox::clicked,[&](bool x){
       le_test->setEchoMode(x?QLineEdit::Normal:QLineEdit::Password);
    });

    connect(cmb_test,&QComboBox::currentTextChanged,[&](QString x){
        qDebug()<<x;
    });

    connect(fcb_test,&QFontComboBox::currentFontChanged,[&](QFont x){
        le_test->setFont(x);
        te_test->setCurrentFont(x);
    });


    /*
    connect(bt_check1,&QCheckBox::clicked,[&](bool x){
      qDebug()<<"bt_check1"<<x;
    });
    connect(bt_check2,&QCheckBox::clicked,[&](bool x){
        qDebug()<<"bt_check2"<<x;
    });
    */


}

void Widget::textbutton1()
{
    qDebug("*******************1");
}


void Widget::textbutton2()
{
    qDebug("*******************2");
}

Widget::~Widget()
{

}
