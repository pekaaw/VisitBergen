#include "Actor.h"

Actor::Actor(const int actorID)
{
	this->actorID = actorID;
}

const int Actor::getActorID() const
{
	return this->actorID;
}

const std::string Actor::getActorName() const
{
	return this->actorName;
}

bool addComponent(std::string componentType)
{
	return true;
	// TODO: finish up
}

std::shared_ptr<ActorComponent> Actor::getComponent(std::string componentType) const
{
	auto it = this->components.find(componentType);

	if (it != this->components.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

