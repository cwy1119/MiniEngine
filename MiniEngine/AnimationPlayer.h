#pragma once
#include <string>
#include <vector>

#include "ObjFile.h"
#include <GLFW/glfw3.h>
using std::string;
class AnimationPlayer
{
	
private:
	string path;
	int start;
	int end;

	int next;
	std::vector<ObjFile> a_objFile;
	double nowTime, lastTime, deltaTime;
	bool isPause;
	double FPS;
public:
	AnimationPlayer(string path,int start,int end);
	~AnimationPlayer();
	void load();
	bool flush(double nowTime);
	void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
};