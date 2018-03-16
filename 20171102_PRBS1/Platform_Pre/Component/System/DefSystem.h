#ifndef DEFSYSTEM_H
#define DEFSYSTEM_H

#include "DefSystemConfig.h"





/************************************************************************/
/*  Operation Mode							                            */
/************************************************************************/
typedef enum enumStateAutoManual
{
	MANUAL_MODE,
	AUTORUN_MODE
} EOpMode;

/************************************************************************/
/* User Group								                            */
/************************************************************************/
typedef enum enumUserGroup
{
	NONE_GROUP,
	OPERATOR_GROUP,
	ENGINEER_GROUP,
	MAKER_GROUP
} EUserGroup;

/************************************************************************/
/*  Operation Status						                            */
/************************************************************************/
typedef enum enumStateOPStatus
{
	INIT_STATUS,
	ERROR_STOP,
	STEP_STOP,
	CYCLE_STOP,
	START_RUN,
	RUN
} EOPStatus;

/************************************************************************/
/*  Run Mode														    */
/************************************************************************/
typedef enum enumOPMode
{
	NORMAL_RUN_MODE,
	PASS_RUN_MODE,
	DRY_RUN_MODE,
	REPAIR_RUN_MODE,
} ERunMode;

/************************************************************************/
/*  Work Side								                            */
/************************************************************************/
typedef enum
{
	WORKING_SIDE_UNKNOWN		= -1, 
	WORKING_SIDE_X_UP			= 0,
	WORKING_SIDE_X_DOWN			= 1,
	WORKING_SIDE_Y_LEFT			= 2,
	WORKING_SIDE_Y_RIGHT		= 3
} EWorkingSide;

/************************************************************************/
/*																		*/
/************************************************************************/
typedef enum
{
	eMarkErrorNone	= -1,
	eMarkStop		= 0,
	eMarkRunManual	= 1,
	eMarkRunRetry	= 2,
	eMarkOS			= 3,
} EMarkErrorTreat;

/************************************************************************/
/*  Prebonder Unit Number												*/
/************************************************************************/
const int DEF_UNIT1				= 1;	// Gate
const int DEF_UNIT2				= 2;	// Source1
const int DEF_UNIT3				= 3;	// Source2

/************************************************************************/
/*  Max Num of Work Side												*/
/************************************************************************/
const int DEF_MAX_WORK_SIDE		= 4;
const int DEF_MAX_WORK_SIDE_PER_UNIT = 2;

/************************************************************************/
/*	Tab								                                    */
/************************************************************************/
const int DEF_MAX_TAB			= 60;
const int DEF_MAX_TAB_XU		= DEF_MAX_TAB;
const int DEF_MAX_TAB_XD		= DEF_MAX_TAB;
const int DEF_MAX_TAB_YL		= DEF_MAX_TAB;
const int DEF_MAX_TAB_YR		= DEF_MAX_TAB;

/************************************************************************/
/*  Common Constant														*/
/************************************************************************/
#define	PI 3.1415926535

const int SUCCESS				= 0;
const int FAILURE				= -1;
const int USER_STOP				= -2;
const int NOT_USE_WORKSIDE		= -3;
const int THERE_IS_NO_TAB_IC	= -4;
const int DEF_DEFAULT			= -5;

/************************************************************************/
/*  X,Y,T,X Coordination					                            */
/************************************************************************/
const int DEF_X					= 0;
const int DEF_Y					= 1;
const int DEF_T					= 2;
const int DEF_Z					= 3;

/************************************************************************/
/*  Worker Group : Front / Rear Group		                            */
/************************************************************************/
const int DEF_MAX_GROUP			= 2;
const int DEF_NONE_GROUP		= -1;
const int DEF_FRONT_GROUP		= 0;
const int DEF_REAR_GROUP		= 1;

#ifdef DEF_SOURCE_SYSTEM
const int DEF_MAX_WORKER_GROUP	= 2;
const int DEF_MAX_WORKER_PER_GROUP = 4;
#else
const int DEF_MAX_WORKER_GROUP	= 2;
const int DEF_MAX_WORKER_PER_GROUP = 2;
#endif

// Panel의 작업 횟수 ////////////////////////////////////////////////
const int	DEF_NONE_WORK_SCHEDULE = -1;
#ifdef DEF_SOURCE_SYSTEM
	const int	DEF_MAX_WORK_SCHEDULE = 2;
#else
	const int	DEF_MAX_WORK_SCHEDULE = 1;
#endif
const int	DEF_WORK_SCHEDULE_FIRST = 0;
const int	DEF_WORK_SCHEDULE_SECOND = 1;


// Air 부상 Vacuum 갯수 ////////////////////////////////////////////////
const int	DEF_MAX_AIRVACUUM	= 4;

// Air 부상 Blow 갯수 ////////////////////////////////////////////////
const int	DEF_MAX_AIRBLOW		= 2;

/************************************************************************/
/*  Dialog & View							                            */
/************************************************************************/
const int DEF_FIX_MODE			= 0;
const int DEF_MODEL_MODE		= 1;

/************************************************************************/
/*  PreBonder System						                            */
/************************************************************************/
const int DEF_MAX_MODEL			= 255;	// Number of SYSTEM MODEL

/************************************************************************/
/*  PanelAligner		  						                        */
/************************************************************************/
const int DEF_MAX_STAGE			= 1;
const int DEF_STAGE				= 0;

/************************************************************************/
/*  Panel Transfer		  						                        */
/************************************************************************/
const int DEF_MAX_PANEL_TRANSFER = 3;
const int DEF_PANEL_TRANSFER_CENTER	= 0;
const int DEF_PANEL_TRANSFER_IN		= 1;
const int DEF_PANEL_TRANSFER_OUT	= 2;

/************************************************************************/
/*  Conveyor			  						                        */
/************************************************************************/
const int DEF_MAX_CONVEYOR		= 2;
const int DEF_LOAD_CONVEYOR		= 0;
const int DEF_UNLOAD_CONVEYOR	= 1;

/************************************************************************/
/*  TabMounter				  						                    */
/************************************************************************/
const int DEF_NONE_TABMOUNTER	= -1;
const int DEF_NONE_TABMOUNTER_GROUP = -1;

