
#ifndef MCANAxis_H
#define MCANAxis_H

#include "IAxis.h"
#include "MAxisData.h"
#include "MTickTimer.h"
#include "DefMotionStruct.h"
#include "MCANSetup.h"
#include "CML_Amp.h"

CML_NAMESPACE_USE();

class IIO;

class MCANAxis : public IAxis
{
private:
	/** Critical Section */
	CRITICAL_SECTION m_csProcess;

	int				m_canNodeID;

	Amp				m_amp;
//	CanOpen			m_canOpen;
//	CopleyCAN*		m_can;

//	static BOOL		m_rgbACSAxisUseFlag;

	SAxis1			m_SaxAxis;
	MAxisData		m_AxisParameter;

//	SACSCommSet		m_sCommSet;

	IIO*			m_plnkIO;

	unsigned short	m_usiInterlockIOAddr;
	BOOL			m_bInterlockJudge;

	CString			m_strFileName;
	
	BOOL			m_bOriginFlag;

	int				m_iOriginStep;
	int				m_iOriginPrevStep;
	int				m_iOriginError;

	MTickTimer		m_ttOriginTimer;
	MTickTimer		m_ttMovingTimer;

	// 원점 복귀 중
	BOOL			m_bOnOriginReturn;

	/** **************************************************************
	 *	Simulation 관련 Mamber 변수
	 ** **************************************************************/
	// 확인 필요 : 나중에 이 변수들에다 모두 simul 접두어 붙일 것.
	/** Simulation 동작 시에 Motion 현재 위치 저장 */
	double			m_dCurrentAxisPos;

	/** Simulation 동작 시에 Motion 목표 위치 관리 */
	double			m_dTargetAxisPos;
	
	/** Simulation 동작 시에 Motion 동작 시간 Check Timer*/
	MTickTimer		m_MotionTimer;

	/** Simulation 동작 시에 R Motion 이동거리 */
	double			m_dRMoveDistance;

	/** Simulation 동작 시에 Motion 속도 */
	double			m_dVelocity;

	/** Simulation 동작 시에 Motion 가속도 */
	int				m_iAccelerate;

	/** Simulation 동작 시에 Motion 감속도 */
	int				m_iDecelerate;

	/** RMove 인지 아닌지 확인 */
	BOOL			m_bRMoveStart;

	int		setUseAxis(BOOL bState);
	BOOL	getUseAxis();

public:
	MCANAxis();
	MCANAxis(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, int canNodeID, MCANSetup* pCANSetup);
	~MCANAxis();

	int	SetData(SSMotionData *pData);
	int	GetData(SSMotionData *pData);

