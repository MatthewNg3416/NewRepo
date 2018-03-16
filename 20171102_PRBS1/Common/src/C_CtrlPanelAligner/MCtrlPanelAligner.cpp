/* 
 * Control PanelAligner Component
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
#include <math.h>
#include "IVision.h"
#include "MCtrlPanelAligner.h"
#include "MPanelAligner.h"
#include "MModelChange.h"
#include "MCameraCarrier.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MProcessData.h"
#include "MTabData.h" // SJ_YYK 150318 Add..
#include "MTabOffsetData.h" //SJ_YYK 150318 Add..
#include "MAutoAlignData.h" //SJ_YYK 150318 Add..
#include "common.h"
#include "MVisionCalibration.h"
#include "MTickTimer.h"
#include "ILighting.h"
#include "IAxis.h"
#include "MAxisData.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
#include "IStateTabMounter.h"
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/**
 * @stereotype constructor
 *
 * @param	commonData			: ���� Component Data
 * @param	listRefComponent	: CtrlPanelAligner Component���� ������ Component Pointer
 * @param	datComponent		: CtrlPanelAligner Component Data
 */
MCtrlPanelAligner::MCtrlPanelAligner(SCommonAttribute commonData, SCtrlPanelAlignerRefCompList listRefComponent, SCtrlPanelAlignerData datComponent)
: ICommon(commonData), m_dLastAutoSearchOffset(0.0)
{	
	SetData(datComponent);
	AssignComponents(listRefComponent);
}

/**
 * Destructor
 */
MCtrlPanelAligner::~MCtrlPanelAligner()
{
}

/**
 * CtrlPanelAligner Component�� �ʱ�ȭ �Ѵ�.
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MCtrlPanelAligner::Initialize(void)
{
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

/**
 * MCtrlPanelAligner�� Data Parameter�� �����Ѵ�. 
 *
 * @param	datcomponent : ������ MCtrlPanelAligner�� Parameter
 * @return	int (0 = Success, Error Code = �� ��)
 */
int	MCtrlPanelAligner::SetData(SCtrlPanelAlignerData datcomponent)
{
	m_plnkSystemData	= datcomponent.m_plnkSystemData;
	m_plnkPanelData		= datcomponent.m_plnkPanelData;
#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	m_plnkPreBonderData	= datcomponent.m_plnkPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	m_plnkACFData		= datcomponent.m_plnkACFData;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	m_plnkFinalData		= datcomponent.m_plnkFinalData;
#endif

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_plnkWorkSchedule[i] = datcomponent.m_plnkWorkSchedule[i];
#ifdef DEF_SOURCE_SYSTEM
		m_plnkWorkScheduleSecond[i] = datcomponent.m_plnkWorkScheduleSecond[i];
#endif
	}

	m_plnkAutoAlignData	= datcomponent.m_plnkAutoAlignData; //SJ_YYK 150318 Add..
	m_plnkTabOffset	= datcomponent.m_plnkTabOffset; //SJ_YYK 150318 Add..
	m_plnkTabOffset_Rear = datcomponent.m_plnkTabOffset_Rear; //SJ_YYK 150318 Add..
	m_plnkTabData = datcomponent.m_plnkTabData; //SJ_YYK 150318 Add..

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

/**
 * MCtrlPanelAligner�� Component List�� �����Ѵ�.
 * @param	listRefCompList : ������ MCtrlPCBHandler�� Component List
 * @return	int (0 = Success, Error Code = �� ��)
 */
int	MCtrlPanelAligner::AssignComponents(SCtrlPanelAlignerRefCompList listRefComponents)
{
	m_plnkVisionCalibration = listRefComponents.m_plnkCtrlVisionCalibration;
	m_plnkVision			= listRefComponents.m_plnkVision;
	m_plnkPanelAligner		= listRefComponents.m_plnkPanelAligner;
#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		m_plnkStateTabMounter[i]	= listRefComponents.m_plnkStateTabMounter[i];
		m_plnkStateRTabMounter[i]	= listRefComponents.m_plnkStateRTabMounter[i];
	}
	for (int i = 0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_plnkStateCameraCarrier[i] = listRefComponents.m_plnkStateCameraCarrier[i];
		m_plnkStateRCameraCarrier[i] = listRefComponents.m_plnkStateRCameraCarrier[i];
	}
#endif
	
	m_plnkModelChange1 = listRefComponents.m_plnkModelChange1;
	m_plnkModelChange2 = listRefComponents.m_plnkModelChange2;

	m_plnkCameraCarrier1 = listRefComponents.m_plnkCameraCarrier1;
	m_plnkCameraCarrier2 = listRefComponents.m_plnkCameraCarrier2;

	for (int i=0; i<DEF_MAX_LIGHTING; i++)
		m_plnkLighting[i] = listRefComponents.m_plnkLighting[i];
	
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::MovetoLoadPos()
{
	if (m_plnkPanelAligner->IsPanelAbsorbed())
		// 310001 = Panel�� �����ϰ� �־ Load ��ġ�� �̵��� �� �����ϴ�.
		return generateErrorCode(310001);

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Load Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
					? m_plnkPanelAligner->CheckVacuum()
					: m_plnkPanelAligner->ReleasePanel();
	if (iResult)
		return iResult;

	if (FALSE == m_plnkPanelAligner->IsDownPickUpUnit())
	{
		iResult = m_plnkPanelAligner->DownPickUpUnit();
		if (iResult)
			return iResult;
	}

	if (!m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_LOAD_POS))
	{
		iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(DEF_PANEL_ALIGNER_LOAD_POS);
		if (iResult)
			return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Load Pos] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::PickUpPanel()
{
	int iResult;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Pick Up Panel] [Start]");
	WriteTactTimeLog(szLogMsg);

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if (iResult)
		return iResult;

	iResult = m_plnkPanelAligner->AbsorbPanel();
	if (iResult)
	{
		Sleep(100);
		iResult = m_plnkPanelAligner->AbsorbPanel();
	}

	if (iResult)
	{
		m_plnkPanelAligner->ReleasePanel();
		Sleep(100);
		m_plnkPanelAligner->DownPickUpUnit();
		m_plnkPanelAligner->UpPickUpUnit();
		Sleep(100);

		iResult = m_plnkPanelAligner->AbsorbPanel();
		if (iResult)
			return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Pick Up Panel] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::SendPanel()
{
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Send Panel] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = m_plnkPanelAligner->ReleasePanel();
	if (iResult)
	{
		Sleep(100);
		iResult = m_plnkPanelAligner->ReleasePanel();
		if (iResult)
			return iResult;
	}

//KKY.Del	Sleep(200);
	
	iResult = m_plnkPanelAligner->DownPickUpUnit();
	if (iResult)
		return iResult;
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Send Panel] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::MovetoAlignPos(BOOL bUseAuto)
{
	int iAlignPosId = DEF_PANEL_ALIGNER_FIRST_MARK_POS;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Align Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(iAlignPosId, bUseAuto);
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Align Pos] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::MovetoInspectPos(int iInspectPosId, BOOL bUseAuto)
{
	ASSERT(iInspectPosId >= DEF_PANEL_ALIGNER_INSPECT_1_POS
		&& iInspectPosId <= DEF_PANEL_ALIGNER_INSPECT_2_POS);

#if defined(DEF_PLATFORM_ARP_SYSTEM) || defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	char szLogMsg[200];
#endif

#if defined(DEF_PLATFORM_ARP_SYSTEM)
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Inspect Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(iInspectPosId, bUseAuto);
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER move to Inspect Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Inspect Pos] [End]");
	WriteTactTimeLog(szLogMsg);
#endif

#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	/*
	double dFiduMarkXPos = m_plnkPanelAligner->GetXYTCurrentPos().dX;
	double dInspectionXPos = m_plnkPanelAligner->GetXYTTargetPos(iInspectPosId).dX;
	double dXDistanceBetweenAlignToInspection = dInspectionXPos - dFiduMarkXPos;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Inspect Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = m_plnkPanelAligner->RMoveXPosOnly(dXDistanceBetweenAlignToInspection, iInspectPosId, bUseAuto);
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER move to Inspect Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Inspect Pos] [End]");
	WriteTactTimeLog(szLogMsg);
	*/
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Inspect Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(iInspectPosId, bUseAuto);
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Inspect Pos] [End]");
	WriteTactTimeLog(szLogMsg);

#endif
	
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::MovetoWorkPos(BOOL bUseAuto)
{
	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	int iWorkPosId = DEF_PANEL_ALIGNER_PRESS_1_POS;

	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Work Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(iWorkPosId, bUseAuto);
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Work Pos] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::MoveZtoWorkPos(BOOL bUseAuto)
{
	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	int iWorkPosId = DEF_PANEL_ALIGNER_Z_WORK_POS;

	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move Z to Work Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	iResult = m_plnkPanelAligner->SafeMoveZPos(iWorkPosId, bUseAuto);
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move Z to Work Pos] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::MovetoUnloadPos(BOOL bUseAuto)
{
	int iResult;

	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		iResult = m_plnkPanelAligner->CheckVacuum();
		if (iResult)
			return iResult;
		
		if (FALSE == m_plnkPanelAligner->IsPanelAbsorbed())
			// 310003 = Panel�� �����ϰ� ���� �ʾ Unload ��ġ�� �̵��� �� �����ϴ�.
			return generateErrorCode(310003);
	}

	int iPanelWorkNo = 0;
#ifdef DEF_SOURCE_SYSTEM
	iPanelWorkNo = 1;
#endif

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Unload Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	MPos_XYT xytUnload = m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_UNLOAD_POS);
	iResult = checkTargetPosBySWLimit(xytUnload);

	iResult = FALSE; //2009.12.10  pth
	if (iResult)
	{
		// ���� : X(+) �� ħ���ϴ� ���� ���ٰ� ����.

		MPos_XYT xytCurrent = m_plnkPanelAligner->GetXYTCurrentPos();
		double dDiffY = xytUnload.dY - xytCurrent.dY;
		
		m_plnkPanelAligner->DownPickUpUnit(TRUE);
		m_plnkPanelAligner->AirCVVacAllOn(iPanelWorkNo);
		Sleep(1000);
		
		iResult = m_plnkPanelAligner->ReleasePanel();
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->DownPickUpUnit();
		if (iResult)
			return iResult;
		
		MPos_XYT xytTarget = xytCurrent;
		xytTarget.dY -= dDiffY;

		iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytTarget);
		if (iResult)
			return iResult;
		
		Sleep(200);
		
		iResult = m_plnkPanelAligner->UpPickUpUnit();
		if (iResult)
			return iResult;

		Sleep(500);	// Up ���� ��� �ð�.
		
		iResult = m_plnkPanelAligner->AbsorbPanel();
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AirCVBlowOn();
		if (iResult)
			return iResult;
		
		Sleep(1500);

		MPos_XYT xytNewUnload = xytUnload;
		xytNewUnload.dY = xytCurrent.dY;

		iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytNewUnload, DEF_PANEL_ALIGNER_UNLOAD_POS, bUseAuto);
		if (iResult)
			return iResult;
	}
	else
	{
		iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(DEF_PANEL_ALIGNER_UNLOAD_POS, bUseAuto);
		if (iResult)
			return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move to Unload Pos] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

/*/
int MCtrlPanelAligner::MoveCameraToPos(int PosID, BOOL wait)
{
	m_plnkCameraCarrier1->ExpandShrinkForPanelAlign(TRUE);
	m_plnkCameraCarrier2->ExpandShrinkForPanelAlign(TRUE);
	
	int iResult = m_plnkCameraCarrier1->ExpandShrinkForPanelAlign();
	if (iResult)
		return iResult;
	iResult = m_plnkCameraCarrier2->ExpandShrinkForPanelAlign();
	if (iResult)
		return iResult;

	if (m_plnkCameraCarrier1->IsInPos(PosID) && m_plnkCameraCarrier2->IsInPos(PosID))
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move Camera PosNo.%d] [Start]", PosID);
	WriteTactTimeLog(szLogMsg);
	CString szErr;

	iResult = m_plnkCameraCarrier1->SafeMovePos(PosID, FALSE);
	if (iResult)
	{
		szErr.Format(_T("CAMERA move to Position %d"),PosID);
		SetErrorLevel(szErr,1, __FILE__, __LINE__);
		return iResult;
	}

	if (wait)
	{
		iResult = m_plnkCameraCarrier2->SafeMovePos(0, PosID);
		if (iResult)
		{
			szErr.Format(_T("CAMERA move to Position %d"),PosID);
			SetErrorLevel(szErr,1, __FILE__, __LINE__);
			return iResult;
		}

		iResult = m_plnkCameraCarrier1->Wait4Done();
		if (iResult)
		{
			szErr.Format(_T("CAMERA move to Position %d"),PosID);
			SetErrorLevel(szErr,1, __FILE__, __LINE__);
			return iResult;
		}
	}
	else
	{
		iResult = m_plnkCameraCarrier2->SafeMovePos(PosID, FALSE);
		if (iResult)
		{
			szErr.Format(_T("CAMERA move to Position %d"),PosID);
			SetErrorLevel(szErr,1, __FILE__, __LINE__);
			return iResult;
		}	
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Move Camera PosNo.%d] [End]", PosID);
	WriteTactTimeLog(szLogMsg);
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}
//______________
/*/

