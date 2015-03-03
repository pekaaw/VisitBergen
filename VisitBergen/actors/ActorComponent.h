#pragma once

#include <memory>

#include <tinyxml2.h>
using namespace tinyxml2;

#include "..\interfaces\interfaces.h"
#include "ActorFactory.h"

class ActorComponent
{
	friend class ActorFactory;

protected:
	std::shared_ptr<Actor> owner;

public:
	virtual ~ActorComponent(void){}

	virtual bool init(XMLElement* data) = 0;
	virtual void postInit(void) {};
	virtual void update(int deltaMs) {};
};