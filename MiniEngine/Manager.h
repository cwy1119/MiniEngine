#pragma once
#include <vector>
#include<string>
#include "ObjFile.h"
#include<GLFW/glfw3.h>
#define MTL_N 7//�������������
using std::string;
class Manager
{
private:
	std::vector<ObjFile> objFiles;
	//std::vector<Obj3D> obj3Ds;
	int mtlfile_id; //mtl�ļ���� 0Ϊ�Դ���ͼ
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

