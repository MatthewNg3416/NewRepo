/* 
 * PanelAligner Component
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
 * MPanelAligner.cpp : Implementation of PanelAligner component.
 */

#include "stdafx.h"
#include <math.h>
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MTeachingPosData.h"
#include "IAxis.h"
#include "ISercosMultiAxis.h"
#include "IRSNMCMultiAxis.h"		//161024 Add...
#include "IIO.h"
#include "ICylinder.h"
#include "IVacuum.h"
#include "MPanelAligner.h"
#include "DefModelChange.h"
#include "IStateModelChange.h"
#include "DefPanelTransfer.h"
#include "IStatePanelTransfer.h"
#include "common.h"
#include "MPlatformOlbSystem.h"
#include "MTabData.h"
#include "MTrsAutoManager.h"
#include "IStateCameraCarrier.h"
#include "DefCameraCarrier.h"

/*
#if !(defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_SOURCE_SYSTEM))
#include "IStateRollerConveyor.h"
#endif
*/
#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
#include "DefIOAddrPreBonder.h"
#include "IStateTabMounter.h"
#include "DefTabMounter.h"
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
#include "DefIOAddrAcf.h"
#include "IStateCarrier.h"
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
#include "DefIOAddrFinal.h"
#include "IStateMainTool.h"
#include "DefMainTool.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef DEG2RAD
#define	DEG2RAD(x)		((x) * PI / 180.0)
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

int MPanelAligner::checkInterlockForXYT(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
//@	MPos_XYT posTolerance = MPos_XYT(0.002, 0.002, 0.002);
	MPos_XYT posTolerance = MPos_XYT(5.0, 5.0, 5.0);
	double dTolerance = 0.01;

	// 목표 위치가 현재 위치이면 Interlock Check 불필요
	if (TRUE == IsInXYTPos(iPosID, &posTolerance))
		return ERR_PANEL_ALIGNER_SUCCESS;

	// A. Panel 흡착하지 않은 경우.
	if (FALSE == IsPanelAbsorbed())
	{
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE || iPosID == DEF_PANEL_ALIGNER_LOAD_POS)
		{
			// 1. Pickup Unit : Not Down
			if (!IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
				// 210100 = 이동 불가. [Panel 흡착하지 않고 Down 상태가 아님.]
				return generateErrorCode(210100);
		}
		return ERR_PANEL_ALIGNER_SUCCESS;
	}
	MTickTimer timer;
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	switch (iPosID)
	{
	case DEF_PANEL_ALIGNER_LOAD_POS:
#ifdef DEF_GATE_SYSTEM
		iResult = checkInterlockWithPrevUnit(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;
#endif
		iResult = checkInterlockWithTransferOut(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;		
		break;

	case DEF_PANEL_ALIGNER_FIRST_MARK_POS:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_ALIGNER_SECOND_MARK_POS:
#endif
	case DEF_PANEL_ALIGNER_INSPECT_1_POS:
	case DEF_PANEL_ALIGNER_UNLOAD_POS:
#ifdef DEF_GATE_SYSTEM
		dTolerance = 100.0;
		if (TRUE == IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dTolerance))
		{
#	ifndef DEF_SETUP	//@수정요.
			if (TRUE == IsPanelWorkingDetected())
				return generateErrorCode(210146);
#	endif
		}
#else
		if (iPosID == DEF_PANEL_ALIGNER_UNLOAD_POS)
		{
#ifndef DEF_SETUP	//@수정요
			if (FALSE == MOlbSystemPre.GetMelsecComponent()->IsOnLowerEqBit(eReserved1_UpperEqBit))
			{
				Sleep(800);
				if (FALSE == MOlbSystemPre.GetMelsecComponent()->IsOnLowerEqBit(eReserved1_UpperEqBit))
				{
					return generateErrorCode(210147);
				}
			}
#endif
		}
#endif
		timer.StartTimer();
	
		while(TRUE)
		{
			iResult = checkInterlockWithTransferOut(iPosID, posTarget, bUseAuto);
			if (iResult == 0)
				break;
			if(timer.MoreThan(10.0))
				return iResult;
			Sleep(15);
		}
		break;
//@미사용	case DEF_PANEL_ALIGNER_PRESS_1_POS:
//@미사용	case DEF_PANEL_ALIGNER_PRESS_2_POS:
//@미사용			break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_ALIGNER_INSPECT_2_POS:
		if (iPosID == DEF_PANEL_ALIGNER_UNLOAD_POS)
		{
#ifndef DEF_SETUP	//@수정요
			if (FALSE == MOlbSystemPre.GetMelsecComponent()->IsOnLowerEqBit(eReserved1_UpperEqBit))
			{
				Sleep(800);
				if (FALSE == MOlbSystemPre.GetMelsecComponent()->IsOnLowerEqBit(eReserved1_UpperEqBit))
				{
					return generateErrorCode(210148);
				}
			}
#endif
		}
		iResult = checkInterlockWithTransferOut(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;

		break;
#endif
//@미사용	case DEF_PANEL_ALIGNER_INSPECT_3_POS:
//@미사용		break;
//@	case DEF_PANEL_ALIGNER_UNLOAD_POS:
//@		break;
	}

	iResult = checkInterlockWithAirFloat(iPosID, posTarget, bUseAuto);
	if (iResult)
		return iResult;

	iResult = checkInterlockWithPRBUnit(iPosID, posTarget, bUseAuto);
	if (iResult)
		return iResult;


	/*/
	//110102 HJG
#ifdef DEF_USE_PANEL_PUSHER
	if(m_plnkSystemData->m_bUsePanelPusher == TRUE)
	{
		if(IsUpPanelPusherUDCyl() == FALSE)
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210114 = 이동 불가. [Pusher Down 상태임.]
			return generateErrorCode(210114);
		}
	}
#endif
	/*/


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	
#endif // DEF_PLATFORM_PREBONDER_SYSTEM
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if defined(DEF_PLATFORM_ARP_SYSTEM)
	// ARP Backup1 Not Down
	if (FALSE == m_plnkStateCarrier1->IsDownBackupUDCyl())
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210048 = Back-Up1이 Down 상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210048);
	}

	// hongju_SESL Auto Run 상태에서는 체크하지 않도록 수정.
	// ARP Press Roller Unit Not Up
	if (FALSE == bUseAuto)
	{
		if (FALSE == m_plnkStateCarrier1->IsUpCarrierPressUDCyl())
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210050 = Carrier Press1이 Up 상태가 아니어서 이동할 수 없습니다.
			return generateErrorCode(210050);
		}
	}

#	ifdef DEF_GATE_SYSTEM

	// 20081014 jdy
	// ARP Recovery Weight Guide Unit Not Up
	if (FALSE == m_plnkStateCarrier1->IsUpRecoveryWeightGuideUDCyl())
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210052 = Recovery Weight Guide1이 Up 상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210052);
	}

	if (m_plnkStateCarrier1->IsDownRecoveryACFLineGripperUDCyl()
		&& m_plnkStateCarrier1->IsForwardRecoveryACFLineFBCyl())
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210054 = Recovery ACF Line Gripper1가 Up 상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210054);
	}

	// ARP Backup2 Not Down
	if (FALSE == m_plnkStateCarrier2->IsDownBackupUDCyl())
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210049 = Back-Up2가 Down 상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210049);
	}

	// hongju_SESL : Autorun에서는 체크하지 않도록
	// ARP Press Roller Unit Not Up
	if (FALSE == bUseAuto)
	{
		if (FALSE == m_plnkStateCarrier2->IsUpCarrierPressUDCyl())
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210051 = Carrier Press2가 Up 상태가 아니어서 이동할 수 없습니다.
			return generateErrorCode(210051);
		}
	}

	// ARP Recovery Weight Guide Unit Not Up
	if (FALSE == m_plnkStateCarrier2->IsUpRecoveryWeightGuideUDCyl())
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210053 = Recovery Weight Guide2가 Up 상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210053);
	}
	if (m_plnkStateCarrier2->IsDownRecoveryACFLineGripperUDCyl()
		&& m_plnkStateCarrier2->IsForwardRecoveryACFLineFBCyl())
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210055 = Recovery ACF Line Gripper2가 Up 상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210055);
	}
#	endif
#endif
	
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	// Final Backup Not Down
	if (FALSE == bUseAuto)
	{
		if (m_plnkStateMainTool1->IsUpBackup())
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210059 = Tool1 Backup 이 상승상태가 아닙니다.
			return generateErrorCode(210059);
		}
		
		if (FALSE == m_plnkStateMainTool1->IsUpCassette())
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210063 = Cassette1이 상승상태가 아니어서 이동할 수 없습니다.
			return generateErrorCode(210063);
		}
//hans ver3 081201
	}

	// Tool Position Check
	double dPos = m_plnkStateMainTool1->GetPressCurrentPosition();
	double dRefPos = m_plnkStateMainTool1->GetPressTargetPosition(DEF_MAINTOOL_EXPANSION_POS);
	if (dPos < dRefPos - 0.05)	// 0.05 : Tolerance
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210061 = Tool1이 안전한 위치에 있지 않습니다.
		return generateErrorCode(210061);
	}
	
#	ifdef DEF_GATE_SYSTEM
	if (FALSE == bUseAuto)
	{
		if (m_plnkStateMainTool2->IsUpBackup())
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210060 = Tool2 Backup 이 상승상태가 아닙니다.
			return generateErrorCode(210060);
		}
		if (FALSE == m_plnkStateMainTool2->IsUpCassette())
		{
			SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
			// 210064 = Cassette2가 상승상태가 아니어서 이동할 수 없습니다.
			return generateErrorCode(210064);
		}
	}

	// Tool Position Check
	dPos = m_plnkStateMainTool2->GetPressCurrentPosition();
	dRefPos = m_plnkStateMainTool2->GetPressTargetPosition(DEF_MAINTOOL_EXPANSION_POS);
	if (dPos < dRefPos - 0.05)	// 0.05 : Tolerance	
	{
		SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
		// 210062 = Tool2가 안전한 위치에 있지 않습니다.
		return generateErrorCode(210062);
	}
#	endif
#endif
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::checkInterlockWithPrevUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	double dTolerance = 0.0;
	
	//@Loading위치에 있으면 확인안함
	if (TRUE == IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	//@__________Gate System_______________
#ifdef DEF_GATE_SYSTEM	
#	ifdef DEF_PLATFORM_ARP_SYSTEM
	return ERR_PANEL_ALIGNER_SUCCESS;	//@ACF Gate는 해당사항 없음
#	else	
	MPos_XYT curPos = GetXYTCurrentPos();
	if (posTarget.dX < curPos.dX - 1.0)	//-방향으로 이동시
	{
		if (FALSE == m_plnkTrsAutoManager->IsPanelEmpty_PrevMCUnload())
			return generateErrorCode(210140);
	}
	if (FALSE == m_plnkTrsAutoManager->IsDoorClose_PrevMCUnload())
		return generateErrorCode(210141);
#	endif
#endif

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::checkInterlockWithNextUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

#ifdef DEF_GATE_SYSTEM
	return ERR_PANEL_ALIGNER_SUCCESS;
#else
#	ifdef DEF_PLATFORM_FINALBONDER_SYSTEM
		return ERR_PANEL_ALIGNER_SUCCESS;
#	else
//@		double dCompPos = GetXYTTargetPos(DEF_PANEL_ALIGNER_PRESS_2_POS).dX;
//@		if (posTarget.dX >= dCompPos - 100.0)
//@		{
//@			if (FALSE == MOlbSystemPre.Get
//@		}
#	endif
#endif
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::checkInterlockWithTransferCenter(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int	MPanelAligner::checkInterlockWithTransferIn(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
#ifdef DEF_SOURCE_SYSTEM
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	double dTolerance = 0.0;
	if(m_plnkPanelData->m_dPanelSize_X < 1500.0)
	{
		if (TRUE == m_plnkStatePanelTransfer->IsPanelAbsorbed()
			&& m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS) == FALSE )
			// 210110 = 이동 불가. [PanelTransfer 가 Panel 흡착 상태임.]
			return generateErrorCode(210110);
		if (FALSE == m_plnkStatePanelTransfer->IsDownPickUpUDCyl()
			&& m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS) == FALSE)
			// 210110 = 이동 불가. [PanelTransfer 가 Panel 흡착 상태임.]
			return generateErrorCode(210111);
	}
	else
	{
		if (TRUE == m_plnkStatePanelTransfer->IsPanelAbsorbed())
			// 210110 = 이동 불가. [PanelTransfer 가 Panel 흡착 상태임.]
			return generateErrorCode(210110);
		if (FALSE == m_plnkStatePanelTransfer->IsDownPickUpUDCyl())
			// 210110 = 이동 불가. [PanelTransfer 가 Panel 흡착 상태임.]
			return generateErrorCode(210111);
	}

	if (iPosID == DEF_PANEL_ALIGNER_LOAD_POS || iPosID == DEF_PANEL_ALIGNER_INSPECT_2_POS)
	{
		dTolerance = 10.0;
		if (TRUE == IsInXPos(DEF_PANEL_ALIGNER_FIRST_MARK_POS, &dTolerance))
		{
#	ifdef DEF_GATE_SYSTEM
			if (TRUE == IsPanelLoadingDetected())
#	else
			if (TRUE == m_plnkStatePanelTransfer->IsPanelLoadingDetected())
#	endif
			{
				//20131015 SJ_HJG
				if(m_plnkPanelData->m_dPanelSize_X < 1500.0)
				{
					if (m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS) == FALSE )
						// 210110 = 이동 불가. [PanelTransfer 가 Panel 흡착 상태임.]
						return generateErrorCode(210143);
				}
				else
					// 210110 = 이동 불가. [PanelTransfer 가 Panel 흡착 상태임.]
					return generateErrorCode(210143);
				//____________________
			}
		}
	}
