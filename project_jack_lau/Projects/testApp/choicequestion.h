#ifndef CHOICEQUESTION_H
#define CHOICEQUESTION_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>

class choiceQuestion : public QWidget
{
    Q_OBJECT
public:
    explicit choiceQuestion(QWidget *parent = nullptr);

signals:
    //用户答题信号
    void chosed(int index);//  信号不需要实现！！！！！！（虽然长得像函数）

public slots:
    //选项点击了
    void choieClicked(bool);

    //设置题目内容
    void setQuestion(QString Question);
    //添加一个选项
    void addChoice(QString Choice);
    //添加一个图片提示
    void setPix(QString filename);

private:
    QGroupBox *gb;
    QVBoxLayout *gb_vboxg;
    QLabel *lb_pix;
    QVector<QRadioButton *> choies;
};

#endif // CHOICEQUESTION_H
