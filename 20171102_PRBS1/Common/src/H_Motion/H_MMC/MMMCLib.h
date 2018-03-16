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
 * CMMCLib.h : This file defines the class of MMC Motion libraries.
 *			   �� ������ MMC Motion Library�� ���� Class�� �����Ѵ�.
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef MMMCLIB_H
#define MMMCLIB_H

#include "DefMotionStruct.h"	// Motion�� ����ü ���� File
//#include "DefMotionError.h"		// Motion�� Error Code ���� File

#include "IMotionLib.h"			// IMotionLib Interface Class Header File
#include "MMotionBoard.h"		// CMotionboard Class Header File

/**
 * This class is defined class of MMC motion libraries.
 * �� Class�� MMC Motion Library�� ���� Class�� �����Ѵ�.
 * 
 * MMC Library �� �Ŵ��� ������ �����ִ� 362���� API�� ���� 147���� Operation���� �����Ͽ�
 * �����ϴ°� �⺻ ����̸�, ���� MMC Library�� �״�� ����� �� �ְ� �ϴ� ���� �����̹Ƿ�
 * Operation�� �̸��� API�� �̸��� ���� �Ѿư��� ����Ͽ���.
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 * @url MMC Library ����(Motion Component).xls
 */
class MMMCLib : public IMotionLib
{

private:	// attribute

    /** Motion Board ���� ���� 
     * @link aggregation
     * @supplierCardinality 1*/
	MMotionBoard *m_pBoardConfig;

	/** MMC Motion Library Object Number */
	static	int	m_iObjNo;

public:		// Constructor & Destructor

	/**
	 * Constructor : create and clear data
	 *				 Data�� �����ϰ� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */
	MMMCLib(SCommonAttribute commonData);

	/**
	 * Destructor : Delete inner object
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	~MMMCLib();

public:		// interface operation


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
	virtual int SetUseAxis(short siAxisID, BOOL bState);

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
	virtual int GetUseAxis(short siAxisID, BOOL *pbState);

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
	virtual int SetMotorParam(short siAxisID, SMotionAxis mAx, BOOL bBootOpt = FALSE);

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
	virtual int GetMotorParam(short siAxisID, SMotionAxis *pmAx);

	/**
	 * Motion Board Parameter ����
	 *
	 * @param	MotionBd		: Motion Board Parameter
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetBoardParam(SMotionBoard MotionBd);
	/**
	 * Motion Board Parameter �б�
	 *
	 * @param	*pmBd			: Motion Board Parameter
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetBoardParam(SMotionBoard *pmBd);

	/**
	 * ������ Board ���� �б�
	 *
	 * @param	*psiBdNum		: Board ���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetBoardNum(short *psiBdNum);

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
	 * MMC Library : mmc_initx()
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
	virtual int Initialize(short siBdNum, long *plAddress);

	/**
	 * Motion Board �ʱ�ȭ ���� �б�
	 *
	 * @param	*pbInit			: Board �ʱ�ȭ ����, TRUE=�ʱ�ȭ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetBoardInit(BOOL *pbInit);

	/**
	 * Motion Board�� ���� �� ���� �����ش�.
	 *
	 * MMC Library : mmc_all_axes(), mmc_axes()
	 *
	 * @param	siBdNum			: Motion Board ID(0 ~ 7), -1=All Board
	 * @param	*psiAxes		: Board�� ������ ���� �� ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAxes(short siBdNum, short *psiAxes);

	/**
	 * ����, ��, ��ȣ���� ������ ������ �� ��ǥ���� ���� ����
	 * ���� Board�� ������ �����ؾ� �Ѵ�.
	 *
	 * MMC Library : map_axes()
	 *
	 * @param	siAxNum			: �� ��(1 ~ 8)
	 * @param	*psiMapArray	: �� ID �迭(���� Board���� �� ID�̾�� �Ѵ�.)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = NO COMMON MOTION BOARD AXIS(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int MapAxes(short siAxNum, short *psiMapArray);

	/**
	 * ����, ��, ��ȣ���� ������ ������ �� ��ǥ���� ���� ����
	 * Motion ���α׷� ������ �� ���� ���� ���� ���� 2������ �����Ѵ�.
	 * �����ð��� 1msec �����̸� 0���� Ŀ�� �Ѵ�.
	 *
	 * MMC Library : mmcDelay(), mmc_dwell()
	 *
	 * @param	siAxis			: �� ID(-1 ~ 63), -1=���α׷�����
	 * @param	*plDuration		: �����ð�(1msec����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID DELAY DURATION(<=0)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Dwell(short siAxis, long *plDuration);

	/**
	 * I/O Bit�� ������ ���·� �� ������ �ش� ���� ���� ���� ������ �����Ѵ�.
	 * I/O Bit No�� Board ���� ���¿� ���� 32�� ������ �����ȴ�.
	 * (Board#1 : 0 ~ 31, Board #2 : 32 ~ 63, Board #3 : 64 ~ 95, Board #4 : 96 ~ 127)
	 *
	 * MMC Library : mmc_io_trigger()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siBitNo			: I/O Bit No
	 * @param	bState			: I/O Bit ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID BIT IO NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int IOTrigger(short siAxis, short siBitNo, BOOL bState);

	/**
	 * �� ��� ���࿡ ���� ���¸� �����ش�.
	 * ��� ���� �Ϸ�� �����̸� SUCCESS�� Return�Ѵ�.
	 *
	 * MMC Library : in_sequence(), in_motion(), in_position()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: ��ȸ ����, 0=�̵����, 1=�ӵ����, 2=InPosition ������ ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID IN COMMAND TYPE(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int InCommand(short siAxis, short siType);

	/**
	 * ����, ��, ��ȣ���� ������ �Ϸ�Ǿ����� ���θ� �����ش�.
	 * MapAxes()���� ������ ��鿡 ���� Ȯ���Ѵ�.
	 *
	 * MMC Library : all_done()
	 *
	 * @param	*pbStatus		: �Ϸ� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int AllDone(BOOL *pbStatus);

	/**
	 * ���� ���� �Ϸ�� ������ ����Ѵ�.
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
	 *
	 * MMC Library : motion_done(), axis_done()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siMode			: ��� ����, 0=�̵� �� �ӵ��̵� �Ϸ�,
	 *										 1=�̵� �� �ӵ��̵� �Ϸ� & InPosition ������ �̵� �Ϸ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID WAITDONE TYPE(MOTIONLIB)
	 *							  xx = AXIS IS IN COMMAND(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int WaitDone(short siAxis, short siMode);

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
	virtual BOOL IsAxisDone(short siAxis);
	
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
	BOOL IsMotionDone(short siAxis);

	/**
	 * ���� ���� ���� �Ϸ�� ������ ��ٸ���.
	 * *psiAxis�� NULL�� ���޵Ǹ� siAxis�� �� ID�� �νĵǾ� �����Ѵ�.
	 * *psiAxis�� NULL�� �ƴѰɷ� ���޵Ǹ� siAxis�� �� ���� �νĵǾ� �����Ѵ�.
	 *
	 * MMC Library : wait_for_done(), wait_for_all()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63) Ȥ�� �� ��(1 ~ 64)
	 * @param	*psiAxis		:(OPTION=NULL) �� ID �迭, siAxis�� �� ���� ��� ���
	 *																 �� ID��� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int WaitForDone(short siAxis, short *psiAxis = NULL);

	/**
	 * ���� ���� AMP Fault�� Clear�ϰų� Fault Port�� Enable ���·� �����Ѵ�.
	 *
	 * MMC Library : amp_fault_reset(), amp_fault_set()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bState			: ������ ����, FALSE=Clear, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpFaultEnable(short siAxis, BOOL bState);

	/**
	 * �� ��� ���� ���¸� �Ѳ����� �д´�.
	 * psIstatus[0] = axis_source �Լ���
	 * psistatus[1] = in_sequence �Լ���
	 * psistatus[2] = get_com_velocity �Լ���
	 * psistatus[3] = get_act_velocity �Լ���
	 * psistatus[4] = motion_done �Լ���
	 * psistatus[5] = in_position �Լ���
	 * psistatus[6] = axis_done �Լ���
	 * 
	 * plstatus[0] = get_io �Լ���
	 * 
	 * pdstatus[0] = get_position �Լ���
	 * pdstatus[1] = get_command �Լ���
	 * pdstatus[2] = get_error �Լ���
	 *
	 * MMC Library : axis_all_status()
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
	virtual int GetAllStatus(short siAxis, short *psiStatus, long *plStatus, double *pdStatus);

	/**
	 * �ص����� ������ �ӵ���ɰ��� PULSE������ �����ش�.
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Puls
	 */
	virtual int GetComVelocity(short sAxis);

