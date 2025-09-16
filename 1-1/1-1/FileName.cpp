#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>

std::mt19937 mtRand(static_cast<unsigned>(time(nullptr)));
std::uniform_real_distribution<float> rgb(0.0f, 1.0f);

bool t_toggle = false;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
float red = 0.0f, green = 0.0f, blue = 1.0f;

int main(int argc, char** argv)
{
	//--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(500, 500); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����(������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ 
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else std::cout << "GLEW Initialized\n";
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard); // Ű���� �Է� �Լ� ����
	glutTimerFunc(100, TimerFunction, 0); // Ÿ�̸� �Լ� ����
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid TimerFunction(int value)
{
	glutPostRedisplay(); // ȭ�� ���� ��û
	if (t_toggle) {
		red = rgb(mtRand), green = rgb(mtRand), blue = rgb(mtRand);
		glutTimerFunc(100, TimerFunction, 0); // 100�и��� �Ŀ� �ٽ� Ÿ�̸� �Լ� ȣ��
	}
}

GLvoid drawScene() //--- �ݹ� �Լ�: ��� �ݹ� �Լ� 
	{
		glClearColor(red, green, blue, 1.0f); // �������� ��blue���� ����
		glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
		// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
		glutSwapBuffers(); // ȭ�鿡 ����ϱ�
	}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
	{
		glViewport(0, 0, w, h);
	}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
	{
		red = 0.0f, green = 1.0f, blue = 1.0f;
		break; //--- ������ û�ϻ����� ����
	}
	case 'm':
		red = 1.0f, green = 0.0f, blue = 1.0f;
		break; //--- ������ ��ȫ������ ����
	case 'y':
		red = 1.0f, green = 1.0f, blue = 0.0f;
		break; //--- ������ ��������� ����
	case 'a':
		red = rgb(mtRand), green = rgb(mtRand), blue = rgb(mtRand);
		break;
	case 'w':
		red = 1.0f, green = 1.0f, blue = 1.0f;
		break;
	case 'k':
		red = 0.0f, green = 0.0f, blue = 0.0f;
		break;
	case 't':
		if (!t_toggle)glutTimerFunc(100, TimerFunction, 0);
		t_toggle = true;
		break;
	case 's':
		t_toggle = false;
		break;
	case 'q':
		glutLeaveMainLoop(); //--- 'q'Ű�� ������ ���α׷� ����
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}