#endif

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int	MPanelAligner::checkInterlockWithTransferOut(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

#ifdef DEF_GATE_SYSTEM
	double dTolerance = 100.0;
	if (FALSE == m_plnkStatePanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, &dTolerance))
#endif
	if(iPosID == DEF_PANEL_ALIGNER_PRESS_2_POS
		|| iPosID == DEF_PANEL_ALIGNER_INSPECT_2_POS
		|| iPosID == DEF_PANEL_ALIGNER_INSPECT_3_POS
		|| iPosID == DEF_PANEL_ALIGNER_UNLOAD_POS
		)
	{
#ifdef DEF_SOURCE_SYSTEM
		if (FALSE == m_plnkStatePanelTransferOut->IsDownPickUpUDCyl())
			return generateErrorCode(210144);
		if (FALSE == m_plnkStatePanelTransferOut->IsPanelReleased())
			return generateErrorCode(210145);
		if (FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad())
			return generateErrorCode(210152);
#endif
	}
	
	if(iPosID == DEF_PANEL_ALIGNER_FIRST_MARK_POS
		|| iPosID == DEF_PANEL_ALIGNER_SECOND_MARK_POS
		|| iPosID == DEF_PANEL_ALIGNER_PRESS_1_POS
		|| iPosID == DEF_PANEL_ALIGNER_INSPECT_1_POS
		)
	{
#ifdef DEF_SOURCE_SYSTEM
		double dTolerance = 10.0;
		if (FALSE == m_plnkStatePanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, &dTolerance))
		{
			if (FALSE == m_plnkStatePanelTransferOut->IsDownPickUpUDCyl())
				return generateErrorCode(210150);
			if (FALSE == m_plnkStatePanelTransferOut->IsPanelReleased())
				return generateErrorCode(210151);
		}
#endif
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int	MPanelAligner::checkInterlockWithAirFloat(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	// 부상 유닛 흡착 해제 상태 아닌 경우
	if (FALSE == IsAirCVVacAllOff())
		// 210056 = Plate 흡착해제상태가 아니어서 이동할 수 없습니다.
		return generateErrorCode(210056);

	// 부상 유닛 부상 상태 아닌 경우	
	if (FALSE == IsAirCVBlowOn())
	{
		MTickTimer timer;
		timer.StartTimer();
		
		while (FALSE == IsAirCVBlowOn())
		{
			Sleep(15);
			
			if (timer.MoreThan(5.0))
				// 210019 = Plate 부상상태가 아니어서 이동할 수 없습니다.
				return generateErrorCode(210019);
		}
	}	
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::checkInterlockWithACFUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::checkInterlockWithPRBUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	int i = 0;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	double dTolerance = 100.0;
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
			//@130204_________________________
#ifdef DEF_USE_PANEL_PUSHER
			if (FALSE == m_plnkStateTabMounter[i]->IsUpPusherCyl())
			{
				Sleep(200);
				if (FALSE == m_plnkStateTabMounter[i]->IsUpPusherCyl())
				{
					SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
					// 210107 = 이동 불가. [전면부 TabMounter 하강 상태임.]
					return generateErrorCode(210125);
				}
			}
#endif
			//@_______________________________
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

			//@130204_________________________
#ifdef DEF_USE_PANEL_PUSHER
			if (FALSE == m_plnkStateRTabMounter[i]->IsUpPusherCyl())
			{
				Sleep(200);
				if (FALSE == m_plnkStateRTabMounter[i]->IsUpPusherCyl())
				{
					SetErrorLevel(_T("PANEL ALIGNER check Interlock for XYT"),2, __FILE__, __LINE__);
					// 210107 = 이동 불가. [전면부 TabMounter 하강 상태임.]
					return generateErrorCode(210126);
				}
			}
#endif
			//@_______________________________
		}
	}

	dTolerance = 1.0;
	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		for(int p = 0; p < 50; p++)
		{
			iResult = 0;
			if (FALSE == m_plnkStateInspectCamCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS, &dTolerance))
				iResult = generateErrorCode(210085);
			if (FALSE == m_plnkStateRInspectCamCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS, &dTolerance))
				iResult = generateErrorCode(210086);
			if(iResult == 0)
				break;
			Sleep(50);
		}
		if(iResult)
			return iResult;
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::checkInterlockWithFNBUnit(int iPosID, MPos_XYT posTarget, BOOL bUseAuto)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	if (FALSE == IsPanelAbsorbed() && TRUE == IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
		return ERR_PANEL_ALIGNER_SUCCESS;

	return ERR_PANEL_ALIGNER_SUCCESS;
}


int MPanelAligner::checkInterlockForZ(int iPosID, double posTarget, BOOL bUseAuto)
{
	switch (iPosID)
	{
	case DEF_PANEL_ALIGNER_Z_DOWN_POS:
		break;
	case DEF_PANEL_ALIGNER_Z_MOVE_UP_POS:
		break;
	case DEF_PANEL_ALIGNER_Z_WORK_POS:
		break;
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::checkInterlockForBackupMoveX(int iPosID, double posTarget, BOOL bUseAuto)
{
	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/
int MPanelAligner::GetStartTabNo(int iPressWorkNo)
{
	ASSERT(iPressWorkNo >= 0 && iPressWorkNo < DEF_MAX_WORK_SCHEDULE);
	return m_iSourceWorkTabStart[iPressWorkNo];
}

int MPanelAligner::GetLastTabNo(int iPressWorkNo)
{
	ASSERT(iPressWorkNo >= 0 && iPressWorkNo < DEF_MAX_WORK_SCHEDULE);
	return m_iSourceWorkTabLast[iPressWorkNo];
}

int	MPanelAligner::CalculateWorkTabNo()
{
	double dPanelHalfSizeX = m_plnkPanelData->m_dPanelSize_X/2.0;
	double dSum = 0.0;
	int iMaxTab = m_plnkTabData->m_iXuTabQty;
	int i = 0;
	
	m_iSourceWorkTabStart[0] = -1;
	m_iSourceWorkTabLast[0] = -1;
	m_iSourceWorkTabStart[1] = -1;
	m_iSourceWorkTabLast[1] = -1;

	//Panel중심기준으로 첫번째 작업변의 시작 Tab번호를 구한다
	for (int i=0; i<iMaxTab; i++)
	{
		dSum += m_plnkTabData->m_dXuTabPitch[i];
		if (dSum > dPanelHalfSizeX)
		{
			m_iSourceWorkTabStart[0] = i;
			m_iSourceWorkTabLast[0] = iMaxTab-1;
			m_iSourceWorkTabStart[1] = 0;
			m_iSourceWorkTabLast[1] = i-1;
			break;
		}
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::calculatePanelAlignerXYTModelPosParameter()
{
	// Model 좌표값 초기화
	for (unsigned int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++) {
		m_rgsXYTModelPos[i].Init(0.0, 0.0, 0.0);
	}

	/*/
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX = 0.0;
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dY = 0.0;
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dT = 0.0;

	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_SECOND_MARK_POS]
	= m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_INSPECT_1_POS]
	= m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_INSPECT_2_POS]	= m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS];

	/*/
	//@Aligner Plate Center가 Camera중심에 놓이는 위치
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX
	= m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_SECOND_MARK_POS].dX = m_plnkSystemData->m_dAlignCam1_FixedPos_X - DEF_PANELALIGNER_DIST_CENTERTOCENTER/2.0;
	
#ifdef DEF_GATE_SYSTEM
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX =
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_INSPECT_2_POS].dX = m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX;

	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX = 
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_SECOND_MARK_POS].dX = 
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX =
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_2_POS].dX = MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X();
#else
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX =
	m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_INSPECT_2_POS].dX = m_rgsXYTFixedPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX;

#	ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX = 
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_SECOND_MARK_POS].dX = - MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X();

	//1번 인식위치는 Normal 라인 전면 기준 부상 우측 끝단. (고정좌표 Panel 위치 동일)
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX = - MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X();
	//2번 인식위치는 Normal 라인 전면 기준 좌측(투입방향쪽) 부상 끝단.
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_2_POS].dX = - MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X() - MOlbSystemPre.GetDifferLen_PlateToPanelLen();

#	else
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX = 
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_SECOND_MARK_POS].dX = MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X();

	//1번 인식위치는 부상 시작 끝단에 패널 마크 끝단 위치
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX = MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X();
	//2번 인식위치는 부상 마지막 끝단에 패널 마크 끝단 위치
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_2_POS].dX = MOlbSystemPre.GetDistance_StagePlateCenterToPanelMark_X() + MOlbSystemPre.GetDifferLen_PlateToPanelLen();

	//SJ_YYK 150109 Add..
	// Panel Align 투입시 Inspect2 번 위치에서 하도록 변경,...
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX = 
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_SECOND_MARK_POS].dX = m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_2_POS].dX;
	
#	endif
	//______________________________________________________________________________________________________________*/
#endif

	/*/
	double dCamDist = 0.0;
	double dTabDist = 0.0;

#ifdef DEF_GATE_SYSTEM
	int iAlignGroup = MOlbSystemPre.GetPanelAlignGroup();
	if (m_plnkPreBonderData->m_dLead_Width_Ytab < DEF_LARGE_TYPE_TAB_WIDTH_STANDARD)
		dCamDist = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[iAlignGroup][0];
	else
		dCamDist = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[iAlignGroup][0];
	dTabDist = m_plnkPreBonderData->m_dLead_Width_Ytab;
	//Tab길이와 Camera중심간 간격이 다를경우대비(Camera확장/축소가 실린더라서 Tab길이 대응이 안됨)
//@	if (TRUE == m_plnkSystemData->m_bUseTabPanelAlign)
	if (TRUE == m_plnkPreBonderData->m_bUseTabPanelAlign)
		m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX += (dCamDist - dTabDist)/2.0;
#else

	CalculateWorkTabNo();
	
	double dTabToTabDist = MOlbSystemPre.GetXDistance_TabCenterToCenter(WORKING_SIDE_X_UP, m_iSourceWorkTabStart[1], m_iSourceWorkTabStart[0]);
//@	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS].dX = dTabToTabDist*(-1.0);
	m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_1_POS] = m_rgsXYTModelPos[DEF_PANEL_ALIGNER_FIRST_MARK_POS];

#ifdef DEF_SOURCE_SYSTEM
	dTabDist = m_plnkPreBonderData->m_dLead_Width_Xtab;
#else
	dTabDist = m_plnkPreBonderData->m_dLead_Width_Ytab;
#endif

	if (dTabDist < DEF_LARGE_TYPE_TAB_WIDTH_STANDARD)
		dCamDist = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[0][0];	//Camera 축소시 거리
	else
		dCamDist = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[0][0];	//Camera 확장시 거리
	#pragma message(__LOC__ "아래 코드는 필요 없을듯... Inspection Camera에서 대응하지 않나???")
	//Tab길이와 Camera중심간 간격이 다를경우대비(Camera확장/축소가 실린더라서 Tab길이 대응이 안됨)
	if (TRUE == m_plnkPreBonderData->m_bUseTabPanelAlign)
		m_rgsXYTModelPos[DEF_PANEL_ALIGNER_INSPECT_1_POS].dX += ((dCamDist - dTabDist)/2.0);
#endif
	/*/

	return ERR_PANEL_ALIGNER_SUCCESS;
}

