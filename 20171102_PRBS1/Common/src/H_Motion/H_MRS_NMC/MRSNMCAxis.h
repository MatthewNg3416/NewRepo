/* 
 * Axis Component
 *
 * Copyright 2006 by Mechatronics & Manufacturing Technology Center, 
 * Samsung Electronics, Inc., All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MRSNMC.h : This file defines the class of single-axis and multi-axis.
 *				  이 파일은 단축 구성을 위한 Class를 정의한다.
 *
 * author Ji-Sung Kim(mickeykj.kim@samsung.com)
 * @version $Revision$
 */

#ifndef MRSNMC_H
#define MRSNMC_H

#include "IAxis.h"							// IAxis Interface Class Header File
#include "MTickTimer.h"						// MTickTimer Class Header File
#include "MAxisData.h"

#include "SMMCDEF.h"

class IIO;
class ISercosSetup;
class IMotionLibNMC;
class MRS_NMCSetup;

/**
 * This class is defined class of single-axis.
 * 이 Class는 단축 및 다축(Stage) 구성을 위한 Class로 정의한다.
 * 
 * 본 Class의 개체가 갖는 축의 개수와 축 개수만큼의 축 정보 영역을 기본으로 갖는다.
 * 본 Class의 기본 기능은 IAxis Interface에서 제공하는 Operation에 대한 실체 구현이며,
 * 모든 Motion Library API 함수들을 제공하지는 않는다.
 * 여기서 제공하는 기능들은 기존 장비기술연구팀에서 사용하던 함수들과 기존 Unit들이 갖는 축
 * 단위로 사용하는 수준의 함수들을 제공하며, Board 단위나 Unit간 다른 축을 묶어서 사용하는 
 * 기능도 제공한다.
 *
 * author Ji-Sung Kim(mickeykj.kim@samsung.com)
 * @version $Revision$
 */
class MRSNMC : public IAxis
{

private:	// attribute

    /** 1축 구성 */
	SAxis1				m_SaxAxis;

	/** IO Component */
	IIO*				m_plnkIO;
	ISercosSetup*		m_plnkSercosSetup;

	/** Interlock IO Address */
	unsigned short		m_usiInterlockIOAddr;

	/** Interlock IO Check Value */
	BOOL				m_bInterlockJudge;

	MAxisData			m_AxisParameter;

	CString				m_strFileName;

    /** 원점복귀 여부 설정(TRUE:원점복귀했음) */
	BOOL				m_bOriginFlag;
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	BOOL				m_bInitOrigin;
	// End.
	/**
     * Motion Library 개체
     * @link aggregation
     * @supplierCardinality 1
     */
    IMotionLibNMC*			m_pMotionLib;

	// Doolin kwangilshin 2017. 08. 27.
	//
	MRS_NMCSetup			*m_pRS_NMCSetup;
	// End.

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
	
	double				m_dCurrentAxisPos;

	double				m_dTargetAxisPos;
	
	/** Simulation 동작시에 Motion 동작 시간 Check Timer*/
	MTickTimer			m_MotionTimer;

	/** Simulation 동작 시에 R Motion 이동거리 */
	double				m_dRMoveDistance;

	/** Simulation 동작시에 R Motion 속도 */
	double				m_dVelocity;

	/** Simulation 동작시에 R Motion 가속도 */
	int					m_iAccelerate;

	/** Simulation 동작시에 R Motion 감속도 */
	int					m_iDecelerate;

	/** RMove 인지 아닌지 확인 */
	BOOL				m_bRMoveStart;

	/** 현재위치에서부터 홈센서 까지의 거리. */
	double				m_dToHomeDistance;

	/** ErrorSting */
//	char	m_cpErrString[ERROR_STRING_LENGTH];


public:		// Constructor & Destructor

	/**
	 * Constructor : Clear data(default value), initialize inner object
	 *				 Data를 초기화하고, 내부 개체들을 초기화한다.
	 * @stereotype constructor
	 */
	MRSNMC();    

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
	MRSNMC(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, ISercosSetup* pSercosSetup);    

