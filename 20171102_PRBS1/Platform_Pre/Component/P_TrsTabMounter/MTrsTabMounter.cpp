/* 
 * TrsTabMounter Component
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
 * MTrsTabMounter.cpp : Implementation of MTrsTabMounter component.
 */

#include "Stdafx.h"
#include "MSystemData.h"
#include "MProcessData.h"
#include "MTabMounter.h"
#include "IStatePanelTransfer.h"
//#include "IStateRollerConveyor.h"
#include "MCtrlTabMounter.h"
#include "MTrsTabMounter.h"
#include "MTrsAutoManager.h"
#include "MCtrlTabFeeder.h"
#include "ITI_PanelAligner.h"
#include "ITI_TabCarrier.h"
#include "ITI_InspectionCamera.h"
#include <math.h>
#include "SystemThreeBtnDlg.h"
#include "MNetH.h"
#include "MPlatformOlbSystem.h"
#include "common.h"
#include "IAxis.h"
#include "MTabCarrier.h"
#include "MTrsInspectionCamera.h"
#include "MTrsPanelAligner.h"
#include "MCameraCarrier.h"
#include "MTrsTabCarrier.h"

//20110910 SJ_HJG
#include "MPreBonderData.h"
#include "MPanelAligner.h"

BOOL MTrsTabMounter::sm_bITIAllInspectionCompleteToPA[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabMounter::sm_bITIAllMountCompleteToPA[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabMounter::sm_bITIAllInspectionCompleteToPASecond[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabMounter::sm_bITIAllMountCompleteToPASecond[DEF_MAX_GROUP] = {FALSE, FALSE};

// sesl_jdy
//BOOL MTrsTabMounter::sm_rgbMountComplete[DEF_MAX_TABMOUNTER];

CRITICAL_SECTION MTrsTabMounter::sm_csTrashBin;
BOOL MTrsTabMounter::sm_bEmergencyStopForTrashBin;

//===================================================================
BOOL MTrsTabMounter::m_bLoadReady[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
BOOL MTrsTabMounter::sm_bRemoveTab[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP] = 
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};

BOOL MTrsTabMounter::m_bAllLoadReady[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabMounter::m_bGroupInspectionComplete[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabMounter::m_bGroupMountStart[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabMounter::m_bGroupMountComplete[DEF_MAX_GROUP] = {FALSE, FALSE};
int	MTrsTabMounter::m_iKeyValue[DEF_MAX_GROUP] = {-99, -99};
int MTrsTabMounter::m_iFirstWorkToolNo[DEF_MAX_GROUP] = { -1, -1};
int	MTrsTabMounter::sm_iPickoutGroup = -1;
int	MTrsTabMounter::sm_iPressGroup = -1;
int MTrsTabMounter::sm_iWorkTabNo[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP] = 
{
#ifdef DEF_SOURCE_SYSTEM
	{-1, -1, -1, -1},	{-1, -1, -1, -1},
#else
	{-1, -1},	{-1, -1},
#endif
};

BOOL MTrsTabMounter::sm_bCycleWorkComplete[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP] = 
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};

int	MTrsTabMounter::m_iScheduleNo[] = {0, 0};
BOOL MTrsTabMounter::m_bPressCompleteSchedule[] = {FALSE, FALSE};

BOOL MTrsTabMounter::sm_bGroupLoadComplete[DEF_MAX_GROUP] = { FALSE, FALSE };
BOOL MTrsTabMounter::m_bNextPosCheckComplete[DEF_MAX_GROUP] = {FALSE, FALSE };

extern MPlatformOlbSystem	MOlbSystemPre;

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/***********************************************************************************************/
BOOL MTrsTabMounter::m_bITIExecuteWorking[DEF_MAX_GROUP] = {FALSE, FALSE};

MTrsTabMounter::MTrsTabMounter(SCommonAttribute commonData, STrsTabMounterRefCompList listRefComponents, STrsTabMounterData datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_iInstanceNo < DEF_MOUNTER_WORK_PER_GROUP)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iOtherWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iOtherWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = DEF_MAX_TABMOUNTER - 1 - m_iInstanceNo;
	}
#else
	/*/
	if (m_iInstanceNo < DEF_MOUNTER_WORK_PER_GROUP)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iOtherWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iOtherWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo - DEF_MAX_TABMOUNTER;
	}
//@#endif

	m_bAutoInitialized = FALSE;

	m_bThreadLife = FALSE;
	m_pThread = NULL;
	m_bAutoRun = FALSE;

	m_bTabMounterVacOn = FALSE; //161221 SJ_YSH
	m_eWorkSide = m_plnkTabMounter->GetTabMounterWorkSide();
	m_iGroupNo = m_plnkTabMounter->GetTabMounterGroupNo();
	m_iPreTabMounterNo = m_plnkTabMounter->GetPreTabMounterNo();
	m_iNextTabMounterNo = m_plnkTabMounter->GetNextTabMounterNo();

//	m_iTabMounterNoInGroup = m_iInstanceNo - m_iGroupNo * DEF_MAX_WORKER_PER_GROUP;
	m_iInspectorUnitNoInGroup = DEF_NONE_INSPECTION_CAMERA;
	m_iMounterTabNo = DEF_NO_WORK;
	
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType && DEF_TABMOUNTER1 == m_iInstanceNo)
		InitializeCriticalSection(&sm_csTrashBin);	// 쓰레기통 점유권 초기화

	sm_bEmergencyStopForTrashBin = FALSE;

	m_dPanelInspectPosFiduMarkPosY = 0.0;
	m_dPanelMountPosFiduMarkPosY = 0.0;

	//===========================================
	m_uiRetryCount = 0;
	m_bGroupMountStart[m_iWorkGroup] = FALSE;
	m_bGroupMountComplete[m_iWorkGroup] = FALSE;
	sm_iPickoutGroup = -1;
	sm_iPressGroup = -1;
	m_iWorkCarrierNo = -1;
	sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = -1;
	sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;

	m_bLoadReady[m_iWorkGroup][m_iInstanceNo] = FALSE;
	m_bAllLoadReady[m_iWorkGroup] = FALSE;
	m_bGroupInspectionComplete[m_iWorkGroup] = FALSE;
	m_iKeyValue[m_iWorkGroup] = -99;
	sm_bCycleWorkComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	//===========================================


	
	initializeITI();
	initializeStep();

	//20131115 SJ_HJG
	m_dMountInspectionDown = 0.0;
	m_dMountInspectionTime = 0.0;
	m_dMountInspectionUp = 0.0;
	m_dMountMove = 0.0;
	m_dMountBondingDown = 0.0;
	m_dMountBondingTime = 0.0;
	m_dMountBondingUp = 0.0;
}

MTrsTabMounter::~MTrsTabMounter()
{
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType && DEF_TABMOUNTER1 == m_iInstanceNo)
		DeleteCriticalSection(&sm_csTrashBin);
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
void MTrsTabMounter::simulLog(CString strMsg, int iStep, int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s : %s(%d)\n"), GetObjectName(), strMsg, iStep);

	FILE *fp = fopen( ".\\StepLog.txt", "a");
	fprintf(fp, "%s\n", LPSTR(LPCTSTR(strLog)));
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

/** Thread 함수*/
UINT MTrsTabMounter::threadFunction(LPVOID pParam)
{
	// TrsTabMounter Object Pointer Assign
	MTrsTabMounter* pThis = (MTrsTabMounter*) pParam;

	TRACE(_T("MTrsTabMounter Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsTabMounter Thread Stop\n"));

	return ERR_TRS_TABMOUNTER_SUCCESS;
}

void MTrsTabMounter::threadJob()
{
	switch (m_plnkTrsAutoManager->GetOPMode())
	{
	case MANUAL_MODE:
		m_bAutoRun = FALSE;
		break;

	case AUTORUN_MODE:
		switch (m_plnkTrsAutoManager->GetOPStatus())
		{
		case ERROR_STOP:
			/* FALL THROUGH */
		case STEP_STOP:
			///m_bITIInspectionAbleToIC = FALSE;
//Del.			m_plnkTabMounter->UpCyl();
			//@131127.KKY___________
			m_plnkTabMounter->UpPusherCyl(TRUE);
			//@_____________________

			//100930.KKY_____
			m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
			//_______________
			m_TimerError.StopTimer();
			m_bAutoRun = FALSE;
			break;

		case START_RUN:
			m_bAutoRun = TRUE;
			break;

		case CYCLE_STOP:
			break;

		case RUN:
			doRunStep();
			break;			
		}
		break;
	}
}

void MTrsTabMounter::doRunStep()
{
	int iResult = ERR_TRS_TABMOUNTER_SUCCESS;
	int i = 0;

	//Break for Debugging_____
	if (m_iWorkGroup == 0)
	{
		int k = 0;
		if (isGroupStarter())
			int l = 0;
		if(m_iWorkInstance == 0)
			int ew = 0;
		if(m_iWorkInstance == 1)
			int ew = 0;
		if(m_iWorkInstance == 2)
			int ew = 0;
		if(m_iWorkInstance == 3)
			int ew = 0;

	}
	else
	{
		int k = 0;
		if (isGroupStarter())
			int l = 0;
	}
	//______________
	if(m_iWorkGroup == DEF_REAR_GROUP)
	{
		return;
	}

	//170612 JSH.s
	//스텝 관계없이 Inspection Camea TabIC Blob 검사중에는 정지...
	if (m_rgplnkITI_InspectionCamera[0]->IsTabBlobWorkingToTM(m_iWorkGroup) == TRUE)
		return;
	//170612 JSH.e


	ITI_TabCarrier*	 pITI_TabCarrier = NULL;
	int iPosID = DEF_TABMOUNTER_READY_POS;
	double dTargetPos = 0.0;

	switch (m_estepCurrent)
	{
	case TABMOUNTER_MOVE_TO_LOAD_POS:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABMOUNTER_MODEL_CHANGE);
			break;
		}

		if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetUseWorkSide()
			|| m_plnkSystemData->m_eRunMode == PASS_RUN_MODE)
		{
			//@130204.Add.PanelPusher사용시 미사용 작업변 이라도 Align에러시 Pusher사용때문에 작업위치로 이동해야함___
			//@131127.KKY______________
//@			if (TRUE == m_plnkSystemData->m_bUsePanelPusher)
			if (m_plnkSystemData->m_bUsePanelPusher != 0)
			//@________________________
			{
				if (TRUE == isGroupStarter())
				{
					double dTgPos = m_plnkCameraCarrier->GetTargetPos(0, DEF_CAMERACARRIER_PANEL_MARK_POS)+20.0;
					if (FALSE == m_plnkTabMounter->IsInPos(dTgPos))
					{
						m_plnkTabMounter->SetGantryMotionEnable(TRUE);
						iResult = m_plnkTabMounter->MovePos(dTgPos, DEF_TABMOUNTER_INSPECTION_POS);
						if (iResult)
						{
							m_plnkTabMounter->SetGantryMotionEnable(FALSE);
							PROCESS_ALARM(iResult);
							break;
						}
						m_plnkTabMounter->SetGantryMotionEnable(FALSE);
					}
					if (FALSE == m_plnkTabMounter->IsInGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS))
					{
						iResult = m_plnkTabMounter->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS);
						if (iResult)
						{
							m_plnkTabMounter->SetGantryMotionEnable(FALSE);
							PROCESS_ALARM(iResult);
							break;
						}
					}
					m_bITITabMounterReadyPusherDownToPA = TRUE;
				}
			}
			//@_______________________________________________________________________________________________________
			break;
		}

		if (TRUE == isGroupStarter())
		{			
			iPosID = DEF_TABMOUNTER_LOAD_POS;
			if (TRUE == m_plnkITI_TabCarrier[DEF_MAX_TABCARRIER-1]->IsLoadingCompleteToTM())
			{
				if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_SupplyWorkList(m_iWorkGroup))
				{
					iResult = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->PickOut_SupplyWorkList(m_iWorkGroup);
					if (iResult == 0)
					{
						m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
						if (m_TabInfo.m_nTabNo == -1)
						{
							iPosID = DEF_TABMOUNTER_ESCAPE_POS;
						}
					}
				}
				else
				{
					m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
					if (m_TabInfo.m_nTabNo == -1)
					{
						iPosID = DEF_TABMOUNTER_ESCAPE_POS;
					}
				}
			}
		}
		else if (m_iWorkInstance == 1)
		{
			if (TRUE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_SupplyWorkList(m_iWorkGroup))
			{
				MTabInfo prevInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, 0);
				m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
				//@1번 Mounter가 작업하지 않고, 2번 Mounter가 작업하면 Loading위치로 바로 이동한다.
				if (prevInfo.m_nTabNo == -1 && m_TabInfo.m_nTabNo != -1)
				{
					m_iWorkCarrierNo = m_iWorkInstance;
					iPosID = getLoadPosID();
				}
			}
		}
		else
			iPosID = DEF_TABMOUNTER_READY_POS;

		// PassRun Mode 인 경우 TabMounter 동작 없음.
		if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE && TRUE == m_plnkTabMounter->IsInPos(m_iScheduleNo[m_iWorkGroup], iPosID))
			break;

		simulLog("TABMOUNTER_MOVE_TO_LOAD_POS", m_estepCurrent, 100);
	
		if (TRUE == isGroupStarter())
			m_plnkTabMounter->SetGantryMotionEnable(TRUE);

		if (FALSE == m_plnkTabMounter->IsInPos(m_iScheduleNo[m_iWorkGroup], iPosID))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], iPosID, -1);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
