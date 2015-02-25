#pragma once

#include <chrono>
#include "../processManager/Process.h"

class Timer : public Process
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

public:
	explicit Timer(unsigned long ms);
	~Timer();

protected:
	virtual void update(unsigned long deltaMs);
	virtual void pause(void);
	virtual void unPause(void);
};