	/**
	 * Destructor : Delete inner opbject
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	~MRSNMC();

public:		// interface operation

	/**
	 * NMC Library가 리턴하는 에러 코드를 System Error형태로 만든다
	 *
	 * @param	iError : Error 코드를 입력 
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	int makeSercosError(int iError);

	/**
	 * 데이타를 설정 한다. 
	 *
	 * @param	*pData : Data Address
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	virtual int	SetData(SSMotionData *pData);

	/**
	 * 설정 데이타를 조회한다. 
	 *
	 * @param	*pData : Data Address
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	virtual int	GetData(SSMotionData *pData);

	/**  파일에서 데이타를 로드 한다. */
	virtual int LoadParameter();

	/**  파일에 데이타를 저장한다.  */
	virtual int SaveParameter(MAxisData *pData);

	/** 축 Parameter(DataManager)를 얻는다. */
	void GetParameter(MAxisData* pData);

	/**
	 * 축의 이름을 Return 한다.
	 * @return : Axis Name
	 */
	CString GetAxisName();

	/**
	 * Single-axis Component를 초기화한다.
	 *
	 * 1. Motion Board 종류에 맞는 Motion Library 개체를 생성한다.
	 * 2. 축 구성 개수를 설정한다.
	 * 3. 축 정보를 설정한다.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXES NUMBER(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis);


	/**
	 * Motion 동작을 즉시 정지할 Interlock 조건에 해당되는 IO Address와 기준값을 설정한다.
	 *
	 * @param	usiIOAddress : 설정할 Interlock IO Address
	 * @param	bJudgeValue	: Interlock 판단할 값
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
    virtual void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue);


	/**
	 * 축 1개에 대한 Data를 설정한다.(구조체)
	 *
	 *	(1) 축 정보 영역이 있어야 한다.
	 *	(2) 축이 이미 사용되고 있으면 안된다.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  xx = USED AXIS ID(MULTIAXES)
	 *							  xx = NO EXIST AXIS PARAMETER AREA(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetAxisData();


    /**
	 * 축 1개에 대한 Data를 읽는다.(구조체)
	 *
	 * @param   *ax1Data        : 설정된 축의 설정 Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int GetAxisData(SAxis1 *ax1Data);


	/** 
	* 현재 장비의 Run Mode를 읽는다.
	*
	* @return	Error Code		: Axis ID
	*/
	virtual EVelocityMode GetVelocityMode();

	/** 
	 * 축 1개에 대한 Axis ID를 읽는다.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetAxisID();

	/** 
	 * 축 1개에 대한 Axis ID를 읽는다.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetRingID();

	/** 
	 * 축 1개에 대한 Axis ID를 읽는다.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetNodeID();

	/** 
	 * 축 1개에 대한 Home 위치(원점복귀위치)를 읽는다.
	 *
	 * @return  double		 : 읽은 dHomePosition	 
	 */
	virtual double GetHomeOffset();

	/** 
	 * 축 1개에 대한 Negative Limit 위치를 읽는다.
	 *
	 * @return   double     : 읽은 dNegativeLimit Position
	 */
	virtual double GetNegativeSWLimit();

	/** 
	 * 축 1개에 대한 Positive Limit 위치를 읽는다.
	 *
	 * @return   double     : 읽은 dPositiveLimit Position	 
	 */
	virtual double GetPositiveSWLimit();

	/** 
	 * 축 1개에 대한 Slow Moving속도(Manual Run Velocity)를 읽는다.
	 *
	* @return  double  : 읽은 dSlowMovingVelocity
	*/
	virtual double GetSlowMovingVelocity();
	
	/** 
	 * 축 1개에 대한 Moving속도, 가속도를 읽는다.
	 *
	 * @return  double  : 읽은 dMovingVelocity	 
	 */
	virtual double GetMovingVelocity();

	/** 
	 * 축 1개에 대한 Moving속도, 가속도를 읽는다.
	 *
	 * @return double  : 읽은 iMovingAccelerate	 
	 */
	virtual int GetMovingAccelerate();

