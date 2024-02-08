#include "widget.h"
#include <QSoundEffect>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    bgm bgm1;
    bgm1.BGM();
    chessBoardPrint();
}

void bgm::BGM()
{
    QSoundEffect *effect = new QSoundEffect();
    effect->setSource(QUrl::fromLocalFile("/Users/jacklau/Downloads/downie4/d587%2F3a34%2F0709%2Fafefff75b1f5db253fb44b71b0c1d83d.wav"));
    effect->setLoopCount(-1); // 设置循环次数，-1 表示无限循环
    effect->setVolume(0.5); // 设置音量大小
    effect->play(); // 开始播放音效
}

void Widget::chessBoardPrint()
{
    m_boardPixmap=QPixmap(512+u,512+u);
    m_boardPixmap.fill(Qt::green);

    QPainter painter(&m_boardPixmap);
    QPen pen(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);

    for (int i = 0; i < 16; i++)
    {
        painter.drawLine(u, u*(i+1), u  * 16, u*(i+1));
        painter.drawLine(u*(i+1), u , u*(i+1), u * 16);
    }

    QLabel *myLabel = new QLabel(this);
    myLabel->setPixmap(m_boardPixmap);
    myLabel->show();
}

void Widget::chessSet()
{
    if (pos.isNull()) {
        return;
    }
    QImage image(m_boardPixmap.toImage());
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    painter.setPen(Qt::NoPen);
    if(colorLog==white)
    {
        colorLog=black;
        painter.setBrush(Qt::black);
    }
    else if(colorLog==black)
    {
        colorLog=white;
        painter.setBrush(Qt::white);
    }
    if (pos.x() / u > 0 && pos.x() / u < 16 && pos.y() / u > 0 && pos.y() / u < 16&& gobang[pos.x() / u - 1][pos.y() / u - 1] == 0)
    {
        gobang[pos.x() / u - 1][pos.y() / u - 1] = colorLog;
        painter.drawEllipse((pos.x() / u) * u , (pos.y() / u) * u , 32, 32);
        qDebug() <<"BLACK: " << pos.x() / u - 1 << " " << pos.y() / u - 1 << " " << gobang[pos.x() / u - 1][pos.y() / u - 1];
    }
    m_boardPixmap = QPixmap::fromImage(image);
    QLabel *myLabel = new QLabel(this);
    myLabel->setPixmap(m_boardPixmap);
    myLabel->show();

    int temp = checkForWinner(colorLog);
    if(temp)
    {
        QMessageBox::StandardButton reply;
        qDebug()<<temp<<" win!";
        if(temp==black)
            reply = QMessageBox::question(this, "Question", "black win!Do you want to play again?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        else
            reply = QMessageBox::question(this, "Question", "white win!Do you want to play again?", QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        if(reply == QMessageBox::Yes)
            resetChessBoard();
        else
            close();
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    pos=event->pos();
    update();
    qDebug()<<"mouse pos"<<event->pos();
    qDebug()<<"mouse button"<<event->button();
    chessSet();
}

int Widget::checkForWinner(Chess color)
{
    int count, i, j;

    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++) {
            if (gobang[i][j] == color) {

                // 检查行
                if (j <= 10)
                {
                    count = 1;
                    for (int k = 1; k < 5; k++)
                    {
                        if (gobang[i][j + k] == color)
                        {
                            count++;
                        }
                        else break;
                    }
                    if (count == 5) return color;
                }

                // 检查列
                if (i <= 10)
                {
                    count = 1;
                    for (int k = 1; k < 5; k++)
                    {
                        if (gobang[i + k][j] == color)
                        {
                            count++;
                        }
                        else break;
                    }
                    if (count == 5) return color;
                }

                // 检查正斜线
                if (i <= 10 && j <= 10)
                {
                    count = 1;
                    for (int k = 1; k < 5; k++)
                    {
                        if (gobang[i + k][j + k] == color)
                        {
                            count++;
                        }
                        else break;
                    }
                    if (count == 5) return color;
                }

                // 检查反斜线
                if (i >= 4 && j <= 10)
                {
                    count = 1;
                    for (int k = 1; k < 5; k++)
                    {
                        if (gobang[i - k][j + k] == color)
                        {
                            count++;
                        }
                        else break;
                    }
                    if (count == 5) return color;
                }
            }
        }
    }
    // 如果没有五子连珠，则返回0
    return 0;
}

void Widget::resetChessBoard()
{
    for (auto& i : gobang) {
        for(Chess& j : i){
            j=none;
        }
    }
    chessBoardPrint();
    colorLog=white;
}

Widget::~Widget()
{
}

