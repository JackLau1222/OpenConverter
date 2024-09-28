#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    /* init objects */
//    quickInfo = new QuickInfo;
    info = new Info;
    encodeParameter = new EncodeParameter;
    encodeSetting = new EncodeSetting(nullptr, encodeParameter);
    encodeSetting->setWindowTitle("Encode Setting");
    processParameter = new ProcessParameter;
    converter = new Converter(processParameter, encodeParameter);
    displayResult = new QMessageBox;

    ui->setupUi(this);

    /* set the converter thread */
    converter->moveToThread(&converterThread);

    connect(&converterThread, &QThread::finished, converter, &QObject::deleteLater);
    connect(this, &Widget::activateConverterThread, converter, &Converter::convert_Format);
    connect(converter, &Converter::return_Value_Converter, this, &Widget::handle_Converter_Result);

    converterThread.start();

    ui->progressBar->setValue(0);
    /* update the progress bar along with encoding  */
    connect(processParameter, &ProcessParameter::update_Process_Number, this, &Widget::update_Process_Bar);

    connect(ui->toolButton, &QToolButton::clicked,[&](){
        QString filename=QFileDialog::getOpenFileName();
        ui->lineEdit_inputFile->setText(filename);
    });

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
    
    connect(ui->pushButton_convert, SIGNAL(clicked(bool)), this, SLOT(convert_Pushed()));
    
    connect(ui->pushButton_encodeSetting, SIGNAL(clicked(bool)), this, SLOT(encode_Setting_Pushed()));


    //Init QuickInfo
//    quickInfo->videoIdx = 0;
//    quickInfo->width = 0;
//    quickInfo->height = 0;
//    quickInfo->colorSpace = "";
//    quickInfo->videoCodec = "";
//    quickInfo->videoBitRate = 0;
//    quickInfo->frameRate = 0;

//    quickInfo->audioIdx = 0;
//    quickInfo->audioCodec = "";
//    quickInfo->audioBitRate = 0;
//    quickInfo->channels = 0;
//    quickInfo->sampleFmt = "";
//    quickInfo->sampleRate = 0;

//    quickInfo->subIdx = 0;

}

void Widget::handle_Converter_Result(bool flag)
{
    if(flag)
    {
        displayResult->setText("Convert success!");
    }else
    {
        displayResult->setText("Convert failed! Please ensure the file path and encode setting is correct");
    }
    displayResult->exec();

}

void Widget::update_Process_Bar(double result)
{
    //static int x = 0;
    int process = result * 100;
    ui->progressBar->setValue(process);
    ui->label_process->setText(QString("Process: %1%").arg(process));
}

void Widget::encode_Setting_Pushed()
{
    encodeSetting->show();
}


void Widget::apply_Pushed()
{

    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
    char *src = ba.data();
    //get info by Decapsulation
    info->send_Info(src);

    //display info on window
    info_Display(info->get_Quick_Info());

}

void Widget::convert_Pushed()
{
    if(encodeSetting->get_Available())
    {
        encodeSetting->get_Encode_Parameter(converter->encodeParameter);
    }


    if(ui->lineEdit_inputFile->text() == ui->lineEdit_outputFile->text())
    {
        displayResult->setText("The input file can't same as ouput file!");
        displayResult->exec();
        return;
    }

    emit activateConverterThread(ui->lineEdit_inputFile->text(), ui->lineEdit_outputFile->text());
}


void Widget::info_Display(QuickInfo *quickInfo)
{
    //video
    ui->label_videoStream->setText(QString("Video : Stream %1").arg(quickInfo->videoIdx));
    ui->label_width->setText(QString("width: %1").arg(quickInfo->width));
    ui->label_height->setText(QString("height: %1").arg(quickInfo->height));
    ui->label_colorSpace->setText(QString("color_space: %1").arg(QString::fromStdString(quickInfo->colorSpace)));
    ui->label_videoCodec->setText(QString("codec: %1").arg(QString::fromStdString(quickInfo->videoCodec)));
    ui->label_videoBitRate->setText(QString("bit_rate: %1").arg(quickInfo->videoBitRate));
    ui->label_frameRate->setText(QString("frame_rate: %1").arg(quickInfo->frameRate));
    //audio
    ui->label_audioStream->setText(QString("Audio : Stream %1").arg(quickInfo->audioIdx));
    ui->label_audioCodec->setText(QString("codec: %1").arg(QString::fromStdString(quickInfo->audioCodec)));
    ui->label_audioBitRate->setText(QString("bit_rate: %1").arg(quickInfo->audioBitRate));
    ui->label_channels->setText(QString("channels: %1").arg(quickInfo->channels));
    ui->label_sampleFmt->setText(QString("sample_fmt: %1").arg(QString::fromStdString(quickInfo->sampleFmt)));
    ui->label_sampleRate->setText(QString("sample_rate: %1").arg(quickInfo->sampleRate));
}

void Widget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    if (encodeSetting) encodeSetting->close();
}

Widget::~Widget()
{
    /* Destory converter thread */
    converterThread.quit();
    converterThread.wait();
    //encodeSetting->close();
    delete ui;
}