	/** 
	 * 축 1개에 대한 Coarse속도, 가속도를 읽는다.
	 *
	 * return   double      : 읽은 dCoarseVelocity	 
	 */
	virtual double GetCoarseVelocity();
	/** 
	 * 축 1개에 대한 Coarse속도, 가속도를 읽는다.
	 *
	 * @return   int    : 읽은 iCoarseAccelerate	 
	 */
	virtual int GetCoarseAccelerate();

	/** 
	 * 축 1개에 대한 Fine속도, 가속도를 읽는다.
	 *
	 * @return  double     : 읽은 dFineVelocity	 
	 */
	virtual double GetFineVelocity();

	/** 
	 * 축 1개에 대한 Fine속도, 가속도를 읽는다.
	 *
	 * @return   double    : 읽은 iFineAccelerate	 
	 */
	virtual int GetFineAccelerate();	

	/** 
	 * 축 1개에 대한 Jog Move의 Slow, 속도를 읽는다.
	 *
	 * @return   double         : 읽은 dJogSlow	 
	 */
	virtual double GetJogSlow();
	
	/** 
	 * 축 1개에 대한 Jog Move의 Fast, 속도를 읽는다.
	 *
	 * @return   double     : 읽은 dJogFast	 
	 */
	virtual double GetJogFast();	

	/** 
	 * 축 1개에 대한 Sign을 읽는다.
	 *
	 * @return   BOOL          : 읽은 bSign	 
	 */
	virtual BOOL GetSign();

	/** 
	 * 축 1개에 대한 Abs Encoder 사용 여부를 읽는다.
	 *
	 * @return   BOOL          : 읽은 bAbsData
	 */
	virtual BOOL IsAbsEncoder();

	/** 
	 * 축 1개에 대한 원점복귀 진행방향을 읽는다.
	 *   Limit Sensor 구성에 따른 원점복귀 초기 진행방향을 읽을 수 있게 한다.
	 *
	 * @return   BOOL           : 설정할 bOriginDir(TRUE : +방향, FALSE : -방향)	
	 */
	virtual BOOL GetOriginDir();

	/** 
	 * 축 1개에 대한 원점복귀 진행(Fine구간)방향을 읽는다.
	 *   Fine 속도 구간에서 초기 진행방향을 읽을 수 있게 한다.
	 *
	 * @return   BOOL           : 설정할 bOriginDir(TRUE : +방향, FALSE : -방향)	 
	 */
	virtual BOOL GetOriginFineDir();

	/** 
	 * 축 1개에 대한 C상 사용 Type 을 읽는다.
	 *
	 * @param   int           : 읽은 C 상 사용 type
	 */
	virtual int GetCPhaseUseType();

	/** 
	 * 축 1개에 대한 Scale을 읽는다.
	 *
	 * @return   double         : 읽은 dScale	 
	 */
	virtual double GetScale();

	/** 
	 * 축 이동 시 지연 시간을 읽는다.
	 *
	 * @return   double          : 설정된 이동 지연 시간(초단위)	 
	 */
	virtual double GetMoveTime();

	/** 
	 * 축 이동 후 안정화 시간을 읽는다.
	 *
	 * @return   double          : 설정된 이동 후 안정화 시간(초단위)
	 */
	virtual double GetMoveAfterTime();

	/** 
	 * 축 위치 허용 오차를 읽는다.
	 *
	 * @return   double     : 설정된 위치 허용 오차(mm단위)	 
	 */
	virtual double GetTolerance();

	/** 
	 * 축 위치 허용 오차를 읽는다.
	 *
	 * @return   double    : 설정된 Offset(mm단위)
	 */
	virtual double GetPassOffset();

	/** 
	 * 축 원점복귀 완료 대기 시간(초)을 읽는다.
	 *
	 * @return   double          : 설정된 원점복귀 완료 대기 시간(초 단위)	 
	 */
	virtual double GetOriginWaitTime();


	/**
	 * 축이 원점복귀 됐는지 확인한다.(한개의 축에 대해 가능)
	 * 
	 * @return	: TRUE = 원점복귀 완료
	 *            FALSE = 원점복귀 미완료
	 */
	virtual BOOL IsOriginReturn();	

