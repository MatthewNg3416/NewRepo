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

#include "stdafx.h"

#include "MSystemInfo.h"
#include "DefSystem.h"
#include "DefActuatorPreBonder.h"
#include "DefAxisPreBonder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static SObjectInfo		s_SystemObjects[DEF_MAX_OBJECT_NO] = 
{
// (1)Object Type,			(2)Object Base								(3)Instance No										(4)Object Name,								(5)Log File Name,					(6)Common I/F Object Pt 
	{ OBJ_NONE,					0,										0,													"None",										"NULL",								NULL},
	{ OBJ_HL_IO,				DEF_IO_OBJECT_BASE,						0,													"Device Net",								"../Log/IO.log",					NULL},	
	{ OBJ_HL_MOTION_LIB,		DEF_MOTION_LIB_OBJECT_BASE,				0,													"Motion Board",								"../Log/MotionBoard.log",			NULL},
	{ OBJ_HL_SERCOS_SETUP,		DEF_SERCOS_SETUP_OBJECT_BASE,			0,													"Sercos Setup",								"../Log/SercosSetup.log",			NULL},
	{ OBJ_HL_CAN_SETUP,			DEF_CAN_SETUP_OBJECT_BASE,				0,													"Can Setup",								"../Log/CanSetup.log",				NULL},

	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					0,													"Vision",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					1,													"Camera1",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					2,													"Camera2",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					3,													"Camera3",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					4,													"Camera4",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					5,													"Camera5",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					6,													"Camera6",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					7,													"Camera7",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					8,													"Camera8",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					9,													"Camera9",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					10,													"Camera10",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					11,													"Camera11",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					12,													"Camera12",									"../Log/Vision.log",				NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					13,													"Camera13",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					14,													"Camera14",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					15,													"Camera15",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					16,													"Camera16",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					17,													"Camera17",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					18,													"Camera18",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					19,													"Camera19",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					20,													"Camera20",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					21,													"Camera21",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					22,													"Camera22",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					23,													"Camera23",									"../Log/Vision.log",				NULL},
	{ OBJ_HL_VISION,			DEF_VISION_OBJECT_BASE,					24,													"Camera24",									"../Log/Vision.log",				NULL},
#endif

	{ OBJ_HL_MELSEC,			DEF_MELSEC_OBJECT_BASE,					0,													"Melsec",									"../Log/Melsec.log",				NULL},
	{ OBJ_HL_MELSEC,			DEF_MELSEC_OBJECT_BASE,					1,													"Melsec2",									"../Log/Melsec.log",				NULL},

	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					0,													"Front Bar",								"../Log/Serial/Front Bar.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					1,													"Front RFID",								"../Log/Serial/Front RFID.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					2,													"Rear Bar",									"../Log/Serial/Rear Bar.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					3,													"Rear RFID",								"../Log/Serial/Rear RFID.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					4,													"Lighting 1",								"../Log/Serial/Lighting1.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					5,													"Lighting 2",								"../Log/Serial/Lighting1.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					6,													"Lighting 3",								"../Log/Serial/Lighting1.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					7,													"Lighting 4",								"../Log/Serial/Lighting1.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					8,													"Lighting 5",								"../Log/Serial/Lighting1.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					9,													"Lighting 6",								"../Log/Serial/Lighting1.log",		NULL},
	{ OBJ_HL_SERIAL,			DEF_SERIAL_OBJECT_BASE,					10,													"Lighting 7",								"../Log/Serial/Lighting1.log",		NULL},

#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PANEL_TRANSFER_IN_UD,						"Transfer In UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PANEL_TRANSFER_OUT_UD,						"Transfer Out UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER1_PRESS_UD,						"TabMounter1 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER2_PRESS_UD,						"TabMounter2 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER3_PRESS_UD,						"TabMounter3 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER4_PRESS_UD,						"TabMounter4 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABMOUNTER1_PRESS_UD,						"TabMounter5 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABMOUNTER2_PRESS_UD,						"TabMounter6 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABMOUNTER3_PRESS_UD,						"TabMounter7 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABMOUNTER4_PRESS_UD,						"TabMounter8 Press UD",						"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_INSPECTION_CAM1_ES,							"Inapection Cam1 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_INSPECTION_CAM2_ES,							"Inapection Cam2 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_INSPECTION_CAM3_ES,							"Inapection Cam3 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_INSPECTION_CAM4_ES,							"Inapection Cam4 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_INSPECTION_CAM1_ES,						"R Inapection Cam1 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_INSPECTION_CAM2_ES,						"R Inapection Cam2 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_INSPECTION_CAM3_ES,						"R Inapection Cam3 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_INSPECTION_CAM4_ES,						"R Inapection Cam4 ES",						"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABCARRIER_UD,								"TabCarrier UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABCARRIER_UD2,								"TabCarrier UD2",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABCARRIER_FB,								"TabCarrier FB",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PREALIGN_CAM_ES,							"Prealign Cam ES",							"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABCARRIER_UD,							"R TabCarrier1 UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABCARRIER_UD2,							"R TabCarrier1 UD2",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABCARRIER_FB,							"R TabCarrier2 UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_PREALIGN_CAM_ES,							"R Prealign Cam ES",						"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER1_PANEL_PUSHER_UD,				"TabMounter1 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER2_PANEL_PUSHER_UD,				"TabMounter2 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER3_PANEL_PUSHER_UD,				"TabMounter3 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER4_PANEL_PUSHER_UD,				"TabMounter4 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_RTABMOUNTER1_PANEL_PUSHER_UD,				"RTabMounter1 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_RTABMOUNTER2_PANEL_PUSHER_UD,				"RTabMounter2 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_RTABMOUNTER3_PANEL_PUSHER_UD,				"RTabMounter3 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_RTABMOUNTER4_PANEL_PUSHER_UD,				"RTabMounter4 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TRANSFER_IN_AIRRAIL_FB1,					"Transfer In AirRail FB1",					"../Log/Cylinders.log",				NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TRANSFER_IN_AIRRAIL_FB2,					"Transfer In AirRail FB2",					"../Log/Cylinders.log",				NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PANEL_TRANSFER_CENTER_UD,					"Transfer Center",							"../Log/Cylinders.log",					NULL },

	//150728_KDH DUMMY
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD,				"TabPress1 Recovery Sprocket UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABPRESS1_RECOVERY_SPROCKET_UD,			"R TabPress1 Recovery Sprocket UD",			"../Log/Cylinders.log",				NULL},
	//____________________

	/*/
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_PRESS_REEL_UD,					"TabPress1 Press Reel UD",					"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_PRESS_REEL_CLAMP_UD,				"TabPress1 Press Reel Clamp UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_UD,	"TabPress1 Attach Cutting Support Left UD",	"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_UD,	"TabPress1 Attach Cutting Support Right UD","../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_ATTACH_PRESS_UD,					"TabPress1 Attach Press UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_ATTACH_REEL_CUTTER_UD,			"TabPress1 Attach Reel Cutter UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_ATTACH_REEL_FB,					"TabPress1 Attach Reel FB",					"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_ATTACH_SUPPLIER_CHUCK_ES,			"TabPress1 Attach Supplier Chuck ES",		"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_RECOVERY_SPROCKET_UD,				"TabPress2 Recovery Sprocket UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_PRESS_REEL_UD,					"TabPress2 Press Reel UD",					"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_PRESS_REEL_CLAMP_UD,				"TabPress2 Press Reel Clamp UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_UD,	"TabPress2 Attach Cutting Support Left UD",	"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_UD,	"TabPress2 Attach Cutting Support Right UD","../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_ATTACH_PRESS_UD,					"TabPress2 Attach Press UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_ATTACH_REEL_CUTTER_UD,			"TabPress2 Attach Reel Cutter UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_ATTACH_REEL_FB,					"TabPress2 Attach Reel FB",					"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS2_ATTACH_SUPPLIER_CHUCK_ES,			"TabPress2 Attach Supplier Chuck ES",		"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TILTING_LU,									"Tilting LU",								"../Log/Cylinders.log",					NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TILTING_UD,									"Tilting UD",								"../Log/Cylinders.log",					NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PANEL_PUSHER_UD,							"PanelPusher UD",							"../Log/Cylinders.log",				NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_DUMP_BASKET_FRONT_FB,						"DumpBasket Front FB",						"../Log/Cylinders.log",				NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_DUMP_BASKET_REAR_FB,						"DumpBasket Rear FB",						"../Log/Cylinders.log",				NULL },
	/*/

