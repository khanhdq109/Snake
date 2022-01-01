#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define UP 1
#define DOWN -1
#define RIGHT 3
#define LEFT -3

// Độ dài tối đa của snake
#define MAX 100

void initGrid(int, int);
void drawGrid();
void drawSnake();
void drawFood();
void random(int&, int&);

#endif