#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "info.h"
#include "converter.h"

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

    void convert_Pushed();

    void info_Display(QuickInfo *info);

private:
    Ui::Widget *ui;

    QuickInfo *quickInfo = new QuickInfo;

    Info *info = new Info;

    Converter *converter = new Converter;

};
#endif // WIDGET_H
