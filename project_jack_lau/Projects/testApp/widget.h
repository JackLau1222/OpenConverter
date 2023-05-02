#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include "choicequestion.h"
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>

class Widget : public QWidget
{
    Q_OBJECT
public slots:
    void makeChoice(int);
    void endTest();
public:
    void createQuestions();
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLabel *lb_name;
    QLCDNumber *lcd_time;
    QVector<choiceQuestion *> questions;    //装所有题目显示板的容器
    QVector<int> questionAnswer;            //装所有答题信息的容器
    QProgressBar *pb_progress;
    QPushButton *bt_over;

    QTimer *t;
};

#endif // WIDGET_H
