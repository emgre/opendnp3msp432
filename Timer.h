#ifndef TIMER_H
#define TIMER_H

#include <openpal/executor/ITimer.h>
#include <openpal/executor/IExecutor.h>

class Executor;

class Timer : public openpal::ITimer
{
public:
	Timer();

	void Set(Executor* pExecutor, const openpal::action_t action, const openpal::MonotonicTimestamp& expirationTime);
	virtual void Cancel();
	virtual openpal::MonotonicTimestamp ExpiresAt();
    openpal::action_t GetAction() const;

private:
	Executor* m_pExecutor;
	openpal::action_t m_action;
	openpal::MonotonicTimestamp m_expirationTime;
};

#endif // TIMER_H
