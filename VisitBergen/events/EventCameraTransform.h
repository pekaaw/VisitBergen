#pragma once

#include <glm\glm.hpp>

#include "..\eventManager\Event.h"

class EventCameraTransform
	: public Event
{
	glm::mat4x4 transformation;

public:
	EventCameraTransform(){}
	~EventCameraTransform(){}

	void setTransform(const glm::mat4x4 &transform)
	{
		this->transformation = transform;
	}

	glm::mat4x4 const getTransform()
	{
		return this->transformation;
	}
};