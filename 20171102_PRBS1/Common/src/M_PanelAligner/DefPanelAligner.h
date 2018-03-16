/* 
 * PanelAligner Component
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
 * DefPanelAligner.h : This file defines the value of PanelAligner component.
 *
 */

#ifndef DEF_PANEL_ALIGNER_H
#define DEF_PANEL_ALIGNER_H

#include "MTeachingInfo.h"

class IAxis;
class ISercosMultiAxis;
class IRSNMCMultiAxis;		//161024 Add...
class ICylinder;
class IVacuum;
class IIO;
class MSystemData;
class MPanelData;
class IStateModelChange;
class IStatePanelTransfer;
class MPreBonderData;
class MTabData;
class MTrsAutoManager;
class IStateCameraCarrier;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
class IStateTabMounter;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
class IStateCarrier;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
class IStateMainTool;
#endif

/* PanelAligner X, Y, T, Z 축 Index 정의 */
const	int	DEF_PANEL_ALIGNER_X_AXIS					= 0;
const	int	DEF_PANEL_ALIGNER_Y_AXIS					= 1;
const	int	DEF_PANEL_ALIGNER_T_AXIS					= 2;
const	int	DEF_PANEL_ALIGNER_Z_AXIS					= 3;

/* PanelAligner XYTZ 축 최대 축 수 */
const	int	DEF_PANEL_ALIGNER_AXIS_MAX_NO				= 4;

const	double DEF_PANEL_ALIGNER_TURN_POS_T_TOLERANCE	= 5.0;
const	double DEF_PANEL_ALIGNER_ANGLE_LIMIT_FOR_SLOW_TURN	= 5.0;

/** 
 * Position Define
 */
/** PanelAligner Working Position Index Define */
const	int	DEF_PANEL_ALIGNER_NONE_POS					= -1;
/*/
const	int DEF_PANEL_ALIGNER_MAX_POSITION				= 15;

const	int DEF_PANEL_ALIGNER_LOAD_POS					= 0;
const	int DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS			= 1;
const	int DEF_PANEL_ALIGNER_FIDU_MARK_YL_POS			= 2;
const	int DEF_PANEL_ALIGNER_FIDU_MARK_YR_POS			= 3;
const	int DEF_PANEL_ALIGNER_SUB_MARK_XU_POS			= 4;
const	int DEF_PANEL_ALIGNER_SUB_MARK_YL_POS			= 5;
const	int DEF_PANEL_ALIGNER_SUB_MARK_YR_POS			= 6;
const	int DEF_PANEL_ALIGNER_PRESS_XU_POS				= 7;	//필요없음
const	int DEF_PANEL_ALIGNER_PRESS_YL_POS				= 8;	//필요없음
const	int DEF_PANEL_ALIGNER_PRESS_YR_POS				= 9;	//필요없음
const	int DEF_PANEL_ALIGNER_INSPECT_XU_POS			= 10;
const	int DEF_PANEL_ALIGNER_INSPECT_YL_POS			= 11;
const	int DEF_PANEL_ALIGNER_INSPECT_YR_POS			= 12;
const	int DEF_PANEL_ALIGNER_UNLOAD_POS				= 13;
const	int DEF_PANEL_ALIGNER_TURN_POS					= 14;
/*/
const	int DEF_PANEL_ALIGNER_MAX_POSITION				= 9;

const	int DEF_PANEL_ALIGNER_LOAD_POS					= 0;
const	int DEF_PANEL_ALIGNER_FIRST_MARK_POS			= 1;
const	int DEF_PANEL_ALIGNER_SECOND_MARK_POS			= 2;
const	int DEF_PANEL_ALIGNER_PRESS_1_POS				= 3;
const	int DEF_PANEL_ALIGNER_PRESS_2_POS				= 4;
const	int DEF_PANEL_ALIGNER_INSPECT_1_POS				= 5;
const	int DEF_PANEL_ALIGNER_INSPECT_2_POS				= 6;
const	int DEF_PANEL_ALIGNER_INSPECT_3_POS				= 7;
const	int DEF_PANEL_ALIGNER_UNLOAD_POS				= 8;


// PanelAligner Z Position Index Define
const	int	DEF_PANEL_ALIGNER_Z_NONE_POS				= -1;	
const	int DEF_PANEL_ALIGNER_Z_MAX_POSITION			= 3;

const	int DEF_PANEL_ALIGNER_Z_DOWN_POS				= 0;	// Down Pos	
const	int DEF_PANEL_ALIGNER_Z_MOVE_UP_POS				= 1;	// 이동 가능한 위치 (부상유닛에서 0.5 ~ 1mm 상승)
const	int DEF_PANEL_ALIGNER_Z_WORK_POS				= 2;	// Press 할때의 위치


/** BackupMove X Working Position Index Define */
const	int	DEF_BACKUP_MOVE_X_NONE_POS				= -1;	
const	int DEF_BACKUP_MOVE_X_MAX_POSITION			= 2;

const	int DEF_BACKUP_MOVE_X_READY_POS				= 0;
const	int DEF_BACKUP_MOVE_X_WORK_POS				= 1;

