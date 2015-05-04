#pragma once

#include <map>
#include <memory>

#include <tinyxml2.h>

#include "..\interfaces\interfaces.h"

using namespace tinyxml2;

class Actor
{
	friend class ActorFactory;

	ActorID actorID;
	std::map<ComponentID, std::shared_ptr<ActorComponent>> actorComponents;

public:
	explicit Actor(ActorID id);
	~Actor(void);

	bool init(XMLElement* root);
	void postInit(void);
	void destroy(void);

	ActorID getID(void) const;

	template <class ComponentType>
	std::weak_ptr<ComponentType> getComponent(ComponentID id)
	{
		// Find component based on id
		auto it = this->actorComponents.find(id);

		// If found, return a weak_ptr to the object
		if (it != this->actorComponents.end())
		{
			// Get base ptr as ActorComponent
			std::shared_ptr<ActorComponent> base(it->second);

			// Cast it to ComponentType
			std::shared_ptr<ComponentType> sub(std::static_pointer_cast<ComponentType>(base));

			// Make it a weak_ptr
			std::weak_ptr<ComponentType> weakSub(sub);
			
			return weakSub;
		}
		// If not found, return an empty weak_ptr
		else
		{
			return std::weak_ptr<ComponentType>();
		}
	}

private:
	void addComponent(std::shared_ptr<ActorComponent> component);
};