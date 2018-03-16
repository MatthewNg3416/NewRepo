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
 *  �ý��ۿ��� �����ϴ� GetTickCount() �Լ��� �̿��Ͽ� ��� �ð��� ����Ѵ�.
 *  49.7�� �̳������� ����� �����ϴ�. 
 */
class MTickTimer  
{
public:
	/* ������ */
	MTickTimer();

	/* �Ҹ��� */
	virtual ~MTickTimer();

private:
	/* Timer�� ���� �Ǿ��°� */
	BOOL	m_bIsStart;

	/* Timer Start�� Tick Value */
	DWORD	m_dwStart;

public:

	/** 
	 * Timer�� ���� �Ѵ�.
	 *
	 * @return 0 : ����, others : ����
	 */
	int	StartTimer();

	/** 
	 * Timer�� ���� �Ѵ�.
	 *
	 * @return 0 : ����, others : ����
	 */
	int	StopTimer();

	/** 
	 * ��� �ð��� �����Ѵ�.
	 *
	 *@ return double : ��� �ð� ()
	 */
	double GetElapseTime();

	/** 
	 * ���� �ð��� �ʰ� ���� �ʾҴ��� üũ �Ѵ�.
	 *
	 * @param  dSec : �����ð� (�ʴ���)
	 * @return TRUE : �����
			   FALSE : ������� ����
	 */
	BOOL LessThan(double dSec);

	/** 
	 * ���� �ð��� �ʰ� �ߴ��� üũ �Ѵ�.
	 *
	 * @param  dSec : �����ð� (�ʴ���)
	 * @return TRUE : ������� ����
			   FALSE : �����
	 */
	BOOL MoreThan(double dSec);
	
	/**
	 * Timer ���� ���θ� �˷��ش�.	
	 */
	BOOL IsTimerStarted() const;

	DWORD GetStartTime();
};

#endif // MTICKTIMER_H
