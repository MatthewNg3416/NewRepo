#ifndef DEFACTUATORPREBONDER_H
#define DEFACTUATORPREBONDER_H

#include "DefSystemConfig.h"

//////////////////////////////////////////////////////////////////////////////
// 		               Cylinder       										//
//////////////////////////////////////////////////////////////////////////////

//100713.KKY__________________
#ifdef DEF_SOURCE_SYSTEM

//Cylinder==================================
const int DEF_CYL_ENTIRE_CYLINDER						= 37;

//150727_KDH 
const int DEF_CYL_DUMMY_NUM								= 2;
const int DEF_CYL_DUMMY									= DEF_CYL_ENTIRE_CYLINDER;
//_______________

const int DEF_CYL_MAX_CYLINDER							= DEF_CYL_ENTIRE_CYLINDER + DEF_CYL_DUMMY_NUM;

const int DEF_CYL_PANEL_TRANSFER_IN_UD						= 0;
const int DEF_CYL_PANEL_TRANSFER_OUT_UD						= 1;
const int DEF_CYL_TABMOUNTER1_PRESS_UD						= 2;
const int DEF_CYL_TABMOUNTER2_PRESS_UD						= 3;
const int DEF_CYL_TABMOUNTER3_PRESS_UD						= 4;
const int DEF_CYL_TABMOUNTER4_PRESS_UD						= 5;
const int DEF_CYL_R_TABMOUNTER1_PRESS_UD					= 6;
const int DEF_CYL_R_TABMOUNTER2_PRESS_UD					= 7;
const int DEF_CYL_R_TABMOUNTER3_PRESS_UD					= 8;
const int DEF_CYL_R_TABMOUNTER4_PRESS_UD					= 9;
const int DEF_CYL_INSPECTION_CAM1_ES						= 10;
const int DEF_CYL_INSPECTION_CAM2_ES						= 11;
const int DEF_CYL_INSPECTION_CAM3_ES						= 12;
const int DEF_CYL_INSPECTION_CAM4_ES						= 13;
const int DEF_CYL_R_INSPECTION_CAM1_ES						= 14;
const int DEF_CYL_R_INSPECTION_CAM2_ES						= 15;
const int DEF_CYL_R_INSPECTION_CAM3_ES						= 16;
const int DEF_CYL_R_INSPECTION_CAM4_ES						= 17;
const int DEF_CYL_TABCARRIER_UD								= 18;
const int DEF_CYL_TABCARRIER_UD2							= 19;
const int DEF_CYL_TABCARRIER_FB								= 20;
const int DEF_CYL_PREALIGN_CAM_ES							= 21;
const int DEF_CYL_R_TABCARRIER_UD							= 22;
const int DEF_CYL_R_TABCARRIER_UD2							= 23;
const int DEF_CYL_R_TABCARRIER_FB							= 24;	
const int DEF_CYL_R_PREALIGN_CAM_ES							= 25;
const int DEF_CYL_TABMOUNTER1_PANEL_PUSHER_UD				= 26;
const int DEF_CYL_TABMOUNTER2_PANEL_PUSHER_UD				= 27;
const int DEF_CYL_TABMOUNTER3_PANEL_PUSHER_UD				= 28;
const int DEF_CYL_TABMOUNTER4_PANEL_PUSHER_UD				= 29;
const int DEF_CYL_RTABMOUNTER1_PANEL_PUSHER_UD				= 30;
const int DEF_CYL_RTABMOUNTER2_PANEL_PUSHER_UD				= 31;
const int DEF_CYL_RTABMOUNTER3_PANEL_PUSHER_UD				= 32;
const int DEF_CYL_RTABMOUNTER4_PANEL_PUSHER_UD				= 33;
const int DEF_CYL_TRANSFER_IN_AIRRAIL_FB1					= 34;
const int DEF_CYL_TRANSFER_IN_AIRRAIL_FB2					= 35;
const int DEF_CYL_PANEL_TRANSFER_CENTER_UD					= 36;

const int DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD			= DEF_CYL_DUMMY;	//-->
const int DEF_CYL_R_TABPRESS1_RECOVERY_SPROCKET_UD			= DEF_CYL_DUMMY + 1;	//-->


//Vacuum====================================
const int DEF_CYL_ENTIRE_VACUUM								= 28;

//150727_KDH
const int DEF_VAC_DUMMY_NUM									= 2;
const int DEF_VAC_DUMMY										= DEF_CYL_ENTIRE_VACUUM;
//_____________

