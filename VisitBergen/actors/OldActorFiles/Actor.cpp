#include "Actor.h"

Actor::Actor(ActorID id)
{
	this->actorID = id;
}

Actor::~Actor(void)
{

}

ActorID Actor::getID(void) const
{
	return this->actorID;
}