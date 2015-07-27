#include "CameraFlightProcess.h"

#include "..\interfaces\Locator.h"
#include "..\events\EventPauseTrip.h"


CameraFlightProcess::CameraFlightProcess(unsigned long ms) :
isPaused(false)
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
	
	//float pointMatrix[16] = {		// COLUMN MAJOR
	//	-0.3f, 5.0f, -5.0f, 0.15f,		// x0, x1, x2, x3
	//	-0.2f, 0.7f, -0.7f, 0.25f,			// y0, y1, y2, y3
	//	1.0f, -2.0f, -2.0f, 0.5f,			// z0, z1, z2, z3
	//	0.0f, 0.0f, 0.0f, 0.0f
	//};

	//float pointMatrix[16] = {		// COLUMN MAJOR
	//	-3.0f, 50.0f, -50.0f, 0.0f,		// x0, x1, x2, x3
	//	0.0f, 14.0f, 0.0f, 4.0f,			// y0, y1, y2, y3
	//	10.0f, -20.0f, -20.0f, 5.0f,			// z0, z1, z2, z3
	//	0.0f, 0.0f, 0.0f, 0.0f
	//};	

	float pointMatrix[16] = {		// COLUMN MAJOR
			0.0f, -200.0f, 100.0f, 0.0f,		// x0, x1, x2, x3
			14.0f, 30.0f, 30.0f, 14.0f,			// y0, y1, y2, y3
			-20.0f, 50.0f, 50.0f, -20.0f,			// z0, z1, z2, z3
			0.0f, 0.0f, 0.0f, 0.0f
		};
	this->pointMatrix = glm::make_mat4(pointMatrix);

	float target_point_matrix[16] = {		// COLUMN MAJOR
		0.0f, -15.0f, -30.0f, 0.0f,		// x0, x1, x2, x3
		0.0f, 0.0f, 0.0f, 0.0f,			// y0, y1, y2, y3
		00.0f, -78.0f, -10.0f, 0.0f,			// z0, z1, z2, z3
		0.0f, 0.0f, 0.0f, 0.0f
	};
	this->targetPointMatrix = glm::make_mat4(target_point_matrix);
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

	this->EventListener::update();

	if (this->isPaused)
	{
		return;
	}

	auto lapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->startTime);
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - this->startTime);

	float t = float(lapsed.count()) / time.count();
	this->oldT = t;

	//std::chrono::duration<std::chrono::milliseconds> lapsed = std::chrono::time_point_cast<std::chrono::milliseconds>(now) - this->startTime;

	//float t = (now - this->startTime) / (this->endTime - this->startTime);

	this->tVector = glm::vec4(t*t*t, t*t, t, 1);

	glm::vec4 tPoint = this->tVector * this->bezierMatrix * this->pointMatrix;
	glm::vec3 point = glm::vec3(tPoint.x, tPoint.y, tPoint.z);

	glm::vec4 targetPoint = this->tVector * this->bezierMatrix * this->targetPointMatrix;
	glm::vec3 target = glm::vec3(targetPoint.x, targetPoint.y, targetPoint.z);

	//point *= 10;

	this->camera->setViewMatrix(point, target);
	//this->camera->setViewMatrix(point, glm::vec3());
}

void CameraFlightProcess::handleEvent(const std::shared_ptr<Event>& event_)
{
	printf("CameraFlightProcess::handleEvent");
	if (std::shared_ptr<EventPauseTrip> pause = std::dynamic_pointer_cast<EventPauseTrip>(event_))
	{
		printf("do pause!");
		if (!this->isPaused)
		{
			this->isPaused = true;
			this->startTime = std::chrono::high_resolution_clock::now();
		}
		else
		{
			this->isPaused = false;
			auto duration = this->endTime - this->startTime;

			this->endTime = std::chrono::high_resolution_clock::now() +	duration;

			float done = ((1 - this->oldT) / this->oldT);
			int millis = duration.count();
			int before = millis * done;

			printf("\nmillis: %d \t before: %d\n", millis, before);
			auto tDone = done * duration;


			//std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> timeDone = std::chrono::duration_cast<std::chrono::milliseconds>(tDone);
			auto timeDone = std::chrono::duration_cast<std::chrono::milliseconds>(tDone);
			this->startTime = std::chrono::high_resolution_clock::now() - timeDone;
		}
	}

}