//@#ifdef DEF_SOURCE_SYSTEM
//@					PROCESS_ALARM(generateErrorCode(425010));
//@#else
					PROCESS_ALARM(generateErrorCode(425028));
//@#endif
				}
				if (TRUE == isGroupStarter())
					m_plnkTabMounter->SetGantryMotionEnable(FALSE);
				break;
			}
			m_TimerError.StopTimer();
			//161221 SJ_YSH
			if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE)
			{
				if(m_bTabMounterVacOn == FALSE)
				{	
					iResult =m_plnkTabMounter->AbsorbTabIC(TRUE);
					if (iResult)
					{
						PROCESS_ALARM(iResult);
						break;
					}
				}
			}
			//___________
			iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//161221 SJ_YSH
			if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE)
			{
				if(m_bTabMounterVacOn == FALSE)
				{	
					if(m_plnkTabMounter->IsAbsorbTabIC())
					{
						if(m_iWorkGroup == DEF_FRONT_GROUP)
							iResult = generateErrorCode(425032);
						else
							iResult = generateErrorCode(425033);
					}
					if(m_plnkTabMounter->CheckVacuum())
					{
						m_plnkTabMounter->ReleaseTabIC();
						iResult = 0;
					}
					
					if(iResult != 0)
					{
						PROCESS_ALARM(iResult);
						break;
					}
				}
			}
			//___________
		}
		if (TRUE == isGroupStarter())
			m_plnkTabMounter->SetGantryMotionEnable(FALSE);

		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = TRUE;
		
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		setStep(TABMOUNTER_PICKOUT_WORK);
		break;

	case TABMOUNTER_PICKOUT_WORK:

		if (TRUE == isGroupStarter())
		{
			int iTabCarrierStarter = MOlbSystemPre.GetTrsTabCarrierComponent(0)->GetGroupStarter();
			//처음 공금하는 Carrier가 Unloading위치로 이동했는지 확인
//@			if (FALSE == m_plnkITI_TabCarrier[DEF_MAX_TABCARRIER-1]->IsUnloadingMoveCompleteToTM())
			if (FALSE == m_plnkITI_TabCarrier[iTabCarrierStarter]->IsLoadingCompleteToTM())
				break;
		}

		//Group Starter일 경우 WorkSchedule로부터 작업을 구해온다
		if (TRUE == isGroupStarter() && TRUE == isAllLoadReady())
		{
			m_bAllLoadReady[m_iWorkGroup] = FALSE;

			if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_SupplyWorkList(m_iWorkGroup))
			{
				iResult = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->PickOut_SupplyWorkList(m_iWorkGroup);
				if (iResult != 0)
					break;
			}

			m_iKeyValue[m_iWorkGroup] = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetKeyValue_SupplyWorkList(m_iWorkGroup);
			m_iFirstWorkToolNo[m_iWorkGroup] = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetFirstWorkToolNo_FromCurrentSupplyList(m_iWorkGroup);
			m_bAllLoadReady[m_iWorkGroup] = TRUE;
		}

		if (FALSE == m_bAllLoadReady[m_iWorkGroup])
			break;

		m_bNextPosCheckComplete[m_iWorkGroup] = FALSE;

		sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = -1;
		m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
		
		sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = m_TabInfo.m_nTabNo;
		setStep(TABMOUNTER_MOVE_LOAD_READY);
		break;

	case TABMOUNTER_MOVE_LOAD_READY:
		if (m_TabInfo.m_nTabNo == -1)
		{
			m_iWorkCarrierNo = -1;
			//@130105_____________
//@			setStep(TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT);
			setStep(TABMOUNTER_MOVE_TO_ESCAPE_POS);
			//____________________
			break;
		}

		setStep(TABMOUNTER_MOVE_LOAD);
		break;

	case TABMOUNTER_MOVE_LOAD:
		//110420.Add_____
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABMOUNTER_MODEL_CHANGE);
			break;
		}
		//_______________

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		m_iWorkCarrierNo = m_iWorkInstance;
//			m_plnkTabMounter->SetGantryMotionEnable(TRUE, m_iWorkCarrierNo);
		m_plnkTabMounter->SetGantryMotionEnable(TRUE);

		iPosID = getLoadPosID();

		if (FALSE == m_plnkTabMounter->IsInPos(m_iScheduleNo[m_iWorkGroup], iPosID))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();

			iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], iPosID, -1);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(425010));
				}
//					m_plnkTabMounter->SetGantryMotionEnable(FALSE, m_iWorkCarrierNo);
				m_plnkTabMounter->SetGantryMotionEnable(FALSE);
				break;
			}
			m_TimerError.StopTimer();
			iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
//			m_plnkTabMounter->SetGantryMotionEnable(FALSE, m_iWorkCarrierNo);
		m_plnkTabMounter->SetGantryMotionEnable(FALSE);
		setStep(TABMOUNTER_LOAD);
		break;

	case TABMOUNTER_LOAD:
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
//			if (FALSE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
//				setStep(TABMOUNTER_MOVE_FOR_REMOVE_READY);
//			else
				setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABMOUNTER_MODEL_CHANGE);
			break;
		}

		if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE)
		{
			setStep(TABMOUNTER_MOVE_TO_LOAD_POS);
			break;
		}

		//SJ_YYK 150331 Add..
		iPosID = getLoadPosID();
		if (FALSE == m_plnkTabMounter->IsInPos(m_iScheduleNo[m_iWorkGroup], iPosID))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();

			iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], iPosID, -1);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(425010));
				}
//					m_plnkTabMounter->SetGantryMotionEnable(FALSE, m_iWorkCarrierNo);
				m_plnkTabMounter->SetGantryMotionEnable(FALSE);
				break;
			}
			m_TimerError.StopTimer();
			iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//____________________*/

		if (m_iWorkCarrierNo != -1)
			pITI_TabCarrier = m_plnkITI_TabCarrier[m_iWorkCarrierNo];

		if (pITI_TabCarrier == NULL)
			break;

		// TabCarrier 의 TabIC SendAble 상태 확인.
		if (pITI_TabCarrier->IsTabICSendAbleToTM() == FALSE)
			break;
