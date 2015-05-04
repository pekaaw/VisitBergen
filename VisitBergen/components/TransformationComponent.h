#pragma once

#include <glm/glm.hpp>

#include "../actorManager/ActorComponent.h"

class TransformationComponent : ActorComponent
{
	glm::mat4 bodyTransform;
	glm::mat4 worldTransform;

public:
	TransformationComponent(const glm::mat4& bodyTransformation, const glm::mat4& worldTransformation)
	{
		this->bodyTransform = bodyTransform;
		this->worldTransform = worldTransform;
	}

	TransformationComponent()
	{
		this->bodyTransform = glm::mat4();
		this->worldTransform = glm::mat4();
	}

	bool dependancyInjection() { return true; }
	void init() {}
};