/* Error Define	*/
const	int ERR_PANEL_ALIGNER_SUCCESS					= 0;

/**
 * PanelAligner Structure Type Define
 */

/**
 * This structure is defined component list of PanelAligner.
 * 이 구조체는 PanelAligner의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	/** IO Object */
	IIO*		m_plnkIO;

	/** PanelAligner Vacuum Object */
	IVacuum*	m_plnkVac1;
	IVacuum*	m_plnkVac2;
	IVacuum*	m_plnkVac3;
//@	IVacuum*	m_plnkVacRotator;
//@	ICylinder*	m_plnkCylUDRotator;
//@	ICylinder*	m_plnkCylTRRotator;
//110102 HJG 추가
#ifdef DEF_USE_PANEL_PUSHER
	ICylinder*	m_plnkPanelPusherUD;
#endif

	/** PanelAligner X, Y, T, Z Motion Object */
	IAxis*	m_plnkX;
	IAxis*	m_plnkY;
	IAxis*	m_plnkT;
	IAxis*	m_plnkZ;

	/** Backup Move X Motion Object */
//@	IAxis*	m_plnkBackupMoveX;


	/** PanelAligner Multi Y Motion Object */
	//161024 Modify...
	//ISercosMultiAxis*	m_plnkMultiYT;
	IRSNMCMultiAxis* m_plnkMultiYT;

} SPanelAlignerRefCompList;

/**
 * PanelAligner Structure Type Define
 */

typedef
/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 PanelAligner의 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
struct
{
	/** Panel Transfer */
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	/** Tab Mounter */
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateCameraCarrier*	m_plnkStateRInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	// ARP
	/** Carrier1 */
	IStateCarrier*	m_plnkStateCarrier1;
	/** Carrier2 */
	IStateCarrier*	m_plnkStateCarrier2;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	IStateMainTool*			m_plnkStateMainTool1;
	IStateMainTool*			m_plnkStateMainTool2;
	// Final
#endif

	IStateModelChange*		m_plnkStateModelChange1;
	IStateModelChange*		m_plnkStateModelChange2;

} SStateCheckCompListForPanelAligner;

typedef
/**
 * 이 구조체는 PanelAligner가 제공받아야 될 Data 및 Data 매니저이다.
 *
 * @stereotype struct 
 */
struct
{
	/** XYT축 Teaching 정보							*/
    STeachingInfo		m_sXYTZTeachingInfo;	
	/** Z축 Teaching 정보							*/
	STeachingInfo		m_sZTeachingInfo;
	/** Backup Move X 축 Position 정보 */
//@	STeachingInfo		m_sBackupMoveXTeachingInfo;

	/** Teaching Position File Name					*/
	CString				m_strPosFileName;
	/** Model Offset File Name						*/
	CString				m_strOffsetPosFileName;
	/** System Data									*/
	MSystemData*		m_plnkSystemData;
	/** Panel Data									*/
    MPanelData*			m_plnkPanelData;

	MPreBonderData*		m_plnkPreBonderData;

	MTabData*			m_plnkTabData;

	// Panel Detect
	unsigned short		m_usIPanelDetect;

	/** Air Levitation Air Blow Output IO Address	*/
	unsigned short		m_usOAddrAirBlow1;
	unsigned short		m_usOAddrAirBlow2;
//@	unsigned short		m_usOAddrAirBlow3;
//@	unsigned short		m_usOAddrAirBlow4;

	/** Air Levitation Air Blow Input IO Address	*/
	unsigned short		m_usIAddrAirBlow1;
	unsigned short		m_usIAddrAirBlow2;
//@	unsigned short		m_usIAddrAirBlow3;
//@	unsigned short		m_usIAddrAirBlow4;

	/** Air Levitation Air Vacuum Output IO Address	*/
	unsigned short		m_usOAddrAirVacuum1;
	unsigned short		m_usOAddrAirVacuum2;
	unsigned short		m_usOAddrAirVacuum3;
	unsigned short		m_usOAddrAirVacuum4;

	/** Air Levitation Air Vacuum Input IO Address	*/
	unsigned short		m_usIAddrAirVacuum1;
	unsigned short		m_usIAddrAirVacuum2;
	unsigned short		m_usIAddrAirVacuum3;
	unsigned short		m_usIAddrAirVacuum4;


//120224.KMS____________
	unsigned short		m_usIPanelDetectLoading;//@Gate에서만 사용
	unsigned short		m_usIPanelDetectWorking;

	//120508.KMS_________
	unsigned short		m_usIIonizerState;

	
	//170724_KDH Spot Cleaner 추가
	unsigned short		m_usOAddrSpotCleanerOn;
	unsigned short		m_usIAddrSpotCleanerState;
	//_________________________
} SPanelAlignerData;


typedef struct
{
	MTrsAutoManager*	m_plnkTrsAutoManager;

} SPanelAlignerRefProcess;

#endif //DEF_PANEL_ALIGNER_H
// End of File DefPanelAligner.h