//@		pITI_TabCarrier->SetTabNo(m_TabInfo.m_nTabNo);
		pITI_TabCarrier->SetTabNo(m_TabInfo.m_nRealTabNo);

		m_bITITabICReceiveStartToTC = TRUE;

		while (TRUE)
		{
			if (TRUE == pITI_TabCarrier->IsTabICSendStartToTM())
				break;

			if (FALSE == pITI_TabCarrier->IsTabICSendAbleToTM())
			{
				m_bITITabICReceiveStartToTC = FALSE;
				return;
			}
			Sleep(5);
		}
		
		simulLog("TABMOUNTER_LOAD", m_estepCurrent, 100);

		// TabIC Load 작업 수행.
		//TabMounter가 이미 TabIC를 흡착한 상태라도 상관 없음.
		{
			BOOL TabICLoading = FALSE;//170830 JSH
			iResult = m_plnkCtrlTabMounter->LoadTabIC(m_iWorkGroup, m_iWorkCarrierNo, &TabICLoading);
			if (iResult)
			{
				m_bITITabICReceiveStartToTC = FALSE;
				m_bITITabICReceiveCompleteToTC = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}

			pITI_TabCarrier->SetTabICUnloadToTabMounter(m_iWorkGroup, m_iWorkCarrierNo, TabICLoading); //170830 JSH			
		}

		m_uiRetryCount = 0;

		//TabRemove후 Reload시 SupplyWorkList==NULL상태임...
		if (FALSE == isExistRemoveTab())
		{
			//TrsTabCarrier에서 하지 않고 TrsTabMounter에서 Update한다
			m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->SetMounterTabLoadComplete_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
			m_TabInfo.m_jobInfo.m_bToolLoadComplete = TRUE;
		}

		// Load 완료 신호 설정.
		m_bITITabICReceiveCompleteToTC = TRUE;

		// TabCarrier : Tab Unload Step 완료 확인.
		while (TRUE)
		{
			if (pITI_TabCarrier->IsTabICSendStartToTM() == FALSE)
				break;

			Sleep(5);
		}

		sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;

		m_bITITabICReceiveStartToTC = FALSE;
		m_bITITabICReceiveCompleteToTC = FALSE;

		setStep(TABMOUNTER_MOVE_TO_ESCAPE_POS);
		break;

	case TABMOUNTER_MOVE_TO_ESCAPE_POS:
		//Next Tool중 Loading할 Tool이 있으면 
		//SJ_YYK 150330 Modify...
		//if (TRUE == isExistNextLoadTab())
		if (TRUE == isExistNextLoadTab() && m_plnkTabMounter->IsAbsorbTabIC() == FALSE)
		{
			if (FALSE == m_plnkTabMounter->IsInPos(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_ESCAPE_POS))
			{
				if (FALSE == m_TimerError.IsTimerStarted())
					m_TimerError.StartTimer();
				iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_ESCAPE_POS, -1);
				if (iResult)
				{
					if (m_TimerError.MoreThan(30.0))
					{
						m_TimerError.StopTimer();
						PROCESS_ALARM(generateErrorCode(425029));
					}
					break;
				}
				m_TimerError.StopTimer();
				iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_ESCAPE_POS);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
		setStep(TABMOUNTER_ALL_LOAD_COMPLETE_WAIT);
		break;

	case TABMOUNTER_ALL_LOAD_COMPLETE_WAIT:
		if (TRUE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsLoadComplete_SupplyList(m_iKeyValue[m_iWorkGroup]))
		{
			if (TRUE == isGroupStarter() && FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_CurrentWorkList())
			{
				iResult = m_plnkTabMounter->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
			setStep(TABMOUNTER_WAIT_INSPECTION_READY);
		}
		break;

		//Inspection과 MounterGroup의 작업List가 일치하는지 확인한다.
	case TABMOUNTER_WAIT_INSPECTION_READY:
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetKeyValue_CurrentWorkList() == m_iKeyValue[m_iWorkGroup])
		{
			//새로운 TabData를 가지고 온다
			m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_CurrentWorkList(m_iWorkInstance);
			setStep(TABMOUNTER_MOVE_TO_INSPECTION_POS);
		}
		break;

	case TABMOUNTER_MOVE_TO_INSPECTION_POS:

		simulLog("TABMOUNTER_MOVE_TO_INSPECTION_POS", m_estepCurrent, 100);
		
		/*/
		//20110910 SJ_HJG
		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			if(MOlbSystemPre.GetPreBonderData()->m_dLength_between_ChipCenter_and_TabEnd > 30)
			{
				//TabIC 길이가 30 보다 큰경우 
				//Panel Aligner가 Paenl을 가지고 있고 Load Postion이 아닌 경우에 TABMOUNTER를 INSPECTION_POS으로 이동 시킨다!
				if(MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed() == FALSE)
					break;

				if(MOlbSystemPre.GetPanelAlignerComponent()->IsInXYTPos(DEF_PANEL_ALIGNER_LOAD_POS))
					break;
			}
		}
		/*/

		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();

		if (m_iWorkInstance == m_iFirstWorkToolNo[m_iWorkGroup])
			m_plnkTabMounter->SetGantryMotionEnable(TRUE);

		//@130105____________
		//Inspection하지 않는 Tool이 있기 때문에 목표위치를 구하여야 한다(Inspect Carrier와 동일)
//@		dTargetPos = m_plnkTabMounter->CalculateMountPos(m_TabInfo.m_dModelOffset);
		if (m_TabInfo.m_nTabNo == -1)
		{
			/*/
			if (TRUE == isExistNextLoadTab())
			{
				#pragma message(__LOC__ "여기 어디로 갈지 위치 지정")
				//@iPosID = DEF_TABMOUNTER_ESCAPE_POS;
				iPosID = DEF_TABMOUNTER_INSPECTION_POS;
				dTargetPos = m_plnkTabMounter->GetTargetPos(m_iScheduleNo[m_iWorkGroup], iPosID, 0);
			}
			else
			{
				iPosID = DEF_TABMOUNTER_READY_POS;
				dTargetPos = m_plnkTabMounter->GetTargetPos(m_iScheduleNo[m_iWorkGroup], iPosID);
			}
			/*/
			iPosID = DEF_TABMOUNTER_INSPECTION_POS;
			MInspectCarrierInfo	m_InspectInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetInspectCarrierInfo_FromCurrentWorkList(m_iWorkInstance);
			dTargetPos = m_plnkTabMounter->CalculateMountPos(m_InspectInfo.m_dInspectPos);
		}
		else
		{
			iPosID = DEF_TABMOUNTER_INSPECTION_POS;
			//Inspection하지 않는 Tool이 있기 때문에 목표위치를 구하여야 한다(Inspect Carrier와 동일)
			dTargetPos = m_plnkTabMounter->CalculateMountPos(m_TabInfo.m_dModelOffset);
		}
		//@__________________

		iResult = m_plnkTabMounter->CheckCollisionOtherD(dTargetPos);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(425011));
			}
//			m_plnkTabMounter->SetGantryMotionEnable(FALSE, 0);
			m_plnkTabMounter->SetGantryMotionEnable(FALSE);
			break;
		}

		m_TimerError.StopTimer();
		// TabMounter : Inspection 위치로 이동.
		iResult = m_plnkTabMounter->MovePos(dTargetPos, iPosID, TRUE);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		m_bITITabMounterReadyPusherDownToPA = TRUE;//@130204.KKY

		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;

//		if (m_TabInfo.m_jobInfo.m_bPressComplete || m_TabInfo.m_jobInfo.m_bSkip)
		if (m_TabInfo.m_jobInfo.m_bPressComplete || m_TabInfo.m_jobInfo.m_bSkip || m_TabInfo.m_nTabNo == -1)
		{
			//@130206.KKY_________________
			m_bITIInspectionAbleToIC = TRUE;
			//@___________________________
			setStep(TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT);
		}
		else
			setStep(TABMOUNTER_INSPECTION_WAIT);
		break;

	case TABMOUNTER_INSPECTION_WAIT:
		simulLog("TABMOUNTER_INSPECTION_WAIT", m_estepCurrent, 100);

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

//		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			if (m_plnkITI_PanelAligner->IsInspectorEnabletoInspect() == FALSE)
				break;
		}
		
		//@KKY.130908추가______________
		if (MOlbSystemPre.GetTrsPanelAlignerComponent()->GetWorkScheduleNo() != m_iScheduleNo[m_iWorkGroup])
			break;
		//@____________________________

		// TabMounter Ready 위치(TabMounter UD Cylinder 1차 하강 위치)로 Down.
		//iResult = m_plnkTabMounter->DownReady();
		//20100820 sj_ysb
//		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			/*100822.임시삭제(IsInPos에서 TabNo인자로 주는것 추가)
			if (FALSE == m_plnkTabMounter->IsInPos(DEF_TABMOUNTER_INSPECTION_POS))
			{
				setStep(TABMOUNTER_MOVE_TO_INSPECTION_POS);
				break;
			}
			*/
			//@131127.KKY___________
			if (m_plnkSystemData->m_bUsePanelPusher == 2)
			{
				m_plnkTabMounter->DownPusherCyl(TRUE);
			}
			//@_____________________

			if (FALSE == m_plnkTabMounter->IsInPosZ(DEF_TABMOUNTER_Z_INSPECTION_POS))
			{
				//20131115 SJ_HJG
				m_TMountParametertimer.StartTimer();

				iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_INSPECTION_POS);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
				//20131115 SJ_HJG 
				m_dMountInspectionDown = m_TMountParametertimer.GetElapseTime();
				m_TMountParametertimer.StopTimer();
			}

			//@131127.KKY___________
			if (m_plnkSystemData->m_bUsePanelPusher == 2)
			{
				m_plnkTabMounter->DownPusherCyl();
			}
			//@_____________________

		}
	
		//20131115 SJ_HJG
		if(m_TMountParametertimer.IsTimerStarted() == FALSE)
			m_TMountParametertimer.StartTimer();

		// TabMounter 1차 Down 에 대한 안정화 시간 적용.
		if (m_plnkSystemData->m_dTabMounterStabilizationDelay > 0.0)
			Sleep(static_cast<unsigned long>(m_plnkSystemData->m_dTabMounterStabilizationDelay * 1000));

		// Inspection Camera 에게 Inspection Able 상태 알림.
		m_bITIInspectionAbleToIC = TRUE;

		// 현재 Tab 에 대한 Inspection 을 수행하는 Inspector Unit 번호를 얻고,
		// 그 Inspector Unit 이 Inspection 을 시작할 때까지 대기한다.
		//		m_iInspectorUnitNoInGroup = m_plnkTrsTabMounterSchedule->GetInspectionUnitNo(m_iWorkInstance);
		m_iInspectorUnitNoInGroup = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetInspectionNoFromCurrentWorkList(m_TabInfo.m_nTabNo);
		
		//		if (m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsInspectionStartToTM(m_iTabMounterNoInGroup) == FALSE)
		
		if (m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsInspectionStartToTM(m_iWorkInstance) == FALSE)
			break;
		
		
		// Inspection Camera 에게 Inspection Wait Start 상태 알림.
		m_bITIInspectionWaitStartToIC = TRUE;
