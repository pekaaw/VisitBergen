#pragma once

#include "..\actorManager\ActorComponent.h"
#include "TransformComponent.h"

class RenderComponent : public ActorComponent
{
	std::shared_ptr<TransformComponent> transform;

public:
	const char* getComponentName(){
		return this->componentName;
	}

	bool dependancyInjection()
	{
		//this->owner->
		return 0;
		// TODO: finish up
	}

	bool preInit(tinyxml2::XMLElement*) { return true; }
	void init() {}

	static const char* componentName;
};

const char* RenderComponent::componentName = "RenderComponent";