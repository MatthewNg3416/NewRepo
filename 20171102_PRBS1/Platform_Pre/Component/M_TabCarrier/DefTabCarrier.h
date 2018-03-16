/* 
 * TabCarrier Component
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
 * DefTabCarrier.h : This file defines the value of TabCarrier component.
 *
 */

#ifndef DEF_TABCARRIER_H
#define DEF_TABCARRIER_H
#include "MTeachingInfo.h"

class MSystemData;
class MPreBonderData;
class MTabOffsetData;
class MTrsAutoManager; //SJ_YYK 150109 Add..
class ICylinder;
class IVacuum;
class IAxis;
class ISercosMultiAxis;
class IStateTabFeeder;
class IStateTabMounter;
class IStateTabCarrier;

/************************************************************************/
/*                             Value Define                             */
/************************************************************************/

/* Tab Carrier Vacuum ID */
const int DEF_VAC_TAB_CARRIER_ALL			= -1;
const int DEF_VAC_TAB_CARRIER_1				= 0;
const int DEF_VAC_TAB_CARRIER_2				= 1;
const int DEF_VAC_TAB_CARRIER_3				= 2;
const int DEF_VAC_TAB_CARRIER_4				= 3;
const int DEF_VAC_TAB_CARRIER_5				= 4;
const int DEF_VAC_TAB_CARRIER_6				= 5;
const int DEF_VAC_TAB_CARRIER_7				= 6;
const int DEF_VAC_TAB_CARRIER_8				= 7;

/** Tab Carrier Axis Order */
//const int DEF_TABCARRIER_AXIS_MAX_NO		= 2;
//const int DEF_TABCARRIER_X_AXIS			= 0;
//const int DEF_TABCARRIER_Y_AXIS			= 1;
const int DEF_TABCARRIER_AXIS_MAX_NO		= 3;
const int DEF_TABCARRIER_X_AXIS				= 0;
const int DEF_TABCARRIER_Y_AXIS				= 1;
const int DEF_TABCARRIER_T_AXIS				= 2;
const int DEF_TABCARRIER_EXPAND_AXIS		= 3;

/** TabCarrier Position Define	*/
/************************************************************************/
/*                           Position  Define                           */
/************************************************************************/

const int DEF_TABCAMERA_SHRINK_POS_ID		= 0;
const int DEF_TABCAMERA_EXPAND_POS_ID		= 1;

const int DEF_TABCARRIER_NO_WORKING_POS		= -1;
const int DEF_TABCARRIER_READY_POS			= 0;
const int DEF_TABCARRIER_LOAD_POS			= 1;
const int DEF_TABCARRIER_PREALIGN_POS		= 2;
const int DEF_TABCARRIER_UNLOAD_POS			= 3;
const int DEF_TABCARRIER_ESCAPE_POS			= 4;
const int DEF_TABCARRIER_LOAD_START_POS		= 5;

const int DEF_TABCARRIER_MAX_POSITION		= 6; //SJ_YYK 150109 Add..


const int DEF_TABCARRIER_EXPAND_NO_POS		= -1;
const int DEF_TABCARRIER_EXPAND_INIT_POS	= 0;
const int DEF_TABCARRIER_EXPAND_WORKING_POS	= 1;
const int DEF_TABCARRIER_EXPAND_MAX_POS		= 2;

//SJ_YYK 150109 Add...
const int DEF_TABCARRIER_Z_DOWN_POS		= 0;
const int DEF_TABCARRIER_Z_UP_POS		= 1;
const int DEF_TABCARRIER_Z_MAX_POS		= 2;

/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

const int ERR_TABCARRIER_SUCCESS			= 0;

/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/

typedef
/**
 * Referenced Component List by TabCarrier Component
 */
struct tagSTabCarrierRefCompList
{
	/** X Axis Object */
    IAxis*		m_plnkXAxis;

	/** Y Axis Object */
    IAxis*		m_plnkYAxis;

	/** T Axis Object */
    IAxis*		m_plnkTAxis;

	IAxis*		m_plnkExpandAxis;

	IAxis*		m_plnkZAxis;

	ISercosMultiAxis*	m_plnkXZAxis;  //SJ_YYK 150212 Add..

	/** Up/Down Cylinder Object */
    ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkUDCyl2; //SJ_YYK 150817 Add...

	/** For/Back Cylinder Object */
    ICylinder*	m_plnkFBCyl;

	ICylinder*	m_plnkCamESCyl;

	ICylinder*	m_plnkDumpBasketFBCyl;

	/** Vacuum Line Object */
	IVacuum*	m_plnkVac;

	
} STabCarrierRefCompList;

typedef
/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
struct
{
	IStateTabFeeder*	m_plnkStateTabFeeder;
	IStateTabMounter*	m_plnkStateTabMounter[DEF_MAX_WORKER_PER_GROUP];
	IStateTabCarrier*	m_plnkStateTabCarrier[DEF_MAX_TABCARRIER];

} SStateCheckCompListForTabCarrier;

typedef
/**
 * TabCarrier Component Data	
 */ 
struct STabCarrierData
{
	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;

	/** PreBonder Data Object */
	MPreBonderData* m_plnkPreBonderData;

	MTabOffsetData*	m_plnkTabOffsetData;

	/** X,Y 축 PositionInfo.inf File 정보 구조체 */
	STeachingInfo	m_sXYTTeachingInfo;

	STeachingInfo	m_sExpandTeachingInfo;

	STeachingInfo	m_sZTeachingInfo; //SJ_YYK 150109 Add...

	/** 고정좌표 Data가 저장될 File Name */
	CString			m_strPosFileName;

	/** Offset Data가 저장될 File Name */
	CString			m_strOffsetPosFileName;

	/** 소유하고 있는 Camera 번호 */
	int				m_iCamNo1;
	int				m_iCamNo2;

	/** **************************************************************************
	 *	이전/다음 TabCarrier 번호 설정
	 *  ⇒ 이전/다음 구분 기준
	 *		- 자신보다 먼저 작업 위치로 이동하는 TabMounter 가 이전 TabMounter 임.
	 */

	/** 이전 TabMounter 번호 : 1번 TabMounter 의 [이전 TabMounter 번호]는 -1 이다.
	 *	: TabMounter 가 Mount 위치로 이동하기 전에, Previous TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iPreTabCarrierNo;

	/** 다음 TabMounter 번호
	 *	: 각 Group 의 마지막 TabMounter 의 [다음 TabMounter 번호]는 DEF_LAST_PER_GROUP 이다.
	 *	: TabMounter 가 Load 위치로 이동하기 전에, Next TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iNextTabCarrierNo;



} STabCarrierData;

#endif //DEF_TABCARRIER_H