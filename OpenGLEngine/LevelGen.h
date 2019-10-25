#ifndef LEVELGEN_H
#define LEVELGEN_H

#include <mesh.h>
#include <shader.h>

void BuildNormalWalls(float x, float y, float length, float width, float height, bool doors[4], vector<Vertex>& vertices);
void BuildDoor();

void AssignNewIndices(bool doors[4], int doorCount, vector<unsigned int>& indices, int vertSize);
void DrawQuadIndices(int val, int xWidth, vector<unsigned int>& indices);
void DrawDoorIndices(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int>& indices);
void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex>& vertices);

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

#endif