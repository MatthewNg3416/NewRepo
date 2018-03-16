/* 
 * Control TabFeeder Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */
 
#include "StdAfx.h"
#include "MTickTimer.h"
#include "MSystemData.h"
#include "MPreBonderData.h"
#include "MProductInfoData.h"
#include "MPanelData.h"
#include "MProcessData.h"
#include "MLCNet.h"
#include "IRFID.h"
#include "MTabFeeder.h"
#include "MCtrlTabFeeder.h"
#include <math.h>
#include "common.h"
#include "MTabData.h"
#include "MManageOpPanel.h"
#include "MTrsAutoManager.h"
#include "DBTabLoss.h"

#include "MPlatformOlbSystem.h"

extern MPlatformOlbSystem	MOlbSystemPre;

/**
 * @stereotype constructor
 *
 * @param	commonData			: 공통 Component Data
 * @param	listRefComponent	: CtrlTabFeeder Component에서 참조할 Component Pointer
 * @param	datComponent		: CtrlTabFeeder Component Data
 */
MCtrlTabFeeder::MCtrlTabFeeder(SCommonAttribute commonData, SCtrlTabFeederRefCompList listRefComponent, SCtrlTabFeederData datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponent);
	Initialize();

	m_iTensionCount			= 0;
	m_dEncoderSum			= 0.0;

	m_bFlgSprocket1Home		= FALSE;
	m_bFlgSprocket2Home		= FALSE;
	m_bFlgFindTabIC			= FALSE;
	m_iBadICCount			= 0;
	m_iMoldCount			= 0;
	memset(m_bFlgTabIC, 0, sizeof(BOOL)*DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS);

	m_iZigUsedQtyEquip		= 0;
	m_iZigTotalQtyEquip		= 0;

#ifdef DEF_SOURCE_SYSTEM 
	m_iTabPitchCount		= m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
	m_dOnePitchDist			= 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	m_iTabPitchCount		= m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
	m_dOnePitchDist			= 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif
	m_dOneTabDist			= m_iTabPitchCount * m_dOnePitchDist;

	m_bConsumeStart[DEF_USING_REEL]	= TRUE;
	m_bConsumeStart[DEF_SPARE_REEL]	= TRUE;
	m_bPreHandState			= TRUE;
	m_bWarningState			= TRUE;

	m_bZigConsumeStart		= TRUE;
	m_bZigWarningState		= FALSE;
	m_bZigRFIDState			= FALSE;

	m_iSprocketFeedingCount	= 0;

	m_iXAssemble			= 0;
	m_iYAssemble			= 0;

	m_iSelectedReel			= DEF_USING_REEL;

	m_hTitleViewWnd			= NULL;

	if (DEF_TABFEEDER1 == m_plnkTabFeeder->GetInstanceNo())
	{
		for (int i = 0 ; i < DEF_MAX_REEL ; i++)
		{
			m_matTabICInfo[i] = m_plnkProductInfoData->m_matTabICInfo[DEF_TABFEEDER1][i];
//			m_matTabICInfo[i].m_sPORTID = "FEEDER1";
			m_matTabICInfo[i].m_sPORTID = m_plnkSystemData->m_strModulePortID[0];
			m_matTabICInfo[i].m_nM_KIND = 13;
		}
		m_matZigInfo = m_plnkProductInfoData->m_matZigInfo[DEF_TABFEEDER1];
//		m_matZigInfo.m_sPORTID = "FEEDER1";
		m_matZigInfo.m_sPORTID = m_plnkSystemData->m_strModulePortID[4];
		m_matZigInfo.m_nM_KIND = 12;
		InitializeTabType();
		m_plnkLCNet->m_oMat_Feeder1 = m_matTabICInfo[DEF_USING_REEL];
		m_plnkLCNet->m_oMat_Feeder3 = m_matTabICInfo[DEF_SPARE_REEL];
		m_plnkLCNet->m_oMat_ZigFeeder1 = m_matZigInfo;
	}
	else
	{
		for (int j = 0 ; j < DEF_MAX_REEL ; j++)
		{
			m_matTabICInfo[j] = m_plnkProductInfoData->m_matTabICInfo[DEF_TABFEEDER2][j];
//			m_matTabICInfo[j].m_sPORTID = "FEEDER2";
			m_matTabICInfo[j].m_sPORTID = m_plnkSystemData->m_strModulePortID[1];
			m_matTabICInfo[j].m_nM_KIND = 13;
		}
		m_matZigInfo = m_plnkProductInfoData->m_matZigInfo[DEF_TABFEEDER2];
//		m_matZigInfo.m_sPORTID = "FEEDER2";
		m_matZigInfo.m_sPORTID = m_plnkSystemData->m_strModulePortID[5];
		m_matZigInfo.m_nM_KIND = 12;
		InitializeTabType();
		m_plnkLCNet->m_oMat_Feeder2 = m_matTabICInfo[DEF_USING_REEL];
		m_plnkLCNet->m_oMat_Feeder4 = m_matTabICInfo[DEF_SPARE_REEL];
		m_plnkLCNet->m_oMat_ZigFeeder2 = m_matZigInfo;
	}

	/*
	//Material AutoChange  안하기로 하므로 삭제 
	if (0 != m_matTabICInfo[DEF_SPARE_REEL].m_nT_QTY)
	{
		if (SUCCESS == m_plnkTabFeeder->AbsorbNewReel())
			m_bConsumeStart[DEF_SPARE_REEL]	= FALSE;
		else
		{
			m_plnkTabFeeder->ReleaseNewReel(TRUE);
			m_matTabICInfo[DEF_SPARE_REEL].ResetData();
		}
	}
	*/

	m_bThreadLife = TRUE;
	m_pThread = NULL;
	m_pThreadSupplyReel = NULL;
	m_bRunTension = FALSE;
	m_bRunTensionReverse = FALSE;
	m_bStopTensionCompleted = TRUE;
	m_bManualOperation = FALSE;
	m_bFeederReady4DryRun = TRUE;
	
	m_eFeederReelMoveType = E_FEEDER_REEL_MOVE_NONE;
	m_bManualFind = FALSE;
	InitializeCriticalSection(&m_csFindSensor);
	threadRun();

	//eMatNG 12.08.14 yh
	m_iNG_CurTab = 0;
	m_iNG_OldTab = 0;
	m_iNG_Count = 0;
}

MCtrlTabFeeder::~MCtrlTabFeeder()
{
	DeleteCriticalSection(&m_csFindSensor);
	threadStop();
}

/**
 * CtrlTabFeeder Component를 초기화 한다.
 *
 * @return	int (0 = Success, Error Code = 그 외)
 */
