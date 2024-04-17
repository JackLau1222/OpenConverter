#include "encode_setting.h"
#include "ui_encode_setting.h"

/* Receive pointers from widget */
EncodeSetting::EncodeSetting(QWidget *parent, EncodeParameter *encodeParamter)
    : QMainWindow(parent)
    , encodeParameter(encodeParamter)
    , ui(new Ui::EncodeSetting)
{
    ui->setupUi(this);

    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(cancel_Pushed()));

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
}

bool EncodeSetting::get_Available()
{
    return encodeParameter->get_Available();
}

bool EncodeSetting::get_Encode_Parameter(EncodeParameter *ep)
{
    ep = encodeParameter;
    if(ep != NULL)
    {
        return true;
    }

    return false;

}

void EncodeSetting::cancel_Pushed()
{
    close();
}

void EncodeSetting::apply_Pushed()
{

    encodeParameter->set_Video_Codec_Name(ui->lineEdit_videoCodec->text());
    encodeParameter->set_Video_Bit_Rate(ui->lineEdit_videoBitRate->text().toLong());

    encodeParameter->set_Audio_Codec_Name(ui->lineEdit_audioCodec->text());
    encodeParameter->set_Audio_Bit_Rate(ui->lineEdit_audioBitRate->text().toLong());

}

EncodeSetting::~EncodeSetting()
{
    delete ui;
}
