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
 *				  �� ������ ���� ������ ���� Class�� �����Ѵ�.
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
 * �� Class�� ���� ������ ���� Class�� �����Ѵ�.
 * 
 * �� Class�� ��ü�� ���� ���� ������ �� ������ŭ�� �� ���� ������ �⺻���� ������,
 * Motion Board�� ����ϴ� IMotionLib Interface�� �����ϴ� ������ ���´�.
 * �� Class�� �⺻ ����� IAxis Interface���� �����ϴ� Operation�� ���� ��ü �����̸�,
 * ��� Motion Library API �Լ����� ���������� �ʴ´�.
 * ���⼭ �����ϴ� ��ɵ��� ���� ����������� ����ϴ� �Լ���� ���� Unit���� ���� ��
 * ������ ����ϴ� ������ �Լ����� �����ϸ�, Board ������ Unit�� �ٸ� ���� ��� ����ϴ� 
 * ��� �� PC10 Option Board���� ��ɵ��� IMotionLob Interface���� �����Ѵ�.
 * �׷��Ƿ� ����ڵ��� IAxis Interface�� ����ϴ� ��ü �ܿ� IMotionLib Interface�� ����ϴ�
 * ��ü�� ���ÿ� �����Ͽ� �����ϴ� ���� ���� ��뿡 ����������.
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */
class MAxis : public IAxis
{

private:	// attribute

    /** 1�� ���� */
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
     * Motion Library ��ü
     * @link aggregation
     * @supplierCardinality 1
     */
    IMotionLib*			m_pMotionLib;
	
    /** �������� ���� ����(TRUE:������������) */
	BOOL				m_bOriginFlag;

	/** �������� Step */
	int					m_iOriginStep;
	int					m_iOriginPrevStep;

	/** �������� Error Code */
	int					m_iOriginError;

	/** �������� �̵� �ð� Timer */
	MTickTimer			m_ttOriginTimer;

	/** Motion �̵� �ð� Timer */
	MTickTimer			m_ttMovingTimer;

	/** �������� thread Handle */
	HANDLE				m_hOriginThread;
	
	/** **************************************************************
	 *	Simulation ���� Mamber ����
	 ** **************************************************************/

	// Ȯ�� �ʿ� : ���߿� �� �����鿡�� ��� simul ���ξ� ���� ��.
	/** Simulation ���� �ÿ� Motion ���� ��ġ ���� */
	double				m_dCurrentAxisPos;

	/** Simulation ���� �ÿ� Motion ��ǥ ��ġ ���� */
	double				m_dTargetAxisPos;
	
	/** Simulation ���� �ÿ� Motion ���� �ð� Check Timer*/
	MTickTimer			m_MotionTimer;

	/** Simulation ���� �ÿ� R Motion �̵��Ÿ� */
	double				m_dRMoveDistance;

	/** Simulation ���� �ÿ� Motion �ӵ� */
	double				m_dVelocity;

	/** Simulation ���� �ÿ� Motion ���ӵ� */
	int					m_iAccelerate;

	/** Simulation ���� �ÿ� Motion ���ӵ� */
	int					m_iDecelerate;

	/** RMove ���� �ƴ��� Ȯ�� */
	BOOL				m_bRMoveStart;

public:		// Constructor & Destructor

	/**
	 * Constructor : Clear data(default value), initialize inner object
	 *				 Data�� �ʱ�ȭ�ϰ�, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */
	MAxis();    

	/**
	 * Constructor : Set data, set inner object
	 *				 ���޹��� �μ��� Data�� �����ϰ�, ���� ��ü�� �����Ѵ�.
	 *
	 * @param	iObjectID		: Object ID
	 * @param	iBoardType		: Motion Board Type(1=MMC Board, 2=MEI board, 3=PPC Board, ...)
	 * @param	saxAxis		    : 1�� ���� ����
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log Level(None, Normal, Warning, Error)
	 * @param	iDays			: Log ���� �� ��
	 * @stereotype constructor
	 */
	MAxis(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO);    

	/**
	 * Destructor : Delete inner opbject
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	~MAxis();

public:		// interface operation

	/**  ����Ÿ�� ���� �Ѵ�. */
	virtual int	SetData(SSMotionData* pData);

	/** ���� ����Ÿ�� ��ȸ �Ѵ�. */
	virtual int	GetData(SSMotionData* pData);

	/**  ���Ͽ��� ����Ÿ�� �ε� �Ѵ�. */
	virtual int LoadParameter();

	/**  ���Ͽ� ����Ÿ�� �����Ѵ�.  */
	virtual int SaveParameter(MAxisData* pData);

	/** �� Parameter(DataManager)�� ��´�. */
	void GetParameter(MAxisData* pData);

	/**
	 * ���� �̸��� Return �Ѵ�.
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
	 * [Simulation] Move ���۽� ���� Pos���� Target Pos���� �̵� �ð��� ����Ѵ�. 
	 * 
	 * @return	 �̵� �ð� 
	 */
	double calculateMotionTime();

	// Null Functions
	
	// Ȯ�� �ʿ� : �Ʒ� �Լ� 5���� Sercos, MMC �� ���� ���, �ٸ� �̸����� ������. ���� �ʿ�...
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
	* Stop(), EStop() �Լ� ������� ���ӵ��� ����.
	* �������� Run ���ӵ� ���� 1/5 �� ����.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID(MOTIONLIB)
	*							  �� �� = Ÿ �Լ� Return Error
	*/
	int SetEStopRate();

	int HomingStop();
	BOOL IsOriginComplete();
	void ResetOriginComplete();
	double DistanceToCurrentToHome();
	int SetHomingSwitchSource(int iSourceAxisNodeID);

	// Motor�� Scale�� ������ �� �ִ�.
	int SetMotorScale(double dScale);
};

#endif //MAxis_H

// End of File MAxis.h
