#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stack>

static const int BOARD_SIZE = 8;   // board size
static const int BOARD_NUMBER = BOARD_SIZE * BOARD_SIZE;

static const int XY6Val[8] = {9, 8, 7, 1, -1, -7, -8, -9};
static const int X6Val[8] = {1, 0, -1, 1, -1, 1, 0, -1};
static const int Y6Val[8] = {1, 1, 1, 0, 0, -1, -1, -1};

typedef enum { no_chess, black_chess, white_chess } chess;

class typeAB
{
public:
    int value[64];
};

class Game
{
public:
    //Game();
    void reset();
    //int userDropChess(int x, int y) { return dropChess(y * BOARD_SIZE + x); }
    int userDropChess(int xy);
    int aiDropChess();
    const chess *getDisplayBoard() { return AB; }
    int totalHit;

private:
    int dropChess(int xy);
    unsigned int dropCheck(int xy);

    void PushAB() {
        std::copy(AB, AB+64, SaveAB+64*ip);
    }
    void PopAB() {
        std::copy(SaveAB+64*ip, SaveAB+64*(ip+1), AB);
    }
    void ChangMan() { Comp = (Comp == black_chess) ? white_chess : black_chess; }
    void TestOk();
    int AI();
    int LoopAB(int xy);

    chess AB[BOARD_NUMBER];
    chess SaveAB[BOARD_NUMBER * 8];
    std::vector<int> moveRecord;
    chess Comp;                     // user black or white
    int AiEnable;
    int GameOver;
    int ip;
};

#endif // GAME_H
