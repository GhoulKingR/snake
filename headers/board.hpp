#pragma once

#include "snake.hpp"
#include <thread>

#define BOARD_WIDTH 100
#define BOARD_HEIGHT 50

class Ball {
    int x, y;
};

class Snake;
class Board {
    Snake* snake;
    Ball ball;
    char buffer[BOARD_HEIGHT][BOARD_WIDTH];
    std::vector<std::thread*> threads;

    struct { int top, left, bottom, right; } bounds;

    void drawBorders();
    void clear();
    void render();

    void initDisplay();
    void initKeyboard();
    
    static void mainloop(Board* ctx, Snake* snake);

public:
    Board(Snake* snake);
    void start();
    void write_char(char c, int x, int y);
};
