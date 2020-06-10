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

//�ṹ����
typedef enum{ViewMode,MergeMode,AnimationMode}RunMode;


//��������
static void reshapeFunc(GLFWwindow* window, int w, int h);
static void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
static void clickFunc(GLFWwindow* window, int button, int action, int mods);
static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y);			//�ص�����, ��ģ�������Ҫ��Ӵ���
static void scrollFunc(GLFWwindow*, double, double);



static void Init();

//ȫ�ֱ���
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
		//todo, obj��ʾģ��,�ɴ�Ҹ���һ��ʵ��
		
		
	}
	else if (strcmp(argv[1], "-m") == 0){
		runMode = MergeMode;
		//todo, �ϲ�objģ��,��cwy����ʵ��,���ڼ��obj�ļ��Ķ�ȡ��д�� 
	}
	else if (strcmp(argv[1], "-a") == 0){
		runMode = AnimationMode;
		//todo, ��������ģ��, ��yyn����ʵ��, ����һϵ��obj�ļ��Ķ�ȡ�Ͳ���
		ap = new AnimationPlayer(argv[2], atoi(argv[3]), atoi(argv[4]));
	}
	else {
		std::cout << "Unknown argument: " << argv[1] << std::endl;
		return -1;
	}

	Init(); //��ʼ������

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

	// �ص�����
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


	//To do: ѭ��ǰ�ĳ�ʼ���������
	reshapeFunc(window, w_width, w_height);
	if (runMode == ViewMode) {
		manager = new Manager();
		//manager->appendObjFile(argv[2]);
		manager->appendObjFile(MODEL_PYRAMID, 5);
	}else if (runMode == AnimationMode) {
		ap->load();
	}
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		//To do:ѭ���ڵĳ�ʼ���������
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
	//�ƹ��������
	light.keyboardFunc(window, key, scancode, action, mods);

	if (runMode == ViewMode) manager->keyboardFunc(window, key, scancode, action, mods);
	else if(runMode == AnimationMode) ap->keyboardFunc(window, key, scancode, action, mods);

	//��ͼ����
	if (action == GLFW_PRESS && key == GLFW_KEY_X) {
		ScreenShot("1.bmp", w_width, w_height);
	}

}

static void clickFunc(GLFWwindow* window, int button, int action, int mods) {
	(void)window;
	(void)mods;

	//����ͷ��ز���
	camera.clickFunc(window, button, action, mods);

	//todo: ��ģ��Ĵ������
	
}


static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y) {
	(void)window;
	

	//����ͷ��ز���
	camera.motionFunc(window, mouse_x, mouse_y);

	//todo: ��ģ��Ĵ������
}

static void screenShot(std::string filename, int x, int y, int width, int height) {
	//lxyʵ����һ����
}

static void scrollFunc(GLFWwindow* window, double x, double y) {
	if (runMode == ViewMode) {

	}
	else if (runMode == AnimationMode) {
		ap->scrollFunc(window, x, y);
	}
}