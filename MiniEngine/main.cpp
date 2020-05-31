#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>
#include<GL/glu.h>
#include<GLFW/glfw3.h>
#pragma warning(disable : 4996)


#include "ObjFile.h"
#include "Camera.h"
#include "trackball.h"
//结构声明
typedef enum{ViewMode,MergeMode,AnimationMode}RunMode;


//函数声明
static void reshapeFunc(GLFWwindow* window, int w, int h);
static void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
static void clickFunc(GLFWwindow* window, int button, int action, int mods);
static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y);			//回调函数, 各模块根据需要添加代码

static void screenShot(std::string filename, int x, int y, int width, int height); //截图函数, 由lxy负责实现


static void Init();

//全局变量
RunMode runMode;
int w_width = 768;
int w_height = 768;
GLFWwindow* window;
ObjFile objFile("");
Camera camera;


static void Init()
{
	trackball(camera.curr_quat, 0, 0, 0, 0);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Too few arguments" << std::endl;
		return -1;
	}
	else if(strcmp(argv[1],"-v") == 0){
		runMode = ViewMode;
		//todo, obj显示模块,由大家负责一起实现
		objFile.setFilename(argv[2]);
	}
	else if (strcmp(argv[1], "-m") == 0){
		runMode = MergeMode;
		//todo, 合并obj模块,由cwy负责实现,用于检测obj文件的读取和写入 
	}
	else if (strcmp(argv[1], "-a") == 0){
		runMode = AnimationMode;
		//todo, 动画播放模块, 由yyn负责实现, 用于一系列obj文件的读取和播放

	}
	else {
		std::cout << "Unknown argument: " << argv[1] << std::endl;
		return -1;
	}

	Init(); //初始化操作

	if (!glfwInit()) {
		std::cerr << "Fail to initialize GLEW." << std::endl;
		return -1;
	}

	window = glfwCreateWindow(w_width, w_height, "miniEngine", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to open GLFW window. " << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// 回调函数
	glfwSetWindowSizeCallback(window, reshapeFunc);
	glfwSetKeyCallback(window, keyboardFunc);
	glfwSetMouseButtonCallback(window, clickFunc);
	glfwSetCursorPosCallback(window, motionFunc);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return -1;
	}


	//To do: 循环前的初始化代码放这
	reshapeFunc(window, w_width, w_height);
	objFile.loadFile();

	while (glfwWindowShouldClose(window) == GL_FALSE) {
		//To do:循环内的初始化代码放这
		glfwPollEvents();
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		GLfloat mat[4][4];
		gluLookAt(camera.eye[0], camera.eye[1], camera.eye[2], camera.lookat[0], camera.lookat[1], camera.lookat[2], camera.up[0],
			camera.up[1], camera.up[2]);
		build_rotmatrix(mat, camera.curr_quat);
		glMultMatrixf(&mat[0][0]);

		objFile.draw();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

static void reshapeFunc(GLFWwindow* window, int w, int h) {
	int fb_w, fb_h;
	glfwGetFramebufferSize(window, &fb_w, &fb_h);

	glViewport(0, 0, fb_w, fb_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 0.01f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	w_width = w;
	w_height = h;
}

static void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

static void clickFunc(GLFWwindow* window, int button, int action, int mods) {
	(void)window;
	(void)mods;

	//摄像头监控部分
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			camera.mouseLeftPressed = true;
			trackball(camera.prev_quat, 0.0, 0.0, 0.0, 0.0);
		}
		else if (action == GLFW_RELEASE) {
			camera.mouseLeftPressed = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			camera.mouseRightPressed = true;
		}
		else if (action == GLFW_RELEASE) {
			camera.mouseRightPressed = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (action == GLFW_PRESS) {
			camera.mouseMiddlePressed = true;
		}
		else if (action == GLFW_RELEASE) {
			camera.mouseMiddlePressed = false;
		}
	}

	//todo: 各模块的代码放这
}


static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y) {
	(void)window;
	float rotScale = 1.0f;
	float transScale = 2.0f;

	//摄像头监控部分
	if (camera.mouseLeftPressed) {
		
			trackball(camera.prev_quat, rotScale * (2.0f * camera.prevMouseX - w_width) / (float)w_width,
				rotScale * (w_height - 2.0f * camera.prevMouseY) / (float)w_height,
				rotScale * (2.0f * mouse_x - w_width) / (float)w_width,
				rotScale * (w_height - 2.0f * mouse_y) / (float)w_height);

			add_quats(camera.prev_quat, camera.curr_quat, camera.curr_quat);
		

	}
	else if (camera.mouseMiddlePressed) {
		camera.eye[0] -= transScale * (mouse_x - camera.prevMouseX) / (float)w_width;
		camera.lookat[0] -= transScale * (mouse_x - camera.prevMouseX) / (float)w_width;
		camera.eye[1] += transScale * (mouse_y - camera.prevMouseY) / (float)w_height;
		camera.lookat[1] += transScale * (mouse_y - camera.prevMouseY) / (float)w_height;
	}
	else if (camera.mouseRightPressed) {
		camera.eye[2] += transScale * (mouse_y - camera.prevMouseY) / (float)w_height;
		camera.lookat[2] += transScale * (mouse_y - camera.prevMouseY) / (float)w_height;
	}

	// Update mouse point
	camera.prevMouseX = mouse_x;
	camera.prevMouseY = mouse_y;

	//todo: 各模块的代码放这
}

static void screenShot(std::string filename, int x, int y, int width, int height) {
	//lxy实现这一部分
}