int MCtrlPanelAligner::InspectPanel(int iPosID)
{
#ifdef SIMULATION
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
#endif

#ifdef DEF_PLATFORM_ARP_SYSTEM
#	ifdef DEF_SOURCE_SYSTEM
	LightingInspection();
#	endif

	int iMovePos = iPosID;
	int iCameraNo1 = DEF_CAMERA_INSPECTION_1;
	int iCameraNo2 = DEF_CAMERA_INSPECTION_2;
	int iMarkNo1 = DEF_BLOB_MARK1;
	int iMarkNo2 = DEF_BLOB_MARK2;

	CString strName;


	switch(iPosID)
	{
	case DEF_PANEL_ALIGNER_INSPECT_XU_POS:
		iCameraNo1 = DEF_PANEL_ALIGN_CAMERA_1;
		iCameraNo2 = DEF_PANEL_ALIGN_CAMERA_2;
		iMarkNo1 = DEF_BLOB_MARK1;
		iMarkNo2 = DEF_BLOB_MARK2;
		break;

	case DEF_PANEL_ALIGNER_INSPECT_YL_POS:
		// Mirror���� �� Yl, Yr  ���� �Ҵ�Ǵ� Cam ��ȣ�� �ݴ��.
#	ifdef DEF_MIRROR_
        iCameraNo1 = DEF_CAMERA_INSPECTION_2;
        iCameraNo2 = DEF_CAMERA_INSPECTION_1;
        iMarkNo1 = DEF_BLOB_MARK2;
        iMarkNo2 = DEF_BLOB_MARK1;
#	else
        iCameraNo1 = DEF_CAMERA_INSPECTION_1;
        iCameraNo2 = DEF_CAMERA_INSPECTION_2;
        iMarkNo1 = DEF_BLOB_MARK1;
        iMarkNo2 = DEF_BLOB_MARK2;
#	endif
		break;

	case DEF_PANEL_ALIGNER_INSPECT_YR_POS:
		// Mirror���� �� Yl, Yr  ���� �Ҵ�Ǵ� Cam ��ȣ�� �ݴ��.
#	ifdef DEF_MIRROR_
        iCameraNo1 = DEF_CAMERA_INSPECTION_2;
        iCameraNo2 = DEF_CAMERA_INSPECTION_1;
        iMarkNo1 = DEF_BLOB_MARK4;
        iMarkNo2 = DEF_BLOB_MARK3;
#	else
        iCameraNo1 = DEF_CAMERA_INSPECTION_1;
        iCameraNo2 = DEF_CAMERA_INSPECTION_2;
        iMarkNo1 = DEF_BLOB_MARK3;
        iMarkNo2 = DEF_BLOB_MARK4;
#	endif
		break;
	}

	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Inspect Panel] [Start]");
	WriteTactTimeLog(szLogMsg);

	if (!m_plnkPanelAligner->IsInXYTPos(iMovePos))
	{
		iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(iMovePos);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
			return iResult;
		}
	}

#	ifdef DEF_SOURCE_SYSTEM
	if (!m_plnkCameraCarrier1->IsInXPos(DEF_CAMERA_INSPECT_POS)
		|| !m_plnkCameraCarrier2->IsInXPos(DEF_CAMERA_INSPECT_POS))
	{
		iResult = m_plnkCameraCarrier1->SafeXMovePos(DEF_CAMERA_INSPECT_POS, FALSE);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
			return iResult;
		}

		iResult = m_plnkCameraCarrier2->SafeXMovePos(DEF_CAMERA_INSPECT_POS);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
			return iResult;
		}	

		iResult = m_plnkCameraCarrier1->WaitForDone();
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
			return iResult;
		}
	}
#	endif

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE
		|| m_plnkSystemData->m_eRunMode == PASS_RUN_MODE)
	{
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	}
	
	m_plnkVision->ClearOverlay(iCameraNo1);
	iResult = m_plnkVision->Grab(iCameraNo1);
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
		return iResult;
	}

	/** 2��° Channel Grab(DEF_CAMERA_INSPECTION_1, DEF_CAMERA_INSPECTION_2 ���� �Ի�) */
	/** Blob Analysis 1 */
	BOOL bAcfInspectResult1 = TRUE;
	int iBlobResult1 = m_plnkVision->RecognitionBLOBPattern(iCameraNo1, iMarkNo1);

	if (iBlobResult1)
	{
		/** Vision Error : 104019 = Blob �м� ��� Score ���� ��� */
		if (iBlobResult1 / 1000 == 104019)
		{
			Sleep(100);
			iBlobResult1 = m_plnkVision->RecognitionBLOBPattern(iCameraNo1, iMarkNo1);
			if (iBlobResult1)
			{
				//WriteErrorLog("InspectPanel():ACF ���� �˻� ����� NG �Դϴ�.");
				//SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);

				bAcfInspectResult1 = FALSE;

				strName.Format(_T("Inspect[Cam%d]"), iCameraNo1);
				m_plnkVision->EnableSaveImage(TRUE);
				m_plnkVision->SaveImage(iCameraNo1, (LPCTSTR)strName);
			}
		}
		else
		{
			return iBlobResult1;
		}
	}

	BOOL bAcfInspectResult2 = TRUE;
#	ifdef DEF_GATE_SYSTEM
	if (m_plnkPanelData->m_bUseFlag_Yl && m_plnkPanelData->m_bUseFlag_Yr)
	{
		Sleep(10);
		m_plnkVision->ClearOverlay(iCameraNo2);
		iResult = m_plnkVision->Grab(iCameraNo2);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
			return iResult;
		}

		/** Blob Analysis 2 */
#	endif
		int iBlobResult2 = m_plnkVision->RecognitionBLOBPattern(iCameraNo2, iMarkNo2);

		if (iBlobResult2)
		{
			/** Vision Error : 104019 = Blob �м� ��� Score ���� ��� */
			if (iBlobResult2 / 1000 == 104019)
			{
				Sleep(100);
				iBlobResult2 = m_plnkVision->RecognitionBLOBPattern(iCameraNo2, iMarkNo2);
				if (iBlobResult2)
				{
					//WriteErrorLog("InspectPanel():ACF ���� �˻� ����� NG �Դϴ�.");
					//SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
					bAcfInspectResult2 = FALSE;

					strName.Format(_T("Inspect[Cam%d]"), iCameraNo2);
					m_plnkVision->EnableSaveImage(TRUE);
					m_plnkVision->SaveImage(iCameraNo2, (LPCTSTR)strName);
				}
			}
			else
			{
				return iBlobResult2;
			}
				
		}
#	ifdef DEF_GATE_SYSTEM
	}
#	endif
	
#	ifdef DEF_GATE_SYSTEM
	if (m_plnkPanelData->m_bUseFlag_Yl && m_plnkPanelData->m_bUseFlag_Yr)
	{
#	endif

		if (!bAcfInspectResult1 || !bAcfInspectResult2)
		{
			if (!bAcfInspectResult1 && !bAcfInspectResult2)
			{
				// 310020 = ACF ���� �˻� ����� NG �Դϴ�. [ 1:NG, 2:NG ]
				SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
				return generateErrorCode(310020);
			}
			else if (!bAcfInspectResult1)
			{
				// 310021 = ACF ���� �˻� ����� NG �Դϴ�. [ 1:NG, 2:OK ]
				SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
				return generateErrorCode(310021);
			}
			else
			{
				// 310022 = ACF ���� �˻� ����� NG �Դϴ�. [ 1:OK, 2:NG ]
				SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
				return generateErrorCode(310022);
			}
		}
#	ifdef DEF_GATE_SYSTEM
	}
	else
	{
		if (!bAcfInspectResult1)
		{
			// 310021 = ACF ���� �˻� ����� NG �Դϴ�. [ 1:NG, 2:OK ]
			SetErrorLevel(_T("PANEL ALIGNER inspect Panel"),1, __FILE__, __LINE__);
			return generateErrorCode(310021);
		}
	}
#	endif

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Inspect Panel] [End]");
	WriteTactTimeLog(szLogMsg);
#endif

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::AlignPanel(BOOL bUseAutoSearch, BOOL bUseManualSearch)
{
	int iResult;
#ifdef SIMULATION
	Sleep(500);
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
#endif	/* SIMULATION */

//	LightingAlign();

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Align Panel] [Start]");
	WriteTactTimeLog(szLogMsg);

	/*
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE
		&& FALSE == m_plnkPanelAligner->IsPanelAbsorbed())
	{
		// 310004 = Align�� �Ϸ��µ�, Panel�� �����Ǿ� ���� �ʽ��ϴ�.
		SetErrorLevel(_T("PANEL ALIGNER align Panel"),1, __FILE__, __LINE__);
		return generateErrorCode(310004);
	}
	*/

	//170913 JSH.s InspectionCamera Move�� TrsInspection ����...
// 	iResult = MOlbSystemPre.MoveInspectCameraPanelAlignPos();
// 	if (iResult)
// 	{
// 		SetErrorLevel(_T("PANEL ALIGNER align Panel"),1, __FILE__, __LINE__);
// 		return iResult;
// 	}
	
	{//ModelChange Move
		int iModelChangeMovePos = DEF_MODEL_CHANGE_PANELALIGN_POS;
		if (FALSE == m_plnkPreBonderData->m_bUseTabPanelAlign)
			iModelChangeMovePos = DEF_MODEL_CHANGE_PANELALIGN_POS;
		else
			iModelChangeMovePos = DEF_MODEL_CHANGE_MODEL_POS;

		if( FALSE == m_plnkModelChange1->IsInYPos(iModelChangeMovePos) ||
				FALSE == m_plnkModelChange2->IsInYPos(iModelChangeMovePos)
		)
		{
			if (FALSE == m_plnkModelChange1->IsInYPos(iModelChangeMovePos))
			{
				iResult = m_plnkModelChange1->SafeMoveYPos(iModelChangeMovePos, TRUE, FALSE);
				if (iResult)
					return iResult;
			}
			if (FALSE == m_plnkModelChange2->IsInYPos(iModelChangeMovePos))
			{
				iResult = m_plnkModelChange2->SafeMoveYPos(iModelChangeMovePos, TRUE, FALSE);
				if (iResult)
					return iResult;
			}
			
			iResult = m_plnkModelChange1->Wait4DoneY();
			if (iResult)
				return iResult;
			
			iResult = m_plnkModelChange2->Wait4DoneY();
			if (iResult)
				return iResult;
		}
	}
	//170913 JSH.e

	// Mark ��ȣ, PanelAligner �� ī�޶� Position�� ã�´�.
	int	iMarkNo[2] = {DEF_FIDU_MARK, DEF_FIDU_MARK};						// Align Mark ��ȣ
	int	iVisionMarkPos = DEF_PANEL_ALIGNER_NONE_POS;	// VisionMark Pos ��ȣ
	setMarkNoCameraPos(iMarkNo, iVisionMarkPos);
	
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		for(int i = 0; i < m_plnkSystemData->m_iPanelAlignRetryLimit; i++)
		{
			TRACE(_T("[MCtrlPanelAligner] [Align Panel Ƚ��] : %d\n"), i+1);
			iResult = alignPanel(iMarkNo, iVisionMarkPos, bUseAutoSearch, bUseManualSearch);
			if(iResult == 0)
				break;
			else if(iResult != 1)
				return iResult;
		
			if (i == m_plnkSystemData->m_iPanelAlignRetryLimit - 1)
				// 410005 = Align Panel ���� ���� Ƚ�� �ʰ�.
				return generateErrorCode(410005);
		}
	}

	iResult = checkPanelAlignerGetPosition(iMarkNo, iVisionMarkPos, bUseManualSearch);
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Align Panel] [End]");
	WriteTactTimeLog(szLogMsg);
	
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::AlignPanelSetPoint(CPoint ptMark1, CPoint ptMark2)
{
	MPos_XYT MarkPosition1;			// ù��° Mark PanelAligner �߽ɿ��������� ��ǥ
	MPos_XYT MarkPosition2;			// �ι�° Mark PanelAligner �߽ɿ��������� ��ǥ
	MPos_XYT ActualAlignPos;		// ���� Align Motion ��ǥ

	calculateFiduMarkPosition(MarkPosition1, MarkPosition2);

	// Mark �ν� ��� ���� Air Conveyor Panel ȸ�� �߽�(Vacuum Pad2 Center)������ �Ÿ��� ���ϱ� ����  Mark�� ��ġ�� ����Ѵ�. 

	// Align�� �ȵǾ��� ��� Fidu Mark ��ġ�� Offset�� ���� ����ؾ� �Ѵ�.
	// Align ��ġ�� Offset�� �ٸ� ��ġ�� ������ ��ġ�� ���� ���� �ϰ��� ���̴�.
	// �ڵ� Ȯ�� �ʿ� : Offset �� �ϴ� �ݿ� ����.

	// PanelAligner���� Offset ��ǥ�� �о�´�.
	// (Align ��ġ�� Offset ���� �ݿ��ϱ� ���ؼ� �ʿ���)
	MPos_XYT sPanelAlignerXYTOffsetPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	m_plnkPanelAligner->GetXYTPosParameter(NULL, sPanelAlignerXYTOffsetPos, NULL);

	int	iMarkNo[2] = {DEF_NONE_MARK, DEF_NONE_MARK};	// Align Mark ��ȣ
	int	iVisionMarkPos = DEF_PANEL_ALIGNER_NONE_POS;	// VisionMark Pos ��ȣ
	setMarkNoCameraPos(iMarkNo, iVisionMarkPos);

	MarkPosition1 = MarkPosition1 - sPanelAlignerXYTOffsetPos[iVisionMarkPos];
	MarkPosition2 = MarkPosition2 - sPanelAlignerXYTOffsetPos[iVisionMarkPos];


	// ù��° Align�ϴ� ��ǥ ����
	ActualAlignPos	= m_plnkPanelAligner->GetXYTCurrentPos();

//____________________________________________________________
//
// Step 2. Fiducial Mark �ν�
//____________________________________________________________
	int rgiCamNo[2] = { DEF_PANEL_ALIGN_CAMERA_1, DEF_PANEL_ALIGN_CAMERA_2 };
	MPos_XYT posMark1;
	MPos_XYT posMark2;
	MPos_XYT* pxytMark[2] = { &posMark1, &posMark2 };
	double dPixelX[2] = {ptMark1.x, ptMark2.x};
	double dPixelY[2] = {ptMark1.y, ptMark2.y};

	for (int i = 0; i < 2; i++)
	{
		m_plnkVisionCalibration->ConvertV2R(rgiCamNo[i], NULL,
											dPixelX[i],
											dPixelY[i],
											pxytMark[i]->dX, pxytMark[i]->dY);
		pxytMark[i]->dT	= 0;
	}

	posMark1 = posMark1 + MarkPosition1;
	posMark2 = posMark2 + MarkPosition2;
//____________________________________________________________
//
// Step 3. Align Offset T ��� �ϱ�
//____________________________________________________________

	// Radian ������ Ʋ���� ���� ���
#ifdef DEF_SOURCE_SYSTEM
	double dT = asin((posMark2.dX - posMark1.dX) / m_plnkPanelData->m_dDistance_FiduMark_Yl);
#else
	double dT = asin((posMark2.dX - posMark1.dX) / m_plnkPanelData->m_dDistance_FiduMark_Xu);
#endif

	
	// Radian => Degree ��ȯ
	double dDegreeT = RAD2DEG(dT);
	
	CString Tolerance;
	Tolerance.Format(_T("��ġ����, posMark1.dX:%.4f, posMark1.dY:%.4f, posMark2.dX:%.4f, posMark2.dY%.4f"),
		posMark1.dX,
		posMark1.dY,
		posMark2.dX,
		posMark2.dY);
	WriteNormalLog(Tolerance);
	
//____________________________________________________________
//
// Step 4. Align Offset ��ü ���.
//____________________________________________________________
	
	double dROffsetX = cos(dT) * (posMark1.dX) - sin(dT) * (posMark1.dY);
	double dROffsetY = sin(dT) * (posMark1.dX) + cos(dT) * (posMark1.dY);
//	double dROffsetY = -sin(dT) * (posMark1.dX) + cos(dT) * (posMark1.dY);
	
	MPos_XYT xytAlign;
	xytAlign.dX = MarkPosition1.dX - dROffsetX;
	xytAlign.dY = MarkPosition1.dY - dROffsetY;
	xytAlign.dT = dDegreeT;

//____________________________________________________________
//
// Step 5. Panel Align Offset ����
//____________________________________________________________

	// T ��ǥ ���� ���� üũ, �ʹ� ���� ���� ���� �ȵȴ�.	
	if (fabs(xytAlign.dT) > DEF_ALIGN_T_TOLERANCE
		|| fabs(xytAlign.dX) > DEF_ALIGN_XY_TOLERANCE
		|| fabs(xytAlign.dY) > DEF_ALIGN_XY_TOLERANCE * 1.5)
		// 310007 = Align �������� ���ġ�� �ʰ��߽��ϴ�. (X,Y ���Ѱ� : 10mm �̳� , T ���Ѱ� : 5��)
		return generateErrorCode(310007);

	// Align ����� Mechanical Layer�� PanelAligner Component�� �����Ѵ�.
	MPos_XYT PriorAlignOffset = m_plnkPanelAligner->GetAlignMarkOffset();	// PanelAligner�� ����� Align�� �о� �´�.
	xytAlign = xytAlign + PriorAlignOffset;									// Align �������� ���� �Ѵ�.
	m_plnkPanelAligner->SetAlignMarkOffset(xytAlign, ActualAlignPos.dT);	// PanelAligner�� Align ������� ���� �Ѵ�.

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}


