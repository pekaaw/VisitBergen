#include "EventListener.h"

EventListener::EventListener()
{

}

EventListener::~EventListener()
{

}

void EventListener::addEvent(const std::shared_ptr<Event>& event)
{
	this->events.push_back(event);
}


void EventListener::update()
{
	while (!this->events.empty())
	{
		this->handleEvent(this->events.front());
		this->events.pop_front();
	}
}