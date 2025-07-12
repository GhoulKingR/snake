#pragma once

#include <deque>
#include <string>

#include "board.hpp"

struct vec { int x, y; };

class Board;
class Snake {
    int length = 1;

    struct pos {
        int x, y, dx, dy;

        bool operator==(const pos& other) {
            return (other.x == x) && (other.y == y);
        }
    } head;
    std::deque<pos> body;
    Board* board;

public:
    Snake(int x, int y, int dx, int dy);
    void drawTo(Board* board);
    void next();
    void changeDirection(int dx, int dy);
    void add_length();

    vec get_head();
    bool selfCollide();
    int score();
    std::string to_string();
};
