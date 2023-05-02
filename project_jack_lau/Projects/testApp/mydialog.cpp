#include "mydialog.h"
#include <QVBoxLayout>
#include <QMessageBox>

myDialog::myDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    loginSuccessed = false;

    lb_logo = new QLabel("欢迎");
    lb_logo->setAlignment(Qt::AlignCenter);

    le_user = new QLineEdit;
    le_user->setPlaceholderText("用户名"); //显示提示信息
    le_pass = new QLineEdit;
    le_pass->setPlaceholderText("密码");

    bt_login = new QPushButton("进入");
    //bt_login->setEnabled(false);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(lb_logo);
    vbox->addWidget(le_user);
    vbox->addWidget(le_pass);
    vbox->addWidget(bt_login);
    setLayout(vbox);

    connect(bt_login, &QPushButton::clicked, [&](){
        if(!le_pass->text().isEmpty() && le_pass->text() == le_user->text())
        {
            loginSuccessed = true;
            close();
        }
        else
            QMessageBox::warning(this, "登陆异常","用户名或密码错误");
    });

}
