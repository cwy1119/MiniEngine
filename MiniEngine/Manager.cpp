#include "Manager.h"
#include<iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Manager::Manager()
	:texture_index(0),material_index(0)
{
}


Manager::~Manager()
{
}

void Manager::appendObjFile(string name)
{
	initDefaultTexture();
	this->objFiles.push_back(ObjFile(name));
	objFiles.at(objFiles.size() - 1).loadFile();
}

void Manager::draw()
{
	glTranslatef(center[0], center[1], center[2]);
	glScalef(fscale[0], fscale[1], fscale[2]);
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis

	for (int i = 0; i < objFiles.size(); i++)
	{
		GLuint id;
		if (texture_index == 0) id = 0;
		else id = texture_ids[texture_index-1];
		objFiles[i].draw(id,material_index);
	}
}

void Manager::initDefaultTexture()
{
	for (int i = 0; i < MTL_N; i++)
	{
		int w, h;
		int comp;
		string texture_filename = string("./texture/") + std::to_string(i) + ".jpg";
		unsigned char* image = stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
		if (!image) {
			std::cerr << "Unable to load texture: " << texture_filename
				<< std::endl;
			exit(1);
		}
		std::cout << "Loaded texture: " << texture_filename << ", w = " << w
			<< ", h = " << h << ", comp = " << comp << std::endl;

		glGenTextures(1, (GLuint*)texture_ids+i);
		glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (comp == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
				GL_UNSIGNED_BYTE, image);
		}
		else if (comp == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, image);
		}
		else {
			//assert(0);  // TODO
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}
}

void Manager::loop()
{
	if (bRotate == true) fRotate += 5.0f;
}
void Manager::keyboardFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_M) {
			texture_index++;
			if (texture_index >= MTL_N) {
				//目前内置7张纹理贴图 可增加
				texture_index = 0;
			}
			printf("mtlfile: %d\n", texture_index);
		}
		if (key == GLFW_KEY_RIGHT) {
			material_index++;
			
			if (material_index >= MER_N) {
				material_index = 0;
			}
			printf("material_id: %d\n", material_index);
		}
	}

	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_SPACE: {
			bRotate = !bRotate;
			break;
		}
		case GLFW_KEY_W: {
			center[1] += 0.5;
			//camera.eye[1] += 0.5;
			break;
		}
		case GLFW_KEY_S: {
			center[1] -= 0.5;
			//camera.eye[1] -= 0.5;
			break;
		}
		case GLFW_KEY_A: {
			center[0] -= 0.5;
			//camera.eye[0] -= 0.5;
			break;
		}
		case GLFW_KEY_D: {
			center[0] += 0.5;
			//camera.eye[0] += 0.5;
			break;
		}
		case GLFW_KEY_Z: {
			center[2] += 0.5;
			break;
		}
		case GLFW_KEY_C: {
			center[2] -= 0.5;
			break;
		}
		case GLFW_KEY_1: {
			fscale[0] += 0.5;
			break;
		}
		case GLFW_KEY_2: {
			fscale[0] -= 0.5;
			break;
		}
		case GLFW_KEY_3: {
			fscale[1] += 0.5;
			break;
		}
		case GLFW_KEY_4: {
			fscale[1] -= 0.5;
			break;
		}
		case GLFW_KEY_5: {
			fscale[2] += 0.5;
			break;
		}
		case GLFW_KEY_6: {
			fscale[2] -= 0.5;
			break;
		}
		}
	}
}
