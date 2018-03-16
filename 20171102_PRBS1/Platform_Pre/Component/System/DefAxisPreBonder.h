#ifndef DEF_AXIS_MPI_PREBONDER_H
#define DEF_AXIS_MPI_PREBONDER_H

#include "DefSystemConfig.h"

//===================================================================
#ifdef DEF_SOURCE_SYSTEM

//____________________________________________________________________________
//161024 Add... 
const int	DEF_NMC_BOARD_ID_1							= 0;
const int	DEF_NMC_BOARD_ID_2							= 1;
const int	DEF_NMC_BOARD_ID_3							= 2;
const int	DEF_NMC_BOARD_ID_4							= 3;

const int	DEF_AXIS_NODE_MAX_NO						= 83;	// 82->83
//__________________________________________________________________________


const int	DEF_AXIS_MAX_PRIORITY					= 20;	// 우선 순위 범위

const int	DEF_AXIS_SERCOS_MAX_AXIS				= 82;//86-14;
const int	DEF_AXIS_ACS_MAX_AXIS					= 2;
const int	DEF_AXIS_SERCOS_NUM_MOTION_BOARD		= 1;	// NMC Board 갯수 

//150728_KDH
const int	DEF_AXIS_DUMMY_NUM						= 12; 		
const int	DEF_AXIS_DUMMY							= DEF_AXIS_SERCOS_MAX_AXIS + DEF_AXIS_ACS_MAX_AXIS;
//_______________

const int	DEF_AXIS_ACS_START						= DEF_AXIS_SERCOS_MAX_AXIS;
const int	DEF_AXIS_ACS_END						=  DEF_AXIS_SERCOS_MAX_AXIS + DEF_AXIS_ACS_MAX_AXIS;
const int	DEF_AXIS_MAX_AXIS						= DEF_AXIS_SERCOS_MAX_AXIS + DEF_AXIS_ACS_MAX_AXIS + DEF_AXIS_DUMMY_NUM;



// B/D #1________________________________________
const int	DEF_AXIS_SERCOS_STAGE_Y					= 0;	//1
const int	DEF_AXIS_SERCOS_STAGE_T					= 1;	//2
const int	DEF_AXIS_SERCOS_STAGE_X					= 2;	//3
const int	DEF_AXIS_SERCOS_STAGE_Z					= 3;	//5	
const int	DEF_AXIS_SERCOS_TRANSFER_IN				= 4;	//6
const int	DEF_AXIS_SERCOS_TRANSFER_OUT			= 5;	//7
const int	DEF_AXIS_SERCOS_MODEL_CHANGE_1			= 6;	//8
const int	DEF_AXIS_SERCOS_MODEL_CHANGE_2			= 7;	//9
const int	DEF_AXIS_SERCOS_MOUNTER1_X				= 8;	//10
const int	DEF_AXIS_SERCOS_MOUNTER2_X				= 9;	//11
const int	DEF_AXIS_SERCOS_MOUNTER3_X				= 10;	//12
const int	DEF_AXIS_SERCOS_MOUNTER4_X				= 11;	//13
const int	DEF_AXIS_SERCOS_MOUNTER1_Z				= 12;	//14
const int	DEF_AXIS_SERCOS_MOUNTER2_Z				= 13;	//15
const int	DEF_AXIS_SERCOS_MOUNTER3_Z				= 14;	//16
const int	DEF_AXIS_SERCOS_MOUNTER4_Z				= 15;	//17

// B/D #2________________________________________
const int	DEF_AXIS_SERCOS_BACKUP1_Z				= 16;	//18
const int	DEF_AXIS_SERCOS_BACKUP2_Z				= 17;	//19
const int	DEF_AXIS_SERCOS_BACKUP3_Z				= 18;	//20
const int	DEF_AXIS_SERCOS_BACKUP4_Z				= 19;	//21
const int	DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X	= 20;	//22
const int	DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X	= 21;	//23
const int	DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X	= 22;	//24
const int	DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X	= 23;	//25
const int	DEF_AXIS_SERCOS_R_MOUNTER1_X			= 24;	//26
const int	DEF_AXIS_SERCOS_R_MOUNTER2_X			= 25;	//27
const int	DEF_AXIS_SERCOS_R_MOUNTER3_X			= 26;	//28
const int	DEF_AXIS_SERCOS_R_MOUNTER4_X			= 27;	//29
const int	DEF_AXIS_SERCOS_R_MOUNTER1_Z			= 28;	//30
const int	DEF_AXIS_SERCOS_R_MOUNTER2_Z			= 29;	//31
const int	DEF_AXIS_SERCOS_R_MOUNTER3_Z			= 30;	//32
const int	DEF_AXIS_SERCOS_R_MOUNTER4_Z			= 31;	//33
const int	DEF_AXIS_SERCOS_R_BACKUP1_Z				= 32;	//34
const int	DEF_AXIS_SERCOS_R_BACKUP2_Z				= 33;	//35
const int	DEF_AXIS_SERCOS_R_BACKUP3_Z				= 34;	//36
const int	DEF_AXIS_SERCOS_R_BACKUP4_Z				= 35;	//37
const int	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X	= 36;	//38
const int	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X	= 37;	//39
const int	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X	= 38;	//40
const int	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X	= 39;	//41


