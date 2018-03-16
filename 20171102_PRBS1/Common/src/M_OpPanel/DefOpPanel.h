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

/** System 기동 시 초기 상태값 */
const int DEF_INITIAL_STATUS				= -1;

/** Touch Panel 앞/뒷면 ID define */
const int DEF_OPPANEL_NONE_PANEL_ID			= 0;

const int DEF_OPPANEL_FRONT_PANEL_ID		= 1;
const int DEF_OPPANEL_BACK_PANEL_ID			= 2;
const int DEF_OPPANEL_FRONT_PANEL_ID_PRESS	= 3;
const int DEF_OPPANEL_BACK_PANEL_ID_PRESS	= 4;

const int DEF_MAX_TOUCH_SELECT				= 4;	//@KKY.130906

/** Jog 관련 define */
const int DEF_OPPANEL_NO_JOGKEY				= -1;

const int DEF_OPPANEL_JOG_X_KEY				= 0;
const int DEF_OPPANEL_JOG_Y_KEY				= 1;
const int DEF_OPPANEL_JOG_T_KEY				= 2;
const int DEF_OPPANEL_JOG_Z_KEY				= 3;

/** Tower Lamp 관련 define */
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
 * 이 구조체는 tower lamp 특성 정보를 관리하는 구조체이다.
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
 * 이 구조체는 OpPanel의 앞/뒷면의 모든 IO Addrss 관련 특성 정보를 관리하는 구조체이다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Touch Panel 선택 IO Address */
//@KKY.130906.	unsigned short	usiTouchSelectAddr;	
	unsigned short	usiTouchSelectAddr[DEF_MAX_TOUCH_SELECT];	
	
	/** 앞 Panel의 IO Address Table 
	 * @link aggregationByValue
	 * @supplierCardinality 1*/
	SPanelIOAddr	FrontPanel;

	/** 뒷 Panel의 IO Address Table 
	 * @link aggregationByValue
	 * @supplierCardinality 1*/
	SPanelIOAddr	BackPanel;

	//@KKY.130906_________
	SPanelIOAddr	FrontPressPanel;
	SPanelIOAddr	BackPressPanel;
	//@___________________

	/** Tower Lamp의 IO Address Table */
	STowerIOAddr	TowerLamp;

	/** E-STOP Switch Status IO Address */
	unsigned short	usiEStopInputAddr[DEF_OPPANEL_MAX_ESTOP_RELAY_NO];

	/** 안전센서 (Door) IO Address */
	unsigned short	usiSafeDoorAddr[DEF_OPPANEL_MAX_DOOR_GROUP][DEF_OPPANEL_MAX_DOOR_SENSOR];

	/** 안전센서 (Door) 사용 여부 */
	BOOL			rgbSafeDoorFlag[DEF_OPPANEL_MAX_DOOR_GROUP][DEF_OPPANEL_MAX_DOOR_SENSOR];

	/* 110411.KKY.Add(자재 교체중인 Door인지 여부...)	*/
	BOOL			rgbMaterialChangeFlag[DEF_OPPANEL_MAX_DOOR_GROUP][DEF_OPPANEL_MAX_DOOR_SENSOR];

	/** Main Air Check IO Address */
	unsigned short	usiMainAirAddr[DEF_OPPANEL_MAX_MAIN_AIR_NO];

	/** Main Vacuum Check IO Address */
	unsigned short	usiMainVacuumAddr[DEF_OPPANEL_MAX_MAIN_VACUUM_NO];

} SOpPanelIOAddr;


typedef
/**
 * This structure is defined one configuration of jog moving table.
 * 이 구조체는 OpPanel의 Jog를 사용한 이동 정보 1개를 관리하는 구조체이다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Jog로 움직일 Motion 대상 */
	IAxis*				m_plnkJog;

	//ISercosMultiAxis*	m_plnkMultiJog;
	IRSNMCMultiAxis*	m_plnkMultiJog;		//161024 Modify...

	/** Jog Key로 움직일 Motion 축 0: 단축, 1: Multi 축 */
	int					m_iAxisIndex;

	/** Motion 종류 0:MMC, 1:Sercos, 2:ACS */
	int					m_iMotionType;
	
	/** Jog 방향 TRUE: 정방향, FALSE : 역방향*/
	BOOL				m_bDirMask;

} SJogMotion;


typedef
/**
 * This structure is defined one configuration of jog moving table.
 * 이 구조체는 OpPanel의 Jog를 사용한 이동 정보 1개를 관리하는 구조체이다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Left/Right Jog Key(X +/-)로 움직일 Motion 축 */
	SJogMotion		m_XKey;

	/** For/Back Jog Key(Y +/-)로 움직일 Motion 축 */
	SJogMotion		m_YKey;

	/** CW/CCW Jog Key(T +/-)로 움직일 Motion 축 */
	SJogMotion		m_TKey;

	/** Up/Down Jog Key(Z +/-)로 움직일 Motion 축 */
	SJogMotion		m_ZKey;

} SJogMotionTable;


typedef
/**
 * This structure is defined all configuration of jog moving table.
 * 이 구조체는 OpPanel의 Jog를 사용한 이동 정보 전체를 관리하는 구조체이다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @stereotype struct 
 */
struct {
	/** Jog로 움직일 대상의 개수 */
	int				m_iListNo;

	/** Jog로 움직일 Motion에 대한 정보 List */
	SJogMotionTable	m_Motion[DEF_OPPANEL_MAX_JOG_LIST];

} SJogTable;

#endif //DEFOPPANEL_H
// End of File DefOpPanel.h