/** Align�� ���� Fidu-Mark�� Position ã�� */
void MCtrlPanelAligner::calculateFiduMarkPosition(MPos_XYT &MarkPosition1, MPos_XYT &MarkPosition2)
{
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (FALSE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
		double dVarPosX = m_plnkPanelAligner->GetXYTModelPosParameter(DEF_PANEL_ALIGNER_FIRST_MARK_POS).dX;
		MarkPosition1.dX = dVarPosX;
		MarkPosition1.dY = -m_plnkPanelData->m_dPanelSize_X/2.0 + m_plnkPanelData->m_dX_FiduMark_Xu;
		MarkPosition2.dX = dVarPosX;
		MarkPosition2.dY = MarkPosition1.dY + m_plnkPanelData->m_dDistance_FiduMark_Xu;
	}
	else
	{
		MInspectCarrierInfo	FirstCarrierInfo = m_plnkWorkSchedule[0]->GetFirstWorkTabCarrierInfo_FromFirstWorkList();
		MInspectCarrierInfo LastCarrierInfo = m_plnkWorkSchedule[0]->GetLastWorkTabCarrierInfo_FromFirstWorkList();
		double dVarPosX = m_plnkPanelAligner->GetXYTModelPosParameter(DEF_PANEL_ALIGNER_FIRST_MARK_POS).dX;
		MarkPosition1.dX = dVarPosX;
		MarkPosition1.dY = FirstCarrierInfo.m_dInspectPos - m_plnkPreBonderData->m_dLead_Width_Xtab/2.0;
		MarkPosition2.dX = dVarPosX;
		MarkPosition2.dY = LastCarrierInfo.m_dInspectPos + m_plnkPreBonderData->m_dLead_Width_Xtab/2.0;
	}
#else
	/*/

	double dFiduMarkPosX = 1.5, dFiduMarkPosY = 1.5, dMarkDist = 0.0;
	double dLeadWidth;
	dFiduMarkPosX = m_plnkPanelData->m_dX_FiduMark_Xu;
	dFiduMarkPosY = m_plnkPanelData->m_dY_FiduMark_Xu;
#ifdef DEF_GATE_SYSTEM
	dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Xu;
	dLeadWidth = m_plnkPreBonderData->m_dLead_Width_Ytab;
#else
	dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Yl;
	dLeadWidth = m_plnkPreBonderData->m_dLead_Width_Xtab;
#endif

	if (FALSE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
//		double dVarPosX = -m_plnkPanelAligner->GetXYTModelPosParameter(DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS).dX;
//@		double dVarPosX = -MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X() + m_plnkPanelData->m_dY_FiduMark_Xu;
#ifdef DEF_GATE_SYSTEM
		//Panel Mark Align�� Xu������
		double dVarPosX = -MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X() + dFiduMarkPosY;
		MarkPosition1.dX = dVarPosX;
		MarkPosition1.dY = -m_plnkPanelData->m_dPanelSize_X/2.0 + dFiduMarkPosX;
		MarkPosition2.dX = dVarPosX;
		MarkPosition2.dY = MarkPosition1.dY + dMarkDist;		
#else
		//Panel Mark Align�� Y������
		double dVarPosX = MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X() + dFiduMarkPosY;//@ + m_plnkPanelData->m_dDistance_FiduMark_Xu;
		MarkPosition1.dX = dVarPosX;
		MarkPosition1.dY = -m_plnkPanelData->m_dPanelSize_Y/2.0 + dFiduMarkPosX;
		MarkPosition2.dX = dVarPosX;
		MarkPosition2.dY = MarkPosition1.dY + dMarkDist;
#endif
	}
	else
	{
		int iAlignGroup = MOlbSystemPre.GetPanelAlignGroup();
		MInspectCarrierInfo	FirstCarrierInfo = m_plnkWorkSchedule[iAlignGroup]->GetFirstWorkTabCarrierInfo_FromFirstWorkList();
		MInspectCarrierInfo LastCarrierInfo = m_plnkWorkSchedule[iAlignGroup]->GetLastWorkTabCarrierInfo_FromFirstWorkList();

		MarkPosition1.dX = FirstCarrierInfo.m_dInspectPos
							- dLeadWidth/2.0
							- MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X()
							+ dFiduMarkPosY;
		MarkPosition2.dX = LastCarrierInfo.m_dInspectPos
							+ dLeadWidth/2.0
							- MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X()
							+ dFiduMarkPosY;
#ifdef DEF_SOURCE_SYSTEM
		/*/
					Xu(Mirror)
			Second		First			--> First �����۾��� Align��
		_________________________
		|			|			|
		|			|			|		-------------> Panel���� ����
		|			|			|
		|			|			|
		_________________________
		/*/
		m_plnkPanelAligner->CalculateWorkTabNo();
		int iTabStart = m_plnkPanelAligner->GetStartTabNo(1);	//�ι�° �����۾� ù��° Tab��ȣ
		int iTabLast = m_plnkPanelAligner->GetStartTabNo(0);	//ù��° �����۾� ù��° Tab��ȣ
		double dDistTabToTab = MOlbSystemPre.GetXDistance_TabCenterToCenter(WORKING_SIDE_X_UP, iTabStart, iTabLast);
		MarkPosition1.dX += dDistTabToTab;
		MarkPosition2.dX += dDistTabToTab;
#endif

		double dMarkDistPanelToTab = 0.0;

#ifdef DEF_GATE_SYSTEM
	#	ifndef DEF_MIRROR_
		if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;
			MarkPosition1.dY = -m_plnkPanelData->m_dPanelSize_X/2.0 + m_plnkPanelData->m_dX_FiduMark_Yl - (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
			MarkPosition2.dY = -m_plnkPanelData->m_dPanelSize_X/2.0 + m_plnkPanelData->m_dX_FiduMark_Yl - (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
		}
		else if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;
			MarkPosition1.dY = m_plnkPanelData->m_dPanelSize_X/2.0 - m_plnkPanelData->m_dX_FiduMark_Yr + (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
			MarkPosition2.dY = m_plnkPanelData->m_dPanelSize_X/2.0 - m_plnkPanelData->m_dX_FiduMark_Yr + (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
		}
	#	else
		if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;
			MarkPosition1.dY = -m_plnkPanelData->m_dPanelSize_X/2.0 + m_plnkPanelData->m_dX_FiduMark_Yr - (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
			MarkPosition2.dY = -m_plnkPanelData->m_dPanelSize_X/2.0 + m_plnkPanelData->m_dX_FiduMark_Yr - (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
		}
		else if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;
			MarkPosition1.dY = m_plnkPanelData->m_dPanelSize_X/2.0 - m_plnkPanelData->m_dX_FiduMark_Yl + (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
			MarkPosition2.dY = m_plnkPanelData->m_dPanelSize_X/2.0 - m_plnkPanelData->m_dX_FiduMark_Yl + (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
		}
	#	endif
#else
		dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xu;
		MarkPosition1.dY = -m_plnkPanelData->m_dPanelSize_Y/2.0 + m_plnkPanelData->m_dX_FiduMark_Xu - (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
		MarkPosition2.dY = -m_plnkPanelData->m_dPanelSize_Y/2.0 + m_plnkPanelData->m_dX_FiduMark_Xu - (dMarkDistPanelToTab+DEF_TABMARK_VISION_CENTER_OFFSET);
#endif
	}
//@#endif
	// ------------------------------------------------------------------------------------------
}

/** �۾��� �� �� ������ ���� MarkNo, PanelAligner Camera Pos�� ã�� �Լ� */
//void MCtrlPanelAligner::setMarkNoCameraPos(int &iMarkNo, int &iVisionMarkPos)
void MCtrlPanelAligner::setMarkNoCameraPos(int* iMarkNo, int &iVisionMarkPos)
{
	ASSERT(iVisionMarkPos == DEF_PANEL_ALIGNER_NONE_POS);
	if (FALSE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
		iMarkNo[0] = iMarkNo[1] = DEF_FIDU_MARK;
	}
	else
	{
//		iMarkNo[0] = DEF_PANEL_LEAD_XU_MARK1;
//		iMarkNo[1] = DEF_PANEL_LEAD_XU_MARK2;
		iMarkNo[0] = iMarkNo[1] = DEF_FIDU_SUB_MARK;
	}
	iVisionMarkPos = DEF_PANEL_ALIGNER_FIRST_MARK_POS;
}

//int MCtrlPanelAligner::searchFiduMark(int* rgiCamNo, int iMarkNo, BOOL* rgbUse, BOOL* rgbResult, int iOpType)
int MCtrlPanelAligner::searchFiduMark(int* rgiCamNo, int* iMarkNo, BOOL* rgbUse, BOOL* rgbResult, int iOpType)
{
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [search FiduMark] [Start]");
	WriteTactTimeLog(szLogMsg);

//	ASSERT(rgiCamNo[0] == DEF_PANEL_ALIGN_CAMERA_1);
//	ASSERT(rgiCamNo[1] == DEF_PANEL_ALIGN_CAMERA_2);
//	ASSERT(iMarkNo == DEF_FIDU_MARK);
	ASSERT(rgbUse[0] == TRUE || rgbUse[1] == TRUE);

	rgbResult[0] = FALSE;
	rgbResult[1] = FALSE;

	//�ӽ�
//	rgiCamNo[0] = DEF_PANEL_ALIGN_CAMERA_1;
//	rgiCamNo[1] = DEF_PANEL_ALIGN_CAMERA_2;

	if (TRUE == rgbUse[0] && TRUE == rgbUse[1])
	{
		//������������������������������������������������������������
		
		// Vision Grab.
		m_plnkVision->LockCamera(rgiCamNo[0]);
		m_plnkVision->LockCamera(rgiCamNo[1]);
		
		m_plnkVision->ClearOverlay(rgiCamNo[0]);
		m_plnkVision->ClearOverlay(rgiCamNo[1]);

		int iResult = m_plnkVision->Grab(rgiCamNo[0]);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(rgiCamNo[0]);
			m_plnkVision->UnlockCamera(rgiCamNo[1]);
			
			return iResult;
			
		}
		
		iResult = m_plnkVision->Grab(rgiCamNo[1]);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(rgiCamNo[0]);
			m_plnkVision->UnlockCamera(rgiCamNo[1]);

			return iResult;
		}
		
		//������������������������������������������������������������

		// Vision �ν�.
		for (int i = 0; i < 2; i++)
		{

			iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], iMarkNo[i]);
			if (iResult)	// Mark �ν� Retry ����.
			{
				if (DEF_SEARCH_FIDU_MARK_IN_SEARCH == iOpType)
				{
					Sleep(200);
					iResult = m_plnkVision->Grab(rgiCamNo[i]);
					iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], iMarkNo[i]);
				}
			}
			if (iResult)
			{
				// Error Image ����.
				CString strMsg;

				switch (iOpType)
				{
				case DEF_SEARCH_FIDU_MARK_IN_SEARCH:
					strMsg.Format(_T("PA cam%d"), i+1);
					break;
				case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_1ST_STEP:
					strMsg.Format(_T("PA cam%d (AS1)"), i+1);
					break;
				case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_2ND_STEP:
					strMsg.Format(_T("PA cam%d (AS2)"), i+1);
					break;
				case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_3RD_STEP:
					strMsg.Format(_T("PA cam%d (AS3)"), i+1);
					break;
				case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_4TH_STEP:
					strMsg.Format(_T("PA cam%d (AS4)"), i+1);
					break;
				}
				m_plnkVision->SaveImage(rgiCamNo[i], (LPCTSTR)strMsg);
			}
			else
			{
				rgbResult[i] = TRUE;
			}
		}
		
		m_plnkVision->UnlockCamera(rgiCamNo[0]);
		m_plnkVision->UnlockCamera(rgiCamNo[1]);
	}
	else
	{
		int iSideNo = -1;
		if (TRUE == rgbUse[0])
			iSideNo = 0;
		else // if (TRUE == rgbUse[1])
			iSideNo = 1;
		
		m_plnkVision->LockCamera(rgiCamNo[iSideNo]);
		m_plnkVision->ClearOverlay(rgiCamNo[iSideNo]);
		
		int iResult = m_plnkVision->Grab(rgiCamNo[iSideNo]);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(rgiCamNo[iSideNo]);
			return iResult;
		}
		
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[iSideNo], iMarkNo[iSideNo]);
		if (iResult)	// Mark �ν� Retry ����.
		{
			Sleep(200);
			iResult = m_plnkVision->Grab(rgiCamNo[iSideNo]);
			iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[iSideNo], iMarkNo[iSideNo]);
		}
		if (iResult)
		{
			// Error Image ����.
			CString strMsg;

			switch (iOpType)
			{
			case DEF_SEARCH_FIDU_MARK_IN_SEARCH:
				strMsg.Format(_T("PA cam%d"), iSideNo+1);
				break;
			case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_1ST_STEP:
				strMsg.Format(_T("PA cam%d (AS1)"), iSideNo+1);
				break;
			case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_2ND_STEP:
				strMsg.Format(_T("PA cam%d (AS2)"), iSideNo+1);
				break;
			case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_3RD_STEP:
				strMsg.Format(_T("PA cam%d (AS3)"), iSideNo+1);
				break;
			case DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_4TH_STEP:
				strMsg.Format(_T("PA cam%d (AS4)"), iSideNo+1);
				break;
			}
			m_plnkVision->SaveImage(rgiCamNo[iSideNo], (LPCTSTR)strMsg);
		}
		else
		{
			rgbResult[iSideNo] = TRUE;
		}
		
		m_plnkVision->UnlockCamera(rgiCamNo[iSideNo]);
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [search FiduMark] [End]");
	WriteTactTimeLog(szLogMsg);
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