const int DEF_VAC_MAX_VACUUM								= DEF_CYL_ENTIRE_VACUUM + DEF_VAC_DUMMY_NUM;


const int DEF_VAC_PANEL_ALIGNER1							= 0;
const int DEF_VAC_PANEL_ALIGNER2							= 1;
const int DEF_VAC_PANEL_ALIGNER3							= 2;
const int DEF_VAC_PANEL_TRANSFER_IN							= 3;
const int DEF_VAC_PANEL_TRANSFER_IN2						= 4;
const int DEF_VAC_PANEL_TRANSFER_OUT						= 5;
const int DEF_VAC_PANEL_TRANSFER_OUT2						= 6;
const int DEF_VAC_TAB_MOUNTER1								= 7;
const int DEF_VAC_TAB_MOUNTER2								= 8;
const int DEF_VAC_TAB_MOUNTER3								= 9;
const int DEF_VAC_TAB_MOUNTER4								= 10;
const int DEF_VAC_R_TAB_MOUNTER1							= 11;
const int DEF_VAC_R_TAB_MOUNTER2							= 12;
const int DEF_VAC_R_TAB_MOUNTER3							= 13;
const int DEF_VAC_R_TAB_MOUNTER4							= 14;
const int DEF_VAC_TABCARRIER1								= 15;
const int DEF_VAC_TABCARRIER2								= 16;
const int DEF_VAC_TABCARRIER3								= 17;
const int DEF_VAC_TABCARRIER4								= 18;
const int DEF_VAC_R_TABCARRIER1								= 19;
const int DEF_VAC_R_TABCARRIER2								= 20;
const int DEF_VAC_R_TABCARRIER3								= 21;
const int DEF_VAC_R_TABCARRIER4								= 22;
const int DEF_VAC_PANEL_TRANSFER_CENTER						= 23;
const int DEF_VAC_PANEL_TRANSFER_CENTER2					= 24;
const int DEF_VAC_TLHANDLER_1								= 25;
const int DEF_VAC_TLHANDLER_2								= 26;
const int DEF_VAC_TLHANDLER_3								= 27;

const int DEF_VAC_TAB_PRESS1								= DEF_VAC_DUMMY;	//-->
const int DEF_VAC_TAB_PRESS2								= DEF_VAC_DUMMY + 1;	//-->
/*/
const int DEF_VAC_PANEL_ALIGNER1							= 0;
const int DEF_VAC_PANEL_ALIGNER2							= 1;
const int DEF_VAC_PANEL_ALIGNER3							= 2;
const int DEF_VAC_PANEL_TRANSFER_CENTER						= 3;
const int DEF_VAC_PANEL_TRANSFER_IN							= 4;
const int DEF_VAC_PANEL_TRANSFER_OUT						= 5;
const int DEF_VAC_TAB_MOUNTER1								= 6;
const int DEF_VAC_TAB_MOUNTER2								= 7;
const int DEF_VAC_TAB_MOUNTER3								= 8;
const int DEF_VAC_TAB_MOUNTER4								= 9;
const int DEF_VAC_TAB_MOUNTER5								= 10;
const int DEF_VAC_TAB_MOUNTER6								= 11;
const int DEF_VAC_TAB_MOUNTER7								= 12;
const int DEF_VAC_TAB_MOUNTER8								= 13;
const int DEF_VAC_TABCARRIER1								= 14;
const int DEF_VAC_TABCARRIER2								= 15;
const int DEF_VAC_TABCARRIER3								= 16;
const int DEF_VAC_TABCARRIER4								= 17;
const int DEF_VAC_R_TABCARRIER1								= 18;
const int DEF_VAC_R_TABCARRIER2								= 19;
const int DEF_VAC_R_TABCARRIER3								= 20;
const int DEF_VAC_R_TABCARRIER4								= 21;
const int DEF_VAC_TAB_PRESS1								= 22;
const int DEF_VAC_TAB_PRESS1_NEW_REEL						= 23;
const int DEF_VAC_TAB_PRESS2								= 24;
const int DEF_VAC_TAB_PRESS2_NEW_REEL						= 25;
/*/

//____________________________

#else		//#ifdef DEF_SOURCE_SYSTEM

//Cylinder==================================
#ifndef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	const int DEF_CYL_ENTIRE_CYLINDER							= 19;
#else
	const int DEF_CYL_ENTIRE_CYLINDER							= 21;
#endif

//150727_KDH
const int DEF_CYL_DUMMY_NUM									= 2;
const int DEF_CYL_DUMMY										= DEF_CYL_ENTIRE_CYLINDER+1;
//______________

