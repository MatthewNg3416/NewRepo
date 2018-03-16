/* 
 * ModelChange Component
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
 * MModelChange.cpp : Implementation of ModelChange component.
 */

#include "stdafx.h"
#include <math.h>
#include "MSystemData.h"
#include "MPanelData.h"
#include "MTeachingPosData.h"
#include "IIO.h"
#include "IAxis.h"
#include "ICylinder.h"
#include "MModelChange.h"
#include "IStatePanelAligner.h"
#include "IStatePanelTransfer.h"
#include "IStateCameraCarrier.h"
#include "DefCameraCarrier.h"

#include "common.h"

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
#include "MPreBonderData.h"
#include "DefIOAddrPreBonder.h"
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
#include "MAcfData.h"
#include "DefIOAddrAcf.h"
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
#include "MFinalData.h"
#include "DefIOAddrFinal.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int MModelChange::checkInterlockForPanelY(int iPosID, BOOL bUseAuto)
{
	// 간섭 가능 Unit
	// 1. Panel 흡착
	//  - Plate
	//  - Panel Aligner
	//  - Panel Transfer
	// 2. Expand/Shrink Cyl 위치 - Model 별 Plate 2,3 위치 확인
	// 3. Rail Lock 상태

	// Plate 흡착
//	if (FALSE == m_plnkStatePanelAligner->IsAirCVVacAllOff())
//	if (FALSE == m_plnkStatePanelAligner->IsAirCVVacPlate1Off() || FALSE == m_plnkStatePanelAligner->IsAirCVVacPlate3Off()
//		|| FALSE == m_plnkStatePanelAligner->IsAirCVVacPlate4Off())

//@	if (FALSE == m_plnkStatePanelAligner->IsAirCVVacPlate3Off())
	if (FALSE == m_plnkStatePanelAligner->IsAirCVVacAllOff())
	{
		// 230006 = Plate Vacuum이 On 상태입니다.
		return generateErrorCode(230006);
	}
	
	if (m_iWorkGroup == DEF_FRONT_GROUP) //161231 SJ_YSH Add..
	{
		for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			if (FALSE == m_plnkStateCameraCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
				return generateErrorCode(230013);
		}
	}

	if (FALSE == IsUnlockRail())
	{
		// 230011 = Rail이 Unlock 상태가 아닙니다.
		return generateErrorCode(230011);
	}

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::checkInterlockForRailMove()
{
	// Plate 흡착
	if (FALSE == m_plnkStatePanelAligner->IsAirCVVacAllOff())
	{
		// 230006 = Plate Vacuum이 On 상태입니다.
		return generateErrorCode(230014);
	}
	if (FALSE == m_plnkStatePanelAligner->IsPanelReleased())
	{
		// 230006 = Plate Vacuum이 On 상태입니다.
		return generateErrorCode(230015);
	}

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::calculateModelChangePanelYModelPosParameter()
{
	//@______________________
	/*/
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		m_rgdYFixedPos[i] = 0.0;
	}
	/*/
	//@______________________

	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
		m_rgdYModelPos[i] = 0.0;

//	m_rgdYModelPos[DEF_MODEL_CHANGE_MODEL_POS] = m_plnkPanelData->m_dDistance_FiduMark_Xu
//											  - m_plnkSystemData->m_dModelChangeOriginDist[m_iWorkGroup];

	//SJ_YYK 110414 Modify.. Model Change 축 설계 치수 907.0 이상 움직이지 않도록...
	double dHalfFiduDist = 0.0;
	/*/
	if(m_plnkPanelData->m_dDistance_FiduMark_Xu < 902.5) 
	{
		dHalfFiduDist = 904.0 / 2.0;
	}
	else
		dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Xu/2.0;
	/*/
	//_____________________________________________________________________________*/

	double dMarkDistPanelToTab = 0.0;
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
#ifdef DEF_GATE_SYSTEM
		dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Xu/2.0;
	#ifndef DEF_MIRROR_
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;
	#else
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;
	#endif
#else
		//dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Yl/2.0;
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xu;
		dHalfFiduDist = m_plnkPanelData->m_dPanelSize_Y/2.0 - m_plnkPanelData->m_dX_FiduMark_Yl; //171003 JSH
#endif
		m_rgdYModelPos[DEF_MODEL_CHANGE_PANELALIGN_POS]
			= -m_plnkSystemData->m_dModelChangeOriginDist[m_iWorkGroup] - (dHalfFiduDist);
		m_rgdYModelPos[DEF_MODEL_CHANGE_MODEL_POS]
			= m_rgdYModelPos[DEF_MODEL_CHANGE_PANELALIGN_POS] - (dMarkDistPanelToTab + DEF_TABMARK_VISION_CENTER_OFFSET);
	}
	else
	{
#ifdef DEF_GATE_SYSTEM
		dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Xu/2.0;
	#ifndef DEF_MIRROR_
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;
	#else
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;
	#endif
#else
		//dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Yl/2.0;
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xd;
		dHalfFiduDist = m_plnkPanelData->m_dPanelSize_Y/2.0 - m_plnkPanelData->m_dY_FiduMark_Yl;//171003 JSH
#endif
		m_rgdYModelPos[DEF_MODEL_CHANGE_PANELALIGN_POS]
			= -m_plnkSystemData->m_dModelChangeOriginDist[m_iWorkGroup] + (dHalfFiduDist);
		m_rgdYModelPos[DEF_MODEL_CHANGE_MODEL_POS]
			= m_rgdYModelPos[DEF_MODEL_CHANGE_PANELALIGN_POS] + (dMarkDistPanelToTab + DEF_TABMARK_VISION_CENTER_OFFSET);
	}

	return ERR_MODEL_CHANGE_SUCCESS;
}

MModelChange::MModelChange(SCommonAttribute commonData, SModelChangeRefCompList listRefComponents, SModelChangeData datComponent)
: ICommon(commonData)
{
	SetCommonAttribute(commonData);
	
	// Component List Assign
	AssignComponents(listRefComponents);

	SetData(datComponent);
	//m_usIAddrUnloadPosYDetect = NULL;

	m_iWorkGroup = m_iInstanceNo;
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
		m_eWorkSide = WORKING_SIDE_Y_LEFT;
	#else
		m_eWorkSide = WORKING_SIDE_Y_RIGHT;
	#endif
#else
		m_eWorkSide = WORKING_SIDE_X_UP;
#endif
	}
	else
	{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
		m_eWorkSide = WORKING_SIDE_Y_RIGHT;
	#else
		m_eWorkSide = WORKING_SIDE_Y_LEFT;
	#endif
#else
		m_eWorkSide = WORKING_SIDE_X_DOWN;
#endif
	}


	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	LoadYFixedPosParameter();		// 고정 좌표 Load
	LoadYOffsetPosParameter();		// Offset 좌표 Load
	calculateModelChangePanelYModelPosParameter();
}

