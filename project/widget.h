#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "info.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT


public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void apply_Pushed();

    void info_Display(QuickInfo *info);

    void line_Text_Get();



private:
    Ui::Widget *ui;
    QuickInfo *quickInfo = new QuickInfo;
    Info *info = new Info;

};
#endif // WIDGET_H
