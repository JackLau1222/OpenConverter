#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QDebug>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QTextEdit>
#include <QSpinBox>
#include <QLCDNumber>
#include <QDial>
#include <QScrollBar>
#include <QSlider>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QPushButton *bt;
    QLineEdit *le;
    QToolButton *bt_tool;
    QRadioButton *bt_radio1,*bt_radio2;
    QCheckBox *bt_check1,*bt_check2;

    QLineEdit *le_test;
    QCheckBox *bt_test;

    QComboBox *cmb_test;
    QFontComboBox *fcb_test;
    QTextEdit *te_test;

    QSpinBox *sp_test;
    QLCDNumber *lcd_test;

    QDial *dl_test;
    QScrollBar *sbr_test;
    QSlider *sd_test;
public slots:
    void textbutton1();
    void textbutton2();
};

#endif // WIDGET_H
