#include "Executor.h"
#include "Timer.h"
#include "driverlib.h"

using namespace openpal;

Executor* gpExecutor = nullptr;

void SYSTICK_ISR()
{
	if(gpExecutor)
	{
		gpExecutor->Tick();
	}
}

Executor::Executor(uint8_t maxQueueSize, uint8_t maxtimers)
:m_ticks(0), 	
m_timers(maxtimers),
m_work(maxQueueSize), 
m_idleTimers(maxtimers),
m_activeTimers(maxtimers)
{
	for(uint8_t i = 0; i < m_timers.Size(); ++i)
	{
		m_idleTimers.Enqueue(&m_timers[i]);
	}
}

MonotonicTimestamp Executor::GetTime()
{
	return MonotonicTimestamp(m_ticks);
}

ITimer* Executor::Start(const TimeDuration& duration, const action_t& action)
{
	return Start(GetTime().Add(duration), action);
}

ITimer* Executor::Start(const MonotonicTimestamp& expiration, const action_t& action)
{
	assert(m_idleTimers.IsNotEmpty());

	Timer** pTimer = m_idleTimers.Pop();
	(*pTimer)->Set(this, action, expiration);
	m_activeTimers.Add(*pTimer);
	return *pTimer;
}

void Executor::Post(const action_t& action)
{
	assert(!m_work.IsFull());

	m_work.Enqueue(action);
}

void Executor::OnCancel(Timer* pTimer)
{
	auto matches = [pTimer](Timer* pItem){ return pTimer == pItem; };
	m_activeTimers.RemoveFirst(matches);	
	m_idleTimers.Enqueue(pTimer);
}

void Executor::Init()
{
	gpExecutor = this;

	MAP_SysTick_enableModule();
	MAP_SysTick_setPeriod(1500); // 100 kHz
	MAP_SysTick_registerInterrupt(&SYSTICK_ISR);
	MAP_SysTick_enableInterrupt();
}

void Executor::Run()
{
	while(RunOne());
}

void Executor::Tick()
{
	++m_ticks;
}

bool Executor::RunOne()
{				
	if(RunOneTimer())
	{
		return true;
	}

	else
	{
		if(m_work.IsNotEmpty())
		{
			(*m_work.Peek())();
			m_work.Pop();
			return true;
		}

		else
		{
			return false;
		}
	}
}

bool Executor::RunOneTimer()

{	
	MonotonicTimestamp time = GetTime();
	auto expired = [time](Timer* pTimer) { return pTimer->ExpiresAt().milliseconds < time.milliseconds; };
	auto pNode = m_activeTimers.RemoveFirst(expired);

	if(pNode)
	{
		m_idleTimers.Enqueue(pNode->value);

		(pNode->value->GetAction())();
		return true;
	}

	else
	{
		return false;
	}
}