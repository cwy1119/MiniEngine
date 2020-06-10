#pragma once
#include <GL/glew.h>
#include<map>
#include<vector>
#pragma warning(disable : 4996)
#include "tiny_obj_loader.h"
class Obj3D
{
public:
	Obj3D(GLuint vb_id,int numTriangles,size_t material_i, const std::map<std::string, GLuint>& textures,const std::vector<tinyobj::material_t>& materials);
	~Obj3D();
	void draw(GLuint texture_id, int meterial_i);
private:
	GLuint vb_id;
	int numTriangles;
	size_t material_id;
	const std::map<std::string, GLuint>& textures;
	const std::vector<tinyobj::material_t>& materials;
};

struct Material {
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];
	GLfloat shininess;
};


