#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "game.h"

struct BWCtype {
    int Left;       //左邊界
    int Top;        //上邊界
    int Right;      //右邊界
    int Bottom;     //下邊界
    int ChessWH;    //單一棋子寬與高
    int BoardSize;  //棋盤單邊數量
    int WHSelf;     //單一棋子寬高的一半
    int WH;         //8個棋子的寬高
    int LeftWH;     //加上左邊界的總寬
    int TopWH;      //加上上邊界的總高
    int Width;      //整個棋盤的寬
    int Height;     //整個棋盤的高
    int FormWidth;  //表單寬
    int FormHeight; //表單高
    int AniDelay;   //動畫延遲秒數
    int TipRectPer; //提示矩型寬的百分比
    int ChessDelay; //棋子播放延遲秒數
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    void setupWidgets();

    struct BWCtype BWC;
    Game *game;
};

#endif // MAINWINDOW_H