	/**
	 * �� ���� ���¸� �д´�.
	 *
	 * 		ST_NONE					0x0000		������
	 * 		ST_HOME_SWITCH			0x0001		���� ���� ����
	 * 		ST_POS_LIMIT			0x0002		+ ���� ���� ����
	 * 		ST_NEG_LIMIT   			0x0004		- ���� ���� ����
	 * 		ST_AMP_FAULT			0x0008		AMP Fault ���� �߻�
	 * 		ST_A_LIMIT    			0x0010		��, ���� �ð��� limit���� Ŭ �� �߻�
	 * 		ST_V_LIMIT  			0x0020		���� �ӵ��� �ӵ� limit�� ��� �� �߻�
	 * 		ST_X_NEG_LIMIT 			0x0040		- Position limit�� ��� �� �߻�
	 * 		ST_X_POS_LIMIT			0x0080		+ Position limit�� ��� �� �߻�
	 * 		ST_ERROR_LIMIT			0x0100		��ġ������ limit������ Ŭ �� �߻�
	 * 		ST_PC_COMMAND  			0x0200		EVENT�� �߻��� ������ �� �߻�
	 * 		ST_OUT_OF_FRAMES    	0x0400		Frame Buffer�� Full ������ �� �߻�
	 * 		ST_AMP_POWER_ONOFF  	0x0800		AMP Disable ������ �� �߻�
	 * 		ST_ABS_COMM_ERROR   	0x1000		ABS Encoder ��� Error �� �߻�
	 * 		ST_INPOSITION_STATUS	0x2000		??
	 * 		ST_RUN_STOP_COMMAND 	0x4000 		??
	 * 		ST_COLLISION_STATE  	0x8000		??
	 *
	 * MMC Library : axis_source()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiState		: ����, �� ���´� bit �������� �����ȴ�.
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual int GetAxisSource(short siAxis, short *psiState);

	/**
	 * ���� ����(Home, Positive, Negative) ���¸� �д´�.
	 *
	 * MMC Library : home_switch(), pos_switch(), neg_switch()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: ���� ����, 0=Home, 1=Positive, 2=Negative
	 * @param	*pbState		: ���� ����, TRUE=Active, FALSE=No Active
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  xx = INVALID SENSOR TYPE(MOTIONLIB)
	 */
	virtual int GetSensorStatus(short siAxis, short siType, BOOL *pbState);