//int MCtrlPanelAligner::getFiduMarkPosition(int* rgiCamNo, int iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL bUseAutoSearch)
int MCtrlPanelAligner::getFiduMarkPosition(int* rgiCamNo, int* iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL bUseAutoSearch, BOOL bUseManualSearch)
{
	BOOL rgbUse[2] = { TRUE, TRUE };
	BOOL rgbResult[2] = { FALSE, FALSE };

	if (FALSE == m_plnkSystemData->m_bUseTabReelAutoChange)
		bUseManualSearch = FALSE;

	int iResult = searchFiduMark(rgiCamNo, iMarkNo, rgbUse, rgbResult);
	if (iResult)	// Mark �ν� �̿��� Vision ��ü���� ���� �߻� ���� Error.
		return iResult;

	if (FALSE == rgbResult[0] || FALSE == rgbResult[1])
	{
		if (FALSE == bUseAutoSearch || FALSE == m_plnkSystemData->m_bUseAutoSearchPanel)	// �ڵ� ã�Ⱑ �ƴ� ���.
		{
			if (FALSE == rgbResult[0] && FALSE == rgbResult[1])
			{
				//@____________________________
				if (TRUE == bUseManualSearch)
				{
					CPoint ptManual[2];
					int iRet = -1; //@Camera2�� �� �����Ҽ� �ְ�
					::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetTitleViewHandle(),
						WM_DISP_MANUAL_SEARCH_POINT, (WPARAM)(ptManual), (LPARAM)(&iRet));
					if (iRet == IDOK)
					{
						for (int i = 0; i < 2; i++)
						{
							m_plnkVisionCalibration->ConvertV2R(rgiCamNo[i], NULL,
																ptManual[i].x,
																ptManual[i].y,
																pxytMark[i]->dX, pxytMark[i]->dY);
							pxytMark[i]->dT	= 0;
						}
						return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
					}
				}
				//@____________________________
				// 310005 = Panel Fiducial Mark �νĿ� �����߽��ϴ�.
				return generateErrorCode(310005);
			}
			else if (FALSE == rgbResult[0])
			{
				// 310010 = Panel Fiducial Mark �ν� ���� [PanelAlign Cam1]
				return generateErrorCode(310010);
			}
			else // if (FALSE == rgbResult[1])
			{
				// 310011 = Panel Fiducial Mark �ν� ���� [PanelAlign Cam2]
				return generateErrorCode(310011);
			}
		}
		return autoSearchFiduMark(rgiCamNo, iMarkNo, iVisionMarkPos, pxytMark, rgbResult);
	}

	//������������������������������������������������������������
	// Calibration Factor ����.
	for (int i = 0; i < 2; i++)
	{
		m_plnkVisionCalibration->ConvertV2R(rgiCamNo[i], NULL,
											m_plnkVision->GetSearchResultX(rgiCamNo[i], iMarkNo[i]),
											m_plnkVision->GetSearchResultY(rgiCamNo[i], iMarkNo[i]),
											pxytMark[i]->dX, pxytMark[i]->dY);
		pxytMark[i]->dT	= 0;
	}
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::CaptureFiduMark(CString strMsg)
{
	/* Log �и��� ���ؼ� ȣ���ϴ� �κп��� Option Check...
	if (FALSE == m_plnkSystemData->m_bLogLevelNormal)
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	*/

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Capture FiduMark] [Start]");
	WriteTactTimeLog(szLogMsg);

	MPos_XYT posResult1(-99.9, -99.9, -99.9);
	MPos_XYT posResult2(-99.9, -99.9, -99.9);

	m_plnkVision->LockCamera(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->LockCamera(DEF_PANEL_ALIGN_CAMERA_2);

	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_2);

	int iResult = m_plnkVision->Grab(DEF_PANEL_ALIGN_CAMERA_1);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_2);
		return iResult;
	}

	iResult = m_plnkVision->Grab(DEF_PANEL_ALIGN_CAMERA_2);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_2);
		return iResult;
	}

	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_SUB_MARK);
	else
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK);
	if (iResult == ERR_VISION_SUCCESS)
	{
		if (m_plnkPreBonderData->m_bUseTabPanelAlign)
			m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, NULL,
				m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_SUB_MARK),
				m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_SUB_MARK),
				posResult1.dX, posResult1.dY);
		else
			m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, NULL,
				m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK),
				m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK),
				posResult1.dX, posResult1.dY);
		posResult1.dT = 0;
	}
	m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
	
	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_SUB_MARK);
	else
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK);
	if (iResult == ERR_VISION_SUCCESS)
	{
		if (m_plnkPreBonderData->m_bUseTabPanelAlign)
			m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_2, NULL,
				m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_SUB_MARK),
				m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_SUB_MARK),
				posResult2.dX, posResult2.dY);
		else
			m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_2, NULL,
				m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK),
				m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK),
				posResult2.dX, posResult2.dY);
		posResult2.dT = 0;
	}
	m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_2);

	/*/// 2010.08.10 KBC -----------------------------------------------
	MPos_XYT posCurrent = m_plnkPanelAligner->GetXYTCurrentPos();
	double dX2Current = m_plnkPanelAligner->GetPanelAlignerX2CurrentPos();

	CString strLog;
	strLog.Format(_T("CaptureFiduMark (mark%d) :,%s,Cam%d [,%.3f,%.3f,] Cam%d [,%.3f,%.3f,] PanelAligner [,%.3f,%.3f,%.3f,X2(,%.3f,)]"),
		DEF_FIDU_MARK, strMsg.GetBuffer(strMsg.GetLength()), DEF_PANEL_ALIGN_CAMERA_1, posResult1.dX, posResult1.dY, DEF_PANEL_ALIGN_CAMERA_2, posResult2.dX, posResult2.dY, posCurrent.dX, posCurrent.dY, posCurrent.dT, dX2Current);
	m_plnkVision->WriteLog(_T("..\\Log\\panel_log.csv"), (LPCTSTR)strLog);

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Capture FiduMark] [End]");
	WriteTactTimeLog(szLogMsg);
	/*/// 2010.08.10 KBC -----------------------------------------------

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::CaptureFiduMarkTest(CString strMsg)
{
	if (FALSE == m_plnkSystemData->m_bLogLevelNormal)
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Capture FiduMark Test] [Start]");
	WriteTactTimeLog(szLogMsg);

	MPos_XYT posResult1(-99.9, -99.9, -99.9);
	MPos_XYT posResult2(-99.9, -99.9, -99.9);

	m_plnkVision->LockCamera(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->LockCamera(DEF_PANEL_ALIGN_CAMERA_2);

	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_2);
	
	int iResult = m_plnkVision->Grab(DEF_PANEL_ALIGN_CAMERA_1);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_2);
		return iResult;
	}

	iResult = m_plnkVision->Grab(DEF_PANEL_ALIGN_CAMERA_2);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_2);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK);
	if (iResult == ERR_VISION_SUCCESS)
	{
		m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, NULL,
			m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK),
			m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK),
			posResult1.dX, posResult1.dY);
		posResult1.dT = 0;
	}
	m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
	
	iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK);
	if (iResult == ERR_VISION_SUCCESS)
	{
		m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_2, NULL,
			m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK),
			m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK),
			posResult2.dX, posResult2.dY);
		posResult2.dT = 0;
	}
	m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_2);

	/*/// 2010.08.10 KBC -----------------------------------------------
	MPos_XYT posCurrent = m_plnkPanelAligner->GetXYTCurrentPos();
	double dX2Current = m_plnkPanelAligner->GetPanelAlignerX2CurrentPos();

	CString strLog;
	strLog.Format(_T("CaptureFiduMark (mark%d) :,%s,Cam%d [,%.3f,%.3f,] Cam%d [,%.3f,%.3f,] PanelAligner [,%.3f,%.3f,%.3f,X2(,%.3f,)]"),
		DEF_FIDU_MARK, strMsg.GetBuffer(strMsg.GetLength()), DEF_PANEL_ALIGN_CAMERA_1, posResult1.dX, posResult1.dY, DEF_PANEL_ALIGN_CAMERA_2, posResult2.dX, posResult2.dY, posCurrent.dX, posCurrent.dY, posCurrent.dT, dX2Current);
	m_plnkVision->WriteLog(_T("..\\Log\\panel_moving_test_log.csv"), (LPCTSTR)strLog);

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Capture FiduMark Test] [End]");
	WriteTactTimeLog(szLogMsg);
	/*/// 2010.08.10 KBC -----------------------------------------------

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

int MCtrlPanelAligner::checkTargetPosBySWLimit(MPos_XYT xytTarget)
{
	IAxis* plnkX = m_plnkPanelAligner->GetMotionComponent(DEF_X);
	IAxis* plnkY = m_plnkPanelAligner->GetMotionComponent(DEF_Y);

	MAxisData axisData;
	plnkX->GetParameter(&axisData);
	double dX_MinusSWLimit = axisData.dCCWSWLimit;
	double dX_PlusSWLimit = axisData.dCWSWLimit;

	plnkY->GetParameter(&axisData);
	double dY_MinusSWLimit = axisData.dCCWSWLimit;
	double dY_PlusSWLimit = axisData.dCWSWLimit;

	if (xytTarget.dX > dX_MinusSWLimit + 0.05
		&& xytTarget.dX < dX_PlusSWLimit - 0.05
		&& xytTarget.dY > dY_MinusSWLimit + 0.05
		&& xytTarget.dY < dY_PlusSWLimit - 0.05)
	{
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	}

	// 310012 = ��ǥ ��ġ �̵� �� S/W Limit ������ �ʰ��մϴ�.
	return generateErrorCode(310012);
}

