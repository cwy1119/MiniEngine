#include "AnimationPlayer.h"
AnimationPlayer::AnimationPlayer(string path, int start, int end)
	:path(path),start(start),end(end),lastTime(0),isPause(false),next(0)
{
	FPS = 200.0;
	std::string fileName = "";
	for (int i = start; i <= end; i++) {
		fileName =  path + " " + std::to_string(i) + ".obj";
		printf(string(fileName+"\n").c_str());
		a_objFile.push_back(ObjFile(fileName));
	}
}


AnimationPlayer::~AnimationPlayer()
{
}


void AnimationPlayer::load()
{
	printf("cwy\n");
	for (int i = 0; i < a_objFile.size(); i++) {
		printf("test\n");
		a_objFile[i].loadFile();
	}
}


bool AnimationPlayer::flush(double nowTime)
{
	this->nowTime = nowTime;
	bool ret;
	double diff = nowTime - lastTime;
	if (diff > 1.0 / FPS) {
		lastTime = nowTime;
		a_objFile[next].draw(-1);
		if (!isPause&&++next >= a_objFile.size()-2)
		{
			next = 0;
		}
		ret = true;
	}
	else {
		ret = false;
	}
	return ret;
}

void AnimationPlayer::keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_SPACE: {
			isPause = !isPause;
			break;
		}
		case GLFW_KEY_DOWN:{
			if (FPS > 20)
			{
				FPS -= 10;
			}
			break;
		}
		case GLFW_KEY_UP: {
			if (FPS < 200)
			{
				FPS += 10;
			}
			break;
		}
		}
	}
}