// B/D #3________________________________________



	const int	DEF_AXIS_SERCOS_TABIC_CARRIER1_Y		= 40;	//48
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER2_Y		= 41;	//49
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER3_Y		= 42;	//50
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER4_Y		= 43;	//51	
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER1_X		= 44;	//52
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER2_X		= 45;	//53
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER3_X		= 46;	//54
	const int	DEF_AXIS_SERCOS_TABIC_CARRIER4_X		= 47;	//55

const int	DEF_AXIS_SERCOS_TABIC_CARRIER1_T		= 48;	//56
const int	DEF_AXIS_SERCOS_TABIC_CARRIER2_T		= 49;	//57
const int	DEF_AXIS_SERCOS_TABIC_CARRIER3_T		= 50;	//58
const int	DEF_AXIS_SERCOS_TABIC_CARRIER4_T		= 51;	//59

//130705.kms______
const int	DEF_AXIS_SERCOS_INSPECT_EXPAND1			= 52;
const int	DEF_AXIS_SERCOS_INSPECT_EXPAND2			= 53;
const int	DEF_AXIS_SERCOS_INSPECT_EXPAND3			= 54;
const int	DEF_AXIS_SERCOS_INSPECT_EXPAND4			= 55;
const int	DEF_AXIS_SERCOS_TABCAMERA_EXPAND		= 56;
//_________________

// B/D #4________________________________________

	

	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y		= 57;
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y		= 58;
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y		= 59;
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y		= 60;	
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X		= 61;
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X		= 62;
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X		= 63;
	const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X		= 64;

const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T		= 65;
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T		= 66;
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T		= 67;
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T		= 68;

//130705.kms________
const int	DEF_AXIS_SERCOS_R_INSPECT_EXPAND1		= 69;
const int	DEF_AXIS_SERCOS_R_INSPECT_EXPAND2		= 70;
const int	DEF_AXIS_SERCOS_R_INSPECT_EXPAND3		= 71;
const int	DEF_AXIS_SERCOS_R_INSPECT_EXPAND4		= 72;
const int	DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND		= 73;	//-->
//__________________
const int	DEF_AXIS_SERCOS_TABCAMERA_EXPAND2		= 74;
const int	DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2		= 75;	//-->
#ifdef DEF_USE_TRANSFER_CENTER
const int	DEF_AXIS_SERCOS_TRANSFER_CENTER_1		= 76;
const int   DEF_AXIS_SERCOS_TRANSFER_CENTER_2		= 77;
#else
const int	DEF_AXIS_SERCOS_SPARE_76				= 76;
const int   DEF_AXIS_SERCOS_SPARE_77				= 77;
#endif
#ifdef DEF_USE_TURN_HANDLER
const int	DEF_AXIS_SERCOS_HANDLER_Z				= 78;
const int	DEF_AXIS_SERCOS_HANDLER_T				= 79;
#else
const int	DEF_AXIS_SERCOS_SPARE_78				= 78;
const int	DEF_AXIS_SERCOS_SPARE_79				= 79;
#endif

const int   DEF_AXIS_SERCOS_TRANSFER_IN_2			= 80;

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	const int	DEF_AXIS_SERCOS_TRANSFER_OUT_2		= 81;
#else
	const int	DEF_AXIS_SERCOS_SPARE_81			= 81;
#endif

//ACF Axis_____________________________________
const int	DEF_AXIS_ACS_GANTRY_Y					= 82;
const int	DEF_AXIS_ACS_R_GANTRY_Y					= 83;


//150727_KDH
const int	DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X		= DEF_AXIS_DUMMY;
const int	DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T	= DEF_AXIS_DUMMY+1;
const int	DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T	= DEF_AXIS_DUMMY+2;
const int	DEF_AXIS_SERCOS_R_TAB_PRESS_Z			= DEF_AXIS_DUMMY+3;
const int	DEF_AXIS_SERCOS_R_FEEDER_REEL_T			= DEF_AXIS_DUMMY+4;
const int	DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z		= DEF_AXIS_DUMMY+5;

