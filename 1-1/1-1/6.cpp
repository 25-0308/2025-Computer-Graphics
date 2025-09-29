#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>
#include <random>
#include <vector>

std::mt19937 gen(std::random_device{}());
std::uniform_real_distribution<float> rCol(0.0f, 1.0f);
std::uniform_real_distribution<float> rPos(-0.9f, 0.9f);
std::uniform_real_distribution<float> rSize(0.2f, 0.4f);
std::uniform_int_distribution<int> rCnt(5, 10);

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define MIN_SIZE 0.05f

#define NONE 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UDLR 5

#define DIAGONAL 6
#define SAMEDIR 7
#define EIGHTDIR 8


struct RECTANGLE {
	float x1, y1, x2, y2;
	float r, g, b;
	float size;
	int dirX, dirY;
	int moveType;
	bool devided = false;
	RECTANGLE(float rr, float gg, float bb)
		: r(rr), g(gg), b(bb),
		dirX(NONE), dirY(NONE),
		moveType(NONE)
	{
		x1 = rPos(gen);
		y1 = rPos(gen);
		size = rSize(gen);
		x2 = x1 + size;
		y2 = y1 + size;
		if (x2 > 1.0f) {
			x2 = 1.0f;
			x1 = x2 - size;
		}
		if (y2 > 1.0f) {
			y2 = 1.0f;
			y1 = y2 - size;
		}
		if (x1 < -1.0f) {
			x1 = -1.0f;
			x2 = x1 + size;
		}
		if (y1 < -1.0f) {
			y1 = -1.0f;
			y2 = y1 + size;
		}
	}
};

std::vector<RECTANGLE> Rects;

GLfloat transformx(int x);
GLfloat transformy(int y);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Timer(int value);
void initRect();

float bgR = 1.f, bgG = 1.f, bgB = 1.f;

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
	glutTimerFunc(0, Timer, 0);
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
		float mx = transformx(x);
		float my = transformy(y);

		for (auto it = Rects.begin(); it != Rects.end(); ++it) {
			if (mx >= it->x1 && mx <= it->x2 && my >= it->y1 && my <= it->y2 && !it->devided) {
				int moveTypes[] = { UDLR, DIAGONAL, SAMEDIR, EIGHTDIR };
				int type = moveTypes[rand() % 4];

				float halfSize = it->size / 2.0f;
				float thirdSize = it->size / 3.0f;
				float x1 = it->x1;
				float y1 = it->y1;
				float r = it->r, g = it->g, b = it->b;

				std::vector<RECTANGLE> newRects;
				if (type == EIGHTDIR) {
					float offsets[8][2] = {
						{0, 0},
						{thirdSize, 0},
						{2 * thirdSize, 0},
						{0, thirdSize},
						{2 * thirdSize, thirdSize},
						{0, 2 * thirdSize},
						{thirdSize, 2 * thirdSize},
						{2 * thirdSize, 2 * thirdSize}
					};
					int directionsX[8] = { LEFT, NONE, RIGHT, LEFT, RIGHT, LEFT, NONE, RIGHT };
					int directionsY[8] = { DOWN, DOWN, DOWN, NONE, NONE, UP, UP, UP };

					for (int i = 0; i < 8; ++i) {
						RECTANGLE newRect(r, g, b);
						newRect.x1 = x1 + offsets[i][0];
						newRect.y1 = y1 + offsets[i][1];
						newRect.size = thirdSize;
						newRect.x2 = newRect.x1 + newRect.size;
						newRect.y2 = newRect.y1 + newRect.size;
						newRect.moveType = EIGHTDIR;
						newRect.dirX = directionsX[i];
						newRect.dirY = directionsY[i];
						newRect.devided = true;
						newRects.push_back(newRect);
					}
				}
				else if (type == SAMEDIR) {
					float offsets[4][2] = {
						{0, 0},
						{halfSize, 0},
						{0, halfSize},
						{halfSize, halfSize}
					};
					int dirChoices[8][2] = {
						{LEFT, UP}, {NONE, UP}, {RIGHT, UP},
						{LEFT, NONE}, {RIGHT, NONE},
						{LEFT, DOWN}, {NONE, DOWN}, {RIGHT, DOWN}
					};
					int randDir = rand() % 8;
					int dirX = dirChoices[randDir][0];
					int dirY = dirChoices[randDir][1];

					for (int i = 0; i < 4; ++i) {
						RECTANGLE newRect(r, g, b);
						newRect.x1 = x1 + offsets[i][0];
						newRect.y1 = y1 + offsets[i][1];
						newRect.size = halfSize;
						newRect.x2 = newRect.x1 + newRect.size;
						newRect.y2 = newRect.y1 + newRect.size;
						newRect.moveType = SAMEDIR;
						newRect.dirX = dirX;
						newRect.dirY = dirY;
						newRect.devided = true;
						newRects.push_back(newRect);
					}
				}
				else {
					float offsets[4][2] = {
						{0, 0},
						{halfSize, 0},
						{0, halfSize},
						{halfSize, halfSize}
					};
					int directionsX[4], directionsY[4];

					if (type == UDLR) {
						directionsX[0] = LEFT;  directionsY[0] = NONE;
						directionsX[1] = RIGHT; directionsY[1] = NONE;
						directionsX[2] = NONE;  directionsY[2] = UP;
						directionsX[3] = NONE;  directionsY[3] = DOWN;
					}
					else if (type == SAMEDIR) {

					}
					else {
						directionsX[0] = LEFT;  directionsY[0] = DOWN;
						directionsX[1] = RIGHT; directionsY[1] = DOWN;
						directionsX[2] = LEFT;  directionsY[2] = UP;
						directionsX[3] = RIGHT; directionsY[3] = UP;
					}

					for (int i = 0; i < 4; ++i) {
						RECTANGLE newRect(r, g, b);
						newRect.x1 = x1 + offsets[i][0];
						newRect.y1 = y1 + offsets[i][1];
						newRect.size = halfSize;
						newRect.x2 = newRect.x1 + newRect.size;
						newRect.y2 = newRect.y1 + newRect.size;
						newRect.moveType = type;
						newRect.dirX = directionsX[i];
						newRect.dirY = directionsY[i];
						newRect.devided = true;
						newRects.push_back(newRect);
					}
				}

				Rects.erase(it);
				Rects.insert(Rects.end(), newRects.begin(), newRects.end());
				break;
			}
		}
		glutPostRedisplay();
	}
}

GLvoid Timer(int value) {
	for (auto it = Rects.begin(); it != Rects.end();) {
		if (it->moveType != NONE) {
			float speed = 0.01f;
			float minusSize = 0.005f;

			if (it->dirX == LEFT)  it->x1 -= speed, it->x2 -= speed;
			if (it->dirX == RIGHT) it->x1 += speed, it->x2 += speed;
			if (it->dirY == UP)    it->y1 += speed, it->y2 += speed;
			if (it->dirY == DOWN)  it->y1 -= speed, it->y2 -= speed;

			it->size -= minusSize;
			if (it->size < 0.01f) {
				it = Rects.erase(it);
				continue;
			}
			else {
				it->x2 = it->x1 + it->size;
				it->y2 = it->y1 + it->size;
			}
		}
		++it;
	}
	glutPostRedisplay();
	glutTimerFunc(100, Timer, 0);
}

void initRect() {
	if (!Rects.empty()) Rects.clear();
	int n = rCnt(gen);
	for (int i = 0; i < n; ++i) {
		Rects.push_back({ rCol(gen), rCol(gen), rCol(gen) });
	}
}