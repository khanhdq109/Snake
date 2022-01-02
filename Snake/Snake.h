#ifndef SNAKE_H
#define SNAKE_H

#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

// Độ dài tối đa của snake
#define MAX 100

// Biến lưu hướng của snake
extern short sDirection;

// Điểm số
extern int score;

// Game Over
extern bool gameOver;

void initGrid(int, int);
void drawGrid();
void drawSnake();
void drawFood();
void random(int&, int&);

#endif