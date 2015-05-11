#include "ActorManager.h"

#include "../components/MeshComponent.h"
#include "../components/RenderComponent.h"
#include "../components/TransformComponent.h"

ActorManager::ActorManager() : 
	nextActorID(0),
	actorXML(nullptr)
{
	this->componentFactory.registerType<MeshComponent>(MeshComponent::componentName);
	this->componentFactory.registerType<RenderComponent>(RenderComponent::componentName);
	this->componentFactory.registerType<TransformComponent>(TransformComponent::componentName);
}

int ActorManager::createActor(const std::string actorPath)
{
	if (!this->actorXML)
	{
		this->actorXML = new XMLDocument();
	}
	else
	{
		assert("How did we come here?" || false);
		this->actorXML->Clear();
	}

	this->actorXML->LoadFile(actorPath.c_str());

	XMLElement* actorDescription = actorXML->RootElement();

	if (actorDescription == nullptr)
	{
		printf("Could not find resource: %s\n", actorPath);
	}

	return createActor(actorDescription);
}

int ActorManager::createActor(XMLElement* actorDescriptionRoot)
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>(this->getNextActorID());

	const char* type = actorDescriptionRoot->Attribute("type");
	if (!type)
	{
		printf("Actor type not specified: Abort actor creation!\n");
		return 0;
	}

	actor->setActorName(type);

	// Add components from xml (if it exists a constructor for it)
	for (XMLElement* node = actorDescriptionRoot->FirstChildElement(); node; node = node->NextSiblingElement())
	{
		std::string name = node->Value();
		std::shared_ptr<ActorComponent> component = this->componentFactory.createComponent(name);

		if (component)
		{
			// Add component and set ownership, if it successfully initializes
			if (component->preInit(node))
			{
				actor->addComponent(name, component);
				component->owner = actor;
			}
			else
			{
				printf("ActorComponent \"%s\" in \"%s\" failed to do pre-initialization, and will not be added.\n", name.c_str(), type);
			}
		}
		else
		{
			printf("ActorComponent \"%s\" not loaded in Actor \"%s\".\n", name.c_str(), type);
		}
	}

	// execute dependancy injection (let components get pointers to other components)
	for (auto& component : actor->components)
	{
		if (component.second->dependancyInjection() == false)
		{
			printf("Dependancy injection failed for %s. Actor creation aborted!\n", type);
			return 0;
		}
	}

	// now all components should be ready, so we can initialize them
	for (auto& component : actor->components)
	{
		component.second->init();
	}

	// now we are finished with the XMLDocument and can safely delete it
	delete this->actorXML;
	this->actorXML = nullptr;

	return 0;
}





const int ActorManager::getNextActorID()
{
	int id = this->nextActorID;
	this->nextActorID += 1;
	return id;
}