#pragma once

#include <map>
#include <vector>
#include <string>

#include <tinyxml2.h>
using namespace tinyxml2;

#include "Actor.h"

class ActorManager
{
	std::vector<std::shared_ptr<Actor>> actorList;
	int nextActorID;

	explicit ActorManager() : nextActorID(0) {};
	const int getNextActorID();

public:
	int createActor(XMLElement* actorDescriptionRoot);
	int createActor(const std::string actorPath);
};