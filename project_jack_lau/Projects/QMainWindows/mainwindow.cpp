#include "mainwindow.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QFontComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*QAction *test=new QAction("open");
    test->setIcon(QIcon("test.JPG"));
    test->setShortcut(QKeySequence("CTRL+P"));

    QMenu *menu=menuBar()->addMenu("File");
    menu->addAction(test);

    connect(test,SIGNAL(trigglered(bool)),this,SLOT(openfile()));*/
    QAction *actnew = new QAction("新建");
    actnew->setIcon(QIcon(":/img/new.png"));
    actnew->setShortcut(QKeySequence("Ctrl+N"));
    QAction *actopen = new QAction("打开");
    actopen->setIcon(QIcon(":/img/open.png"));
    actopen->setShortcut(QKeySequence("Ctrl+O"));
    QAction *actsave = new QAction("保存");
    actsave->setIcon(QIcon(":/img/save.png"));
    actsave->setShortcut(QKeySequence("Ctrl+S"));
    QAction *actsaveas = new QAction("另存为");
    actsaveas->setIcon(QIcon(":/img/saveas.png"));
    actsaveas->setShortcut(QKeySequence("Ctrl+Shift+S"));

    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(actnew);
    fileMenu->addAction(actopen);
    fileMenu->addAction(actsave);
    fileMenu->addAction(actsaveas);


    QToolBar *filetool = addToolBar("文件");
    filetool->addAction(actnew);
    filetool->addAction(actopen);
    QToolBar *edittool = addToolBar("编辑");
    //edittool->addAction(actcopy);
    //edittool->addAction(actpaste);
    QToolBar *settool = addToolBar("设置");
    QFontComboBox *fc = new QFontComboBox;
    settool->addWidget(fc);

    QLabel *lb = new QLabel("1.txt");
    this->statusBar()->addWidget(lb);

    te = new QTextEdit;
    te->setMinimumSize(640, 480);
    this->setCentralWidget(te);
}

MainWindow::~MainWindow()
{

}

void MainWindow::openfile()
{
    //QFileDialog::getOpenFileName();
    QString filename = QFileDialog::getOpenFileName();
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QByteArray buf = f.readAll();
    f.close();

    te->setText(buf);
}
