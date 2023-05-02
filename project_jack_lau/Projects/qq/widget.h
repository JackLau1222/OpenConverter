#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QLabel *image,*usr,*pwd;
    QLineEdit *username,*password;
    QPushButton *login,*exit;




};

#endif // WIDGET_H
