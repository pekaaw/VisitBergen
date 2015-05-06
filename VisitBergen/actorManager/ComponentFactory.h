#pragma once

#include <memory>
#include <map>
#include <string>
#include "ActorComponent.h"

// the function that will create a component
template <class ComponentType>
std::shared_ptr<ActorComponent> createNewComponent(void)
{
	return std::make_shared<ComponentType>();
}

class ComponentFactory
{
	// function-ptr returning a shared ActorComponent ptr
	typedef std::shared_ptr<ActorComponent>(*ComponentConstructor)(void);

	// map name of component to constructor
	std::map<std::string, ComponentConstructor> constructorRegister;

public:

	// Register a component type by specifying the name of the component
	template <class ComponentType>
	void registerType(std::string type)
	{
		auto it = constructorRegister.find(type);
		if (it == constructorRegister.end())
		{
			// store pointer to a function that will return a new component
			constructorRegister[type] = &createNewComponent<ComponentType>;
		}
	}


	std::shared_ptr<ActorComponent> createComponent(std::string type)
	{
		auto it = constructorRegister.find(type);
		if (it != constructorRegister.end())
		{
			ComponentConstructor constructor = it->second;
			return constructor();
		}
		return nullptr;
	}

};