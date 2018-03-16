
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

#ifndef IRSNMCMULTIAXIS_H
#define IRSNMCMULTIAXIS_H

#include "MRSNMCAxis.h"	

	
/**
 * MSercosMultiAxis.h : MPI를 이용한 다축제어 Class의 InterFace Class
 */

class IRSNMCMultiAxis :public virtual ICommon	//**1222
{


public:		


	/**
	 * 축 이동  - 이동 완료된 후 return
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
	virtual ErrorInfo Move(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION) = 0;

	/**
	 * 축 이동  - 이동 명령 후 바로 return
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
	virtual ErrorInfo StartMove (double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE) = 0;

	/**
	* 축 이동 후 완료를 확인한다. (Polling에의한 확인)  
	* "DEF_DONE_EVENT_TYPE"이 Define되면 Event방식에의한 Done을 사용한다.
	*
	* @param   bSkipStableTime  : (OPTION=FALSE) 안정화시간을 대기하지 않는다.
	* @param   eDoneType		:
	*                             - DEF_IN_POSITION_FINE_DONE : InPosition 신호 (축의 정지상태) 까지 기다린다.
	*							  - DEF_IN_POSITION_COARSE_DONE : CoarseDone 신호 (사용자가 정한 중간의 임의지점)까지 기다리고 Return한다.
	*							  - DEF_AT_TARGET_DONE : AtTarget 신호 (Command명령이 끝난시점) 까지 기다리고 Return한다.
	* @param	bUseInterlock    : (OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
	* @return	Error Code		 : 0 = SUCCESS
	*							  그 외 = 타 함수 Return Error
	*/
	virtual ErrorInfo Wait4Done (BOOL bSkipMode = FALSE, int iMoveType = DEF_MOVE_POSITION, BOOL bUseInterlock = FALSE) = 0 ;

	/**
	 * 축 이동  - 이동 완료된 후 return
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
	virtual ErrorInfo RMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION) = 0;

	/**
	 * 축 이동  - 이동 명령 후 바로 return
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
	virtual ErrorInfo StartRMove (double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE) = 0;

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
	virtual ErrorInfo JogMoveSlow (BOOL bDir, double dSlow = 0.0) = 0;

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
	virtual ErrorInfo JogMoveFast (BOOL bDir, double dFast = 0.0) = 0;

	/**
	 * Jog 이동을 정지 시킨다.
	 * 	 
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  그 외 =  Error Code
	 */
	virtual ErrorInfo JogStop() = 0;

	virtual ErrorInfo Stop() = 0;

	virtual ErrorInfo GroupDisable() = 0;

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
	virtual ErrorInfo MoveSplineLine2 (int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE) = 0;
};

#endif // !defined(MSercosMultiAxis_H)
