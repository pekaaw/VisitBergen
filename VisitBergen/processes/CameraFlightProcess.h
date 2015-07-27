#pragma once

#include <chrono>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "..\processManager\Process.h"
#include "..\renderer\Camera.h"
#include "..\eventManager\EventListener.h"


class CameraFlightProcess : public Process, public EventListener
{
private:

	std::shared_ptr<Camera> camera;
	glm::vec4 tVector;
	glm::mat4 bezierMatrix;
	glm::mat4 pointMatrix;
	glm::mat4 targetPointMatrix;

	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

	bool isPaused;
	float oldT;

public:
	explicit CameraFlightProcess(unsigned long ms);
	~CameraFlightProcess(void){};

	virtual void init(void);
	virtual void update(unsigned long deltaMs);

	void handleEvent(const std::shared_ptr<Event>& event_);
};