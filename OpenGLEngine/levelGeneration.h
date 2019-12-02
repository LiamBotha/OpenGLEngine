#ifndef LEVELGEN_H
#define LEVELGEN_H

#include <mesh.h>
#include <shader.h>
#include <glm/glm.hpp>

using namespace glm;

void BuildNormalWalls(float x, float y, float length, float width, float height, int doors[5], vector<Vertex>& vertices);
void BuildNormalWallsDoors(float x, float y, float length, float width, float height, int doors[5], vector<Vertex>& vertices);

Room GenerateRoom(float x, float y, float length, float width, float height, int doors[], string floorPath, string wallPath, string ceilingPath);

void AssignNewIndices(int doors[5], int doorCount, vector<unsigned int>& indices, int vertSize);
void DrawQuadIndices(int val, int xWidth, vector<unsigned int>& indices);
void DrawDoorIndices(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int>& indices);
void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex>& vertices);

void CalculateNormals(vector<unsigned int>& indices, vector<Vertex>& vertices, bool flipNormal = false);
void CalculateTangents(vector<unsigned int>& indices, vector<Vertex>& vertices);
vec3 CrossProduct(vec3 a, vec3 b);

Mesh GenerateCube(float x, float y, float z)
{
	vector<Vertex> cubeVerts = {};
	vector<unsigned int> cubeIndices = {};

	vector<Vertex> cubeVertices = {
		//position					 color					   uv				   normal
		{vec3(x - 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Bottom Left (0)
		{vec3(x + 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Bottom Right (1)
		{vec3(x + 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Top Right (2)
		{vec3(x - 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Top Left (3)

		{vec3(x-0.5f, y-0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Bottom Left (4)
		{vec3(x+0.5f, y-0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Bottom Right (5)
		{vec3(x+0.5f, y+0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Top Right (6)
		{vec3(x-0.5f, y+0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Top Left (7)

		{vec3(x-0.5f, y-0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Bottom Left (8)
		{vec3(x-0.5f, y-0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Bottom Right (9)
		{vec3(x-0.5f, y+0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Top Right (10)
		{vec3(x-0.5f, y+0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Top Left (11)

		{vec3(x+0.5f, y-0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Bottom Left (12)
		{vec3(x+0.5f, y-0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Bottom Right (13)
		{vec3(x+0.5f, y+0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Top Right (14)
		{vec3(x+0.5f, y+0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Top Left (15)

		{vec3(x-0.5f, y+0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Bottom Left (16)
		{vec3(x+0.5f, y+0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Bottom Right (17)
		{vec3(x+0.5f, y+0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Top Right (18)
		{vec3(x-0.5f, y+0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Top Left (19)

		{vec3(x-0.5f, y-0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, -1.0f, 0.0f)}, // Bottom Bottom Left (20)
		{vec3(x+0.5f, y-0.5f, z+0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, -1.0f, 0.0f)}, // Bottom Bottom Right (21)
		{vec3(x+0.5f, y-0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, -1.0f, 0.0f)}, // Bottom Top Right (22)
		{vec3(x-0.5f, y-0.5f, z-0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, -1.0f, 0.0f)}  // Bottom Top Left (23)*/
	};

	cubeIndices.push_back(0);
	cubeIndices.push_back(1);
	cubeIndices.push_back(2);
	cubeIndices.push_back(2);
	cubeIndices.push_back(3);
	cubeIndices.push_back(0);

	cubeIndices.push_back(4);
	cubeIndices.push_back(7);
	cubeIndices.push_back(6);
	cubeIndices.push_back(6);
	cubeIndices.push_back(5);
	cubeIndices.push_back(4);

	cubeIndices.push_back(8);
	cubeIndices.push_back(9);
	cubeIndices.push_back(10);
	cubeIndices.push_back(10);
	cubeIndices.push_back(11);
	cubeIndices.push_back(8);

	cubeIndices.push_back(12);
	cubeIndices.push_back(13);
	cubeIndices.push_back(14);
	cubeIndices.push_back(14);
	cubeIndices.push_back(15);
	cubeIndices.push_back(12);

	cubeIndices.push_back(16);
	cubeIndices.push_back(17);
	cubeIndices.push_back(18);
	cubeIndices.push_back(18);
	cubeIndices.push_back(19);
	cubeIndices.push_back(16);

	cubeIndices.push_back(20);
	cubeIndices.push_back(21);
	cubeIndices.push_back(22);
	cubeIndices.push_back(22);
	cubeIndices.push_back(23);
	cubeIndices.push_back(20);

	return Mesh(cubeVertices, cubeIndices, {});
}

Mesh GenerateCube(vec3 position)
{
	float x = position.x;
	float y = position.y;
	float z = position.z;

	vector<Vertex> cubeVerts = {};
	vector<unsigned int> cubeIndices = {};

	vector<Vertex> cubeVertices = {
		//position					 color					   uv				   normal
		{vec3(x - 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Bottom Left (0)
		{vec3(x + 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Bottom Right (1)
		{vec3(x + 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Top Right (2)
		{vec3(x - 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, 0.0f, -1.0f)}, // Front Top Left (3)

		{vec3(x - 0.5f, y - 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Bottom Left (4)
		{vec3(x + 0.5f, y - 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Bottom Right (5)
		{vec3(x + 0.5f, y + 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Top Right (6)
		{vec3(x - 0.5f, y + 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, 0.0f, 1.0f)},  // Back Top Left (7)

		{vec3(x - 0.5f, y - 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Bottom Left (8)
		{vec3(x - 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Bottom Right (9)
		{vec3(x - 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Top Right (10)
		{vec3(x - 0.5f, y + 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(-1.0f, 0.0f, 0.0f)}, // Left Top Left (11)

		{vec3(x + 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Bottom Left (12)
		{vec3(x + 0.5f, y - 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Bottom Right (13)
		{vec3(x + 0.5f, y + 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Top Right (14)
		{vec3(x + 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(1.0f, 0.0f, 0.0f)},  // Right Top Left (15)

		{vec3(x - 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Bottom Left (16)
		{vec3(x + 0.5f, y + 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Bottom Right (17)
		{vec3(x + 0.5f, y + 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Top Right (18)
		{vec3(x - 0.5f, y + 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, 1.0f, 0.0f)},  // Top Top Left (19)

		{vec3(x - 0.5f, y - 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 1.0f),   vec3(0.0f, -1.0f, 0.0f)}, // Bottom Bottom Left (20)
		{vec3(x + 0.5f, y - 0.5f, z + 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 1.0f),   vec3(0.0f, -1.0f, 0.0f)}, // Bottom Bottom Right (21)
		{vec3(x + 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(0.0f, 0.0f),   vec3(0.0f, -1.0f, 0.0f)}, // Bottom Top Right (22)
		{vec3(x - 0.5f, y - 0.5f, z - 0.5f),  vec3(1.0f, 1.0f, 1.0f),   vec2(1.0f, 0.0f),   vec3(0.0f, -1.0f, 0.0f)}  // Bottom Top Left (23)*/
	};

	cubeIndices.push_back(0);
	cubeIndices.push_back(1);
	cubeIndices.push_back(2);
	cubeIndices.push_back(2);
	cubeIndices.push_back(3);
	cubeIndices.push_back(0);

	cubeIndices.push_back(4);
	cubeIndices.push_back(7);
	cubeIndices.push_back(6);
	cubeIndices.push_back(6);
	cubeIndices.push_back(5);
	cubeIndices.push_back(4);

	cubeIndices.push_back(8);
	cubeIndices.push_back(9);
	cubeIndices.push_back(10);
	cubeIndices.push_back(10);
	cubeIndices.push_back(11);
	cubeIndices.push_back(8);

	cubeIndices.push_back(12);
	cubeIndices.push_back(13);
	cubeIndices.push_back(14);
	cubeIndices.push_back(14);
	cubeIndices.push_back(15);
	cubeIndices.push_back(12);

	cubeIndices.push_back(16);
	cubeIndices.push_back(17);
	cubeIndices.push_back(18);
	cubeIndices.push_back(18);
	cubeIndices.push_back(19);
	cubeIndices.push_back(16);

	cubeIndices.push_back(20);
	cubeIndices.push_back(21);
	cubeIndices.push_back(22);
	cubeIndices.push_back(22);
	cubeIndices.push_back(23);
	cubeIndices.push_back(20);

	return Mesh(cubeVertices, cubeIndices, {});
}

//Room GenerateRoom(float x, float y, float length, float width, float height, bool doors[], string floorPath, string wallPath, string ceilingPath)
//{
//	int doorCount = 0;
//	for (int i = 0; i < 4; ++i)
//	{
//		if (doors[i] == true)
//		{
//			++doorCount;
//		}
//	}
//
//	vector<Vertex> floorVertices = {};
//	vector<unsigned int> floorIndices = {};
//	vector<Texture> floorTextures = {};
//
//	vector<Vertex> wallVertices = {};
//	vector<unsigned int> wallIndices = {};
//	vector<Texture> wallTextures = {};
//
//	vector<Vertex> ceilingVertices = {};
//	vector<unsigned int> ceilingIndices = {};
//	vector<Texture> ceilingTextures = {};
//
//	Texture floorTex = LoadTexture(floorPath.c_str(), "texture_diffuse");
//	Texture floorSpec = LoadTexture(floorPath.c_str(), "texture_specular");
//	Texture floorNorm = LoadTexture("Textures/brickwall_normal.jpg", "texture_normal");
//	floorTextures.push_back(floorTex);
//	floorTextures.push_back(floorSpec);
//	floorTextures.push_back(floorNorm);
//
//	Texture wallTex = LoadTexture(wallPath.c_str(), "texture_diffuse");
//	Texture wallSpec = LoadTexture(wallPath.c_str(), "texture_specular");
//	wallTextures.push_back(wallTex);
//	wallTextures.push_back(wallSpec);
//
//	Texture ceilingTex = LoadTexture(ceilingPath.c_str(), "texture_diffuse");
//	Texture ceilingSpec = LoadTexture(ceilingPath.c_str(), "texture_specular");
//	ceilingTextures.push_back(ceilingTex);
//	ceilingTextures.push_back(ceilingSpec);
//
//	int texCoordWidth = (1 * width / 2);
//	int texCoordLength = (1 * length / 2);
//
//	GenerateVert(x - (width / 2), y + (length / 2), 0, 0.0, texCoordLength, floorVertices); // 0
//	GenerateVert(x + (width / 2), y + (length / 2), 0, texCoordWidth, texCoordLength, floorVertices); // 1
//	GenerateVert(x + (width / 2), y - (length / 2), 0, texCoordWidth, 0.0, floorVertices); // 2
//	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, floorVertices); // 3
//
//	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, texCoordLength, ceilingVertices); // 0
//	GenerateVert(x + (width / 2), y + (length / 2), height, texCoordWidth, texCoordLength, ceilingVertices); // 1
//	GenerateVert(x + (width / 2), y - (length / 2), height, texCoordWidth, 0.0, ceilingVertices); // 2
//	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, 0.0, ceilingVertices); // 3
//
//	floorIndices.push_back(0);
//	floorIndices.push_back(3);
//	floorIndices.push_back(2);
//	floorIndices.push_back(2);
//	floorIndices.push_back(1);
//	floorIndices.push_back(0);
//
//	ceilingIndices.push_back(0);
//	ceilingIndices.push_back(1);
//	ceilingIndices.push_back(2);
//	ceilingIndices.push_back(2);
//	ceilingIndices.push_back(3);
//	ceilingIndices.push_back(0);
//
//	//BuildWalls(x, y, length, width, height, doors, doorCount, wallVertices);
//	//AssignIndices(doors, doorCount, wallIndices, wallVertices.size());
//	BuildNormalWalls(x, y, length, width, height, doors, wallVertices);
//	AssignNewIndices(doors, doorCount, wallIndices, wallVertices.size());
//
//	CalculateNormals(wallIndices, wallVertices, true);
//	CalculateNormals(floorIndices, floorVertices);
//	CalculateNormals(ceilingIndices, ceilingVertices);
//
//	CalculateTangents(wallIndices, wallVertices);
//	CalculateTangents(floorIndices, floorVertices);
//	CalculateTangents(ceilingIndices, ceilingVertices);
//
//	vec3 pos = { x,0,y};
//
//	return Room(pos, Mesh(floorVertices, floorIndices, floorTextures), Mesh(wallVertices, wallIndices, wallTextures), Mesh(ceilingVertices, ceilingIndices, ceilingTextures));
//}

Room GenerateRoom(float x, float y, float length, float width, float height, int doors[], string floorPath, string wallPath, string ceilingPath)
{
	int doorCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (doors[i] == 2)
		{
			++doorCount;
		}
	}

	vector<Vertex> floorVertices = {};
	vector<unsigned int> floorIndices = {};
	vector<Texture> floorTextures = {};

	vector<Vertex> wallVertices = {};
	vector<unsigned int> wallIndices = {};
	vector<Texture> wallTextures = {};

	vector<Vertex> ceilingVertices = {};
	vector<unsigned int> ceilingIndices = {};
	vector<Texture> ceilingTextures = {};

	Texture floorTex = LoadTexture(floorPath.c_str(), "texture_diffuse");
	Texture floorSpec = LoadTexture(floorPath.c_str(), "texture_specular");
	Texture floorNorm = LoadTexture("Textures/brickwall_normal.jpg", "texture_normal");
	floorTextures.push_back(floorTex);
	floorTextures.push_back(floorSpec);
	floorTextures.push_back(floorNorm);

	Texture wallTex = LoadTexture(wallPath.c_str(), "texture_diffuse");
	Texture wallSpec = LoadTexture(wallPath.c_str(), "texture_specular");
	wallTextures.push_back(wallTex);
	wallTextures.push_back(wallSpec);

	int texCoordWidth = (1 * width / 2);
	int texCoordLength = (1 * length / 2);

	if (doors[4] != 0)
	{
		Texture ceilingTex = LoadTexture(ceilingPath.c_str(), "texture_diffuse");
		Texture ceilingSpec = LoadTexture(ceilingPath.c_str(), "texture_specular");
		ceilingTextures.push_back(ceilingTex);
		ceilingTextures.push_back(ceilingSpec);

		GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, texCoordLength, ceilingVertices); // 0
		GenerateVert(x + (width / 2), y + (length / 2), height, texCoordWidth, texCoordLength, ceilingVertices); // 1
		GenerateVert(x + (width / 2), y - (length / 2), height, texCoordWidth, 0.0, ceilingVertices); // 2
		GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, 0.0, ceilingVertices); // 3

	}

	GenerateVert(x - (width / 2), y + (length / 2), 0, 0.0, texCoordLength, floorVertices); // 0
	GenerateVert(x + (width / 2), y + (length / 2), 0, texCoordWidth, texCoordLength, floorVertices); // 1
	GenerateVert(x + (width / 2), y - (length / 2), 0, texCoordWidth, 0.0, floorVertices); // 2
	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, floorVertices); // 3

	floorIndices.push_back(0);
	floorIndices.push_back(3);
	floorIndices.push_back(2);
	floorIndices.push_back(2);
	floorIndices.push_back(1);
	floorIndices.push_back(0);

	if (doors[4] != 0)
	{
		ceilingIndices.push_back(0);
		ceilingIndices.push_back(1);
		ceilingIndices.push_back(2);
		ceilingIndices.push_back(2);
		ceilingIndices.push_back(3);
		ceilingIndices.push_back(0);
	}


	//BuildWalls(x, y, length, width, height, doors, doorCount, wallVertices);
	//AssignIndices(doors, doorCount, wallIndices, wallVertices.size());
	BuildNormalWallsDoors(x, y, length, width, height, doors, wallVertices);
	AssignNewIndices(doors, doorCount, wallIndices, wallVertices.size());

	CalculateNormals(wallIndices, wallVertices, true);
	CalculateNormals(floorIndices, floorVertices);

	Mesh ceilingMesh = {};

	if (doors[4] != 0)
	{
		CalculateNormals(ceilingIndices, ceilingVertices);
		CalculateTangents(ceilingIndices, ceilingVertices);

		ceilingMesh = Mesh(ceilingVertices, ceilingIndices, ceilingTextures);
	}

	CalculateTangents(wallIndices, wallVertices);
	CalculateTangents(floorIndices, floorVertices);


	vec3 pos = { x,0,y };


	return Room(pos, Mesh(floorVertices, floorIndices, floorTextures), Mesh(wallVertices, wallIndices, wallTextures), ceilingMesh);
}

//void BuildNormalWalls(float x, float y,float length, float width, float height, bool doors[4], vector<Vertex> &vertices)
//{
//	length -= 0.001f;
//	width -= 0.001f;
//
//	int lengthFinal = (length / 2);
//	int widthFinal = 1.0 * width / 2;
//	int topHeightTex = 1 * height / 2;
//	float middleTexHeight = 0.667 * topHeightTex;
//
//	//Bottom
//	//Wall A
//	GenerateVert(x - (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
//	if (doors[0] == true)
//	{
//		GenerateVert(x - (width / 4), y + (length / 2), 0, 0.25 * widthFinal, 0.0, vertices); // 5.1
//		GenerateVert(x + (width / 4), y + (length / 2), 0, 0.75 * widthFinal, 0.0, vertices); // 5.2
//	}
//	GenerateVert(x + (width / 2), y + (length / 2), 0, widthFinal, 0.0, vertices);
//
//	//Wall B
//	GenerateVert(x + (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
//	if (doors[1] == true)
//	{
//		GenerateVert(x + (width / 2), y + (length / 4), 0, 0.25 * lengthFinal, 0.0, vertices); // 5.1
//		GenerateVert(x + (width / 2), y - (length / 4), 0, 0.75 * lengthFinal, 0.0, vertices); // 5.2
//	}
//	GenerateVert(x + (width / 2), y - (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);
//	 
//	//Wall C
//	GenerateVert(x + (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
//	if (doors[2] == true)
//	{
//		GenerateVert(x + (width / 4), y - (length / 2), 0, 0.25 * widthFinal, 0.0, vertices); // 5.1
//		GenerateVert(x - (width / 4), y - (length / 2), 0, 0.75 * widthFinal, 0.0, vertices); // 5.2
//	}
//	GenerateVert(x - (width / 2), y - (length / 2), 0, widthFinal, 0.0, vertices);
//
//	//Wall D
//	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
//	if (doors[3] == true)
//	{
//		GenerateVert(x - (width / 2), y - (length / 4), 0,(0.25 * lengthFinal), 0.0, vertices); // 5.1
//		GenerateVert(x - (width / 2), y + (length / 4), 0,(0.75 * lengthFinal), 0.0, vertices); // 5.2
//	}
//	GenerateVert(x - (width / 2), y + (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);
//
//	//Middle
//	if (doors[0] == true)
//	{
//		GenerateVert(x - (width / 4), y + (length / 2), (height / 1.5), 0.25 * widthFinal, middleTexHeight, vertices);
//		GenerateVert(x + (width / 4), y + (length / 2), (height / 1.5), 0.75 * widthFinal, middleTexHeight, vertices);
//	}
//	if (doors[1] == true)
//	{
//		GenerateVert(x + (width / 2), y + (length / 4), (height / 1.5), (0.25 * lengthFinal), middleTexHeight, vertices);
//		GenerateVert(x + (width / 2), y - (length / 4), (height / 1.5), (0.75 * lengthFinal), middleTexHeight, vertices);
//	}
//	if (doors[2] == true)
//	{
//		GenerateVert(x + (width / 4), y - (length / 2), (height / 1.5), 0.25 * widthFinal, middleTexHeight, vertices);
//		GenerateVert(x - (width / 4), y - (length / 2), (height / 1.5), 0.75 * widthFinal, middleTexHeight, vertices);
//	}
//	if (doors[3] == true)
//	{
//		GenerateVert(x - (width / 2), y - (length / 4), (height / 1.5), (0.25 * lengthFinal), middleTexHeight, vertices);
//		GenerateVert(x - (width / 2), y + (length / 4), (height / 1.5), (0.75 * lengthFinal), middleTexHeight, vertices);
//	}
//
//	//Top
//	//Wall A
//	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
//	if (doors[0] == true)
//	{
//		GenerateVert(x - (width / 4), y + (length / 2), height, 0.25 * widthFinal, topHeightTex, vertices); // 5.1
//		GenerateVert(x + (width / 4), y + (length / 2), height, 0.75 * widthFinal, topHeightTex, vertices); // 5.2
//	}
//	GenerateVert(x + (width / 2), y + (length / 2), height, widthFinal, topHeightTex, vertices);
//	
//	//Wall B
//	GenerateVert(x + (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
//	if (doors[1] == true)
//	{
//		GenerateVert(x + (width / 2), y + (length / 4), height, (0.25 * lengthFinal), topHeightTex, vertices); // 5.1
//		GenerateVert(x + (width / 2), y - (length / 4), height, (0.75 * lengthFinal), topHeightTex, vertices); // 5.2
//	}
//	GenerateVert(x + (width / 2), y - (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);
//	
//	//Wall C
//	GenerateVert(x + (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
//	if (doors[2] == true)
//	{
//		GenerateVert(x + (width / 4), y - (length / 2), height, 0.25 * widthFinal, topHeightTex, vertices); // 5.1
//		GenerateVert(x - (width / 4), y - (length / 2), height, 0.75 * widthFinal, topHeightTex, vertices); // 5.2
//	}
//	GenerateVert(x - (width / 2), y - (length / 2), height, widthFinal, topHeightTex, vertices);
//	
//	//Wall D
//	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
//	if (doors[3] == true)
//	{
//		GenerateVert(x - (width / 2), y - (length / 4), height, (0.25 * lengthFinal), topHeightTex, vertices); // 5.1
//		GenerateVert(x - (width / 2), y + (length / 4), height, (0.75 * lengthFinal), topHeightTex, vertices); // 5.2
//	}
//	GenerateVert(x - (width / 2), y + (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);
//}

void BuildNormalWalls(float x, float y, float length, float width, float height, int doors[5], vector<Vertex>& vertices)
{
	length -= 0.001f;
	width -= 0.001f;

	int lengthFinal = (length / 2);
	int widthFinal = 1.0 * width / 2;
	int topHeightTex = 1 * height / 2;
	float middleTexHeight = 0.667 * topHeightTex;

	//Bottom
	//Wall A
	GenerateVert(x - (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
	if (doors[0] == 2)
	{
		GenerateVert(x - (width / 4), y + (length / 2), 0, 0.25 * widthFinal, 0.0, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), 0, 0.75 * widthFinal, 0.0, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y + (length / 2), 0, widthFinal, 0.0, vertices);

	//Wall B
	GenerateVert(x + (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
	if (doors[1] == 2)
	{
		GenerateVert(x + (width / 2), y + (length / 4), 0, 0.25 * lengthFinal, 0.0, vertices); // 5.1
		GenerateVert(x + (width / 2), y - (length / 4), 0, 0.75 * lengthFinal, 0.0, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y - (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);

	//Wall C
	GenerateVert(x + (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
	if (doors[2] == 2)
	{
		GenerateVert(x + (width / 4), y - (length / 2), 0, 0.25 * widthFinal, 0.0, vertices); // 5.1
		GenerateVert(x - (width / 4), y - (length / 2), 0, 0.75 * widthFinal, 0.0, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y - (length / 2), 0, widthFinal, 0.0, vertices);

	//Wall D
	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
	if (doors[3] == 2)
	{
		GenerateVert(x - (width / 2), y - (length / 4), 0, (0.25 * lengthFinal), 0.0, vertices); // 5.1
		GenerateVert(x - (width / 2), y + (length / 4), 0, (0.75 * lengthFinal), 0.0, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y + (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);

	//Middle
	if (doors[0] == 2)
	{
		GenerateVert(x - (width / 4), y + (length / 2), (height / 1.5), 0.25 * widthFinal, middleTexHeight, vertices);
		GenerateVert(x + (width / 4), y + (length / 2), (height / 1.5), 0.75 * widthFinal, middleTexHeight, vertices);
	}
	if (doors[1] == 2)
	{
		GenerateVert(x + (width / 2), y + (length / 4), (height / 1.5), (0.25 * lengthFinal), middleTexHeight, vertices);
		GenerateVert(x + (width / 2), y - (length / 4), (height / 1.5), (0.75 * lengthFinal), middleTexHeight, vertices);
	}
	if (doors[2] == 2)
	{
		GenerateVert(x + (width / 4), y - (length / 2), (height / 1.5), 0.25 * widthFinal, middleTexHeight, vertices);
		GenerateVert(x - (width / 4), y - (length / 2), (height / 1.5), 0.75 * widthFinal, middleTexHeight, vertices);
	}
	if (doors[3] == 2)
	{
		GenerateVert(x - (width / 2), y - (length / 4), (height / 1.5), (0.25 * lengthFinal), middleTexHeight, vertices);
		GenerateVert(x - (width / 2), y + (length / 4), (height / 1.5), (0.75 * lengthFinal), middleTexHeight, vertices);
	}

	//Top
	//Wall A
	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[0] == 2)
	{
		GenerateVert(x - (width / 4), y + (length / 2), height, 0.25 * widthFinal, topHeightTex, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), height, 0.75 * widthFinal, topHeightTex, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y + (length / 2), height, widthFinal, topHeightTex, vertices);

	//Wall B
	GenerateVert(x + (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[1] == 2)
	{
		GenerateVert(x + (width / 2), y + (length / 4), height, (0.25 * lengthFinal), topHeightTex, vertices); // 5.1
		GenerateVert(x + (width / 2), y - (length / 4), height, (0.75 * lengthFinal), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y - (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);

	//Wall C
	GenerateVert(x + (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[2] == 2)
	{
		GenerateVert(x + (width / 4), y - (length / 2), height, 0.25 * widthFinal, topHeightTex, vertices); // 5.1
		GenerateVert(x - (width / 4), y - (length / 2), height, 0.75 * widthFinal, topHeightTex, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y - (length / 2), height, widthFinal, topHeightTex, vertices);

	//Wall D
	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[3] == 2)
	{
		GenerateVert(x - (width / 2), y - (length / 4), height, (0.25 * lengthFinal), topHeightTex, vertices); // 5.1
		GenerateVert(x - (width / 2), y + (length / 4), height, (0.75 * lengthFinal), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y + (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);
}

void BuildNormalWallsDoors(float x, float y, float length, float width, float height, int doors[5], vector<Vertex>& vertices)
{
	length -= 0.001f;
	width -= 0.001f;

	int lengthFinal = 1.0 * length / 2;
	int widthFinal = 1.0 * width / 2;
	int topHeightTex = 1 * height / 2;
	float middleTexHeight = 0.667 * topHeightTex;

	float doorSize = 0.8f;

	//Bottom
	//Wall A
	GenerateVert(x - (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
	if (doors[0] == 2)
	{
		GenerateVert(x - doorSize, y + (length / 2), 0, (0.5 * (widthFinal - doorSize)), 0.0, vertices); // 5.1
		GenerateVert(x + doorSize, y + (length / 2), 0, (0.5 * (widthFinal + doorSize)), 0.0, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y + (length / 2), 0, widthFinal, 0.0, vertices);

	//Wall B
	GenerateVert(x + (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
	if (doors[1] == 2)
	{
		GenerateVert(x + (width / 2), y + doorSize, 0, (0.5 * (lengthFinal - doorSize)), 0.0, vertices); // 5.1
		GenerateVert(x + (width / 2), y - doorSize, 0, (0.5 * (lengthFinal + doorSize)), 0.0, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y - (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);

	//Wall C
	GenerateVert(x + (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
	if (doors[2] == 2)
	{
		GenerateVert(x + doorSize, y - (length / 2), 0, (0.5 * (widthFinal - doorSize)), 0.0, vertices); // 5.1
		GenerateVert(x - doorSize, y - (length / 2), 0, (0.5 * (widthFinal + doorSize)), 0.0, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y - (length / 2), 0, widthFinal, 0.0, vertices);

	//Wall D
	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
	if (doors[3] == 2)
	{
		GenerateVert(x - (width / 2), y - doorSize, 0, (0.5 * (lengthFinal - doorSize)), 0.0, vertices); // 5.1
		GenerateVert(x - (width / 2), y + doorSize, 0, (0.5 * (lengthFinal + doorSize)), 0.0, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y + (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);

	//Middle
	if (doors[0] == 2)
	{
		GenerateVert(x - doorSize, y + (length / 2), (height / 1.5), (0.5 * (widthFinal - doorSize)), middleTexHeight, vertices);
		GenerateVert(x + doorSize, y + (length / 2), (height / 1.5), (0.5 * (widthFinal + doorSize)), middleTexHeight, vertices);
	}
	if (doors[1] == 2)
	{
		GenerateVert(x + (width / 2), y + doorSize, (height / 1.5), (0.5 * (lengthFinal - doorSize)), middleTexHeight, vertices);
		GenerateVert(x + (width / 2), y - doorSize, (height / 1.5), (0.5 * (lengthFinal + doorSize)), middleTexHeight, vertices);
	}
	if (doors[2] == 2)
	{
		GenerateVert(x + doorSize, y - (length / 2), (height / 1.5), (0.5 * (widthFinal - doorSize)), middleTexHeight, vertices);
		GenerateVert(x - doorSize, y - (length / 2), (height / 1.5), (0.5 * (widthFinal + doorSize)), middleTexHeight, vertices);
	}
	if (doors[3] == 2)
	{
		GenerateVert(x - (width / 2), y - doorSize, (height / 1.5), (0.5 * (lengthFinal - doorSize)), middleTexHeight, vertices);
		GenerateVert(x - (width / 2), y + doorSize, (height / 1.5), (0.5 * (lengthFinal + doorSize)), middleTexHeight, vertices);
	}

	//Top
	//Wall A
	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[0] == 2)
	{
		GenerateVert(x - doorSize, y + (length / 2), height, (0.5 * (widthFinal - doorSize)), topHeightTex, vertices); // 5.1
		GenerateVert(x + doorSize, y + (length / 2), height, (0.5 * (widthFinal + doorSize)), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y + (length / 2), height, widthFinal, topHeightTex, vertices);

	//Wall B
	GenerateVert(x + (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[1] == 2)
	{
		GenerateVert(x + (width / 2), y + doorSize, height, (0.5 * (lengthFinal - doorSize)), topHeightTex, vertices); // 5.1
		GenerateVert(x + (width / 2), y - doorSize, height, (0.5 * (lengthFinal + doorSize)), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y - (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);

	//Wall C
	GenerateVert(x + (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[2] == 2)
	{
		GenerateVert(x + doorSize, y - (length / 2), height, (0.5 * (widthFinal - doorSize)), topHeightTex, vertices); // 5.1
		GenerateVert(x - doorSize, y - (length / 2), height, (0.5 * (widthFinal + doorSize)), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y - (length / 2), height, widthFinal, topHeightTex, vertices);

	//Wall D
	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[3] == 2)
	{
		GenerateVert(x - (width / 2), y - doorSize, height, (0.5 * (lengthFinal - doorSize)), topHeightTex, vertices); // 5.1
		GenerateVert(x - (width / 2), y + doorSize, height, (0.5 * (lengthFinal + doorSize)), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y + (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);
}

void AssignNewIndices(int doors[5], int doorCount, vector<unsigned int>& indices, int vertSize)
{
	int yWidth = 8 + (4 * doorCount);
	int xWidth = (8 + (2 * doorCount));

	int val2 = (vertSize - (xWidth * 2 + (doorCount * 2)));

	for (int i = 0, j = 0; j < 4; i += 2, ++j) // increment j in code
	{
		if (doors != nullptr && doors[j] == 2)
		{
			DrawDoorIndices(i, yWidth, xWidth, doorCount, j + 1, indices);
			i += 2;
		}
		else if(doors[j] == 1)
		{
			DrawQuadIndices(i, yWidth, indices);
		}
	}
}

void DrawQuadIndices(int val, int xWidth, vector<unsigned int>& indices)
{
	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + xWidth + 1);

	indices.push_back(val + xWidth + 1);
	indices.push_back(val + xWidth);
	indices.push_back(val);
}

void DrawDoorIndices(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int>& indices)
{
	int doorOffset = (2 + (2 * doorCount));

	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + yWidth + 1);

	indices.push_back(val + yWidth + 1);
	indices.push_back(val + yWidth);
	indices.push_back(val);

	++val;

	int middleVal = val + xWidth - doorNum - (doorNum - 1);
	indices.push_back(middleVal);
	indices.push_back(middleVal + 1);
	indices.push_back(val + xWidth + doorOffset - 1);

	indices.push_back(val + xWidth + doorOffset - 1);
	indices.push_back(val + xWidth + doorOffset - 2);
	indices.push_back(middleVal);

	++val;

	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + yWidth + 1);

	indices.push_back(val + yWidth + 1);
	indices.push_back(val + yWidth);
	indices.push_back(val);
}

void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex>& vertices)
{
	Vertex vert = {};

	vert.Position = { x,z,y };
	vert.TexCoords = { u,v };

	vertices.push_back(vert);
}

void CalculateNormals(vector<unsigned int>& indices, vector<Vertex>& vertices, bool flipNormal)
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		int j = i + 1;
		int k = i + 2;

		Vertex vert1 = vertices[indices[i]];
		Vertex vert2 = vertices[indices[j]];
		Vertex vert3 = vertices[indices[k]];

		glm::vec3 edgeA = vert1.Position - vert2.Position;
		glm::vec3 edgeB = vert1.Position - vert3.Position;

		glm::vec3 normal = CrossProduct(edgeA, edgeB);


		if (flipNormal)
		{
			normal = -normal;
		}

		vertices[indices[i]].Normal = normal;
		vertices[indices[j]].Normal = normal;
		vertices[indices[k]].Normal = normal;
	}

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}
}

void CalculateTangents(vector<unsigned int>& indices, vector<Vertex>& vertices)
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		int j = i + 1;
		int k = i + 2;

		vec3 tangent;
		vec3 bitangent;

		vec3 pos1 = vertices[indices[i]].Position;
		vec3 pos2 = vertices[indices[j]].Position;
		vec3 pos3 = vertices[indices[k]].Position;

		vec2 uv1 = vertices[indices[i]].TexCoords;
		vec2 uv2 = vertices[indices[j]].TexCoords;
		vec2 uv3 = vertices[indices[k]].TexCoords;

		vec3 edgeA = pos2 - pos1;
		vec3 edgeB = pos3 - pos1;

		vec2 deltaUV1 = uv2 - uv1;
		vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		
		tangent.x = f * (deltaUV2.y * edgeA.x - deltaUV1.y * edgeB.x);
		tangent.y = f * (deltaUV2.y * edgeA.y - deltaUV1.y * edgeB.y);
		tangent.z = f * (deltaUV2.y * edgeA.z - deltaUV1.y * edgeB.z);
		tangent = glm::normalize(tangent);

		bitangent.x = f * (-deltaUV2.x * edgeA.x + deltaUV1.x * edgeB.x);
		bitangent.y = f * (-deltaUV2.x * edgeA.y + deltaUV1.x * edgeB.y);
		bitangent.z = f * (-deltaUV2.x * edgeA.z + deltaUV1.x * edgeB.z);
		bitangent = glm::normalize(bitangent);

		vertices[indices[i]].Tangent = tangent;
		vertices[indices[i]].Bitangent = bitangent;
	}
}

vec3 CrossProduct(vec3 a, vec3 b)
{
	float crossX = (a.y * b.z) - (a.z * b.y);
	float crossY = (a.x * b.z) - (a.z * b.x);
	float crossZ = (a.x * b.y) - (a.y * b.x);

	return glm::vec3(crossX, crossY, crossZ);
}

#endif