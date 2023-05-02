#include "widget.h"
#include <QInputDialog>
#include <QDialog>
#include "mydialog.h"
#include <QDebug>
#include <choicequestion.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //QString str =  QInputDialog::getText(this, "登录", "请输入名字");

    myDialog *d = new myDialog;
    d->exec();
    if(!d->getLoginStat())
        exit(0);

    lb_name = new QLabel(d->getUserName());
    lcd_time = new QLCDNumber;
    t = new QTimer;
    t->start(1000);
    connect(t, &QTimer::timeout, [&](){
        static int data = 100;
        lcd_time->display(data--);
        if(data == 0)
            endTest();
    });

    createQuestions();
    QWidget *w = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    w->setLayout(vbox);
    for(int i=0; i<questions.length(); i++)
        vbox->addWidget(questions[i]);
    QScrollArea *sa = new QScrollArea;
    sa->setWidget(w);

    pb_progress = new QProgressBar;
    bt_over = new QPushButton("交卷");
    connect(bt_over, &QPushButton::clicked, this, &Widget::endTest);


    QVBoxLayout *mainbox = new QVBoxLayout;
    mainbox->addWidget(lb_name);
    mainbox->addWidget(lcd_time);
    mainbox->addWidget(sa);
    mainbox->addWidget(pb_progress);
    mainbox->addWidget(bt_over);
    setLayout(mainbox);
}

void Widget::endTest()
{
    QMessageBox::information(this, "成绩", QString("你做了%0道题").arg(questionAnswer.length()-questionAnswer.count(-1)));
}

void Widget::createQuestions()
{
    for(int i=0; i<100; i++)
    {
        choiceQuestion *t = new choiceQuestion;
        t->setQuestion(QString::number(i)+".xxxxxx");
        t->addChoice(QString("A")+" yyyy"); //QString("%0 xxxxxx").arg('A')
        t->addChoice(QString("B")+" zzzz");
        t->addChoice(QString("C")+" mmm");
        t->addChoice(QString("D")+" nnnn");
        questions.append(t);
        questionAnswer.append(-1);
        connect(t, SIGNAL(chosed(int)), this, SLOT(makeChoice(int)));
    }
}

void Widget::makeChoice(int choiceNum)
{
    choiceQuestion *t = static_cast<choiceQuestion *>( sender() );
    int questionNum = 0;
    for(; questionNum<questions.length(); questionNum++)
        if(t == questions[questionNum])
            break;

    //记录某道题已经做了
    questionAnswer[questionNum] = choiceNum;

    //显示进度
    pb_progress->setValue(questionAnswer.length()-questionAnswer.count(-1));

}

Widget::~Widget()
{

}
