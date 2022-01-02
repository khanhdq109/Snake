#include <windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include "Snake.h"
#include <ctime>

int gridX, gridY;
int snake_length = 5;

bool food = true; // Kiểm tra xem có vẽ lại Food khi Callback không
int foodX, foodY; // Tọa độ của food

int posX[100] = { 20,20,20,20,20 }, posY[100] = { 20,19,18,17,16 };

void initGrid(int x, int y) {
	gridX = x;
	gridY = y;
}

void unit(int, int);

void drawGrid() {
	for (int i = 0; i < gridX; i++) 
		for (int j = 0; j < gridY; j++)
			unit(i, j);
	unit(20, 20);
}

// Vẽ từng ô vuông
void unit(int x, int y) {
	// Thiết lập màu vẽ nếu viền thì màu nâu cứt, ở trong thì màu trắng
	if (x == 0 || y == 0 || x == gridX - 1 || y == gridY - 1) {
		glLineWidth(3.0);
		glColor3f(0.5, 0.5, 0.0);
	}
	else {
		glLineWidth(1.0);
		glColor3f(1.0, 1.0, 1.0);
	}

	// Vẽ ô vuông
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);
	glEnd();
}

void drawSnake() {
	// Lưu snake trong posX, posY
	for (int i = snake_length - 1; i > 0; i--) {
		posX[i] = posX[i - 1];
		posY[i] = posY[i - 1];
	}

	// Snake di chuyển
	if (sDirection == UP) posY[0]++;
	else if (sDirection == DOWN) posY[0]--;
	else if (sDirection == RIGHT) posX[0]++;
	else if (sDirection == LEFT) posX[0]--;

	// Snake ăn food
	if (posX[0] == foodX && posY[0] == foodY) {
		score++;
		snake_length++;
		if (snake_length > MAX)
			snake_length = MAX;
		food = true; // Vẽ lại food
	}

	// Kiểm tra điều kiện Game Over
	// Chạm tường
	if (posX[0] == 0 || posX[0] == gridX - 1 || posY[0] == 0 || posY[0] == gridY - 1)
		gameOver = true;
	// Chạm thân
	for (int i = 1; i < snake_length; i++) 
		if (posX[0] == posX[i] && posY[0] == posY[i])
			gameOver = true;

	for (int i = 0; i < snake_length; i++) {
		if (i == 0)
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 1.0);

		glRectd(posX[i], posY[i], posX[i] + 1.0, posY[i] + 1.0); // Vẽ hình chữ nhật
	}
}

// Vẽ food
void drawFood() {
	if (food)
		random(foodX, foodY);
	food = false;
	glColor3f(1.0, 0.0, 0.0);
	glRectf(foodX, foodY, foodX + 1, foodY + 1);
}

// Sinh ngẫu nhiên food
void random(int& x, int& y) {
	int maxX = gridX - 2, maxY = gridY - 2;
	int min = 1;
	srand(time(NULL));
	x = min + rand() % (maxX - min);
	y = min + rand() % (maxY - min);
}