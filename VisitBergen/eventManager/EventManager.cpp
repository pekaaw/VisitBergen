#include "EventManager.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
	// release all the remaining events;
	for (auto it = this->events.begin(); it != this->events.end(); ++it)
	{
		it->reset();
	}

	// for all types of events
	for (auto it = this->listeners.begin(); it != this->listeners.end(); ++it)
	{
		// release all listeners
		for (auto itt = it->second->begin(); itt != it->second->end(); ++itt)
		{
			itt->reset();
		}
	}
}

void EventManager::update()
{
	std::shared_ptr<Event> event;

	while (!this->events.empty())
	{
		event = this->events.back();
		TypeInfo typeInfo(typeid(*event));

		auto& eventIt = this->listeners.find(typeInfo);

		if (eventIt != this->listeners.end())
		{
			auto& vector = *eventIt->second;
			for (auto& listener : vector)
			{
				listener->addEvent(event);
			}
		}

		this->events.pop_back();
	}
}

///////////////////////////////////////////////////////////////////////////////
///   EventManager Singleton Instance Getter //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::shared_ptr<EventManager> EventManager::instance = nullptr;
std::shared_ptr<EventManager> EventManager::getInstance()
{
	if (EventManager::instance == nullptr)
	{
		EventManager::instance = std::make_shared<EventManager>();
	}

	return EventManager::instance;
}
