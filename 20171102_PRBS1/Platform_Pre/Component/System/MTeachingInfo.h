/* samsung */
/* 
 * System Info 
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
 * MTeachingInfo.h : This file defines object type of System.
 *            
 *
 */

#ifndef MTeachingInfo_H
#define MTeachingInfo_H

#include "DefSystem.h"

const int DEF_MAX_TEACHING_POS	= 40;
const int DEF_MAX_TEACHING_AXIS	= 4;

/////////////////////////////////////////////////////////
/** Teaching Unit Index Value for motion(PreBonder) */
#ifdef DEF_SOURCE_SYSTEM

const int DEF_PREBONDER_UNIT_MAX_TEACHING_UNIT			= 75;

const int DEF_PREBONDER_UNIT_STAGE_XYT					= 0;
const int DEF_PREBONDER_UNIT_STAGE_Z					= 1;
const int DEF_PREBONDER_UNIT_PANEL_TRANSFER_IN_X		= 2;
const int DEF_PREBONDER_UNIT_PANEL_TRANSFER_OUT_X		= 3;
const int DEF_PREBONDER_UNIT_MODEL_CHANGE_Y1			= 4;
const int DEF_PREBONDER_UNIT_MODEL_CHANGE_Y2			= 5;

const int DEF_PREBONDER_UNIT_BADTAB_DETECTOR_X			= 6;
const int DEF_PREBONDER_UNIT_FEEDER_SPROCKET1_T			= 7;
const int DEF_PREBONDER_UNIT_FEEDER_SPROCKET2_T			= 8;
const int DEF_PREBONDER_UNIT_TAB_PRESS_Z				= 9;
const int DEF_PREBONDER_UNIT_MOLDING_UPDOWN_Z			= 10;

const int DEF_PREBONDER_UNIT_R_BADTAB_DETECTOR_X		= 11;
const int DEF_PREBONDER_UNIT_R_FEEDER_SPROCKET1_T		= 12;
const int DEF_PREBONDER_UNIT_R_FEEDER_SPROCKET2_T		= 13;
const int DEF_PREBONDER_UNIT_R_TAB_PRESS_Z				= 14;
const int DEF_PREBONDER_UNIT_R_MOLDING_UPDOWN_Z			= 15;

const int DEF_PREBONDER_UNIT_MOUNTER1_X					= 16;
const int DEF_PREBONDER_UNIT_MOUNTER2_X					= 17;
const int DEF_PREBONDER_UNIT_MOUNTER3_X					= 18;
const int DEF_PREBONDER_UNIT_MOUNTER4_X					= 19;

const int DEF_PREBONDER_UNIT_R_MOUNTER1_X				= 20;
const int DEF_PREBONDER_UNIT_R_MOUNTER2_X				= 21;
const int DEF_PREBONDER_UNIT_R_MOUNTER3_X				= 22;
const int DEF_PREBONDER_UNIT_R_MOUNTER4_X				= 23;

const int DEF_PREBONDER_UNIT_MOUNTER1_Z					= 24;
const int DEF_PREBONDER_UNIT_MOUNTER2_Z					= 25;
const int DEF_PREBONDER_UNIT_MOUNTER3_Z					= 26;
const int DEF_PREBONDER_UNIT_MOUNTER4_Z					= 27;

const int DEF_PREBONDER_UNIT_R_MOUNTER1_Z				= 28;
const int DEF_PREBONDER_UNIT_R_MOUNTER2_Z				= 29;
const int DEF_PREBONDER_UNIT_R_MOUNTER3_Z				= 30;
const int DEF_PREBONDER_UNIT_R_MOUNTER4_Z				= 31;

const int DEF_PREBONDER_UNIT_BACKUP1_Z					= 32;
const int DEF_PREBONDER_UNIT_BACKUP2_Z					= 33;
const int DEF_PREBONDER_UNIT_BACKUP3_Z					= 34;
const int DEF_PREBONDER_UNIT_BACKUP4_Z					= 35;

const int DEF_PREBONDER_UNIT_R_BACKUP1_Z				= 36;
const int DEF_PREBONDER_UNIT_R_BACKUP2_Z				= 37;
const int DEF_PREBONDER_UNIT_R_BACKUP3_Z				= 38;
const int DEF_PREBONDER_UNIT_R_BACKUP4_Z				= 39;

const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA1_X		= 40;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA2_X		= 41;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA3_X		= 42;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA4_X		= 43;

const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA1_X		= 44;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA2_X		= 45;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA3_X		= 46;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA4_X		= 47;

const int DEF_PREBONDER_UNIT_TABIC_CARRIER1_XYT			= 48;
const int DEF_PREBONDER_UNIT_TABIC_CARRIER2_XYT			= 49;
const int DEF_PREBONDER_UNIT_TABIC_CARRIER3_XYT			= 50;
const int DEF_PREBONDER_UNIT_TABIC_CARRIER4_XYT			= 51;

const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER1_XYT		= 52;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER2_XYT		= 53;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER3_XYT		= 54;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER4_XYT		= 55;

const int DEF_PREBONDER_UNIT_TABMOUNTER_GANTRY_Y		= 56;
const int DEF_PREBONDER_UNIT_R_TABMOUNTER_GANTRY_Y		= 57;

const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA1_EXPAND	= 58;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA2_EXPAND	= 59;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA3_EXPAND	= 60;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA4_EXPAND	= 61;
const int DEF_PREBONDER_UNIT_TABIC_CARRIER_EXPAND		= 62;

