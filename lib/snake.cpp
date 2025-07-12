#include <sstream>
#include <string>

#include "../headers/snake.hpp"

Snake::Snake(int x, int y, int dx, int dy) {
    head.x = x;
    head.y = y;
    head.dx = dx;
    head.dy = dy;
}

void Snake::drawTo(Board* board) {
    board->write_char('O', head.x, head.y);

    int len = body.size();
    for (int i = 0; i < len; i++) {
        pos part = body[i];
        if (i == 0) {
            board->write_char('o', part.x, part.y);
        } else {
            if (part.dx != 0 && part.dy == 0) {
                board->write_char('-', part.x, part.y);
            } else if (part.dx == 0 && part.dy != 0) {
                board->write_char('|', part.x, part.y);
            }
        }
    }
}

bool Snake::selfCollide() {
    for (auto part : body) {
        if (part == head)
            return true;
    }
    return false;
}

vec Snake::get_head() {
    vec new_vec;
    new_vec.x = head.x;
    new_vec.y = head.y;
    return new_vec;
}

void Snake::add_length() {
    length++;
}

void Snake::changeDirection(int dx, int dy) {
    if (dx != 0 && dy == 0 && head.dx == 0 && head.dy != 0) {
        head.dx = dx;
        head.dy = dy;
    } else if (dx == 0 && dy != 0 && head.dx != 0 && head.dy == 0) {
        head.dx = dx;
        head.dy = dy;
    }
}

template <class T>
T my_mod(T a, T b) {
    return ((a % b) + b) % b;
}

void Snake::next() {
    body.push_back(head);
    head.x += head.dx;
    head.y += head.dy;

    head.x = my_mod(head.x - 1, BOARD_WIDTH - 2) + 1;
    head.y = my_mod(head.y - 1, BOARD_HEIGHT - 2) + 1;

    while (body.size() > length) {
        body.pop_front();
    }
}

int Snake::score() {
    return length - 1;
}

std::string Snake::to_string() {
    std::stringstream ss;
    ss << "Snake( x: " << head.x << ", y: " << head.y << ", dx: " << head.dx << ", dy: " << head.dy << " )";
    return ss.str();
}
