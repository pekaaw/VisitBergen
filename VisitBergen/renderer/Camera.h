#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 upVector;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

public:
	Camera()
	{
		position = glm::vec3(0.0f, 0.0f, -2.0f);
		target = glm::vec3(0.0f, 0.0f, 0.0f);
		upVector = glm::vec3(0.0f, 1.0f, 0.0f);

		this->viewMatrix = glm::lookAt(position, target, upVector);
	}

	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upVector)
	{
		this->position = position;
		this->target = target;
		this->upVector = upVector;

		this->viewMatrix = glm::lookAt(position, target, upVector);
	}

	glm::mat4 transform(const glm::mat4& matrix)
	{
		this->viewMatrix *= matrix;
		return this->viewMatrix;
	}

	const glm::mat4 getViewMatrix() const
	{
		return this->viewMatrix;
	}

	void setProjectionMatrix(const glm::mat4 & projectionMatrix)
	{
		this->projectionMatrix = projectionMatrix;
	}

	const glm::mat4 getProjectionMatrix() const
	{
		return this->projectionMatrix;
	}
};