int MCtrlTabFeeder::Initialize(void)
{
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::InitializeTabType(void)
{
	//Tab Type Setting
	switch (m_plnkSystemData->m_eSystemType)
	{
	default:
	case SYSTEM_TYPE_SOURCE:
		m_matTabICInfo[DEF_USING_REEL].m_sM_STEP = "SOURCE";
		m_matTabICInfo[DEF_SPARE_REEL].m_sM_STEP = "SOURCE";
		m_matZigInfo.m_sM_STEP = "SOURCE";
		break;
	case SYSTEM_TYPE_GATE:
		m_matTabICInfo[DEF_USING_REEL].m_sM_STEP = "GATE";
		m_matTabICInfo[DEF_SPARE_REEL].m_sM_STEP = "GATE";
		m_matZigInfo.m_sM_STEP = "GATE";
		break;
		/* 삭제 예정.
	default:
		m_matTabICInfo[DEF_USING_REEL].m_sM_STEP = "SOURCE";
		m_matTabICInfo[DEF_SPARE_REEL].m_sM_STEP = "SOURCE";
		m_matZigInfo.m_sM_STEP = "SOURCE";
		break;
		*/
	}

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

/**
* MCtrlTabFeeder에 Data Parameter를 설정한다. 
* @param	datcomponent : 설정할 MCtrlTabFeeder에 Parameter
*/
int	MCtrlTabFeeder::SetData(SCtrlTabFeederData datcomponent)
{
	m_plnkSystemData		= datcomponent.m_plnkSystemData;
	m_plnkTabData			= datcomponent.m_plnkTabData;
	m_plnkProductInfoData	= datcomponent.m_plnkProductInfoData;
	m_plnkPanelData			= datcomponent.m_plnkPanelData;
	m_plnkPreBonderData		= datcomponent.m_plnkPreBonderData;
	m_plnkProcessData		= datcomponent.m_plnkProcessData;
	m_plnkDBTabLoss			= datcomponent.m_plnkDBTabLoss;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

/**
* MCtrlTabFeeder에 Component List를 설정한다.
* @param	listRefComponents : 설정할 MCtrlTabFeeder에 Component List
*/
int	MCtrlTabFeeder::AssignComponents(SCtrlTabFeederRefCompList listRefComponents)
{
	m_plnkTabFeeder		= listRefComponents.m_plnkTabFeeder;
	//KKY_____
//	m_plnkTabCarrier	= listRefComponents.m_plnkTabCarrier;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_plnkTabCarrier[i] = listRefComponents.m_plnkTabCarrier[i];
	}
	//________
	m_plnkLCNet			= listRefComponents.m_plnkLCNet;
	m_plnkRFID			= listRefComponents.m_plnkRFID;
	m_plnkManageOpPanel	= listRefComponents.m_plnkManageOpPanel;	
	//111014 SJ_KJS
	//Automanager 생성 되기 전에  Control단이 생성 되므로 0x00000 이 들어간다. 
//	m_plnkTrsAutoManager = listRefComponents.m_plnkTrsAutoManager;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::FeedTabIC()
{
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		if (FALSE == m_plnkSystemData->m_bUseDryrunFeed)
		{
			//For Test...StartSW or ResetSW를 1초이상 누르면 Feeder 다 소모되었음...
			BOOL bSwitchPushed = (DEF_TABFEEDER1 == m_iInstanceNo) ? m_plnkManageOpPanel->GetStartSWStatus() : m_plnkManageOpPanel->GetResetSWStatus();
			if (TRUE == bSwitchPushed)
			{
				DWORD sTime = GetTickCount();
				while(1)
				{
					Sleep(10);
					bSwitchPushed = (DEF_TABFEEDER1 == m_iInstanceNo) ? m_plnkManageOpPanel->GetStartSWStatus() : m_plnkManageOpPanel->GetResetSWStatus();
					if (FALSE == bSwitchPushed)
						return ERR_CTRL_TABFEEDER_SUCCESS;
					if (GetTickCount() - sTime > 1000)
						break;
				}
				SetFeederReady4DryRun(FALSE);
				SetFindTabICFlag(FALSE);

				return generateErrorCode(315001);
			}
		}	
		
		if (FALSE == m_plnkSystemData->m_bUseDryrunFeed)
			return ERR_CTRL_TABFEEDER_SUCCESS;
	}

/*	if (!m_plnkTabFeeder->IsReleasePress())
	{
		iResult = m_plnkTabFeeder->ReleasePress();
		if (iResult)
			return iResult;
	}
*/	

	

	if (!m_plnkTabFeeder->IsInPressPos(DEF_PRESS_READY_POS))
	{
		iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_READY_POS);
		if (iResult)
			return iResult;
	}
	
	
	
	iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
	if (iResult)
		return iResult;

//#endif

/*
	//For Test...StartSW or ResetSW를 1초이상 누르면 Feeder 다 소모되었음...
	BOOL bSwitchPushed = (DEF_TABFEEDER1 == m_iInstanceNo) ? m_plnkManageOpPanel->GetStartSWStatus() : m_plnkManageOpPanel->GetResetSWStatus();
	if (TRUE == bSwitchPushed)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			bSwitchPushed = (DEF_TABFEEDER1 == m_iInstanceNo) ? m_plnkManageOpPanel->GetStartSWStatus() : m_plnkManageOpPanel->GetResetSWStatus();
			if (FALSE == bSwitchPushed)
				return ERR_CTRL_TABFEEDER_SUCCESS;
			if (GetTickCount() - sTime > 1000)
				break;
		}
		SetFeederReady4DryRun(FALSE);
		SetFindTabICFlag(FALSE);
		return generateErrorCode(315001);
	}
*/
	int iBadTabCount = getBadTabCount();

	
	if (m_plnkSystemData->m_nSprocket1ClutchMode == 2)
		m_plnkTabFeeder->ClutchOffSprocket1Axis();
	else
		m_plnkTabFeeder->ClutchOnSprocket1Axis();
	SetFeederReelMoveType(E_FEEDER_FIND_DOWN);

	

SOURCE_FEEDING:
	
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		iResult = feedMotor();	
		if (iResult)
		{
			SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			return iResult;
		}
	}
	else
	{
		if (m_iBadICCount > INT(m_plnkSystemData->m_uiBadTabLimitCount))
		{
			SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			m_iBadICCount = 0;
			SetFindTabICFlag(FALSE);

			// 315001 = BadIC 숫자가 System Data에 설정된 Limit Count수를 초과하였습니다.
			return generateErrorCode(315001);
		}

		if (FALSE == m_bFlgTabIC[iBadTabCount])
		{
			iResult = feedMotor();
			if (iResult)
			{
				SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
				return iResult;
			}

			goto SOURCE_FEEDING;
		}
	}

	if (m_plnkSystemData->m_nSprocket1ClutchMode == 0)	//Feeding후 ClutchOff
		m_plnkTabFeeder->ClutchOffSprocket1Axis();
	//111019 sj_kjs 임시..
//	SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

/** Motor Feeding  */
int MCtrlTabFeeder::feedMotor()
{
//	ASSERT(DRY_RUN_MODE != m_plnkSystemData->m_eRunMode);

	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

	//120410.kms___________
	iResult = m_plnkTabFeeder->CheckFeederReelOrigin();
	if (iResult)
		return iResult;
	//_____________________

	//@130119.KKY.Reel축 떨림으로 동작하지 않을경우________
	DWORD sTime = GetTickCount();
	while(TRUE == m_plnkTabFeeder->IsTensionUpperDetected() || TRUE == m_plnkTabFeeder->IsTensionUpperLimitDetected())
	{
		Sleep(10);
		if (GetTickCount() - sTime > 5000)
			return generateErrorCode(315029);
	}
	//@________________________________

	int iFeedingTab = 1;

	if (2 == m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[m_iInstanceNo])
	{
		int iBadTabCount = getBadTabCount();
		if (FALSE == m_bFlgTabIC[iBadTabCount - 1])
			iFeedingTab = 2;
	}

	int rgiSensorType[2];	// Left : 0, Right : 1
	if (TRUE == m_plnkPreBonderData->m_bUseTCP)
	{
//@		rgiSensorType[0] = DEF_BADTABDETECTOR_LEFT_SENSOR_TCP;
//@		rgiSensorType[1] = DEF_BADTABDETECTOR_RIGHT_SENSOR_TCP;
		rgiSensorType[0] = DEF_BADTABDETECTOR_SENSOR_TCP;
		rgiSensorType[1] = NULL;
	}
	else
	{
//@		rgiSensorType[0] = DEF_BADTABDETECTOR_LEFT_SENSOR_COF;
//@		rgiSensorType[1] = DEF_BADTABDETECTOR_RIGHT_SENSOR_COF;
		rgiSensorType[0] = DEF_BADTABDETECTOR_SENSOR_COF;
		rgiSensorType[1] = NULL;
	}

	// Bad IC 여부 Check : Left Sensor
	m_bFlgTabIC[0] = m_plnkTabFeeder->IsBadTabDetected(rgiSensorType[0]);
	
	if (TRUE == m_bFlgTabIC[0])
		m_iBadICCount = 0;
	else 
		m_iBadICCount++;

	// P-Reel 회수 Motor 동작.
	m_bRunTension = TRUE;

	iResult = m_plnkTabFeeder->RMultiMoveSprocketPos(m_dOneTabDist*(double)iFeedingTab);

	//m_bRunTension = FALSE;

	if (iResult)
		return iResult;

	for (int i = DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS - 1; i > iFeedingTab - 1; i--)
		m_bFlgTabIC[i] = m_bFlgTabIC[i - iFeedingTab];

	// Bad IC 여부 Check : Right Sensor - 2 tab 한 번에 Feeding 한 경우만 해당됨.
	if (2 == iFeedingTab)
	{
		m_bFlgTabIC[1] = m_plnkTabFeeder->IsBadTabDetected(rgiSensorType[1]);
		if (FALSE == m_bFlgTabIC[1])
			m_iBadICCount++;
	}
	m_bRunTension = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::FeedTabIC(int iSel)
{
	ASSERT(iSel == -2 || iSel == -1 || iSel == 1 || iSel == 2);

	// Sprocket1 Tension 유지..
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;
	if (FALSE == m_plnkTabFeeder->IsReleasePress())
	{
		iResult = m_plnkTabFeeder->ReleasePress();
		if (iResult)
			return iResult;
	}

	iResult = prepareBeforeFeeding();
	if (iResult)
		return iResult;


	BOOL bOldClutchStatus = m_plnkTabFeeder->IsClutchOnSprocket1Axis();
	if (m_plnkSystemData->m_nSprocket1ClutchMode == 2)
		m_plnkTabFeeder->ClutchOffSprocket1Axis();
	else
		m_plnkTabFeeder->ClutchOnSprocket1Axis();

	// +1 Pitch 이동
	if (iSel == 1)
	{
		// Pitch 이동시 FindTabIC Flag 해제.
		SetFindTabICFlag(FALSE);
		iResult = m_plnkTabFeeder->RMultiMoveSprocketPos(m_dOnePitchDist);
		if (iResult)
			return iResult;
	}
	// -1 Pitch 이동
	else if (iSel == -1)
	{
		SetFindTabICFlag(FALSE);
		SetRunTensionReverse(TRUE);
		iResult = m_plnkTabFeeder->RMultiMoveSprocketPos(-m_dOnePitchDist);
		if (iResult) 
		{
			SetRunTensionReverse(FALSE);
			return iResult;
		}
		SetRunTensionReverse(FALSE);
	}
	// +1 Tab 이동
	else if (iSel == 2)
	{
		int iSensorType;	// Left Sensor
		if (TRUE == m_plnkPreBonderData->m_bUseTCP)
//@			iSensorType = DEF_BADTABDETECTOR_LEFT_SENSOR_TCP;
			iSensorType = DEF_BADTABDETECTOR_SENSOR_TCP;
		else
//@			iSensorType = DEF_BADTABDETECTOR_LEFT_SENSOR_TCP;
			iSensorType = DEF_BADTABDETECTOR_SENSOR_COF;

		m_bFlgTabIC[0] = m_plnkTabFeeder->IsBadTabDetected(iSensorType);

		if (TRUE == m_bFlgTabIC[0])
			m_iBadICCount = 0;
		else 
			m_iBadICCount++;
		iResult = m_plnkTabFeeder->RMultiMoveSprocketPos(m_dOneTabDist);
		if (iResult)
		{
			SetFindTabICFlag(FALSE);
			return iResult;
		}

		Sleep(50);

		for (int i = DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS - 1; i > 0; i--)
			m_bFlgTabIC[i] = m_bFlgTabIC[i - 1];
	}
	// -1 Tab 이동
	else if (iSel == -2)
	{
		int iSensorType;	// Left Sensor
		if (TRUE == m_plnkPreBonderData->m_bUseTCP)
//@			iSensorType = DEF_BADTABDETECTOR_LEFT_SENSOR_TCP;
			iSensorType = DEF_BADTABDETECTOR_SENSOR_TCP;
		else
//@			iSensorType = DEF_BADTABDETECTOR_LEFT_SENSOR_COF;
			iSensorType = DEF_BADTABDETECTOR_SENSOR_COF;

		m_bFlgTabIC[0] = m_plnkTabFeeder->IsBadTabDetected(iSensorType);

		SetRunTensionReverse(TRUE);
		iResult = m_plnkTabFeeder->RMultiMoveSprocketPos(-m_dOneTabDist);
		if (iResult)
		{
			SetRunTensionReverse(FALSE);
			SetFindTabICFlag(FALSE);
			return iResult;
		}
		SetRunTensionReverse(FALSE);
		Sleep(50);

		for (int i = 0; i < DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS - 1; i++)
			m_bFlgTabIC[i] = m_bFlgTabIC[i + 1];
	}

	m_plnkTabFeeder->ClutchOffSprocket1Axis();

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::PressTapeStep1()
{
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		if (FALSE == m_plnkSystemData->m_bUseDryrunFeed)
		{
			Sleep(100);

			//110704.kms___________________
			//For Dry Run
			iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS);
			if (iResult)
				return iResult;
			//_____________________________

			return ERR_CTRL_TABFEEDER_SUCCESS;
		}
	}

	if (TRUE == m_plnkTabFeeder->IsAbsorbPress())
	{
		if (TRUE == m_plnkTabFeeder->IsInPressPos(DEF_PRESS_DOWN_POS))
			return ERR_CTRL_TABFEEDER_SUCCESS;

		// 315019 = Tap Press 불가. [Press 가 이미 TabIC 흡착하고 있음. 제거 필요.]
		return generateErrorCode(315019);
	}

	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		if (!m_plnkTabFeeder->IsPressMoldDetect())
		{
			// 315013 = TabIC 타발 작업을 진행할 수 없습니다. [금형 감지 안됨 : 센서 확인 필요.]
			return generateErrorCode(315013);
		}
	}

	/*
	// Reel Cyl Down
	int iResult = m_plnkTabFeeder->DownPressReelCyl();
	if (iResult)
	{
		SetErrorLevel(_T("TABFEEDER press Tape Step1"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkTabFeeder->DownPressClampCyl();
	if (iResult)
	{
		SetErrorLevel(_T("TABFEEDER press Tape Step1"),1, __FILE__, __LINE__);
		return iResult;
	}
	*/
//	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;


	if (m_plnkSystemData->m_nPressActionMode == 0)
		iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS);
	else
		iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS, FALSE);
	if (iResult)
		return iResult;
	//________________________

	m_plnkTabFeeder->AbsorbPress(TRUE);

	iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_DOWN_POS);
	if (iResult)
		return iResult;

	//eMatNG 12.08.14 yh
	m_iNG_CurTab++;


	if (m_plnkSystemData->m_nPressActionMode == 1)
	{
		iResult = m_plnkTabFeeder->Wait4DoneMoldPin();
		if (iResult)
			return iResult;
	}

	m_plnkTabFeeder->ClutchOffSprocket1Axis();

	int iBadTabCount = getBadTabCount();
	m_bFlgTabIC[iBadTabCount] = FALSE;

	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		iResult = m_plnkTabFeeder->AbsorbPress();
		if (iResult)
		{
			m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_UP_POS);

			//2009.10.27 CYJ 공정 오명수D 요청
			m_plnkTabFeeder->MoldBlowOn();

