//*** 헤더파일과 라이브러리 포함시키기
// 헤더파일 디렉토리 추가하기: 프로젝트 메뉴 -> 맨 아래에 있는 프로젝트 속성 -> VC++ 디렉토리 -> 일반의 포함 디렉토리 -> 편집으로 가서 현재 디렉토리의 include 디렉토리 추가하기
// 라이브러리 디렉토리 추가하기: 프로젝트 메뉴 -> 맨 아래에 있는 프로젝트 속성 -> VC++ 디렉토리 -> 일반의 라이브러리 디렉토리 -> 편집으로 가서 현재 디렉토리의 lib 디렉토리 추가하기

#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <gl/glew.h>				
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma warning(disable: 4711 4710 4100)

GLvoid keyboard(unsigned char, int, int);
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

void InitBuffer();

GLuint shaderProgramID;	
GLuint vertexShader;	
GLuint fragmentShader;	

GLuint vao, vbo[2];
GLuint vao_pyramid, vbo_pyramid[2];
GLuint vao_lightcube, vbo_lightcube[2];

bool light_Frag = true;

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb"); 		
	if (!fptr) 			
		return NULL;
	fseek(fptr, 0, SEEK_END); 		
	length = ftell(fptr); 			
	buf = (char*)malloc(length + 1); 		
	fseek(fptr, 0, SEEK_SET); 		
	fread(buf, length, 1, fptr); 		
	fclose(fptr); 			
	buf[length] = 0; 			

	return buf; 			
}


int main(int argc, char** argv) 			
{	
	//--- 윈도우 생성하기
	glutInit(&argc, argv);				
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 		
	glutInitWindowPosition(0, 0); 			
	glutInitWindowSize(1280, 960); 			
	glutCreateWindow("2025 Coding Test-Computer Graphics"); 			

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)			
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";


	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders();	
	make_fragmentShaders();	
	shaderProgramID = make_shaderProgram();
	
	InitBuffer();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene); 			
	glutReshapeFunc(Reshape);		

	glutMainLoop();				
}
float r = 1.6f;
bool toggle_n = false;
bool toggle_y = false;
bool toggle_r = false;
GLvoid keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case'q':
		exit(0);
		break;
	case 'm':
		light_Frag = !light_Frag;
		glutPostRedisplay(); // 화면 갱신
		break;
	case 'n':
		toggle_n=!toggle_n;
		glutPostRedisplay(); // 화면 갱신
		break;
	case 'z':
		if (r > 0.7f ) {
			r -= 0.1f;
		}
		glutPostRedisplay(); // 화면 갱신
		break;
	case 'Z':
		if (r < 4.0f) {
			r += 0.1f;
		}
		glutPostRedisplay(); // 화면 갱신
		break;
	case 'y':
		toggle_y = !toggle_y;
		break;
	case 'r':
		toggle_r = !toggle_r;
		break;
	}
}

