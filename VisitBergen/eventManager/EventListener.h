/**
 * EventListener.h
 *
 *	Include in any class to enable the class to listen for events from the 
 *	EventManager. 
 *	
 *	Each class need a method "void handleEvent(std::shared_ptr<Event>)" to
 *	handle incoming events.
 */

#pragma once

#include <deque>
#include <memory>
#include "Event.h"
#include "type_info.h"


class EventListener
{
public:
	EventListener();
	~EventListener();

	/**
	* Add an event to this class (inherited deque). During update(), all events
	*	will go to handleEvent().
	*/
	void addEvent(const std::shared_ptr<Event>& event);

	/**
	*	Let the child run a function to handle events in the deque
	*	(all events are removed when handled).
	*/
	void update();

	/**
	 *	Pure virtual method that child classes should declare as public or
	 *	protected. It should manage all the events that the child class listens
	 *	to.
	 */
	virtual void handleEvent(const std::shared_ptr<Event>& event) = 0;

private:
	std::deque<std::shared_ptr<Event>> events;
};