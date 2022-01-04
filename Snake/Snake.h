#ifndef SNAKE_H
#define SNAKE_H

#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

#define WALL 10
#define CLASSIC 20

// Độ dài tối đa của snake
#define MAX 1601

// Biến lưu hướng của snake
extern short sDirection;

// Điểm số
extern int score;
extern int high_score[5];
extern int rank;

// Bonus
extern bool bonus;

// Game Over
extern bool gameOver;

void initHigh_Score();
void initGrid(int, int);
void drawGrid();

// Mode
void Wall();
void Classic();

void drawSnake();
void drawScore();
void drawFood();
void drawBonus();
void drawHigh_Score();
void randomFood(int&, int&);
void randomBonus(int&, int&);

#endif