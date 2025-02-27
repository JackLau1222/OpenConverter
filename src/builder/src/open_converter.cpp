#include "../include/open_converter.h"
#include "ui_open_converter.h"

OpenConverter::OpenConverter(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::OpenConverter) {
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

    // Enable drag and drop
    setAcceptDrops(true);

    /* set the converter thread */
    converter->moveToThread(&converterThread);

    connect(&converterThread, &QThread::finished, converter,
            &QObject::deleteLater);
    connect(this, &OpenConverter::activateConverterThread, converter,
            &Converter::convert_Format);
    connect(converter, &Converter::return_Value_Converter, this,
            &OpenConverter::handle_Converter_Result);

    converterThread.start();

    ui->progressBar->setValue(0);
    /* update the progress bar along with encoding  */
    connect(processParameter, &ProcessParameter::update_Process_Number, this,
            &OpenConverter::update_Process_Bar);

    connect(processParameter, &ProcessParameter::update_Time_Required, this,
            &OpenConverter::update_Time_Required);

    connect(ui->toolButton, &QToolButton::clicked, [&]() {
        QString filename = QFileDialog::getOpenFileName();
        ui->lineEdit_inputFile->setText(filename);
    });

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this,
            SLOT(apply_Pushed()));

    connect(ui->pushButton_convert, SIGNAL(clicked(bool)), this,
            SLOT(convert_Pushed()));

    connect(ui->pushButton_encodeSetting, SIGNAL(clicked(bool)), this,
            SLOT(encode_Setting_Pushed()));

    connect(ui->menuLanguage, SIGNAL(triggered(QAction *)), this,
            SLOT(slotLanguageChanged(QAction *)));

    connect(ui->menuTranscoder, SIGNAL(triggered(QAction *)), this,
            SLOT(slotTranscoderChanged(QAction *)));

    m_currLang = "english";
    m_langPath = ":/";
}

void OpenConverter::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void OpenConverter::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        const QUrl url = event->mimeData()->urls().first();
        ui->lineEdit_inputFile->setText(url.toLocalFile());
        event->acceptProposedAction();
    }
}

// Called every time, when a menu entry of the transcoder menu is called
void OpenConverter::slotTranscoderChanged(QAction *action) {
    if (0 != action) {
        std::string transcoderName = action->objectName().toStdString();
        bool isValid = false;
#ifdef ENABLE_FFMPEG
        if (transcoderName == "FFMPEG") {
            converter->set_Transcoder(transcoderName);
            isValid = true;
        }
#endif
#ifdef ENABLE_FFTOOL
        if (transcoderName == "FFTOOL") {
            converter->set_Transcoder(transcoderName);
            isValid = true;
        }
#endif
#ifdef ENABLE_BMF
        if (transcoderName == "BMF") {
            converter->set_Transcoder(transcoderName);
            isValid = true;
        }
#endif
        // If the transcoder name is not valid, log an error
        if (isValid) {
            ui->statusBar->showMessage(
                tr("Current Transcoder changed to %1")
                    .arg(QString::fromStdString(transcoderName)));
        } else {
            std::cout << "Error: Undefined transcoder name - "
                      << transcoderName.c_str() << std::endl;
        }
    }
}

// Called every time, when a menu entry of the language menu is called
void OpenConverter::slotLanguageChanged(QAction *action) {
    if (0 != action) {
        // load the language dependant on the action content
        loadLanguage(action->objectName());
        setWindowIcon(action->icon());
    }
}

void switchTranslator(QTranslator &translator, const QString &filename) {
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    //    QString path = QApplication::applicationDirPath();
    //    path.append(":/");
    if (translator.load(QString(":/%1").arg(
            filename))) // Here Path and Filename has to be entered because the
                        // system didn't find the QM Files else
        qApp->installTranslator(&translator);
}

void OpenConverter::loadLanguage(const QString &rLanguage) {
    if (m_currLang != rLanguage) {
        m_currLang = rLanguage;
        //        QLocale locale = QLocale(m_currLang);
        //        QLocale::setDefault(locale);
        //        QString languageName =
        //        QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("lang_%1.qm").arg(rLanguage));
        //        switchTranslator(m_translatorQt,
        //        QString("qt_%1.qm").arg(rLanguage));
        ui->statusBar->showMessage(
            tr("Current Language changed to %1").arg(rLanguage));
    }
}

