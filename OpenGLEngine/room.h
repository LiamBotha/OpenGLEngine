#ifndef ROOM_H
#define ROOM_H

#include <mesh.h>
#include <shader.h>
#include <glm/glm.hpp>

class Room
{
public:

	vector<Mesh> meshes;
	glm::vec3 position = {0,0,0};

	Room()
	{
		meshes = {};
		position = {0,0,0};
	}

	Room(glm::vec3 pos, Mesh floor, Mesh walls, Mesh ceiling)
	{
		position = pos;

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
