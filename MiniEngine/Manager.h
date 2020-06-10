#pragma once
#include <vector>
#include<string>
#include "ObjFile.h"
#include<GLFW/glfw3.h>
#define MTL_N 7//内置纹理的数量
#define MER_N 5
using std::string;
class Manager
{
private:
	std::vector<ObjFile> objFiles;
	//std::vector<Obj3D> obj3Ds;
	GLuint texture_index; //纹理编号 0为自带贴图
	GLuint material_index; //材质编号 0为自带贴图
	GLuint texture_ids[MTL_N];

	float fRotate = 0.0f;
	bool bRotate = false;
	float center[3] = { 0,0,0 };
	float fscale[3] = { 1,1,1 };

public:
	Manager();
	~Manager();
	void appendObjFile(string filename);
	void draw();
	void initDefaultTexture();
	void keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods);
	void loop();
};

