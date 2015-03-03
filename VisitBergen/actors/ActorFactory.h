#pragma once

#include <map>
#include <string>
#include <memory>

#include <tinyxml2.h>

#include "..\interfaces\interfaces.h"
#include "Actor.h"

typedef ActorComponent *(*ActorComponentCreator)(void);

using namespace tinyxml2;

class ActorFactory
{
	ActorID nextActorID;
	ActorID getNextActorID(void);

protected:
	std::map<std::string, ActorComponentCreator> componentCreatorMap;
	virtual std::shared_ptr<ActorComponent> createComponent(XMLElement* componentName);

public:
	ActorFactory();
	~ActorFactory();

	std::shared_ptr<Actor> createActor(const char* actorSpecification);
};