#include "EventManager.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
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