//100906.Del		Sleep(100);//100830. 임시
		m_TMountParametertimer.StartTimer();
		while (TRUE)
		{
			// 현재 Tab 에 대한 Inspection 완료 확인
			// 만약 Inspection 중 Error (Vision Error etc.) 발생 시, TrsInspectionCamera 에서 해당 Tab Work Skip 을
			// 설정하고 나머지 작업 계속.
			//			if (m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsInspectionCompleteToTM(m_iTabMounterNoInGroup) == TRUE)
			if (m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsInspectionCompleteToTM(m_iWorkInstance) == TRUE)
				break;
			
			// Vision 인식 Error 발생으로 더 이상 진행 안될 때 처리. : 현재 Step 은 유지.
			//			if (FALSE == m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsInspectionStartToTM(m_iTabMounterNoInGroup))
			if (FALSE == m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsInspectionStartToTM(m_iWorkInstance))
			{
				m_bITIInspectionAbleToIC = FALSE;
				m_bITIInspectionWaitStartToIC = FALSE;
				//m_plnkTabMounter->Up();
				m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
				//@131127.KKY___________
				m_plnkTabMounter->UpPusherCyl();
				//@_____________________
				Sleep(100);
				return;
			}

			if (m_TMountParametertimer.MoreThan(20))
			{
				m_bITIInspectionAbleToIC = FALSE;
				m_bITIInspectionWaitStartToIC = FALSE;
				PROCESS_ALARM(generateErrorCode(425008));
				return;
			}
			Sleep(5);
		}
		
		
		//20131115 SJ_HJG 
		m_dMountInspectionTime = m_TMountParametertimer.GetElapseTime();
		m_TMountParametertimer.StopTimer();

		// Inspection Camera 에게 Inspection Wait 상태 해제 알림.
		m_bITIInspectionAbleToIC = FALSE;
		m_bITIInspectionWaitStartToIC = FALSE;

		//@131127.KKY___________
		m_plnkTabMounter->UpPusherCyl(TRUE);
		//@_____________________

		//20131115 SJ_HJG
		m_TMountParametertimer.StartTimer();

		//m_plnkTabMounter->Up();
		//100930.KKY_____
//		m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		{
			double dTarget = m_plnkTabMounter->GetTargetPosZ(DEF_TABMOUNTER_Z_INSPECTION_POS)
				+ DEF_TABMOUNTER_SAFE_HEIGHT;
			iResult = m_plnkTabMounter->MoveZPos(dTarget, DEF_TABMOUNTER_Z_MOVE_POS);
			if (iResult)//110120.Add
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//_______________

		//20131115 SJ_HJG 
		m_dMountInspectionUp = m_TMountParametertimer.GetElapseTime();
		m_TMountParametertimer.StopTimer();
		//_______________

		//@131127.KKY___________
		//170715_KDH 임시삭제
		/*	
		if (FALSE == m_plnkTabMounter->IsUpPusherCyl())
		{
			iResult = m_plnkTabMounter->UpPusherCyl();
			if (iResult)
			{
				SetErrorLevel(_T("TABMOUNTER_INSPECTION_WAIT"), 0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
*/
		//_________________
		//@_____________________

		/*
		//TrsTabInspection에서 인식에러나서 Tab을 다시 물고와야 하는지 확인...
		if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsInspectionSkip_CurrentWorkList(m_iWorkInstance))
		{
			setStep(TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT);
			sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = TRUE;
		}
		else
		{
			setStep(TABMOUNTER_MOVE_TO_MOUNT_POS);
		}
		*/
		setStep(TABMOUNTER_MOVE_TO_MOUNT_POS);
		break;

	case TABMOUNTER_MOVE_TO_MOUNT_POS:
		simulLog("TABMOUNTER_MOVE_TO_MOUNT_POS", m_estepCurrent, 100);

		//Inspection Error발생하여 Tab Reload하지 않으면 Mount위치로 이동...
		if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsInspectionSkip_CurrentWorkList(m_iWorkInstance))
		{
			//20131115 SJ_HJG 			
			m_TMountParametertimer.StartTimer();
//@			iResult = m_plnkCtrlTabMounter->MoveToMountPos(m_eWorkSide, m_TabInfo.m_nTabNo);//m_iMounterTabNo);
			iResult = m_plnkCtrlTabMounter->MoveToMountPos(m_iScheduleNo[m_iWorkGroup], m_eWorkSide, m_TabInfo.m_nRealTabNo);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//20131115 SJ_HJG 
			m_dMountMove = m_TMountParametertimer.GetElapseTime();
			m_TMountParametertimer.StopTimer();
		}
		//Inspection Error발생하여 Tab Reload하면...
		else
		{
			sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = TRUE;
		}

		sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = m_TabInfo.m_nTabNo;

		setStep(TABMOUNTER_ALL_INSPECTION_COMPLETE_WAIT);
		break;

	case TABMOUNTER_ALL_INSPECTION_COMPLETE_WAIT:
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
//			if (FALSE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
//				setStep(TABMOUNTER_MOVE_FOR_REMOVE_READY);
//			else
				setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		simulLog("TABMOUNTER_ALL_INSPECTION_COMPLETE_WAIT", m_estepCurrent, 100);

		// Test Mode : TabMounter 가 Inspection Offset 을 반영해서 Mount 위치로 이동한 후
		//             그 위치 확인을 위해서 자동운전을 진행하지 않고 대기한다.
//		if (TRUE == m_plnkSystemData->m_bUseTestStop) // 2009.01.23 CYJ UseTestStop 기능을 UsePanelPusher로 변경.
//			break;

		//Inspection 완료 되었는지 기다린다(skip 포함)
//@#ifdef DEF_SOURCE_SYSTEM
//@		if (TRUE == isGroupStarter())
//@#else
		if (m_iWorkInstance == m_iFirstWorkToolNo[m_iWorkGroup])
//@#endif
		{
			if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsInspectionCompleteGroup_CurrentWorkList())
			{
				Sleep(10);
				break;
			}
			
			if (FALSE == isExistRemoveTab())
			{
				if (TRUE == m_plnkSystemData->m_bUsePanelAbsorbOffsetReflection)
				{
					m_dPanelInspectPosFiduMarkPosY = 0.0;
					
					// Inspection 위치에 있는 Panel 의 기준 Fidu Mark 위치 측정.
					iResult = m_plnkCtrlTabMounter->GetPanelRefFiduMarkPosY(&m_dPanelInspectPosFiduMarkPosY);
					if (iResult)
					{
						// Error 발생 시, Fiducial Mark 인식 가능하도록 조치할 수 있다면 바로 Start 가능.
						//  => 조치 불가능(Panel 깨짐 등)하다면 Panel 제거 => 자동운전 초기화 하므로 처음부터 Step 진행.
						// 425007 = 공정 작업을 위한 Panel Fidu. Mark 인식 실패. [Panel Fidu. Mark 상태 확인 필요]
						PROCESS_ALARM(generateErrorCode(425007));
						break;
					}
				}
				sm_bITIAllInspectionCompleteToPA[m_iWorkGroup] = TRUE;
				if (m_plnkITI_PanelAligner->IsInspectorEnabletoInspect() == TRUE)
					break;
				sm_bITIAllInspectionCompleteToPA[m_iWorkGroup] = FALSE;
			}
//@#ifdef DEF_GATE_SYSTEM
			iResult = m_plnkTabMounter->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS);
			if (iResult)
			{
				// 425007 = 공정 작업을 위한 Panel Fidu. Mark 인식 실패. [Panel Fidu. Mark 상태 확인 필요]
				PROCESS_ALARM(iResult);
				break;
			}
//@#endif

			//@________________
//@			m_plnkTrsAutoManager->SetOPStatus(STEP_STOP);
			//@_________________

			m_bGroupInspectionComplete[m_iWorkGroup] = TRUE;

		}

		if (FALSE == m_bGroupInspectionComplete[m_iWorkGroup])
			break;

//		m_bAllLoadReady[m_iWorkGroup] = FALSE;

		if (TRUE == isExistRemoveTab())
		{
			setStep(TABMOUNTER_MOVE_TO_READY_POS_AFTER_WORKDONE);
			break;
		}


		setStep(TABMOUNTER_PREPARE_MOUNT_READY);
		break;

		//170907 JSh.s
	case TABMOUNTER_PREPARE_MOUNT_READY:
		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/
		// Panel Aligner 의 압착 가능 신호 확인.
		if (m_plnkITI_PanelAligner->IsWorkerEnabletoWork() == FALSE)
			break;

		//170905 JSH.s //InspectionCamera의 압착 가능 신호 확인
		// TabMounter::InspectionMountReadyWait 시작 확인.
		m_bITIInspectionMountReadyWaitToIC = TRUE;

 		if (m_rgplnkITI_InspectionCamera[m_iInspectorUnitNoInGroup]->IsMountReadyToTM(0) == FALSE)
			break;

		m_bITIInspectionMountReadyWaitToIC = FALSE;

		setStep(TABMOUNTER_PREPARE_MOUNT);
		break;

	case TABMOUNTER_PREPARE_MOUNT:
		//170905 JSH.e
//@#ifdef DEF_SOURCE_SYSTEM
//@		if (TRUE == isGroupStarter())
//@#else
		if (m_iWorkInstance == m_iFirstWorkToolNo[m_iWorkGroup])
