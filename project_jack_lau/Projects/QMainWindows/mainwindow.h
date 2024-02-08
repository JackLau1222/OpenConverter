#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void openfile();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    QTextEdit *te;
};
#endif // MAINWINDOW_H