	virtual void SetOriginFlag();	

	/**
	 * 축 원점복귀 해제하기(한개의 축에 대해 가능)
	 * 
	 * @param   *pbReturn        :(OPTION = NULL) 원점복귀 결과 읽기
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ResetOrigin(BOOL *pbReturn = NULL);


	/**
	 * 축 원점복귀 강제 종료하기(구성된 모든 축에 대해 동작 정지 명령 수행)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int StopReturnOrigin(void);


	/**
	 * 축의 현재좌표를 읽는다.
	 * 
	 * @return   double   : 현재 좌표값
	 * @param   bCommandType         : 읽을 위치 종류, FALSE=실제위치(Encode), TRUE=목표위치(Command)     
	 */
	virtual double GetCurrentPosition(BOOL bCommandType = FALSE);


	/**
	 * 축의 현재좌표를 설정한다.
	 * 
	 * @param   dCurrentPosition     : 현재 좌표값
	 * @param   bType                : 읽을 위치 종류, FALSE=실제위치, TRUE=목표위치
	 * @return	Error Code		     : 0 = SUCCESS
	 *								  xx = INVALID AXIS ID(MULTIAXES)
	 *								  xx = INVALID POINTER(MULTIAXES)
	 *								  그 외 = 타 함수 Return Error
	 */
	virtual int SetCurrentPosition(double dCurrentPosition, BOOL bType = FALSE);
	void SetTargetPos(double dTargetPos) {
		m_dTargetAxisPos = dTargetPos;
	}

	/**
	 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
	 * 
	 * @param	bPosOpt              : 비교할 위치 종류, FALSE=현재위치, TRUE=Motion의 목표위치
	 * @param   dTargetPosition      : 비교할 좌표값	 
	 * @param   *pdPermission        :(OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
	 * @param   *pdDeviation         :(OPTION = NULL) 비교 차이값     
	 * @return  BOOL             : 비교결과
	 */
	virtual BOOL ComparePosition(BOOL bPosOpt, double dTargetPosition,
								 double *pdPermission = NULL, double *pdDeviation = NULL);


	/**
	 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
	 * 
	 * @param   dTargetPosition      : 비교할 좌표값	 
	 * @param   *pdPermission        :(OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
	 * @return  BOOL	             : 비교결과, 현재 좌표 + Tolerance 보다 작으면 True, 아니면 False
	 */
	virtual BOOL IsLessThanTargetPosition(double dTargetPosition,
								 double *pdPermission = NULL);


	/**
	 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
	 * 
	 * @param   dTargetPosition      : 비교할 좌표값	 
	 * @param   *pdPermission        :(OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
	 * @return  BOOL	             : 비교결과, 현재 좌표 - Tolerance 보다 크면 True, 아니면 False
	 */
	virtual BOOL IsGreaterThanTargetPosition(double dTargetPosition,
								 double *pdPermission = NULL);


