#ifndef LEVELGEN_H
#define LEVELGEN_H

#include <mesh.h>
#include <shader.h>


void BuildNormalWalls(float x, float y,float length, float width, float height, vector<Vertex> &vertices)
{
	GenerateVert(x - (width / 2), y + (length / 2), 0, 0, 1.0, vertices);
	GenerateVert(x + (width / 2), y + (length / 2), 0, 1, 1.0, vertices);

	GenerateVert(x + (width / 2), y + (length / 2), 0, 0, 1.0, vertices);
	GenerateVert(x + (width / 2), y - (length / 2), 0, 1, 1.0, vertices);

	GenerateVert(x + (width / 2), y - (length / 2), 0, 0, 1.0, vertices);
	GenerateVert(x - (width / 2), y - (length / 2), 0, 1, 1.0, vertices);

	GenerateVert(x - (width / 2), y - (length / 2), 0, 0, 1.0, vertices);
	GenerateVert(x - (width / 2), y + (length / 2), 0, 1, 1.0, vertices);
}

void BuildDoor()
{

}

void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex>& vertices)
{
	Vertex vert = {};

	vert.Position = { x,z,y };
	vert.TexCoords = { u,v };

	vertices.push_back(vert);
}

#endif