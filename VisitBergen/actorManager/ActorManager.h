#pragma once

#include <map>
#include <vector>
#include <string>

#include <tinyxml2.h>
using namespace tinyxml2;

#include "Actor.h"
#include "ComponentFactory.h"

class MeshComponent;

class ActorManager
{
private:
	std::vector<std::shared_ptr<Actor>> actorList;
	int nextActorID;

	ComponentFactory componentFactory;
	XMLDocument* actorXML;

	const int getNextActorID();
	int createActor(XMLElement* actorDescriptionRoot);

public:

	explicit ActorManager();

	int createActor(const std::string actorPath);
};