void OpenConverter::changeEvent(QEvent *event) {
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
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QMainWindow::changeEvent(event);
}

void OpenConverter::handle_Converter_Result(bool flag) {
    if (flag) {
        displayResult->setText("Convert success!");
        ui->label_timeRequiredResult->setText(QString("%1s").arg(0));
    } else {
        displayResult->setText("Convert failed! Please ensure the file path "
                               "and encode setting is correct");
    }
    displayResult->exec();
}

void OpenConverter::update_Process_Bar(double result) {
    // static int x = 0;
    // int process = result * 100;
    int process = result;
    ui->progressBar->setValue(process);
    ui->label_processResult->setText(QString("%1%").arg(process));
}

void OpenConverter::update_Time_Required(double result) {
    ui->label_timeRequiredResult->setText(
        QString("%1s").arg(QString::number(result, 'f', 2)));
}

void OpenConverter::encode_Setting_Pushed() { encodeSetting->show(); }

void OpenConverter::apply_Pushed() {

    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
    char *src = ba.data();
    // get info by Decapsulation
    info->send_Info(src);

    // display info on window
    info_Display(info->get_Quick_Info());
}

void OpenConverter::convert_Pushed() {

    // get the input file path
    QString inputFilePath = ui->lineEdit_inputFile->text();
    // check the input file path
    if (inputFilePath.isEmpty()) {
        displayResult->setText("Please select an input file.");
        displayResult->exec();
        return;
    }
    // get the output file path
    QString outputFilePath = ui->lineEdit_outputFile->text();
    // if the output file path is empty, generate a default output filename
    if (outputFilePath.isEmpty()) {
        QFileInfo fileInfo(inputFilePath);
        outputFilePath = fileInfo.absolutePath() + "/" +
                         fileInfo.completeBaseName() + "-oc-output." +
                         fileInfo.suffix();
        ui->lineEdit_outputFile->setText(outputFilePath);
    }

    // Check if the input file and output file are the same
    if (inputFilePath == outputFilePath) {
        displayResult->setText(
            "The input file can't be the same as the output file!");
        displayResult->exec();
        return;
    }

    if (encodeSetting->get_Available()) {
        encodeSetting->get_Encode_Parameter(converter->encodeParameter);
    }

    emit activateConverterThread(ui->lineEdit_inputFile->text(),
                                 ui->lineEdit_outputFile->text());
}

void OpenConverter::info_Display(QuickInfo *quickInfo) {
    // video
    ui->label_videoStreamResult->setText(
        QString("%1").arg(quickInfo->videoIdx));
    ui->label_widthResult->setText(QString("%1").arg(quickInfo->width));
    ui->label_heightResult->setText(QString("%1").arg(quickInfo->height));
    ui->label_colorSpaceResult->setText(
        QString("%1").arg(QString::fromStdString(quickInfo->colorSpace)));
    ui->label_videoCodecResult->setText(
        QString("%1").arg(QString::fromStdString(quickInfo->videoCodec)));
    ui->label_videoBitRateResult->setText(
        QString("%1").arg(quickInfo->videoBitRate));
    ui->label_frameRateResult->setText(QString("%1").arg(quickInfo->frameRate));
    // audio
    ui->label_audioStreamResult->setText(
        QString("%1").arg(quickInfo->audioIdx));
    ui->label_audioCodecResult->setText(
        QString("%1").arg(QString::fromStdString(quickInfo->audioCodec)));
    ui->label_audioBitRateResult->setText(
        QString("%1").arg(quickInfo->audioBitRate));
    ui->label_channelsResult->setText(QString("%1").arg(quickInfo->channels));
    ui->label_sampleFmtResult->setText(
        QString("%1").arg(QString::fromStdString(quickInfo->sampleFmt)));
    ui->label_sampleRateResult->setText(
        QString("%1").arg(quickInfo->sampleRate));
}

OpenConverter::~OpenConverter() {
    /* Destory converter thread */
    converterThread.quit();
    converterThread.wait();
    // encodeSetting->close();
    delete ui;
}