const int DEF_TABMOUNTER1		= 0;
const int DEF_TABMOUNTER2		= 1;
const int DEF_TABMOUNTER3		= 2;
const int DEF_TABMOUNTER4		= 3;
const int DEF_TABMOUNTER5		= 4;
const int DEF_TABMOUNTER6		= 5;
const int DEF_TABMOUNTER7		= 6;
const int DEF_TABMOUNTER8		= 7;
const int DEF_TABMOUNTER9		= 8;
const int DEF_TABMOUNTER10		= 9;
const int DEF_TABMOUNTER11		= 10;
const int DEF_TABMOUNTER12		= 11;

#ifdef DEF_SOURCE_SYSTEM
const int DEF_MAX_TABMOUNTER	= 4;

const int DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[DEF_MAX_WORKER_GROUP] = { DEF_TABMOUNTER1, DEF_TABMOUNTER5};
const int DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[DEF_MAX_WORKER_GROUP] = { DEF_TABMOUNTER4, DEF_TABMOUNTER8 };
#else
const int DEF_MAX_TABMOUNTER	= 2;

const int DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[DEF_MAX_WORKER_GROUP] = { DEF_TABMOUNTER1, DEF_TABMOUNTER3 };
const int DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[DEF_MAX_WORKER_GROUP] = { DEF_TABMOUNTER2, DEF_TABMOUNTER4 };
#endif



const int	DEF_MOUNTER_NO_WORK			= -1;

#ifdef DEF_SOURCE_SYSTEM
const int	DEF_MOUNTER_WORK_PER_GROUP	= DEF_MAX_TABMOUNTER;
#else
const int	DEF_MOUNTER_WORK_PER_GROUP	= DEF_MAX_TABMOUNTER;
#endif

//SJ_YYK 150817 Add...
const int DEF_AOC_UNIT_NO_1				= 0;
const int DEF_AOC_UNIT_NO_2				= 1;
const int DEF_AOC_UNIT_NO_3				= 2;
const int DEF_AOC_UNIT_NO_4				= 3;

/************************************************************************/
/*  TabAligner				  					                        */
/************************************************************************/
#ifdef DEF_SOURCE_SYSTEM
const int DEF_MAX_TABALIGNER	= 2;
#else
const int DEF_MAX_TABALIGNER	= 2;
#endif

const int DEF_TABALIGNER1		= 0;
const int DEF_TABALIGNER2		= 1;
const int DEF_TABALIGNER3		= 2;
const int DEF_TABALIGNER4		= 3;

const int DEF_NONE_TABALIGNER			= -1;
const int DEF_MAX_TABALIGNER_PER_GROUP	= 2;
const int DEF_TABALIGNER1_PER_GROUP		= 0;
const int DEF_TABALIGNER2_PER_GROUP		= 1;

/************************************************************************/
/*  TabFeeder			  						                        */
/************************************************************************/
const int DEF_MAX_TABFEEDER		= 2;//100713.KKY DEF_MAX_WORKER_GROUP;
const int DEF_TABFEEDER1		= 0;
const int DEF_TABFEEDER2		= 1;

const int DEF_NONE_TABFEEDER	= -1;
const int DEF_FEEDER_NO_WORK	= -1;
//120111.kms_____
const int DEF_TABFEEDER_1		= 0;
const int DEF_TABFEEDER_2		= 1;
const int DEF_TABFEEDER_ALL		= 2;
//________________
// TabFeeder 공급 Tension 유지를 위해 Sprocket 이 회전해야 하는 회전 각도.
// 기구 구조물의 치수로 인해 결정된다. (Source/Gate 동일)...Ver에 상관없이 통일한다. 50.0
const double DEF_ROTATION_ANGLE = 50.0;

// Reel index for each Feeder
const int DEF_MAX_REEL			= 2;

const int DEF_USING_REEL		= 0;
const int DEF_SPARE_REEL		= 1;

/************************************************************************/
/*  BadTab Detector	(Per TabFeeder)				                        */
/************************************************************************/
// 0,2 번은 TCP Detect 1,3 : COF Detect
const int DEF_MAX_BADTAB_DETECTOR	= 4;
const int DEF_BADTAB_DETECTOR1		= 0;
const int DEF_BADTAB_DETECTOR2		= 1;
const int DEF_BADTAB_DETECTOR3		= 2;
const int DEF_BADTAB_DETECTOR4		= 3;

/************************************************************************/
/*  TabAlignCarrier			  						                        */
/************************************************************************/
const int DEF_MAX_TABALIGNER_CAM_CARRIER	= 2;
const int DEF_TABALIGNER_CARRIER_1		= 0;
const int DEF_TABALIGNER_CARRIER_2		= 1;

/************************************************************************/
/*  Camera Carrier Axis		  											*/
/************************************************************************/
const int DEF_NONE_INSPECTION_CAMERA	= -1;
const int DEF_INSPECTION_CAMERA_NONE_GROUP = -1;


/************************************************************************/
/*  Inspection Camera Carrier			  						        */
/************************************************************************/

const int DEF_MAX_CAM_EXPAND		= 6;
const int DEF_CAM_EXPAND_1			= 0;	//@Inspection Camera1
const int DEF_CAM_EXPAND_2			= 1;	//@Inspection Camera2
const int DEF_CAM_EXPAND_3			= 2;	//@Inspection Camera3
const int DEF_CAM_EXPAND_4			= 3;	//@Inspection Camera4
const int DEF_CAM_EXPAND_5			= 4;	//@TabCarrier Camera
const int DEF_CAM_EXPAND_6			= 5;    //@TabCarrier Camera 2 //SJ_YYK 150817 Add...
const int DEF_CAM_INSPECTION1		= DEF_CAM_EXPAND_1;
const int DEF_CAM_INSPECTION2		= DEF_CAM_EXPAND_2;
const int DEF_CAM_INSPECTION3		= DEF_CAM_EXPAND_3;
const int DEF_CAM_INSPECTION4		= DEF_CAM_EXPAND_4;
const int DEF_CAM_TABCARRIER		= DEF_CAM_EXPAND_5;
const int DEF_CAM_TABCARRIER2		= DEF_CAM_EXPAND_6;  //SJ_YYK 150817 Add...


#ifdef DEF_SOURCE_SYSTEM
	const int DEF_MAX_INSPECTION_CAM_CARRIER	= 4;
#else
	const int DEF_MAX_INSPECTION_CAM_CARRIER	= 2;
#endif

const int DEF_INSPECTION_CARRIER_1			= 0;
const int DEF_INSPECTION_CARRIER_2			= 1;
const int DEF_INSPECTION_CARRIER_3			= 2;
const int DEF_INSPECTION_CARRIER_4			= 3;

