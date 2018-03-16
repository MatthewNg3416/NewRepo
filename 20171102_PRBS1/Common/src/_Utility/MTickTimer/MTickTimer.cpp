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
 * Timer�� ���� �Ѵ�.
 *
 * @return 0 : ����, others : ����
 */
int MTickTimer::StartTimer()
{
	m_dwStart = GetTickCount();
	m_bIsStart = TRUE;
	return DEF_TIMER_SUCCESS;
}

/** 
 * Timer�� ���� �Ѵ�.
 *
 * @return 0 : ����, others : ����
 */
int MTickTimer::StopTimer()
{
	if (!m_bIsStart)
		return ERR_TIMER_NOT_STARTED;

	m_bIsStart = FALSE;
	return DEF_TIMER_SUCCESS;
}

/**  
 * ��� �ð��� �����Ѵ�.
 *
 *@ return ��� : ��� �ð� (��)
 */
double	MTickTimer::GetElapseTime()
{
	DWORD dwElasp;					// ��� �ð� 
	DWORD dwStop = GetTickCount();	// Stop���� ��

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
 * ���� �ð��� �ʰ� ���� �ʾҴ��� üũ �Ѵ�.
 *
 * @param  dwMsec : �����ð� msec
 * @return TRUE : �����
	       FALSE : ������� ����
 */
BOOL MTickTimer::LessThan(double dSec)
{
	if (GetElapseTime() < dSec) return TRUE;
	else return FALSE;
}

/** 
 * ���� �ð��� �ʰ� �ߴ��� üũ �Ѵ�.
 *
 * @param  dwMsec : �����ð� msec
 * @return TRUE : ������� ����
	       FALSE : �����
 */
BOOL MTickTimer::MoreThan(double dSec)
{
	if (GetElapseTime() > dSec) return TRUE;
	else return FALSE;
}

/**
 * Timer ���� ���θ� �˷��ش�.	
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
