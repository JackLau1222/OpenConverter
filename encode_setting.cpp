#include "encode_setting.h"
#include "ui_encode_setting.h"

EncodeSetting::EncodeSetting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EncodeSetting)
{
    ui->setupUi(this);

    encodeInfo = new EncodeInfo;

    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(cancel_Pushed()));

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
}



void EncodeSetting::cancel_Pushed()
{
    close();
}

void EncodeSetting::apply_Pushed()
{


}

EncodeSetting::~EncodeSetting()
{
    delete ui;
}