//int MCtrlPanelAligner::checkPanelAlignerGetPosition(int iMarkNo, int iVisionMarkPos)
int MCtrlPanelAligner::checkPanelAlignerGetPosition(int* iMarkNo, int iVisionMarkPos, BOOL bUseManualSearch)
{
//@	MPos_XYT xytVisionPos = m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS);
	MPos_XYT xytVisionPos = m_plnkPanelAligner->GetXYTTargetPos(iVisionMarkPos);

	int iResult = checkTargetPosBySWLimit(xytVisionPos);
	if (ERR_CTRL_PANEL_ALIGNER_SUCCESS == iResult)
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;

//	m_plnkPanelAligner->DownPickUpUnit(TRUE);
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
	if (iResult)
		return iResult;
	int iPanelWorkNo = 0;
	if (iVisionMarkPos == DEF_PANEL_ALIGNER_SECOND_MARK_POS)
		iPanelWorkNo = 1;
//@	m_plnkPanelAligner->AirCVVacAllOn();
	m_plnkPanelAligner->AirCVVacAllOn(iPanelWorkNo);
//	Sleep(1000);
	Sleep(100);//KKY
	
	iResult = m_plnkPanelAligner->ReleasePanel();
	if (iResult)
		return iResult;
	
	iResult = m_plnkPanelAligner->Wait4DoneZ();
	if (iResult)
		return iResult;

//	iResult = m_plnkPanelAligner->DownPickUpUnit();
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);
	if (iResult)
		return iResult;
	
	iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(DEF_PANEL_ALIGNER_LOAD_POS);
	if (iResult)
		return iResult;

	Sleep(200);

	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if (iResult)
		return iResult;

	//	Sleep(500);	// Up ���� ��� �ð�.

	iResult = m_plnkPanelAligner->AbsorbPanel();
	if (iResult)
		return iResult;
	
	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if (iResult)
		return iResult;

	//20100823 sj_ysb
	iResult = m_plnkPanelAligner->AirCVVacAllOff();
	if (iResult)
		return iResult;

//	Sleep(1000);
	Sleep(100);//KKY

	// ���� Align Offset Clear �ϰ�, �ٽ� alignPanel ȣ��.
	m_plnkPanelAligner->InitAlignMarkOffset();
	
	iResult = alignPanel(iMarkNo, iVisionMarkPos, FALSE, bUseManualSearch);
	if(iResult)
		return iResult;
	return alignPanel(iMarkNo, iVisionMarkPos, FALSE, bUseManualSearch);
}

//int MCtrlPanelAligner::autoSearchFiduMark(int* rgiCamNo, int iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL* rgbResult)
int MCtrlPanelAligner::autoSearchFiduMark(int* rgiCamNo, int* iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL* rgbResult)
{
	// 1. Y �� �������� �����̸鼭 Mark 2�� ��� ã�� ��� -> ����
	// 2. Y �� �������� �����̸鼭 Mark 1���� ã�� ���
	//      -> ã�� ��ũ ��ġ ������ �ΰ�
	//		-> X �� �������� ã�� ����. (S/W (-) Limit ��� �ʿ�)

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [Start]");
	WriteTactTimeLog(szLogMsg);

	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	int i = 0;
	int j = 0;
	double dXOffset[4];
	double dYOffset[5];
	double dYRecognitionOffset = 0.0;

	BOOL rgbUse[2] = { TRUE, TRUE };
	BOOL rgbSuccess[2] = { FALSE, FALSE };
	MPos_XYT rgOffset[2];
	MPos_XYT rgRobot[2];
	MPos_XYT xytTarget;

	// PanelAligner ���� ��ġ �б�
	MPos_XYT xytCurrent = m_plnkPanelAligner->GetXYTCurrentPos();

	// Panel Ʋ���� ������ ���ؼ��� X �� �������� +/- ��ġ �̵��� �ʿ��ϴ�.
	dXOffset[0] = m_plnkSystemData->m_dAutoSearchPanelMarkDistance;
	dXOffset[1] = -m_plnkSystemData->m_dAutoSearchPanelMarkDistance;
	dXOffset[2] = m_plnkSystemData->m_dAutoSearchPanelMarkDistance * 2.0;
	dXOffset[3] = -m_plnkSystemData->m_dAutoSearchPanelMarkDistance * 2.0;
	
	// [���� Panel AutoSearch ���� Offset ����] + [Y �� ���� 4�� �̵� Point Offset ���]
	int iSign = (m_dLastAutoSearchOffset > 0.0) ? -1 : 1;

	if (TRUE == rgbResult[0] || TRUE == rgbResult[1])
		dYOffset[0] = 0.0;
	else
		dYOffset[0] = m_dLastAutoSearchOffset;

	dYOffset[1] = -m_plnkSystemData->m_dAutoSearchPanelMarkDistance * iSign;
	dYOffset[2] = -m_plnkSystemData->m_dAutoSearchPanelMarkDistance * iSign * 2.0;
	dYOffset[3] = m_plnkSystemData->m_dAutoSearchPanelMarkDistance * iSign;
	dYOffset[4] = m_plnkSystemData->m_dAutoSearchPanelMarkDistance * iSign * 2.0;

	IAxis* plnkX = m_plnkPanelAligner->GetMotionComponent(DEF_X);
	IAxis* plnkY = m_plnkPanelAligner->GetMotionComponent(DEF_Y);

	MAxisData axisData;
	plnkX->GetParameter(&axisData);
	double dX_MinusSWLimit = axisData.dCCWSWLimit;

	plnkY->GetParameter(&axisData);
	double dY_MinusSWLimit = axisData.dCCWSWLimit;
	double dY_PlusSWLimit = axisData.dCWSWLimit;

	// ��ġ �̵��ϸ鼭 Fiducial Mark �ν� �õ�.
	for (int i = 0; i < 5; i++)
	{
		if (0 != i && dYOffset[i] == dYOffset[0])
			continue;

		if (0 != i || dYOffset[0] != 0.0)
		{
			xytTarget = xytCurrent;
			xytTarget.dY += dYOffset[i];

			iResult = checkTargetPosBySWLimit(xytTarget);
			if (iResult)
				continue;
			
			iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytTarget, iVisionMarkPos, TRUE, TRUE);
			if (iResult)
				return iResult;
			
			Sleep(100);
		}

		iResult = searchFiduMark(rgiCamNo, iMarkNo, rgbUse, rgbSuccess, DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_1ST_STEP);
		if (iResult)
			return iResult;

		for (j = 0; j < 2; j++)
		{
			if (TRUE == rgbSuccess[j])
			{
				rgOffset[j].dX = 0.0;
				rgOffset[j].dY = dYOffset[i];
				rgOffset[j].dT = 0.0;
			}
		}

		// 1�� Auto Search ���� ���� Mark �� ��� ã�� ���.
		if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
		{
			if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
			{
				CString strResult;
				strResult.Format(_T("Success,Step1,Move1[,,%.3f,,]"), dYOffset[i]);
				m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), (LPCTSTR)strResult);
			}
			
			break;
		}
		else if (TRUE == rgbSuccess[0])	// 2�� �ʿ��� Mark ã��.
		{
			double dMarkX;
			double dMarkY;

			m_plnkVisionCalibration->ConvertV2R(rgiCamNo[0], NULL,
				m_plnkVision->GetSearchResultX(rgiCamNo[0], iMarkNo[0]), m_plnkVision->GetSearchResultY(rgiCamNo[0], iMarkNo[0]),
				dMarkX, dMarkY);

			MPos_XYT xytMovePos = m_plnkPanelAligner->GetXYTCurrentPos();
			xytMovePos.dY -= dMarkY;

			dYRecognitionOffset = dMarkY;

			iResult = checkTargetPosBySWLimit(xytMovePos);
			if (iResult)
			{
				xytMovePos.dY += dMarkY / 2.0;

				dYRecognitionOffset = dMarkY / 2.0;

				iResult = checkTargetPosBySWLimit(xytMovePos);
			}

			if (ERR_CTRL_PANEL_ALIGNER_SUCCESS == iResult)
			{
				iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytMovePos, iVisionMarkPos, TRUE, TRUE);
				if (iResult)
					return iResult;
			}
			else
				dYRecognitionOffset = 0.0;

			iResult = searchFiduMark(rgiCamNo, iMarkNo, rgbUse, rgbSuccess, DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_2ND_STEP);
			if (iResult)
				return iResult;

			if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
			{
				if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
				{
					CString strResult;
					strResult.Format(_T("Success,Step2,Move1[,,%.3f,%.3f,]"), dYOffset[i], dYRecognitionOffset);
					m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), (LPCTSTR)strResult);
				}
				break;
			}

			MPos_XYT xytNewCurrent = m_plnkPanelAligner->GetXYTCurrentPos();

			m_plnkVisionCalibration->ConvertV2R(rgiCamNo[0], NULL,
				m_plnkVision->GetSearchResultX(rgiCamNo[0], iMarkNo[0]), m_plnkVision->GetSearchResultY(rgiCamNo[0], iMarkNo[0]),
				pxytMark[0]->dX, pxytMark[0]->dY);
			
			pxytMark[0]->dT	= 0;
			
			pxytMark[0]->dY = pxytMark[0]->dY - (dYOffset[i] - dYRecognitionOffset);

			rgbUse[0] = FALSE;
			int k = 0;
			for ( ; k < 4; k++)
			{
				xytTarget = xytNewCurrent;
				xytTarget.dX += dXOffset[k];
				
				iResult = checkTargetPosBySWLimit(xytTarget);
				if (iResult)
					continue;
				
				iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytTarget, iVisionMarkPos, TRUE, TRUE);
				if (iResult)
					return iResult;
				
				Sleep(100);
				
				iResult = searchFiduMark(rgiCamNo, iMarkNo, rgbUse, rgbSuccess, DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_3RD_STEP);
				if (iResult)
					return iResult;

				if (TRUE == rgbSuccess[1])
				{
					rgOffset[1].dX = dXOffset[k];
					rgOffset[1].dY = dYOffset[i] - dYRecognitionOffset;
					rgOffset[1].dT = 0.0;

					m_plnkVisionCalibration->ConvertV2R(rgiCamNo[1], NULL,
						m_plnkVision->GetSearchResultX(rgiCamNo[1], iMarkNo[1]), m_plnkVision->GetSearchResultY(rgiCamNo[1], iMarkNo[1]),
						pxytMark[1]->dX, pxytMark[1]->dY);
					
					pxytMark[1]->dT	= 0;
					
					pxytMark[1]->dX = pxytMark[1]->dX - rgOffset[1].dX;
					pxytMark[1]->dY = pxytMark[1]->dY - rgOffset[1].dY;

					memset(szLogMsg, 0x00, sizeof(szLogMsg));
					sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Success]");
					WriteTactTimeLog(szLogMsg);
					
					if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
					{
						CString strResult;
						strResult.Format(_T("Success,Step3,Move1[,,%.3f,%.3f,],Move2[,%.3f,%.3f,]"), dYOffset[i], dYRecognitionOffset, rgOffset[1].dX, rgOffset[1].dY);
						m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), (LPCTSTR)strResult);
					}
					
					// Ʋ���� ���� m_dLastAutoSearchOffset ó�� ���Ѵ�.

					return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
				}
			}

			if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
				m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), _T("Failure,Step3"));
			
			// ���� ��ġ�� �̵���
			iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytCurrent, iVisionMarkPos, TRUE, TRUE);
			if (iResult)
				return iResult;
			
			memset(szLogMsg, 0x00, sizeof(szLogMsg));
			sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Failed]");
			WriteTactTimeLog(szLogMsg);

			// 310009 = Panel Mark �� �ν��� �� �����ϴ�. [PanelAlign Cam2 : Panel ����, �̹� Ȯ�� �ʿ�]
			return generateErrorCode(310009);
		}
		else if (TRUE == rgbSuccess[1])	// 1�� �ʿ��� Mark ã��.
		{
			double dMarkX;
			double dMarkY;

			m_plnkVisionCalibration->ConvertV2R(rgiCamNo[1], NULL,
				m_plnkVision->GetSearchResultX(rgiCamNo[1], iMarkNo[1]), m_plnkVision->GetSearchResultY(rgiCamNo[1], iMarkNo[1]),
				dMarkX, dMarkY);

			MPos_XYT xytMovePos = m_plnkPanelAligner->GetXYTCurrentPos();
			xytMovePos.dY -= dMarkY;

			dYRecognitionOffset = dMarkY;

			iResult = checkTargetPosBySWLimit(xytMovePos);
			if (iResult)
			{
				xytMovePos.dY += dMarkY / 2.0;

				dYRecognitionOffset = dMarkY / 2.0;

				iResult = checkTargetPosBySWLimit(xytMovePos);
			}

			if (ERR_CTRL_PANEL_ALIGNER_SUCCESS == iResult)
			{
				iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytMovePos, iVisionMarkPos, TRUE, TRUE);
				if (iResult)
					return iResult;
			}
			else
				dYRecognitionOffset = 0.0;

			iResult = searchFiduMark(rgiCamNo, iMarkNo, rgbUse, rgbSuccess, DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_2ND_STEP);
			if (iResult)
				return iResult;

			if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
			{
				if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
				{
					CString strResult;
					strResult.Format(_T("Success,Step2,Move1[,,%.3f,%.3f,]"), dYOffset[i], dYRecognitionOffset);
					m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), (LPCTSTR)strResult);
				}
				break;
			}

			MPos_XYT xytNewCurrent = m_plnkPanelAligner->GetXYTCurrentPos();

			m_plnkVisionCalibration->ConvertV2R(rgiCamNo[1], NULL,
				m_plnkVision->GetSearchResultX(rgiCamNo[1], iMarkNo[1]), m_plnkVision->GetSearchResultY(rgiCamNo[1], iMarkNo[1]),
				pxytMark[1]->dX, pxytMark[1]->dY);
			
			pxytMark[1]->dT	= 0;
			
			pxytMark[1]->dY = pxytMark[1]->dY - (dYOffset[i] - dYRecognitionOffset);

			rgbUse[1] = FALSE;
			int k = 0;
			for ( ; k < 4; k++)
			{
				xytTarget = xytNewCurrent;
				xytTarget.dX += dXOffset[k];

				iResult = checkTargetPosBySWLimit(xytTarget);
				if (iResult)
					continue;
				
				iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytTarget, iVisionMarkPos, TRUE, TRUE);
				if (iResult)
					return iResult;
				
				Sleep(100);
				
				iResult = searchFiduMark(rgiCamNo, iMarkNo, rgbUse, rgbSuccess, DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_3RD_STEP);
				if (iResult)
					return iResult;

				if (TRUE == rgbSuccess[0])
				{
					rgOffset[0].dX = dXOffset[k];
					rgOffset[0].dY = dYOffset[i] - dYRecognitionOffset;
					rgOffset[0].dT = 0.0;

					m_plnkVisionCalibration->ConvertV2R(rgiCamNo[0], NULL,
						m_plnkVision->GetSearchResultX(rgiCamNo[0], iMarkNo[0]), m_plnkVision->GetSearchResultY(rgiCamNo[0], iMarkNo[0]),
						pxytMark[0]->dX, pxytMark[0]->dY);
					
					pxytMark[0]->dT	= 0;
					
					pxytMark[0]->dX = pxytMark[0]->dX - rgOffset[0].dX;
					pxytMark[0]->dY = pxytMark[0]->dY - rgOffset[0].dY;

					if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
					{
						CString strResult;
						strResult.Format(_T("Success,Step3,Move1[,%.3f,%.3f,%.3f,],Move2[,,%.3f,]"), rgOffset[0].dX, rgOffset[0].dY, dYRecognitionOffset, dYOffset[i]);
						m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), (LPCTSTR)strResult);
					}

					memset(szLogMsg, 0x00, sizeof(szLogMsg));
					sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Success]");
					WriteTactTimeLog(szLogMsg);
					
					// Ʋ���� ���� m_dLastAutoSearchOffset ó�� ���Ѵ�.

					return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
				}
			}

			if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
				m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), _T("Failure,Step3"));
			
			// ���� ��ġ�� �̵���
			iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytCurrent, iVisionMarkPos, TRUE, TRUE);
			if (iResult)
				return iResult;
			
			memset(szLogMsg, 0x00, sizeof(szLogMsg));
			sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Failed]");
			WriteTactTimeLog(szLogMsg);

			// 310008 = Panel Mark �� �ν��� �� �����ϴ�. [PanelAlign Cam1 : Panel ����, �̹� Ȯ�� �ʿ�]
			return generateErrorCode(310008);
		}
	}

	if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
	{
		for (j = 0; j < 2; j++)
		{
			m_plnkVisionCalibration->ConvertV2R(rgiCamNo[j], NULL,
				m_plnkVision->GetSearchResultX(rgiCamNo[j], iMarkNo[j]), m_plnkVision->GetSearchResultY(rgiCamNo[j], iMarkNo[j]),
				pxytMark[j]->dX, pxytMark[j]->dY);
			
			pxytMark[j]->dT	= 0;
			
			pxytMark[j]->dY = pxytMark[j]->dY - (dYOffset[i] - dYRecognitionOffset);
		}
		
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Success]");
		WriteTactTimeLog(szLogMsg);
		
		m_dLastAutoSearchOffset = dYOffset[i];
		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	}
	else	// Mark �� �ϳ��� ã�� ���� ��쿡 ���� ó��.
	{
		int k = 0;
		
		// X �� �̵�.
		for (int i = 0; i < 4; i++)
		{
			xytTarget = xytCurrent;	// ������ ���� ��ġ.
			xytTarget.dX += dXOffset[i];
			
			for (j = 0; j < 5; j++)
			{
				if (0 != j)
					xytTarget.dY = xytCurrent.dY + dYOffset[j];
				
				iResult = checkTargetPosBySWLimit(xytTarget);
				if (iResult)
					continue;

				iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytTarget, iVisionMarkPos, TRUE, TRUE);
				if (iResult)
					return iResult;
				
				Sleep(100);
				
				//������������������������������������������������������������
				// Vision Grab.
				m_plnkVision->LockCamera(rgiCamNo[0]);
				m_plnkVision->LockCamera(rgiCamNo[1]);
				
				m_plnkVision->ClearOverlay(rgiCamNo[0]);
				m_plnkVision->ClearOverlay(rgiCamNo[1]);
				
				int iResult = m_plnkVision->Grab(rgiCamNo[0]);
				if (iResult)
				{
					m_plnkVision->UnlockCamera(rgiCamNo[0]);
					m_plnkVision->UnlockCamera(rgiCamNo[1]);
					return iResult;
				}
				
				iResult = m_plnkVision->Grab(rgiCamNo[1]);
				if (iResult)
				{
					m_plnkVision->UnlockCamera(rgiCamNo[0]);
					m_plnkVision->UnlockCamera(rgiCamNo[1]);
					return iResult;
				}
				
				//������������������������������������������������������������
				// Vision �ν�.
				for (k = 0; k < 2; k++)
				{
					if (TRUE == rgbSuccess[k])
						continue;
					
					iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[k], iMarkNo[k]);
					if (iResult)	// Mark �ν� Retry ����.
					{
						Sleep(200);
						iResult = m_plnkVision->Grab(rgiCamNo[k]);
						iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[k], iMarkNo[k]);
					}
					
					if (iResult)
					{
						// Error Image ����.
						CString strMsg;
						strMsg.Format(_T("PA cam%d (AS4)"), k+1);
						m_plnkVision->SaveImage(rgiCamNo[k], (LPCTSTR)strMsg);
					}
					else
					{
						rgbSuccess[k] = TRUE;
						rgOffset[k].dX = dXOffset[i];

						if (j != 0)
							rgOffset[k].dY = dYOffset[j];

						rgOffset[k].dT = 0.0;

						m_plnkVisionCalibration->ConvertV2R(rgiCamNo[k], NULL,
							m_plnkVision->GetSearchResultX(rgiCamNo[k], iMarkNo[k]), m_plnkVision->GetSearchResultY(rgiCamNo[k], iMarkNo[k]),
							rgRobot[k].dX, rgRobot[k].dY);
						
						rgRobot[k].dT = 0;
					}
				}
				
				m_plnkVision->UnlockCamera(rgiCamNo[0]);
				m_plnkVision->UnlockCamera(rgiCamNo[1]);

				if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
					break;
			}

			if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
					break;
		}

		if (TRUE == rgbSuccess[0] && TRUE == rgbSuccess[1])
		{
			for (k = 0; k < 2; k++)
			{
				pxytMark[k]->dX = rgRobot[k].dX - rgOffset[k].dX;
				pxytMark[k]->dY = rgRobot[k].dY - rgOffset[k].dY;
				pxytMark[k]->dT = 0.0;
			}

			memset(szLogMsg, 0x00, sizeof(szLogMsg));
			sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Success]");
			WriteTactTimeLog(szLogMsg);
			
			if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
			{
				CString strResult;
				strResult.Format(_T("Success,Step4,Move1[,%.3f,%.3f,,],Move2[,%.3f,%.3f,]"), rgOffset[0].dX, rgOffset[0].dY, rgOffset[1].dX, rgOffset[1].dY);
				m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), (LPCTSTR)strResult);
			}
			
			// Ʋ���� ���� m_dLastAutoSearchOffset ó�� ���Ѵ�.
			
			return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
		}
	}

	if (TRUE == m_plnkSystemData->m_bLogLevelMonitoring)
		m_plnkVision->WriteLog(_T("..\\Log\\autosearch_log.csv"), _T("Failure,Case5"));

	// ���� ��ġ�� �̵���
	iResult = m_plnkPanelAligner->SyncMoveXYTPos(xytCurrent, iVisionMarkPos, TRUE, TRUE);
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [autoSearch FiduMark] [End Failed]");
	WriteTactTimeLog(szLogMsg);

	// 310006 = Panel Mark Auto Search�� �����߽��ϴ�.
	return generateErrorCode(310006);
}