const int DEF_CYL_MAX_CYLINDER								= DEF_CYL_ENTIRE_CYLINDER + DEF_CYL_DUMMY_NUM;


const int DEF_CYL_PANEL_TRANSFER_OUT_UD						= 0;
const int DEF_CYL_TABMOUNTER1_PRESS_UD						= 1;
const int DEF_CYL_TABMOUNTER2_PRESS_UD						= 2;
const int DEF_CYL_R_TABMOUNTER1_PRESS_UD					= 3;
const int DEF_CYL_R_TABMOUNTER2_PRESS_UD					= 4;
const int DEF_CYL_INSPECTION_CAM1_ES						= 5;
const int DEF_CYL_INSPECTION_CAM2_ES						= 6;
const int DEF_CYL_R_INSPECTION_CAM1_ES						= 7;
const int DEF_CYL_R_INSPECTION_CAM2_ES						= 8;
const int DEF_CYL_TABCARRIER_UD								= 9;
const int DEF_CYL_TABCARRIER_FB								= 10;
const int DEF_CYL_PREALIGN_CAM_ES							= 11;
const int DEF_CYL_R_TABCARRIER_UD							= 12;
const int DEF_CYL_R_TABCARRIER_FB							= 13;
const int DEF_CYL_R_PREALIGN_CAM_ES							= 14;
const int DEF_CYL_TABMOUNTER1_PANEL_PUSHER_UD				= 15;
const int DEF_CYL_TABMOUNTER2_PANEL_PUSHER_UD				= 16;
const int DEF_CYL_RTABMOUNTER1_PANEL_PUSHER_UD				= 17;
const int DEF_CYL_RTABMOUNTER2_PANEL_PUSHER_UD				= 18;

#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	const int DEF_CYL_MODELCHANGE_AIRRAIL_FB1				= 19;
	const int DEF_CYL_MODELCHANGE_AIRRAIL_FB2				= 20;
#endif

const int DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD			= DEF_CYL_DUMMY;
const int DEF_CYL_R_TABPRESS1_RECOVERY_SPROCKET_UD			= DEF_CYL_DUMMY + 1;
/*/
const int DEF_CYL_PANEL_TRANSFER_OUT_UD						= 0;
const int DEF_CYL_PANEL_ROTATE_TR							= 1;	//Turn Return
const int DEF_CYL_PANEL_ROTATE_UD							= 2;
const int DEF_CYL_TABMOUNTER1_PRESS_UD						= 3;
const int DEF_CYL_TABMOUNTER2_PRESS_UD						= 4;
const int DEF_CYL_TABMOUNTER3_PRESS_UD						= 5;
const int DEF_CYL_TABMOUNTER4_PRESS_UD						= 6;
const int DEF_CYL_R_TABMOUNTER1_PRESS_UD					= 7;
const int DEF_CYL_R_TABMOUNTER2_PRESS_UD					= 8;
const int DEF_CYL_R_TABMOUNTER3_PRESS_UD					= 9;
const int DEF_CYL_R_TABMOUNTER4_PRESS_UD					= 10;
const int DEF_CYL_TABCARRIER_UD								= 11;
const int DEF_CYL_TABCARRIER_FB								= 12;
const int DEF_CYL_R_TABCARRIER_UD							= 13;
const int DEF_CYL_R_TABCARRIER_FB							= 14;

const int DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD			= 15;
const int DEF_CYL_TABPRESS1_PRESS_REEL_UD					= 16;
const int DEF_CYL_TABPRESS1_PRESS_REEL_CLAMP_UD				= 17;
const int DEF_CYL_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_UD	= 18;
const int DEF_CYL_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_UD	= 19;
const int DEF_CYL_TABPRESS1_ATTACH_PRESS_UD					= 20;
const int DEF_CYL_TABPRESS1_ATTACH_REEL_CUTTER_UD			= 21;
const int DEF_CYL_TABPRESS1_ATTACH_REEL_FB					= 22;
const int DEF_CYL_TABPRESS1_ATTACH_SUPPLIER_CHUCK_ES		= 23;

const int DEF_CYL_TABPRESS2_RECOVERY_SPROCKET_UD			= 24;
const int DEF_CYL_TABPRESS2_PRESS_REEL_UD					= 25;
const int DEF_CYL_TABPRESS2_PRESS_REEL_CLAMP_UD				= 26;
const int DEF_CYL_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_UD	= 27;
const int DEF_CYL_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_UD	= 28;
const int DEF_CYL_TABPRESS2_ATTACH_PRESS_UD					= 29;
const int DEF_CYL_TABPRESS2_ATTACH_REEL_CUTTER_UD			= 30;
const int DEF_CYL_TABPRESS2_ATTACH_REEL_FB					= 31;
const int DEF_CYL_TABPRESS2_ATTACH_SUPPLIER_CHUCK_ES		= 32;

const int DEF_CYL_PREALIGN_CAM_ES							= 33;
const int DEF_CYL_R_PREALIGN_CAM_ES							= 34;

const int DEF_CYL_INSPECTION_CAM1_ES						= 35;
const int DEF_CYL_INSPECTION_CAM2_ES						= 36;
const int DEF_CYL_INSPECTION_CAM3_ES						= 37;
const int DEF_CYL_INSPECTION_CAM4_ES						= 38;

const int DEF_CYL_R_INSPECTION_CAM1_ES						= 39;
const int DEF_CYL_R_INSPECTION_CAM2_ES						= 40;
const int DEF_CYL_R_INSPECTION_CAM3_ES						= 41;
const int DEF_CYL_R_INSPECTION_CAM4_ES						= 42;
/*/