	int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis);

	int LoadParameter();
	int SaveParameter(MAxisData *pData);
	void GetParameter(MAxisData *pData);



	int GetAxisData(SAxis1 *ax1Data);
	int SetAxisConfig(SMotionAxis sMotionAxis);
	int GetAxisConfig(SMotionAxis* psMotionAxis);


	int Move(double dPosition, double dVelocity = 0.0, int iAccelerate = 0,
			 int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION);
	int StartMove(double dPosition, double dVelocity = 0.0, int iAccelerate = 0,
				  int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION);
	int RMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0,
			  int iDecelerate = 0, int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE);
	int StartRMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0,
				   int iDecelerate = 0, int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE);
	int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE);
	int Wait4Done(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE);
	int MoveSplineLine(int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE);
	int JogMoveSlow(BOOL bDir, double dSlow = 0.0);
	int JogMoveFast(BOOL bDir, double dFast = 0.0);
	int JogStop();
	int Stop(BOOL *pbStatus = NULL);
	int VStop(BOOL *pbStatus = NULL);
	int EStop(BOOL *pbStatus = NULL);
	int ClearAxis(BOOL *pbStatus = NULL);
	int SetStopRate(short siRate);
	int SetEStopRate(short siRate);
	int SetEncoderDirection(BOOL bEncDir, BOOL bCoorDir);


	int GetAxisSource();
	int GetAxisState(int* piReturn);
	BOOL IsAxisDone();
	BOOL IsInMotion();
	BOOL IsMotionDone();
	int GetComVelocity();
	BOOL IsEStopEvent();


	double GetCurrentPosition(BOOL bCommandType = FALSE);
	int SetCurrentPosition(double dCurrentPosition, BOOL bType);
	BOOL ComparePosition(BOOL bPosOpt, double dTargetPosition,
								 double *pdPermission = NULL, double *pdDeviation = NULL);
	BOOL IsLessThanTargetPosition(double dTargetPosition,
								 double *pdPermission = NULL);
	BOOL IsGreaterThanTargetPosition(double dTargetPosition,
								 double *pdPermission = NULL);


	BOOL IsAmpEnable();
	int SetAmpEnable(BOOL bEnable);
	int SetAmpEnableLevel(BOOL bLevel);
	BOOL IsAmpFault();
	int ResetAmpFault(BOOL bIsCheck = FALSE);
	int SetAmpResetLevel(BOOL bLevel);
	int SetAmpFaultEnable();
	int SetAmpFaultLevel(BOOL bLevel);
	int ServoOn(BOOL *pbStatus = NULL,  BOOL bIsCheck = FALSE);
	int ServoOff(BOOL *pbStatus = NULL, BOOL bIsCheck = FALSE);


	CString GetAxisName();
    void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue);
	int SetInPositionData();
	int GetAxisID();
	double GetHomeOffset();
	double GetNegativeSWLimit();
	double GetPositiveSWLimit();
	double GetMovingVelocity();
	double GetSlowMovingVelocity();	

	int GetMovingAccelerate();
	double GetCoarseVelocity();
	int GetCoarseAccelerate();
	double GetFineVelocity();
	int GetFineAccelerate();
	double GetJogSlow();
	double GetJogFast();
	BOOL GetSign();
	BOOL IsAbsEncoder();
	BOOL GetOriginDir();
	BOOL GetOriginFineDir();
	int GetCPhaseUseType();
	double GetScale();
	double GetMoveTime();
	double GetMoveAfterTime();
	double GetTolerance();
	double GetPassOffset();
	ERoleOfHome GetRoleOfHomeSensor();


	double GetOriginWaitTime();
	BOOL IsOriginReturn();
	void SetOriginFlag();

	int ResetOrigin(BOOL *pbReturn = NULL);
	int GetIndexRequired(BOOL *pbIndexReq);
	int SetIndexRequired(BOOL bIndexReq);
	int GetOriginPriority();
	int OriginReturn(int iOriginMoveType);
	int StopReturnOrigin(void);
	int SetOriginStep(int iStep);
	int GetOriginStep();
	int ClearOriginError();
	int GetOriginError(int *piError);	


	BOOL IsOnHomeSensor();
	BOOL IsOnPositiveSensor();
	BOOL IsOnNegativeSensor();
	int SetHomeSensorEvent(int iLimit);
	int SetHomeSensorLevel(BOOL bLevel);
	int SetPositiveSensorEvent(int iLimit);
	int SetPositiveSensorLevel(BOOL bLevel);
	int SetNegativeSensorEvent(int iLimit);
	int SetNegativeSensorLevel(BOOL bLevel);

	int SetAxisData();
	int SetAutoCP(int iBoardNo, BOOL* pbAutoSet);
	int GetAutoCP(int iBoardNo, BOOL* pbAutoSet);
	int GetRingID();
	int GetNodeID();
	BOOL CheckHomeSensor();
	BOOL CheckPositiveSensor();
	BOOL CheckNegativeSensor();
	BOOL GetAmpEnable();
	BOOL GetAmpFault();
	int checkAxisState(int iAxisID);
	int ChangePhase(int Phase);
	int NodeClear();
	void SetAxisPosReset();
	int CheckAxisStateForJog(BOOL bDir);


private:

	int checkAxisParameter(SAxis1 srcAx);
	int checkAxisState();
	int	checkSWLimit(double dPosition);

	/**
	 * [Simulation] Move 동작시 현재 Pos에서 Target Pos까지 이동 시간을 계산한다. 
	 * 
	 * @return	 이동 시간 
	 */
	double calculateMotionTime();
	int SetEStopRate();

	int HomingStop();
	BOOL IsOriginComplete();
	void ResetOriginComplete();
	double DistanceToCurrentToHome();
	int SetHomingSwitchSource(int iSourceAxisNodeID);

	// Motor의 Scale을 변경할 수 있다.
	int SetMotorScale(double dScale);

	/**
	 * Copley VCM Library가 리턴하는 에러 코드를 System Error형태로 만든다
	 *
	 * @param	iError : Error 코드를 입력 
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	int makeCanVcmError(int iError);

};
#endif //MCANAxis_H
