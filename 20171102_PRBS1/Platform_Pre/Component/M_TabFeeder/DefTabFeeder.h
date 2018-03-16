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
const	int DEF_MOLDPIN_MAX_POSITION			= 2;//Mech Ver2������

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

// sprocket feeding ������ recovery���� ���� Ƚ�� �̻��ϸ� Reel �ɸ��� ������ �߻���
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

// �ʿ��� �κ����� Ȯ�� �ʿ�...
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
	E_FEEDER_REEL_MOVE_NONE			= 0,	//�������� ����
//	E_FEEDER_REEL_MOVE_SUPPLY		= 1,	//Down������ ����� Down���� �����Ҷ����� �����ϴ� ����
//	E_FEEDER_REEL_MOVE_RECOVER		= 2,	//Up������ ����� Up���� �����Ҷ����� ȸ���ϴ� ����
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
	//Mech Ver2������_____
	IAxis*			m_plnkMoldPinAxis;
	IAxis*			m_plnkFeederReelAxis;
	//____________________

	/** SprocketT MultiMotion Object */
	ISercosMultiAxis*	m_plnkMultiSprocketAxis;

	/** Cylinder Object */
//	ICylinder*		m_plnkFeederSprocketUDCyl;
//	ICylinder*		m_plnkTensionSprocketUDCyl;
	ICylinder*		m_plnkRecoverySprocketUDCyl;
	ICylinder*		m_plnkPressReelUDCyl;	//@������
	ICylinder*		m_plnkPressClampUDCyl;	//@������

	ICylinder*		m_plnkAttachCuttingSupportLeftUDCyl;	//@������
	ICylinder*		m_plnkAttachCuttingSupportRightUDCyl;	//@������
    ICylinder*		m_plnkAttachPressUDCyl;					//@������
    ICylinder*		m_plnkAttachReelCutterUDCyl;			//@������
    ICylinder*		m_plnkAttachReelFBCyl;					//@������
    ICylinder*		m_plnkSupplierChuckOCCyl;				//@������

	/** Vac Object */
	IVacuum*		m_plnkPressVac;
	IVacuum*		m_plnkNewReelVac;						//@������
	
	
} STabFeederRefCompList;

typedef
/**
 * �� ����ü�� Mechanical Layer�� ��ȣ ������ ���� ���ο� �����
 * Component List�� �����ϴ� ����ü�̴�.
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
	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;
	/** PreBonder Data Object */
	MPreBonderData* m_plnkPreBonderData;

	/** PositionInfo.inf File ���� ����ü */
	STeachingInfo	m_sPressTeachingInfo;
	/** Press ������ǥ Data�� ����� File Name */
	CString			m_strPressPosFileName;
	/** Press Offset Data�� ����� File Name */
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
	unsigned short	m_usOAddrRecoveryTensionMotorOn;	//@������

	/** Supplier Tension Motor On IO Address */
	unsigned short	m_usOAddrSupplierTensionMotorOn;	//@������
	/** Supplier Large Tension IO Address */
	unsigned short	m_usOAddrSupplierLargeTension;		//@������
	/** Supplier Middle Tension IO Address */
	unsigned short	m_usOAddrSupplierMiddleTension;		//@������
	/** Supplier Small Tension IO Address */
	unsigned short	m_usOAddrSupplierSmallTension;		//@������

	/** Cleaning Motor On IO Address */
	unsigned short	m_usOAddrCleaningMotorOn;			//@������
	unsigned short	m_usOAddrPReelTensionMotorOn;		//@������
//	unsigned short	m_usOAddrPReelReverseMotorOn;

	/** RFID Reset IO Address */
	unsigned short	m_usOAddrPressRFIDReset;
	
	//Mech Ver2������_____
	unsigned short	m_usOAddrSprocket1Clutch;
	unsigned short	m_usOPReelMotorPower;				//@��ȣ�� ȸ����
	unsigned short	m_usOPReelMotorWind;				//@��ȣ�� ȸ����
	unsigned short	m_usOPReelMotorRelease;				//@��ȣ�� ȸ����
	unsigned short	m_usOPReelMotorBreak;				//@��ȣ�� ȸ����
	//____________________
//_________________________________________________________________________________________________
// Input
	/** BadTab ������ ���� IO Address */
	unsigned short	m_usIAddrBadTabDetect[DEF_BADTABDETECTOR_MAX_SENSOR];
	
	/** Tension �� ������ ���� IO Address */
	unsigned short	m_usIAddrTensionUpperLimitDetect;	//@������
	unsigned short	m_usIAddrTensionUpperDetect;
	unsigned short	m_usIAddrTensionLowerDetect;
	unsigned short	m_usIAddrTensionLowerLimitDetect;	//@������

	/** Supplier ������ ���� IO Address */
	unsigned short	m_usIAddrSupplierLargeDetect;
	unsigned short	m_usIAddrSupplierMiddleDetect;
	unsigned short	m_usIAddrSupplierSmallDetect;

	/** Reel ������ ���� IO Address */
	unsigned short	m_usIAddrSupplierCurrentReelDetect;	//@������
	unsigned short	m_usIAddrSupplierNewReelDetect;		//@������

	/** Rail ������ ���� IO Address */
	unsigned short	m_usIAddrRailUnfoldDetect;
//	unsigned short	m_usIAddrRailDiscardReelDetect;

	// Mold ������ ���� IO Address
	unsigned short	m_usIAddrMoldDetect;

	// ��ȣ �ʸ� ������ ���� IO Address
	unsigned short	m_usIAddrProtectDownDetect;			//@������

	// ��ȣ �ʸ� ȸ���� Arm Up/Down����(Mech Ver2���� ���)
	unsigned short	m_usIAddrProtectRecoveryArmUpDetect;
	unsigned short	m_usIAddrProtectRecoveryArmDownDetect;

} STabFeederData;


#endif //DEF_TABFEEDER_H
