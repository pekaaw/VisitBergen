#pragma once

#include <memory>
#include "..\interfaces\interfaces.h"

class ActorComponent
{
	friend class ActorManager;

protected:
	std::shared_ptr<Actor> owner;
	std::string componentType;

public:
	virtual bool dependancyInjection() = 0;
	virtual void init() = 0;

	virtual ~ActorComponent(void){}
};
