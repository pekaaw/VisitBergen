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
	GLfloat lightRadius;
	GLfloat litRadius;		// must not be 0 when used as point light
};

struct GLState
{
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	Material material;
	Light headLight;
	Light directionalLight;
	Light movingLight;

	GLState()
	{
		this->material.shininess = 1.0f;
		this->material.ambient = glm::vec3(0.0f, 1.0f, 0.0f);
		this->material.diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
		this->material.specular = glm::vec3(0.0f, 1.0f, 0.0f);
		
		this->headLight.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // not used
		this->headLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->headLight.diffuse = glm::vec3(1.0f, 0.77f, 0.16f);
		this->headLight.specular = glm::vec3(1.0f, 0.77f, 0.16f);
		this->headLight.intensities = glm::vec3(0.0f, 0.3f, 0.9f);	// vec3(ambient-, diffuse-, specular-) intensity

		this->directionalLight.position = glm::vec4(1000.0f, 400.0f, -3.0f, 0.0f);
		this->directionalLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->directionalLight.diffuse = glm::vec3(0.25f, 0.89f, 1.0f);
		this->directionalLight.specular = glm::vec3(0.0f, 0.0f, 1.0f);
		this->directionalLight.intensities = glm::vec3(0.0f, 0.8f, 0.9f);	// vec3(ambient-, diffuse-, specular-) intensity

		this->movingLight.position = glm::vec4(200.0f, 300.0f, 0.0f, 1.0f);
		this->movingLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		this->movingLight.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		this->movingLight.specular = glm::vec3(0.3f, 0.252f, 0.0f);
		//this->movingLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		//this->movingLight.diffuse = glm::vec3(1.0f, 0.84f, 0.0f);
		//this->movingLight.specular = glm::vec3(1.0f, 0.84f, 0.0f);
		//this->movingLight.ambient = glm::vec3(0.16f, 0.16f, 0.0f);
		//this->movingLight.diffuse = glm::vec3(0.16f, 0.16f, 0.0f);
		//this->movingLight.specular = glm::vec3(0.16f, 0.16f, 0.0f);
		this->movingLight.intensities = glm::vec3(0.0f, 1.0f, 0.5f);
		this->movingLight.lightRadius = 5.0f;
		this->movingLight.litRadius = 500.0f; // must not be 0 when used as point light
	}
};