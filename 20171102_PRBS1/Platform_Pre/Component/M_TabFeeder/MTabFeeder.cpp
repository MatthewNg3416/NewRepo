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
 * TabFeeder.cpp : Implementation of TabFeeder component.
 */

#include "stdafx.h"
#include "MSystemData.h"
#include "IIO.h"
#include "ICylinder.h"
#include "IVacuum.h"
#include "IAxis.h"
#include "ISercosMultiAxis.h"
#include "MTabFeeder.h"
#include "MTeachingPosData.h"
#include <math.h>
#include "MPreBonderData.h"
#include "IOpPanel.h"
#include "MCtrlTabFeeder.h"
#include "MTrsAutoManager.h"

// hongju_SESL
#include "DefIOAddrPreBonder.h"

#include "common.h"

#include "DefAxisPreBonder.h"
#include "MPlatformOlbSystem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;
/** 
 * ������
 */
MTabFeeder::MTabFeeder()
{
	// jdy
	m_bSprocket1ReverseRotation = FALSE;
}

/** 
 * ������
 *
 * @param commonData : ObjectI�� Log�� ���� Component Common Data
 * @param listRefComponent : ������ Component ������ ����Ʈ
 * @param datComponent : TabFeeder Data
 */
MTabFeeder::MTabFeeder(SCommonAttribute commonData, STabFeederRefCompList listRefComponents, STabFeederData datComponent)
: ICommon(commonData)
{
	m_iSprocketRotateCount = 0;

	// ������ǥ ���.
	for (int i = 0 ; i < DEF_PRESS_MAX_POSITION; i++)
	{
		m_rgsPressFixedPos[i]		= 0.0;
		m_rgsPressOffsetPos[i]		= 0.0;
		m_rgsPressModelPos[i]		= 0.0;
	}
	
	for (int i = 0 ; i < DEF_SUPPLIER_MAX_POSITION; i++)
	{
		m_rgsSupplierFixedPos[i]	= 0.0;
		m_rgsSupplierOffsetPos[i]	= 0.0;
		m_rgsSupplierModelPos[i]	= 0.0;
	}

	// ����ǥ ���.
	for (int i = 0 ; i < DEF_BADTABDETECTOR_MAX_AXIS; i++)
	{
		m_rgsBadTabOffsetPos[i]		= 0.0;
		m_rgsBadTabModelPos[i]		= 0.0;
	}

	// ����ǥ ���.
	for (int i = 0 ; i < DEF_SPROCKET_MAX_POSITION; i++)
	{
		m_rgsSprocket1OffsetPos[i]	= 0.0;
		m_rgsSprocket1ModelPos[i]	= 0.0;
	}

	// ����ǥ ���.
	for (int i = 0 ; i < DEF_SPROCKET_MAX_POSITION; i++)
	{
		m_rgsSprocket2OffsetPos[i]	= 0.0;
		m_rgsSprocket2ModelPos[i]	= 0.0;
	}

	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION; i++)
	{
		m_rgsMoldPinFixedPos[i]		= 0.0;
		m_rgsMoldPinOffsetPos[i]	= 0.0;
		m_rgsMoldPinModelPos[i]		= 0.0;
	}

	// jdy
	m_bSprocket1ReverseRotation = FALSE;

	int iResult	= AssignComponents(listRefComponents);
	if (iResult)
	{
		//SetErrorLevel(_T("TAB FEEDER creation"),2, __FILE__, __LINE__);
		//WriteErrorLog("MTabFeeder() : AssignComponent Error!");
	}

	iResult = SetData(datComponent);
	if (iResult)
	{
		//SetErrorLevel(_T("TAB FEEDER creation"),2, __FILE__, __LINE__);
		//WriteErrorLog("MTabFeeder() : SetData Error!");
	}

	LoadPressFixedPosParameter();
	LoadPressOffsetPosParameter();
	calculatePressModelPosParameter();

	LoadSupplierFixedPosParameter();
	calculateSupplierModelPosParameter();

	m_strBadTabOffsetPosFilePath.Format(_T("%s\\%s"),
										m_plnkSystemData->m_strModelFileDir,
										m_plnkSystemData->m_strModelFileName);
	LoadBadTabOffsetPosParameter();
	calculateBadTabModelPosParameter();

	m_strSprocket1OffsetPosFilePath.Format(_T("%s\\%s"),
											m_plnkSystemData->m_strModelFileDir,
											m_plnkSystemData->m_strModelFileName);
	LoadSprocket1OffsetPosParameter();
	calculateSprocket1ModelPosParameter();

	m_strSprocket2OffsetPosFilePath.Format(_T("%s\\%s"),
											m_plnkSystemData->m_strModelFileDir,
											m_plnkSystemData->m_strModelFileName);
	LoadSprocket2OffsetPosParameter();
	calculateSprocket2ModelPosParameter();


	m_strMoldPinOffsetPosFilePath.Format(_T("%s\\%s"),
											m_plnkSystemData->m_strModelFileDir,
											m_plnkSystemData->m_strModelFileName);
	LoadMoldPinFixedPosParameter();	
	LoadMoldPinOffsetPosParameter();
}

/**
 * �Ҹ���
 */
MTabFeeder::~MTabFeeder()
{
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/

/**
 * Press Model ��ǥ�� ����Ѵ�..
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::calculatePressModelPosParameter()
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * Supplier Model ��ǥ�� ����Ѵ�..
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::calculateSupplierModelPosParameter()
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * BadTab Model ��ǥ�� ����Ѵ�..
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::calculateBadTabModelPosParameter()
{
	for (int i = 0 ; i < DEF_BADTABDETECTOR_MAX_AXIS; i++)
		m_rgsBadTabModelPos[DEF_BADTABDETECTOR_WORK_POS] = 0.0;

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * Sprocket1 Model ��ǥ�� ����Ѵ�..
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::calculateSprocket1ModelPosParameter()
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * Sprocket2 Model ��ǥ�� ����Ѵ�..
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::calculateSprocket2ModelPosParameter()
{
	return ERR_TABFEEDER_SUCCESS;
}


/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

/**
 * TabFeeder�� Data Parameter�� �����Ѵ�. 
 *
 * @param	STabFeederParam : ������ TabFeeder Parameter
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SetData(STabFeederData datcomponent)
{
	m_iGroupNo						= datcomponent.m_iGroupNo;
	m_plnkSystemData				= datcomponent.m_plnkSystemData;
	m_plnkPreBonderData				= datcomponent.m_plnkPreBonderData;

	m_sPressTeachingInfo			= datcomponent.m_sPressTeachingInfo;
	m_strPressPosFileName			= datcomponent.m_strPressPosFileName;
	m_strPressOffsetPosFileName		= datcomponent.m_strPressOffsetPosFileName;
	m_strPressOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	m_sSupplierTeachingInfo			= datcomponent.m_sSupplierTeachingInfo;
	m_strSupplierPosFileName		= datcomponent.m_strSupplierPosFileName;
	m_strSupplierOffsetPosFileName	= datcomponent.m_strSupplierOffsetPosFileName;
	m_strSupplierOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	m_sBadTabTeachingInfo			= datcomponent.m_sBadTabTeachingInfo;
	m_strBadTabPosFileName			= datcomponent.m_strBadTabPosFileName;
	m_strBadTabOffsetPosFileName	= datcomponent.m_strBadTabOffsetPosFileName;
	m_strBadTabOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	m_sSprocket1TeachingInfo		= datcomponent.m_sSprocket1TeachingInfo;
	m_strSprocket1PosFileName		= datcomponent.m_strSprocket1PosFileName;
	m_strSprocket1OffsetPosFileName = datcomponent.m_strSprocket1OffsetPosFileName;
	m_strSprocket1OffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	m_sSprocket2TeachingInfo		= datcomponent.m_sSprocket2TeachingInfo;
	m_strSprocket2PosFileName		= datcomponent.m_strSprocket2PosFileName;
	m_strSprocket2OffsetPosFileName = datcomponent.m_strSprocket2OffsetPosFileName;
	m_strSprocket2OffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	m_sMoldPinTeachingInfo			= datcomponent.m_sMoldPinTeachingInfo;
	m_strMoldPinPosFileName			= datcomponent.m_strMoldPinPosFileName;
	m_strMoldPinOffsetPosFileName	= datcomponent.m_strMoldPinOffsetPosFileName;
	m_strMoldPinOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	for (int i = 0 ; i < DEF_BADTABDETECTOR_MAX_SENSOR; i++)
		m_usIAddrBadTabDetect[i] = datcomponent.m_usIAddrBadTabDetect[i];

	m_usIAddrTensionUpperLimitDetect= datcomponent.m_usIAddrTensionUpperLimitDetect;
	m_usIAddrTensionUpperDetect		= datcomponent.m_usIAddrTensionUpperDetect;
	m_usIAddrTensionLowerDetect		= datcomponent.m_usIAddrTensionLowerDetect;
	m_usIAddrTensionLowerLimitDetect= datcomponent.m_usIAddrTensionLowerLimitDetect;
	m_usOAddrCleaningMotorOn		= datcomponent.m_usOAddrCleaningMotorOn;
	m_usOAddrPReelTensionMotorOn	= datcomponent.m_usOAddrPReelTensionMotorOn;
//	m_usOAddrPReelReverseMotorOn	= datcomponent.m_usOAddrPReelReverseMotorOn;
	m_usOAddrPressRFIDReset			= datcomponent.m_usOAddrPressRFIDReset;
	m_usOAddrRecoveryTensionMotorOn	= datcomponent.m_usOAddrRecoveryTensionMotorOn;
	m_usOAddrMoldBlowOn				= datcomponent.m_usOAddrMoldBlowOn;
	m_usOAddrSupplierLargeTension	= datcomponent.m_usOAddrSupplierLargeTension;
	m_usOAddrSupplierMiddleTension	= datcomponent.m_usOAddrSupplierMiddleTension;
	m_usOAddrSupplierSmallTension	= datcomponent.m_usOAddrSupplierSmallTension;
	m_usOAddrSupplierTensionMotorOn	= datcomponent.m_usOAddrSupplierTensionMotorOn;

	m_usIAddrRailUnfoldDetect		= datcomponent.m_usIAddrRailUnfoldDetect;
//	m_usIAddrRailDiscardReelDetect	= datcomponent.m_usIAddrRailDiscardReelDetect;
	m_usIAddrSupplierCurrentReelDetect = datcomponent.m_usIAddrSupplierCurrentReelDetect;
	m_usIAddrSupplierNewReelDetect	= datcomponent.m_usIAddrSupplierNewReelDetect;
	m_usIAddrMoldDetect				= datcomponent.m_usIAddrMoldDetect;

	m_usIAddrProtectDownDetect		= datcomponent.m_usIAddrProtectDownDetect;

	m_usOAddrSprocket1Clutch			= datcomponent.m_usOAddrSprocket1Clutch;
	m_usIAddrProtectRecoveryArmUpDetect = datcomponent.m_usIAddrProtectRecoveryArmUpDetect;
	m_usIAddrProtectRecoveryArmDownDetect = datcomponent.m_usIAddrProtectRecoveryArmDownDetect;

	m_usOPReelMotorPower			= datcomponent.m_usOPReelMotorPower;
	m_usOPReelMotorWind				= datcomponent.m_usOPReelMotorWind;
	m_usOPReelMotorRelease			= datcomponent.m_usOPReelMotorRelease;
	m_usOPReelMotorBreak			= datcomponent.m_usOPReelMotorBreak;

	m_usIAddrSupplierSmallDetect	= datcomponent.m_usIAddrSupplierSmallDetect;
	m_usIAddrSupplierMiddleDetect	= datcomponent.m_usIAddrSupplierMiddleDetect;
	m_usIAddrSupplierLargeDetect	= datcomponent.m_usIAddrSupplierLargeDetect;

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Component List�� �����Ѵ�.
 *
 * @param	STabFeederRefCompList : ������ TabFeeder Component List
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::AssignComponents(STabFeederRefCompList TabFeederCompList)
{
	m_plnkIO						= TabFeederCompList.m_plnkIO;

	m_plnkPressAxis					= TabFeederCompList.m_plnkPressAxis;
	m_plnkSupplierAxis				= TabFeederCompList.m_plnkSupplierAxis;
	m_plnkMoldPinAxis				= TabFeederCompList.m_plnkMoldPinAxis;
	m_plnkFeederReelAxis			= TabFeederCompList.m_plnkFeederReelAxis;
	m_plnkBadTabAxis				= TabFeederCompList.m_plnkBadTabAxis;
	m_plnkSprocket1Axis				= TabFeederCompList.m_plnkSprocket1Axis;
	m_plnkSprocket2Axis				= TabFeederCompList.m_plnkSprocket2Axis;
	m_plnkMultiSprocketAxis			= TabFeederCompList.m_plnkMultiSprocketAxis;
	
//	m_plnkFeederSprocketUDCyl		= TabFeederCompList.m_plnkFeederSprocketUDCyl;
//	m_plnkTensionSprocketUDCyl		= TabFeederCompList.m_plnkTensionSprocketUDCyl;
	m_plnkRecoverySprocketUDCyl		= TabFeederCompList.m_plnkRecoverySprocketUDCyl;
	m_plnkPressReelUDCyl			= TabFeederCompList.m_plnkPressReelUDCyl;
	m_plnkPressClampUDCyl			= TabFeederCompList.m_plnkPressClampUDCyl;

	m_plnkAttachCuttingSupportLeftUDCyl	= TabFeederCompList.m_plnkAttachCuttingSupportLeftUDCyl;
	m_plnkAttachCuttingSupportRightUDCyl= TabFeederCompList.m_plnkAttachCuttingSupportRightUDCyl;
    m_plnkAttachPressUDCyl			= TabFeederCompList.m_plnkAttachPressUDCyl;
    m_plnkAttachReelCutterUDCyl		= TabFeederCompList.m_plnkAttachReelCutterUDCyl;
    m_plnkAttachReelFBCyl			= TabFeederCompList.m_plnkAttachReelFBCyl;
    m_plnkSupplierChuckOCCyl		= TabFeederCompList.m_plnkSupplierChuckOCCyl;
//    m_plnkReelRecoveryUDCyl			= TabFeederCompList.m_plnkReelRecoveryUDCyl;
//    m_plnkReelRecoveryFBCyl			= TabFeederCompList.m_plnkReelRecoveryFBCyl;

	m_plnkPressVac					= TabFeederCompList.m_plnkPressVac;			
	m_plnkNewReelVac				= TabFeederCompList.m_plnkNewReelVac;			

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� State Ȯ���� Component List�� �����Ѵ�.
 *
 * @param	sCompList : ������ State Ȯ���� ���� Component List
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::AssignStateCheckComponents(SStateCheckCompListForTabFeeder sCompList)
{
//	m_plnkCarrier = sCompList.m_plnkCarrier;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		m_plnkCarrier[i] = sCompList.m_plnkCarrier[i];
	m_plnkOpPanel = sCompList.m_plnkOpPanel;
	return ERR_TABFEEDER_SUCCESS;
}

//110610.KKY_______________
int MTabFeeder::AssignCtrlComponents(SCtrlCompListForTabFeeder sCompList)
{
	m_plnkCtrlTabFeeder = sCompList.m_plnkCtrlTabFeeder;
	return ERR_TABFEEDER_SUCCESS;
}
//_________________________

/**
 * TabFeeder�� Motion Component�� ���� �޴´�.
 *
 * @param nAxisID : �� ID (0: Press ��, 1: Sprocket �� , 2: Bad Tab Detect ��)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
IAxis* MTabFeeder::GetMotionComponent(int nAxisID)
{
	// Motion Component �� ID�� ����
	switch (nAxisID)
	{
	case DEF_TABFEEDER_PRESS_AXIS:
		return m_plnkPressAxis;

	case DEF_TABFEEDER_SUPPLIER_AXIS:
		return m_plnkSupplierAxis;

	case DEF_TABFEEDER_BADTABDETECTOR_AXIS :
		return m_plnkBadTabAxis;

	case DEF_TABFEEDER_SPROCKET1_AXIS :
		return m_plnkSprocket1Axis;

	case DEF_TABFEEDER_SPROCKET2_AXIS :
		return m_plnkSprocket2Axis;

	case DEF_TABFEEDER_MOLDPIN_AXIS:
		return m_plnkMoldPinAxis;
	case DEF_TABFEEDER_REEL_AXIS:
		return m_plnkFeederReelAxis;

	default :					// �� ID ����
		return NULL;
	}
}

/**
 * Model�� �������� TabFeeder�� �˸���.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::ChangeModel(void)
{
	int iResult = ERR_TABFEEDER_SUCCESS;

	m_strBadTabOffsetPosFilePath.Format(_T("%s\\%s"),
		m_plnkSystemData->m_strModelFileDir,
		m_plnkSystemData->m_strModelFileName);

	m_strSprocket1OffsetPosFilePath.Format(_T("%s\\%s"),
		m_plnkSystemData->m_strModelFileDir,
		m_plnkSystemData->m_strModelFileName);

	m_strSprocket2OffsetPosFilePath.Format(_T("%s\\%s"),
		m_plnkSystemData->m_strModelFileDir,
		m_plnkSystemData->m_strModelFileName);

	m_strMoldPinOffsetPosFilePath.Format(_T("%s\\%s"),
		m_plnkSystemData->m_strModelFileDir,
		m_plnkSystemData->m_strModelFileName);	

	//130424.kms_________
	iResult = LoadPressOffsetPosParameter();
	if (iResult != ERR_TABFEEDER_SUCCESS)
		return iResult;
	calculatePressModelPosParameter();
	//___________________

	// Press�� Offset ��ǥ Load
/*	iResult = LoadPressOffsetPosParameter();
	if (iResult != ERR_TABFEEDER_SUCCESS)
		return iResult;
	calculatePressModelPosParameter();
*/
	// Supplier�� Offset ��ǥ Load
/*	iResult = LoadSupplierOffsetPosParameter();
	if (iResult != ERR_TABFEEDER_SUCCESS)
		return iResult;
	calculateSupplierModelPosParameter();
*/
	// BadTab�� Offset ��ǥ Load
	iResult = LoadBadTabOffsetPosParameter();
	if (iResult)
		return iResult;
	calculateBadTabModelPosParameter();

	iResult = LoadSprocket1OffsetPosParameter();
	if (iResult)
		return iResult;
	calculateSprocket1ModelPosParameter();

	iResult = LoadSprocket2OffsetPosParameter();
	if (iResult)
		return iResult;
	calculateSprocket2ModelPosParameter();


	iResult = LoadMoldPinOffsetPosParameter();
	if (iResult)
		return iResult;
	return ERR_TABFEEDER_SUCCESS;
}

