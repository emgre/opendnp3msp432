#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <openpal/executor/IExecutor.h>
#include <openpal/container/Queue.h>
#include <openpal/container/LinkedList.h>

#include "Timer.h"

class Executor : public openpal::IExecutor
{
public:
	Executor(uint8_t maxQueueSize, uint8_t maxtimers);

	virtual openpal::MonotonicTimestamp GetTime();

	virtual openpal::ITimer* Start(const openpal::TimeDuration& duration, const openpal::action_t& action);
	virtual openpal::ITimer* Start(const openpal::MonotonicTimestamp& expiration, const openpal::action_t& action);
	virtual void Post(const openpal::action_t& action);
	void OnCancel(Timer* pTimer);

	void Init();
	void Run();
	void Tick();

private:
	bool RunOne();
	bool RunOneTimer();

	int64_t m_ticks;		

	openpal::Array<Timer, uint8_t> m_timers;
	openpal::Queue<openpal::action_t, uint8_t> m_work;

	openpal::Queue<Timer*, uint8_t> m_idleTimers;
	openpal::LinkedList<Timer*, uint8_t> m_activeTimers;
};

#endif // EXECUTOR_H
