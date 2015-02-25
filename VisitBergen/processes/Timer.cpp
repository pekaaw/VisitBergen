#include "Timer.h"

/** constructor
 *
 * Set startTime as now() and calculate endTime from specified pause length.
 */
Timer::Timer(unsigned long ms)
{
	//printf("Timer constructor...\n");
	this->startTime = std::chrono::high_resolution_clock::now();
	this->endTime = this->startTime + std::chrono::milliseconds(ms);
}

Timer::~Timer()
{}

/** update(deltaMs)
 *
 * Set as success when time is finished.
 */
void Timer::update(unsigned long deltaMs)
{
	if (std::chrono::high_resolution_clock::now() >= this->endTime)
	{
		this->succeed();
	}
}

/** pause(void)
 *
 * Set the time in pause mode. By storing now() in startTime, we will be able
 * to calculate remaining time upon unpause.
 */
void Timer::pause(void)
{
	this->startTime = std::chrono::high_resolution_clock::now();
}

/** unPause(void)
 *
 * Calculate remaining time and start the timer over with it
 */
void Timer::unPause(void)
{
	// calculate how much remains and set endTime to so much into the future.
	this->endTime = std::chrono::high_resolution_clock::now() +
		(this->endTime - this->startTime);

	// set the startTime to now
	this->startTime = std::chrono::high_resolution_clock::now();
}