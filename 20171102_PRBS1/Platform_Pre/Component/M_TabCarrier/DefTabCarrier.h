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
 * �� ����ü�� Mechanical Layer�� ��ȣ ������ ���� ���ο� �����
 * Component List�� �����ϴ� ����ü�̴�.
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
	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;

	/** PreBonder Data Object */
	MPreBonderData* m_plnkPreBonderData;

	MTabOffsetData*	m_plnkTabOffsetData;

	/** X,Y �� PositionInfo.inf File ���� ����ü */
	STeachingInfo	m_sXYTTeachingInfo;

	STeachingInfo	m_sExpandTeachingInfo;

	STeachingInfo	m_sZTeachingInfo; //SJ_YYK 150109 Add...

	/** ������ǥ Data�� ����� File Name */
	CString			m_strPosFileName;

	/** Offset Data�� ����� File Name */
	CString			m_strOffsetPosFileName;

	/** �����ϰ� �ִ� Camera ��ȣ */
	int				m_iCamNo1;
	int				m_iCamNo2;

	/** **************************************************************************
	 *	����/���� TabCarrier ��ȣ ����
	 *  �� ����/���� ���� ����
	 *		- �ڽź��� ���� �۾� ��ġ�� �̵��ϴ� TabMounter �� ���� TabMounter ��.
	 */

	/** ���� TabMounter ��ȣ : 1�� TabMounter �� [���� TabMounter ��ȣ]�� -1 �̴�.
	 *	: TabMounter �� Mount ��ġ�� �̵��ϱ� ����, Previous TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iPreTabCarrierNo;

	/** ���� TabMounter ��ȣ
	 *	: �� Group �� ������ TabMounter �� [���� TabMounter ��ȣ]�� DEF_LAST_PER_GROUP �̴�.
	 *	: TabMounter �� Load ��ġ�� �̵��ϱ� ����, Next TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int				m_iNextTabCarrierNo;



} STabCarrierData;

#endif //DEF_TABCARRIER_H