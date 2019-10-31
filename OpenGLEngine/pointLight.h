#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <shader.h>
using namespace glm;

class Light
{
public:

	vec3 position;
	vec3 rotation;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	int type = 0;

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float cutOff;
	float outerCutOff;

	Light(int getType = 0)
	{
		type = getType;

		position = {0,0,0};
		rotation = {0,0,0};

		ambient = {0,0,0};
		diffuse = {0,0,0};
		specular = {0,0,0};
	}

	Light(vec3 getPosition, vec3 getRotation = {0,0,0}, int getType = 0)
	{
		type = getType;

		position = getPosition;
		rotation = getRotation;

		ambient = { 0.05f, 0.05f, 0.05f };
		diffuse = { 0.8f, 0.8f, 0.8f };
		specular = { 1.0f, 1.0f, 1.0f };

		if (type == 1)
		{
			cutOff = cos(glm::radians(12.5f));
			outerCutOff = cos(glm::radians(15.0f));
		}
	}

	void RenderLight(int val, Shader shader)
	{
		string pointLight = "";

		if (type == 0)
			pointLight = "pointLights[" + std::to_string(val) + "]";
		else if (type == 1)
			pointLight = "spotLights[" + std::to_string(val) + "]";

		shader.setVec3(pointLight + ".position", position);
		shader.setVec3(pointLight + ".ambient", ambient);
		shader.setVec3(pointLight + ".diffuse", diffuse);
		shader.setVec3(pointLight + ".specular", specular);
		shader.setFloat(pointLight + ".constant", constant);
		shader.setFloat(pointLight + ".linear", linear);
		shader.setFloat(pointLight + ".quadratic", quadratic);

		if (type == 1)
		{
			shader.setVec3(pointLight + ".direction", vec3(0,1,0));
			shader.setFloat(pointLight + ".cutOff", cutOff);
			shader.setFloat(pointLight + ".outerCutOff", outerCutOff);
		}
	}
};

#endif