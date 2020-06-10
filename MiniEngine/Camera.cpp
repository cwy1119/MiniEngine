#include <GL/glew.h>
#include<GL/glu.h>
#include "Camera.h"
#include "trackball.h"


Camera::Camera(int& width, int& height):
	w_width(width),w_height(height)
{
	
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 3.0f;

	lookat[0] = 0.0f;
	lookat[1] = 0.0f;
	lookat[2] = 0.0f;

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
	trackball(curr_quat, 0, 0, 0, 0);
}


Camera::~Camera()
{
}

void Camera::clickFunc(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			mouseLeftPressed = true;
			trackball(prev_quat, 0.0, 0.0, 0.0, 0.0);
		}
		else if (action == GLFW_RELEASE) {
			mouseLeftPressed = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			mouseRightPressed = true;
		}
		else if (action == GLFW_RELEASE) {
			mouseRightPressed = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (action == GLFW_PRESS) {
			mouseMiddlePressed = true;
		}
		else if (action == GLFW_RELEASE) {
			mouseMiddlePressed = false;
		}
	}
}

void Camera::motionFunc(GLFWwindow* window, double mouse_x, double mouse_y)
{
	float rotScale = 1.0f;
	float transScale = 2.0f;
	if (mouseLeftPressed) {

		trackball(prev_quat, rotScale * (2.0f * prevMouseX - w_width) / (float)w_width,
			rotScale * (w_height - 2.0f * prevMouseY) / (float)w_height,
			rotScale * (2.0f * mouse_x - w_width) / (float)w_width,
			rotScale * (w_height - 2.0f * mouse_y) / (float)w_height);

		add_quats(prev_quat, curr_quat, curr_quat);


	}
	else if (mouseMiddlePressed) {
		eye[0] -= transScale * (mouse_x - prevMouseX) / (float)w_width;
		lookat[0] -= transScale * (mouse_x - prevMouseX) / (float)w_width;
		eye[1] += transScale * (mouse_y - prevMouseY) / (float)w_height;
		lookat[1] += transScale * (mouse_y - prevMouseY) / (float)w_height;
	}
	else if (mouseRightPressed) {
		eye[2] += transScale * (mouse_y - prevMouseY) / (float)w_height;
		lookat[2] += transScale * (mouse_y - prevMouseY) / (float)w_height;
	}

	// Update mouse point
	prevMouseX = mouse_x;
	prevMouseY = mouse_y;
}
void Camera::setLookAt()
{
	GLfloat mat[4][4];
	gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up[0],
		up[1], up[2]);
	build_rotmatrix(mat, curr_quat);
	glMultMatrixf(&mat[0][0]);
}