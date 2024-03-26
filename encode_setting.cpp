#include "encode_setting.h"
#include "ui_encode_setting.h"

EncodeSetting::EncodeSetting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EncodeSetting)
{
    ui->setupUi(this);

    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(cancel_Pushed()));

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
}

void EncodeSetting::set_Info(QString in, QString out)
{
    input = in;
    output = out;
}

void EncodeSetting::cancel_Pushed()
{
    qDebug() << "encode setting\n";
    close();
}

void EncodeSetting::apply_Pushed()
{
    QByteArray ba = input.toLocal8Bit();
    char *src = ba.data();

    QByteArray ba1 = output.toLocal8Bit();
    char *dst = ba1.data();


}

EncodeSetting::~EncodeSetting()
{
    delete ui;
}