const int DEF_MAX_INSPECTION_CAM_PER_GROUP = DEF_MAX_INSPECTION_CAM_CARRIER;

#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	const int DEF_PANEL_ALIGN_CAMERA_CARRIER1	= DEF_INSPECTION_CARRIER_4;
#	else
	const int DEF_PANEL_ALIGN_CAMERA_CARRIER1	= DEF_INSPECTION_CARRIER_1;//Front Carrier
#endif
	const int DEF_PANEL_ALIGN_CAMERA_CARRIER2	= DEF_INSPECTION_CARRIER_1;//Rear Carrier
#else
	const int DEF_PANEL_ALIGN_CAMERA_CARRIER1	= DEF_INSPECTION_CARRIER_1;//Front Carrier
	const int DEF_PANEL_ALIGN_CAMERA_CARRIER2	= DEF_INSPECTION_CARRIER_1;//Rear Carrier
#endif


/************************************************************************/
/*  Tab Carrier Axis		  											*/
/************************************************************************/
const int DEF_NONE_TABCARRIER		= -1;

#ifdef DEF_SOURCE_SYSTEM
	const int DEF_MAX_TABCARRIER		= 4;
#else
	const int DEF_MAX_TABCARRIER		= 2;
#endif

const int DEF_TABCARRIER1			= 0;
const int DEF_TABCARRIER2			= 1;
const int DEF_TABCARRIER3			= 2;
const int DEF_TABCARRIER4			= 3;




// 주의 : 이 값이 2가 아닌 값이 되면 이 값으로 % 연산하는 부분의 Code 수정 필요하다.
const int DEF_INSPECT_TAB_GAP			= 2;

//110620.KKY____________
const int DEF_MAX_PRODUCT_PLAN_CNT	= 5;

const int DEF_MAX_PRODUCT_PLAN		= 3;
const int DEF_CUR_PRODUCT_PLAN		= 0;
const int DEF_NEXT_PRODUCT_PLAN		= 1;
const int DEF_END_PRODUCT_PLAN		= 2;
const int DEF_NONE_PRODUCT_PLAN	= -1;

const int DEF_MAX_MCODE			= 10;
const int DEF_MCODE_1			= 0;
const int DEF_MCODE_2			= 1;
const int DEF_MCODE_3			= 2;
const int DEF_MCODE_4			= 3;
const int DEF_MCODE_5			= 4;
const int DEF_MCODE_6			= 5;
const int DEF_MCODE_7			= 6;
const int DEF_MCODE_8			= 7;
const int DEF_MCODE_9			= 8;
const int DEF_MCODE_10			= 9;
//______________________

/************************************************************************/
/*  Jog										                            */
/************************************************************************/
const int DEF_JOG_NULL_BASE					= 100;

#ifdef DEF_SOURCE_SYSTEM

#	ifdef DEF_USE_TURN_HANDLER
		const int DEF_MAX_JOG_LIST					= 79;
#	else
		const int DEF_MAX_JOG_LIST					= 77;
#	endif

const int DEF_JOG_STAGE_YT					= 0;	// Panel Aligner Y1, Y2 비동기 제어
const int DEF_JOG_STAGE_XYT					= 1;	// Panel Aligner Y1, Y2 동기 제어
const int DEF_JOG_STAGE_XY					= 2; 
const int DEF_JOG_STAGE_Z					= 3;	
const int DEF_JOG_PANEL_TRANSFER_IN_X		= 4;	// PanelTransfer In X1, X2 동기 제어
const int DEF_JOG_PANEL_TRANSFER_OUT_X		= 5;	// PanelTransfer Out X1, X2 동기 제어
const int DEF_JOG_MODEL_CHANGE_Y1			= 6;
const int DEF_JOG_MODEL_CHANGE_Y2			= 7;

const int DEF_JOG_F_BADTAB_DETECTOR_X		= 8;
const int DEF_JOG_F_TABFEEDER_SPROCKET1_T	= 9;
const int DEF_JOG_F_TABFEEDER_SPROCKET2_T	= 10;
const int DEF_JOG_F_TAB_PRESS_Z				= 11;
const int DEF_JOG_R_BADTAB_DETECTOR_X		= 12;
const int DEF_JOG_R_TABFEEDER_SPROCKET1_T	= 13;
const int DEF_JOG_R_TABFEEDER_SPROCKET2_T	= 14;
const int DEF_JOG_R_TAB_PRESS_Z				= 15;

const int DEF_JOG_F_TABMOUNTER1_X			= 16;
const int DEF_JOG_F_TABMOUNTER2_X			= 17;
const int DEF_JOG_F_TABMOUNTER3_X			= 18;
const int DEF_JOG_F_TABMOUNTER4_X			= 19;
const int DEF_JOG_F_TABMOUNTER1_Z			= 20;
const int DEF_JOG_F_TABMOUNTER2_Z			= 21;
const int DEF_JOG_F_TABMOUNTER3_Z			= 22;
const int DEF_JOG_F_TABMOUNTER4_Z			= 23;

const int DEF_JOG_R_TABMOUNTER1_X			= 24;
const int DEF_JOG_R_TABMOUNTER2_X			= 25;
const int DEF_JOG_R_TABMOUNTER3_X			= 26;
const int DEF_JOG_R_TABMOUNTER4_X			= 27;
const int DEF_JOG_R_TABMOUNTER1_Z			= 28;
const int DEF_JOG_R_TABMOUNTER2_Z			= 29;
const int DEF_JOG_R_TABMOUNTER3_Z			= 30;
const int DEF_JOG_R_TABMOUNTER4_Z			= 31;

const int DEF_JOG_F_INSPECTION_CAMERA1_X	= 32;
const int DEF_JOG_F_INSPECTION_CAMERA2_X	= 33;
const int DEF_JOG_F_INSPECTION_CAMERA3_X	= 34;
const int DEF_JOG_F_INSPECTION_CAMERA4_X	= 35;
const int DEF_JOG_F_BACKUP1_Z				= 36;
const int DEF_JOG_F_BACKUP2_Z				= 37;
const int DEF_JOG_F_BACKUP3_Z				= 38;
const int DEF_JOG_F_BACKUP4_Z				= 39;