const GLfloat cube_vertices[36][3] = {
	// 앞면 (z = 0.5)
	{-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
	{-0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},

	// 뒷면 (z = -0.5)
	{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
	{-0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},

	// 상면 (y = 0.5)
	{-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
	{-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},

	// 하면 (y = -0.5)
	{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f},
	{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},

	// 오른면 (x = 0.5)
	{ 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
	{ 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},

	// 왼면 (x = -0.5)
	{-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f},
	{-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}
};
const GLfloat cube_normals[36][3] = {
	// 앞면 (z = 0.5)
	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},

	// 뒷면 (z = -0.5)
	{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
	{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},

	// 상면 (y = 0.5)
	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},

	// 하면 (y = -0.5)
	{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},

	// 오른면 (x = 0.5)
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},

	// 왼면 (x = -0.5)
	{-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}
};

const GLfloat pyramid_vertices[18][3] = {
	// 바닥 (두 삼각형)
	{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f},
	{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f},

	// 앞면
	{-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},  {0.0f, 0.5f, 0.0f},

	// 오른면
	{0.5f, -0.5f, 0.5f},   {0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.0f},

	// 뒷면
	{0.5f, -0.5f, -0.5f},  {-0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.0f},

	// 왼면
	{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f},  {0.0f, 0.5f, 0.0f}
};
const GLfloat pyramid_normals[18][3] = {
	// 바닥 (두 삼각형, 모두 (0, -1, 0))
	{0.000000f, -1.000000f, 0.000000f}, {0.000000f, -1.000000f, 0.000000f}, {0.000000f, -1.000000f, 0.000000f},
	{0.000000f, -1.000000f, 0.000000f}, {0.000000f, -1.000000f, 0.000000f}, {0.000000f, -1.000000f, 0.000000f},

	// 앞면 ((0, 0.447214, 0.894427))
	{0.000000f, 0.447214f, 0.894427f}, {0.000000f, 0.447214f, 0.894427f}, {0.000000f, 0.447214f, 0.894427f},

	// 오른면 ((0.894427, 0.447214, 0))
	{0.894427f, 0.447214f, 0.000000f}, {0.894427f, 0.447214f, 0.000000f}, {0.894427f, 0.447214f, 0.000000f},

	// 뒷면 ((0, 0.447214, -0.894427))
	{0.000000f, 0.447214f, -0.894427f}, {0.000000f, 0.447214f, -0.894427f}, {0.000000f, 0.447214f, -0.894427f},

	// 왼면 ((-0.894427, 0.447214, 0))
	{-0.894427f, 0.447214f, 0.000000f}, {-0.894427f, 0.447214f, 0.000000f}, {-0.894427f, 0.447214f, 0.000000f}
};

const GLfloat lightcube_vertices[36][3] = {
	// 앞면
	{-0.2f, -0.2f,  0.2f}, { 0.2f, -0.2f,  0.2f}, { 0.2f,  0.2f,  0.2f},
	{-0.2f, -0.2f,  0.2f}, { 0.2f,  0.2f,  0.2f}, {-0.2f,  0.2f,  0.2f},
	// 뒷면
	{-0.2f, -0.2f, -0.2f}, { 0.2f, -0.2f, -0.2f}, { 0.2f,  0.2f, -0.2f},
	{-0.2f, -0.2f, -0.2f}, { 0.2f,  0.2f, -0.2f}, {-0.2f,  0.2f, -0.2f},
	// 상면
	{-0.2f,  0.2f, -0.2f}, { 0.2f,  0.2f, -0.2f}, { 0.2f,  0.2f,  0.2f},
	{-0.2f,  0.2f, -0.2f}, { 0.2f,  0.2f,  0.2f}, {-0.2f,  0.2f,  0.2f},
	// 하면
	{-0.2f, -0.2f, -0.2f}, { 0.2f, -0.2f, -0.2f}, { 0.2f, -0.2f,  0.2f},
	{-0.2f, -0.2f, -0.2f}, { 0.2f, -0.2f,  0.2f}, {-0.2f, -0.2f,  0.2f},
	// 오른면
	{ 0.2f, -0.2f, -0.2f}, { 0.2f,  0.2f, -0.2f}, { 0.2f,  0.2f,  0.2f},
	{ 0.2f, -0.2f, -0.2f}, { 0.2f,  0.2f,  0.2f}, { 0.2f, -0.2f,  0.2f},
	// 왼면
	{-0.2f, -0.2f, -0.2f}, {-0.2f,  0.2f, -0.2f}, {-0.2f,  0.2f,  0.2f},
	{-0.2f, -0.2f, -0.2f}, {-0.2f,  0.2f,  0.2f}, {-0.2f, -0.2f,  0.2f}
};



const GLfloat cube_colors[6][3] = {
	// 상면: 녹색
	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
};

GLuint ebo;
unsigned int lightPosLocation;
void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	// 정점 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 법선 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &vao_pyramid);
	glBindVertexArray(vao_pyramid);

	glGenBuffers(2, vbo_pyramid);

	// 정점 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 법선 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_normals), pyramid_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 조명 큐브 VAO 생성
	glGenVertexArrays(1, &vao_lightcube);
	glBindVertexArray(vao_lightcube);
	glGenBuffers(1, vbo_lightcube);

	// 정점 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, vbo_lightcube[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightcube_vertices), lightcube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgramID);
	lightPosLocation = glGetUniformLocation(shaderProgramID, "lightpos");
	glUniform3f(lightPosLocation,-0.4f , 0.0f,1.6f);
	int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor");
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
	int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor");
	glUniform3f(objColorLocation, 0.0f, 1.0f, 0.0f);
}

float angle = 0.0f;
float light_angle = 100.0f;

GLvoid drawScene() 				//--- 콜백 함수: 출력 콜백 함수
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);

	glm::mat4 view = glm::lookAt (
		glm::vec3(-3.0f, 3.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 projection = glm::perspective(
		glm::radians(70.0f),
		1280.0f / 960.0f,
		0.1f,
		100.0f
	);
	
	GLint locview = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(locview, 1, GL_FALSE, glm::value_ptr(view));

	GLint locprojection = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(locprojection, 1, GL_FALSE, glm::value_ptr(projection));

	GLint lightOn_Frag = glGetUniformLocation(shaderProgramID, "lightOn");
	glUniform1i(lightOn_Frag, light_Frag ? 1 : 0);
	GLint locModel = glGetUniformLocation(shaderProgramID, "model");
	

	glm::mat4 model = glm::mat4(1.0f);

	//y축 회전
	if (toggle_y) {
		angle += 0.5f;
		if (angle >= 360.0f) angle -= 360.0f;
	}
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	
	if (!toggle_n) {
		glBindVertexArray(vao_pyramid);
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 18);
	}
	else {
		glBindVertexArray(vao);
		glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(vao_lightcube);
	glm::mat4 model_light = glm::mat4(1.0f);

	if (toggle_r) { // 물체 공전
		light_angle += 0.5f;
		if (light_angle >= 360.0f) light_angle -= 360.0f;
	}


	float center_x = 0.0f, center_z = 0.0f;
	float rad = glm::radians(light_angle);
	float orbit_x = center_x + r * cos(rad);
	float orbit_z = center_z + r * sin(rad);
	float orbit_y = 0.0f;


	// 조명 위치를 쉐이더에 전달
	glUniform3f(lightPosLocation, orbit_x, orbit_y, orbit_z);


	model_light = glm::translate(model_light, glm::vec3(orbit_x, orbit_y, orbit_z));
	glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model_light));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model_light = glm::mat4(1.0f);
	glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model_light));

	//궤도 그리기
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float rad = glm::radians((float)i);
		float x = center_x + r * cos(rad);
		float z = center_z + r * sin(rad);
		glVertex3f(x, 0.0f, z);
	}
	glEnd();

	glutPostRedisplay();
	glutSwapBuffers(); 			

}


GLvoid Reshape(int w, int h)			//--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}


void make_vertexShaders()
{
	GLchar* vertexSource;

	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;

	
	fragmentSource = filetobuf("fragment.glsl");    
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**) & fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram();			

	glAttachShader(shaderID, vertexShader);		
	glAttachShader(shaderID, fragmentShader);		

	glLinkProgram(shaderID);			

	glDeleteShader(vertexShader);			
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}

	glUseProgram(shaderID);			

	return shaderID;
}

