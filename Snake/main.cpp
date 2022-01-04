#include <Windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif
#include "Snake.h"
#include <iostream>
#include <string>
#include <thread>

#define COLUMNS 40
#define ROWS 40

#define FPS 10

std::string fuck[5] = {
	"\n~CON GA~",
	"\nKHON THI SONG, NGU THI CHET, KHOC LOC CL :))",
	"\nCHOI NGU VAY THI XOA CMN GAME DI BAN EIIIII",
	"\n~TUOI LOZ BAN EI~",
	"\n~NON VKL~"
};

// Biến hướng của snake
short sDirection = RIGHT;

// Điểm số
int score = 0;
int high_score[5] = { 0,0,0,0,0 };
int rank = 5;

// Bonus
bool bonus = false;

// Game Over
bool gameOver = false;

// Hàm xử lý sự kiện chuột
void keyboard_callback(int, int, int);

void timer_callback(int);
void display_callback();
void reshape_callback(int, int);

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	initGrid(COLUMNS, ROWS);
	initHigh_Score();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(756, 550);
	glutInitWindowPosition(450, 150);
	glutCreateWindow("SNAKE GAME");

	init();
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 0);

	// Xử lý sự kiện
	glutSpecialFunc(keyboard_callback);

	glutMainLoop();
	
	return 0;
}

void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawGrid();
	drawSnake();
	drawScore();
	drawHigh_Score();
	if (bonus == false) drawFood();
	else drawBonus();
	
	// Kết thúc bản vẽ trên Buffer hiện tại, đẩy ra màn hình và chuyển qua Buffer còn lại
	// Nhìn chung thì nó cũng có thể phần nào được coi như giống chức năng với Flush hoặc Finish
	glutSwapBuffers(); 

	// Hiển thị thông báo khi Game Over
	if (gameOver) {
		std::string str = "SCORE: ";
		str += std::to_string(score);
		if (rank == 5) {
			srand(time(NULL));
			int v = rand() % 5;
			str += fuck[v];
		}
		else
			str += "\nCongratulation! You got HIGH SCORE";
		LPWSTR ws = new wchar_t[str.size() + 1];
		copy(str.begin(), str.end(), ws);
		ws[str.size()] = 0;

		MessageBox(NULL, ws, L"GAME OVER", 0);
		exit(0);
	}
}

void reshape_callback(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, COLUMNS + 15.0, 0.0, ROWS, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void timer_callback(int) {
	glutPostRedisplay(); //Gọi hàm vẽ cập nhật lại màn hình
	glutTimerFunc(1000 / FPS, timer_callback, 0); //0,1 giây sau gọi lại hàm này, kiểu đệ quy
}

// Xử lý sự kiện bàn phím
void keyboard_callback(int key, int, int) {
	switch (key) {
	case GLUT_KEY_UP:
		if (sDirection != DOWN)
			sDirection = UP;
		break;
	case GLUT_KEY_DOWN:
		if (sDirection != UP)
			sDirection = DOWN;
		break;
	case GLUT_KEY_RIGHT:
		if (sDirection != LEFT)
			sDirection = RIGHT;
		break;
	case GLUT_KEY_LEFT:
		if (sDirection != RIGHT)
			sDirection = LEFT;
		break;
	}
}
