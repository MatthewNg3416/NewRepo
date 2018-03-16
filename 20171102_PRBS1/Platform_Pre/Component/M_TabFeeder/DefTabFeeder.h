/* 
 * TabFeeder Component
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
 * DefTabFeeder.h : This file defines the value of TabFeeder component.
 *
 */

#ifndef DEF_TABFEEDER_H
#define DEF_TABFEEDER_H

#include "MTeachingInfo.h"

class MSystemData;
class IIO;
class ICylinder;
class IVacuum;
class IIndMotor;
class ISercosMultiAxis;
class IAxis;
class MPreBonderData;
class IStateTabCarrier;
class IOpPanel;
class MCtrlTabFeeder;
/************************************************************************/
/*                             Value Define                             */
/************************************************************************/

/** TabFeeder Axis Order */
const	int DEF_TABFEEDER_AXIS_MAX_NO			= 7;
const	int	DEF_TABFEEDER_PRESS_AXIS			= 0;
const	int	DEF_TABFEEDER_SPROCKET1_AXIS		= 1; 
const	int	DEF_TABFEEDER_SPROCKET2_AXIS		= 2; 
const	int	DEF_TABFEEDER_BADTABDETECTOR_AXIS	= 3; 
const	int	DEF_TABFEEDER_SUPPLIER_AXIS			= 4; 
const	int DEF_TABFEEDER_MOLDPIN_AXIS			= 5;
const	int	DEF_TABFEEDER_REEL_AXIS				= 6;

/** TabFeeder Position Define	*/
const	int DEF_PRESS_MAX_POSITION				= 3;
const	int DEF_BADTABDETECTOR_MAX_POSITION		= 2;
const	int DEF_SPROCKET_MAX_POSITION			= 1;
//const	int DEF_SPROCKET2_MAX_POSITION			= 1;
const	int DEF_SUPPLIER_MAX_POSITION			= 3;
const	int DEF_MOLDPIN_MAX_POSITION			= 2;//Mech Ver2에서만

const	int DEF_SPROCKET1_UNIT					= 1;
const	int DEF_SPROCKET2_UNIT					= 2;

/************************************************************************/
/*                           Reel Define	                           */
/************************************************************************/
//const	int DEF_REEL_ENCODER_PER_ROTATE			= 4000;
//const	int DEF_REEL_MIN_RADIUS					= 50;
//const	int DEF_REEL_MAX_RADIUS					= 250;

//#define	SPROCKET_SCALE							10.0*10000.0
//#define	DEF_SPROCKET_PITCH						4.75
// selsk_jdy
const double DEF_SPROCKET_ONE_PITCH_DIST		= 4.75;

// sprocket feeding 동작을 recovery없이 일정 횟수 이상하면 Reel 걸리는 현상이 발생함
//const	int DEF_MAX_ONETAB_COUNT				= 10;

/************************************************************************/
/*                           Position  Define                           */
/************************************************************************/

const	int	DEF_PRESS_NO_WORKING_POS			= -1;
const	int	DEF_PRESS_UP_POS					= 0;
const	int	DEF_PRESS_READY_POS					= 1;
const	int	DEF_PRESS_DOWN_POS					= 2;

const	int	DEF_SUPPLIER_NO_WORKING_POS			= -1;
const	int	DEF_SUPPLIER_READY_POS				= 0;
const	int	DEF_SUPPLIER_WORK_POS				= 1;
const	int	DEF_SUPPLIER_SUPPLY_POS				= 2;

const	int	DEF_BADTABDETECTOR_NO_WORKING_POS	= -1;
const	int	DEF_BADTABDETECTOR_READY_POS		= 0;
const	int	DEF_BADTABDETECTOR_WORK_POS			= 1;

const	int	DEF_SPROCKET1_NO_WORKING_POS		= -1;
const	int	DEF_SPROCKET1_HOME_POS				= 0;

const	int	DEF_SPROCKET2_NO_WORKING_POS		= -1;
const	int	DEF_SPROCKET2_HOME_POS				= 0;

const	int DEF_MOLDPIN_NO_WORKING_POS			= -1;
const	int DEF_MOLDPIN_RELEASE_POS				= 0;
const	int DEF_MOLDPIN_FIX_POS					= 1;