MModelChange::~MModelChange()
{

}

int MModelChange::AssignComponents(SModelChangeRefCompList ModelChangeCompList)
{
	m_plnkIO			= ModelChangeCompList.m_plnkIO;
	m_plnkY				= ModelChangeCompList.m_plnkY;
	m_plnkESCyl			= ModelChangeCompList.m_plnkESCyl;
	m_plnkUDCyl			= ModelChangeCompList.m_plnkUDCyl;
	m_plnkAirRailFB1	= ModelChangeCompList.m_plnkAirRailFB1;
	m_plnkAirRailFB2	= ModelChangeCompList.m_plnkAirRailFB2;

	return ERR_MODEL_CHANGE_SUCCESS;
}

IAxis* MModelChange::GetMotionComponent()
{
	return m_plnkY;
}

int MModelChange::LoadYFixedPosParameter(void)
{
	MTeachingPosData YFixedPosDat(m_sYTeachingInfo,m_strPosFileName);
	
	// Fixed Pos 
	BOOL bResult = YFixedPosDat.ReadData();
	
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		m_rgdYFixedPos[i] = YFixedPosDat.m_TeachingPos[i][0];
	}

	m_rgdYFixedPos[DEF_MODEL_CHANGE_INIT_POS] = 0.0;
	
	if (!bResult)
	{
		// 230000 = Y축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(230000);
	}
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::SaveYFixedPosParameter(double* rgdFixedPos)
{
	ASSERT(rgdFixedPos != NULL);

	MTeachingPosData ModelChangeYFixPosDat(m_sYTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		m_rgdYFixedPos[i] = rgdFixedPos[i];
		ModelChangeYFixPosDat.m_TeachingPos[i][0] = m_rgdYFixedPos[i];
	}

	BOOL bResult = ModelChangeYFixPosDat.WriteData();
	if (!bResult)
	{
		// 230001 = Y축 고정좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(230001);
	}
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::LoadYOffsetPosParameter(void)
{
	MTeachingPosData YOffsetPosDat(m_sYTeachingInfo,
									m_strOffsetPosFileName, 
									m_strOffsetPosFilePath);
	
	// Offset Pos 
	BOOL bResult = YOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		m_rgdYOffsetPos[i] = YOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
	{
		// 230002 = Y축 Offset 좌표를 읽어오지 못했습니다.
		return generateErrorCode(230002);
	}
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::SaveYOffsetPosParameter(double* rgdOffsetPos)
{
	ASSERT(rgdOffsetPos != NULL);

	MTeachingPosData YOffsetPosDat(m_sYTeachingInfo, 
									m_strOffsetPosFileName, 
									m_strOffsetPosFilePath);
	// Offset Pos 
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION ; i++)
	{
		m_rgdYOffsetPos[i] = rgdOffsetPos[i];
		YOffsetPosDat.m_TeachingPos[i][0] = m_rgdYOffsetPos[i];	
	}

	BOOL bResult = YOffsetPosDat.WriteData();
	if (!bResult)
	{
		// 230003 = Y축 Offset 좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(230003);
	}
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::GetYPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos)
{
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		if (rgdFixedPos != NULL)
			rgdFixedPos[i] = m_rgdYFixedPos[i];		
		if (rgdOffsetPos != NULL)
			rgdOffsetPos[i] = m_rgdYOffsetPos[i];		
		if (rgdModelPos != NULL)
			rgdModelPos[i]	= m_rgdYModelPos[i];
	}
	return ERR_MODEL_CHANGE_SUCCESS;
}

