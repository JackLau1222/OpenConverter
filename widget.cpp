#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(apply_Pushed()));
}

void Widget::apply_Pushed()
{

    QByteArray ba = ui->lineEdit->text().toLocal8Bit();
    char *src = ba.data();


    //get info by Decapsulation
    info->send_info(src, quickInfo);

    //display info on window
    info_Display(quickInfo);

}

void Widget::line_Text_Get()
{

}

void Widget::info_Display(QuickInfo *quickInfo)
{
    //video
    ui->label_3->setText(QString("Video : Stream %1").arg(quickInfo->videoIdx));
    ui->label_2->setText(QString("width: %1").arg(quickInfo->width));
    ui->label->setText(QString("height: %1").arg(quickInfo->height));
    ui->label_7->setText(QString("color_space: %1").arg(quickInfo->colorSpace));
    ui->label_5->setText(QString("codec: %1").arg(quickInfo->videoCodec));
    ui->label_6->setText(QString("bit_rate: %1").arg(quickInfo->videoBitRate));
    ui->label_8->setText(QString("frame_rate: %1").arg(quickInfo->frameRate));
    //audio
    ui->label_9->setText(QString("Audio : Stream %1").arg(quickInfo->audioIdx));
    ui->label_4->setText(QString("codec: %1").arg(quickInfo->audioCodec));
    ui->label_10->setText(QString("bit_rate: %1").arg(quickInfo->audioBitRate));
    ui->label_11->setText(QString("channels: %1").arg(quickInfo->channels));
    ui->label_12->setText(QString("sample_fmt: %1").arg(quickInfo->sampleFmt));
    ui->label_13->setText(QString("sample_rate: %1").arg(quickInfo->sampleRate));
}


Widget::~Widget()
{
    delete ui;
}