	/**
	 * AMP Disable/Enable ���¸� �д´�.(Servo ON/OFF)
	 *
	 * MMC Library : get_amp_enable()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbState		: AMP Enable ����, TRUE=Enable, FALSE=Disable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpEnable(short siAxis, BOOL *pbState);

	/**
	 * AMP Enable�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_amp_enable_level(), fget_amp_enable_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Enable Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpEnableLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� Event �߻� ���¸� �д´�.
	 *
	 *		NO_EVENT		0		Event �߻����� ���� ����
	 *		STOP_EVENT		1		stop_rate�� �����ϸ鼭 ����
	 *		E_STOP_EVENT	2		e_stop_rate�� �����ϸ鼭 ����
	 *		ABORT_EVENT		3		AMP disable ����
	 *
	 * MMC Library : axis_state()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiState		: Event ����, 0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual int GetAxisState(short siAxis, short *psiState);

	/**
	 * Board�� Position Latch ���θ� �д´�.
	 *
	 * MMC Library : latch_status()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbState		: Position Latch ����, TRUE=Latch����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual int GetAxisLatchStatus(short siBdNum, BOOL *pbState);

	/**
	 * �࿡ �߻��� Event�� �����ϰ�, ���� ��ɺ��� �����Ѵ�.
	 * Event �߻� �Ŀ��� �׻� Event�� ������ �־�� �Ѵ�.
	 * ABORT_EVENT �߻� �ÿ��� Event ���� �� AMP�� Disable �����̹Ƿ� �ٽ� Enable���־�� �Ѵ�.
	 *
	 * MMC Library : clear_status()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearStatus(short siAxis);

	/**
	 * ���� Frame Buffer�� Clear�Ѵ�.
	 * �ະ �ִ� 50���� Frame�� ������ Clear�Ѵ�. �� ���� �������� ����� ��� ����ȴ�.
	 *
	 * MMC Library : frames_clear()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearFrames(short siAxis);

	/**
	 * ���� ����ִ� Interpolation Frame ������ �����ش�.
	 *
	 * MMC Library : frames_interpolation()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiFrameNo		: Frame ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual int FramesInterpolation(short siAxis, short *psiFrameNo);

	/**
	 * ���� ����ִ� Frame ������ �����ش�.
	 *
	 * MMC Library : frames_left()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiFrameNo		: ����ִ� Frame ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual int FramesLeft(short siAxis, short *psiFrameNo);

	/**
	 * �ش� Board�� Latch ���¸� �����ϰ�, Latch Status�� False�� ����ų�, S/W������ Position�� Latch�Ѵ�.
	 *
	 * MMC Library : latch(), arm_latch()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bType			: Latch ����, FALSE=S/W Position Latch, TRUE=Board Latch Enable/Disable ����
	 * @param	bState			:(OPTION=FALSE) bType=TRUE�� ��� Enable/Disable ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Latch(short siBdNum, BOOL bType, BOOL bState = FALSE);

	/**
	 * ���� ���� Latch�� Position�� �����ش�.
	 *
	 * MMC Library : get_latched_position()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdPosition		: Latch�� Position
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetLatchedPosition(short siAxis, double *pdPosition);

	/**
	 * ������ ��ǥ��ġ�� �������� �� ����Ѵ�.(��, ��ȣ �����߿��� ������� �ʴ´�.)
	 *
	 * MMC Library : compensation_pos()
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
	virtual int CompensationPos(short siLen, short *psiAxes, double *pdDist, short *psiAccel);

	/**
	 * Board DPRAM Address�� �д´�.
	 *
	 * MMC Library : get_dpram_addr()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*plAddr			: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetDpramAddress(short siBdNum, long *plAddr);

	/**
	 * ����ġ Motor�� Type�� �д´�.
	 *
	 * MMC Library : get_abs_encoder_type()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiType		: Motor ����, 1=�ＺCSDJ, CSDJ+SERVO DRIVE, 2=YASKAWA SERVO DRIVE
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAbsEncoderType(short siAxis, short *psiType);

	/**
	 * ���� �̵� �ְ�ӵ��� ��,���� �������� ���Ѱ��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_accel_limit(), fget_accel_limit(), get_vel_limit(), fget_vel_limit()
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
	virtual int GetVelLimit(short siAxis, double *pdVelocity, short *psiAccel, BOOL bBootOpt = FALSE);

	/**
	 * AMP Drive�� Fault �߻� ���¸� �д´�.
	 *
	 * MMC Library : amp_fault_switch()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbStatus		: AMP Fault ���¸� �д´�. TRUE=FAULT, FALSE=NORMAL
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	virtual int GetAmpFaultStatus(short siAxis, BOOL *pbStatus);

	/**
	 * AMP Drive�� Fault �߻� �� ������ Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_amp_fault(), fget_amp_fault()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiAction		: ������ Event, NO EVENT, STOP EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpFaultEvent(short siAxis, short *psiAction, BOOL bBootOpt = FALSE);

	/**
	 * AMP Enable�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_amp_enable_level(), fget_amp_enable_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Enable ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpEanbleLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	/**
	 * AMP Fault�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_amp_fault_level(), fget_amp_fault_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Fault ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpFaultLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	/**
	 * AMP Reset�� Active Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_amp_reset_level(), fget_amp_reset_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbLevel		: Reset ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpResetLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� AMP Drive�� Resolution�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_amp_resolution(), fget_amp_resolution()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiResolution	: AMP Resolution, default=2500 pulse/rev
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAmpResolution(short siAxis, short *psiResolution, BOOL bBootOpt = FALSE);

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
	virtual int GetEncoderRatio(short siAxis, short *psiRatioA, short *psiRatioB, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� ȸ��/������ϴ� ����ȸ�� ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_endless_linearax(), fget_endless_linearax(),
	 *				 get_endless_rotationax(), fget_endless_rotationax()
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
	virtual int GetEndlessAx(short siAxis, BOOL *pbStatus, BOOL bType, BOOL bBootOpt = FALSE);

	/**
	 * ����ȸ�� ���� �����̴� ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_endless_range(), fget_endless_range(),
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdRange		: �̵� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetEndlessRange(short siAxis, double *pdRange, BOOL bBootOpt = FALSE);

	/**
	 * ���� ��ġ���� �Ϸᰪ�� ��ġ���� �� ��ȣ Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_in_position(), fget_in_position(),
	 *				 get_in_position_level(), fget_in_position_level()
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
	virtual int GetInPosition(short siAxis, double *pdInPosition, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� InPosition ��ȣ ��뿩�θ� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_inposition_required(), fget_inposition_required()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbReq			: ��� ����, TRUE=���
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetInpositionRequired(short siAxis, BOOL *pbReq, BOOL bBootOpt = FALSE);

	/**
	 * ���� ��ġ���� ���Ѱ��� Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_error_limit(), fget_error_limit()
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
	virtual int GetErrorLimit(short siAxis, double *pdLimit, short *psiAction, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� �� Encoder�� C�� �޽� �̿� ���θ� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_index_required(), fget_index_required()
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
	virtual int GetIndexRequired(short siAxis, BOOL *pbIndexReq, BOOL bBootOpt = FALSE);

	/**
	 * I/O 8���� ����  ��,��� ��带 �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_io_mode(), fget_io_mode()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbMode			: ��, ��� ���, TRUE=���, FALSE=�Է�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetIOMode(short siBdNum, BOOL *pbMode, BOOL bBootOpt = FALSE);

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ Event�� ��ȣ Level�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_home_level(), fget_home_level(),
	 *				 get_home(), fget_home(),
	 *				 get_negative_level(), fget_negative_level(),
	 *				 get_negative_limit(), fget_negative_limit(),
	 *				 get_positive_level(), fget_positive_level(),
	 *				 get_positive_limit(), fget_positive_limit()
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
	virtual int GetSensorLimit(short siAxis, short siType, short *psiLimit, BOOL *pbLevel, BOOL bBootOpt = FALSE);

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
	virtual int GetSensorEvent(short siAxis, short siType, short *psiLimit, BOOL bBootOpt = FALSE);

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
	virtual int GetSensorLevel(short siAxis, short siType, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	/**
	 * +/- �������� Motor�� �̵��� �� �ִ� ���� ��ġ���� �� ��ġ���� �������� �� ������ Event�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_positive_sw_limit(), fget_positive_sw_limit()
	 *				 get_negative_sw_limit(), fget_negative_sw_limit()
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
	virtual int GetSWLimit(short siAxis, BOOL bType, double *pdPosition, short *psiLimit, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� � Motor�� �����ϴ� ������ �����Ǿ� �ִ��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_stepper(), fget_stepper(), get_micro_stepper(), fget_micro_stepper()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiType		: Motor ����, 0=�ӵ���Servo, 1=�Ϲ�Stepper, 2=MicroStepper Ȥ�� ��ġ��Servo
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetMotorType(short siAxis, short *psiType, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� Feedback ��ġ�� Loop ���¸� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_feedback(), fget_feedback(), get_closed_loop(), fget_closed_loop()
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
	virtual int GetAxisProperty(short siAxis, short *psiDevice, BOOL *pbLoop, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� Pulse ���ֺ�� ���ڱ��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_pulse_ratio(), fget_pulse_ratio(),
	 *				 get_electric_gear(), fget_electric_gear()
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
	virtual int GetRatioProperty(short siAxis, short *psiPgratio, double *pdEgratio, BOOL bBootOpt = FALSE);

	/**
	 * �ӵ��� Servo�� ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_control(), fget_control(), get_unipolar(), fget_unipolar()
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
	virtual int GetVServoProperty(short siAxis, BOOL *pbControl, BOOL *pbPolar, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� Pulse ��� ���¸� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_step_mode(), fget_step_mode()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbMode			: Pulse ��� ����, FALSE=Two Pulse(CW+CCW), TRUE=Sign+Pulse
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetStepMode(short siAxis, BOOL *pbMode, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� Encoder �Է� ����� ��ǥ ������ �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_encoder_direction(), fget_encoder_direction(),
	 *				 get_coordinate_direction(), fget_coordinate_direction()
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
	virtual int GetEncoderDirection(short siAxis, BOOL *pbEncDir, BOOL *pbCoorDir, BOOL bBootOpt = FALSE);

	/**
	 * ������ ���� STOP EVENT, ESTOP EVENT ���� �� ���� �ð��� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_stop_rate(), fget_stop_rate(), get_e_stop_rate(), fget_e_stop_rate()
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
	virtual int GetStopRate(short siAxis, BOOL bType, short *psiRate, BOOL bBootOpt = FALSE);

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
	virtual int GetSyncGain(long *plCoeff);

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
	virtual int GetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� �������� �� �������� ��带 �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_p_integration(), fget_p_integration(),
	 *				 get_v_integration(), fget_v_integration()
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
	virtual int GetIntegration(short siAxis, BOOL bType, BOOL *pbMode, BOOL bBootOpt = FALSE);

	/**
	 * �ӵ����� Ȥ�� ��ũ ���ɿ� ���� Low Pass Filter Ȥ�� Notch Filter�� ���� Filter ���� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_position_lowpass_filter(), fget_position_lowpass_filter(),
	 *				 get_position_notch_filter(), fget_position_notch_filter(),
	 *				 get_velocity_lowpass_filter(), fget_velocity_lowpass_filter(),
	 *				 get_velocity_notch_filter(), fget_velocity_notch_filter()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bCommandType	: ���� ����, FALSE=�ӵ�(Position), TRUE=��ũ(Velocity)
	 * @param	bFilterType		: Filter ����, FALSE=LowPass, TRUE=Notch
	 * @param	*pdFilter		: Filter ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double *pdFilter,
							BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� ���� �� �ӵ��� �д´�.(��� ���� ���� ��)
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_act_velocity(), get_com_velocity(),
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: �ӵ� ����, FALSE=�����ӵ���, TRUE=�ӵ���ɰ�
	 * @param	*psiPulse		: �ӵ��� Pulse��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetVelocity(short siAxis, BOOL bType, short *psiPulse);

	/**
	 * ������ Board�� �ະ ���ۿ��θ� �д´�.
	 *
	 *		b7	b6	b5	b4	b3	b2	b1	b0
	 *		��8	��7	��6	��5	��4	��3	��2	��1
	 *
	 *		bit = TRUE : ���� ����
	 *		bit = FALSE : ���� ����
	 *
	 * MMC Library : get_axis_runstop()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiState		: �ະ ���� ����, bit�� �� ��(b0=��1, b1=��2, ...), TRUE=����, FALSE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAxisRunStop(short siBdNum, short *psiState);

	/**
	 * ���� ������ġ �� ��ǥ��ġ�� �д´�.
	 *
	 * MMC Library : get_command(), get_position()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bCommandType    : ��ġ ����, FALSE=������ġ(Encoder)
	 *                                       TRUE=��ǥ��ġ(Command)
	 * @param	*pdPosition		: bType=FALSE�̸�, ������ ������ġ
	 *							  bType=TRUE �̸�, ������ ��ǥ��ġ
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetPosition(short siAxis, BOOL bCommandType, double *pdPosition);

	/**
	 * Motor�� ����ġ RPM�̳� ���� RPM�� �д´�.
	 *
	 * MMC Library : get_command_rpm(), get_encoder_rpm()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: RMP ����, FALSE=����RPM, TRUE=����ġRPM
	 * @param	*psiRpm			: RPM��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetRpm(short siAxis, BOOL bType, short *psiRpm);

	/**
	 * Board�� Sampling Rate�� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_control_timer(), fget_control_timer()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiTime		: Sampling Rate, msec����(1=4msec, 2=2msec, 3=1msec�� ����)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetControlTimer(short siBdNum, short *psiTime, BOOL bBootOpt = FALSE);

	/**
	 * ���� ��ǥ��ġ�� ������ġ�� ���̰��� ��ġ������ �д´�.
	 *
	 * MMC Library : get_error()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdError		: ��ġ����,(��ǥ��ġ-������ġ)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetError(short siAxis, double *pdError);

	/**
	 * Ư�� ���� Encoder Feedback Data�� ������ �о���� �� ���(50usec �ֱ� Update)
	 *
	 * MMC Library : get_fast_read_encoder()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pbStatus		: ���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetFastReadEncoder(short siAxis, BOOL *pbStatus);

	/**
	 * �ش� ���� Analog Offset ���� �д´�.
	 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
	 *
	 * MMC Library : get_analog_offset(), fget_analog_offset()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*psiOffset		: Analog Offer, +/-2048, +/-32767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAnalogOffset(short siAxis, short *psiOffset, BOOL bBootOpt = FALSE);

	/**
	 * ��, ��� Port�� 32bit Data�� �д´�.
	 *
	 * MMC Library : get_io(), get_out_io()
	 *
	 * @param	siPort			: ��, ��� Port ��ȣ(0 ~ 3, Board ���� ������ ���� ����)
	 * @param	bType			: ��, ��� ����, FALSE=�Է�, TRUE=���
	 * @param	*plValue		: 32bit Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO PORT ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetIO(short siPort, BOOL bType, long *plValue);

	/**
	 * ������ Analog ��/����� 12/16bit Data ���� �д´�.
	 *
	 * MMC Library : get_analog(), get_dac_output()
	 *
	 * @param	siChannel		: Analog �Է� ä�� ��(0 ~ 7) Ȥ�� ��� �� ID(0 ~ 63)
	 * @param	bType			: ��, ��� ����, FALSE=�Է�, TRUE=���
	 * @param	*psiValue		: bType=FALSE�̸� Analog �Է� ��, -2048 ~ +2047
	 *							  bType=TRUE�̸� Analog ��� ��, +/-2048, +/-32767
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID ANALOG INPUT CHANNEL ID(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAnalog(short siChannel, BOOL bType, short *psiValue);

	/**
	 * Board�� �浹���� ����� ��뿩�θ� �д´�.
	 *
	 * MMC Library : get_collision_prevent_flag()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbMode			: ��뿩��, TRUE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetCollisionPreventFlag(short siBdNum, BOOL *pbMode);

	/**
	 * �������� ���θ� �д´�.
	 *
	 * MMC Library : get_sync_control()
	 *
	 * @param	*pbState		: ���� ����, TRUE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetSyncControl(BOOL *pbState);

	/**
	 * Master��� Slave���� ������ġ�� �д´�.
	 *
	 * MMC Library : get_sync_position()
	 *
	 * @param	*pdMasterPos	: Master �� ��ġ
	 * @param	*pdSlavePos		: Slave �� ��ġ
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetSyncPosition(double *pdMasterPos, double *pdSlavePos);

	/**
	 * ���庰�� I/O Interrupt�� Enable/Diable�ϰų�, I/O Interrupt �߻� ��
	 * STOP-EVENT�� E-STOP-EVENT�� �����࿡ �߻����� ���θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : io_interrupt_enable(), fio_interrupt_enable(),
	 *				 io_interrupt_on_e_stop(), fio_interrupt_on_e_stop(),
	 *				 io_interrupt_on_stop(), fio_interrupt_on_stop()
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
	virtual int IOInterrupt(short siID, short siType, BOOL bState, BOOL bBootOpt = FALSE);

	/**
	 * I/O Interrupt �߻� �� PC������ Interrupt�� �߻���ų�� ���θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : io_interrupt_pcirq(), fio_interrupt_pcirq()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bState			: �߻� ����, TRUE=�߻�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int IOInterruptPCIRQ(short siBdNum, BOOL bState, BOOL bBootOpt = FALSE);

	/**
	 * PC Interrupt �߻� �� end of interrupt ��ȣ�� �߻���Ų��.
	 *
	 * MMC Library : io_interrupt_pcirq_eoi()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int IOInterruptPCIRQ_EOI(short siBdNum);

	/**
	 * ���� ���� PID ���� ���θ� �����Ѵ�.
	 *
	 * MMC Library : controller_idle(), controller_run()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bState			: PID ���� ����, FALSE=PID���� �̽ǽ�, Analog ��� 0volt,
	 *											 TRUE =PID���� �ǽ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetController(short siAxis, BOOL bState);

	/**
	 * AMP Disable/Enable ���¸� �����Ѵ�.
	 *
	 * MMC Library : set_amp_enable()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bState			: AMP Enable ����, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpEnable(short siAxis, BOOL bState);

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
	virtual int SetVelLimit(short siAxis, double dVelocity, short siAccel);

	/**
	 * AMP Drive�� Fault �߻� �� ������ Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_amp_fault(), fset_amp_fault()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siAction		: ������ Event, NO EVENT, STOP EVENT, ESTOP EVENT, ABORT EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID EVENT VALUE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpFaultEvent(short siAxis, short siAction, BOOL bBootOpt = FALSE);

	/**
	 * AMP Enable�� Active Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_amp_enable_level(), fset_amp_enable_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bLevel			: Enable Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpEnableLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE);

	/**
	 * AMP Fault�� Active Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_amp_fault_level(), fset_amp_fault_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bLevel			: Fault Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpFaultLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE);

	/**
	 * AMP Reset�� Active Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_amp_reset_level(), fset_amp_reset_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bLevel			: Reset Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpResetLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� AMP Drive�� Resolution�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_amp_resolution(), fset_amp_resolution()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siResolution	: AMP Resolution, default=2500 pulse/rev
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID RESOLUTION NUMBER(<0)(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpResolution(short siAxis, short siResolution, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� ���ֺ� ���� ���ڰ�, �и��� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_encoder_ratioa(), fset_encoder_ratioa(),
	 *				 set_encoder_ratiob(), fset_encoder_ratiob()
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
	virtual int SetEncoderRatio(short siAxis, short siRatioA, short siRatioB, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� ȸ��/������ϴ� ����ȸ�� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_endless_linearax(), fset_endless_linearax(),
	 *				 set_endless_rotationax(), fset_endless_rotationax()
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
	virtual int SetEndlessAx(short siAxis, BOOL bStatus, short siResolution, BOOL bType,
							   BOOL bBootOpt = FALSE);

	/**
	 * ����ȸ�� ���� �����̴� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_endless_range(), fset_endless_range()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dRange			: �̵� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetEndlessRange(short siAxis, double dRange, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� �� Encoder�� C�� �޽� �̿� ���θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_index_required(), fset_index_required()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bIndexReq		: C�� �޽� ��� ����, TRUE =Home Sensor�� Encoder�� Index Pulse�� ���� ����,
	 *												  FALSE=Home Sensor�� ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetIndexRequired(short siAxis, BOOL bIndexReq, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� �ش� Motor ������ �����ϴ� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_servo(), fset_servo(),
	 *				 set_stepper(), fset_stepper(),
	 *				 set_micro_stepper(), fset_micro_stepper()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Motor ����, 0=�ӵ���Servo, 1=�Ϲ�Stepper, 2=MicroStepper Ȥ�� ��ġ��Servo
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID MOTOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetMotorType(short siAxis, short siType, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� Feedback ��ġ�� Loop ���¸� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_feedback(), fset_feedback(),
	 *				 set_closed_loop(), fset_closed_loop()
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
	virtual int SetAxisProperty(short siAxis, short siDevice, BOOL bLoop, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� ���ֺ�� ���ڱ��� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_pulse_ratio(), fset_pulse_ratio(),
	 *				 set_electric_gear(), fset_electric_gear()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siPgratio		: Pulse ���ֺ�, default=8
	 * @param	dEgratio		: ���ڱ���, default=1.0
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetRatioProperty(short siAxis, short siPgratio, double dEgratio, BOOL bBootOpt = FALSE);

	/**
	 * �ӵ��� Servo�� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_control(), fset_control(), set_unipolar(), fset_unipolar()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bControl		: ������, FALSE=�ӵ�����, TRUE=��ũ����
	 * @param	bPolar			: Analog ��� ����,  TRUE=UNIPOLAR, FALSE=BIPOLER
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetVServoProperty(short siAxis, BOOL bControl, BOOL bPolar, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� Pulse ��� ���¸� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_step_mode(), fset_step_mode()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bMode			: Pulse ��� ����, FALSE=Two Pulse(CW+CCW), TRUE=Sign+Pulse
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetStepMode(short siAxis, BOOL bMode, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� Encoder �Է� ����� ��ǥ ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_encoder_direction(), fset_encoder_direction(),
	 *				 set_coordinate_direction(), fset_coordinate_direction()
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
	virtual int SetEncoderDirection(short siAxis, BOOL bEncDir, BOOL bCoorDir, BOOL bBootOpt = FALSE);

	/**
	 * I/O 8���� ����  ��,��� ��带 �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_io_mode(), fset_io_mode()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bMode			: ��, ��� ���, TRUE=���, FALSE=�Է�
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetIOMode(short siBdNum, BOOL bMode, BOOL bBootOpt = FALSE);

	/**
	 * ���� ��ġ���� �Ϸᰪ�� ��ġ���� �� ��ȣ Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_in_position(), fset_in_position(),
	 *				 set_inposition_level(), fset_inposition_level()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dInPosition		: ��ġ ������
	 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FALSE=LOW
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetInPosition(short siAxis, double dInPosition, BOOL bLevel, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� InPosition ��ȣ ��뿩�θ� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_inposition_required(), fset_inposition_required()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bReq			: ��� ����, TRUE=���
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetInpositionRequired(short siAxis, BOOL bReq, BOOL bBootOpt = FALSE);

	/**
	 * ���� ��ġ���� Limit���� Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_error_limit(), fset_error_limit()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dLimit			: ��ġ���� Limit��, �ִ� 35000 count
	 * @param	siAction		: ��ġ���� Event, NO EVENT, STOP EVENT, ESTOP EVENT
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetErrorLimit(short siAxis, double dLimit, short siAction, BOOL bBootOpt = FALSE);

	/**
	 * ������ ���� STOP EVENT, ESTOP EVENT ���� �� ���� �ð��� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_stop_rate(), fset_stop_rate(), set_e_stop_rate(), fset_e_stop_rate()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ���� ����, FALSE=STOP, TRUE=ESTOP
	 * @param	siRate			: ���� �ð�, default=10
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetStopRate(short siAxis, BOOL bType, short siRate, BOOL bBootOpt = FALSE);

	/**
	 * Home, +/- ���� Limit Switch Active�� ������ Event�� ��ȣ Level�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_home_level(), fset_home_level(),
	 *				 set_home(), fset_home(),
	 *				 set_positive_level(), fset_positive_level(),
	 *				 set_positive_limit(), fset_positive_limit(),
	 *				 set_negative_level(), fset_negative_level(),
	 *				 set_negative_limit(), fset_negative_limit()
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
	virtual int SetSensorLimit(short siAxis, short siType, short siLimit, BOOL bLevel, BOOL bBootOpt = FALSE);

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
	virtual int SetSensorEvent(short siAxis, short siType, short siLimit, BOOL bBootOpt = FALSE);

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
	virtual int SetSensorLevel(short siAxis, short siType, BOOL bLevel, BOOL bBootOpt = FALSE);

	/**
	 * +/- �������� Motor�� �̵��� �� �ִ� Limit ��ġ���� �� ��ġ���� �������� �� ����� Event�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_positive_sw_limit(), fset_positive_sw_limit(),
	 *				 set_negative_sw_limit(), fset_negative_sw_limit()
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
	virtual int SetSWLimit(short siAxis, BOOL bType, double dPosition, short siLimit, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� �ӵ� �Ǵ� ��ġ�� ���� PID & FF Gain ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_gain(), fset_gain(), set_v_gain(), fset_v_gain()
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
	virtual int SetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = FALSE);

	/**
	 * �ش� ���� ���� ���� �� ���� ���� ��带 �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_p_integration(), fset_p_integration(),
	 *				 set_v_integration(), fset_v_integration()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ������, FALSE=��ġ, TRUE=�ӵ�
	 * @param	bMode			: �������� ���, FALSE=�׻�����, TRUE=����������
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetIntegration(short siAxis, BOOL bType, BOOL bMode, BOOL bBootOpt = FALSE);

	/**
	 * �ӵ����� Ȥ�� ��ũ ���ɿ� ���� Low Pass Filter Ȥ�� Notch Filter�� ���� Filter ���� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_position_lowpass_filter(), fset_position_lowpass_filter(),
	 *				 set_position_notch_filter(), fset_position_notch_filter(),
	 *				 set_velocity_lowpass_filter(), fset_velocity_lowpass_filter(),
	 *				 set_velocity_notch_filter(), fset_velocity_notch_filter()
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
	virtual int SetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double dFilter,
							BOOL bBootOpt = FALSE);

	/**
	 * ������ Board�� �ະ ���� ���θ� �����Ѵ�.
	 *
	 *		b7	b6	b5	b4	b3	b2	b1	b0
	 *		��8	��7	��6	��5	��4	��3	��2	��1
	 *
	 *		bit = TRUE : ���� ����
	 *		bit = FALSE : ���� ����
	 *
	 * MMC Library : set_axis_runstop()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siState			: �ະ ���� ����, bit�� �� ��(b0=��1, b1=��2, ...), TRUE=����, FALSE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAxisRunStop(short siBdNum, short siState);

	/**
	 * ���� I/O bit�� HIGH(1)/LOW(0) ���·� �����.
	 *
	 *		 Board��	  I/O Bit ����
	 *			1			0  ~ 31
	 *			2			32 ~ 63
	 *			3			64 ~ 95
	 *			4			96 ~ 127
	 *
	 * MMC Library : set_bit(), reset_bit()
	 *
	 * @param	siBitNo			: ������ I/O Bit ��ȣ(������ Board�� ������ ���� �޶���)
	 * @param	bValue			: ������ ��,(TRUE, FALSE)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO BIT NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetBit(short siBitNo, BOOL bValue);

	/**
	 * 32bit�� I/O Data�� ��� Port�� ���� ��������.
	 *
	 * MMC Library : set_io()
	 *
	 * @param	siPort			: ��� Port ��ȣ(0 ~ 3, Board ���� ������ ���� ����)
	 * @param	lValue			: ��� ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID IO PORT ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetIO(short siPort, long lValue);

	/**
	 * 12/16 bit Analog ��������� ��������.
	 *
	 * MMC Library : set_dac_output()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siValue			: ����� ���� ��, +/-2048, +/-32767
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetDacOut(short siAxis, short siValue);

	/**
	 * �ش� ���� Analog Offset���� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_analog_offset(), fset_analog_offset()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siOffset		: Analog OFfset, +/-2048, +/-32767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAnalogOffset(short siAxis, short siOffset, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� ��������� ������ �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_analog_limit(), fset_analog_limit()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siLimit			: A��� ���� ����, 0 ~ 32767
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAnalogLimit(short siAxis, short siLimit, BOOL bBootOpt = FALSE);

	/**
	 * ���� ���� ��ġ �� ��ǥ ��ġ�� �����Ѵ�.(AMP Disable ���¿��� �����ϴ°� ����.)
	 *
	 * MMC Library : set_command(), set_position()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bType			: ��ġ ����, FALSE=������ġ, TRUE=��ǥ��ġ
	 * @param	dPosition		: bType=FALSE�̸� ������ ������ġ, bType=TRUE�̸� ������ ��ǥ��ġ
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetPosition(short siAxis, BOOL bType, double dPosition);

	/**
	 * Ư�� ���� Encoder Feedback Data�� ������ �о���� �� ���(50usec �ֱ� Update)
	 *
	 * MMC Library : set_fast_read_encoder()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	bStatus			: ���� ����, TRUE=����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetFastReadEncoder(short siAxis, BOOL bStatus);

	/**
	 * ����ڰ� Motion ���� S/W�� ��ü �����Ͽ� �ý����� ���۽�ų �� �ֵ��� �������ִ� ���
	 *
	 * MMC Library : set_interpolation()
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
	virtual int SetInterpolation(short siLen, short *psiAxes, long *plDelt, short siFlag);

	/**
	 * �浹���� ����� ����� Mastr/Slave�� �� �浹���� �Ÿ� �� ����(+, -, >, <)�� �����Ѵ�.
	 *
	 * MMC Library : set_collision_prevent()
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
		virtual int SetCollisionPrevent(short siMasterAx, short siSlaveAx,
									  BOOL bAddSub, BOOL bNonEqual, double dPosition, BOOL bState);

	/**
	 * �浹���� ����� ��뿩�θ� �����Ѵ�.
	 *
	 * MMC Library : set_collision_prevent_flag()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bMode			: ��� ����, TRUE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetCollisionPreventFlag(short siBdNum, BOOL bMode);

	/**
	 * Board DPRAM Address�� �����Ѵ�.
	 *
	 * MMC Library : set_dpram_addr()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	lAddr			: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetDpramAddress(short siBdNum, long lAddr);

	/**
	 * ����ġ Motor�� Type�� �����Ѵ�.
	 *
	 * MMC Library : set_abs_encoder_type()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: Motor ����, 1=�ＺCSDJ, CSDJ+SERVO DRIVE, 2=YASKAWA SERVO DRIVE
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID ABS MOTOR TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAbsEncoderType(short siAxis, short siType);

	/**
	 * ����ġ Motor�� �����Ѵ�.
	 *
	 * MMC Library : set_abs_encoder()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAbsEncoder(short siAxis);

	/**
	 * Servo Linear Flag ���¸� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_servo_linear_flag(), fset_servo_linear_flag()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siFlag			: Servo Linear Flag ����
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetServoLinearFlag(short siAxis, short siFlag, BOOL bBootOpt = FALSE);

	/**
	 * �������� ���θ� �����Ѵ�.
	 *
	 * MMC Library : set_sync_control()
	 *
	 * @param	bState			: ���� ����, TRUE=�������� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetSyncControl(BOOL bState);

	/**
	 * ���������� Master��� Slave���� �����Ѵ�.
	 *
	 * MMC Library : set_sync_map_axes()
	 *
	 * @param	siMasterAx		: Master �� ID(0 ~ 63)
	 * @param	siSlaveAx		: Slave �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetSyncMapAxes(short siMasterAx, short siSlaveAx);

	/**
	 * ��������� ����Ǵ� ���� Gain���� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_sync_gain(), fset_sync_gain()
	 *
	 * @param	siCoeff			: ���� Gain ��
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetSyncGain(short siCoeff, BOOL bBootOpt = FALSE);

	/**
	 * Board�� Sampling Rate�� �����Ѵ�.
	 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
	 *
	 * MMC Library : set_control_timer(), fset_control_timer()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siTime			: Sampling Time(msec ����)(1=4msec, 2=2msec, 3=1msec�� ����)
	 * @param	bBootOpt		:(OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID SAMPLING RATE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetControlTimer(short siBdNum, short siTime, BOOL bBootOpt = FALSE);

//#if FALSE	// MC Library Header File�� ����(MMCWFI30.h)
	/**
	 * PositionIoOnOff()�� ������ ���� �����Ѵ�.
	 *
	 * MMC Library : position_io_allclear(), position_io_clear()
	 *
	 * @param	siPosNum		:(OPTION=0) ��ġ ��ȣ, 1 ~ 10, 0=��� ��ġ ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION IO NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
//	virtual int PositionIOClear(short siPosNum = 0);
//#endif

//#if FALSE	// MC Library Header File�� ����(MMCWFI30.h)
	/**
	 * ���� ���� ������ ��ġ�� ���� �� ���� IO�� ����Ѵ�.
	 *
	 * MMC Library : position_io_onoff()
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
//	virtual int PositionIoOnoff(short siPosNum, short siIONum, short siAxis, double dPosition, BOOL bEncFlag);
//#endif

	/**
	 * ���� ���� Ȥ�� ����, ��, ��ȣ���� ���� �� �ӵ��� ��,���ӵ��� �����Ѵ�.
	 *
	 * MMC Library : set_move_accel(), set_move_speed()
	 *
	 * @param	dVelocity		: �ӵ�
	 * @param	siAccel			: ��,���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetMoveSpeed(double dVelocity, short siAccel);

	/**
	 * �ڵ� ��,���� ����� ��뿩�θ� �����Ѵ�.
	 *
	 * MMC Library : set_spl_auto_off()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bState			: ��뿩��, FALSE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetSplAutoOff(short siBdNum, BOOL bState);

	/**
	 * �ڵ� ��,���� ����� ��뿩�θ� �д´�.
	 * Library�� �����Ǵ� �Լ��� ���� ����� ���� Data���� �о�´�.
	 *
	 * MMC Library : 
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*pbState		: ��뿩��, TURE=���
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetSplAutoOff(short siBdNum, BOOL *pbState);

	/*
	 *	�� �����̺�Ʈ(V-Stop)�� �߻��Ѵ�.
	 */
	virtual int SetStopEvent(short siAxis);

