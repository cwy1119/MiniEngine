#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

void Light::keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_L: {
			bLight = !bLight;
			break;
		}
		case GLFW_KEY_T: {
			LightPosition[1] += 1;
			break;
		}
		case GLFW_KEY_G: {
			LightPosition[1] -= 1;
			break;
		}
		case GLFW_KEY_F: {
			LightPosition[0] -= 1;
			break;
		}
		case GLFW_KEY_H: {
			LightPosition[0] += 1;
			break;
		}
		case GLFW_KEY_V: {
			LightPosition[2] -= 1;
			break;
		}
		case GLFW_KEY_N: {
			LightPosition[2] += 1;
			break;
		}
		case GLFW_KEY_7: {
			AmbientLight[0] -= 0.1;
			break;
		}
		case GLFW_KEY_8: {
			AmbientLight[0] += 0.1;
			break;
		}
		case GLFW_KEY_9: {
			AmbientLight[1] -= 0.1;
			break;
		}
		case GLFW_KEY_0: {
			AmbientLight[1] += 0.1;
			break;
		}
		case GLFW_KEY_MINUS: {
			AmbientLight[2] -= 0.1;
			break;
		}
		case GLFW_KEY_EQUAL: {
			AmbientLight[2] += 0.1;
			break;
		}
		}
	}
}

void Light::init()
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	if (bLight == false) {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}
	else {
		glEnable(GL_LIGHT0);      //允许0#灯使用
		glEnable(GL_LIGHTING);   //开灯
	}
}
