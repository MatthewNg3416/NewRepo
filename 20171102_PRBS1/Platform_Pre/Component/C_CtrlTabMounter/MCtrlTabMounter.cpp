/* 
 * Control TabMounter Component
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
#include "MPlatformOlbSystem.h"
#include "IVision.h"
#include "MCtrlTabMounter.h"
//MERGE_LKH_090704 : SESL_조재근S 버전으로 적용
#include "MPreBonderData.h"
#include "MTabMounter.h"
#include "MTabAligner.h"
#include "MTabCarrier.h"
#include "DefTabCarrier.h"
#include "MSystemData.h"
#include "MProcessData.h"
#include "MTabOffsetData.h"
#include "MVisionCalibration.h"
#include "common.h"
#include "MTickTimer.h"
#include "DBTabLoss.h"
#include "MTrsAutoManager.h"
#include "MTrsTabMounter.h"

extern MPlatformOlbSystem	MOlbSystemPre;

MCtrlTabMounter::MCtrlTabMounter(SCommonAttribute commonData, SCtrlTabMounterRefCompList listRefComponent, SCtrlTabMounterData datComponent)
: ICommon(commonData)
{	
	SetData(datComponent);
	AssignComponents(listRefComponent);
	
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_iInstanceNo < DEF_MOUNTER_WORK_PER_GROUP)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = DEF_MAX_TABMOUNTER - 1 - m_iInstanceNo;
	}
#else
	/*/
	if (m_iInstanceNo < DEF_MOUNTER_WORK_PER_GROUP)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo - DEF_MAX_TABMOUNTER;
	}
//@#endif

}

MCtrlTabMounter::~MCtrlTabMounter()
{
}

int MCtrlTabMounter::Initialize(void)
{
	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

void MCtrlTabMounter::SetData(SCtrlTabMounterData datcomponent)
{
	m_plnkSystemData = datcomponent.m_plnkSystemData;
	m_plnkProcessData = datcomponent.m_plnkProcessData;
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST] = datcomponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND] = datcomponent.m_plnkWorkScheduleSecond;
#endif
	m_plnkTabOffsetData = datcomponent.m_plnkTabOffsetData;
	//120208.kms_______
	m_plnkTabOffsetData_Rear = datcomponent.m_plnkTabOffsetData_Rear;
//MERGE_LKH_090704 : SESL_조재근S 버전으로 적용
	//20090620 sj_ysb
	m_plnkPreBonderData		= datcomponent.m_plnkPreBonderData;
	m_plnkDBTabLoss			= datcomponent.m_plnkDBTabLoss;
}

void MCtrlTabMounter::AssignComponents(SCtrlTabMounterRefCompList listRefComponents)
{
	m_plnkTabMounter = listRefComponents.m_plnkTabMounter;
//KKY.Del	m_rgplnkTabAligner[DEF_TABALIGNER1_PER_GROUP] = listRefComponents.m_rgplnkTabAligner[DEF_TABALIGNER1_PER_GROUP];
//KKY.Del	m_rgplnkTabAligner[DEF_TABALIGNER2_PER_GROUP] = listRefComponents.m_rgplnkTabAligner[DEF_TABALIGNER2_PER_GROUP];
//KKY.Del	m_plnkTabCarrier = listRefComponents.m_plnkTabCarrier;
	
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_plnkTabCarrier[i] = listRefComponents.m_plnkTabCarrier[i];
		m_plnkRTabCarrier[i] = listRefComponents.m_plnkRTabCarrier[i];
	}

	m_plnkVision = listRefComponents.m_plnkVision;
	m_plnkVisionCalibration = listRefComponents.m_plnkCtrlVisionCalibration;
}