const int DEF_JOG_R_INSPECTION_CAMERA1_X	= 40;
const int DEF_JOG_R_INSPECTION_CAMERA2_X	= 41;
const int DEF_JOG_R_INSPECTION_CAMERA3_X	= 42;
const int DEF_JOG_R_INSPECTION_CAMERA4_X	= 43;
const int DEF_JOG_R_BACKUP1_Z				= 44;
const int DEF_JOG_R_BACKUP2_Z				= 45;
const int DEF_JOG_R_BACKUP3_Z				= 46;
const int DEF_JOG_R_BACKUP4_Z				= 47;

const int DEF_JOG_F_TABCARRIER1_XYT			= 48;
const int DEF_JOG_F_TABCARRIER2_XYT			= 49;
const int DEF_JOG_F_TABCARRIER3_XYT			= 50;
const int DEF_JOG_F_TABCARRIER4_XYT			= 51;
const int DEF_JOG_R_TABCARRIER1_XYT			= 52;
const int DEF_JOG_R_TABCARRIER2_XYT			= 53;
const int DEF_JOG_R_TABCARRIER3_XYT			= 54;
const int DEF_JOG_R_TABCARRIER4_XYT			= 55;

const int DEF_JOG_F_FEEDER_REEL_T			= 56;
const int DEF_JOG_F_MOLDING_UPDOWN_Z		= 57;
const int DEF_JOG_R_FEEDER_REEL_T			= 58;
const int DEF_JOG_R_MOLDING_UPDOWN_Z		= 59;

const int DEF_JOG_GANTRY_Y					= 60;
const int DEF_JOG_R_GANTRY_Y				= 61;

const int DEF_JOG_F_INSPECTION_CAMERA1_EXPAND	= 62;
const int DEF_JOG_F_INSPECTION_CAMERA2_EXPAND	= 63;
const int DEF_JOG_F_INSPECTION_CAMERA3_EXPAND	= 64;
const int DEF_JOG_F_INSPECTION_CAMERA4_EXPAND	= 65;
const int DEF_JOG_F_TABCARRIER_CAMERA_EXPAND	= 66;

const int DEF_JOG_R_INSPECTION_CAMERA1_EXPAND	= 67;
const int DEF_JOG_R_INSPECTION_CAMERA2_EXPAND	= 68;
const int DEF_JOG_R_INSPECTION_CAMERA3_EXPAND	= 69;
const int DEF_JOG_R_INSPECTION_CAMERA4_EXPAND	= 70;
const int DEF_JOG_R_TABCARRIER_CAMERA_EXPAND	= 71;

const int DEF_JOG_F_TABCARRIER_Z				= 72;  //SJ_YYK 150109 Add..
const int DEF_JOG_R_TABCARRIER_Z				= 73;
const int DEF_JOG_F_TABCARRIER_CAMERA_EXPAND2	= 74;
const int DEF_JOG_R_TABCARRIER_CAMERA_EXPAND2	= 75;

const int DEF_JOG_PANEL_TRANSFER_CENTER_X		= 76;

#		ifdef DEF_USE_TURN_HANDLER
		const int DEF_JOG_HANDLER_XT					= 77;
		const int DEF_JOG_HANDLER_Z						= 78;
#		endif


	

#else	//#ifdef DEF_SOURCE_SYSTEM

const int DEF_MAX_JOG_LIST					= 41;

const int DEF_JOG_STAGE_YT					= 0;	// Panel Aligner Y1, Y2 비동기 제어
const int DEF_JOG_STAGE_XYT					= 1;	// Panel Aligner Y1, Y2 동기 제어
const int DEF_JOG_STAGE_XY					= 2; 
const int DEF_JOG_STAGE_Z					= 3;	
const int DEF_JOG_PANEL_TRANSFER_OUT_X		= 4;	// PanelTransfer Out X1, X2 동기 제어
const int DEF_JOG_MODEL_CHANGE_Y1			= 5;
const int DEF_JOG_MODEL_CHANGE_Y2			= 6;

const int DEF_JOG_F_BADTAB_DETECTOR_X		= 7;
const int DEF_JOG_F_TABFEEDER_SPROCKET1_T	= 8;
const int DEF_JOG_F_TABFEEDER_SPROCKET2_T	= 9;
const int DEF_JOG_F_TAB_PRESS_Z				= 10;
const int DEF_JOG_R_BADTAB_DETECTOR_X		= 11;
const int DEF_JOG_R_TABFEEDER_SPROCKET1_T	= 12;
const int DEF_JOG_R_TABFEEDER_SPROCKET2_T	= 13;
const int DEF_JOG_R_TAB_PRESS_Z				= 14;

const int DEF_JOG_F_TABMOUNTER1_X			= 15;
const int DEF_JOG_F_TABMOUNTER2_X			= 16;
const int DEF_JOG_F_TABMOUNTER1_Z			= 17;
const int DEF_JOG_F_TABMOUNTER2_Z			= 18;

const int DEF_JOG_R_TABMOUNTER1_X			= 19;
const int DEF_JOG_R_TABMOUNTER2_X			= 20;
const int DEF_JOG_R_TABMOUNTER1_Z			= 21;
const int DEF_JOG_R_TABMOUNTER2_Z			= 22;

const int DEF_JOG_F_INSPECTION_CAMERA1_X	= 23;
const int DEF_JOG_F_INSPECTION_CAMERA2_X	= 24;
const int DEF_JOG_F_BACKUP1_Z				= 25;
const int DEF_JOG_F_BACKUP2_Z				= 26;
const int DEF_JOG_R_INSPECTION_CAMERA1_X	= 27;
const int DEF_JOG_R_INSPECTION_CAMERA2_X	= 28;
const int DEF_JOG_R_BACKUP1_Z				= 29;
const int DEF_JOG_R_BACKUP2_Z				= 30;

const int DEF_JOG_F_TABCARRIER1_XYT			= 31;
const int DEF_JOG_F_TABCARRIER2_XYT			= 32;
const int DEF_JOG_R_TABCARRIER1_XYT			= 33;
const int DEF_JOG_R_TABCARRIER2_XYT			= 34;

const int DEF_JOG_F_FEEDER_REEL_T			= 35;
const int DEF_JOG_F_MOLDING_UPDOWN_Z		= 36;
const int DEF_JOG_R_FEEDER_REEL_T			= 37;
const int DEF_JOG_R_MOLDING_UPDOWN_Z		= 38;

