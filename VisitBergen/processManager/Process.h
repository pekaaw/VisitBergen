#pragma once
#include <memory>

class Process
{
	friend class ProcessManager;

public:
	enum State
	{
		UNINITIALIZED = 0,
			REMOVED,
			RUNNING,
			PAUSED,
			SUCCEEDED,
			FAILED,
			ABORTED
	};

private:
	State state;
	std::shared_ptr<Process> childProcess;

public:
	Process(void);
	virtual ~Process(void);

protected:
	// Execution methods to be implemented as necessary
	virtual void init(void) { this->state = RUNNING; }
	virtual void update(unsigned long deltaMs) = 0;
	virtual void onSuccess(void) {}
	virtual void onFail(void) {}
	virtual void onAbort(void) {}

public:
	// Methods for ending process
	void succeed(void);
	inline void fail(void);

	// Methods for pausing process
	inline void pause(void);
	inline void unPause(void);

	// Discern process state
	State getState(void) const { return this->state; }
	bool isAlive(void) const { return (this->state == RUNNING || this->state == PAUSED); }
	bool isDead(void) const { return (this->state == SUCCEEDED || this->state == FAILED || this->state == ABORTED); }
	bool isRemoved(void) const { return (this->state == REMOVED); }
	bool isPaused(void) const { return (this->state == PAUSED); }

	// Handle child process
	void attachChild(std::shared_ptr<Process> child);
	std::shared_ptr<Process> removeChild(void);
	std::shared_ptr<Process> peekChild(void) { return this->childProcess; }

private:
	// Change state
	void setState(State state) { this->state = state; }
};