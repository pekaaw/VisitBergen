#include "Actor.h"

Actor::Actor(const int actorID)
{
	this->actorID = actorID;
}

const int Actor::getActorID() const
{
	return this->actorID;
}

void Actor::setActorName(std::string name)
{
	this->actorName = name;
}

const std::string Actor::getActorName() const
{
	return this->actorName;
}

bool Actor::addComponent(std::string componentType, std::shared_ptr<ActorComponent> component)
{
	this->components[componentType] = component;
	return true;
}

std::shared_ptr<ActorComponent> Actor::getComponent(std::string componentType) const
{
	auto it = this->components.find(componentType);

	if (it != this->components.end())
	{
		return it->second;
	}
	
	return nullptr;
}

