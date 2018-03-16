/* 
 * TabMounter Component
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
 * DefTabMounter.h : This file defines the value of TabMounter component.
 *
 */

#ifndef DEF_TABMOUNTER_H
#define DEF_TABMOUNTER_H

#include "MTeachingInfo.h"

class ICylinder;
class IVacuum;
class IAxis;
class MSystemData;
class MPanelData;
class MPreBonderData;
class MProcessData;
class MWorkSchedule;
class MTabOffsetData;
class IStateTabMounter;
class IStateTabAligner;
class IStateCameraCarrier;
class IStatePanelAligner;
class IStatePanelTransfer;
class IStateTabCarrier;

/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/

/**
 * Referenced Component List by TabMounter Component
 */
typedef struct tagSTabMounterRefCompList
{
    IAxis*		m_plnkAxis;			// Mounter Axis Object
	IAxis*		m_plnkAxisZ;
	IAxis*		m_plnkGantryAxis;	// Gantry Axis Object

    ICylinder*	m_plnk1stUDCyl;
	ICylinder*	m_plnkPusherUDCyl;
	IVacuum*	m_plnkVac;		// Vacuum Line Object

} STabMounterRefCompList;

/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	IStateCameraCarrier*	m_plnkCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateTabMounter*		m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP];
	IStatePanelAligner*		m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkPanelTransfer;
//@	IStateTabCarrier*		m_plnkTabCarrier;
//@	IStateTabCarrier*		m_plnkTabCarrierOther;
#endif
	IStateTabCarrier*		m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	IStatePanelTransfer*	m_plnkPanelTransferOut;

} SStateCheckCompListForTabMounter;

typedef
/**
 * TabMounter Component Data	
 */ 
struct STabMounterData
{
	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData* m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkScheduleSecond;

	MTabOffsetData*	m_plnkTabOffsetData;

	//120208.kms______
	MTabOffsetData* m_plnkRearTabOffsetData;

	/** PositionInfo.inf File 정보 구조체 */
	STeachingInfo	m_sTeachingInfoMounter;
	STeachingInfo	m_sTeachingInfoMounterZ;
	STeachingInfo	m_sTeachingInfoGantry;
	
	/** Offset Data가 저장될 File Name */
	CString			m_strOffsetPosFileName;
	
	/** Parameter Data 가 저장될 File Name */
	CString			m_strPosFileName;

	/** TabMounter 가 속한 Group 번호 */
	int				m_iGroupNo;

	/** 작업변 정보 */
	EWorkingSide	m_eWorkSide;

	/** **************************************************************************
	 *	이전/다음 TabMounter 번호 설정
	 *  ⇒ 이전/다음 구분 기준
	 *		- 자신보다 먼저 Prealign 위치로 이동하는 TabMounter 가 이전 TabMounter 임.
	 *		- Gate 의 경우 TabCarrier Unload 위치에 따라 Front/Rear Group 의 TabMounter 출발 위치가 달라짐.
	 */

