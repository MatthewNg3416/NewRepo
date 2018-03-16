/* 
 * RollerConveyor Component
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
 * DefRollerConveyor.h : This file defines the value of RollerConveyor component.
 *
 */

#ifndef DEF_ROLLER_CONVEYOR_H
#define DEF_ROLLER_CONVEYOR_H

class IIO;
class ICylinder;
class IInverter;
class IStatePanelAligner;
class IStatePanelTransfer;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
class MPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
class MACFData;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
class MFinalData;
#endif

/**
 * This structure is defined component list of RollerConveyor.
 * 이 구조체는 RollerConveyor의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	IIO*		m_plnkIO;

	ICylinder*	m_plnkStopperUDCyl;

	IInverter*	m_plnkInverter;

	//20081110 by kss
	ICylinder*	m_plnkPanelPreAlignFBCyl;

} SRollerConveyorRefCompList;

/**
 * 이 구조체는 RollerConveyor의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	IStatePanelAligner*		m_plnkStatePanelAligner;
	IStatePanelTransfer*	m_plnkStatePanelTransfer;

} SStateCheckCompListForRollerConveyor;

/**
 * This structure is defined data of RollerConveyor.
 * 이 구조체는 RollerConveyor 특성 정보를 관리하는 구조체이다.
 *
 * @version $Revision$
 *
 * @stereotype struct 
 */
typedef struct
{
#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	/** PreBonder Data		*/
	MPreBonderData*		m_plnkPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	/** ACF Data			*/
	MACFData*			m_plnkACFData;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	/** Final Data			*/
	MFinalData*			m_plnkFinalData;
#endif

	/** Panel 감속 감지를 위한 IO Address */
	unsigned short	m_usIAddrPanelDecelDetect;

	/** Panel 정지 감지를 위한 IO Address */
	unsigned short	m_usIAddrPanelStopDetect;

	/** Roller Conveyor Index */
	int				m_iRollerConveyorIndex;

} SRollerConveyorData;

/** 
 *	Define
 */
const int ERR_ROLLERCONVEYOR_SUCCESS		= 0;

const int DEF_CONVEYOR_NORMAL_SPEED			= 0;
const int DEF_CONVEYOR_REDUCED_SPEED		= 1;
const int DEF_CONVEYOR_TRANSFER_SPEED		= 1;

const int DEF_CONVEYOR_MOVE_FORWARD			= 0;
const int DEF_CONVEYOR_MOVE_BACKWARD		= 1;
const int DEF_CONVEYOR_STOPPER_UP			= 2;
const int DEF_CONVEYOR_STOPPER_DOWN			= 3;

#endif //DEF_ROLLER_CONVEYOR_H

// End of File DefRollerConveyor.h
