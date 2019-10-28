#ifndef LEVELGEN_H
#define LEVELGEN_H

#include <mesh.h>
#include <shader.h>
#include <glm/glm.hpp>

using namespace glm;

void BuildNormalWalls(float x, float y, float length, float width, float height, bool doors[4], vector<Vertex>& vertices);
void BuildDoor();

Room GenerateRoom(float x, float y, float length, float width, float height, bool doors[], string floorPath, string wallPath, string ceilingPath);

void AssignNewIndices(bool doors[4], int doorCount, vector<unsigned int>& indices, int vertSize);
void DrawQuadIndices(int val, int xWidth, vector<unsigned int>& indices);
void DrawDoorIndices(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int>& indices);
void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex>& vertices);

void CalculateNormals(vector<unsigned int>& indices, vector<Vertex>& vertices);
glm::vec3 CrossProduct(glm::vec3 a, glm::vec3 b);


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

Room GenerateRoom(float x, float y, float length, float width, float height, bool doors[], string floorPath, string wallPath, string ceilingPath)
{
	int doorCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (doors[i] == true)
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
	floorTextures.push_back(floorTex);

	Texture wallTex = LoadTexture(wallPath.c_str(), "texture_diffuse");
	wallTextures.push_back(wallTex);

	Texture ceilingTex = LoadTexture(ceilingPath.c_str(), "texture_diffuse");
	ceilingTextures.push_back(ceilingTex);

	int texCoordWidth = (1 * width / 2);
	int texCoordLength = (1 * length / 2);

	GenerateVert(x - (width / 2), y + (length / 2), 0, 0.0, texCoordLength, floorVertices); // 0
	GenerateVert(x + (width / 2), y + (length / 2), 0, texCoordWidth, texCoordLength, floorVertices); // 1
	GenerateVert(x + (width / 2), y - (length / 2), 0, texCoordWidth, 0.0, floorVertices); // 2
	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, floorVertices); // 3

	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, texCoordLength, ceilingVertices); // 0
	GenerateVert(x + (width / 2), y + (length / 2), height, texCoordWidth, texCoordLength, ceilingVertices); // 1
	GenerateVert(x + (width / 2), y - (length / 2), height, texCoordWidth, 0.0, ceilingVertices); // 2
	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, 0.0, ceilingVertices); // 3

	floorIndices.push_back(0);
	floorIndices.push_back(3);
	floorIndices.push_back(2);
	floorIndices.push_back(2);
	floorIndices.push_back(1);
	floorIndices.push_back(0);

	ceilingIndices.push_back(0);
	ceilingIndices.push_back(1);
	ceilingIndices.push_back(2);
	ceilingIndices.push_back(2);
	ceilingIndices.push_back(3);
	ceilingIndices.push_back(0);

	//BuildWalls(x, y, length, width, height, doors, doorCount, wallVertices);
	//AssignIndices(doors, doorCount, wallIndices, wallVertices.size());
	BuildNormalWalls(x, y, length, width, height, doors, wallVertices);
	AssignNewIndices(doors, doorCount, wallIndices, wallVertices.size());

	CalculateNormals(wallIndices, wallVertices);
	CalculateNormals(floorIndices, floorVertices);
	CalculateNormals(ceilingIndices, ceilingVertices);

	return Room(Mesh(floorVertices, floorIndices, floorTextures), Mesh(wallVertices, wallIndices, wallTextures), Mesh(ceilingVertices, ceilingIndices, ceilingTextures));
}

