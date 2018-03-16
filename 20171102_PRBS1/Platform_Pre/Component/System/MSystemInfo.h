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
 * MSystemInfo.h : This file defines object type of System.
 *            
 *
 */

#ifndef MSYSTEMINFO_H
#define MSYSTEMINFO_H

#include "ICommon.h"

/** Value Define */
const	int		DEF_MAX_OBJECT_NO								= 5000;
const	int		DEF_MAX_OBJECT_INFO_NO							= 500;
const	int		DEF_MAX_OBJ_KINDS								= 100;
const	int		DEF_MAX_SYSTEM_INFO_LOG_ITEM					= 200;

/** Error Define */
const	int		ERR_SYSTEMINFO_SUCCESS							= 0;
const	int		ERR_SYSTEMINFO_INVALID_POINTER					= 1;
const	int		ERR_SYSTEMINFO_INVALID_OBJECTID					= 2;
const	int		ERR_SYSTEMINFO_NOT_REGISTED_OBJECTID			= 3;
const	int		ERR_SYSTEMINFO_CANNOT_FIND_LOG_OBJECT			= 4;
const	int		ERR_SYSTEMINFO_REGISTER_FAIL					= 5;


const	int		DEF_SYSTEM_INFO_MIN_LOG_KEEPING_DAYS			= 5;
const	int		DEF_SYSTEM_INFO_MAX_LOG_KEEPING_DAYS			= 90;


/** Object Base 정의 */
//Hardware Layer (0~1999)
const int	DEF_IO_OBJECT_BASE								= 1;
const int	DEF_MOTION_LIB_OBJECT_BASE						= 2;
const int	DEF_VISION_OBJECT_BASE							= 3;
const int	DEF_SERIAL_OBJECT_BASE							= 10;
const int	DEF_MELSEC_OBJECT_BASE							= 50;
const int	DEF_CYLINDER_OBJECT_BASE						= 100;
const int	DEF_VACUUM_OBJECT_BASE							= 500;
//const int	DEF_INDMOTOR_OBJECT_BASE						= 700;
const int	DEF_DACONVERTER_OBJECT_BASE						= 800;
const int	DEF_MOTION_OBJECT_BASE							= 1000;
const int	DEF_MULTI_MOTION_OBJECT_BASE					= 1100;
const int	DEF_LIGHTING_OBJECT_BASE						= 1150;	// 18 개까지 사용. 1160 번대도 비워 둘 것.
//const int	DEF_LIGHTING_OBJECT_BASE_2						= 1160;
const int	DEF_INVERTER_OBJECT_BASE						= 1170;
const int	DEF_SERCOS_SETUP_OBJECT_BASE					= 1180;
const int	DEF_CAN_SETUP_OBJECT_BASE						= 1190;
//const int	DEF_SENSORCOMM_OBJECT_BASE						= 1200;

//Mechanical Layer (2000~2999)
//const int	DEF_LIGNTENING_OBJECT_BASE						= 2000;
const int	DEF_ONLINE_OBJECT_BASE							= 2010;
const int	DEF_OPPANEL_OBJECT_BASE							= 2020;
const int	DEF_BCR_OBJECT_BASE								= 2030;
const int	DEF_RFID_OBJECT_BASE							= 2040;
//const int	DEF_STAGE_OBJECT_BASE							= 2050;
const int	DEF_HANDLER_OBJECT_BASE							= 2060;
//const int	DEF_WORKBENCH_OBJECT_BASE						= 2070;
//const int	DEF_ACFFEEDER_OBJECT_BASE						= 2080;
//const int	DEF_ACFTOOL_OBJECT_BASE							= 2090;
//const int	DEF_MAINTOOL_OBJECT_BASE						= 2100;
//const int	DEF_TRAYLOADER_OBJECT_BASE						= 2110;
//const int	DEF_TRAYUNLOADER_OBJECT_BASE					= 2120;
//const int	DEF_NEWTRAYTRANSFER_OBJECT_BASE					= 2130;
//const int	DEF_EMPTYTRAYTRANSFER_OBJECT_BASE				= 2140;
//const int	DEF_PCBTRANSFER_OBJECT_BASE						= 2150;
//const int	DEF_PCBHANDLER_OBJECT_BASE						= 2160;
//const int	DEF_ACFREEL_OBJECT_BASE							= 2170;
const int	DEF_CAMERA_CARRIER_OBJECT_BASE					= 2180;
//const int	DEF_TABLOADER_OBJECT_BASE						= 2190;