	/**
	 * 축 이동(한개의 축에 대한 이동) - 이동 완료된 후 return
	 * 
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        :(OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      :(OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      :(OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	iMoveType        :(OPTION=1) 이동 Type,
	 *											0=사다리꼴 속도 Profile, 절대좌표 이동
	 *											1=S-Curve 속도 Profile, 절대좌표 이동
	 *											4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
	 *											5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Move(double dPosition, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
					  int iMoveType = DEF_MOVE_POSITION);


	/**
	 * 축 이동(한개의 축에 대한 이동) - 이동 명령 후 바로 return
	 * 
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        :(OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      :(OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      :(OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	iMoveType        :(OPTION=1) 이동 Type,
	 *											0=사다리꼴 속도 Profile, 절대좌표 이동
	 *											1=S-Curve 속도 Profile, 절대좌표 이동
	 *											4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
	 *											5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int StartMove(double dPosition, double dVelocity = 0.0, int iAccelerate = 0,
						   int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION);

	/**
	 * 축 이동(한개의 축에 대한 상대위치 이동) - 이동 완료된 후 return
	 * 
	 * @param   dDistance        : 이동할 거리
	 * @param   dVelocity        :(OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      :(OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      :(OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	iMoveType        :(OPTION=3) 이동 Type,
	 *											2=사다리꼴 속도 Profile, 상대거리 이동
	 *											3=S-Curve 속도 Profile, 상대거리 이동
	 *											6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
	 *											7=비대칭 S-Curve 속도 Profile, 상대거리 이동
	 * @param	bClearOpt        :(OPTION=FALSE) 이동 전과 후에 Encoder 값을 Clear하는 동작 사용 여부(TRUE:사용, FALSE:미사용)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	 int RMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
					   int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE);

 
    /**
	 * 축 이동(한개의 축에 대한 상대위치 이동) - 이동 명령 후 바로 return
	 * 
	 * @param   dDistance        : 이동할 거리
	 * @param   dVelocity        :(OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      :(OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      :(OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	iMoveType        :(OPTION=2) 이동 Type,
	 *											2=사다리꼴 속도 Profile, 상대거리 이동
	 *											3=S-Curve 속도 Profile, 상대거리 이동
	 *											6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
	 *											7=비대칭 S-Curve 속도 Profile, 상대거리 이동
	 * @param	bClearOpt        :(OPTION=FALSE) 이동 전과 후에 Encoder 값을 Clear하는 동작 사용 여부(TRUE:사용, FALSE:미사용)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	 int StartRMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
							int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE);


	/**
	 * 축 이동(한개의 축에 대한 등속 이동, 등속 위치까진 가속 이동함) 
	 * 
	 * @param   dVelocity        :(OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      :(OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용
	 * @param   bDir             :(OPTION=TRUE) 이동할 방향, TRUE:(+), FALSE:(-), 생략하면(+)방향으로 이동
	 *                             dVelocity에 값을 넣어주면 bDir은 생략해서 사용하면 된다.
	 *                             이 경우는 dVelocity의 부호에 의해 이동 방향이 결정된다.
	 *                             dVelocity에 0.0을 넣어 지정된 속도를 사용하는 경우는
	 *                             bDir로(+/-) 방향을 설정할 수 있다.
	 *                             만약, dVelocity에 값을 넣은 경우 bDir을 설정을 하게 되면
	 *                             지정된 dVelocuty, dAccelerate에 bDir이 반영되어 이동을 하게 된다.
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE);

	/**
	 * 축 이동 후 완료를 확인한다.(한개의 축에 대한 완료 확인) 
	 * 
	 * @param   bSkipMode        :(OPTION=FALSE) 위치 확인 대기, 이동 후 안정화 시간 지연 사용 여부
	 * @param	bUseInterlock    :(OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부(FALSE:사용안함, TRUE:사용함)
   	 * @param	bRMove		     :(OPTION=FALSE) RMove시 기능 사용 여부(FALSE:사용안함, TRUE:사용함)
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  xx = TIMEOUT MOVE-TIME(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Wait4Done(BOOL bSkipMode = FALSE,BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE);

	/**
	 * 축의 동작 완료 여부를 Return 한다.(정헌식 : 2005.01.11)
	 * 이동 및 속도 이동 완료 확인 또는 더불어 In-Position 여부까지 확인한다.
	 *
	 * MMC Library : axis_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsAxisDone();

	/**
	 *	축이 이동 중(속도 > 0) 인지를 Return 한다.
	 *
	 *	MMC Library : in_motion(axis)
	 *
	 *	@return : TRUE  = 축 이동 중.
	 *            FALSE = 축 정지 상태
	 */
	virtual BOOL IsInMotion();
	