MPanelAligner::MPanelAligner(SCommonAttribute commonData, SPanelAlignerRefCompList listRefComponents, SPanelAlignerData datComponent)
: ICommon(commonData)
{
	SetCommonAttribute(commonData);
	
	// Component List Assign
	AssignComponents(listRefComponents);

	// Align 변수 초기화
	m_bMarkAligned = FALSE;
	m_dValueThetaAlign = 0.0;
	m_rgAlignMarkOffset.Init(0.0, 0.0, 0.0);
	m_rgPanelCenterOffset.Init(0.0, 0.0, 0.0);

	// PanelAligner X, Y, T축 좌표 초기화
	for (int i = 0 ; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
	{
		m_rgsXYTFixedPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTOffsetPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTModelPos[i].Init(0.0, 0.0, 0.0);
	}

	// Panel Z축 좌표 초기화
	for (int i = 0; i < DEF_PANEL_ALIGNER_Z_MAX_POSITION; i++)
	{
		m_rgdZFixedPos[i] = 0.0;
		m_rgdZOffsetPos[i] = 0.0;
		m_rgdZModelPos[i] = 0.0;
	}

	/*/
	// BackupMoveX축 좌표 초기화
	for (int i = 0; i < DEF_BACKUP_MOVE_X_MAX_POSITION; i++)
	{
		m_rgdBackupXFixedPos[i] = 0.0;
		m_rgdBackupXModelPos[i] = 0.0;
		m_rgdBackupXOffsetPos[i] = 0.0;
	}
	/*/

	SetData(datComponent);

	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	LoadXYTFixedPosParameter();				// XYTZ 고정 좌표 Load
	LoadZFixedPosParameter();
	LoadXYTOffsetPosParameter();			// XYTZ 모델 Offset Load	
	LoadZOffsetPosParameter();
	calculatePanelAlignerXYTModelPosParameter();

#ifdef DEF_SOURCE_SYSTEM
//@	LoadBackupMoveXFixedPosParameter();
//@	LoadBackupMoveXOffsetPosParameter();
#endif
}

MPanelAligner::~MPanelAligner()
{

}

int MPanelAligner::AssignComponents(SPanelAlignerRefCompList PanelAlignerCompList)
{
	m_plnkIO			= PanelAlignerCompList.m_plnkIO;
	m_plnkVac1			= PanelAlignerCompList.m_plnkVac1;
	m_plnkVac2			= PanelAlignerCompList.m_plnkVac2;
	m_plnkVac3			= PanelAlignerCompList.m_plnkVac3;
//@	m_plnkVacRotator	= PanelAlignerCompList.m_plnkVacRotator;
//@	m_plnkCylUDRotator	= PanelAlignerCompList.m_plnkCylUDRotator;
//@	m_plnkCylTRRotator	= PanelAlignerCompList.m_plnkCylTRRotator;
	//110102 HJG 
#ifdef DEF_USE_PANEL_PUSHER
	m_plnkPanelPusherUD	= PanelAlignerCompList.m_plnkPanelPusherUD;
#endif

	m_plnkX				= PanelAlignerCompList.m_plnkX;
	m_plnkY				= PanelAlignerCompList.m_plnkY;
	m_plnkT				= PanelAlignerCompList.m_plnkT;
	m_plnkZ				= PanelAlignerCompList.m_plnkZ;
	m_plnkMultiYT		= PanelAlignerCompList.m_plnkMultiYT;

//@	m_plnkBackupMoveX	= PanelAlignerCompList.m_plnkBackupMoveX;
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AssignProcess(SPanelAlignerRefProcess PanelAlignerRefProcess)
{
	m_plnkTrsAutoManager = PanelAlignerRefProcess.m_plnkTrsAutoManager;

	return ERR_PANEL_ALIGNER_SUCCESS;
}

IAxis* MPanelAligner::GetMotionComponent(int nAxisID)
{
	// Motion Component 축 ID에 따라
	switch (nAxisID)
	{
	case DEF_PANEL_ALIGNER_X_AXIS:		// PanelAligner X축 
		return m_plnkX;

	case DEF_PANEL_ALIGNER_Y_AXIS:		// PanelAligner Y축
		return m_plnkY;

	case DEF_PANEL_ALIGNER_T_AXIS:		// PanelAligner T축
		return m_plnkT;

	default :					// 축 ID 오류
		return NULL;
	}
}

IAxis* MPanelAligner::GetZMotionComponent()
{
	return m_plnkZ;
}

/*/
IAxis* MPanelAligner::GetBackupXMotionComponent()
{
	return m_plnkBackupMoveX;
}
/*/

//____________________________________________________________________________
//20161024 Modify...
//ISercosMultiAxis* MPanelAligner::GetMultiMotionComponent()
IRSNMCMultiAxis* MPanelAligner::GetMultiMotionComponent()
{
	return m_plnkMultiYT;
}

/************************************************************************/
/*        Teaching 관련 Interface                                       */
/************************************************************************/

int MPanelAligner::LoadXYTFixedPosParameter(void)
{
	MTeachingPosData PanelAlignerXYTZFixPosDat(m_sXYTZTeachingInfo, m_strPosFileName);

	BOOL bResult = PanelAlignerXYTZFixPosDat.ReadData();
	for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
	{
		m_rgsXYTFixedPos[i].dX = PanelAlignerXYTZFixPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_X_AXIS];
		m_rgsXYTFixedPos[i].dY = PanelAlignerXYTZFixPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_Y_AXIS];
		m_rgsXYTFixedPos[i].dT = PanelAlignerXYTZFixPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_T_AXIS];
	}
	if (!bResult)
		// 210001 = XYT 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210001);
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::LoadZFixedPosParameter(void)
{
	MTeachingPosData PanelAlignerZFixPosDat(m_sZTeachingInfo, m_strPosFileName);
	
	BOOL bResult = PanelAlignerZFixPosDat.ReadData();
	for (int i = 0; i < DEF_PANEL_ALIGNER_Z_MAX_POSITION; i++)
		m_rgdZFixedPos[i] = PanelAlignerZFixPosDat.m_TeachingPos[i][0];
	if (!bResult)
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210063);
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::LoadBackupMoveXFixedPosParameter(void)
{
	MTeachingPosData BackupMoveXFixPosDat(m_sBackupMoveXTeachingInfo, m_strPosFileName);
	
	BOOL bResult = BackupMoveXFixPosDat.ReadData();
	for (int i = 0; i < DEF_BACKUP_MOVE_X_MAX_POSITION; i++)
		m_rgdBackupXFixedPos[i] = BackupMoveXFixPosDat.m_TeachingPos[i][0];
	if (!bResult)
	{
		//WriteErrorLog("LoadXYTFixedPosParameter() : \
		//	           Loading PanelAligner XYT Fixed Pos Parameter Error : Reading Failure");
		SetErrorLevel(_T("BackupMove load X Fixed Position Parameter"), 2, __FILE__, __LINE__);
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210064);
	}
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

