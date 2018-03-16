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
 *				  �� ������ Motion Library�� ���� Class�� ���� Interface Class�� �����Ѵ�.
 *
 * @author Yong-seog Pahn(dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef IMOTIONLIBNMC_H
#define IMOTIONLIBNMC_H

#include "ICommon.h"

#include "DefMotionStruct.h"

/** This class is defined Motion component interface class of Motion libraries as a abstract class.
 * �� Class�� Motion Library�� ���� Class�� ���� Interface Class�� �����Ѵ�.
 * 
 * Motion Board���� �����ϴ� ��� API Library�� ����� �� �ֵ��� �ϱ� ���� Interface�� �����Ǵ� ��ü�� 
 * Motion Board ������ ���� CMMCLib, Ȥ�� CMEILib Class�̴�.(CMEILib Class�� Class ���¸� ����)
 * �� Interface���� �����Ǵ� API Library�� ���� ���� MMC �Ŵ��� �ִ� ���븸 �����Ǿ��ִ�.
 *(�Ŵ��� ���� API �Լ����� ������ �� �����ڵ� �𸣱� ������...)
 * �� nterface�� ����ϰ� �Ǹ� Board �ʱ�ȭ�� ��Ÿ ������ ���� �� ���� �������� ������� ������� �����Ӱ�
 * ����� �� �����Ƿ� IAxis Interface�� ���������� ���� ��� ���� ��Ҹ� ������ �� �ִ�.
 * �� Interface�� ���� ��ü�� ������ ���� �����ϸ�, ��� ��ü������ �ѹ� �ʱ�ȭ�� �����ϸ� �ٸ� ��ü������
 * ������ �� ������ �����Ǿ� �ִ�.(static ���� ó��)
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
	 * �� ��� ���� ����(�̹� ������� ���� ������� ���� �Ұ�)
	 *
	 * �� ��� ���� ������ Motor Parameter �������� �����ϸ�, ����ڿ� ���� ������ �Ǿ�� �Ѵ�.
	 *
	 * @param	siAxisID		: �� ID
	 * @param	bState			: ��� ����(TRUE=���, FALSE=�̻��)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetUseAxis(int iBoardNo, short siAxisID, BOOL bState) = 0;

	/**
	 * �� ��� ���� �б�
	 *
	 * @param	siAxisID		: �� ID
	 * @param	*pbState		: ��� ����(TRUE=���, FALSE=�̻��)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetUseAxis(short siAxisID, BOOL *pbState) = 0;

	/**
	 * Motor Parameter ����(Board �ʱ�ȭ �� ��� ����)
	 *
	 * ���޵� SMotionAxis�� �������� Motion�� �����Ѵ�.(�� 1�� ������ ����)
	 *
	 * 1. Board�� �ʱ�ȭ �ȵǾ� ������ Error Return
	 * 2. �� AMP Enable(Servo ON)�̸� Disable(OFF) �� �۾� ����
	 * 3. Motor ���� ����
	 * 4. Feedback Device ����
	 * 5. Loop ���� ����
	 * 6. Motor ������ ����
	 *		6.1 �ӵ��� Servo�� ���
	 *			���� ���� ��� ���� ����
	 *		6.2 �Ϲ� Stepper�� ���
	 *			�޽� ���ֺ�� ���ڱ��� ����
	 *		6.3 Micro Stepper Ȥ�� ��ġ�� Servo�� ���
	 *			�޽� ���ֺ�(default:8)�� ���ڱ���(default:1.0) ����
	 * 7. ��� �޽� ���� ����
	 * 8. Encoder�� ��ǥ ���� ����
	 * 9. AMP Enable, Fault, Reset ����
	 * 10. Home, Positive, Negative Sensor ����
	 * 11. Positive, Negative S/W Limit ����
	 * 12. In-Position ����
	 * 13. �� AMP Enable(Servo ON)�̾����� Enable(ON) ����
	 *
	 * @param	siAxisID		: �� ID, -1=���ȵ�
	 * @param	mAx				: Motor Parameter
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ������� ����, TRUE=boot file�� ���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  xx = MOTION BOARD NO INITIALIZED(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetMotorParam(short siAxisID, SMotionAxis mAx, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Motor Parameter �б�
	 *
	 * @param	siAxisID		: �� ID, -1=All Motor
	 * @param	*pmAx			: Motor Parameter, siAxisID=-1�̸� �迭 ������ ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetMotorParam(short siAxisID, SMotionAxis *pmAx) = 0;


	/**
	 * Motion Board Parameter ����
	 *
	 * @param	iObjectID		: Object ID
	 * @param	MotionBd		: Motion Board Parameter
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log LEvel(None, Normal, Warning, Error)
	 * @param	iDays			: Log ���� �� ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetBoardParam(SMotionBoard MotionBd) = 0;

	/**
	 * Motion Board Parameter �б�
	 *
	 * @param	*pmBd			: Motion Board Parameter
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetBoardParam(SMotionBoard *pmBd) = 0;

	/**
	 * ������ Board ���� �б�
	 *
	 * @param	*psiBdNum		: Board ���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetBoardNum(short *psiBdNum) = 0;

	/**
	 * Motion Board �ʱ�ȭ
	 * 
	 * 1. Motion Board �ʱ�ȭ�� ����� ���
	 * 	(1) Board ���� ���� Board DPRAM Address�� ���� ���
	 * 		�� Board �ʱ�ȭ ���� Pass
	 * 	(2) Board ���� ���� �ٸ� ���
	 * 		�� Board �ʱ�ȭ ���� ó��
	 * 	(3) Board DPRAM Address�� �ٸ� ���
	 * 		�� Board �ʱ�ȭ ���� ó��
	 * 
	 * 2. Motion Board �ʱ�ȭ�� ������� ���� ���
	 * 	�� Board �ʱ�ȭ ����
	 *
	 * @param	siBdNum			: Motion Board ��(1 ~ 4)
	 * @param	*plAddress		: Board DPRAM Address(Board ����ŭ)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = NOT EQUAL MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = NOT EQUAL MOTION DPRAM ADDRESS(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Initialize(short siBdNum, long *plAddress) = 0;

	/**
	 * Motion Board �ʱ�ȭ ���� �б�
	 *
	 * @param	*pbInit			: Board �ʱ�ȭ ����, TRUE=�ʱ�ȭ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetBoardInit(BOOL *pbInit) = 0;

	/**
	 * Motion Board�� ���� �� ���� �����ش�.
	 *
	 * @param	siBdNum			: Motion Board ID(0 ~ 7), -1=All Board
	 * @param	*psiAxes		: Board�� ������ ���� �� ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAxes(short siBdNum, short *psiAxes) = 0;

	/**
	 * ����, ��, ��ȣ���� ������ ������ �� ��ǥ���� ���� ����
	 * ���� Board�� ������ �����ؾ� �Ѵ�.
	 *
	 * @param	siAxNum			: �� ��(1 ~ 8)
	 * @param	*psiMapArray	: �� ID �迭(���� Board���� �� ID�̾�� �Ѵ�.)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = NO COMMON MOTION BOARD AXIS(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo MapAxes(short siAxNum, short *psiMapArray) = 0;

	/**
	 * ����, ��, ��ȣ���� ������ ������ �� ��ǥ���� ���� ����
	 * Motion ���α׷� ������ �� ���� ���� ���� ���� 2������ �����Ѵ�.
	 * �����ð��� 1msec �����̸� 0���� Ŀ�� �Ѵ�.
	 *
	 * @param	siAxis			: �� ID(-1 ~ 63), -1=���α׷�����
	 * @param	*plDuration		: �����ð�(1msec����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID DELAY DURATION(<=0)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Dwell(short siAxis, long *plDuration) = 0;

	/**
	 * I/O Bit�� ������ ���·� �� ������ �ش� ���� ���� ���� ������ �����Ѵ�.
	 * I/O Bit No�� Board ���� ���¿� ���� 64�� ������ �����ȴ�.
	 * (Board#1 : 0 ~ 63, Board #2 : 64 ~ 63, Board #3 : 64 ~ 95, Board #4 : 96 ~ 127)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siBitNo			: I/O Bit No
	 * @param	bState			: I/O Bit ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID BIT IO NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo IOTrigger(short siAxis, short siBitNo, BOOL bState) = 0;

	/**
	 * �� ��� ���࿡ ���� ���¸� �����ش�.
	 * ��� ���� �Ϸ�� �����̸� SUCCESS�� Return�Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: ��ȸ ����, 0=�̵����, 1=�ӵ����, 2=InPosition ������ ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID IN COMMAND TYPE(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo InCommand(short siAxis, BOOL *bState, short siType=0) = 0;

	/**
	 * ����, ��, ��ȣ���� ������ �Ϸ�Ǿ����� ���θ� �����ش�.
	 * MapAxes()���� ������ ��鿡 ���� Ȯ���Ѵ�.
	 *
	 * @param	*pbStatus		: �Ϸ� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo AllDone(BOOL *pbStatus) = 0;

	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.(����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
	 *
	 * MMC Library : axis_done()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsAxisDone(short siAxis) = 0;
	
	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.(����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�. InPosition Ȯ�� ����
	 *
	 * MMC Library : axis_done()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsMotionDone(short siAxis) = 0;

	/**
	 * ���� ���� ���� �Ϸ�� ������ ��ٸ���.
	 * *psiAxis�� NULL�� ���޵Ǹ� siAxis�� �� ID�� �νĵǾ� �����Ѵ�.
	 * *psiAxis�� NULL�� �ƴѰɷ� ���޵Ǹ� siAxis�� �� ���� �νĵǾ� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63) Ȥ�� �� ��(1 ~ 64)
	 * @param	*psiAxis		:(OPTION=NULL) �� ID �迭, siAxis�� �� ���� ��� ���
	 *																 �� ID��� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo WaitForDone(short siAxis) = 0;

	/**
	 * ���� ���� AMP Fault�� Clear�ϰų� Fault Port�� Enable ���·� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bState			: ������ ����, FALSE=Clear, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpFaultEnable(short siAxis, BOOL bState) = 0;

	/**
	 * �� ��� ���� ���¸� �Ѳ����� �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiState		: ����,(axis_source, in_sequence, get_com_velocity, get_act_velocity,
	 *									 motion_done, in_position, axis_done �����)
	 * @param	*plStatus		: ����,(get_io �����)
	 * @param	*pdStatus		: ����,(get_position, get_command, get_error �����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAllStatus(short siAxis, short *psiStatus, long *plStatus, double *pdStatus) = 0;

	/**
	 * �ص����� ������ �ӵ���ɰ��� PULSE������ �����ش�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Puls
	 */
	virtual ErrorInfo GetComVelocity(short sAxis) = 0;

	/**
	 * �� ���� ���¸� �д´�.
	 * ���´� bit �������� �����Ǿ� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiState		: ����, �� ���´� bit �������� �����ȴ�.
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAxisSource(short siAxis, short *psiState) = 0;

	/**
	 * ���� ����(Home, Positive, Negative) ���¸� �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: ���� ����, 0=Home, 1=Positive, 2=Negative
	 * @param	*pbState		: ���� ����, TRUE=Active, FALSE=No Active
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 */
	virtual ErrorInfo GetSensorStatus(short siAxis, short siType, BOOL *pbState) = 0;

	/**
	 * AMP Disable/Enable ���¸� �д´�.(Servo ON/OFF)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbState		: AMP Enable ����, TRUE=Enable, FALSE=Disable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpEnable(short siAxis, BOOL *pbState) = 0;

	/**
	 * AMP Enable�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Enable Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpEnableLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� Event �߻� ���¸� �д´�.
	 *
	 *		NO_EVENT		0		Event �߻����� ���� ����
	 *		STOP_EVENT		1		stop_rate�� �����ϸ鼭 ����
	 *		E_STOP_EVENT	2		e_stop_rate�� �����ϸ鼭 ����
	 *		ABORT_EVENT		3		AMP disable ����
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiState		: Event ����, 0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	//virtual int GetAxisState(short siAxis, short *psiState) = 0;
	virtual ErrorInfo GetAxisState(short siAxis, UINT32 *psiState) = 0; //170713 JSH

	/**
	 * Board�� Position Latch ���θ� �д´�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbState		: Position Latch ����, TRUE=Latch����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAxisLatchStatus(short siBdNum, BOOL *pbState) = 0;

	/**
	 * �࿡ �߻��� Event�� �����ϰ�, ���� ��ɺ��� �����Ѵ�.
	 * Event �߻� �Ŀ��� �׻� Event�� ������ �־�� �Ѵ�.
	 * ABORT_EVENT �߻� �ÿ��� Event ���� �� AMP�� Disable �����̹Ƿ� �ٽ� Enable���־�� �Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo ClearStatus(short siAxis) = 0;

	/**
	 * ���� Frame Buffer�� Clear�Ѵ�.
	 * �ະ �ִ� 50���� Frame�� ������ Clear�Ѵ�. �� ���� �������� ����� ��� ����ȴ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo ClearFrames(short siAxis) = 0;

	/**
	 * ���� ����ִ� Interpolation Frame ������ �����ش�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiFrameNo		: Frame ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo FramesInterpolation(short siAxis, short *psiFrameNo) = 0;

	/**
	 * ���� ����ִ� Frame ������ �����ش�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiFrameNo		: ����ִ� Frame ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo FramesLeft(short siAxis, short *psiFrameNo) = 0;

	/**
	 * �ش� Board�� Latch ���¸� �����ϰ�, Latch Status�� False�� ����ų�, S/W������ Position�� Latch�Ѵ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bType			: Latch ����, FALSE=S/W Position Latch, TRUE=Board Latch Enable/Disable ����
	 * @param	bState			:(OPTION=FALSE) bType=TRUE�� ��� Enable/Disable ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Latch(short siBdNum, BOOL bType, BOOL bState = FALSE) = 0;

	/**
	 * ���� ���� Latch�� Position�� �����ش�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdPosition		: Latch�� Position
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetLatchedPosition(short siAxis, double *pdPosition) = 0;

	/**
	 * ������ ��ǥ��ġ�� �������� �� ����Ѵ�.(��, ��ȣ �����߿��� ������� �ʴ´�.)
	 *
	 * @param	siLen			: �� ��
	 * @param	*psiAxes		: �� ID �迭
	 * @param	*pdDist			: ��ġ ������
	 * @param	*psiAccel		: �̵� ��,���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(<=0)(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo CompensationPos(short siLen, short *psiAxes, double *pdDist, short *psiAccel) = 0;

	/**
	 * Board DPRAM Address�� �д´�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*plAddr			: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetDpramAddress(short siBdNum, long *plAddr) = 0;

	/**
	 * ����ġ Motor�� Type�� �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiType		: Motor ����, 1=�ＺCSDJ, CSDJ+SERVO DRIVE, 2=YASKAWA SERVO DRIVE
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAbsEncoderType(short siAxis, short *psiType) = 0;

	/**
	 * ���� �̵� �ְ�ӵ��� ��,���� �������� ���Ѱ��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdVelocity		: �̵� �ְ�ӵ�, 1 ~ 2047000 coutn/sec
	 * @param	*psiAccel		: ��,���� ������, 1 ~ 200, 10msec ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetVelLimit(short siAxis, double *pdVelocity, short *psiAccel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Drive�� Fault �߻� ���¸� �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbStatus		: AMP Fault ���¸� �д´�. TRUE=FAULT, FALSE=NORMAL
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual ErrorInfo GetAmpFaultStatus(short siAxis, BOOL *pbStatus) = 0;

	/**
	 * AMP Drive�� Fault �߻� �� ������ Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiAction		: ������ Event, NO EVENT, STOP EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpFaultEvent(short siAxis, short *psiAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Enable�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Enable ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpEanbleLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Fault�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Fault ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpFaultLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Reset�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Reset ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpResetLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� AMP Drive�� Resolution�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiResolution	: AMP Resolution, default=2500 pulse/rev
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAmpResolution(short siAxis, short *psiResolution, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� ���ֺ� ���� ���ڰ�, �и��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiRatioA		: Encoder ���ֺ� ���ڰ�
	 * @param	*psiRatioB		: Encoder ���ֺ� �и�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID DENOMINATOR(<1)(MOTIONLIB)
	 *							  xx = INVALID MOLECULE(<1)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetEncoderRatio(short siAxis, short *psiRatioA, short *psiRatioB, BOOL bBootOpt= FALSE) = 0;

	/**
	 * ���� ���� ȸ��/������ϴ� ����ȸ�� ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbStatus		: ����ȸ�� �� ��������
	 * @param	bType			: � ����, FALSE=����, TRUE=ȸ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetEndlessAx(short siAxis, BOOL *pbStatus, BOOL bType, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ����ȸ�� ���� �����̴� ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdRange		: �̵� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetEndlessRange(short siAxis, double *pdRange, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ��ġ���� �Ϸᰪ�� ��ġ���� �� ��ȣ Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdInPosition	: ��ġ ������
	 * @param	*pbLevel		: ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetInPosition(short siAxis, double *pdInPosition, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� InPosition ��ȣ ��뿩�θ� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbReq			: ��� ����, TRUE=���
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetInpositionRequired(short siAxis, BOOL *pbReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ��ġ���� ���Ѱ��� Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdLimit		: ��ġ���� ���Ѱ�, �ִ� 35000 count
	 * @param	*psiAction		: ��ġ���� Event, NO EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetErrorLimit(short siAxis, double *pdLimit, short *psiAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� �� Encoder�� C�� �޽� �̿� ���θ� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbIndexReq		: C�� �޽� ��� ����, TRUE=Home Sensor�� Encoder�� Index Pulse�� ���� ����,
	 *												  FALSE=Home Sensor�� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetIndexRequired(short siAxis, BOOL *pbIndexReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * I/O 8���� ����  ��,��� ��带 �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbMode			: ��, ��� ���, TRUE=���, FALSE=�Է�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetIOMode(short siBdNum, BOOL *pbMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ Event�� ��ȣ Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
	 * @param	*psiLimit		: ������ Event
	 * @param	*pbLevel		: ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSensorLimit(short siAxis, short siType, short *psiLimit, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
	 * @param	*psiLimit		: ������ Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSensorEvent(short siAxis, short siType, short *psiLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ ��ȣ Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
	 * @param	*pbLevel		: ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSensorLevel(short siAxis, short siType, BOOL *pbLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * +/- �������� Motor�� �̵��� �� �ִ� ���� ��ġ���� �� ��ġ���� �������� �� ������ Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: �̵� ����, TRUE=Positive, FALSE=Negative
	 * @param	*pdPosition		: ���� ��ġ��
	 * @param	*psiLimit		: ������ Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSWLimit(short siAxis, BOOL bType, double *pdPosition, short *psiLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� � Motor�� �����ϴ� ������ �����Ǿ� �ִ��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiType		: Motor ����, 0=�ӵ���Servo, 1=�Ϲ�Stepper, 2=MicroStepper Ȥ�� ��ġ��Servo
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetMotorType(short siAxis, short *psiType, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� Feedback ��ġ�� Loop ���¸� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiDevice		: Feedback ��ġ, 0=Encoder�Է�, 1=0~10volt�Է�, 2=-10~10volt�Է�
	 * @param	*pbLoop			: Loop ����, FALSE=Open Loop, TRUE=Closed Loop
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAxisProperty(short siAxis, short *psiDevice, BOOL *pbLoop, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� Pulse ���ֺ�� ���ڱ��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiPgratio		: Pulse ���ֺ�
	 * @param	*pdEgratio		: ���ڱ���
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetRatioProperty(short siAxis, short *psiPgratio, double *pdEgratio, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ӵ��� Servo�� ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbControl		: ������, FALSE=�ӵ�����, TRUE=��ġ����
	 * @param	*pbPolar		: Analog ��� ����, FALSE=UNIPOLAR, TRUE=BIPOLER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS,
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetVServoProperty(short siAxis, BOOL *pbControl, BOOL *pbPolar, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� Pulse ��� ���¸� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbMode			: Pulse ��� ����, FALSE=Two Pulse(CW+CCW), TRUE=Sign+Pulse
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetStepMode(short siAxis, BOOL *pbMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� Encoder �Է� ����� ��ǥ ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbEncDir		: Encoder �Է� ����, FALSE=ENCO_CW(�ð����, - Count)
	 *												 TRUE =ENCO_CCW(�ݽð����, + Count)
	 * @param	*pbCoorDir		: ��ǥ����, FALSE=CORD_CW(�ð����, + ��ǥ �̵�)
	 *										TRUE =CORD_CCW(�ݽð����, - ��ǥ �̵�)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetEncoderDirection(short siAxis, BOOL *pbEncDir, BOOL *pbCoorDir, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ������ ���� STOP EVENT, ESTOP EVENT ���� �� ���� �ð��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ���� ����, FALSE=STOP, TRUE=E-STOP
	 * @param	*psiRate		: ���� �ð�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetStopRate(short siAxis, BOOL bType, short *psiRate, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ��������� ����Ǵ� ���� Gain���� �д´�.
	 *
	 * MMC Library : get_sync_gain()
	 *
	 * @param	*plCoeff		: ���� Gain ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSyncGain(long *plCoeff) = 0;

	/**
	 * �ش� ���� �ӵ� �Ǵ� ��ġ�� ���� PID & FF Gain������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_gain(), fget_gain(), get_v_gain(), fget_v_gain()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bVelType		: ��ġ/�ӵ� ���� ����, FALSE=��ġ, TRUE=�ӵ�
	 * @param	*plGain			: Gain �� �迭, �迭�μ���ġ�� �Ʒ��� ����.
	 *								0=GA_P, 1=GA_I, 2=GA_D, 3=GA_F, 4=GA_LIMIT, 5=GAIN_MUNBER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� �������� �� �������� ��带 �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ������, FALSE=��ġ, TRUE=�ӵ�
	 * @param	*pbMode			: �������� ���, FALSE=����������, TRUE=�׻�����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetIntegration(short siAxis, BOOL bType, BOOL *pbMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ӵ����� Ȥ�� ��ũ ���ɿ� ���� Low Pass Filter Ȥ�� Notch Filter�� ���� Filter ���� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bCommandType	: ���� ����, FALSE=�ӵ�(Position), TRUE=��ũ(Velocity)
	 * @param	bFilterType		: Filter ����, FALSE=LowPass, TRUE=Notch
	 * @param	*pdFilter		: Filter ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double *pdFilter,
							BOOL bBootOpt= FALSE) = 0;

	/**
	 * �ش� ���� ���� �� �ӵ��� �д´�.(��� ���� ���� ��)
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: �ӵ� ����, FALSE=�����ӵ���, TRUE=�ӵ���ɰ�
	 * @param	*psiPulse		: �ӵ��� Pulse��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetVelocity(short siAxis, BOOL bType, double *pdPulse) = 0;

	/**
	 * ������ Board�� �ະ ���ۿ��θ� �д´�.
	 *
	 *		b7	b6	b5	b4	b3	b2	b1	b0
	 *		��8	��7	��6	��5	��4	��3	��2	��1
	 *
	 *		bit = TRUE : ���� ����
	 *		bit = FALSE : ���� ����
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiState		: �ະ ���� ����, bit�� �� ��(b0=��1, b1=��2, ...), TRUE=����, FALSE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAxisRunStop(short siBdNum, short *psiState) = 0;

	/**
	 * ���� ������ġ �� ��ǥ��ġ�� �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bCommandType	: ��ġ ����, FALSE=������ġ(Encode), TRUE=��ǥ��ġ(Command)
	 * @param	*pdPosition		: bType=FALSE�̸�, ������ ������ġ(Encode)
	 *							  bType=TRUE �̸�, ������ ��ǥ��ġ(Command)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetPosition(short siAxis, BOOL bCommandType, double *pdPosition) = 0;

	/**
	 * Motor�� ����ġ RPM�̳� ���� RPM�� �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: RMP ����, FALSE=����RPM, TRUE=����ġRPM
	 * @param	*psiRpm			: RPM��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetRpm(short siAxis, BOOL bType, short *psiRpm) = 0;

	/**
	 * Board�� Sampling Rate�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiTime		: Sampling Rate, msec����(1=4msec, 2=2msec, 3=1msec�� ����)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetControlTimer(short siBdNum, short *psiTime, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ��ǥ��ġ�� ������ġ�� ���̰��� ��ġ������ �д´�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdError		: ��ġ����,(��ǥ��ġ-������ġ)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetError(short siAxis, double *pdError) = 0;

	/**
	 * Ư�� ���� Encoder Feedback Data�� ������ �о���� �� ���(50usec �ֱ� Update)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbStatus		: ���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetFastReadEncoder(short siAxis, BOOL *pbStatus) = 0;

	/**
	 * �ش� ���� Analog Offset ���� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiOffset		: Analog Offer, +/-2048, +/-64767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAnalogOffset(short siAxis, short *psiOffset, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ��, ��� Port�� 64bit Data�� �д´�.
	 *
	 * @param	siPort			: ��, ��� Port ��ȣ(0 ~ 3, Board ���� ������ ���� ����)
	 * @param	bType			: ��, ��� ����, FALSE=�Է�, TRUE=���
	 * @param	*plValue		: 64bit Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO PORT ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetIO(short siPort, BOOL bType, long *plValue) = 0;

	/**
	 * ������ Analog ��/����� 12/16bit Data ���� �д´�.
	 *
	 * @param	siChannel		: Analog �Է� ä�� ��(0 ~ 7) Ȥ�� ��� �� ID(0 ~ 63)
	 * @param	bType			: ��, ��� ����, FALSE=�Է�, TRUE=���
	 * @param	*psiValue		: bType=FALSE�̸� Analog �Է� ��, -2048 ~ +2047
	 *							  bType=TRUE�̸� Analog ��� ��, +/-2048, +/-64767
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID ANALOG INPUT CHANNEL ID(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetAnalog(short siChannel, BOOL bType, short *psiValue) = 0;

	/**
	 * Board�� �浹���� ����� ��뿩�θ� �д´�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbMode			: ��뿩��, TRUE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetCollisionPreventFlag(short siBdNum, BOOL *pbMode) = 0;

	/**
	 * �������� ���θ� �д´�.
	 *
	 * @param	*pbState		: ���� ����, TRUE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSyncControl(short siAxis, BOOL *pbState) = 0;

	/**
	 * Master��� Slave���� ������ġ�� �д´�.
	 *
	 * @param	*pdMasterPos	: Master �� ��ġ
	 * @param	*pdSlavePos		: Slave �� ��ġ
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSyncPosition(double *pdMasterPos, double *pdSlavePos) = 0;

	/**
	 * ���庰�� I/O Interrupt�� Enable/Diable�ϰų�, I/O Interrupt �߻� ��
	 * STOP-EVENT�� E-STOP-EVENT�� �����࿡ �߻����� ���θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siID			: Board(0 ~ 7) Ȥ�� �� ID(0 ~ 63)
	 * @param	siType			: ����, 0=Board Enable/Disable, 1=STOP EVENT����, 2=ESTOP EVENT����
	 * @param	bState			: ����, TRUE =Enable����,  STOP EVENT/ESTOP EVENT����,
	 *									FALSE=Disable����, STOP EVENT/ESTOP EVENT������
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID IO INTTERUPT TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo IOInterrupt(short siID, short siType, BOOL bState, BOOL bBootOpt = FALSE) = 0;

	/**
	 * I/O Interrupt �߻� �� PC������ Interrupt�� �߻���ų�� ���θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bState			: �߻� ����, TRUE=�߻�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo IOInterruptPCIRQ(short siBdNum, BOOL bState, BOOL bBootOpt = FALSE) = 0;

	/**
	 * PC Interrupt �߻� �� end of interrupt ��ȣ�� �߻���Ų��.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo IOInterruptPCIRQ_EOI(short siBdNum) = 0;

	/**
	 * ���� ���� PID ���� ���θ� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bState			: PID ���� ����, FALSE=PID���� �̽ǽ�, Analog ��� 0volt,
	 *											 TRUE =PID���� �ǽ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetController(short siAxis, BOOL bState) = 0;

	/**
	 * AMP Disable/Enable ���¸� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bState			: AMP Enable ����, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpEnable(short siAxis, BOOL bState) = 0;

	/**
	 * ���� �̵� �ְ�ӵ��� ��,���� �������� Limit�� �����Ѵ�.(boot file�� �ڵ� ����)
	 *
	 * MMC Library : set_accel_limit(), set_vel_limit()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dVelocity		: �̵��ӵ�, 1 ~ 2047000 count/rev
	 * @param	siAccel			: ��,���ӱ�����, 1 ~ 200, 10msec����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID VELOCITY VALUE(MOTIONLIB)
	 *							  xx = INVALID ACCELERATE VALUE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetVelLimit(short siAxis, double dVelocity, double dAccel, double dDecel) = 0;

	/**
	 * AMP Drive�� Fault �߻� �� ������ Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siAction		: ������ Event, NO EVENT, STOP EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID EVENT VALUE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpFaultEvent(short siAxis, short siAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Enable�� Active Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bLevel			: Enable Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpEnableLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Fault�� Active Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bLevel			: Fault Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpFaultLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * AMP Reset�� Active Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bLevel			: Reset Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpResetLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� AMP Drive�� Resolution�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siResolution	: AMP Resolution, default=2500 pulse/rev
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID RESOLUTION NUMBER(<0)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAmpResolution(short siAxis, short siResolution, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� ���ֺ� ���� ���ڰ�, �и��� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siRatioA		: Encoder ���ֺ� ���ڰ�
	 * @param	siRatioB		: Encoder ���ֺ� �и�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID DENOMINATOR(<1)(MOTIONLIB)
	 *							  xx = INVALID MOLECULE(<1)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetEncoderRatio(short siAxis, short siRatioA, short siRatioB, BOOL bBootOpt= FALSE) = 0;

	/**
	 * ���� ���� ȸ��/������ϴ� ����ȸ�� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bStatus			: ����ȸ�� �� ��������
	 * @param	siResolution	: Motor 1ȸ���� Pulse��
	 * @param	bType			: � ����, FALSE=����, TRUE=ȸ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID RESOLUTION NUMBER(<0)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetEndlessAx(short siAxis, BOOL bStatus, short siResolution, BOOL bType,
							   BOOL bBootOpt = FALSE) = 0;

	/**
	 * ����ȸ�� ���� �����̴� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dRange			: �̵� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetEndlessRange(short siAxis, double dRange, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� �� Encoder�� C�� �޽� �̿� ���θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bIndexReq		: C�� �޽� ��� ����, TRUE =Home Sensor�� Encoder�� Index Pulse�� ���� ����,
	 *												  FALSE=Home Sensor�� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetIndexRequired(short siAxis, BOOL bIndexReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� �ش� Motor ������ �����ϴ� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Motor ����, 0=�ӵ���Servo, 1=�Ϲ�Stepper, 2=MicroStepper Ȥ�� ��ġ��Servo
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOTOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetMotorType(short siAxis, short siType, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� Feedback ��ġ�� Loop ���¸� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siDevice		: Feedback ��ġ, 0=ENCODER, 1=0~10volt�Է�, 2=-10~10volt�Է�
	 * @param	bLoop			: Loop ����, FALSE=Open Loop, TRUE=Closed Loop
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOTOR FEEDBACK DEVICE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAxisProperty(short siAxis, short siDevice, BOOL bLoop, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� ���ֺ�� ���ڱ��� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siPgratio		: Pulse ���ֺ�, default=8
	 * @param	dEgratio		: ���ڱ���, default=1.0
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetRatioProperty(short siAxis, short siPgratio, double dEgratio, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ӵ��� Servo�� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bControl		: ������, FALSE=�ӵ�����, TRUE=��ũ����
	 * @param	bPolar			: Analog ��� ����,  TRUE=UNIPOLAR, FALSE=BIPOLER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetVServoProperty(short siAxis, BOOL bControl, BOOL bPolar, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� Pulse ��� ���¸� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bMode			: Pulse ��� ����, FALSE=Two Pulse(CW+CCW), TRUE=Sign+Pulse
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetStepMode(short siAxis, BOOL bMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� Encoder �Է� ����� ��ǥ ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bEncDir			: Encoder �Է� ����, FALSE=ENCO_CW(�ð����, - count),
	 *												 TRUE =ENCO_CCW(�ݽð����, + count)
	 * @param	bCoorDir		: ��ǥ����, FALSE=CORD_CW(�ð����, +��ǥ�̵�),
	 *										TRUE =CORD_CCW(�ݽð����, -��ǥ�̵�)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetEncoderDirection(short siAxis, BOOL bEncDir, BOOL bCoorDir, BOOL bBootOpt = FALSE) = 0;

	/**
	 * I/O 8���� ����  ��,��� ��带 �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bMode			: ��, ��� ���, TRUE=���, FALSE=�Է�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetIOMode(short siBdNum, BOOL bMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ��ġ���� �Ϸᰪ�� ��ġ���� �� ��ȣ Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dInPosition		: ��ġ ������
	 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetInPosition(short siAxis, double dInPosition, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� InPosition ��ȣ ��뿩�θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bReq			: ��� ����, TRUE=���
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetInpositionRequired(short siAxis, BOOL bReq, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ��ġ���� Limit���� Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dLimit			: ��ġ���� Limit��, �ִ� 35000 count
	 * @param	siAction		: ��ġ���� Event, NO EVENT, STOP EVENT, ESTOP EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetErrorLimit(short siAxis, double dLimit, short siAction, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ������ ���� STOP EVENT, ESTOP EVENT ���� �� ���� �ð��� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ���� ����, FALSE=STOP, TRUE=ESTOP
	 * @param	siRate			: ���� �ð�, default=10
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetStopRate(short siAxis, BOOL bType, short siRate, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ Event�� ��ȣ Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
	 * @param	siLimit			: ������ Event
	 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FLASE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSensorLimit(short siAxis, short siType, short siLimit, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
	 * @param	siLimit			: ������ Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSensorEvent(short siAxis, short siType, short siLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ ��ȣ Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
	 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FLASE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSensorLevel(short siAxis, short siType, BOOL bLevel, BOOL bBootOpt = FALSE) = 0;

	/**
	 * +/- �������� Motor�� �̵��� �� �ִ� Limit ��ġ���� �� ��ġ���� �������� �� ����� Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ����, FALSE=Negative, TRUE=Positive
	 * @param	dPosition		: ���� ��ġ��, +/-2147483647
	 * @param	siLimit			: ����� Event
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSWLimit(short siAxis, BOOL bType, double dPosition, short siLimit, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� �ӵ� �Ǵ� ��ġ�� ���� PID & FF Gain ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bVelType		: ��ġ/�ӵ� ���� ����, FALSE=��ġ, TRUE=�ӵ�
	 * @param	*plGain			: Gain �� �迭, �迭�μ� ��ġ�� �Ʒ��� ����.
	 *								0=GA_P, 1=GA_I, 2=GA_D, 3=GA_F, 4=GA_ILIMIT, 5=GAIN_NUMBER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ش� ���� ���� ���� �� ���� ���� ��带 �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ������, FALSE=��ġ, TRUE=�ӵ�
	 * @param	bMode			: �������� ���, FALSE=�׻�����, TRUE=����������
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetIntegration(short siAxis, BOOL bType, BOOL bMode, BOOL bBootOpt = FALSE) = 0;

	/**
	 * �ӵ����� Ȥ�� ��ũ ���ɿ� ���� Low Pass Filter Ȥ�� Notch Filter�� ���� Filter ���� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bCommandType	: ���� ����, FALSE=�ӵ�(Position), TRUE=��ũ(Velocity)
	 * @param	bFilterType		: Filter ����, FALSE=LowPass, TRUE=Notch
	 * @param	dFilter			: Filter ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double dFilter,
							BOOL bBootOpt = FALSE) = 0;

	/**
	 * ������ Board�� �ະ ���� ���θ� �����Ѵ�.
	 *
	 *		b7	b6	b5	b4	b3	b2	b1	b0
	 *		��8	��7	��6	��5	��4	��3	��2	��1
	 *
	 *		bit = TRUE : ���� ����
	 *		bit = FALSE : ���� ����
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siState			: �ະ ���� ����, bit�� �� ��(b0=��1, b1=��2, ...), TRUE=����, FALSE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAxisRunStop(short siBdNum, short siState) = 0;

	/**
	 * ���� I/O bit�� HIGH(1)/LOW(0) ���·� �����.
	 *
	 *		 Board��	  I/O Bit ����
	 *			1			0  ~ 63
	 *			2			64 ~ 63
	 *			3			64 ~ 95
	 *			4			96 ~ 127
	 *
	 * @param	siBitNo			: ������ I/O Bit ��ȣ(������ Board�� ������ ���� �޶���)
	 * @param	bValue			: ������ ��,(TRUE, FALSE)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetBit(short siBitNo, BOOL bValue) = 0;

	/**
	 * 64bit�� I/O Data�� ��� Port�� ���� ��������.
	 *
	 * @param	siPort			: ��� Port ��ȣ(0 ~ 3, Board ���� ������ ���� ����)
	 * @param	lValue			: ��� ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO PORT ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetIO(short siPort, long lValue) = 0;

	/**
	 * 12/16 bit Analog ��������� ��������.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siValue			: ����� ���� ��, +/-2048, +/-64767
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetDacOut(short siAxis, short siValue) = 0;

	/**
	 * �ش� ���� Analog Offset���� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siOffset		: Analog OFfset, +/-2048, +/-64767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAnalogOffset(short siAxis, short siOffset, BOOL bBootOpt = FALSE) = 0;

	/**
	 * ���� ���� ��������� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siLimit			: A��� ���� ����, 0 ~ 64767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAnalogLimit(short siAxis, short siLimit, BOOL bBootOpt= FALSE) = 0;

	/**
	 * ���� ���� ��ġ �� ��ǥ ��ġ�� �����Ѵ�.(AMP Disable ���¿��� �����ϴ°� ����.)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ��ġ ����, FALSE=������ġ, TRUE=��ǥ��ġ
	 * @param	dPosition		: bType=FALSE�̸� ������ ������ġ, bType=TRUE�̸� ������ ��ǥ��ġ
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetPosition(short siAxis, BOOL bType, double dPosition) = 0;

	/**
	 * Ư�� ���� Encoder Feedback Data�� ������ �о���� �� ���(50usec �ֱ� Update)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bStatus			: ���� ����, TRUE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetFastReadEncoder(short siAxis, BOOL bStatus) = 0;

	/**
	 * ����ڰ� Motion ���� S/W�� ��ü �����Ͽ� �ý����� ���۽�ų �� �ֵ��� �������ִ� ���
	 *
	 * @param	siLen			: ������ �� ��, Board�� ���� ����� ��ġ��Ų��. 8�� Board => 8, 4�� Board => 4
	 * @param	*psiAxes		: ������ �� ID�� �迭������ ����
	 * @param	*plDelt			: �� Sampling Time(10msec)�� ��ġ������ Data
	 * @param	siFlag			: �ӵ� Profile�� ���۰� ���� �˷��ִµ� ���, 1=���۽���, 2=������, 3=���ۿϷ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID INTERPOLATION FLAG TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetInterpolation(short siLen, short *psiAxes, long *plDelt, short siFlag) = 0;

	/**
	 * �浹���� ����� ����� Mastr/Slave�� �� �浹���� �Ÿ� �� ����(+, -, >, <)�� �����Ѵ�.
	 *
	 * @param	siMasterAx		: Master �� ID(0 ~ 63)
	 * @param	siSlaveAx		: Slave �� ID(0 ~ 63)
	 * @param	bAddSub			: ���� ���, FALSE=(Master������ġ-Slave������ġ),
	 *										 TRUE=(Master������ġ+Slave������ġ)
	 * @param	bNonEqual		: ��, FALSE=(dPosition < bAddSub���ġ),
	 *									TRUE=(dPosition > bAddSub���ġ)
	 * @param	dPosition		: �浹 ���� �Ÿ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetCollisionPrevent(short siMasterAx, short siSlaveAx,
									  BOOL bAddSub, BOOL bNonEqual, double dPosition, BOOL bState) = 0;

	/**
	 * �浹���� ����� ��뿩�θ� �����Ѵ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bMode			: ��� ����, TRUE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetCollisionPreventFlag(short siBdNum, BOOL bMode) = 0;

	/**
	 * Board DPRAM Address�� �����Ѵ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	lAddr			: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetDpramAddress(short siBdNum, long lAddr) = 0;

	/**
	 * ����ġ Motor�� Type�� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Motor ����, 1=�ＺCSDJ, CSDJ+SERVO DRIVE, 2=YASKAWA SERVO DRIVE
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID ABS MOTOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAbsEncoderType(short siAxis, short siType) = 0;

	/**
	 * ����ġ Motor�� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetAbsEncoder(short siAxis) = 0;

	/**
	 * Servo Linear Flag ���¸� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siFlag			: Servo Linear Flag ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetServoLinearFlag(short siAxis, short siFlag, BOOL bBootOpt= FALSE) = 0;

	/**
	 * �������� ���θ� �����Ѵ�.
	 *
	 * @param	bState			: ���� ����, TRUE=�������� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSyncControl(BOOL bState) = 0;

	/**
	 * ���������� Master��� Slave���� �����Ѵ�.
	 *
	 * @param	siMasterAx		: Master �� ID(0 ~ 63)
	 * @param	siSlaveAx		: Slave �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSyncMapAxes(short siMasterAx, short siSlaveAx) = 0;

	/**
	 * ��������� ����Ǵ� ���� Gain���� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siCoeff			: ���� Gain ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSyncGain(short siCoeff, BOOL bBootOpt = FALSE) = 0;

	/**
	 * Board�� Sampling Rate�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siTime			: Sampling Time(msec ����)(1=4msec, 2=2msec, 3=1msec�� ����)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID SAMPLING RATE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetControlTimer(short siBdNum, short siTime, BOOL bBootOpt = FALSE) = 0;

//#if FALSE	// MMC Library Header File�� ����(MMCWFI30.h)
	/**
	 * PositionIoOnOff()�� ������ ���� �����Ѵ�.
	 *
	 * @param	siPosNum		:(OPTION=0) ��ġ ��ȣ, 1 ~ 10, 0=��� ��ġ ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION IO NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
//	virtual int PositionIOClear(short siPosNum = 0) = 0;
//#endif

//#if FALSE	// MC Library Header File�� ����(MMCWFI30.h)
	/**
	 * ���� ���� ������ ��ġ�� ���� �� ���� IO�� ����Ѵ�.
	 *
	 * @param	siPosNum		: ��ġ ��ȣ, 1 ~ 10
	 * @param	siIONum			: I/O ��ȣ, ��������=ON, ��������=OFF
	 * @param	siAxis			: �� ID
	 * @param	dPosition		: ���� ���� ��ġ��
	 * @param	bEncFlag		: Encoder Flag, FALSE=������ġ Counter ���, TRUE=�ܺ� Encoder ���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION IO NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
//	virtual int PositionIoOnoff(short siPosNum, short siIONum, short siAxis, double dPosition, BOOL bEncFlag) = 0;
//#endif

	/**
	 * ���� ���� Ȥ�� ����, ��, ��ȣ���� ���� �� �ӵ��� ��,���ӵ��� �����Ѵ�.
	 *
	 * @param	dVelocity		: �ӵ�
	 * @param	siAccel			: ��,���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetMoveSpeed(double dVelocity, short siAccel) = 0;

	/**
	 * �ڵ� ��,���� ����� ��뿩�θ� �����Ѵ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bState			: ��뿩��, FALSE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetSplAutoOff(short siBdNum, BOOL bState) = 0;

	/**
	 * �ڵ� ��,���� ����� ��뿩�θ� �д´�.
	 * Library�� �����Ǵ� �Լ��� ���� ����� ���� Data���� �о�´�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbState		: ��뿩��, TURE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo GetSplAutoOff(short siBdNum, BOOL *pbState) = 0;

	/**
	 * �� �̵��� �����Ѵ�.
	 * �Ϲ�����, �������, �ӵ��̵������� �����Ѵ�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: ���� ����, 0=STOP, 1=ESTOP, 2=VSTOP
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID STOP TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetStop(short siAxis, short siType, double fDecel, double fJerk) = 0;

	/**
	 * 1�� �ӵ� Profile �̵�(�� 1�� ������ �̵� ������)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dPosition		: �̵��� ��ġ, Ȥ�� ���Ÿ�
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @param	siDecel			: �̵� ���ӵ�(���Ī(t) Type�� ����)
	 * @param	siType			: �̵� Type, 0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 *										 2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *										 3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 *										 4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
	 *										 6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *										 7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 * @param	bWaitOpt		:(OPTION=FALSE) �̵� �Ϸ� ��� ����, TRUE=�̵��Ϸ�ɶ��������
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Move(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, short siType, BOOL bWaitOpt = FALSE, double dJerk = 0.0) = 0;

	/**
	 * ���� �ӵ� Profile ���� �̵�(���Ÿ� �̵��� �������� �ʴ´�.)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		| �ӵ� #1 | �ӵ� #2 | �ӵ� #3 | ... | �ӵ� #n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ���� #1 | ���� #2 | ���� #3 | ... | ���� #n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ���� #1 | ���� #2 | ���� #3 | ... | ���� #n |
	 *		+---------+---------+---------+-----+---------+
	 *
	 * @param	siLen			: �� ��, >0
	 * @param	*psiAxis		: �� ID(�� �� ��ŭ ����)
	 * @param	*pdPosition		: �̵��� ��ġ, Ȥ�� ���Ÿ�(�� �� ��ŭ ����)
	 * @param	*pdVelocity		: �̵� �ӵ�(�� �� ��ŭ ����)
	 * @param	*psiAccel		: �̵� ���ӵ�(�� �� ��ŭ ����)
	 * @param	*psiDecel		: �̵� ���ӵ�(�� �� ��ŭ ����)
	 * @param	siType			: �̵� Type, 0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 *										 4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
	 * @param	bWaitOpt		:(OPTION=FALSE) �̵� �Ϸ� ��� ����, TRUE=�̵��Ϸ�ɶ��������
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo MoveAll(short siLen, short *psiAxes, double *pdPosition, double *pdVelocity,
						  short *psiAccel, short *psiDecel, short siType, BOOL bWaitOpt = FALSE) = 0;

	/**
	 * ������ n���� �־��� ��ǥ����ŭ �����̵��� �Ѵ�.(�ٸ� Board�� �� ��� �Ұ�)
	 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *
	 * @param	siLen			: �� ��, 2 <= siLen <= 8
	 * @param	*pdPosition		: �̵��� ��ǥ��(�� �� ��ŭ ����)
	 * @param	siType			: �̵� Type, 0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo MoveN(short siLen, double *pdPosition, short siType) = 0;

	/**
	 * ������ n���� �־��� ��ǥ����ŭ �����̵��� �Ѵ�.(�ٸ� Board�� �� ��� ����)
	 *
	 *		|-------------------> siLen <-----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siLen			: �� ��, >=2
	 * @param	*psiAxes		: �� ID �迭
	 * @param	*pdPosition		: �̵��� ��ǥ��(�� �� ��ŭ ����)
	 * @param	siType			: �̵� Type, 0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo MoveNAx(short siLen, short *psiAxes, double *pdPosition, short siType, double dVelocity, short siAccel) = 0;

	/**
	 * ������ �׷��� ����� �־��� ��ǥ����ŭ �����̵��� �Ѵ�.
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siGrpNum		: Group ��ȣ, 1 ~ 4
	 * @param	siLen			: �� ��, 2 <= siLen <= 4
	 * @param	*psiAxes		: �� ID �迭(�� �� ��ŭ ����)
	 * @param	*pdPosition		: �̵��� ��ǥ��(�� �� ��ŭ ����)
	 * @param	siType			: �̵� Type, 0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *										 1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID GROUP NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOVE TYPE(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo MoveNAxGr(short siGrpNum, short siLen, short *psiAxes, double *pdPosition,
							short siType, double dVelocity, short siAccel) = 0;

	/**
	 * ���� �� ��� �̵��Ѵ�.(�� 1�� �����θ� ���� �����ϴ�.)
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo VMove(short siAxis, double dVelocity, double dAccel, double dDecel = 0.0, double dJerk = 0.0) = 0;

	/**
	 * ���� ��ġ���� �־��� 2/3���� ������ ��ǥ������ ��,�����ϸ鼭 ��ȣ CP Motion���� �̵��Ѵ�.
	 * (�ٸ� Board�� �� ��� �Ұ�)
	 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
	 *
	 *		|---------------> siAxNum = n <---------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+-----------+-----------+------+--------+----------+
	 *		| ȸ���߽�X | ȸ���߽�Y | �ӵ� | ���ӵ� | ȸ������ |
	 *		+-----------+-----------+------+--------+----------+
	 *
	 * @param	siAxNum			: �� ��, 2=2��, 3=3��
	 * @param	dCenterX		: ȸ�� �߽� X��ǥ
	 * @param	dCenterY		: ȸ�� �߽� Y��ǥ
	 * @param	*pdPoint		: �̵��� ��ǥ, 2���̸� 2�����迭, 3���̸� 3�����迭
	 * @param	dVelocity		: �̵� �ӵ�(0.0=SetMoveSpeed()���� ������ �ӵ� ���)
	 * @param	siAccel			: �̵� ���ӵ�(0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
	 * @param	bDir			: ȸ������, FALSE=CIR_CW(�ð����), TRUE=CIR_CCW(�ݽð����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplArcMove(short siAxNum, double dCenterX, double dCenterY,
							 double *pdPoint, double dVelocity, short siAccel, BOOL bDir) = 0;

	/**
	 * ���� ��ġ���� �־��� ��ǥ������ ��,�����ϸ鼭 ��ȣ CP Motion���� �̵��Ѵ�.
	 * (�ٸ� Board�� �� ��� ����)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+-----------+-----------+------+--------+----------+
	 *		| ȸ���߽�X | ȸ���߽�Y | �ӵ� | ���ӵ� | ȸ������ |
	 *		+-----------+-----------+------+--------+----------+
	 *
	 * @param	siLen			: �� ��(>= 2)
	 * @param	*psiAxes		: �� ID �迭(�� ����ŭ ����)
	 * @param	siCenterX		: ȸ�� �߽� X��ǥ
	 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
	 * @param	*pdPoint		: �̵��� ��ǥ(�� �� ��ŭ ����)
	 * @param	dVelocity		: �̵� �ӵ�(0.0=SetMoveSpeed()���� ������ �ӵ� ���)
	 * @param	siAccel			: �̵� ���ӵ�(0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
	 * @param	bDir			: ȸ������, FALSE=CIR_CW(�ð����), TRUE=CIR_CCW(�ݽð����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY,
								double *pdPoint, double dVelocity, short siAccel, BOOL bDir) = 0;

	/**
	 * ���� ��ġ���� �־��� ��ǥ������ ��,�����ϸ鼭 ��ȣ CP Motion���� �̵��Ѵ�.
	 * (�ٸ� Board�� �� ��� ����)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+-----------+-----------+------+--------+----------+
	 *		| ȸ���߽�X | ȸ���߽�Y | �ӵ� | ���ӵ� | ȸ������ |
	 *		+-----------+-----------+------+--------+----------+
	 *
	 * MMC Library : spl_arc_move2ax(), spl_arc_move3ax(), spl_arc_movenax()
	 *
	 * @param	siLen			: �� ��(>= 2)
	 * @param	*psiAxes		: �� ID �迭(�� ����ŭ ����)
	 * @param	siCenterX		: ȸ�� �߽� X��ǥ
	 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
	 * @param	*pdPoint		: �̵��� ��ǥ(�� �� ��ŭ ����)
	 * @param	dVelocity		: �̵� �ӵ�(0.0=SetMoveSpeed()���� ������ �ӵ� ���)
	 * @param	siAccel			: �̵� ���ӵ�(0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
	 * @param	bDir			: ȸ������, FALSE=CIR_CW(�ð����), TRUE=CIR_CCW(�ݽð����)
	 * @param	bAutoFlag		: �ڵ� Auto CP, FALSE=Auto CP ����, TRUE=Auto CP ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplAutoArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY,
								  double *pdPoint, double dVelocity, short siAccel, BOOL bDir, BOOL bAutoFlag) = 0;

	/**
	 * ���� ��ġ���� �־��� 2/3���� ������ ��ǥ������ ��,�����ϸ鼭 ���� CP Motion���� �̵��Ѵ�.
	 * (�ٸ� Board�� �� ��� �Ұ�)
	 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
	 *
	 *		|---------------> siAxNum = n <---------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siAxNum			: �� ��, 2=2��, 3=3��
	 * @param	*pdPoint		: �̵��� ��ǥ, 2���̸� 2�����迭, 3���̸� 3�����迭
	 * @param	dVelocity		: �̵� �ӵ�(0.0=SetMoveSpeed()���� ������ �ӵ� ���)
	 * @param	siAccel			: �̵� ���ӵ�(0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplLineMoveN(short siAxNum, double *pdPoint, double dVelocity, short siAccel) = 0;

    /**
     * ���� ��ġ���� �־��� ��ǥ������ ��,�����ϸ鼭 ���� CP Motion���� �̵��Ѵ�.
	 * (�ٸ� Board�� �� ��� ����)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siLen			: �� ��
	 * @param	*psiAxes		: �� ID �迭
	 * @param	*pdPoint		: �̵��� ��ǥ(�� �� ��ŭ ����)
	 * @param	dVelocity		: �̵� �ӵ�, 0.0=�������� �ӵ��� �̵�
	 * @param	siAccel			: �̵� ���ӵ�, 0=�������� ���ӵ��� �̵�
	 * @return	Error Code		: 0 = SUCCESS, �� �� = Error
	 */
	virtual ErrorInfo SplLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel) = 0;

    /**
     * ���� ��ġ���� �־��� ��ǥ������ �ڵ� ��,���� ���θ� ���ڷ� �����޾Ƽ� ���� CP Motion���� �̵��Ѵ�.
	 * (�ٸ� Board�� �� ��� ����)
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siLen			: �� ��
	 * @param	*psiAxes		: �� ID �迭
	 * @param	*pdPoint		: �̵��� ��ǥ(�� �� ��ŭ ����)
	 * @param	dVelocity		: �̵� �ӵ�, 0.0=�������� �ӵ��� �̵�
	 * @param	siAccel			: �̵� ���ӵ�, 0=�������� ���ӵ��� �̵�
	 * @param	bAutoFlag		: �ڵ� Auto CP, FALSE=Auto CP ����, TRUE=Auto CP ��
	 * @return	Error Code		: 0 = SUCCESS, �� �� = Error
	 */
	virtual ErrorInfo SplAutoLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel, BOOL bAutoFlag) = 0;

	/**
	 * ��, ��ȣ �̵� �� ���ּӵ��� �����Ѵ�.
	 *
	 * @param	dDegree			: ���ּӵ�, 0 < dDegree < 1000.0
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID ARC DIVISION DEGREE NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SetArcDivision(double dDegree) = 0;

	/**
	 * �־��� �߽ɿ��� ������ ������ŭ ��ȣ�� �׸��� ������ �����Ѵ�.
	 * (�ٸ� Board�� �� ��� �Ұ�)
	 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
	 *
	 * @param	siCenterX		: ȸ�� �߽� X��ǥ
	 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
	 * @param	dAngle			: ȸ�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Arc2(double dXCenter, double dYCenter, double dAngle) = 0;

	/**
	 * �־��� �߽ɿ��� ������ ������ŭ ��ȣ�� �׸��� ������ �����Ѵ�.
	 * (�ٸ� Board�� �� ��� �Ұ�)
	 *
	 * @param	siAxis1			: ��1 ID(0 ~ 63)
	 * @param	siAxis2			: ��2 ID(0 ~ 63)
	 * @param	siCenterX		: ȸ�� �߽� X��ǥ
	 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
	 * @param	dAngle			: ȸ�� ����
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Arc2Ax(short siAxis1, short siAxis2, double dXCenter, double dYCenter,
						 double dAngle, double dVelocity, short siAccel) = 0;

	/**
	 * �־��� �簢���� ���ο� ������ ���̸� �̿��Ͽ� ������ġ���� ����̵��� �ϸ鼭 CP Motion���� �簢���� �׸���.
	 *
	 *			+--------------------+ pdPoint(X, Y)
	 *			|					 |
	 *			|					 |
	 *			|					 |
	 *			|					 |
	 *			|					 |
	 *			+--------------------+
	 *		������ġ
	 *
	 * @param	siAxis1			: ��1 ID(0 ~ 63)
	 * @param	siAxis2			: ��2 ID(0 ~ 63)
	 * @param	*pdPoint		: ������ġ�� �밢�������� X, Y ��ǥ
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo RectMove(short siAxis1, short siAxis2, double *pdPoint,
						   double dVelocity, short siAccel) = 0;

	/**
	 * ������ġ���� �־��� ��ġ�� �����ϸ鼭 CP Motion���� �̵��Ѵ�.
	 *
	 *				  |----------------> siLen = n <----------------|
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| �� ID#2 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| �� ID#3 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siLen			: ��ġ Data ����, �ִ� 30
	 * @param	siAxis1			: ��1 ID(0 ~ 63)
	 * @param	siAxis2			: ��2 ID(0 ~ 63)
	 * @param	siAxis3			: ��3 ID(0 ~ 63)
	 * @param	*pdPointX		: X��ǥ �迭(��ġ Data ������ŭ ����)
	 * @param	*pdPointY		: Y��ǥ �迭(��ġ Data ������ŭ ����)
	 * @param	*pdPointZ		: Z��ǥ �迭(��ġ Data ������ŭ ����)
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION DATA NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplMove(short siLen, short siAxis1, short siAxis2, short siAxis3,
						  double *pdPointX, double *pdPointY, double *pdPointZ,
						  double dVelocity, short siAccel) = 0;

	/**
	 * SplMoveX()�� �ʿ��� ��ġ��θ� �����Ѵ�.
	 *
	 *				  |----------------> siLen = n <----------------|
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| �� ID#1 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| �� ID#2 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		| �� ID#3 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siSplNum		: Spline Motion ��ȣ, 1 ~ 20
	 * @param	siLen			: �̵� ��� ��, 1 ~ 500
	 * @param	siAxis1			: ��1 ID(0 ~ 63)
	 * @param	siAxis2			: ��2 ID(0 ~ 63)
	 * @param	siAxis3			: ��3 ID(0 ~ 63)
	 * @param	*pdPoint1		: 1��ǥ �迭
	 * @param	*pdPoint2		: 2��ǥ �迭
	 * @param	*pdPoint3		: 3��ǥ �迭
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID SPLINE MOTION NUMBER(MOTIONLIB)
	 *							  xx = INVALID MOVE PATH NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplMoveData(short siSplNum, short siLen, short siAxis1, short siAxis2,
							  short siAxis3, double *pdPoint1, double *pdPoint2,
							  double *pdPoint3, double dVelocity, short siAccel) = 0;

	/**
	 * ���� 3���� Spline Motion���� SplMoveData()���� ������ ��ġ�� �����ϸ鼭 ���� �̵��Ѵ�.
	 *
	 * @param	siSplNum		: Spline Motion ��ȣ, 1 ~ 20
	 * @param	siAxis1			: ��1 ID(0 ~ 63)
	 * @param	siAxis2			: ��2 ID(0 ~ 63)
	 * @param	siAxis3			: ��3 ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID SPLINE MOTION NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo SplMovex(short siSplNum, short siAxis1, short siAxis2, short siAxis3) = 0;

	/**
	 * �� Board�� ROM Version�� �д´�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiVersion		: ROM Version, 101 => 1.01
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo VersionCheck(short siBdNum, short *psiVersion) = 0;

	/**
	 * �ش� Error Code�� Error Message�� ��ȯ�Ѵ�.
	 *
	 * @param	siCode			: Error Code
	 * @param	*pcDst			: Error Message, ERR_MAX_ERROR_LEN(80)���� ũ�ų� ���ƾ� �Ѵ�.
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo ErrorMessage(int iCode, char *pcDst) = 0;

	/**
	 * Position Compare Board�� �ʱ�ȭ�Ѵ�.(�� 2�� ����� ���� ���� Board�� ���̾�� �Ѵ�.)
	 *
	 * @param	siIndexSel		: Position Compare�� ��, 1 ~ 2
	 * @param	siAxis1			: ��1 ID(0 ~ 63), ���� Board�� ���̾�� �Ѵ�.
	 * @param	siAxis2			: ��2 ID(0 ~ 63), ���� Board�� ���̾�� �Ѵ�.(siIndexSel=2�� ���� ����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION COMPARE INDEXSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo PositionCompareInit(short siIndexSel, short siAxis1, short siAxis2) = 0;

	/**
	 * Position Compare�� �����Ѵ�.(Standard Type)
	 *
	 * @param	siIndexSel		: Position Compare�� ����� �� ��, 1 ~ 2
	 * @param	siIndexNum		: Position Compare�� �ǽ��� Index ��ȣ, 1 ~ 8
	 * @param	siBitNo			: ����� I/O Bit ��ȣ, 0 ~ 63
	 * @param	siAxis1			: ��1 ID(0 ~ 63), ���� Board�� ���̾�� �Ѵ�.
	 * @param	siAxis2			: ��2 ID(0 ~ 63), ���� Board�� ���̾�� �Ѵ�.(siIndexSel=2�� ���� ����)
	 * @param	bLatch			: I/O ��¸��, FALSE=Transparent Mode, TRUE=Latch Mode
	 * @param	siFunction		: Position Compare�� ����� �ε�ȣ, 1="=", 2=">", 3="<"
	 * @param	siOutMode		: ���� I/O�� ��¸��, 0=�ະ ON/OFF, 1=���� AND, 2=���� OR
	 * @param	dPosition		: Position Compare�� ���� ��ġ Data(> 0.0)
	 * @param	lTime			: I/O ��� �ð�, Transparent Mode�϶��� ����, 40usec����,  �ִ� 5.38sec
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION COMPARE INDEXSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE INDEX NUMBER(MOTIONLIB)
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE FUNCTION TYPE(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE OUT MODE TYPE(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE IO OUT TIME(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo PositionCompare(short siIndexSel, short siIndexNum, short siBitNo, short siAxis1,
								  short siAxis2, BOOL bLatch, short siFunction, short siOutMode,
								  double dPosition, long lTime) = 0;

	/**
	 * Position Compare�� �����Ѵ�.(Interval Type)
	 *
	 * @param	bDir			: Position Compare�� +�������� �̵��� ���۽�ų ������ -�������� ���۽�ų ���� ����
	 *							   FALSE="+", TRUE="-"
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siBitNo			: ����� I/O Bit ��ȣ(0 ~ 63)
	 * @param	dStartPos		: I/O�� ���۵� ������ Position ��
	 * @param	dLimitPos		: I/O�� ���۵� ������ Position ��
	 * @param	lInterval		: I/O�� �ݺ��� ������ �޽��� ������ ����
	 * @param	lTime			: I/O ����� ���ӵ� �ð�, 40sec ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE IO REPEAT TIME(MOTIONLIB)
	 *							  xx = INVALID POSITION COMPARE IO OUT TIME(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo PositionCompareInterval(BOOL bDir, short siAxis, short siBitNo, double dStartPos, double dLimitPos,
								  long lInterval, long lTime) = 0;

	/**
	 * Position Compare ������ �� ������ ���θ� �����Ѵ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bFlag			: FALSE=Position Compare ���� Disable, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo PositionCompareEnable(short siBdNum, BOOL bFlag) = 0;

	/**
	 * Position Compare�� Index�� �ʱ�ȭ�Ѵ�.
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siIndexSel		: �׻� "1"�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo PositionCompareClear(short siBdNum, short siIndexSel) = 0;

	/**
	 * Position Compare ������ ���� Encoder ���� �о��.
	 *
	 * @param	siIndexSel		: �׻� "1"�� ����
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdPosition		: Encoder ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo PositionCompareRead(short siIndexSel, short siAxis, double *pdPosition) = 0;

	/**
	* Board�� Sampling Rate�� �����Ѵ�.
	* boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	*
	* @param siAxis   : AxisID
	* @param siTime   : Sampling Time(msec ����) (1=4msec, 2=2msec, 3=1msec�� ����)
	* @param bBootOpt  : (OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	* @return Error Code  : 0 = SUCCESS
	*         xx = INVALID MOTION BOARD ID (MOTIONLIB)
	*         xx = INVALID SAMPLING RATE (MOTIONLIB)
	*         �� �� = Ÿ �Լ� Return Error
	*/
	virtual ErrorInfo SetControlTimerForAx(short siAxis, short siTime, BOOL bBootOpt = FALSE) = 0;

	///////////////////////////////////////
	// RS EtherCat ���� �߰�
	///////////////////////////////////////
	

	// ���� ���� ����� ���������� üũ (��Ǹ���� ��� ù��° �࿡���� üũ�ϸ� ��)
	virtual BOOL IsOpMode_NMC(short siAxis) = 0;
	
	// ����(���� ����) ���� ���� ���.
	//////////////////////////////////////////////
	
	// ����(���� ����) ����
	virtual ErrorInfo EnableSyncMotion_NMC(int nMasterAxis, int nSlaveAxis, int nRatioNumerator, int nRatioDenominator, double fAccel, double fDecel, double fJerk) = 0;
	
	// ����(���� ����) ����
	virtual ErrorInfo DisableSyncMotion_NMC(int nSlaveAxis) = 0;
	
	// ����(���� ����)�� Ȱ��ȭ �Ǿ� �ִ��� üũ. ��ȯ���� true�϶� ��ũ�� �ɷ��ִ� ������.
	virtual BOOL IsSyncEnable_NMC(int nSlaveAxis) = 0;

	// ���� ���� ����
	virtual ErrorInfo HomeSetConfig(short siAxis, int iHomeMode, BOOL bDirection) = 0;
	virtual ErrorInfo HomeSetVelocity(short siAxis, double dFastVelocity, double dSlowVelocity, double dAccel, double dDecel, double dPosOffset) = 0;
	virtual ErrorInfo HomingStart(short siAxis) = 0;
	virtual ErrorInfo IsHoming(short siAxis, BOOL *bState) = 0;
	virtual ErrorInfo IsHomeEnd(short siAxis, BOOL *bState) = 0;

	// RS NMC �ڵ� ������ �̵� ����
	/**
     * ���� ��ġ���� �־��� ��ǥ������ �ڵ� ��,���� ���θ� ���ڷ� �����޾Ƽ� ���� CP Motion���� �̵��Ѵ�.
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *		+------+--------+
	 *		| �ӵ� | ���ӵ� |
	 *		+------+--------+
	 *
	 * @param	siAxis			: �� ID
	 * @param	dPosition		: ��ǥ ��ġ
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	dAccel			: �̵� ���ӵ�
	 * @param	dDecel			: �̵� ���ӵ�
	 * @param	iBufferMode		: mcAborting : �ڵ� ������, mcBuffered : �̵� �Ϸ� �� ���� �̵�, mcBlendingLow, mcBlendingPrevious, mcBlendingNext, mcBlendingHigh
	 * @return	Error Code		: 0 = SUCCESS, �� �� = Error
	 */
	virtual ErrorInfo SplAutoLineMove(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, int iBufferMode) = 0;

	// Motion���� �߻��� ���� ������ String �������� �����Ѵ�.
	virtual CString ReturnCurrentMotionError() = 0;

	//SJ_YYK 160317 Add..
	virtual ErrorInfo SetOriginComplete(short siAxis) = 0;
	virtual ErrorInfo GetOriginCompleteFlag(short siAxis, short *psiState) = 0;
	virtual ErrorInfo ResetOriginComplete(short siAxis) = 0;
};

#endif //IMotionLibNMC_H

// End of File IMotionLibNMC.h