	/**
	 * 축의 동작 완료 여부를 Return 한다.(정헌식 : 2005.01.11)
	 * 이동 및 속도 이동 완료 확인한다.  In-Position 확인안함
	 *
	 * MMC Library : motion_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsMotionDone();

	/**
	 * 직선보간 이동한다.
	 * 
	 * @param   iMaxPoint        : 직선보간 이동구간 개수
	 * @param   *pdPosition      : 직선보간 이동구간 지정, iMaxPoint만큼 설정
	 * @param   *pdVelocity      : 이동 시 속도, iMaxPoint만큼 설정(Scale은 적용되지 않는다.)
	 * @param   *piAccelerate    : 이동 시 가속도, iMaxPoint만큼 설정
	 * @param	bAutoFlag        : Auto CP 여부, FALSE=AutoCP 사용안함, TRUE=AutoCP 사용함
	 * @param	bWaitOpt         :(OPTION=TRUE)축 이동 완료 대기 여부, FALSE=대기 안함, TRUE=대기함
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  xx = NO EXIST AXIS ID TO MOVE(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int MoveSplineLine(int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE);

	/**
	 * Jog Slow에 의한 이동한다.
	 * 
	 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
	 * @param   dSlow           :(OPTION = 0.0) 이동할 거리, 0.0 = 지정된 Slow거리 사용
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0);

	/**
	 * Jog Fast에 의한 이동한다.
	 * 
	 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
	 * @param   dFast        :(OPTION = 0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int JogMoveFast(BOOL bDir, double dFast = 0.0);

	/**
	 * Jog 이동을 정지 시킨다.
	 * 	 
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  그 외 =  Error Code
	 */
	virtual int JogStop();

	/**
	 * 축을 정지한다.(한개의 축에 대한 정지)
	 * 
	 * @param   *pbStatus      :(OPTION = NULL) 축의 Stop 상태
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Stop(BOOL *pbStatus = NULL);


	/**
	 * 축을 등속이동에 대해 정지한다.(한개 축의 등속이동에 대한 정지)
	 * 
	 * @param   *pbState         :(OPTION = NULL) 각 축의 VStop 상태
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int VStop(BOOL *pbStatus = NULL);


	/**
	 * 축을 비상정지한다.(한개의 축에 대한 비상정지)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) 각 축의 EStop 상태
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int EStop(BOOL *pbStatus = NULL);


	/**
	 * 축의 Servo를 On 한다.(한개의 축에 대한 Servo On 수행)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) 각 축의 Servo ON 상태
	 * @param   *bLinearAxis        :(OPTION = FALSE) 축이 Linear Type이면 TRUE, 그외 FALSE
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ServoOn(BOOL *pbStatus = NULL,  BOOL bSkipSensor = FALSE);


	/**
	 * 축의 Servo를 Off 한다.(한개의 축에 대한 Servo Off 수행)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) 각 축의 Servo OFF 상태
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ServoOff(BOOL *pbStatus = NULL, BOOL bSkipSensor = FALSE);


	/**
	 * 축의 Home Sensor 상태를 읽는다.(한개의 축에 대한 상태읽기)
	 * 
	 * @return  BOOL        : 각 축의 Home Sensor 상태     
	 */
	virtual BOOL IsOnHomeSensor();


	/**
	 * 축의 Positive Sensor 상태를 읽는다.(한개의 축에 대한 상태읽기)
	 * 
	 * @return   BOOL        : 각 축의 Positive Sensor 상태     
	 */
	virtual BOOL IsOnPositiveSensor();


	/**
	 * 축의 Negative Sensor 상태를 읽는다.(한개에 대한 상태읽기)
	 * 
	 * @return   BOOL        : 각 축의 Negative Sensor 상태     
	 */
	virtual BOOL IsOnNegativeSensor();

	/**
	 * 축의 상태(Source)를 읽는다. 
	 * 
	 * @return   int       : 축 하나에 대한 상태(Source)     
	 */
//	virtual int GetAxisSource();
	virtual int GetAxisSource(int *piReturn); //170713 JSH


	/**
	 * 축의 상태(State)를 읽는다. 
	 * 
	 * @retunr  int         : 축 하나에 대한 상태(State)     
	 */
	//virtual int GetAxisState(int* piReturn);
	virtual int GetAxisState(UINT32* piReturn); //170713 JSH


	/**
	 * 축의 AMP Enable 상태를 읽는다. 
	 * 
	 * @param   BOOL        : 축에 대한 AMP상태(AMP Enable : TRUE, 그외 : FALSE)     
	 */
	virtual BOOL IsAmpEnable();