//			if (m_plnkTrsAutoManager->GetOPMode() != MANUAL_MODE)
			if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
			{				
				m_plnkDBTabLoss->AppendFieldCount(eTP);
			}

			return iResult;
		}
	}

	m_matTabICInfo[DEF_USING_REEL].m_nU_QTY++;
	m_matTabICInfo[DEF_USING_REEL].m_nR_QTY--;
	m_iSprocketFeedingCount++;


	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::PressTapeStep2()
{
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		if (FALSE == m_plnkSystemData->m_bUseDryrunFeed)
		{
			Sleep(100);

			//110704.kms__________
			//For Dry Run
			iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS, FALSE);
			if (iResult)
				return iResult;
			//____________________

			return ERR_CTRL_TABFEEDER_SUCCESS;
		}
	}

//	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;
	
	if (!m_plnkTabFeeder->IsReleasePress())
	{
		iResult = m_plnkTabFeeder->ReleasePress();
		if (iResult)
			return iResult;
	}

	iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS, FALSE);
	if (iResult)
		return iResult;

	iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_READY_POS);
	if (iResult)
		return iResult;


	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE) //SJ_YYK 150226
	{
		iResult = m_plnkTabFeeder->Wait4DoneMoldPin();
		if (iResult)
			return iResult;
	}

	// Park, YK - For Online

	//TabIC Event
	if (FALSE == m_bConsumeStart[DEF_USING_REEL])
	{
		m_bPreHandState = FALSE;
		m_bWarningState = FALSE;
		m_bConsumeStart[DEF_USING_REEL] = TRUE;
		m_plnkLCNet->MaterialReport(eMatConsumeStart, &m_matTabICInfo[DEF_USING_REEL]);
	}

	if (!m_bPreHandState && m_matTabICInfo[DEF_USING_REEL].m_nR_QTY < m_plnkProductInfoData->m_iPreHandQty)
	{
		m_bPreHandState = TRUE;
		m_plnkLCNet->MaterialReport(eMatPrehand, &m_matTabICInfo[DEF_USING_REEL]);
	}			

	if (!m_bWarningState && m_matTabICInfo[DEF_USING_REEL].m_nR_QTY < m_plnkProductInfoData->m_iWarningQty)
	{
		m_bWarningState = TRUE;
		m_plnkLCNet->MaterialReport(eMatWarning, &m_matTabICInfo[DEF_USING_REEL]);
	}			

	// Zig Event
	m_iMoldCount++;

	if(m_plnkSystemData->m_bUseMoldValidation == TRUE)
	{
	}

	if (!m_bZigConsumeStart)
	{
		m_bZigConsumeStart = TRUE;
		m_plnkLCNet->MaterialReport(eMatConsumeStart, &m_matZigInfo);
	}

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::ExecuteAutoHoming(BOOL bHomingSprocket1)
{
	int i = 0;
	int iBadTabCount = getBadTabCount();
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

#ifdef DEF_SOURCE_SYSTEM
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	int iTabPitchCount = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
	double dOnePitchDist = 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif
	double dOneTabDist = iTabPitchCount * dOnePitchDist;

	iResult = prepareBeforeFeeding(DEF_SPROCKET2_UNIT);
	if (iResult)
		return iResult;

	m_bRunTension = TRUE;

	// Sercos의 원점 센서 감지 후 원점 복귀 함수를 이용하여 원점 복귀 완료 후 Offset만큼 이동.
	if (FALSE == GetHomeFlagSprocket2())
	{
		m_bRunTension = TRUE;
		//Sensor원점 복귀 동작후, 원점복귀시 이동거리만큼 다시 복귀하는 동작까지
		iResult = m_plnkTabFeeder->HomingSprocket(DEF_SPROCKET2_UNIT);
		if (iResult != ERR_CTRL_TABFEEDER_SUCCESS)
		{
			m_bRunTension = FALSE;
			return iResult;
		}

		iResult = FindWeightDownSensor();
		if (iResult)
		{
			m_bRunTension = FALSE;
			return iResult;
		}
		if (DEF_TABFEEDER1 == m_plnkTabFeeder->GetInstanceNo())
		{
			//iResult = m_plnkTabFeeder->RMoveSprocket2Pos(m_plnkSystemData->m_dSprocket2_F_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset);
			iResult = m_plnkTabFeeder->RMoveSprocket2Pos(m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset);
			if (iResult != ERR_CTRL_TABFEEDER_SUCCESS)
			{
				m_bRunTension = FALSE;
				return iResult;
			}
		}
		else
		{
			//iResult = m_plnkTabFeeder->RMoveSprocket2Pos(m_plnkSystemData->m_dSprocket2_R_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset);
			iResult = m_plnkTabFeeder->RMoveSprocket2Pos(m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset);
			if (iResult != ERR_CTRL_TABFEEDER_SUCCESS)
			{
				m_bRunTension = FALSE;
				return iResult;
			}
		}

		// hongju_090903 
		// Sprocket1 Tension 유지..
		iResult = m_plnkTabFeeder->TensionMaintenance(DEF_SPROCKET1_UNIT);
		if (iResult)
		{
			m_bRunTension = FALSE;
			return iResult;
		}

		
		SetHomeFlagSprocket2(TRUE);
	}

	// 5. BadTab Detector Work 위치로 이동한다. 
	iResult = m_plnkTabFeeder->SafeMoveBadTabPos(DEF_BADTABDETECTOR_WORK_POS);
	if (iResult)
	{
		m_bRunTension = FALSE;
		return iResult;
	}

	Sleep(50);

	//3. Auto Find IC 실행(TabIC위치를 찾는동작)
	iResult = ExecuteAutoFindIC();
	if (iResult)
	{
		m_bRunTension = FALSE;
		return iResult;
	}

	//4. BadTab 체크 센서부터 Press 위치까지의 Tab 수 만큼 Back : 단, AutoChange 직후에는 Skip.
	if (FALSE == bHomingSprocket1)
	{
		SetFeederReelMoveType(E_FEEDER_ESCAPE_DOWN);
		m_plnkTabFeeder->ClutchOffSprocket1Axis();  //SJ_YYK 150213 Add..
		SetRunTensionReverse(TRUE);
		iResult = m_plnkTabFeeder->RMultiMoveSprocketPos(-m_dOneTabDist * iBadTabCount);
		if (iResult)
		{
			SetRunTensionReverse(FALSE);
			SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			m_bRunTension = FALSE;
			return iResult;
		}
		SetRunTensionReverse(FALSE);

		SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
	}



	SetFeederReelMoveType(E_FEEDER_FIND_DOWN);
	for (int i = 0; i < iBadTabCount; i++)
	{
		iResult = FeedTabIC(2);
		if (iResult)
		{
			SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			m_bRunTension = FALSE;
			return iResult;
		}
	}

	SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);

	//5. Feed TabIC
	if (!m_plnkTabFeeder->IsAbsorbPress())
	{
		iResult = FeedTabIC();
		if (iResult)
		{
			m_bRunTension = FALSE;
			return iResult;
		}
	}

	m_bRunTension = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::ExecuteAutoFindIC()
{
	int	iResult = ERR_CTRL_TABFEEDER_SUCCESS;

#ifdef DEF_SETUP
	ASSERT(TRUE == GetHomeFlagSprocket2());
#endif

	m_plnkTabFeeder->ClutchOnSprocket1Axis();

	if (FALSE == GetFindTabICFlag())
	{
		int iSensorType;
		if (TRUE == m_plnkPreBonderData->m_bUseTCP)
//@			iSensorType = DEF_BADTABDETECTOR_LEFT_SENSOR_TCP;
			iSensorType = DEF_BADTABDETECTOR_SENSOR_TCP;
		else
//@			iSensorType = DEF_BADTABDETECTOR_LEFT_SENSOR_COF;
			iSensorType = DEF_BADTABDETECTOR_SENSOR_COF;

		//2009.04.19 SESL_CYJ SESL에서 IC 낭비를 줄이려고 새 Reel 앞에 더미를 길게 붙여서 에러 발생.Check 양을 늘림.
//		for (int i = 0; i < 8 * m_iTabPitchCount; i++)
		for (int i = 0; i < 30 * m_iTabPitchCount; i++)
		{
			if (m_plnkTabFeeder->IsTensionUpperDetected() || m_plnkTabFeeder->IsTensionUpperLimitDetected())
			{
				iResult = FindWeightDownSensor();
				if (iResult)
					return iResult;
			}

			// hongju_SESL 수정 필요...BadTab감지하고 특정Tab 만큼이동해서 다시 확인 필요..Tape 붙였을 경우 오 동작함..
			if (TRUE == m_plnkTabFeeder->IsBadTabDetected(iSensorType))
			{
				SetFindTabICFlag(TRUE);
				return ERR_CTRL_TABFEEDER_SUCCESS;
			}
			iResult = FeedTabIC(1);
			if (iResult)
				return iResult;
			
			Sleep(100);
		}

		SetFindTabICFlag(FALSE);
		return generateErrorCode(315006);
	}
	
	SetFindTabICFlag(TRUE);

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

void MCtrlTabFeeder::SetHomeFlagSprocket1(BOOL bFlag)			
{ 
	m_bFlgSprocket1Home = bFlag; 
}

void MCtrlTabFeeder::SetHomeFlagSprocket2(BOOL bFlag)
{ 
	m_bFlgSprocket2Home = bFlag;

	if (FALSE == bFlag)
		m_bFlgFindTabIC = bFlag;
}

void MCtrlTabFeeder::SetFindTabICFlag(BOOL bFlag)
{
	m_bFlgFindTabIC = bFlag;
}

BOOL MCtrlTabFeeder::GetHomeFlagSprocket1()					
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		return TRUE;

	return m_bFlgSprocket1Home; 
}