/************************************************************************/
/*        Teaching ���� ���� Interface                                  */
/************************************************************************/
//_________________________________________________________________________________________________________
// Press ���� �Լ�
/**
 * TabFeeder�� Press �� ���� ������ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadPressFixedPosParameter(void)
{
	MTeachingPosData TabFeederPressFixPosDat(m_sPressTeachingInfo, m_strPressPosFileName);

	BOOL bResult = TabFeederPressFixPosDat.ReadData();

	for (int i = 0; i < DEF_PRESS_MAX_POSITION ; i++)
		m_rgsPressFixedPos[i] = TabFeederPressFixPosDat.m_TeachingPos[i][0];

	if (!bResult)
		return generateErrorCode(215001);	// Tab Feeder Press Z �� ������ǥ Load �ϴµ� �����Ͽ����ϴ�.

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Press �� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadPressOffsetPosParameter(void)
{
	BOOL bResult = 0;

	MTeachingPosData TabFeederPressOffsetPosDat(m_sPressTeachingInfo, 
												m_strPressOffsetPosFileName, 
												m_strPressOffsetPosFilePath);

	// Offset Pos
//KKY.Del	BOOL bResult = TabFeederPressOffsetPosDat.ReadData();	
	bResult = TabFeederPressOffsetPosDat.ReadData();	

	/*
	for (int i = 0; i < DEF_PRESS_MAX_POSITION ; i++)
		//KKY.Modify_____
		m_rgsPressOffsetPos[i] = TabFeederPressOffsetPosDat.m_TeachingPos[i][0];
//KKY.Del		m_rgsPressOffsetPos[i] = 0.0;
		//_______________
		*/

	m_rgsPressOffsetPos[0] = 0.0;
	m_rgsPressOffsetPos[1] = 0.0;
	m_rgsPressOffsetPos[2] = TabFeederPressOffsetPosDat.m_TeachingPos[2][0];

	if (!bResult)
		return generateErrorCode(215002);	//Tab Feeder Press Z �� Offset ��ǥ Load �ϴµ� �����Ͽ����ϴ�.
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Press �� ���� ������ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsFixedPos : ������ ������ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SavePressFixedPosParameter(double* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

	MTeachingPosData TabFeederPressFixPosDat(m_sPressTeachingInfo, m_strPressPosFileName);
	
	for (int i = 0; i < DEF_PRESS_MAX_POSITION ; i++)
	{
		m_rgsPressFixedPos[i] = rgsFixedPos[i];
		TabFeederPressFixPosDat.m_TeachingPos[i][0] = m_rgsPressFixedPos[i];
	}

	BOOL bResult = TabFeederPressFixPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215003);	// Tab Feeder Press Z �� ���� ��ǥ �����ϴµ� �����Ͽ����ϴ�.

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Press �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SavePressOffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	MTeachingPosData TabFeederPressOffsetPosDat(m_sPressTeachingInfo, 
												m_strPressOffsetPosFileName, 
												m_strPressOffsetPosFilePath);

	// Offset Pos 
	for (int i = 0; i < DEF_PRESS_MAX_POSITION ; i++)
	{
		m_rgsPressOffsetPos[i] = rgsOffsetPos[i];
		TabFeederPressOffsetPosDat.m_TeachingPos[i][0] = m_rgsPressOffsetPos[i];
	}

	BOOL bResult = TabFeederPressOffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215004);	// Tab Feeder Press Z �� Offset ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Press �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgsFixedPos : ������ǥ �迭 (Z)
 * @param	rgsOffsetPos : Offset��ǥ �迭 (Z)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::GetPressPosParameter(double* rgsFixedPos, double* rgsOffsetPos)
{
//	ASSERT(rgsFixedPos != NULL && rgsOffsetPos != NULL);

	for (int i = 0; i < DEF_PRESS_MAX_POSITION; i++)
	{
		if (rgsFixedPos != NULL)
			rgsFixedPos[i] = m_rgsPressFixedPos[i];

		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i] = m_rgsPressOffsetPos[i];
	}
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder Press Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MTabFeeder::GetPressTeachingInfo()
{
	return m_sPressTeachingInfo;
}

//_________________________________________________________________________________________________________
// Supplier ���� �Լ�
/**
 * TabFeeder�� Supplier �� ���� ������ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadSupplierFixedPosParameter(void)
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Supplier �� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadSupplierOffsetPosParameter(void)
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Supplier �� ���� ������ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsFixedPos : ������ ������ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SaveSupplierFixedPosParameter(double* rgsFixedPos)
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Supplier �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SaveSupplierOffsetPosParameter(double* rgsOffsetPos)
{
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Supplier �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgsFixedPos : ������ǥ �迭 (Z)
 * @param	rgsOffsetPos : Offset��ǥ �迭 (Z)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::GetSupplierPosParameter(double* rgsFixedPos, double* rgsOffsetPos)
{
//	ASSERT(rgsFixedPos != NULL && rgsOffsetPos != NULL);

	for (int i = 0; i < DEF_SUPPLIER_MAX_POSITION; i++)
	{
		if (rgsFixedPos != NULL)
			rgsFixedPos[i] = m_rgsSupplierFixedPos[i];

		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i] = m_rgsSupplierOffsetPos[i];
	}
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder Supplier Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MTabFeeder::GetSupplierTeachingInfo()
{
	return m_sSupplierTeachingInfo;
}

//_________________________________________________________________________________________________________
// BadTab ���� �Լ�
/**
 * TabFeeder�� BadTab �� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadBadTabOffsetPosParameter()
{
	BOOL bResult = FALSE;
	int i = 0;
	MTeachingPosData TabFeederBadTabOffsetPosDat(	m_sBadTabTeachingInfo,
													m_strBadTabOffsetPosFileName,
													m_strBadTabOffsetPosFilePath);

	bResult = TabFeederBadTabOffsetPosDat.ReadData();
	if (!bResult)
		return generateErrorCode(215005); // Bad Tab Z�� Offset ��ǥ Load�ϴµ� �����Ͽ����ϴ�.


	for (int i = 0; i < DEF_BADTABDETECTOR_MAX_POSITION; i++)
		m_rgsBadTabOffsetPos[i] = TabFeederBadTabOffsetPosDat.m_TeachingPos[i][0];

	return ERR_TABFEEDER_SUCCESS;
}


/**
 * TabFeeder�� BadTab �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SaveBadTabOffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	MTeachingPosData TabFeederBadTabOffsetPosDat(m_sBadTabTeachingInfo, 
												m_strBadTabOffsetPosFileName, 
												m_strBadTabOffsetPosFilePath);

	// Offset Pos 
	for (int i = 0; i < DEF_BADTABDETECTOR_MAX_POSITION; i++)
	{
		m_rgsBadTabOffsetPos[i] = rgsOffsetPos[i];
		TabFeederBadTabOffsetPosDat.m_TeachingPos[i][0] = m_rgsBadTabOffsetPos[i];
	}

	BOOL bResult = TabFeederBadTabOffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215006); // Bad Tab Z��  Offset ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� BadTab �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	iPosition : ������ Offset Position
 * @param	rgsOffsetPos : ������ Offset��ǥ ��
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SaveBadTabOffsetPosParameter(int iPosition, double dOffsetPos)
{
	MTeachingPosData TabFeederBadTabOffsetPosDat(m_sBadTabTeachingInfo, 
												m_strBadTabOffsetPosFileName, 
												m_strBadTabOffsetPosFilePath);

	// Offset Pos 
	m_rgsBadTabOffsetPos[iPosition] = dOffsetPos;
	TabFeederBadTabOffsetPosDat.m_TeachingPos[iPosition][0] = m_rgsBadTabOffsetPos[iPosition];

	BOOL bResult = TabFeederBadTabOffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215006); // Bad Tab Z��  Offset ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� BadTab �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgsFixedPos : ������ǥ �迭 (X, Y, T)
 * @param	rgsOffsetPos : Offset��ǥ �迭 (X, Y, T)
 * @param	rgsModelPos : Model��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::GetBadTabPosParameter(double* rgsOffsetPos, double* rgsModelPos)
{
//	ASSERT((rgsFixedPos != NULL) && (rgsOffsetPos != NULL) && (rgsModelPos != NULL));

	for (int i=0; i<DEF_BADTABDETECTOR_MAX_POSITION; i++)
	{
		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i] = m_rgsBadTabOffsetPos[i];

		if (rgsModelPos != NULL)
			rgsModelPos[i] = m_rgsBadTabModelPos[i];
	}

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder BadTab Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MTabFeeder::GetBadTabTeachingInfo()
{
	return m_sBadTabTeachingInfo;
}

//_________________________________________________________________________________________________________
// Sprocket 1 ���� �Լ�
/**
 * TabFeeder�� Sprocket T�� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadSprocket1OffsetPosParameter(void)
{
#if FALSE	//KKY.�̻��
	MTeachingPosData TabFeederSprocket1OffsetPosDat(m_sSprocket1TeachingInfo,
													m_strSprocket1OffsetPosFileName,
													m_strSprocket1OffsetPosFilePath);
	// Offset Pos
	BOOL bResult = TabFeederSprocket1OffsetPosDat.ReadData();
	for (int i = 0; i < DEF_SPROCKET_MAX_POSITION ; i++)
	{
		m_rgsSprocket1OffsetPos[i] = TabFeederSprocket1OffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
	{
		//WriteErrorLog("LoadSprocket1OffsetPosParameter() : Loading TabFeeder Sprocket1 Offset Pos Parameter Error : Reading Failure");
		SetErrorLevel(_T("TAB FEEDER load Offset Pos Parameter(Sprocket1 Axis)"),2, __FILE__, __LINE__);
		return generateErrorCode(215007); // Bad Tab Y�� Offset ��ǥ Load�ϴµ� �����Ͽ����ϴ�.
	}
#endif

	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Sprocket1 �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SaveSprocket1OffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	MTeachingPosData TabFeederSprocket1OffsetPosDat(m_sSprocket1TeachingInfo,
													m_strSprocket1OffsetPosFileName,
													m_strSprocket1OffsetPosFilePath);
	// Offset Pos 
	for (int i = 0; i < DEF_SPROCKET_MAX_POSITION; i++)
	{
		m_rgsSprocket1OffsetPos[i] = rgsOffsetPos[i];
		TabFeederSprocket1OffsetPosDat.m_TeachingPos[i][0] = m_rgsSprocket1OffsetPos[i];
	}

	BOOL bResult = TabFeederSprocket1OffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215008); // Bad Tab Y��  Offset ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Sprocket1 �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgsFixedPos : ������ǥ �迭 (X, Y, T)
 * @param	rgsOffsetPos : Offset��ǥ �迭 (X, Y, T)
 * @param	rgsModelPos : Model��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::GetSprocket1PosParameter(double* rgsOffsetPos, double* rgsModelPos)
{
	for (int i=0; i<DEF_SPROCKET_MAX_POSITION; i++)
	{
		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i] = m_rgsSprocket1OffsetPos[i];

		if (rgsModelPos != NULL)
			rgsModelPos[i] = m_rgsSprocket1ModelPos[i];
	}
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder Sprocket1 Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MTabFeeder::GetSprocket1TeachingInfo()
{
	return m_sSprocket1TeachingInfo;
}

//_________________________________________________________________________________________________________
// Sprocket 2 ���� �Լ�
/**
 * TabFeeder�� Sprocket �� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::LoadSprocket2OffsetPosParameter(void)
{
#if FALSE	//KKY.�̻��
	MTeachingPosData TabFeederSprocket2OffsetPosDat(m_sSprocket2TeachingInfo,
													m_strSprocket2OffsetPosFileName,
													m_strSprocket2OffsetPosFilePath);
	// Offset Pos
	BOOL bResult = TabFeederSprocket2OffsetPosDat.ReadData();
	for (int i = 0; i < DEF_SPROCKET_MAX_POSITION ; i++)
	{
		m_rgsSprocket2OffsetPos[i] = TabFeederSprocket2OffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
	{
		//WriteErrorLog("LoadSprocket2OffsetPosParameter() : Loading TabFeeder Sprocket2 Offset Pos Parameter Error : Reading Failure");
		SetErrorLevel(_T("TAB FEEDER load Offset Pos Parameter(Sprocket2 Axis)"),2, __FILE__, __LINE__);
		return generateErrorCode(215007); // Bad Tab Y�� Offset ��ǥ Load�ϴµ� �����Ͽ����ϴ�.
	}
#endif
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Sprocket2 �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SaveSprocket2OffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	MTeachingPosData TabFeederSprocket2OffsetPosDat(m_sSprocket2TeachingInfo,
													m_strSprocket2OffsetPosFileName,
													m_strSprocket2OffsetPosFilePath);
	// Offset Pos 
	for (int i = 0; i < DEF_SPROCKET_MAX_POSITION; i++)
	{
		m_rgsSprocket2OffsetPos[i] = rgsOffsetPos[i];
		TabFeederSprocket2OffsetPosDat.m_TeachingPos[i][0] = m_rgsSprocket2OffsetPos[i];
	}

	BOOL bResult = TabFeederSprocket2OffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215008); // Bad Tab Y��  Offset ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Sprocket2 �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgsFixedPos : ������ǥ �迭 (X, Y, T)
 * @param	rgsOffsetPos : Offset��ǥ �迭 (X, Y, T)
 * @param	rgsModelPos : Model��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::GetSprocket2PosParameter(double* rgsOffsetPos, double* rgsModelPos)
{
	for (int i=0; i<DEF_SPROCKET_MAX_POSITION; i++)
	{
		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i] = m_rgsSprocket2OffsetPos[i];

		if (rgsModelPos != NULL)
			rgsModelPos[i] = m_rgsSprocket2ModelPos[i];
	}
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder Sprocket2 Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MTabFeeder::GetSprocket2TeachingInfo()
{
	return m_sSprocket2TeachingInfo;
}


int MTabFeeder::LoadMoldPinFixedPosParameter(void)
{
	MTeachingPosData MoldPinFixPosDat(m_sMoldPinTeachingInfo, m_strMoldPinPosFileName);

	BOOL bResult = MoldPinFixPosDat.ReadData();

	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION ; i++)
		m_rgsMoldPinFixedPos[i] = MoldPinFixPosDat.m_TeachingPos[i][0];

	if (!bResult)
		return generateErrorCode(215032);	// Tab Feeder Press Z �� ������ǥ Load �ϴµ� �����Ͽ����ϴ�.

	return ERR_TABFEEDER_SUCCESS;
}

int MTabFeeder::LoadMoldPinOffsetPosParameter(void)
{
	/*
	MTeachingPosData MoldPinOffsetPosDat(m_sMoldPinTeachingInfo, 
										m_strMoldPinOffsetPosFileName,
										m_strMoldPinOffsetPosFilePath);

	BOOL bResult = MoldPinOffsetPosDat.ReadData();

	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION ; i++)
		m_rgsMoldPinOffsetPos[i] = MoldPinOffsetPosDat.m_TeachingPos[i][0];

	if (!bResult)
	{
		SetErrorLevel(_T("TAB FEEDER load Offset Pos Parameter(Press Axis)"),2, __FILE__, __LINE__);
		return generateErrorCode(215033);	// Tab Feeder Press Z �� ������ǥ Load �ϴµ� �����Ͽ����ϴ�.
	}
	*/

	return ERR_TABFEEDER_SUCCESS;
}

