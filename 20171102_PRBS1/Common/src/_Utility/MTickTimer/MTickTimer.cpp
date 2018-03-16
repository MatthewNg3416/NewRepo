// MTickTimer.cpp: implementation of the TickTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTickTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MTickTimer::MTickTimer()
{
	m_bIsStart = FALSE;
	StartTimer();
}

MTickTimer::~MTickTimer()
{
}

/** 
 * Timer를 시작 한다.
 *
 * @return 0 : 성공, others : 에러
 */
int MTickTimer::StartTimer()
{
	m_dwStart = GetTickCount();
	m_bIsStart = TRUE;
	return DEF_TIMER_SUCCESS;
}

/** 
 * Timer를 시작 한다.
 *
 * @return 0 : 성공, others : 에러
 */
int MTickTimer::StopTimer()
{
	if (!m_bIsStart)
		return ERR_TIMER_NOT_STARTED;

	m_bIsStart = FALSE;
	return DEF_TIMER_SUCCESS;
}

/**  
 * 경과 시간을 리턴한다.
 *
 *@ return 양수 : 경과 시간 (초)
 */
double	MTickTimer::GetElapseTime()
{
	DWORD dwElasp;					// 경과 시간 
	DWORD dwStop = GetTickCount();	// Stop시의 값

	if (!m_bIsStart)
		return 0.0;

	if (m_dwStart <= dwStop)			//check for wrap around condition
	{
		dwElasp = dwStop - m_dwStart;	//no wrap
	}
	else
	{
		dwElasp = (0xFFFFFFFFL - m_dwStart); //calculate time from start up to wrap
		dwElasp += 1;   //time to wrap from FFFFFFFF to 0 is 1 tick
		dwElasp += dwStop; //add in time after  0
	}

	return (double)dwElasp/1000.;
}

/** 
 * 지정 시간을 초과 하지 않았는지 체크 한다.
 *
 * @param  dwMsec : 지정시간 msec
 * @return TRUE : 경과함
	       FALSE : 경과하지 않음
 */
BOOL MTickTimer::LessThan(double dSec)
{
	if (GetElapseTime() < dSec) return TRUE;
	else return FALSE;
}

/** 
 * 지정 시간을 초과 했는지 체크 한다.
 *
 * @param  dwMsec : 지정시간 msec
 * @return TRUE : 경과하지 않음
	       FALSE : 경과함
 */
BOOL MTickTimer::MoreThan(double dSec)
{
	if (GetElapseTime() > dSec) return TRUE;
	else return FALSE;
}

/**
 * Timer 시작 여부를 알려준다.	
 */
BOOL MTickTimer::IsTimerStarted() const
{
	return m_bIsStart;
}

DWORD MTickTimer::GetStartTime()
{
	if (!m_bIsStart)
		return 0.0;
	else
		return m_dwStart;
}
/* End Of Code */