BOOL MCtrlTabFeeder::GetHomeFlagSprocket2()					
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		return TRUE;

	return m_bFlgSprocket2Home; 
}

BOOL MCtrlTabFeeder::GetFindTabICFlag()
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		return TRUE;

	return m_bFlgFindTabIC;
}

// AutoRun Initialization  
int MCtrlTabFeeder::InitializeTabFeeder()
{
	ReadZigInfo();
	//int iResult;

	//SJ_YYK 110415 Test 임시삭제..
	
	int iResult = m_plnkTabFeeder->ReleasePress();
	if (iResult)
		return iResult;
	Sleep(500);		

	//SJ_YYK 110415 Test Add...
	/*if(!m_plnkTabFeeder->IsAbsorbPress())
	{

		iResult = prepareBeforeFeeding();
		if (iResult)
			return iResult;
	}*/
	
	iResult = prepareBeforeFeeding();
		if (iResult)
			return iResult;
	

	if (FALSE == m_plnkTabFeeder->IsInBadTabPos(DEF_BADTABDETECTOR_WORK_POS))
	{
		iResult = m_plnkTabFeeder->SafeMoveBadTabPos(DEF_BADTABDETECTOR_WORK_POS);
		if (iResult)
			return generateErrorCode(315017);
	}

	if (FALSE == m_plnkTabFeeder->IsInSupplierPos(DEF_SUPPLIER_WORK_POS))
		return generateErrorCode(315018);
	
	//SJ_YYK 110415 Add Test.//
	/*
	if(!m_plnkTabFeeder->IsAbsorbPress())
	{
		iResult = FeedTabIC();
		if (iResult)
		{
			SetErrorLevel(_T("TABFEEDER initialize Tab Feeder"),1, __FILE__, __LINE__);
			return iResult;
		}
	}
	*/
	
	iResult = FeedTabIC();
	if (iResult)
		return iResult;

	// Vacuum 체크
	// 흡착 되어 있으면 Error를 막기 위해서 TabIC를 떨어뜨리고, 강제로 배출한다.
	// 110415 임시 삭제/..
	
	if (m_plnkTabFeeder->IsAbsorbPress())
	{
		iResult = m_plnkTabFeeder->ReleasePress();
		if (iResult)
			return iResult;

		Sleep(500);

		for (int i = 0; i < 5; i++)
		{
			iResult = FeedTabIC(2);
			if (iResult)
				return iResult;
			iResult = FindWeightDownSensor();
			if (iResult)
				return iResult;
		}
	}

	// Sprocket2 homing 여부 확인.
	if (!GetHomeFlagSprocket2() || !GetFindTabICFlag())
	{
		iResult = ExecuteAutoHoming();
		if (iResult)
			return iResult;
	}

	Sleep(500);
	// Sprocket Setposition Zero;
	m_plnkTabFeeder->SetSprocket1CurrentPos(0.0);
	m_plnkTabFeeder->SetSprocket2CurrentPos(0.0);
	Sleep(500);

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

void MCtrlTabFeeder::SetMoldCount(int iMoldCount)			
{ 
	m_iMoldCount = iMoldCount; 
}

int MCtrlTabFeeder::GetMoldCount()			
{ 
	return m_iMoldCount; 
}

int MCtrlTabFeeder::prepareReelAutoChange()
{
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;


	return iResult;
}

int MCtrlTabFeeder::ExecuteReelAutoChange()
{
	m_bRunTension = FALSE;
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::prepareBeforeFeeding(int iCheckSkipSprocketNo)
{
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

	// 동작 전 체크 사항(모든 축 원점 복귀 확인)
	iResult = m_plnkTabFeeder->CheckBadTabOrigin();
	if (iResult)
		return iResult;

	iResult = m_plnkTabFeeder->CheckPressOrigin();
	if (iResult)
		return iResult;

	if (DEF_SPROCKET1_UNIT != iCheckSkipSprocketNo)
	{
		iResult = m_plnkTabFeeder->CheckSprocket1Origin();
		if (iResult)
			return iResult;
	}
	
	if (DEF_SPROCKET2_UNIT != iCheckSkipSprocketNo)
	{
		iResult = m_plnkTabFeeder->CheckSprocket2Origin();
		if (iResult)
			return iResult;
	}

	iResult = m_plnkTabFeeder->CheckMoldPinOrigin();
	if (iResult)
		return iResult;

	//120410.kms___________
	iResult = m_plnkTabFeeder->CheckFeederReelOrigin();
	if (iResult)
		return iResult;
	//_____________________

	iResult = m_plnkTabFeeder->ServoOnOffFeederReel(TRUE);
	if (iResult)
		return iResult;
	
	// Induction Motor On
	iResult = m_plnkTabFeeder->RecoveryIndMotorOn();
	if (iResult)
		return iResult;


	// hongju 수정... 추후 확인 필요.
/*	iResult = m_plnkTabFeeder->BrushMotorOn();
	if (iResult)
		return iResult;
*/
	// Feeder, Tension, Recovery Sprocket Down
	iResult = m_plnkTabFeeder->CloseRecoverySprocketCyl();
	if (iResult)
		return iResult;

/*
	iResult = m_plnkTabFeeder->CloseFeederSprocketCyl();
	if (iResult)
	{
		SetErrorLevel(_T("TABFEEDER prepare PreFeeding"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkTabFeeder->CloseTensionSprocketCyl();
	if (iResult)
	{
		SetErrorLevel(_T("TABFEEDER prepare PreFeeding"),1, __FILE__, __LINE__);
		return iResult;
	}
*/

	// Press 안전 위치로... Press Clamp, Press Reel Up
	if (FALSE == m_plnkTabFeeder->IsInPressPos(DEF_PRESS_READY_POS))
	{
		iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_READY_POS);
		if (iResult)
			return iResult;
	}

	if (FALSE == m_plnkTabFeeder->IsInMoldPinPos(DEF_MOLDPIN_RELEASE_POS))
	{
		iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
		if (iResult)
			return iResult;
	}


	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::ReadZigInfo()
{
	CString strTemp;
//	int iReadData;
	m_bZigRFIDState = FALSE; // 통신 초기 상태. 

	// ZIG ID
	m_strZigBatchID = m_plnkRFID->ReadData(ZIGID);
	if (m_strZigBatchID == "Data Read Error")
		return generateErrorCode(315008);

	m_strZigBatchID.Remove(' ');
	m_matZigInfo.m_sM_BATCHID = m_strZigBatchID;

	//100825.KMS 금형 온라인으로 상위에서 타발수 제어 하기 위해 설비 에서 RFID로 읽어서 쓰는 것 제거
/*	
	// Mat code
	strTemp = m_plnkRFID->ReadData(MATCODE);
	if (strTemp == "Data Read Error")
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315008);
	}
	strTemp.Remove(' ');
	m_matZigInfo.m_sM_CODE = strTemp;

	// RGD 후 타발 제한 횟수 읽기
	m_strZigTotalQtyAfterGRD = m_plnkRFID->ReadData(TOT_QTY_AFTER_REG);
	if (m_strZigTotalQtyAfterGRD == "Data Read Error")
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315008);
	}
	m_strZigTotalQtyAfterGRD.Remove(' ');
	iReadData = _wtoi(m_strZigTotalQtyAfterGRD);
	if (iReadData < 0 || iReadData > 99999999)
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315009);
	}
	m_matZigInfo.m_nT_QTY = iReadData;

	// RGD 후 타발 사용 횟수 읽기
	m_strZigUsedQtyAfterRGD = m_plnkRFID->ReadData(REG_USED_QTY);
	if (m_strZigUsedQtyAfterRGD == "Data Read Error")
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315008);
	}
	m_strZigUsedQtyAfterRGD.Remove(' ');
	iReadData = _wtoi(m_strZigUsedQtyAfterRGD);
	if (iReadData < 0 || iReadData > 99999999)
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315009);
	}
	m_matZigInfo.m_nU_QTY = iReadData;

	// 전체 타발 제한 횟수 읽기
	m_strZigTotalQty = m_plnkRFID->ReadData(TOT_QTY_ZIG);
	if (m_strZigTotalQty == "Data Read Error")
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315008);
	}
	m_strZigTotalQty.Remove(' ');
	m_matZigInfo.m_sPORT_STATE = m_strZigTotalQty;
	iReadData = _wtoi(m_strZigTotalQty);
	m_iZigTotalQtyEquip = iReadData;

	//전체 타발 사용 횟수 읽기
	m_strZigUsedQty = m_plnkRFID->ReadData(ZIG_USED_QTY);
	if (m_strZigUsedQty == "Data Read Error")
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315008);
	}
	m_strZigUsedQty.Remove(' ');
	m_matZigInfo.m_sPORT_TYPE = m_strZigUsedQty;
	iReadData = _wtoi(m_strZigUsedQty);
	if (iReadData < 0 || iReadData > 99999999)
	{
		SetErrorLevel(_T("TABFEEDER read Zig Info"),1, __FILE__, __LINE__);
		return generateErrorCode(315009);
	}
	m_iZigUsedQtyEquip = iReadData;

  */
	m_bZigRFIDState = TRUE; // 통신 상태. 모든 data를 읽어야만 TRUE

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::WriteZigInfo()
{
	int iResult;
	char strData[50];

	m_bZigRFIDState = FALSE; // 통신 초기 상태. 

	memset(strData,0,sizeof(strData));


	// RGD후 누적 타발 횟수 쓰기
	sprintf(strData,"        ");
	iResult = m_plnkRFID->WriteData(REG_USED_QTY,strData);
	Sleep(100);
	sprintf(strData,"%d",m_matZigInfo.m_nU_QTY);
	iResult = m_plnkRFID->WriteData(REG_USED_QTY, strData);
	if (iResult)
		return iResult;

	// 전체 타발 횟수 쓰기
	sprintf(strData,"        ");
	iResult = m_plnkRFID->WriteData(ZIG_USED_QTY,strData);
	Sleep(100);
	sprintf(strData,"%d",m_iZigUsedQtyEquip);

	iResult = m_plnkRFID->WriteData(ZIG_USED_QTY, strData);
	if (iResult)
		return iResult;

	if (iResult != SUCCESS)
		return iResult;
	m_bZigRFIDState = TRUE; // 통신 상태. 모든 data를 읽어야만 TRUE
	
	return iResult;
}

BOOL MCtrlTabFeeder::GetZigRFIDState()
{
	return m_bZigRFIDState;
}

void MCtrlTabFeeder::InitAssembleCount()
{
	BOOL bSelectTab;
	m_iXAssemble = 0;
	m_iYAssemble = 0;

	if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE)
	{
		if (m_plnkPanelData->m_bUseFlag_Xu)
		{
			for (int i=0 ; i < m_plnkTabData->m_iXuTabQty ; i++)
			{
				bSelectTab = /*(m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
							? m_plnkTabData->m_bUseRepairXuTab[i]
							:*/ m_plnkTabData->m_bUseXuTab[i];
				if (bSelectTab)
					m_iXAssemble++;
			}
		}
		if (m_plnkPanelData->m_bUseFlag_Xd)
		{
			for (int i=0 ; i < m_plnkTabData->m_iXdTabQty ; i++)
			{
				if (m_plnkTabData->m_bUseXdTab[i])
					m_iXAssemble++;
			}
		}
	}
	if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE)
	{
		if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			for (int i=0 ; i< m_plnkTabData->m_iYlTabQty ; i++)
			{
				if (m_plnkTabData->m_bUseYlTab[i])
					m_iYAssemble++;
			}
		}
		if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			for (int i=0 ; i < m_plnkTabData->m_iYrTabQty ; i++)
			{
				if (m_plnkTabData->m_bUseYrTab[i])
					m_iYAssemble++;
			}
		}
	}
}

