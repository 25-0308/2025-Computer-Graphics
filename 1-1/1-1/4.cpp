#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_real_distribution<float> rgb(0.0f, 1.0f);
std::uniform_real_distribution<float> pos(10.0f, 740.0f);
std::uniform_real_distribution<float> random_size(-5.0f, 5.0f);

bool mouse_left = false;
bool mouse_right = false;

bool c_1 = false;
bool c_2 = false;
bool c_3 = false;
bool c_4 = false;
bool c_5 = false;

struct rect_xy {
	float x, y;
}typedef origin;

struct Rectangle {
	float x, y;
	float r, g, b;
	float size = 25.0f / 400.0f;
	float dx = 15.0f, dy = 15.0f;
}typedef Rect;

std::vector<Rect> rectangles;
std::vector<origin> orig;

int mouse_x, mouse_y;

int rect_num = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

int main(int argc, char** argv)
{
	//--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard); // 키보드 입력 함수 지정
	glutTimerFunc(100, TimerFunction, 0); // 타이머 함수 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid Motion(int x, int y)
{
	if (mouse_left) {
		
	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (rect_num < 5) {
			rectangles.push_back({
					static_cast<float>(x), static_cast<float>(y),
				rgb(mtRand),rgb(mtRand),rgb(mtRand)
				});
			orig.push_back({
				static_cast<float>(x), static_cast<float>(y)
				});
			rect_num++;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	}
}

GLvoid TimerFunction(int value)
{
	if (c_1 || c_2 || c_3 || c_4 || c_5) {
		if (c_1) {
			for (auto& rect : rectangles) {
				if (rect.x + 25.0f > 800.0f || rect.x - 25.0f < 0) rect.dx *= -1;
				if (rect.y + 25.0f > 800.0f || rect.y - 25.0f < 0) rect.dy *= -1;
				rect.x += rect.dx;
				rect.y -= rect.dy;
			}
		}
		if (c_2) {
			for (auto& rect : rectangles) {
				if (rect.x + 25.0f > 800.0f || rect.x - 25.0f < 0) {
					rect.dx *= -1;
					if (rect.dy > 0) rect.y += 15.0f;
					else rect.y -= 15.0f;
				}
				if (rect.y + 25.0f > 800.0f || rect.y - 25.0f < 0) rect.dy *= -1;
				rect.x += rect.dx;
			}
		}
		if (c_3) {
			for (auto& rect : rectangles) {
				rect.size += random_size(mtRand) / 400.0f;
				if (rect.size < 0.0f) rect.size = 5.0f / 400.0f;
			}
		}
		if (c_4) {
			for (auto& rect : rectangles) {
				rect.r = rgb(mtRand);
				rect.g = rgb(mtRand);
				rect.b = rgb(mtRand);
			}
		}
		if (c_5) {
			if (rectangles.size() >= 2) {
				for (int i = 1; i < rectangles.size(); i++) {
					float dx = rectangles[i - 1].x - rectangles[i].x;
					float dy = rectangles[i - 1].y - rectangles[i].y;
					float dist = std::sqrt(dx * dx + dy * dy);
					dist -= 25.0f;
					if (dist > 1.0f) {
						rectangles[i].x += dx * dist / 400.0f;
						rectangles[i].y += dy * dist / 400.0f;
						// 경계 처리
						float size_pixels = rectangles[i].size * 400.0f;
						rectangles[i].x = std::max(size_pixels, std::min(800.0f - size_pixels, rectangles[i].x));
						rectangles[i].y = std::max(size_pixels, std::min(800.0f - size_pixels, rectangles[i].y));
					}
				}
			}
		}
		glutTimerFunc(500, TimerFunction, 0);
	}
	glutPostRedisplay();
}


GLvoid drawScene() //--- 콜백 함수: 출력 콜백 함수 
{
	glClearColor(0.1, 0.1, 0.1, 1.0f); // 바탕색을 ‘blue’로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기glBegin(GL_QUADS);
	for (const auto& rect : rectangles) {
		glColor3f(rect.r, rect.g, rect.b); // 사각형 색상 설정
		float x = rect.x / 400.0f - 1.0f;
		float y = -(rect.y / 400.0f - 1.0f); // y축 반전
		glRectf(x - rect.size, y - rect.size, x + rect.size, y + rect.size);
	}
	glEnd();
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		c_1 = !c_1;
		if (c_1)
			if (!c_2 && !c_3 && !c_4 && !c_5) glutTimerFunc(500, TimerFunction, 0);
		break;
	case '2':
		c_2 = !c_2;
		if (c_2)
			if (!c_1 && !c_3 && !c_4 && !c_5) glutTimerFunc(500, TimerFunction, 0);
		break;
	case '3':
		c_3 = !c_3;
		if (c_3)
			if (!c_1 && !c_2 && !c_4 && !c_5) glutTimerFunc(500, TimerFunction, 0);
		break;
	case '4':
		c_4 = !c_4;
		if (c_4)
			if (!c_1 && !c_2 && !c_3 && !c_5) glutTimerFunc(500, TimerFunction, 0);
		break;
	case '5':
		c_5 = !c_5;
		if (c_5)
			if (!c_2 && !c_3 && !c_4 && !c_1) glutTimerFunc(500, TimerFunction, 0);
		break;
	case 's':
		c_1 = false;
		c_2 = false;
		c_3 = false;
		c_4 = false;
		c_5 = false;
		break;
	case 'm':
		for (int i = 0;i < orig.size(); i++) {
			rectangles[i].x = orig[i].x;
			rectangles[i].y = orig[i].y;
		}
		break;
	case 'r':
	{
		rectangles.clear();
		orig.clear();
		rect_num = 0;
		break;
	}
	case 'q':
		glutLeaveMainLoop(); //--- 'q'키를 누르면 프로그램 종료
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}