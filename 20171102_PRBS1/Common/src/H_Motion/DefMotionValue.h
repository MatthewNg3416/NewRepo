/* 
 * Motion Component
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
 * DefMotionValue.h : This file defines the value of Motion component.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */
#include "DefCommonMotion.h"

#ifndef DEFMOTIONVALUE_H
#define DEFMOTIONVALUE_H

#define	MAX_RING_NUM			8
#define	MAX_NODE_NUM			24
#define DEFAULT_PPM				10000000

//	Default Param
#define PTP_COMMAND_MODE					0x0310
#define	ORIGIN_MODE							0x0046
#define IN_C2D								0x00001000 // Servo On
#define IN_C1D								0x00002000 // AMP FAULT

// Board Value
//const	int	DEF_NON_BOARD_TYPE			= 0;			// Motion Board No-Use
//const	int	DEF_MMC_BOARD_TYPE			= 1;			// MMC Motion Board Use
//const	int	DEF_MEI_BOARD_TYPE			= 2;			// MEI Motion Board Use

//const	int	DEF_0AXIS_BOARD				= 0;			// 0축을 갖는 Motion Board
//const	int	DEF_4AXIS_BOARD				= 4;			// 4축을 갖는 Motion Board
//const	int	DEF_8AXIS_BOARD				= 8;			// 8축을 갖는 Motion Board

const	int	DEF_MAX_MOTION_BD			= 8;			// Motion Board Max. Number
const	int	DEF_NON_MOTION_BD			= 0;			// Motion Board No-Use Number
const	int	DEF_NON_MOTION_BD_ID		= -1;			// Motion Board Non-Config

const	int	DEF_ALL_MOTION_BD_ID		= -1;			// Motion Board ID ALL
const	int	DEF_MOTION_BD_ID1			= 0;			// Motion Board ID #1
const	int	DEF_MOTION_BD_ID2			= 1;			// Motion Board ID #2
const	int	DEF_MOTION_BD_ID3			= 2;			// Motion Board ID #3
const	int	DEF_MOTION_BD_ID4			= 3;			// Motion Board ID #4
const	int	DEF_MOTION_BD_ID5			= 4;			// Motion Board ID #5
const	int	DEF_MOTION_BD_ID6			= 5;			// Motion Board ID #6
const	int	DEF_MOTION_BD_ID7			= 6;			// Motion Board ID #7
const	int	DEF_MOTION_BD_ID8			= 7;			// Motion Board ID #8

const	BOOL	DEF_AUTO_CP_NON			= FALSE;		// 자동 가,감속 미사용
const	BOOL	DEF_AUTO_CP_USE			= TRUE;			// 자동 가,감속 사  용

// Board에 따른 축 수
const	int	DEF_AXIS_NO_PER_BRD		= 120;			// Motion Board가 Full-Size이면 8개 //SJ_YYK 161104 임의..

// 축 수
const	int	DEF_MAX_AXIS_NO				= DEF_AXIS_NO_PER_BRD * DEF_MAX_MOTION_BD;
const	int	DEF_MIN_AXIS_NO				= 0;

// Coordinate Value
const	int	DEF_MAX_COORDINATE			= DEF_MAX_AXIS_NO;	// Mulit Axes의 축 Max.

const	int	DEF_NO_COORDINATE			= 0;			// Multi Axes의 축 구성 안함
const	int	DEF_ALL_COORDINATE			= -1;			// Multi Axes의 모든 축 선택

const	int	DEF_AXIS_USE				= TRUE;			// 축 사  용
const	int	DEF_AXIS_NON				= FALSE;		// 축 미사용

const	int	DEF_AXIS_NON_NO				= 0;			// 축 0개
const	int	DEF_AXIS_NON_ID				= -1;			// 축 ID 미할당 (축 미구성)
const	int	DEF_AXIS_ALL_ID				= -1;			// 축 전체 선택
//const	int	DEF_AXIS_BLANK_ID			= -2;			// 빈 축 ID (축 구성)