STeachingInfo MModelChange::GetYTeachingInfo()
{
	return m_sYTeachingInfo;
}

int MModelChange::SafeMoveYPos(int iPosID, BOOL bUseAuto, BOOL bWaitOption)
{
	double dTarget = GetYTargetPos(iPosID);		// 목표 위치 읽기
	return MoveYPos(dTarget, iPosID, bUseAuto, bWaitOption);	// 해당 위치로 이동
}

int MModelChange::CheckOrigin(void)
{
	// ModelChange Y Motion Check Origin
	if (m_plnkY->IsOriginReturn() == FALSE)
	{
		// 230004 = Y축 원점복귀되어 있지 않습니다.
		return generateErrorCode(230004);
	}
	return ERR_MODEL_CHANGE_SUCCESS;
}

double MModelChange::GetYCurrentPos(BOOL bType)
{
	ASSERT(m_plnkY != NULL);

	double dCurPos = m_plnkY->GetCurrentPosition(bType);
	return dCurPos;
}

double MModelChange::GetYTargetPos(int iPosID)
{
	ASSERT((iPosID >= 0) && (iPosID < DEF_MODEL_CHANGE_MAX_POSITION));

	// 목표좌표 = 고정 좌표	+ Model 좌표 + Offset 좌표
	double dTarget = m_rgdYFixedPos[iPosID] + m_rgdYModelPos[iPosID] + m_rgdYOffsetPos[iPosID];
	return dTarget;
}

double MModelChange::GetCurrentWidth(BOOL bType)
{
	double dCurPos = GetYCurrentPos(bType);
	double cWidth = m_plnkSystemData->m_dModelChangeOriginDist[m_iWorkGroup] + dCurPos;
	return cWidth;
}

double MModelChange::GetTargetWidth(int iPosID)
{
	double dTarget = GetYTargetPos(iPosID);
	double tWidth = m_plnkSystemData->m_dModelChangeOriginDist[m_iWorkGroup] + dTarget;
	return tWidth;
}

BOOL MModelChange::IsInYPos(int iPosID, double* pdTolerance)
{
	double dPosCompare = GetYTargetPos(iPosID);
	return IsInYPos(dPosCompare, pdTolerance);
}