const int DEF_JOG_GANTRY_Y					= 39;
const int DEF_JOG_R_GANTRY_Y				= 40;

const int DEF_JOG_F_INSPECTION_CAMERA1_EXPAND	= 62;
const int DEF_JOG_F_INSPECTION_CAMERA2_EXPAND	= 63;
const int DEF_JOG_F_TABCARRIER_CAMERA_EXPAND	= 66;

const int DEF_JOG_R_INSPECTION_CAMERA1_EXPAND	= 67;
const int DEF_JOG_R_INSPECTION_CAMERA2_EXPAND	= 68;
const int DEF_JOG_R_TABCARRIER_CAMERA_EXPAND	= 71;

#endif


/************************************************************************/
/*  Camera									                            */
/************************************************************************/
const int DEF_CAMERA_1					= 0;
const int DEF_CAMERA_2					= 1;
const int DEF_CAMERA_3					= 2;
const int DEF_CAMERA_4					= 3;
const int DEF_CAMERA_5					= 4;
const int DEF_CAMERA_6					= 5;
const int DEF_CAMERA_7					= 6;
const int DEF_CAMERA_8					= 7;
const int DEF_CAMERA_9					= 8;
const int DEF_CAMERA_10					= 9;
const int DEF_CAMERA_11					= 10;
const int DEF_CAMERA_12					= 11;
const int DEF_CAMERA_13					= 12;
const int DEF_CAMERA_14					= 13;
const int DEF_CAMERA_15					= 14;
const int DEF_CAMERA_16					= 15;
const int DEF_CAMERA_17					= 16;
const int DEF_CAMERA_18					= 17;
const int DEF_CAMERA_19					= 18;
const int DEF_CAMERA_20					= 19;
const int DEF_CAMERA_21					= 20;
const int DEF_CAMERA_22					= 21;
const int DEF_CAMERA_23					= 22;
const int DEF_CAMERA_24					= 23;

#ifdef DEF_SOURCE_SYSTEM
	const int DEF_MAX_CAMERA_NO				= 24;

	const int DEF_F_TABALIGNER_CAMERA_1		= DEF_CAMERA_1;
	const int DEF_F_TABALIGNER_CAMERA_2		= DEF_CAMERA_2;
	const int DEF_F_TABALIGNER_CAMERA_3		= DEF_CAMERA_3;
	const int DEF_F_TABALIGNER_CAMERA_4		= DEF_CAMERA_4;
	const int DEF_R_TABALIGNER_CAMERA_1		= DEF_CAMERA_5;
	const int DEF_R_TABALIGNER_CAMERA_2		= DEF_CAMERA_6;
	const int DEF_R_TABALIGNER_CAMERA_3		= DEF_CAMERA_7;
	const int DEF_R_TABALIGNER_CAMERA_4		= DEF_CAMERA_8;

	const int DEF_INSPECTION_CAMERA_1		= DEF_CAMERA_9;
	const int DEF_INSPECTION_CAMERA_2		= DEF_CAMERA_10;
	const int DEF_INSPECTION_CAMERA_3		= DEF_CAMERA_11;
	const int DEF_INSPECTION_CAMERA_4		= DEF_CAMERA_12;
	const int DEF_INSPECTION_CAMERA_5		= DEF_CAMERA_13;
	const int DEF_INSPECTION_CAMERA_6		= DEF_CAMERA_14;
	const int DEF_INSPECTION_CAMERA_7		= DEF_CAMERA_15;
	const int DEF_INSPECTION_CAMERA_8		= DEF_CAMERA_16;

	const int DEF_R_INSPECTION_CAMERA_1		= DEF_CAMERA_17;
	const int DEF_R_INSPECTION_CAMERA_2		= DEF_CAMERA_18;
	const int DEF_R_INSPECTION_CAMERA_3		= DEF_CAMERA_19;
	const int DEF_R_INSPECTION_CAMERA_4		= DEF_CAMERA_20;
	const int DEF_R_INSPECTION_CAMERA_5		= DEF_CAMERA_21;
	const int DEF_R_INSPECTION_CAMERA_6		= DEF_CAMERA_22;
	const int DEF_R_INSPECTION_CAMERA_7		= DEF_CAMERA_23;
	const int DEF_R_INSPECTION_CAMERA_8		= DEF_CAMERA_24;

	const int DEF_PANEL_ALIGN_CAMERA_1		= DEF_INSPECTION_CAMERA_8;
	const int DEF_PANEL_ALIGN_CAMERA_2		= DEF_R_INSPECTION_CAMERA_2; //170714_KDH 수정 DEF_R_INSPECTION_CAMERA_1-> DEF_R_INSPECTION_CAMERA_2


#else
	const int DEF_MAX_CAMERA_NO				= 12;

	const int DEF_F_TABALIGNER_CAMERA_1		= DEF_CAMERA_1;
	const int DEF_F_TABALIGNER_CAMERA_2		= DEF_CAMERA_2;
	const int DEF_R_TABALIGNER_CAMERA_1		= DEF_CAMERA_3;
	const int DEF_R_TABALIGNER_CAMERA_2		= DEF_CAMERA_4;

	const int DEF_INSPECTION_CAMERA_1		= DEF_CAMERA_5;
	const int DEF_INSPECTION_CAMERA_2		= DEF_CAMERA_6;
	const int DEF_INSPECTION_CAMERA_3		= DEF_CAMERA_7;
	const int DEF_INSPECTION_CAMERA_4		= DEF_CAMERA_8;

	const int DEF_R_INSPECTION_CAMERA_1		= DEF_CAMERA_9;
	const int DEF_R_INSPECTION_CAMERA_2		= DEF_CAMERA_10;
	const int DEF_R_INSPECTION_CAMERA_3		= DEF_CAMERA_11;
	const int DEF_R_INSPECTION_CAMERA_4		= DEF_CAMERA_12;

	const int DEF_PANEL_ALIGN_CAMERA_1		= DEF_INSPECTION_CAMERA_1;
	const int DEF_PANEL_ALIGN_CAMERA_2		= DEF_R_INSPECTION_CAMERA_1;
#endif

//const int DEF_CALIB_GROUP_MAX				= 4;
const int DEF_CALIB_GROUP_MAX				= 3;
const int DEF_CALIB_GROUP_PANELALIGNER		= 0;
const int DEF_CALIB_GROUP_MOUNTER			= 1;
const int DEF_CALIB_GROUP_TABCARRIER		= 2;

