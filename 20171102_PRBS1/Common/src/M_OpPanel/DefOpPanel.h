/* 
 * OpPanel Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * DefOpPanel.h : This file defines the structure.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef DEFOPPANEL_H
#define DEFOPPANEL_H

#include "IIO.h"
#include "IAxis.h"
#include "IRSNMCMultiAxis.h"

/**
 * Error Code Define
 */
const int ERR_OPPANEL_SUCCESS			= 0;

/**
 * Op Panel Value Define
 */

/** System �⵿ �� �ʱ� ���°� */
const int DEF_INITIAL_STATUS				= -1;

/** Touch Panel ��/�޸� ID define */
const int DEF_OPPANEL_NONE_PANEL_ID			= 0;

const int DEF_OPPANEL_FRONT_PANEL_ID		= 1;
const int DEF_OPPANEL_BACK_PANEL_ID			= 2;
const int DEF_OPPANEL_FRONT_PANEL_ID_PRESS	= 3;
const int DEF_OPPANEL_BACK_PANEL_ID_PRESS	= 4;

const int DEF_MAX_TOUCH_SELECT				= 4;	//@KKY.130906

/** Jog ���� define */
const int DEF_OPPANEL_NO_JOGKEY				= -1;

const int DEF_OPPANEL_JOG_X_KEY				= 0;
const int DEF_OPPANEL_JOG_Y_KEY				= 1;
const int DEF_OPPANEL_JOG_T_KEY				= 2;
const int DEF_OPPANEL_JOG_Z_KEY				= 3;

/** Tower Lamp ���� define */
const int DEF_OPPANEL_BUZZER_ALL			= -1;
const int DEF_OPPANEL_BUZZER_K1				= 0;
const int DEF_OPPANEL_BUZZER_K2				= 1;
const int DEF_OPPANEL_BUZZER_K3				= 2;
const int DEF_OPPANEL_BUZZER_K4				= 3;

/** Max. Value */
const int DEF_OPPANEL_MAX_MOTION_LIST		= 64;
//@const int DEF_OPPANEL_MAX_JOG_LIST			= 64;
const int DEF_OPPANEL_MAX_JOG_LIST			= 100;
const int DEF_OPPANEL_MAX_BUZZER_MODE		= 4;
const int DEF_OPPANEL_MAX_ESTOP_RELAY_NO	= 3;
const int DEF_OPPANEL_MAX_HEATER_ERR_NO		= 28;
const int DEF_OPPANEL_MAX_MAIN_AIR_NO		= 2;
const int DEF_OPPANEL_MAX_MAIN_VACUUM_NO	= 4;
const int DEF_OPPANEL_MAX_DOOR_SENSOR		= 16;
const int DEF_OPPANEL_MAX_DOOR_GROUP		= 2;


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

	/** Teaching Pendant Detected IO Address */
	unsigned short	usiTPDetectedInputAddr;

	/** RUN Switch LED IO Address */
	unsigned short	usiRunOutputAddr;

	/** STOP Switch LED IO Address */
	unsigned short	usiStopOutputAddr;

	/** RESET Switch LED IO Address */
	unsigned short	usiResetOutputAddr;

	/** Jog X(+) Switch Status IO Address */
	unsigned short	usiXpInputAddr;

	/** Jog X(-) Switch Status IO Address */
	unsigned short	usiXnInputAddr;

	/** Jog Y(+) Switch Status IO Address */
	unsigned short	usiYpInputAddr;

	/** Jog Y(-) Switch Status IO Address */
	unsigned short	usiYnInputAddr;

	/** Jog T(+) Switch Status IO Address */
	unsigned short	usiTpInputAddr;

	/** Jog T(-) Switch Status IO Address */
	unsigned short	usiTnInputAddr;

	/** Jog Z(+) Switch Status IO Address */
	unsigned short	usiZpInputAddr;

	/** Jog Z(-) Switch Status IO Address */
	unsigned short	usiZnInputAddr;
} SPanelIOAddr;


typedef
/**
 * This structure is defined configuration of tower lamp.
 * �� ����ü�� tower lamp Ư�� ������ �����ϴ� ����ü�̴�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Tower Red Lamp Operate IO Address */
	unsigned short	usiRedLampAddr;

	/** Tower Yellow Lamp Operate IO Address */
	unsigned short	usiYellowLampAddr;

	/** Tower Green Lamp Operate IO Address */
	unsigned short	usiGreenLampAddr;

	/** Buzzer Operate IO Address */
	unsigned short	usiBuzzerAddr[DEF_OPPANEL_MAX_BUZZER_MODE];
} STowerIOAddr;


