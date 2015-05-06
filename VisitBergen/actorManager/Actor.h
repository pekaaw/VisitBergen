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

protected:
	bool addComponent(std::string componentType, std::shared_ptr<ActorComponent> component);
	void setActorName(std::string name);

public:
	explicit Actor(const int actorID);
	const int getActorID() const;
	const std::string getActorName() const;
	std::shared_ptr<ActorComponent> getComponent(std::string componentType) const;
};