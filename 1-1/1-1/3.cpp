#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_real_distribution<float> rgb(0.0f, 1.0f);
std::uniform_real_distribution<float> pos(10.0f, 740.0f);

bool mouse_left = false;
bool mouse_right = false;

struct Rectangle {
	float x, y;
	float r, g, b;
	float size = 25.0f / 400.0f;
}typedef Rect;

std::vector<Rect> rectangles;

int mouse_x, mouse_y;

int index = -1;
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
	if (mouse_left && index >= 0 && index < rectangles.size()) {
		// 마우스 드래그로 사각형 이동
		rectangles[index].x = static_cast<float>(x);
		rectangles[index].y = static_cast<float>(y);
		glutPostRedisplay();
	}
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouse_left = true;
		float min_dist = 50.0f; // 25픽셀 이내 클릭 시 선택
		for (size_t i = 0; i < rectangles.size(); ++i) {
			float dx = rectangles[i].x - x;
			float dy = rectangles[i].y - y;
			float dist = std::sqrt(dx * dx + dy * dy);
			if (dist < min_dist) {
				min_dist = dist;
				index = i;
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if (index >= 0 && index < rectangles.size()) { // 유효한 인덱스 확인
			for (size_t i = 0; i < rectangles.size(); ++i) {
				if (i != index) {
					float dx = rectangles[i].x - rectangles[index].x;
					float dy = rectangles[i].y - rectangles[index].y;
					float dist = std::sqrt(dx * dx + dy * dy);

					float temp;
					if (rectangles[i].size >= rectangles[index].size) temp = rectangles[i].size;
					else temp = rectangles[index].size;
					
					if (dist/400.0f < temp) {
						rectangles[i].size += rectangles[index].size; // 크기 증가
						for (size_t i = index; i < rectangles.size() - 1; ++i) {
							rectangles[i] = rectangles[i + 1];
						}
						if (!rectangles.empty()) {
							rectangles.pop_back(); // 마지막 요소 제거
							rect_num--;
						}
					}
				}
			}
			// index 위치의 사각형을 제외하고 나머지를 한 칸씩 앞으로 땡기기
			
		}
		mouse_left = false;
		index = -1; // 선택 해제
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

		mouse_right = true;
		float min_dist = 25.0f; // 25픽셀 이내 클릭 시 선택

		for (size_t i = 0; i < rectangles.size(); ++i) {
			float dx = rectangles[i].x - x;
			float dy = rectangles[i].y - y;
			float dist = std::sqrt(dx * dx + dy * dy);
			if (dist < min_dist) {
				min_dist = dist;
				index = i;
			}
			if (index >= 0 && index < rectangles.size() && rect_num < 30) {
				rectangles[index].size /= 2; // 원래 사각형 크기 절반
				float new_size = rectangles[index].size; // 새 사각형 크기
				rectangles.push_back({
					rectangles[index].x - 25.0f, rectangles[index].y - 25.0f, // 왼쪽 상단
					rgb(mtRand), rgb(mtRand), rgb(mtRand), new_size
					});
				rectangles[index].x += 12.5f;
				rectangles[index].y += 12.5f;
				rect_num++;
				index = -1;
			}
		}
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		mouse_right = false;
		index = -1;
	}
}

GLvoid TimerFunction(int value)
{
	glutPostRedisplay(); // 화면 갱신 요청
}

GLvoid drawScene() //--- 콜백 함수: 출력 콜백 함수 
{
	glClearColor(1.0, 1.0, 1.0, 1.0f); // 바탕색을 ‘blue’로 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기glBegin(GL_QUADS);
	for (const auto& rect : rectangles) {
		glColor3f(rect.r, rect.g, rect.b); // 사각형 색상 설정
		float x = rect.x / 400.0f - 1.0f;
		float y = -(rect.y / 400.0f - 1.0f); // y축 반전
		glRectf(x-rect.size, y-rect.size, x + rect.size, y + rect.size);
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
	case 'a':
	{
		if (rect_num < 30) {
			rectangles.push_back({
				pos(mtRand), pos(mtRand),
			rgb(mtRand),rgb(mtRand),rgb(mtRand)
				});
			rect_num++;
		}
		break; 
	}
	case 'q':
		glutLeaveMainLoop(); //--- 'q'키를 누르면 프로그램 종료
		break;
	}
	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}