typedef
/**
 * This structure is defined configuration of all op-panel.
 * �� ����ü�� OpPanel�� ��/�޸��� ��� IO Addrss ���� Ư�� ������ �����ϴ� ����ü�̴�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Touch Panel ���� IO Address */
//@KKY.130906.	unsigned short	usiTouchSelectAddr;	
	unsigned short	usiTouchSelectAddr[DEF_MAX_TOUCH_SELECT];	
	
	/** �� Panel�� IO Address Table 
	 * @link aggregationByValue
	 * @supplierCardinality 1*/
	SPanelIOAddr	FrontPanel;

	/** �� Panel�� IO Address Table 
	 * @link aggregationByValue
	 * @supplierCardinality 1*/
	SPanelIOAddr	BackPanel;

	//@KKY.130906_________
	SPanelIOAddr	FrontPressPanel;
	SPanelIOAddr	BackPressPanel;
	//@___________________

	/** Tower Lamp�� IO Address Table */
	STowerIOAddr	TowerLamp;

	/** E-STOP Switch Status IO Address */
	unsigned short	usiEStopInputAddr[DEF_OPPANEL_MAX_ESTOP_RELAY_NO];

	/** �������� (Door) IO Address */
	unsigned short	usiSafeDoorAddr[DEF_OPPANEL_MAX_DOOR_GROUP][DEF_OPPANEL_MAX_DOOR_SENSOR];

	/** �������� (Door) ��� ���� */
	BOOL			rgbSafeDoorFlag[DEF_OPPANEL_MAX_DOOR_GROUP][DEF_OPPANEL_MAX_DOOR_SENSOR];

	/* 110411.KKY.Add(���� ��ü���� Door���� ����...)	*/
	BOOL			rgbMaterialChangeFlag[DEF_OPPANEL_MAX_DOOR_GROUP][DEF_OPPANEL_MAX_DOOR_SENSOR];

	/** Main Air Check IO Address */
	unsigned short	usiMainAirAddr[DEF_OPPANEL_MAX_MAIN_AIR_NO];

	/** Main Vacuum Check IO Address */
	unsigned short	usiMainVacuumAddr[DEF_OPPANEL_MAX_MAIN_VACUUM_NO];

} SOpPanelIOAddr;


typedef
/**
 * This structure is defined one configuration of jog moving table.
 * �� ����ü�� OpPanel�� Jog�� ����� �̵� ���� 1���� �����ϴ� ����ü�̴�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Jog�� ������ Motion ��� */
	IAxis*				m_plnkJog;

	//ISercosMultiAxis*	m_plnkMultiJog;
	IRSNMCMultiAxis*	m_plnkMultiJog;		//161024 Modify...

	/** Jog Key�� ������ Motion �� 0: ����, 1: Multi �� */
	int					m_iAxisIndex;

	/** Motion ���� 0:MMC, 1:Sercos, 2:ACS */
	int					m_iMotionType;
	
	/** Jog ���� TRUE: ������, FALSE : ������*/
	BOOL				m_bDirMask;

} SJogMotion;


typedef
/**
 * This structure is defined one configuration of jog moving table.
 * �� ����ü�� OpPanel�� Jog�� ����� �̵� ���� 1���� �����ϴ� ����ü�̴�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Left/Right Jog Key(X +/-)�� ������ Motion �� */
	SJogMotion		m_XKey;

	/** For/Back Jog Key(Y +/-)�� ������ Motion �� */
	SJogMotion		m_YKey;

	/** CW/CCW Jog Key(T +/-)�� ������ Motion �� */
	SJogMotion		m_TKey;

	/** Up/Down Jog Key(Z +/-)�� ������ Motion �� */
	SJogMotion		m_ZKey;

} SJogMotionTable;


typedef
/**
 * This structure is defined all configuration of jog moving table.
 * �� ����ü�� OpPanel�� Jog�� ����� �̵� ���� ��ü�� �����ϴ� ����ü�̴�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Jog�� ������ ����� ���� */
	int				m_iListNo;

	/** Jog�� ������ Motion�� ���� ���� List */
	SJogMotionTable	m_Motion[DEF_OPPANEL_MAX_JOG_LIST];

} SJogTable;

#endif //DEFOPPANEL_H
// End of File DefOpPanel.h