//int MCtrlTabMounter::LoadTabIC(int iCarrierWorkGroup, int iCarrierNo)
int MCtrlTabMounter::LoadTabIC(int iCarrierWorkGroup, int iCarrierNo, BOOL* parpTabLoading) //170830 JSH
{
	int iResult = ERR_CTRL_TABMOUNTER_SUCCESS;

	IStateTabCarrier* pTabCarrier = NULL;
	int nLoadPosID = DEF_TABMOUNTER_LOAD_POS;
	int nZLoadPosID = DEF_TABMOUNTER_Z_LOAD_POS;
	BOOL bTabLoading = FALSE;

	//170830 JSH.s
	if(parpTabLoading != NULL)
	{
		*parpTabLoading = bTabLoading;
	}
	//170830 JSH.e
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (iCarrierWorkGroup == DEF_FRONT_GROUP)
	{
//		pTabCarrier = m_plnkTabCarrier[m_iWorkInstance];//[iCarrierNo];
		pTabCarrier = m_plnkTabCarrier[iCarrierNo];
	}
	else
	{
//		pTabCarrier = m_plnkRTabCarrier[m_iWorkInstance];//[iCarrierNo];
		pTabCarrier = m_plnkRTabCarrier[iCarrierNo];
	}
#else
	/*/
	pTabCarrier = m_plnkTabCarrier[iCarrierNo];
//@#endif

	/*/
	if (iCarrierWorkGroup == m_iWorkGroup)
		nLoadPosID = DEF_TABMOUNTER_LOAD_POS;
	else
		nLoadPosID = DEF_TABMOUNTER_R_LOAD_POS;
	/*/
	nLoadPosID = DEF_TABMOUNTER_LOAD_POS;
	nZLoadPosID = DEF_TABMOUNTER_Z_LOAD_POS;

	double dTolerance = 0.002;
	if (FALSE == m_plnkTabMounter->IsInPos(0, nLoadPosID, &dTolerance))
	{
		Sleep(200);		
		double dTolerance = 0.002;
		if (FALSE == m_plnkTabMounter->IsInPos(0, nLoadPosID, &dTolerance))
		{
			if (m_plnkTabMounter->SafeMovePos(0, nLoadPosID))
			{
				// 323006 = Load 위치가 아님. (Tab Load 동작 시작 시)
				return generateErrorCode(323006);
			}
		}
	}

	MPos_XYT tolerance(1.0, 1.0, 1.0);
	if (FALSE == pTabCarrier->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS, &tolerance))
		// 323000 = Tab Load 동작 시 TabCarrier 가 Unload 위치가 아님.
		return generateErrorCode(323000);

	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		iResult = m_plnkTabMounter->SafeMoveZPos(nZLoadPosID);
		if (iResult)
		{
			m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
			return iResult;
		}
//		Sleep(500);
		Sleep(200);
		iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		if (iResult)
			return iResult;
		return ERR_CTRL_TABMOUNTER_SUCCESS;
	}

	if (TRUE == m_plnkTabMounter->IsAbsorbTabIC())
		return ERR_CTRL_TABMOUNTER_SUCCESS;
	
	iResult = m_plnkTabMounter->SafeMoveZPos(nZLoadPosID);
	if (iResult)
		return iResult;
	 
//	iResult = m_plnkTabMounter->AbsorbTabIC(TRUE);
	iResult = m_plnkTabMounter->AbsorbTabIC();
	if (iResult)
	{
		m_plnkTabMounter->ReleaseTabIC();
		//m_plnkTabMounter->Up();
		m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);

		if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		{
			E_TABVAC_ERR_FIELD eField;
			switch (m_iWorkInstance)
			{
			case 0:
				eField = eTM1;
				break;
			case 1:
				eField = eTM2;
				break;
			case 2:
				eField = eTM3;
				break;
			case 3:
				eField = eTM4;
				break;
			}
			m_plnkDBTabLoss->AppendFieldCount(eField);
		}

		return iResult;
	}

//	Sleep(100);

	// 주의 : TabCarrier 의 Vacuum 을 TabMounter 가 제어한다.
	iResult = pTabCarrier->ReleaseTabIC();
	if (iResult)
	{
		// 확인 필요 : 잔압이 빠지지 않을 수 있는 구조인지 확인할 것.
		// Vacuum 해제 동작에서 Error 발생하는 경우에 대한 처리 : Retry
		Sleep(800);

		iResult = pTabCarrier->ReleaseTabIC();
		if (iResult)
		{
			pTabCarrier->ReleaseTabIC();
			//m_plnkTabMounter->Up();
			m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
			return iResult;
		}
	}

	if (FALSE == m_plnkTabMounter->IsAbsorbTabIC())
	{
		iResult = m_plnkTabMounter->AbsorbTabIC();
		if (iResult)
		{
			pTabCarrier->AbsorbTabIC();
			Sleep(200);
			m_plnkTabMounter->ReleaseTabIC();
			m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
			return iResult;
		}
	}

	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
	{
		m_plnkTabMounter->ReleaseTabIC();
//		m_plnkTabMounter->Up(TRUE);
		return iResult;
	}

	if (FALSE == m_plnkTabMounter->IsAbsorbTabIC())
	{
		iResult = m_plnkTabMounter->AbsorbTabIC();
		if (iResult)
		{
			m_plnkTabMounter->ReleaseTabIC();
			return iResult;
		}
	}
	
	//170830 JSH.s
	bTabLoading = TRUE;
	if(parpTabLoading != NULL)
	{
		*parpTabLoading = bTabLoading;
	}
	//170830 JSH.e
	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

int MCtrlTabMounter::Remove()	//수정요
{
	int iResult = ERR_CTRL_TABMOUNTER_SUCCESS;
//	if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode && FALSE == m_plnkTabMounter->IsAbsorbTabIC())
//		return ERR_CTRL_TABMOUNTER_SUCCESS;
	
	/*/
	MTabCarrier* pTabCarrier = NULL;
#ifdef DEF_SOURCE_SYSTEM
	if (m_iWorkGroup == DEF_FRONT_GROUP)
		pTabCarrier = m_plnkTabCarrier[DEF_TABCARRIER4];
	else
		pTabCarrier = m_plnkRTabCarrier[DEF_TABCARRIER4];
#else
	pTabCarrier = m_plnkTabCarrier[m_iWorkInstance];
#endif

	if (FALSE == pTabCarrier->IsForwardDumpBasket())
	{
		iResult = pTabCarrier->ForwardDumpBasket();
		if (iResult)
		{
			SetErrorLevel(_T("TAB MOUNTER Remove Tab IC"),1, __FILE__, __LINE__);
			return iResult;
		}
	}
	/*/
	
	int iRemovePos = DEF_TABMOUNTER_REMOVE_POS;
	//int iRemovePos = DEF_TABMOUNTER_LOAD_POS;

	if (FALSE == m_plnkTabMounter->IsInGantryPos(DEF_TABMOUNTER_GANTRY_TABREMOVE_POS))
	{
		iResult = m_plnkTabMounter->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_TABREMOVE_POS);
		if (iResult)
			return iResult;
	}

	m_plnkTabMounter->SetGantryMotionEnable(FALSE);

	if (FALSE == m_plnkTabMounter->IsInPos(0, iRemovePos))
	{
		iResult = m_plnkTabMounter->SafeMovePos(0, iRemovePos);
		if (iResult)
			return iResult;
	}

	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_LOAD_POS);//@DEF_TABMOUNTER_Z_REMOVE_POS);
	if (iResult)
		return iResult;

	iResult = m_plnkTabMounter->ReleaseTabIC();
	if (iResult)
	{
		m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		return iResult;
	}
	Sleep(500);

	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
		return iResult;


	/*
	if (FALSE == m_plnkTabMounter->IsAbsorbTabIC())
		return ERR_CTRL_TABMOUNTER_SUCCESS;

	MTabCarrier* pTabCarrier = NULL;
#ifdef DEF_SOURCE_SYSTEM
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		pTabCarrier = m_plnkTabCarrier[m_iWorkInstance];
	}
	else
	{
		pTabCarrier = m_plnkRTabCarrier[m_iWorkInstance];
	}
#else
	pTabCarrier = m_plnkTabCarrier[m_iWorkInstance];
#endif
	
	
	if (FALSE == m_plnkTabMounter->IsInPos(DEF_TABMOUNTER_LOAD_POS))
	{
		Sleep(200);
		double dTolerance = 0.1;
		if (FALSE == m_plnkTabMounter->IsInPos(DEF_TABMOUNTER_LOAD_POS, &dTolerance))
		{
			if (m_plnkTabMounter->SafeMovePos(0, DEF_TABMOUNTER_LOAD_POS))
			{
				SetErrorLevel(_T("TAB MOUNTER load Tab IC"),1, __FILE__, __LINE__);
				// 323006 = Load 위치가 아님. (Tab Load 동작 시작 시)
				return generateErrorCode(323013);
			}
		}
	}
	MPos_XYT tolerance(2.0, 2.0, 2.0);
	if (FALSE == pTabCarrier->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS, &tolerance))
	{
		SetErrorLevel(_T("TAB MOUNTER load Tab IC"),1, __FILE__, __LINE__);
		// 323000 = Tab Load 동작 시 TabCarrier 가 Unload 위치가 아님.
		return generateErrorCode(323005);
	}

	MPos_XYT PosTg = pTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_REMOVE_POS);
	iResult = pTabCarrier->MoveXYTPos(PosTg, DEF_TABCARRIER_UNLOAD_POS);
	if (iResult)
		return iResult;

	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_REMOVE_POS);
	if (iResult)
		return iResult;

	iResult = m_plnkTabMounter->ReleaseTabIC();
	if (iResult)
	{
		m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		pTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		return iResult;
	}
	
	Sleep(500);

	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
	{
		pTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		return iResult;
	}

	iResult = pTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	if (iResult)
		return iResult;
	*/
	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

int MCtrlTabMounter::MountTabIC(EWorkingSide eWorkSide, int iTabNo)
{
	int iResult = ERR_CTRL_TABMOUNTER_SUCCESS;

	//171024_KDH Mount Press 작업 전 TabIC 확인을 한다. (없는 상태에서도 Mount 작업이 진행되서 TabIC 미부착 발생)
	if(m_plnkTabMounter->IsAbsorbTabIC() == FALSE)
		return generateErrorCode(324240);

	//_____________

	//20131115 SJ_HJG
	MTickTimer timerMountBondingParam;
	/*
	if (m_plnkSystemData->m_bUseInspectCamera == FALSE)
	{
		double dMountPos = m_plnkTabMounter->GetTargetPos(DEF_TABMOUNTER_MOUNT_POS, iTabNo);
		double dCurrentPos = m_plnkTabMounter->GetCurrentPos();

		if (fabs(dMountPos - dCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
		{
			int iResult = m_plnkTabMounter->MovePos(dMountPos, DEF_TABMOUNTER_MOUNT_POS);
			if (iResult)
			{
				SetErrorLevel(_T("TAB MOUNTER mount Tab IC"),1, __FILE__, __LINE__);
				return iResult;
			}
		}
	}
	*/

	// Unit 들 작업 위치 확인은 Skip! : PanelAligner, TabMounter

	/*
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		Sleep((DWORD)(m_plnkSystemData->m_dBondTime * 1000));
		return ERR_CTRL_TABMOUNTER_SUCCESS;
	}
	*/

	//@131127.KKY___________
	if (m_plnkSystemData->m_bUsePanelPusher == 2)
	{
		m_plnkTabMounter->DownPusherCyl();
	}
	//@_____________________

	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	//20131115 SJ_HJG
	timerMountBondingParam.StartTimer();
	//_______________
	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOUNT_POS);
	if (iResult)
	{
		//@131127.KKY___________
		m_plnkTabMounter->UpPusherCyl();
		//@_____________________
		return iResult;
	}
	double TabMounterDownPressElapseTime = timerPressCylinder.GetElapseTime();
	timerPressCylinder.StopTimer();

	//20131115 SJ_HJG
	MOlbSystemPre.GetTrsTabMounterComponent(m_iInstanceNo)->m_dMountBondingDown = timerMountBondingParam.GetElapseTime();
	timerMountBondingParam.StopTimer();
	//_______________

	int TabMounterNumber = m_plnkTabMounter->GetTabMounterNo();

	/*
	CString strLog;
	strLog.Format(_T("TabMounter %d DownPress Time :,%.3f"), TabMounterNumber, TabMounterDownPressElapseTime);
	m_plnkVision->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);

	MTickTimer timerPress;
	timerPress.StartTimer();

	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType
			&& TRUE == m_plnkProcessData->IsWorkSideTabMounterGroupLeader(eWorkSide, m_iInstanceNo))
		{
			LogVisionMarkPos(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_XU_MARK, "press(After Mount)");
			LogVisionMarkPos(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_XU_MARK, "press(After Mount)");
		}
	}
	*/
	MTickTimer timerPress;
	timerPress.StartTimer();
	
	//20131115 SJ_HJG
	timerMountBondingParam.StartTimer();
	//_______________

	BOOL bVacuumRelease = FALSE;
	double dVacuumReleaseTime = m_plnkSystemData->m_dBondTime / 2;
	while (TRUE == timerPress.LessThan(m_plnkSystemData->m_dBondTime))
	{
		if (FALSE == bVacuumRelease && TRUE == timerPress.MoreThan(dVacuumReleaseTime))
		{
			m_plnkTabMounter->ReleaseTabIC(TRUE);
			bVacuumRelease = TRUE;
		}

		Sleep(10);
	}
	
	iResult = m_plnkTabMounter->ReleaseTabIC();
	if (iResult)
	{
		Sleep(1000);

		// Vacuum Sol Off -> Cylinder Up : 압착 이후이므로 TabIC 부착 상태에 영향을 주지는 않음.
		m_plnkTabMounter->ReleaseTabIC(TRUE);
		//m_plnkTabMounter->Up();
		m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		iResult = m_plnkTabMounter->ReleaseTabIC();
		if (iResult)
			return iResult;
	}

	//@131127.KKY___________
	m_plnkTabMounter->UpPusherCyl(TRUE);
	//@_____________________

	//20131115 SJ_HJG
	MOlbSystemPre.GetTrsTabMounterComponent(m_iInstanceNo)->m_dMountBondingTime = timerMountBondingParam.GetElapseTime();
	timerMountBondingParam.StopTimer();
	//________________

	//20131115 SJ_HJG
	timerMountBondingParam.StartTimer();
	//_______________
	//iResult = m_plnkTabMounter->Up(TRUE);
	//20110126 sj_ysb
	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, FALSE);
