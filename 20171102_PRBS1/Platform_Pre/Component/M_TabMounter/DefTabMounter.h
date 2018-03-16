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
 * �� ����ü�� Mechanical Layer�� ��ȣ ������ ���� ���ο� �����
 * Component List�� �����ϴ� ����ü�̴�.
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
	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData* m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkScheduleSecond;

	MTabOffsetData*	m_plnkTabOffsetData;

	//120208.kms______
	MTabOffsetData* m_plnkRearTabOffsetData;

	/** PositionInfo.inf File ���� ����ü */
	STeachingInfo	m_sTeachingInfoMounter;
	STeachingInfo	m_sTeachingInfoMounterZ;
	STeachingInfo	m_sTeachingInfoGantry;
	
	/** Offset Data�� ����� File Name */
	CString			m_strOffsetPosFileName;
	
	/** Parameter Data �� ����� File Name */
	CString			m_strPosFileName;

	/** TabMounter �� ���� Group ��ȣ */
	int				m_iGroupNo;

	/** �۾��� ���� */
	EWorkingSide	m_eWorkSide;

	/** **************************************************************************
	 *	����/���� TabMounter ��ȣ ����
	 *  �� ����/���� ���� ����
	 *		- �ڽź��� ���� Prealign ��ġ�� �̵��ϴ� TabMounter �� ���� TabMounter ��.
	 *		- Gate �� ��� TabCarrier Unload ��ġ�� ���� Front/Rear Group �� TabMounter ��� ��ġ�� �޶���.
	 */

	/** ���� TabMounter ��ȣ : 1�� TabMounter �� [���� TabMounter ��ȣ]�� -1 �̴�.
	 *	: TabMounter �� Mount ��ġ�� �̵��ϱ� ����, Previous TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iPreTabMounterNo;

	/** ���� TabMounter ��ȣ
	 *	: �� Group �� ������ TabMounter �� [���� TabMounter ��ȣ]�� DEF_LAST_PER_GROUP �̴�.
	 *	: TabMounter �� Load ��ġ�� �̵��ϱ� ����, Next TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
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

 const int DEF_TABMOUNTER_READY_POS				= 0;	// Gate : Tab PreAlign �� ������ TabMounter ��� ��ġ
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
														//���� ��ǥ ���� Tool
const int DEF_TABMOUNTER_READY_POS				= 0;	//Loading��ġ�� ����													//1~4�� 1�� Tool�� ����, 5~8�� 8�� Tool�� ����
const int DEF_TABMOUNTER_LOAD_POS				= 1;	//1~4���� 1��Tool�� ����, 5~8���� 8��Tool�� ����						//1~8�� 1�� Tool�� ����
const int DEF_TABMOUNTER_R_LOAD_POS				= 2;	//1~4���� 8��Tool Load��ġ�� ����, 5~8���� 1��Tool Load��ġ�� ����		//1~8�� 8�� Tool�� ����
const int DEF_TABMOUNTER_ESCAPE_POS				= 2;	//Gate������. Loading������ Next Head�� TabLoading�ϱ����� �����ִ� ��ġ
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

//const int DEF_TABMOUNTER_GANTRY_LOAD1_POS		= 0;	//TabCarrier1�� Loading��ġ
//const int DEF_TABMOUNTER_GANTRY_LOAD2_POS		= 1;	//TabCarrier2�� Loading��ġ
//const int DEF_TABMOUNTER_GANTRY_LOAD3_POS		= 2;	//TabCarrier3�� Loading��ġ
//const int DEF_TABMOUNTER_GANTRY_LOAD4_POS		= 3;	//TabCarrier4�� Loading��ġ
const int DEF_TABMOUNTER_GANTRY_LOAD_POS		= 0;	//TabCarrier Loading��ġ
const int DEF_TABMOUNTER_GANTRY_INSPECTION_POS	= 1;	//
const int DEF_TABMOUNTER_GANTRY_MOUNT_POS		= 2;	//Tab Align ��� �ݿ��ؼ� �� ��. ���� Tab ��? �ƴϸ� �������? Ȯ�� �ʿ�.
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
