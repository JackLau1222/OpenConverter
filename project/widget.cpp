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
    qDebug() << ui->lineEdit->text() <<"\n";

    QByteArray ba = ui->lineEdit->text().toLocal8Bit();
    char *src = ba.data();

    QuickInfo *info = new QuickInfo;
    Core *core = new Core;

    core->send_info(src, info);
    //video
    ui->label_3->setText(QString("Video : Stream %1").arg(info->videoIdx));
    ui->label_2->setText(QString("width: %1").arg(info->width));
    ui->label->setText(QString("height: %1").arg(info->height));
    ui->label_7->setText(QString("color_space: %1").arg(info->colorSpace));
    ui->label_5->setText(QString("codec: %1").arg(info->videoCodec));
    ui->label_6->setText(QString("bit_rate: %1").arg(info->videoBitRate));
    ui->label_8->setText(QString("frame_rate: %1").arg(info->frameRate));
    //audio
    ui->label_9->setText(QString("Audio : Stream %1").arg(info->audioIdx));
    ui->label_4->setText(QString("codec: %1").arg(info->audioCodec));
    ui->label_10->setText(QString("bit_rate: %1").arg(info->audioBitRate));
    ui->label_11->setText(QString("channels: %1").arg(info->channels));
    ui->label_12->setText(QString("sample_fmt: %1").arg(info->sampleFmt));
    ui->label_13->setText(QString("sample_rate: %1").arg(info->sampleRate));



}


Widget::~Widget()
{
    delete ui;
}
