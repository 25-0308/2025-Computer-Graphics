#include <GL/glut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define _USE_MATH_DEFINES // M_PI 사용을 위해 <cmath> 전에 정의
#include <cmath>
int mode = 0; // 0: 큐브, 1: 사각뿔
std::vector<bool> cube_faces(6, false); // 큐브 면 선택 (0-5)
std::vector<bool> pyramid_faces(5, false); // 사각뿔 면 (0: 바닥, 1-4: 옆면)
bool cull = false;
bool swap = false;
float rotation_angle_x = 0.0f;
float rotation_angle_y = 0.0f;
bool rotate_x = false;
bool rotate_y = false;
float rot_dir_x = 0.0f; // 0: no rotation, -1: negative x, +1: positive x
float rot_dir_y = 0.0f; // 0: no rotation, -1: negative y, +1: positive y
// 큐브 애니메이션 변수
float rot_angle_z = 0.0f; // t: z축 회전 각도
float rot_dir_z = 0.0f; // z축 회전 방향
float door_angle = 0.0f; // f: 앞면 문 열기 각도
float target_door = 0.0f; // 문 목표 각도
bool door_open = false; // 문 토글 상태
float side_rot_angle = 0.0f; // s: 옆면 회전 각도
float side_dir = 0.0f; // 옆면 회전 방향
float back_scale = 1.0f; // b: 뒷면 스케일
bool back_anim_on = false; // b: 애니메이션 on/off
bool back_anim_dir = false; // b: 애니메이션 방향 (false: 축소, true: 확대)
// 큐브 크기
float cube_half_size = 0.5f;
// 사각뿔 베이스 크기와 높이
float pyramid_half_base = 0.25f;
float pyramid_height = 0.5f;
float camera_x = 3.0f;
float camera_y = 3.0f;
float camera_z = 3.0f;
// 큐브
std::vector<std::vector<glm::vec3>> cube_vertices = {
	// 앞면 (z = cube_half_size) - CCW, y from 0 to 1
	{ glm::vec3(-cube_half_size, 0.0f, cube_half_size), glm::vec3(cube_half_size, 0.0f, cube_half_size), glm::vec3(cube_half_size, 1.0f, cube_half_size), glm::vec3(-cube_half_size, 1.0f, cube_half_size) },
	// 뒷면 (z = -cube_half_size)
	{ glm::vec3(-cube_half_size, 0.0f, -cube_half_size), glm::vec3(cube_half_size, 0.0f, -cube_half_size), glm::vec3(cube_half_size, 1.0f, -cube_half_size), glm::vec3(-cube_half_size, 1.0f, -cube_half_size) },
	// 윗면 (y = 1.0)
	{ glm::vec3(-cube_half_size, 1.0f, -cube_half_size), glm::vec3(-cube_half_size, 1.0f, cube_half_size), glm::vec3(cube_half_size, 1.0f, cube_half_size), glm::vec3(cube_half_size, 1.0f, -cube_half_size) },
	// 아랫면 (y = 0.0)
	{ glm::vec3(-cube_half_size, 0.0f, -cube_half_size), glm::vec3(-cube_half_size, 0.0f, cube_half_size), glm::vec3(cube_half_size, 0.0f, cube_half_size), glm::vec3(cube_half_size, 0.0f, -cube_half_size) },
	// 오른쪽 면 (x = cube_half_size)
	{ glm::vec3(cube_half_size, 0.0f, -cube_half_size), glm::vec3(cube_half_size, 1.0f, -cube_half_size), glm::vec3(cube_half_size, 1.0f, cube_half_size), glm::vec3(cube_half_size, 0.0f, cube_half_size) },
	// 왼쪽 면 (x = -cube_half_size)
	{ glm::vec3(-cube_half_size, 1.0f, -cube_half_size), glm::vec3(-cube_half_size, 1.0f, cube_half_size), glm::vec3(-cube_half_size, 0.0f, cube_half_size), glm::vec3(-cube_half_size, 0.0f, -cube_half_size) }
};
// 삼각형 (사각뿔)
std::vector<std::vector<glm::vec3>> pyramid_vertices = {
	// 바닥 (xz 평면 y=0)
	{ glm::vec3(-pyramid_half_base, 0.0f, pyramid_half_base), glm::vec3(pyramid_half_base, 0.0f, pyramid_half_base), glm::vec3(pyramid_half_base, 0.0f, -pyramid_half_base), glm::vec3(-pyramid_half_base, 0.0f, -pyramid_half_base) },
	// 옆면 1 (앞)
	{ glm::vec3(-pyramid_half_base, 0.0f, pyramid_half_base), glm::vec3(pyramid_half_base, 0.0f, pyramid_half_base), glm::vec3(0.0f, pyramid_height, 0.0f) },
	// 옆면 2 (오른쪽)
	{ glm::vec3(pyramid_half_base, 0.0f, pyramid_half_base), glm::vec3(pyramid_half_base, 0.0f, -pyramid_half_base), glm::vec3(0.0f, pyramid_height, 0.0f) },
	// 옆면 3 (뒤)
	{ glm::vec3(pyramid_half_base, 0.0f, -pyramid_half_base), glm::vec3(-pyramid_half_base, 0.0f, -pyramid_half_base), glm::vec3(0.0f, pyramid_height, 0.0f) },
	// 옆면 4 (왼쪽)
	{ glm::vec3(-pyramid_half_base, 0.0f, -pyramid_half_base), glm::vec3(-pyramid_half_base, 0.0f, pyramid_half_base), glm::vec3(0.0f, pyramid_height, 0.0f) }
};
// 색
std::vector<glm::vec3> colors = {
glm::vec3(1.0, 0.0, 0.0), // 빨강
glm::vec3(0.0, 1.0, 0.0), // 초록
glm::vec3(0.0, 0.0, 1.0), // 파랑
glm::vec3(1.0, 1.0, 0.0), // 노랑
glm::vec3(1.0, 0.0, 1.0), // 자홍
glm::vec3(0.0, 1.0, 1.0), // 청록
glm::vec3(1.0, 0.5, 0.0), // 주황
glm::vec3(0.5, 0.0, 1.0), // 보라
glm::vec3(0.5, 1.0, 0.0), // 라임
glm::vec3(1.0, 0.5, 0.5) // 분홍
};
void animate() {
	static float last_time = 0.0f;
	float current_time = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	if (last_time > 0.0f) {
		float delta = current_time - last_time;
		if (rot_dir_x != 0.0f) {
			rotation_angle_x += rot_dir_x * 90.0f * delta; // 90 degrees per second
		}
		if (rot_dir_y != 0.0f) {
			rotation_angle_y += rot_dir_y * 90.0f * delta; // 90 degrees per second
		}
		if (mode == 0) { // 큐브 모드에서만 애니메이션
			if (rot_dir_z != 0.0f) {
				rot_angle_z += rot_dir_z * 45.0f * delta; // z축 회전 속도
			}
			if (door_open) {
				float sign = (target_door > door_angle) ? 1.0f : ((target_door < door_angle) ? -1.0f : 0.0f);
				door_angle += sign * 90.0f * delta;
				if (std::abs(door_angle - target_door) < 1.0f) {
					door_angle = target_door;
					door_open = false;
				}
			}
			if (side_dir != 0.0f) {
				side_rot_angle += side_dir * 60.0f * delta; // 옆면 회전 속도
			}
			if (back_anim_on) {
				if (!back_anim_dir) {
					back_scale -= 2.0f * delta; // 축소 속도
					if (back_scale <= 0.0f) {
						back_scale = 0.0f;
						back_anim_dir = true;
						back_anim_on = false;
					}
				}
				else {
					back_scale += 2.0f * delta; // 확대 속도
					if (back_scale >= 1.0f) {
						back_scale = 1.0f;
						back_anim_dir = false;
						back_anim_on = false;
					}
				}
			}
		}
	}
	last_time = current_time;
	glutPostRedisplay();
}
void draw_axes(glm::mat4& model_view) {
	glDisable(GL_DEPTH_TEST); // 축이 가려지지 않게
	// 모델-뷰 행렬 적용 (축도 변환 적용)
	glMultMatrixf(glm::value_ptr(model_view));
	// x축 빨강
	{
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex3f(-1.0, 0.5f, 0.0); // y=0.5 (객체 중심)로 조정
		glVertex3f(1.0, 0.5f, 0.0);
		glEnd();
	}
	// y축 초록 (바닥 y=0부터 위로)
	{
		glColor3f(0.0, 1.0, 0.0);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0); // 바닥 y=0
		glVertex3f(0.0, 1.5, 0.0); // 위로 1.5까지 (객체 높이 고려)
		glEnd();
	}
	// z축 파랑
	{
		glColor3f(0.0, 0.0, 1.0);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.5f, -1.0); // y=0.5로 조정
		glVertex3f(0.0, 0.5f, 1.0);
		glEnd();
		glLineWidth(1.0);
		glEnable(GL_DEPTH_TEST);
	}
}
void draw_face(int face_idx, bool is_cube, glm::mat4& model) {
	if (!is_cube || mode != 0) { // 피라미드나 큐브 아닌 경우 기본
		// 모델 행렬 적용
		glPushMatrix();
		glMultMatrixf(glm::value_ptr(model));
		if (is_cube) {
			// 큐브 면
			glBegin(GL_POLYGON);
			glColor3fv(glm::value_ptr(colors[face_idx % colors.size()]));
			for (const auto& vert : cube_vertices[face_idx]) {
				glVertex3fv(glm::value_ptr(vert));
			}
			glEnd();
		}
		else {
			// 사각뿔 면
			if (face_idx == 0) {
				// 바닥
				glBegin(GL_POLYGON);
				glColor3fv(glm::value_ptr(colors[0]));
				for (const auto& vert : pyramid_vertices[0]) {
					glVertex3fv(glm::value_ptr(vert));
				}
				glEnd();
			}
			else {
				// 옆면
				glBegin(GL_TRIANGLES);
				glColor3fv(glm::value_ptr(colors[(face_idx - 1) % colors.size()]));
				for (size_t i = 0; i < pyramid_vertices[face_idx].size(); ++i) {
					glVertex3fv(glm::value_ptr(pyramid_vertices[face_idx][i]));
				}
				glEnd();
			}
		}
		glPopMatrix();
		return;
	}
	// 큐브 모드: 면 별 애니메이션 적용
	const float pi = 3.141592653589793f;
	glm::mat4 face_model = model;
	switch (face_idx) {
	case 0: // 앞면: 문 열기 (위축 x축 기준)
		face_model = glm::translate(face_model, glm::vec3(0.0f, 0.0f, cube_half_size));
		face_model = glm::rotate(face_model, door_angle * pi / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		face_model = glm::translate(face_model, glm::vec3(0.0f, 0.0f, -cube_half_size));
		break;
	case 1: // 뒷면: 스케일 (자기 중심에서)
		face_model = glm::translate(face_model, glm::vec3(0.0f, 0.5f, -cube_half_size));
		face_model = glm::scale(face_model, glm::vec3(back_scale, back_scale, back_scale));
		face_model = glm::translate(face_model, glm::vec3(0.0f, -0.5f, cube_half_size));
		break;
	case 2: // 윗면: z축 회전 (가운데 중심)
		face_model = glm::translate(face_model, glm::vec3(0.0f, 1.0f, 0.0f));
		face_model = glm::rotate(face_model, rot_angle_z * pi / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		face_model = glm::translate(face_model, glm::vec3(0.0f, -1.0f, 0.0f));
		break;
	case 4: // 오른쪽 면: 제자리 회전 (중점 y축)
		face_model = glm::translate(face_model, glm::vec3(cube_half_size, 0.5f, 0.0f));
		face_model = glm::rotate(face_model, side_rot_angle * pi / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		face_model = glm::translate(face_model, glm::vec3(-cube_half_size, -0.5f, 0.0f));
		break;
	case 5: // 왼쪽 면: 제자리 회전 (중점 y축)
		face_model = glm::translate(face_model, glm::vec3(-cube_half_size, 0.5f, 0.0f));
		face_model = glm::rotate(face_model, side_rot_angle * pi / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		face_model = glm::translate(face_model, glm::vec3(cube_half_size, -0.5f, 0.0f));
		break;
	default:
		break;
	}
	// 모델 행렬 적용
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(face_model));
	glBegin(GL_POLYGON);
	glColor3fv(glm::value_ptr(colors[face_idx % colors.size()]));
	for (const auto& vert : cube_vertices[face_idx]) {
		glVertex3fv(glm::value_ptr(vert));
	}
	glEnd();
	glPopMatrix();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// 카메라
	glm::mat4 view = glm::lookAt(
		glm::vec3(camera_x, camera_y, camera_z), // 카메라 위치
		glm::vec3(0.0f, 0.0f, 0.0f), // 시선 방향
		glm::vec3(0.0f, 1.0f, 0.0f) // 업 벡터
	);
	glMultMatrixf(glm::value_ptr(view));
	// 축: 고정 (회전 안 함)
	glm::mat4 axes_model = glm::mat4(1.0f);
	draw_axes(axes_model);
	// 객체: 회전 적용
	glm::mat4 model = glm::mat4(1.0f);
	const float pi = 3.141592653589793f;
	model = glm::rotate(model, rotation_angle_x * pi / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation_angle_y * pi / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (mode == 0) { // 큐브 모드
		// 지정된 면들 그리기
		for (int i = 0; i < 6; ++i) {
			if (cube_faces[i]) draw_face(i, true, model);
		}
	}
	else { // 사각뿔 모드
		// 지정된 면들 그리기
		for (int i = 1; i < 5; ++i) {
			if (pyramid_faces[i]) draw_face(i, false, model);
		}
		if (pyramid_faces[0]) draw_face(0, false, model);
	}
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'c':
		swap = false;
		mode = 0;
		std::fill(cube_faces.begin(), cube_faces.end(), true);
		// 모든 애니메이션 초기화
		rotation_angle_x = 0.0f;
		rotation_angle_y = 0.0f;
		rotate_x = false;
		rotate_y = false;
		rot_dir_x = 0.0f;
		rot_dir_y = 0.0f;
		rot_angle_z = 0.0f;
		rot_dir_z = 0.0f;
		door_angle = 0.0f;
		target_door = 0.0f;
		door_open = false;
		side_rot_angle = 0.0f;
		side_dir = 0.0f;
		back_scale = 1.0f;
		back_anim_on = false;
		back_anim_dir = false;
		glutPostRedisplay();
		break;
	case 'p':
		if (!swap) {
			mode = 0;
			std::fill(cube_faces.begin(), cube_faces.end(), true);
		}
		else {
			mode = 1;
			std::fill(pyramid_faces.begin(), pyramid_faces.end(), true);
		}
		swap = !swap;
		glutPostRedisplay();
		break;
	case 'h': //은면제거 on/off
		cull = !cull;
		if (cull) {
			glEnable(GL_CULL_FACE);
		}
		else {
			glDisable(GL_CULL_FACE);
		}
		glutPostRedisplay();
		break;
	case 'x':
		if (rotate_y) {
			rotation_angle_y = 0.0f;
			rot_dir_y = 0.0f;
			rotate_y = false;
		}
		else {
			rotate_x = true;
			rot_dir_x = (rot_dir_x == 0.0f) ? -1.0f : -rot_dir_x; // 0 -> -1 (음의), -1 -> +1 (양의), +1 -> -1 (토글)
		}
		glutPostRedisplay();
		break;
	case 'y':
		rotate_y = true;
		rot_dir_y = (rot_dir_y == 0.0f) ? -1.0f : -rot_dir_y; // 0 -> -1 (음의), -1 -> +1 (양의), +1 -> -1 (토글)
		glutPostRedisplay();
		break;
	case 't': // 윗면 z축 회전 토글
		if (mode == 0) {
			rot_dir_z = (rot_dir_z == 0.0f) ? 1.0f : 0.0f; // 토글 시작/멈춤
		}
		glutPostRedisplay();
		break;
	case 'f': // 앞면 열기/닫기
		if (mode == 0) {
			target_door = (std::abs(door_angle - 90.0f) < 1.0f) ? 0.0f : 90.0f;
			door_open = true; // 애니메이션 시작
		}
		glutPostRedisplay();
		break;
	case 's': // 옆면 회전 토글 (기존 s와 공유, 하지만 큐브에서만)
		if (mode == 0) {
			side_dir = (side_dir == 0.0f) ? 1.0f : 0.0f; // 토글
		}
		else {
			// 피라미드에서 기존 초기화
			rotation_angle_x = 0.0f;
			rotation_angle_y = 0.0f;
			rot_dir_x = 0.0f;
			rot_dir_y = 0.0f;
		}
		glutPostRedisplay();
		break;
	case 'b': // 뒷면 스케일 애니메이션 토글
		if (mode == 0) {
			if (std::abs(back_scale - 1.0f) < 0.01f) {
				back_anim_dir = false;
				back_anim_on = true;
			}
			else if (std::abs(back_scale - 0.0f) < 0.01f) {
				back_anim_dir = true;
				back_anim_on = true;
			}
		}
		glutPostRedisplay();
		break;
	case 27: // ESC
		exit(0);
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	glutIdleFunc(animate); // 애니메이션 시작
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("15");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

