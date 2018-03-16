/* 
 * Motion Component
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
 * IMotionLibNMC.h : This file defines the interface class of Motion libraries.
 *				  이 파일은 Motion Library를 위한 Class에 대한 Interface Class를 정의한다.
 *
 * @author Yong-seog Pahn(dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef IMOTIONLIBNMC_H
#define IMOTIONLIBNMC_H

#include "ICommon.h"

#include "DefMotionStruct.h"

/** This class is defined Motion component interface class of Motion libraries as a abstract class.
 * 이 Class는 Motion Library를 위한 Class에 대한 Interface Class로 정의한다.
 * 
 * Motion Board에서 제공하는 모든 API Library를 사용할 수 있도록 하기 위한 Interface로 생성되는 개체는 
 * Motion Board 종류에 따라 CMMCLib, 혹은 CMEILib Class이다.(CMEILib Class는 Class 형태만 제공)
 * 본 Interface에서 제공되는 API Library는 현재 구한 MMC 매뉴얼에 있는 내용만 구현되어있다.
 *(매뉴얼에 없는 API 함수들의 사용법은 본 개발자도 모르기 때문에...)
 * 본 nterface를 사용하게 되면 Board 초기화나 기타 세세한 설정 및 동작 구현등이 사용자의 의지대로 자유롭게
 * 사용할 수 있으므로 IAxis Interface를 제공함으로 인한 기능 제한 요소를 제거할 수 있다.
 * 본 Interface에 대한 개체는 무한정 생성 가능하며, 어느 개체에서든 한번 초기화를 수행하면 다른 개체에서는
 * 수행할 수 없도록 구성되어 있다.(static 변수 처리)
 *
 * @author Yong-seog Pahn(dragon.pahn@samsung.com )
 * @version $Revision$
 *
 * @interface
 */
class IMotionLibNMC : public virtual ICommon
{

public:
	virtual ~IMotionLibNMC() {}; 


	/**
	 * 축 사용 여부 설정(이미 사용중인 축은 사용으로 설정 불가)
	 *
	 * 축 사용 여부 설정은 Motor Parameter 설정과는 무관하며, 사용자에 의해 설정이 되어야 한다.
	 *
	 * @param	siAxisID		: 축 ID
	 * @param	bState			: 사용 여부(TRUE=사용, FALSE=미사용)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetUseAxis(int iBoardNo, short siAxisID, BOOL bState) = 0;

	/**
	 * 축 사용 여부 읽기
	 *
	 * @param	siAxisID		: 축 ID
	 * @param	*pbState		: 사용 여부(TRUE=사용, FALSE=미사용)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetUseAxis(short siAxisID, BOOL *pbState) = 0;

	/**
	 * Motor Parameter 설정(Board 초기화 후 사용 가능)
	 *
	 * 전달된 SMotionAxis의 내용으로 Motion을 설정한다.(축 1개 단위로 설정)
	 *
	 * 1. Board가 초기화 안되어 있으면 Error Return
	 * 2. 축 AMP Enable(Servo ON)이면 Disable(OFF) 후 작업 진행
	 * 3. Motor 종류 설정
	 * 4. Feedback Device 설정
	 * 5. Loop 형태 설정
	 * 6. Motor 종류에 따라
	 *		6.1 속도형 Servo의 경우
	 *			제어 모드와 출력 형태 설정
	 *		6.2 일반 Stepper의 경우
	 *			펄스 분주비와 전자기어비 설정
	 *		6.3 Micro Stepper 혹은 위치형 Servo의 경우
	 *			펄스 분주비(default:8)와 전자기어비(default:1.0) 설정
	 * 7. 출력 펄스 형태 설정
	 * 8. Encoder와 좌표 방향 설정
	 * 9. AMP Enable, Fault, Reset 설정
	 * 10. Home, Positive, Negative Sensor 설정
	 * 11. Positive, Negative S/W Limit 설정
	 * 12. In-Position 설정
	 * 13. 축 AMP Enable(Servo ON)이었으면 Enable(ON) 설정
	 *
	 * @param	siAxisID		: 축 ID, -1=허용안됨
	 * @param	mAx				: Motor Parameter
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 기록할지 여부, TRUE=boot file에 기록
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  xx = MOTION BOARD NO INITIALIZED(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetMotorParam(short siAxisID, SMotionAxis mAx, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Motor Parameter 읽기
	 *
	 * @param	siAxisID		: 축 ID, -1=All Motor
	 * @param	*pmAx			: Motor Parameter, siAxisID=-1이면 배열 구조로 구성
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetMotorParam(short siAxisID, SMotionAxis *pmAx) = 0;


	/**
	 * Motion Board Parameter 설정
	 *
	 * @param	iObjectID		: Object ID
	 * @param	MotionBd		: Motion Board Parameter
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log LEvel(None, Normal, Warning, Error)
	 * @param	iDays			: Log 우지 일 수
	 * @return	Error Code		: 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetBoardParam(SMotionBoard MotionBd) = 0;

	/**
	 * Motion Board Parameter 읽기
	 *
	 * @param	*pmBd			: Motion Board Parameter
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetBoardParam(SMotionBoard *pmBd) = 0;

	/**
	 * 구성된 Board 개수 읽기
	 *
	 * @param	*psiBdNum		: Board 구성 개수
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetBoardNum(short *psiBdNum) = 0;

	/**
	 * Motion Board 초기화
	 * 
	 * 1. Motion Board 초기화가 수행된 경우
	 * 	(1) Board 구성 수와 Board DPRAM Address가 같은 경우
	 * 		→ Board 초기화 과정 Pass
	 * 	(2) Board 구성 수가 다른 경우
	 * 		→ Board 초기화 오류 처리
	 * 	(3) Board DPRAM Address가 다른 경우
	 * 		→ Board 초기화 오류 처리
	 * 
	 * 2. Motion Board 초기화가 수행되지 않은 경우
	 * 	→ Board 초기화 수행
	 *
	 * @param	siBdNum			: Motion Board 수(1 ~ 4)
	 * @param	*plAddress		: Board DPRAM Address(Board 수만큼)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = NOT EQUAL MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = NOT EQUAL MOTION DPRAM ADDRESS(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo Initialize(short siBdNum, long *plAddress) = 0;

	/**
	 * Motion Board 초기화 여부 읽기
	 *
	 * @param	*pbInit			: Board 초기화 여부, TRUE=초기화됐음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetBoardInit(BOOL *pbInit) = 0;

	/**
	 * Motion Board의 제어 축 수를 돌려준다.
	 *
	 * @param	siBdNum			: Motion Board ID(0 ~ 7), -1=All Board
	 * @param	*psiAxes		: Board에 구성된 제어 축 수
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAxes(short siBdNum, short *psiAxes) = 0;

	/**
	 * 직선, 원, 원호등의 동작을 수행할 각 좌표계의 축을 정의
	 * 같은 Board의 축으로 구성해야 한다.
	 *
	 * @param	siAxNum			: 축 수(1 ~ 8)
	 * @param	*psiMapArray	: 축 ID 배열(같은 Board안의 축 ID이어야 한다.)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = NO COMMON MOTION BOARD AXIS(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo MapAxes(short siAxNum, short *psiMapArray) = 0;

	/**
	 * 직선, 원, 원호등의 동작을 수행할 각 좌표계의 축을 정의
	 * Motion 프로그램 지연과 축 다음 동작 실행 지연 2가지를 제공한다.
	 * 지연시간은 1msec 단위이며 0보다 커야 한다.
	 *
	 * @param	siAxis			: 축 ID(-1 ~ 63), -1=프로그램지연
	 * @param	*plDuration		: 지연시간(1msec단위)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID DELAY DURATION(<=0)(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo Dwell(short siAxis, long *plDuration) = 0;

	/**
	 * I/O Bit가 지정된 상태로 될 때까지 해당 축의 다음 동작 실행을 지연한다.
	 * I/O Bit No는 Board 구성 상태에 따라 64개 단위로 변동된다.
	 * (Board#1 : 0 ~ 63, Board #2 : 64 ~ 63, Board #3 : 64 ~ 95, Board #4 : 96 ~ 127)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siBitNo			: I/O Bit No
	 * @param	bState			: I/O Bit 상태
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID BIT IO NUMBER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo IOTrigger(short siAxis, short siBitNo, BOOL bState) = 0;

	/**
	 * 축 명령 수행에 대한 상태를 돌려준다.
	 * 명령 수행 완료된 상태이면 SUCCESS를 Return한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: 조회 종류, 0=이동명령, 1=속도명령, 2=InPosition 범위내 여부
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID IN COMMAND TYPE(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo InCommand(short siAxis, BOOL *bState, short siType=0) = 0;

	/**
	 * 직선, 원, 원호등의 동작이 완료되었는지 여부를 돌려준다.
	 * MapAxes()에서 설정한 축들에 대해 확인한다.
	 *
	 * @param	*pbStatus		: 완료 여부
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo AllDone(BOOL *pbStatus) = 0;

	/**
	 * 축의 동작 완료 여부를 Return 한다.(정헌식 : 2005.01.11)
	 * 이동 및 속도 이동 완료 확인 또는 더불어 In-Position 여부까지 확인한다.
	 *
	 * MMC Library : axis_done()
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsAxisDone(short siAxis) = 0;
	
	/**
	 * 축의 동작 완료 여부를 Return 한다.(정헌식 : 2005.01.11)
	 * 이동 및 속도 이동 완료 확인한다. InPosition 확인 안함
	 *
	 * MMC Library : axis_done()
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsMotionDone(short siAxis) = 0;

	/**
	 * 지정 축이 동작 완료될 때까지 기다린다.
	 * *psiAxis가 NULL로 전달되면 siAxis가 축 ID로 인식되어 동작한다.
	 * *psiAxis가 NULL이 아닌걸로 전달되면 siAxis가 축 수로 인식되어 동작한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63) 혹은 축 수(1 ~ 64)
	 * @param	*psiAxis		:(OPTION=NULL) 축 ID 배열, siAxis가 축 수일 경우 사용
	 *																 축 ID라면 생략
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo WaitForDone(short siAxis) = 0;

	/**
	 * 지정 축의 AMP Fault를 Clear하거나 Fault Port를 Enable 상태로 지정한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bState			: 지정할 상태, FALSE=Clear, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpFaultEnable(short siAxis, BOOL bState) = 0;

	/**
	 * 축 모든 현재 상태를 한꺼번에 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiState		: 상태,(axis_source, in_sequence, get_com_velocity, get_act_velocity,
	 *									 motion_done, in_position, axis_done 결과값)
	 * @param	*plStatus		: 상태,(get_io 결과값)
	 * @param	*pdStatus		: 상태,(get_position, get_command, get_error 결과값)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAllStatus(short siAxis, short *psiStatus, long *plStatus, double *pdStatus) = 0;

	/**
	 * 해동축의 동작중 속도명령값을 PULSE값으로 돌려준다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @return	Puls
	 */
	virtual ErrorInfo GetComVelocity(short sAxis) = 0;

