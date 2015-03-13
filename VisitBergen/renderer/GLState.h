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
	glm::vec3 intensities;
};

struct GLState
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	Material material;
	Light headLight;
	Light directionalLight;

	GLState()
	{
		this->material.shininess = 1.0f;
		this->material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		this->material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		
		this->headLight.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // not used
		this->headLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->headLight.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
		this->headLight.specular = glm::vec3(0.0f, 1.0f, 0.0f);
		this->headLight.intensities = glm::vec3(0.1f, 0.5f, 0.9f);	// vec3(ambient-, diffuse-, specular-) intensity

		this->directionalLight.position = glm::vec4(10.0f, 3.0f, -3.0f, 0.0f);
		this->directionalLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->directionalLight.diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
		this->directionalLight.specular = glm::vec3(0.0f, 0.0f, 1.0f);
		this->directionalLight.intensities = glm::vec3(0.1f, 0.8f, 0.9f);	// vec3(ambient-, diffuse-, specular-) intensity

	}
};