/* ���� ����.
int	MCtrlPanelAligner::getFiduMarkPositionByManual(int iCameraNo, int iMarkNo, int iPosIndex, MPos_XYT *pResult)
{
	int	iResult;
	MPos_XYT sCur1Pos;

	if (m_uiMessage == NULL)	
		return	generateErrorCode(310013);	//Window Message�� NULL �Դϴ�.

	// Jog�� ã�� �õ�
	// Jog �� Vision ȭ��ȿ� ������ �Ѵ�.
	m_iUserJogMode = DEF_ALIGNPANEL_USER_WAIT;

	// Jog ��� �� ��ġ ���
	MPos_XYT sCurrentPos = m_plnkPanelAligner->GetXYTCurrentPos();

	// Main Frame���� Vision Dialog�� ó���ϵ��� �޼����� �����Ѵ�.
	::SendMessage(AfxGetMainWnd()->m_hWnd, m_uiMessage, iCameraNo, DEF_ALIGNPANEL_JOG_MODE);
	Sleep(100);

	// ����� ���� ��ٸ�
	while (m_iUserJogMode == DEF_ALIGNPANEL_USER_WAIT) 
	{
		Sleep(10);
	}

	// Jog �̵� �� �ν� ��� -> �ν� �õ� �� ���и� ����� ���� �ܰ�� �̵�
	if (m_iUserJogMode == DEF_ALIGNPANEL_USER_RECOG)
	{
		m_iUserJogMode = DEF_ALIGNPANEL_USER_WAIT;

		// Jog ��� �� ��ġ ���
		sCur1Pos = m_plnkPanelAligner->GetXYTCurrentPos();

		// �ν� �õ�
		m_plnkVision->ClearOverlay(iCameraNo);
		iResult = m_plnkVision->Grab(iCameraNo);
		if (iResult)
			return iResult;

		iResult = m_plnkVision->RecognitionPatternMark(iCameraNo, iMarkNo);
		// �ν� �õ� - �����̸�
		if (iResult == ERR_VISION_SUCCESS)
		{
			m_plnkVisionCalibration->ConvertV2R(iCameraNo,
												NULL,
												m_plnkVision->GetSearchResultX(iCameraNo, iMarkNo),
												m_plnkVision->GetSearchResultY(iCameraNo, iMarkNo),
												pResult->dX,
												pResult->dY);

			pResult->Init(	sCurrentPos.dX - sCur1Pos.dX + pResult->dX,
							sCurrentPos.dY - sCur1Pos.dY + pResult->dY,
							sCurrentPos.dT - sCur1Pos.dT);	//sResult.m_dRobotT);
			return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
		}
	}

	// Jog �ν� ������ ���
	m_iUserJogMode = DEF_ALIGNPANEL_USER_WAIT;

	// ����� ���� ��ٸ���
	m_iUserDirection = DEF_ALIGNPANEL_USER_WAIT;

	// Jog ��� �� ��ġ�� �̵�
	iResult	= m_plnkPanelAligner->SafeSyncMoveXYTPos(iPosIndex);
	if (iResult)
		return iResult;

	// Jog ��� �� ��ġ ���
	sCurrentPos = m_plnkPanelAligner->GetXYTCurrentPos();

	// Main Frame���� Vision Dialog�� ó���ϵ��� �޼����� �����Ѵ�.
	::SendMessage(AfxGetMainWnd()->m_hWnd, m_uiMessage, iCameraNo, DEF_ALIGNPANEL_USER_MODE);
	Sleep(100);

	// ����� ���� ��ٸ�
	while (m_iUserDirection == DEF_ALIGNPANEL_USER_WAIT) 
	{
		Sleep(10);
	}

	if (m_iUserDirection == DEF_ALIGNPANEL_USER_RECOG)
	{
		m_iUserDirection = DEF_ALIGNPANEL_USER_WAIT;

		// Jog ��� �� ��ġ ���
		sCur1Pos = m_plnkPanelAligner->GetXYTCurrentPos();

		// ���� PanelAligner�� ��ġ�� �ν� ��ġ�� �����
		pResult->Init(	sCurrentPos.dX - sCur1Pos.dX,
						sCurrentPos.dY - sCur1Pos.dY,
						sCurrentPos.dT - sCur1Pos.dT);
//		*piMatchRatio = (int)99.0;

		return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	}
	return generateErrorCode(310014);	//Manual Align�� �����߽��ϴ�.
}
*/