const int	DEF_TABCARRIER_OBJECT_BASE						= 2200;
const int	DEF_TABFEEDER_OBJECT_BASE						= 2210;
//const int	DEF_INDEX_OBJECT_BASE							= 2220;
//const int	DEF_MONITOUCH_OBJECT_BASE						= 2230;
//const int	DEF_BACKUP_OBJECT_BASE							= 2240;
//const int	DEF_TABDETECTOR_OBJECT_BASE						= 2250;
//const int	DEF_MONITOUCH_SWITCH_OBJECT_BASE				= 2260;
const int	DEF_ROLLERCONVEYOR_OBJECT_BASE					= 2270;
const int	DEF_PANEL_ALIGNER_OBJECT_BASE					= 2280;
const int	DEF_PANEL_TRANSFER_OBJECT_BASE					= 2290;
const int	DEF_TAB_MOUNTER_OBJECT_BASE						= 2300;
//const int	DEF_TAB_MOUNTER_OBJECT_BASE						= 2310;	// Mounter 12 개까지 사용 => 10번대까지 비워둬야 함.
const int	DEF_MODEL_CHANGE_OBJECT_BASE					= 2320;
const int	DEF_TAB_ALIGNER_OBJECT_BASE						= 2330;

//const int	DEF_HEART_CONTROLLER_OBJECT_BASE				= 2500;
//const int	DEF_INTERFACE_CTRL_OBJECT_BASE					= 2510;
const int	DEF_MANAGE_OPPANEL_OBJECT_BASE					= 2520;
const int	DEF_TEACHING_OJBECT_BASE						= 2530;
//const int   DEF_PCBLOADER_OBJECT_BASE						= 2540;

//Control Layer (3000 ~ 3999)
//const int	DEF_CTR_AUTOMANAGER_OBJECT_BASE					= 3000;
const int	DEF_CTR_PANEL_ALIGNER_OBJECT_BASE				= 3010;
const int	DEF_CTR_HANDLER_OBJECT_BASE						= 3020;
//const int	DEF_CTR_WORKBENCH_OBJECT_BASE					= 3030;
//const int	DEF_CTR_ACFFEEDER_OBJECT_BASE					= 3040;
//const int	DEF_CTR_ACFTOOL_OBJECT_BASE						= 3050;
//const int	DEF_CTR_MAINTOOL_OBJECT_BASE					= 3060;
//const int	DEF_CTR_TRAYLOADER_OBJECT_BASE					= 3070;
//const int	DEF_CTR_TRAYUNLOADER_OBJECT_BASE				= 3080;
//const int	DEF_CTR_NEWTRAYTRANSFER_OBJECT_BASE				= 3090;
//const int	DEF_CTR_EMPTYTRAYTRANSFER_OBJECT_BASE			= 3100;
//const int	DEF_CTR_PCBTRANSFER_OBJECT_BASE					= 3110;
//const int	DEF_CTR_PCBHANDLER_OBJECT_BASE					= 3120;
const int	DEF_CTR_JOG_OBJECT_BASE							= 3200;
const int	DEF_CTR_MANAGE_OPPANEL_OBJECT_BASE				= 3210;
//const int	DEF_CTR_MANAGE_PRODUCT_OBJECT_BASE				= 3220;
const int	DEF_CTR_TEACHING_OBJECT_BASE					= 3230;
const int	DEF_CTR_VISION_CALIBRATION_OBJECT_BASE			= 3240;
const int	DEF_CTR_CAMERA_CARRIER_OBJECT_BASE				= 3250;
const int	DEF_CTR_TAB_FEEDER_OBJECT_BASE					= 3260;
//const int	DEF_CTR_INDEX_OBJECT_BASE						= 3270;
const int	DEF_CTR_TAB_CARRIER_OBJECT_BASE					= 3280;
//const int	DEF_CTR_MONITOUCH_OBJECT_BASE					= 3290;
//const int	DEF_CTR_TAB_LOADER_OBJECT_BASE					= 3300;
const int	DEF_CTR_TAB_MOUNTER_OBJECT_BASE					= 3310;
const int	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE			= 3330;
//const int	DEF_RESOURCE_MANAGER_OBJECT_BASE				= 3500;

