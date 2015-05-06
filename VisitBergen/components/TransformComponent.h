#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <tinyxml2.h>
#include "../actorManager/ActorComponent.h"

class TransformComponent : public ActorComponent
{
public:
	TransformComponent(const glm::mat4& bodyTransformation, const glm::mat4& worldTransformation)
	{
		this->bodyTransform = bodyTransform;
		this->worldTransform = worldTransform;
	}

	TransformComponent()
	{
		this->bodyTransform = glm::mat4();
		this->worldTransform = glm::mat4();
	}

	const char* getComponentName(){ return this->componentName; }
	bool preInit(tinyxml2::XMLElement*);
	bool dependancyInjection() { return true; }
	void init() {}

	static const char* componentName;

	glm::mat4 bodyTransform;
	glm::mat4 worldTransform;
};