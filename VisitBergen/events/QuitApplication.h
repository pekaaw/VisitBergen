#pragma once

#include <cstdio>
#include "..\eventManager\Event.h"

class QuitApplication :
	public Event
{
public:
	QuitApplication(){};

	void sayHallo()
	{
		std::printf("Quit application!\n");
	}
};

