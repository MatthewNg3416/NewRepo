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
 * �� ����ü��Motor Ư�� ������ �����ϴ� ����ü�̴�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Motor Type (0:�ӵ��� Servo, 1:Stepper, 2:Micro Stepper or ��ġ�� Servo) - 2���� ���� */
	int		iMotorType;
	/** Loop Type (0:Open Loop, 1:Closed Loop) */
	BOOL	bLoopType;
	/** Feedback Device Type (0:Encoder, 1:Unipolar, 2:Bipolar) */
	int		iFeedbackType;
	/** �ӵ��� Servo ������ (FALSE:�ӵ�����, TRUE:��ũ����) */
	BOOL	bVServoControl;
	/** �ӵ��� Servo ��¸�� (TRUE:Uni-Polar, FALSE:Bi-Polar) */
	BOOL	bVServoPolar;
	/** Stepper Pulse ���ֺ� */
	int		iStepperPulseR;
	/** Stepper ���ڱ��� */
	double	dStepperEGear;
	/** Motor Pulse Type (0:Two-Pulse(CW+CCW), 1:Sign+Pulse) */
	BOOL	bPulseType;
	/** Encoder ���� () */
	BOOL	bEncoderDir;
	/** ��ǥ ���� () */
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
 * �� ����ü�� Motion Board ���� ������ �����ϴ� ����ü�̴�.
 * static type���� ��ü�� ����� ��ü�� ���� Data�� �����ϵ��� �Ѵ�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	// Motion Board - �ʱ� ����
    /** ������ Board ���� (0:�̱���, 1 ~ 8) */
	int		iMaxBoardNo;						// Initialize()���� ���
    /** Motion Board DPRAM Address */
	long	rglAddress[DEF_MAX_MOTION_BD];		// Initialize()���� ���
    /** �ڵ� ��,���� ���� ���� (TRUE:�ڵ�) */
	BOOL	rgbAutoCP[DEF_MAX_MOTION_BD];		// SetAutoCP()���� ���
	/** �������� ��� �ð� (�ʴ���) */
	//double	dOriginWaitTime;

	/** MMC-PC10 Option Board IndexSel */
	int		rgiPC10IndexSel[DEF_MAX_MOTION_BD];
	/** MMC-PC10 Option Board ��� �� */
	int		rgiPC10Axis[DEF_MAX_MOTION_BD][2];
} SMotionBoard;

typedef
/**
 * This structure is defined configuration of single-axis.
 * �� ����ü�� �� 1���� ���� Ư�� ������ �����ϴ� ����ü�̴�.
 * 
 * Motor�� ���� Ư�� ������ SMotionAxis���� �����Ѵ�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct
 */
struct {
    /** �� ID (-1:�̻��, 0 ~ 64) */
	int		iAxisID;
	/** �� �̸� (�ִ� 16����) */
//	char	szName[MAX_LENGTH_AXIS_NAME];
	WCHAR	szName[MAX_LENGTH_AXIS_NAME];
    /** �������� ���� �̵� �� �켱 ���� ���� (1 ~ 64, 0:�̻��) */
	int		iOriginPriority;
    /** �� �̵����� (TRUE:+, FALSE:-) */
	BOOL	bSign;
	/** �� �������� �������(Coarse �ӵ� ����) (TRUE:+, FALSE:-) */
	BOOL	bOriginDir;
	/** �� �������� �������(Fine �ӵ� ����) (TRUEL+, FALSE:-) */
	BOOL	bOriginFineDir;
    /** C�� ��� ���
	 *	DEF_USE_SWITCH_ONLY		(0) : Home Sensor �� ���
	 *	DEF_USE_SWITCH_N_MARKER (1) : Home Sensor ���� �� Marker ���
	 *	DEF_USE_MARKER_ONLY		(2) : Marker �� ���
	 */
	int		iCPhaseUseType;	
	/** Home Sensor �뵵 */
	ERoleOfHome	iRoleOfHome;
    /** �̵� ���� ���� Scale (default:1.0) */
    double	dScale;
    /** Abs Encoder ���� (TRUE:ABS, FALSE:Incremental) */
	BOOL	bAbsEncoder;
	
	/** Sercos Ring Number (0 ~ 9 , default : 0) */
	int			iRingNo;
	/** Sercos Node Number (1 ~ 24 , default : 0, Pulse Module ���� : Node ��ȣ * 100 + 1~24) */
	int			iNodeNo;
	/** Sercos Servo Driver ( Yaskawa :1 , RSA(CSDM): 2, PulseModule: 3, Etc:4 , default : 0) */
	int			iServoType;

} SAxis1;

#if FALSE

// 1222 : Component�� �ʿ��� ����Ÿ�� ������ ���Ͽ��� �о���� ���� �߰��� ����
typedef
/** ��� Component ����Ÿ : System���� �Ѿ���� ��� Component ����Ÿ
    MultiAxes�� 
*/
struct {

	/** �Ķ���� ���� Name */
	CString		m_strFileName;

	/** Motion Board Ÿ�� */
	int			m_iBoardType;

	/** �� ���� */
	int			m_iAxesNum;

	SAxisVal	m_sAxisVal[DEF_MAX_COORDINATE];

} SMotionData;

#endif

// 1222 : Component�� �ʿ��� ����Ÿ�� ������ ���Ͽ��� �о���� ���� �߰��� ����
typedef
/** ��� Component ����Ÿ : System���� �Ѿ���� ��� Component ����Ÿ
    Single Axis��
*/
struct {

	/** �Ķ���� ���� Name */
	CString		m_strFileName;

	/** Motion Board Ÿ�� */
	int			m_iBoardType;

	/** �� ���� */
//	int			m_iAxesNum;

	/** �� ������ */
	SAxis1		m_sAxisVal;

	int		m_iACSMotionID;			//@ ACS �࿡����...
	int		m_iACSMotionInstance;	//@

} SSMotionData;


typedef
/**
 * This structure is defined configuration of origin thread parameter.
 * �� ����ü�� �������� Thread�� ������ �μ��� ���� Data�� �����ϴ� ����ü�̴�.
 * 
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct
 */
struct {
	/** �������� �� �̵��� �� ����, TRUE=�̵� ���� */
	BOOL	bMoveOpt;
	/** ���������ϱ� �� Limit Sensor Event ���� �� */
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
 * �� ����ü�� OpPanel Ư�� ������ �����ϴ� ����ü�̴�.
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

	long lP_Gain; //��ʰ���
	long lI_Gain; //���а���
	long lD_Gain; //�̺�����
	long lF_Gain; //Feedforward����
	long lILimit_Gain; //�������� Limit
	long lGainNum; //������ ����

} SMotionAxisGain;

// ACS Motion
typedef
struct {
	/** Communication Handle */
	HANDLE Handle;
	/** Communication IP �ּ� */
	CString strIPAddr;
	/** Communication ������� */
	BOOL m_bConnect;

	int	m_iACSMotionID;

} SACSCommSet;

#endif // DEFMOTIONSTRUCT_H

// End of File DefMotionStruct.h
