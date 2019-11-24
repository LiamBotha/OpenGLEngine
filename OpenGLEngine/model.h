#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Model
{
public:
	vector<Mesh> meshes;
	glm::vec3 position = { 0,0,0 };

	Model()
	{

	}

	Model(string const &path, glm::vec3 newPos = { 0,0,0 })
	{
		position = newPos;

		LoadModel(path);
	}

	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].Draw(shader);
		}
	}

private:

	void LoadModel(string const& path)
	{
		tinyobj::attrib_t attrib;
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		vector<vector<Texture>> textures;
		string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), "Models/Materials/")) {
			throw std::runtime_error(warn + err);
		}

		for (int k = 0; k < materials.size(); ++k)
		{
			vector<Texture> meshTextures = {};

			tinyobj::material_t* mp = &materials[k];

			if (mp->diffuse_texname.length() > 0)
			{
				GLuint texture_id;
				int w, h;
				int comp;

				std::string texture_filename = mp->diffuse_texname;
				std::string specular_filename = mp->specular_texname;
				std::string normal_filename = mp->normal_texname;

				texture_filename = "Models/Materials/" + texture_filename;
				specular_filename = "Models/Materials/" + specular_filename;
				normal_filename = "Models/Materials/" + normal_filename;

				cout << texture_filename << " - Texture" << endl;
				cout << specular_filename << " - Specular" << endl;
				cout << normal_filename << " - Normal" << endl;

				//Load texture from path
				Texture tex = LoadTexture(texture_filename.c_str(), "texture_diffuse");
				Texture spec = LoadTexture(specular_filename.c_str(), "texture_specular");
				Texture norm = LoadTexture(normal_filename.c_str(), "texture_normal");

				meshTextures.push_back(tex);
				meshTextures.push_back(spec);
				//meshTextures.push_back(norm);

				textures.push_back(meshTextures);
			}

		}

		for (int i = 0; i < shapes.size(); ++i)
		{
			vector<Vertex> meshVerts;
			vector<unsigned int> meshIndices;

			int matId = shapes[i].mesh.material_ids[i];

			int indSize = shapes[i].mesh.indices.size();
			for (int j = 0; j < indSize; ++j)
			{
				tinyobj::index_t index = shapes[i].mesh.indices[j];

				Vertex vert = {};

				vert.Position = {
					attrib.vertices[3 * (int32_t)index.vertex_index + 0],
					attrib.vertices[3 * (int32_t)index.vertex_index + 1],
					attrib.vertices[3 * (int32_t)index.vertex_index + 2]};

				vert.TexCoords = {
					attrib.texcoords[2 * (int32_t)index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * (int32_t)index.texcoord_index + 1]
				};

				vert.Normal = {
					attrib.normals[3 * (int32_t)index.normal_index + 0],
					attrib.normals[3 * (int32_t)index.normal_index + 1],
					attrib.normals[3 * (int32_t)index.normal_index + 2]
				};

				meshVerts.push_back(vert);

				meshIndices.push_back(meshIndices.size());
			}

			meshes.push_back(Mesh(meshVerts, meshIndices, textures[matId])); // TODO - add textures support
		}
	}
};

#endif