const int DEF_FACTOR_NUMBER				= 9;

/************************************************************************/
/*  Vision Mark								                            */
/************************************************************************/

enum ePatternMatchingMarkNo
{
#undef VISION_MARK
#define VISION_MARK(no, pm_mark, blob_mark) pm_mark,
#include "..\..\info\VisionMark.inf"
#undef VISION_MARK
};

enum eBlobMarkNo
{
#undef VISION_MARK
#define VISION_MARK(no, pm_mark, blob_mark) blob_mark,
#include "..\..\info\VisionMark.inf"
#undef VISION_MARK
};

/************************************************************************/
/*  Lighting								                            */
/************************************************************************/

#ifdef DEF_SOURCE_SYSTEM
	const	int DEF_MAX_LIGHTING				= 28;
#else
	const	int DEF_MAX_LIGHTING				= 14;
#endif

													//	[Source System]					[Gate System]
const int DEF_LIGHTING_1				= 0;		// PreAlign Cam1,2 측광 조명		source와 동일
const int DEF_LIGHTING_2				= 1;		// PreAlign Cam3,4 측광 조명		source와 동일
const int DEF_LIGHTING_3				= 2;		// Inspection Cam1 동축 조명		source와 동일
const int DEF_LIGHTING_4				= 3;		// Inspection Cam2 동축 조명		source와 동일
const int DEF_LIGHTING_5				= 4;		// Inspection Cam3,4 측광 조명		source와 동일
const int DEF_LIGHTING_6				= 5;		// Inspection Cam3 동축 조명		source와 동일
const int DEF_LIGHTING_7				= 6;		// Inspection Cam4 동축 조명		source와 동일
const int DEF_LIGHTING_8				= 7;		// Inspection Cam3,4 측광 조명		source와 동일
const int DEF_LIGHTING_9				= 8;		// Inspection Cam5 동축 조명		source와 동일
const int DEF_LIGHTING_10				= 9;		// Inspection Cam6 동축 조명		source와 동일
const int DEF_LIGHTING_11				= 10;		// Inspection Cam5,6 측광 조명		source와 동일
const int DEF_LIGHTING_12				= 11;		// Inspection Cam7 동축 조명		source와 동일
const int DEF_LIGHTING_13				= 12;		// Inspection Cam8 동축 조명		source와 동일
const int DEF_LIGHTING_14				= 13;		// Inspection Cam7,8 측광 조명		source와 동일
const int DEF_LIGHTING_15				= 14;		//								R Inspection Cam1 동축 조명											
const int DEF_LIGHTING_16				= 15;		//								R Inspection Cam2 동축 조명														
const int DEF_LIGHTING_17				= 16;		//								R Inspection Cam3,4 측광 조명
const int DEF_LIGHTING_18				= 17;		//								R Inspection Cam3 동축 조명
const int DEF_LIGHTING_19				= 18;		//								R Inspection Cam4 동축 조명
const int DEF_LIGHTING_20				= 19;		//								R Inspection Cam3,4 측광 조명
const int DEF_LIGHTING_21				= 20;		//								R Inspection Cam5 동축 조명
const int DEF_LIGHTING_22				= 21;		//								R Inspection Cam6 동축 조명
const int DEF_LIGHTING_23				= 22;		//								R Inspection Cam5,6 측광 조명
const int DEF_LIGHTING_24				= 23;		//								R Inspection Cam7 동축 조명
const int DEF_LIGHTING_25				= 24;		//								R Inspection Cam8 동축 조명
const int DEF_LIGHTING_26				= 25;		//								R Inspection Cam7,8 측광 조명
const int DEF_LIGHTING_27				= 26;
const int DEF_LIGHTING_28				= 27;



// x : 측광 조명, y : 동축 조명
static const POINT s_rgLightIdForCam[DEF_MAX_CAMERA_NO] = 
{
	{ DEF_LIGHTING_1,	-1 },
	{ DEF_LIGHTING_1,	-1 },
	{ DEF_LIGHTING_2,	-1 },
	{ DEF_LIGHTING_2,	-1 },
	{ DEF_LIGHTING_3,   -1 },
	{ DEF_LIGHTING_3,   -1 },
	{ DEF_LIGHTING_4,   -1 },
	{ DEF_LIGHTING_4,   -1 },
	{ DEF_LIGHTING_7,	DEF_LIGHTING_5 },
	{ DEF_LIGHTING_7,	DEF_LIGHTING_6 },
	{ DEF_LIGHTING_10,	DEF_LIGHTING_8 },
	{ DEF_LIGHTING_10,	DEF_LIGHTING_9 },
	{ DEF_LIGHTING_13,	DEF_LIGHTING_11 },
	{ DEF_LIGHTING_13,	DEF_LIGHTING_12},
	{ DEF_LIGHTING_16,	DEF_LIGHTING_14},
	{ DEF_LIGHTING_16,	DEF_LIGHTING_15},
#ifdef DEF_SOURCE_SYSTEM
	{ DEF_LIGHTING_19,	DEF_LIGHTING_17 },
	{ DEF_LIGHTING_19,	DEF_LIGHTING_18 },
	{ DEF_LIGHTING_22,	DEF_LIGHTING_20 },
	{ DEF_LIGHTING_22,	DEF_LIGHTING_21 },
	{ DEF_LIGHTING_25,	DEF_LIGHTING_23 },
	{ DEF_LIGHTING_25,	DEF_LIGHTING_24},
	{ DEF_LIGHTING_28,	DEF_LIGHTING_26},
	{ DEF_LIGHTING_28,	DEF_LIGHTING_27},
#endif
};

/************************************************************************/
/*  Log Category Info													*/
/************************************************************************/
const int DEF_ERROR_LOG					= 0;
const int DEF_LINE_TACT_LOG				= 1;
const int DEF_MACHINE_TACT_LOG			= 2;

/************************************************************************/
/*  Process Object Instance Number										*/
/************************************************************************/

#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TURN_HANDLER
	const int DEF_MAX_PROCESS_INSTANCE				= 31;
#	else
	const int DEF_MAX_PROCESS_INSTANCE				= 30;
