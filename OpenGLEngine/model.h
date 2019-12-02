#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <mesh.h>
#include <levelGeneration.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Model
{
public:
	vector<Mesh> meshes;
	glm::vec3 position = { 0,0,0 };

	Model()
	{

	}

	Model(string const &path, string fileName, glm::vec3 newPos = { 0,0,0 })
	{
		position = newPos;

		LoadModel(path, fileName);
	}

	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].Draw(shader);
		}
	}

private:

	void LoadModel(string const& path, string fileName)
	{
		tinyobj::attrib_t attrib;
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		vector<vector<Texture>> textures;
		string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,(path + fileName + ".obj").c_str(), path.c_str())) {
			
			cout << "ERROR - " << err;
			//throw std::runtime_error(warn + err);
		}

		if (!warn.empty())
		{
			cerr << warn << endl;
		}

		if (!err.empty())
		{
			cerr << err << endl;
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

				if (!texture_filename._Equal(""))
				{
					texture_filename = path + texture_filename;
					std::cout << texture_filename << " - Texture" << endl;
					Texture tex = LoadTexture(texture_filename.c_str(), "texture_diffuse");
					meshTextures.push_back(tex);
				}
				else
				{
					Texture tex = {};
					tex.id = 0;
					tex.type = "texture_diffuse";
					meshTextures.push_back(tex);
				}
			}

			if (mp->specular_texname.length() > 0)
			{
				std::string specular_filename = mp->specular_texname;

				if (!specular_filename._Equal(""))
				{
					specular_filename = path + specular_filename;
					std::cout << specular_filename << " - Specular" << endl;
					Texture spec = LoadTexture(specular_filename.c_str(), "texture_specular");
					meshTextures.push_back(spec);
				}
				else
				{
					Texture spec = {};
					spec.id = 0;
					spec.type = "texture_specular";
					meshTextures.push_back(spec);
				}
			}

			if (mp->normal_texname.length() > 0)
			{
				std::string normal_filename = mp->normal_texname;

				if (!normal_filename._Equal(""))
				{
					normal_filename = path + normal_filename;
					std::cout << normal_filename << " - Normal" << endl;
					Texture norm = LoadTexture(normal_filename.c_str(), "texture_normal");
					meshTextures.push_back(norm);
				}
				else
				{
					Texture norm = {};
					norm.id = 0;
					norm.type = "texture_normal";
					meshTextures.push_back(norm);
				}
			}

			if (mp->bump_texname.length() > 0)
			{
				std::string bump_filename = mp->bump_texname;

				if (!bump_filename._Equal(""))
				{
					bump_filename = path + bump_filename;
					std::cout << bump_filename << " - Bump" << endl;
					Texture bump = LoadTexture(bump_filename.c_str(), "texture_bump");
					meshTextures.push_back(bump);
				}
				else
				{
					Texture bump = {};
					bump.id = 0;
					bump.type = "texture_bump";
					meshTextures.push_back(bump);
				}
			}

			textures.push_back(meshTextures);

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
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]};


				if (!attrib.texcoords.empty())
				{
					vert.TexCoords = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				vert.Normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};

				meshVerts.push_back(vert);

				meshIndices.push_back(meshIndices.size());
			}

			CalculateTangents(meshIndices, meshVerts);

			if (matId > -1)
				meshes.push_back(Mesh(meshVerts, meshIndices, textures[matId])); // TODO - add textures support
			else
				meshes.push_back(Mesh(meshVerts, meshIndices, {})); // TODO - add textures support
		}
	}
};

#endif