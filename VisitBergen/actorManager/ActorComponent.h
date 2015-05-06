#pragma once

#include <memory>
#include <string>
#include <tinyxml2.h>
#include "..\interfaces\interfaces.h"

class ActorComponent
{
	friend class ActorManager;

protected:
	std::shared_ptr<Actor> owner;

public:
	virtual bool dependancyInjection() = 0;
	virtual const char* getComponentName() = 0;
	virtual bool preInit(tinyxml2::XMLElement*) = 0;
	virtual void init() = 0;

	virtual ~ActorComponent(void){}
};