#	endif
	const int DEF_PROCESS_PANEL_ALIGNER				= 0;
	const int DEF_PROCESS_PANEL_TRANSFER_CENTER		= 1;
	const int DEF_PROCESS_PANEL_TRANSFER_IN			= 2;
	const int DEF_PROCESS_PANEL_TRANSFER_OUT		= 3;
	const int DEF_PROCESS_TABMOUNTER1				= 4;
	const int DEF_PROCESS_TABMOUNTER2				= 5;
	const int DEF_PROCESS_TABMOUNTER3				= 6;
	const int DEF_PROCESS_TABMOUNTER4				= 7;
	const int DEF_PROCESS_R_TABMOUNTER1				= 8;
	const int DEF_PROCESS_R_TABMOUNTER2				= 9;
	const int DEF_PROCESS_R_TABMOUNTER3				= 10;
	const int DEF_PROCESS_R_TABMOUNTER4				= 11;
	const int DEF_PROCESS_TABCARRIER1				= 12;
	const int DEF_PROCESS_TABCARRIER2				= 13;
	const int DEF_PROCESS_TABCARRIER3				= 14;
	const int DEF_PROCESS_TABCARRIER4				= 15;
	const int DEF_PROCESS_R_TABCARRIER1				= 16;
	const int DEF_PROCESS_R_TABCARRIER2				= 17;
	const int DEF_PROCESS_R_TABCARRIER3				= 18;
	const int DEF_PROCESS_R_TABCARRIER4				= 19;
	const int DEF_PROCESS_INSPECT_CAMERA1			= 20;
	const int DEF_PROCESS_INSPECT_CAMERA2			= 21;
	const int DEF_PROCESS_INSPECT_CAMERA3			= 22;
	const int DEF_PROCESS_INSPECT_CAMERA4			= 23;
	const int DEF_PROCESS_R_INSPECT_CAMERA1			= 24;
	const int DEF_PROCESS_R_INSPECT_CAMERA2			= 25;
	const int DEF_PROCESS_R_INSPECT_CAMERA3			= 26;
	const int DEF_PROCESS_R_INSPECT_CAMERA4			= 27;
	const int DEF_PROCESS_TABFEEDER1				= 28;
	const int DEF_PROCESS_TABFEEDER2				= 29;
#	ifdef DEF_USE_TURN_HANDLER
	const int DEF_PROCESS_TURN_HANDLER				= 30;
#	endif

#else

	const int DEF_MAX_PROCESS_INSTANCE				= 16;

	const int DEF_PROCESS_PANEL_ALIGNER				= 0;
	const int DEF_PROCESS_PANEL_TRANSFER_OUT		= 1;
	const int DEF_PROCESS_TABMOUNTER1				= 2;
	const int DEF_PROCESS_TABMOUNTER2				= 3;
	const int DEF_PROCESS_R_TABMOUNTER1				= 4;
	const int DEF_PROCESS_R_TABMOUNTER2				= 5;
	const int DEF_PROCESS_TABCARRIER1				= 6;
	const int DEF_PROCESS_TABCARRIER2				= 7;
	const int DEF_PROCESS_R_TABCARRIER1				= 8;
	const int DEF_PROCESS_R_TABCARRIER2				= 9;
	const int DEF_PROCESS_INSPECT_CAMERA1			= 10;
	const int DEF_PROCESS_INSPECT_CAMERA2			= 11;
	const int DEF_PROCESS_R_INSPECT_CAMERA1			= 12;
	const int DEF_PROCESS_R_INSPECT_CAMERA2			= 13;
	const int DEF_PROCESS_TABFEEDER1				= 14;
	const int DEF_PROCESS_TABFEEDER2				= 15;
#endif
//_____________________________________

/************************************************************************/
/*  Serial Communication					                            */
/************************************************************************/

const int DEF_MAX_SERIAL				= 11;
const int DEF_MAX_RFID					= 2;

const int DEF_SERIAL_FRONT_BCR_READER	= 0;
const int DEF_SERIAL_FRONT_RFID			= 1;
const int DEF_SERIAL_REAR_BCR_READER	= 2;
const int DEF_SERIAL_REAR_RFID			= 3;
const int DEF_SERIAL_LIGHT1				= 4;
const int DEF_SERIAL_LIGHT2				= 5;
const int DEF_SERIAL_LIGHT3				= 6;
const int DEF_SERIAL_LIGHT4				= 7;
const int DEF_SERIAL_LIGHT5				= 8;
const int DEF_SERIAL_LIGHT6				= 9;
const int DEF_SERIAL_LIGHT7				= 10;

const int DEF_RFID1						= 0;
const int DEF_RFID2						= 1;

/************************************************************************/
/*  ETC.																*/
/************************************************************************/
// 자동운전 초기화 시 Mode
const int DEF_INIT_WHOLE_WORK			= 0;	// 전체 초기화
const int DEF_PASS_PANEL				= 2;	// 현재 Panel 만 Pass Run 진행

// Camera 별 Visioin Calibration Data 저장 개수
const int DEF_VISION_CALIBRATION_DATA_NUM_PER_CAMERA_FOR_PRISM = 2;
const int DEF_VISION_CALIBRATION_UNIT_PER_CAM = 4;


/************************************************************************/
/*  Lower Equip, Upper Equip의 IO Interface 주소						*/
/************************************************************************/
const int DEF_MAX_INTERFACE_IO = 8;

typedef enum enumInterfaceIOAddr
{
	eMachineRun,							
	eEmergency,							
	eNetReady,							
	eStage1Occupied,						
	eArmViolation,						
	eHandlerMovePermissionFromStage,
	eHandlerYMovePermissionFromStage2,	
	eStageMoveInhibit,
} EIfIOAddr;

/************************************************************************/
/*  Operator Call Select 												*/
/************************************************************************/
const int DEF_MAX_OP_CALL_SELECT		= 3;	// Op Call Dlg Button 개수

const int DEF_FEEDER1_TABIC_EMPTY		= 0;
const int DEF_FEEDER2_TABIC_EMPTY		= 1;
const int DEF_OP_CALL_RESERVED3			= 2;

/************************************************************************/
/*  Unit Index For Glass Data											*/
/************************************************************************/

/*/
const int DEF_MAX_PANEL_UNIT			= 4;

const int DEF_PANEL_TRANSFER_CENTER_PANEL	= 0;
const int DEF_PANEL_TRANSFER_PANEL			= 1;
const int DEF_PANEL_ALIGNER_PANEL			= 2;
const int DEF_PANEL_TRANSFER_OUT_PANEL		= 3;
/*/
#ifdef DEF_GATE_SYSTEM
	const int DEF_MAX_PANEL_UNIT			= 2;

	const int DEF_PANEL_ALIGNER_PANEL			= 0;
	const int DEF_PANEL_TRANSFER_OUT_PANEL		= 1;

	const int DEF_PANEL_TRANSFER_CENTER_PANEL	= -1;
	const int DEF_PANEL_TRANSFER_PANEL			= -1;	//DUMMY
