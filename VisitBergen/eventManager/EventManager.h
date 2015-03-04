#pragma once

#include<cstdio>

#include <memory>
#include <vector>
#include <map>
#include "type_info.h"
#include "Event.h"
#include "EventListener.h"

/**
 *	This is the EventManager that will receive events and listeners.
 *	Listeners will be notified when events are fired.
 */
class EventManager
{
public:
	EventManager();
	~EventManager();

	static std::shared_ptr<EventManager> getInstance();


	/**
	 * Make an instance start to listen for the given event type.
	 */
	template <class InstanceT, class EventT>
	void addListener(const std::shared_ptr<InstanceT>& instance, const std::shared_ptr<EventT>& eventType);

	/**
	 *	Remove a listener for the instance on a given event type
	 */
	template <class InstanceT, class EventT>
	void removeListener(const std::shared_ptr<InstanceT>& instance, const std::shared_ptr<EventT>& eventType);

	/**
	 *	Fire an event
	 */
	template <class EventT>
	void fireEvent(const std::shared_ptr<EventT>& event);

	/**
	 *	Updatemethod for the eventManager. Will notify all instances about events.
	 */
	void update();

private:
	static std::shared_ptr<EventManager> instance;

	typedef std::vector<std::shared_ptr<EventListener>> InstanceVector;

	std::vector<std::shared_ptr<Event>> events;
	std::map<TypeInfo, std::unique_ptr<InstanceVector>> listeners;
};


///////////////////////////////////////////////////////////////////////////////
///   Template class methods   ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <class InstanceT, class EventT>
void EventManager::addListener(const std::shared_ptr<InstanceT>& instance, const std::shared_ptr<EventT>& eventType)
{
	TypeInfo typeInfo(typeid(*eventType));

	auto typeIterator = this->listeners.find(typeInfo);

	if (typeIterator == this->listeners.end())
	{
		this->listeners[typeInfo] = std::make_unique<InstanceVector>();
		this->listeners[typeInfo]->push_back(instance);
	}
	else
	{
		auto& vector = typeIterator->second;

		auto instanceIterator = std::find( vector->begin(), vector->end(), instance);
		
		if (instanceIterator == this->listeners[typeInfo]->end())
		{
			this->listeners[typeInfo]->push_back(instance);
		}
	}
}

template <class InstanceT, class EventT>
void EventManager::removeListener(const std::shared_ptr<InstanceT>& instance, const std::shared_ptr<EventT>& eventType)
{
	TypeInfo eventTypeInfo(typeid(EventT));

	auto typeIterator = this->listeners.find(eventTypeInfo);

	if (typeIterator != this->listeners.end())
	{
		auto& vector = typeIterator->second;

		auto& instanceToRemove = std::find(vector->begin(), vector->end(), instance);

		if (instanceToRemove != vector->end())
		{
			vector->erase(instanceToRemove);
		}
	}
}

template <class EventT>
void EventManager::fireEvent(const std::shared_ptr<EventT>& event)
{
	this->events.push_back(event);
}