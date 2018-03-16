#ifndef MAxisData_H
#define MAxisData_H

#include "MFileManager.h"

#define MAX_AXIS	4	// X,Y,T,Z��
/**
 * 1. Panel ���� Data�� ���Ͽ��� �о Member Variable�� �� ���� ���� �ִ�.
 * 2. �ʿ��� Component�� �� Variable�� ���� �� ���� �аų� ���� ���ؼ� ���õ� �Լ������� ���� �ϴ�.
 */

typedef enum
{
	VELOCITY_MODE_NORMAL	= 0,
	VELOCITY_MODE_SLOW		= 1
} EVelocityMode;

class MAxisData : public MFileManager 
{
private:
	CString			m_strSection;
	static CString	m_strSectionCommon;
	
public:
	 /** Velocity Mode ���� */
	static	EVelocityMode  eVelocityMode;
	
	double	dCWSWLimit;
	double	dCCWSWLimit;
	double	dHomeFastVelocity;
	double	dHomeSlowVelocity;
	int		iHomeAccelerate;
	double	dHomeOffset;
	double	dJogSlow;
	double	dJogFast;
	int		iRunAccelerate;
	double	dRunVelocity;
	double	dSlowRunVelocity;
	double	dLimitTime;
	double	dOriginLimitTime;
	double	dStabilityTime;
	double	dTolerance;
	double	dPassOffset;
    
public:

	MAxisData(){};
	MAxisData(CString strSection, CString strFileName, CString strPath);
	MAxisData(CString strSection, CString strFileName);
	~MAxisData();

	/** Data�� File���� �о� ���� �Լ� (iAxisNo�� ������  ���� ��ŭ �о�´�.*/
    BOOL ReadData();
	/** Data�� File�� ���� ���� �Լ� */
    BOOL WriteData();
	/** ��� ������ �ʱ�ȭ �Ѵ�. */
	void Initialize();

	/** Section�� ���� �Ѵ� */
	void SetSection(CString strSection);

};
#endif //MAxisData_H