//	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
		return iResult;
	
	//20131115 SJ_HJG
	MOlbSystemPre.GetTrsTabMounterComponent(m_iInstanceNo)->m_dMountBondingUp = timerMountBondingParam.GetElapseTime();
	timerMountBondingParam.StopTimer();
	//_______________

	//@131127.KKY___________
	//170715_KDH 임시삭제
	/*
	if (FALSE == m_plnkTabMounter->IsUpPusherCyl())
	{
		iResult = m_plnkTabMounter->UpPusherCyl();
		if (iResult)
		{
			SetErrorLevel(_T("TAB MOUNTER mount Tab IC"),1, __FILE__, __LINE__);
			return iResult;
		}
	}
	*/
	//________________
	//@_____________________

	Sleep(10);	// Up 동작 SkipSensor 사용 => 약간의 안정화 시간.

	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

int MCtrlTabMounter::LogVisionMarkPos(int iCameraNo, int iMarkNo, CString strMsg)
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		return ERR_CTRL_TABMOUNTER_SUCCESS;

#ifdef SIMULATION
	return ERR_CTRL_TABMOUNTER_SUCCESS;
#endif

	m_plnkVision->LockCamera(iCameraNo);

	m_plnkVision->ClearOverlay(iCameraNo);
	int iResult = m_plnkVision->Grab(iCameraNo);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCameraNo);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iCameraNo, iMarkNo);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCameraNo);
		return ERR_CTRL_TABMOUNTER_SUCCESS;
	}
	
	MPos_XYT xytResult;
	m_plnkVisionCalibration->ConvertV2R(iCameraNo, NULL,
										m_plnkVision->GetSearchResultX(iCameraNo, iMarkNo),
										m_plnkVision->GetSearchResultY(iCameraNo, iMarkNo),
										xytResult.dX, xytResult.dY);
	xytResult.dT = 0.0;

	m_plnkVision->UnlockCamera(iCameraNo);

	CString strLog;
	strLog.Format(_T("LogVisionMarkPos :,%s,Cam%d Mark%d Result[,%.3f,%.3f,]"), (LPCTSTR)strMsg, iCameraNo, iMarkNo, xytResult.dX, xytResult.dY);
	m_plnkVision->WriteLog(_T("..\\Log\\panel_log.csv"), (LPCTSTR)strLog);
	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