//Process Layer (4000 ~ 4999)
const int	DEF_TRS_AUTOMANAGER_OBJECT_BASE					= 4000;
const int	DEF_TRS_PANEL_ALIGNER_OBJECT_BASE				= 4010;
const int	DEF_TRS_HANDLER_OBJECT_BASE						= 4020;
//const int	DEF_TRS_WORKBENCH_OBJECT_BASE					= 4030;
//const int	DEF_TRS_ACFFEEDER_OBJECT_BASE					= 4040;
//const int	DEF_TRS_ACFTOOL_OBJECT_BASE						= 4050;
//const int	DEF_TRS_MAINTOOL_OBJECT_BASE					= 4060;
//const int	DEF_TRS_TRAYLOADER_OBJECT_BASE					= 4070;
//const int	DEF_TRS_TRAYUNLOADER_OBJECT_BASE				= 4080;
//const int	DEF_TRS_NEWTRAYTRANSFER_OBJECT_BASE				= 4090;
//const int	DEF_TRS_EMPTYTRAYTRANSFER_OBJECT_BASE			= 4100;
//const int	DEF_TRS_PCBTRANSFER_OBJECT_BASE					= 4110;
//const int	DEF_TRS_PCBHANDLER_OBJECT_BASE					= 4120;
const int	DEF_TRS_TABFEEDER_OBJECT_BASE					= 4130;
//const int	DEF_TRS_INDEX_OBJECT_BASE						= 4140;
const int	DEF_TRS_TABCARRIER_OBJECT_BASE					= 4150;
const int	DEF_TRS_MONITOUCHTENSION_OBJECT_BASE			= 4160;
//const int	DEF_TRS_MANAGEINDEX_OBJECT_BASE					= 4170;
//const int	DEF_TRS_LOADINDEX_OBJECT_BASE					= 4180;
//const int	DEF_TRS_CLEANINDEX_OBJECT_BASE					= 4190;
//const int	DEF_TRS_REMOVEINDEX_OBJECT_BASE					= 4200;
//const int	DEF_TRS_TABCARRIER_SCHEDULER_OBJECT_BASE		= 4210;
//const int	DEF_TRS_PREALIGNINDEX_OBJECT_BASE				= 4220;
//const int	DEF_TRS_CONVEYOR_OBJECT_BASE					= 4230;
const int	DEF_TRS_PANEL_TRANSFER_OBJECT_BASE				= 4240;
const int	DEF_TRS_TAB_MOUNTER_OBJECT_BASE					= 4250;
//const int	DEF_TRS_TABLOADER_OBJECT_BASE					= 4260;
//const int	DEF_TRS_CAMERACARRIER_OBJECT_BASE				= 4270;
const int	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE			= 4280;
const int	DEF_TRS_LOAD_CONVEYOR_OBJECT_BASE				= 4290;
const int	DEF_TRS_UNLOAD_CONVEYOR_OBJECT_BASE				= 4300;
const int	DEF_TRS_JOG_OBJECT_BASE							= 4400;

typedef
/**
 * Object Type Enum Define
 * @stereotype enum
 */
enum
{
	OBJ_NONE,

	// Hardware Layer
	OBJ_HL_IO						= 100,
	OBJ_HL_CYLINDER,				//101
	OBJ_HL_VACUUM,					//102
	OBJ_HL_INDUCTION_MOTOR,			//103
	OBJ_HL_VISION,					//104
	OBJ_HL_MOTION,					//105
	OBJ_HL_MOTION_LIB,				//106
	OBJ_HL_SERCOS_MOTION,			//107
	OBJ_HL_SERIAL,					//108
	OBJ_HL_MELSEC,					//109
	OBJ_HL_ETHERNET,				//110
	OBJ_HL_DACONVERTER,				//111
	OBJ_HL_SPARE,					//112..syr..SESL 090610 사용하지 않음
	OBJ_HL_LIGHTING,				//113
	OBJ_HL_SENSORCOMM,				//114
	OBJ_HL_MULTI_MOTION,			//115
	OBJ_HL_INVERTER,				//116
	OBJ_HL_ACS_MOTION,				//117
	OBJ_HL_SERCOS_SETUP,			//118
	OBJ_HL_CAN_MOTION,				//119
	OBJ_HL_CAN_SETUP,				//120
	
