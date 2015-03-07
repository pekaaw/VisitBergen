#pragma once

#include <glm\glm.hpp>

#include "..\eventManager\EventListener.h"

class ArcBall :
	public EventListener
{
public:
	ArcBall(int width, int height);
	~ArcBall(){}

	void setDimensions(int width, int height);

private:
	void mapToSphere(int x, int y, glm::vec3* vector);

	glm::vec3 startVector;
	glm::vec3 endVector;

	float widthAdjustmentFactor;
	float heightAdjustmentFactor;

	void handleEvent(const std::shared_ptr<Event>& event_);
};