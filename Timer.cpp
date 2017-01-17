#include "Timer.h"
#include "Executor.h"

using namespace openpal;

Timer::Timer()
:m_pExecutor(nullptr),
m_expirationTime(MonotonicTimestamp::Min())
{

}

void Timer::Set(Executor* pExecutor, const openpal::action_t action, const openpal::MonotonicTimestamp& expirationTime)
{
	m_pExecutor = pExecutor;
	m_action = action;
	m_expirationTime = expirationTime;
}

void Timer::Cancel()
{
	if(m_pExecutor)
	{
		m_pExecutor->OnCancel(this);	
	}
}

MonotonicTimestamp Timer::ExpiresAt()
{
	return m_expirationTime;
}

action_t Timer::GetAction() const
{
    return m_action;
}