void MCtrlTabFeeder::SetSelectedReel(int iReelSelect)
{
	m_iSelectedReel = iReelSelect;
}

int MCtrlTabFeeder::GetSelectedReel()
{
	return m_iSelectedReel;
}

void MCtrlTabFeeder::SetConsumStart(int iReelSelect, BOOL bConsumeStart) 
{ 
	m_bConsumeStart[iReelSelect] = bConsumeStart; 
}

void MCtrlTabFeeder::SetZigConsumStart(BOOL bConsumeStart) 
{ 
	m_bZigConsumeStart = bConsumeStart; 
}

void MCtrlTabFeeder::SetZigWarningState(BOOL bWarningState) 
{ 
	m_bZigWarningState = bWarningState; 
}

void MCtrlTabFeeder::ChangeSpareReelInfotoUsingReel()
{
	// Spare Reel 에 대한 자재 정보가 없다면 Data Transfer 작업은 Skip.
	if (TRUE == m_bConsumeStart[DEF_SPARE_REEL])
		return;

	// 대기 Reel 있을때 자동교체 후
	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatSupplementReq, &m_matTabICInfo[DEF_USING_REEL]);
//	m_plnkLCNet->MaterialReport(eMatDockOut, &m_matTabICInfo[DEF_USING_REEL]);			

	m_matTabICInfo[DEF_USING_REEL].CopyData(&m_matTabICInfo[DEF_SPARE_REEL]);