#else		//#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PANEL_TRANSFER_OUT_UD,						"Transfer Out UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER1_PRESS_UD,						"TabMounter1 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER2_PRESS_UD,						"TabMounter2 Press UD",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABMOUNTER1_PRESS_UD,						"RTabMounter1 Press UD",					"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABMOUNTER2_PRESS_UD,						"RTabMounter2 Press UD",					"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_INSPECTION_CAM1_ES,							"Inapection Cam1 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_INSPECTION_CAM2_ES,							"Inapection Cam2 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_INSPECTION_CAM1_ES,						"R Inapection Cam1 ES",						"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_INSPECTION_CAM2_ES,						"R Inapection Cam2 ES",						"../Log/Cylinders.log",				NULL},
	
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABCARRIER_UD,								"TabCarrier1 UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABCARRIER_FB,								"TabCarrier2 UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD,				"TabPress1 Recovery Sprocket UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_PREALIGN_CAM_ES,							"Prealign Cam ES",							"../Log/Cylinders.log",				NULL},
	
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABCARRIER_UD,							"R TabCarrier1 UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABCARRIER_FB,							"R TabCarrier2 UD",							"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_TABPRESS1_RECOVERY_SPROCKET_UD,			"R TabPress1 Recovery Sprocket UD",			"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_R_PREALIGN_CAM_ES,							"R Prealign Cam ES",						"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER1_PANEL_PUSHER_UD,				"TabMounter1 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_TABMOUNTER2_PANEL_PUSHER_UD,				"TabMounter2 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_RTABMOUNTER1_PANEL_PUSHER_UD,				"RTabMounter1 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_RTABMOUNTER2_PANEL_PUSHER_UD,				"RTabMounter2 Panel Pusher UD",				"../Log/Cylinders.log",				NULL},

	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_MODELCHANGE_AIRRAIL_FB1,					"ModelChange AirRail FB1",					"../Log/Cylinders.log",				NULL },
	{ OBJ_HL_CYLINDER,			DEF_CYLINDER_OBJECT_BASE,				DEF_CYL_MODELCHANGE_AIRRAIL_FB2,					"ModelChange AirRail FB2",					"../Log/Cylinders.log",				NULL },
#endif

#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ALIGNER1,							"Panel Aligner Vac1",						"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ALIGNER2,							"Panel Aligner Vac2",						"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ALIGNER3,							"Panel Aligner Vac3",						"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_IN,						"Panel Transfer In Vac",					"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_IN2,						"Panel Transfer In2 Vac",					"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_OUT,						"Panel Transfer Out Vac",					"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_OUT2,					"Panel Transfer Out2 Vac",					"../Log/Vacuum.log",					NULL},		

	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_MOUNTER1,							"TabMounter1 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_MOUNTER2,							"TabMounter2 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_MOUNTER3,							"TabMounter3 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_MOUNTER4,							"TabMounter4 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TAB_MOUNTER1,							"RTabMounter1 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TAB_MOUNTER2,							"RTabMounter2 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TAB_MOUNTER3,							"RTabMounter3 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TAB_MOUNTER4,							"RTabMounter4 Vac",							"../Log/Vacuum.log",					NULL},

	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TABCARRIER1,							"TabCarrier1 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TABCARRIER2,							"TabCarrier2 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TABCARRIER3,							"TabCarrier3 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TABCARRIER4,							"TabCarrier4 Vac",							"../Log/Vacuum.log",					NULL},
	
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TABCARRIER1,							"R TabCarrier1 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TABCARRIER2,							"R TabCarrier2 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TABCARRIER3,							"R TabCarrier3 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TABCARRIER4,							"R TabCarrier4 Vac",						"../Log/Vacuum.log",					NULL},
		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_CENTER,					"PanelTransfer Center Vac",					"../Log/Vacuum.log",					NULL },
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_CENTER2,					"PanelTransfer Center Vac2",					"../Log/Vacuum.log",					NULL },
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TLHANDLER_1,							"THandler Vac1",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TLHANDLER_2,							"THandler Vac2",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TLHANDLER_3,							"THandler Vac3",							"../Log/Vacuum.log",					NULL},


	//150728_KDH
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS1,								"TabPress1 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS2,								"TabPress2 Vac",							"../Log/Vacuum.log",					NULL},
	//______________

	/*/
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS1_NEW_REEL,					"TabPress1 New Reel Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS2_NEW_REEL,					"TabPress2 New Reel Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_CENTER,					"Panel Transfer Center Vac",							"../Log/Vacuum.log",					NULL},
	/*/
