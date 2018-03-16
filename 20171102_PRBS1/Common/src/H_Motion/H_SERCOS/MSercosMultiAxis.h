/* 
 * Motion Component
 *
 * Copyright 2006 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef MSercosMultiAxis_H
#define MSercosMultiAxis_H

#include "ISercosMultiAxis.h"		

/**
 * MSercosMultiAxis.h : MPI를 이용한 다축제어 Class 
 *
 * Single Axis의 Handle을 이용하여 MultiMotion을 구성한다..
 *
 */

class MSercosMultiAxis :public virtual ISercosMultiAxis, public virtual ICommon 	//**1222
{
protected:
	
	int			m_iAxisNum;
	MSercosAxis ** m_plnkAxis;
	INT32S*		m_plnkRingID;
	INT32S*		m_plnkNodeID;

	double*		m_pMoveVelocity;
	double*		m_pMoveSlowVelocity;
	int*		m_pAccelerate;
	double*		m_pScale;
	int*		m_pAxisID;

	double		m_dStabilityTime;
	double		m_dLimitTime;

	/** Motion 이동 시간 Timer */
	MTickTimer	m_ttMovingTimer;

public:		
	/**
	 * Constructor : Clear data (default value), initialize inner object
	 *				 Data를 초기화하고, 내부 개체들을 초기화한다.
	 * @stereotype constructor
	 */
	MSercosMultiAxis();    

	/* Constructor 
	 * @param   pAxis  : 기 생성된 IMPIAxis개체 배열의 주소값.
	 * @param   iAxisNum  : 기 생성된 IMPIAxis개체 배열의 크기
	            MS IMPIAxis 첫번째 개체의 MS로 자동맵핑되고, 자동반환된다.
				그러나 임의의 SuperVisor에 맵핑시킬경우 맵핑과 언맵핑을 항상 사용하여야 한다.
	 */
	MSercosMultiAxis(SCommonAttribute commonData, IAxis** pAxis, int iAxisNum);

	/**
	 * Destructor : Delete inner opbject
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	virtual ~MSercosMultiAxis();

	/**
	 * 축 이동 (한개의 축에 대한 이동) - 이동 완료된 후 return
	 * 
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Move(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL, 
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION);


	/**
	 * 축 이동 (한개의 축에 대한 이동) - 이동 명령 후 바로 return
	 * 
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int StartMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL, 
		double* dDecelerate = NULL, int bIsTrapMove = FALSE);

	/**
	 * 축 이동 후 완료를 확인한다. (한개의 축에 대한 완료 확인) 
	 * 
	 * @param   bSkipMode        : (OPTION=FALSE) 위치 확인 대기, 이동 후 안정화 시간 지연 사용 여부
	 * @param	bUseInterlock    : (OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
   	 * @param	bRMove		     : (OPTION=FALSE) RMove시 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
	 * @param   eDoneType		 :  
	 *                            - DEF_IN_POSITION_FINE_DONE : InPosition 신호 (축의 정지상태) 까지 기다린다.
	 *							  - DEF_IN_POSITION_COARSE_DONE : CoarseDone 신호 (사용자가 정한 중간의 임의지점)까지 기다리고 Return한다.
	 *							  - DEF_AT_TARGET_DONE : AtTarget 신호 (Command명령이 끝난시점) 까지 기다리고 Return한다.
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Wait4Done(BOOL bSkipMode = FALSE, int iMoveType = DEF_MOVE_POSITION, BOOL bUseInterlock = FALSE) ;


	/**
	 * 축 이동 (한개의 축에 대한 이동) - 이동 완료된 후 return
	 * 
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int RMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL, 
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION);


	/**
	 * 축 이동 (한개의 축에 대한 이동) - 이동 명령 후 바로 return
	 * 
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int StartRMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL, 
		double* dDecelerate = NULL, int bIsTrapMove = FALSE);

	/**
	 * Jog Slow에 의한 이동한다.
	 * 
	 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
	 * @param   dSlow           : (OPTION = 0.0) 이동할 거리, 0.0 = 지정된 Slow거리 사용
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0);

	/**
	 * Jog Fast에 의한 이동한다.
	 * 
	 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
	 * @param   dFast        : (OPTION = 0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
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

	virtual int Stop();

	/**
	 * 직선보간 이동한다.
	 * 
	 * @param   iMaxPoint        : 직선보간 이동구간 개수
	 * @param   *pdPosition      : 직선보간 이동구간 지정, iMaxPoint만큼 설정
	 * @param   *pdVelocity      : 이동 시 속도, iMaxPoint만큼 설정 (Scale은 적용되지 않는다.)
	 * @param   *piAccelerate    : 이동 시 가속도, iMaxPoint만큼 설정
	 * @param	bAutoFlag        : Auto CP 여부, FALSE=AutoCP 사용안함, TRUE=AutoCP 사용함
	 * @param	bWaitOpt         : (OPTION=TRUE)축 이동 완료 대기 여부, FALSE=대기 안함, TRUE=대기함
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int MoveSplineLine2(int iMaxPoint, double *pdPosition, double *pdPosition2, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE);
};

#endif // !defined(MSercosMultiAxis_H)
