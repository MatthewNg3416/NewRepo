/* 
 * CameraCarrier Component
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
 * DefCameraCarrier.h : This file defines the value of CameraCarrier component.
 *
 */

#ifndef DEF_CAMERACARRIER_H
#define DEF_CAMERACARRIER_H

#include "MTeachingInfo.h"

class IAxis;
class ICylinder;
class MSystemData;
class MPanelData;
class MPreBonderData;
class MProcessData;
class MTabData;
class IStateTabMounter;
class IStateCameraCarrier;
class IStatePanelAligner;
class MWorkSchedule;

/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/
typedef enum enumStateCamera
{
	ALIGN_TYPE		= 0,
	INSPECTION_TYPE
} ECameraWorkType;

/**
 * Referenced Component List by CameraCarrier Component
 */
typedef struct tagSCameraCarrierRefCompList
{
    IAxis*		m_plnkAxis;
	IAxis*		m_plnkAxisBackupZ;
	IAxis*		m_plnkAxisExpand;
	ICylinder*	m_plnkESCyl;	// Only Use for Mech ver2.0
	
} SCameraCarrierRefCompList;

/**
 * �� ����ü�� Mechanical Layer�� ��ȣ ������ ���� ���ο� �����
 * Component List�� �����ϴ� ����ü�̴�.
 *
 * @stereotype struct 
 */
typedef struct
{
	IStateTabMounter*		m_plnkTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkPreCamCarrier;
	IStateCameraCarrier*	m_plnkNextCamCarrier;
	IStatePanelAligner*		m_plnkPanelAligner;
#if FALSE
	IStateTabMounter*		m_plnkTabMounter;
#endif
} SStateCheckCompListForCameraCarrier;

typedef
/**
 * CameraCarrier Component Data	
 */ 
struct SCameraCarrierData
{
	/** Data ���� Class */
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData*	m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
	MTabData*		m_plnkTabData; //SJ_YYK 150109 Add..
	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkScheduleSecond;

	/** PositionInfo.inf File ���� ����ü */
	STeachingInfo	m_sTeachingInfo;
	STeachingInfo	m_sTeachingInfoBackupZ;
	STeachingInfo	m_sTeachingInfoExpand;

	/** Offset Data�� ����� File Name */
	CString			m_strOffsetPosFileName;
	
	/** Parameter Data �� ����� File Name */
	CString			m_strPosFileName;

	/** CameraCarrier �� ���� Group ��ȣ */
	int				m_iGroupNo;

	/** �۾��� ���� */
	EWorkingSide	m_eWorkSide;

	/** �����ϰ� �ִ� Camera ��ȣ */
	int				m_iCamNo1;
	int				m_iCamNo2;

#if FALSE
	/** �����ϰ� �ִ� Camera ��ȣ */
	int				m_iCamNo;
#endif

} SCameraCarrierData;

/************************************************************************/
/*                           Position  Define                           */
/************************************************************************/

const int DEF_CAMERACARRIER_AXIS_MAX_NO			= 1;
const int DEF_CAMERACARRIER_AXIS				= 0;

//const int DEF_CAMERACARRIER_MAX_POS				= 3;
const int DEF_CAMERACARRIER_MAX_POS				= 4;
const int DEF_CAMERACARRIER_NONE_POS			= -1;

const int DEF_CAMERACARRIER_PANEL_MARK_POS		= 0;
const int DEF_CAMERACARRIER_PANEL_SUB_MARK_POS	= 1;
const int DEF_CAMERACARRIER_TAB_MARK_POS		= 2;
const int DEF_CAMERACARRIER_TAB_MOUNT_POS		= 3;


const int DEF_CAMERACARRIER_BACKUP_MAX_POS		= 3;
const int DEF_CAMERACARRIER_BACKUP_Z_NONE_POS	= -1;

const int DEF_CAMERACARRIER_BACKUP_DOWN_POS		= 0;	
const int DEF_CAMERACARRIER_BACKUP_VISION_POS	= 1;	// TAB ��ũ�� Panel ��ũ inspection �� �� Mount ��ġ�� ������ ���̷� Panel ���� ������ �ʿ��Ѱ�??.
const int DEF_CAMERACARRIER_BACKUP_UP_POS		= 2;	// Mount ��ġ


const int DEF_CAMERACARRIER_EXPAND_NO_POS		= -1;
const int DEF_CAMERACARRIER_EXPAND_INIT_POS		= 0;
const int DEF_CAMERACARRIER_EXPAND_WORKING_POS	= 1;
const int DEF_CAMERACARRIER_EXPAND_MAX_POS		= 2;

/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

const int ERR_CAMERACARRIER_SUCCESS				= 0;

#endif //DEF_CameraCarrier_H
