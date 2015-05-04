#pragma once

#include "..\actorManager\ActorComponent.h"
#include "TransformationComponent.h"

class RenderComponent : public ActorComponent
{
	std::shared_ptr<TransformationComponent> transform;

	bool dependancyInjection()
	{
		//this->owner->
		return 0;
		// TODO: finish up
	}

	void init( void ) {}
};