int MTabFeeder::SaveMoldPinFixedPosParameter(double* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

	MTeachingPosData MoldPinFixPosDat(m_sMoldPinTeachingInfo, m_strMoldPinPosFileName);
	
	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION ; i++)
	{
		m_rgsMoldPinFixedPos[i] = rgsFixedPos[i];
		MoldPinFixPosDat.m_TeachingPos[i][0] = m_rgsMoldPinFixedPos[i];
	}

	BOOL bResult = MoldPinFixPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215034);	// Tab Feeder Press Z �� ���� ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	return ERR_TABFEEDER_SUCCESS;
}

int MTabFeeder::SaveMoldPinOffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	MTeachingPosData MoldPinOffsetPosDat(m_sMoldPinTeachingInfo, 
										m_strMoldPinOffsetPosFileName,
										m_strMoldPinOffsetPosFilePath);

	// Offset Pos 
	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION ; i++)
	{
		m_rgsMoldPinOffsetPos[i] = rgsOffsetPos[i];
		MoldPinOffsetPosDat.m_TeachingPos[i][0] = m_rgsMoldPinOffsetPos[i];
	}

	BOOL bResult = MoldPinOffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(215035);	// Tab Feeder Press Z �� Offset ��ǥ �����ϴµ� �����Ͽ����ϴ�.
	return ERR_TABFEEDER_SUCCESS;
}

int MTabFeeder::GetMoldPinPosParameter(double* rgsFixedPos, double* rgsOffsetPos)
{
	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION; i++)
	{
		if (rgsFixedPos != NULL)
			rgsFixedPos[i] = m_rgsMoldPinFixedPos[i];

		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i] = m_rgsMoldPinOffsetPos[i];
	}
	return ERR_TABFEEDER_SUCCESS;
}

STeachingInfo MTabFeeder::GetMoldPinTeachingInfo()
{
	return m_sMoldPinTeachingInfo;
}


/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/

//_________________________________________________________________________________________________________
// Press Vacuum ���� �Լ�

/**
 * Press Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::CheckPressVacuum()
{
	// Vacuum�� IO Senser Check
	int iResult = ERR_TABFEEDER_SUCCESS;
	
	iResult = m_plnkPressVac->CompareIO();
	if (iResult)
		return iResult;

	return iResult;
}

/**
 * Press Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::AbsorbPress(BOOL bSkipSensor)
{
	if (TRUE == IsAbsorbPress())
		return ERR_TABFEEDER_SUCCESS;

	ASSERT(m_plnkPressVac != NULL);

	WriteTactTimeLog("[MTabFeeder] [M] [Press Vacuum Absorb] [Start]");
	
	int iResult = m_plnkPressVac->On(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabFeeder] [M] [Press Vacuum Absorb] [End]");
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * Press Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::ReleasePress(BOOL bSkipSensor)
{
	ASSERT(m_plnkPressVac != NULL);
	
	int iResult = ERR_TABFEEDER_SUCCESS;
	
	WriteTactTimeLog("[MTabFeeder] [M] [Press Vacuum Release] [Start]");
	
	iResult = m_plnkPressVac->Off(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabFeeder] [M] [Press Vacuum Absorb] [End]");
	
	return iResult;
}

/**
 * TabFeeder�� Vacuum ������¸� Ȯ���Ѵ�.
 *
 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
 */
BOOL MTabFeeder::IsAbsorbPress()
{
	ASSERT(m_plnkPressVac != NULL);

	return m_plnkPressVac->IsOn();
}

/**
 * TabFeeder�� Vacuum �������¸� Ȯ���Ѵ�.
 *
 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
 */
BOOL MTabFeeder::IsReleasePress()
{
	ASSERT(m_plnkPressVac != NULL);

	return m_plnkPressVac->IsOff();
}

/**
 * New Reel Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::CheckNewReelVacuum()
{
	// Vacuum�� IO Senser Check
	int iResult = ERR_TABFEEDER_SUCCESS;
	/*/
	iResult = m_plnkNewReelVac->CompareIO();
	if (iResult)
	{
		SetErrorLevel(_T("TAB FEEDER check New Reel Vacuum"),2, __FILE__, __LINE__);
		return iResult;
	}
	/*/
	return iResult;
}

/**
 * New Reel Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::AbsorbNewReel(BOOL bSkipSensor)
{
	/*/
	ASSERT(m_plnkNewReelVac != NULL);
	
	int iResult = ERR_TABFEEDER_SUCCESS;
	
	WriteTactTimeLog("[MTabFeeder] [M] [New Reel Vacuum Absorb] [Start]");
	
	iResult = m_plnkNewReelVac->On(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("(New Reel)Vacuum Absorb action error");
		SetErrorLevel(_T("TAB FEEDER absorb New Reel Vacuum"),2, __FILE__, __LINE__);
		return iResult;
	}
	
	WriteTactTimeLog("[MTabFeeder] [M] [New Reel Vacuum Absorb] [End]");
	
	return iResult;
	/*/
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * New Reel Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::ReleaseNewReel(BOOL bSkipSensor)
{
	/*/
	ASSERT(m_plnkNewReelVac != NULL);

	int iResult = ERR_TABFEEDER_SUCCESS;
	
	WriteTactTimeLog("[MTabFeeder] [M] [New Reel Vacuum Release] [Start]");
	
	iResult = m_plnkNewReelVac->Off(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("(New Reel)Vacuum Release action error");
		SetErrorLevel(_T("TAB FEEDER release New Reel Vacuum"),2, __FILE__, __LINE__);
		return iResult;
	}
	
	WriteTactTimeLog("[MTabFeeder] [M] [New Reel Vacuum Absorb] [End]");
	
	return iResult;
	/*/
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * TabFeeder�� Vacuum ������¸� Ȯ���Ѵ�.
 *
 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
 */
BOOL MTabFeeder::IsAbsorbNewReel()
{
	/*/
	ASSERT(m_plnkNewReelVac != NULL);

	return m_plnkNewReelVac->IsOn();
	/*/
	return FALSE;
}

/**
 * TabFeeder�� Vacuum �������¸� Ȯ���Ѵ�.
 *
 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
 */
BOOL MTabFeeder::IsReleaseNewReel()
{
	/*/
	ASSERT(m_plnkNewReelVac != NULL);

	return m_plnkNewReelVac->IsOff();
	/*/
	return TRUE;
}


/************************************************************************/
/*        Cylinder Operation Interface                                    */
/************************************************************************/

//_________________________________________________________________________________________________________
// 
/**
 * Feeder Sprocket Cylinder�� ��½�Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
/*
int MTabFeeder::OpenFeederSprocketCyl(BOOL bSkipSensor)
{
#ifdef DEF_USE_FEEDER_SPROCKET_UD_CYL
	ASSERT(m_plnkFeederSprocketUDCyl != NULL);
	if (TRUE == m_plnkFeederSprocketUDCyl->IsUp())
		return ERR_TABFEEDER_SUCCESS;
	
	WriteTactTimeLog("[MTabFeeder] [M] [Feeder Sprocket Up] [Start]");
	
	int iResult = m_plnkFeederSprocketUDCyl->Up(bSkipSensor);
	if (iResult)
	{
		SetErrorLevel(_T("TAB FEEDER up Feeder Sprocket Cylinder"),2, __FILE__, __LINE__);
		return iResult;
	}
	
	WriteTactTimeLog("[MTabFeeder] [M] [Feeder Sprocket Up] [End]");
#endif
	return ERR_TABFEEDER_SUCCESS;
}
*/

/**
 * Feeder Sprocket Cylinder�� �ϰ���Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
/*
int MTabFeeder::CloseFeederSprocketCyl(BOOL bSkipSensor)
{
	int iResult = ERR_TABFEEDER_SUCCESS;
#ifdef DEF_USE_FEEDER_SPROCKET_UD_CYL
	ASSERT(m_plnkFeederSprocketUDCyl != NULL);
	if (!m_plnkFeederSprocketUDCyl->IsDown())
	{
		WriteTactTimeLog("[MTabFeeder] [M] [Feeder Sprocket Down] [Start]");
		
		iResult = m_plnkFeederSprocketUDCyl->Down(bSkipSensor);
		if (iResult)
		{
			//WriteErrorLog("Feeder Sprocket Down Action Error");
			SetErrorLevel(_T("TAB FEEDER down Feeder Sprocket Cylinder"),2, __FILE__, __LINE__);
			return iResult;
		}
		
		WriteTactTimeLog("[MTabFeeder] [M] [Feeder Sprocket Down] [End]");
	}
#endif
	return iResult;
}
*/
/**
 * Feeder Sprocket Cyl ��µ� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Up, FALSE = �� ��)
 */
/*
BOOL MTabFeeder::IsOpenFeederSprocketCyl(void)
{
#ifdef DEF_USE_FEEDER_SPROCKET_UD_CYL
	ASSERT(m_plnkFeederSprocketUDCyl);
	return m_plnkFeederSprocketUDCyl->IsUp();
#else
	return FALSE;
#endif
}
*/
/**
 * Feeder Sprocket Cyl �ϰ��� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Down, FALSE = �� ��)
 */
/*
BOOL MTabFeeder::IsCloseFeederSprocketCyl(void)
{
#ifdef DEF_USE_FEEDER_SPROCKET_UD_CYL
	ASSERT(m_plnkFeederSprocketUDCyl);
	return m_plnkFeederSprocketUDCyl->IsDown();
#else
	return FALSE;
#endif
}
*/
/**
 * Tension Sprocket Cylinder�� ��½�Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
/*
int MTabFeeder::OpenTensionSprocketCyl(BOOL bSkipSensor)
{
#ifdef DEF_USE_TENSION_SPROCKET_UD_CYL
	ASSERT(m_plnkTensionSprocketUDCyl != NULL);

	if (m_plnkTensionSprocketUDCyl->IsUp())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Tension Sprocket Up] [Start]");

	int iResult = m_plnkTensionSprocketUDCyl->Up(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("Tension Sprocket Up Action Error");
		SetErrorLevel(_T("TAB FEEDER up Tension Sprocket Cylinder"),2, __FILE__, __LINE__);
		return iResult;
	}

	WriteTactTimeLog("[MTabFeeder] [M] [Tension Sprocket Up] [End]");
#endif
	return ERR_TABFEEDER_SUCCESS;
}
*/
/**
 * Tension Sprocket Cylinder�� �ϰ���Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
/*
int MTabFeeder::CloseTensionSprocketCyl(BOOL bSkipSensor)
{
#ifdef DEF_USE_TENSION_SPROCKET_UD_CYL
	ASSERT(m_plnkTensionSprocketUDCyl != NULL);

	if (m_plnkTensionSprocketUDCyl->IsDown())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Tension Sprocket Down] [Start]");

	int iResult = m_plnkTensionSprocketUDCyl->Down(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("Tension Sprocket Down Action Error");
		SetErrorLevel(_T("TAB FEEDER down Sprocket Sprocket Cylinder"),2, __FILE__, __LINE__);
		return iResult;
	}

	WriteTactTimeLog("[MTabFeeder] [M] [Tension Sprocket Down] [End]");
#endif
	return ERR_TABFEEDER_SUCCESS;
}
*/
/**
 * Tension Sprocket Cyl ��µ� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Up, FALSE = �� ��)
 */
