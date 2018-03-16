/* 
 * ModelChange Component
 *
 * Copyright 2005 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * DefModelChange.h : This file defines the value of ModelChange component.
 *
 */

#ifndef DEF_MODEL_CHANGE_H
#define DEF_MODEL_CHANGE_H

#include "MTeachingInfo.h"

class IAxis;
class ICylinder;
class IStatePanelAligner;
class IStatePanelTransfer;
class IStateCameraCarrier;
class MSystemData;
class MPanelData;
class IIO;

/** ModelChange Y 축 Index 정의 */
const	int	DEF_MODEL_CHANGE_Y_AXIS				= 0;

/** 
 * Position Define
 */
/** ModelChange Working Position Index Define */
const	int	DEF_MODEL_CHANGE_NONE_POS			= -1;	
const	int DEF_MODEL_CHANGE_MAX_POSITION		= 3;

const	int DEF_MODEL_CHANGE_INIT_POS			= 0;
const	int DEF_MODEL_CHANGE_MODEL_POS			= 1;
const	int DEF_MODEL_CHANGE_PANELALIGN_POS		= 2;

/* Error Define	*/
const	int ERR_MODEL_CHANGE_SUCCESS			= 0;

/**
 * ModelChange Structure Type Define
 */

/**
 * This structure is defined component list of ModelChange.
 * 이 구조체는 ModelChange의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	/** ModelChange Lock/Unlock I/O Object */
	IIO*		m_plnkIO;

	/** ModelChange Expand/Shrink Cylinder */
	ICylinder*	m_plnkESCyl;

	/** ModelChange UP/DOWN Cylinder */
	ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkAirRailFB1;
	ICylinder*	m_plnkAirRailFB2;

	/** ModelChange Y Motion Object */
	IAxis*		m_plnkY;

} SModelChangeRefCompList;

/**
 * ModelChange Structure Type Define
 */

typedef
/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 ModelChange의 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
struct
{
	IStatePanelAligner*		m_plnkStatePanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	IStateCameraCarrier*	m_plnkStateCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
} SStateCheckCompListForModelChange;

typedef
/**
 * 이 구조체는 ModelChange가 제공받아야 될 Data 및 Data 매니저이다.
 *
 * @stereotype struct 
 */
struct
{
	/** Lock/Unlock을 위한 IO Address */
	unsigned short	m_usOLockModelChangeRail;

	/** PanelAlignerXYT축 Teaching 정보				*/
    STeachingInfo		m_sPanelAlignerXYTTeachingInfo;	
	/** PanelTransfer X축 Teaching 정보				*/
	STeachingInfo		m_sPanelTransferXTeachingInfo;
	/** ModelChange Y축 Teaching 정보				*/
	STeachingInfo		m_sYTeachingInfo;
	/** Teaching Position File Name					*/
	CString				m_strPosFileName;
	/** Model Offset File Name						*/
	CString				m_strOffsetPosFileName;
	/** System Data									*/
	MSystemData*		m_plnkSystemData;
	/** Panel Data									*/
    MPanelData*			m_plnkPanelData;

} SModelChangeData;

#endif //DEF_MODEL_CHANGE_H
// End of File DefModelChange.h
