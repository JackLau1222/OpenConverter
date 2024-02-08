#include "widget.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.resize(512+u,512+u);
    qDebug()<<w.size();
    w.show();

    return a.exec();
}
