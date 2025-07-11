#pragma once

#include "board.hpp"
#include <atomic>
#include <random>

class Board;
class Ball {
    int x, y;
    std::atomic<bool> internal_state;
    std::mt19937 gen;
    std::uniform_int_distribution<> distx, disty;

    void randomly_place();

public:
    Ball();
    void toggle_state();
    void drawTo(Board* board);
};