/*
BOOL MTabFeeder::IsOpenTensionSprocketCyl(void)
{
#ifdef DEF_USE_TENSION_SPROCKET_UD_CYL
	ASSERT(m_plnkTensionSprocketUDCyl);
	return m_plnkTensionSprocketUDCyl->IsUp();
#else
	return FALSE;
#endif
}
*/
/**
 * Tension Sprocket Cyl �ϰ��� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Down, FALSE = �� ��)
 */
/*
BOOL MTabFeeder::IsCloseTensionSprocketCyl(void)
{
#ifdef DEF_USE_TENSION_SPROCKET_UD_CYL
	ASSERT(m_plnkTensionSprocketUDCyl);
	return m_plnkTensionSprocketUDCyl->IsDown();
#else
	return FALSE;
#endif
}
*/
/**
 * Recovery Sprocket Cylinder�� ��½�Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
int MTabFeeder::OpenRecoverySprocketCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkRecoverySprocketUDCyl != NULL);

	if (m_plnkRecoverySprocketUDCyl->IsUp())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Recovery Sprocket Up] [Start]");

	int iResult = m_plnkRecoverySprocketUDCyl->Up(bSkipSensor);
	if (iResult)
		return iResult;

	WriteTactTimeLog("[MTabFeeder] [M] [Recovery Sprocket Up] [End]");
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * Recovery Sprocket Cylinder�� �ϰ���Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
int MTabFeeder::CloseRecoverySprocketCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkRecoverySprocketUDCyl != NULL);

	if (m_plnkRecoverySprocketUDCyl->IsDown())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Recovery Sprocket Down] [Start]");

	int iResult = m_plnkRecoverySprocketUDCyl->Down(bSkipSensor);
	if (iResult)
		return iResult;

	WriteTactTimeLog("[MTabFeeder] [M] [Recovery Sprocket Down] [End]");
	return iResult;
}

/**
 * Recovery Sprocket Cyl ��µ� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Up, FALSE = �� ��)
 */
BOOL MTabFeeder::IsOpenRecoverySprocketCyl(void)
{
	ASSERT(m_plnkRecoverySprocketUDCyl);
	return m_plnkRecoverySprocketUDCyl->IsUp();
}

/**
 * Recovery Sprocket Cyl �ϰ��� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Down, FALSE = �� ��)
 */
BOOL MTabFeeder::IsCloseRecoverySprocketCyl(void)
{
	ASSERT(m_plnkRecoverySprocketUDCyl);
	return m_plnkRecoverySprocketUDCyl->IsDown();
}


/**
 * Supplier chuck Cylinder�� Open��Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
int MTabFeeder::UnlockSupplierChuckCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkSupplierChuckOCCyl != NULL);

	if (m_plnkSupplierChuckOCCyl->IsOpen())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Supplier Chuck Open] [Start]");

	int iResult = m_plnkSupplierChuckOCCyl->Open(bSkipSensor);
	if (iResult)
		return iResult;

	WriteTactTimeLog("[MTabFeeder] [M] [Supplier Chuck Open] [End]");
	return iResult;
}

/**
 * Supplier Chuck Cyl Close��Ų��.
 *
 * @param	BOOL bSkipSensor (���� �� Sensor Ȯ���� skip �� �������� ����, default = FALSE)
 * @return	int (0 = Success, Error Code = �� ��) 
 */
int MTabFeeder::LockSupplierChuckCyl(BOOL bSkipSensor)
{
	/*/
	ASSERT(m_plnkSupplierChuckOCCyl != NULL);

	if (m_plnkSupplierChuckOCCyl->IsClose())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Supplier Chuck Close] [Start]");

	int iResult = m_plnkSupplierChuckOCCyl->Close(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("Supplier Chuck Close Action Error");
		SetErrorLevel(_T("TAB FEEDER close Supplier Chuck Cylinder"),2, __FILE__, __LINE__);
		return iResult;
	}

	WriteTactTimeLog("[MTabFeeder] [M] [Supplier Chuck Close] [End]");
	return iResult;
	/*/
	return ERR_TABFEEDER_SUCCESS;
}

/**
 * Supplier Chuck Cyl Open�� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Lock, FALSE = �� ��)
 */
BOOL MTabFeeder::IsUnlockSupplierChuckCyl(void)
{
	ASSERT(m_plnkSupplierChuckOCCyl);
	return m_plnkSupplierChuckOCCyl->IsOpen();
}

/**
 * Supplier Chuck Cyl Close�� �������� Ȯ���Ѵ�.
 *
 * @param	void
 * @return	BOOL (TRUE = Unlock, FALSE = �� ��)
 */
BOOL MTabFeeder::IsLockSupplierChuckCyl(void)
{
	ASSERT(m_plnkSupplierChuckOCCyl);
	return m_plnkSupplierChuckOCCyl->IsClose();
}

/*
int MTabFeeder::UpReelRecoveryCyl(BOOL bSkipSensor)
{
	if (NULL == m_plnkReelRecoveryUDCyl)
		return ERR_TABFEEDER_SUCCESS;

	if (m_plnkReelRecoveryUDCyl->IsUp())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Up] [Start]");

	int iResult = m_plnkReelRecoveryUDCyl->Up(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("Reel Recovery Up action error");
		SetErrorLevel(_T("TAB FEEDER check Chuck Cylinder Close"),2, __FILE__, __LINE__);
		return iResult;
	}

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Up] [End]");
	return iResult;
}

int MTabFeeder::DownReelRecoveryCyl(BOOL bSkipSensor)
{
	if (NULL == m_plnkReelRecoveryUDCyl)
		return ERR_TABFEEDER_SUCCESS;

	if (m_plnkReelRecoveryUDCyl->IsDown())
		return ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Down] [Start]");

	int iResult = m_plnkReelRecoveryUDCyl->Down(bSkipSensor);
	if (iResult)
	{
		//WriteErrorLog("Reel Recovery Down action error");
		SetErrorLevel(_T("TAB FEEDER down Reel Recovery Cylinder"),2, __FILE__, __LINE__);
		return iResult;
	}

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Down] [End]");
	return ERR_TABFEEDER_SUCCESS;
}

BOOL MTabFeeder::IsUpReelRecoveryCyl()
{
	if (NULL == m_plnkReelRecoveryUDCyl)
		return FALSE;

	return m_plnkReelRecoveryUDCyl->IsUp();
}

BOOL MTabFeeder::IsDownReelRecoveryCyl()
{
	if (NULL == m_plnkReelRecoveryUDCyl)
		return FALSE;

	return m_plnkReelRecoveryUDCyl->IsDown();
}

int MTabFeeder::ForwardReelRecoveryCyl(BOOL bSkipSensor)
{
	if (NULL == m_plnkReelRecoveryFBCyl)
		return ERR_TABFEEDER_SUCCESS;

	int iResult = ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Forward] [Start]");

	if (FALSE == m_plnkReelRecoveryFBCyl->IsFront())
	{
		iResult = m_plnkReelRecoveryFBCyl->Front(bSkipSensor);
		if (iResult)
		{
			//WriteErrorLog("Reel Recovery Forward Action Error");
			SetErrorLevel(_T("TAB FEEDER forward Reel Recoveery Cylinder"),2, __FILE__, __LINE__);
			return iResult;
		}
	}
	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Forward] [End]");
	return iResult;
}

int MTabFeeder::BackwardReelRecoveryCyl(BOOL bSkipSensor)
{
	if (NULL == m_plnkReelRecoveryFBCyl)
		return ERR_TABFEEDER_SUCCESS;

	int iResult = ERR_TABFEEDER_SUCCESS;

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Backward] [Start]");

	if (FALSE == m_plnkReelRecoveryFBCyl->IsBack())
	{
		iResult = m_plnkReelRecoveryFBCyl->Back(bSkipSensor);
		if (iResult)
		{
			//WriteErrorLog("Reel Recovery Backward Action Error");
			SetErrorLevel(_T("TAB FEEDER backward Reel Recoveery Cylinder"),2, __FILE__, __LINE__);
			return iResult;
		}
	}

	WriteTactTimeLog("[MTabFeeder] [M] [Reel Recovery Backward] [End]");
	return iResult;
}

BOOL MTabFeeder::IsForwardReelRecoveryCyl()
{
	if (NULL == m_plnkReelRecoveryFBCyl)
		return FALSE;

	return m_plnkReelRecoveryFBCyl->IsFront();
}

BOOL MTabFeeder::IsBackwardReelRecoveryCyl()
{
	if (NULL == m_plnkReelRecoveryFBCyl)
		return FALSE;

	return m_plnkReelRecoveryFBCyl->IsBack();
}
*/

BOOL MTabFeeder::IsDetectProtectRecoveryArmUp()
{
	return m_plnkIO->IsOn(m_usIAddrProtectRecoveryArmUpDetect);
}

BOOL MTabFeeder::IsDetectProtectRecoveryArmDown()
{
	return m_plnkIO->IsOn(m_usIAddrProtectRecoveryArmDownDetect);
}

BOOL MTabFeeder::IsClutchOnSprocket1Axis()
{
	return m_plnkIO->IsOn(m_usOAddrSprocket1Clutch);
}

int MTabFeeder::ClutchOnSprocket1Axis()
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	if (FALSE == m_plnkIO->IsOn(m_usOAddrSprocket1Clutch))
	{
		iResult = m_plnkIO->OutputOn(m_usOAddrSprocket1Clutch);
		m_plnkIO->UpdateIO();
//		Sleep(10);
	}
	return iResult;
}

int MTabFeeder::ClutchOffSprocket1Axis()
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	if (FALSE == m_plnkIO->IsOff(m_usOAddrSprocket1Clutch))
	{
		iResult = m_plnkIO->OutputOff(m_usOAddrSprocket1Clutch);
		m_plnkIO->UpdateIO();
//		Sleep(10);
	}
	return iResult;
}

//110630______________
int MTabFeeder::PReelBreakOnOff(BOOL bOn)
{
	/*/	---> PowerOn/Off�� Break�����ϴ� Type���� �ٲ�
	int iResult = 0;
	if (bOn)
		iResult = m_plnkIO->OutputOff(m_usOPReelMotorBreak);
	else
		iResult = m_plnkIO->OutputOn(m_usOPReelMotorBreak);
	return iResult;
	/*/
	return ERR_TABFEEDER_SUCCESS;
}

BOOL MTabFeeder::IsPReelBreakOn()
{
//@	return m_plnkIO->IsOff(m_usOPReelMotorBreak);//Break����
	return TRUE;
}
//____________________

//@____________________
int MTabFeeder::PReelMotorWind()
{
	//20131003 SJ_HJG Door Open �� ���¿��� ���� ����...
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen() == TRUE)
	{
		m_plnkIO->OutputOn(m_usOPReelMotorPower);
		m_plnkIO->OutputOff(m_usOPReelMotorWind);
		m_plnkIO->OutputOff(m_usOPReelMotorRelease);
		m_plnkIO->UpdateIO();
	}
	else
	{
		m_plnkIO->OutputOff(m_usOPReelMotorRelease);
		m_plnkIO->OutputOn(m_usOPReelMotorWind);
		m_plnkIO->OutputOn(m_usOPReelMotorPower);
		m_plnkIO->UpdateIO();
	}
	return ERR_TABFEEDER_SUCCESS;
}

int MTabFeeder::PReelMotorRelease()
{
	//20131003 SJ_HJG Door Open �� ���¿��� ���� ����...
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen() == TRUE)
	{
		m_plnkIO->OutputOn(m_usOPReelMotorPower);
		m_plnkIO->OutputOff(m_usOPReelMotorWind);
		m_plnkIO->OutputOff(m_usOPReelMotorRelease);
		m_plnkIO->UpdateIO();
	}
	else
	{
		m_plnkIO->OutputOff(m_usOPReelMotorWind);
		m_plnkIO->OutputOn(m_usOPReelMotorRelease);
		m_plnkIO->OutputOn(m_usOPReelMotorPower);
		m_plnkIO->UpdateIO();
	}
	return ERR_TABFEEDER_SUCCESS;
}

int	MTabFeeder::PReelMotorStop()
{
//@	m_plnkIO->OutputOff(m_usOPReelMotorPower);
	m_plnkIO->OutputOn(m_usOPReelMotorPower);
	m_plnkIO->OutputOff(m_usOPReelMotorWind);
	m_plnkIO->OutputOff(m_usOPReelMotorRelease);
	m_plnkIO->UpdateIO();
	return ERR_TABFEEDER_SUCCESS;
}

BOOL MTabFeeder::IsPReelMotorWind()
{
	BOOL bRet = m_plnkIO->OutputOn(m_usOPReelMotorWind)
				&& m_plnkIO->OutputOff(m_usOPReelMotorRelease)
				&& m_plnkIO->OutputOn(m_usOPReelMotorPower);
	return bRet;
}

BOOL MTabFeeder::ISPReelMotorRelease()
{
	BOOL bRet = m_plnkIO->OutputOff(m_usOPReelMotorWind)
				&& m_plnkIO->OutputOn(m_usOPReelMotorRelease)
				&& m_plnkIO->OutputOn(m_usOPReelMotorPower);
	return bRet;
}

BOOL MTabFeeder::IsPReelMotorStop()
{
	BOOL bRet = m_plnkIO->OutputOff(m_usOPReelMotorWind)
				&& m_plnkIO->OutputOff(m_usOPReelMotorRelease);
//@				&& m_plnkIO->OutputOff(m_usOPReelMotorPower);
	return bRet;
}
//@____________________

/************************************************************************/
/*        IO Operation Interface									   */
/************************************************************************/

/**
 * BadTab Detect
 *
 * @return	BOOL : (TRUE = Tab ������, FALSE = BadTab)
 */
BOOL MTabFeeder::IsBadTabDetected(int iSensorNo)
{
	return m_plnkIO->IsOff(m_usIAddrBadTabDetect[iSensorNo]);
}

/** ���� ���� ���� */
BOOL MTabFeeder::IsPressMoldDetect(void)
{
	return m_plnkIO->IsOn(m_usIAddrMoldDetect);
}

// ��ȣ�� ���� ����...
BOOL MTabFeeder::IsDownProtectDetect(void)
{
//@	return m_plnkIO->IsOn(m_usIAddrProtectDownDetect);
	return TRUE;
}

BOOL MTabFeeder::IsRailUnfoldDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrRailUnfoldDetect);
}

//BOOL MTabFeeder::IsRailDiscardReelDtetcted(void)
//{
//	return m_plnkIO->IsOn(m_usIAddrRailDiscardReelDetect);
//}