int MCtrlTabMounter::MoveToMountPos(int iWorkScheduleNo, EWorkingSide eWorkSide, int iRealTabNo)	//수정요
{
	double dTargetPos = 0.0;
	double dBacklash = 0.0;
	double dCommandCurrentPos = 0.0;
	double dInspectionOffset = 0.0;
	double dEncoderCurrentPos = 0.0;
	double dToolOffset = 0.0;
	double dTabOffset = 0.0;
	BOOL bWaitOption = TRUE;

//	if (TRUE == m_plnkSystemData->m_bUseInspectCamera)
	{
		// Inspection 결과 Offset 적용 후, 최종 압착 위치로 이동.
		// : 이동량 자체가 작으므로 별도 위치 점유 처리 없음. (Mount Pos => Mount Pos 이동 개념)
		
		// 현재 위치(Inspection 위치) 에 Inspection Offset 을 더해서 상대 이동하는 개념으로 적용.
		// 단, 이 때 TabMounter 의 Current 위치는 Command 로 읽어 낸다. (gain 등의 문제로 Axis 떨고 있는 상황 등에 대한 대응)
		dCommandCurrentPos = m_plnkTabMounter->GetCurrentPos(TRUE);
		//@130116.KKY_________
//@		dInspectionOffset = m_plnkWorkSchedule[iWorkScheduleNo]->GetInspectionOffset(iRealTabNo).dY;	//확인요
		dInspectionOffset = m_plnkWorkSchedule[iWorkScheduleNo]->GetInspectionOffset(iRealTabNo).dX;
		//@130116.KKY_________
		dEncoderCurrentPos = m_plnkTabMounter->GetCurrentPos();
		dToolOffset = m_plnkSystemData->m_rgdToolOffsetMounter[m_iInstanceNo];
		
		/*/
//20120206 sj_hjg
#ifndef DEF_GATE_SYSTEM
		if(m_plnkSystemData->m_bUseAutoAlignSystem == TRUE)
		{	
			if(eWorkSide == WORKING_SIDE_X_UP)
			{
				dTabOffset = m_plnkTabMounter->GetReceiveAkdata(eWorkSide, iRealTabNo);
			}				
			else
			{
				if (m_iWorkGroup == DEF_FRONT_GROUP)
					dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(eWorkSide, iRealTabNo);
				else
					dTabOffset = m_plnkTabOffsetData_Rear->GetTabOffsetMounter(eWorkSide, iRealTabNo);				
			}
		}
		else
#endif
		/*/
		{
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
				dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(eWorkSide, iRealTabNo);
			else
				dTabOffset = m_plnkTabOffsetData_Rear->GetTabOffsetMounter(eWorkSide, iRealTabNo);
			/*/
			dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(eWorkSide, iRealTabNo);
		}
		
#ifndef SIMULATION
		if (fabs(dCommandCurrentPos - dEncoderCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
		{
			// 확인 필요 : Log
			CString strLog;
			strLog.Format(_T("MoveToMountPos Start - Current command: %.4f, feedback: %.4f"), dCommandCurrentPos, dEncoderCurrentPos);
			WriteNormalLog(strLog);

			Sleep(300);

			dCommandCurrentPos = m_plnkTabMounter->GetCurrentPos(TRUE);
			dEncoderCurrentPos = m_plnkTabMounter->GetCurrentPos();
			
			if (fabs(dCommandCurrentPos - dEncoderCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
			{
				Sleep(500);
				dEncoderCurrentPos = m_plnkTabMounter->GetCurrentPos();//170215 SJ_YSH Add..
				if (fabs(dCommandCurrentPos - dEncoderCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
				{
					Sleep(500);
					dEncoderCurrentPos = m_plnkTabMounter->GetCurrentPos();//170215 SJ_YSH Add..
					if (fabs(dCommandCurrentPos - dEncoderCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
					{
						Sleep(500);
						dEncoderCurrentPos = m_plnkTabMounter->GetCurrentPos();//170215 SJ_YSH Add..
						//170215 SJ_YSH Retry 1회 및 Trace 추가
						if (fabs(dCommandCurrentPos - dEncoderCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
						{
							SetErrorLevel(_T("TAB MOUNTER move to Mount Position"),1, __FILE__, __LINE__);
							// 323015 = Inspection 위치의 Command 값과 Encoder 값 차가 Mount Pos Tolerance 값을 초과함.
							TRACE(_T("TabMounter%d : Command : %d, Encoder %d\n"), m_iWorkInstance + 1, dCommandCurrentPos, dEncoderCurrentPos);
							return generateErrorCode(323015);
						}
					}
				}
			}
		}
#endif
		
		dTargetPos = dCommandCurrentPos + dInspectionOffset + dToolOffset + dTabOffset;

		const double DEF_LINEAR_BACKLASH_OFFSET = 0.1;
		/*
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
		{
			// Prealign 위치 => Inspection 위치 이동 방향 : (+)
			// Mount 위치로 이동할 때도 항상 (+) 측으로 갈 수 있게 할 것.
			if (dTargetPos < dCommandCurrentPos + 0.001)
			{
				dBacklash = DEF_LINEAR_BACKLASH_OFFSET;
				dTargetPos -= DEF_LINEAR_BACKLASH_OFFSET;
			}
		}
		else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
		{
			int iGroupNo = m_plnkTabMounter->GetTabMounterGroupNo();
			if (DEF_FRONT_GROUP == iGroupNo)
			{
				// Ready 위치 => Inspection 위치 이동 방향 : (-)
				// Mount 위치로 이동할 때도 항상 (-) 측으로 갈 수 있게 할 것.
				if (dTargetPos > dCommandCurrentPos - 0.001)
				{
					dBacklash = -DEF_LINEAR_BACKLASH_OFFSET;
					dTargetPos += DEF_LINEAR_BACKLASH_OFFSET;
				}
			}
			else // if (DEF_REAR_GROUP == iGroupNo)
			{
				// Ready 위치 => Inspection 위치 이동 방향 : (+)
				// Mount 위치로 이동할 때도 항상 (+) 측으로 갈 수 있게 할 것.
				if (dTargetPos < dCommandCurrentPos + 0.001)
				{
					dBacklash = DEF_LINEAR_BACKLASH_OFFSET;
					dTargetPos -= DEF_LINEAR_BACKLASH_OFFSET;
				}
			}
		}
		*/
		if (dTargetPos < dCommandCurrentPos + 0.001)
		{
			dBacklash = DEF_LINEAR_BACKLASH_OFFSET;
			dTargetPos -= DEF_LINEAR_BACKLASH_OFFSET;
		}
	}
	/*
	else
	{
		bWaitOption = FALSE;
		dTargetPos = m_plnkTabMounter->GetTargetPos(DEF_TABMOUNTER_MOUNT_POS, iRealTabNo);
	}
	*/

	int iResult = m_plnkTabMounter->MovePos(dTargetPos, DEF_TABMOUNTER_MOUNT_POS, bWaitOption);
	if (iResult)
		return iResult;

	double dCommandCurrentPos2 = 0.0;
	double dTargetPos2 = dTargetPos;
	if (0.0 != dBacklash)
	{
		dCommandCurrentPos2 = m_plnkTabMounter->GetCurrentPos(TRUE);
		dTargetPos2 = dCommandCurrentPos2 + dBacklash;

#ifndef SIMULATION
		if (fabs(dTargetPos - dCommandCurrentPos2) > m_plnkSystemData->m_dMountPosTolerance)
		{
			SetErrorLevel(_T("TAB MOUNTER move to Mount Position"),1, __FILE__, __LINE__);
			// 323017 = Backlash 동작 수행 시, 최초 목표 위치에 대한 Motion Tolerance 초과.
			return generateErrorCode(323017);
		}
#endif

		iResult = m_plnkTabMounter->MovePos(dTargetPos2, DEF_TABMOUNTER_MOUNT_POS);
		if (iResult)
			return iResult;
	}

	/*
	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		CString strLog;
		double dModelPosByTabData = m_plnkWorkSchedule->GetTabPos(iRealTabNo);
		double dTabPrealignOffset = m_plnkProcessData->GetTabPrealignOffset(eWorkSide, iRealTabNo);
		
//		if (TRUE == m_plnkSystemData->m_bUseInspectCamera)
		{
			double dAfterCurrentPos = m_plnkTabMounter->GetCurrentPos();

			strLog.Format(_T("MoveToMountPos :,Mounter%d,tab%02d,[ModelPos,%.3f,] [ToolOffset,%.3f,] [TabOffset,%.3f,] [Tab Prealign Offset,%.3f,] [InspectOffset,%.3f,] [Backlash,%.3f,] [Before(C),%.3f,] [Before(E),%.3f,] [Target,%.3f,] [After(bl),%.3f,] [Target(final),%.3f,] [After,%.3f,]"),
				m_iInstanceNo, iRealTabNo, dModelPosByTabData, dToolOffset, dTabOffset, dTabPrealignOffset, dInspectionOffset, dBacklash, dCommandCurrentPos, dEncoderCurrentPos, dTargetPos, dCommandCurrentPos2, dTargetPos2, dAfterCurrentPos);
		}
		/ *
		else
		{
			dToolOffset = m_plnkSystemData->m_rgdToolOffsetMounter[m_iInstanceNo];
			dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(eWorkSide, iRealTabNo);
			
			strLog.Format(_T("MoveToMountPos :,Mounter%d,tab%02d,[ModelPos,%.3f,] [ToolOffset,%.3f,] [TabOffset,%.3f,] [Tab Prealign Offset,%.3f,],__,__,__,__,__,__,__,[Target,%.3f,],__,__,__,__,__,__"),
				m_iInstanceNo, iRealTabNo, dModelPosByTabData, dToolOffset, dTabOffset, dTabPrealignOffset, dTargetPos);
		}
		* /
		m_plnkVision->WriteLog(_T("..\\Log\\move_to_mount_pos_log.csv"), (LPCTSTR)strLog);
	}
	*/
	
	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

int MCtrlTabMounter::GetPanelRefFiduMarkPosY(double* pdPanelRefFiduMarkPosY)
{
#ifdef SIMULATION
	return ERR_CTRL_TABMOUNTER_SUCCESS;
#endif

	if (FALSE == m_plnkSystemData->m_bUsePanelAbsorbOffsetReflection)
		return ERR_CTRL_TABMOUNTER_SUCCESS;

	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
		return ERR_CTRL_TABMOUNTER_SUCCESS;
		
	return ERR_CTRL_TABMOUNTER_SUCCESS;

	/*
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		return ERR_CTRL_TABMOUNTER_SUCCESS;

	m_plnkVision->LockCamera(DEF_PANEL_ALIGN_CAMERA_1);

	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);
	int iResult = m_plnkVision->Grab(DEF_PANEL_ALIGN_CAMERA_1);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
		SetErrorLevel(_T("TAB MOUNTER get Panel Reference Fidu Mark Position Y"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);
		SetErrorLevel(_T("TAB MOUNTER get Panel Reference Fidu Mark Position Y"),1, __FILE__, __LINE__);
		return iResult;
	}
	
	double dResultX = 0.0;
	double dResultY = 0.0;
	m_plnkVisionCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, NULL,
										m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK),
										m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK),
										dResultX, dResultY);

	m_plnkVision->UnlockCamera(DEF_PANEL_ALIGN_CAMERA_1);

	*pdPanelRefFiduMarkPosY = dResultY;

	return ERR_CTRL_TABMOUNTER_SUCCESS;
	*/
}

int MCtrlTabMounter::MoveForPanelAbsorbOffsetReflection(EWorkingSide eWorkSide, int iTabNo, double dPanelAbsorbOffset)	//@--->미사용 함수
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		return ERR_CTRL_TABMOUNTER_SUCCESS;

	// 전제 : TabMounter 는 이미 모든 Offset 을 적용해서 Mount 위치까지 이동 완료하고 있어야 한다.

	// 현재 위치(Mount 위치) 에 Panel Absorb Offset 을 더해서 상대 이동하는 개념으로 적용.
	// 단, 이 때 TabMounter 의 Current 위치는 Command 로 읽어 낸다. (gain 등의 문제로 Axis 떨고 있는 상황 등에 대한 대응)
	double dCommandCurrentPos = m_plnkTabMounter->GetCurrentPos(TRUE);
	double dEncoderCurrentPos = m_plnkTabMounter->GetCurrentPos();

	if (fabs(dCommandCurrentPos - dEncoderCurrentPos) > m_plnkSystemData->m_dMountPosTolerance)
		// 323016 = Mount 위치의 Command 값과 Encoder 값 차가 Mount Pos Tolerance 값을 초과함.
		return generateErrorCode(323016);

	double dTargetPos = dCommandCurrentPos + dPanelAbsorbOffset;

	double dBacklash = 0.0;
	const double DEF_LINEAR_BACKLASH_OFFSET = 0.1;
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		// Prealign 위치 => Inspection 위치 이동 방향 : (+)
		// Mount 위치로 이동할 때도 항상 (+) 측으로 갈 수 있게 할 것.
		if (dTargetPos < dCommandCurrentPos + 0.001)
		{
			dBacklash = DEF_LINEAR_BACKLASH_OFFSET;
			dTargetPos -= DEF_LINEAR_BACKLASH_OFFSET;
		}
	}
	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		int iGroupNo = m_plnkTabMounter->GetTabMounterGroupNo();
		if (DEF_FRONT_GROUP == iGroupNo)
		{
			// Prealign 위치 => Inspection 위치 이동 방향 : (-)
			// Mount 위치로 이동할 때도 항상 (-) 측으로 갈 수 있게 할 것.
			if (dTargetPos > dCommandCurrentPos - 0.001)
			{
				dBacklash = -DEF_LINEAR_BACKLASH_OFFSET;
				dTargetPos += DEF_LINEAR_BACKLASH_OFFSET;
			}
		}
		else // if (DEF_REAR_GROUP == iGroupNo)
		{
			// Prealign 위치 => Inspection 위치 이동 방향 : (+)
			// Mount 위치로 이동할 때도 항상 (+) 측으로 갈 수 있게 할 것.
			if (dTargetPos < dCommandCurrentPos + 0.001)
			{
				dBacklash = DEF_LINEAR_BACKLASH_OFFSET;
				dTargetPos -= DEF_LINEAR_BACKLASH_OFFSET;
			}
		}
	}

	int iResult = m_plnkTabMounter->MovePos(dTargetPos, DEF_TABMOUNTER_MOUNT_POS);
	if (iResult)
		return iResult;

	double dCommandCurrentPos2 = 0.0;
	double dTargetPos2 = dTargetPos;
	if (0.0 != dBacklash)
	{
		dCommandCurrentPos2 = m_plnkTabMounter->GetCurrentPos(TRUE);
		dTargetPos2 = dCommandCurrentPos2 + dBacklash;

#ifndef SIMULATION
		if (fabs(dTargetPos - dCommandCurrentPos2) > m_plnkSystemData->m_dMountPosTolerance)
		{
			SetErrorLevel(_T("TAB MOUNTER move to Absorb Panel(Offset Reflection)"),1, __FILE__, __LINE__);
			// 323017 = Backlash 동작 수행 시, 최초 목표 위치에 대한 Motion Tolerance 초과.
			return generateErrorCode(323017);
		}
#endif

		iResult = m_plnkTabMounter->MovePos(dTargetPos2, DEF_TABMOUNTER_MOUNT_POS);
		if (iResult)
			return iResult;
	}

	/*
	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		CString strLog;
		double dModelPosByTabData = m_plnkWorkSchedule->GetTabPos(iTabNo);
		double dTabPrealignOffset = m_plnkProcessData->GetTabPrealignOffset(eWorkSide, iTabNo);
		double dInspectionOffset = m_plnkProcessData->GetInspectionOffset(eWorkSide, iTabNo);
		double dToolOffset = m_plnkSystemData->m_rgdToolOffsetMounter[m_iInstanceNo];
		double dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(eWorkSide, iTabNo);
		double dAfterCurrentPos = m_plnkTabMounter->GetCurrentPos();
		
		strLog.Format(_T("MoveForPanelAbsorbOffsetReflection :,Mounter%d,tab%02d,[ModelPos,%.3f,] [ToolOffset,%.3f,] [TabOffset,%.3f,] [Tab Prealign Offset,%.3f,] [InspectOffset,%.3f,] [Backlash,%.3f,] [Before(C),%.3f,] [Before(E),%.3f,] [Target,%.3f,] [After(bl),%.3f,] [Target(final),%.3f,] [After,%.3f,] [PanelAbsorbOffset,%.3f,]"),
			m_iInstanceNo, iTabNo, dModelPosByTabData, dToolOffset, dTabOffset, dTabPrealignOffset, dInspectionOffset, dBacklash, dCommandCurrentPos, dEncoderCurrentPos, dTargetPos, dCommandCurrentPos2, dTargetPos2, dAfterCurrentPos, dPanelAbsorbOffset);
		
		m_plnkVision->WriteLog(_T("..\\Log\\move_to_mount_pos_log.csv"), (LPCTSTR)strLog);
	}
	*/

	return ERR_CTRL_TABMOUNTER_SUCCESS;
}

int MCtrlTabMounter::getWorkTabAlignerIdPerGroup(EWorkingSide eWorkSide, int iTabNo, int& iReturn)
{
	iReturn = m_plnkProcessData->GetWorkTabAlignerID(eWorkSide, iTabNo);
	if (-1 == iReturn)
		// 323001 = 사용할 수 있는 TabAligner 없음. [TabAligner 사용 Data 확인 필요.]
		return generateErrorCode(323001);

	return ERR_CTRL_TABMOUNTER_SUCCESS;
}
