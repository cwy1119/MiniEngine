#include "ObjFile.h"
#include <cstdio>
#include <iostream>



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
ObjFile::ObjFile(string filename)
{
}


ObjFile::~ObjFile()
{
}

void ObjFile::setFilename(string filename) {
	this->filename = filename;
}

bool ObjFile::loadFile()
{
	if (!FileExists(filename)) {
		std::cout << "File does not exist!" << std::endl;
		return false;
	}

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	

	std::string base_dir = GetBaseDir(filename);
	if (base_dir.empty()) {
		base_dir = ".";
	}

	base_dir += "\\";
	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),base_dir.c_str());
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		std::cerr << "Failed to load " << filename << std::endl;
		return false;
	}

	materials.push_back(tinyobj::material_t());

	// 加载不同的纹理
	{
		for (size_t m = 0; m < materials.size(); m++) {
			tinyobj::material_t* mp = &materials[m];

			if (mp->diffuse_texname.length() > 0) {
				// Only load the texture if it is not already loaded
				if (textures.find(mp->diffuse_texname) == textures.end()) {
					printf("cwy: texturename %s\n", mp->diffuse_texname.c_str());
					GLuint texture_id;
					int w, h;
					int comp;

					std::string texture_filename = mp->diffuse_texname;
					if (!FileExists(texture_filename)) {
						// Append base dir.
						texture_filename = base_dir + mp->diffuse_texname;
						if (!FileExists(texture_filename)) {
							std::cerr << "Unable to find file: " << mp->diffuse_texname
								<< std::endl;
							exit(1);
						}
					}

					unsigned char* image =
						stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
					if (!image) {
						std::cerr << "Unable to load texture: " << texture_filename
							<< std::endl;
						exit(1);
					}
					std::cout << "Loaded texture: " << texture_filename << ", w = " << w
						<< ", h = " << h << ", comp = " << comp << std::endl;

					glGenTextures(1, &texture_id);
					glBindTexture(GL_TEXTURE_2D, texture_id);
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
						assert(0);  // TODO
					}
					glBindTexture(GL_TEXTURE_2D, 0);
					stbi_image_free(image);
					textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
				}
			}
		}
	}

	{
		for (size_t s = 0; s < shapes.size(); s++) {
			//DrawObject o;
			
			std::vector<float> buffer;  // pos(3float), normal(3float), color(3float)

			// Check for smoothing group and compute smoothing normals
			std::map<int, vec3> smoothVertexNormals;
			if (hasSmoothingGroup(shapes[s]) > 0) {
				std::cout << "Compute smoothingNormal for shape [" << s << "]" << std::endl;
				computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);
			}

			for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
				tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
				tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
				tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

				int current_material_id = shapes[s].mesh.material_ids[f];

				if ((current_material_id < 0) ||
					(current_material_id >= static_cast<int>(materials.size()))) {
					// Invaid material ID. Use default material.
					current_material_id =
						materials.size() -
						1;  // Default material is added to the last item in `materials`.
				}
				// if (current_material_id >= materials.size()) {
				//    std::cerr << "Invalid material index: " << current_material_id <<
				//    std::endl;
				//}
				//
				float diffuse[3];
				for (size_t i = 0; i < 3; i++) {
					diffuse[i] = materials[current_material_id].diffuse[i];
				}
				float tc[3][2];
				if (attrib.texcoords.size() > 0) {
					if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
						(idx2.texcoord_index < 0)) {
						// face does not contain valid uv index.
						tc[0][0] = 0.0f;
						tc[0][1] = 0.0f;
						tc[1][0] = 0.0f;
						tc[1][1] = 0.0f;
						tc[2][0] = 0.0f;
						tc[2][1] = 0.0f;
					}
					else {
						assert(attrib.texcoords.size() >
							size_t(2 * idx0.texcoord_index + 1));
						assert(attrib.texcoords.size() >
							size_t(2 * idx1.texcoord_index + 1));
						assert(attrib.texcoords.size() >
							size_t(2 * idx2.texcoord_index + 1));

						// Flip Y coord.
						tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
						tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
						tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
						tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
						tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
						tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
					}
				}
				else {
					tc[0][0] = 0.0f;
					tc[0][1] = 0.0f;
					tc[1][0] = 0.0f;
					tc[1][1] = 0.0f;
					tc[2][0] = 0.0f;
					tc[2][1] = 0.0f;
				}

				float v[3][3];
				for (int k = 0; k < 3; k++) {
					int f0 = idx0.vertex_index;
					int f1 = idx1.vertex_index;
					int f2 = idx2.vertex_index;
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);

					v[0][k] = attrib.vertices[3 * f0 + k];
					v[1][k] = attrib.vertices[3 * f1 + k];
					v[2][k] = attrib.vertices[3 * f2 + k];
				/*	bmin[k] = std::min(v[0][k], bmin[k]);
					bmin[k] = std::min(v[1][k], bmin[k]);
					bmin[k] = std::min(v[2][k], bmin[k]);
					bmax[k] = std::max(v[0][k], bmax[k]);
					bmax[k] = std::max(v[1][k], bmax[k]);
					bmax[k] = std::max(v[2][k], bmax[k]); */
				}

				float n[3][3];
				{
					bool invalid_normal_index = false;
					if (attrib.normals.size() > 0) {
						int nf0 = idx0.normal_index;
						int nf1 = idx1.normal_index;
						int nf2 = idx2.normal_index;

						if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) {
							// normal index is missing from this face.
							invalid_normal_index = true;
						}
						else {
							for (int k = 0; k < 3; k++) {
								assert(size_t(3 * nf0 + k) < attrib.normals.size());
								assert(size_t(3 * nf1 + k) < attrib.normals.size());
								assert(size_t(3 * nf2 + k) < attrib.normals.size());
								n[0][k] = attrib.normals[3 * nf0 + k];
								n[1][k] = attrib.normals[3 * nf1 + k];
								n[2][k] = attrib.normals[3 * nf2 + k];
							}
						}
					}
					else {
						invalid_normal_index = true;
					}

					if (invalid_normal_index && !smoothVertexNormals.empty()) {
						// Use smoothing normals
						int f0 = idx0.vertex_index;
						int f1 = idx1.vertex_index;
						int f2 = idx2.vertex_index;

						if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
							n[0][0] = smoothVertexNormals[f0].v[0];
							n[0][1] = smoothVertexNormals[f0].v[1];
							n[0][2] = smoothVertexNormals[f0].v[2];

							n[1][0] = smoothVertexNormals[f1].v[0];
							n[1][1] = smoothVertexNormals[f1].v[1];
							n[1][2] = smoothVertexNormals[f1].v[2];

							n[2][0] = smoothVertexNormals[f2].v[0];
							n[2][1] = smoothVertexNormals[f2].v[1];
							n[2][2] = smoothVertexNormals[f2].v[2];

							invalid_normal_index = false;
						}
					}

					if (invalid_normal_index) {
						// compute geometric normal
						CalcNormal(n[0], v[0], v[1], v[2]);
						n[1][0] = n[0][0];
						n[1][1] = n[0][1];
						n[1][2] = n[0][2];
						n[2][0] = n[0][0];
						n[2][1] = n[0][1];
						n[2][2] = n[0][2];
					}
				}

				for (int k = 0; k < 3; k++) {
					buffer.push_back(v[k][0]);
					buffer.push_back(v[k][1]);
					buffer.push_back(v[k][2]);
					buffer.push_back(n[k][0]);
					buffer.push_back(n[k][1]);
					buffer.push_back(n[k][2]);
					// Combine normal and diffuse to get color.
					float normal_factor = 0.2;
					float diffuse_factor = 1 - normal_factor;
					float c[3] = { n[k][0] * normal_factor + diffuse[0] * diffuse_factor,
								  n[k][1] * normal_factor + diffuse[1] * diffuse_factor,
								  n[k][2] * normal_factor + diffuse[2] * diffuse_factor };
					float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
					if (len2 > 0.0f) {
						float len = sqrtf(len2);

						c[0] /= len;
						c[1] /= len;
						c[2] /= len;
					}
					buffer.push_back(c[0] * 0.5 + 0.5);
					buffer.push_back(c[1] * 0.5 + 0.5);
					buffer.push_back(c[2] * 0.5 + 0.5);

					buffer.push_back(tc[k][0]);
					buffer.push_back(tc[k][1]);
				}
			}

			GLuint vb_id = 0;
			int numTriangles = 0;
			size_t material_id;

			// OpenGL viewer does not support texturing with per-face material.
			if (shapes[s].mesh.material_ids.size() > 0 &&
				shapes[s].mesh.material_ids.size() > s) {
				material_id = shapes[s].mesh.material_ids[0];  // use the material ID
																 // of the first face.
			}
			else {
				material_id = materials.size() - 1;  // = ID for default material.
			}
			printf("shape[%d] material_id %d\n", int(s), int(material_id));

			if (buffer.size() > 0) {
				glGenBuffers(1, &vb_id);
				glBindBuffer(GL_ARRAY_BUFFER, vb_id);
				glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
					&buffer.at(0), GL_STATIC_DRAW);
				numTriangles = buffer.size() / (3 + 3 + 3 + 2) /
					3;  // 3:vtx, 3:normal, 3:col, 2:texcoord

				printf("shape[%d] # of triangles = %d\n", static_cast<int>(s),
					numTriangles);
			}

			objects.push_back(Obj3D(vb_id,numTriangles,material_id));
		}
	}
}

