#include "Obj3D.h"
#include <string>
Obj3D::Obj3D(GLuint vb_id, int numTriangles, size_t material_i, const std::map<std::string, GLuint>& textures, const std::vector<tinyobj::material_t>& materials):
	vb_id(vb_id),numTriangles(numTriangles),material_id(material_i),textures(textures),materials(materials)
{
	
}


Obj3D::~Obj3D()
{
}

void Obj3D::draw(GLuint texture_id, int material_index)
{
	
	if (vb_id < 1) {
		return;
	}
	GLsizei stride = (3 + 3 + 3 + 2) * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, vb_id);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (texture_id == 0) {
		if ((material_id < materials.size())) {
			std::string diffuse_texname = materials[material_id].diffuse_texname;
			if (textures.find(diffuse_texname) != textures.end()) {
				glBindTexture(GL_TEXTURE_2D, textures.at(diffuse_texname));
			}
		}
		else glBindTexture(GL_TEXTURE_2D, 0);
	}
	else glBindTexture(GL_TEXTURE_2D, texture_id);
	/*
	}*/
	glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
	glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
	glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
	glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

	//定义材质
	if (material_index == 0) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, materials[material_id].specular);//设置镜面颜色
		glMaterialfv(GL_FRONT, GL_AMBIENT, materials[material_id].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[material_id].diffuse);//材料的环境和漫反射颜色
		glMateriali(GL_FRONT, GL_SHININESS, materials[material_id].shininess);//设置镜面指数*/
	}
	else {
		Material mer[5];
		{
			//emerald
			mer[0].ambient[0] = 0.0215; mer[0].ambient[1] = 0.1745; mer[0].ambient[2] = 0.0215;
			mer[0].diffuse[0] = 0.0757; mer[0].diffuse[1] = 0.6142; mer[0].diffuse[2] = 0.0757;
			mer[0].specular[0] = 0.633; mer[0].specular[1] = 0.7278; mer[0].specular[2] = 0.633;
			mer[0].shininess = 0.6;

			//jade
			mer[1].ambient[0] = 0.135; mer[1].ambient[1] = 0.2225; mer[1].ambient[2] = 0.1575;
			mer[1].diffuse[0] = 0.54; mer[1].diffuse[1] = 0.98; mer[1].diffuse[2] = 0.63;
			mer[1].specular[0] = 0.3162; mer[1].specular[1] = 0.3162; mer[1].specular[2] = 0.3162;
			mer[1].shininess = 0.1;

			//obsidian
			mer[2].ambient[0] = 0.0538; mer[2].ambient[1] = 0.05; mer[2].ambient[2] = 0.0663;
			mer[2].diffuse[0] = 0.1828; mer[2].diffuse[1] = 0.17; mer[2].diffuse[2] = 0.2253;
			mer[2].specular[0] = 0.3327; mer[2].specular[1] = 0.3286; mer[3].specular[2] = 0.3464;
			mer[2].shininess = 0.3;

			//black plastic
			mer[3].ambient[0] = 0; mer[3].ambient[1] = 0; mer[3].ambient[2] = 0;
			mer[3].diffuse[0] = 0.01; mer[3].diffuse[1] = 0.01; mer[3].diffuse[2] = 0.01;
			mer[3].specular[0] = 0.5; mer[3].specular[1] = 0.5; mer[3].specular[2] = 0.5;
			mer[4].shininess = 25;

			//red rubber
			mer[4].ambient[0] = 0.05; mer[4].ambient[1] = 0; mer[4].ambient[2] = 0;
			mer[4].diffuse[0] = 0.4; mer[4].diffuse[1] = 0.4; mer[4].diffuse[2] = 0.7;
			mer[4].specular[0] = 0.7; mer[4].specular[1] = 0.04; mer[4].specular[2] = 0.04;
			mer[4].shininess = 0.078;
		}

		glMaterialfv(GL_FRONT, GL_SPECULAR, mer[material_index - 1].specular);//设置镜面颜色
		glMaterialfv(GL_FRONT, GL_AMBIENT, mer[material_index - 1].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mer[material_index - 1].diffuse);//材料的环境和漫反射颜色
		glMateriali(GL_FRONT, GL_SHININESS, mer[material_index - 1].shininess);//设置镜面指数*/
	}

	
	
	glDrawArrays(GL_TRIANGLES, 0, 3 * numTriangles);
	//CheckErrors("drawarrays");
	glBindTexture(GL_TEXTURE_2D, 0);
}
