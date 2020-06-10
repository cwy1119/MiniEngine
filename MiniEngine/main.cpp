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



#include "Camera.h"
#include "AnimationPlayer.h"
#include "Manager.h"
#include "ScreenShot.h"
#include "Light.h"

//结构声明
typedef enum{ViewMode,MergeMode,AnimationMode}RunMode;


//函数声明
static void reshapeFunc(GLFWwindow* window, int w, int h);
static void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
static void clickFunc(GLFWwindow* window, int button, int action, int mods);
static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y);			//回调函数, 各模块根据需要添加代码
static void scrollFunc(GLFWwindow*, double, double);



static void Init();

//全局变量
RunMode runMode;
int w_width = 768;
int w_height = 768;
GLFWwindow* window;

Camera camera(w_width,w_height);
Light light;
AnimationPlayer* ap;
Manager* manager;

static void Init()
{
	
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Too few arguments" << std::endl;
		return -1;
	}
	else if(strcmp(argv[1],"-v") == 0){
		runMode = ViewMode;
		//todo, obj显示模块,由大家负责一起实现
		
		
	}
	else if (strcmp(argv[1], "-m") == 0){
		runMode = MergeMode;
		//todo, 合并obj模块,由cwy负责实现,用于检测obj文件的读取和写入 
	}
	else if (strcmp(argv[1], "-a") == 0){
		runMode = AnimationMode;
		//todo, 动画播放模块, 由yyn负责实现, 用于一系列obj文件的读取和播放
		ap = new AnimationPlayer(argv[2], atoi(argv[3]), atoi(argv[4]));
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
	glfwSetScrollCallback(window, scrollFunc);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return -1;
	}


	//To do: 循环前的初始化代码放这
	reshapeFunc(window, w_width, w_height);
	if (runMode == ViewMode) {
		manager = new Manager();
		//manager->appendObjFile(argv[2]);
		manager->appendObjFile(MODEL_PYRAMID, 5);
	}else if (runMode == AnimationMode) {
		ap->load();
	}
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		//To do:循环内的初始化代码放这
		glfwPollEvents();
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		camera.setLookAt();

		light.init();

		if (runMode == ViewMode) {
			manager->loop();
			manager->draw();
		}
		else if (runMode == AnimationMode) {
			while (glfwWindowShouldClose(window) == GL_FALSE) {
				bool ret = ap->flush(glfwGetTime());
				if(ret) break;
				
			}
		}
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
	//灯光监听部分
	light.keyboardFunc(window, key, scancode, action, mods);

	if (runMode == ViewMode) manager->keyboardFunc(window, key, scancode, action, mods);
	else if(runMode == AnimationMode) ap->keyboardFunc(window, key, scancode, action, mods);

	//截图部分
	if (action == GLFW_PRESS && key == GLFW_KEY_X) {
		ScreenShot("1.bmp", w_width, w_height);
	}

}

static void clickFunc(GLFWwindow* window, int button, int action, int mods) {
	(void)window;
	(void)mods;

	//摄像头监控部分
	camera.clickFunc(window, button, action, mods);

	//todo: 各模块的代码放这
	
}


static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y) {
	(void)window;
	

	//摄像头监控部分
	camera.motionFunc(window, mouse_x, mouse_y);

	//todo: 各模块的代码放这
}

static void screenShot(std::string filename, int x, int y, int width, int height) {
	//lxy实现这一部分
}

static void scrollFunc(GLFWwindow* window, double x, double y) {
	if (runMode == ViewMode) {

	}
	else if (runMode == AnimationMode) {
		ap->scrollFunc(window, x, y);
	}
}