int MPanelAligner::SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

	MTeachingPosData PanelAlignerXYTZFixPosDat(m_sXYTZTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
	{
		m_rgsXYTFixedPos[i].dX = rgsFixedPos[i].dX;
		m_rgsXYTFixedPos[i].dY = rgsFixedPos[i].dY; 
		m_rgsXYTFixedPos[i].dT = rgsFixedPos[i].dT;
		
		PanelAlignerXYTZFixPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_X_AXIS] = m_rgsXYTFixedPos[i].dX;
		PanelAlignerXYTZFixPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_Y_AXIS] = m_rgsXYTFixedPos[i].dY;
		PanelAlignerXYTZFixPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_T_AXIS] = m_rgsXYTFixedPos[i].dT;
	}

	BOOL bResult = PanelAlignerXYTZFixPosDat.WriteData();
	if (!bResult)
		// 210002 = XYT 축 고정좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(210002);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::SaveZFixedPosParameter(double *rgdFixedPos)
{
	ASSERT(rgdFixedPos != NULL);

	MTeachingPosData PanelAlignerZFixPosDat(m_sZTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_PANEL_ALIGNER_Z_MAX_POSITION; i++)
	{
		m_rgdZFixedPos[i] = rgdFixedPos[i];
		
		PanelAlignerZFixPosDat.m_TeachingPos[i][0] = m_rgdZFixedPos[i];
	}

	BOOL bResult = PanelAlignerZFixPosDat.WriteData();
	if (!bResult)
		// 210002 = XYT 축 고정좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(210067);
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::SaveBackupMoveXFixedPosParameter(double *rgdFixed)
{
	ASSERT(rgdFixed != NULL);

	MTeachingPosData BackupMoveXFixPosDat(m_sBackupMoveXTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_BACKUP_MOVE_X_MAX_POSITION; i++)
	{
		m_rgdBackupXFixedPos[i] = rgdFixed[i];

		BackupMoveXFixPosDat.m_TeachingPos[i][0] = m_rgdBackupXFixedPos[i];
	}
	BOOL bResult = BackupMoveXFixPosDat.WriteData();
	if (!bResult)
	{
		SetErrorLevel(_T("BackupMove save X Fixed Position Parameter"), 2, __FILE__, __LINE__);
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210068);
	}
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

int MPanelAligner::LoadXYTOffsetPosParameter(void)
{
	MTeachingPosData PanelAlignerXYTOffsetPosDat(m_sXYTZTeachingInfo, 
										  m_strOffsetPosFileName,
										  m_strOffsetPosFilePath);
	// Offset Pos
	BOOL bResult = PanelAlignerXYTOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
	{
		m_rgsXYTOffsetPos[i].dX = PanelAlignerXYTOffsetPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_X_AXIS];
		m_rgsXYTOffsetPos[i].dY = PanelAlignerXYTOffsetPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_Y_AXIS];
		m_rgsXYTOffsetPos[i].dT = PanelAlignerXYTOffsetPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_T_AXIS];
	}

	if (!bResult)
		// 210003 = XYT 축 Offset 좌표를 읽어오지 못했습니다.
		return generateErrorCode(210003);
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::LoadZOffsetPosParameter()
{
	MTeachingPosData PanelAlignerZOffsetPosDat(m_sZTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);
	
	BOOL bResult = PanelAlignerZOffsetPosDat.ReadData();
	for (int i = 0; i < DEF_PANEL_ALIGNER_Z_MAX_POSITION; i++)
		m_rgdZOffsetPos[i] = PanelAlignerZOffsetPosDat.m_TeachingPos[i][0];
	if (!bResult)
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210065);
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::LoadBackupMoveXOffsetPosParameter()
{
	MTeachingPosData BackupMoveXOffsetPosDat(m_sBackupMoveXTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);
	
	BOOL bResult = BackupMoveXOffsetPosDat.ReadData();
	for (int i = 0; i < DEF_BACKUP_MOVE_X_MAX_POSITION; i++)
		m_rgdBackupXOffsetPos[i] = BackupMoveXOffsetPosDat.m_TeachingPos[i][0];
	if (!bResult)
	{
		//WriteErrorLog("LoadXYTFixedPosParameter() : \
		//	           Loading PanelAligner XYT Fixed Pos Parameter Error : Reading Failure");
		SetErrorLevel(_T("BackupMove load X Offset Position Parameter"), 2, __FILE__, __LINE__);
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210066);
	}
	return ERR_PANEL_ALIGNER_SUCCESS;	
}
/*/

int MPanelAligner::SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	MTeachingPosData PanelAlignerXYTZOffsetPosDat(m_sXYTZTeachingInfo, 
										  m_strOffsetPosFileName, 
										  m_strOffsetPosFilePath);
	// Offset Pos 
	for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
	{
		m_rgsXYTOffsetPos[i].dX = rgsOffsetPos[i].dX;
		m_rgsXYTOffsetPos[i].dY = rgsOffsetPos[i].dY;
		m_rgsXYTOffsetPos[i].dT = rgsOffsetPos[i].dT;

		PanelAlignerXYTZOffsetPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_X_AXIS] = m_rgsXYTOffsetPos[i].dX;
		PanelAlignerXYTZOffsetPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_Y_AXIS] = m_rgsXYTOffsetPos[i].dY;
		PanelAlignerXYTZOffsetPosDat.m_TeachingPos[i][DEF_PANEL_ALIGNER_T_AXIS] = m_rgsXYTOffsetPos[i].dT;
	}

	BOOL bResult = PanelAlignerXYTZOffsetPosDat.WriteData();
	if (!bResult)
		// 210004 = XYT 축 Offset 좌표를 File 에 기록할 수 없습니다.
		return generateErrorCode(210004);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::SaveZOffsetPosParameter(double* rgdOffsetPos)
{
	ASSERT(rgdOffsetPos != NULL);

	MTeachingPosData PanelAlignerZOffsetPosDat(m_sZTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);
	
	for (int i = 0; i < DEF_PANEL_ALIGNER_Z_MAX_POSITION; i++)
	{
		m_rgdZOffsetPos[i] = rgdOffsetPos[i];

		PanelAlignerZOffsetPosDat.m_TeachingPos[i][0] = m_rgdZOffsetPos[i];
	}
	int bResult = PanelAlignerZOffsetPosDat.WriteData();
	if (!bResult)
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210069);
	
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::SaveBackupMoveXOffsetPosParameter(double* rgdOffsetPos)
{
	MTeachingPosData BackupMoveXOffsetPosDat(m_sBackupMoveXTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);
	
	for (int i = 0; i < DEF_BACKUP_MOVE_X_MAX_POSITION; i++)
	{
		m_rgdBackupXOffsetPos[i] = rgdOffsetPos[i];

		BackupMoveXOffsetPosDat.m_TeachingPos[i][0] = m_rgdBackupXOffsetPos[i];
	}
	int bResult = BackupMoveXOffsetPosDat.WriteData();
	if (!bResult)
	{
		SetErrorLevel(_T("BackupMove save X Offset Position Parameter"), 2, __FILE__, __LINE__);
		// 210063 = Z 축 고정좌표를 읽어오지 못했습니다.
		return generateErrorCode(210070);
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

int MPanelAligner::GetXYTPosParameter(MPos_XYT* rgsFixedPos,
									   MPos_XYT* rgsOffsetPos,
									   MPos_XYT* rgsModelPos)
{
	for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_POSITION; i++)
	{
		if (rgsFixedPos != NULL)
			rgsFixedPos[i] = m_rgsXYTFixedPos[i];
		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i]	= m_rgsXYTOffsetPos[i];
		if (rgsModelPos != NULL)
			rgsModelPos[i] = m_rgsXYTModelPos[i];
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::GetZPosParameter(double *rgdFixedPos, double *rdgOffsetPos, double *rgdModelPos)
{
	for (int i = 0; i < DEF_PANEL_ALIGNER_Z_MAX_POSITION; i++)
	{
		if (rgdFixedPos != NULL)
			rgdFixedPos[i] = m_rgdZFixedPos[i];
		if (rdgOffsetPos != NULL)
			rdgOffsetPos[i] = m_rgdZOffsetPos[i];
		if (rgdModelPos != NULL)
			rgdModelPos[i] = m_rgdZModelPos[i];
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::GetBackupMoveXPosParameter(double *rgdFixedPos, double *rgdOffsetPos, double *rgdModelPos)
{
	for (int i = 0; i < DEF_BACKUP_MOVE_X_MAX_POSITION; i++)
	{
		if (rgdFixedPos != NULL)
			rgdFixedPos[i] = m_rgdBackupXFixedPos[i];
		if (rgdOffsetPos != NULL)
			rgdOffsetPos[i] = m_rgdBackupXOffsetPos[i];
		if (rgdModelPos != NULL)
			rgdModelPos[i] = m_rgdBackupXModelPos[i];
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

MPos_XYT MPanelAligner::GetXYTModelPosParameter(int iPos)
{
	ASSERT(iPos < DEF_PANEL_ALIGNER_MAX_POSITION);

	return m_rgsXYTModelPos[iPos];
}

double MPanelAligner::GetZModelPosParameter(int iPos)
{
	ASSERT(iPos < DEF_PANEL_ALIGNER_Z_MAX_POSITION);

	return m_rgdZModelPos[iPos];
}

/*/
double MPanelAligner::GetBackupMoveXModelPosParameter(int iPos)
{
	ASSERT(iPos < DEF_BACKUP_MOVE_X_MAX_POSITION);

	return m_rgdBackupXModelPos[iPos];
}
/*/
	
MPos_XYT MPanelAligner::GetXYTOffsetPosParameter(int iPos) 
{
	ASSERT(iPos < DEF_PANEL_ALIGNER_MAX_POSITION);

	return m_rgsXYTOffsetPos[iPos];
}

double MPanelAligner::GetZOffsetPosParameter(int iPos)
{
	ASSERT(iPos < DEF_PANEL_ALIGNER_Z_MAX_POSITION);

	return m_rgdZOffsetPos[iPos];
}

/*/	
double MPanelAligner::GetBackupMoveXOffsetPosParameter(int iPos)
{
	ASSERT(iPos < DEF_BACKUP_MOVE_X_MAX_POSITION);

	return m_rgdBackupXOffsetPos[iPos];
}
/*/

STeachingInfo MPanelAligner::GetXYTTeachingInfo()
{
	return m_sXYTZTeachingInfo;
}

STeachingInfo MPanelAligner::GetZTeachingInfo()
{
	return m_sZTeachingInfo;
}

/*/
STeachingInfo MPanelAligner::GetBackupMoveXTeachingInfo()
{
	return m_sBackupMoveXTeachingInfo;
}
/*/

int MPanelAligner::AbsorbPanel(BOOL bSkipSensor)
{
	/** Vacuum Off되어 있는지 확인 */
	if (TRUE == IsPanelAbsorbed())
		return ERR_PANEL_ALIGNER_SUCCESS;
	
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Absorb Panel (SkipMode:%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	BOOL bVacOn1, bVacOn2, bVacOn3;
	bVacOn1 = bVacOn2 = bVacOn3 = TRUE;

	/*/
	int nVacCount = 0;
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_40INCH - 10.0)			nVacCount = 1;
	else if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)	nVacCount = 2;
	else																		nVacCount = 3;
#else
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)	nVacCount = 2;
	else																nVacCount = 3;
#endif
	/*/
	int nVacCount = 3;	//@모두 흡착

#ifdef SIMULATION
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif

	IVacuum* pVac[3] = {m_plnkVac1, m_plnkVac2, m_plnkVac3};
	for (int i=0; i<nVacCount; i++) {
		pVac[i]->On(TRUE);
	}

	int nVacOnCnt = 0;
	MTickTimer T_TimeOut;
	double dTurningTime, dOnSettingTime, dOffSettingTime;
	pVac[0]->GetVacuumTime(dTurningTime, dOnSettingTime, dOffSettingTime);

	if (!bSkipSensor) {
		T_TimeOut.StartTimer();
		while (1) {
			Sleep(15);
			nVacOnCnt=0;
			for (int i=0; i<nVacCount; i++) {
				if (pVac[i]->IsOn()) nVacOnCnt++;
			}
			if (nVacOnCnt == nVacCount)
				break;

			if (T_TimeOut.MoreThan(dTurningTime)) {
				for (int i=0; i<nVacCount; i++) {
					if (!pVac[i]->IsOn()) {
						iResult = 102001*1000 + pVac[i]->GetInstanceNo();
						for (int j=0; j<nVacCount; j++) {
							pVac[j]->Off();
						}
						return iResult;
					}
				}
				nVacOnCnt = 0;
			}
		}
		Sleep(DWORD(dOnSettingTime*1000));
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Absorb Panel (SkipMode:%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::ReleasePanel(BOOL bSkipSensor)
{
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Release Panel (SkipMode:%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	/*/
	int nVacCount = 0;
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_40INCH - 10.0)			nVacCount = 1;
	else if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)	nVacCount = 2;
	else																		nVacCount = 3;
#else
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)	nVacCount = 2;
	else																nVacCount = 3;
#endif
	/*/
	int nVacCount = 3;	//@모두 해제

#ifdef SIMULATION
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif

	IVacuum* pVac[3] = {m_plnkVac1, m_plnkVac2, m_plnkVac3};
	for (int i=0; i<nVacCount; i++) {
		pVac[i]->Off(TRUE);
	}

	int nVacOffCnt = 0;
	MTickTimer T_TimeOut;
	double dTurningTime, dOnSettingTime, dOffSettingTime;
	pVac[0]->GetVacuumTime(dTurningTime, dOnSettingTime, dOffSettingTime);

	if (!bSkipSensor) {
		T_TimeOut.StartTimer();
		while (1) {
			Sleep(15);
			for (int i=0; i<nVacCount; i++) {
				if (pVac[i]->IsOff()) nVacOffCnt++;
			}
			if (nVacOffCnt == nVacCount) break;

			if (T_TimeOut.MoreThan(dTurningTime)) {
				for (int i=0; i<nVacCount; i++) {
					if (!pVac[i]->IsOff()) {
						iResult = 102002*1000 + pVac[i]->GetInstanceNo();
						for (int j=0; j<nVacCount; j++) {
							pVac[j]->Off();
						}
						return iResult;
					}
				}
			}
			nVacOffCnt = 0;
		}
		Sleep(DWORD(dOnSettingTime));
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Release Panel (SkipMode:%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::CheckVacuum(void)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_40INCH - 10.0 )
	{
		return m_plnkVac1->CompareIO();
	}
	else if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)
	{
		iResult = m_plnkVac1->CompareIO();
		if (iResult)
			return iResult;
		iResult = m_plnkVac2->CompareIO();
		if (iResult)
			return iResult;
	}
	else
	{
		iResult = m_plnkVac1->CompareIO();
		if (iResult)
			return iResult;
		iResult = m_plnkVac2->CompareIO();
		if (iResult)
			return iResult;		
		iResult = m_plnkVac3->CompareIO();
		if (iResult)
			return iResult;	
	}
#else
	/*/
	iResult = m_plnkVac1->CompareIO();
	if (iResult)
		return iResult;
	iResult = m_plnkVac2->CompareIO();
	if (iResult)
		return iResult;		
	iResult = m_plnkVac3->CompareIO();
	if (iResult)
		return iResult;	
//@#endif

	return ERR_PANEL_ALIGNER_SUCCESS;
}

const BOOL MPanelAligner::IsPanelAbsorbed(void)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_40INCH - 10.0) {
		return m_plnkVac1->IsOn();
	}
	else if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0) {
		return (m_plnkVac1->IsOn() || m_plnkVac2->IsOn());
	}
	else {
		return (m_plnkVac1->IsOn() || m_plnkVac2->IsOn() || m_plnkVac3->IsOn());
	}
#else
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)
	{
		return (m_plnkVac1->IsOn() || m_plnkVac2->IsOn());
	}
	else
	{
		return (m_plnkVac1->IsOn() || m_plnkVac2->IsOn() || m_plnkVac3->IsOn());
	}
#endif
	return ERR_PANEL_ALIGNER_SUCCESS;
	/*/
	return (m_plnkVac1->IsOn() || m_plnkVac2->IsOn() || m_plnkVac3->IsOn());
}

const BOOL MPanelAligner::IsPanelReleased(void)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_40INCH - 10.0 )
	{
		return m_plnkVac1->IsOff();
	}
	else if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)
	{
		return (m_plnkVac1->IsOff() && m_plnkVac2->IsOff());
	}
	else
	{
		return (m_plnkVac1->IsOff() && m_plnkVac2->IsOff() && m_plnkVac3->IsOff());
	}
#else
	if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_52INCH - 10.0)
	{
		return (m_plnkVac1->IsOff() && m_plnkVac2->IsOff());
	}
	else
	{
		return (m_plnkVac1->IsOff() && m_plnkVac2->IsOff() && m_plnkVac3->IsOff());
	}
#endif
	return ERR_PANEL_ALIGNER_SUCCESS;
	/*/
	return (m_plnkVac1->IsOff() && m_plnkVac2->IsOff() && m_plnkVac3->IsOff());
}

/*/
int MPanelAligner::AbsorbPanelRotator(BOOL bSkipSensor)
{
	/ ** Vacuum Off되어 있는지 확인 * /
	if (TRUE == IsPanelAbsorbedRotator())
		return ERR_PANEL_ALIGNER_SUCCESS;
	
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Absorb Panel Rotator (SkipMode:%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	iResult = m_plnkVacRotator->On(bSkipSensor);


	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Absorb Panel Rotator (SkipMode:%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

/*/
int MPanelAligner::ReleasePanelRotator(BOOL bSkipSensor)
{
	/ ** Vacuum Off되어 있는지 확인 * /
	if (TRUE == IsPanelReleasedRotator())
		return ERR_PANEL_ALIGNER_SUCCESS;
	
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Release Panel Rotator (SkipMode:%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;
	iResult = m_plnkVacRotator->Off(bSkipSensor);


	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Release Panel Rotator (SkipMode:%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

/*/
int MPanelAligner::CheckVacuumRotator(void)
{
//@	return m_plnkVacRotator->CompareIO();
	return ERR_PANEL_ALIGNER_SUCCESS;
}

const BOOL MPanelAligner::IsPanelAbsorbedRotator(void)
{
//@	return m_plnkVacRotator->IsOn();
	return ERR_PANEL_ALIGNER_SUCCESS;
}

const BOOL MPanelAligner::IsPanelReleasedRotator(void)
{
//@	return m_plnkVacRotator->IsOff();
	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

int MPanelAligner::SafeMoveXYTPos(int iPosID, BOOL bUseAuto)
{
	MPos_XYT posTarget = GetXYTTargetPos(iPosID);	// 목표 위치 읽기
	return MoveXYTPos(posTarget, iPosID, bUseAuto);	// 해당 위치로 이동
}

int MPanelAligner::SafeSyncMoveXYTPos(int iPosID, BOOL bUseAuto)
{
	MPos_XYT posTarget = GetXYTTargetPos(iPosID);	// 목표 위치 읽기
	return SyncMoveXYTPos(posTarget, iPosID, bUseAuto);	// 해당 위치로 이동
}

int MPanelAligner::SafeMoveZPos(int iPosID, BOOL bUseAuto, BOOL bWaitOption)
{
	double	posTarget = GetZTargetPos(iPosID);
	return MoveZPos(posTarget, iPosID, bUseAuto, bWaitOption);
}

int MPanelAligner::MoveZPos(double posTarget, int iPosID, BOOL bUseAuto, BOOL bWaitOption)
{
	CString strLogMsg;
	ASSERT(iPosID >= DEF_PANEL_ALIGNER_Z_NONE_POS && iPosID < DEF_PANEL_ALIGNER_Z_MAX_POSITION);

#ifdef SIMULATION
	Sleep(500);
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif

	if (IsInZPos(posTarget))
		return ERR_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Move Z to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult;
	if (iPosID != DEF_PANEL_ALIGNER_Z_NONE_POS)
	{
		iResult = checkInterlockForZ(iPosID, posTarget, bUseAuto);
		if (iResult != ERR_PANEL_ALIGNER_SUCCESS)
			return iResult;
	}

	double posDiff = posTarget - m_plnkZ->GetCurrentPosition(FALSE);
	if (fabs(posDiff) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		double dNormalVel = m_plnkZ->GetMovingVelocity();
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkZ->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / DEF_DIVIDER_FOR_LOW_SPEED;

		iResult = m_plnkZ->StartMove(posTarget, dLowVel);
	}
	else
		iResult = m_plnkZ->StartMove(posTarget);

	if (iResult != ERR_PANEL_ALIGNER_SUCCESS)
		return iResult;

	Sleep(10);	// WaitOption == FALSE 일 경우를 대비한 Motion 동작 시작 후 안정화 Delay. 

	if (FALSE == bWaitOption)
	{
#ifndef SIMULATION
		if (TRUE == m_plnkZ->IsAmpFault() || FALSE == m_plnkZ->IsAmpEnable())
		{
			SetErrorLevel(_T("PANEL ALIGNER move Z Position"),2, __FILE__, __LINE__);
			return generateErrorCode(210073);
		}
#endif
	}
	else
	{
		iResult = m_plnkZ->Wait4Done();// Wait4DoneZ();
		if (iResult)
			return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Move Z to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::Wait4DoneZ(BOOL bSkipMode)
{
	return m_plnkZ->Wait4Done(bSkipMode);
}	

/*/
int MPanelAligner::MoveBackupMoveXPos(double posTarget, int iPosID, BOOL bUseAuto)
{
	CString strLogMsg;
	ASSERT(iPosID >= DEF_BACKUP_MOVE_X_NONE_POS && iPosID < DEF_BACKUP_MOVE_X_MAX_POSITION);

#ifdef SIMULATION
	Sleep(500);
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif

	if (IsInBackupMoveXPos(posTarget))
		return ERR_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Move BackupX to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult;
	if (iPosID != DEF_BACKUP_MOVE_X_NONE_POS)
	{
		iResult = checkInterlockForBackupMoveX(iPosID, posTarget, bUseAuto);
		if (iResult != ERR_PANEL_ALIGNER_SUCCESS)
		{
			SetErrorLevel(_T("PANEL ALIGNER move BackupMoveX Position"),2, __FILE__, __LINE__);
			return iResult;
		}
	}

	double posDiff = posTarget - m_plnkBackupMoveX->GetCurrentPosition(FALSE);
	if (fabs(posDiff) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		double dNormalVel = m_plnkBackupMoveX->GetMovingVelocity();
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkBackupMoveX->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / DEF_DIVIDER_FOR_LOW_SPEED;

		iResult = m_plnkBackupMoveX->StartMove(posTarget, dLowVel);
	}
	else
		iResult = m_plnkBackupMoveX->StartMove(posTarget);

	if (iResult != ERR_PANEL_ALIGNER_SUCCESS)
	{
		SetErrorLevel(_T("PANEL ALIGNER move BackupMoveX Position"),2, __FILE__, __LINE__);
		return iResult;
	}

	Sleep(10);	// WaitOption == FALSE 일 경우를 대비한 Motion 동작 시작 후 안정화 Delay. 

//	if (FALSE == bWaitOption)
//	{
//#ifndef SIMULATION
//		if (TRUE == m_plnkZ->IsAmpFault() || FALSE == m_plnkZ->IsAmpEnable())
//		{
//			SetErrorLevel(_T("PANEL ALIGNER move Z Position"),2, __FILE__, __LINE__);
//			return generateErrorCode(210073);
//		}
//#endif
//	}
//	else
	{
		iResult = m_plnkBackupMoveX->Wait4Done();// Wait4DoneZ();
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER move BakcupmoveX Position"),2, __FILE__, __LINE__);
			return iResult;
		}
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Move backupmoveX to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::SafeMoveBackupMoveXPos(int iPosID, BOOL bUseAuto)
{
#ifdef DEF_SOURCE_SYSTEM
	double posTarget = GetBackupMoveXTargetPos(iPosID);
	return MoveBackupMoveXPos(posTarget, iPosID, bUseAuto);
#else
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif
}
/*/

void MPanelAligner::InitAlignMarkOffset(void)
{
	m_bMarkAligned = FALSE;
	m_rgAlignMarkOffset.Init(0.0, 0.0, 0.0);
}

MPos_XYT MPanelAligner::CalculateAlignMarkOffset(MPos_XYT posxytTarget)
{
	MPos_XYT rgdFiduMark;

	if (m_bMarkAligned)
	{
		rgdFiduMark.dT = DEG2RAD(posxytTarget.dT - m_dValueThetaAlign);	// T차이값 계산

		// X축 보정값 계산
		rgdFiduMark.dX = cos(rgdFiduMark.dT) * m_rgAlignMarkOffset.dX 
			             - sin(rgdFiduMark.dT) * m_rgAlignMarkOffset.dY; 
		
		// Y축 보정값 계산
		rgdFiduMark.dY = sin(rgdFiduMark.dT) * m_rgAlignMarkOffset.dX 
			             + cos(rgdFiduMark.dT) * m_rgAlignMarkOffset.dY; 

		// T축 보정값
		rgdFiduMark.dT = m_rgAlignMarkOffset.dT;
	}
	else
	{
		rgdFiduMark.Init(0.0,0.0,0.0);
	}

	return rgdFiduMark;
}

void MPanelAligner::SetAlignMarkOffset(MPos_XYT sPos, double dValueThetaAlign)
{
	// Align Mark Offset 설정 
	m_rgAlignMarkOffset.Init(sPos.dX, sPos.dY, sPos.dT);
	m_dValueThetaAlign = dValueThetaAlign;

	m_bMarkAligned = TRUE;
}

MPos_XYT MPanelAligner::GetAlignMarkOffset()
{
	MPos_XYT psPos;

	if (m_bMarkAligned == TRUE)
	{
		// Align Mark OFfset 값 전달
		psPos.Init(m_rgAlignMarkOffset.dX, m_rgAlignMarkOffset.dY, m_rgAlignMarkOffset.dT);
	}
	else
	{
		psPos.Init(0.0, 0.0, 0.0);	// Align Mark Offset 값 0.0 전달 
	}

	return psPos;
}

BOOL MPanelAligner::IsAligned()
{
	return m_bMarkAligned;
}

//void MPanelAligner::SetPanelCenterOffset(MPos_XYT sPos)
//{
//	m_rgPanelCenterOffset = sPos;
//}

/*
//Align위치에서 Stage중심에서 Panel중심까지의 거리를 구함
void MPanelAligner::CalculatePanelCenterOffset()
{
	double dTDeg = -m_rgAlignMarkOffset.dT;
	double dTRad = DEG2RAD(dTDeg);
	MPos_XYT EccentricPos(0.0, 0.0, 0.0);
	EccentricPos.dX = MOlbSystemPre.GetEccentricDist_StageToPanel_X();
	EccentricPos.dX -= m_rgAlignMarkOffset.dX;
	EccentricPos.dY -= m_rgAlignMarkOffset.dY;
	m_rgPanelCenterOffset.dX = cos(dTRad)*EccentricPos.dX - sin(dTRad)*EccentricPos.dY;
	m_rgPanelCenterOffset.dY = sin(dTRad)*EccentricPos.dX + cos(dTRad)*EccentricPos.dY;
}

MPos_XYT MPanelAligner::GetPanelCenterOffset()
{
	return m_rgPanelCenterOffset;
}

//Compare위치에서 Aligner중심이 Panel중심에 위치하는 좌표를 구한다
MPos_XYT MPanelAligner::GetPanelCenterPosition(MPos_XYT ComparePos)
{
//	MPos_XYT CurPos = GetXYTCurrentPos();
	CalculatePanelCenterOffset();
	double dT = DEG2RAD(ComparePos.dT - m_dValueThetaAlign);
	MPos_XYT CenterOffset = GetPanelCenterOffset();
	MPos_XYT CenterOffset2;
	CenterOffset2.dX = cos(dT)*CenterOffset.dX - sin(dT)*CenterOffset.dY;
	CenterOffset2.dY = sin(dT)*CenterOffset.dX + cos(dT)*CenterOffset.dY;
	MPos_XYT CenterPos;
	CenterPos.dX = ComparePos.dX + CenterOffset2.dX;
	CenterPos.dY = ComparePos.dY + CenterOffset2.dY;
	CenterPos.dT = ComparePos.dT;
	return CenterPos;
}

MPos_XYT MPanelAligner::GetPanelRotationPosition()
{
	MPos_XYT AlignPos = GetXYTTargetPos(DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS);
	MPos_XYT CenterPos = GetPanelCenterPosition(AlignPos);
	MPos_XYT RotatePos = ConvertPanelAlignerToRotator(CenterPos);
	return RotatePos;
}

//PanelAligner의 좌표를 Rotator실린더 중심위치로 변경한다.
MPos_XYT MPanelAligner::ConvertPanelAlignerToRotator(MPos_XYT PanelAlignerTgPos)
{
	MPos_XYT TgPos;
	TgPos.dX = PanelAlignerTgPos.dX - m_plnkSystemData->m_dDist_AlignerToRotator_X;
	TgPos.dY = PanelAlignerTgPos.dY - m_plnkSystemData->m_dDist_AlignerToRotator_Y;
	TgPos.dT = 0.0;
	return TgPos;
}

//Rotator실린더 중심위치를 PanelAligner의 좌표로 변경한다.
MPos_XYT MPanelAligner::ConvertRotatorToPanelAligner(MPos_XYT RotatorTgPos)
{
	MPos_XYT TgPos;
	TgPos.dX = RotatorTgPos.dX + m_plnkSystemData->m_dDist_AlignerToRotator_X;
	TgPos.dY = RotatorTgPos.dY + m_plnkSystemData->m_dDist_AlignerToRotator_Y;
	TgPos.dT = 0.0;
	return TgPos;
}
*/

int MPanelAligner::CheckOrigin(void)
{
	if (m_plnkX->IsOriginReturn() == FALSE)
		// 210005 = X축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210005);

	if (m_plnkY->IsOriginReturn() == FALSE)
		// 210006 = Y축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210006);

	if (m_plnkT->IsOriginReturn() == FALSE)
		// 210007 = T축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210007);

	if (m_plnkZ->IsOriginReturn() == FALSE)
		// 210007 = T축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210071);

	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkBackupMoveX->IsOriginReturn() == FALSE)
	{
		SetErrorLevel(_T("PANEL ALIGNER check Origin"),2, __FILE__, __LINE__);
		// 210007 = T축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210072);
	}
#endif
	/*/

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::CheckOriginZ()
{
	if (m_plnkZ->IsOriginReturn() == FALSE)
		// 210007 = T축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210071);
	return ERR_PANEL_ALIGNER_SUCCESS;
}

/*/
int MPanelAligner::CheckOriginBackupMoveX()
{
	if (m_plnkBackupMoveX->IsOriginReturn() == FALSE)
	{
		SetErrorLevel(_T("PANEL ALIGNER check Origin"),2, __FILE__, __LINE__);
		// 210007 = T축 원점복귀되어 있지 않습니다.
		return generateErrorCode(210072);
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}
/*/

MPos_XYT MPanelAligner::GetXYTCurrentPos(BOOL bType)
{
	ASSERT(m_plnkX != NULL && m_plnkY != NULL && m_plnkT != NULL);

	MPos_XYT sCurPos;

	sCurPos.dX = m_plnkX->GetCurrentPosition(bType);
	sCurPos.dY = m_plnkY->GetCurrentPosition(bType);
	double dCurrentY2 = m_plnkT->GetCurrentPosition(bType);
	sCurPos.dT = convertY2toT(dCurrentY2 - sCurPos.dY);

	return sCurPos;
}

double MPanelAligner::GetZCurrentPos(BOOL bType)
{
	ASSERT(m_plnkZ != NULL);
	double dCurPos = m_plnkZ->GetCurrentPosition(bType);
	return dCurPos;
}

/*/
double MPanelAligner::GetBackupMoveXCurrentPos(BOOL bType)
{
	ASSERT(m_plnkBackupMoveX != NULL);
	double dCurPos = m_plnkBackupMoveX->GetCurrentPosition(bType);
	return dCurPos;	
}
/*/

MPos_XYT MPanelAligner::GetXYTTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_PANEL_ALIGNER_MAX_POSITION);

	MPos_XYT posTarget;

	// 목표좌표 = 고정 좌표	+ 모델좌표 + Offset 좌표 + Etc Offset 좌표
	posTarget.dX = m_rgsXYTFixedPos[iPosID].dX
					+ m_rgsXYTModelPos[iPosID].dX
					+ m_rgsXYTOffsetPos[iPosID].dX;
					//+ m_rgsXYTEtcOffsetPos[iPosID].dX;
	posTarget.dY = m_rgsXYTFixedPos[iPosID].dY
					+ m_rgsXYTModelPos[iPosID].dY
					+ m_rgsXYTOffsetPos[iPosID].dY;
					//+ m_rgsXYTEtcOffsetPos[iPosID].dY;
	posTarget.dT = m_rgsXYTFixedPos[iPosID].dT
					+ m_rgsXYTModelPos[iPosID].dT
					+ m_rgsXYTOffsetPos[iPosID].dT;
					//+ m_rgsXYTEtcOffsetPos[iPosID].dT;

	// Align이 수행되어 있으면
	if (IsAligned())
	{
		// LOAD POS에서는 Mark Offset을 반영하지 않는다.
		if (iPosID != DEF_PANEL_ALIGNER_LOAD_POS)
		{
			MPos_XYT posAlignOffset = CalculateAlignMarkOffset(posTarget);
			posTarget = posTarget + posAlignOffset;
		}
	}

	return posTarget;
}

double MPanelAligner::GetZTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_PANEL_ALIGNER_Z_MAX_POSITION);
	double posTarget = 	m_rgdZFixedPos[iPosID] + m_rgdZOffsetPos[iPosID] + m_rgdZModelPos[iPosID];
	return posTarget;
}

/*/
double MPanelAligner::GetBackupMoveXTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_BACKUP_MOVE_X_MAX_POSITION);
	double posTarget = 	m_rgdBackupXFixedPos[iPosID] + m_rgdBackupXOffsetPos[iPosID] + m_rgdBackupXModelPos[iPosID];
	return posTarget;
}
/*/

BOOL MPanelAligner::IsInXYTPos(int iPosID, MPos_XYT* pTolerance)
{
	MPos_XYT posCompare = GetXYTTargetPos(iPosID);

	return IsInXYTPos(posCompare, pTolerance);
}

BOOL MPanelAligner::IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance)
{
	BOOL bState[DEF_PANEL_ALIGNER_AXIS_MAX_NO];

	if (pTolerance != NULL)
	{
		bState[DEF_PANEL_ALIGNER_X_AXIS] = m_plnkX->ComparePosition(FALSE , sTargetPos.dX,&pTolerance->dX);
		bState[DEF_PANEL_ALIGNER_Y_AXIS] = m_plnkY->ComparePosition(FALSE , sTargetPos.dY, &pTolerance->dY);
		double dTargetPosY2 = convertTtoY2(sTargetPos.dT) + sTargetPos.dY;
		bState[DEF_PANEL_ALIGNER_T_AXIS] = m_plnkT->ComparePosition(FALSE , dTargetPosY2, &pTolerance->dT);
	}
	else
	{
		bState[DEF_PANEL_ALIGNER_X_AXIS] = m_plnkX->ComparePosition(FALSE , sTargetPos.dX, NULL);
		bState[DEF_PANEL_ALIGNER_Y_AXIS] = m_plnkY->ComparePosition(FALSE , sTargetPos.dY,  NULL);
		double dTargetPosY2 = convertTtoY2(sTargetPos.dT) + sTargetPos.dY;
		bState[DEF_PANEL_ALIGNER_T_AXIS] = m_plnkT->ComparePosition(FALSE , dTargetPosY2, NULL);
	}

	return bState[DEF_PANEL_ALIGNER_X_AXIS] && bState[DEF_PANEL_ALIGNER_Y_AXIS] && bState[DEF_PANEL_ALIGNER_T_AXIS];
}

BOOL MPanelAligner::IsInXPos(int iPosID, double* pdTolerance)
{
	MPos_XYT posCompare = GetXYTTargetPos(iPosID);

	return IsInXPos(posCompare.dX, pdTolerance);
}

BOOL MPanelAligner::IsInXPos(double dTargetPos, double* pdTolerance)
{
	return m_plnkX->ComparePosition(FALSE , dTargetPos, pdTolerance);
}

BOOL MPanelAligner::IsInYPos(int iPosID, double* pdTolerance)
{
	MPos_XYT posCompare = GetXYTTargetPos(iPosID);

	return IsInYPos(posCompare.dY, pdTolerance);
}

BOOL MPanelAligner::IsInYPos(double dTargetPos, double* pdTolerance)
{
	return m_plnkY->ComparePosition(FALSE , dTargetPos, pdTolerance);
}

BOOL MPanelAligner::IsInTPos(int iPosID, double* pdTolerance)
{
	MPos_XYT posCompare = GetXYTTargetPos(iPosID);

	return IsInTPos(posCompare.dT, pdTolerance);
}

BOOL MPanelAligner::IsInTPos(double dTargetPos, double* pdTolerance)
{
	double dTargetPosY2 = convertTtoY2(dTargetPos);

	return m_plnkT->ComparePosition(FALSE , dTargetPosY2, pdTolerance);
}

BOOL MPanelAligner::IsInZPos(int iPosID, double *pTolerance)
{
	double posCompare = GetZTargetPos(iPosID);
	return IsInZPos(posCompare, pTolerance);
}

BOOL MPanelAligner::IsInZPos(double dTargetPos, double *pTolerance)
{
	return m_plnkZ->ComparePosition(FALSE, dTargetPos, pTolerance);
}

/*/
BOOL MPanelAligner::IsInBackupMoveXPos(int iPosID, double *pTolerance)
{
	double posCompare = GetBackupMoveXTargetPos(iPosID);
	return IsInBackupMoveXPos(posCompare, pTolerance);
}

BOOL MPanelAligner::IsInBackupMoveXPos(double dTargetPos, double *pTolerance)
{
	return m_plnkBackupMoveX->ComparePosition(FALSE, dTargetPos, pTolerance);
}
/*/


BOOL MPanelAligner::IsInXYTMotion()
{
#ifdef SIMULATION
	return TRUE;
#endif

	return (m_plnkX->IsInMotion() || m_plnkY->IsInMotion() || m_plnkT->IsInMotion());
}

int MPanelAligner::SetData(SPanelAlignerData datComponent)
{
	m_sXYTZTeachingInfo			= datComponent.m_sXYTZTeachingInfo;
	m_sZTeachingInfo			= datComponent.m_sZTeachingInfo;
//@	m_sBackupMoveXTeachingInfo	= datComponent.m_sBackupMoveXTeachingInfo;
	m_strPosFileName			= datComponent.m_strPosFileName;
	m_strOffsetPosFileName		= datComponent.m_strOffsetPosFileName;
	m_plnkSystemData			= datComponent.m_plnkSystemData;
	m_plnkPanelData				= datComponent.m_plnkPanelData;
	m_plnkPreBonderData			= datComponent.m_plnkPreBonderData;
	m_plnkTabData				= datComponent.m_plnkTabData;

	m_usIPanelDetect			= datComponent.m_usIPanelDetect;

	m_usOAddrAirBlow1			= datComponent.m_usOAddrAirBlow1;
	m_usOAddrAirBlow2			= datComponent.m_usOAddrAirBlow2;
//@	m_usOAddrAirBlow3			= datComponent.m_usOAddrAirBlow3;
//@	m_usOAddrAirBlow4			= datComponent.m_usOAddrAirBlow4;

	m_usIAddrAirBlow1			= datComponent.m_usIAddrAirBlow1;
	m_usIAddrAirBlow2			= datComponent.m_usIAddrAirBlow2;
//@	m_usIAddrAirBlow3			= datComponent.m_usIAddrAirBlow3;
//@	m_usIAddrAirBlow4			= datComponent.m_usIAddrAirBlow4;

	m_usOAddrAirVacuum1			= datComponent.m_usOAddrAirVacuum1;
	m_usOAddrAirVacuum2			= datComponent.m_usOAddrAirVacuum2;
	m_usOAddrAirVacuum3			= datComponent.m_usOAddrAirVacuum3;
	m_usOAddrAirVacuum4			= datComponent.m_usOAddrAirVacuum4;

	m_usIAddrAirVacuum1			= datComponent.m_usIAddrAirVacuum1;
	m_usIAddrAirVacuum2			= datComponent.m_usIAddrAirVacuum2;
	m_usIAddrAirVacuum3			= datComponent.m_usIAddrAirVacuum3;
	m_usIAddrAirVacuum4			= datComponent.m_usIAddrAirVacuum4;

	//120224.KMS__________
	m_usIPanelDetectLoading			= datComponent.m_usIPanelDetectLoading;
	m_usIPanelDetectWorking			= datComponent.m_usIPanelDetectWorking;
	//____________________

	//120508.kms__________
	m_usIIonizerState           = datComponent.m_usIIonizerState;
	
	//170724_KDH Spot Cleaner 추가
	m_usOAddrSpotCleanerOn      = datComponent.m_usOAddrSpotCleanerOn;
	m_usIAddrSpotCleanerState   = datComponent.m_usIAddrSpotCleanerState;
	//___________________
	

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AssignStateCheckComponents(SStateCheckCompListForPanelAligner sCompList)
{
#ifdef DEF_SOURCE_SYSTEM
	m_plnkStatePanelTransfer	= sCompList.m_plnkStatePanelTransfer;
#endif
	m_plnkStatePanelTransferOut	= sCompList.m_plnkStatePanelTransferOut;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	/** Tab Mounter */
	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		m_plnkStateTabMounter[i] = sCompList.m_plnkStateTabMounter[i];
		m_plnkStateRTabMounter[i] = sCompList.m_plnkStateRTabMounter[i];
	}
	for (int i = 0 ; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_plnkStateInspectCamCarrier[i] = sCompList.m_plnkStateInspectCamCarrier[i];
		m_plnkStateRInspectCamCarrier[i] = sCompList.m_plnkStateRInspectCamCarrier[i];
	}

#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	// ARP
	/** Carrier1 */
	m_plnkStateCarrier1 = sCompList.m_plnkStateCarrier1;
	/** Carrier2 */
	m_plnkStateCarrier2 = sCompList.m_plnkStateCarrier2;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	// Final
	m_plnkStateMainTool1 = sCompList.m_plnkStateMainTool1;
	m_plnkStateMainTool2 = sCompList.m_plnkStateMainTool2;
#endif

	m_plnkStateModelChange1	= sCompList.m_plnkStateModelChange1;
	m_plnkStateModelChange2	= sCompList.m_plnkStateModelChange2;

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::ChangeModel()
{
	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
		m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);

	LoadXYTFixedPosParameter();				// XYT 고정 좌표 Load
	LoadXYTOffsetPosParameter();			// XYT 모델 Offset Load	
//	LoadPanelTransferXFixedPosParameter();	// PanelTransferX   고정 좌표 Load
	calculatePanelAlignerXYTModelPosParameter();

#ifdef DEF_SOURCE_SYSTEM
//@	LoadBackupMoveXFixedPosParameter();
//@	LoadBackupMoveXOffsetPosParameter();
#endif

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::Initialize()
{
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::MoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bUseAuto, BOOL bOnMarkAutoSearch)
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif
	
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Move XYT to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult;
	if (iPosID != DEF_PANEL_ALIGNER_NONE_POS)
	{
		iResult	= checkInterlockForXYT(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;
	}
	
	// Load Position으로 이동 시 Align Offset Clear
	if (iPosID == DEF_PANEL_ALIGNER_LOAD_POS)
		InitAlignMarkOffset();

	double dPosT = posTarget.dT;
	MPos_XYT posCurrent = GetXYTCurrentPos();
	MPos_XYT posDiff = posTarget - GetXYTCurrentPos();
//	double dTargetX2 = convertTtoX2(posTarget.dT);
	double dTargetY2 = convertTtoY2(posTarget.dT);

	double dNormalVel = m_plnkX->GetMovingVelocity();

	double dDistance4LowVel = (double)m_plnkSystemData->m_iDistance4DifVel;
	if (TRUE == bOnMarkAutoSearch)
	{
		dDistance4LowVel = 4 * m_plnkSystemData->m_dAutoSearchPanelMarkDistance + 1.0;
	}
	
	// X, Y, T축 목표위치로 이동
	// T축이 Turn 위치에서 이동한 상태이면 X,Y,T가 동시 동작을 수행해도 간섭이 없음. 
//	if (fabs(posDiff.dX) < dDistance4LowVel)
	if (fabs(posDiff.dY) < dDistance4LowVel)
	{
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			//dNormalVel = m_plnkX->GetSlowMovingVelocity();
			dNormalVel = m_plnkY->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / 10.0;
		
//		iResult = m_plnkX->StartMove(posTarget.dX, dLowVel);
		iResult = m_plnkY->StartMove(posTarget.dY, dLowVel);
		if (iResult)
			return iResult;

//		iResult = m_plnkT->StartMove(dTargetX2 + posTarget.dX, dLowVel);
		iResult = m_plnkT->StartMove(dTargetY2 + posTarget.dY, dLowVel);
		if (iResult)
		{
			//m_plnkX->Stop();
			m_plnkY->Stop();
			return iResult;
		}
	}
	else
	{
//		iResult = m_plnkX->StartMove(posTarget.dX, dNormalVel);
		iResult = m_plnkY->StartMove(posTarget.dY, dNormalVel);
		if (iResult)
			return iResult;

		// T 축은 X 축과 동일 속도를 써야 하므로...
//		iResult = m_plnkT->StartMove(dTargetX2 + posTarget.dX, dNormalVel);
		iResult = m_plnkT->StartMove(dTargetY2 + posTarget.dY, dNormalVel);
		if (iResult)
		{
			//m_plnkX->Stop();
			m_plnkY->Stop();
			return iResult;
		}
	}

//	if (fabs(posDiff.dY) < dDistance4LowVel
	if (fabs(posDiff.dX) < dDistance4LowVel
		&& fabs(posDiff.dT) < DEF_PANEL_ALIGNER_TURN_POS_T_TOLERANCE) 
	{
		//dNormalVel = m_plnkY->GetMovingVelocity();
		dNormalVel = m_plnkX->GetMovingVelocity();
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			//dNormalVel = m_plnkY->GetSlowMovingVelocity();
			dNormalVel = m_plnkX->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / 10.0;
		
//		iResult = m_plnkY->StartMove(posTarget.dY, dLowVel);
		iResult = m_plnkX->StartMove(posTarget.dX, dLowVel);
	}
	else
	{
		double dVel = 0.0;
		if (iPosID == DEF_PANEL_ALIGNER_LOAD_POS && TRUE == bUseAuto && FALSE == IsPanelAbsorbed())
			dVel = 1000.0;
//		iResult = m_plnkY->StartMove(posTarget.dY);
		iResult = m_plnkX->StartMove(posTarget.dX, dVel);	
	}

	if (iResult)
	{
//		m_plnkX->Stop();
		m_plnkY->Stop();
		m_plnkT->Stop();
		return iResult;
	}

//	iResult = m_plnkX->Wait4Done(TRUE);
	iResult = m_plnkY->Wait4Done(TRUE);
	if (iResult)
	{
//		m_plnkY->Stop();
		m_plnkX->Stop();
		m_plnkT->Stop();
		return iResult;
	}
	
//	iResult = m_plnkY->Wait4Done(TRUE);
	iResult = m_plnkX->Wait4Done(TRUE);
	if (iResult)
	{
		m_plnkT->Stop();
		return iResult;
	}

	iResult = m_plnkT->Wait4Done();
	if (iResult)
		return iResult;
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Move XYT to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::SyncMoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bUseAuto, BOOL bOnMarkAutoSearch)
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif
	
	// jdy sesl
//	char szLogMsg[200];
	/*memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [SyncMove XYT to PosID : %d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);
*/
	int iResult;
	if (iPosID != DEF_PANEL_ALIGNER_NONE_POS)
	{
		iResult	= checkInterlockForXYT(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;
	}

	// Load Position으로 이동 시 Align Offset Clear
	if (iPosID == DEF_PANEL_ALIGNER_LOAD_POS)
	{
		InitAlignMarkOffset();
	}

	double dPosT = posTarget.dT;
	MPos_XYT posCurrent = GetXYTCurrentPos();
	MPos_XYT posDiff = posTarget - GetXYTCurrentPos();
	double dTargetY2 = convertTtoY2(posTarget.dT);
	double dDiffY2 = convertTtoY2(posTarget.dT) - convertTtoY2(posCurrent.dT);
	double rgdTarget[2] = { posTarget.dY, posTarget.dY + dTargetY2 };
	double rgdDistance[2] = { posDiff.dY, posDiff.dY + dDiffY2 };

	// hongju_20090731 : Press 위치로 이동시 속도가 너무 빠르면 Slip 현상이 발생하므로 이때는 최대속도가 800mm 이하로 유지한다.
	double dNormalVel = 0.0;

	/*
	if ((DEF_PANEL_ALIGNER_PRESS_XU_POS == iPosID)
		|| (DEF_PANEL_ALIGNER_PRESS_YL_POS == iPosID)
		|| (DEF_PANEL_ALIGNER_PRESS_YR_POS == iPosID))
	*/
	if (DEF_PANEL_ALIGNER_INSPECT_1_POS == iPosID
		|| DEF_PANEL_ALIGNER_INSPECT_2_POS == iPosID
		|| DEF_PANEL_ALIGNER_INSPECT_3_POS == iPosID)
	{
		if (m_plnkY->GetMovingVelocity() > 800.0)
		{
			dNormalVel = 800.0 * 1.414;
		}
		else
		{
			dNormalVel = m_plnkSystemData->m_dPanelAlignerWorkVelocity * 1.414;
			if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode) {
				dNormalVel = m_plnkX->GetSlowMovingVelocity() * 1.414;
			}
		}
	}
	else
	{
		//dNormalVel = m_plnkX->GetMovingVelocity() * 1.414;
		dNormalVel = m_plnkY->GetMovingVelocity() * 1.414;
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkY->GetSlowMovingVelocity() * 1.414;
	}

	double dLowVel = dNormalVel / 10.0;
	double rgdNormalVel[2] = { dNormalVel, dNormalVel };
	double rgdLowVel[2] = { dLowVel, dLowVel };

	double dDistance4LowVel = (double)m_plnkSystemData->m_iDistance4DifVel;
	if (TRUE == bOnMarkAutoSearch)
	{
		dDistance4LowVel = 4 * m_plnkSystemData->m_dAutoSearchPanelMarkDistance + 1.0;
	}

	// Multi XT 축 목표위치로 이동
	if (fabs(posDiff.dY) < dDistance4LowVel)
	{
		iResult = m_plnkMultiYT->StartMove(rgdTarget, rgdLowVel);
	}
	else
	{
		iResult = m_plnkMultiYT->StartMove(rgdTarget, rgdNormalVel);
	}
	if (iResult)
		return iResult;

	// Y축 이동 ---> X축 이동
	if (fabs(posDiff.dX) < dDistance4LowVel
		&& fabs(posDiff.dT) < DEF_PANEL_ALIGNER_TURN_POS_T_TOLERANCE) 
	{
		dNormalVel = m_plnkX->GetMovingVelocity() * 1.414;
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkX->GetSlowMovingVelocity() * 1.414;
		
		dLowVel = dNormalVel / 10.0;
		
		iResult = m_plnkX->StartMove(posTarget.dX, dLowVel);
	}
	else
	{
		double dVel = 0.0;
		if (iPosID == DEF_PANEL_ALIGNER_LOAD_POS && TRUE == bUseAuto && FALSE == IsPanelAbsorbed())
			dVel = 1000.0;
		iResult = m_plnkX->StartMove(posTarget.dX, dVel);
	}
	if (iResult)
		return iResult;

	iResult = m_plnkMultiYT->Wait4Done();
	if (iResult)
	{
		m_plnkX->Stop();
		return iResult;
	}

	iResult = m_plnkX->Wait4Done(TRUE);
	if (iResult)
		return iResult;

/*	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [SyncMove XYT to PosID : %d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);
*/
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::UpPickUpUnit(BOOL bWaitOption)
{
	if (TRUE == IsUpPickUpUnit()) {
		return ERR_PANEL_ALIGNER_SUCCESS;
	}
	
	int iResult = SafeMoveZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS, bWaitOption);
	if (iResult) return iResult;

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::DownPickUpUnit(BOOL bWaitOption)
{
	if (TRUE == IsDownPickUpUnit()) {
		return ERR_PANEL_ALIGNER_SUCCESS;
	}
	
	int iResult = SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS, bWaitOption);
	if (iResult) return iResult;

	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsUpPickUpUnit(void)
{
	return IsInZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS);
}

BOOL MPanelAligner::IsDownPickUpUnit(void)
{
	return IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);
}

/*/
int MPanelAligner::UpRotatorUnit(BOOL bSkipSensor)
{
	if (TRUE == IsUpRotatorUnit())
		return ERR_PANEL_ALIGNER_SUCCESS;
	
	int iResult = m_plnkCylUDRotator->Up(bSkipSensor);
	return iResult;
}

int MPanelAligner::DownRotatorUnit(BOOL bSkipSensor)
{
	if (TRUE == IsDownRotatorUnit())
		return ERR_PANEL_ALIGNER_SUCCESS;
	if (FALSE == IsPanelReleasedRotator())
		return generateErrorCode(210075);

	int iResult = m_plnkCylUDRotator->Down(bSkipSensor);
	return iResult;
}

BOOL MPanelAligner::IsUpRotatorUnit(void)
{
	return m_plnkCylUDRotator->IsUp();
}

BOOL MPanelAligner::IsDownRotatorUnit(void)
{
	return m_plnkCylUDRotator->IsDown();
}

int MPanelAligner::TurnRotatorUnit(BOOL bSkipSensor)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

#ifdef DEF_GATE_SYSTEM

	if (TRUE == IsTurnRotatorUnit())
		return ERR_PANEL_ALIGNER_SUCCESS;

	//하강이 아닐때
	if (FALSE == IsDownRotatorUnit())
	{
		//	PanelAligner가 하강상태이어야 한다.
		if (FALSE == IsDownPickUpUnit())
		{
			return generateErrorCode(210118);
		}
		//Rotator가	Panel중심 위치이어야 한다
//		MPos_XYT RotatePos = GetPanelRotationPosition();
//		if (FALSE == IsInXYTPos(RotatePos))
		if (FALSE == IsInXYTPos(DEF_PANEL_ALIGNER_TURN_POS))
		{
			return generateErrorCode(210119);
		}
		//	Panel흡착 확인한다
		AbsorbPanelRotator(TRUE);
		DWORD stime = GetTickCount();
		while(1)
		{
			Sleep(10);
			if (IsPanelAbsorbedRotator())
				break;
			if (GetTickCount() - stime > 500)
				break;
		}
		if (FALSE == IsPanelAbsorbedRotator())
			ReleasePanelRotator();
		else
		{
			//	에어 부상상태이어야 한다
			if (FALSE == IsAirCVBlowOn())
			{
				AirCVBlowOn();
				Sleep(200);
			}
			// TabMounter상승 상태이어야 한다
			if (FALSE == m_plnkStateTabMounter[0]->IsUpAllTabMounterInGroup())
				return generateErrorCode(210120);
			if (FALSE == m_plnkStateRTabMounter[0]->IsUpAllTabMounterInGroup())
				return generateErrorCode(210121);
		}
		
	}
	iResult = m_plnkCylTRRotator->CW(bSkipSensor);
#endif

	return iResult;
}

int MPanelAligner::ReturnRotatorUnit(BOOL bSkipSensor)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

#ifdef DEF_GATE_SYSTEM

	if (TRUE == IsReturnRotatorUnit())
		return ERR_PANEL_ALIGNER_SUCCESS;

	//하강이 아닐때
	if (FALSE == IsDownRotatorUnit())
	{
		//	PanelAligner가 하강상태이어야 한다.
		if (FALSE == IsDownPickUpUnit())
		{
			return generateErrorCode(210118);
		}
		//Rotator가	Panel중심 위치이어야 한다
//		MPos_XYT RotatePos = GetPanelRotationPosition();
//		if (FALSE == IsInXYTPos(RotatePos))
		if (FALSE == IsInXYTPos(DEF_PANEL_ALIGNER_TURN_POS))
		{
			return generateErrorCode(210119);
		}
		//	Panel흡착 확인한다
		AbsorbPanelRotator(TRUE);
		DWORD stime = GetTickCount();
		while(1)
		{
			Sleep(10);
			if (IsPanelAbsorbedRotator())
				break;
			if (GetTickCount() - stime > 500)
				break;
		}
		if (FALSE == IsPanelAbsorbedRotator())
			ReleasePanelRotator();
		else
		{
			//	에어 부상상태이어야 한다
			if (FALSE == IsAirCVBlowOn())
			{
				AirCVBlowOn();
				Sleep(200);
			}
			// TabMounter상승 상태이어야 한다
			if (FALSE == m_plnkStateTabMounter[0]->IsUpAllTabMounterInGroup())
				return generateErrorCode(210120);
			if (FALSE == m_plnkStateRTabMounter[0]->IsUpAllTabMounterInGroup())
				return generateErrorCode(210121);
		}
	}

	iResult = m_plnkCylTRRotator->CCW(bSkipSensor);
#endif

	return iResult;
}

BOOL MPanelAligner::IsTurnRotatorUnit(void)
{
	return m_plnkCylTRRotator->IsCW();
}

BOOL MPanelAligner::IsReturnRotatorUnit(void)
{
	return m_plnkCylTRRotator->IsCCW();
}
/*/

double MPanelAligner::convertTtoY2(double dDegreeT)
{
	double dOriginDistBetweenAligners = m_plnkSystemData->m_dDistBetweenAligners;
	double dY2 = dOriginDistBetweenAligners * sin(DEG2RAD(dDegreeT));
	return dY2;
}

double MPanelAligner::convertY2toT(double dY)
{
	//임시 20100816 sj_ysb
	//double dOriginDistBetweenAligners = m_plnkSystemData->m_dDistBetweenAligners;
	double dOriginDistBetweenAligners = 500;
	double dDegreeT = RAD2DEG(asin(dY / dOriginDistBetweenAligners));
	return dDegreeT;
}

int MPanelAligner::AirCVBlowOn()
{
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Blow On] [Start]");
	WriteTactTimeLog(szLogMsg);

	m_plnkIO->OutputOn(m_usOAddrAirBlow1);
	m_plnkIO->OutputOn(m_usOAddrAirBlow2);

	AirCVVacAllOff();
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Blow On] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVBlowOff()
{
//	if (FALSE == IsAirCVBlowOff())
	{
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Blow Off] [Start]");
		WriteTactTimeLog(szLogMsg);
		m_plnkIO->OutputOff(m_usOAddrAirBlow1);
		m_plnkIO->OutputOff(m_usOAddrAirBlow2);

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Blow Off] [End]");
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsAirCVBlowOn()
{
	BOOL bRtn = FALSE;
	bRtn = (m_plnkIO->IsOn(m_usIAddrAirBlow1) && m_plnkIO->IsOn(m_usIAddrAirBlow2));
	return bRtn;
}

BOOL MPanelAligner::IsAirCVBlowOff()
{
	BOOL bRtn = (m_plnkIO->IsOff(m_usIAddrAirBlow1) && m_plnkIO->IsOff(m_usIAddrAirBlow2));
	return bRtn;
}

int MPanelAligner::AirCVVacAllOn(int iPanelWorkNo)
{
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Vac On] [Start]");
	WriteTactTimeLog(szLogMsg);


	int i = 0;
	unsigned short OAddr[DEF_MAX_AIRVACUUM] = {m_usOAddrAirVacuum1, m_usOAddrAirVacuum2, m_usOAddrAirVacuum3, m_usOAddrAirVacuum4};
	if (iPanelWorkNo == DEF_NONE_WORK_SCHEDULE)
	{
		for (int i=0; i<DEF_MAX_AIRVACUUM; i++)
		{
			m_plnkIO->OutputOn(OAddr[i]);
		}
	}
	else
	{
		for (int i=0; i<DEF_MAX_AIRVACUUM; i++)
		{
			if (TRUE == m_plnkPanelData->m_bUseAirVacuum[iPanelWorkNo][i])
				m_plnkIO->OutputOn(OAddr[i]);
		}
	}

	AirCVBlowOff();

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Vac On] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVVacAllOff()
{
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Vac Off] [Start]");
	WriteTactTimeLog(szLogMsg);

	m_plnkIO->OutputOff(m_usOAddrAirVacuum1);
	m_plnkIO->OutputOff(m_usOAddrAirVacuum2);
	m_plnkIO->OutputOff(m_usOAddrAirVacuum3);
	m_plnkIO->OutputOff(m_usOAddrAirVacuum4);

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [All Plates Air Vac Off] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsAirCVVacAllOn(int iPanelWorkNo)
{
	BOOL bRtn = FALSE;
	int i = 0;
	unsigned short IAddr[DEF_MAX_AIRVACUUM] = {m_usIAddrAirVacuum1, m_usIAddrAirVacuum2, m_usIAddrAirVacuum3, m_usIAddrAirVacuum4};
	if (iPanelWorkNo == DEF_NONE_WORK_SCHEDULE)		//전체
	{
		bRtn = FALSE;
		for (int i=0; i<DEF_MAX_AIRVACUUM; i++)
			bRtn |= m_plnkIO->IsOn(IAddr[i]);
	}
	else
	{
		bRtn = TRUE;
		for (int i=0; i<DEF_MAX_AIRVACUUM; i++)
		{
			if (TRUE == m_plnkPanelData->m_bUseAirVacuum[iPanelWorkNo][i])
				bRtn &= m_plnkIO->IsOn(IAddr[i]);
		}
	}

	return bRtn;
}

BOOL MPanelAligner::IsAirCVVacAllOff()
{
	return	(m_plnkIO->IsOff(m_usIAddrAirVacuum1) && 
			 m_plnkIO->IsOff(m_usIAddrAirVacuum2) &&
			 m_plnkIO->IsOff(m_usIAddrAirVacuum3) &&
			 m_plnkIO->IsOff(m_usIAddrAirVacuum4));
	return FALSE;
}

int MPanelAligner::AirCVVacPlate1On()
{
	if (FALSE == IsAirCVVacPlate1On())
	{
		int iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate1 Air Vac On] [Start]");
		WriteTactTimeLog(szLogMsg);

		if (m_plnkIO->IsOn(m_usOAddrAirBlow1))
		{
			iResult = m_plnkIO->OutputOff(m_usOAddrAirBlow1);	// Palte 1,2
			if (iResult)
			{
				m_plnkIO->OutputOn(m_usOAddrAirBlow1);
				return iResult;
			}

			// 부상 유닛 탱크의 Air 빠져나가는 시간 대기
			Sleep(500);
		}

		iResult = m_plnkIO->OutputOn(m_usOAddrAirVacuum1);	// Palte 1,2
		if (iResult)
		{
			m_plnkIO->OutputOff(m_usOAddrAirVacuum1);
			return iResult;
		}

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate1,2 Air Vac On] [End]");
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVVacPlate1Off()
{
	if (FALSE == IsAirCVVacPlate1Off())
	{
		int iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate1 Air Vac Off] [Start]");
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkIO->OutputOff(m_usOAddrAirVacuum1);	// Palte 1,2
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate1,2 Air Vac Off] [End]");
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsAirCVVacPlate1On()
{
	return m_plnkIO->IsOn(m_usIAddrAirVacuum1);
}
 
BOOL MPanelAligner::IsAirCVVacPlate1Off()
{
	return m_plnkIO->IsOff(m_usIAddrAirVacuum1);
}

int MPanelAligner::AirCVVacPlate2On()
{
	if (TRUE == IsAirCVVacPlate2On())
		return ERR_PANEL_ALIGNER_SUCCESS;

	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Plate2 Air Vac On] [Start]");
	WriteTactTimeLog(szLogMsg);
	
	if (m_plnkIO->IsOn(m_usOAddrAirBlow2))
	{
		iResult = m_plnkIO->OutputOff(m_usOAddrAirBlow2);	// Palte 2
		if (iResult)
		{
			m_plnkIO->OutputOn(m_usOAddrAirBlow2);
			return iResult;
		}
		// 부상 유닛 탱크의 Air 빠져나가는 시간 대기
		Sleep(500);
	}
	
	iResult = m_plnkIO->OutputOn(m_usOAddrAirVacuum2);	// Palte 2
	if (iResult)
	{
		m_plnkIO->OutputOff(m_usOAddrAirVacuum2);
		return iResult;
	}
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Plate2 Air Vac On] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVVacPlate2Off()
{
	if (FALSE == IsAirCVVacPlate2Off())
	{
		int iResult;

		// jdy sesl
		char szLogMsg[200];
		
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate2 Air Vac Off] [Start]");
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkIO->OutputOff(m_usOAddrAirVacuum2);	// Palte 3,4
		if (iResult)
			return iResult;

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate2 Air Vac Off] [End]");
		WriteTactTimeLog(szLogMsg);
	}

	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsAirCVVacPlate2On()
{
	return m_plnkIO->IsOn(m_usIAddrAirVacuum2);	// Plate 3,4
}
 