// 필요한 부분인지 확인 필요...
const	int DEF_BADTABDETECTOR_SENSOR_ALL		= -1;
/*/
const	int DEF_BADTABDETECTOR_LEFT_SENSOR_TCP	= 0;
const	int DEF_BADTABDETECTOR_LEFT_SENSOR_COF	= 1;
const	int DEF_BADTABDETECTOR_RIGHT_SENSOR_TCP	= 2;
const	int DEF_BADTABDETECTOR_RIGHT_SENSOR_COF	= 3;
const	int DEF_BADTABDETECTOR_MAX_SENSOR		= 4;
/*/
const	int DEF_BADTABDETECTOR_SENSOR_TCP		= 0;
const	int DEF_BADTABDETECTOR_SENSOR_COF		= 1;
const	int DEF_BADTABDETECTOR_MAX_SENSOR		= 2;

const	int DEF_BADTABDETECTOR_MAX_ALL			= -1;
const	int DEF_BADTABDETECTOR_AXIS				= 0;
const	int DEF_BADTABDETECTOR_MAX_AXIS			= 1;

const	int DEF_PRESS_VAC_ALL					= -1;
const	int DEF_PRESS_VAC						= 0;
const	int DEF_PRESS_MAX_VAC					= 1;

//Mech Ver2_________________________________________
typedef enum
{
	E_FEEDER_REEL_MOVE_NONE			= 0,	//동작하지 않음
//	E_FEEDER_REEL_MOVE_SUPPLY		= 1,	//Down센서를 벗어나면 Down센서 감지할때까지 공급하는 동작
//	E_FEEDER_REEL_MOVE_RECOVER		= 2,	//Up센서를 벗어나면 Up센서 감지할때까지 회수하는 동작
	E_FEEDER_FIND_DOWN				= 1,
	E_FEEDER_ESCAPE_DOWN			= 2,
	E_FEEDER_FIND_UP				= 3,
	E_FEEDER_ESCAPE_UP				= 4,
} EFeederReelMoveType;

//__________________________________________________
/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

const	int	ERR_TABFEEDER_SUCCESS				= 0;

/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/

typedef
/**
 * Referenced Component List by TabFeeder Component
 */
struct tagSTabFeederRefCompList
{
	/** MPI Lib */
//	MMPIBoard*		m_plnkMPIBoard;
	/** I/O Object */
	IIO*			m_plnkIO;

	/** Axis Object */
	IAxis*			m_plnkPressAxis;
	IAxis*			m_plnkSprocket1Axis;
	IAxis*			m_plnkSprocket2Axis;
	IAxis*			m_plnkBadTabAxis;
	IAxis*			m_plnkSupplierAxis;
	//Mech Ver2에서만_____
	IAxis*			m_plnkMoldPinAxis;
	IAxis*			m_plnkFeederReelAxis;
	//____________________

	/** SprocketT MultiMotion Object */
	ISercosMultiAxis*	m_plnkMultiSprocketAxis;

	/** Cylinder Object */
//	ICylinder*		m_plnkFeederSprocketUDCyl;
//	ICylinder*		m_plnkTensionSprocketUDCyl;
	ICylinder*		m_plnkRecoverySprocketUDCyl;
	ICylinder*		m_plnkPressReelUDCyl;	//@구버젼
	ICylinder*		m_plnkPressClampUDCyl;	//@구버젼

	ICylinder*		m_plnkAttachCuttingSupportLeftUDCyl;	//@구버젼
	ICylinder*		m_plnkAttachCuttingSupportRightUDCyl;	//@구버젼
    ICylinder*		m_plnkAttachPressUDCyl;					//@구버젼
    ICylinder*		m_plnkAttachReelCutterUDCyl;			//@구버젼
    ICylinder*		m_plnkAttachReelFBCyl;					//@구버젼
    ICylinder*		m_plnkSupplierChuckOCCyl;				//@구버젼

	/** Vac Object */
	IVacuum*		m_plnkPressVac;
	IVacuum*		m_plnkNewReelVac;						//@구버젼
	
	
} STabFeederRefCompList;

typedef
/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
struct
{
	IStateTabCarrier*	m_plnkCarrier[DEF_MAX_TABCARRIER];
	IOpPanel*			m_plnkOpPanel;

} SStateCheckCompListForTabFeeder;

//110610.KKY_____
typedef struct
{
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder;
} SCtrlCompListForTabFeeder;
//_______________

typedef
/**
 * TabFeeder Component Data	
 */ 
struct STabFeederData
{
	int					m_iGroupNo;
	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;
	/** PreBonder Data Object */
	MPreBonderData* m_plnkPreBonderData;

