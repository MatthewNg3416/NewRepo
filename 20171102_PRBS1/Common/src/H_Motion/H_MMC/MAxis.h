/* 
 * Axis Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * CAxis.h : This file defines the class of single-axis.
 *				  이 파일은 단축 구성을 위한 Class를 정의한다.
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef MAxis_H
#define MAxis_H

#include "IMotionLib.h"
#include "IAxis.h"
#include "MAxisData.h"
#include "MTickTimer.h"

class IIO;

/**
 * This class is defined class of single-axis.
 * 이 Class는 단축 구성을 위한 Class로 정의한다.
 * 
 * 본 Class의 개체가 갖는 축의 개수와 축 개수만큼의 축 정보 영역을 기본으로 갖으며,
 * Motion Board를 사용하는 IMotionLib Interface를 포함하는 구조를 갖는다.
 * 본 Class의 기본 기능은 IAxis Interface에서 제공하는 Operation에 대한 실체 구현이며,
 * 모든 Motion Library API 함수들을 제공하지는 않는다.
 * 여기서 제공하는 기능들은 기존 장비사업팀에서 사용하던 함수들과 기존 Unit들이 갖는 축
 * 단위로 사용하는 수준의 함수들을 제공하며, Board 단위나 Unit간 다른 축을 묶어서 사용하는 
 * 기능 및 PC10 Option Board등의 기능들은 IMotionLob Interface에서 제공한다.
 * 그러므로 사용자들은 IAxis Interface를 사용하는 개체 외에 IMotionLib Interface를 사용하는
 * 개체를 동시에 생성하여 관리하는 것이 실제 사용에 용이해진다.
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */
class MAxis : public IAxis
{

private:	// attribute

    /** 1축 구성 */
	SAxis1				m_SaxAxis;

	/** IO Component */
	IIO*				m_plnkIO;

	/** Interlock IO Address */
	unsigned short		m_usiInterlockIOAddr;

	/** Interlock IO Check Value */
	BOOL				m_bInterlockJudge;

	MAxisData			m_AxisParameter;

	CString				m_strFileName;

    /**
     * Motion Library 개체
     * @link aggregation
     * @supplierCardinality 1
     */
    IMotionLib*			m_pMotionLib;
	
    /** 원점복귀 여부 설정(TRUE:원점복귀했음) */
	BOOL				m_bOriginFlag;

	/** 원점복귀 Step */
	int					m_iOriginStep;
	int					m_iOriginPrevStep;

	/** 원점복귀 Error Code */
	int					m_iOriginError;

	/** 원점복귀 이동 시간 Timer */
	MTickTimer			m_ttOriginTimer;

	/** Motion 이동 시간 Timer */
	MTickTimer			m_ttMovingTimer;

	/** 원점복귀 thread Handle */
	HANDLE				m_hOriginThread;
	
	/** **************************************************************
	 *	Simulation 관련 Mamber 변수
	 ** **************************************************************/

	// 확인 필요 : 나중에 이 변수들에다 모두 simul 접두어 붙일 것.
	/** Simulation 동작 시에 Motion 현재 위치 저장 */
	double				m_dCurrentAxisPos;

	/** Simulation 동작 시에 Motion 목표 위치 관리 */
	double				m_dTargetAxisPos;
	
	/** Simulation 동작 시에 Motion 동작 시간 Check Timer*/
	MTickTimer			m_MotionTimer;

	/** Simulation 동작 시에 R Motion 이동거리 */
	double				m_dRMoveDistance;

	/** Simulation 동작 시에 Motion 속도 */
	double				m_dVelocity;

	/** Simulation 동작 시에 Motion 가속도 */
	int					m_iAccelerate;

	/** Simulation 동작 시에 Motion 감속도 */
	int					m_iDecelerate;

	/** RMove 인지 아닌지 확인 */
	BOOL				m_bRMoveStart;

public:		// Constructor & Destructor

	/**
	 * Constructor : Clear data(default value), initialize inner object
	 *				 Data를 초기화하고, 내부 개체들을 초기화한다.
	 * @stereotype constructor
	 */
	MAxis();    

