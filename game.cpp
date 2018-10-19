#include <algorithm>
#include <QDebug>
#include "game.h"

static int ScoreAB[BOARD_NUMBER] = {
    200, -10,  30,  15,  15,  30, -10, 200,
    -10, -90,   2,   2,   2,   2, -90, -10,
     30,   2,   4,   3,   3,   4,   3,  30,
     15,   2,   3,   3,   3,   3,   3,  15,
     15,   2,   3,   3,   3,   3,   3,  15,
     30,   2,   4,   3,   3,   4,   3,  30,
    -10, -90,   2,   2,   2,   2, -90, -10,
    200, -10,  30,  15,  15,  30, -10, 200
};

void Game::reset()
{
    std::fill_n(AB, BOARD_NUMBER, no_chess);

    AB[28] = black_chess;
    AB[35] = black_chess;
    AB[27] = white_chess;
    AB[36] = white_chess;

    Comp = black_chess;
    AiEnable = 1;
    GameOver = 0;
    ip = 5;
}

int Game::aiDropChess()
{
    if (!AiEnable) return 0;

    int chessNumber = 0;

    while (Comp == white_chess && GameOver == 0) {
        chessNumber = AI();
    }

    return chessNumber;
}

int Game::AI()
{
    int xy= -1;
    int Hav = -10000;

    totalHit = 0;

    for (int i = 0; i < BOARD_NUMBER; i++) {        // next user drop check
        if (dropCheck(i) > 0) {
            int Test = ScoreAB[i];
            for (auto &it : moveRecord) {
                Test += ScoreAB[it];
            }

            Test -= LoopAB(i);

            if (Hav < Test) {
                Hav = Test;
                xy = i;
            }
        }
    }

    qDebug("AI score = %d, put at %d", Hav, xy);
    if (xy < 0)  {
        qDebug("OOPS! Comp = %d", Comp);
        Comp = black_chess;
        return 0;
    }

    return userDropChess(xy);
    /*
    int chessNumber = 0;
    if (xy >= 0 && dropCheck(xy) > 0) {
        for (auto &it : moveRecord) {
            AB[it] = Comp;
            chessNumber++;
        }
        AB[xy] = Comp;
        chessNumber++;

        ChangMan();
        TestOk();
    }

    return chessNumber;
    */
}

int Game::LoopAB(int xy)
{
    int max = -10000;

    ip--;
    PushAB();
    if (xy >= 0) {
        for (auto &it : moveRecord) {
            AB[it] = Comp;
        }
        AB[xy] = Comp;
        totalHit++;
    }

    ChangMan();

    for (int i = 0; i < BOARD_NUMBER; i++) {
        if (dropCheck(i) > 0) {
            int Test = ScoreAB[i];
            for (auto &it : moveRecord) {
                Test += ScoreAB[it];
            }

            if (ip > 0) {
                Test = Test - LoopAB(i);
            }
            if (max < Test) {
                max = Test;
            }
        }
    }

    if (max == -10000) {
        if (ip > 0) {
            max = -LoopAB(-1);
        } else {
            max = 0;
        }
    }

    PopAB();
    ChangMan();
    ip++;

    return max;
}

int Game::userDropChess(int xy)
{
    int chessNumber = 0;

    if (dropCheck(xy) > 0) {
        int Test = 0;
        for (auto &it : moveRecord) {
            AB[it] = Comp;
            Test += ScoreAB[it];
            chessNumber++;
        }
        AB[xy] = Comp;
        Test += ScoreAB[xy];
        chessNumber++;

        qDebug("put score = %d, put at %d", Test, xy);

        ChangMan();
        TestOk();
    }

    return chessNumber;
}

void Game::TestOk()
{
    for (int i = 0; i < BOARD_NUMBER; i++) {        // next user drop check
        if (dropCheck(i) > 0) return;
    }

    if (Comp == black_chess) {
        qDebug("force change black to white");
    } else {
        qDebug("force change white to black");
    }
    ChangMan();

    for (int i = 0; i < BOARD_NUMBER; i++) {        // next next user drop check
        if (dropCheck(i) > 0) return;
    }

    qDebug("Game Over");
    GameOver = 1;
}

unsigned int Game::dropCheck(int xy)
{
    moveRecord.clear();

    if (AB[xy] != no_chess) return 0;               // have chess

    //0 右下, 1 下, 2 左下, 3 右, 4 左, 5 右上, 6 上, 7 左上
    for (int H = 0; H < 8; H++) {                   // 分八個方向討論
        int XY6, X6;

        XY6 = xy + XY6Val[H];
        if (XY6 < 0 || XY6 > 63) continue;

        if (AB[XY6] == no_chess) continue;          // 相鄰無子
        if (AB[XY6] == Comp) continue;              // 相鄰自己的子

        X6 = (xy % 8) + X6Val[H];
        if (X6 == 8 || X6 == -1) continue;          // 超出邊界

        while (1) {
            XY6 += XY6Val[H];
            if (XY6 < 0 || XY6 > 63) break;

            if (AB[XY6] == no_chess) break;         // 無子

            X6 += X6Val[H];
            if (X6 == 8 || X6 == -1) break;

            if (AB[XY6] == Comp) {                  // 找到自已的子
                while (1) {
                    XY6 -= XY6Val[H];
                    if (XY6 == xy) break;           // 回到起點就不再用

                    moveRecord.push_back(XY6);
                }
                break;
            }
        }
    }

    return moveRecord.size();
}

/*
Game::Game()
{

}
*/