const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA1_EXPAND	= 63;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA2_EXPAND	= 64;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA3_EXPAND	= 65;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA4_EXPAND	= 66;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER_EXPAND			= 67;

const int DEF_PREBONDER_UNIT_TABIC_CARRIER_Z				= 68;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER_Z				= 69;

const int DEF_PREBONDER_UNIT_TABIC_CARRIER_EXPAND2		= 70;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER_EXPAND2			= 71;

//SJ_YYK 161104 Add..
const int DEF_PREBONDER_UNIT_PANEL_TRANSFER_CENTER_X		= 72;
const int DEF_PREBONDER_UNIT_HANDLER_XT						= 73;
const int DEF_PREBONDER_UNIT_HANDLER_Z						= 74; 

	

#else	//#ifdef DEF_SOURCE_SYSTEM

const int DEF_PREBONDER_UNIT_MAX_TEACHING_UNIT			= 43;

const int DEF_PREBONDER_UNIT_STAGE_XYT					= 0;
const int DEF_PREBONDER_UNIT_STAGE_Z					= 1;
const int DEF_PREBONDER_UNIT_PANEL_TRANSFER_OUT_X		= 2;
const int DEF_PREBONDER_UNIT_MODEL_CHANGE_Y1			= 3;
const int DEF_PREBONDER_UNIT_MODEL_CHANGE_Y2			= 4;

const int DEF_PREBONDER_UNIT_BADTAB_DETECTOR_X			= 5;
const int DEF_PREBONDER_UNIT_FEEDER_SPROCKET1_T			= 6;
const int DEF_PREBONDER_UNIT_FEEDER_SPROCKET2_T			= 7;
const int DEF_PREBONDER_UNIT_TAB_PRESS_Z				= 8;
const int DEF_PREBONDER_UNIT_MOLDING_UPDOWN_Z			= 9;

const int DEF_PREBONDER_UNIT_R_BADTAB_DETECTOR_X		= 10;
const int DEF_PREBONDER_UNIT_R_FEEDER_SPROCKET1_T		= 11;
const int DEF_PREBONDER_UNIT_R_FEEDER_SPROCKET2_T		= 12;
const int DEF_PREBONDER_UNIT_R_TAB_PRESS_Z				= 13;
const int DEF_PREBONDER_UNIT_R_MOLDING_UPDOWN_Z			= 14;

const int DEF_PREBONDER_UNIT_MOUNTER1_X					= 15;
const int DEF_PREBONDER_UNIT_MOUNTER2_X					= 16;

const int DEF_PREBONDER_UNIT_R_MOUNTER1_X				= 17;
const int DEF_PREBONDER_UNIT_R_MOUNTER2_X				= 18;

const int DEF_PREBONDER_UNIT_MOUNTER1_Z					= 19;
const int DEF_PREBONDER_UNIT_MOUNTER2_Z					= 20;

const int DEF_PREBONDER_UNIT_R_MOUNTER1_Z				= 21;
const int DEF_PREBONDER_UNIT_R_MOUNTER2_Z				= 22;

const int DEF_PREBONDER_UNIT_BACKUP1_Z					= 23;
const int DEF_PREBONDER_UNIT_BACKUP2_Z					= 24;

const int DEF_PREBONDER_UNIT_R_BACKUP1_Z				= 25;
const int DEF_PREBONDER_UNIT_R_BACKUP2_Z				= 26;

const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA1_X		= 27;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA2_X		= 28;

const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA1_X		= 29;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA2_X		= 30;

const int DEF_PREBONDER_UNIT_TABIC_CARRIER1_XYT			= 31;
const int DEF_PREBONDER_UNIT_TABIC_CARRIER2_XYT			= 32;

const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER1_XYT		= 33;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER2_XYT		= 34;

const int DEF_PREBONDER_UNIT_TABMOUNTER_GANTRY_Y		= 35;
const int DEF_PREBONDER_UNIT_R_TABMOUNTER_GANTRY_Y		= 36;

const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA1_EXPAND	= 37;
const int DEF_PREBONDER_UNIT_INSPECTION_CAMERA2_EXPAND	= 38;
const int DEF_PREBONDER_UNIT_TABIC_CARRIER_EXPAND		= 39;

const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA1_EXPAND	= 40;
const int DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA2_EXPAND	= 41;
const int DEF_PREBONDER_UNIT_R_TABIC_CARRIER_EXPAND			= 42;

#endif

//_________________________________________________________

typedef struct  tagSPosition
{
	BOOL			m_bUse;
	BOOL			m_bEnable;
	CString			m_strNewLineName[3];	// GUI에서 사용할 Name, Language Support
	CString			m_strName;				// Position Key Name
} SPosition;

// 파일에서 읽어 들일 Teaching Postion Info
typedef struct  tagSTeachingInfo
{
	CString			m_strUnitName[3];		// GUI Name
	CString			m_strSectionName;		// Section Name
	int				m_iNumAxis;
	CString			m_strSectionPostfix[DEF_MAX_TEACHING_AXIS];
	int				m_iNumTeachingPos;
	SPosition		m_sPosition[DEF_MAX_TEACHING_POS];
} STeachingInfo;

/**
 * MTeachingInfo Class Definition	
 */
class MTeachingInfo
{
private:
	CString			m_strFile;
	STeachingInfo	m_sTeachingInfo[DEF_PREBONDER_UNIT_MAX_TEACHING_UNIT];

public:    
    MTeachingInfo(CString strFileName);
	~MTeachingInfo();

	int				LoadTeachingInfo();

	STeachingInfo	GetTeachingInfo(int iIndex);
};
#endif //MTeachingInfo_H