	// Mechanical Layer - Common
	OBJ_ML_OP_PANEL					= 200,
	OBJ_ML_HEATER_CONTROLLER,		//201
	OBJ_ML_ONLINE,					//202
	OBJ_ML_M2M_INTERFACE,			//203
	OBJ_ML_IO_INTERFACE,			//204
	OBJ_ML_LIGHTENING,				//205
	OBJ_ML_RFID,					//206
		
	OBJ_ML_PANEL_ALIGNER			= 210,	//Common 변경금지
	OBJ_ML_HANDLER,					//211
	OBJ_ML_MAINTOOL,				//212
	OBJ_ML_WORK_BENCH,				//213
	OBJ_ML_CAMERA_CARRIER,			//214
	OBJ_ML_TAB_FEEDER,				//215
	OBJ_ML_INDEX,					//216
	OBJ_ML_TAB_CARRIER,				//217
	OBJ_ML_MONITOUCH,				//218
	OBJ_ML_ACF_TOOL,				//219
	OBJ_ML_ACF_CARRIER,				//220
	OBJ_ML_ACF_REEL,				//221
	OBJ_ML_BACKUP,					//222
	OBJ_ML_TAB_DETECTOR,			//223
	OBJ_ML_MONITOUCH_SWITCH,		//224
	OBJ_ML_TAB_LOADER,				//225
	OBJ_ML_ROLLER_CONVEYOR			= 226,	//Common 변경금지
	OBJ_ML_PANEL_TRANSFER			= 227,	//Common 변경금지
	OBJ_ML_TAB_MOUNTER,				//228
	OBJ_ML_TAB_ALIGNER,				//229
	OBJ_ML_MODEL_CHANGE				= 230,	//Common 변경금지

	OBJ_ML_PCB_LOADER				= 240,
	OBJ_ML_PCB_TRANSFER,			//241
	OBJ_ML_PCB_HANDLER,				//242
	OBJ_ML_TRAY_LOADER,				//243
	OBJ_ML_TRAY_UNLOADER,			//244
	OBJ_ML_TRAY_NTTU,				//245
	OBJ_ML_TRAY_ETTU,				//246

	// Control Layer - Common
	OBJ_CL_MANAGE_OP_PANEL			= 300,
	OBJ_CL_TEACHING,				//301
	OBJ_CL_VISION_CALIBRATION,		//302
	OBJ_CL_EXCHANGE_SHEET,			//303

	OBJ_CL_PANEL_ALIGNER			= 310,	//Common 변경금지
	OBJ_CL_HANDLER,					//311
	OBJ_CL_MAIN_TOOL,				//312
	OBJ_CL_WORK_BENCH,				//313
	OBJ_CL_CAMERA_CARRIER,			//314
	OBJ_CL_TAB_FEEDER,				//315
	OBJ_CL_INDEX,					//316
	OBJ_CL_TAB_CARRIER,				//317
	OBJ_CL_MONITOUCH,				//318
	OBJ_CL_ACF_TOOL,				//319
	OBJ_CL_ACF_CARRIER,				//320
	OBJ_CL_ACF_REEL,				//321
	OBJ_CL_TAB_LOADER,				//322
	OBJ_CL_TAB_MOUNTER,				//323
	OBJ_CL_INSPECTION_CAMERA,		//324

	OBJ_CL_PCB_LOADER				= 330,
	OBJ_CL_PCB_TRANSFER,			//331
	OBJ_CL_PCB_HANDLER,				//332
	OBJ_CL_TRAY_LOADER,				//333
	OBJ_CL_TRAY_UNLOADER,			//334
	OBJ_CL_NTTU,					//335
	OBJ_CL_ETTU,					//336


	// Process Layer - Common
	OBJ_PL_AUTO_MANAGER				= 400,
	OBJ_PL_JOG,						//401

