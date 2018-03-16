/* 
 * PanelTransfer Component
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
 * MPanelTransfer.cpp : Implementation of PanelTransfer component.
 */

#include "stdafx.h"
#include <math.h>
#include "MSystemData.h"
#include "MPanelData.h"
#include "MTeachingPosData.h"
#include "IAxis.h"
#include "ISercosMultiAxis.h"
#include "IRSNMCMultiAxis.h"		//161024 Add...
#include "IIO.h"
#include "ICylinder.h"
#include "IVacuum.h"
#include "MPanelTransfer.h"
#include "DefPanelAligner.h"
#include "IStatePanelAligner.h"
#include "MTrsAutoManager.h"
#include "MPlatformOlbSystem.h"

#include "common.h"

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
#	include "DefIOAddrPreBonder.h"
#	include "IStateTabMounter.h"
#	include "DefTabMounter.h"
#	include "DefCameraCarrier.h"
extern MPlatformOlbSystem	MOlbSystemPre;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
#	include "DefIOAddrAcf.h"
#	include "IStateCarrier.h"
#	include "IStateCarrier.h"
extern MPlatformOlbSystem	MOlbSystemACF;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
#	include "DefIOAddrFinal.h"
#	include "IStateMainTool.h"
#	include "DefMainTool.h"
extern MPlatformOlbSystem	MOlbSystemFinal;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int MPanelTransfer::checkInterlockForPanelX(int iPosID, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_TRANSFER_SUCCESS;
	//@공통부____________________________________

// 	//20171003 KDW Start
// 	if(MOlbSystemPre.IsGearOut(DEF_AXIS_SERCOS_TRANSFER_IN))
// 	{
// 		SetErrorLevel(_T("PANEL TRANSFER check Interlock for Panel X"),2, __FILE__, __LINE__);
// 		// 227062 = Transfer In Unit 이 기어드 상태가 아닙니다. 강제 원점 복귀 후 다시 시도해 주세요.
// 		return generateErrorCode(227062);
// 	}
// #ifdef DEF_1ST_EQ 
// 	if(MOlbSystemPre.IsGearOut(DEF_AXIS_SERCOS_TRANSFER_CENTER_1))
// 	{
// 		SetErrorLevel(_T("PANEL TRANSFER check Interlock for Panel X"),2, __FILE__, __LINE__);
// 		// 227062 = Transfer Center Unit 이 기어드 상태가 아닙니다. 강제 원점 복귀 후 다시 시도해 주세요.
// 		return generateErrorCode(227063);
// 	}
// 
// 	if(MOlbSystemPre.IsGearOut(DEF_AXIS_SERCOS_TRANSFER_OUT))
// 	{
// 		SetErrorLevel(_T("PANEL TRANSFER check Interlock for Panel X"),2, __FILE__, __LINE__);
// 		// 227062 = Transfer Out Unit 이 기어드 상태가 아닙니다. 강제 원점 복귀 후 다시 시도해 주세요.
// 		return generateErrorCode(227064);
// 	}
// #endif
// 	//20171003 KDW End

	if (IsPanelAbsorbed() == FALSE)
	{
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE || iPosID == DEF_PANEL_TRANSFER_LOAD_POS)
		{
			if (FALSE == IsDownPickUpUDCyl()) 
				// 227013 = PickUp Unit가 Down 상태가 아닙니다.
				return generateErrorCode(227013);
		}
		if (iPosID < 0 || iPosID >= DEF_PANEL_TRANSFER_MAX_POSITION)
			// 227007 = X축 위치 Index 가 이상합니다.
			return generateErrorCode(227007);
		//@__________________________________________
//		return ERR_PANEL_TRANSFER_SUCCESS;
	}


	if (m_uiTransferType == DEF_PANEL_TRANSFER_IN)
	{
		iResult = checkInterlockForTransferIn(iPosID, bUseAuto);
		if (iResult)
			return iResult;
	}
	else if (m_uiTransferType == DEF_PANEL_TRANSFER_OUT)
	{
		iResult = checkInterlockForTransferOut(iPosID, bUseAuto);
		if (iResult)
			return iResult;
	}
	//171004 JSH.s
