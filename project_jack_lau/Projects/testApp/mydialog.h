#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class myDialog : public QDialog
{
public:
    myDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    bool getLoginStat() const
    {
        return loginSuccessed;
    }
    QString getUserName() const
    {
        return le_user->text();
    }
private:
    QLabel *lb_logo;
    QLineEdit *le_user;
    QLineEdit *le_pass;
    QPushButton *bt_login;

    bool loginSuccessed;
};

#endif // MYDIALOG_H