//int MCtrlPanelAligner::alignPanel(int iMarkNo, int iVisionMarkPos, BOOL bUseAutoSearch)
int MCtrlPanelAligner::alignPanel(int iMarkNo[2], int iVisionMarkPos, BOOL bUseAutoSearch, BOOL bUseManualSearch)
{
	MPos_XYT MarkPosition1;			// ù��° Mark PanelAligner �߽ɿ��������� ��ǥ
	MPos_XYT MarkPosition2;			// �ι�° Mark PanelAligner �߽ɿ��������� ��ǥ
	MPos_XYT ActualAlignPos;		// ���� Align Motion ��ǥ

	calculateFiduMarkPosition(MarkPosition1, MarkPosition2);

	double dMarkDist = 0.0;
	if (FALSE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_MIRROR_
		dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Yr;
#	else
		dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Yl;
#	endif
#else
		dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Xu;
#endif
	}
	else
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		dMarkDist = MarkPosition2.dY - MarkPosition1.dY;
//@#else
		dMarkDist = MarkPosition2.dX - MarkPosition1.dX;
//@#endif
	}

	// Mark �ν� ��� ���� Air Conveyor Panel ȸ�� �߽�(Vacuum Pad2 Center)������ �Ÿ��� ���ϱ� ����  Mark�� ��ġ�� ����Ѵ�. 

	// Align�� �ȵǾ��� ��� Fidu Mark ��ġ�� Offset�� ���� ����ؾ� �Ѵ�.
	// Align ��ġ�� Offset�� �ٸ� ��ġ�� ������ ��ġ�� ���� ���� �ϰ��� ���̴�.
	// �ڵ� Ȯ�� �ʿ� : Offset �� �ϴ� �ݿ� ����.

	// PanelAligner���� Offset ��ǥ�� �о�´�.
	// (Align ��ġ�� Offset ���� �ݿ��ϱ� ���ؼ� �ʿ���)
	MPos_XYT sPanelAlignerXYTOffsetPos[DEF_PANEL_ALIGNER_MAX_POSITION];
	m_plnkPanelAligner->GetXYTPosParameter(NULL, sPanelAlignerXYTOffsetPos, NULL);

	MarkPosition1 = MarkPosition1 - sPanelAlignerXYTOffsetPos[iVisionMarkPos];
	MarkPosition2 = MarkPosition2 - sPanelAlignerXYTOffsetPos[iVisionMarkPos];

//____________________________________________________________
//
// Step 1. 1st Mark �ν� ��ġ�� �̵��ϱ�
//____________________________________________________________

	int iResult	= ERR_CTRL_PANEL_ALIGNER_SUCCESS;
	
//	if (FALSE == m_plnkPanelAligner->IsInXYTPos(iVisionMarkPos))
	{
		iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(iVisionMarkPos);
		if (iResult)
			return iResult;
	}

	// ù��° Align�ϴ� ��ǥ ����
	ActualAlignPos	= m_plnkPanelAligner->GetXYTCurrentPos();

//____________________________________________________________
//
// Step 2. Fiducial Mark �ν�
//____________________________________________________________
	int rgiCamNo[2] = { DEF_PANEL_ALIGN_CAMERA_1, DEF_PANEL_ALIGN_CAMERA_2 };
	
	if (TRUE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
		/*/
#ifdef DEF_SOURCE_SYSTEM
		MInspectCarrierInfo	FirstCarrierInfo = m_plnkWorkSchedule[0]->GetFirstWorkTabCarrierInfo_FromFirstWorkList();
		MInspectCarrierInfo LastCarrierInfo = m_plnkWorkSchedule[0]->GetLastWorkTabCarrierInfo_FromFirstWorkList();	
		int iCarrierNo[2] = {FirstCarrierInfo.m_nCarrierNo, LastCarrierInfo.m_nCarrierNo};
		for (int i=0; i<2; i++)
			rgiCamNo[i] = m_plnkStateCameraCarrier[iCarrierNo[i]]->GetCameraNo(i);
#else
		/*/
		int iAlignGroup = MOlbSystemPre.GetPanelAlignGroup();
		MInspectCarrierInfo	FirstCarrierInfo = m_plnkWorkSchedule[iAlignGroup]->GetFirstWorkTabCarrierInfo_FromFirstWorkList();
		MInspectCarrierInfo LastCarrierInfo = m_plnkWorkSchedule[iAlignGroup]->GetLastWorkTabCarrierInfo_FromFirstWorkList();	
		int iCarrierNo[2] = {FirstCarrierInfo.m_nCarrierNo, LastCarrierInfo.m_nCarrierNo};
		for (int i=0; i<2; i++)
		{
			if (iAlignGroup == DEF_FRONT_GROUP)
				rgiCamNo[i] = m_plnkStateCameraCarrier[iCarrierNo[i]]->GetCameraNo(i);
			else
				rgiCamNo[i] = m_plnkStateRCameraCarrier[iCarrierNo[i]]->GetCameraNo(i);
		}
//@#endif
	}
	
	// Dry Run Mode������ ��������� �۾���
	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)	
	{
		m_plnkPanelAligner->SetAlignMarkOffset(MPos_XYT(0.0, 0.0, 0.0), ActualAlignPos.dT);	// PanelAligner�� Align ������� ���� �Ѵ�.
		return iResult;
	}

	m_plnkLighting[s_rgLightIdForCam[rgiCamNo[0]].x]->LightOn(); //171002 JSH
	m_plnkLighting[s_rgLightIdForCam[rgiCamNo[1]].x]->LightOn(); //171002 JSH

	Sleep(100);

	MPos_XYT posMark1;
	MPos_XYT posMark2;
	MPos_XYT* xytMark[2] = { &posMark1, &posMark2 };
	iResult = getFiduMarkPosition(rgiCamNo, iMarkNo, iVisionMarkPos, xytMark, bUseAutoSearch, bUseManualSearch);
	if (iResult)
	{
		m_plnkLighting[s_rgLightIdForCam[rgiCamNo[0]].x]->LightOff(); //171002 JSH
		m_plnkLighting[s_rgLightIdForCam[rgiCamNo[1]].x]->LightOff(); //171002 JSH
		return iResult;
	}

//#ifdef DEF_GATE_SYSTEM
	if (TRUE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
		if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			posMark1.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
			posMark2.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
		}
		else if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			posMark1.dY += DEF_TABMARK_VISION_CENTER_OFFSET;
			posMark2.dY += DEF_TABMARK_VISION_CENTER_OFFSET;
		}
	#else
		if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			posMark1.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
			posMark2.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
		}
		else if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			posMark1.dY += DEF_TABMARK_VISION_CENTER_OFFSET;
			posMark2.dY += DEF_TABMARK_VISION_CENTER_OFFSET;
		}
	#endif
#else
//		#pragma message(__LOC__ "MCtrlPanelAligner::alignPanel() ���� ��ȣ Ȯ�ο�")
		posMark1.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
		posMark2.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
#endif
	}
//@#endif

	posMark1 = posMark1 + MarkPosition1;
	posMark2 = posMark2 + MarkPosition2;

//____________________________________________________________
//
// Step 3. Align Offset T ��� �ϱ�
//____________________________________________________________

	// Radian ������ Ʋ���� ���� ���
//	double dT = asin((posMark2.dX - posMark1.dX) / m_plnkPanelData->m_dDistance_FiduMark_Xu);
	double dT = 0.0;
//@#ifdef DEF_SOURCE_SYSTEM
//@	dT = asin((posMark2.dX - posMark1.dX) / dMarkDist);
//@#else
	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		dT = asin((posMark2.dY - posMark1.dY) / dMarkDist);
	else
		dT = asin((posMark2.dX - posMark1.dX) / dMarkDist);
//@#endif
	// Radian => Degree ��ȯ
	double dDegreeT = RAD2DEG(dT);
	
	CString Tolerance;
	Tolerance.Format(_T("��ġ����, posMark1.dX:%.4f, posMark1.dY:%.4f, posMark2.dX:%.4f, posMark2.dY%.4f"),
		posMark1.dX,
		posMark1.dY,
		posMark2.dX,
		posMark2.dY);
	WriteNormalLog(Tolerance);
	
//____________________________________________________________
//
// Step 4. Align Offset ��ü ���.
//____________________________________________________________

	/*/
#ifdef DEF_SOURCE_SYSTEM
	double dROffsetX = cos(dT) * (posMark1.dX) - sin(dT) * (posMark1.dY);
//	double dROffsetY = sin(dT) * (posMark1.dX) + cos(dT) * (posMark1.dY);
	double dROffsetY = -sin(dT) * (posMark1.dX) + cos(dT) * (posMark1.dY);
#else
	/*/
	double dROffsetX = cos(dT) * (posMark1.dX) - sin(dT) * (posMark1.dY);
	double dROffsetY = sin(dT) * (posMark1.dX) + cos(dT) * (posMark1.dY);
//@#endif

	MPos_XYT xytAlign;
	xytAlign.dX = MarkPosition1.dX - dROffsetX;
	xytAlign.dY = MarkPosition1.dY - dROffsetY;
	xytAlign.dT = dDegreeT;

	double dMarkX;
	dMarkX = MarkPosition2.dX - MarkPosition1.dX;

//____________________________________________________________
//
// Step 5. Panel Align Offset ����
//____________________________________________________________

	// T ��ǥ ���� ���� üũ, �ʹ� ���� ���� ���� �ȵȴ�.	
	if (fabs(xytAlign.dT) > DEF_ALIGN_T_TOLERANCE
		|| fabs(xytAlign.dX) > DEF_ALIGN_XY_TOLERANCE
		|| fabs(xytAlign.dY) > DEF_ALIGN_XY_TOLERANCE * 1.5)
	{
		// 310007 = Align �������� ���ġ�� �ʰ��߽��ϴ�. (X,Y ���Ѱ� : 10mm �̳� , T ���Ѱ� : 5��)
		m_plnkLighting[s_rgLightIdForCam[rgiCamNo[0]].x]->LightOff(); //171002 JSH
		m_plnkLighting[s_rgLightIdForCam[rgiCamNo[1]].x]->LightOff(); //171002 JSH
		return generateErrorCode(310007);
	}

	// Align ����� Mechanical Layer�� PanelAligner Component�� �����Ѵ�.
	MPos_XYT PriorAlignOffset = m_plnkPanelAligner->GetAlignMarkOffset();	// PanelAligner�� ����� Align�� �о� �´�.
	MPos_XYT AlignResult = xytAlign + PriorAlignOffset;									// Align �������� ���� �Ѵ�.
	m_plnkPanelAligner->SetAlignMarkOffset(AlignResult, ActualAlignPos.dT);	// PanelAligner�� Align ������� ���� �Ѵ�.
	
	m_plnkLighting[s_rgLightIdForCam[rgiCamNo[0]].x]->LightOff(); //171002 JSH
	m_plnkLighting[s_rgLightIdForCam[rgiCamNo[1]].x]->LightOff(); //171002 JSH
	
	if(bUseManualSearch == FALSE)
	{
		if(fabs(xytAlign.dX) > m_plnkSystemData->m_dPanelAlignTolerance ||
			fabs(xytAlign.dY) > m_plnkSystemData->m_dPanelAlignTolerance ||
			fabs(xytAlign.dT) > 0.01
			)
		{
			return 1;
		}
	}
	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

