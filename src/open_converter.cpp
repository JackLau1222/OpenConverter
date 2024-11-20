#include "open_converter.h"
#include "ui_open_converter.h"


OpenConverter::OpenConverter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OpenConverter)
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
    connect(this, &OpenConverter::activateConverterThread, converter, &Converter::convert_Format);
    connect(converter, &Converter::return_Value_Converter, this, &OpenConverter::handle_Converter_Result);

    converterThread.start();

    ui->progressBar->setValue(0);
    /* update the progress bar along with encoding  */
    connect(processParameter, &ProcessParameter::update_Process_Number, this, &OpenConverter::update_Process_Bar);

    connect(ui->toolButton, &QToolButton::clicked,[&](){
        QString filename=QFileDialog::getOpenFileName();
        ui->lineEdit_inputFile->setText(filename);
    });

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
    
    connect(ui->pushButton_convert, SIGNAL(clicked(bool)), this, SLOT(convert_Pushed()));
    
    connect(ui->pushButton_encodeSetting, SIGNAL(clicked(bool)), this, SLOT(encode_Setting_Pushed()));

    connect(ui->menuLanguage, SIGNAL(triggered(QAction *)), this, SLOT(slotLanguageChanged(QAction *)));

    m_currLang = "english";
    m_langPath = ":/";
}

// Called every time, when a menu entry of the language menu is called
void OpenConverter::slotLanguageChanged(QAction* action)
{
    if(0 != action) {
        // load the language dependant on the action content
        loadLanguage(action->objectName());
        setWindowIcon(action->icon());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename) {
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
//    QString path = QApplication::applicationDirPath();
//    path.append(":/");
    if(translator.load(QString(":/%1").arg(filename))) //Here Path and Filename has to be entered because the system didn't find the QM Files else
        qApp->installTranslator(&translator);
}

void OpenConverter::loadLanguage(const QString& rLanguage) {
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
//        QLocale locale = QLocale(m_currLang);
//        QLocale::setDefault(locale);
//        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("lang_%1.qm").arg(rLanguage));
//        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        ui->statusBar->showMessage(tr("Current Language changed to %1").arg(rLanguage));
    }
}

void OpenConverter::changeEvent(QEvent* event) {
//    if(0 != event) {
//        switch(event->type()) {
//        // this event is send if a translator is loaded
//        case QEvent::LanguageChange:
//            ui->retranslateUi(this);
//            qDebug() << "change event emitted";
//            break;

//            // this event is send, if the system, language changes
////        case QEvent::LocaleChange:
////        {
////            QString locale = QLocale::system().name();
////            locale.truncate(locale.lastIndexOf('_'));
////            loadLanguage(locale);
////        }
////        break;
//        }
//    }
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QMainWindow::changeEvent(event);
}

void OpenConverter::handle_Converter_Result(bool flag)
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

void OpenConverter::update_Process_Bar(double result)
{
    //static int x = 0;
    int process = result * 100;
    ui->progressBar->setValue(process);
    ui->label_processResult->setText(QString("%1%").arg(process));
}

void OpenConverter::encode_Setting_Pushed()
{
    encodeSetting->show();
}


void OpenConverter::apply_Pushed()
{

    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
    char *src = ba.data();
    //get info by Decapsulation
    info->send_Info(src);

    //display info on window
    info_Display(info->get_Quick_Info());

}

void OpenConverter::convert_Pushed()
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


void OpenConverter::info_Display(QuickInfo *quickInfo)
{
    //video
    ui->label_videoStreamResult->setText(QString("%1").arg(quickInfo->videoIdx));
    ui->label_widthResult->setText(QString("%1").arg(quickInfo->width));
    ui->label_heightResult->setText(QString("%1").arg(quickInfo->height));
    ui->label_colorSpaceResult->setText(QString("%1").arg(QString::fromStdString(quickInfo->colorSpace)));
    ui->label_videoCodecResult->setText(QString("%1").arg(QString::fromStdString(quickInfo->videoCodec)));
    ui->label_videoBitRateResult->setText(QString("%1").arg(quickInfo->videoBitRate));
    ui->label_frameRateResult->setText(QString("%1").arg(quickInfo->frameRate));
    //audio
    ui->label_audioStreamResult->setText(QString("%1").arg(quickInfo->audioIdx));
    ui->label_audioCodecResult->setText(QString("%1").arg(QString::fromStdString(quickInfo->audioCodec)));
    ui->label_audioBitRateResult->setText(QString("%1").arg(quickInfo->audioBitRate));
    ui->label_channelsResult->setText(QString("%1").arg(quickInfo->channels));
    ui->label_sampleFmtResult->setText(QString("%1").arg(QString::fromStdString(quickInfo->sampleFmt)));
    ui->label_sampleRateResult->setText(QString("%1").arg(quickInfo->sampleRate));
}

OpenConverter::~OpenConverter()
{
    /* Destory converter thread */
    converterThread.quit();
    converterThread.wait();
    //encodeSetting->close();
    delete ui;
}