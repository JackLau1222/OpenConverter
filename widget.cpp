#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->toolButton, &QToolButton::clicked,[&](){
        QString filename=QFileDialog::getOpenFileName();
        ui->lineEdit_inputFile->setText(filename);
        //qDebug() << ui->lineEdit_inputFile->text();
    });


    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
    
    connect(ui->pushButton_convert, SIGNAL(clicked(bool)), this, SLOT(convert_Pushed()));
    
    connect(ui->pushButton_encodeSetting, SIGNAL(clicked(bool)), this, SLOT(encode_Setting_Pushed()));

}

void Widget::encode_Setting_Pushed()
{
//    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
//    char *src = ba.data();

//    QByteArray ba1 = ui->lineEdit_outputFile->text().toLocal8Bit();
//    char *dst = ba1.data();


    EncodeSetting *encodeSetting = new EncodeSetting;

    encodeSetting->show();
}


void Widget::apply_Pushed()
{

    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
    char *src = ba.data();
    //get info by Decapsulation
    info->send_info(src, quickInfo);

    //display info on window
    info_Display(quickInfo);

}

void Widget::convert_Pushed()
{
    QMessageBox displayResult;
    if(ui->lineEdit_inputFile->text() == ui->lineEdit_outputFile->text())
    {
        displayResult.setText("The input file can't same as ouput file!");
        displayResult.exec();
        return;
    }
    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
    char *src = ba.data();

    QByteArray ba1 = ui->lineEdit_outputFile->text().toLocal8Bit();
    char *dst = ba1.data();



    if(converter->convert_Format(src, dst))
    {
        displayResult.setText("Convert success!");
    }else
    {
        displayResult.setText("Convert failed! Please ensure the file path is correct");
    }
    displayResult.exec();

}


void Widget::info_Display(QuickInfo *quickInfo)
{
    //video
    ui->label_videoStream->setText(QString("Video : Stream %1").arg(quickInfo->videoIdx));
    ui->label_width->setText(QString("width: %1").arg(quickInfo->width));
    ui->label_height->setText(QString("height: %1").arg(quickInfo->height));
    ui->label_colorSpace->setText(QString("color_space: %1").arg(quickInfo->colorSpace));
    ui->label_videoCodec->setText(QString("codec: %1").arg(quickInfo->videoCodec));
    ui->label_videoBitRate->setText(QString("bit_rate: %1").arg(quickInfo->videoBitRate));
    ui->label_frameRate->setText(QString("frame_rate: %1").arg(quickInfo->frameRate));
    //audio
    ui->label_audioStream->setText(QString("Audio : Stream %1").arg(quickInfo->audioIdx));
    ui->label_audioCodec->setText(QString("codec: %1").arg(quickInfo->audioCodec));
    ui->label_audioBitRate->setText(QString("bit_rate: %1").arg(quickInfo->audioBitRate));
    ui->label_channels->setText(QString("channels: %1").arg(quickInfo->channels));
    ui->label_sampleFmt->setText(QString("sample_fmt: %1").arg(quickInfo->sampleFmt));
    ui->label_sampleRate->setText(QString("sample_rate: %1").arg(quickInfo->sampleRate));
}


Widget::~Widget()
{
    delete ui;
}