BOOL MTabFeeder::IsCurrentReelDtetcted(void)
{
//@	return m_plnkIO->IsOn(m_usIAddrSupplierCurrentReelDetect);
	return TRUE;
}

BOOL MTabFeeder::IsNewReelDtetcted(void)
{
//@	return m_plnkIO->IsOn(m_usIAddrSupplierNewReelDetect);
	return FALSE;
}

BOOL MTabFeeder::IsTensionUpperLimitDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrTensionUpperLimitDetect);
}

BOOL MTabFeeder::IsTensionUpperDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrTensionUpperDetect);
}

BOOL MTabFeeder::IsTensionLowerDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrTensionLowerDetect);
}

BOOL MTabFeeder::IsTensionLowerLimitDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrTensionLowerLimitDetect);
}

BOOL MTabFeeder::IsSupplierSmallTensionDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrSupplierSmallDetect);
}
BOOL MTabFeeder::IsSupplierMiddleTensionDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrSupplierMiddleDetect);
}
BOOL MTabFeeder::IsSupplierLargeTensionDetected(void)
{
	return m_plnkIO->IsOn(m_usIAddrSupplierLargeDetect);
}

/************************************************************************/
/*        TabFeeder Move Operation Interface                           */
/************************************************************************/

//_________________________________________________________________________________________________________
// Press Move ���� �Լ�
/**
 * Press�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SafeMovePressPos(int iPosID)
{
	double posTarget = GetPressTargetPos(iPosID);	// ��ǥ ��ġ �б�
	return MovePressPos(posTarget, iPosID);		// �ش� ��ġ�� �̵�
}

/**
 * Press�� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @param	iPosID : �̵��� Position Index
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::MovePressPos(double posTarget, int iPosID, BOOL wait)
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Press Position:%d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	// Interlock Check : Interlock ����.

	int iResult = m_plnkPressAxis->StartMove(posTarget);
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkPressAxis->Stop();
		return iResult;
	}

	if (wait)
	{
		iResult = m_plnkPressAxis->Wait4Done();
		if (iResult != ERR_TABFEEDER_SUCCESS)
		{
			m_plnkPressAxis->Stop();
			return iResult;
		}
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Press Position:%d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return iResult;
}

/**
 * Press�� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @param	iPosID : �̵��� Position Index
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::StartMovePressPos(int iPosID)
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Press Position:%d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	double posTarget = GetPressTargetPos(iPosID);

	int iResult = m_plnkPressAxis->StartMove(posTarget);
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkPressAxis->Stop();
		return iResult;
	}

	return iResult;
}
/**
 * Press�� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @param	iPosID : �̵��� Position Index
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::Wait4DoneMovePressPos(int iPosID)
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif

	int iResult = m_plnkPressAxis->Wait4Done();
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkPressAxis->Stop();
		return iResult;
	}

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Press Position:%d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return iResult;
}

//_________________________________________________________________________________________________________
// Supplier Move ���� �Լ�
/**
 * Supplier�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SafeMoveSupplierPos(int iPosID)
{
	double posTarget = GetSupplierTargetPos(iPosID);	// ��ǥ ��ġ �б�
	return MoveSupplierPos(posTarget, iPosID);		// �ش� ��ġ�� �̵�
}

/**
 * Supplier�� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @param	iPosID : �̵��� Position Index
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::MoveSupplierPos(double posTarget, int iPosID)
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif
	
	return ERR_TABFEEDER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Supplier Position:%d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	// hongju ����...Chuck���� Input ������ ����....���� Interlock ���� ����...
	// Interlock Check : Interlock ����.
	//80507.KKY_____
	//Ready��ġ�� �̵��ÿ��� ������ Unlock�Ѵ�.
/*	if (!IsInSupplierPos(DEF_SUPPLIER_READY_POS) && iPosID == DEF_SUPPLIER_READY_POS)
	{
		if (m_iInstanceNo == 0 && !m_plnkIO->IsOn(OUT_TABPRESS1_SUPPLIER_CHUCK_EXPAND))
		{
			// 215031 = Supplier Chuck Unlock �� �ȵǾ� �ֽ��ϴ�.
			return generateErrorCode(215031);
		}
		if (m_iInstanceNo == 1 && !m_plnkIO->IsOn(OUT_TABPRESS2_SUPPLIER_CHUCK_EXPAND))
		{
			return generateErrorCode(215031);
		}
	}
	//Work��ġ�� Supply��ġ�� �̵��ÿ���
	if (iPosID == DEF_SUPPLIER_WORK_POS || iPosID == DEF_SUPPLIER_SUPPLY_POS)
	{
		double cPos = GetSupplierCurrentPos();
		double compPos = GetSupplierTargetPos(DEF_SUPPLIER_WORK_POS);
		//���� ��ġ�� Work��ġ���� �ڿ� �������� �̵��� ������ Unlock�Ѵ�.
		if (compPos > cPos + 1.0)
		{
			if (m_iInstanceNo == 0 && !m_plnkIO->IsOn(OUT_TABPRESS1_SUPPLIER_CHUCK_EXPAND))
			{
				return generateErrorCode(215031);
			}			
			if (m_iInstanceNo == 1 && !m_plnkIO->IsOn(OUT_TABPRESS2_SUPPLIER_CHUCK_EXPAND))
			{
				return generateErrorCode(215031);
			}			
		}
	}
*/	//______________

	int iResult = m_plnkSupplierAxis->StartMove(posTarget);
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkSupplierAxis->Stop();
		return iResult;
	}

	iResult = m_plnkSupplierAxis->Wait4Done();
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkSupplierAxis->Stop();
		return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Supplier Position:%d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return iResult;
}

//_________________________________________________________________________________________________________
// BadTab Move ���� �Լ�
/**
 * BadTab�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SafeMoveBadTabPos(int iPosID)
{
	double posTarget = GetBadTabTargetPos(iPosID);		// ��ǥ ��ġ �б�
	return MoveBadTabPos(posTarget, iPosID);
}

/**
 * BadTab�� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @param	iPosID : �̵��� Position Index
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::MoveBadTabPos(double posTarget, int iPosID)
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif
	
	int iResult = ERR_TABFEEDER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Move Bad Position:%d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);
	
	// ��ǥ��ġ�� �̵�
	iResult = m_plnkBadTabAxis->StartMove(posTarget);
	if (iResult)
	{
		m_plnkBadTabAxis->Stop();
		return iResult;
	}
	
	iResult = m_plnkBadTabAxis->Wait4Done();
	if (iResult)
	{
		m_plnkBadTabAxis->Stop();
		return iResult;
	}
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move Move Bad Position:%d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return iResult;
}

//_________________________________________________________________________________________________________
// Supply Move ���� �Լ�
/**
 * Supply T�� �̵���Ų��. 
 *
 * @param	dVelocity : VMode �ӵ�. default �Ķ���� ���� ������ ���� �ӵ��� ��� �̵��Ѵ�.
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::VMoveSprocket1(double dVelocity, BOOL bDir)
{
	return m_plnkSprocket1Axis->VMove(dVelocity, 0, bDir);
}
/**
 * Supply T�� ������Ų��.. 
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::VStopSprocket1()
{
	return m_plnkSprocket1Axis->VStop();
}

/**
 * Sprocket2 �� �̵���Ų��. 
 *
 * @param	dVelocity : VMode �ӵ�. default �Ķ���� ���� ������ ���� �ӵ��� ��� �̵��Ѵ�.
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::VMoveSprocket2(double dVelocity, BOOL bDir)
{
	return m_plnkSprocket2Axis->VMove(dVelocity, 0, bDir);
}
/**
 * Supply T�� ������Ų��.. 
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::VStopSprocket2()
{
	return m_plnkSprocket2Axis->VStop();
}

//_________________________________________________________________________________________________________
// Sprocket1 Move ���� �Լ�
/**
 * Sprocket T�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SafeRMoveSprocket1Pos(int iPosID)
{
	double posTarget = GetSprocket1TargetPos(iPosID);
	return RMoveSprocket1Pos(posTarget);
}

/**
 * @param	posTarget : ��ǥ ��ġ
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::RMoveSprocket1Pos(double posTarget, BOOL bControlSprocket2, double dVelocity)
{
	//hongju ����...20080325
//////////////////////////////////
    int	iTmpValue = 0;
	int	iResult = ERR_TABFEEDER_SUCCESS;

	// selsk_jdy Yl, Yr ���� �۾��� Tab Pitch ������ Ʋ�� ��쿡 ���ؼ� �ڵ� �ʿ�.
#ifdef DEF_SOURCE_SYSTEM
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif
	double dOneTabDist = iTabPitchCount * dOnePitchDist;
//////////////////////////////////

	WriteTactTimeLog("[MTabFeeder] [M] [Sprocket1 RMove] [Start]");

	if (TRUE == bControlSprocket2)
	{
		// hongju �߰�...
		//------------------------------------------------------------------------------------------------------
		if ((TRUE == m_plnkSprocket1Axis->IsOriginReturn())
			&& (TRUE == m_plnkSprocket2Axis->IsOriginReturn())
			)
		{
			if (posTarget > 0.0)
			{
				if (posTarget > (2*dOneTabDist))
				{
					iTmpValue = (int)posTarget/(int)dOneTabDist; // �� �� Tab ���� ����...
					posTarget = posTarget - ((iTmpValue-1) * dOneTabDist);
					
					// RMultiMove ����..
					iResult = RMultiMoveSprocketPos((iTmpValue-1)*dOneTabDist);
					if (iResult)
						return iResult;
				}
			}
			else
			{
				if ((-1*posTarget) > (2*dOneTabDist))
				{
					iTmpValue = (int)posTarget/(int)dOneTabDist; // �� �� Tab ���� ����...
					posTarget = posTarget - ((iTmpValue+1) * dOneTabDist);
					
					// RMultiMove ����..
					iResult = RMultiMoveSprocketPos((iTmpValue+1)*dOneTabDist);
					if (iResult)
						return iResult;
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------
	}
	
	if (posTarget < 0.5)	// ��ȸ���ϴ� ���.
	{
		m_bSprocket1ReverseRotation = TRUE;
	}

	iResult = m_plnkSprocket1Axis->StartRMove(posTarget, dVelocity);
	if (iResult != ERR_MOTION_SUCCESS)
	{
		// jdy
		m_bSprocket1ReverseRotation = FALSE;
		m_plnkSprocket1Axis->Stop();
		
		return iResult;
	}

	/* jdy 12pm_s : ���� : �ڵ����� �� Feeding �ÿ� Sprocket2 �� ��ȸ�� �ϴ� ���� �߻�. �Ʒ� �ڵ� �ּ�ó���Ǿ� ������ �ȵ�.
	// hongju ����. �Ʒ� �ڵ忡�� Ȯ�� �ϹǷ� �켱 Pass
/*	if (FALSE == bControlSprocket2)
	*/ 
	if (FALSE == bControlSprocket2)
	// jdy 12pm_e
	{
		iResult = m_plnkSprocket1Axis->Wait4Done(FALSE, FALSE, TRUE);

		// jdy
		m_bSprocket1ReverseRotation = FALSE;
		
		if (iResult)
			return iResult;
		
		WriteTactTimeLog("[MTabFeeder] [M] [Sprocket1 Move] [End]");
		return ERR_TABFEEDER_SUCCESS;
	}
// jdy 12pm		*/

	MTickTimer tmpTimer;
	tmpTimer.StartTimer();
	while (FALSE == m_plnkSprocket1Axis->IsMotionDone())
	{
		if (IsTensionUpperDetected() || IsTensionUpperLimitDetected())
			m_plnkSprocket2Axis->RMove(-getRotationAlgne(), 2*dVelocity);
		
		if (IsTensionLowerDetected() || IsTensionLowerLimitDetected())
			m_plnkSprocket2Axis->RMove(getRotationAlgne(), 2*dVelocity);
		
		Sleep(10);

		if (tmpTimer.MoreThan(30.0))
		{
			tmpTimer.StopTimer();
			break;
		}
	}

	iResult = m_plnkSprocket1Axis->Wait4Done(FALSE, FALSE, TRUE);

	// jdy
	m_bSprocket1ReverseRotation = FALSE;

	iResult = m_plnkSprocket1Axis->Wait4Done(FALSE, FALSE, TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	if (IsTensionUpperDetected() || IsTensionUpperLimitDetected())
		m_plnkSprocket2Axis->RMove(-getRotationAlgne(), 2*dVelocity);
	
	if (IsTensionLowerDetected() || IsTensionLowerLimitDetected())
		m_plnkSprocket2Axis->RMove(getRotationAlgne(), 2*dVelocity);

	WriteTactTimeLog("[MTabFeeder] [M] [Sprocket1 Move] [End]");
	return ERR_TABFEEDER_SUCCESS;
}

