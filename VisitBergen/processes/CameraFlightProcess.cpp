#include "CameraFlightProcess.h"

#include "..\interfaces\Locator.h"


CameraFlightProcess::CameraFlightProcess(unsigned long ms)
{
	this->startTime = std::chrono::high_resolution_clock::now();
	this->endTime = this->startTime + std::chrono::milliseconds(ms);

	this->tVector = glm::vec4();

	float bezierMatrixContent[16] = { 
		-1.0f, 3.0f, -3.0f, 1.0f, 
		3.0f, -6.0f, 3.0f, 0.0f, 
		-3.0f, 3.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 0.0f 
	};
	this->bezierMatrix = glm::make_mat4(bezierMatrixContent);

	//float pointMatrix[16] = {
	//	0.7f, 0.2f, 0.2f, 0.0f,
	//	0.0f, 0.4f, 0.8f, 0.0f,
	//	1.0f, 0.6f, 0.8f, 0.0f,
	//	0.3f, 0.8f, 0.2f, 0.0f
	//};
	
	float pointMatrix[16] = {
			-0.3f, 2.0f, -2.0f, 0.3f,			// x0, x1, x2, x3
			0.2f, 0.7f, 0.7f, 0.2f,			// y0, y1, y2, y3
			1.0f, 2.0f, 2.0f, 1.0f,			// z0, z1, z2, z3
			0.0f, 0.0f, 0.0f, 0.0f
		};
	this->pointMatrix = glm::make_mat4(pointMatrix);

}

void CameraFlightProcess::init()
{
	this->Process::init();

	this->camera = Locator::getRenderer()->getCamera();
}

void CameraFlightProcess::update(unsigned long deltaMs)
{
	std::chrono::time_point<std::chrono::high_resolution_clock> now = 
		std::chrono::high_resolution_clock::now();

	if (now >= this->endTime)
	{
		this->succeed();
		return;
	}

	auto lapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startTime);
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - this->startTime);

	float t = float(lapsed.count()) / time.count();

	//std::chrono::duration<std::chrono::milliseconds> lapsed = std::chrono::time_point_cast<std::chrono::milliseconds>(now) - this->startTime;

	//float t = (now - this->startTime) / (this->endTime - this->startTime);

	this->tVector = glm::vec4(t*t*t, t*t, t, 1);

	glm::vec4 tPoint = this->tVector * this->bezierMatrix * this->pointMatrix;
	glm::vec3 point = glm::vec3(tPoint.x, tPoint.y, tPoint.z);

	this->camera->setViewMatrix(point, glm::vec3());
}