bool ObjFile::FileExists(const std::string test_filename) {
	bool ret;
	FILE* fp = fopen(test_filename.c_str(),"rb");
	if (fp) {
		ret = true;
		fclose(fp);
	}
	else {
		ret = false;
	}
	return ret;
}

string ObjFile::GetBaseDir(const std::string& filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

bool ObjFile::hasSmoothingGroup(const tinyobj::shape_t& shape)
{
	for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++) {
		if (shape.mesh.smoothing_group_ids[i] > 0) {
			return true;
		}
	}
	return false;
}

void ObjFile::computeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,std::map<int, vec3>& smoothVertexNormals) {
	smoothVertexNormals.clear();
	std::map<int, vec3>::iterator iter;

	for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
		// Get the three indexes of the face (all faces are triangular)
		tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
		tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
		tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

		// Get the three vertex indexes and coordinates
		int vi[3];      // indexes
		float v[3][3];  // coordinates

		for (int k = 0; k < 3; k++) {
			vi[0] = idx0.vertex_index;
			vi[1] = idx1.vertex_index;
			vi[2] = idx2.vertex_index;
			assert(vi[0] >= 0);
			assert(vi[1] >= 0);
			assert(vi[2] >= 0);

			v[0][k] = attrib.vertices[3 * vi[0] + k];
			v[1][k] = attrib.vertices[3 * vi[1] + k];
			v[2][k] = attrib.vertices[3 * vi[2] + k];
		}

		// Compute the normal of the face
		float normal[3];
		CalcNormal(normal, v[0], v[1], v[2]);

		// Add the normal to the three vertexes
		for (size_t i = 0; i < 3; ++i) {
			iter = smoothVertexNormals.find(vi[i]);
			if (iter != smoothVertexNormals.end()) {
				// add
				iter->second.v[0] += normal[0];
				iter->second.v[1] += normal[1];
				iter->second.v[2] += normal[2];
			}
			else {
				smoothVertexNormals[vi[i]].v[0] = normal[0];
				smoothVertexNormals[vi[i]].v[1] = normal[1];
				smoothVertexNormals[vi[i]].v[2] = normal[2];
			}
		}

	}  // f

	// Normalize the normals, that is, make them unit vectors
	for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end();
		iter++) {
		normalizeVector(iter->second);
	}

}  // computeSmoothingNormals


void ObjFile::CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v10[1] * v20[2] - v10[2] * v20[1];
	N[1] = v10[2] * v20[0] - v10[0] * v20[2];
	N[2] = v10[0] * v20[1] - v10[1] * v20[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		N[0] /= len;
		N[1] /= len;
		N[2] /= len;
	}
}

void ObjFile::normalizeVector(vec3 &v) {
	float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		v.v[0] /= len;
		v.v[1] /= len;
		v.v[2] /= len;
	}
}

void ObjFile::draw()
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	int n = objects.size();
	for (int i = 0; i < n; i++) {
		objects[i].draw();
	}
}