// Move Priority Value ("1"이 우선순위 높음)
const	int	DEF_PRIORITY_NO				= DEF_MAX_AXIS_NO;	// Multi Axes의 다축 이동 시 우선순위 사용 가능 개수
const	int	DEF_PRIORITY_NONE			= -1;			// Multi Axes의 다축 이동 시 우선순위 1
const	int	DEF_PRIORITY_1				= 1;			// Multi Axes의 다축 이동 시 우선순위 1
const	int	DEF_PRIORITY_2				= 2;			// Multi Axes의 다축 이동 시 우선순위 2
const	int	DEF_PRIORITY_3				= 3;			// Multi Axes의 다축 이동 시 우선순위 3
const	int	DEF_PRIORITY_4				= 4;			// Multi Axes의 다축 이동 시 우선순위 4
const	int	DEF_PRIORITY_5				= 5;			// Multi Axes의 다축 이동 시 우선순위 5
const	int	DEF_PRIORITY_6				= 6;			// Multi Axes의 다축 이동 시 우선순위 6
const	int	DEF_PRIORITY_7				= 7;			// Multi Axes의 다축 이동 시 우선순위 7
const	int	DEF_PRIORITY_8				= 8;			// Multi Axes의 다축 이동 시 우선순위 8
const	int	DEF_PRIORITY_9				= 9;			// Multi Axes의 다축 이동 시 우선순위 9
const	int	DEF_PRIORITY_10				= 10;			// Multi Axes의 다축 이동 시 우선순위 10
const	int	DEF_PRIORITY_11				= 11;			// Multi Axes의 다축 이동 시 우선순위 11
const	int	DEF_PRIORITY_12				= 12;			// Multi Axes의 다축 이동 시 우선순위 12
const	int	DEF_PRIORITY_13				= 13;			// Multi Axes의 다축 이동 시 우선순위 13
const	int	DEF_PRIORITY_14				= 14;			// Multi Axes의 다축 이동 시 우선순위 14
const	int	DEF_PRIORITY_15				= 15;			// Multi Axes의 다축 이동 시 우선순위 15
const	int	DEF_PRIORITY_16				= 16;			// Multi Axes의 다축 이동 시 우선순위 16
const	int	DEF_PRIORITY_17				= 17;			// Multi Axes의 다축 이동 시 우선순위 17
const	int	DEF_PRIORITY_18				= 18;			// Multi Axes의 다축 이동 시 우선순위 18
const	int	DEF_PRIORITY_19				= 19;			// Multi Axes의 다축 이동 시 우선순위 19
const	int	DEF_PRIORITY_20				= 20;			// Multi Axes의 다축 이동 시 우선순위 20
const	int	DEF_PRIORITY_21				= 21;			// Multi Axes의 다축 이동 시 우선순위 21
const	int	DEF_PRIORITY_22				= 22;			// Multi Axes의 다축 이동 시 우선순위 22
const	int	DEF_PRIORITY_23				= 23;			// Multi Axes의 다축 이동 시 우선순위 23
const	int	DEF_PRIORITY_24				= 24;			// Multi Axes의 다축 이동 시 우선순위 24
const	int	DEF_PRIORITY_25				= 25;			// Multi Axes의 다축 이동 시 우선순위 25
const	int	DEF_PRIORITY_26				= 26;			// Multi Axes의 다축 이동 시 우선순위 26
const	int	DEF_PRIORITY_27				= 27;			// Multi Axes의 다축 이동 시 우선순위 27
const	int	DEF_PRIORITY_28				= 28;			// Multi Axes의 다축 이동 시 우선순위 28
const	int	DEF_PRIORITY_29				= 29;			// Multi Axes의 다축 이동 시 우선순위 29
const	int	DEF_PRIORITY_30				= 30;			// Multi Axes의 다축 이동 시 우선순위 30
const	int	DEF_PRIORITY_31				= 31;			// Multi Axes의 다축 이동 시 우선순위 31
const	int	DEF_PRIORITY_32				= 32;			// Multi Axes의 다축 이동 시 우선순위 32
const	int	DEF_PRIORITY_33				= 33;			// Multi Axes의 다축 이동 시 우선순위 33
const	int	DEF_PRIORITY_34				= 34;			// Multi Axes의 다축 이동 시 우선순위 34
const	int	DEF_PRIORITY_35				= 35;			// Multi Axes의 다축 이동 시 우선순위 35
const	int	DEF_PRIORITY_36				= 36;			// Multi Axes의 다축 이동 시 우선순위 36
const	int	DEF_PRIORITY_37				= 37;			// Multi Axes의 다축 이동 시 우선순위 37
const	int	DEF_PRIORITY_38				= 38;			// Multi Axes의 다축 이동 시 우선순위 38
const	int	DEF_PRIORITY_39				= 39;			// Multi Axes의 다축 이동 시 우선순위 39
const	int	DEF_PRIORITY_40				= 40;			// Multi Axes의 다축 이동 시 우선순위 40
const	int	DEF_PRIORITY_41				= 41;			// Multi Axes의 다축 이동 시 우선순위 41
const	int	DEF_PRIORITY_42				= 42;			// Multi Axes의 다축 이동 시 우선순위 42
const	int	DEF_PRIORITY_43				= 43;			// Multi Axes의 다축 이동 시 우선순위 43
const	int	DEF_PRIORITY_44				= 44;			// Multi Axes의 다축 이동 시 우선순위 44
const	int	DEF_PRIORITY_45				= 45;			// Multi Axes의 다축 이동 시 우선순위 45
const	int	DEF_PRIORITY_46				= 46;			// Multi Axes의 다축 이동 시 우선순위 46
const	int	DEF_PRIORITY_47				= 47;			// Multi Axes의 다축 이동 시 우선순위 47
const	int	DEF_PRIORITY_48				= 48;			// Multi Axes의 다축 이동 시 우선순위 48
const	int	DEF_PRIORITY_49				= 49;			// Multi Axes의 다축 이동 시 우선순위 49
const	int	DEF_PRIORITY_50				= 50;			// Multi Axes의 다축 이동 시 우선순위 50
const	int	DEF_PRIORITY_53				= 53;			// Multi Axes의 다축 이동 시 우선순위 53
const	int	DEF_PRIORITY_54				= 54;			// Multi Axes의 다축 이동 시 우선순위 54
const	int	DEF_PRIORITY_55				= 55;			// Multi Axes의 다축 이동 시 우선순위 55
const	int	DEF_PRIORITY_56				= 56;			// Multi Axes의 다축 이동 시 우선순위 56
const	int	DEF_PRIORITY_57				= 57;			// Multi Axes의 다축 이동 시 우선순위 57
const	int	DEF_PRIORITY_58				= 58;			// Multi Axes의 다축 이동 시 우선순위 58
const	int	DEF_PRIORITY_59				= 59;			// Multi Axes의 다축 이동 시 우선순위 59
const	int	DEF_PRIORITY_60				= 60;			// Multi Axes의 다축 이동 시 우선순위 60
const	int	DEF_PRIORITY_61				= 61;			// Multi Axes의 다축 이동 시 우선순위 61
const	int	DEF_PRIORITY_62				= 62;			// Multi Axes의 다축 이동 시 우선순위 62
const	int	DEF_PRIORITY_63				= 63;			// Multi Axes의 다축 이동 시 우선순위 63
const	int	DEF_PRIORITY_64				= 64;			// Multi Axes의 다축 이동 시 우선순위 64

