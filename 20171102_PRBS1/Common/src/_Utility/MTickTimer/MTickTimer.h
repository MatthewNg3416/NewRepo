#ifndef MTICKTIMER_H
#define MTICKTIMER_H

const int DEF_TIMER_SUCCESS		= 0;		
const int ERR_TIMER_NOT_STARTED	= 1;
const int ERR_TIMER_ALREADY_STARTED = 2;

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

/**
 *  시스템에서 제공하는 GetTickCount() 함수를 이용하여 경과 시간을 계산한다.
 *  49.7일 이내에서만 사용이 가능하다. 
 */
class MTickTimer  
{
public:
	/* 생성자 */
	MTickTimer();

	/* 소멸자 */
	virtual ~MTickTimer();

private:
	/* Timer가 시작 되었는가 */
	BOOL	m_bIsStart;

	/* Timer Start시 Tick Value */
	DWORD	m_dwStart;

public:

	/** 
	 * Timer를 시작 한다.
	 *
	 * @return 0 : 성공, others : 에러
	 */
	int	StartTimer();

	/** 
	 * Timer를 종료 한다.
	 *
	 * @return 0 : 성공, others : 에러
	 */
	int	StopTimer();

	/** 
	 * 경과 시간을 리턴한다.
	 *
	 *@ return double : 경과 시간 ()
	 */
	double GetElapseTime();

	/** 
	 * 지정 시간을 초과 하지 않았는지 체크 한다.
	 *
	 * @param  dSec : 지정시간 (초단위)
	 * @return TRUE : 경과함
			   FALSE : 경과하지 않음
	 */
	BOOL LessThan(double dSec);

	/** 
	 * 지정 시간을 초과 했는지 체크 한다.
	 *
	 * @param  dSec : 지정시간 (초단위)
	 * @return TRUE : 경과하지 않음
			   FALSE : 경과함
	 */
	BOOL MoreThan(double dSec);
	
	/**
	 * Timer 시작 여부를 알려준다.	
	 */
	BOOL IsTimerStarted() const;

	DWORD GetStartTime();
};

#endif // MTICKTIMER_H
