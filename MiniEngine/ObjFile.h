#pragma once
#include<string>
#include<vector>
#include<map>
#include "Obj3D.h"


//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"




using std::string;
/*
	Obj�ļ���
	����:
		1.��ȡ��д��Obj�ļ�
		2.��Obj�ļ�����Ϊ3D����(һ������������ΪMaya�е�һ������
		3.�ϲ�����obj�ļ�
	
*/

namespace  // Local utility functions
{
	struct vec3 {
		float v[3];
		vec3() {
			v[0] = 0.0f;
			v[1] = 0.0f;
			v[2] = 0.0f;
		}
	};
}

class ObjFile
{
public:
	std::vector<Obj3D> objects;
	string filename;
	std::map<std::string, GLuint> textures;
	std::vector<tinyobj::material_t> materials;

	ObjFile(string filename);
	~ObjFile();
	void setFilename(string filename);
	bool loadFile();
	void saveFile();
	void draw();
private:
	static bool FileExists(const std::string test_filename);
	static string GetBaseDir(const std::string& filepath);
	bool hasSmoothingGroup(const tinyobj::shape_t& shape);
	void computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, vec3>& smoothVertexNormals);
	static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
	void normalizeVector(vec3 &v);
};

