#pragma once

#include <glm\glm.hpp>
#include "../util/includeGL.h"

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct Light
{
	glm::vec4 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct GLState
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	Material material;
	Light light;

	GLState()
	{
		this->material.shininess = 0.5f;
		this->material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		this->material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		
		this->light.position = glm::vec4(0.0f, 0.0f, -10.0f, 0.0f);
		this->light.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		this->light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	}
};