	/**
	 * Constructor : Set data, set inner object
	 *				 전달받은 인수로 Data를 설정하고, 내부 개체를 설정한다.
	 *
	 * @param	iObjectID		: Object ID
	 * @param	iBoardType		: Motion Board Type(1=MMC Board, 2=MEI board, 3=PPC Board, ...)
	 * @param	saxAxis		    : 1축 구성 정보
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log Level(None, Normal, Warning, Error)
	 * @param	iDays			: Log 유지 일 수
	 * @stereotype constructor
	 */
	MAxis(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO);    

	/**
	 * Destructor : Delete inner opbject
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	~MAxis();

public:		// interface operation

	/**  데이타를 설정 한다. */
	virtual int	SetData(SSMotionData* pData);

	/** 설정 데이타를 조회 한다. */
	virtual int	GetData(SSMotionData* pData);

	/**  파일에서 데이타를 로드 한다. */
	virtual int LoadParameter();

	/**  파일에 데이타를 저장한다.  */
	virtual int SaveParameter(MAxisData* pData);

	/** 축 Parameter(DataManager)를 얻는다. */
	void GetParameter(MAxisData* pData);

	/**
	 * 축의 이름을 Return 한다.
	 * @return : Axis Name
	 */
	CString GetAxisName();


	virtual int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis);

    virtual void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue);

	virtual int SetAxisData();

	virtual int GetAxisData(SAxis1* ax1Data);

	virtual int SetAutoCP(int iBoardNo, BOOL* pbAutoSet);

	virtual int GetAutoCP(int iBoardNo, BOOL* pbAutoSet);

	virtual int GetAxisID();

	virtual double GetHomeOffset();

	virtual double GetNegativeSWLimit();

	virtual double GetPositiveSWLimit();

	virtual double GetMovingVelocity();

	virtual double GetSlowMovingVelocity();	

	virtual int GetMovingAccelerate();

	virtual double GetCoarseVelocity();

	virtual int GetCoarseAccelerate();

	virtual double GetFineVelocity();

	virtual int GetFineAccelerate();	

	virtual double GetJogSlow();

	virtual double GetJogFast();	

	virtual BOOL GetSign();

	virtual BOOL IsAbsEncoder();

	virtual BOOL GetOriginDir();

	virtual BOOL GetOriginFineDir();

	virtual int GetCPhaseUseType();

	virtual double GetScale();

	virtual double GetMoveTime();

	virtual double GetMoveAfterTime();

	virtual double GetTolerance();

	virtual double GetPassOffset();

	virtual double GetOriginWaitTime();

	virtual BOOL IsOriginReturn();	

	virtual void SetOriginFlag();

	virtual int ResetOrigin(BOOL* pbReturn = NULL);

	virtual int StopReturnOrigin(void);

	virtual double GetCurrentPosition(BOOL bCommandType = FALSE);

	virtual int SetCurrentPosition(double dCurrentPosition, BOOL bType);

	virtual BOOL ComparePosition(BOOL bPosOpt, 
								  double dTargetPosition,
								  double* pdPermission = NULL,
								  double* pdDeviation = NULL);

	virtual BOOL IsLessThanTargetPosition(double dTargetPosition,
										   double* pdPermission = NULL);


	virtual BOOL IsGreaterThanTargetPosition(double dTargetPosition,
											  double* pdPermission = NULL);


	virtual int Move(double dPosition, 
					  double dVelocity = 0.0, 
					  int iAccelerate = 0, 
					  int iDecelerate = 0,
					  int iMoveType = DEF_MOVE_POSITION);

	virtual int StartMove(double dPosition, 
						   double dVelocity = 0.0, 
						   int iAccelerate = 0,
						   int iDecelerate = 0, 
						   int iMoveType = DEF_MOVE_POSITION);

	 int RMove(double dDistance, 
		        double dVelocity = 0.0, 
				int iAccelerate = 0, 
				int iDecelerate = 0,
				int iMoveType = DEF_SMOVE_DISTANCE, 
				BOOL bClearOpt = TRUE);

	 int StartRMove(double dDistance, 
					 double dVelocity = 0.0,
					 int iAccelerate = 0,
					 int iDecelerate = 0,
					 int iMoveType = DEF_SMOVE_DISTANCE, 
					 BOOL bClearOpt = TRUE);

	virtual int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE);

	virtual int Wait4Done(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE);

	virtual BOOL IsAxisDone();

	virtual BOOL IsInMotion();

	virtual BOOL IsMotionDone();

	virtual int GetComVelocity();

	virtual int MoveSplineLine(int iMaxPoint, 
							    double* pdPosition, 
								double* pdVelocity, 
								int* piAccelerate,
								BOOL bAutoFlag, 
								BOOL bWaitOpt = TRUE);

	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0);

	virtual int JogMoveFast(BOOL bDir, double dFast = 0.0);

	virtual int Stop(BOOL* pbStatus = NULL);

	virtual int VStop(BOOL* pbStatus = NULL);

	virtual int EStop(BOOL* pbStatus = NULL);

	virtual int ServoOn(BOOL* pbStatus = NULL, BOOL bIsCheck = FALSE);

	virtual int ServoOff(BOOL* pbStatus = NULL,  BOOL bIsCheck = FALSE);

	virtual BOOL CheckHomeSensor();

	virtual BOOL CheckPositiveSensor();

	virtual BOOL CheckNegativeSensor();

	virtual int SetHomeSensorEvent(int iLimit);

	virtual int SetHomeSensorLevel(BOOL bLevel);

	virtual int SetPositiveSensorEvent(int iLimit);

	virtual int SetPositiveSensorLevel(BOOL bLevel);

	virtual int SetNegativeSensorEvent(int iLimit);

	virtual int SetNegativeSensorLevel(BOOL bLevel);

	virtual int GetAxisSource();

	virtual int GetAxisState(int *piReturn);

	virtual BOOL GetAmpEnable();

	virtual int SetAmpEnable(BOOL bEnable);

	virtual BOOL GetAmpFault();

	virtual int ResetAmpFault(BOOL bIsCheck = FALSE);

	virtual int SetAmpFaultEnable();

	virtual int GetIndexRequired(BOOL* pbIndexReq);

	virtual int SetIndexRequired(BOOL bIndexReq);

	virtual int ClearAxis(BOOL* pbStatus = NULL);

	virtual int GetOriginPriority();

	virtual int OriginReturn(int iOriginMoveType);

	virtual int SetOriginStep(int iStep);

	virtual int GetOriginStep();

	virtual int ClearOriginError();

	virtual int GetOriginError(int* piError);

	virtual ERoleOfHome GetRoleOfHomeSensor();

	virtual void SetAxisPosReset();

	virtual int CheckAxisStateForJog(BOOL bDir);
	int JogStop();