BOOL MPanelAligner::IsAirCVVacPlate2Off()
{
	return m_plnkIO->IsOff(m_usIAddrAirVacuum2);// Plate 3,4
}

int MPanelAligner::AirCVVacPlate3On()
{
	if (TRUE == IsAirCVVacPlate3On())
		return ERR_PANEL_ALIGNER_SUCCESS;

	int iResult;

	// jdy sesl
	char szLogMsg[200];
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Plate3 Air Vac On] [Start]");
	WriteTactTimeLog(szLogMsg);
	
	if (m_plnkIO->IsOn(m_usOAddrAirBlow1) || m_plnkIO->IsOn(m_usOAddrAirBlow2))
	{
		m_plnkIO->OutputOff(m_usOAddrAirBlow1);
		m_plnkIO->OutputOff(m_usOAddrAirBlow2);
		// 부상 유닛 탱크의 Air 빠져나가는 시간 대기
		Sleep(500);
	}
	
	iResult = m_plnkIO->OutputOn(m_usOAddrAirVacuum3);	// Palte 2
	if (iResult)
	{
		m_plnkIO->OutputOff(m_usOAddrAirVacuum3);
		return iResult;
	}
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Plate3 Air Vac On] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVVacPlate3Off()
{
	if (FALSE == IsAirCVVacPlate3Off())
	{
		int iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate3 Air Vac Off] [Start]");
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkIO->OutputOff(m_usOAddrAirVacuum3);
		if (iResult)
			return iResult;
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate3 Air Vac Off] [End]");
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsAirCVVacPlate3On()
{
	return m_plnkIO->IsOn(m_usIAddrAirVacuum3);
	return FALSE;
}
 