	OBJ_PL_PANEL_ALIGNER			= 410,	//Common 변경금지
	OBJ_PL_HANDLER,					//411
	OBJ_PL_MAIN_TOOL,				//412
	OBJ_PL_WORK_BENCH,				//413
	OBJ_PL_CAMERA_CARRIER,			//414
	OBJ_PL_TAB_FEEDER,				//415
	OBJ_PL_INDEX,					//416
	OBJ_PL_TAB_CARRIER,				//417
	OBJ_PL_MONITOUCH_TENSION,		//418
	OBJ_PL_ACF_TOOL,				//419
	OBJ_PL_ACF_CARRIER,				//420
	OBJ_PL_ACF_REEL,				//421
	OBJ_PL_TABCARRIER_SCHEDULER,	//422
	OBJ_PL_PANEL_TRANSFER			= 424,	//Common 변경금지
	OBJ_PL_TAB_MOUNTER,				//425
	OBJ_PL_TAB_LOADER,				//426
	OBJ_PL_LOAD_CONVEYOR			= 427,	//Common 변경금지
	OBJ_PL_UNLOAD_CONVEYOR			= 428,	//Common 변경금지
//	OBJ_PL_OUT_CONVEYOR,			//429

	OBJ_PL_PCB_LOADER				= 430,	
	OBJ_PL_PCB_TRANSFER,			//431
	OBJ_PL_PCB_HANDLER,				//432
	OBJ_PL_TRAY_LOADER,				//433
	OBJ_PL_TRAY_UNLOADER,			//434
	OBJ_PL_NTTU,					//435
	OBJ_PL_ETTU,					//436
	OBJ_PL_TRANSFER_CENTER,			//437
	OBJ_PL_TRANSFER_OUT,			//438

	OBJ_PL_WORKSCHEDULE				= 440,
	
	// Common Layer
	OBJ_ETC_RESOURCE_MANAGER		= 600,

} EObjectType;

typedef	enum	enumLogCategory
{
	LOG_CATEGORY_TACT_TIME,
	LOG_CATEGORY_HARDWARE_LAYER,
	LOG_CATEGROY_MECHANICAL_LAYER,
	LOG_CATEGORY_CONTROL_LAYER,
	LOG_CATEGORY_PROCESS_LAYER
} eLogCategory;


typedef
/**
 * Object 정보 구조체 Define
 * @stereotype struct
 */
struct	tagSObjectInfo
{
	/** Object 종류 */
	int		m_iObjectType;	
	/** Object ID Base*/
	int		m_iObjectBase;
	/** Instance NO */
	int		m_iInstanceNo;
	/** Object 이름 */
	char	m_strObjectName[256];
	/** Object Log Path */
	char	m_strLogFileName[512];
	/** Common Interface Pointer */
	ICommon	*m_pObj;

} SObjectInfo;


/**
 * MSystemInfo Class Definition	
 */
class MSystemInfo
{
private:
	int		m_iIndexTable[DEF_MAX_OBJECT_NO];

	/** Number of component objects */
	int		m_iNumObjects;

public:    
	/**
	 * 생성자
	 * @stereotype constructor
	 */
    MSystemInfo();

	/**
	 * 소멸자
	 * @stereotype destructor
	 */
	virtual	~MSystemInfo();

	void	GetObjectAttributes(int iObjectID,
							int	iInstanceNo,
							int& iObjType, 
							CString& strObjName, 
							CString& strLogFileFullName);
	
	int		GetObjectInstanceNo(int iObjectID);
	int		GetObjectType( int iObjectID );
	CString GetObjectName(int iObjectID);
	int		GetObjectName(int iComponentCode, int iInstanceNo, CString* pstrName);

	CString GetLogFileName(int iObjectID);
	ICommon	*GetCommonObject( int iObjectID );
	SCommonAttribute GetCommonData(int iObjectID);
	
	void	SetAllObjsLogAttribute(BYTE ucLevel, int iKeepingDays);
	void	DeleteOldLogFilesAllObjs();

	int		GetNumObjects();
	int		GetObjectInform(SObjectInfo sInfoObject[DEF_MAX_OBJECT_INFO_NO]);

	int		RegisterComponent(int iObjectID, ICommon *plinkComp);
	void	MakeErrorMessageFile();

private:
	void	makeSystemInfo();
	void	makeIndexTable();
	int		getTableIndex(int iObjectID);

	/** Object 정보 */
	SObjectInfo	m_infoObject[DEF_MAX_OBJECT_NO]; 
};

#endif //MSYSTEMINFO_H