	/**
	 * 축의 AMP Enable/Disable 상태로 만든다.(정헌식 : 2005.01.11)
	 * 
	 * @param   bEnable          : AMP Enable 상태로 변경 : TRUE
	 *							   AMP Disable 상태로 변경 : FALSE
	 * @param   bLinearAxis        :(OPTION = FALSE) 축이 Linear Type이면 TRUE, 그외 FALSE
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetAmpEnable(BOOL bEnable);


	/**
	 * 축의 AMP Fault 상태를 읽는다. 
	 * 
	 * @return   BOOL         : 축에 대한 AMP Fault상태(AMP Fault:TRUE, Fault아니면:FALSE)
	 */
	virtual BOOL IsAmpFault();

	/**
	 * 축의 AMP Fault 상태를 Reset 한다.(정헌식 : 2005.01.11)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ResetAmpFault(BOOL bIsCheck = FALSE);


	/**
	 * 축의 AMP Fault 상태를 Enable 한다. (정헌식 수정 : 2005.01.11)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetAmpFaultEnable();

	/**
	 * 축의 상태를 초기화 한다.(한개의 축에 대해 초기화)
	 *  Clear Status & Clear Frames
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) 각 축의 초기화 상태
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ClearAxis(BOOL *pbStatus = NULL);

	/** 
	 * 원점복귀 우선 순위를 Return 한다.
	 *
	 * @return	우선 순위		: 0~63
	 */
	virtual int GetOriginPriority();

	/** 
	 * 원점복귀 동작 수행 함수(한번 호출에 한 Step씩 수행)
	 * 원점 복귀 Step이 완료 Step이 될 때 까지 계속 호출 해야함
	 *
	 * @param   iOriginMoveType :	DEF_ORIGIN_RETURN_NONE_MOVE(0) -> Motion 동작 없이 현재 위치를 원점으로 설정하는 경우.
	 *								DEF_ORIGIN_RETURN_ALL_MOVE	(1) -> 원점 복귀 Offset 이동 동작을 포함한 모든 원점 복귀 관련 Motion 동작을 수행하는 경우.
	 *								DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE	(2) -> 원점 복귀 Offset 이동 동작을 제외한 원점 복귀 관련 Motion 동작을 수행하는 경우.
	 * @return	Origin Step		: 1000 = ORIGIN RETURN SUCCESS
	 *							  990 = ERROR
	 *							  xx = 원점 복귀 중...
	 */
	virtual int OriginReturn(int iOriginMoveType);

	//100712.KKY_____
	virtual int OriginReturnByUser();
	//_______________

	/**
	 * 원점복귀 Step을 설정한다.(한개의 축)
	 * 
	 * @param   iStep            : 설정값(0:시작, 999:오류, 1000:완료, 그외:동작중)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID ORIGIN STEP(<0)(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetOriginStep(int iStep);


	/**
	 * 원점복귀 Step을 읽는다.(한개의 축)
	 * 
	 * @return	Origin Step
	 */
	virtual int GetOriginStep();


	/**
	 * 원점복귀 Error를 초기화한다.(한개의 축)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ClearOriginError();


	/**
	 * 원점복귀 Error를 읽는다.(한개의 축)
	 * 
 	 * @param   *piError         : 발생한 오류 Code
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int GetOriginError(int *piError);
	
	/**
	 * Home Sensor의 역할을 반환 한다.
	 *  	 
     * @return	ERoleOfHome		HOME_N_NONE : Home Sensor 역할
								HOME_N_POSITIVE_LIMIT : Home, Positive Sensor 역할
								HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor 역할
	 */
	virtual ERoleOfHome GetRoleOfHomeSensor();
	

	virtual BOOL IsEStopEvent();

	/*
	*
	*
	*/
	virtual int ChangePhase(int Phase);


	/*
	*
	*
	*/
	virtual int NodeClear();