BOOL MPanelAligner::IsAirCVVacPlate3Off()
{
	return m_plnkIO->IsOff(m_usIAddrAirVacuum3);
	return FALSE;
}


int MPanelAligner::AirCVVacPlate4On()
{
	if (TRUE == IsAirCVVacPlate4On())
		return ERR_PANEL_ALIGNER_SUCCESS;

	int iResult;

	char szLogMsg[200];
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Plate4 Air Vac On] [Start]");
	WriteTactTimeLog(szLogMsg);
	
	if (m_plnkIO->IsOn(m_usOAddrAirBlow1) || m_plnkIO->IsOn(m_usOAddrAirBlow2))
	{
		m_plnkIO->OutputOff(m_usOAddrAirBlow1);
		m_plnkIO->OutputOff(m_usOAddrAirBlow2);
		// 부상 유닛 탱크의 Air 빠져나가는 시간 대기
		Sleep(500);
	}
	
	iResult = m_plnkIO->OutputOn(m_usOAddrAirVacuum4);	// Palte 2
	if (iResult)
	{
		m_plnkIO->OutputOff(m_usOAddrAirVacuum4);
		return iResult;
	}
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [Plate4 Air Vac On] [End]");
	WriteTactTimeLog(szLogMsg);
	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVVacPlate4Off()
{
	if (FALSE == IsAirCVVacPlate4Off())
	{
		int iResult;

		// jdy sesl
		char szLogMsg[200];

		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate4 Air Vac Off] [Start]");
		WriteTactTimeLog(szLogMsg);

		iResult = m_plnkIO->OutputOff(m_usOAddrAirVacuum4);	// Palte 3,4
		if (iResult)
			return iResult;
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [Plate3 Air Vac Off] [End]");
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_ALIGNER_SUCCESS;
}

