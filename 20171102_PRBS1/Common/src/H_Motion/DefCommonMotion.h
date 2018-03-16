/* 
 * DefCommonMotion Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * DefCommonMotion.h : This file defines the value of Motion component.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 *		   Heon-sik Jeong (heonsik@samsung.com)
 * @version $Revision$
 */

#ifndef DEFCOMMONMOTION_H
#define DEFCOMMONMOTION_H

const	int ERR_MOTION_SUCCESS			= 0;		// Success
const	int ERR_MOTION_FAIL				= -1;

const	int	MAX_LENGTH_AXIS_NAME		= 32;

// Move ����
const	int	DEF_MOVE_POSITION			= 0;			// ��ٸ��� �ӵ� Profile, ������ǥ �̵�
const	int	DEF_SMOVE_POSITION			= 1;			// S-Curve �ӵ� Profile, ������ǥ �̵�
const	int	DEF_MOVE_DISTANCE			= 2;			// ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
const	int	DEF_SMOVE_DISTANCE			= 3;			// S-Curve �ӵ� Profile, ���Ÿ� �̵�
const	int	DEF_TMOVE_POSITION			= 4;			// ���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
const	int	DEF_TSMOVE_POSITION			= 5;			// ���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
const	int	DEF_TMOVE_DISTANCE			= 6;			// ���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
const	int	DEF_TSMOVE_DISTANCE			= 7;			// ���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�

// Stop ����
const	int	DEF_STOP					= 0;			// Stop
const	int	DEF_ESTOP					= 1;			// E-Stop
const	int	DEF_VSTOP					= 2;			// V-Stop

// Limit Sensor ����
const	int	DEF_HOME_SENSOR				= 0;			// Home Sensor
const	int	DEF_POSITIVE_SENSOR			= 1;			// Positive Sensor
const	int	DEF_NEGATIVE_SENSOR			= 2;			// Negative Sensor

// S/W Limit ����
const	BOOL	DEF_POSITIVE_SW			= TRUE;			// S/W Positive Limit
const	BOOL	DEF_NEGATIVE_SW			= FALSE;		// S/W Negative Limit

// Level ����
const	BOOL	DEF_HIGH				= TRUE;			// High Level
const	BOOL	DEF_LOW					= FALSE;		// Low Level

// Event ����
const	int	DEF_NO_EVENT				= 0;			// ignore a condition
const	int	DEF_STOP_EVENT				= 1;			// generate a stop event
const	int	DEF_E_STOP_EVENT			= 2;			// generate an e_stop event
const	int	DEF_ABORT_EVENT				= 3;			// disable PID control, and disable the amplifier

// Motor ����
const	int	DEF_SERVO_MOTOR				= 0;			// �ӵ��� Servo
const	int	DEF_STEPPER					= 1;			// �Ϲ� Stepper
const	int	DEF_MICRO_STEPPER			= 2;			// Micro Stepper Ȥ�� ��ġ�� Servo

// Feedback ����
const	int	DEF_FB_ENCODER				= 0;			// Encoder Feedback Device
const	int	DEF_FB_UNIPOLAR				= 1;			// Unipolar Feedback Device
const	int	DEF_FB_BIPOLAR				= 2;			// Bipolar Feedback Device

// Control_Loop ����
const	BOOL	DEF_OPEN_LOOP			= FALSE;		// Open Loop
const	BOOL	DEF_CLOSED_LOOP			= TRUE;			// Closed Loop

// Control ���
const	BOOL	DEF_V_CONTROL			= FALSE;		// �ӵ�����
const	BOOL	DEF_T_CONTROL			= TRUE;			// ��ũ����

const	BOOL	DEF_UNIPOLAR			= FALSE;
const	BOOL	DEF_BIPOLAR				= TRUE;

// ���и�� ���� ���
const	int	DEF_IN_STANDING				= 0;			// �����ø� ����
const	int	DEF_IN_ALWAYS				= 1;			// �׻� ����

// Pulse ����
const	int	DEF_TWO_PULSE				= 0;			// Two Pulse, CW+CCW
const	int	DEF_SIGN_PULSE				= 1;			// Sign + Pulse

// Encoder Direction
const	int	DEF_CORD_CCW 				= 1;			// �ݽð����
const	int	DEF_CORD_CW					= 0;			// �ð����

// I/O ����
const	int	DEF_MAX_IO_PER_BOARD		= 32;			// Board�� I/O Bit ��
const	int	DEF_MIN_IO_BIT				= 0;
const	BOOL	DEF_IO_TYPE_IN			= FALSE;
const	BOOL	DEF_IO_TYPE_OUT			= TRUE;
const	int	DEF_MAX_IO_PORT				= 3;
const	int	DEF_MIN_IO_PORT				= 0;
const	int	DEF_MAX_ANALOG_CH			= 4;
const	int	DEF_MIN_ANALOG_CH			= 0;