#else		//#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ALIGNER1,							"Panel Aligner Vac1",						"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ALIGNER2,							"Panel Aligner Vac2",						"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ALIGNER3,							"Panel Aligner Vac3",						"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_OUT,						"Panel Transfer Out Vac",					"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_TRANSFER_OUT2,					"Panel Transfer Out2 Vac",					"../Log/Vacuum.log",					NULL},
	
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_MOUNTER1,							"TabMounter1 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_MOUNTER2,							"TabMounter2 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TAB_MOUNTER1,							"RTabMounter1 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TAB_MOUNTER2,							"RTabMounter2 Vac",							"../Log/Vacuum.log",					NULL},

	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TABCARRIER1,							"TabCarrier1 Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TABCARRIER2,							"TabCarrier2 Vac",							"../Log/Vacuum.log",					NULL},	
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TABCARRIER1,							"R TabCarrier1 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_R_TABCARRIER2,							"R TabCarrier2 Vac",							"../Log/Vacuum.log",					NULL},		

	//150728_KDH
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS1,								"TabPress1 Vac",							"../Log/Vacuum.log",					NULL},		
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS2,								"TabPress2 Vac",							"../Log/Vacuum.log",					NULL},
	//_________________
	/*/
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS1_NEW_REEL,					"TabPress1 New Reel Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_TAB_PRESS2_NEW_REEL,					"TabPress2 New Reel Vac",							"../Log/Vacuum.log",					NULL},
	{ OBJ_HL_VACUUM,			DEF_VACUUM_OBJECT_BASE,					DEF_VAC_PANEL_ROTATE,							"Panel Rotate Vac",							"../Log/Vacuum.log",					NULL},
	/*/
#endif

#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_Y				,						"PanelStage Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_T				,						"PanelStage T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_X				,						"PanelStage X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_Z				,						"PanelStage Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_IN			,						"Transfer In X",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_OUT		,						"Transfer Out X",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MODEL_CHANGE_1		,						"Model Change F",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MODEL_CHANGE_2		,						"Model Change R",				"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER1_X			,						"Mounter1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER2_X			,						"Mounter2 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER3_X			,						"Mounter3 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER4_X			,						"Mounter4 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER1_Z			,						"Mounter1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER2_Z			,						"Mounter2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER3_Z			,						"Mounter3 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER4_Z			,						"Mounter4 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BACKUP1_Z			,						"Backup Z1",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BACKUP2_Z			,						"Backup Z2",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BACKUP3_Z			,						"Backup Z3",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BACKUP4_Z			,						"Backup Z4",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X,						"Inspection Camera1 X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X,						"Inspection Camera2 X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X,						"Inspection Camera3 X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X,						"Inspection Camera4 X",			"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER1_X		,						"RMounter1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER2_X		,						"RMounter2 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER3_X		,						"RMounter3 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER4_X		,						"RMounter4 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER1_Z		,						"RMounter1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER2_Z		,						"RMounter2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER3_Z		,						"RMounter3 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER4_Z		,						"RMounter4 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BACKUP1_Z			,						"RBackup1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BACKUP2_Z			,						"RBackup2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BACKUP3_Z			,						"RBackup3 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BACKUP4_Z			,						"RBackup4 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X,						"RInspection Camera1 X",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X,						"RInspection Camera2 X",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X,						"RInspection Camera3 X",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X,						"RInspection Camera4 X",		"../Log/MultiMotion.log",			NULL},		




	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER1_Y	,						"Carrier1 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER2_Y	,						"Carrier2 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER3_Y	,						"Carrier3 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER4_Y	,						"Carrier4 Y",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER1_X	,						"Carrier1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER2_X	,						"Carrier2 X",					"../Log/MultiMotion.log",			NULL},		  //SJ_YYK 150109 Modify..
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER3_X	,						"Carrier3 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER4_X	,						"Carrier4 X",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER1_T	,						"Carrier1 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER2_T	,						"Carrier2 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER3_T	,						"Carrier3 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER4_T	,						"Carrier4 T",					"../Log/MultiMotion.log",			NULL},		
	
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECT_EXPAND1		,						"Inspection Cam1 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECT_EXPAND2		,						"Inspection Cam2 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECT_EXPAND3		,						"Inspection Cam3 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECT_EXPAND4		,						"Inspection Cam4 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABCAMERA_EXPAND	,						"Carrier Expand",				"../Log/MultiMotion.log",			NULL},		
	
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y	,						"R Carrier1 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y	,						"R Carrier2 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y	,						"R Carrier3 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y	,						"R Carrier4 Y",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X	,						"R Carrier1 X",					"../Log/MultiMotion.log",			NULL},			
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X	,						"R Carrier2 X",					"../Log/MultiMotion.log",			NULL},		 //SJ_YYK 150109 Modify..
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X	,						"R Carrier3 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X	,						"R Carrier4 X",					"../Log/MultiMotion.log",			NULL},			

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T	,						"R Carrier1 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T	,						"R Carrier2 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T	,						"R Carrier3 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T	,						"R Carrier4 T",					"../Log/MultiMotion.log",			NULL},
	
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECT_EXPAND1	,						"R Inspection Cam1 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECT_EXPAND2	,						"R Inspection Cam2 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECT_EXPAND3	,						"R Inspection Cam3 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECT_EXPAND4	,						"R Inspection Cam4 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND	,						"R Carrier Expand",				"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABCAMERA_EXPAND2	,						"Carrier Expand2",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2	,						"R Carrier Expand2",				"../Log/MultiMotion.log",			NULL},		

	//SJ_YYK 161104 Add.
#ifdef DEF_USE_TRANSFER_CENTER
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_CENTER_1	,						"Transfer Center1",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_CENTER_2	,						"Transfer Center2",				"../Log/MultiMotion.log",			NULL},
#else
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_SPARE_76			,						"Transfer Center1",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_SPARE_77			,						"Transfer Center2",				"../Log/MultiMotion.log",			NULL},
#endif
#ifdef DEF_USE_TURN_HANDLER
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_HANDLER_T	,								"Turn Handler T",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_HANDLER_Z	,								"Turn Handler Z",				"../Log/MultiMotion.log",			NULL},
#else
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_SPARE_78	,								"Spare 78",						"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_SPARE_79	,								"Spare 79",						"../Log/MultiMotion.log",			NULL},
#endif
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_IN_2	,							"Transfer In2",					"../Log/MultiMotion.log",			NULL},	
	//_____________*/

	{ OBJ_HL_ACS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_ACS_GANTRY_Y,										"Gantry Y",						"../Log/Motion.log",					NULL},
	{ OBJ_HL_ACS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_ACS_R_GANTRY_Y,									"R Gantry Y",					"../Log/Motion.log",					NULL},

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_OUT_2	,							"Transfer Out2",				"../Log/MultiMotion.log",			NULL},	
#else
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_SPARE_81	,								"Spare 83",						"../Log/MultiMotion.log",			NULL},	
#endif
	//150728_KDH DUMMY

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X	,						"R BadTab Detector X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T,						"R Sprocket1 T",				"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T,						"R Sprocket2 T",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_TAB_PRESS_Z		,						"R Tab Press Z",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_FEEDER_REEL_T		,						"R FeederReelT",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z	,						"R MoldUpDown Z",				"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BADTAB_DETECTOR_X	,						"BadTab Detector X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T	,						"Sprocket1 T",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T	,						"Sprocket2 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TAB_PRESS_Z			,						"Tab Press Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_FEEDER_REEL_T		,						"FeederReelT",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z		,					"MoldUpDown Z",					"../Log/MultiMotion.log",			NULL},		
	//_____________

	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_STAGE_YT,								"PanelAligner YT",				"../Log/MultiMotion.log",			NULL},	
