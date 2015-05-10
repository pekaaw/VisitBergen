#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <tinyxml2.h>
#include "../actorManager/ActorComponent.h"

class TransformComponent : public ActorComponent
{
private:
	glm::mat4 transform;

public:
	//TransformComponent(const glm::mat4& bodyTransformation, const glm::mat4& worldTransformation)
	TransformComponent(const glm::mat4& transform)
	{
		this->transform = transform;
		//this->bodyTransform = bodyTransform;
		//this->worldTransform = worldTransform;
	}

	TransformComponent()
	{
		this->transform = glm::mat4();
		//this->bodyTransform = glm::mat4();
		//this->worldTransform = glm::mat4();
	}

	const char* getComponentName(){ return this->componentName; }
	bool preInit(tinyxml2::XMLElement*);
	bool dependancyInjection() { return true; }
	void init() {}

	static const char* componentName;

	const glm::mat4& getTransform() const;

	//glm::mat4 bodyTransform;
	//glm::mat4 worldTransform;
};