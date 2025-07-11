#pragma once

#include "snake.hpp"
#include "ball.hpp"
#include <thread>

#define BOARD_WIDTH 100
#define BOARD_HEIGHT 50

#define SCORE_POS_X 110
#define SCORE_POS_Y 10

class Snake;
class Ball;
class Board {
    Snake* snake;
    Ball* ball;
    char buffer[BOARD_HEIGHT][BOARD_WIDTH];
    std::vector<std::thread*> threads;

    struct { int top, left, bottom, right; } bounds;

    void drawBorders();
    void clear();
    void render();

    void initDisplay();
    void initKeyboard();
    
    static void mainloop(Board* ctx, Snake* snake, Ball* ball);

public:
    Board(Snake* snake, Ball* ball);
    void start();
    void write_char(char c, int x, int y);
};
