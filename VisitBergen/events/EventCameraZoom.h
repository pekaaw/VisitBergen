#pragma once

#include "..\eventManager\Event.h"

class EventCameraZoom :
	public Event
{
public:
	EventCameraZoom():zoom(0){}
	EventCameraZoom(double zoom):zoom(zoom){};
	~EventCameraZoom(){};

	double getZoom()
	{
		return this->zoom;
	}

private:
	double zoom;
};