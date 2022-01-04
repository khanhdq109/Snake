#include <windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include "Snake.h"
#include <ctime>
#include <thread>
#include <string>
#include <fstream>

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

// Snake di chuyển
void moveSnake() {
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
}

void eatBody() {
	for (int i = 1; i < snake_length; i++)
		if (posX[0] == posX[i] && posY[0] == posY[i])
			gameOver = true;
}

void saveHigh_Score();

void eatFood() {
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
	else food = true;
	saveHigh_Score();
}

void eatBonus() {
	score += count * 60;
	snake_length++;
	if (snake_length > MAX)
		snake_length = MAX;
	bonus = false;
	Kclock = 1;
	food = true;
	saveHigh_Score();
}

void drawSnake() {
	// Di chuyển
	moveSnake();
	// Chạm tường
	Classic();
	// Vẽ snake
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
			glVertex2f(posX[i] + 0.5, posY[i] + 0.5);
			glEnd();
		}
	}
	// Chạm thân
	eatBody();
	// Snake ăn food
	if (bonus == false && posX[0] == foodX && posY[0] == foodY) 
		eatFood();
	// Snake ăn bonus
	else if (bonus == true && (posX[0] >= foodX - 1 && posX[0] <= foodX + 1) && (posY[0] >= foodY - 1 && posY[0] <= foodY + 1)) 
		eatBonus();
	// Save high score
}

void initHigh_Score() {
	std::ifstream ip;
	ip.open("DIEM_CAO.txt", std::ios::in);
	std::string s;
	int i = 0;
	while (!ip.eof()) {
		getline(ip, s);
		high_score[i] = stoi(s);
		i++;
	}
	ip.close();
}
 
void saveHigh_Score() {
	if (rank >= 5) {
		if (score >= high_score[4])
			rank--;
	}
	else {
		if (score >= high_score[rank]) {
			int prerank = rank;
			if (rank > 0) {
				int k = rank - 1;
				while (high_score[k] <= score) {
					k--;
					if (k == -1) break;
				}
				k++;
				rank = k;
				if (k <= 3 && rank != prerank)
					high_score[k + 1] = high_score[k];
				high_score[k] = score;
			}
			else
				high_score[rank] = score;
		}
	}
	std::ofstream op;
	op.open("DIEM_CAO.txt", std::ios::out);
	std::string s;
	for (int i = 0; i < 5; i++) {
		s = std::to_string(high_score[i]);
		op << s;
		if (i != 4) op << '\n';
	}
	op.close();
}

void drawHigh_Score() {
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(44.0, 35.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'D');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'I');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'M');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'C');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'O');
	std::ifstream ip;
	ip.open("DIEM_CAO.txt", std::ios::in);
	std::string s;
	int pos = 32.5;
	while (!ip.eof()) {
		getline(ip, s);
		if (s.size() == 1) glRasterPos2f(47.0, pos);
		else if (s.size() == 2) glRasterPos2f(46.6, pos);
		else if (s.size() == 3) glRasterPos2f(46.3, pos);
		else if (s.size() == 4) glRasterPos2f(45.9, pos);
		for(unsigned int i = 0; i < s.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
		pos -= 3;
	}
	ip.close();
}

bool checkBodyFood(int x, int y) {
	for (int i = 0; i < snake_length; i++)
		if (x == posX[i] && y == posY[i])
			return false;
	return true;
}

bool checkBodyBonus(int x, int y) {
	for (int i = 0; i < snake_length; i++)
		if ((posX[i] >= x - 1 && posX[i] <= x + 1) && (posY[i] >= y - 1 && posY[i] <= y + 1))
			return false;
	return true;
}

void drawScore() {
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(43.5, 3.0);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'C');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'O');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ':');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
	std::string s = std::to_string(score);
	for(unsigned int i = 0; i< s.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
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

// Sinh ngẫu nhiên food
void randomFood(int& x, int& y) {
	int maxX = gridX - 2, maxY = gridY - 2;
	int min = 1;
	srand(time(NULL));
	bool done = false;
	while (!done) {
		x = min + rand() % (maxX - min);
		y = min + rand() % (maxY - min);
		done = checkBodyFood(x, y);
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
		done = checkBodyBonus(x, y);
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