void BuildNormalWalls(float x, float y,float length, float width, float height, bool doors[4], vector<Vertex> &vertices)
{
	int lengthFinal = (length / 2);
	//Bottom
	//Wall A
	GenerateVert(x - (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
	if (doors[0] == true)
	{
		GenerateVert(x - (width / 4), y + (length / 2), 0, 0.25 * width / 2, 0.0, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), 0, 0.75 * width / 2, 0.0, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y + (length / 2), 0, 1.0 * width / 2, 0.0, vertices);

	//Wall B
	GenerateVert(x + (width / 2), y + (length / 2), 0, 0, 0.0, vertices);
	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), 0, 0.25 * lengthFinal, 0.0, vertices); // 5.1
		GenerateVert(x + (width / 2), y - (length / 4), 0, 0.75 * lengthFinal, 0.0, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y - (length / 2), 0, (int)(1 * length / 2), 0.0, vertices); cout << (int)(1 * length / 2) << " Max length" << endl;
	 
	//Wall C
	GenerateVert(x + (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), 0, 0.25 * width / 2, 0.0, vertices); // 5.1
		GenerateVert(x - (width / 4), y - (length / 2), 0, 0.75 * width / 2, 0.0, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y - (length / 2), 0, 1.0 * width / 2, 0.0, vertices);

	//Wall D
	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, vertices);
	if (doors[3] == true)
	{
		GenerateVert(x - (width / 2), y - (length / 4), 0,(0.25 * lengthFinal), 0.0, vertices); // 5.1
		GenerateVert(x - (width / 2), y + (length / 4), 0,(0.75 * lengthFinal), 0.0, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y + (length / 2), 0, (int)(1 * length / 2), 0.0, vertices);


	float middleTexHeight = 0.667 * height / 2;

	cout << middleTexHeight << " middle height texture" << endl;
	//Middle
	if (doors[0] == true)
	{
		GenerateVert(x - (width / 4), y + (length / 2), (height / 1.5), 0.25 * width / 2, middleTexHeight, vertices);
		GenerateVert(x + (width / 4), y + (length / 2), (height / 1.5), 0.75 * width / 2, middleTexHeight, vertices);
	}
	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), (height / 1.5), (0.25 * lengthFinal), middleTexHeight, vertices);
		GenerateVert(x + (width / 2), y - (length / 4), (height / 1.5), (0.75 * lengthFinal), middleTexHeight, vertices);
	}
	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), (height / 1.5), 0.25 * width / 2, middleTexHeight, vertices);
		GenerateVert(x - (width / 4), y - (length / 2), (height / 1.5), 0.75 * width / 2, middleTexHeight, vertices);
	}
	if (doors[3] == true)
	{
		GenerateVert(x - (width / 2), y - (length / 4), (height / 1.5), (0.25 * lengthFinal), middleTexHeight, vertices);
		GenerateVert(x - (width / 2), y + (length / 4), (height / 1.5), (0.75 * lengthFinal), middleTexHeight, vertices);
	}

	//Top
	int topHeightTex = 1 * height / 2;
	//Wall A
	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[0] == true)
	{
		GenerateVert(x - (width / 4), y + (length / 2), height, 0.25 * width / 2, topHeightTex, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), height, 0.75 * width / 2, topHeightTex, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y + (length / 2), height, 1 * width / 2, topHeightTex, vertices);
	
	//Wall B
	GenerateVert(x + (width / 2), y + (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), height, (0.25 * lengthFinal), topHeightTex, vertices); // 5.1
		GenerateVert(x + (width / 2), y - (length / 4), height, (0.75 * lengthFinal), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x + (width / 2), y - (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);
	
	//Wall C
	GenerateVert(x + (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), height, 0.25 * width / 2, topHeightTex, vertices); // 5.1
		GenerateVert(x - (width / 4), y - (length / 2), height, 0.75 * width / 2, topHeightTex, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y - (length / 2), height, 1 * width / 2, topHeightTex, vertices);
	
	//Wall D
	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, topHeightTex, vertices);
	if (doors[3] == true)
	{
		GenerateVert(x - (width / 2), y - (length / 4), height, (0.25 * lengthFinal), topHeightTex, vertices); // 5.1
		GenerateVert(x - (width / 2), y + (length / 4), height, (0.75 * lengthFinal), topHeightTex, vertices); // 5.2
	}
	GenerateVert(x - (width / 2), y + (length / 2), height, (int)(1 * length / 2), topHeightTex, vertices);
}

void BuildDoor()
{

}

void AssignNewIndices(bool doors[4], int doorCount, vector<unsigned int>& indices, int vertSize)
{
	int yWidth = 8 + (4 * doorCount);
	int xWidth = (8 + (2 * doorCount));

	int val2 = (vertSize - (xWidth * 2 + (doorCount * 2)));

	for (int i = 0, j = 0; j < 4; i += 2, ++j) // increment j in code
	{
		if (doors != nullptr && doors[j] == true)
		{
			DrawDoorIndices(i, yWidth, xWidth, doorCount, j + 1, indices);
			i += 2;
		}
		else
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

	int middleVal = val + xWidth - doorNum - (doorNum - 1); cout << middleVal << "," << middleVal + 1 << "," << val + xWidth + doorOffset - doorNum << "middle val index" << endl;
	indices.push_back(middleVal);
	indices.push_back(middleVal + 1);
	indices.push_back(val + xWidth + doorOffset - 1);

	indices.push_back(val + xWidth + doorOffset - 1); cout << val + xWidth + doorOffset - doorNum << "," << val + xWidth + doorOffset - (doorNum + 1) << "," << middleVal << "middle val index 2" << endl;
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

void CalculateNormals(vector<unsigned int>& indices, vector<Vertex>& vertices)
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		Vertex vert1 = vertices[indices[i]];
		Vertex vert2 = vertices[indices[i + 1]];
		Vertex vert3 = vertices[indices[i + 2]];

		//cout << vert1.Position.x << "," << vert2.Position.x << "," << vert3.Position.x << " X pos" << endl;

		glm::vec3 edgeA = vert1.Position - vert2.Position;
		glm::vec3 edgeB = vert1.Position - vert3.Position;

		glm::vec3 normal = CrossProduct(edgeA, edgeB);

		vertices[indices[i]].Normal = normal;
		vertices[indices[i + 1]].Normal = normal;
		vertices[indices[i + 2]].Normal = normal;

		//cout << edgeA.x << "," << edgeA.y << "," << edgeA.z << " - Edge A" << endl;
		//cout << edgeB.x << "," << edgeB.y << "," << edgeB.z << " - Edge B" << endl;
	}

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}
}

glm::vec3 CrossProduct(glm::vec3 a, glm::vec3 b)
{
	int crossX = (a.y * b.z) - (a.z * b.y);
	int crossY = (a.x * b.z) - (a.z * b.x);
	int crossZ = (a.x * b.y) - (a.y * b.x);

	//cout << crossX;
	//cout << crossY;
	//cout << crossZ;
	//cout << endl;;

	return glm::vec3(crossX, crossY, crossZ);
}

#endif