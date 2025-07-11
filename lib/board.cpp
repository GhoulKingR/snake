#include <atomic>
#include <csignal>
#include <cstdio>
#include <iostream>
#include <thread>
#include <termios.h>
#include <unistd.h>

#include "../headers/board.hpp"

std::atomic<bool> terminate_flag(false);

void Board::drawBorders() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        auto row = buffer[i];
        row[0] = '|';
        row[BOARD_WIDTH - 1] = '|';
    }

    auto first = buffer[0], last = buffer[BOARD_HEIGHT - 1];
    memset(first, '-', BOARD_WIDTH * sizeof(char));
    memset(last, '-', BOARD_WIDTH * sizeof(char));

    bounds.top = 1;
    bounds.left = 1;
    bounds.bottom = BOARD_HEIGHT - 2;
    bounds.right = BOARD_WIDTH - 2;
}

void Board::clear(){
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        auto row = buffer[i];
        memset(row, ' ', BOARD_WIDTH * sizeof(char));
    }
    
}

void Board::render() {
    for (int i = 0; i < SCORE_POS_Y; i++)
        std::cout << "\n";
    std::cout << "\x1b[" << SCORE_POS_X << "C";
    std::cout << "Score: " << snake->score();
    std::cout << "\x1b[" << SCORE_POS_Y << "F";

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            auto c = buffer[i][j];
            std::cout << c;
        }
        std::cout << '\n';
    }
}

void Board::mainloop(Board* ctx, Snake* snake, Ball* ball) {
    using namespace std::chrono_literals;
    while(!terminate_flag) {
        // prepare the base board
        ctx->clear();
        ctx->drawBorders();

        // update objects for next frame
        snake->next();
        ball->checkCollision(snake);

        // update the buffer
        snake->drawTo(ctx);
        ball->drawTo(ctx);

        // render the screeen
        std::cout << "\x1b[" << BOARD_HEIGHT << "F";
        ctx->render();
        std::this_thread::sleep_for(100ms);
    }
}

termios oldt;

void handle_exit(int code) {
    terminate_flag = true;
    std::cout << "Gracefully shutting down...\n";
    std::cout << "\x1b[?25h";
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void Board::initDisplay() {
    // hide cursor
    std::cout << "\x1b[?25l";

    // disable line buffering and user input
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= (~ECHO & ~ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    signal(SIGINT, handle_exit);
}

void keyboard_thread(Snake* snake) {
    using namespace std::chrono_literals;

    char ch;
    while((ch = getchar()) != EOF) {
        switch (ch) {
            case 'w':
            case 'k':
                snake->changeDirection(0, -1);
                break;
            case 's':
            case 'j':
                snake->changeDirection(0, 1);
                break;
            case 'a':
            case 'h':
                snake->changeDirection(-1, 0);
                break;
            case 'd':
            case 'l':
                snake->changeDirection(1, 0);
                break;

            default:
                break;
        }
        std::this_thread::sleep_for(100ms);
    }
}

void Board::initKeyboard() {
    std::thread kt(keyboard_thread, snake);
    threads.push_back(&kt);
    kt.detach();
}

Board::Board(Snake* snake, Ball* ball): snake(snake), ball(ball) {
    clear();
    drawBorders();
    snake->drawTo(this);
    ball->drawTo(this);

    initDisplay();
    initKeyboard();
    render();
}

void Board::start() {
    std::thread t(mainloop, this, snake, ball);
    threads.push_back(&t);
    t.join();
}

void Board::write_char(char c, int x, int y) {
    buffer[y][x] = c;
}
