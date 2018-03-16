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

/** ModelChange Y �� Index ���� */
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
 * �� ����ü�� ModelChange�� ���� ����� Component List�� �����ϴ� ����ü�̴�.
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
 * �� ����ü�� Mechanical Layer�� ��ȣ ������ ���� ModelChange�� ���ο� �����
 * Component List�� �����ϴ� ����ü�̴�.
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
 * �� ����ü�� ModelChange�� �����޾ƾ� �� Data �� Data �Ŵ����̴�.
 *
 * @stereotype struct 
 */
struct
{
	/** Lock/Unlock�� ���� IO Address */
	unsigned short	m_usOLockModelChangeRail;

	/** PanelAlignerXYT�� Teaching ����				*/
    STeachingInfo		m_sPanelAlignerXYTTeachingInfo;	
	/** PanelTransfer X�� Teaching ����				*/
	STeachingInfo		m_sPanelTransferXTeachingInfo;
	/** ModelChange Y�� Teaching ����				*/
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
