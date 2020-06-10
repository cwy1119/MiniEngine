#pragma once
#include <GL/glew.h>
#include<GLFW/glfw3.h>
class Light
{
private:
	bool bLight = false;
	GLfloat AmbientLight[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat LightPosition[4] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat DiffuseLight[4] = { 0.0f,0.0f,1.0f,1.0f };
	GLfloat SpecularLight[4] = { 1.0f,1.0f,1.0f,1.0f };
public:
	Light();
	~Light();
	void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
	void init();
};