BOOL MModelChange::IsInYPos(double dTargetPos, double* pdTolerance)
{
	return m_plnkY->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

BOOL MModelChange::IsInYMotion()
{
#ifdef SIMULATION
	return TRUE;
#endif

	return m_plnkY->IsInMotion();
}

int MModelChange::SetData(SModelChangeData datComponent)
{
	m_sPanelAlignerXYTTeachingInfo	= datComponent.m_sPanelAlignerXYTTeachingInfo;
	m_sPanelTransferXTeachingInfo	= datComponent.m_sPanelTransferXTeachingInfo;
	m_sYTeachingInfo				= datComponent.m_sYTeachingInfo;
	m_strPosFileName				= datComponent.m_strPosFileName;
	m_strOffsetPosFileName			= datComponent.m_strOffsetPosFileName;
	m_plnkSystemData				= datComponent.m_plnkSystemData;
	m_plnkPanelData					= datComponent.m_plnkPanelData;
	m_usOLockModelChangeRail		= datComponent.m_usOLockModelChangeRail;

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::AssignStateCheckComponents(SStateCheckCompListForModelChange sCompList)
{
	m_plnkStatePanelAligner		= sCompList.m_plnkStatePanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkStatePanelTransfer	= sCompList.m_plnkStatePanelTransfer;
#endif
	m_plnkStatePanelTransferOut	= sCompList.m_plnkStatePanelTransferOut;
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		m_plnkStateCameraCarrier[i] = sCompList.m_plnkStateCameraCarrier[i];

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::ChangeModel()
{
	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	LoadYFixedPosParameter();		// 고정 좌표 Load
	LoadYOffsetPosParameter();		// Offset 좌표 Load
	calculateModelChangePanelYModelPosParameter();

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::Initialize()
{
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::MoveYPos(double dTarget, int iPosID, BOOL bUseAuto, BOOL bWaitOption)
{

	double dTolerance = 0.001;
	if (TRUE == IsInYPos(dTarget, &dTolerance))
		return ERR_MODEL_CHANGE_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MModelChange] [M] [Move Y to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult = UnlockRail();	
	if (iResult)
		return iResult;

	iResult	= checkInterlockForPanelY(iPosID, bUseAuto);
	if (iResult)
		return iResult;

	// 축 이동 시작
	iResult = m_plnkY->StartMove(dTarget);
	if (iResult)
	{
		m_plnkY->Stop();
		//WriteErrorLog("MoveYPos() : ModelChange Y Axis StartMove() Fail");
		return iResult;
	}

	if (FALSE == bWaitOption)
	{
		Sleep(50);
		return ERR_MODEL_CHANGE_SUCCESS;
	}
 
	// 이동 완료 점검 
	iResult = m_plnkY->Wait4Done();
	if (iResult)
		return iResult;

	iResult = LockRail();	
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MModelChange] [M] [Move Y to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::Wait4DoneY(BOOL bSkipMode)
{
	int iResult = ERR_MODEL_CHANGE_SUCCESS;

	// 이동 완료 점검 
	iResult = m_plnkY->Wait4Done(bSkipMode);
	if (iResult)
		return iResult;

	iResult = LockRail();
	if (iResult)
		return iResult;

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::Expand(BOOL bSkipSensor)
{

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::Shrink(BOOL bSkipSensor)
{

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::Up(BOOL bSkipSensor)
{

	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::Down(BOOL bSkipSensor)
{
	return ERR_MODEL_CHANGE_SUCCESS;
}

BOOL MModelChange::IsExpand(void)
{
	return TRUE;
}

BOOL MModelChange::IsShrink(void)
{
	return TRUE;
}

BOOL MModelChange::IsUp(void)
{
	return TRUE;
}

BOOL MModelChange::IsDown(void)
{
	return TRUE;
}

int MModelChange::LockRail()
{
	return ERR_MODEL_CHANGE_SUCCESS;
	/*/
#ifdef SIMULATION
	return ERR_MODEL_CHANGE_SUCCESS;
#endif

//#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
//	return ERR_MODEL_CHANGE_SUCCESS;
//#endif

#ifdef DEF_GATE_SYSTEM
	ASSERT(m_plnkIO != NULL);

	return m_plnkIO->OutputOn(m_usOLockModelChangeRail);
#else
	return ERR_MODEL_CHANGE_SUCCESS;
#endif
	/*/
}

int MModelChange::UnlockRail()
{
	return ERR_MODEL_CHANGE_SUCCESS;
	/*/
#ifdef SIMULATION
	return ERR_MODEL_CHANGE_SUCCESS;
#endif

//#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
//	return ERR_MODEL_CHANGE_SUCCESS;
//#endif

#ifdef DEF_GATE_SYSTEM
	ASSERT(m_plnkIO != NULL);

	return m_plnkIO->OutputOff(m_usOLockModelChangeRail);
#else
	return ERR_MODEL_CHANGE_SUCCESS;
#endif
	/*/
}

BOOL MModelChange::IsLockRail(void)
{
#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	return FALSE;
#endif

	ASSERT(m_plnkIO != NULL);

	return m_plnkIO->IsOn(m_usOLockModelChangeRail);
}

BOOL MModelChange::IsUnlockRail(void)
{
#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	return TRUE;
#endif

	ASSERT(m_plnkIO != NULL);

	return m_plnkIO->IsOff(m_usOLockModelChangeRail);
}

double MModelChange::GetModelPosOffset()
{
	return m_rgdYOffsetPos[DEF_MODEL_CHANGE_MODEL_POS];		
}

//@______________________________________________
int MModelChange::ForAirRailFB1(BOOL bSkipSensor)
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB1 != NULL);
	if (FALSE == IsForAirRailFB1())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail1 FB Cyl For (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB1->Front(bSkipSensor);
		if (iResult)
		{
			SetErrorLevel(_T("MODEL CHANGE AirRail1 FB Cylinder For"),2, __FILE__, __LINE__);
			return iResult;
		}

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail1 FB Cyl For (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
#endif
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::ForAirRailFB2(BOOL bSkipSensor)
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB2 != NULL);
	if (FALSE == IsForAirRailFB2())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail2 FB Cyl For (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB2->Front(bSkipSensor);
		if (iResult)
		{
			SetErrorLevel(_T("MODEL CHANGE AirRail2 FB Cylinder For"),2, __FILE__, __LINE__);
			return iResult;
		}

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail2 FB Cyl For (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
#endif
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::BackAirRailFB1(BOOL bSkipSensor)
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB1 != NULL);
	if (FALSE == IsBackAirRailFB1())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail1 FB Cyl Back (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB1->Back(bSkipSensor);
		if (iResult)
		{
			SetErrorLevel(_T("MODEL CHANGE AirRail1 FB Cylinder Back"),2, __FILE__, __LINE__);
			return iResult;
		}

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail1 FB Cyl Back(SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
#endif
	return ERR_MODEL_CHANGE_SUCCESS;
}

int MModelChange::BackAirRailFB2(BOOL bSkipSensor)
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB2 != NULL);
	if (FALSE == IsBackAirRailFB2())
	{
		int iResult = checkInterlockForRailMove();
		if (iResult)
			return iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail2 FB Cyl Back (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkAirRailFB2->Back(bSkipSensor);
		if (iResult)
		{
			SetErrorLevel(_T("MODEL CHANGE AirRail2 FB Cylinder Back"),2, __FILE__, __LINE__);
			return iResult;
		}

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MModelChange] [M] [ModelChange AirRail2 FB Cyl Back(SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
#endif
	return ERR_MODEL_CHANGE_SUCCESS;
}

BOOL MModelChange::IsForAirRailFB1()
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB1 != NULL);
	return m_plnkAirRailFB1->IsFront();
#else
	return TRUE;
#endif
}

BOOL MModelChange::IsForAirRailFB2()
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB2 != NULL);
	return m_plnkAirRailFB2->IsFront();
#else
	return TRUE;
#endif
}

BOOL MModelChange::IsBackAirRailFB1()
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB1 != NULL);
	return m_plnkAirRailFB1->IsBack();
#else
	return TRUE;
#endif
}

BOOL MModelChange::IsBackAirRailFB2()
{
#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	ASSERT(m_plnkAirRailFB2 != NULL);
	return m_plnkAirRailFB2->IsBack();
#else
	return TRUE;
#endif
}
//@______________________________________________


// End Of Code
