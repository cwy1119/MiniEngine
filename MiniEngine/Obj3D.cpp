#include "Obj3D.h"
#include <string>
Obj3D::Obj3D(GLuint vb_id, int numTriangles, size_t material_i, const std::map<std::string, GLuint>& textures, const std::vector<tinyobj::material_t>& materials):
	vb_id(vb_id),numTriangles(numTriangles),material_id(material_i),textures(textures),materials(materials)
{
}


Obj3D::~Obj3D()
{
}

void Obj3D::draw(GLuint texture_id)
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

	if (texture_id == -1) {
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

	glDrawArrays(GL_TRIANGLES, 0, 3 * numTriangles);
	//CheckErrors("drawarrays");
	glBindTexture(GL_TEXTURE_2D, 0);
}
