#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <pthread.h>

#define BOARD_WIDTH 100
#define BOARD_HEIGHT 50
#define POS_X 0
#define POS_Y 1
#define DIR_X 2
#define DIR_Y 3
#define DELTA 100
#define sleep_ms(milliseconds) usleep(milliseconds * 1000)

typedef struct {
    int pos[2];
} Ball;

typedef struct {
    int head[4];
    int body[(BOARD_HEIGHT - 2) * (BOARD_WIDTH - 2)][4];
    int length, pos;
} Snake;

struct termios original, noecho;

pthread_t keyboard_thread;
void* keyboard_listener(void* arg) {
    Snake* snake = (Snake*) arg;
    while (true) {
        char c = getchar();
        switch (c) {
            case 'W':
            case 'w':
                if (snake->head[DIR_Y] == 0) {
                    snake->head[DIR_X] = 0;
                    snake->head[DIR_Y] = -1;
                }
                break;
            case 'A':
            case 'a':
                if (snake->head[DIR_X] == 0) {
                    snake->head[DIR_X] = -1;
                    snake->head[DIR_Y] = 0;
                }
                break;
            case 'S':
            case 's':
                if (snake->head[DIR_Y] == 0) {
                    snake->head[DIR_X] = 0;
                    snake->head[DIR_Y] = 1;
                }
                break;
            case 'D':
            case 'd':
                if (snake->head[DIR_X] == 0) {
                    snake->head[DIR_X] = 1;
                    snake->head[DIR_Y] = 0;
                }
                break;

            default:
                break;
        }
        sleep_ms(DELTA);
    }
    return NULL;
}

void printBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], Snake* snake) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        char line[BOARD_WIDTH + 1];
        for (int j = 0; j < BOARD_WIDTH; j++) {
            line[j] = board[i][j];
        }
        line[BOARD_WIDTH] = '\0';
        printf("%s\n", line);
    }
    printf("Score: %d\n", snake->length - 1);
}

void renderBorders(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
        board[0][j] = '-';
        board[BOARD_HEIGHT - 1][j] = '-';
    }

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        board[i][0] = '|';
        board[i][BOARD_WIDTH - 1] = '|';
    }

    board[0][0] = '+';
    board[0][BOARD_WIDTH - 1] = '+';
    board[BOARD_HEIGHT - 1][0] = '+';
    board[BOARD_HEIGHT - 1][BOARD_WIDTH - 1] = '+';
}

void renderBall(char board[BOARD_HEIGHT][BOARD_WIDTH], Ball *ball) {
    int x = ball->pos[POS_X], y = ball->pos[POS_Y];
    board[y][x] = 'o';
}

void renderSnake(char board[BOARD_HEIGHT][BOARD_WIDTH], Snake *snake) {
    int head_x = snake->head[POS_X], head_y = snake->head[POS_Y];
    board[head_y][head_x] = '0';

    for (int i = 0; i < snake->length; i++) {
        int part[4];
        memcpy(part, snake->body[i], 4 * sizeof(int));
        if (part[DIR_X] == 0) {
            board[part[POS_Y]][part[POS_X]] = '|';
        } else {
            board[part[POS_Y]][part[POS_X]] = '-';
        }
    }
}

void moveSnake(Snake *snake) {
    memcpy(snake->body[snake->pos], snake->head, 4 * sizeof(int));
    snake->pos++;
    snake->pos = ((snake->pos % snake->length) + snake->length) % snake->length;
    snake->head[POS_X] += snake->head[DIR_X];
    snake->head[POS_Y] += snake->head[DIR_Y];
}

void exit_handler(int code) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    exit(code);
}

void game_over() {
    printf("Game over\n");
    exit_handler(0);
}

void checkCollision(Snake* snake, Ball* ball) {
    if (snake->head[POS_X] == ball->pos[POS_X] && snake->head[POS_Y] == ball->pos[POS_Y]) {
        ball->pos[POS_X] = (rand() % (BOARD_WIDTH - 2)) + 1;
        ball->pos[POS_Y] = (rand() % (BOARD_HEIGHT - 2)) + 1;

        snake->length++;
        int tmp[4];
        memcpy(tmp, snake->body[snake->pos], 4 * sizeof(int));
        for (int i = snake->pos + 1; i < snake->length; i++) {
            int t[4];
            memcpy(t, snake->body[i], 4 * sizeof(int));
            memcpy(snake->body[i], tmp, 4 * sizeof(int));
            memcpy(tmp, t, 4 * sizeof(int));
        }
    }
}

bool checkBorderCollision(Snake* snake) {
    return (
        snake->head[POS_X] == 0 ||
        snake->head[POS_X] == BOARD_WIDTH - 1 ||
        snake->head[POS_Y] == 0 ||
        snake->head[POS_Y] == BOARD_HEIGHT - 1
    );
}

bool checkBodyCollision(Snake* snake) {
    for (int i = 0; i < snake->length; i++) {
        if (
            snake->head[POS_X] == snake->body[i][POS_X] &&
            snake->head[POS_Y] == snake->body[i][POS_Y]
        ) {
            return true;
        }
    }
    return false;
}

int main() {
    Snake snake;
    snake.head[POS_X] = BOARD_WIDTH  / 2;
    snake.head[POS_Y] = BOARD_HEIGHT / 2;
    snake.head[DIR_X] = 1;
    snake.head[DIR_Y] = 0;
    snake.pos = 0;
    snake.length = 1;

    Ball ball;
    srand(time(0));
    ball.pos[POS_X] = (rand() % (BOARD_WIDTH - 2)) + 1;
    ball.pos[POS_Y] = (rand() % (BOARD_HEIGHT - 2)) + 1;

    char board[BOARD_HEIGHT][BOARD_WIDTH];
    pthread_create(&keyboard_thread, NULL, keyboard_listener, &snake);

    tcgetattr(STDIN_FILENO, &original);
    noecho = original;
    noecho.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &noecho);
    signal(SIGINT, exit_handler);

    while (true) {
        memset(board, ' ', BOARD_WIDTH * BOARD_HEIGHT);
        moveSnake(&snake);
        checkCollision(&snake, &ball);

        renderBorders(board);
        renderBall(board, &ball);
        renderSnake(board, &snake);

        printBoard(board, &snake);
        if (checkBorderCollision(&snake) || checkBodyCollision(&snake)) {
            break;
        }

        sleep_ms(DELTA);
        printf("\x1B[%dF", BOARD_HEIGHT + 1);
    }

    game_over();
    return 0;
}