//-->
//-->
const int	DEF_AXIS_SERCOS_BADTAB_DETECTOR_X		= DEF_AXIS_DUMMY+6;	//42
const int	DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T		= DEF_AXIS_DUMMY+7;	//43
const int	DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T		= DEF_AXIS_DUMMY+8;	//44		
const int	DEF_AXIS_SERCOS_TAB_PRESS_Z				= DEF_AXIS_DUMMY+9;	//45	
const int	DEF_AXIS_SERCOS_FEEDER_REEL_T			= DEF_AXIS_DUMMY+10;	//46
const int	DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z		= DEF_AXIS_DUMMY+11;	//47
//-->
//__________________


//MultiAxis____________________________________
const int	DEF_MULTI_AXIS_MAX_AXIS					= 6;	//5->6

	const int	DEF_AXIS_SERCOS_MULTI_STAGE_YT			= 0;
#ifdef DEF_USE_TRANSFER_CENTER
	const int	DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER	= 1; //SJ_YYK 161104 Add.
#else
	const int	DEF_AXIS_SERCOS_SPARE_1					= 1; //SJ_YYK 161104 Add.
#endif
	const int   DEF_AXIS_SERCOS_MULTI_TRANFSER_IN		= 2; //SJ_YYK 161104 Add.
	const int	DEF_AXIS_SERCOS_MULTI_CARRIER_XZ		= 3;//SJ_YYK 150212 Add..
	const int	DEF_AXIS_SERCOS_MULTI_CARRIER_R_XZ		= 4;//SJ_YYK 150212 Add..

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	const int	DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT		= 5; 
#else
	const int	DEF_AXIS_SERCOS_SPARE_5					= 5; 
#endif

//150727_KDH
const int	DEF_MULTI_AXIS_DUMMY					= DEF_MULTI_AXIS_MAX_AXIS + 1;
const int	DEF_AXIS_SERCOS_MULTI_FRONT_SPROCKET	= DEF_MULTI_AXIS_DUMMY;	//-->
const int	DEF_AXIS_SERCOS_MULTI_REAR_SPROCKET		= DEF_MULTI_AXIS_DUMMY+1;	//-->
//___________________

//===================================================================

//===================================================================
#else	//#ifdef DEF_SOURCE_SYSTEM

const int	DEF_AXIS_MAX_PRIORITY					= 20;	// 우선 순위 범위

const int	DEF_AXIS_SERCOS_MAX_AXIS				= 39; //53 - 14;
const int	DEF_AXIS_ACS_MAX_AXIS					= 2;
const int	DEF_AXIS_SERCOS_NUM_MOTION_BOARD		= 3;	// NMC Board 갯수 

//150728_KDH
const int	DEF_AXIS_DUMMY_NUM						= 14;		
const int	DEF_AXIS_DUMMY							= DEF_AXIS_SERCOS_MAX_AXIS + DEF_AXIS_ACS_MAX_AXIS + 1;
//_______________

const int	DEF_AXIS_ACS_START						= DEF_AXIS_SERCOS_MAX_AXIS;
const int	DEF_AXIS_MAX_AXIS						= DEF_AXIS_SERCOS_MAX_AXIS + DEF_AXIS_ACS_MAX_AXIS + DEF_AXIS_DUMMY_NUM;

// B/D #1________________________________________
const int	DEF_AXIS_SERCOS_STAGE_Y					= 0;	//1
const int	DEF_AXIS_SERCOS_STAGE_T					= 1;	//2
const int	DEF_AXIS_SERCOS_STAGE_X					= 2;	//3
const int	DEF_AXIS_SERCOS_STAGE_Z					= 3;	//5
const int	DEF_AXIS_SERCOS_TRANSFER_OUT			= 4;	//6
const int	DEF_AXIS_SERCOS_MODEL_CHANGE_1			= 5;	//7
const int	DEF_AXIS_SERCOS_MODEL_CHANGE_2			= 6;	//8
const int	DEF_AXIS_SERCOS_MOUNTER1_X				= 7;	//9
const int	DEF_AXIS_SERCOS_MOUNTER2_X				= 8;	//10
const int	DEF_AXIS_SERCOS_MOUNTER1_Z				= 9;	//11
const int	DEF_AXIS_SERCOS_MOUNTER2_Z				= 10;	//12
const int	DEF_AXIS_SERCOS_BACKUP1_Z				= 11;	//13
const int	DEF_AXIS_SERCOS_BACKUP2_Z				= 12;	//14
const int	DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X	= 13;	//15
const int	DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X	= 14;	//16
const int	DEF_AXIS_SERCOS_R_MOUNTER1_X			= 15;	//17
const int	DEF_AXIS_SERCOS_R_MOUNTER2_X			= 16;	//18
const int	DEF_AXIS_SERCOS_R_MOUNTER1_Z			= 17;	//19
const int	DEF_AXIS_SERCOS_R_MOUNTER2_Z			= 18;	//20
const int	DEF_AXIS_SERCOS_R_BACKUP1_Z				= 19;	//21
const int	DEF_AXIS_SERCOS_R_BACKUP2_Z				= 20;	//22
const int	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X	= 21;	//23
const int	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X	= 22;	//24

