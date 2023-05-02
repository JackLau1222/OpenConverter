#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    image=new QLabel;
    image->setPixmap(QPixmap("test.JPG"));
    usr=new QLabel("username",this);
    username=new QLineEdit;
    pwd=new QLabel("password",this);
    password=new QLineEdit;
    password->setEchoMode(QLineEdit::Password);
    QCheckBox *show=new QCheckBox("show",this);

    login=new QPushButton("login",this);
    exit=new QPushButton("exit",this);
    QHBoxLayout *first=new QHBoxLayout;
    first->addWidget(image);
    QHBoxLayout *user_name=new QHBoxLayout;
    user_name->addWidget(usr);
    user_name->addWidget(username);

    QHBoxLayout *pass_word=new QHBoxLayout;
    pass_word->addWidget(pwd);
    pass_word->addWidget(password);
    pass_word->addWidget(show);

    QHBoxLayout *final=new QHBoxLayout;
    final->addWidget(login);
    final->addWidget(exit);

    QVBoxLayout *mainbox=new QVBoxLayout;
    mainbox->addLayout(first);
    mainbox->addLayout(user_name);
    mainbox->addLayout(pass_word);
    mainbox->addLayout(final);
    setLayout(mainbox);

    connect(show,&QCheckBox::clicked,[&](bool x){
        password->setEchoMode(x?QLineEdit::Normal : QLineEdit::Password);
    });

    connect(login,&QPushButton::clicked,[&](){
        username->clear();
        password->clear();
    });

    connect(exit,&QPushButton::clicked,this,&Widget::close);


}

Widget::~Widget()
{

}
