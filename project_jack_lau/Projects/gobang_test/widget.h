#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QMouseEvent"
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QMessageBox>
#define u (int)32
#define boardSize (size_t)15

enum Chess{
    none=0,
    black=1,
    white=2
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void mousePressEvent(QMouseEvent *event);

    void chessBoardPrint();

    void resetChessBoard();

    void chessSet();

    int checkForWinner(Chess color);

    //void BGM();

    QPoint pos;

    QPixmap m_chessPixmap;

    QPixmap m_boardPixmap;

    Chess gobang[15][15]={{none}};

    Chess colorLog=white;

};

class bgm
{
public:
    void BGM();
};

#endif // WIDGET_H
