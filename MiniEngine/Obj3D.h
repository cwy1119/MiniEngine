#pragma once

#include <GL/glew.h>
#pragma warning(disable : 4996)
class Obj3D
{
public:
	Obj3D(GLuint vb_id,int numTriangles,size_t material_i);
	~Obj3D();
	void draw();
private:
	GLuint vb_id;
	int numTriangles;
	size_t material_id;
};