BOOL MPanelAligner::IsAirCVVacPlate4On()
{
	return m_plnkIO->IsOn(m_usIAddrAirVacuum4);
}
 
BOOL MPanelAligner::IsAirCVVacPlate4Off()
{
	return m_plnkIO->IsOff(m_usIAddrAirVacuum4);// Plate4
}

int MPanelAligner::AirCVBlowOn1()
{
	m_plnkIO->OutputOff(m_usOAddrAirVacuum1);
	return 	m_plnkIO->OutputOn(m_usOAddrAirBlow1);
}

int MPanelAligner::AirCVBlowOff1()
{	
	return 	m_plnkIO->OutputOff(m_usOAddrAirBlow1);
}

int MPanelAligner::AirCVBlowOn2()
{
	m_plnkIO->OutputOff(m_usOAddrAirVacuum2);
	return 	m_plnkIO->OutputOn(m_usOAddrAirBlow2);
}

int MPanelAligner::AirCVBlowOff2()
{
	return 	m_plnkIO->OutputOff(m_usOAddrAirBlow2);
}

/*/
int MPanelAligner::AirCVBlowOn3()
{
	m_plnkIO->OutputOff(m_usOAddrAirVacuum3);
	return 	m_plnkIO->OutputOn(m_usOAddrAirBlow3);
}

int MPanelAligner::AirCVBlowOff3()
{
	return 	m_plnkIO->OutputOff(m_usOAddrAirBlow3);
}

int MPanelAligner::AirCVBlowOn4()
{
	m_plnkIO->OutputOff(m_usOAddrAirVacuum4);
	return 	m_plnkIO->OutputOn(m_usOAddrAirBlow4);
}

int MPanelAligner::AirCVBlowOff4()
{
	return 	m_plnkIO->OutputOff(m_usOAddrAirBlow4);
}
/*/