//@#endif
		{
			iResult = prepareMountWork();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			m_bGroupMountStart[m_iWorkGroup] = TRUE;
		}

		if (FALSE == m_bGroupMountStart[m_iWorkGroup])
			break;

		simulLog("TABMOUNTER_PREPARE_MOUNT", m_estepCurrent, 100);
		
		setStep(TABMOUNTER_MOUNT);
		break;
		
	case TABMOUNTER_MOUNT:		
		simulLog("TABMOUNTER_MOUNT", m_estepCurrent, 100);
		sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;

		// TabMounter Mount 작업 수행.
		iResult = m_plnkCtrlTabMounter->MountTabIC(m_eWorkSide, m_iMounterTabNo);
		if (iResult)
		{
			//171024_KDH 해당 작업에서 문제가 발생할경우, Work Schedule이 꼬일 가능성이 있음,
			m_bAutoInitialized = FALSE;
			// Mount 작업 중 Error 는 TabIC 문제가 아니므로 Retry 하지 않는다.
			// (Retry - Tab Work Skip 을 설정 후, 새로 Tab 을 가져와서 재작업 시도)
			PROCESS_ALARM(iResult);
			break;
		}
		m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->SetPressComplete(m_TabInfo.m_nTabNo);
//		TRACE(_T("<<<<<Press Complete : WorkGroup=%d, WorkInstance=%d(%d)>>>>>\n"), m_iWorkGroup, m_iWorkInstance, GetTickCount());
		//_______________________________________
		m_bTabMounterVacOn = FALSE; //161221 SJ_YSH

		//20131115 SJ_HJG
		SProcessParameter sProcessParameter;
		sProcessParameter.m_dMountInspectionDown= m_dMountInspectionDown;
		sProcessParameter.m_dMountInspectionTime= m_dMountInspectionTime;
		sProcessParameter.m_dMountInspectionUp	= m_dMountInspectionUp;
		sProcessParameter.m_dMountMove			= m_dMountMove;
		sProcessParameter.m_dMountBondingDown	= m_dMountBondingDown;
		sProcessParameter.m_dMountBondingTime	= m_dMountBondingTime;
		sProcessParameter.m_dMountBondingUp		= m_dMountBondingUp;

		m_plnkTrsAutoManager->SetMountBondingParameter(m_eWorkSide, m_TabInfo.m_nTabNo, sProcessParameter);

#ifdef SIMULATION
		// Simulation Mode 에서 Flag 변화 Display 를 위한 Delay.
		Sleep(500);
#endif

		setStep(TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT);
		break;

	//지금작업하고 있는 Mounter들의 작업이 끝났는지 확인
	case TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT:
		simulLog("TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT", m_estepCurrent, 500);

		sm_bCycleWorkComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
//			if (FALSE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
//				setStep(TABMOUNTER_MOVE_FOR_REMOVE_READY);
//			else
				setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		if (TRUE == isExistRemoveTab())
		{
			setStep(TABMOUNTER_MOVE_TO_READY_POS_AFTER_WORKDONE);
			break;
		}

		if (TRUE == isGroupStarter())	
		{
			//작업하지 않는 Tab일경우 TABMOUNTER_LOAD step이 완료되지 않았을때 꼬이는 현상 때문에...
			if (FALSE == isAllCycleWorkComplete())
				break;
			for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
				sm_bCycleWorkComplete[m_iWorkGroup][i] = FALSE;

			m_bGroupMountComplete[m_iWorkGroup] = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsPressComplete_CurrentWorkList();
			if (m_bGroupMountComplete[m_iWorkGroup])
			{
				Sleep(100); //Test 무언정지 방지.
//				m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->AppendSkipTabAfterInspection();
				if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsPressComplete_AllWorkList())
				{
//@					m_plnkTrsInspectionCarrier->AddendWorkSchedule(m_iWorkGroup);
//@					m_plnkTrsPanelAligner->AppendWorkSchedule();
					m_bPressCompleteSchedule[m_iWorkGroup] = TRUE;
					//@__________________________
					m_iScheduleNo[m_iWorkGroup]++;
					
					//170719_KDH 작업할꺼 없으면 Skip Test중.
					for(i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
					{
						MListWorkTab ListWorkTab;
						m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
						if(ListWorkTab.GetTotalWorkTabCount() > 0)
							break;
					}
					m_iScheduleNo[m_iWorkGroup] = i;
					//@__________________________
					
					if (m_iScheduleNo[m_iWorkGroup] >= DEF_MAX_WORK_SCHEDULE)
					{
					//	m_iScheduleNo[m_iWorkGroup] = 0;
						//170719_KDH 작업할꺼 없으면 Skip Test중.
						m_iScheduleNo[m_iWorkGroup] = 0;	//@
						BOOL bLastWorkGroup = TRUE;
						int i = 0;
						for(i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
						{
							MListWorkTab ListWorkTab;
							m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
							if(ListWorkTab.GetTotalWorkTabCount() > 0)
								break;			
						}
						m_iScheduleNo[m_iWorkGroup] = i;
						//_______________________
					}
					//@__________________________

					sm_iPickoutGroup = -1;
					sm_iPressGroup = -1;
				}
				else
				{
					sm_iPressGroup = m_iWorkGroup;
				}
				sm_bITIAllMountCompleteToPA[m_iWorkGroup] = TRUE;
				m_iKeyValue[m_iWorkGroup] = -99;
//				TRACE(_T("<<<<<Group Press Complete : WorkGroup=%d(%d)>>>>>\n"), m_iWorkGroup, GetTickCount());
			}
		}

		if (FALSE == m_bGroupMountComplete[m_iWorkGroup])
			break;

		//@130206.KKY___________
		m_bITIInspectionAbleToIC = FALSE;
		//@_____________________

		m_bITITabMounterReadyPusherDownToPA = FALSE;//@130204.KKY

		sm_bGroupLoadComplete[m_iWorkGroup] = FALSE;

		setStep(TABMOUNTER_ALL_GROUP_MOUNT_COMPLETE_WAIT);
		break;
	
	//Panel의 Press작업이 끝났는지 확인
	case TABMOUNTER_ALL_GROUP_MOUNT_COMPLETE_WAIT:
		simulLog("TABMOUNTER_ALL_GROUP_MOUNT_COMPLETE_WAIT", m_estepCurrent, 100);

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		//@_________________________________
		if (TRUE == m_bPressCompleteSchedule[m_iWorkGroup])
		{
//@			if (FALSE == m_plnkITI_PanelAligner->IsReceivedPressCompleteSchedule(m_iWorkGroup)
//@				|| FALSE == m_rgplnkITI_InspectionCamera[0]->IsReceivedPressCompleteSchedule(m_iWorkGroup))
//@				break;
			m_bPressCompleteSchedule[m_iWorkGroup] = FALSE;
			m_plnkITI_PanelAligner->ResetReceivedPressCompleteSchedule(m_iWorkGroup);
			m_rgplnkITI_InspectionCamera[0]->ResetReceivedPressCompleteSchedule(m_iWorkGroup);
		}
		//@_________________________________

		if (TRUE == isGroupStarter())
		{			
			// Panel Aligner 동작 진행 상태 신호 확인
			if (m_plnkITI_PanelAligner->IsWorkerEnabletoWork() == TRUE)
				break;
			sm_bITIAllMountCompleteToPA[m_iWorkGroup] = FALSE;
			for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
			{
				m_bLoadReady[m_iWorkGroup][i] = FALSE;
			}
			m_bAllLoadReady[m_iWorkGroup] = FALSE;
			m_bGroupInspectionComplete[m_iWorkGroup] = FALSE;
			m_bGroupMountStart[m_iWorkGroup] = FALSE;
			m_bGroupMountComplete[m_iWorkGroup] = FALSE;
		}

		if (TRUE == m_bGroupMountComplete[m_iWorkGroup])
			break;

		if (FALSE == m_plnkTabMounter->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
		{
			iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
			if (iResult)//110120.Add
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}

		setStep(TABMOUNTER_MOVE_TO_READY_POS_AFTER_WORKDONE);
		break;

	case TABMOUNTER_MOVE_TO_READY_POS_AFTER_WORKDONE:
		simulLog("TABMOUNTER_MOVE_TO_READY_POS_AFTER_WORKDONE", m_estepCurrent, 100);
		
		if (FALSE == isGroupStarter())
		{
			if (FALSE == m_bNextPosCheckComplete[m_iWorkGroup])
				break;
		}

		iPosID = DEF_TABMOUNTER_READY_POS;
//		if (TRUE == isGroupStarter())
		if (m_iWorkInstance == DEF_MOUNTER_WORK_PER_GROUP-1)
//			m_plnkTabMounter->SetGantryMotionEnable(TRUE, DEF_MAX_TABCARRIER-1);
			m_plnkTabMounter->SetGantryMotionEnable(TRUE);

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
//			if (FALSE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
//				setStep(TABMOUNTER_MOVE_FOR_REMOVE_READY);
//			else
				setStep(TABMOUNTER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		// 091028 공정 Key Parameter 전송 요구
		/*
		if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE
			|| m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
		{
			m_plnkTrsAutoManager->SetMeasureProcessData(TRUE);
			//TRACE(_T("Parameter Measure Start"));
		}
		*/

		if (TRUE == isGroupStarter())
		{
			if (TRUE == m_plnkITI_TabCarrier[DEF_MAX_TABCARRIER-1]->IsLoadingCompleteToTM())
			{
				if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_SupplyWorkList(m_iWorkGroup))
				{
					iResult = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->PickOut_SupplyWorkList(m_iWorkGroup);
					if (iResult == 0)
					{
						m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
						if (m_TabInfo.m_nTabNo == -1)
						{
							iPosID = DEF_TABMOUNTER_ESCAPE_POS;
						}
					}
				}
				else
				{
					m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
					if (m_TabInfo.m_nTabNo == -1)
					{
						iPosID = DEF_TABMOUNTER_ESCAPE_POS;
					}
				}
			}
			m_bNextPosCheckComplete[m_iWorkGroup] = TRUE;
		}
		else if (m_iWorkInstance == 1)
		{
			if (TRUE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_SupplyWorkList(m_iWorkGroup))
			{
				MTabInfo prevInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, 0);
				m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_SupplyWorkList(m_iWorkGroup, m_iWorkInstance);
				//@1번 Mounter가 작업하지 않고, 2번 Mounter가 작업하면 Loading위치로 바로 이동한다.
				if (prevInfo.m_nTabNo == -1 && m_TabInfo.m_nTabNo != -1)
				{
					m_iWorkCarrierNo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetCarrierNo_FromCurrentSupplyList(m_iWorkGroup, m_TabInfo.m_nTabNo);
					iPosID = getLoadPosID();
				}
			}
		}

		if (FALSE == m_plnkTabMounter->IsInPos(m_iScheduleNo[m_iWorkGroup], iPosID))
		{	
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			/*/
			iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], iPosID, -1);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					SetErrorLevel(_T("TAB MOUNTER move to Load Position"), 0, __FILE__, __LINE__);
					PROCESS_ALARM(generateErrorCode(425010));
				}
				if (TRUE == isGroupStarter())
//					m_plnkTabMounter->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
					m_plnkTabMounter->SetGantryMotionEnable(FALSE);
				break;
			}
			/*/
			while(1)
			{
				Sleep(10);
				iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], iPosID, -1);
				if (iResult)
				{
					if (m_TimerError.MoreThan(30.0))
					{
						m_TimerError.StopTimer();
						PROCESS_ALARM(generateErrorCode(425010));
						return;
					}
				}
				else
				{
					if (isGroupStarter())
	//					m_plnkTabMounter->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
						m_plnkTabMounter->SetGantryMotionEnable(FALSE);
					break;
				}
			}
			m_TimerError.StopTimer();
			//161221 SJ_YSH
			if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE)
			{
				if(m_bTabMounterVacOn == FALSE)
				{	
					iResult =m_plnkTabMounter->AbsorbTabIC(TRUE);
					if (iResult)
					{
						PROCESS_ALARM(iResult);
						break;
					}
				}
			}
			//___________
			iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//161221 SJ_YSH
			if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE)
			{
				if(m_bTabMounterVacOn == FALSE)
				{	
					if(m_plnkTabMounter->IsAbsorbTabIC())
					{
						if(m_iWorkGroup == DEF_FRONT_GROUP)
							iResult = generateErrorCode(425032);
						else
							iResult = generateErrorCode(425033);
					}
					if(m_plnkTabMounter->CheckVacuum())
					{
						m_plnkTabMounter->ReleaseTabIC();
						iResult = 0;
					}
					
					if(iResult != 0)
					{
						PROCESS_ALARM(iResult);
						break;
					}
				}
			}
			//___________
		}

		if (isExistRemoveTab())
		{
			TRACE(_T("<<<<<Complete MoveLoadPos For Remove : WorkGroup=%d, WorkInstance=%d, TabNo=%d>>>>>\n"),
				m_iWorkGroup, m_iWorkInstance, m_TabInfo.m_nTabNo);
			setStep(TABMOUNTER_MOVE_TO_READY_POS_FOR_REMOVE);
			break;
		}

		setStep(TABMOUNTER_MOVE_TO_LOAD_POS);
		break;

	case TABMOUNTER_MOVE_TO_READY_POS_FOR_REMOVE:
		simulLog("TABMOUNTER_MOVE_TO_READY_POS_FOR_REMOVE", m_estepCurrent, 100);
		setStep(TABMOUNTER_REMOVE_WAIT);
		break;
		
	case TABMOUNTER_REMOVE_WAIT:
		simulLog("TABMOUNTER_REMOVE_WAIT", m_estepCurrent, 100);

		if (FALSE == pITI_TabCarrier->IsUnloadingMoveCompleteToTM())
			break;			

		if (FALSE == sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance])
		{
			m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = TRUE;
		}
		else
		{
//			if (FALSE == pITI_TabCarrier->IsUnloadingMoveCompleteToTM())
//				break;			
			// TabMounter Tab 제거.
			iResult = m_plnkCtrlTabMounter->Remove();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//111118.kms Compare 중 추가
			TRACE(_T("자동중에 버리는 동작!?"));
			//SJ_YYK 110926 /Add.... 근데 왜 버리냐...
			iResult = MOlbSystemPre.GetTabCarrierComponent(0)->BackwardDumpBasket();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//___________*/

			m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = TRUE;
		}

		if (isGroupStarter() && TRUE == isAllLoadReady())
		{
			m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->ResetInspectionComplete_CurWorkList();
			m_bAllLoadReady[m_iWorkGroup] = TRUE;
		}
		
		if (FALSE == m_bAllLoadReady[m_iWorkGroup])
			break;
		
		//새로운 TabData를 가지고 온다