//	if ("FEEDER3" == m_matTabICInfo[DEF_SPARE_REEL].m_sPORTID)
//	{
//		m_matTabICInfo[DEF_USING_REEL].m_sPORTID = "FEEDER1";
//	}
	if (m_plnkSystemData->m_strModulePortID[2] == m_matTabICInfo[DEF_SPARE_REEL].m_sPORTID)
	{
		m_matTabICInfo[DEF_USING_REEL].m_sPORTID = m_plnkSystemData->m_strModulePortID[0];
	}
	else //if ("FEEDER4" == m_matTabICInfo[DEF_SPARE_REEL].m_sPORTID)
	{
//		m_matTabICInfo[DEF_USING_REEL].m_sPORTID = "FEEDER2";
		m_matTabICInfo[DEF_USING_REEL].m_sPORTID = m_plnkSystemData->m_strModulePortID[1];
	}
	m_bConsumeStart[DEF_USING_REEL] = m_bConsumeStart[DEF_SPARE_REEL];

	m_plnkLCNet->MaterialReport(eMatAutoChange, &m_matTabICInfo[DEF_USING_REEL]);
	m_matTabICInfo[DEF_SPARE_REEL].ResetData();
	m_bConsumeStart[DEF_SPARE_REEL]	= TRUE;
}

void MCtrlTabFeeder::SetTitleWindowHandle(HWND hWnd)
{
	m_hTitleViewWnd = hWnd;
}