const	int	DEF_1AXIS_MOVE				= 1;
const	int	DEF_2AXIS_MOVE				= 2;
const	int	DEF_3AXIS_MOVE				= 3;
const	int	DEF_4AXIS_MOVE				= 4;

const	int	DEF_MAX_GROUP_NO			= 8;
const	int	DEF_MIN_GROUP_NO			= 1;

// PC10 정의
const	int	DEF_PC_INDEXSEL_0			= 0;			// IndexSel 없음
const	int	DEF_PC_INDEXSEL_1			= 1;			// IndexSel 1축
const	int	DEF_PC_INDEXSEL_2			= 2;			// IndexSel 2축

const	int	DEF_PC_INDEX_MAX_NO			= 8;			// Position Compare를 실시할 Index 최대 번호
const	int	DEF_PC_INDEX_MIN_NO			= 1;

const	BOOL	DEF_PC_TRANSPARENT		= FALSE;		// Transparent Mode
const	BOOL	DEF_PC_LATCH			= TRUE;			// Latch Mode

const	int	DEF_PC_EQUAL				= 1;			// Equal
const	int	DEF_PC_GT					= 2;			// >
const	int	DEF_PC_LT					= 3;			// <

const	int	DEF_PC_OUT_NON				= 0;			// 축별 ON/OFF
const	int	DEF_PC_OUT_AND				= 1;			// 두 축 AND
const	int	DEF_PC_OUT_OR				= 2;			// 두 축 OR


// Digital Filter Defines
const	int	DEF_GAIN_NUMBER				= 5;			// elements expected get/set_filter(...)
const	int	DEF_GA_P					= 0;			// proportional gain
const	int	DEF_GA_I					= 1;			// integral gain
const	int	DEF_GA_D					= 2;			// derivative gain-damping term
const	int	DEF_GA_F       				= 3;			// velocity feed forward
const	int	DEF_GA_ILIMIT 				= 4;			// integration summing limit

// Sampling Rate
const	int	DEF_SAMPLING_4MSEC			= 1;			// Samspling Rate 4msec
const	int	DEF_SAMPLING_2MSEC			= 2;			// Samspling Rate 2msec
const	int	DEF_SAMPLING_1MSEC			= 3;			// Samspling Rate 1msec