//		sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = -1;
		m_TabInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabInfo_CurrentWorkList(m_iWorkInstance);

		TRACE(_T("<<<<<GetTab After Remove : WorkGroup=%d, WorkInstance=%d, TabNo=%d>>>>>\n"),
			m_iWorkGroup, m_iWorkInstance, m_TabInfo.m_nTabNo);

		setStep(TABMOUNTER_LOAD);
		break;

		//NSMC KJS
	case TABMOUNTER_MODEL_CHANGE: simulLog("TABMOUNTER_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;
		
		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;
		
		m_bModelChangeReady = FALSE;
		
		setStep(TABMOUNTER_MOVE_TO_LOAD_POS);
		break;

	case TABMOUNTER_MOVE_FOR_REMOVE_READY:
		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();		
//@		dTargetPos = m_plnkTabMounter->GetTargetPos(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_REMOVE_POS) + (DEF_TABMOUNTER_STANDARD_GAP+2.0)*m_iWorkInstance;
		dTargetPos = m_plnkTabMounter->GetTargetPos(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_REMOVE_POS) + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL+2.0)*m_iWorkInstance;

		iResult = m_plnkTabMounter->CheckCollisionOtherD(dTargetPos);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(425030));
			}
			break;
		}
		m_TimerError.StopTimer();
		iResult = m_plnkTabMounter->MovePos(dTargetPos, DEF_TABMOUNTER_REMOVE_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		setStep(TABMOUNTER_REMOVE_TAB);
		break;

	case TABMOUNTER_REMOVE_TAB:
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
			if (FALSE == MOlbSystemPre.GetTabCarrierComponent(DEF_MAX_TABCARRIER-1)->IsInXYTPos(DEF_TABCARRIER_READY_POS))
				break;
		}
		else
		{
			if (FALSE == MOlbSystemPre.GetRTabCarrierComponent(DEF_MAX_TABCARRIER-1)->IsInXYTPos(DEF_TABCARRIER_READY_POS))
				break;
		}

		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();
		iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_REMOVE_POS, -1);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(425031));
			}
			break;
		}
		m_TimerError.StopTimer();
		iResult = m_plnkCtrlTabMounter->Remove();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_READY_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		setStep(TABMOUNTER_WAIT_REMOVE_COMPLETE);
		break;

	case TABMOUNTER_READY_MOVE_FOR_FEEDERREADY:
		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();
		iResult = m_plnkTabMounter->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_LOAD_POS, -1);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(425010));
			}
			break;
		}
		m_TimerError.StopTimer();
		iResult = m_plnkTabMounter->SafeMovePos(m_iScheduleNo[m_iWorkGroup], DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		if (FALSE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
		{
			iResult = m_plnkTabMounter->ReleaseTabIC();
			Sleep(100);
		}
		m_b1FeederAllMountInitReady = FALSE;
		setStep(TABMOUNTER_WAIT_1FEEDERALLMOUNT_INIT);
		break;

	case TABMOUNTER_WAIT_1FEEDERALLMOUNT_INIT:
		m_TimerError.StopTimer();

		//120116.KMS________
		if(m_iWorkGroup == DEF_FRONT_GROUP)
		{
			m_bMountAutoChangeReady[0] = TRUE;
			if(TRUE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
				break;

			if(m_plnkSystemData->m_bUse1FeederAllMount == TRUE)
			{
				m_b1FeederAllMountInitReady = TRUE;
				if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
				{
					break;
				}
			}
			m_bMountAutoChangeReady[0] = FALSE;
		}
		else
		{
			m_bMountAutoChangeReady[1] = TRUE;
			if(TRUE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
				break;

			if(m_plnkSystemData->m_bUse1FeederAllMount == TRUE)
			{
				m_b1FeederAllMountInitReady = TRUE;
				if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
				{
					break;
				}
			}
			m_bMountAutoChangeReady[1] = FALSE;

		}
		//__________________

		initializeITI();	//TrsPanelAligner과의 IF중일수 있으므로, 위치를 여기로 이동
		m_iMounterTabNo = DEF_NO_WORK;
		m_uiRetryCount = 0;
		m_bLoadReady[m_iWorkGroup][m_iInstanceNo] = FALSE;
		m_bGroupInspectionComplete[m_iWorkGroup] = FALSE;
		m_bAllLoadReady[m_iWorkGroup] = FALSE;
		m_iKeyValue[m_iWorkGroup] = -99;
		sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bGroupMountStart[m_iWorkGroup] = FALSE;
		m_bGroupMountComplete[m_iWorkGroup] = FALSE;
		sm_iPickoutGroup = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetLastPickoutGroup();
		sm_iPressGroup = sm_iPickoutGroup;
		sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = -1;
		sm_bCycleWorkComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bGroupLoadComplete[m_iWorkGroup] = FALSE;
		m_b1FeederAllMountInitReady = FALSE;
		//120116.KMS__________
		m_bMountAutoChangeReady[0] = FALSE;
		m_bMountAutoChangeReady[1] = FALSE;
		//____________________
		Sleep(1000);
		setStep(TABMOUNTER_MOVE_TO_LOAD_POS);
		break;

	default:
		break;
	}
}

void MTrsTabMounter::initializeITI()
{
	m_bITITabICReceiveStartToTC = FALSE;
	m_bITITabICReceiveCompleteToTC = FALSE;
	m_bITIInspectionAbleToIC = FALSE;
	m_bITIInspectionWaitStartToIC = FALSE;
	sm_bITIAllInspectionCompleteToPA[m_iWorkGroup] = FALSE;
	sm_bITIAllMountCompleteToPA[m_iWorkGroup] = FALSE;

	//TrsCarrier Remove_______________
	m_bITITabICRemoveRequestToTC = FALSE;
	m_bITITabICRemoveReadyToTC = FALSE;
	m_bITITabICRemoveCompleteToTC = FALSE;

	m_bITITabICReloadRequestToTC = FALSE;

	m_bITITabMounterReadyPusherDownToPA = FALSE;	//@130204.KKY
	m_bITIInspectionMountReadyWaitToIC = FALSE; //170918 JSH
}

void MTrsTabMounter::initializeStep()
{
	m_estepPrevious	= (enumStepTabMounter)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepTabMounter)0;	//  현재 Step Clear
}

void MTrsTabMounter::setStep(EStepTabMounter estepPanelAligner)
{
	m_estepPrevious	= m_estepCurrent;		// 이전 Step 저장
	m_estepCurrent	= estepPanelAligner;	// 새로 Step으로 전환
}

int MTrsTabMounter::checkGantryState(int iGantryPosID)
{
	int iResult = ERR_TRS_TABMOUNTER_SUCCESS;
	MTickTimer timerLimit;
	timerLimit.StartTimer();

	while (TRUE)
	{
		iResult = m_plnkTabMounter->CheckGantryState(iGantryPosID);
		if (ERR_TABMOUNTER_SUCCESS == iResult)
			return ERR_TRS_TABMOUNTER_SUCCESS;

		if (RUN != m_plnkTrsAutoManager->GetOPStatus())
			return ERR_TRS_TABMOUNTER_SUCCESS;

		// Time Out : 2분 설정.
//syr..SESL 수정
//		if (timerLimit.MoreThan(120.0))
		if (timerLimit.MoreThan(30.0))	// hongju 수정..2분은 너무 길다.
		{
			CString strLog;
			strLog.Format(_T("TM%d check gantry state(g-pos%d) : %s "), m_iInstanceNo + 1, iGantryPosID, m_plnkProcessData->GetDataLog(m_eWorkSide));
			m_plnkTrsAutoManager->WriteMonitoringLog(strLog);
			return iResult;
		}

		Sleep(100);
	}

	return ERR_TRS_TABMOUNTER_SUCCESS;
}

int MTrsTabMounter::prepareMountWork()
{
#if FALSE	//=============================================
	// 현재 작업 Cycle 에서 압착할 Tab 이 없는 경우, Backup 동작 Skip
	// 현재 작업 Cycle 에서 압착할 Tab 이 없는 경우는 모든 Tab 에 WorkSkip 설정된 경우 뿐이다.
	if (TRUE == m_plnkProcessData->IsCurrentWorkCycleAllWorkSkip(m_eWorkSide))
	if (m_TabInfo.m_nTabNo == -1 
		|| TRUE == m_TabInfo.m_jobInfo.m_bSkip 
		|| TRUE == m_TabInfo.m_jobInfo.m_bPressComplete)
		return ERR_TRS_TABMOUNTER_SUCCESS;

	int iResult = 0;
	
	iResult = m_plnkTabMounter->ForwardBackupFBCyl();
	if (iResult)
	{
		SetErrorLevel(_T("TAB MOUNTER prepare Mount Work"), 0, __FILE__, __LINE__);
		return iResult;
	}
	
	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
		{
			m_plnkCtrlTabMounter->LogVisionMarkPos(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK, "press(before backup up)");
			m_plnkCtrlTabMounter->LogVisionMarkPos(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK, "press(before backup up)");
		}
	}
		
#endif		//#if FALSE =============================================

	int iResult = ERR_TRS_TABMOUNTER_SUCCESS;

	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
		{
			m_plnkCtrlTabMounter->LogVisionMarkPos(DEF_PANEL_ALIGN_CAMERA_1, DEF_FIDU_MARK, "press(after backup up)");
			m_plnkCtrlTabMounter->LogVisionMarkPos(DEF_PANEL_ALIGN_CAMERA_2, DEF_FIDU_MARK, "press(after backup up)");
		}
	}
	
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType
		&& TRUE == m_plnkSystemData->m_bUsePanelAbsorbOffsetReflection)
	{
		m_dPanelMountPosFiduMarkPosY = 0.0;
		
		// Mark Pos 얻는 작업에서 Error 치면 추가적인 Offset 적용 작업 Skip!
		iResult = m_plnkCtrlTabMounter->GetPanelRefFiduMarkPosY(&m_dPanelMountPosFiduMarkPosY);
		if (iResult)
		{
			// 425001 = 공정 작업 중 Panel 위치 변화량 측정 실패. [Panel Fidu. Mark 상태 확인 필요]
			return generateErrorCode(425001);
		}
	}
	
	return ERR_TRS_TABMOUNTER_SUCCESS;
}