int MPanelAligner::RMoveYPosOnly(double dYDistance, int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	// 각 축에 목표 위치 넘기고 각 축 별 이동 시간 받고
	// 가장 큰 이동 시간만큼 
	Sleep(500);
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif

	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [RMove Y Only] [Start]");
	WriteTactTimeLog(szLogMsg);

	if (iPosID != DEF_PANEL_ALIGNER_NONE_POS)
	{
		MPos_XYT posTarget = GetXYTTargetPos(iPosID);	// 목표 위치 읽기
		iResult	= checkInterlockForXYT(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;
	}

	double rgdTarget[2] = { dYDistance, dYDistance };

	double dNormalVel = m_plnkY->GetMovingVelocity();
	if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
		dNormalVel = m_plnkY->GetSlowMovingVelocity();

	double dLowVel = dNormalVel / 10.0;
	double rgdNormalVel[2] = { dNormalVel, dNormalVel };
	double rgdLowVel[2] = { dLowVel, dLowVel };

	// MultiXT 축 목표위치로 이동
	if (fabs(dYDistance) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		iResult = m_plnkMultiYT->StartRMove(rgdTarget, rgdLowVel);
	}
	else
	{
		iResult = m_plnkMultiYT->StartRMove(rgdTarget, rgdNormalVel);
	}
	if (iResult)
		return iResult;

	//SJ_YYK 161227 Modify..
	//iResult = m_plnkMultiYT->Wait4Done();
	iResult= m_plnkY->Wait4Done();
	if (iResult)
		return iResult;

	iResult= m_plnkT->Wait4Done();
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [RMove Y Only] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::RMoveXPosOnly(double dXDistance, int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	// 각 축에 목표 위치 넘기고 각 축 별 이동 시간 받고
	// 가장 큰 이동 시간만큼 
	Sleep(500);
	return ERR_PANEL_ALIGNER_SUCCESS;
#endif
	
	int iResult = ERR_PANEL_ALIGNER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [RMove X Only] [Start]");
	WriteTactTimeLog(szLogMsg);

	if (iPosID != DEF_PANEL_ALIGNER_NONE_POS)
	{
		MPos_XYT posTarget = GetXYTTargetPos(iPosID);	// 목표 위치 읽기
		iResult	= checkInterlockForXYT(iPosID, posTarget, bUseAuto);
		if (iResult)
			return iResult;
	}

	// Y 축 목표위치로 이동
	if (fabs(dXDistance) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		double dNormalVel = m_plnkX->GetMovingVelocity();
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkX->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / 10.0;
		
		iResult = m_plnkX->StartRMove(dXDistance, dLowVel);
	}
	else
	{
		iResult = m_plnkX->StartRMove(dXDistance);
	}
	if (iResult)
		return iResult;

	iResult = m_plnkX->Wait4Done();
	if (iResult)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MPanelAligner] [M] [RMove X Only] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_PANEL_ALIGNER_SUCCESS;
}

int MPanelAligner::AirCVFloatingOn()
{
	AirCVBlowOn();
//	Sleep(100);
//	AirCVVacAllOff();

	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelAligner::AirCVFloatingOff()
{
	AirCVBlowOff();
//	AirCVVacAllOn();

	return ERR_PANEL_TRANSFER_SUCCESS;
}

#ifdef DEF_GATE_SYSTEM
BOOL MPanelAligner::IsPanelLoadingDetected()
{
	return m_plnkIO->IsOn(m_usIPanelDetectLoading);
}
#endif

//120224.kms_________
BOOL MPanelAligner::IsPanelWorkingDetected()
{
//120224.KMS____________
	return m_plnkIO->IsOn(m_usIPanelDetectWorking);
//___________________
}

//110102 HJG
#ifdef DEF_USE_PANEL_PUSHER
int MPanelAligner::UpPanelPusherUDCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkPanelPusherUD != NULL);
	if (m_plnkPanelPusherUD->IsUp() == FALSE)
	{
		// jdy sesl
		char szLogMsg[200];
		
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [MPanelAligner Panel Pusher UD Cyl Up (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
		
		int iResult = m_plnkPanelPusherUD->Up(bSkipSensor);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL Aligner Panel Pusher Up/Down Cylinder"),2, __FILE__, __LINE__);
			return iResult;
		}
		
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [MPanelAligner Panel Pusher UD Cyl Up (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}	
	return ERR_PANEL_TRANSFER_SUCCESS;
}

int MPanelAligner::DownPanelPusherUDCyl(BOOL bSkipSensor)
{
	ASSERT(m_plnkPanelPusherUD != NULL);
	if (m_plnkPanelPusherUD->IsDown() == FALSE)
	{
		// jdy sesl
		char szLogMsg[200];
		
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [MPanelAligner Panel Pusher UD Cyl Down (SkipMode:%d)] [Start]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
		
		int iResult = m_plnkPanelPusherUD->Down(bSkipSensor);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL Aligner Panel Pusher Up/Down Cylinder"),2, __FILE__, __LINE__);
			return iResult;
		}
		
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MPanelAligner] [M] [MPanelAligner Panel Pusher UD Cyl Down (SkipMode:%d)] [End]", bSkipSensor);
		WriteTactTimeLog(szLogMsg);
	}
	return ERR_PANEL_TRANSFER_SUCCESS;
}

BOOL MPanelAligner::IsUpPanelPusherUDCyl()
{
	ASSERT(m_plnkPanelPusherUD != NULL);
	return m_plnkPanelPusherUD->IsUp();
}

BOOL MPanelAligner::IsDownPanelPusherUDCyl()
{
	ASSERT(m_plnkPanelPusherUD != NULL);
	return m_plnkPanelPusherUD->IsDown();
}
#endif

//120508.kms_________
BOOL MPanelAligner::IsIonizerState()
{
	return m_plnkIO->IsOn(m_usIIonizerState);
}

//170724_KDH Spot Cleaner 추가
BOOL MPanelAligner::IsSpotCleanerState()
{
	return m_plnkIO->IsOn(m_usIAddrSpotCleanerState);
}

int MPanelAligner::SpotCleanerOn()
{
	return m_plnkIO->OutputOn(m_usOAddrSpotCleanerOn);
}

int MPanelAligner::SpotCleanerOff()
{
	return m_plnkIO->OutputOff(m_usOAddrSpotCleanerOn);
}
//____________________


int	MPanelAligner::GetPanelWorkNoFromCurPos()
{
	int iWorkNo = 0;
	double dCurPosX = GetXYTCurrentPos().dX;
	double dCompPosX = GetXYTTargetPos(DEF_PANEL_ALIGNER_PRESS_1_POS).dX + 100.0;
	if (dCurPosX < dCompPosX)
		iWorkNo = 0;
	else
		iWorkNo = 1;
	return iWorkNo;
}

// End Of Code

