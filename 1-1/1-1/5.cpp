#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>

std::mt19937 gen(std::random_device{}());
std::uniform_real_distribution<float> rPos(-0.9f, 0.9f);
std::uniform_real_distribution<float> rCol(0.0f, 1.0f);
std::uniform_int_distribution<int> rCnt(20, 40);

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_CNT 40
#define RECT_SIZE 0.1f

struct RECTANGLE {
	float x1, y1, x2, y2;
	float r, g, b;
	float size = RECT_SIZE;
};

std::vector<RECTANGLE> Rects;
std::vector<RECTANGLE> Eraser;
float bgR = 1.f, bgG = 1.f, bgB = 1.f;
bool LmouseDown = false;
bool isFirst = true;
int Cnt{};
int aliveCnt{};

GLfloat transformx(int x);
GLfloat transformy(int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Move(int x, int y);
bool Colliding(RECTANGLE& eraser, RECTANGLE& r);
void initRect();
void Merge();

void main(int argc, char** argv) {//--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(300, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성 (윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	initRect();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(KeyBoard); // 키보드 함수 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Move);
	glutMainLoop(); // 이벤트 처리 시작
}

GLfloat transformx(int x) {
	return ((float)x / (WINDOW_WIDTH / 2)) - 1.0f;
}

GLfloat transformy(int y) {
	return ((WINDOW_HEIGHT - (float)y) / (WINDOW_HEIGHT / 2)) - 1.0f;
}

GLvoid drawScene() //--- 콜백 함수: 출력 콜백 함수 
{
	glClearColor(bgR, bgG, bgB, 1.0f); // 바탕색을 ‘blue’로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	for (auto& rect : Rects) {
		glColor3f(rect.r, rect.g, rect.b);
		glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
	}
	if (LmouseDown) {
		for (auto& rect : Eraser) {
			glColor3f(rect.r, rect.g, rect.b);
			glRectf(rect.x1, rect.y1, rect.x2, rect.y2);
		}
	}
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid KeyBoard(unsigned char key, int x, int y) //--- 콜백 함수: 키보드 콜백 함수 
{
	switch (key)
	{
	case 'r': {
		if (!Rects.empty()) Rects.clear();
		if (!Eraser.empty()) Eraser.clear();
		if (!isFirst) isFirst = true;
		initRect();
		break;
	}
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		LmouseDown = true;
		if (isFirst) {
			float mx = transformx(x);
			float my = transformy(y);
			float ex1 = mx - RECT_SIZE / 2;
			float ey1 = my - RECT_SIZE / 2;
			Eraser.push_back({ ex1, ey1, ex1 + RECT_SIZE, ey1 + RECT_SIZE, 0.f, 0.f, 0.f });
			isFirst = false;
		}
		else {
			float mx = transformx(x);
			float my = transformy(y);
			for (auto& e : Eraser) {
				float ex1 = mx - e.size / 2;
				float ey1 = my - e.size / 2;
				Merge();
				e.x1 = ex1; e.y1 = ey1; e.x2 = ex1 + e.size; e.y2 = ey1 + e.size;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		LmouseDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		float mx = transformx(x);
		float my = transformy(y);
		if (aliveCnt < Cnt) {
			Rects.push_back({ mx - RECT_SIZE / 2, my - RECT_SIZE / 2, mx + RECT_SIZE / 2, my + RECT_SIZE / 2, rCol(gen), rCol(gen), rCol(gen) });
			for (auto& e : Eraser) {
				e.size -= 0.02f;
			}
			aliveCnt++;
		}
	}
	glutPostRedisplay();
}

float offsetX = 0.0f, offsetY = 0.0f;
GLvoid Move(int x, int y) {
	if (x >= 0 && x <= WINDOW_WIDTH && y >= 0 && y <= WINDOW_HEIGHT && LmouseDown) {
		float mx = transformx(x);
		float my = transformy(y);
		for (auto& e : Eraser) {
			float ex1 = mx - e.size / 2;
			float ey1 = my - e.size / 2;
			e.x1 = ex1; e.y1 = ey1; e.x2 = ex1 + e.size; e.y2 = ey1 + e.size;
		}
		Merge();
	}
	glutPostRedisplay();
}

bool Colliding(RECTANGLE& eraser, RECTANGLE& r) {
	return (eraser.x1 < r.x2 && eraser.x2 > r.x1 &&
		eraser.y1 < r.y2 && eraser.y2 > r.y1);
}

void initRect() {
	aliveCnt = Cnt = rCnt(gen);
	for (int i = 0; i < Cnt; ++i) {
		float x = rPos(gen);
		float y = rPos(gen);
		Rects.push_back({ x, y, x + RECT_SIZE, y + RECT_SIZE, rCol(gen), rCol(gen), rCol(gen) });
	}
}

void Merge() {
	for (auto& e : Eraser) {
		for (int i = 0; i < Rects.size(); ++i) {
			if (Colliding(e, Rects[i])) {
				e.size += 0.02f;
				e.r = Rects[i].r; e.g = Rects[i].g; e.b = Rects[i].b;
				Rects.erase(Rects.begin() + i);
				aliveCnt--;
				return;
			}
		}
	}
}