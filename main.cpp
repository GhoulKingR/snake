#include "headers/board.hpp"
#include "headers/snake.hpp"
#include "headers/ball.hpp"

#include <iostream>

int main() {
    Snake snake(1, 1, 1, 0);
    Ball ball;
    Board board(&snake, &ball);
    board.start();

    std::cout << "Game over" << std::endl;
    return 0;
}
