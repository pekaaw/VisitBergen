#include "ActorManager.h"

#include "../components/RenderComponent.h"

int ActorManager::createActor(const std::string actorPath)
{

	XMLDocument actorXML;
	actorXML.LoadFile(actorPath.c_str());

	XMLElement* actorDescription = actorXML.RootElement();

	return createActor(actorDescription);
}

int ActorManager::createActor(XMLElement* actorDescriptionRoot)
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>(this->getNextActorID());

	std::shared_ptr<RenderComponent> renderer = std::make_shared<RenderComponent>();
	renderer->owner = actor;

	//actor->
	return 0;
}





const int ActorManager::getNextActorID()
{
	int id = this->nextActorID;
	this->nextActorID += 1;
	return id;
}