	/**
	 * 축 현재 상태를 읽는다.
	 * 상태는 bit 조합으로 구성되어 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiState		: 상태, 각 상태는 bit 조합으로 구성된다.
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAxisSource(short siAxis, short *psiState) = 0;

	/**
	 * 축의 센서(Home, Positive, Negative) 상태를 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: 센서 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	*pbState		: 센서 상태, TRUE=Active, FALSE=No Active
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 */
	virtual ErrorInfo GetSensorStatus(short siAxis, short siType, BOOL *pbState) = 0;

	/**
	 * AMP Disable/Enable 상태를 읽는다.(Servo ON/OFF)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbState		: AMP Enable 상태, TRUE=Enable, FALSE=Disable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpEnable(short siAxis, BOOL *pbState) = 0;

	/**
	 * AMP Enable의 Active Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbLevel		: Enable Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpEnableLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 축의 현재 Event 발생 상태를 읽는다.
	 *
	 *		NO_EVENT		0		Event 발생없이 정상 동작
	 *		STOP_EVENT		1		stop_rate로 감속하면서 정지
	 *		E_STOP_EVENT	2		e_stop_rate로 감속하면서 정지
	 *		ABORT_EVENT		3		AMP disable 상태
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiState		: Event 내역, 0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	//virtual int GetAxisState(short siAxis, short *psiState) = 0;
	virtual ErrorInfo GetAxisState(short siAxis, UINT32 *psiState) = 0; //170713 JSH

	/**
	 * Board의 Position Latch 여부를 읽는다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbState		: Position Latch 여부, TRUE=Latch상태
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAxisLatchStatus(short siBdNum, BOOL *pbState) = 0;

	/**
	 * 축에 발생된 Event를 해제하고, 다음 명령부터 실행한다.
	 * Event 발생 후에는 항상 Event를 해제해 주어야 한다.
	 * ABORT_EVENT 발생 시에는 Event 해제 후 AMP가 Disable 상태이므로 다시 Enable해주어야 한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo ClearStatus(short siAxis) = 0;

	/**
	 * 축의 Frame Buffer를 Clear한다.
	 * 축별 최대 50개의 Frame의 내용을 Clear한다. 단 현재 실행중인 명령은 계속 수행된다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo ClearFrames(short siAxis) = 0;

	/**
	 * 축의 비어있는 Interpolation Frame 개수를 돌려준다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiFrameNo		: Frame 개수
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo FramesInterpolation(short siAxis, short *psiFrameNo) = 0;

	/**
	 * 축의 비어있는 Frame 개수를 돌려준다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiFrameNo		: 비어있는 Frame 개수
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo FramesLeft(short siAxis, short *psiFrameNo) = 0;

	/**
	 * 해당 Board의 Latch 상태를 지정하고, Latch Status를 False로 만들거나, S/W적으로 Position을 Latch한다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bType			: Latch 종류, FALSE=S/W Position Latch, TRUE=Board Latch Enable/Disable 지정
	 * @param	bState			:(OPTION=FALSE) bType=TRUE인 경우 Enable/Disable 지정
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo Latch(short siBdNum, BOOL bType, BOOL bState = FALSE) = 0;

	/**
	 * 지정 축의 Latch된 Position을 돌려준다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdPosition		: Latch된 Position
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetLatchedPosition(short siAxis, double *pdPosition) = 0;

	/**
	 * 동작중 목표위치를 재지정할 때 사용한다.(원, 원호 동작중에는 적용되지 않는다.)
	 *
	 * @param	siLen			: 축 수
	 * @param	*psiAxes		: 축 ID 배열
	 * @param	*pdDist			: 위치 보정값
	 * @param	*psiAccel		: 이동 가,감속 구간
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(<=0)(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo CompensationPos(short siLen, short *psiAxes, double *pdDist, short *psiAccel) = 0;

	/**
	 * Board DPRAM Address를 읽는다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*plAddr			: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetDpramAddress(short siBdNum, long *plAddr) = 0;

	/**
	 * 절대치 Motor의 Type을 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiType		: Motor 종류, 1=삼성CSDJ, CSDJ+SERVO DRIVE, 2=YASKAWA SERVO DRIVE
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAbsEncoderType(short siAxis, short *psiType) = 0;

	/**
	 * 축의 이동 최고속도와 가,감속 구간값의 제한값을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdVelocity		: 이동 최고속도, 1 ~ 2047000 coutn/sec
	 * @param	*psiAccel		: 가,감속 구간값, 1 ~ 200, 10msec 단위
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetVelLimit(short siAxis, double *pdVelocity, short *psiAccel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Drive에 Fault 발생 상태를 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbStatus		: AMP Fault 상태를 읽는다. TRUE=FAULT, FALSE=NORMAL
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAmpFaultStatus(short siAxis, BOOL *pbStatus) = 0;

	/**
	 * AMP Drive에 Fault 발생 시 동작할 Event를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiAction		: 동작할 Event, NO EVENT, STOP EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpFaultEvent(short siAxis, short *psiAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Enable의 Active Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbLevel		: Enable 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpEanbleLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Fault의 Active Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbLevel		: Fault 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpFaultLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Reset의 Active Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbLevel		: Reset 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpResetLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 AMP Drive의 Resolution을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiResolution	: AMP Resolution, default=2500 pulse/rev
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAmpResolution(short siAxis, short *psiResolution, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 분주비에 대한 분자값, 분모값을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiRatioA		: Encoder 분주비 분자값
	 * @param	*psiRatioB		: Encoder 분주비 분모값
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID DENOMINATOR(<1)(MOTIONLIB)
	 *							  xx = INVALID MOLECULE(<1)(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetEncoderRatio(short siAxis, short *psiRatioA, short *psiRatioB, BOOL bBootOpt= FALSE) = 0;

	/**
	 * 지정 축이 회전/직선운동하는 무한회전 축인지 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbStatus		: 무한회전 축 설정여부
	 * @param	bType			: 운동 종류, FALSE=직선, TRUE=회전
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetEndlessAx(short siAxis, BOOL *pbStatus, BOOL bType, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 무한회전 축의 움직이는 영역을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdRange		: 이동 영역
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetEndlessRange(short siAxis, double *pdRange, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 축의 위치결정 완료값과 위치결정 시 신호 Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdInPosition	: 위치 결정값
	 * @param	*pbLevel		: 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetInPosition(short siAxis, double *pdInPosition, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 InPosition 신호 사용여부를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbReq			: 사용 여부, TRUE=사용
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetInpositionRequired(short siAxis, BOOL *pbReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 축의 위치오차 제한값과 Event를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdLimit		: 위치오차 제한값, 최대 35000 count
	 * @param	*psiAction		: 위치오차 Event, NO EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetErrorLimit(short siAxis, double *pdLimit, short *psiAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 원점 복귀 시 Encoder의 C상 펄스 이용 여부를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbIndexReq		: C상 펄스 사용 여부, TRUE=Home Sensor와 Encoder의 Index Pulse를 동시 검출,
	 *												  FALSE=Home Sensor만 검출
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetIndexRequired(short siAxis, BOOL *pbIndexReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * I/O 8점에 대한  입,출력 모드를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbMode			: 입, 출력 모드, TRUE=출력, FALSE=입력
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetIOMode(short siBdNum, BOOL *pbMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- 방향 Limit Switch Active시 동작할 Event와 신호 Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	*psiLimit		: 동작할 Event
	 * @param	*pbLevel		: 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSensorLimit(short siAxis, short siType, short *psiLimit, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- 방향 Limit Switch Active시 동작할 Event를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	*psiLimit		: 동작할 Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSensorEvent(short siAxis, short siType, short *psiLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- 방향 Limit Switch Active시 동작할 신호 Level을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	*pbLevel		: 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSensorLevel(short siAxis, short siType, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * +/- 방향으로 Motor가 이동할 수 있는 제한 위치값과 그 위치값에 도달했을 때 적용할 Event를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 이동 방향, TRUE=Positive, FALSE=Negative
	 * @param	*pdPosition		: 제한 위치값
	 * @param	*psiLimit		: 적용할 Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSWLimit(short siAxis, BOOL bType, double *pdPosition, short *psiLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축이 어떤 Motor로 제어하는 축으로 지정되어 있는지 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiType		: Motor 종류, 0=속도형Servo, 1=일반Stepper, 2=MicroStepper 혹은 위치형Servo
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetMotorType(short siAxis, short *psiType, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 Feedback 장치와 Loop 형태를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiDevice		: Feedback 장치, 0=Encoder입력, 1=0~10volt입력, 2=-10~10volt입력
	 * @param	*pbLoop			: Loop 형태, FALSE=Open Loop, TRUE=Closed Loop
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAxisProperty(short siAxis, short *psiDevice, BOOL *pbLoop, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 Pulse 분주비와 전자기어비를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiPgratio		: Pulse 분주비
	 * @param	*pdEgratio		: 전자기어비
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetRatioProperty(short siAxis, short *psiPgratio, double *pdEgratio, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 속도형 Servo의 설정을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbControl		: 제어모드, FALSE=속도제어, TRUE=위치제어
	 * @param	*pbPolar		: Analog 출력 종류, FALSE=UNIPOLAR, TRUE=BIPOLER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS,
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetVServoProperty(short siAxis, BOOL *pbControl, BOOL *pbPolar, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 Pulse 출력 형태를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbMode			: Pulse 출력 형태, FALSE=Two Pulse(CW+CCW), TRUE=Sign+Pulse
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetStepMode(short siAxis, BOOL *pbMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 Encoder 입력 방향과 좌표 방향을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbEncDir		: Encoder 입력 방향, FALSE=ENCO_CW(시계방향, - Count)
	 *												 TRUE =ENCO_CCW(반시계방향, + Count)
	 * @param	*pbCoorDir		: 좌표방향, FALSE=CORD_CW(시계방향, + 좌표 이동)
	 *										TRUE =CORD_CCW(반시계방향, - 좌표 이동)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetEncoderDirection(short siAxis, BOOL *pbEncDir, BOOL *pbCoorDir, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정된 축의 STOP EVENT, ESTOP EVENT 수행 시 감속 시간을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 정지 종류, FALSE=STOP, TRUE=E-STOP
	 * @param	*psiRate		: 감속 시간
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetStopRate(short siAxis, BOOL bType, short *psiRate, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 동기제어시 적용되는 보상 Gain값을 읽는다.
	 *
	 * MMC Library : get_sync_gain()
	 *
	 * @param	*plCoeff		: 보상 Gain 값
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSyncGain(long *plCoeff) = 0;

	/**
	 * 해당 축의 속도 또는 위치에 대한 PID & FF Gain값들을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * MMC Library : get_gain(), fget_gain(), get_v_gain(), fget_v_gain()
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bVelType		: 위치/속도 종류 지정, FALSE=위치, TRUE=속도
	 * @param	*plGain			: Gain 값 배열, 배열인수위치는 아래와 같다.
	 *								0=GA_P, 1=GA_I, 2=GA_D, 3=GA_F, 4=GA_LIMIT, 5=GAIN_MUNBER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 적분제어 시 적분제어 모드를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 제어모드, FALSE=위치, TRUE=속도
	 * @param	*pbMode			: 적분제어 모드, FALSE=정지시적용, TRUE=항상적용
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetIntegration(short siAxis, BOOL bType, BOOL *pbMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 속도지령 혹은 토크 지령에 대해 Low Pass Filter 혹은 Notch Filter에 대한 Filter 값을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bCommandType	: 지령 종류, FALSE=속도(Position), TRUE=토크(Velocity)
	 * @param	bFilterType		: Filter 종류, FALSE=LowPass, TRUE=Notch
	 * @param	*pdFilter		: Filter 값
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double *pdFilter,
							BOOL bBootOpt= FALSE) = 0;

	/**
	 * 해당 축의 동작 중 속도를 읽는다.(명령 값과 실제 값)
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 속도 종류, FALSE=실제속도값, TRUE=속도명령값
	 * @param	*psiPulse		: 속도의 Pulse값
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetVelocity(short siAxis, BOOL bType, double *pdPulse) = 0;

	/**
	 * 지정된 Board의 축별 동작여부를 읽는다.
	 *
	 *		b7	b6	b5	b4	b3	b2	b1	b0
	 *		축8	축7	축6	축5	축4	축3	축2	축1
	 *
	 *		bit = TRUE : 동작 금지
	 *		bit = FALSE : 동작 가능
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiState		: 축별 동작 여부, bit가 한 축(b0=축1, b1=축2, ...), TRUE=정지, FALSE=동작
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAxisRunStop(short siBdNum, short *psiState) = 0;

	/**
	 * 축의 실제위치 및 목표위치를 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bCommandType	: 위치 종류, FALSE=실제위치(Encode), TRUE=목표위치(Command)
	 * @param	*pdPosition		: bType=FALSE이면, 지정할 실제위치(Encode)
	 *							  bType=TRUE 이면, 지정할 목표위치(Command)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetPosition(short siAxis, BOOL bCommandType, double *pdPosition) = 0;

	/**
	 * Motor의 지령치 RPM이나 실제 RPM을 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: RMP 종류, FALSE=실제RPM, TRUE=지령치RPM
	 * @param	*psiRpm			: RPM값
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetRpm(short siAxis, BOOL bType, short *psiRpm) = 0;

	/**
	 * Board별 Sampling Rate를 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiTime		: Sampling Rate, msec단위(1=4msec, 2=2msec, 3=1msec만 지원)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetControlTimer(short siBdNum, short *psiTime, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 축의 목표위치와 실제위치의 차이값인 위치오차를 읽는다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdError		: 위치오차,(목표위치-실제위치)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetError(short siAxis, double *pdError) = 0;

	/**
	 * 특정 축의 Encoder Feedback Data를 빠르게 읽어들일 때 사용(50usec 주기 Update)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pbStatus		: 설정 여부
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetFastReadEncoder(short siAxis, BOOL *pbStatus) = 0;

	/**
	 * 해당 축의 Analog Offset 값을 읽는다.
	 * boot file 또는 실행중인 memory에서 읽을 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*psiOffset		: Analog Offer, +/-2048, +/-64767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAnalogOffset(short siAxis, short *psiOffset, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 입, 출력 Port의 64bit Data를 읽는다.
	 *
	 * @param	siPort			: 입, 출력 Port 번호(0 ~ 3, Board 구성 개수에 따라 변동)
	 * @param	bType			: 입, 출력 종류, FALSE=입력, TRUE=출력
	 * @param	*plValue		: 64bit Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO PORT ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetIO(short siPort, BOOL bType, long *plValue) = 0;

	/**
	 * 지정된 Analog 입/출력의 12/16bit Data 값을 읽는다.
	 *
	 * @param	siChannel		: Analog 입력 채널 수(0 ~ 7) 혹은 출력 축 ID(0 ~ 63)
	 * @param	bType			: 입, 출력 종류, FALSE=입력, TRUE=출력
	 * @param	*psiValue		: bType=FALSE이면 Analog 입력 값, -2048 ~ +2047
	 *							  bType=TRUE이면 Analog 출력 값, +/-2048, +/-64767
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID ANALOG INPUT CHANNEL ID(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetAnalog(short siChannel, BOOL bType, short *psiValue) = 0;

	/**
	 * Board의 충돌방지 기능의 사용여부를 읽는다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbMode			: 사용여부, TRUE=사용
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetCollisionPreventFlag(short siBdNum, BOOL *pbMode) = 0;

	/**
	 * 동기제어 여부를 읽는다.
	 *
	 * @param	*pbState		: 지정 여부, TRUE=지정
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSyncControl(short siAxis, BOOL *pbState) = 0;

	/**
	 * Master축과 Slave축의 실제위치를 읽는다.
	 *
	 * @param	*pdMasterPos	: Master 축 위치
	 * @param	*pdSlavePos		: Slave 축 위치
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSyncPosition(double *pdMasterPos, double *pdSlavePos) = 0;

	/**
	 * 보드별로 I/O Interrupt를 Enable/Diable하거나, I/O Interrupt 발생 시
	 * STOP-EVENT나 E-STOP-EVENT를 지정축에 발생할지 여부를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siID			: Board(0 ~ 7) 혹은 축 ID(0 ~ 63)
	 * @param	siType			: 종류, 0=Board Enable/Disable, 1=STOP EVENT지정, 2=ESTOP EVENT지정
	 * @param	bState			: 설정, TRUE =Enable지정,  STOP EVENT/ESTOP EVENT지정,
	 *									FALSE=Disable지정, STOP EVENT/ESTOP EVENT미지정
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID IO INTTERUPT TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo IOInterrupt(short siID, short siType, BOOL bState, BOOL bBootOpt = FALSE) = 0;

	/**
	 * I/O Interrupt 발생 시 PC쪽으로 Interrupt를 발생시킬지 여부를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bState			: 발생 여부, TRUE=발생
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo IOInterruptPCIRQ(short siBdNum, BOOL bState, BOOL bBootOpt = FALSE) = 0;

	/**
	 * PC Interrupt 발생 시 end of interrupt 신호를 발생시킨다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo IOInterruptPCIRQ_EOI(short siBdNum) = 0;

	/**
	 * 지정 축의 PID 제어 여부를 지정한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bState			: PID 제어 여부, FALSE=PID제어 미실시, Analog 출력 0volt,
	 *											 TRUE =PID제어 실시
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetController(short siAxis, BOOL bState) = 0;

	/**
	 * AMP Disable/Enable 상태를 설정한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bState			: AMP Enable 상태, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpEnable(short siAxis, BOOL bState) = 0;

	/**
	 * 축의 이동 최고속도와 가,감속 구간값의 Limit를 지정한다.(boot file에 자동 저장)
	 *
	 * MMC Library : set_accel_limit(), set_vel_limit()
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	dVelocity		: 이동속도, 1 ~ 2047000 count/rev
	 * @param	siAccel			: 가,감속구간값, 1 ~ 200, 10msec단위
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID VELOCITY VALUE(MOTIONLIB)
	 *							  xx = INVALID ACCELERATE VALUE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetVelLimit(short siAxis, double dVelocity, double dAccel, double dDecel) = 0;

	/**
	 * AMP Drive에 Fault 발생 시 동작할 Event를 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siAction		: 동작할 Event, NO EVENT, STOP EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID EVENT VALUE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpFaultEvent(short siAxis, short siAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Enable의 Active Level을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bLevel			: Enable Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpEnableLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Fault의 Active Level을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bLevel			: Fault Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpFaultLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Reset의 Active Level을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bLevel			: Reset Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpResetLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 AMP Drive의 Resolution을 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siResolution	: AMP Resolution, default=2500 pulse/rev
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID RESOLUTION NUMBER(<0)(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAmpResolution(short siAxis, short siResolution, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 분주비에 대한 분자값, 분모값을 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siRatioA		: Encoder 분주비 분자값
	 * @param	siRatioB		: Encoder 분주비 분모값
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID DENOMINATOR(<1)(MOTIONLIB)
	 *							  xx = INVALID MOLECULE(<1)(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetEncoderRatio(short siAxis, short siRatioA, short siRatioB, BOOL bBootOpt= FALSE) = 0;

	/**
	 * 지정 축을 회전/직선운동하는 무한회전 축으로 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bStatus			: 무한회전 축 설정여부
	 * @param	siResolution	: Motor 1회전당 Pulse수
	 * @param	bType			: 운동 종류, FALSE=직선, TRUE=회전
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID RESOLUTION NUMBER(<0)(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetEndlessAx(short siAxis, BOOL bStatus, short siResolution, BOOL bType,
							   BOOL bBootOpt = FALSE) = 0;

	/**
	 * 무한회전 축의 움직이는 영역을 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	dRange			: 이동 영역
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetEndlessRange(short siAxis, double dRange, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 원점 복귀 시 Encoder의 C상 펄스 이용 여부를 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bIndexReq		: C상 펄스 사용 여부, TRUE =Home Sensor와 Encoder의 Index Pulse를 동시 검출,
	 *												  FALSE=Home Sensor만 검출
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetIndexRequired(short siAxis, BOOL bIndexReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축을 해당 Motor 종류로 제어하는 축으로 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Motor 종류, 0=속도형Servo, 1=일반Stepper, 2=MicroStepper 혹은 위치형Servo
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOTOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetMotorType(short siAxis, short siType, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 Feedback 장치와 Loop 형태를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siDevice		: Feedback 장치, 0=ENCODER, 1=0~10volt입력, 2=-10~10volt입력
	 * @param	bLoop			: Loop 형태, FALSE=Open Loop, TRUE=Closed Loop
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOTOR FEEDBACK DEVICE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAxisProperty(short siAxis, short siDevice, BOOL bLoop, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 분주비와 전자기어비를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siPgratio		: Pulse 분주비, default=8
	 * @param	dEgratio		: 전자기어비, default=1.0
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetRatioProperty(short siAxis, short siPgratio, double dEgratio, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 속도형 Servo의 설정을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bControl		: 제어모드, FALSE=속도제어, TRUE=토크제어
	 * @param	bPolar			: Analog 출력 종류,  TRUE=UNIPOLAR, FALSE=BIPOLER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetVServoProperty(short siAxis, BOOL bControl, BOOL bPolar, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 Pulse 출력 형태를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bMode			: Pulse 출력 형태, FALSE=Two Pulse(CW+CCW), TRUE=Sign+Pulse
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetStepMode(short siAxis, BOOL bMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 Encoder 입력 방향과 좌표 방향을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bEncDir			: Encoder 입력 방향, FALSE=ENCO_CW(시계방향, - count),
	 *												 TRUE =ENCO_CCW(반시계방향, + count)
	 * @param	bCoorDir		: 좌표방향, FALSE=CORD_CW(시계방향, +좌표이동),
	 *										TRUE =CORD_CCW(반시계방향, -좌표이동)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetEncoderDirection(short siAxis, BOOL bEncDir, BOOL bCoorDir, BOOL bBootOpt = FALSE) = 0;

	/**
	 * I/O 8점에 대한  입,출력 모드를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bMode			: 입, 출력 모드, TRUE=출력, FALSE=입력
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetIOMode(short siBdNum, BOOL bMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 축의 위치결정 완료값과 위치결정 시 신호 Level을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	dInPosition		: 위치 결정값
	 * @param	bLevel			: 신호 Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetInPosition(short siAxis, double dInPosition, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 InPosition 신호 사용여부를 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bReq			: 사용 여부, TRUE=사용
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetInpositionRequired(short siAxis, BOOL bReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 축의 위치오차 Limit값과 Event를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	dLimit			: 위치오차 Limit값, 최대 35000 count
	 * @param	siAction		: 위치오차 Event, NO EVENT, STOP EVENT, ESTOP EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetErrorLimit(short siAxis, double dLimit, short siAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정된 축의 STOP EVENT, ESTOP EVENT 수행 시 감속 시간을 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 정지 종류, FALSE=STOP, TRUE=ESTOP
	 * @param	siRate			: 감속 시간, default=10
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetStopRate(short siAxis, BOOL bType, short siRate, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- 방향 Limit Switch Active시 동작할 Event와 신호 Level을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	siLimit			: 동작할 Event
	 * @param	bLevel			: 신호 Level, TRUE=HIGH, FLASE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSensorLimit(short siAxis, short siType, short siLimit, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- 방향 Limit Switch Active시 동작할 Event를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	siLimit			: 동작할 Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSensorEvent(short siAxis, short siType, short siLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- 방향 Limit Switch Active시 동작할 신호 Level을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
	 * @param	bLevel			: 신호 Level, TRUE=HIGH, FLASE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSensorLevel(short siAxis, short siType, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * +/- 방향으로 Motor가 이동할 수 있는 Limit 위치값과 그 위치값에 도달했을 때 적용될 Event를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 방향, FALSE=Negative, TRUE=Positive
	 * @param	dPosition		: 제한 위치값, +/-2147483647
	 * @param	siLimit			: 적용될 Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSWLimit(short siAxis, BOOL bType, double dPosition, short siLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 속도 또는 위치에 대한 PID & FF Gain 값들을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bVelType		: 위치/속도 종류 지정, FALSE=위치, TRUE=속도
	 * @param	*plGain			: Gain 값 배열, 배열인수 위치는 아래와 같다.
	 *								0=GA_P, 1=GA_I, 2=GA_D, 3=GA_F, 4=GA_ILIMIT, 5=GAIN_NUMBER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 해당 축의 적분 제어 시 적분 제어 모드를 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 제어모드, FALSE=위치, TRUE=속도
	 * @param	bMode			: 적분제어 모드, FALSE=항상적용, TRUE=정지시적용
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetIntegration(short siAxis, BOOL bType, BOOL bMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 속도지령 혹은 토크 지령에 대해 Low Pass Filter 혹은 Notch Filter에 대한 Filter 값을 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bCommandType	: 지령 종류, FALSE=속도(Position), TRUE=토크(Velocity)
	 * @param	bFilterType		: Filter 종류, FALSE=LowPass, TRUE=Notch
	 * @param	dFilter			: Filter 값
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double dFilter,
							BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정된 Board의 축별 동작 여부를 설정한다.
	 *
	 *		b7	b6	b5	b4	b3	b2	b1	b0
	 *		축8	축7	축6	축5	축4	축3	축2	축1
	 *
	 *		bit = TRUE : 동작 금지
	 *		bit = FALSE : 동작 가능
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siState			: 축별 동작 여부, bit가 한 축(b0=축1, b1=축2, ...), TRUE=정지, FALSE=동작
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAxisRunStop(short siBdNum, short siState) = 0;

	/**
	 * 지정 I/O bit를 HIGH(1)/LOW(0) 상태로 만든다.
	 *
	 *		 Board수	  I/O Bit 범위
	 *			1			0  ~ 63
	 *			2			64 ~ 63
	 *			3			64 ~ 95
	 *			4			96 ~ 127
	 *
	 * @param	siBitNo			: 지정할 I/O Bit 번호(장착된 Board의 수량에 따라 달라짐)
	 * @param	bValue			: 지정할 값,(TRUE, FALSE)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetBit(short siBitNo, BOOL bValue) = 0;

	/**
	 * 64bit의 I/O Data를 출력 Port를 통해 내보낸다.
	 *
	 * @param	siPort			: 출력 Port 번호(0 ~ 3, Board 구성 개수에 따라 변동)
	 * @param	lValue			: 출력 값
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO PORT ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetIO(short siPort, long lValue) = 0;

	/**
	 * 12/16 bit Analog 출력전압을 내보낸다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siValue			: 출력할 전압 값, +/-2048, +/-64767
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetDacOut(short siAxis, short siValue) = 0;

	/**
	 * 해당 축의 Analog Offset값을 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siOffset		: Analog OFfset, +/-2048, +/-64767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAnalogOffset(short siAxis, short siOffset, BOOL bBootOpt = FALSE) = 0;

	/**
	 * 지정 축의 출력전압의 범위를 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siLimit			: A출력 전압 범위, 0 ~ 64767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAnalogLimit(short siAxis, short siLimit, BOOL bBootOpt= FALSE) = 0;

	/**
	 * 축의 실제 위치 및 목표 위치를 지정한다.(AMP Disable 상태에서 수행하는게 좋다.)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bType			: 위치 종류, FALSE=실제위치, TRUE=목표위치
	 * @param	dPosition		: bType=FALSE이면 지정할 실제위치, bType=TRUE이면 지정할 목표위치
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetPosition(short siAxis, BOOL bType, double dPosition) = 0;

	/**
	 * 특정 축의 Encoder Feedback Data를 빠르게 읽어들일 때 사용(50usec 주기 Update)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	bStatus			: 설정 여부, TRUE=설정
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetFastReadEncoder(short siAxis, BOOL bStatus) = 0;

	/**
	 * 사용자가 Motion 관련 S/W를 자체 개발하여 시스템을 동작시킬 수 있도록 지원해주는 기능
	 *
	 * @param	siLen			: 제어대상 축 수, Board의 제어 축수와 일치시킨다. 8축 Board => 8, 4축 Board => 4
	 * @param	*psiAxes		: 제어대상 축 ID를 배열구조로 설정
	 * @param	*plDelt			: 매 Sampling Time(10msec)당 위치증가분 Data
	 * @param	siFlag			: 속도 Profile의 시작과 끝을 알려주는데 사용, 1=동작시작, 2=동작중, 3=동작완료
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID INTERPOLATION FLAG TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetInterpolation(short siLen, short *psiAxes, long *plDelt, short siFlag) = 0;

	/**
	 * 충돌방지 기능을 사용할 Mastr/Slave축 및 충돌방지 거리 및 조건(+, -, >, <)을 설정한다.
	 *
	 * @param	siMasterAx		: Master 축 ID(0 ~ 63)
	 * @param	siSlaveAx		: Slave 축 ID(0 ~ 63)
	 * @param	bAddSub			: 오차 계산, FALSE=(Master현재위치-Slave현재위치),
	 *										 TRUE=(Master현재위치+Slave현재위치)
	 * @param	bNonEqual		: 비교, FALSE=(dPosition < bAddSub결과치),
	 *									TRUE=(dPosition > bAddSub결과치)
	 * @param	dPosition		: 충돌 방지 거리
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetCollisionPrevent(short siMasterAx, short siSlaveAx,
									  BOOL bAddSub, BOOL bNonEqual, double dPosition, BOOL bState) = 0;

	/**
	 * 충돌방지 기능의 사용여부를 설정한다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bMode			: 사용 여부, TRUE=사용
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetCollisionPreventFlag(short siBdNum, BOOL bMode) = 0;

	/**
	 * Board DPRAM Address를 설정한다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	lAddr			: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetDpramAddress(short siBdNum, long lAddr) = 0;

	/**
	 * 절대치 Motor의 Type을 지정한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: Motor 종류, 1=삼성CSDJ, CSDJ+SERVO DRIVE, 2=YASKAWA SERVO DRIVE
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID ABS MOTOR TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAbsEncoderType(short siAxis, short siType) = 0;

	/**
	 * 절대치 Motor를 설정한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetAbsEncoder(short siAxis) = 0;

	/**
	 * Servo Linear Flag 상태를 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siFlag			: Servo Linear Flag 상태
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetServoLinearFlag(short siAxis, short siFlag, BOOL bBootOpt= FALSE) = 0;

	/**
	 * 동기제어 여부를 지정한다.
	 *
	 * @param	bState			: 지정 여부, TRUE=동기제어 실행
	 * @return	Error Code		: 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSyncControl(BOOL bState) = 0;

	/**
	 * 동기제어할 Master축과 Slave축을 지정한다.
	 *
	 * @param	siMasterAx		: Master 축 ID(0 ~ 63)
	 * @param	siSlaveAx		: Slave 축 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSyncMapAxes(short siMasterAx, short siSlaveAx) = 0;

	/**
	 * 동기제어시 적용되는 보상 Gain값을 지정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siCoeff			: 보상 Gain 값
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSyncGain(short siCoeff, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Board별 Sampling Rate를 설정한다.
	 * boot file 또는 실행중인 memory에 저장할 수 있다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siTime			: Sampling Time(msec 단위)(1=4msec, 2=2msec, 3=1msec만 지원)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID SAMPLING RATE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetControlTimer(short siBdNum, short siTime, BOOL bBootOpt = FALSE) = 0;

//#if FALSE	// MMC Library Header File에 없음(MMCWFI30.h)
	/**
	 * PositionIoOnOff()로 설정된 것을 해제한다.
	 *
	 * @param	siPosNum		:(OPTION=0) 위치 번호, 1 ~ 10, 0=모든 위치 해제
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION IO NUMBER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
//	virtual int PositionIOClear(short siPosNum = 0) = 0;
//#endif

//#if FALSE	// MC Library Header File에 없음(MMCWFI30.h)
	/**
	 * 지정 축이 지정된 위치를 지날 때 지정 IO를 출력한다.
	 *
	 * @param	siPosNum		: 위치 번호, 1 ~ 10
	 * @param	siIONum			: I/O 번호, 양의정수=ON, 음의정수=OFF
	 * @param	siAxis			: 축 ID
	 * @param	dPosition		: 지정 축의 위치값
	 * @param	bEncFlag		: Encoder Flag, FALSE=내부위치 Counter 사용, TRUE=외부 Encoder 사용
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION IO NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
//	virtual int PositionIoOnoff(short siPosNum, short siIONum, short siAxis, double dPosition, BOOL bEncFlag) = 0;
//#endif

	/**
	 * 직선 동작 혹은 직선, 원, 원호등의 동작 시 속도와 가,감속도를 지정한다.
	 *
	 * @param	dVelocity		: 속도
	 * @param	siAccel			: 가,감속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetMoveSpeed(double dVelocity, short siAccel) = 0;

	/**
	 * 자동 가,감속 기능의 사용여부를 지정한다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bState			: 사용여부, FALSE=사용
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetSplAutoOff(short siBdNum, BOOL bState) = 0;

	/**
	 * 자동 가,감속 기능의 사용여부를 읽는다.
	 * Library에 제공되는 함수가 없는 관계로 설정 Data에서 읽어온다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbState		: 사용여부, TURE=사용
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo GetSplAutoOff(short siBdNum, BOOL *pbState) = 0;

	/**
	 * 축 이동을 정지한다.
	 * 일반정지, 비상정지, 속도이동정지를 제공한다.
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siType			: 정지 종류, 0=STOP, 1=ESTOP, 2=VSTOP
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID STOP TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetStop(short siAxis, short siType, double fDecel, double fJerk) = 0;

	/**
	 * 1축 속도 Profile 이동(축 1개 단위만 이동 가능함)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	dPosition		: 이동할 위치, 혹은 상대거리
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @param	siDecel			: 이동 감속도(비대칭(t) Type만 적용)
	 * @param	siType			: 이동 Type, 0=사다리꼴 속도 Profile, 절대좌표 이동
	 *										 1=S-Curve 속도 Profile, 절대좌표 이동
	 *										 2=사다리꼴 속도 Profile, 상대거리 이동
	 *										 3=S-Curve 속도 Profile, 상대거리 이동
	 *										 4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
	 *										 5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
	 *										 6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
	 *										 7=비대칭 S-Curve 속도 Profile, 상대거리 이동
	 * @param	bWaitOpt		:(OPTION=FALSE) 이동 완료 대기 여부, TRUE=이동완료될때까지대기
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo Move(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, short siType, BOOL bWaitOpt = FALSE, double dJerk = 0.0) = 0;

	/**
	 * 다축 속도 Profile 동시 이동(상대거리 이동은 지원하지 않는다.)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 속도 #1 | 속도 #2 | 속도 #3 | ... | 속도 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 가속 #1 | 가속 #2 | 가속 #3 | ... | 가속 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 감속 #1 | 감속 #2 | 감속 #3 | ... | 감속 #n |
	 *		+---------+---------+---------+-----+---------+
	 *
	 * @param	siLen			: 축 수, >0
	 * @param	*psiAxis		: 축 ID(축 수 만큼 존재)
	 * @param	*pdPosition		: 이동할 위치, 혹은 상대거리(축 수 만큼 존재)
	 * @param	*pdVelocity		: 이동 속도(축 수 만큼 존재)
	 * @param	*psiAccel		: 이동 가속도(축 수 만큼 존재)
	 * @param	*psiDecel		: 이동 감속도(축 수 만큼 존재)
	 * @param	siType			: 이동 Type, 0=사다리꼴 속도 Profile, 절대좌표 이동
	 *										 1=S-Curve 속도 Profile, 절대좌표 이동
	 *										 4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
	 *										 5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
	 * @param	bWaitOpt		:(OPTION=FALSE) 이동 완료 대기 여부, TRUE=이동완료될때까지대기
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo MoveAll(short siLen, short *psiAxes, double *pdPosition, double *pdVelocity,
						  short *psiAccel, short *psiDecel, short siType, BOOL bWaitOpt = FALSE) = 0;

	/**
	 * 지정된 n축이 주어진 좌표값만큼 직선이동을 한다.(다른 Board의 축 사용 불가)
	 *  MapAxes(), SetMoveSpped()에서 지정된 축들이 이동한다.
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *
	 * @param	siLen			: 축 수, 2 <= siLen <= 8
	 * @param	*pdPosition		: 이동할 좌표값(축 수 만큼 존재)
	 * @param	siType			: 이동 Type, 0=사다리꼴 속도 Profile, 절대좌표 이동
	 *										 1=S-Curve 속도 Profile, 절대좌표 이동
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo MoveN(short siLen, double *pdPosition, short siType) = 0;

	/**
	 * 지정된 n축이 주어진 좌표값만큼 직선이동을 한다.(다른 Board의 축 사용 가능)
	 *
	 *		|-------------------> siLen <-----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siLen			: 축 수, >=2
	 * @param	*psiAxes		: 축 ID 배열
	 * @param	*pdPosition		: 이동할 좌표값(축 수 만큼 존재)
	 * @param	siType			: 이동 Type, 0=사다리꼴 속도 Profile, 절대좌표 이동
	 *										 1=S-Curve 속도 Profile, 절대좌표 이동
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo MoveNAx(short siLen, short *psiAxes, double *pdPosition, short siType, double dVelocity, short siAccel) = 0;

	/**
	 * 지정된 그룹의 축들이 주어진 좌표값만큼 직선이동을 한다.
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siGrpNum		: Group 번호, 1 ~ 4
	 * @param	siLen			: 축 수, 2 <= siLen <= 4
	 * @param	*psiAxes		: 축 ID 배열(축 수 만큼 존재)
	 * @param	*pdPosition		: 이동할 좌표값(축 수 만큼 존재)
	 * @param	siType			: 이동 Type, 0=사다리꼴 속도 Profile, 절대좌표 이동
	 *										 1=S-Curve 속도 Profile, 절대좌표 이동
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID GROUP NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo MoveNAxGr(short siGrpNum, short siLen, short *psiAxes, double *pdPosition,
							short siType, double dVelocity, short siAccel) = 0;

	/**
	 * 가속 후 등속 이동한다.(축 1개 단위로만 동작 가능하다.)
	 *
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo VMove(short siAxis, double dVelocity, double dAccel, double dDecel = 0.0, double dJerk = 0.0) = 0;

	/**
	 * 현재 위치에서 주어진 2/3차원 평면상의 좌표점까지 가,감속하면서 원호 CP Motion으로 이동한다.
	 * (다른 Board의 축 사용 불가)
	 *  MapAxes(), SetMoveSpped()에서 지정된 축들이 이동한다.
	 *
	 *		|---------------> siAxNum = n <---------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+-----------+-----------+------+--------+----------+
	 *		| 회전중심X | 회전중심Y | 속도 | 가속도 | 회전방향 |
	 *		+-----------+-----------+------+--------+----------+
	 *
	 * @param	siAxNum			: 축 수, 2=2축, 3=3축
	 * @param	dCenterX		: 회전 중심 X좌표
	 * @param	dCenterY		: 회전 중심 Y좌표
	 * @param	*pdPoint		: 이동할 좌표, 2축이면 2차원배열, 3축이면 3차원배열
	 * @param	dVelocity		: 이동 속도(0.0=SetMoveSpeed()에서 지정한 속도 사용)
	 * @param	siAccel			: 이동 가속도(0=SetMoveSpeed()에서 지정한 가,감속도 사용)
	 * @param	bDir			: 회전방향, FALSE=CIR_CW(시계방향), TRUE=CIR_CCW(반시계방향)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplArcMove(short siAxNum, double dCenterX, double dCenterY,
							 double *pdPoint, double dVelocity, short siAccel, BOOL bDir) = 0;

	/**
	 * 현재 위치에서 주어진 좌표점까지 가,감속하면서 원호 CP Motion으로 이동한다.
	 * (다른 Board의 축 사용 가능)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+-----------+-----------+------+--------+----------+
	 *		| 회전중심X | 회전중심Y | 속도 | 가속도 | 회전방향 |
	 *		+-----------+-----------+------+--------+----------+
	 *
	 * @param	siLen			: 축 수(>= 2)
	 * @param	*psiAxes		: 축 ID 배열(축 수만큼 존재)
	 * @param	siCenterX		: 회전 중심 X좌표
	 * @param	siCenterY		: 회전 중심 Y좌표
	 * @param	*pdPoint		: 이동할 좌표(축 수 만큼 존재)
	 * @param	dVelocity		: 이동 속도(0.0=SetMoveSpeed()에서 지정한 속도 사용)
	 * @param	siAccel			: 이동 가속도(0=SetMoveSpeed()에서 지정한 가,감속도 사용)
	 * @param	bDir			: 회전방향, FALSE=CIR_CW(시계방향), TRUE=CIR_CCW(반시계방향)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY,
								double *pdPoint, double dVelocity, short siAccel, BOOL bDir) = 0;

	/**
	 * 현재 위치에서 주어진 좌표점까지 가,감속하면서 원호 CP Motion으로 이동한다.
	 * (다른 Board의 축 사용 가능)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+-----------+-----------+------+--------+----------+
	 *		| 회전중심X | 회전중심Y | 속도 | 가속도 | 회전방향 |
	 *		+-----------+-----------+------+--------+----------+
	 *
	 * MMC Library : spl_arc_move2ax(), spl_arc_move3ax(), spl_arc_movenax()
	 *
	 * @param	siLen			: 축 수(>= 2)
	 * @param	*psiAxes		: 축 ID 배열(축 수만큼 존재)
	 * @param	siCenterX		: 회전 중심 X좌표
	 * @param	siCenterY		: 회전 중심 Y좌표
	 * @param	*pdPoint		: 이동할 좌표(축 수 만큼 존재)
	 * @param	dVelocity		: 이동 속도(0.0=SetMoveSpeed()에서 지정한 속도 사용)
	 * @param	siAccel			: 이동 가속도(0=SetMoveSpeed()에서 지정한 가,감속도 사용)
	 * @param	bDir			: 회전방향, FALSE=CIR_CW(시계방향), TRUE=CIR_CCW(반시계방향)
	 * @param	bAutoFlag		: 자동 Auto CP, FALSE=Auto CP 안함, TRUE=Auto CP 함
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplAutoArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY,
								  double *pdPoint, double dVelocity, short siAccel, BOOL bDir, BOOL bAutoFlag) = 0;

	/**
	 * 현재 위치에서 주어진 2/3차원 평면상의 좌표점까지 가,감속하면서 직선 CP Motion으로 이동한다.
	 * (다른 Board의 축 사용 불가)
	 *  MapAxes(), SetMoveSpped()에서 지정된 축들이 이동한다.
	 *
	 *		|---------------> siAxNum = n <---------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siAxNum			: 축 수, 2=2축, 3=3축
	 * @param	*pdPoint		: 이동할 좌표, 2축이면 2차원배열, 3축이면 3차원배열
	 * @param	dVelocity		: 이동 속도(0.0=SetMoveSpeed()에서 지정한 속도 사용)
	 * @param	siAccel			: 이동 가속도(0=SetMoveSpeed()에서 지정한 가,감속도 사용)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplLineMoveN(short siAxNum, double *pdPoint, double dVelocity, short siAccel) = 0;

    /**
     * 현재 위치에서 주어진 좌표점까지 가,감속하면서 직선 CP Motion으로 이동한다.
	 * (다른 Board의 축 사용 가능)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siLen			: 축 수
	 * @param	*psiAxes		: 축 ID 배열
	 * @param	*pdPoint		: 이동할 좌표(축 수 만큼 존재)
	 * @param	dVelocity		: 이동 속도, 0.0=기지정된 속도로 이동
	 * @param	siAccel			: 이동 가속도, 0=기지정된 가속도로 이동
	 * @return	Error Code		: 0 = SUCCESS, 그 외 = Error
	 */
	virtual ErrorInfo SplLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel) = 0;

    /**
     * 현재 위치에서 주어진 좌표점까지 자동 가,감속 여부를 인자로 설정받아서 직선 CP Motion으로 이동한다.
	 * (다른 Board의 축 사용 가능)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siLen			: 축 수
	 * @param	*psiAxes		: 축 ID 배열
	 * @param	*pdPoint		: 이동할 좌표(축 수 만큼 존재)
	 * @param	dVelocity		: 이동 속도, 0.0=기지정된 속도로 이동
	 * @param	siAccel			: 이동 가속도, 0=기지정된 가속도로 이동
	 * @param	bAutoFlag		: 자동 Auto CP, FALSE=Auto CP 안함, TRUE=Auto CP 함
	 * @return	Error Code		: 0 = SUCCESS, 그 외 = Error
	 */
	virtual ErrorInfo SplAutoLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel, BOOL bAutoFlag) = 0;

	/**
	 * 원, 원호 이동 시 원주속도를 지정한다.
	 *
	 * @param	dDegree			: 원주속도, 0 < dDegree < 1000.0
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID ARC DIVISION DEGREE NUMBER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SetArcDivision(double dDegree) = 0;

	/**
	 * 주어진 중심에서 지정된 각도만큼 원호를 그리며 동작을 수행한다.
	 * (다른 Board의 축 사용 불가)
	 *  MapAxes(), SetMoveSpped()에서 지정된 축들이 이동한다.
	 *
	 * @param	siCenterX		: 회전 중심 X좌표
	 * @param	siCenterY		: 회전 중심 Y좌표
	 * @param	dAngle			: 회전 각도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo Arc2(double dXCenter, double dYCenter, double dAngle) = 0;

	/**
	 * 주어진 중심에서 지정된 각도만큼 원호를 그리며 동작을 수행한다.
	 * (다른 Board의 축 사용 불가)
	 *
	 * @param	siAxis1			: 축1 ID(0 ~ 63)
	 * @param	siAxis2			: 축2 ID(0 ~ 63)
	 * @param	siCenterX		: 회전 중심 X좌표
	 * @param	siCenterY		: 회전 중심 Y좌표
	 * @param	dAngle			: 회전 각도
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo Arc2Ax(short siAxis1, short siAxis2, double dXCenter, double dYCenter,
						 double dAngle, double dVelocity, short siAccel) = 0;

	/**
	 * 주어진 사각형의 가로와 세로의 길이를 이용하여 현재위치에서 상대이동을 하면서 CP Motion으로 사각형을 그린다.
	 *
	 *			+--------------------+ pdPoint(X, Y)
	 *			|					 |
	 *			|					 |
	 *			|					 |
	 *			|					 |
	 *			|					 |
	 *			+--------------------+
	 *		현재위치
	 *
	 * @param	siAxis1			: 축1 ID(0 ~ 63)
	 * @param	siAxis2			: 축2 ID(0 ~ 63)
	 * @param	*pdPoint		: 현재위치와 대각선방향의 X, Y 좌표
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo RectMove(short siAxis1, short siAxis2, double *pdPoint,
						   double dVelocity, short siAccel) = 0;

	/**
	 * 현재위치에서 주어진 위치를 경유하면서 CP Motion으로 이동한다.
	 *
	 *				  |----------------> siLen = n <----------------|
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| 축 ID#2 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| 축 ID#3 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siLen			: 위치 Data 개수, 최대 30
	 * @param	siAxis1			: 축1 ID(0 ~ 63)
	 * @param	siAxis2			: 축2 ID(0 ~ 63)
	 * @param	siAxis3			: 축3 ID(0 ~ 63)
	 * @param	*pdPointX		: X좌표 배열(위치 Data 개수만큼 존재)
	 * @param	*pdPointY		: Y좌표 배열(위치 Data 개수만큼 존재)
	 * @param	*pdPointZ		: Z좌표 배열(위치 Data 개수만큼 존재)
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION DATA NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplMove(short siLen, short siAxis1, short siAxis2, short siAxis3,
						  double *pdPointX, double *pdPointY, double *pdPointZ,
						  double dVelocity, short siAccel) = 0;

	/**
	 * SplMoveX()에 필요한 위치경로를 설정한다.
	 *
	 *				  |----------------> siLen = n <----------------|
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| 축 ID#1 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| 축 ID#2 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| 축 ID#3 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siSplNum		: Spline Motion 번호, 1 ~ 20
	 * @param	siLen			: 이동 경로 수, 1 ~ 500
	 * @param	siAxis1			: 축1 ID(0 ~ 63)
	 * @param	siAxis2			: 축2 ID(0 ~ 63)
	 * @param	siAxis3			: 축3 ID(0 ~ 63)
	 * @param	*pdPoint1		: 1좌표 배열
	 * @param	*pdPoint2		: 2좌표 배열
	 * @param	*pdPoint3		: 3좌표 배열
	 * @param	dVelocity		: 이동 속도
	 * @param	siAccel			: 이동 가속도
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID SPLINE MOTION NUMBER(MOTIONLIB)
	 *							  xx = INVALID MOVE PATH NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplMoveData(short siSplNum, short siLen, short siAxis1, short siAxis2,
							  short siAxis3, double *pdPoint1, double *pdPoint2,
							  double *pdPoint3, double dVelocity, short siAccel) = 0;

	/**
	 * 지정 3축이 Spline Motion으로 SplMoveData()에서 지정한 위치를 경유하면서 연속 이동한다.
	 *
	 * @param	siSplNum		: Spline Motion 번호, 1 ~ 20
	 * @param	siAxis1			: 축1 ID(0 ~ 63)
	 * @param	siAxis2			: 축2 ID(0 ~ 63)
	 * @param	siAxis3			: 축3 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID SPLINE MOTION NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo SplMovex(short siSplNum, short siAxis1, short siAxis2, short siAxis3) = 0;

	/**
	 * 각 Board별 ROM Version을 읽는다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiVersion		: ROM Version, 101 => 1.01
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo VersionCheck(short siBdNum, short *psiVersion) = 0;

	/**
	 * 해당 Error Code의 Error Message를 반환한다.
	 *
	 * @param	siCode			: Error Code
	 * @param	*pcDst			: Error Message, ERR_MAX_ERROR_LEN(80)보다 크거나 같아야 한다.
	 * @return	Error Code		: 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo ErrorMessage(int iCode, char *pcDst) = 0;

	/**
	 * Position Compare Board를 초기화한다.(축 2를 사용할 때는 같은 Board의 축이어야 한다.)
	 *
	 * @param	siIndexSel		: Position Compare할 축, 1 ~ 2
	 * @param	siAxis1			: 축1 ID(0 ~ 63), 동일 Board의 축이어야 한다.
	 * @param	siAxis2			: 축2 ID(0 ~ 63), 동일 Board의 축이어야 한다.(siIndexSel=2일 때만 적용)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION COMPARE INDEXSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo PositionCompareInit(short siIndexSel, short siAxis1, short siAxis2) = 0;

	/**
	 * Position Compare를 설정한다.(Standard Type)
	 *
	 * @param	siIndexSel		: Position Compare시 사용할 축 수, 1 ~ 2
	 * @param	siIndexNum		: Position Compare를 실시할 Index 번호, 1 ~ 8
	 * @param	siBitNo			: 출력할 I/O Bit 번호, 0 ~ 63
	 * @param	siAxis1			: 축1 ID(0 ~ 63), 동일 Board의 축이어야 한다.
	 * @param	siAxis2			: 축2 ID(0 ~ 63), 동일 Board의 축이어야 한다.(siIndexSel=2일 때만 적용)
	 * @param	bLatch			: I/O 출력모드, FALSE=Transparent Mode, TRUE=Latch Mode
	 * @param	siFunction		: Position Compare에 사용할 부등호, 1="=", 2=">", 3="<"
	 * @param	siOutMode		: 지정 I/O의 출력모드, 0=축별 ON/OFF, 1=두축 AND, 2=두축 OR
	 * @param	dPosition		: Position Compare에 사용될 위치 Data(> 0.0)
	 * @param	lTime			: I/O 출력 시간, Transparent Mode일때만 적용, 40usec단위,  최대 5.38sec
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION COMPARE INDEXSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE INDEX NUMBER(MOTIONLIB)
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE FUNCTION TYPE(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE OUT MODE TYPE(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE IO OUT TIME(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo PositionCompare(short siIndexSel, short siIndexNum, short siBitNo, short siAxis1,
								  short siAxis2, BOOL bLatch, short siFunction, short siOutMode,
								  double dPosition, long lTime) = 0;

	/**
	 * Position Compare를 설정한다.(Interval Type)
	 *
	 * @param	bDir			: Position Compare시 +방향으로 이동시 동작시킬 것인지 -방향으로 동작시킬 것인 설정
	 *							   FALSE="+", TRUE="-"
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	siBitNo			: 출력할 I/O Bit 번호(0 ~ 63)
	 * @param	dStartPos		: I/O가 동작될 최초의 Position 값
	 * @param	dLimitPos		: I/O가 동작될 마지막 Position 값
	 * @param	lInterval		: I/O가 반복될 간격을 펄스수 단위로 지정
	 * @param	lTime			: I/O 출력이 지속될 시간, 40sec 단위
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE IO REPEAT TIME(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE IO OUT TIME(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo PositionCompareInterval(BOOL bDir, short siAxis, short siBitNo, double dStartPos, double dLimitPos,
								  long lInterval, long lTime) = 0;

	/**
	 * Position Compare 동작을 할 것인지 여부를 설정한다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bFlag			: FALSE=Position Compare 동작 Disable, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo PositionCompareEnable(short siBdNum, BOOL bFlag) = 0;

	/**
	 * Position Compare의 Index를 초기화한다.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siIndexSel		: 항상 "1"로 설정
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo PositionCompareClear(short siBdNum, short siIndexSel) = 0;

	/**
	 * Position Compare 설정된 축의 Encoder 값을 읽어낸다.
	 *
	 * @param	siIndexSel		: 항상 "1"로 설정
	 * @param	siAxis			: 축 ID(0 ~ 63)
	 * @param	*pdPosition		: Encoder 값
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual ErrorInfo PositionCompareRead(short siIndexSel, short siAxis, double *pdPosition) = 0;

	/**
	* Board별 Sampling Rate를 설정한다.
	* boot file 또는 실행중인 memory에 저장할 수 있다.
	*
	* @param siAxis   : AxisID
	* @param siTime   : Sampling Time(msec 단위) (1=4msec, 2=2msec, 3=1msec만 지원)
	* @param bBootOpt  : (OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
	* @return Error Code  : 0 = SUCCESS
	*         xx = INVALID MOTION BOARD ID (MOTIONLIB)
	*         xx = INVALID SAMPLING RATE (MOTIONLIB)
	*         그 외 = 타 함수 Return Error
	*/
	virtual ErrorInfo SetControlTimerForAx(short siAxis, short siTime, BOOL bBootOpt = FALSE) = 0;

	///////////////////////////////////////
	// RS EtherCat 관련 추가
	///////////////////////////////////////
	

	// 현재 축의 통신이 정상적인지 체크 (펑션모듈인 경우 첫번째 축에서만 체크하면 됨)
	virtual BOOL IsOpMode_NMC(short siAxis) = 0;
	
	// 기어드(동기 제어) 동작 관련 기능.
	//////////////////////////////////////////////
	
	// 기어드(동기 제어) 설정
	virtual ErrorInfo EnableSyncMotion_NMC(int nMasterAxis, int nSlaveAxis, int nRatioNumerator, int nRatioDenominator, double fAccel, double fDecel, double fJerk) = 0;
	
	// 기어드(동기 제어) 해제
	virtual ErrorInfo DisableSyncMotion_NMC(int nSlaveAxis) = 0;
	
	// 기어드(동기 제어)가 활성화 되어 있는지 체크. 반환값이 true일때 싱크가 걸려있는 상태임.
	virtual BOOL IsSyncEnable_NMC(int nSlaveAxis) = 0;

	// 원점 복귀 관련
	virtual ErrorInfo HomeSetConfig(short siAxis, int iHomeMode, BOOL bDirection) = 0;
	virtual ErrorInfo HomeSetVelocity(short siAxis, double dFastVelocity, double dSlowVelocity, double dAccel, double dDecel, double dPosOffset) = 0;
	virtual ErrorInfo HomingStart(short siAxis) = 0;
	virtual ErrorInfo IsHoming(short siAxis, BOOL *bState) = 0;
	virtual ErrorInfo IsHomeEnd(short siAxis, BOOL *bState) = 0;

	// RS NMC 자동 가감속 이동 관련
	/**
     * 현재 위치에서 주어진 좌표점까지 자동 가,감속 여부를 인자로 설정받아서 직선 CP Motion으로 이동한다.
	 *		+---------+---------+---------+-----+---------+
	 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| 속도 | 가속도 |
	 *		+------+--------+
	 *
	 * @param	siAxis			: 축 ID
	 * @param	dPosition		: 목표 위치
	 * @param	dVelocity		: 이동 속도
	 * @param	dAccel			: 이동 가속도
	 * @param	dDecel			: 이동 감속도
	 * @param	iBufferMode		: mcAborting : 자동 가감속, mcBuffered : 이동 완료 후 다음 이동, mcBlendingLow, mcBlendingPrevious, mcBlendingNext, mcBlendingHigh
	 * @return	Error Code		: 0 = SUCCESS, 그 외 = Error
	 */
	virtual ErrorInfo SplAutoLineMove(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, int iBufferMode) = 0;

	// Motion에서 발생된 에러 내역을 String 형식으로 리턴한다.
	virtual CString ReturnCurrentMotionError() = 0;

	//SJ_YYK 160317 Add..
	virtual ErrorInfo SetOriginComplete(short siAxis) = 0;
	virtual ErrorInfo GetOriginCompleteFlag(short siAxis, short *psiState) = 0;
	virtual ErrorInfo ResetOriginComplete(short siAxis) = 0;
};

#endif //IMotionLibNMC_H

// End of File IMotionLibNMC.h