int MTrsTabMounter::checkBackupState()
{
	return ERR_TRS_TABMOUNTER_SUCCESS;
}

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

void MTrsTabMounter::AssignComponents(STrsTabMounterRefCompList listRefComponents)
{
	m_plnkTrsAutoManager = listRefComponents.m_plnkTrsAutoManager;
	m_plnkCtrlTabMounter = listRefComponents.m_plnkCtrlTabMounter;
	m_plnkTabMounter = listRefComponents.m_plnkTabMounter;
	m_plnkMelsecIF = listRefComponents.m_plnkMelsecIF;
	m_plnkCameraCarrier = listRefComponents.m_plnkCameraCarrier;
}

void MTrsTabMounter::SetData(STrsTabMounterData datComponent)
{
	m_plnkSystemData	= datComponent.m_plnkSystemData;
	m_plnkProcessData	= datComponent.m_plnkProcessData;
//@	m_plnkWorkSchedule	= datComponent.m_plnkWorkSchedule;
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]	= datComponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND]	= datComponent.m_plnkWorkScheduleSecond;
#endif
}

void MTrsTabMounter::AssignProcess(STrsTabMounterRefProcess listRefTrs)
{
	m_plnkCtrlTabFeeder[DEF_TABFEEDER1] = listRefTrs.m_plnkCtrlTabFeeder[DEF_TABFEEDER1];
	m_plnkCtrlTabFeeder[DEF_TABFEEDER2] = listRefTrs.m_plnkCtrlTabFeeder[DEF_TABFEEDER2];
	m_plnkTrsPanelAligner		= listRefTrs.m_plnkTrsPanelAligner;
	m_plnkTrsInspectionCarrier	= listRefTrs.m_plnkTrsInspectionCarrier;
}

