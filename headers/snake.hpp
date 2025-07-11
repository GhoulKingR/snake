#pragma once

#include <deque>
#include <string>

#include "board.hpp"

class Board;
class Snake {
    int length = 1;

    struct pos { int x, y, dx, dy; } head;
    std::deque<pos> body;
    Board* board;

public:
    Snake(int x, int y, int dx, int dy);
    void drawTo(Board* board);
    void next();
    void changeDirection(int dx, int dy);

    int score();
    std::string to_string();
};