#ifdef DEF_USE_TRANSFER_CENTER
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER,						"Panel Transfer Center X",		"../Log/MultiMotion.log",			NULL},	//SJ_YYK 161104 Add.
#else
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_SPARE_1,									"Spare1",						"../Log/MultiMotion.log",			NULL},	//SJ_YYK 161104 Add.
#endif

	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_TRANFSER_IN,							"PanelTransfer X",				"../Log/MultiMotion.log",			NULL},	//SJ_YYK 161104 Add.	

	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_CARRIER_XZ,							"Carrier XZ",					"../Log/MultiMotion.log",			NULL},		//SJ_YYK 150212 Add..
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_CARRIER_R_XZ,							"Carreir R XZ",					"../Log/MultiMotion.log",			NULL},		
#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT,							"Panel Transfer Out X",			"../Log/MultiMotion.log",			NULL},	//SJ_YYK 161104 Add.
#else
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_SPARE_5,									"Spare5",						"../Log/MultiMotion.log",			NULL},	//SJ_YYK 161104 Add.
#endif

	//150728_KDH
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_FRONT_SPROCKET,						"Front Sprocket T",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_REAR_SPROCKET,						"Rear Sprocket T",				"../Log/MultiMotion.log",			NULL},	
	//____________

//@	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_TRANSFER,								"PanelTransfer X",				"../Log/MultiMotion.log",			NULL},		
//@	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_TRANSFER_OUT,							"PanelTransfer Out X",			"../Log/MultiMotion.log",			NULL},		


	//______________
#	else		//#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_Y				,						"PanelStage Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_T				,						"PanelStage T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_X				,						"PanelStage X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_STAGE_Z				,						"PanelStage Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TRANSFER_OUT		,						"Transfer Out X",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MODEL_CHANGE_1		,						"Model Change F",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MODEL_CHANGE_2		,						"Model Change R",				"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER1_X			,						"Mounter1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER2_X			,						"Mounter2 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER1_Z			,						"Mounter1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOUNTER2_Z			,						"Mounter2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BACKUP1_Z			,						"Backup1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BACKUP2_Z			,						"Backup2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X,						"Inspection Camera1 X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X,						"Inspection Camera2 X",			"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER1_X		,						"RMounter1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER2_X		,						"RMounter2 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER1_Z		,						"RMounter1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOUNTER2_Z		,						"RMounter2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BACKUP1_Z			,						"RBackup1 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BACKUP2_Z			,						"RBackup2 Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X,						"RInspection Camera1 X",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X,						"RInspection Camera2 X",		"../Log/MultiMotion.log",			NULL},		




	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER1_Y	,						"Carrier1 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER2_Y	,						"Carrier2 Y",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER1_X	,						"Carrier1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER2_X	,						"Carrier2 X",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER1_T	,						"Carrier1 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_CARRIER2_T	,						"Carrier2 T",					"../Log/MultiMotion.log",			NULL},		




	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y	,						"R Carrier1 Y",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y	,						"R Carrier2 Y",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X	,						"R Carrier1 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X	,						"R Carrier2 X",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T	,						"R Carrier1 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T	,						"R Carrier2 T",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECT_EXPAND1		,						"Inspection Cam1 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_INSPECT_EXPAND2		,						"Inspection Cam2 Expand",		"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECT_EXPAND1	,						"R Inspection Cam1 Expand",		"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_INSPECT_EXPAND2	,						"R Inspection Cam2 Expand",		"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_ACS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_ACS_GANTRY_Y,										"Gantry Y",									"../Log/Motion.log",					NULL},
	{ OBJ_HL_ACS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_ACS_R_GANTRY_Y,									"R Gantry Y",								"../Log/Motion.log",					NULL},

	//150727_KDH
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_BADTAB_DETECTOR_X	,						"BadTab Detector X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T	,						"Sprocket1 T",					"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T	,						"Sprocket2 T",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TAB_PRESS_Z			,						"Tab Press Z",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_FEEDER_REEL_T		,						"FeederReelT",					"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z		,					"MoldUpDown Z",					"../Log/MultiMotion.log",			NULL},		


	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X	,						"R BadTab Detector X",			"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T,						"R Sprocket1 T",				"../Log/MultiMotion.log",			NULL},		

	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T,						"R Sprocket2 T",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_TAB_PRESS_Z		,						"R Tab Press Z",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_FEEDER_REEL_T		,						"R FeederReelT",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z	,						"R MoldUpDown Z",				"../Log/MultiMotion.log",			NULL},		


	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_TABCAMERA_EXPAND	,						"Carrier Expand",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MOTION_OBJECT_BASE,			DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND	,						"R Carrier Expand",				"../Log/MultiMotion.log",			NULL},		
	//________

	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_STAGE_YT,								"PanelAligner YT",				"../Log/MultiMotion.log",			NULL},	
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_FRONT_SPROCKET,						"Front Sprocket T",				"../Log/MultiMotion.log",			NULL},		
	{ OBJ_HL_SERCOS_MOTION,		DEF_MULTI_MOTION_OBJECT_BASE,	DEF_AXIS_SERCOS_MULTI_REAR_SPROCKET,						"Rear Sprocket T",				"../Log/MultiMotion.log",			NULL},	
#endif


	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_1,										"Lighting 1",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_2,										"Lighting 2",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_3,										"Lighting 3",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_4,										"Lighting 4",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_5,										"Lighting 5",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_6,										"Lighting 6",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_7,										"Lighting 7",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_8,										"Lighting 8",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_9,										"Lighting 9",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_10,									"Lighting 10",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_11,									"Lighting 11",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_12,									"Lighting 12",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_13,									"Lighting 13",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_14,									"Lighting 14",								"../Log/Lighting.log",				NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_15,									"Lighting 15",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_16,									"Lighting 16",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_17,									"Lighting 17",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_18,									"Lighting 18",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_19,									"Lighting 19",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_20,									"Lighting 20",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_21,									"Lighting 21",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_22,									"Lighting 22",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_23,									"Lighting 23",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_24,									"Lighting 24",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_25,									"Lighting 25",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_26,									"Lighting 26",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_27,									"Lighting 27",								"../Log/Lighting.log",				NULL},
	{ OBJ_HL_LIGHTING,			DEF_LIGHTING_OBJECT_BASE,				DEF_LIGHTING_28,									"Lighting 28",								"../Log/Lighting.log",				NULL},
