#include "open_converter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    OpenConverter w;
    w.setWindowTitle("Open Converter");
    w.show();
    return a.exec();
}
