#include "ActorFactory.h"

ActorFactory::ActorFactory()
{
}

ActorFactory::~ActorFactory()
{
}

ActorID ActorFactory::getNextActorID(void)
{
	++this->nextActorID;
	return this->nextActorID;
}

std::shared_ptr<Actor> ActorFactory::createActor(const char* actorSpecification)
{
	XMLDocument document;
	XMLError status = document.LoadFile(actorSpecification);

	if (status != XML_SUCCESS)
	{
		printf("Could not load XML: %s \n", actorSpecification);
		return std::shared_ptr<Actor>();
	}

	XMLNode* root = document.FirstChild();

	if (root == nullptr)
	{
		printf("Failed to crate actor from resource: %s \n");
		return std::shared_ptr<Actor>();
	}

	std::shared_ptr<Actor> actor(new Actor(this->getNextActorID()));


	return actor;
}

std::shared_ptr<ActorComponent> ActorFactory::createComponent(XMLElement* componentName)
{
	return std::shared_ptr<ActorComponent>();
}