#endif

//@	{ OBJ_HL_INVERTER,			DEF_INVERTER_OBJECT_BASE,				DEF_LOAD_CONVEYOR,									"Load Conveyor Inverter",					"../Log/Inverter.log",				NULL},
//@	{ OBJ_HL_INVERTER,			DEF_INVERTER_OBJECT_BASE,				DEF_UNLOAD_CONVEYOR,								"Unload Conveyor Inverter",					"../Log/Inverter.log",				NULL},
					
	{ OBJ_ML_OP_PANEL,			DEF_OPPANEL_OBJECT_BASE,				0,													"Op Panel",									"../Log/OpPanel.log",				NULL},

	{ OBJ_ML_ONLINE,			DEF_ONLINE_OBJECT_BASE,					0,													"OnLine",									"../Log/OnLine.log",					NULL},
					
	{ OBJ_ML_PANEL_ALIGNER,		DEF_PANEL_ALIGNER_OBJECT_BASE,			0,													"PanelAligner",								"../Log/PanelAligner.log",			NULL},
//@	{ OBJ_ML_ROLLER_CONVEYOR,	DEF_ROLLERCONVEYOR_OBJECT_BASE,			DEF_LOAD_CONVEYOR,									"Load Roller Conveyor",						"../Log/RollerConveyor.log",			NULL},
//@	{ OBJ_ML_ROLLER_CONVEYOR,	DEF_ROLLERCONVEYOR_OBJECT_BASE,			DEF_UNLOAD_CONVEYOR,								"Unload Roller Conveyor",					"../Log/RollerConveyor.log",			NULL},

	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_1,									"Front Inspection Camera1 Carrier",			"../Log/CameraCarrier.log",			NULL},
	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_2,									"Front Inspection Camera2 Carrier",			"../Log/CameraCarrier.log",			NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_3,									"Front Inspection Camera3 Carrier",			"../Log/CameraCarrier.log",			NULL},
	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_4,									"Front Inspection Camera4 Carrier",			"../Log/CameraCarrier.log",			NULL},
#endif

	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_1+DEF_MAX_INSPECTION_CAM_CARRIER,	"Rear Inspection Camera1 Carrier",			"../Log/CameraCarrier.log",			NULL},
	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_2+DEF_MAX_INSPECTION_CAM_CARRIER,	"Rear Inspection Camera2 Carrier",			"../Log/CameraCarrier.log",			NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_3+DEF_MAX_INSPECTION_CAM_CARRIER,	"Rear Inspection Camera3 Carrier",			"../Log/CameraCarrier.log",			NULL},
	{ OBJ_ML_CAMERA_CARRIER,	DEF_CAMERA_CARRIER_OBJECT_BASE,			DEF_INSPECTION_CARRIER_4+DEF_MAX_INSPECTION_CAM_CARRIER,	"Rear Inspection Camera4 Carrier",			"../Log/CameraCarrier.log",			NULL},
#endif

	{ OBJ_ML_TAB_FEEDER,		DEF_TABFEEDER_OBJECT_BASE,				DEF_TABFEEDER1,										"Tab Feeder1",								"../Log/TabFeeder.log",				NULL},
	{ OBJ_ML_TAB_FEEDER,		DEF_TABFEEDER_OBJECT_BASE,				DEF_TABFEEDER2,										"Tab Feeder2",								"../Log/TabFeeder.log",				NULL},

	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER1,									"Tab Carrier1",								"../Log/TabCarrier.log",				NULL},
	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER2,									"Tab Carrier2",								"../Log/TabCarrier.log",				NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER3,									"Tab Carrier3",								"../Log/TabCarrier.log",				NULL},
	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER4,									"Tab Carrier4",								"../Log/TabCarrier.log",				NULL},
#endif

	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER1+DEF_MAX_TABCARRIER,					"R Tab Carrier1",							"../Log/TabCarrier.log",				NULL},
	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER2+DEF_MAX_TABCARRIER,					"R Tab Carrier2",							"../Log/TabCarrier.log",				NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER3+DEF_MAX_TABCARRIER,					"R Tab Carrier3",							"../Log/TabCarrier.log",				NULL},
	{ OBJ_ML_TAB_CARRIER,		DEF_TABCARRIER_OBJECT_BASE,				DEF_TABCARRIER4+DEF_MAX_TABCARRIER,					"R Tab Carrier4",							"../Log/TabCarrier.log",				NULL},
#endif

