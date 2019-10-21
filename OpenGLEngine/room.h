#ifndef ROOM_H
#define ROOM_H

#include <mesh.h>
#include <shader.h>

class Room
{
public:

	vector<Mesh> meshes;

	Room()
	{
		meshes = {};
	}

	Room(Mesh floor, Mesh walls, Mesh ceiling)
	{
		meshes.push_back(floor);
		meshes.push_back(walls);
		meshes.push_back(ceiling);
	}

	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

};

#endif
