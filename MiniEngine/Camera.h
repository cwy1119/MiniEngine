#pragma once
#include <GLFW/glfw3.h>
class Camera
{
private:
	int& w_width;
	int& w_height;
public:
	float eye[3];
	float lookat[3];
	float up[3];
	double prevMouseX, prevMouseY;
	bool mouseLeftPressed;
	bool mouseMiddlePressed;
	bool mouseRightPressed;
	float curr_quat[4];
	float prev_quat[4];
	Camera(int& width, int& height);
	~Camera();
	void setLookAt();
	void clickFunc(GLFWwindow* window, int button, int action, int mods);
	void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y);
};