//@	{ OBJ_ML_PANEL_TRANSFER,	DEF_PANEL_TRANSFER_OBJECT_BASE,			0,													"PanelTransfer",							"../Log/PanelTransfer.log",			NULL},
	{ OBJ_ML_PANEL_TRANSFER,	DEF_PANEL_TRANSFER_OBJECT_BASE,			DEF_PANEL_TRANSFER_CENTER,							"PanelTransferCenter",							"../Log/PanelTransfer.log",			NULL},
	{ OBJ_ML_PANEL_TRANSFER,	DEF_PANEL_TRANSFER_OBJECT_BASE,			DEF_PANEL_TRANSFER_IN,								"PanelTransferIn",							"../Log/PanelTransfer.log",			NULL},
	{ OBJ_ML_PANEL_TRANSFER,	DEF_PANEL_TRANSFER_OBJECT_BASE,			DEF_PANEL_TRANSFER_OUT,								"PanelTransferOut",							"../Log/PanelTransfer.log",			NULL},

	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER1,									"TabMounter1",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER2,									"TabMounter2",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER3,									"TabMounter3",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER4,									"TabMounter4",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER5,									"TabMounter5",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER6,									"TabMounter6",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER7,									"TabMounter7",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER8,									"TabMounter8",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER9,									"TabMounter9",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER10,									"TabMounter10",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER11,									"TabMounter11",								"../Log/TabMounter.log",				NULL},
	{ OBJ_ML_TAB_MOUNTER,		DEF_TAB_MOUNTER_OBJECT_BASE,			DEF_TABMOUNTER12,									"TabMounter12",								"../Log/TabMounter.log",				NULL},

	{ OBJ_ML_TAB_ALIGNER,		DEF_TAB_ALIGNER_OBJECT_BASE,			DEF_TABALIGNER1,									"Aligner1",									"../Log/Aligner.log",				NULL},
	{ OBJ_ML_TAB_ALIGNER,		DEF_TAB_ALIGNER_OBJECT_BASE,			DEF_TABALIGNER2,									"Aligner2",									"../Log/Aligner.log",				NULL},
	{ OBJ_ML_TAB_ALIGNER,		DEF_TAB_ALIGNER_OBJECT_BASE,			DEF_TABALIGNER3,									"Aligner3",									"../Log/Aligner.log",				NULL},
	{ OBJ_ML_TAB_ALIGNER,		DEF_TAB_ALIGNER_OBJECT_BASE,			DEF_TABALIGNER4,									"Aligner4",									"../Log/Aligner.log",				NULL},

	{ OBJ_ML_MODEL_CHANGE,		DEF_MODEL_CHANGE_OBJECT_BASE,			0,													"F ModelChange",							"../Log/ModelChange.log",			NULL},
	{ OBJ_ML_MODEL_CHANGE,		DEF_MODEL_CHANGE_OBJECT_BASE,			1,													"R ModelChange",							"../Log/ModelChange.log",			NULL},

	{ OBJ_ML_RFID,				DEF_RFID_OBJECT_BASE,					0,													"FrontRFID",								"../Log/FrontRFID.log",			NULL},
	{ OBJ_ML_RFID,				DEF_RFID_OBJECT_BASE,					1,													"RearRFID",									"../Log/RearRFID.log",			NULL},

	 //SJ_YYK 161104 Add.
	{ OBJ_ML_HANDLER,			DEF_HANDLER_OBJECT_BASE,				0,													"Load Handler",								"../Log/LoadHandler.log",				NULL },
	{ OBJ_ML_HANDLER,			DEF_HANDLER_OBJECT_BASE,				1,													"Turn Handler",								"../Log/TurnHandler.log",				NULL },	
						
	{ OBJ_CL_MANAGE_OP_PANEL,	DEF_CTR_MANAGE_OPPANEL_OBJECT_BASE,		0,													"Manage OpPanel",							"../Log/ManageOpPanel.log",			NULL},
	{ OBJ_CL_TEACHING,			DEF_CTR_TEACHING_OBJECT_BASE,			0,													"Teaching",									"../Log/Teaching.log",				NULL},
	{ OBJ_CL_VISION_CALIBRATION,DEF_CTR_VISION_CALIBRATION_OBJECT_BASE,	0,													"Vision Calibration",						"../Log/VisionCalibration1.log",		NULL},
	{ OBJ_CL_PANEL_ALIGNER,		DEF_CTR_PANEL_ALIGNER_OBJECT_BASE,		0,													"CtrlPanelAligner",							"../Log/CtrlPanelAligner.log",		NULL},

	{ OBJ_CL_TAB_FEEDER,		DEF_CTR_TAB_FEEDER_OBJECT_BASE,			DEF_TABFEEDER1,										"CtrlTabFeeder1",							"../Log/CtrlTabFeeder.log",			NULL},
	{ OBJ_CL_TAB_FEEDER,		DEF_CTR_TAB_FEEDER_OBJECT_BASE,			DEF_TABFEEDER2,										"CtrlTabFeeder2",							"../Log/CtrlTabFeeder.log",			NULL},

	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER1,									"CtrlTabCarrier1",							"../Log/CtrlTabCarrier.log",			NULL},
	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER2,									"CtrlTabCarrier2",							"../Log/CtrlTabCarrier.log",			NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER3,									"CtrlTabCarrier3",							"../Log/CtrlTabCarrier.log",			NULL},
	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER4,									"CtrlTabCarrier4",							"../Log/CtrlTabCarrier.log",			NULL},
#endif

	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER1+DEF_MAX_TABCARRIER,					"CtrlRTabCarrier1",							"../Log/CtrlTabCarrier.log",			NULL},
	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER2+DEF_MAX_TABCARRIER,					"CtrlRTabCarrier2",							"../Log/CtrlTabCarrier.log",			NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER3+DEF_MAX_TABCARRIER,					"CtrlRTabCarrier3",							"../Log/CtrlTabCarrier.log",			NULL},
	{ OBJ_CL_TAB_CARRIER,		DEF_CTR_TAB_CARRIER_OBJECT_BASE,		DEF_TABCARRIER4+DEF_MAX_TABCARRIER,					"CtrlRTabCarrier4",							"../Log/CtrlTabCarrier.log",			NULL},
#endif

	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER1,									"CtrlTabMounter1",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER2,									"CtrlTabMounter2",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER3,									"CtrlTabMounter3",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER4,									"CtrlTabMounter4",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER5,									"CtrlTabMounter5",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER6,									"CtrlTabMounter6",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER7,									"CtrlTabMounter7",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER8,									"CtrlTabMounter8",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER9,									"CtrlTabMounter9",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER10,									"CtrlTabMounter10",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER11,									"CtrlTabMounter11",							"../Log/CtrlTabMounter.log",			NULL},
	{ OBJ_CL_TAB_MOUNTER,		DEF_CTR_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER12,									"CtrlTabMounter12",							"../Log/CtrlTabMounter.log",			NULL},

	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_1,								"CtrlInspectionCamera1",					"../Log/CtrlInspectionCamera.log",	NULL},
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_2,								"CtrlInspectionCamera2",					"../Log/CtrlInspectionCamera.log",	NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_3,								"CtrlInspectionCamera3",					"../Log/CtrlInspectionCamera.log",	NULL},
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_4,								"CtrlInspectionCamera4",					"../Log/CtrlInspectionCamera.log",	NULL},
#endif
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_1+DEF_MAX_INSPECTION_CAM_CARRIER,"CtrlInspectionCamera5",					"../Log/CtrlInspectionCamera.log",	NULL},
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_2+DEF_MAX_INSPECTION_CAM_CARRIER,"CtrlInspectionCamera6",					"../Log/CtrlInspectionCamera.log",	NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_3+DEF_MAX_INSPECTION_CAM_CARRIER,"CtrlInspectionCamera7",					"../Log/CtrlInspectionCamera.log",	NULL},
	{ OBJ_CL_INSPECTION_CAMERA,	DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_4+DEF_MAX_INSPECTION_CAM_CARRIER,"CtrlInspectionCamera8",					"../Log/CtrlInspectionCamera.log",	NULL},