//_________________________________________________________________________________________________________
// Sprocket2 Move ���� �Լ�
/**
 * Sprocket2�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::SafeRMoveSprocket2Pos(int iPosID)
{
	double posTarget = GetSprocket2TargetPos(iPosID);	// ��ǥ ��ġ �б�
	return RMoveSprocket2Pos(posTarget);
}

/**
 * Sprocket2�� TabType�� ���� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::RMoveSprocket2Pos(double posTarget, BOOL bControlSprocket1, double dVelocity)
{

    int	iTmpValue = 0;
	int	iResult = ERR_TABFEEDER_SUCCESS;

#ifdef DEF_SOURCE_SYSTEM
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;					//One Tab�� Pitch����
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;	//Sprocket One Pitch�� ����
#else
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;					//One Tab�� Pitch����
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;	//Sprocket One Pitch�� ����
#endif
	double dOneTabDist = iTabPitchCount * dOnePitchDist;								//Ont Tab�� ����
//////////////////////////////////

	WriteTactTimeLog("[MTabFeeder] [M] [Sprocket2 Move] [Start]");
	
	// selsk_jdy if (IsInPressPos(DEF_PRESS_DOWN_POS))
	if (FALSE == IsInPressPos(DEF_PRESS_UP_POS) && FALSE == IsInPressPos(DEF_PRESS_READY_POS))
	{
		iResult = SafeMovePressPos(DEF_PRESS_UP_POS);
		if (iResult)
			return iResult;
	}
	
	iResult = SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
	if (iResult)
		return iResult;



	iResult = m_plnkSprocket2Axis->StartRMove(posTarget, dVelocity);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;
	Sleep(20);//110610.KKY.Add

	if (FALSE == bControlSprocket1)
	{
		iResult = m_plnkSprocket2Axis->Wait4Done(FALSE, FALSE, TRUE);
		if (iResult)
			return iResult;
		
		WriteTactTimeLog("[MTabFeeder] [M] [Sprocket2 Move] [End]");
		return ERR_TABFEEDER_SUCCESS;
	}

	MTickTimer tmpTimer;
	tmpTimer.StartTimer();

	while (FALSE == m_plnkSprocket2Axis->IsMotionDone())
	{
		//111021 sj_kjs
		if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			//if (m_plnkIO->IsOn(IN_FRONT_STEP_STOP_SW) || m_plnkIO->IsOn(IN_REAR_STEP_STOP_SW))
		{
			m_plnkSprocket1Axis->Stop();
			m_plnkSprocket2Axis->HomingStop();
			return generateErrorCode(315002);
		}
		//________________________________
		if (posTarget > 0.0)
			iResult = m_plnkCtrlTabFeeder->FindWeightDownSensor();
		else if (posTarget < 0.0)
			iResult = m_plnkCtrlTabFeeder->EscapeWeightDownSensor();
		if (iResult)
			return iResult;			
		Sleep(10);
		if (tmpTimer.MoreThan(30.0))
		{
			tmpTimer.StopTimer();
			break;
		}
	}



	iResult = m_plnkSprocket2Axis->Wait4Done(FALSE, FALSE, TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

#ifdef DEF_MECH_1
	if (IsTensionUpperDetected() || IsTensionUpperLimitDetected())
		m_plnkSprocket1Axis->RMove(getRotationAlgne(), 2*dVelocity);
	
	if (IsTensionLowerDetected() || IsTensionLowerLimitDetected())
		m_plnkSprocket1Axis->RMove(-getRotationAlgne(), 2*dVelocity);
#endif

	WriteTactTimeLog("[MTabFeeder] [M] [Sprocket2 Move] [End]");
	return ERR_TABFEEDER_SUCCESS;
}

//_________________________________________________________________________________________________________
// Sprocket1,2 ����Move ���� �Լ�
int MTabFeeder::SafeRMultiMoveSprocketPos(int iPosID)
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	double posTarget = GetSprocket2TargetPos(iPosID);	// ��ǥ ��ġ �б�

	iResult = RMultiMoveSprocketPos(posTarget);
	if (iResult)
		return iResult;

	return iResult;
}

/**
 * Sprocket2�� TabType�� ���� �̵���Ų��.
 *
 * @param	posTarget : ��ǥ ��ġ
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MTabFeeder::RMultiMoveSprocketPos(double posTarget, BOOL bWaitOption)
{
	int iResult = ERR_TABFEEDER_SUCCESS;

#ifdef DEF_SOURCE_SYSTEM
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;					//One Tab�� Pitch����
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;	//Sprocket One Pitch�� ����
#else
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;					//One Tab�� Pitch����
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;	//Sprocket One Pitch�� ����
#endif
	double dOneTabDist = iTabPitchCount * dOnePitchDist;								//Ont Tab�� ����
	//20121004 SJ_HJG Ⱦ����
	double TabEndMarkDistance = 0.0;
	if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
	{
		/*/
		if (fabs(TabEndMarkDistance) > 0.2)
		{
			TabEndMarkDistance = 0.0;
			//317023 = TabCarrier TabEndMark Distance�� �ʹ� Ů�ϴ�(0.2 �̻�).
			return generateErrorCode(317023);
		}
		
		if (m_iOldTabEndMarkDistance == TabEndMarkDistance)
		{
			TabEndMarkDistance = 0.0;
		}
		/*/

		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
			//TabEndMarkDistance =  - MOlbSystemPre.GetTabEndMarkDistance(0);	//TabEndMarkDistance�� ����
			//TabEndMarkDistance = 360.0 / DEF_SPROCKET_ONE_PITCH_DIST / iTabPitchCount * MOlbSystemPre.GetTabEndMarkDistance(0);
			TabEndMarkDistance =  - (dOnePitchDist * MOlbSystemPre.GetTabEndMarkDistance(0))/ DEF_SPROCKET_ONE_PITCH_DIST;
		}
		else
		{
			//TabEndMarkDistance =  - MOlbSystemPre.GetTabEndMarkDistance(1);	//TabEndMarkDistance�� ����
			//TabEndMarkDistance = 360.0 / DEF_SPROCKET_ONE_PITCH_DIST / iTabPitchCount * MOlbSystemPre.GetTabEndMarkDistance(0);
			TabEndMarkDistance =  - (dOnePitchDist * MOlbSystemPre.GetTabEndMarkDistance(1))/ DEF_SPROCKET_ONE_PITCH_DIST;
		}
	}

	// posTarget�� ������ ��쵵 ����ؼ� ����� �־�� ��.
	// Target ��ġ�� �� �� Pitch �̵� �ߴ��� ����ؼ� �� ���� m_iSprocketRotateCount�� �����ش�.
	// 3Pitch �̵��ô� ������ �������Ƿ� ������3���ؼ� �������� 1�̸� ���ϱ�1, �������� 2�̸� ���ϱ� 2�� �Ѵ�.
	int iPitchCount = 0;
	iPitchCount = (int)(posTarget/dOnePitchDist);
	iPitchCount = iPitchCount%3;

	if (1 == iPitchCount)
		m_iSprocketRotateCount +=1;
	else if (2 == iPitchCount)
		m_iSprocketRotateCount +=2;
	
	// hongju ����. 60�� �����ϱ� ���ؼ�....0.0028�� ���� 10�޽� ������(30Pitch) �������������� 0.0028�� �� �����ָ� ��.
#ifdef DEF_SOURCE_SYSTEM
	if(m_iSprocketRotateCount > 30 && m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab == 60)
#else
	if(m_iSprocketRotateCount > 30 && m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab == 60)
#endif
	{
		m_iSprocketRotateCount -= 30;
		posTarget += 0.0028;
	}
	//20121004 SJ_HJG Ⱦ����
	if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
	{
		posTarget = posTarget + TabEndMarkDistance;
	}

	double rgdTarget[2];
	rgdTarget[0] = rgdTarget[1] = posTarget;

	//SJ_YYK 150213 Add..
#ifdef DEF_SOURCE_SYSTEM
	rgdTarget[1] = rgdTarget[1] / dOnePitchDist * (360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab_Second);
#else
	rgdTarget[1] = rgdTarget[1] / dOnePitchDist * (360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab_Second);
#endif

	m_iOldTabEndMarkDistance = TabEndMarkDistance;

	if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
			MOlbSystemPre.SetTabEndMarkDistance(DEF_FRONT_GROUP, 0.0);
		}
		else
		{
			MOlbSystemPre.SetTabEndMarkDistance(DEF_REAR_GROUP, 0.0);
		}
	}
	//________________________
	double rgdVelocity[2];
	// hongju ����...�̵��Ÿ��� Ŭ�� ������ �����ϱ� ���Ͽ� 
	double rgdAccelerate[2];

	if (posTarget > 0.0)
	{
		if (posTarget > 1000.0)
		{
			rgdVelocity[0] = rgdVelocity[1] = 60.0;
			// hongju �߰�..�켱 ������ 1�� �� ����...
			rgdAccelerate[0] = rgdAccelerate[1] = 1000;
		}
		else
			rgdVelocity[0] = rgdVelocity[1] = m_plnkSprocket1Axis->GetMovingVelocity() * 1.414;
	}
	else
	{
		//100916.KKY_____
//		rgdVelocity[0] = rgdVelocity[1] = 30.0;
		rgdVelocity[0] = rgdVelocity[1] = 50.0;
		//_______________
	}
	
	WriteTactTimeLog("[MTabFeeder] [M] [Sprocket MultiMove] [Start]");
	
	if (FALSE == IsInPressPos(DEF_PRESS_UP_POS) && FALSE == IsInPressPos(DEF_PRESS_READY_POS))
	{
		iResult = SafeMovePressPos(DEF_PRESS_UP_POS);
		if (iResult)
			return iResult;
	}

	if (posTarget < 0.5)	// ��ȸ���ϴ� ���.
	{
		m_bSprocket1ReverseRotation = TRUE;
	}

	DWORD stime = GetTickCount();
	if (posTarget > 1000.0)
	{
		iResult = m_plnkMultiSprocketAxis->StartRMove(rgdTarget, rgdVelocity, rgdAccelerate, rgdAccelerate);
		if (iResult != ERR_TABFEEDER_SUCCESS)
		{
			// JDY
			m_bSprocket1ReverseRotation = FALSE;
			return iResult;
		}
	}
	else
	{
		iResult = m_plnkMultiSprocketAxis->StartRMove(rgdTarget, rgdVelocity);		
		if (iResult != ERR_TABFEEDER_SUCCESS)
		{
			// JDY
			m_bSprocket1ReverseRotation = FALSE;
			return iResult;
		}
	}

	if (TRUE == bWaitOption)
	{
		iResult = m_plnkMultiSprocketAxis->Wait4Done();
		if (iResult)
		{
			// JDY
			m_bSprocket1ReverseRotation = FALSE;
			return iResult;
		}
	}
	TRACE(_T("[%d] Feeder Real Feeding Elaspe Time = %d\n"), m_iInstanceNo, int(GetTickCount() - stime));

	// JDY
	m_bSprocket1ReverseRotation = FALSE;

	WriteTactTimeLog("[MTabFeeder] [M] [Sprocket MultiMove] [End]");

	return iResult;
}

BOOL MTabFeeder::IsInMotionSprocket2()
{
	return m_plnkSprocket2Axis->IsInMotion();
}


int MTabFeeder::SafeMoveMoldPinPos(int iPosID, BOOL wait)
{
	double posTarget = GetMoldPinTargetPos(iPosID);
	return MoveMoldPinPos(posTarget, iPosID, wait);
}

int MTabFeeder::MoveMoldPinPos(double posTarget, int iPosID, BOOL wait)
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif
	
	double dTolerance = 0.1;
	if (TRUE == IsInMoldPinPos(iPosID, &dTolerance))
	{
		return ERR_TABFEEDER_SUCCESS;
	}

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move MoldPin Position:%d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	// Interlock Check : Interlock ����.
	int iResult = CheckMoldPinOrigin();
	if (iResult != ERR_TABFEEDER_SUCCESS)
		return iResult;

	iResult = m_plnkMoldPinAxis->StartMove(posTarget);
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkMoldPinAxis->Stop();
		return iResult;
	}
	if (wait)
	{
		Sleep(30);
		iResult = m_plnkMoldPinAxis->Wait4Done();
		if (iResult != ERR_TABFEEDER_SUCCESS)
		{
			m_plnkMoldPinAxis->Stop();
			return iResult;
		}
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabFeeder] [M] [Move MoldPin Position:%d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return iResult;
}

int MTabFeeder::Wait4DoneMoldPin()
{
#ifdef SIMULATION
	return ERR_TABFEEDER_SUCCESS;
#endif

	int iResult = m_plnkMoldPinAxis->Wait4Done();
	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
		m_plnkMoldPinAxis->Stop();
		return iResult;
	}

	return iResult;
}

/************************************************************************/
/*        TabFeeder PositionȮ�� Interface                              */
/************************************************************************/

//_________________________________________________________________________________________________________
// Press Position ���� �Լ�

int MTabFeeder::CheckPressOrigin()
{
	// Press Motion Check Origin
	if (m_plnkPressAxis->IsOriginReturn() == FALSE)
	{
		//WriteErrorLog("CheckPressOrigin() : Press Z �� �������� �Ǿ� ���� �ʽ��ϴ�.");
		return generateErrorCode(215011); // Tab Feeder Press Z �� �������� �Ǿ� ���� �ʽ��ϴ�.
	}
	return ERR_TABFEEDER_SUCCESS;
}

double MTabFeeder::GetPressCurrentPos(BOOL bType)
{
	ASSERT(m_plnkPressAxis != NULL);
	return m_plnkPressAxis->GetCurrentPosition(bType);
}

double MTabFeeder::GetPressTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_PRESS_MAX_POSITION);

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ
	double posTarget = m_rgsPressFixedPos[iPosID] + m_rgsPressModelPos[iPosID] + m_rgsPressOffsetPos[iPosID];
	return posTarget;
}

BOOL MTabFeeder::IsInPressPos(int iPosID, double* dTolerance)
{
	double dTargetPos = GetPressTargetPos(iPosID);

	if (m_plnkPressAxis->ComparePosition(FALSE, dTargetPos, dTolerance))
		return TRUE;

	return FALSE;
}

//_________________________________________________________________________________________________________
// Supplier Position ���� �Լ�

int MTabFeeder::CheckSupplierOrigin()
{
	return ERR_TABFEEDER_SUCCESS;
}

double MTabFeeder::GetSupplierCurrentPos(BOOL bType)
{
	return 0.0;
}

double MTabFeeder::GetSupplierTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_SUPPLIER_MAX_POSITION);

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ
	double posTarget = m_rgsSupplierFixedPos[iPosID] + m_rgsSupplierModelPos[iPosID] + m_rgsSupplierOffsetPos[iPosID];
	return posTarget;
}

BOOL MTabFeeder::IsInSupplierPos(int iPosID, double* dTolerance)
{
	return TRUE;

	double dTargetPos = GetSupplierTargetPos(iPosID);
	if (m_plnkSupplierAxis->ComparePosition(FALSE, dTargetPos, dTolerance))
		return TRUE;

	return FALSE;
}

//_________________________________________________________________________________________________________
// BadTab Position ���� �Լ�

int MTabFeeder::CheckBadTabOrigin()
{
	// BadTab Motion Check Origin
	if (m_plnkBadTabAxis->IsOriginReturn() == FALSE)
		return generateErrorCode(215014);		// ERR_TABFEEDER_IS_NOT_ORIGIN

	return ERR_TABFEEDER_SUCCESS;
}

double MTabFeeder::GetBadTabCurrentPos(BOOL bType)
{
	for (int i = 0 ; i < DEF_BADTABDETECTOR_MAX_AXIS; i++)
		ASSERT(m_plnkBadTabAxis != NULL);
	
	double dCurPos = 0.0;

	dCurPos = m_plnkBadTabAxis->GetCurrentPosition(bType);

	return dCurPos;
}

double MTabFeeder::GetBadTabTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_BADTABDETECTOR_MAX_POSITION);

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ
	double posTarget = m_rgsBadTabModelPos[iPosID] + m_rgsBadTabOffsetPos[iPosID];

	// hongju Ȯ�� �ʿ�..20071123
	// Sprocket Offset �ݿ�
/*	if (GetInstanceNo() == DEF_TABFEEDER1)
		posTarget += m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset;
	else
		posTarget += m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset;
*/
	/* selsk_jdy ���ʿ�.
	if (DEF_BADTABDETECTOR_WORK_POS == iPosID)
	{
		if (GetInstanceNo() == DEF_TABFEEDER1)
			posTarget = posTarget 
						+ m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset
						+ m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset;
		else
			posTarget = posTarget
						+ m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset
						+ m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
	}
	*/

	return posTarget;
}

BOOL MTabFeeder::IsInBadTabPos(int iPosID, double* dTolerance)
{
	double dTargetPos;
	dTargetPos = GetBadTabTargetPos(iPosID);
	
	if (m_plnkBadTabAxis->ComparePosition(FALSE, dTargetPos, dTolerance))
		return TRUE;

	return FALSE;
}

//_________________________________________________________________________________________________________
// Sprocket1 Position ���� �Լ�

int MTabFeeder::CheckSprocket1Origin()
{
	// Sprocket1 Motion Check Origin
	if (m_plnkSprocket1Axis->IsOriginReturn() == FALSE)
		// 215012 = Sprocket1 T �� �������� �Ǿ� ���� �ʽ��ϴ�.
		return generateErrorCode(215012);
	return ERR_TABFEEDER_SUCCESS;
}

double MTabFeeder::GetSprocket1CurrentPos(BOOL bType)
{
	ASSERT(m_plnkSprocket1Axis != NULL);
	return m_plnkSprocket1Axis->GetCurrentPosition(bType);
}

double MTabFeeder::GetSprocket1TargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_PRESS_MAX_POSITION);

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ
	double posTarget = m_rgsSprocket1ModelPos[iPosID] + m_rgsSprocket1OffsetPos[iPosID];
	return posTarget;
}

BOOL MTabFeeder::IsInSprocket1Pos(int iPosID, double* dTolerance)
{
	double dTargetPos = GetSprocket1TargetPos(iPosID);

	if (m_plnkSprocket1Axis->ComparePosition(FALSE , dTargetPos, dTolerance))
		return TRUE;
	return FALSE;
}

//_________________________________________________________________________________________________________
// Sprocket2 Position ���� �Լ�

int MTabFeeder::CheckSprocket2Origin()
{
	// Sprocket2 Motion Check Origin
	if (m_plnkSprocket2Axis->IsOriginReturn() == FALSE)
		// 215023 = Sprocket2 T �� �������� �Ǿ� ���� �ʽ��ϴ�.
		return generateErrorCode(215023);

	return ERR_TABFEEDER_SUCCESS;
}

double MTabFeeder::GetSprocket2CurrentPos(BOOL bType)
{
	ASSERT(m_plnkSprocket2Axis != NULL);
	return m_plnkSprocket2Axis->GetCurrentPosition(bType);
}

double MTabFeeder::GetSprocket2TargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_PRESS_MAX_POSITION);

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ
	double posTarget = m_rgsSprocket2ModelPos[iPosID] + m_rgsSprocket2OffsetPos[iPosID];

	return posTarget;
}

BOOL MTabFeeder::IsInSprocket2Pos(int iPosID, double* dTolerance)
{
	double dTargetPos = GetSprocket2TargetPos(iPosID);

	if (m_plnkSprocket2Axis->ComparePosition(FALSE , dTargetPos, dTolerance))
		return TRUE;

	return FALSE;
}

int MTabFeeder::CheckMoldPinOrigin(void)
{
	// Press Motion Check Origin
	if (m_plnkMoldPinAxis->IsOriginReturn() == FALSE)
		return generateErrorCode(215036); // Tab Feeder Press Z �� �������� �Ǿ� ���� �ʽ��ϴ�.
	return ERR_TABFEEDER_SUCCESS;
}

double MTabFeeder::GetMoldPinCurrentPos(BOOL bType)
{
	ASSERT(m_plnkMoldPinAxis != NULL);
	return m_plnkMoldPinAxis->GetCurrentPosition(bType);
}

double MTabFeeder::GetMoldPinTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_MOLDPIN_MAX_POSITION);

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ
	double posTarget = m_rgsMoldPinFixedPos[iPosID];// + m_rgsMoldPinModelPos[iPosID] + m_rgsMoldPinOffsetPos[iPosID];
	return posTarget;
}

BOOL MTabFeeder::IsInMoldPinPos(int iPosID, double *dTolerance)
{
	double dTargetPos = GetMoldPinTargetPos(iPosID);

	if (CheckMoldPinOrigin() != ERR_TABFEEDER_SUCCESS)
		return FALSE;

	if (m_plnkMoldPinAxis->ComparePosition(FALSE, dTargetPos, dTolerance))
		return TRUE;

	return FALSE;
}

int MTabFeeder::MoldBlowOn()
{
	return m_plnkIO->OutputOn(m_usOAddrMoldBlowOn);
}

int MTabFeeder::MoldBlowOff()
{
	return m_plnkIO->OutputOff(m_usOAddrMoldBlowOn);
}

int MTabFeeder::RecoveryIndMotorOn()
{
	return m_plnkIO->OutputOn(m_usOAddrRecoveryTensionMotorOn);
}

int MTabFeeder::RecoveryIndMotorOff()
{
	return m_plnkIO->OutputOff(m_usOAddrRecoveryTensionMotorOn);
}

BOOL MTabFeeder::IsRecoveryIndMotorOn()
{
	return m_plnkIO->IsOn(m_usOAddrRecoveryTensionMotorOn);
}

BOOL MTabFeeder::IsRecoveryIndMotorOff()
{
	return m_plnkIO->IsOff(m_usOAddrRecoveryTensionMotorOn);
}

int MTabFeeder::BrushMotorOn()
{
	return 0;
}

int MTabFeeder::BrushMotorOff()
{
	return 0;
}

int MTabFeeder::IsBrushMotorOn()
{
	return FALSE;
}

int MTabFeeder::IsBrushMotorOff()
{
	return FALSE;
}

int MTabFeeder::PReelMotorOn()
{
//	#pragma message(__LOC__ "MTabFeeder::PReelMotorOn() ���� Ȯ�� �ϱ�")
//@	m_plnkIO->OutputOn(m_usOPReelMotorBreak);//Break����

	//20131003 SJ_HJG Door Open �� ���¿��� ���� ����...
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen() == TRUE)
	{
		m_plnkIO->OutputOn(m_usOPReelMotorPower);
		m_plnkIO->OutputOff(m_usOPReelMotorWind);
		m_plnkIO->OutputOff(m_usOPReelMotorRelease);
	}
	else
	{
		m_plnkIO->OutputOff(m_usOPReelMotorRelease);
		m_plnkIO->OutputOn(m_usOPReelMotorWind);
		m_plnkIO->OutputOn(m_usOPReelMotorPower);//PowerOn
	}
	return 0;
}

int MTabFeeder::PReelMotorOff(BOOL bBreakOn)
{
	/*/
	m_plnkIO->OutputOff(m_usOPReelMotorRelease);
	m_plnkIO->OutputOff(m_usOPReelMotorWind);
//	if (bBreakOn)
//		m_plnkIO->OutputOff(m_usOPReelMotorBreak);//Break����
//	else
		m_plnkIO->OutputOn(m_usOPReelMotorBreak);//Break����
	m_plnkIO->OutputOn(m_usOPReelMotorPower);//PowerOn
	/*/
//	#pragma message(__LOC__ "MTabFeeder::PReelMotorOff() ���� Ȯ�� �ϱ�")
	m_plnkIO->OutputOff(m_usOPReelMotorRelease);
	m_plnkIO->OutputOff(m_usOPReelMotorWind);
//@	m_plnkIO->OutputOff(m_usOPReelMotorPower);
	m_plnkIO->OutputOn(m_usOPReelMotorPower);
	return 0;
}

int MTabFeeder::IsPReelMotorOn()
{
	/* jdy 12pm_s
	//80506.KKY_____
	//return m_plnkIO->IsOn(m_usOAddrPReelTensionMotorOn);
	return (m_plnkIO->IsOn(m_usOAddrPReelTensionMotorOn) && m_plnkIO->IsOn(m_usOAddrSupplierLargeTension));
	//______________
	*/

	/*/
	BOOL bSts = m_plnkIO->IsOff(m_usOPReelMotorRelease)
				&& m_plnkIO->IsOn(m_usOPReelMotorWind)
				&& m_plnkIO->IsOn(m_usOPReelMotorBreak)
				&& m_plnkIO->IsOn(m_usOPReelMotorPower);
	return bSts;
	/*/
	return m_plnkIO->IsOn(m_usOPReelMotorPower);
}

int MTabFeeder::IsPReelMotorOff()
{
	/*/
	//80506.KKY_____
	//return m_plnkIO->IsOff(m_usOAddrPReelTensionMotorOn);
	BOOL bSts1 = m_plnkIO->IsOff(m_usOPReelMotorRelease)
				&& m_plnkIO->IsOff(m_usOPReelMotorWind);
	BOOL bSts2 = m_plnkIO->IsOff(m_usOPReelMotorPower) || m_plnkIO->IsOff(m_usOPReelMotorBreak);
	return bSts1 || bSts2;
	//______________
	/*/
	return m_plnkIO->IsOff(m_usOPReelMotorPower);
}

/*
int MTabFeeder::PReelReverseMotorOn()
{
	//80506.KKY_____
	m_plnkIO->OutputOn(m_usOAddrSupplierLargeTension);
	//______________
	return m_plnkIO->OutputOn(m_usOAddrPReelReverseMotorOn);
}

int MTabFeeder::PReelReverseMotorOff()
{
	//80506.KKY_____
	m_plnkIO->OutputOff(m_usOAddrSupplierLargeTension);
	//______________
	return m_plnkIO->OutputOff(m_usOAddrPReelReverseMotorOn);
}

int MTabFeeder::IsPReelReverseMotorOn()
{
	/ * jdy 12pm_s
	//80506.KKY_____
	//return m_plnkIO->IsOn(m_usOAddrPReelReverseMotorOn);
	return (m_plnkIO->IsOn(m_usOAddrPReelReverseMotorOn) && m_plnkIO->IsOn(m_usOAddrSupplierLargeTension));
	//______________
	* /
	return m_plnkIO->IsOn(m_usOAddrPReelReverseMotorOn);
	// jdy 12pm_e
}

int MTabFeeder::IsPReelReverseMotorOff()
{
	//80506.KKY_____
	//return m_plnkIO->IsOff(m_usOAddrPReelReverseMotorOn);
	return !IsPReelReverseMotorOn();
	//______________
}
*/

int MTabFeeder::MoveOriginFast(UINT nSel, BOOL dir)
{
	int iResult = ERR_TABFEEDER_SUCCESS;

	if (DEF_TABFEEDER_SPROCKET2_AXIS == nSel)
	{
		double dVelocity = m_plnkSprocket2Axis->GetCoarseVelocity();
		iResult = m_plnkSprocket2Axis->VMove(dVelocity, 0, dir);
		if (iResult)
			return iResult;
	}
	else
	{
		double dVelocity = m_plnkSprocket1Axis->GetCoarseVelocity();
		iResult = m_plnkSprocket1Axis->VMove(dVelocity, 0, dir);
		if (iResult)
			return iResult;
	}
	return ERR_TABFEEDER_SUCCESS;
}

int MTabFeeder::MoveOriginSlow(UINT nSel, BOOL dir)
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	
	if (nSel == DEF_TABFEEDER_SPROCKET2_AXIS)
	{
		double dVelocity = m_plnkSprocket2Axis->GetFineVelocity();
		iResult = m_plnkSprocket2Axis->VMove(dVelocity, 0, dir);
	}
	else
	{
		double dVelocity = m_plnkSprocket1Axis->GetFineVelocity();
		iResult = m_plnkSprocket1Axis->VMove(dVelocity, 0, dir);
	}
	return iResult;
}

void MTabFeeder::SetSprocket1CurrentPos (double dTarget)
{
	m_plnkSprocket1Axis->SetCurrentPosition(dTarget);
}

void MTabFeeder::SetSprocket2CurrentPos (double dTarget)
{
	m_plnkSprocket2Axis->SetCurrentPosition(dTarget);
}

// Homing ������ �����ϰ� Z�� ã�� �� ����.. �� �ܴ� Prebonder Data�� �̿�..
int MTabFeeder::HomingSprocket(int iSprocket)
{
	int iResult = ERR_TABFEEDER_SUCCESS;

	// selsk_jdy Yl, Yr ���� �۾��� Tab Pitch ������ Ʋ�� ��쿡 ���ؼ� �ڵ� �ʿ�.
#ifdef DEF_SOURCE_SYSTEM
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;					//One Tab�� Pitch����
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;	//Sprocket One Pitch�� ����
#else
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;					//One Tab�� Pitch����
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;	//Sprocket One Pitch�� ����
#endif
	double dOneTabDist = iTabPitchCount * dOnePitchDist;								//Ont Tab�� ����
	MTickTimer tmpHomingExeTimer;
	int iDistanceCurrentToHome;

	//120217.KMS____________
	//TabData�� �߸� �־��� ��쿡  Program �� ƨ��ų� Tab IC �� ���� ���� ���� ������ Error ó��.
	//315024 = ���� Tab Data �� ������ �־� One Tab Dist ���� O �̰ų� �۽��ϴ�. Tab Data�� Ȯ���� �ּ���.
	if(dOneTabDist <= 0)
		return generateErrorCode(315028);
	//_______________________


	// �������͸� �ϱ� ���� �۾�.
	if (DEF_SPROCKET1_UNIT == iSprocket)
	{
		iResult = CheckSprocket2Origin();
		if (iResult)
			return iResult;

		m_plnkSprocket1Axis->ResetOrigin();
		m_plnkSprocket1Axis->ResetOriginComplete();

		double dOriginVelocity = m_plnkSprocket1Axis->GetCoarseVelocity();

		tmpHomingExeTimer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			{
				m_plnkSprocket2Axis->Stop();
				m_plnkSprocket1Axis->HomingStop();
				return generateErrorCode(315002);
			}

			if (TRUE == IsTensionLowerDetected() || TRUE == IsTensionLowerLimitDetected())
			{
				int iResult = RMoveSprocket2Pos(getRotationAlgne(), FALSE, dOriginVelocity * 2.0);
				if (iResult)
				{
					m_plnkSprocket2Axis->Stop();
					m_plnkSprocket1Axis->HomingStop();
					return iResult;
				}
			}

			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{
				// jdy : Homing ���� �� Sprocket1 �� ��ȸ���ϰ� �ִٰ� �Ǵ��� �� �ִ�.
				m_bSprocket1ReverseRotation = TRUE;

				int iResult = RMoveSprocket2Pos(-getRotationAlgne(), FALSE, dOriginVelocity * 2.0);

				// jdy
				m_bSprocket1ReverseRotation = FALSE;

				if (iResult)
				{
					m_plnkSprocket2Axis->Stop();
					m_plnkSprocket1Axis->HomingStop();
					return iResult;
				}
			}

			if (tmpHomingExeTimer.MoreThan(60.0))
			{
				m_plnkSprocket2Axis->Stop();
				m_plnkSprocket1Axis->HomingStop();
				return generateErrorCode(315004);
			}
			
			if (m_plnkSprocket1Axis->GetOriginStep() == DEF_ORIGIN_FINISH_STEP)
				break;

			m_plnkSprocket1Axis->OriginReturn(TRUE);
			m_plnkSprocket1Axis->GetOriginError(&iResult);
			if (iResult)
			{
				m_plnkSprocket2Axis->Stop();
				m_plnkSprocket1Axis->HomingStop();
				return iResult;
			}

			Sleep(1);
		}

		iResult = TensionMaintenance(DEF_SPROCKET2_UNIT);
		if (iResult)
			return iResult;

		iDistanceCurrentToHome = (int)(m_plnkSprocket1Axis->DistanceToCurrentToHome()/dOneTabDist) + 1;
		m_plnkCtrlTabFeeder->SetRunTensionReverse(TRUE);
		iResult = RMultiMoveSprocketPos(-dOneTabDist*(double)iDistanceCurrentToHome);
		if (iResult)
		{
			m_plnkCtrlTabFeeder->SetRunTensionReverse(FALSE);
			return iResult;
		}
		m_plnkCtrlTabFeeder->SetRunTensionReverse(FALSE);
	}

	if (DEF_SPROCKET2_UNIT == iSprocket)
	{
		iResult = CheckSprocket1Origin();
		if (iResult)
			return iResult;
		
		m_plnkSprocket2Axis->ResetOrigin();
		m_plnkSprocket2Axis->ResetOriginComplete();

		//Clutch�� �������� FeederReel�� +�������� ȸ���Ͽ� �ϰ����� ��ġ�� ã�´�
		ClutchOffSprocket1Axis();

		iResult = m_plnkCtrlTabFeeder->FindWeightDownSensor();
		if (iResult)
		{
			if (iResult)
				return iResult;
		}

		
		double dOriginVelocity = m_plnkSprocket2Axis->GetCoarseVelocity();
		tmpHomingExeTimer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			//if (m_plnkIO->IsOn(IN_FRONT_STEP_STOP_SW) || m_plnkIO->IsOn(IN_REAR_STEP_STOP_SW))
			{
				m_plnkSprocket1Axis->Stop();
				m_plnkSprocket2Axis->HomingStop();
				return generateErrorCode(315002);
			}

			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{

				int iResult = m_plnkCtrlTabFeeder->EscapeWeightUpSensor();
				if (iResult)
				{
					m_plnkSprocket1Axis->Stop();
					m_plnkSprocket2Axis->HomingStop();
					return iResult;
				}
			}

			if (TRUE == IsTensionLowerDetected() || TRUE == IsTensionLowerLimitDetected())
			{
				int iResult= m_plnkCtrlTabFeeder->EscapeWeightDownSensor();
				if (iResult)
				{
					m_plnkSprocket1Axis->Stop();
					m_plnkSprocket2Axis->HomingStop();
					return iResult;
				}
			}

			if (tmpHomingExeTimer.MoreThan(60.0))
			{
				m_plnkSprocket1Axis->Stop();
				m_plnkSprocket2Axis->HomingStop();
				return generateErrorCode(315004);
			}
			
			if (m_plnkSprocket2Axis->GetOriginStep() == DEF_ORIGIN_FINISH_STEP)
				break;

			// Origin Return ���� ����.
			m_plnkSprocket2Axis->OriginReturn(TRUE);

			m_plnkSprocket2Axis->GetOriginError(&iResult);
			if (iResult)
			{
				m_plnkSprocket1Axis->Stop();
				m_plnkSprocket2Axis->HomingStop();
				return iResult;
			}

			Sleep(1);
		}
		iResult = m_plnkCtrlTabFeeder->FindWeightUpSensor();
		if (iResult)
			return iResult;

		iDistanceCurrentToHome = (int)(m_plnkSprocket2Axis->DistanceToCurrentToHome()/dOneTabDist) + 1;
		//110617.KKY_____
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_FIND_UP);
		ClutchOnSprocket1Axis();
