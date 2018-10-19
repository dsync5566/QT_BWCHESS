#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QElapsedTimer>
#include <QDebug>

void MainWindow::setupWidgets()
{
    BWC.ChessWH = 80;
    BWC.Top = 50;
    BWC.Left = 50;
    BWC.Right = 20;
    BWC.Bottom = 20;
    //BWC.AniDelay = 10;
    //BWC.TipRectPer = 40;
    //BWC.ChessDelay = 10;

    BWC.WHSelf = BWC.ChessWH / 2;
    BWC.WH = BWC.ChessWH * 8;
    BWC.LeftWH = BWC.Left + BWC.WH;
    BWC.TopWH = BWC.Top + BWC.WH;
    BWC.Width = BWC.Left + BWC.WH + BWC.Right;
    BWC.Height = BWC.Top + BWC.WH + BWC.Bottom;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    //qDebug("Mouse click on: (%d, %d)", x, y);

    if  (x < BWC.Left || x >= BWC.LeftWH || y < BWC.Top || y >= BWC.TopWH) return;

    x = (x - BWC.Left) / BWC.ChessWH;
    y = (y - BWC.Top) / BWC.ChessWH;

    qDebug("user drop chess at (%d, %d)", x, y);

    if (game->userDropChess(y * BOARD_SIZE + x)) update();

    QElapsedTimer timer;

    timer.start();
    if (game->aiDropChess()) {
        if (timer.elapsed() > 0) {
            qDebug() << "The AI operation took" << timer.elapsed() << "milliseconds" << "hit " << game->totalHit;
        }
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw board line
    painter.setPen(Qt::black);
    for (int L = 0; L <= BWC.WH; L += BWC.ChessWH) {
        painter.drawLine(QPoint(L + BWC.Left, BWC.Top), QPoint(L + BWC.Left, BWC.Top + BWC.WH));
        painter.drawLine(QPoint(BWC.Left, L + BWC.Top), QPoint(BWC.Left + BWC.WH, L + BWC.Top));
    }

    // draw chess
    const chess *AB = game->getDisplayBoard();

    for (int L = 0; L < BOARD_NUMBER; L++) {
        int x, y;

        if (AB[L] == black_chess) {
            painter.setBrush(Qt::black);
        } else if (AB[L] == white_chess) {
            painter.setBrush(Qt::white);
        } else {
            continue;
        }
        x = L % 8;
        y = L / 8;
        painter.drawEllipse(QPoint(BWC.Left + x * BWC.ChessWH + BWC.WHSelf, BWC.Top + y * BWC.ChessWH + BWC.WHSelf ), BWC.WHSelf - 2, BWC.WHSelf - 2);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupWidgets();

    game = new Game();
    game->reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}
