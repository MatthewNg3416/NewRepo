#ifndef MAxisData_H
#define MAxisData_H

#include "MFileManager.h"

#define MAX_AXIS	4	// X,Y,T,Z축
/**
 * 1. Panel 관련 Data를 파일에서 읽어서 Member Variable이 그 값을 갖고 있다.
 * 2. 필요한 Component는 각 Variable로 부터 그 값을 읽거나 쓰기 위해서 관련된 함수에서만 가능 하다.
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
	 /** Velocity Mode 설정 */
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

	/** Data를 File에서 읽어 오는 함수 (iAxisNo로 지정한  갯수 만큼 읽어온다.*/
    BOOL ReadData();
	/** Data를 File에 쓰는 오는 함수 */
    BOOL WriteData();
	/** 멤버 변수를 초기화 한다. */
	void Initialize();

	/** Section을 설정 한다 */
	void SetSection(CString strSection);

};
#endif //MAxisData_H