	/** PositionInfo.inf File 정보 구조체 */
	STeachingInfo	m_sPressTeachingInfo;
	/** Press 고정좌표 Data가 저장될 File Name */
	CString			m_strPressPosFileName;
	/** Press Offset Data가 저장될 File Name */
	CString			m_strPressOffsetPosFileName;

	STeachingInfo	m_sSupplierTeachingInfo;
	CString			m_strSupplierPosFileName;
	CString			m_strSupplierOffsetPosFileName;

	STeachingInfo	m_sBadTabTeachingInfo;
	CString			m_strBadTabPosFileName;
	CString			m_strBadTabOffsetPosFileName;

	STeachingInfo	m_sSprocket1TeachingInfo;
	CString			m_strSprocket1PosFileName;
	CString			m_strSprocket1OffsetPosFileName;

	STeachingInfo	m_sSprocket2TeachingInfo;
	CString			m_strSprocket2PosFileName;
	CString			m_strSprocket2OffsetPosFileName;

    STeachingInfo	m_sMoldPinTeachingInfo;
	CString			m_strMoldPinPosFileName;
	CString			m_strMoldPinOffsetPosFileName;

//_________________________________________________________________________________________________
// Output
	/** Mold Blow IO Address */
	unsigned short	m_usOAddrMoldBlowOn;

	/** Recovery Tension Motor On IO Address */
	unsigned short	m_usOAddrRecoveryTensionMotorOn;	//@구버젼

	/** Supplier Tension Motor On IO Address */
	unsigned short	m_usOAddrSupplierTensionMotorOn;	//@구버젼
	/** Supplier Large Tension IO Address */
	unsigned short	m_usOAddrSupplierLargeTension;		//@구버젼
	/** Supplier Middle Tension IO Address */
	unsigned short	m_usOAddrSupplierMiddleTension;		//@구버젼
	/** Supplier Small Tension IO Address */
	unsigned short	m_usOAddrSupplierSmallTension;		//@구버젼

	/** Cleaning Motor On IO Address */
	unsigned short	m_usOAddrCleaningMotorOn;			//@구버젼
	unsigned short	m_usOAddrPReelTensionMotorOn;		//@구버젼
//	unsigned short	m_usOAddrPReelReverseMotorOn;

	/** RFID Reset IO Address */
	unsigned short	m_usOAddrPressRFIDReset;
	
	//Mech Ver2에서만_____
	unsigned short	m_usOAddrSprocket1Clutch;
	unsigned short	m_usOPReelMotorPower;				//@보호지 회수부
	unsigned short	m_usOPReelMotorWind;				//@보호지 회수부
	unsigned short	m_usOPReelMotorRelease;				//@보호지 회수부
	unsigned short	m_usOPReelMotorBreak;				//@보호지 회수부
	//____________________
//_________________________________________________________________________________________________
// Input
	/** BadTab 감지를 위한 IO Address */
	unsigned short	m_usIAddrBadTabDetect[DEF_BADTABDETECTOR_MAX_SENSOR];
	
	/** Tension 추 감지를 위한 IO Address */
	unsigned short	m_usIAddrTensionUpperLimitDetect;	//@구버젼
	unsigned short	m_usIAddrTensionUpperDetect;
	unsigned short	m_usIAddrTensionLowerDetect;
	unsigned short	m_usIAddrTensionLowerLimitDetect;	//@구버젼

	/** Supplier 감지를 위한 IO Address */
	unsigned short	m_usIAddrSupplierLargeDetect;
	unsigned short	m_usIAddrSupplierMiddleDetect;
	unsigned short	m_usIAddrSupplierSmallDetect;

	/** Reel 감지를 위한 IO Address */
	unsigned short	m_usIAddrSupplierCurrentReelDetect;	//@구버젼
	unsigned short	m_usIAddrSupplierNewReelDetect;		//@구버젼

	/** Rail 감지를 위한 IO Address */
	unsigned short	m_usIAddrRailUnfoldDetect;
//	unsigned short	m_usIAddrRailDiscardReelDetect;

	// Mold 감지를 위한 IO Address
	unsigned short	m_usIAddrMoldDetect;

	// 보호 필름 감지를 위한 IO Address
	unsigned short	m_usIAddrProtectDownDetect;			//@구버젼

	// 보호 필름 회수부 Arm Up/Down센서(Mech Ver2에서 사용)
	unsigned short	m_usIAddrProtectRecoveryArmUpDetect;
	unsigned short	m_usIAddrProtectRecoveryArmDownDetect;

} STabFeederData;


#endif //DEF_TABFEEDER_H