	/**
	 * 이동할 위치가 SW Limit을 벗어나는지 확인한다.
	 * 
	 * @param   dPosition		: 검사할 위치
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = OVER SW POSITIVE LIMIT(MULTIAXES)
	 *							  xx = OVER SW NEGATIVE LIMIT(MULTIAXES)
	 */
	int	checkSWLimit(double dPosition);

	/**
	 * 원점복귀된 축에 대해서 Event가 발생한 경우에 대해 점검 및 조치를 한다.
	 * 
	 * @param   iAxisID			: 축 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = HOME SWITCH AXIS SOURCE(MULTIAXES)
	 *							  xx = POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP FAULT AXIS SOURCE(MULTIAXES)
	 *							  xx = ACCELERATE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = VELOCITY LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = ERROR LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = PC COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = OUT OF FRAMES AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP POWER ON OFF AXIS SOURCE(MULTIAXES)
	 *							  xx = RUN STOP COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = COLLISION STATE AXIS SOURCE(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	int checkAxisState(int iAxisID);

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

	void SetAxisPosReset();

	virtual int SetAutoCP(int iBoardNo, BOOL* pbAutoSet);

	virtual int GetAutoCP(int iBoardNo, BOOL* pbAutoSet);

	virtual int GetComVelocity();

	virtual int SetHomeSensorEvent(int iLimit);

	virtual int SetHomeSensorLevel(BOOL bLevel);

	virtual int SetPositiveSensorEvent(int iLimit);

	virtual int SetPositiveSensorLevel(BOOL bLevel);

	virtual int SetNegativeSensorEvent(int iLimit);

	virtual int SetNegativeSensorLevel(BOOL bLevel);

	virtual int GetIndexRequired(BOOL* pbIndexReq);

	virtual int SetIndexRequired(BOOL bIndexReq);

	/**
	 * Jog동작중 Event가 발생한 경우에 대해 점검 및 조치를 한다.
	 * 
	 * @param   iAxisID			: 축 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP FAULT AXIS SOURCE(MULTIAXES)
	 *							  xx = ACCELERATE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = VELOCITY LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = ERROR LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = PC COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = OUT OF FRAMES AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP POWER ON OFF AXIS SOURCE(MULTIAXES)
	 *							  xx = RUN STOP COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = COLLISION STATE AXIS SOURCE(MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	int CheckAxisStateForJog(BOOL bDir);
	
	// Control 전원이 떨어진적이 있거나, 외부에서 Set_position한적 있는지 여부 TRUE: 원점복귀 완료. FALSE : 원점 복귀 미 완료.
	BOOL IsOriginComplete();

	void ResetOriginComplete();

	double DistanceToCurrentToHome();

	int SetHomingSwitchSource(int iSourceAxisNodeID);

	int ClearFrames();

	//100712.KKY_____
	virtual int GetHomeSensorLevel(BOOL *bLevel);
	virtual int GetPositiveSensorLevel(BOOL *bLevel);
	virtual int GetNegativeSensorLevel(BOOL *bLevel);
	//_______________


private:	// internal operation

	/**
	 * SAxis1 Data에 대한 오류 점검 수행한다.
	 *
	 * @param	srcAx			: 오류 점검할 원본 Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MULTIAXES)
	 *							  xx = INVALID MOTION AXIS ID(MULTIAXES)
	 *							  xx = INVALID MOVE-PRIORITY NUMBER(MULTIAXES)
	 *							  xx = INVALID ORIGIN-PRIORITY NUMBER(MULTIAXES)
	 */
	int checkAxisParameter(SAxis1 srcAx);

	/**
	 * Move 동작시 현재 Pos에서 Target Pos까지 이동 시간을 계산한다. 
	 * 
	 * @return	 이동 시간 
	 */
	double calculateMotionTime();

	/**
	 * RMove 동작시 현재 Pos에서 Target Pos까지 이동 시간을 계산한다. 
	 * 
	 * @return	 이동 시간 
	 */
	
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

	// Motor의 Scale을 변경할 수 있다.
	int SetMotorScale(double dScale);	
};

#endif //MRSNMC_H

// End of File MRSNMC.h