	/** 이전 TabMounter 번호 : 1번 TabMounter 의 [이전 TabMounter 번호]는 -1 이다.
	 *	: TabMounter 가 Mount 위치로 이동하기 전에, Previous TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iPreTabMounterNo;

	/** 다음 TabMounter 번호
	 *	: 각 Group 의 마지막 TabMounter 의 [다음 TabMounter 번호]는 DEF_LAST_PER_GROUP 이다.
	 *	: TabMounter 가 Load 위치로 이동하기 전에, Next TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iNextTabMounterNo;

} STabMounterData;

/************************************************************************/
/*                           Position  Define                           */
/************************************************************************/
//@#ifdef DEF_SOURCE_SYSTEM
//@const int DEF_TABMOUNTER_AXIS_MAX_NO			= 2;
//@#else
const int DEF_TABMOUNTER_AXIS_MAX_NO			= 3;
//@#endif

const int DEF_TABMOUNTER_AXIS					= 0;
const int DEF_TABMOUNTER_AXIS_Z					= 1;
const int DEF_TABMOUNTER_GANTRY_AXIS			= 2;


//@#ifdef DEF_SOURCE_SYSTEM
//@const int DEF_TABMOUNTER_AXIS_TYPE				= 2;
//@#else
const int DEF_TABMOUNTER_AXIS_TYPE				= 3;
//@#endif

const int DEF_MOUNT_Y							= 0;
const int DEF_MOUNT_Z							= 1;
const int DEF_GANTRY							= 2;



/*
#ifdef DEF_SOURCE_SYSTEM
const int DEF_TABMOUNTER_MAX_POS				= 7;
#endif
#ifdef DEF_GATE_SYSTEM
const int DEF_TABMOUNTER_MAX_POS				= 8;
#endif

 const int DEF_TABMOUNTER_READY_POS				= 0;	// Gate : Tab PreAlign 시 나머지 TabMounter 대기 위치
const int DEF_TABMOUNTER_LOAD_POS				= 1;
const int DEF_TABMOUNTER_ALIGN1_POS				= 2;
const int DEF_TABMOUNTER_ALIGN2_POS				= 3;
const int DEF_TABMOUNTER_INSPECTION_POS			= 4;
const int DEF_TABMOUNTER_MOUNT_POS				= 5;
const int DEF_TABMOUNTER_REMOVE1_POS			= 6;
const int DEF_TABMOUNTER_REMOVE2_POS			= 7;
*/

const int DEF_TABMOUNTER_MAX_POS				= 6;
const int DEF_TABMOUNTER_NONE_POS				= -1;
														//고정 좌표 지정 Tool
const int DEF_TABMOUNTER_READY_POS				= 0;	//Loading위치와 동일													//1~4번 1번 Tool로 지정, 5~8번 8번 Tool로 지정
const int DEF_TABMOUNTER_LOAD_POS				= 1;	//1~4번은 1번Tool로 지정, 5~8번은 8번Tool로 지정						//1~8번 1번 Tool로 지정
const int DEF_TABMOUNTER_R_LOAD_POS				= 2;	//1~4번은 8번Tool Load위치로 지정, 5~8번은 1번Tool Load위치로 지정		//1~8번 8번 Tool로 지정
const int DEF_TABMOUNTER_ESCAPE_POS				= 2;	//Gate에서만. Loading동작후 Next Head가 TabLoading하기위해 피해주는 위치
const int DEF_TABMOUNTER_INSPECTION_POS			= 3;
const int DEF_TABMOUNTER_MOUNT_POS				= 4;
const int DEF_TABMOUNTER_REMOVE_POS				= 5;


const int DEF_TABMOUNTER_Z_MAX_POS				= 7; //150416 SJ_YSH 6 => 7
const int DEF_TABMOUNTER_Z_NONE_POS				= -1;

const int DEF_TABMOUNTER_Z_MOVE_POS				= 0;
const int DEF_TABMOUNTER_Z_LOAD_POS				= 1;
const int DEF_TABMOUNTER_Z_R_LOAD_POS			= 2;
const int DEF_TABMOUNTER_Z_INSPECTION_POS		= 3;
const int DEF_TABMOUNTER_Z_MOUNT_POS			= 4;
const int DEF_TABMOUNTER_Z_REMOVE_POS			= 5;
const int DEF_TABMOUNTER_Z_MOUNT_CHECK_POS		= 6; //150416 SJ_YSH


//const int DEF_TABMOUNTER_GANTRY_MAX_POS			= 6;
const int DEF_TABMOUNTER_GANTRY_MAX_POS			= 4;

//const int DEF_TABMOUNTER_GANTRY_LOAD1_POS		= 0;	//TabCarrier1번 Loading위치
//const int DEF_TABMOUNTER_GANTRY_LOAD2_POS		= 1;	//TabCarrier2번 Loading위치
//const int DEF_TABMOUNTER_GANTRY_LOAD3_POS		= 2;	//TabCarrier3번 Loading위치
//const int DEF_TABMOUNTER_GANTRY_LOAD4_POS		= 3;	//TabCarrier4번 Loading위치
const int DEF_TABMOUNTER_GANTRY_LOAD_POS		= 0;	//TabCarrier Loading위치
const int DEF_TABMOUNTER_GANTRY_INSPECTION_POS	= 1;	//
const int DEF_TABMOUNTER_GANTRY_MOUNT_POS		= 2;	//Tab Align 결과 반영해서 들어갈 것. 기준 Tab 은? 아니면 평균으로? 확인 필요.
const int DEF_TABMOUNTER_GANTRY_TABREMOVE_POS	= 3;	//@130202
/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

const int ERR_TABMOUNTER_SUCCESS				= 0;
const int ERR_TABMOUNTER_WAIT					= 1;

/************************************************************************/
/*                          Etc. Define			                        */
/************************************************************************/

const int DEF_TABMOUNTER_DOWN_TYPE_MIDDLE		= 0;
const int DEF_TABMOUNTER_DOWN_TYPE_DOWN			= 1;
#endif //DEF_TabMounter_H