const BOOL MCtrlPanelAligner::CheckFiduMarkPos()
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		return TRUE;
	}

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Check FiduMark Pos] [Start]");
	WriteTactTimeLog(szLogMsg);

	MPos_XYT posResult1(-99.9, -99.9, -99.9);
	MPos_XYT posResult2(-99.9, -99.9, -99.9);

	int rgiCamNo[2] = {DEF_PANEL_ALIGN_CAMERA_1, DEF_PANEL_ALIGN_CAMERA_2};
	if (TRUE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
		/*/
#ifdef DEF_SOURCE_SYSTEM
		MInspectCarrierInfo	FirstCarrierInfo = m_plnkWorkSchedule[0]->GetFirstWorkTabCarrierInfo_FromFirstWorkList();
		MInspectCarrierInfo LastCarrierInfo = m_plnkWorkSchedule[0]->GetLastWorkTabCarrierInfo_FromFirstWorkList();	
		int iCarrierNo[2] = {FirstCarrierInfo.m_nCarrierNo, LastCarrierInfo.m_nCarrierNo};
		for (int i=0; i<2; i++)
			rgiCamNo[i] = m_plnkStateCameraCarrier[iCarrierNo[i]]->GetCameraNo(i);
#else
		/*/
		int iAlignGroup = MOlbSystemPre.GetPanelAlignGroup();
		MInspectCarrierInfo	FirstCarrierInfo = m_plnkWorkSchedule[iAlignGroup]->GetFirstWorkTabCarrierInfo_FromFirstWorkList();
		MInspectCarrierInfo LastCarrierInfo = m_plnkWorkSchedule[iAlignGroup]->GetLastWorkTabCarrierInfo_FromFirstWorkList();	
		int iCarrierNo[2] = {FirstCarrierInfo.m_nCarrierNo, LastCarrierInfo.m_nCarrierNo};
		for (int i=0; i<2; i++)
		{
			if (iAlignGroup == DEF_FRONT_GROUP)
				rgiCamNo[i] = m_plnkStateCameraCarrier[iCarrierNo[i]]->GetCameraNo(i);
			else
				rgiCamNo[i] = m_plnkStateRCameraCarrier[iCarrierNo[i]]->GetCameraNo(i);
		}
//@#endif
	}

	m_plnkVision->LockCamera(rgiCamNo[0]);
	m_plnkVision->LockCamera(rgiCamNo[1]);

	m_plnkVision->ClearOverlay(rgiCamNo[0]);
	m_plnkVision->ClearOverlay(rgiCamNo[1]);
	
	int iResult = m_plnkVision->Grab(rgiCamNo[0]);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(rgiCamNo[0]);
		m_plnkVision->UnlockCamera(rgiCamNo[1]);
		return iResult;
	}

	iResult = m_plnkVision->Grab(rgiCamNo[1]);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(rgiCamNo[0]);
		m_plnkVision->UnlockCamera(rgiCamNo[1]);
		return iResult;
	}

	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[0], DEF_FIDU_SUB_MARK);
	else
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[0], DEF_FIDU_MARK);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(rgiCamNo[0]);
		m_plnkVision->UnlockCamera(rgiCamNo[1]);

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Check FiduMark Pos] [End Failed]");
		WriteTactTimeLog(szLogMsg);

		return FALSE;
	}

	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		m_plnkVisionCalibration->ConvertV2R(rgiCamNo[0], NULL,
			m_plnkVision->GetSearchResultX(rgiCamNo[0], DEF_FIDU_SUB_MARK),
			m_plnkVision->GetSearchResultY(rgiCamNo[0], DEF_FIDU_SUB_MARK),
			posResult1.dX, posResult1.dY);
	else
		m_plnkVisionCalibration->ConvertV2R(rgiCamNo[0], NULL,
			m_plnkVision->GetSearchResultX(rgiCamNo[0], DEF_FIDU_MARK),
			m_plnkVision->GetSearchResultY(rgiCamNo[0], DEF_FIDU_MARK),
			posResult1.dX, posResult1.dY);
	posResult1.dT = 0;
	
	m_plnkVision->UnlockCamera(rgiCamNo[0]);
	
	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[1], DEF_FIDU_SUB_MARK);
	else
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[1], DEF_FIDU_MARK);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(rgiCamNo[1]);

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Check FiduMark Pos] [End Failed]");
		WriteTactTimeLog(szLogMsg);

		return FALSE;
	}

	if (m_plnkPreBonderData->m_bUseTabPanelAlign)
		m_plnkVisionCalibration->ConvertV2R(rgiCamNo[1], NULL,
			m_plnkVision->GetSearchResultX(rgiCamNo[1], DEF_FIDU_SUB_MARK),
			m_plnkVision->GetSearchResultY(rgiCamNo[1], DEF_FIDU_SUB_MARK),
			posResult2.dX, posResult2.dY);
	else
		m_plnkVisionCalibration->ConvertV2R(rgiCamNo[1], NULL,
			m_plnkVision->GetSearchResultX(rgiCamNo[1], DEF_FIDU_MARK),
			m_plnkVision->GetSearchResultY(rgiCamNo[1], DEF_FIDU_MARK),
			posResult2.dX, posResult2.dY);
	posResult2.dT = 0;

	m_plnkVision->UnlockCamera(rgiCamNo[1]);

#ifdef DEF_GATE_SYSTEM
	if (TRUE == m_plnkPreBonderData->m_bUseTabPanelAlign)
	{
		if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			posResult1.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
			posResult2.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
		}
		else if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			posResult1.dY += DEF_TABMARK_VISION_CENTER_OFFSET;
			posResult2.dY += DEF_TABMARK_VISION_CENTER_OFFSET;
		}
#	ifdef DEF_MIRROR_
		posResult1.dY *= -1.0;
		posResult1.dY *= -1.0;
#	endif
	}
#else
//	#pragma message(__LOC__ "MCtrlPanelAligner::CheckFiduMarkPos() ���� ��ȣ Ȯ�ο�")
//@	posResult1.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
//@	posResult2.dY -= DEF_TABMARK_VISION_CENTER_OFFSET;
#endif

	if (fabs(posResult1.dX) < m_plnkSystemData->m_dPanelAlignTolerance
		&& fabs(posResult1.dY) < m_plnkSystemData->m_dPanelAlignTolerance
		&& fabs(posResult2.dX) < m_plnkSystemData->m_dPanelAlignTolerance)
	{
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Check FiduMark Pos] [End Success]");
		WriteTactTimeLog(szLogMsg);

		return TRUE;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCtrlPanelAligner] [C] [Check FiduMark Pos] [End Failed]");
	WriteTactTimeLog(szLogMsg);
	TRACE(_T("Align ����!! \n"));
	return FALSE;
}

void MCtrlPanelAligner::CheckPlateBlowOn()
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		return;
	}

	BOOL bBlowOnCheck = m_plnkPanelAligner->IsAirCVBlowOn();

	m_plnkPanelAligner->AirCVBlowOn();	// Plate Vacuum Off & Blow On.

	MTickTimer timerLimit;
	timerLimit.StartTimer();
	while (FALSE == m_plnkPanelAligner->IsAirCVBlowOn())
	{
		Sleep(10);
		
		if (TRUE == timerLimit.MoreThan(1.0))
			break;
	}

	//141113 SJ_YSH
	timerLimit.StartTimer();
	while (FALSE == m_plnkPanelAligner->IsAirCVVacAllOff())
	{
		Sleep(10);
		
		if (TRUE == timerLimit.MoreThan(1.0))
			break;
	}
	//_____________

	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		TRACE1("Plate Vacuum Release Time : %.1lf\n", timerLimit.GetElapseTime());
	}

	if (FALSE == bBlowOnCheck && m_plnkSystemData->m_dAirPlateBlowOnDelay > 0.0)
		Sleep((DWORD)m_plnkSystemData->m_dAirPlateBlowOnDelay * 1000);
}

//1.PanelAligner�� Panel������ ���·� Align��ġ�� �̵��ϰ�
//2.�λ��� ����, Aligner���� ����, �ϰ��ϰ�
//3.Rotator�Ǹ����� Panel�߽���ġ�� �̵��ϰ�
//4.Rotator�Ǹ����� ���/����
int MCtrlPanelAligner::TransferPanel_AlignerToRotator()
{
	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	/*/
	//PanelAligner�� Panel������ ���·� Align��ġ �Ǵ� Inspection��ġ���� �Ѵ�
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		if (FALSE == m_plnkPanelAligner->IsPanelAbsorbed())
		{
			SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
			return generateErrorCode(310014);			
		}
	}

	if (FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS)
//		&& FALSE== m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS)
		)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return generateErrorCode(310013);
	}

	//Air�λ�
	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAligner Z�� Move��ġ�� �̵�
	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAlign ��ġ�� �̵�
	iResult = MovetoAlignPos();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Air�λ󿡼� Panel����
	iResult = m_plnkPanelAligner->AirCVVacAllOn();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAligner Panel��������
	iResult = m_plnkPanelAligner->ReleasePanel();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAligner Z�� �ϰ�
	iResult = m_plnkPanelAligner->DownPickUpUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Rotator �ϰ��� Return
	iResult = m_plnkPanelAligner->DownRotatorUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}

//	iResult = m_plnkPanelAligner->ReturnRotatorUnit();
//	if (iResult)
//	{
//		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
//		return iResult;
//	}

	//Rotator���� ��ġ�� �̵�
	iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(DEF_PANEL_ALIGNER_TURN_POS);
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Rotator ���
	iResult = m_plnkPanelAligner->UpRotatorUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Rotator Panel ����
	iResult = m_plnkPanelAligner->AbsorbPanelRotator();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	/*/

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

//1.Rotator�� Panel������ ����
//2.�λ��� ����, Rotator���� ����, �ϰ��ϰ�
//3.Aligner�� PanelAligne��ġ�� �̵��ϰ�
//4.Aligner�� ���/����
int MCtrlPanelAligner::TransferPanel_RotatorToAligner()
{
	/*/
	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;
//	//Rotator�� Panel������ ���·� Panel�߽� ��ġ���� �Ѵ�
//	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
//	{
//		if (FALSE == m_plnkPanelAligner->IsPanelAbsorbedRotator())
//		{
//			SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
//			return generateErrorCode(310014);			
//		}
//	}

	//Air�λ󿡼� Panel����
	iResult = m_plnkPanelAligner->AirCVVacAllOn();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Rotator Panel��������
	iResult = m_plnkPanelAligner->ReleasePanelRotator();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Rotator �ϰ�
	iResult = m_plnkPanelAligner->DownRotatorUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAligner Z�� �ϰ�
	iResult = m_plnkPanelAligner->DownPickUpUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}

//	//PanelAlign ��ġ�� �̵�
//	m_plnkPanelAligner->InitAlignMarkOffset();

	iResult = MovetoAlignPos();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAligner Z�� Move��ġ�� �̵�
	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//PanelAligner Panel����
	iResult = m_plnkPanelAligner->AbsorbPanel();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	//Air�λ󿡼� Panel����
	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER TransferPanel_AlignerToRotator."),1, __FILE__, __LINE__);
		return iResult;
	}
	/*/

	return ERR_CTRL_PANEL_ALIGNER_SUCCESS;
}

//20111020 by kss
/////////////////////////////////////////////////////////
BOOL MCtrlPanelAligner::WriteAPCPreLog(CString strMsg)
{	
	FILE* fsource;
	CFileFind fn;
	CString fname;

	CTime tmCurr = CTime::GetCurrentTime();
	strMsg = tmCurr.Format(_T("[%m/%d-%H:%M:%S]")) + strMsg;
	fname = _T("..\\Log\\APCPreLog") + tmCurr.Format(_T("%m%d")) + _T(".txt");
	
	//LKH_20090130 : Log Unicode Writing
	if ((fsource = _wfopen(fname, _T("ab+,ccs=UNICODE"))) == NULL)
	{
		return FALSE;
	}
	//���� �߻� �α� ���� ���
	fwprintf(fsource, _T("%s\r\n"), strMsg.GetBuffer());
	fclose(fsource);

	return TRUE;
}
/////////////////////////////////////////////////////////
//SJ_YYK 150318 Add..
int MCtrlPanelAligner::UpdateAlignData()
{
	int iResult = ERR_CTRL_PANEL_ALIGNER_SUCCESS;

	int i;

	if(m_plnkPanelData->m_bUseFlag_Xu == TRUE)
	{
		for(i=0; i<m_plnkTabData->m_iXuTabQty; i++)
		{
			m_plnkTabOffset->m_rgdXuTabOffsetTabMounter[i] += m_plnkAutoAlignData->XuTabOffset_X[i];
			m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabMounter[i] += m_plnkAutoAlignData->XuTabOffset_X[i];

			m_plnkTabOffset->m_rgdXuTabOffsetTabAligner[i] += m_plnkAutoAlignData->XuTabOffset_Y[i];
			m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAligner[i] += m_plnkAutoAlignData->XuTabOffset_Y[i];

			m_plnkTabOffset->m_rgdXuTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->XuTabOffset_T[i];
			m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->XuTabOffset_T[i];
		}
	}

	if(m_plnkPanelData->m_bUseFlag_Xd == TRUE)
	{
		for(i=0; i<m_plnkTabData->m_iXdTabQty; i++)
		{
			m_plnkTabOffset->m_rgdXdTabOffsetTabMounter[i] += m_plnkAutoAlignData->XdTabOffset_X[i];
			m_plnkTabOffset_Rear->m_rgdXdTabOffsetTabMounter[i] += m_plnkAutoAlignData->XdTabOffset_X[i];

			m_plnkTabOffset->m_rgdXdTabOffsetTabAligner[i] += m_plnkAutoAlignData->XdTabOffset_Y[i];
			m_plnkTabOffset_Rear->m_rgdXdTabOffsetTabAligner[i] += m_plnkAutoAlignData->XdTabOffset_Y[i];

			m_plnkTabOffset->m_rgdXdTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->XdTabOffset_T[i];
			m_plnkTabOffset_Rear->m_rgdXdTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->XdTabOffset_T[i];
		}
	}

	if(m_plnkPanelData->m_bUseFlag_Yl == TRUE)
	{
		for(i=0; i<m_plnkTabData->m_iYlTabQty; i++)
		{
			m_plnkTabOffset->m_rgdYlTabOffsetTabMounter[i] += m_plnkAutoAlignData->YlTabOffset_X[i];
			m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabMounter[i] += m_plnkAutoAlignData->YlTabOffset_X[i];

			m_plnkTabOffset->m_rgdYlTabOffsetTabAligner[i] += m_plnkAutoAlignData->YlTabOffset_Y[i];
			m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabAligner[i] += m_plnkAutoAlignData->YlTabOffset_Y[i];

			m_plnkTabOffset->m_rgdYlTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->YlTabOffset_T[i];
			m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->YlTabOffset_T[i];

		}
	}

	if(m_plnkPanelData->m_bUseFlag_Yr == TRUE)
	{
		for(i=0; i<m_plnkTabData->m_iYrTabQty; i++)
		{
			m_plnkTabOffset->m_rgdYrTabOffsetTabMounter[i] += m_plnkAutoAlignData->YrTabOffset_X[i];
			m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabMounter[i] += m_plnkAutoAlignData->YrTabOffset_X[i];

			m_plnkTabOffset->m_rgdYrTabOffsetTabAligner[i] += m_plnkAutoAlignData->YrTabOffset_Y[i];
			m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabAligner[i] += m_plnkAutoAlignData->YrTabOffset_Y[i];

			m_plnkTabOffset->m_rgdYrTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->YrTabOffset_T[i];
			m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabAlignerT[i] += m_plnkAutoAlignData->YrTabOffset_T[i];
		}
	}

	return iResult;
}
//______________
/* End Of File */