#endif

	{ OBJ_CL_HANDLER,			DEF_CTR_HANDLER_OBJECT_BASE,			0,													"CtrlTurnHandler",						"../Log/CtrlTurnHandler.log",			NULL }, //SJ_YYK 161104 Add.
	
	{ OBJ_PL_AUTO_MANAGER,		DEF_TRS_AUTOMANAGER_OBJECT_BASE,		0,													"AutoManager",								"../Log/TrsAutoManage.log",			NULL},
	{ OBJ_PL_JOG,				DEF_TRS_JOG_OBJECT_BASE,				0,													"Jog",										"../Log/TrsJog.log",					NULL},
	{ OBJ_PL_PANEL_ALIGNER,		DEF_TRS_PANEL_ALIGNER_OBJECT_BASE,		0,													"TrsPanelAligner",							"../Log/TrsPanelAligner.log",		NULL},
	{ OBJ_PL_LOAD_CONVEYOR,		DEF_TRS_LOAD_CONVEYOR_OBJECT_BASE,		0,													"TrsLoadConveyor",							"../Log/TrsLoadConveyor.log",		NULL},
	{ OBJ_PL_UNLOAD_CONVEYOR,	DEF_TRS_UNLOAD_CONVEYOR_OBJECT_BASE,	0,													"TrsUnloadConveyor",						"../Log/TrsUnloadConveyor.log",		NULL},
	{ OBJ_PL_TAB_FEEDER,		DEF_TRS_TABFEEDER_OBJECT_BASE,			DEF_TABFEEDER1,										"TrsTabFeeder1",							"../Log/TrsTabFeeder.log",			NULL},
	{ OBJ_PL_TAB_FEEDER,		DEF_TRS_TABFEEDER_OBJECT_BASE,			DEF_TABFEEDER2,										"TrsTabFeeder2",							"../Log/TrsTabFeeder.log",			NULL},
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER1,									"TrsTabCarrier1",							"../Log/TrsTabCarrier.log",			NULL},
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER2,									"TrsTabCarrier2",							"../Log/TrsTabCarrier.log",			NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER3,									"TrsTabCarrier3",							"../Log/TrsTabCarrier.log",			NULL},
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER4,									"TrsTabCarrier4",							"../Log/TrsTabCarrier.log",			NULL},
#endif
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER1+DEF_MAX_TABCARRIER,					"TrsRTabCarrier1",							"../Log/TrsTabCarrier.log",			NULL},
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER2+DEF_MAX_TABCARRIER,					"TrsRTabCarrier2",							"../Log/TrsTabCarrier.log",			NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER3+DEF_MAX_TABCARRIER,					"TrsRTabCarrier3",							"../Log/TrsTabCarrier.log",			NULL},
	{ OBJ_PL_TAB_CARRIER,		DEF_TRS_TABCARRIER_OBJECT_BASE,			DEF_TABCARRIER4+DEF_MAX_TABCARRIER,					"TrsRTabCarrier4",							"../Log/TrsTabCarrier.log",			NULL},
#endif

	{ OBJ_PL_TRANSFER_CENTER,	DEF_TRS_PANEL_TRANSFER_OBJECT_BASE,		0,													"TrsPanelTransferCenter",							"../Log/TrsPanelTransfer.log",		NULL},
	{ OBJ_PL_PANEL_TRANSFER,	DEF_TRS_PANEL_TRANSFER_OBJECT_BASE,		1,													"TrsPanelTransferIn",							"../Log/TrsPanelTransfer.log",		NULL},
	{ OBJ_PL_TRANSFER_OUT,		DEF_TRS_PANEL_TRANSFER_OBJECT_BASE,		2,													"TrsPanelTransferOut",							"../Log/TrsPanelTransfer.log",		NULL},

	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER1,									"TrsTabMounter1",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER2,									"TrsTabMounter2",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER3,									"TrsTabMounter3",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER4,									"TrsTabMounter4",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER5,									"TrsTabMounter5",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER6,									"TrsTabMounter6",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER7,									"TrsTabMounter7",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER8,									"TrsTabMounter8",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER9,									"TrsTabMounter9",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER10,									"TrsTabMounter10",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER11,									"TrsTabMounter11",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_TAB_MOUNTER,		DEF_TRS_TAB_MOUNTER_OBJECT_BASE,		DEF_TABMOUNTER12,									"TrsTabMounter12",							"../Log/TrsTabMounter.log",			NULL},
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_1,							"TrsInspectionCamera1",						"../Log/TrsInspectionCamera.log",	NULL},
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_2,							"TrsInspectionCamera2",						"../Log/TrsInspectionCamera.log",	NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_3,							"TrsInspectionCamera3",						"../Log/TrsInspectionCamera.log",	NULL},
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_4,							"TrsInspectionCamera4",						"../Log/TrsInspectionCamera.log",	NULL},
#endif
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_1+DEF_MAX_INSPECTION_CAM_CARRIER,	"TrsInspectionCamera5",						"../Log/TrsInspectionCamera.log",	NULL},
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_2+DEF_MAX_INSPECTION_CAM_CARRIER,	"TrsInspectionCamera6",						"../Log/TrsInspectionCamera.log",	NULL},
#ifdef DEF_SOURCE_SYSTEM
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_3+DEF_MAX_INSPECTION_CAM_CARRIER,	"TrsInspectionCamera7",						"../Log/TrsInspectionCamera.log",	NULL},
	{ OBJ_PL_CAMERA_CARRIER,	DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE,	DEF_INSPECTION_CARRIER_4+DEF_MAX_INSPECTION_CAM_CARRIER,	"TrsInspectionCamera8",						"../Log/TrsInspectionCamera.log",	NULL},
#endif

	{ OBJ_PL_HANDLER,			DEF_TRS_HANDLER_OBJECT_BASE,			0,													"TrsTHandler",								"../Log/TrsTHandler.log",	NULL}, //SJ_YYK 161104 Add.

};				
					
MSystemInfo::MSystemInfo() : m_iNumObjects(0)					
{					
	for (int i = 0; i < DEF_MAX_OBJECT_NO; i++)					
		m_iIndexTable[i] = -1;			

	makeSystemInfo();
	makeIndexTable();
}

MSystemInfo::~MSystemInfo()
{
}

void MSystemInfo::GetObjectAttributes(int iObjectID, int iInstanceNo, int& eObjType, CString& strObjName, CString& strLogFileFullName)
{
	int iIndex = getTableIndex(iObjectID);
	if (iIndex < 0)
	{
		iInstanceNo = 0;
		eObjType = OBJ_NONE;
		strObjName = "";
		strLogFileFullName = "";
	}
	else
	{
		iInstanceNo = m_infoObject[iIndex].m_iInstanceNo;
		eObjType = m_infoObject[iIndex].m_iObjectType;
		strObjName = m_infoObject[iIndex].m_strObjectName;
		strLogFileFullName = m_infoObject[iIndex].m_strLogFileName;
	}
}

/**	Get Valid Number of Component Objects	*/
int	MSystemInfo::GetNumObjects()
{
	return m_iNumObjects;
}

/** Get Valid Object Information of the registered objects	 */
int MSystemInfo::GetObjectInform(SObjectInfo sInfoObject[DEF_MAX_OBJECT_INFO_NO])
{
	for (int i = 0; i < DEF_MAX_OBJECT_INFO_NO; i++)
		sInfoObject[i] = m_infoObject[i];
	return ERR_SYSTEMINFO_SUCCESS;
}

/**	Get Object Type	 */
int MSystemInfo::GetObjectType(int iObjectID)
{
	int iIndex = getTableIndex(iObjectID);
	if (iIndex == -1)
		return OBJ_NONE;
	return m_infoObject[iIndex].m_iObjectType;
}