// I/O Interrupt
const	int	DEF_BOARD_ENABLE_MODE		= 0;			// Board Interrup Enable Mode
const	int	DEF_STOP_EVENT_MODE			= 1;			// Stop Event Interrup Mode
const	int	DEF_ESTOP_EVENT_MODE		= 2;			// E-Stop Event Interrup Mode

// In-Command ����
const	int	DEF_INSEQUENCE				= 0;			// in sequence (�̵�)
const	int	DEF_INMOTION				= 1;			// in motion (�ӵ� �̵�)
const	int	DEF_INPOSITION				= 2;			// in position (��ġ)

// Wait Done ����
const	BOOL	DEF_MOTION_DONE			= FALSE;		// motion_done
const	BOOL	DEF_AXIS_DONE			= TRUE;			// axis_done

// Current/Command ����
const	BOOL	DEF_CURRENT_VAL			= FALSE;		// ���簪
const	BOOL	DEF_COMMAND_VAL			= TRUE;			// ��ɰ�

// ABS Motor ����
const	int	DEF_SAMSUNGCSDJ				= 1;			// �Ｚ CSDJ, CSDJ + SERVO DRIVE
const	int	DEF_YASKAWA_SERVO_DRIVE		= 2;			// YASKAWA SERVO DRIVE
const	int DEF_YASKAWA_S3_SERVO_DRIVE	= 22;			// YASKAWA SIGMA3 SERVO DRIVE

// Spline Data
const	int	DEF_SPLINE_MOTION_MAX_NO	= 20;
const	int	DEF_SPLINE_MOTION_MIN_NO	= 1;
const	int	DEF_SPLINE_MOVE_PATH_MAX_NO	= 500;
const	int	DEF_SPLINE_MOVE_PATH_MIN_NO	= 1;

// �������� Step
const	int	DEF_ORIGIN_START_STEP		= 0;			// START
const	int	DEF_ORIGIN_FIRST_SET_STEP	= 10;			// FIRST SETTING
const	int	DEF_ORIGIN_1ST_MOVE_STEP	= 20;			// MOVE 1st
const	int	DEF_ORIGIN_2ND_MOVE_STEP	= 30;			// MOVE 2nd
const	int	DEF_ORIGIN_3RD_MOVE_STEP	= 40;			// MOVE 3rd
const	int	DEF_ORIGIN_4TH_MOVE_STEP	= 50;			// MOVE 4th
const	int	DEF_ORIGIN_STOP_MOVE_STEP	= 60;			// STOP MOVE
const	int	DEF_ORIGIN_LAST_SET_STEP	= 70;			// LAST SETTING
const	int	DEF_ORIGIN_SET_ORIGIN_STEP	= 80;			// SET ORIGIN
const	int DEF_ORIGIN_RESET_STEP		= 100;			// Origin Reset �Լ��� �̿��ؼ� �۾������� ó�� ���� Step
const	int	DEF_ORIGIN_ERROR_STEP		= 999;			// ERROR
const	int	DEF_ORIGIN_FINISH_STEP		= 1000;			// FINISH

// ��Ÿ
const	BOOL	DEF_ENABLE				= TRUE;			// Enable
const	BOOL	DEF_DISABLE				= FALSE;		// Disable

const	BOOL	DEF_POS_COMMAND			= TRUE;			// Command Pos
const	BOOL	DEF_POS_ENCODER			= FALSE;		// Encoder Pos

typedef enum enumRoleOfHome
{
	HOME_N_NONE,
	HOME_N_POSITIVE_LIMIT,
	HOME_N_NEGATIVE_LIMIT
		
} ERoleOfHome;

/* ���� ���� �� Home Sensor �� C ��(Marker) ���� ��� Ÿ�� */
const int DEF_USE_SWITCH_ONLY			= 1;	// Home Sensor �� ���
const int DEF_USE_SWITCH_F_LIMIT		= 2;    // Home ��� Limit ��� .
const int DEF_USE_SWITCH_N_MARKER		= 0;	// Home Sensor ���� �� Marker ���
const int DEF_USE_MARKER_ONLY			= 0;	// Marker �� ���

/* Origin Ruturn �Լ� �������� Move Type */
const int DEF_ORIGIN_RETURN_NONE_MOVE					= 0;	// Motion ���� ���� ���� ��ġ�� �������� �����ϴ� ���.
const int DEF_ORIGIN_RETURN_ALL_MOVE					= 1;	// ���� ���� Offset �̵� ������ ������ ��� ���� ���� ���� Motion ������ �����ϴ� ���.
const int DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE	= 2;	// ���� ���� Offset �̵� ������ ������ ���� ���� ���� Motion ������ �����ϴ� ���.

#endif //DEFCOMMONMOTION_H

// End of File DefCommonMotion.h
