#include <windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include "Snake.h"
#include <ctime>
#include <thread>

int gridX, gridY;
int snake_length = 5;

// Bonus
int Kclock = 1, move = 0, count = 5;
bool isDraw = false;

bool food = true; // Kiểm tra xem có vẽ lại Food khi Callback không
int foodX, foodY; // Tọa độ của food

int posX[1601] = { 20,20,20,20,20 }, posY[1601] = { 20,19,18,17,16 };

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

void Wall() {
	if (posX[0] == 0 || posX[0] == gridX - 1 || posY[0] == 0 || posY[0] == gridY - 1)
		gameOver = true;
}

void Classic() {
	if (posX[0] == 0) posX[0] = gridX - 2;
	if (posX[0] == gridX - 1) posX[0] = 1;
	if (posY[0] == 0) posY[0] = gridY - 2;
	if (posY[0] == gridY - 1) posY[0] = 1;
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

	for (int i = 0; i < snake_length; i++) {
		if (i == 0)
			glColor3f(0.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 1.0);

		glRectd(posX[i], posY[i], posX[i] + 1.0, posY[i] + 1.0); // Vẽ hình chữ nhật
		if (i != 0) {
			glColor3f(1.0, 1.0, 0.0);
			glPointSize(3.0);
			glBegin(GL_POINTS);
			glVertex2d(posX[i] + 0.5, posY[i] + 0.5);
			glEnd();
		}
	}

	// Kiểm tra điều kiện Game Over
	// Chạm tường
	Classic();
	// Chạm thân
	for (int i = 1; i < snake_length; i++)
		if (posX[0] == posX[i] && posY[0] == posY[i])
			gameOver = true;

	// Snake ăn food
	if (bonus == false && posX[0] == foodX && posY[0] == foodY) {
		score += 5;
		snake_length++;
		if (snake_length > MAX)
			snake_length = MAX;
		Kclock++;
		Kclock %= 6;
		if (Kclock == 0) {
			bonus = true;
			isDraw = true;
		}
		else food = true; // Vẽ lại food
	}

	// Snake ăn bonus
	else if (bonus == true && (posX[0] >= foodX - 1 && posX[0] <= foodX + 1) && (posY[0] >= foodY - 1 && posY[0] <= foodY + 1)) {
		score += count * 60;
		snake_length++;
		if (snake_length > MAX)
			snake_length = MAX;
		bonus = false;
		Kclock = 1;
		food = true; // Vẽ lại food
	}
}

bool checkBody(int x, int y) {
	for (int i = 0; i < snake_length; i++)
		if (x == posX[i] && y == posY[i])
			return false;
	return true;
}

void Timer() {
	count = 5;
	for (int i = 0; i < 5; i++) {
		if (!bonus)
			return;
		count--;
		if (count == 0) {
			bonus = false;
			food = true;
			return;
		}
		Sleep(1000);
	}
}

// Vẽ food
void drawFood() {
	if (food)
		randomFood(foodX, foodY);
	food = false;
	glColor3f(1.0, 0.0, 0.0);
	glRectf(foodX, foodY, foodX + 1, foodY + 1);
}

// Vẽ bonus
void drawBonus() {
	if (isDraw) {
		std::thread counter;
		counter = std::thread(Timer);
		counter.detach();
		randomFood(foodX, foodY);
		isDraw = false;
	}
	glColor3f(1.0, 0.0, 0.0);
	if (move < 2) {
		glRectf(foodX - 1, foodY - 1, foodX + 2, foodY + 2);
		move++;
	}
	else if (move < 4) {
		glRectf(foodX - 0.6, foodY - 0.6, foodX + 1.6, foodY + 1.6);
		move++;
		if (move == 4) move = 0;
	}
}

// Sinh ngẫu nhiên food
void randomFood(int& x, int& y) {
	int maxX = gridX - 2, maxY = gridY - 2;
	int min = 1;
	srand(time(NULL));
	bool done = false;
	while (!done) {
		x = min + rand() % (maxX - min);
		y = min + rand() % (maxY - min);
		done = checkBody(x, y);
	}
}

// Sinh ngẫu nhiên bonus
void randomBonus(int& x, int& y) {
	int maxX = gridX - 3, maxY = gridY - 3;
	int min = 2;
	srand(time(NULL));
	bool done = false;
	while (!done) {
		x = min + rand() % (maxX - min);
		y = min + rand() % (maxY - min);
		done = checkBody(x, y);
	}
}