//Vacuum====================================
const int DEF_CYL_ENTIRE_VACUUM								= 13;

//150727_KDH
const int DEF_VAC_DUMMY_NUM									= 2;
const int DEF_VAC_DUMMY										= DEF_CYL_ENTIRE_VACUUM+1;
//____________

const int DEF_VAC_MAX_VACUUM								= DEF_CYL_ENTIRE_VACUUM + DEF_VAC_DUMMY_NUM;

const int DEF_VAC_PANEL_ALIGNER1							= 0;
const int DEF_VAC_PANEL_ALIGNER2							= 1;
const int DEF_VAC_PANEL_ALIGNER3							= 2;
const int DEF_VAC_PANEL_TRANSFER_OUT						= 3;
const int DEF_VAC_PANEL_TRANSFER_OUT2						= 4;
const int DEF_VAC_TAB_MOUNTER1								= 5;
const int DEF_VAC_TAB_MOUNTER2								= 6;
const int DEF_VAC_R_TAB_MOUNTER1							= 7;
const int DEF_VAC_R_TAB_MOUNTER2							= 8;
const int DEF_VAC_TABCARRIER1								= 9;
const int DEF_VAC_TABCARRIER2								= 10;
const int DEF_VAC_R_TABCARRIER1								= 11;
const int DEF_VAC_R_TABCARRIER2								= 12;



const int DEF_VAC_TAB_PRESS1								= DEF_VAC_DUMMY;
const int DEF_VAC_TAB_PRESS2								= DEF_VAC_DUMMY + 1;

/*/
const int DEF_VAC_PANEL_ALIGNER1							= 0;
const int DEF_VAC_PANEL_ALIGNER2							= 1;
const int DEF_VAC_PANEL_ALIGNER3							= 2;
const int DEF_VAC_PANEL_ROTATE								= 3;
const int DEF_VAC_PANEL_TRANSFER_OUT						= 4;
const int DEF_VAC_TAB_MOUNTER1								= 5;
const int DEF_VAC_TAB_MOUNTER2								= 6;
const int DEF_VAC_TAB_MOUNTER3								= 7;
const int DEF_VAC_TAB_MOUNTER4								= 8;
const int DEF_VAC_R_TAB_MOUNTER1							= 9;
const int DEF_VAC_R_TAB_MOUNTER2							= 10;
const int DEF_VAC_R_TAB_MOUNTER3							= 11;
const int DEF_VAC_R_TAB_MOUNTER4							= 12;
const int DEF_VAC_TABCARRIER1								= 13;
const int DEF_VAC_TABCARRIER2								= 14;
const int DEF_VAC_TABCARRIER3								= 15;
const int DEF_VAC_TABCARRIER4								= 16;
const int DEF_VAC_R_TABCARRIER1								= 17;
const int DEF_VAC_R_TABCARRIER2								= 18;
const int DEF_VAC_R_TABCARRIER3								= 19;
const int DEF_VAC_R_TABCARRIER4								= 20;
const int DEF_VAC_TAB_PRESS1								= 21;
const int DEF_VAC_TAB_PRESS1_NEW_REEL						= 22;
const int DEF_VAC_TAB_PRESS2								= 23;
const int DEF_VAC_TAB_PRESS2_NEW_REEL						= 24;
/*/
#endif

#endif //DEFACTUATORPREBONDER_H

