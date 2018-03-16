/* 
 * PanelTransfer Component
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
 * DefPanelTransfer.h : This file defines the value of PanelTransfer component.
 *
 */

#ifndef DEF_PANEL_TRANSFER_H
#define DEF_PANEL_TRANSFER_H

#include "MTeachingInfo.h"

class IAxis;
class ISercosMultiAxis;
class IRSNMCMultiAxis;
class ICylinder;
class IVacuum;
class IIO;
class MSystemData;
class MPanelData;
class IStateModelChange;
class IStatePanelAligner;
//class IStateRollerConveyor;
class IStateCarrier;
class IStatePanelTransfer;
class IStateCameraCarrier;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
class IStateTabMounter;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	// ARP
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	// Final
class IStateMainTool;
#endif


/** PanelTransfer X 축 Index 정의 */
const	int	DEF_PANEL_TRASFER_X1_AXIS			= 0;
const	int	DEF_PANEL_TRASFER_X2_AXIS			= 1;
//const	int	DEF_PANEL_TRASFER_MULTI_X_AXIS		= 2;

/** 
 * Position Define
 */
/** PanelTransfer Working Position Index Define */
const	int	DEF_PANEL_TRANSFER_NONE_POS			= -1;	
//const	int DEF_PANEL_TRANSFER_MAX_POSITION		= 2;
const	int DEF_PANEL_TRANSFER_MAX_POSITION		= 3; //170201 SJ_YSH

const	int DEF_PANEL_TRANSFER_LOAD_POS			= 0;
const	int DEF_PANEL_TRANSFER_UNLOAD_POS		= 1;
const	int DEF_PANEL_TRANSFER_WAIT_POS			= 2; //170201 SJ_YSH

/* Error Define	*/
const	int ERR_PANEL_TRANSFER_SUCCESS			= 0;

/**
 * PanelTransfer Structure Type Define
 */

/**
 * This structure is defined component list of PanelTransfer.
 * 이 구조체는 PanelTransfer의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	/** IO Object */
	IIO*		m_plnkIO;

	/** PanelTransfer X Motion Object */
	IAxis*	m_plnkX1;
	IAxis*	m_plnkX2;

	/** PanelTransfer X MultiMotion Object */
	//ISercosMultiAxis*	m_plnkMultiX;
	IRSNMCMultiAxis* m_plnkMultiX;

	/** PanelTransfer Vacuum */
	IVacuum*	m_plnkVac;
	IVacuum*	m_plnkVac2;

	/** PanelTransfer Up/Down Cylinder */
	ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkAirRailFB1;
	ICylinder*	m_plnkAirRailFB2;

} SPanelTransferRefCompList;

/**
 * PanelTransfer Structure Type Define
 */

typedef
/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 PanelTransfer의 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
struct
{
	/** Panel Aligner */
	IStatePanelAligner*		m_plnkStatePanelAligner;
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateCameraCarrier*	m_plnkStateRInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	IStatePanelTransfer*	m_plnkStatePanelTransferIn;
#endif

} SStateCheckCompListForPanelTransfer;

typedef
/**
 * 이 구조체는 PanelTransfer가 제공받아야 될 Data 및 Data 매니저이다.
 *
 * @stereotype struct 
 */
struct
{
	/** XYT축 Teaching 정보							*/
    STeachingInfo		m_sXYTTeachingInfo;		// Inspection 1 : 사용 안함.
	/** PanelTransfer X축 Teaching 정보			*/
	STeachingInfo		m_sXTeachingInfo;
	/** Teaching Position File Name					*/
	CString				m_strPosFileName;
	/** Model Offset File Name						*/
	CString				m_strOffsetPosFileName;
	/** System Data									*/
	MSystemData*		m_plnkSystemData;
	/** Panel Data									*/
    MPanelData*			m_plnkPanelData;

	// Transfer Type
	UINT				m_uiTransferType;

	/** Air Levitation Air Blow Output IO Address	*/
//@	unsigned short		m_usOAddrAirBlow1;
//@	unsigned short		m_usOAddrAirBlow2;

	/** Air Levitation Air Blow Input IO Address	*/
//@	unsigned short		m_usIAddrAirBlow1;
//@	unsigned short		m_usIAddrAirBlow2;

	/** Air Levitation Air Vacuum Output IO Address	*/
//@	unsigned short		m_usOAddrAirVacuum1;
//@	unsigned short		m_usOAddrAirVacuum2;

	/** Air Levitation Air Vacuum Input IO Address	*/
//@	unsigned short		m_usIAddrAirVacuum1;
//@	unsigned short		m_usIAddrAirVacuum2;

	// Panel Detect
	unsigned short		m_usIPanelDetect;

	//120224.KMS____________
	unsigned short		m_usIPanelDetectLoading;
	unsigned short		m_usIPanelDetectUnLoading;

} SPanelTransferData;

#endif //DEF_PANEL_TRANSFER_H
// End of File DefPanelTransfer.h