#ifdef DEF_1ST_EQ	
	else if (m_uiTransferType == DEF_PANEL_TRANSFER_CENTER)
	{
		if(MOlbSystemPre.IsGearOut(DEF_AXIS_SERCOS_TRANSFER_CENTER_1))
		{
			iResult = MOlbSystemPre.SetGearIn(DEF_AXIS_SERCOS_TRANSFER_CENTER_1);
			if(iResult)
			{
				//171004 JSH.s
				ResetOrigin();
				//171004 JSH.e
				// 227062 = Transfer Center Unit 이 기어드 상태가 아닙니다. 강제 원점 복귀 후 다시 시도해 주세요.
				return generateErrorCode(227063);
			}
		}
	}
#endif
	//171004 JSH.e

	/*/
//110102 HJG
#ifdef DEF_USE_PANEL_PUSHER
	if(m_plnkSystemData->m_bUsePanelPusher == TRUE)
	{
		if (TRUE == IsPanelAbsorbed())
		{
			if(m_uiTransferType == DEF_PANEL_TRANSFER_IN)
			{
				if(iPosID == DEF_PANEL_TRANSFER_LOAD_POS || iPosID == DEF_PANEL_TRANSFER_UNLOAD_POS)
				{
					if(m_plnkStatePanelAligner->IsUpPanelPusherUDCyl() == FALSE)
					{
						SetErrorLevel(_T("PANEL TRANSFER check Interlock for Panel X"),2, __FILE__, __LINE__);
						// 227039 = Panel Pusher가 Up 상태가 아니어서 이동할 수 없습니다.
						return generateErrorCode(227039);
					}
				}
			}

			if(m_uiTransferType == DEF_PANEL_TRANSFER_OUT)
			{
				if(iPosID == DEF_PANEL_TRANSFER_UNLOAD_POS)
				{
					if(m_plnkStatePanelAligner->IsUpPanelPusherUDCyl() == FALSE)
					{
						SetErrorLevel(_T("PANEL TRANSFER check Interlock for Panel X"),2, __FILE__, __LINE__);
						// 227039 = Panel Pusher가 Up 상태가 아니어서 이동할 수 없습니다.
						return generateErrorCode(227039);
					}
				}
			}
		}
	}
#endif
	/*/

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::checkInterlockForTransferIn(int iPosID, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_TRANSFER_SUCCESS;

#ifdef DEF_SOURCE_SYSTEM
	double dTolerance = 0.0;
	BOOL bChkPanelAlignerUnit = FALSE;
	BOOL bChkAirPlate = FALSE;
	int i = 0;

	//20171003 KDW Start
	if(MOlbSystemPre.IsGearOut(DEF_AXIS_SERCOS_TRANSFER_IN))
	{
		iResult = MOlbSystemPre.SetGearIn(DEF_AXIS_SERCOS_TRANSFER_IN);
		if(iResult)
		{
			//171004 JSH.s
			ResetOrigin();
			//171004 JSH.e
			// 227062 = Transfer In Unit 이 기어드 상태가 아닙니다. 강제 원점 복귀 후 다시 시도해 주세요.
			return generateErrorCode(227062);
		}
	}
	//20171003 KDW End

	if (TRUE == IsPanelReleased())
		return ERR_PANEL_TRANSFER_SUCCESS;

	switch (iPosID)
	{
	case DEF_PANEL_TRANSFER_LOAD_POS:
		if (TRUE == IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
			break;
#ifndef DEF_SETUP	//@확인요
		//@앞설비 상태 확인
		if (FALSE == MOlbSystemPre.GetTrsAutoManagerComponent()->IsEnableToMovePrevMCWithPanel())
		{
			return generateErrorCode(227050);
		}
#endif
		bChkPanelAlignerUnit = TRUE;
		bChkAirPlate = TRUE;
		break;

	case DEF_PANEL_TRANSFER_UNLOAD_POS:
		if (TRUE == IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
			break;
			
		//@Panel Aligner및 부상 상태 확인
		bChkPanelAlignerUnit = TRUE;
		bChkAirPlate = TRUE;

		break;
	}
	

	if (TRUE == bChkAirPlate)
	{
	}
#endif

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::checkInterlockForTransferOut(int iPosID, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_TRANSFER_SUCCESS;

	double dTolerance = 0.0;
	BOOL bChkPanelAlignerUnit = FALSE;
	BOOL bChkAirPlate = FALSE;
	int i = 0;
	//20171003 KDW Start
#ifdef DEF_1ST_EQ
	if(MOlbSystemPre.IsGearOut(DEF_AXIS_SERCOS_TRANSFER_OUT))
	{
		iResult = MOlbSystemPre.SetGearIn(DEF_AXIS_SERCOS_TRANSFER_OUT);	
		if(iResult)
		{
			//171004 JSH.s
			ResetOrigin();
			//171004 JSH.e
			// 227062 = Transfer Out Unit 이 기어드 상태가 아닙니다. 강제 원점 복귀 후 다시 시도해 주세요.
			return generateErrorCode(227064);
		}
	}
#endif
	//20171003 KDW End

	if (TRUE == IsPanelReleased())
		return ERR_PANEL_TRANSFER_SUCCESS;

	switch (iPosID)
	{
	case DEF_PANEL_TRANSFER_LOAD_POS:
		if (TRUE == IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
			break;

#ifdef DEF_GATE_SYSTEM
		//@Panel Aligner 상태 확인
		dTolerance = 10.0;
		if (FALSE == m_plnkStatePanelAligner->IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dTolerance))
			bChkPanelAlignerUnit = TRUE;
		//@부상 상태 확인
		bChkAirPlate = TRUE;		
#else
		//@Panel Aligner및 부상 상태 확인
		bChkPanelAlignerUnit = TRUE;
		bChkAirPlate = TRUE;		
#endif
		if (TRUE == bChkPanelAlignerUnit)
		{
			if (TRUE == m_plnkStatePanelAligner->IsPanelWorkingDetected())
			{
				return generateErrorCode(227051);
			}
		}
		break;

	case DEF_PANEL_TRANSFER_UNLOAD_POS:
		if (TRUE == IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
			break;
			
#ifdef DEF_GATE_SYSTEM
		//@Panel Aligner 상태 확인
		dTolerance = 10.0;
		if (FALSE == m_plnkStatePanelAligner->IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dTolerance))
			bChkPanelAlignerUnit = TRUE;
		//@부상 상태 확인
		bChkAirPlate = TRUE;		
#else
		//@Panel Aligner및 부상 상태 확인
		dTolerance = 10.0;
		if (FALSE == m_plnkStatePanelAligner->IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dTolerance))

		bChkPanelAlignerUnit = TRUE;
		bChkAirPlate = TRUE;		
#endif
		//@후설비 상태 확인
		if (FALSE == MOlbSystemPre.GetTrsAutoManagerComponent()->IsEnableToMoveNextMCWithPanel())
		{			
			return generateErrorCode(227052);
		}

		break;
	}

	if (TRUE == bChkPanelAlignerUnit)
	{
		if (FALSE == m_plnkStatePanelAligner->IsPanelReleased())
			// 210038 = 이동 불가[Panel Aligner 가 Panel 흡착 상태임]
			return generateErrorCode(227038);
		
		if (FALSE == m_plnkStatePanelAligner->IsDownPickUpUnit())
			// 227039 = 이동 불가[Panel Aligner Z축이 Down 상태가 아님]
			return generateErrorCode(227013);
	}

	if (TRUE == bChkAirPlate)
	{
		// 부상유닛 흡착해제상태 아닌경우
		if (FALSE == m_plnkStatePanelAligner->IsAirCVVacAllOff())
			// 227024 = Plate 흡착해제상태가 아니어서 이동할 수 없습니다.
			return generateErrorCode(227024);
		
		// 부상유닛 부상상태 아닌경우
		if (FALSE == m_plnkStatePanelAligner->IsAirCVBlowOn())
		{
			if (TRUE == bUseAuto)
			{
				MTickTimer timer;
				timer.StartTimer();
				
				while (TRUE)
				{
					if (TRUE == m_plnkStatePanelAligner->IsAirCVBlowOn())
						break;
					
					if (TRUE == timer.MoreThan(10.0))
						// 227015 = Plate 부상상태가 아니어서 이동할 수 없습니다.
						return generateErrorCode(227015);
					
					Sleep(15);
				}
			}
			else
				// 227015 = Plate 부상상태가 아니어서 이동할 수 없습니다.
				return generateErrorCode(227015);
		}

		dTolerance = 100.0;
		if (TRUE == m_plnkStateTabMounter[0]->IsInGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS, &dTolerance))
		{
			dTolerance = 1.0;
			for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
			{
				if (FALSE == m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance))
				{
					Sleep(200);
					if (FALSE == m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance))
						// 210107 = 이동 불가. [전면부 TabMounter 하강 상태임.]
						return generateErrorCode(210107);
				}
			}
		}
		
		dTolerance = 100.0;
		if (TRUE == m_plnkStateRTabMounter[0]->IsInGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS, &dTolerance))
		{
			dTolerance = 1.0;
			for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
			{
				if (FALSE == m_plnkStateRTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance))
				{
					Sleep(200);
					if (FALSE == m_plnkStateRTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance))
						// 210108 = 이동 불가. [후면부 TabMounter 하강 상태임.]
						return generateErrorCode(210108);
				}
			}
		}

		dTolerance = 1.0;
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			if (FALSE == m_plnkStateInspectCamCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS, &dTolerance))
				return generateErrorCode(210085);
			if (FALSE == m_plnkStateRInspectCamCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS, &dTolerance))
				return generateErrorCode(210086);
		}
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::checkInterlockForRailMove()
{
	int iResult = ERR_PANEL_TRANSFER_SUCCESS;
	if (FALSE == IsPanelReleased())
	{
		return generateErrorCode(227060);
	}
	if (FALSE == m_plnkStatePanelAligner->IsPanelReleased())
	{
		return generateErrorCode(227061);
	}
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::calculatePanelTransferPanelXModelPosParameter()
{
	// Model 좌표값 초기화
	for (unsigned int i = 0; i < DEF_PANEL_TRANSFER_MAX_POSITION; i++)
		m_rgdXModelPos[i] = 0.0;

	m_rgdXModelPos[DEF_PANEL_TRANSFER_LOAD_POS]		= 0.0;
	m_rgdXModelPos[DEF_PANEL_TRANSFER_UNLOAD_POS]	= 0.0;
	return ERR_PANEL_TRANSFER_SUCCESS;
}

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

MPanelTransfer::MPanelTransfer(SCommonAttribute commonData, SPanelTransferRefCompList listRefComponents, SPanelTransferData datComponent)
: ICommon(commonData)
{
	SetCommonAttribute(commonData);
	
	// Component List Assign
	AssignComponents(listRefComponents);

	SetData(datComponent);

	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	LoadXFixedPosParameter();			// PanelX   고정 좌표 Load
	LoadXOffsetPosParameter();
	calculatePanelTransferPanelXModelPosParameter();
}

MPanelTransfer::~MPanelTransfer()
{

}

int MPanelTransfer::AssignComponents(SPanelTransferRefCompList PanelTransferCompList)
{
	m_plnkIO			= PanelTransferCompList.m_plnkIO;
	m_plnkX1			= PanelTransferCompList.m_plnkX1;
	m_plnkX2			= PanelTransferCompList.m_plnkX2;
	m_plnkMultiX		= PanelTransferCompList.m_plnkMultiX;
	m_plnkVac			= PanelTransferCompList.m_plnkVac;
	m_plnkVac2			= PanelTransferCompList.m_plnkVac2;
	m_plnkUDCyl			= PanelTransferCompList.m_plnkUDCyl;
	m_plnkAirRailFB1	= PanelTransferCompList.m_plnkAirRailFB1;
	m_plnkAirRailFB2	= PanelTransferCompList.m_plnkAirRailFB2;

	return ERR_PANEL_TRANSFER_SUCCESS;
}

IAxis* MPanelTransfer::GetMotionComponent(int nAxisID)
{
	// Motion Component 축 ID에 따라
	switch (nAxisID)
	{
	case DEF_PANEL_TRASFER_X1_AXIS:		// PanelTransfer의 X1축 
		return m_plnkX1;

	case DEF_PANEL_TRASFER_X2_AXIS:		// PanelTransfer의 X2축
		return m_plnkX2;

	default :							// 축 ID 오류
		return NULL;
	}
}

//ISercosMultiAxis* MPanelTransfer::GetMultiMotionComponent()
IRSNMCMultiAxis* MPanelTransfer::GetMultiMotionComponent()
{	
	return m_plnkMultiX;
}

int MPanelTransfer::LoadXFixedPosParameter(void)
{
	MTeachingPosData XFixedPosDat(m_sXTeachingInfo, m_strPosFileName);

	// Fixed Pos 
	BOOL bResult = XFixedPosDat.ReadData();

	for (int i = 0; i < DEF_PANEL_TRANSFER_MAX_POSITION; i++)
	{
		m_rgdXFixedPos[i] = XFixedPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 227001 = X축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(227001);
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::SaveXFixedPosParameter(double* rgdFixedPos)
{
	ASSERT(rgdFixedPos != NULL);

	MTeachingPosData PanelTransferZFixPosDat(m_sXTeachingInfo, m_strPosFileName);

	for (int i = 0; i < DEF_PANEL_TRANSFER_MAX_POSITION; i++)
	{
		m_rgdXFixedPos[i] = rgdFixedPos[i];
		PanelTransferZFixPosDat.m_TeachingPos[i][0] = m_rgdXFixedPos[i];
	}

	BOOL bResult = PanelTransferZFixPosDat.WriteData();
	if (!bResult)
		// 227002 = X축 고정좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(227002);

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::LoadXOffsetPosParameter(void)
{
	MTeachingPosData XOffsetPosDat(m_sXTeachingInfo,
									m_strOffsetPosFileName, 
									m_strOffsetPosFilePath);

	// Offset Pos 
	BOOL bResult = XOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_PANEL_TRANSFER_MAX_POSITION; i++)
	{
		m_rgdXOffsetPos[i] = XOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 227003 = X축 Offset 좌표를 읽어오지 못했습니다.
		return generateErrorCode(227003);

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::SaveXOffsetPosParameter(double* rgdOffsetPos)
{
	ASSERT(rgdOffsetPos != NULL);

	MTeachingPosData XOffsetPosDat(m_sXTeachingInfo, 
									m_strOffsetPosFileName, 
									m_strOffsetPosFilePath);
	// Offset Pos 
	for (int i = 0; i < DEF_PANEL_TRANSFER_MAX_POSITION ; i++)
	{
		m_rgdXOffsetPos[i] = rgdOffsetPos[i];
		XOffsetPosDat.m_TeachingPos[i][0] = m_rgdXOffsetPos[i];	
	}

	BOOL bResult = XOffsetPosDat.WriteData();
	if (!bResult)
		// 227004 = X축 Offset 좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(227004);

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::GetXPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos)
{
	for (int i = 0; i < DEF_PANEL_TRANSFER_MAX_POSITION; i++)
	{
		if (rgdFixedPos != NULL)
			rgdFixedPos[i] = m_rgdXFixedPos[i];		
		if (rgdOffsetPos != NULL)
			rgdOffsetPos[i] = m_rgdXOffsetPos[i];		
		if (rgdModelPos != NULL)
			rgdModelPos[i]	= m_rgdXModelPos[i];
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

STeachingInfo MPanelTransfer::GetXTeachingInfo()
{
	return m_sXTeachingInfo;
}

int MPanelTransfer::CheckVacuum(void)
{
	ASSERT(m_plnkVac != NULL && m_plnkVac2 != NULL);
	int iResult = m_plnkVac->CompareIO();
	if (iResult)
		return iResult;
	iResult = m_plnkVac2->CompareIO();
	if (iResult)
		return iResult;
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::AbsorbPanel(BOOL bSkipSensor)
{
	ASSERT(m_plnkVac != NULL && m_plnkVac2 != NULL);

	/** Vacuum Off되어 있는지 확인 */
	if (!IsPanelAbsorbed())
	{
		int iResult = ERR_PANEL_TRANSFER_SUCCESS;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [Absorb Panel : SkipSensor(%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		m_plnkVac->On(TRUE);
		m_plnkVac2->On(TRUE);

		iResult = m_plnkVac->On(bSkipSensor);
		if (iResult != ERR_PANEL_TRANSFER_SUCCESS)
		{
			m_plnkVac->Off(FALSE);
			m_plnkVac2->Off(FALSE);
			return iResult;
		}
		iResult = m_plnkVac2->On(bSkipSensor);
		if (iResult != ERR_PANEL_TRANSFER_SUCCESS)
		{
			m_plnkVac->Off();
			m_plnkVac2->Off();
			return iResult;
		}
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [Absorb Panel : SkipSensor(%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::ReleasePanel(BOOL bSkipSensor)
{
	ASSERT(m_plnkVac != NULL && m_plnkVac2 != NULL);

	int iResult = ERR_PANEL_TRANSFER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelTransfer] [M] [Release Panel : SkipSensor(%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);

	m_plnkVac->Off(TRUE);
	m_plnkVac2->Off(TRUE);

	iResult = m_plnkVac->Off(bSkipSensor);
	if (iResult != ERR_PANEL_TRANSFER_SUCCESS)
		return iResult;
	iResult = m_plnkVac2->Off(bSkipSensor);
	if (iResult != ERR_PANEL_TRANSFER_SUCCESS)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelTransfer] [M] [Release Panel : SkipSensor(%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_TRANSFER_SUCCESS;
}

BOOL MPanelTransfer::IsPanelAbsorbed(void)
{
	ASSERT(m_plnkVac != NULL && m_plnkVac2 != NULL);
	return (m_plnkVac->IsOn() && m_plnkVac2->IsOn());
}

BOOL MPanelTransfer::IsPanelReleased(void)
{
	ASSERT(m_plnkVac != NULL && m_plnkVac2 != NULL);
	return (m_plnkVac->IsOff() && m_plnkVac2->IsOff());
}

int MPanelTransfer::SafeSyncMoveXPos(int iPosID, BOOL bUseAuto)
{
	double dTarget = GetXTargetPos(iPosID);		// 목표 위치 읽기
	return SyncMoveXPos(dTarget, iPosID, bUseAuto);	// 해당 위치로 이동
}

int MPanelTransfer::SafeMoveXPos(int iPosID, BOOL bUseAuto)
{
	double dTarget = GetXTargetPos(iPosID);		// 목표 위치 읽기
	return MoveXPos(dTarget, iPosID, bUseAuto);	// 해당 위치로 이동
}

int MPanelTransfer::CheckOrigin(void)
{
	// PanelTransfer X Motion Check Origin
	if (m_plnkX1->IsOriginReturn() == FALSE)
		// 227005 = X1축 원점복귀되어 있지 않습니다.
		return generateErrorCode(227005);

	//if (m_uiTransferType != DEF_PANEL_TRANSFER_CENTER) 
#ifndef DEF_1ST_EQ
	if (m_uiTransferType != DEF_PANEL_TRANSFER_OUT) 
#endif
	{
		if (m_plnkX2->IsOriginReturn() == FALSE)
			// 227006 = X2축 원점복귀되어 있지 않습니다.
			return generateErrorCode(227006);
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

//171004 JSH.s
int MPanelTransfer::ResetOrigin(void)
{
	// PanelTransfer X Motion Reset Origin
	int iResult = 0;
	
	iResult = m_plnkX1->ResetOrigin();
	m_plnkX1->ResetOriginComplete();
	if (iResult)
		return iResult;
	
	//if (m_uiTransferType != DEF_PANEL_TRANSFER_CENTER) 
#ifndef DEF_1ST_EQ
	if (m_uiTransferType != DEF_PANEL_TRANSFER_OUT) 
#endif
	{
		iResult = m_plnkX2->ResetOrigin();
		m_plnkX2->ResetOriginComplete();
		if (iResult)
			return iResult;
	}
	
 	return ERR_PANEL_TRANSFER_SUCCESS;
 }
//171004 JSH.e
double MPanelTransfer::GetXCurrentPos(BOOL bType)
{
	ASSERT(m_plnkX1 != NULL);
	double dCurPos = m_plnkX1->GetCurrentPosition(bType);
	return dCurPos;
}

double MPanelTransfer::GetX2CurrentPos(BOOL bType)
{
	if (m_uiTransferType != DEF_PANEL_TRANSFER_OUT) 
	{
		ASSERT(m_plnkX2 != NULL);
		double dCurPos = m_plnkX2->GetCurrentPosition(bType);
		return dCurPos;
	}
	else
		return GetXCurrentPos(bType);
}

double MPanelTransfer::GetXTargetPos(int iPosID)
{
	ASSERT((iPosID >= 0) && (iPosID < DEF_PANEL_TRANSFER_MAX_POSITION));
	// 목표좌표 = 고정 좌표	+ Model 좌표 + Offset 좌표
	double dTarget = m_rgdXFixedPos[iPosID] + m_rgdXModelPos[iPosID] + m_rgdXOffsetPos[iPosID];
	return dTarget;
}

BOOL MPanelTransfer::IsInXPos(int iPosID, double* pdTolerance)
{
	double dPosCompare = GetXTargetPos(iPosID);
	return IsInXPos(dPosCompare, pdTolerance);
}

BOOL MPanelTransfer::IsInXPos(double dTargetPos, double* pdTolerance)
{
	return m_plnkX1->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

int MPanelTransfer::SetData(SPanelTransferData datComponent)
{
	m_sXTeachingInfo			= datComponent.m_sXTeachingInfo;
	m_strPosFileName			= datComponent.m_strPosFileName;
	m_strOffsetPosFileName		= datComponent.m_strOffsetPosFileName;
	m_plnkSystemData			= datComponent.m_plnkSystemData;
	m_plnkPanelData				= datComponent.m_plnkPanelData;
	m_uiTransferType			= datComponent.m_uiTransferType;

	/*/
	m_usOAddrAirBlow1			= datComponent.m_usOAddrAirBlow1;
	m_usOAddrAirBlow2			= datComponent.m_usOAddrAirBlow2;
	m_usIAddrAirBlow1			= datComponent.m_usIAddrAirBlow1;
	m_usIAddrAirBlow2			= datComponent.m_usIAddrAirBlow2;

	m_usOAddrAirVacuum1			= datComponent.m_usOAddrAirVacuum1;
	m_usOAddrAirVacuum2			= datComponent.m_usOAddrAirVacuum2;
	m_usIAddrAirVacuum1			= datComponent.m_usIAddrAirVacuum1;
	m_usIAddrAirVacuum2			= datComponent.m_usIAddrAirVacuum2;
	/*/

	m_usIPanelDetect			= datComponent.m_usIPanelDetect;

//120224.KMS____________
	m_usIPanelDetectLoading     = datComponent.m_usIPanelDetectLoading;
	m_usIPanelDetectUnLoading	= datComponent.m_usIPanelDetectUnLoading;

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::AssignStateCheckComponents(SStateCheckCompListForPanelTransfer sCompList)
{
	m_plnkStatePanelAligner		= sCompList.m_plnkStatePanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkStatePanelTransferOut	= sCompList.m_plnkStatePanelTransferOut;
	m_plnkStatePanelTransferIn	= sCompList.m_plnkStatePanelTransferIn;
#endif
	/** Tab Mounter */
	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		m_plnkStateTabMounter[i]	= sCompList.m_plnkStateTabMounter[i];
		m_plnkStateRTabMounter[i]	= sCompList.m_plnkStateRTabMounter[i];
	}

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_plnkStateInspectCamCarrier[i] = sCompList.m_plnkStateInspectCamCarrier[i];
		m_plnkStateRInspectCamCarrier[i] = sCompList.m_plnkStateRInspectCamCarrier[i];
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::ChangeModel()
{
	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	LoadXFixedPosParameter();				// PanelX   고정 좌표 Load
	LoadXOffsetPosParameter();
	calculatePanelTransferPanelXModelPosParameter();

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::Initialize()
{
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::SyncMoveXPos(double dTarget, int iPosID, BOOL bUseAuto)	// Inspection 8 : WaitOption 추가 ?
{
#ifdef SIMULATION
	// 각 축에 목표 위치 넘기고 각 축 별 이동 시간 받고
	// 가장 큰 이동 시간만큼 
	Sleep(500);
	return ERR_PANEL_TRANSFER_SUCCESS;
#endif

	
#ifndef DEF_USE_TRANSFER_OUT_MULTI_AXIS //170719 JSH
	if (m_uiTransferType == DEF_PANEL_TRANSFER_OUT) 
		return MoveXPos(dTarget, iPosID, bUseAuto);	// 해당 위치로 이동
#endif
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelTransfer] [M] [SyncMove PanelX to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult = 0;

	iResult	= checkInterlockForPanelX(iPosID, bUseAuto);
	if (iResult)
		return iResult;

	double dNormalVel = m_plnkX1->GetMovingVelocity() * 1.414;
	if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
		dNormalVel = m_plnkX1->GetSlowMovingVelocity() * 1.414;

	double rgdNormalVel[2] = { dNormalVel, dNormalVel };
	double rgdTarget[2] = {dTarget, dTarget};

	// 축 이동 시작
	iResult = m_plnkMultiX->Move(rgdTarget, rgdNormalVel);
	if (iResult)
		return iResult;
 
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelTransfer] [M] [SyncMove PanelX to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::MoveXPos(double dTarget, int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	// 각 축에 목표 위치 넘기고 각 축 별 이동 시간 받고
	// 가장 큰 이동 시간만큼 
	Sleep(500);
	return ERR_PANEL_TRANSFER_SUCCESS;
#endif

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelX to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult	= checkInterlockForPanelX(iPosID, bUseAuto);
	if (iResult)
		return iResult;

	double dVel = 0.0;
	if (bUseAuto && iPosID == DEF_PANEL_TRANSFER_LOAD_POS && FALSE == IsPanelAbsorbed())
		dVel = 800.0;

	// 축 이동 시작
	iResult = m_plnkX1->Move(dTarget, dVel);
	if (iResult) 
		return iResult;
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelX to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::UpPickUpUDCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkUDCyl != NULL);
/*/// 2010.07.27 KBC ----------------------------------------
	if (!m_plnkStatePanelAligner->IsAirCVVacAllOff())
	{
		SetErrorLevel(_T("PANEL TRANSFER up Pickup Up/Down Cylinder"),2, __FILE__, __LINE__);
		//부상유닛에서 Panel을 흡착하고 있어서 Pickup 실린더를 상승할 수 없습니다.
		return generateErrorCode(227034);
	}
/*/// 2010.07.27 KBC ----------------------------------------
	// Up/Down Cylinder가 이미 상승된 상태인지 확인
	if (m_plnkUDCyl->IsUp() == FALSE)
	{
		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer UD Cyl Up (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		int iResult = m_plnkUDCyl->Up(bSkipSensor);
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer UD Cyl Up (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::DownPickUpUDCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkUDCyl != NULL);

	// Up/Down Cylinder가 이미 하강된 상태인지 확인
	if (m_plnkUDCyl->IsDown() == FALSE)
	{
		// Panel 흡착하고 있는지 확인
		if (FALSE == IsPanelReleased())
			// 227028 = Panel을 흡착하고 있어서 PickUp Unit을 하강할 수 없습니다.
			return generateErrorCode(227028);

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer UD Cyl Down (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		int iResult = m_plnkUDCyl->Down(bSkipSensor);
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer UD Cyl Down (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_TRANSFER_SUCCESS;
}

BOOL MPanelTransfer::IsUpPickUpUDCyl(void)
{
	ASSERT(m_plnkUDCyl != NULL);
	return m_plnkUDCyl->IsUp();
}

BOOL MPanelTransfer::IsDownPickUpUDCyl(void)
{
	ASSERT(m_plnkUDCyl != NULL);
	return m_plnkUDCyl->IsDown();
}

//@______________________________________________
int MPanelTransfer::ForAirRailFB1(BOOL bSkipSensor)
{
	ASSERT(m_plnkAirRailFB1 != NULL);
	if (FALSE == IsForAirRailFB1())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail1 FB Cyl For (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB1->Front(bSkipSensor);
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail1 FB Cyl For (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::ForAirRailFB2(BOOL bSkipSensor)
{
	ASSERT(m_plnkAirRailFB2 != NULL);
	if (FALSE == IsForAirRailFB2())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail2 FB Cyl For (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB2->Front(bSkipSensor);
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail2 FB Cyl For (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::BackAirRailFB1(BOOL bSkipSensor)
{
	ASSERT(m_plnkAirRailFB1 != NULL);
	if (FALSE == IsBackAirRailFB1())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail1 FB Cyl Back (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB1->Back(bSkipSensor);
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail1 FB Cyl Back(SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelTransfer::BackAirRailFB2(BOOL bSkipSensor)
{
	ASSERT(m_plnkAirRailFB2 != NULL);
	if (FALSE == IsBackAirRailFB2())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail2 FB Cyl Back (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB2->Back(bSkipSensor);
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelTransfer] [M] [PanelTransfer AirRail2 FB Cyl Back(SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_TRANSFER_SUCCESS;
}

BOOL MPanelTransfer::IsForAirRailFB1()
{
	ASSERT(m_plnkAirRailFB1 != NULL);
	return m_plnkAirRailFB1->IsFront();
}

BOOL MPanelTransfer::IsForAirRailFB2()
{
	ASSERT(m_plnkAirRailFB2 != NULL);
	return m_plnkAirRailFB2->IsFront();
}

BOOL MPanelTransfer::IsBackAirRailFB1()
{
	ASSERT(m_plnkAirRailFB1 != NULL);
	return m_plnkAirRailFB1->IsBack();
}

BOOL MPanelTransfer::IsBackAirRailFB2()
{
	ASSERT(m_plnkAirRailFB2 != NULL);
	return m_plnkAirRailFB2->IsBack();
}
//@______________________________________________

BOOL MPanelTransfer::IsPanelDetected()
{
	return m_plnkIO->IsOn(m_usIPanelDetect);
}

BOOL MPanelTransfer::IsPanelLoadingDetected()
{
#ifndef DEF_SETUP	//@수정요
	if (m_uiTransferType == DEF_PANEL_TRANSFER_IN)
	{
		return m_plnkIO->IsOn(m_usIPanelDetectLoading);
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
#else
	return FALSE;
#endif
}

//@모델대응 위치로 센서 찾는 동작
int	MPanelTransfer::FindSensorTransferInAirRail()
{
	int iResult = ERR_PANEL_TRANSFER_SUCCESS;	

	return ERR_PANEL_TRANSFER_SUCCESS;
}


// End Of Code
