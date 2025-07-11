#include "headers/board.hpp"
#include "headers/snake.hpp"

int main() {
    Snake snake(1, 1, 1, 0);
    Board board(&snake);
    board.start();
    return 0;
}