//		iResult = RMoveSprocket2Pos(-dOneTabDist*(double)iDistanceCurrentToHome);
		m_plnkCtrlTabFeeder->SetRunTensionReverse(TRUE);
		iResult = RMultiMoveSprocketPos(-dOneTabDist*(double)iDistanceCurrentToHome);

		if (iResult)
		{
			m_plnkCtrlTabFeeder->SetRunTensionReverse(FALSE);
			m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			return iResult;
		}
		m_plnkCtrlTabFeeder->SetRunTensionReverse(FALSE);
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);//110617.KKY
		ClutchOffSprocket1Axis();//110617.KKY
	}

	return iResult;
}

int MTabFeeder::WriteMultiMoveLog(CString strLog)
{
	CString fname = _T("..\\Log\\Feeder_RMultiMove.csv");
	CFileFind find;
	FILE *fp = NULL;
	if (!find.FindFile(fname)){
		//LKH_090226 : Writing UNICODE Log
//		fp = _wfopen(fname, _T("w+"));
		fp = _wfopen(fname, _T("wb+,ccs=UNICODE"));
		if (fp == NULL) return 0;
		fclose(fp);
		fp = NULL;
	}
	//LKH_090226 : Writing UNICODE Log
//	fp = _wfopen(fname, _T("a+"));
	fp = _wfopen(fname, _T("ab+,ccs=UNICODE"));
	if (fp == NULL) return 0;
	CTime cTime = CTime::GetCurrentTime();
	CString strLine;
	strLine.Format(_T("Unit : %d, %s, [%d-%d-%d], [%d:%d:%d], %.3f, %.3f,%.3f, %.3f\n"), 
		GetInstanceNo(), strLog, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),
		cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond(), GetSprocket1CurrentPos(TRUE), GetSprocket1CurrentPos(FALSE), GetSprocket2CurrentPos(TRUE), GetSprocket2CurrentPos(FALSE));
	fwprintf(fp, strLine);
	fclose(fp);
	return TRUE;
}

int MTabFeeder::WriteAutoChangeLog(CString strLog, int iResult)
{
	CTime cTime = CTime::GetCurrentTime();
	CString strLine;

	CString fname = _T("..\\Log\\AutoChange.csv");
	CFileFind find;
	FILE *fp = NULL;
	if (!find.FindFile(fname))
	{
		//LKH_090226 : Writing UNICODE Log
//		fp = _wfopen(fname, _T("w+"));
		fp = _wfopen(fname, _T("wb+,ccs=UNICODE"));
		if (fp == NULL) return 0;
		fclose(fp);
		fp = NULL;
	}
	//LKH_090226 : Writing UNICODE Log
//	fp = _wfopen(fname, _T("a+"));
	fp = _wfopen(fname, _T("ab+,ccs=UNICODE"));
	if (fp == NULL) return 0;
	strLine.Format(_T("Unit : %d, %s, [%d-%d-%d], [%d:%d:%d], %d\n"), 
		GetInstanceNo(), strLog, cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),
		cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond(), iResult);
	fwprintf(fp, strLine);
	fclose(fp);
	return TRUE;
}

double MTabFeeder::getRotationAlgne()
{
	// selsk_jdy Yl, Yr ���� �۾��� Tab Pitch ������ Ʋ�� ��쿡 ���ؼ� �ڵ� �ʿ�.
#ifdef DEF_SOURCE_SYSTEM
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;					//One Tab�� Pitch��
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;	//One Pitch�� ȸ����
#else
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;					//One Tab�� Pitch��
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;	//One Pitch�� ȸ����
#endif
	double dOneTabDist = iTabPitchCount * dOnePitchDist;								//One Tab�� ȸ����

	int iCount = static_cast<int>(DEF_ROTATION_ANGLE / dOneTabDist);
	double dAngle = dOneTabDist * iCount;
	return dAngle;
}

int MTabFeeder::TensionMaintenance(int iSprocket)
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	// jdy 12pm_s
	int iRepeatCount = 0;

	if (DEF_SPROCKET1_UNIT == iSprocket)
	{

//110617.Del		ClutchOffSprocket1Axis();

		iResult = m_plnkCtrlTabFeeder->FindWeightDownSensor();
		if (iResult)
			return iResult;
		iResult = m_plnkCtrlTabFeeder->EscapeWeightDownSensor();
		if (iResult)
			return iResult;


	}


	if (DEF_SPROCKET2_UNIT == iSprocket)
	{
		// jdy 12pm_s
		iRepeatCount = 0;

SPROCKET2_FORWARDING:
		// jdy 12pm_e
		//����������������������������������������������������������������������������
		//111021 sj_kjs
		if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			//if (m_plnkIO->IsOn(IN_FRONT_STEP_STOP_SW) || m_plnkIO->IsOn(IN_REAR_STEP_STOP_SW))
		{
			m_plnkSprocket1Axis->Stop();


			m_plnkSprocket2Axis->HomingStop();
			return generateErrorCode(315002);
		}
		//____________________________________
		if (TRUE == IsTensionLowerDetected() && TRUE == IsTensionLowerLimitDetected())
		{
			iResult = RMoveSprocket2Pos(getRotationAlgne(), FALSE);
			if (iResult)
				return iResult;
			// jdy 12pm_s
			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{
				iResult = RMoveSprocket2Pos(-getRotationAlgne() / 2, FALSE);
				if (iResult)
					return iResult;
			}
			
			iRepeatCount++;
			
			if (iRepeatCount < 4)
				goto SPROCKET2_FORWARDING;
				/*	
			if (TRUE == IsTensionLowerDetected() && TRUE == IsTensionLowerLimitDetected())
			{
				iResult = RMoveSprocket2Pos(getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}

			if (TRUE == IsTensionLowerDetected() && TRUE == IsTensionLowerLimitDetected())
			{
				iResult = RMoveSprocket2Pos(getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}
			
			if (TRUE == IsTensionLowerDetected() && TRUE == IsTensionLowerLimitDetected())
			{
				iResult = RMoveSprocket2Pos(getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}
			
			if (TRUE == IsTensionLowerDetected() && TRUE == IsTensionLowerLimitDetected())
			{
				iResult = RMoveSprocket2Pos(getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}
			*/	// jdy 12pm_e
		}
		
		/* jdy 12pm_s
		if (TRUE == IsTensionLowerDetected() || TRUE == IsTensionLowerLimitDetected())
		*/
		if (4 == iRepeatCount)
		// jdy 12pm_e
		{
			// 215026 = Sprocket2 Forward ȸ���Ͽ�����, Low sensor ����� ����.
			return generateErrorCode(215026);
		}
		//����������������������������������������������������������������������������
		
		// jdy 12pm_s
		iRepeatCount = 0;

SPROCKET2_BACKWARDING:
		// jdy 12pm_e
		//����������������������������������������������������������������������������
		//111021 sj_kjs
		if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			//if (m_plnkIO->IsOn(IN_FRONT_STEP_STOP_SW) || m_plnkIO->IsOn(IN_REAR_STEP_STOP_SW))
		{
			m_plnkSprocket1Axis->Stop();
			m_plnkSprocket2Axis->HomingStop();
			return generateErrorCode(315002);
		}
		//_______________________________________________
		if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
		{
			iResult = RMoveSprocket2Pos(-getRotationAlgne(), FALSE);
			if (iResult)
				return iResult;

			// jdy 12pm_s
			if (TRUE == IsTensionLowerDetected() && TRUE == IsTensionLowerLimitDetected())
			{
				iResult = RMoveSprocket2Pos(getRotationAlgne() / 2, FALSE);
				if (iResult)
					return iResult;
			}
			
			iRepeatCount++;

			if (iRepeatCount < 4)
				goto SPROCKET2_BACKWARDING;
			/*				
			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{
				iResult = RMoveSprocket2Pos(-getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}

			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{
				iResult = RMoveSprocket2Pos(-getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}
			
			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{
				iResult = RMoveSprocket2Pos(-getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}
			
			if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			{
				iResult = RMoveSprocket2Pos(-getRotationAlgne(), FALSE);
				if (iResult)
				{
					SetErrorLevel(_T("TAB FEEDER maintenance Tension"),2, __FILE__, __LINE__);
					return iResult;
				}
			}
			*/	// jdy 12pm_e
		}
		
		/* jdy 12pm_s
		if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
		*/
		if (4 == iRepeatCount)
		// jdy 12pm_e
		if (TRUE == IsTensionUpperDetected() || TRUE == IsTensionUpperLimitDetected())
			// 215027 = Sprocket2 Backward ȸ���Ͽ�����, Up sensor ����� ����.
			return generateErrorCode(215027);		
		//����������������������������������������������������������������������������
	}

	return ERR_TABFEEDER_SUCCESS;
}

const BOOL MTabFeeder::IsSprocket1ReverseRotation() const
{
	return m_bSprocket1ReverseRotation;
}

BOOL MTabFeeder::IsServoOnFeederReel()
{
	BOOL bOn = m_plnkFeederReelAxis->IsAmpEnable() && !m_plnkFeederReelAxis->IsAmpFault();
	return bOn;
}

BOOL MTabFeeder::IsServoOffFeederReel()
{
	BOOL bOff = !m_plnkFeederReelAxis->IsAmpEnable() || m_plnkFeederReelAxis->IsAmpFault();
	return bOff;
}

int	MTabFeeder::ServoOnOffFeederReel(BOOL bOn)
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	if (bOn)
	{
		iResult = m_plnkFeederReelAxis->ServoOn();

		//120411.KMS__________
		// Feeder Reel Motion Check Origin
		if (m_plnkFeederReelAxis->IsOriginReturn() == FALSE)
		{
			MTickTimer ReelHomeTimer;
			ReelHomeTimer.StartTimer();
			while(m_plnkFeederReelAxis->IsOriginReturn() == FALSE)
			{
				m_plnkFeederReelAxis->OriginReturn(FALSE);

				if (m_plnkFeederReelAxis->IsOriginReturn() == TRUE)
					break;
				if (ReelHomeTimer.MoreThan(10.0))
				{
					ReelHomeTimer.StopTimer();
					break;
				}
			}
		}
		//___________________
	}
	else
	{
		iResult = m_plnkFeederReelAxis->ServoOff();
		m_plnkFeederReelAxis->ResetOrigin();
	}
	return iResult;
}

int	MTabFeeder::VMove_FeederReel(int Dir, double dVel, int iAcl)
{
	int iResult = ERR_TABFEEDER_SUCCESS;
	BOOL bDir = ((Dir == -1) ? 0 : 1) ;
	iResult = m_plnkFeederReelAxis->VMove(dVel, iAcl, bDir);
	return iResult;
}

int MTabFeeder::VStop_FeederReel()
{
	int iResult = m_plnkFeederReelAxis->VStop();
	return iResult;
}

int MTabFeeder::CheckFeederReelOrigin()
{
	// Feeder Reel Motion Check Origin
	if (m_plnkFeederReelAxis->IsOriginReturn() == FALSE)
	{
		MTickTimer ReelHomeTimer;
		ReelHomeTimer.StartTimer();
		while(m_plnkFeederReelAxis->IsOriginReturn() == FALSE)
		{
			m_plnkFeederReelAxis->OriginReturn(FALSE);

			if (m_plnkFeederReelAxis->IsOriginReturn() == TRUE)
				break;
			if (ReelHomeTimer.MoreThan(10.0))
			{
				ReelHomeTimer.StopTimer();
				break;
			}
		}

		if (m_plnkFeederReelAxis->IsOriginReturn() == FALSE)
			// 215037 = Sprocket2 T �� �������� �Ǿ� ���� �ʽ��ϴ�.
			return generateErrorCode(215037);
	}

	return ERR_TABFEEDER_SUCCESS;
}