#else
	const int DEF_MAX_PANEL_UNIT			= 5;

	const int DEF_PANEL_TRANSFER_CENTER_PANEL	= 0;
	const int DEF_THANDLER_PANEL				= 1;
	const int DEF_PANEL_TRANSFER_PANEL			= 2;
	const int DEF_PANEL_ALIGNER_PANEL			= 3;
	const int DEF_PANEL_TRANSFER_OUT_PANEL		= 4;

//	const int DEF_PANEL_TRANSFER_CENTER_PANEL	= -1;
#endif

//___________________________________

//const int DEF_OUT_CONVEYOR_PANEL		= 4;	// 가압착에서는 사용하지 않음.

/************************************************************************/
/*  Camera 가변 관련 Data												*/
/************************************************************************/
const double DEF_LARGE_TYPE_TAB_WIDTH_STANDARD			= 50.0;	//@(축소:38.0, 확대:63.0)
const double DEF_INSPECTION_CAMERA_VARIABLE_DIST_LIMIT	= 13.0;

/************************************************************************/
/* TabIC 를 Panel 에 압착하기 전인지 후인지 구분을 위한 상수.			*/
/* 타발 정도 확인 Option												*/
/************************************************************************/
const int DEF_BEFORE_MOUNT				= 0;
const int DEF_AFTER_MOUNT				= 1;
const int DEF_CHECK_PUNCH_REPEATABILITY	= 2;

// sesl_jdy
/************************************************************************/
/*  자동운전 중 Panel Mark 인식 Error 에 대한 처리						*/
/************************************************************************/
const int DEF_PANEL_ALIGN_PASS			= 0;
const int DEF_PANEL_ALIGN_PULL_OUT		= 1;
const int DEF_PANEL_ALIGN_RUN_STOP		= 2;

/************************************************************************/
/*  자동운전 중 TabInspection Mark 인식 Error 에 대한 처리						*/
/************************************************************************/
const int DEF_TABINSPECTION_RUN_STOP			= 0;
const int DEF_TABINSPECTION_CURTAB_RETRY		= 1;
const int DEF_TABINSPECTION_TAB_RELOAD			= 2;

/************************************************************************/
/*  PanelAligner 관련													*/
/************************************************************************/

/************************************************************************/
/*  위치 비교 처리														*/
/************************************************************************/
typedef enum enumCompareType
{
	MORE_THAN,
	MORE_THAN_EQUAL,
	EQUAL,
	LESS_THAN_EQUAL,
	LESS_THAN
}ECompareType;

/************************************************************************/
/* Panel Size X 구분을 위한 상수.			*/
/************************************************************************/


/************************************************************************/
/* ETC.			*/
/************************************************************************/

#ifdef DEF_SOURCE_SYSTEM
	const int	DEF_MAX_WORK_PER_UNIT		= 4;	//Tool4개, Carrier4개, Inspection4개
#else
	const int	DEF_MAX_WORK_PER_UNIT		= 2;	//Tool2개, Carrier2개, Inspection2개
#endif

//100807.ECID_____________
#ifdef DEF_SOURCE_SYSTEM
	const int DEF_EC_ID_CYLINDER_BASE		= 20000;
	const int DEF_EC_ID_VACUUM_BASE			= 20500;
	const int DEF_EC_ID_MOTION_BASE			= 21000;
	const int DEF_EC_ID_SYSTEM_BASE			= 22000;
#else
	const int DEF_EC_ID_CYLINDER_BASE		= 25000;
	const int DEF_EC_ID_VACUUM_BASE			= 25500;
	const int DEF_EC_ID_MOTION_BASE			= 26000;
	const int DEF_EC_ID_SYSTEM_BASE			= 27000;
#endif


// 101108 Modify. ECID 전체 개수가 1000개가 넘어서 줄임
const int DEF_MOTION_DATA_SIZE		= 8;
//const int DEF_MOTION_DATA_SIZE		= 14;
//const int DEF_MOTION_DATA_SIZE		= 16;
const int DEF_CYLINDER_DATA_SIZE		= 4;
const int DEF_VACUUM_DATA_SIZE			= 3;
//________________________

//SJ_YYK 150821 Addd
/////////////////////////////////////////
///AOC Material Port Unit ID/////////////
/////////////////////////////////////////
const int DEF_FRONT_OLB_ACF_AOC_UNIT_ID			= 1;
const int DEF_FRONT_OLB_ACF_2_AOC_UNIT_ID		= 2;
const int DEF_FRONT_TAB_IC_AOC_UNIT_ID			= 3;
const int DEF_FRONT_TAB_IC_2_AOC_UNIT_ID		= 4;
const int DEF_FRONT_MOLD_AOC_UNIT_ID			= 5;
const int DEF_FRONT_MOLD2_AOC_UNIT_ID			= 6;
const int DEF_REAR_OLB_ACF_AOC_UNIT_ID			= 7;
const int DEF_REAR_OLB_ACF_2_AOC_UNIT_ID		= 8;
const int DEF_REAR_TAB_IC_AOC_UNIT_ID			= 9;
const int DEF_REAR_TAB_IC_2_AOC_UNIT_ID			= 10;
const int DEF_REAR_MOLD_AOC_UNIT_ID				= 11;
const int DEF_REAR_MOLD2_AOC_UNIT_ID			= 12;
//_______________________________________________________

/////////////////////////////////////////////////////////////////////////////////////////////////
// Number of Reference MODEL
/////////////////////////////////////////////////////////////////////////////////////////////////
const	int	DEF_MAX_REFERENCE_MODEL			= 30;

const int DEF_MAX_ERROR_NOT_SEND = 30;


const	int DEF_MAX_THANDLER_VAC			= 3;


//171003 JSH.s
const	double DEF_ALIGN_TOLERANCE_TAB_CARRIER_X = 0.02;
const	double DEF_ALIGN_TOLERANCE_TAB_CARRIER_Y = 0.005;
const	double DEF_ALIGN_TOLERANCE_TAB_CARRIER_T = 0.002;
//171003 JSH.e

#endif // DEFSYSTEM_H