// B/D #2________________________________________
const int	DEF_AXIS_SERCOS_TABIC_CARRIER1_Y		= 23;	//31
const int	DEF_AXIS_SERCOS_TABIC_CARRIER2_Y		= 24;	//32
const int	DEF_AXIS_SERCOS_TABIC_CARRIER1_X		= 25;	//33
const int	DEF_AXIS_SERCOS_TABIC_CARRIER2_X		= 26;	//34

const int	DEF_AXIS_SERCOS_TABIC_CARRIER1_T		= 27;	//35
const int	DEF_AXIS_SERCOS_TABIC_CARRIER2_T		= 28;	//36

const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y		= 29;	//43
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y		= 30;	//44
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X		= 31;	//45
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X		= 32;	//46

const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T		= 33;	//47
const int	DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T		= 34;	//48

const int	DEF_AXIS_SERCOS_INSPECT_EXPAND1			= 35;	//49
const int	DEF_AXIS_SERCOS_INSPECT_EXPAND2			= 36;	//50
const int	DEF_AXIS_SERCOS_R_INSPECT_EXPAND1		= 37;	//52
const int	DEF_AXIS_SERCOS_R_INSPECT_EXPAND2		= 38;	//53

//ACS Axis_____________________________________
const int	DEF_AXIS_ACS_GANTRY_Y					= 39;
const int	DEF_AXIS_ACS_R_GANTRY_Y					= 40;

//150728_KDH
const int	DEF_AXIS_SERCOS_BADTAB_DETECTOR_X		= DEF_AXIS_DUMMY;	//25
const int	DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T		= DEF_AXIS_DUMMY + 1;	//26
const int	DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T		= DEF_AXIS_DUMMY + 2;	//27
const int	DEF_AXIS_SERCOS_TAB_PRESS_Z				= DEF_AXIS_DUMMY + 3;	//28
const int	DEF_AXIS_SERCOS_FEEDER_REEL_T			= DEF_AXIS_DUMMY + 4;	//29
const int	DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z		= DEF_AXIS_DUMMY + 5;	//30


const int	DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X		= DEF_AXIS_DUMMY + 6;	//37
const int	DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T	= DEF_AXIS_DUMMY + 7;	//38
const int	DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T	= DEF_AXIS_DUMMY + 8;	//39
const int	DEF_AXIS_SERCOS_R_TAB_PRESS_Z			= DEF_AXIS_DUMMY + 9;	//40
const int	DEF_AXIS_SERCOS_R_FEEDER_REEL_T			= DEF_AXIS_DUMMY + 10;	//41
const int	DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z		= DEF_AXIS_DUMMY + 11;	//42

const int	DEF_AXIS_SERCOS_TABCAMERA_EXPAND		= DEF_AXIS_DUMMY + 12;	//51
const int	DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND		= DEF_AXIS_DUMMY + 13;	//54
//________________


//MultiAxis____________________________________
const int	DEF_MULTI_AXIS_MAX_AXIS					= 1;
const int	DEF_AXIS_SERCOS_MULTI_STAGE_YT			= 0;
	
//150727_KDH
const int	DEF_MULTI_AXIS_DUMMY					= DEF_MULTI_AXIS_MAX_AXIS + 1;
const int	DEF_AXIS_SERCOS_MULTI_FRONT_SPROCKET	= DEF_MULTI_AXIS_DUMMY;	//-->
const int	DEF_AXIS_SERCOS_MULTI_REAR_SPROCKET		= DEF_MULTI_AXIS_DUMMY+1;	//-->
//___________________

#endif
//===================================================================


#endif // DEF_AXIS_MPI_PREBONDER_H