int MCtrlTabFeeder::ChangeModel(void)
{
	/*
	// selsk_jdy Yl, Yr 동시 작업에 Tab Pitch 개수가 틀릴 경우에 대해서 코딩 필요.
	if (DEF_TABFEEDER1 == m_iInstanceNo)
	{
		m_iTabPitchCount= m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
		m_dOnePitchDist	= 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
	}
	else // (DEF_TABFEEDER2 == m_iInstanceNo)
	{
		m_iTabPitchCount= m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
		m_dOnePitchDist	= 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
	}
	*/
#ifdef DEF_SOURCE_SYSTEM
	m_iTabPitchCount= m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
	m_dOnePitchDist	= 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	m_iTabPitchCount= m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
	m_dOnePitchDist	= 360.0 / m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif
	m_dOneTabDist	= m_iTabPitchCount * m_dOnePitchDist;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::getBadTabCount()
{
#ifdef DEF_SOURCE_SYSTEM
	return (int)m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab;
#else
	return (int)m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab;
#endif
}

int MCtrlTabFeeder::CheckAndRequestRailUnfold(int iRetryLimit)
{
	if (WORKING_SIDE_UNKNOWN == m_plnkProcessData->GetCurrentWorkingSide(m_iInstanceNo))
		return ERR_CTRL_TABFEEDER_SUCCESS;

	if (iRetryLimit < 0)
		iRetryLimit = 0;
	else if (iRetryLimit > 10)
		iRetryLimit = 10;

	ASSERT(NULL != m_hTitleViewWnd);
	int iOpCount = 0;
	while (FALSE == m_plnkTabFeeder->IsRailUnfoldDetected())
	{
		if (iOpCount > iRetryLimit)
			// 315014 = 회수 장치 펴짐이 감지되지 않습니다.
			return generateErrorCode(315014);

		::SendMessage(m_hTitleViewWnd, WM_TABFEEDER_RAIL_UNFOLD_REQ, (WPARAM)m_iInstanceNo, NULL);
		Sleep(10);
		iOpCount++;
	}

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

void MCtrlTabFeeder::threadRun()
{
	if (m_bThreadLife)
	{
		threadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);

	m_pThreadSupplyReel = AfxBeginThread(threadFunctionSupplyFeeder, this);
}

void MCtrlTabFeeder::threadStop()
{
	m_bThreadLife = FALSE;
	DWORD exitcode = 0;

	if (NULL != m_pThread)
	{
		exitcode = ::WaitForSingleObject(m_pThread->m_hThread, 10000);	
		if (exitcode == WAIT_TIMEOUT)
			::TerminateThread(m_pThread->m_hThread, exitcode);
	}

	if (NULL != m_pThreadSupplyReel)
	{
		exitcode = ::WaitForSingleObject(m_pThreadSupplyReel->m_hThread, 10000);	
		if (exitcode == WAIT_TIMEOUT)
			::TerminateThread(m_pThreadSupplyReel->m_hThread, exitcode);
	}

	m_pThread = NULL;
	m_pThreadSupplyReel = NULL;
}

UINT MCtrlTabFeeder::threadFunction(LPVOID pParam)
{
	MCtrlTabFeeder*	pThis = (MCtrlTabFeeder*) pParam;

	TRACE(_T("MCtrlTabFeeder Thread Run\n"));

	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MCtrlTabFeeder Thread Stop\n"));

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

void MCtrlTabFeeder::threadJob()
{
#ifdef SIMULATION
	return;
#endif

	if (TRUE == m_bManualOperation)
		return;

	/*/
	//110630.회수부Reel감지확인_____
	if (FALSE == m_plnkTabFeeder->IsRailUnfoldDetected())
	{
		m_plnkTabFeeder->PReelMotorOff();
		m_bStopTensionCompleted = TRUE;
	}
	//___________
	/*/

	if(MOlbSystemPre.GetTrsAutoManagerComponent() != NULL)
	{
		if(MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen() == TRUE)
		{
			m_plnkTabFeeder->PReelMotorStop();
		}
	}

	//111014 SJ_KJS AutoRun 일때 down 감지되면 recovery 동작..
	//임시..
	if (FALSE == m_bRunTension)
	{
		if (TRUE == m_plnkTabFeeder->IsDetectProtectRecoveryArmDown())
		{
			//m_bRunTension 동작이 아닐경우 한번 정지하면 Down센서 감지되어도 Tension동작 시키지 않는다
			if (TRUE == m_bStopTensionCompleted)
				return;
//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorOn())
			if (FALSE == m_plnkTabFeeder->IsPReelMotorWind())
			{
//@				m_plnkTabFeeder->PReelMotorOn();
				m_plnkTabFeeder->PReelMotorWind();
				m_tmrRecoveryTension.StartTimer();
				Sleep(500);
			}
		}
		else if (TRUE == m_plnkTabFeeder->IsDetectProtectRecoveryArmUp())
		{
//@			if (TRUE == m_plnkTabFeeder->IsPReelMotorOn())
//@			if (TRUE == m_plnkTabFeeder->IsPReelMotorWind())
			{
				m_tmrRecoveryTension.StopTimer();
//@				m_plnkTabFeeder->PReelMotorOff();
				m_plnkTabFeeder->PReelMotorStop();
				Sleep(500);
			}
			m_bStopTensionCompleted = TRUE;
		}
		else
		{
			if (m_tmrRecoveryTension.IsTimerStarted() && m_tmrRecoveryTension.MoreThan(10.0))
			{
				m_tmrRecoveryTension.StopTimer();
//@				m_plnkTabFeeder->PReelMotorOff();
				m_plnkTabFeeder->PReelMotorStop();
				Sleep(500);
			}
		}
	}
	else
	{
		if (TRUE == m_bRunTensionReverse)
		{
			if (TRUE == m_plnkTabFeeder->IsDetectProtectRecoveryArmUp())
			{
	//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorOn())
				if (FALSE == m_plnkTabFeeder->ISPReelMotorRelease())
				{
	//@				m_plnkTabFeeder->PReelMotorOn();
					m_plnkTabFeeder->PReelMotorRelease();
					m_tmrRecoveryTension.StartTimer();
					Sleep(500);
				}
			}
			else if (TRUE == m_plnkTabFeeder->IsDetectProtectRecoveryArmDown())
			{
	//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorOff())
	//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorStop())
				{
					m_tmrRecoveryTension.StopTimer();
	//@				m_plnkTabFeeder->PReelMotorOff();
					m_plnkTabFeeder->PReelMotorStop();
					Sleep(500);
				}
			}
			else
			{
				if (m_tmrRecoveryTension.IsTimerStarted() && m_tmrRecoveryTension.MoreThan(10.0))
				{
					m_tmrRecoveryTension.StopTimer();
	//@				m_plnkTabFeeder->PReelMotorOff();
					m_plnkTabFeeder->PReelMotorStop();
					Sleep(500);
				}
			}
		}
		else
		{
			if (TRUE == m_plnkTabFeeder->IsDetectProtectRecoveryArmDown())
			{
	//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorOn())
				if (FALSE == m_plnkTabFeeder->IsPReelMotorWind())
				{
	//@				m_plnkTabFeeder->PReelMotorOn();
					m_plnkTabFeeder->PReelMotorWind();
					m_tmrRecoveryTension.StartTimer();
					Sleep(500);
				}
			}
			else if (TRUE == m_plnkTabFeeder->IsDetectProtectRecoveryArmUp())
			{
	//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorOff())
	//@			if (FALSE == m_plnkTabFeeder->IsPReelMotorStop())
				{
					m_tmrRecoveryTension.StopTimer();
	//@				m_plnkTabFeeder->PReelMotorOff();
					m_plnkTabFeeder->PReelMotorStop();
					Sleep(500);
				}
			}
			else
			{
				if (m_tmrRecoveryTension.IsTimerStarted() && m_tmrRecoveryTension.MoreThan(10.0))
				{
					m_tmrRecoveryTension.StopTimer();
	//@				m_plnkTabFeeder->PReelMotorOff();
					m_plnkTabFeeder->PReelMotorStop();
					Sleep(500);
				}
			}
		}
		m_bStopTensionCompleted = FALSE;
	}
	
	

	/*
	// 공급부가 역회전하는 경우.
	if (TRUE == m_plnkTabFeeder->IsSprocket1ReverseRotation())
	{
//		if (FALSE == m_plnkTabFeeder->IsDownProtectDetect())
//		{
//			if (FALSE == m_plnkTabFeeder->IsPReelReverseMotorOn())
//				m_plnkTabFeeder->PReelReverseMotorOn();
//
//			if (FALSE == m_plnkTabFeeder->IsPReelMotorOn())
//				m_plnkTabFeeder->PReelMotorOn();
//
//			return;
//		}

		if (FALSE == m_plnkTabFeeder->IsPReelMotorOff())
		{
			m_plnkTabFeeder->PReelMotorOff();
			return;
		}
		//_______________
	}
	else	// 공급부가 정회전하는 경우.
	{
//		if (TRUE == m_plnkTabFeeder->IsDownProtectDetect())
//		{
//			if (TRUE == m_plnkTabFeeder->IsPReelReverseMotorOn())
//				m_plnkTabFeeder->PReelReverseMotorOff();
//			
//			if (FALSE == m_plnkTabFeeder->IsPReelMotorOn())
//				m_plnkTabFeeder->PReelMotorOn();
//			
//			return;
//		}

		if (FALSE == m_plnkTabFeeder->IsPReelMotorOn())
		{
			m_plnkTabFeeder->PReelMotorOn();
			return;
		}
		//_______________
	}

	if (TRUE == m_plnkTabFeeder->IsPReelMotorOn())
		m_plnkTabFeeder->PReelMotorOff();
	
	if (TRUE == m_plnkTabFeeder->IsPReelReverseMotorOn())
		m_plnkTabFeeder->PReelReverseMotorOff();
	*/
}

//Mech Ver2_____
UINT MCtrlTabFeeder::threadFunctionSupplyFeeder(LPVOID pParam)
{
	MCtrlTabFeeder* pThis = (MCtrlTabFeeder*)pParam;

	while (pThis->m_bThreadLife)
	{
		pThis->threadJobSupplyFeeder();
		Sleep(10);
	}

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

void MCtrlTabFeeder::threadJobSupplyFeeder()
{
	if (m_eFeederReelMoveType == E_FEEDER_REEL_MOVE_NONE)
		return;

	int iResult = 0;
	m_iFeederReelMoveErr = ERR_CTRL_TABFEEDER_SUCCESS;
	
	if (m_bManualFind == TRUE)
		return;

	switch (m_eFeederReelMoveType)
	{
	case E_FEEDER_REEL_MOVE_NONE:		
		break;
	case E_FEEDER_FIND_DOWN:
		m_iFeederReelMoveErr = find_WeightDownSensor();
		break;
	case E_FEEDER_ESCAPE_DOWN:
		m_iFeederReelMoveErr = escape_WeightDownSensor();
		break;
	case E_FEEDER_FIND_UP:
		m_iFeederReelMoveErr = find_WeightUpSensor();
		break;
	case E_FEEDER_ESCAPE_UP:
		m_iFeederReelMoveErr = escape_WeightUpSensor();
		break;
	}

	if (m_iFeederReelMoveErr != ERR_CTRL_TABFEEDER_SUCCESS)
		m_eFeederReelMoveType = E_FEEDER_REEL_MOVE_NONE;
}
//______________


void MCtrlTabFeeder::SetPReelTensionManualOperation(BOOL bState)
{
	m_bManualOperation = bState;
}

void MCtrlTabFeeder::SetRunTension(BOOL bset)
{
	m_bRunTension = bset;
}

void MCtrlTabFeeder::SetRunTensionReverse(BOOL bset)
{
	m_bRunTensionReverse = bset;
}

int MCtrlTabFeeder::ExecuteSprocket1Homing()
{
	int iResult = ERR_CTRL_TABFEEDER_SUCCESS;

	m_plnkTabFeeder->RecoveryIndMotorOn();

	iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
	if (iResult)
		return iResult;


	iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_UP_POS);
	if (iResult)
		return iResult;

	m_bRunTension = TRUE;

	iResult = m_plnkTabFeeder->HomingSprocket(DEF_SPROCKET1_UNIT);
	if (iResult)
	{
		m_bRunTension = FALSE;
		return iResult;
	}
	
	double dAutoHomingOffset;
	if (DEF_TABFEEDER1 == m_iInstanceNo)			// Front
		dAutoHomingOffset = m_plnkPreBonderData->m_dSprocket1_F_AutoHoming_Offset;
	else	// if (DEF_TABFEEDER2 == m_iInstanceNo)	// Rear
		dAutoHomingOffset = m_plnkPreBonderData->m_dSprocket1_R_AutoHoming_Offset;

	iResult = m_plnkTabFeeder->RMoveSprocket1Pos(dAutoHomingOffset);

	//m_bRunTension = FALSE;
	
	if (iResult)
		return iResult;
	
	SetHomeFlagSprocket1(TRUE);
	m_bRunTension = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

//110323.Add_____
BOOL MCtrlTabFeeder::GetFeederReady()
{
	BOOL bReady = FALSE;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		bReady = m_bFeederReady4DryRun;
	}
	else
	{
		bReady = GetHomeFlagSprocket2() && GetFindTabICFlag();
	}
	return bReady;
}

void MCtrlTabFeeder::SetFeederReady4DryRun(BOOL bReady)
{
	m_bFeederReady4DryRun = bReady;
}

//_______________

//Mech Ver2______
void MCtrlTabFeeder::SetFeederReelMoveType(EFeederReelMoveType eType)
{

	if (m_eFeederReelMoveType != eType && eType == E_FEEDER_REEL_MOVE_NONE)
	{
		m_plnkTabFeeder->VStop_FeederReel();
	}
	m_eFeederReelMoveType = eType;
	m_iFeederReelMoveErr = 0;
}

EFeederReelMoveType	MCtrlTabFeeder::GetFeederReelMoveType()
{
	return m_eFeederReelMoveType;
}

int MCtrlTabFeeder::GetFeederReelMoveErr()
{
	return m_iFeederReelMoveErr;
}

int	MCtrlTabFeeder::FindWeightUpSensor()
{
	if (m_plnkTabFeeder->IsTensionUpperDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;
	
	m_bManualFind = TRUE;
	int iResult = find_WeightUpSensor();
	m_bManualFind = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::FindWeightDownSensor()
{
	if (m_plnkTabFeeder->IsTensionLowerDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	m_bManualFind = TRUE;
	int iResult = find_WeightDownSensor();
	m_bManualFind = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::EscapeWeightUpSensor()
{
	if (FALSE == m_plnkTabFeeder->IsTensionUpperDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	m_bManualFind = TRUE;
	int iResult = escape_WeightUpSensor();
	m_bManualFind = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::EscapeWeightDownSensor()
{
	if (FALSE == m_plnkTabFeeder->IsTensionLowerDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	m_bManualFind = TRUE;
	int iResult = escape_WeightDownSensor();
	m_bManualFind = FALSE;

	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int	MCtrlTabFeeder::find_WeightUpSensor()
{
	if (TRUE == m_plnkTabFeeder->IsTensionUpperDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	EnterCriticalSection(&m_csFindSensor);

	//120410.kms___________
	int iResult = m_plnkTabFeeder->CheckFeederReelOrigin();
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}
	//_____________________

	iResult = m_plnkTabFeeder->VMove_FeederReel(-1); //Reel을 감는다(-방향)
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}
	DWORD sTime = GetTickCount();
	while(1)
	{
		Sleep(5);
		if (TRUE == m_plnkTabFeeder->IsTensionUpperDetected() || FALSE == m_bThreadLife)	//Up센서 감지되면 Reel을 정지하고 빠져나간다
		{
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		if (GetTickCount() - sTime > 10*1000)
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315023);
		}
		if (m_plnkTabFeeder->IsServoOffFeederReel())
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315027);
		}
		//외부에서 동작을 강제로 정지시킴_____
		if (m_bManualFind != TRUE && m_eFeederReelMoveType == E_FEEDER_REEL_MOVE_NONE)
		{
			//SJ_YYK 110727 Add....
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		//____________________________________
	}
	LeaveCriticalSection(&m_csFindSensor);
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::find_WeightDownSensor()
{
	if (TRUE == m_plnkTabFeeder->IsTensionLowerDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	EnterCriticalSection(&m_csFindSensor);

	//120410.kms___________
	int iResult = m_plnkTabFeeder->CheckFeederReelOrigin();
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}
	//_____________________

	iResult = m_plnkTabFeeder->VMove_FeederReel(1); //Reel을 푼다(+방향)
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}

	DWORD sTime = GetTickCount();
	while(1)
	{
		Sleep(5);
		if (TRUE == m_plnkTabFeeder->IsTensionLowerDetected() || FALSE == m_bThreadLife)	//Down센서를 벗어나면 Reel을 정지하고 빠져나간다
		{
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		if (GetTickCount() - sTime > 10*1000)
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315026);
		}
		if (m_plnkTabFeeder->IsServoOffFeederReel())
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315027);
		}
		//외부에서 동작을 강제로 정지시킴_____
		if (m_bManualFind != TRUE && m_eFeederReelMoveType == E_FEEDER_REEL_MOVE_NONE)
		{
			//SJ_YYK 110727 Add....
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		//____________________________________
	}
	LeaveCriticalSection(&m_csFindSensor);
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::escape_WeightUpSensor()
{
	if (FALSE == m_plnkTabFeeder->IsTensionUpperDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	EnterCriticalSection(&m_csFindSensor);

	//120410.kms___________
	int iResult = m_plnkTabFeeder->CheckFeederReelOrigin();
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}
	//_____________________

	iResult = m_plnkTabFeeder->VMove_FeederReel(1);	//Reel을 푼다(+방향)
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}

	DWORD sTime = GetTickCount();
	while(1)
	{
		Sleep(5);
		if (FALSE == m_plnkTabFeeder->IsTensionUpperDetected() || FALSE == m_bThreadLife)	//Up센서를 벗어나면 Reel을 정지하고 빠져나간다
		{			
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		if (GetTickCount() - sTime > 10*1000)
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315025);
		}
		if (m_plnkTabFeeder->IsServoOffFeederReel())
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315027);
		}
		//외부에서 동작을 강제로 정지시킴_____
		if (m_bManualFind != TRUE && m_eFeederReelMoveType == E_FEEDER_REEL_MOVE_NONE)
		{
			//SJ_YYK 110727 Add....
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		//____________________________________
	}
	LeaveCriticalSection(&m_csFindSensor);
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

int MCtrlTabFeeder::escape_WeightDownSensor()
{
	if (FALSE == m_plnkTabFeeder->IsTensionLowerDetected())
		return ERR_CTRL_TABFEEDER_SUCCESS;

	EnterCriticalSection(&m_csFindSensor);

	//120410.kms___________
	int iResult = m_plnkTabFeeder->CheckFeederReelOrigin();
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}
	//_____________________

	iResult = m_plnkTabFeeder->VMove_FeederReel(-1); //Reel을 감는다(-방향)
	if (iResult)
	{
		LeaveCriticalSection(&m_csFindSensor);
		return iResult;
	}

	DWORD sTime = GetTickCount();
	while(1)
	{
		Sleep(5);
		if (FALSE == m_plnkTabFeeder->IsTensionLowerDetected() || FALSE == m_bThreadLife)	//Down센서를 벗어나면 Reel을 정지하고 빠져나간다
		{
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		if (GetTickCount() - sTime > 10*1000)
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315026);
		}
		if (m_plnkTabFeeder->IsServoOffFeederReel())
		{
			m_plnkTabFeeder->VStop_FeederReel();
			LeaveCriticalSection(&m_csFindSensor);
			return generateErrorCode(315027);
		}
		//외부에서 동작을 강제로 정지시킴_____
		if (m_bManualFind != TRUE && m_eFeederReelMoveType == E_FEEDER_REEL_MOVE_NONE)
		{
			//SJ_YYK 110727 Add....
			m_plnkTabFeeder->VStop_FeederReel();
			break;
		}
		//____________________________________
	}
	LeaveCriticalSection(&m_csFindSensor);
	return ERR_CTRL_TABFEEDER_SUCCESS;
}

//_______________

//130218.kms_________________
BOOL MCtrlTabFeeder::GetBadTabCntInterlock()
{
	if (NORMAL_RUN_MODE != m_plnkSystemData->m_eRunMode)
		return FALSE; 

	if(m_plnkSystemData->m_dStopperArrivingDelay == 0.5)
		return FALSE;


#ifdef DEF_SOURCE_SYSTEM
	if (DEF_TABFEEDER1 == m_plnkTabFeeder->GetInstanceNo() && m_plnkPanelData->m_bUseFlag_Xu == FALSE)
		return FALSE; 
	if (DEF_TABFEEDER2 == m_plnkTabFeeder->GetInstanceNo() && m_plnkPanelData->m_bUseFlag_Xd == FALSE)
		return FALSE; 
#else
	if (DEF_TABFEEDER1 == m_plnkTabFeeder->GetInstanceNo() && m_plnkPanelData->m_bUseFlag_Yr == FALSE)
		return FALSE; 
	if (DEF_TABFEEDER2 == m_plnkTabFeeder->GetInstanceNo() && m_plnkPanelData->m_bUseFlag_Yl == FALSE)
		return FALSE; 
#endif

	//원점 복귀후 BadTab의 0 위치 에서 Press의 Center 까지의 거리
	double m_dBadOriginPCenterDist = 0.0;
	if (DEF_TABFEEDER1 == m_plnkTabFeeder->GetInstanceNo())
		m_dBadOriginPCenterDist = m_plnkSystemData->m_dDistBadTabOriginDist[0];
	else
		m_dBadOriginPCenterDist = m_plnkSystemData->m_dDistBadTabOriginDist[1];


	//현재 Bad Tab 의 위치 
	double m_dBadTabCurPos = 0.0;
	m_dBadTabCurPos = m_plnkTabFeeder->GetBadTabCurrentPos();
	//Bad Tab 의 목표 위치 
	double m_dBadTabModelTgtPos = 0.0;
	m_dBadTabModelTgtPos = m_plnkTabFeeder->GetBadTabTargetPos(DEF_BADTABDETECTOR_WORK_POS);
	//Tab의 Hole 간 거리
	double m_dTabHoleDist = 0.0;
	m_dTabHoleDist = 4.75;

#ifdef DEF_SOURCE_SYSTEM
	//현재 Tab Data의 Bad Tab Count
	double m_dCurTabCnt = 0.0;
	m_dCurTabCnt = m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab;
	//Tab Pitch Count
	double m_dTabPitchCnt = 0.0;
	m_dTabPitchCnt = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
#else
	//현재 Tab Data의 Bad Tab Count
	double m_dCurTabCnt = 0.0;
	m_dCurTabCnt = m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab;
	//Tab Pitch Count
	double m_dTabPitchCnt = 0.0;
	m_dTabPitchCnt = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
#endif

	//Calculate....
	double m_dCalBadCnt = 0.0;
	m_dCalBadCnt = (m_dBadOriginPCenterDist + (- m_dBadTabModelTgtPos)) / (m_dTabHoleDist * m_dTabPitchCnt);


	if( (m_dCalBadCnt >= m_dCurTabCnt + 0.5) || (m_dCalBadCnt < m_dCurTabCnt - 0.5))
		return TRUE;


	return FALSE; 
}
//___________________________