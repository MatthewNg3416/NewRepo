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
 * DefMotionStruct.h : This file defines the structure.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef DEFMOTIONSTRUCT_H
#define DEFMOTIONSTRUCT_H

#include "DefCommonMotion.h"
#include "DefMotionValue.h"


typedef
/**
 * This structure is defined configuration of one motor.
 * 이 구조체는Motor 특성 정보를 관리하는 구조체이다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Motor Type (0:속도형 Servo, 1:Stepper, 2:Micro Stepper or 위치형 Servo) - 2번만 지원 */
	int		iMotorType;
	/** Loop Type (0:Open Loop, 1:Closed Loop) */
	BOOL	bLoopType;
	/** Feedback Device Type (0:Encoder, 1:Unipolar, 2:Bipolar) */
	int		iFeedbackType;
	/** 속도형 Servo 제어모드 (FALSE:속도제어, TRUE:토크제어) */
	BOOL	bVServoControl;
	/** 속도형 Servo 출력모드 (TRUE:Uni-Polar, FALSE:Bi-Polar) */
	BOOL	bVServoPolar;
	/** Stepper Pulse 분주비 */
	int		iStepperPulseR;
	/** Stepper 전자기어비 */
	double	dStepperEGear;
	/** Motor Pulse Type (0:Two-Pulse(CW+CCW), 1:Sign+Pulse) */
	BOOL	bPulseType;
	/** Encoder 방향 () */
	BOOL	bEncoderDir;
	/** 좌표 방향 () */
	BOOL	bCoordinateDir;
	/** AMP Enable Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bAmpEnableLevel;
	/** AMP Reset Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bAmpResetLevel;
	/** AMP Fault Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bAmpFaultLevel;
	/** In-Position Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bInpositionLevel;
	/** Positive Sensor Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bPositiveLevel;
	/** Negative Sensor Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bNegativeLevel;
	/** Home Sensor Level (TRUE:HIGH, FALSE:LOW) */
	BOOL	bHomeLevel;
	/** AMP Fault Event */
	int		iAmpFaultEvent;
	/** Positive Sensor Event */
	int		iPositiveEvent;
	/** Negative Sensor Event */
	int		iNegativeEvent;
	/** Home Sensor Event */
	int		iHomeEvent;
	/** Positive SW Limit */
	double	dPositiveSWLimit;
	/** Negative SW Limit */
	double	dNegativeSWLimit;
	/** Positive SW Limit Event */
	int		iPositiveSWEvent;
	/** Negative SW Limit Event */
	int		iNegativeSWEvent;
	/** In-Position Error */
	double	dInpositionError;
} SMotionAxis;

typedef
/**
 * This structure is defined configuration of Motion boards.
 * You must use a object of this structure by static type,
 * because these are operated with same value.
 * 이 구조체는 Motion Board 구성 정보를 관리하는 구조체이다.
 * static type으로 개체를 만들어 개체간 동일 Data로 공유하도록 한다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	// Motion Board - 초기 구성
    /** 장착된 Board 개수 (0:미구성, 1 ~ 8) */
	int		iMaxBoardNo;						// Initialize()에서 기록
    /** Motion Board DPRAM Address */
	long	rglAddress[DEF_MAX_MOTION_BD];		// Initialize()에서 기록
    /** 자동 가,감속 설정 여부 (TRUE:자동) */
	BOOL	rgbAutoCP[DEF_MAX_MOTION_BD];		// SetAutoCP()에서 기록
	/** 원점복귀 대기 시간 (초단위) */
	//double	dOriginWaitTime;

	/** MMC-PC10 Option Board IndexSel */
	int		rgiPC10IndexSel[DEF_MAX_MOTION_BD];
	/** MMC-PC10 Option Board 사용 축 */
	int		rgiPC10Axis[DEF_MAX_MOTION_BD][2];
} SMotionBoard;

typedef
/**
 * This structure is defined configuration of single-axis.
 * 이 구조체는 축 1개에 대한 특성 정보를 관리하는 구조체이다.
 * 
 * Motor에 대한 특성 정보는 SMotionAxis에서 관리한다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct
 */