	/**
	 * �� �̵��� �����Ѵ�.
	 * �Ϲ�����, �������, �ӵ��̵������� �����Ѵ�.
	 *
	 * MMC Library : set_stop(), set_e_stop(), v_move_stop()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	siType			: ���� ����, 0=STOP, 1=ESTOP, 2=VSTOP
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID STOP TYPE(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetStop(short siAxis, short siType);

	/**
	 * 1�� �ӵ� Profile �̵�(�� 1�� ������ �̵� ������)
	 *
	 * MMC Library : move(), start_move(),
	 *				 s_move(), start_s_move(),
	 *				 r_move(), start_r_move(),
	 *				 rs_move(), start_rs_move(),
	 *				 t_move(), start_t_move(),
	 *				 ts_move(), start_ts_move(),
	 *				 tr_move(), start_tr_move(),
	 *				 trs_move(), start_trs_move()
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
	virtual int Move(short siAxis, double dPosition, double dVelocity, short siAccel,
					   short siDecel, short siType, BOOL bWaitOpt = FALSE);

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
	 * MMC Library : move_all(), start_move_all(),
	 *				 s_move_all(), start_s_move_all(),
	 *				 t_move_all(), start_t_move_all(),
	 *				 ts_move_all(), start_ts_move_all(),
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
	virtual int MoveAll(short siLen, short *psiAxes, double *pdPosition, double *pdVelocity,
						  short *psiAccel, short *psiDecel, short siType, BOOL bWaitOpt = FALSE);

	/**
	 * ������ n���� �־��� ��ǥ����ŭ �����̵��� �Ѵ�.(�ٸ� Board�� �� ��� �Ұ�)
	 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
	 *
	 *		|----------------> siLen = n <----------------|
	 *		+---------+---------+---------+-----+---------+
	 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
	 *		+---------+---------+---------+-----+---------+
	 *
	 * MMC Library : move_2(), move_3(), move_4(), move_n(),
	 *				 smove_2(), smove_3(), smove_4(), smove_n()
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
	virtual int MoveN(short siLen, double *pdPosition, short siType);

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
	 * MMC Library : move_2ax(), move_3ax(), move_4ax(), move_nax()
	 *				 smove_2ax(), smove_3ax(), smove_4ax(), smove_nax()
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
	virtual int MoveNAx(short siLen, short *psiAxes, double *pdPosition, short siType,
						  double dVelocity, short siAccel);

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
	 * MMC Library : move_2axgr(), move_3axgr(), move_4axgr(),
	 *				 smove_2axgr(), smove_3axgr(), smove_4axgr()
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
	virtual int MoveNAxGr(short siGrpNum, short siLen, short *psiAxes, double *pdPosition,
							short siType, double dVelocity, short siAccel);

	/**
	 * ���� �� ��� �̵��Ѵ�.(�� 1�� �����θ� ���� �����ϴ�.)
	 *
	 * MMC Library : v_move()
	 *
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	dVelocity		: �̵� �ӵ�
	 * @param	siAccel			: �̵� ���ӵ�
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VMove(short siAxis, double dVelocity, short siAccel);

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
	 * MMC Library : spl_arc_move2(), spl_arc_move3()
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
	virtual int SplArcMove(short siAxNum, double dCenterX, double dCenterY,
							 double *pdPoint, double dVelocity, short siAccel, BOOL bDir);

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
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SplArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY,
								double *pdPoint, double dVelocity, short siAccel, BOOL bDir);

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
	virtual int SplAutoArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY,
								  double *pdPoint, double dVelocity, short siAccel, BOOL bDir, BOOL bAutoFlag);

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
	 * MMC Library : spl_line_move1(), spl_line_move2(), spl_line_move3()
	 *
	 * @param	siAxNum			: �� ��, 1=1��, 2=2��, 3=3��
	 * @param	*pdPoint		: �̵��� ��ǥ, 1���̸� 1�����迭, 2���̸� 2�����迭, 3���̸� 3�����迭
	 * @param	dVelocity		: �̵� �ӵ�(0.0=SetMoveSpeed()���� ������ �ӵ� ���)
	 * @param	siAccel			: �̵� ���ӵ�(0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SplLineMoveN(short siAxNum, double *pdPoint, double dVelocity, short siAccel);

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
	virtual int SplLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel);

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
	virtual int SplAutoLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel, BOOL bAutoFlag);

	/**
	 * ��, ��ȣ �̵� �� ���ּӵ��� �����Ѵ�.
	 *
	 * MMC Library : set_arc_division()
	 *
	 * @param	dDegree			: ���ּӵ�, 0 < dDegree < 1000.0
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID ARC DIVISION DEGREE NUMBER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetArcDivision(double dDegree);

	/**
	 * �־��� �߽ɿ��� ������ ������ŭ ��ȣ�� �׸��� ������ �����Ѵ�.
	 * (�ٸ� Board�� �� ��� �Ұ�)
	 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
	 *
	 * MMC Library : arc_2()
	 *
	 * @param	siCenterX		: ȸ�� �߽� X��ǥ
	 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
	 * @param	dAngle			: ȸ�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Arc2(double dXCenter, double dYCenter, double dAngle);

	/**
	 * �־��� �߽ɿ��� ������ ������ŭ ��ȣ�� �׸��� ������ �����Ѵ�.
	 * ���� Board�� ���� �ƴϸ� Error ó����.
	 * (�ٸ� Board�� �� ��� �Ұ�)
	 *
	 * MMC Library : arc_2ax()
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
	virtual int Arc2Ax(short siAxis1, short siAxis2, double dXCenter, double dYCenter,
						 double dAngle, double dVelocity, short siAccel);

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
	 * MMC Library : rect_move()
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
	virtual int RectMove(short siAxis1, short siAxis2, double *pdPoint,
						   double dVelocity, short siAccel);

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
	 * MMC Library : spl_move()
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
	virtual int SplMove(short siLen, short siAxis1, short siAxis2, short siAxis3,
						  double *pdPointX, double *pdPointY, double *pdPointZ,
						  double dVelocity, short siAccel);

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
	 * MMC Library : spl_move_data()
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
	virtual int SplMoveData(short siSplNum, short siLen, short siAxis1, short siAxis2,
							  short siAxis3, double *pdPoint1, double *pdPoint2,
							  double *pdPoint3, double dVelocity, short siAccel);

	/**
	 * ���� 3���� Spline Motion���� SplMoveData()���� ������ ��ġ�� �����ϸ鼭 ���� �̵��Ѵ�.
	 *
	 * MMC Library : spl_movex()
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
	virtual int SplMovex(short siSplNum, short siAxis1, short siAxis2, short siAxis3);

	/**
	 * �� Board�� ROM Version�� �д´�.
	 *
	 * MMC Library : version_chk()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	*psiVersion		: ROM Version, 101 => 1.01
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VersionCheck(short siBdNum, short *psiVersion);

	/**
	 * �ش� Error Code�� Error Message�� ��ȯ�Ѵ�.
	 *
	 * MMC Library : error_message(), _error_message()
	 *
	 * @param	siCode			: Error Code
	 * @param	*pcDst			: Error Message, ERR_MAX_ERROR_LEN(80)���� ũ�ų� ���ƾ� �Ѵ�.
	 * @return	Error Code		: 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ErrorMessage(short siCode, char *pcDst);

	/**
	 * Position Compare Board�� �ʱ�ȭ�Ѵ�.(�� 2�� ����� ���� ���� Board�� ���̾�� �Ѵ�.)
	 *
	 * MMC Library : position_compare_init()
	 *
	 * @param	siIndexSel		: Position Compare�� ��, 1 ~ 2
	 * @param	siAxis1			: ��1 ID(0 ~ 63), ���� Board�� ���̾�� �Ѵ�.
	 * @param	siAxis2			: ��2 ID(0 ~ 63), ���� Board�� ���̾�� �Ѵ�.(siIndexSel=2�� ���� ����)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID POSITION COMPARE INDEXSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int PositionCompareInit(short siIndexSel, short siAxis1, short siAxis2);

	/**
	 * Position Compare�� �����Ѵ�.(Standard Type)
	 * ���� Board�� ���� �ƴϸ� Error ó����.
	 * Board ������ ����Ѵ�.
	 *
	 * MMC Library : position_compare()
	 *
	 * @param	siIndexSel		: Position Compare�� ����� �� ��, 1 ~ 2
	 * @param	siIndexNum		: Position Compare�� �ǽ��� Index ��ȣ, 1 ~ 8
	 * @param	siBitNo			: ����� I/O Bit ��ȣ, 0 ~ 31
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
	virtual int PositionCompare(short siIndexSel, short siIndexNum, short siBitNo, short siAxis1,
								  short siAxis2, BOOL bLatch, short siFunction, short siOutMode,
								  double dPosition, long lTime);

	/**
	 * Position Compare�� �����Ѵ�.(Interval Type)
	 *
	 * MMC Library : position_compare_interval()
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
	virtual int PositionCompareInterval(BOOL bDir, short siAxis, short siBitNo, double dStartPos, double dLimitPos,
								  long lInterval, long lTime);

	/**
	 * Position Compare ������ �� ������ ���θ� �����Ѵ�.
	 *
	 * MMC Library : position_compare_enable()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	bFlag			: FALSE=Position Compare ���� Disable, TRUE=Enable
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int PositionCompareEnable(short siBdNum, BOOL bFlag);

	/**
	 * Position Compare�� Index�� �ʱ�ȭ�Ѵ�.
	 *
	 * MMC Library : position_compare_index_clear()
	 *
	 * @param	siBdNum			: Board ID(0 ~ 7)
	 * @param	siIndexSel		: �׻� "1"�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int PositionCompareClear(short siBdNum, short siIndexSel);

	/**
	 * Position Compare ������ ���� Encoder ���� �о��.
	 *
	 * MMC Library : position_compare_read()
	 *
	 * @param	siIndexSel		: �׻� "1"�� ����
	 * @param	siAxis			: �� ID(0 ~ 63)
	 * @param	*pdPosition		: Encoder ��
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int PositionCompareRead(short siIndexSel, short siAxis, double *pdPosition);

private:	// internal operation

	// �ݿø� Function
	double motionRoundValue(double dVal);

	// MMC Library�� �����ϴ� ���� �ڵ带 �����Ѵ�.
	int makeMMCError(int iError);

};
#endif // MMMCLIB_H

// End of File MMMCLib.h
