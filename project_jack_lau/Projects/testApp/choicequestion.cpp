#include "choicequestion.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QDebug>

choiceQuestion::choiceQuestion(QWidget *parent) : QWidget(parent)
{
    lb_pix = new QLabel;
    lb_pix->setMaximumSize(200, 100);
    lb_pix->setScaledContents(true);
    choies.clear();

    gb = new QGroupBox;
    gb->setMinimumWidth(640);
    gb_vboxg = new QVBoxLayout;
    gb->setLayout(gb_vboxg);
    gb_vboxg->addWidget(lb_pix);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(gb);
    setLayout(vbox);
}

//设置题目内容
void choiceQuestion::setQuestion(QString Question)
{
    gb->setTitle(Question);
}

//添加一个选项
void choiceQuestion::addChoice(QString Choice)
{
    QRadioButton *t = new QRadioButton(Choice);
    choies.append(t);
    gb_vboxg->addWidget(t);
    connect(t, SIGNAL(clicked(bool)), this, SLOT(choieClicked(bool)));
}

void choiceQuestion::choieClicked(bool x)
{
    //提取选项
    QRadioButton *t = static_cast<QRadioButton *>( sender() );

    //换算下表
    for(int index = 0;index<choies.length(); index++)
    {
       // qDebug() << int(t) << int(choies[index]);
        if(t == choies[index])
        {
            emit chosed(index); //发射信号
            break;
        }
    }

}

//添加一个图片提示
void choiceQuestion::setPix(QString filename)
{
    lb_pix->setPixmap(QPixmap(filename));
}