// Analog Channel
const	int	DEF_ANALOG_IN_MAX_CH		= 4;			// Analog Input Channel 최대값
const	int	DEF_ANALOG_IN_NON_CH		= 0;			//

// Lmit Vlaue
const	int	DEF_ACCEL_LIMIT				= 25000;
const	int	DEF_VEL_LIMIT				= 5000000;
const	int	DEF_POS_SW_LIMIT			= 2147483640;
const	int	DEF_NEG_SW_LIMIT			= -2147483640;
const	int	DEF_ERROR_LIMIT				= 35000;
const	int	DEF_PULSE_RATIO				= 255;
const	double	DEF_DEGREE				= 1000.0;

// Interpolation 관련
const	int	DEF_START_MOVE				= 1;			// 동작 시작
const	int	DEF_MOVING					= 2;			// 동작 중
const	int	DEF_END_MOVE				= 3;			// 동작 완료

// Position I/O
const	int	DEF_POSITION_IO_MAX_NO		= 10;			//
const	int	DEF_POSITION_IO_MIN_NO		= 1;			//
const	int	DEF_POSITION_IO_NON_NO		= 0;			//

// Spline Data
const	int	DEF_POSITION_DATA_MAX_NO	= 30;
const	int	DEF_POSITION_DATA_MIN_NO	= 1;

// Event Source Status defines
const	int	DEF_ST_NONE					= 0x0000;
const	int	DEF_ST_HOME_SWITCH			= 0x0001;
const	int	DEF_ST_POS_LIMIT			= 0x0002;
const	int	DEF_ST_NEG_LIMIT   			= 0x0004;
const	int	DEF_ST_AMP_FAULT			= 0x0008;
const	int	DEF_ST_A_LIMIT    			= 0x0010;
const	int	DEF_ST_V_LIMIT  			= 0x0020;
const	int	DEF_ST_X_NEG_LIMIT 			= 0x0040;
const	int	DEF_ST_X_POS_LIMIT			= 0x0080;
const	int	DEF_ST_ERROR_LIMIT			= 0x0100;
const	int	DEF_ST_PC_COMMAND  			= 0x0200;
const	int	DEF_ST_OUT_OF_FRAMES    	= 0x0400;
const	int	DEF_ST_AMP_POWER_ONOFF  	= 0x0800;
const	int	DEF_ST_ABS_COMM_ERROR   	= 0x1000;
const	int	DEF_ST_INPOSITION_STATUS	= 0x2000;
const	int	DEF_ST_RUN_STOP_COMMAND 	= 0x4000;
const	int	DEF_ST_COLLISION_STATE  	= 0x8000;


const	int		ERR_MAX_ERROR_LEN			= 80;			// maximum length for error massage string
const	int		DEF_DEFAULT_ORIGIN_WAIT_TIME	= 60;		// 원점복귀 제한 시간 default 값
const	BOOL	DEF_COLLISION_SUB_POS	= FALSE;		// Collision Prevent 시 두 축 값의 차를 사용
const	BOOL	DEF_COLLISION_ADD_POS	= TRUE;			// Collision Prevent 시 두 축 값의 합을 사용
const	BOOL	DEF_COLLISION_LESSTHAN	= FALSE;		// Collision Prevent 시 기준값이 두 축 값다 작을 때 정지
const	BOOL	DEF_COLLISION_GREATTHAN	= TRUE;			// Collision Prevent 시 기준값이 두 축 값다 클 때 정지


/////////////////////////////////////////////////////////////////////////////////////////
// ACS Motion
/////////////////////////////////////////////////////////////////////////////////////////

const	int	DEF_ACS_MAX_MOTION_CONTROLLER		= 2;		// ACS Motion Controller Max. Number

// 축 수
const	int	DEF_ACS_MAX_AXIS_NO					= 0;
const	int	DEF_ACS_MIN_AXIS_NO					= 0;

const	int	DEF_ACS_AXIS_NON_NO					= -1;		// 축 0개

const	int	DEF_ACS_FAIL						= 0;		// Fail : 0, Success : non-zero

//@const int DEF_ACS_ORIGIN_RETURN_BUFFER			= 1;
const	int DEF_ACS_ORIGIN_RETURN_BUFFER[]		= {1, 2};
//const int DEF_ACS_GANTRY_GROUPING_BUFFER		= 2;
//const int DEF_ACS_GANTRY_UNGROUPING_BUFFER		= 3;

#endif //DEFMOTIONVALUE_H

// End of File DefMotionValue.h
