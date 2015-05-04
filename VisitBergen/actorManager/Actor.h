#pragma once

#include <map>
#include <string>
#include <memory>

class Actor
{
	friend class ActorComponent;
	friend class ActorManager;

	int actorID;
	std::string actorName;
	std::map<std::string, std::shared_ptr<ActorComponent>> components;

private:
	bool addComponent(std::string componentType);
	std::shared_ptr<ActorComponent> getComponent(std::string componentType) const;

public:
	explicit Actor(const int actorID);
	const int getActorID() const;
	const std::string getActorName() const;
};