struct {
    /** 축 ID (-1:미사용, 0 ~ 64) */
	int		iAxisID;
	/** 축 이름 (최대 16문자) */
//	char	szName[MAX_LENGTH_AXIS_NAME];
	WCHAR	szName[MAX_LENGTH_AXIS_NAME];
    /** 원점복귀 동시 이동 시 우선 순위 지정 (1 ~ 64, 0:미사용) */
	int		iOriginPriority;
    /** 축 이동방향 (TRUE:+, FALSE:-) */
	BOOL	bSign;
	/** 축 원점복귀 진행방향(Coarse 속도 구간) (TRUE:+, FALSE:-) */
	BOOL	bOriginDir;
	/** 축 원점복귀 진행방향(Fine 속도 구간) (TRUEL+, FALSE:-) */
	BOOL	bOriginFineDir;
    /** C상 사용 방식
	 *	DEF_USE_SWITCH_ONLY		(0) : Home Sensor 만 사용
	 *	DEF_USE_SWITCH_N_MARKER (1) : Home Sensor 감지 후 Marker 사용
	 *	DEF_USE_MARKER_ONLY		(2) : Marker 만 사용
	 */
	int		iCPhaseUseType;	
	/** Home Sensor 용도 */
	ERoleOfHome	iRoleOfHome;
    /** 이동 값에 대한 Scale (default:1.0) */
    double	dScale;
    /** Abs Encoder 설정 (TRUE:ABS, FALSE:Incremental) */
	BOOL	bAbsEncoder;
	
	/** Sercos Ring Number (0 ~ 9 , default : 0) */
	int			iRingNo;
	/** Sercos Node Number (1 ~ 24 , default : 0, Pulse Module 사용시 : Node 번호 * 100 + 1~24) */
	int			iNodeNo;
	/** Sercos Servo Driver ( Yaskawa :1 , RSA(CSDM): 2, PulseModule: 3, Etc:4 , default : 0) */
	int			iServoType;

} SAxis1;

#if FALSE

// 1222 : Component가 필요한 데이타를 스스로 파일에서 읽어오기 위하 추가한 구조
typedef
/** 모션 Component 데이타 : System에서 넘어오는 모션 Component 데이타
    MultiAxes용 
*/
struct {

	/** 파라미터 파일 Name */
	CString		m_strFileName;

	/** Motion Board 타입 */
	int			m_iBoardType;

	/** 축 갯수 */
	int			m_iAxesNum;

	SAxisVal	m_sAxisVal[DEF_MAX_COORDINATE];

} SMotionData;

#endif

// 1222 : Component가 필요한 데이타를 스스로 파일에서 읽어오기 위하 추가한 구조
typedef
/** 모션 Component 데이타 : System에서 넘어오는 모션 Component 데이타
    Single Axis용
*/
struct {

	/** 파라미터 파일 Name */
	CString		m_strFileName;

	/** Motion Board 타입 */
	int			m_iBoardType;

	/** 축 갯수 */
//	int			m_iAxesNum;

	/** 축 설정값 */
	SAxis1		m_sAxisVal;

	int		m_iACSMotionID;			//@ ACS 축에서만...
	int		m_iACSMotionInstance;	//@

} SSMotionData;


typedef
/**
 * This structure is defined configuration of origin thread parameter.
 * 이 구조체는 원점복귀 Thread에 전달할 인수에 대한 Data를 관리하는 구조체이다.
 * 
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct
 */
struct {
	/** 원점복귀 시 이동할 지 여부, TRUE=이동 포함 */
	BOOL	bMoveOpt;
	/** 원점복귀하기 전 Limit Sensor Event 설정 값 */
	short	siPositiveLimit;		
	short	siNegativeLimit;		
	short	siHomeLimit;
	BOOL	bPositiveLevel;
	BOOL	bNegativeLevel;
	BOOL	bHomeLevel;
} SOriginThreadAx1;

typedef
/**
 * This structure is defined configuration of op-panel.
 * 이 구조체는 OpPanel 특성 정보를 관리하는 구조체이다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** RUN Switch Status IO Address */
	unsigned short	usiRunInputAddr;
	/** STOP Switch Status IO Address */
	unsigned short	usiStopInputAddr;
	/** RESET Switch Status IO Address */
	unsigned short	usiResetInputAddr;
	/** Teaching Pendant SStop Switch IO Address */
	unsigned short	usiTPStopInputAddr;

} SMaxesPanelIOAddr;

typedef struct{

	long lP_Gain; //비례게인
	long lI_Gain; //적분게인
	long lD_Gain; //미분제인
	long lF_Gain; //Feedforward게인
	long lILimit_Gain; //오차합의 Limit
	long lGainNum; //게인의 갯수

} SMotionAxisGain;

// ACS Motion
typedef
struct {
	/** Communication Handle */
	HANDLE Handle;
	/** Communication IP 주소 */
	CString strIPAddr;
	/** Communication 연결상태 */
	BOOL m_bConnect;

	int	m_iACSMotionID;

} SACSCommSet;

#endif // DEFMOTIONSTRUCT_H

// End of File DefMotionStruct.h
