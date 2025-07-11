#include "../headers/ball.hpp"
#include <thread>

void Ball::toggle_state() {
    internal_state = !internal_state;
}

void flipper(Ball* ball) {
    using namespace std::chrono_literals;
    while(true) {
        ball->toggle_state();
        std::this_thread::sleep_for(1000ms);
    }
}

Ball::Ball(): internal_state(false) {
    std::thread t(flipper, this);
    t.detach();

    // random number generator
    std::random_device rd;

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distx(1, BOARD_WIDTH - 2);
    std::uniform_int_distribution<> disty(1, BOARD_HEIGHT - 2);

    this->gen = gen;
    this->distx = distx;
    this->disty = disty;

    randomly_place();
}

void Ball::drawTo(Board* board) {
    char to_write = internal_state ? 'O' : 'o';
    board->write_char(to_write, x, y);
}

void Ball::randomly_place() {
    x = distx(gen);
    y = disty(gen);
}