/**	Get Object name	 */
CString MSystemInfo::GetObjectName(int iObjectID)
{
	int iIndex = getTableIndex(iObjectID);
	if (iIndex == -1)
		return "";
	return m_infoObject[iIndex].m_strObjectName;
}

int MSystemInfo::GetObjectName(int iComponentCode, int iInstanceNo, CString* pstrName)
{
	for (int i = 0; i < DEF_MAX_OBJECT_INFO_NO; i++)
	{
		if (iComponentCode == m_infoObject[i].m_iObjectType
			&& iInstanceNo == m_infoObject[i].m_iInstanceNo)
		{
			*pstrName = m_infoObject[i].m_strObjectName;
			return ERR_SYSTEMINFO_SUCCESS;
		}
	}

	return -1;	
}


/**	Get Object InstanceNo */
int MSystemInfo::GetObjectInstanceNo(int iObjectID)
{
	int iIndex = getTableIndex(iObjectID);
	if (iIndex == -1)
		return 0;
	return m_infoObject[iIndex].m_iInstanceNo;
}

ICommon	*MSystemInfo::GetCommonObject(int iObjectID)
{
	int iIndex = getTableIndex(iObjectID);
	if (iIndex == -1)
		return NULL;
	return m_infoObject[iIndex].m_pObj;
}

SCommonAttribute MSystemInfo::GetCommonData(int iObjectID)
{
	SCommonAttribute commonData;
	commonData.m_iObjectID = iObjectID;
	commonData.m_iInstanceNo = GetObjectInstanceNo(iObjectID);
	commonData.m_strObjectName = GetObjectName(iObjectID);
	commonData.m_strLogFileName = GetLogFileName(iObjectID);
	commonData.m_ucLevel = 0x00;
	commonData.m_iLogKeepingDays = 7;
	return commonData;
}

void MSystemInfo::SetAllObjsLogAttribute(BYTE ucLevel, int iKeepingDays)
{
	ICommon	*pCommon;
	MLog	*pLog;

	for (int i = 0; i < DEF_MAX_OBJECT_INFO_NO; i++)
	{
		pCommon = m_infoObject[i].m_pObj;
		if (pCommon != NULL)
		{
			pLog = pCommon->GetLogManager();
			pLog->SetLogLevel(ucLevel);
			pLog->SetLogKeepingDays(iKeepingDays);
		}
	}

	ICommon::m_pErrorLogMng->SetLogLevel(ucLevel);
	ICommon::m_pErrorLogMng->SetLogKeepingDays(iKeepingDays);

	ICommon::m_pTactTimeLogMng->SetLogLevel(ucLevel);
	ICommon::m_pTactTimeLogMng->SetLogKeepingDays(iKeepingDays);
//170427 JSH.s
	ICommon::m_pEqTactTimeLogMng->SetLogLevel(ucLevel);
	ICommon::m_pEqTactTimeLogMng->SetLogKeepingDays(iKeepingDays);
//170427 JSH.e
}

void MSystemInfo::DeleteOldLogFilesAllObjs()
{
	ICommon	*pCommon;
	MLog	*pLog;

	for (int i = 0; i < DEF_MAX_OBJECT_INFO_NO; i++)
	{
		pCommon = m_infoObject[i].m_pObj;
		if (pCommon != NULL)
		{
			pLog = pCommon->GetLogManager();
			pLog->DeleteOldLogFiles();
		}
	}
}

/**	Get Object File Name of logging	 */
CString MSystemInfo::GetLogFileName(int iObjectID)
{
	int iIndex = getTableIndex(iObjectID);
	if (iIndex == -1)
		return "";
	return m_infoObject[iIndex].m_strLogFileName;
}

void MSystemInfo::makeIndexTable()
{
	int iObjectID;

	for (int i = 0; i < DEF_MAX_OBJECT_INFO_NO; i++)
	{
		if (m_infoObject[i].m_iObjectType > OBJ_NONE &&  m_infoObject[i].m_iObjectType <= DEF_MAX_OBJECT_INFO_NO)
		{
			iObjectID = m_infoObject[i].m_iObjectBase + m_infoObject[i].m_iInstanceNo;
			m_iIndexTable[iObjectID] = i;	
		}	
	}
}

int MSystemInfo::getTableIndex(int iObjectID)
{
	if (iObjectID >= DEF_MAX_OBJECT_NO)
		return -1;

	if (iObjectID <= 0)
		return -1;

	return m_iIndexTable[iObjectID];
}

void MSystemInfo::makeSystemInfo()
{

	m_iNumObjects = 0;
	CString strFileOrg, strFileResult;
	
	for (int i = 0; i < DEF_MAX_OBJECT_INFO_NO; i++)
	{
		m_infoObject[i] = s_SystemObjects[i];
		if (s_SystemObjects[i].m_iObjectType >= 1)
		{
			m_iNumObjects++;
		}
	}
}

int MSystemInfo::RegisterComponent(int iObjectID, ICommon *plinkComp)
{
	int iTableIndex = getTableIndex(iObjectID);
	if (iTableIndex == -1)
		return -1;

	m_infoObject[iTableIndex].m_pObj  = plinkComp;

	return ERR_SYSTEMINFO_SUCCESS;
}

void MSystemInfo::MakeErrorMessageFile()
{
	FILE* fp;
	int iErrorCode;
    WCHAR chrData[256];
	CString strErrorCode;
	CString strFilePath;
	char strGeneratedError[512];

	fp = fopen(".\\Info\\newErrorMessage.dat", "a");
    memset(chrData, NULL, sizeof(chrData));
	memset(strGeneratedError, NULL, sizeof(strGeneratedError));
	strFilePath = ".\\Info\\ErrorMessage.dat";

	// selsk_jdy fprintf(fp,"[Error Korean]\n");
	fprintf(fp,"[Error English]\n");

	for (int i = 0; i < DEF_MAX_OBJECT_NO; i++)
	{
		if (s_SystemObjects[i].m_iObjectType != 0)
		{
			for (int j = 0; j < 1000; j++)
			{
				iErrorCode = s_SystemObjects[i].m_iObjectType*1000 + j;
				strErrorCode.Format(_T("%d"),iErrorCode);
				// selsk_jdy GetPrivateProfileString("ERROR KOREAN", strErrorCode, "ERROR", chrData, 256, strFilePath);
				GetPrivateProfileString(_T("ERROR ENGLISH"), strErrorCode, _T("ERROR"), chrData, 256, strFilePath);

				if (wcscmp(chrData, _T("ERROR")) != 0)
				{
					sprintf(strGeneratedError,"%d=%s %s\n",iErrorCode*1000+s_SystemObjects[i].m_iInstanceNo, s_SystemObjects[i].m_strObjectName, LPSTR(LPCTSTR(chrData)));
					fprintf(fp,strGeneratedError);
				}
			}
		}
	}
	fclose(fp);
}