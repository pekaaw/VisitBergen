#pragma once
#include "..\processManager\Process.h"
#include "..\eventManager\EventListener.h"

class Renderer :
	public Process,
	public EventListener
{
public:
	Renderer();
	~Renderer();

	virtual void init(void);
	virtual void update(unsigned long deltaMs);
	virtual void handleEvent(const std::shared_ptr<Event>& event);
};

