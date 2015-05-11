#pragma once

#include <chrono>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "..\processManager\Process.h"
#include "..\renderer\Camera.h"


class CameraFlightProcess : public Process
{
private:

	std::shared_ptr<Camera> camera;
	glm::vec4 tVector;
	glm::mat4 bezierMatrix;
	glm::mat4 pointMatrix;

	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

public:
	explicit CameraFlightProcess(unsigned long ms);
	~CameraFlightProcess(void){};

	virtual void init(void);
	virtual void update(unsigned long deltaMs);
};