int MTrsTabMounter::Initialize()
{
	// 전체 초기화( or Current Panel Pass) 인 경우만 존재한다.
	m_bAutoInitialized = FALSE;
	int iResult = ERR_TRS_TABMOUNTER_SUCCESS;

	if(m_iWorkGroup == DEF_FRONT_GROUP)
	{
		iResult = m_plnkTabMounter->CheckVacuum();
		if (iResult)
			m_plnkTabMounter->ReleaseTabIC();
		//161221 SJ_YSH
		if(m_plnkTabMounter->IsAbsorbTabIC())
			m_bTabMounterVacOn = TRUE;
		else
		{
			iResult = m_plnkTabMounter->AbsorbTabIC();
			if (iResult == 0)
			{
				if(m_iWorkGroup == DEF_FRONT_GROUP)
					return generateErrorCode(425034);
				else
					return generateErrorCode(425035);
			}	
		}
		//_____________
		
		iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		if (iResult)
			return iResult;
		iResult = m_plnkTabMounter->DownCyl();
		if (iResult)
			return iResult;
		iResult = m_plnkTabMounter->UpPusherCyl();
		if (iResult)
			return iResult;
	}
	else
	{
		m_iMounterTabNo = DEF_NO_WORK;
		
		m_dPanelInspectPosFiduMarkPosY = 0.0;
		m_dPanelMountPosFiduMarkPosY = 0.0;
		
		initializeStep();
		initializeITI();
		
		m_uiRetryCount = 0;
		
		//===============================================================
		m_bLoadReady[m_iWorkGroup][m_iInstanceNo] = FALSE;
		m_bGroupInspectionComplete[m_iWorkGroup] = FALSE;
		m_bAllLoadReady[m_iWorkGroup] = FALSE;
		m_iKeyValue[m_iWorkGroup] = -99;
		sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bGroupMountStart[m_iWorkGroup] = FALSE;
		m_bGroupMountComplete[m_iWorkGroup] = FALSE;
		//	sm_iPickoutGroup = -1;
		sm_iPickoutGroup = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetLastPickoutGroup();
		sm_iPressGroup = sm_iPickoutGroup;
		sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = -1;
		sm_bCycleWorkComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bGroupLoadComplete[m_iWorkGroup] = FALSE;
		m_bPressCompleteSchedule[m_iWorkGroup] = FALSE;
		m_bNextPosCheckComplete[m_iWorkGroup] = FALSE;
		
		m_TimerError.StopTimer();
		m_b1FeederAllMountInitReady = FALSE;
		//===============================================================
		
		//120116.KMS________
		m_bMountAutoChangeReady[0] = FALSE;
		m_bMountAutoChangeReady[1] = FALSE;
		m_plnkTrsAutoManager->SetMountAutoChange(FALSE);
		
		m_bAutoInitialized = TRUE;
		return ERR_TRS_TABMOUNTER_SUCCESS; 
	}

	/*/@130204.이동동작은 AutoMainForm::initTabMounter() 에서 한다
//@#ifdef DEF_GATE_SYSTEM
	if (TRUE == isGroupStarter())
//		m_plnkTabMounter->SetGantryMotionEnable(TRUE, DEF_MAX_TABCARRIER-1);
		m_plnkTabMounter->SetGantryMotionEnable(TRUE);
//@#endif

//@#ifdef DEF_SOURCE_SYSTEM
//	iResult = m_plnkTabMounter->SafeMovePos(DEF_TABMOUNTER_LOAD_POS, FALSE);
//@	iResult = m_plnkTabMounter->SafeMovePos(DEF_TABMOUNTER_READY_POS, FALSE);
//@#else
	iResult = m_plnkTabMounter->SafeMovePos(0, DEF_TABMOUNTER_READY_POS, FALSE);
//@#endif
	if (iResult)
	{
		SetErrorLevel(_T("TAB MOUNTER initialize"), 0, __FILE__, __LINE__);
		return iResult;
	}

	Sleep(static_cast<unsigned long>(m_plnkSystemData->m_dStartMoveDelay) * 1000);
	/*/

	m_iMounterTabNo = DEF_NO_WORK;

	m_dPanelInspectPosFiduMarkPosY = 0.0;
	m_dPanelMountPosFiduMarkPosY = 0.0;

	initializeStep();
	initializeITI();

	m_uiRetryCount = 0;

	//170719_KDH 작업할꺼 없으면 Skip Test중.
	m_iScheduleNo[m_iWorkGroup] = 0;	//@
	BOOL bLastWorkGroup = TRUE;
	int i = 0;
	for(i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
	{
		MListWorkTab ListWorkTab;
		m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
		if(ListWorkTab.GetTotalWorkTabCount() > 0)
		{
			
			//170920 JSH.s
			if(i != DEF_MAX_WORK_SCHEDULE -1)
			{
				if(m_plnkWorkSchedule[i]->IsPressComplete_AllWorkList())
					continue;
			}
			//170920 JSH.e
			
			break;			
		}
	}
	//170920 JSH.s
	if(i == DEF_MAX_WORK_SCHEDULE)
		i = 0;
	//170920 JSH.e
	m_iScheduleNo[m_iWorkGroup] = i;
	//_______________________


	//===============================================================
	m_bLoadReady[m_iWorkGroup][m_iInstanceNo] = FALSE;
	m_bGroupInspectionComplete[m_iWorkGroup] = FALSE;
	m_bAllLoadReady[m_iWorkGroup] = FALSE;
	m_iKeyValue[m_iWorkGroup] = -99;
	sm_bRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
	m_bGroupMountStart[m_iWorkGroup] = FALSE;
	m_bGroupMountComplete[m_iWorkGroup] = FALSE;
//	sm_iPickoutGroup = -1;
	sm_iPickoutGroup = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetLastPickoutGroup();
	sm_iPressGroup = sm_iPickoutGroup;
	sm_iWorkTabNo[m_iWorkGroup][m_iWorkInstance] = -1;
	sm_bCycleWorkComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bGroupLoadComplete[m_iWorkGroup] = FALSE;
	m_bPressCompleteSchedule[m_iWorkGroup] = FALSE;
	m_bNextPosCheckComplete[m_iWorkGroup] = FALSE;

	m_TimerError.StopTimer();
	m_b1FeederAllMountInitReady = FALSE;
	//===============================================================

	//120116.KMS________
	m_bMountAutoChangeReady[0] = FALSE;
	m_bMountAutoChangeReady[1] = FALSE;
	m_plnkTrsAutoManager->SetMountAutoChange(FALSE);

	m_bAutoInitialized = TRUE;
	return ERR_TRS_TABMOUNTER_SUCCESS; 
}

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/

void MTrsTabMounter::AssignITI(STabMounterRefITIList sITIList)
{
	int i = 0;
	m_plnkITI_PanelAligner			= sITIList.m_plnkITI_PanelAligner;
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		m_plnkITI_TabCarrier[i] = sITIList.m_plnkITI_TabCarrier[i];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		m_rgplnkITI_InspectionCamera[i] = (sITIList.m_rgplnkITI_InspectionCamera)[i];
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/

void MTrsTabMounter::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}

	m_bThreadLife = TRUE;
	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsTabMounter::ThreadStop()
{
	m_bThreadLife = FALSE;

	if (m_pThread != NULL)
	{
		DWORD exitcode = ::WaitForSingleObject(m_pThread->m_hThread, 10000);

		if (exitcode == WAIT_TIMEOUT)
			::TerminateThread(m_pThread->m_hThread, exitcode);

		m_pThread = NULL;
	}
}

// NSMC
BOOL MTrsTabMounter::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsTabMounter::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

//120116.kms__________
BOOL MTrsTabMounter::IsMountAutoChangeReady1()
{
	return m_bMountAutoChangeReady[0];
}

BOOL MTrsTabMounter::IsMountAutoChangeReady2()
{
	return m_bMountAutoChangeReady[1];
}

BOOL MTrsTabMounter::isGroupStarter()
{
//@#ifdef DEF_SOURCE_SYSTEM
//@	if (m_iWorkInstance == DEF_MOUNTER_WORK_PER_GROUP-1)
//@#else
	if (m_iWorkInstance == 0)
//@#endif
		return TRUE;
	return FALSE;
}

BOOL MTrsTabMounter::isAllLoadReady()
{
	BOOL bReady = TRUE;
	for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		bReady &= m_bLoadReady[m_iWorkGroup][i];
	}
	return bReady;
}

BOOL MTrsTabMounter::isAllCycleWorkComplete()
{
	BOOL bComplete = TRUE;
	for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		bComplete &= sm_bCycleWorkComplete[m_iWorkGroup][i];
	}
	return bComplete;
}

BOOL MTrsTabMounter::isExistRemoveTab()
{
	for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (TRUE == sm_bRemoveTab[m_iWorkGroup][i])
			return TRUE;
	}
	return FALSE;
}

BOOL MTrsTabMounter::isLoadCompleteOtherGroup()
{
	BOOL isComplete = TRUE;

	/*/
#ifdef DEF_SOURCE_SYSTEM	//Source에서 양쪽 Feeder 사용시만 확인.
	if (m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_ALL)
	{
#	ifndef DEF_MIRROR_
		//Front Group이 먼저 작업시작하고, 서로 교대로 작업하도록 하기위해(순서가 정해지지 않으면 정도 맞추기 힘들다고 함...)
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
			//초기나 RearGroup이 Pickout하지 않았으면 대기...
			if (sm_iPickoutGroup != -1 && sm_iPickoutGroup != DEF_REAR_GROUP)
				isComplete = FALSE;
		}
		else
		{
			//Front Group이 Pickout하지 않았으면 대기...
			if (sm_iPickoutGroup != DEF_FRONT_GROUP)
				isComplete = FALSE;
		}
#	else
		//Rear Group이 먼저 작업시작하고, 서로 교대로 작업하도록 하기위해(순서가 정해지지 않으면 정도 맞추기 힘들다고 함...)
		if (m_iWorkGroup == DEF_REAR_GROUP)
		{
			//초기나 FrontGroup이 Pickout하지 않았으면 대기...
			if (sm_iPickoutGroup != -1 && sm_iPickoutGroup != DEF_FRONT_GROUP)
				isComplete = FALSE;
		}
		else
		{
			//Rear Group이 Pickout하지 않았으면 대기...
			if (sm_iPickoutGroup != DEF_REAR_GROUP)
				isComplete = FALSE;
		}
#	endif
	}
#endif
	/*/

	return isComplete;
}

BOOL MTrsTabMounter::isPressCompleteOtherGroup()
{
	BOOL isComplete = TRUE;

#	ifndef DEF_MIRROR_
	//Front Group이 먼저 작업시작하고, 서로 교대로 작업하도록 하기위해
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		//초기나 RearGroup이 Pickout하지 않았으면 대기...
		if (sm_iPressGroup != -1 && sm_iPressGroup != DEF_REAR_GROUP)
			isComplete = FALSE;
	}
	else
	{
		//Front Group이 Pickout하지 않았으면 대기...
		if (sm_iPressGroup != DEF_FRONT_GROUP)
			isComplete = FALSE;
	}
#	else
	//Rear Group이 먼저 작업시작하고, 서로 교대로 작업하도록 하기위해
	if (m_iWorkGroup == DEF_REAR_GROUP)
	{
		//초기나 FrontGroup이 Pickout하지 않았으면 대기...
		if (sm_iPressGroup != -1 && sm_iPressGroup != DEF_FRONT_GROUP)
			isComplete = FALSE;
	}
	else
	{
		//Rear Group이 Pickout하지 않았으면 대기...
		if (sm_iPressGroup != DEF_REAR_GROUP)
			isComplete = FALSE;
	}
#	endif

	return isComplete;
}

BOOL MTrsTabMounter::isExistNextLoadTab()
{
	BOOL bExist = FALSE;
	//@130104.KKY____________
	for (int i=m_iWorkInstance+1; i<DEF_MAX_TABCARRIER; i++)
//@	for (int i=m_iWorkInstance-1; i>=0; i--)
	{
		if (sm_iWorkTabNo[m_iWorkGroup][i] != -1)
		{
			bExist = TRUE;
			break;
		}
	}
	//@______________________
	return bExist;
}

BOOL MTrsTabMounter::is1FeederAllMounterInitStart()
{
	BOOL bInitStart = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount && FALSE == sm_bGroupLoadComplete[m_iWorkGroup])
	{
		if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
		{
			bInitStart = TRUE;
		}
	}
#endif
	/*/
	return bInitStart;
}

BOOL MTrsTabMounter::isTabCarrierOtherGroup()
{
	BOOL bOtherGroup = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if (m_plnkTrsAutoManager->GetFeederUseMode() != DEF_TABFEEDER_ALL && m_plnkTrsAutoManager->GetFeederUseMode() != m_iWorkGroup)
			bOtherGroup = TRUE;
	}
#endif
	/*/
	return bOtherGroup;
}

int	MTrsTabMounter::getLoadPosID()
{
	int iPosID = DEF_TABMOUNTER_LOAD_POS;
	if (TRUE == isTabCarrierOtherGroup())
		iPosID = DEF_TABMOUNTER_R_LOAD_POS;
	return iPosID;
}

BOOL MTrsTabMounter::IsLoadComplete(int iWorkGroup)
{	
	return sm_bGroupLoadComplete[iWorkGroup];
}