private:	// internal operation

	int checkAxisParameter(SAxis1 srcAx);

	int checkAxisExistArgument(BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer, BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, void* pPointer2, BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, void* pPointer2, void* pPointer3, BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, 
								void* pPointer2, 
								void* pPointer3,
								void* pPointer4,
								BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, 
								void* pPointer2, 
								void* pPointer3, 
								void* pPointer4,
								void* pPointer5,
								BOOL bAxisExist);

	int checkBoardExistArgument(int iBoardID, void* pPointer, BOOL bBoardExist);

	int checkAxisState(int iAxisID);

	int	checkSWLimit(double dPosition);

	/**
	 * [Simulation] Move 동작시 현재 Pos에서 Target Pos까지 이동 시간을 계산한다. 
	 * 
	 * @return	 이동 시간 
	 */
	double calculateMotionTime();

	// Null Functions
	
	// 확인 필요 : 아래 함수 5개는 Sercos, MMC 에 같은 기능, 다른 이름으로 존재함. 정리 필요...
	virtual BOOL IsOnHomeSensor();
	virtual BOOL IsOnPositiveSensor();
	virtual BOOL IsOnNegativeSensor();
	virtual BOOL IsAmpEnable();
	virtual BOOL IsAmpFault();

	virtual BOOL IsEStopEvent();

	virtual int GetRingID();
	virtual int GetNodeID();
	virtual int ChangePhase(int Phase);
	virtual int NodeClear();


	/**
	* Stop(), EStop() 함수 실행시의 감속도를 설정.
	* 개별축의 Run 가속도 기준 1/5 로 설정.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID(MOTIONLIB)
	*							  그 외 = 타 함수 Return Error
	*/
	int SetEStopRate();

	int HomingStop();
	BOOL IsOriginComplete();
	void ResetOriginComplete();
	double DistanceToCurrentToHome();
	int SetHomingSwitchSource(int iSourceAxisNodeID);

	// Motor의 Scale을 변경할 수 있다.
	int SetMotorScale(double dScale);
};

#endif //MAxis_H

// End of File MAxis.h
