/* 
 * TrsTabCarrier Component
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
 * MTrsTabCarrier.cpp : Implementation of MTrsTabCarrier component.
 */

#include "Stdafx.h"
#include "MSystemData.h"
#include "MProcessData.h"
#include "MPreBonderData.h"//2009.04.04 SESL_CYJ
#include "MCtrlTabCarrier.h"
#include "MTrsTabCarrier.h"
#include "MTrsAutoManager.h"
#include "ITI_TabFeeder.h"
#include "ITI_TabMounter.h"
#include "MTabCarrier.h"
#include "IStatePanelAligner.h"
#include "MCtrlTabFeeder.h"
#include "DBTabLoss.h"
#include "MPlatformOlbSystem.h"

//#include "IStateRollerConveyor.h"
#include "MNetH.h"
#include "Math.h"
#include "common.h"
#include "IAxis.h" //170203 SJ_YSH

extern MPlatformOlbSystem	MOlbSystemPre;

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
//static Members_______________________________________________________________
STrsTabCarrierReloadInfo MTrsTabCarrier::m_sReloadInfo[DEF_MAX_GROUP];
CCriticalSection MTrsTabCarrier::m_csLock[DEF_MAX_GROUP];
BOOL MTrsTabCarrier::m_bLoadReady[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
BOOL MTrsTabCarrier::m_bAllLoadReady[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabCarrier::m_bUnloadComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};

BOOL MTrsTabCarrier::m_bAllUnloadComplete[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsTabCarrier::m_bGroupSupplyComplete[DEF_MAX_GROUP] = {FALSE, FALSE};

BOOL MTrsTabCarrier::m_bLoadMoveComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
BOOL MTrsTabCarrier::m_bAllLoadMoveComplete[DEF_MAX_GROUP] = {FALSE, FALSE};

BOOL MTrsTabCarrier::m_iWorkGroupMounter[DEF_MAX_GROUP] = { DEF_FRONT_GROUP, DEF_REAR_GROUP };
int	MTrsTabCarrier::m_iScheduleNo[DEF_MAX_GROUP] = {0, 0};

//SJ_YYK 150109 Add...
BOOL MTrsTabCarrier::m_bUnloadingReady[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};

BOOL MTrsTabCarrier::m_bAllUnloadingReady[DEF_MAX_GROUP] = {FALSE, FALSE};

BOOL MTrsTabCarrier::m_bPrealignComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
//_______________________________________________*/

//SJ_YYK 150908 Add...
BOOL MTrsTabCarrier::m_bLoadingComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] =
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
//_______________________*/

//170830 JSh.s
BOOL MTrsTabCarrier::m_bTabICUnloadToTabMounter[DEF_MAX_GROUP][DEF_MAX_TABCARRIER] = { {FALSE,}, };

MTrsTabCarrier::MTrsTabCarrier(SCommonAttribute commonData,
							   STrsTabCarrierRefCompList listRefComponents, STrsTabCarrierData datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	if (m_iInstanceNo < DEF_MAX_TABCARRIER)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkGroupMounter[m_iWorkGroup] = DEF_FRONT_GROUP;
		m_iOtherWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo;

		//SJ_YYK 150817 Add...
		if(m_iWorkInstance %2 == 0)
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_1;
		else
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_2;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkGroupMounter[m_iWorkGroup] = DEF_REAR_GROUP;
		m_iOtherWorkGroup = DEF_FRONT_GROUP;
//		m_iWorkInstance = DEF_MAX_TABCARRIER*2 - 1 - m_iInstanceNo;
		m_iWorkInstance = m_iInstanceNo-DEF_MAX_TABCARRIER;

		//SJ_YYK 150817 Add...
		if(m_iWorkInstance %2 == 0)
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_3;
		else
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_4;
	}	

	m_bAutoInitialized = FALSE;

	m_bThreadLife = FALSE;
	m_pThread = NULL;
	m_bAutoRun = FALSE;

	//m_bTabCarrierVacOn = FALSE; //161221 SJ_YSH
	m_uiRetryCount = 0;
	m_iWorkTabNo = -1;
	m_iWorkToolNo = -1;

	initializeITI();
	initializeStep();
	
	freeReload();

	m_bModelChangeReady = FALSE;
	m_b1FeederAllMountInitReady = FALSE;
	//120116.KMS_________
	m_bMountAutoChangeReady[0] = FALSE;
	m_bMountAutoChangeReady[1] = FALSE;
//KKY.Del	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
//KKY.Del		m_rgiWorkTabICNo[i] = -1;
	m_iScheduleNo[m_iWorkGroup] = 0;
}

MTrsTabCarrier::~MTrsTabCarrier()
{
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
void MTrsTabCarrier::simulLog(CString strMsg, int iStep, int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s : %s(%d)\n"), GetObjectName(), strMsg, iStep);

	FILE *fp = fopen(".\\StepLog.txt", "a");
//	fprintf(fp, "%s\n", strLog);
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

int MTrsTabCarrier::getVacPosID(int iTabNo)		//@--->미사용 함수
{
	int iTabCarrierVacPosID = -1;
	int iIndex = DEF_MAX_WORKER_PER_GROUP;
	int k = 0;
	switch (m_eWorkSide)
	{
	case WORKING_SIDE_X_UP:
#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (k = m_plnkProcessData->GetStartTabNo(WORKING_SIDE_X_UP); k > iTabNo - 1; k--)
#else	// Mirror Line 인 경우
		for (k = m_plnkProcessData->GetStartTabNo(WORKING_SIDE_X_UP) - 1; k < iTabNo; k++)
#endif
		{
			iIndex = ((0 == iIndex) ? DEF_MAX_WORKER_PER_GROUP : iIndex) - 1;
		}

		iTabCarrierVacPosID = iIndex;
		break;
	case WORKING_SIDE_X_DOWN:
		break;
	case WORKING_SIDE_Y_LEFT:
		iTabCarrierVacPosID = iTabNo % DEF_MAX_WORKER_PER_GROUP;
		break;
	case WORKING_SIDE_Y_RIGHT:
		iTabCarrierVacPosID = iTabNo % DEF_MAX_WORKER_PER_GROUP;
		break;
	default:
		break;
	}
	return iTabCarrierVacPosID;
}

/** Thread 함수*/
UINT MTrsTabCarrier::threadFunction(LPVOID pParam)
{
	// TrsTabCarrier Object Pointer Assign
	MTrsTabCarrier*	pThis = (MTrsTabCarrier*) pParam;

	TRACE(_T("MTrsTabCarrier Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsTabCarrier Thread Stop\n"));

	return ERR_TRS_TABCARRIER_SUCCESS;
}

void MTrsTabCarrier::threadJob()
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
			if (m_estepCurrent == TABCARRIER_LOADING) 
			{
				m_plnkTabCarrier->Down();
				initializeMelsecAOCIF();
			}
			//_______________________
			m_bTabICSendAbleToTM = FALSE;
			m_TimerError.StopTimer();

//			m_plnkTabCarrier->Down(TRUE);
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

/** RUN Step을 수행한다 */
void MTrsTabCarrier::doRunStep()
{
	int iResult = ERR_TRS_TABCARRIER_SUCCESS;
	BOOL bAllMountComplete = TRUE;
	int i = 0;
	int iPosID = 0;
	MPos_XYT TolPos;
	BOOL bFirstCarrier = FALSE;
	BOOL bSecondCarrier = FALSE;

	MTickTimer timer;

	
	if(m_iWorkGroup == DEF_REAR_GROUP)
		return;

	//_________________________________
	if (isExistReload())
	{
		initializeITI();
		iResult = escapeForReload();
		if (iResult)
			PROCESS_ALARM(iResult);
		return;
	}
	//_________________________________

	ITI_TabMounter*	pITITabMounter = NULL;

//	TRACE(_T("Carrier%d :: Step : %d\n"), m_iWorkInstance, (int) m_estepCurrent);
	switch (m_estepCurrent)
	{
	case TABCARRIER_WAIT_JOB:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_READY_POS))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();

			iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_READY_POS);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(417102));
				}
				break;
			}
			m_TimerError.StopTimer();
			//161221 SJ_YSH
			if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE)
			{
				//if(m_bTabCarrierVacOn == FALSE)
				if(m_bTabICUnloadToTabMounter[m_iWorkGroup][m_iWorkInstance] == TRUE)
				{	
					iResult = m_plnkTabCarrier->AbsorbTabIC(TRUE);
					if (iResult)
					{
						PROCESS_ALARM(iResult);
						break;
					}
				}
			}
			//___________

			iResult = m_plnkCtrlTabCarrier->MoveReadyPos();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//161221 SJ_YSH
			if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE && DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
			{
				//if(m_bTabCarrierVacOn == FALSE)
				if(m_bTabICUnloadToTabMounter[m_iWorkGroup][m_iWorkInstance] == TRUE)
				{
					m_bTabICUnloadToTabMounter[m_iWorkGroup][m_iWorkInstance] = FALSE;
					if(m_plnkTabCarrier->IsAbsorbTabIC())
					{
						if(m_iWorkGroup == DEF_FRONT_GROUP)
							iResult = generateErrorCode(417107);
						else
							iResult = generateErrorCode(417108);
					}
					if(m_plnkTabCarrier->CheckVacuum())
					{
						iResult = 0;
						m_plnkTabCarrier->ReleaseTabIC();
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

		m_bLoadingCompleteToTM = FALSE;
		m_bUnloadComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = TRUE;
		
		m_bPrealignComplete[m_iWorkGroup][m_iWorkInstance] = FALSE; //SJ_YYK 150109 Add..
		m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance] = FALSE; //SJ_YYK 150908 Add...


		//Group Starter일 경우 WorkSchedule로부터 작업을 구해온다
		if (TRUE == isGroupStarter() && TRUE == isAllLoadReady())
		{
			m_bAllLoadReady[m_iWorkGroup] = FALSE;
			m_bAllUnloadComplete[m_iWorkGroup] = FALSE;
			if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetUseWorkSide())
				break;
			if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetSupplyTabCnt(m_iWorkGroup) <= 0)
				break;

			m_bAllLoadReady[m_iWorkGroup] = TRUE;
		}
		
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		if (FALSE == m_bAllLoadReady[m_iWorkGroup])
			break;

		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bAllLoadMoveComplete[m_iWorkGroup] = FALSE;
		m_bLoadMoveComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;

		m_bSupplyTab = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsSupplyTab(m_iWorkGroup, m_iWorkInstance);
		if (TRUE == m_bSupplyTab)
			setStep(TABCARRIER_LOADINGREADY_MOVE);
		else
		{
			m_bUnloadingReady[m_iWorkGroup][m_iWorkInstance] = TRUE;//SJ_YYK 150109 Add..
			m_bPrealignComplete[m_iWorkGroup][m_iWorkInstance] = TRUE; //SJ_YYK 150109 Add..
			m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance] = TRUE; //SJ_YYK 150908 Add..

			if (TRUE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExistNextSupplyCarrier(m_iWorkGroup, m_iWorkInstance))		
				setStep(TABCARRIER_UNLOADING_MOVE);
			else
				setStep(TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP);
		}
		break;

	case TABCARRIER_LOADINGREADY_MOVE:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		if (m_iWorkInstance == GetGroupStarter() || m_iWorkInstance == GetNextGroupStarter())
		{
			setStep(TABCARRIER_LOADING_MOVE);
		}
		else
		{
			MPos_XYT TgPos = m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS);
			//4,3번 Carrier일 경우 PreCarrier가 Loading위치로 이동시 따라 움직이게 한다
			TgPos.dX += (DEF_TABCARRIER_STANDARD_GAP + 1.0);
			iResult = m_plnkTabCarrier->CheckCollisionOther(TgPos);
			if (iResult)
				break;
			//SJ_YYK 150109 
			iResult = m_plnkTabCarrier->MoveXYTPos(TgPos, DEF_TABCARRIER_READY_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			setStep(TABCARRIER_LOADING_MOVE);
		}
		break;

	case TABCARRIER_LOADING_MOVE:

		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		if (PASS_RUN_MODE == m_plnkSystemData->m_eRunMode)
			break;

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();

		//SJ_YYK 150109 
		iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(417103));
			}
			break;
		}
		//_________________________*/

		if (m_sReloadInfo[m_iWorkGroup].iInstanceNo == m_iWorkInstance)
		{
			m_plnkTabCarrier->ReleaseTabIC();
			Sleep(100);
		}

		m_TimerError.StopTimer();
		simulLog("TABCARRIER_LOADING_MOVE", m_estepCurrent, 100);

		if (FALSE == m_plnkTabCarrier->IsAbsorbTabIC())
		{	
			m_plnkTabCarrier->ReleaseTabIC(TRUE);
			{
			}
		}

		//SJ_YYK 150109 Modify...
		iResult = m_plnkCtrlTabCarrier->MoveLoadingPos();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		//_______________________*/

		if (FALSE == m_plnkTabCarrier->IsAbsorbTabIC())
		{	
			m_plnkTabCarrier->ReleaseTabIC(TRUE);

			//m_plnkMelsecIF2->SetUpperEqBit(eReceiveAble, TRUE, m_iAOCUnitNo);

			//SJ_YYK 150908 Add...
			if(m_iWorkInstance % 2 == 0)
			{
				if(m_iWorkGroup == DEF_FRONT_GROUP)
				{
					if(m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance + 1] == TRUE
						|| TRUE == MOlbSystemPre.GetTabCarrierComponent(m_iWorkInstance +1)->IsAbsorbTabIC())
						m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, FALSE, m_iAOCUnitNo, TRUE);
					else 
					{
						m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, TRUE, m_iAOCUnitNo, TRUE);
						m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, TRUE, m_iAOCUnitNo, TRUE);
					}
				}
				else
				{
					if(m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance + 1] == TRUE
						|| TRUE == MOlbSystemPre.GetRTabCarrierComponent(m_iWorkInstance +1)->IsAbsorbTabIC())
						m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, FALSE, m_iAOCUnitNo, TRUE);
					else 
					{
						m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, TRUE, m_iAOCUnitNo, TRUE);
						m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, TRUE, m_iAOCUnitNo, TRUE);
					}
				}

			}

		}
		//SJ_YYK 150908 Add...
		else
		{
			if(m_iWorkInstance %2 == 0)
			{
				m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, FALSE, m_iAOCUnitNo, TRUE);

			}
			
		}
		m_bLoadingMoveCompleteToTM = TRUE;

		if (m_sReloadInfo[m_iWorkGroup].iInstanceNo == m_iWorkInstance)
		{
			freeReload();
		}

//SJ_YYK 150817 Modify...
		//SJ_YYK 150913 Modify..
		if(TRUE == m_plnkTabCarrier->IsAbsorbTabIC())
		{
			m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;
			m_bLoadingCompleteToTM = TRUE;
			setStep(TABCARRIER_MOVE_PREALIGN_POS);
			break;
		}

		setStep(TABCARRIER_LOADING);
		break;
		
	case TABCARRIER_LOADING: simulLog("TABCARRIER_LOADING_WAIT", m_estepCurrent + 100, 100);

		//SJ_YYK 151105 Add..
		if(m_iWorkInstance % 2 == 0)
			m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, TRUE, m_iAOCUnitNo, TRUE);
		else
			m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, TRUE, m_iAOCUnitNo, TRUE); //170718_KDH 제어기 분리해서 -1 있으면 안됨!!

		m_plnkMelsecIF2->SetUpperEqBit(eReceiveAble, TRUE, m_iAOCUnitNo, TRUE);



		/*100822.임시삭제.필요한지 확인요_____
		iResult = m_plnkCtrlTabCarrier->WaitTabIC();
		if (iResult)
		{
			SetErrorLevel(_T("TAB CARRIER load Tab IC"), 0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break; 
		}
		*/
		//110420.Add_____
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}
		//_______________

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/


		/*/@130119.KKY.Del(금형 Pin Up/Down일 경우 Pin상승후 Carrier상승해야 Carrier노즐에 충격이 없음)
//		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			if (TRUE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
			{
				m_plnkTabCarrier->Up(TRUE);
			}
		}
		/*/
		
		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			//jskim 여기에 AOC와 Interface 추가 해야함..
			if (FALSE == m_plnkMelsecIF2->IsOnUpperEqBit(eSendAble, m_iAOCUnitNo, TRUE))
				break;

			m_plnkMelsecIF2->SetUpperEqBit(eReceiveStart, TRUE, m_iAOCUnitNo, TRUE);

			while (TRUE)
			{
				if (TRUE == m_plnkMelsecIF2->IsOnUpperEqBit(eSendStart, m_iAOCUnitNo, TRUE))
					break;

				if (FALSE == m_plnkMelsecIF2->IsOnUpperEqBit(eSendAble, m_iAOCUnitNo, TRUE))
				{
					m_plnkMelsecIF2->SetUpperEqBit(eReceiveStart, FALSE, m_iAOCUnitNo, TRUE);
					return;
				}
				Sleep(5);
			}

			simulLog("TABCARRIER_LOADING", m_estepCurrent, 100);
			
			iResult = m_plnkCtrlTabCarrier->LoadTabIC();
			if (iResult)
			{
				if(m_iWorkInstance %2 == 0)
				{
					m_plnkTabCarrier->Down();
					//SJ_YYK 150908 Add...
					if(m_iWorkGroup == DEF_FRONT_GROUP)
						m_plnkMelsecIF2->SetUpperEqBit(eTurnSafetyAOC1, TRUE, DEF_AOC_UNIT_NO_1, TRUE);
					else
						m_plnkMelsecIF2->SetUpperEqBit(eTurnSafetyAOC1, TRUE, DEF_AOC_UNIT_NO_3, TRUE);
				}
				else 
				{
					m_plnkTabCarrier->Down2();
					//SJ_YYK 150908 Add...
					if(m_iWorkGroup == DEF_FRONT_GROUP)
						m_plnkMelsecIF2->SetUpperEqBit(eTurnSafetyAOC1, TRUE, DEF_AOC_UNIT_NO_2, TRUE);
					else
						m_plnkMelsecIF2->SetUpperEqBit(eTurnSafetyAOC1, TRUE, DEF_AOC_UNIT_NO_4, TRUE);
				}

				initializeMelsecAOCIF(); //SJ_YYK 150913 Add..
				PROCESS_ALARM(iResult);
				break;
			}

			m_plnkMelsecIF2->SetUpperEqBit(eReceiveComplete, TRUE, m_iAOCUnitNo, TRUE);

			m_TimerError.StartTimer();

			while (TRUE)
			{
				if (TRUE == m_plnkMelsecIF2->IsOnUpperEqBit(eSendComplete, m_iAOCUnitNo, TRUE))
					break;

				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(217032));
					break;
				}			

				Sleep(5);
			}

			initializeMelsecAOCIF(); //SJ_YYK 150913 Add..
		}


		if(m_iWorkInstance %2 == 0)
		{
			if (FALSE == m_plnkTabCarrier->IsDown())
			{
				iResult = m_plnkTabCarrier->Down();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					return;
				}
			}
		}
		else
		{
			if (FALSE == m_plnkTabCarrier->IsDown2())
			{
				iResult = m_plnkTabCarrier->Down2();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					return;
				}
			}
		}

		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode
			&& FALSE == m_plnkTabCarrier->IsAbsorbTabIC() )
		{
			// 417007 = Loading 동작 완료 후 TabIC 를 흡착하고 있지 않음.
			PROCESS_ALARM(generateErrorCode(417007));
			return;
		}



		m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance] = TRUE; //SJ_YYK 150908 Add...

		setStep(TABCARRIER_MOVE_PREALIGN_POS);
		break;

	case TABCARRIER_MOVE_PREALIGN_POS:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		m_bLoadingCompleteToTM = TRUE;

		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();


		//SJ_YYK 150908 Add...
		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			if(m_iWorkInstance %2 == 0)
			{
				if(m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance + 1] == FALSE)
					break;
			}
			else
			{
				if(m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance - 1] == FALSE)
					break;
			}
		}

		//SJ_YYK 151105 Del..
		/*
		if(m_iWorkInstance % 2 == 0)
		{
			m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, FALSE, m_iAOCUnitNo);
			m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, FALSE, m_iAOCUnitNo);
		}
		*/
		//______________________________*/

		iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			//110128.Add_____
			double dPrealignPosT;
			int iResult2 = 0;
			dPrealignPosT = m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dT;
			
			iResult2 = m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->StartMove(dPrealignPosT);
			if(iResult2)
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(iResult);
				break;
			}

			//_______________

			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(417104));
			}
			break;
		}
		m_TimerError.StopTimer();

		//SJ_YYK 150213 Add..

		iResult = m_plnkCtrlTabCarrier->MovePreAlignPos();
		if (iResult != ERR_TRS_TABCARRIER_SUCCESS)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(TABCARRIER_PREALIGN);
		break;

	case TABCARRIER_PREALIGN:
//SJ_YYK 150213 Add..
		if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_PREALIGN_POS))
		{
			setStep(TABCARRIER_MOVE_PREALIGN_POS);
			break;
		}

		m_posAlignOffset.Init(0.0, 0.0, 0.0);
		iResult = m_plnkCtrlTabCarrier->AlignTabIC(&m_posAlignOffset);
		if (iResult)
		{
			Sleep(200);
			m_uiRetryCount++;
			if (m_uiRetryCount >= 3)
			{
				m_uiRetryCount = 0;
				PROCESS_ALARM(iResult);
				if (!isExistReload())
				{
					registerReload();
					setStep(TABCARRIER_LOADING_MOVE);
					m_bLoadingCompleteToTM = FALSE;
				}
				break;
			}

			setStep(TABCARRIER_MOVE_PREALIGN_POS);
			break;
		}

		if(FALSE)
		{	
			Sleep(700);
			m_uiRetryCount++;
			if (m_uiRetryCount >= 5)
			{
				m_uiRetryCount = 0;
				PROCESS_ALARM(generateErrorCode(417101));
				break;
			}
			break;
		}
		m_uiRetryCount = 0;
//@#ifdef DEF_SOURCE_SYSTEM
//@		setStep(TABCARRIER_UNLOADING_MOVE);
//@#else

		m_bPrealignComplete[m_iWorkGroup][m_iWorkInstance] = TRUE; //SJ_YYK 150109 Add..

		m_bUnloadingReady[m_iWorkGroup][m_iWorkInstance] = TRUE; //SJ_YYK 150211 Add..

		//SJ_YYK 150331 Add... Align Offset 전달 용..
		m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS); 

		setStep(TABCARRIER_UNLOADING_MOVE_READY);
//@#endif
		break;
		
	case TABCARRIER_UNLOADING_MOVE_READY:
		
		//SJ_YYK 110503 Add.. One Touch Model Change ...
//@#ifdef DEF_GATE_SYSTEM
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}
//@#endif
		if (FALSE == isGroupStarter())
		{

			//SJ_YYK 150109 modify..
			MPos_XYT UnloadReadyPos = m_plnkTabCarrier->GetUnloadingReadyPos();
			iResult = m_plnkTabCarrier->CheckCollisionOther(UnloadReadyPos);
			if (iResult)
				break;
			iResult = m_plnkTabCarrier->MoveXYTPos(UnloadReadyPos, DEF_TABCARRIER_UNLOAD_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			//_____________________________*/

			/*/
			//111118.KMS Compare 중 추가
			//20111104. SJ_YTH. Unloading 전에 확인.
			if( FALSE == m_plnkTabCarrier->IsBackwardDumpBasket())
			{
				Sleep(300);
				if( FALSE == m_plnkTabCarrier->IsBackwardDumpBasket())
				{
					iResult = m_plnkTabCarrier->BackwardDumpBasket();
					if (iResult)
					{
						SetErrorLevel(_T("TAB MOUNTER Remove Tab IC"),1, __FILE__, __LINE__);						
						PROCESS_ALARM(iResult);
						return;
					}
				}
			}
			/*/

		}

		setStep(TABCARRIER_UNLOADING_MOVE);
		break;

	case TABCARRIER_UNLOADING_MOVE: simulLog("TABCARRIER_UNLOADING_MOVE", m_estepCurrent, 100);
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

//@#ifdef DEF_GATE_SYSTEM
		iPosID = DEF_TABCARRIER_UNLOAD_POS;
//@#else
//		if (m_iWorkGroupMounter[m_iWorkGroup] == m_iWorkGroup)
//			iPosID = DEF_TABCARRIER_UNLOAD_POS;
//		else
//			iPosID = DEF_TABCARRIER_R_UNLOAD_POS;
//@		iPosID = DEF_TABCARRIER_UNLOAD_POS;
//@#endif
				
		if (FALSE == m_plnkTabCarrier->IsInXYTPos(iPosID))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();

			iResult = m_plnkTabCarrier->CheckCollisionOther(iPosID);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(417105));
				}
				break;
			}
			m_TimerError.StopTimer();
			
//			if (iPosID == DEF_TABCARRIER_UNLOAD_POS)
				iResult = m_plnkCtrlTabCarrier->MoveUnloadingPos();
//			else
//				iResult = m_plnkCtrlTabCarrier->MoveUnloading2Pos();
			if (iResult != ERR_TRS_TABCARRIER_SUCCESS)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		if (TRUE == m_bSupplyTab)
		{
			m_bUnloadingMoveCompleteToTM = TRUE;
			setStep(TABCARRIER_UNLOADING);
		}
		else
		{
			setStep(TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP);
		}
#else
		/*/


		if (TRUE == m_bSupplyTab)
		{
			m_bUnloadingMoveCompleteToTM = TRUE;
			setStep(TABCARRIER_UNLOADING);
		}
		else
		{
			m_bLoadingCompleteToTM = TRUE;
			setStep(TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP);
		}
//@#endif
		break;
		
	case TABCARRIER_UNLOADING: simulLog("TABCARRIER_UNLOADING_WAIT", m_estepCurrent + 100, 100);
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		if (TRUE == isGroupStarter())
		{
			if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_SupplyWorkList(m_iWorkGroupMounter[m_iWorkGroup]))
				break;
		}

		m_iWorkToolNo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetCarrierToTool_FromCurrentSupplyList(m_iWorkGroupMounter[m_iWorkGroup], m_iWorkGroup, m_iWorkInstance);
		if (m_iWorkToolNo == -1)
			break;

		pITITabMounter = m_plnkITITabMounter[m_iWorkToolNo];

// 		TolPos.Init(1.0,1.0,1.0);
// 		if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS, &TolPos))
// 		{
// 			setStep(TABCARRIER_UNLOADING_MOVE);
// 			break;
// 		}

		// Data 유효성 검증.
		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			BOOL bTabICLoadingRetry = FALSE;
			BOOL bAbsorbStatus = m_plnkTabCarrier->IsAbsorbTabIC();
			if (FALSE == bAbsorbStatus)
			{
				// 확인 필요 : 순간적으로 Vacuum 압이 떨어져서 발생하는 문제일 수 있나?
				Sleep(300);

				if (FALSE == m_plnkTabCarrier->IsAbsorbTabIC())	// Vacuum 상태 재확인.
				{
					bTabICLoadingRetry = TRUE;
				}
			}

			if (TRUE == bTabICLoadingRetry)
			{
				if (isExistReload())
					break;
				registerReload();
				setStep(TABCARRIER_LOADING_MOVE);
				m_bLoadingCompleteToTM = FALSE;
				break;
			}
		}

		m_bTabICSendAbleToTM = TRUE;

		if (m_iWorkTabNo != -1)
		{
			iResult = m_plnkCtrlTabCarrier->MoveUnloadingPos(m_iWorkTabNo);
			if (iResult)
			{
				m_bTabICSendAbleToTM = FALSE;
				m_bTabICSendStartToTM = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
		//iWorkTabMounterNo = GetTabMounterNo();
		if (FALSE == pITITabMounter->IsTabICReceiveStartToTC())
			break;
		
		
		m_bTabICSendStartToTM = TRUE;

		simulLog("TABCARRIER_UNLOADING", m_estepCurrent, 100);

		timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == timer.MoreThan(30.0))
			{
				timer.StopTimer();
				// 217022 = TabCarrier Unloading 중 TimeOut
				PROCESS_ALARM(generateErrorCode(217022));
				m_bTabICSendAbleToTM = FALSE;
				m_bTabICSendStartToTM = FALSE;
				//break;
				return;
			}

			Sleep(5);

			if (pITITabMounter->IsTabICReceiveCompleteToTC())
				break;

			if (FALSE == pITITabMounter->IsTabICReceiveStartToTC())
			{
				m_bTabICSendAbleToTM = FALSE;
				m_bTabICSendStartToTM = FALSE;
				return;
			}
		}

//@#ifdef DEF_GATE_SYSTEM
//		if (TRUE == isGroupStarter())
		{
			m_bUnloadComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;
		}
//@#endif
		m_bSupplyTab = FALSE;
		m_bTabICSendAbleToTM = FALSE;
		m_bTabICSendStartToTM = FALSE;
		m_iWorkTabNo = -1;

//@#ifdef DEF_SOURCE_SYSTEM
//@		setStep(TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP);
//@#else

		//@130106________
		//@setStep(TABCARRIER_MOVE_ESPACE_POS);
		setStep(TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP);
//@#endif
		break;

//@#ifdef DEF_GATE_SYSTEM
	case TABCARRIER_MOVE_ESPACE_POS:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_ESCAPE_POS))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();

			iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_ESCAPE_POS);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
			 		PROCESS_ALARM(generateErrorCode(417106));
				}
				break;
			}
			m_TimerError.StopTimer();

			iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_ESCAPE_POS);
			if (iResult != ERR_TRS_TABCARRIER_SUCCESS)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		setStep(TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP);
		break;
//@#endif

	case TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
#endif
		/*/

		m_bUnloadComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;
		//161221 SJ_YSH
		if(m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading == TRUE)
		{
			//if(m_bTabCarrierVacOn == FALSE)
			if(m_bTabICUnloadToTabMounter[m_iWorkGroup][m_iWorkInstance] == TRUE)
			{	
				iResult = m_plnkTabCarrier->AbsorbTabIC(TRUE);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
		//___________
		if (TRUE == isGroupStarter() && TRUE == isAllUnloadComplete())
		{	
			
			//SJ_YYK 150227 Modify 시점 이동....
			if (FALSE == m_bAllUnloadComplete[m_iWorkGroup])
			{
				//@_____________________________
				if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsAllSupplyComplete(TRUE, m_iWorkGroup))
				{
					m_iScheduleNo[m_iWorkGroup]++;
					
					//170719_KDH 작업할꺼 없으면 Skip Test중.
					for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
					{
						MListWorkTab ListWorkTab;
						m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
						if(ListWorkTab.GetTotalWorkTabCount() > 0)
							break;
					}
					m_iScheduleNo[m_iWorkGroup] = i;
					//____________________
					if (m_iScheduleNo[m_iWorkGroup] >= DEF_MAX_WORK_SCHEDULE)
					{
//						m_iScheduleNo[m_iWorkGroup] = 0;
						//170719_KDH 작업할꺼 없으면 Skip Test중.
						m_iScheduleNo[m_iWorkGroup] = 0;	//@
						BOOL bLastWorkGroup = TRUE;
						int i = 0;
						for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
						{
							MListWorkTab ListWorkTab;
							m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
							if(ListWorkTab.GetTotalWorkTabCount() > 0)
								break;			
						}
						m_iScheduleNo[m_iWorkGroup] = i;
						//_______________________

						Sleep(100);	//@KKY.130824
					}
				}
				//@_____________________________

				//@140311.KKY___________
				iResult = moveAll_Unload_To_LoadReady();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
				//@_____________________

			}

			//@______________


			//@KKY.130908___________
			MPos_XYT tgPos[DEF_MAX_TABCARRIER];
			int i = 0;
			//170203 SJ_YSH Del.
			/*/
			for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
			{

				if (m_iWorkGroup == DEF_FRONT_GROUP)
				{
					tgPos[i] = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
					iResult = MOlbSystemPre.GetTabCarrierComponent(i)->MoveXYTPos(tgPos[i], DEF_TABCARRIER_READY_POS, FALSE, TRUE);
				}
				else
				{
					tgPos[i] = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
					iResult = MOlbSystemPre.GetRTabCarrierComponent(i)->MoveXYTPos(tgPos[i], DEF_TABCARRIER_READY_POS, FALSE, TRUE);
				}
				if (iResult)
				{
					break;
				}
			}
			/*/

			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			iResult = m_plnkTabCarrier->Wait4DoneXYT();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
			{
				if (m_iWorkGroup == DEF_FRONT_GROUP)
				{
					MOlbSystemPre.GetTabCarrierComponent(i)->SetMovingY(FALSE, tgPos[i]);
				}
				else
				{
					MOlbSystemPre.GetRTabCarrierComponent(i)->SetMovingY(FALSE, tgPos[i]);
				}
			}
			//@_____________________
			m_bAllLoadReady[m_iWorkGroup] = FALSE;
			m_bAllUnloadComplete[m_iWorkGroup] = TRUE;
//			TRACE(_T("<<<<<MTrsTabCarrier:All Unloading Complete[WorkGroup=%d]>>>>>\n"), m_iWorkGroup);
		}

		if (TRUE == m_bAllUnloadComplete[m_iWorkGroup])
		{
			m_bLoadingMoveCompleteToTM = FALSE;
			m_bLoadingCompleteToTM = FALSE;
			m_bUnloadingMoveCompleteToTM = FALSE;
			m_bUnloadComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
			m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
			m_bUnloadingReady[m_iWorkGroup][m_iWorkInstance] = FALSE; //SJ_YYK 150109 Add..
			m_bAllUnloadingReady[m_iWorkGroup] = FALSE; //SJ_YYK 150109 Add..
			//setStep(TABCARRIER_WAIT_JOB);
		}
		else
		{
			break;
		}

		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();
		
		//SJ_YYK 110901 Add...
		//Ready 위치로 빠지기전 align Offset이 1.5 이상이 될 경우 
		//Check Collision 에서 무한 루프...
		m_plnkTabCarrier->InitAlignMarkOffset();
		iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_READY_POS);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(417102));
			}
			break;
		}
		m_TimerError.StopTimer();

		/*
		//111229 Add_____________
		//Unloading위치에서 Loading위치로이동시 미리 타발하도록
		if (TRUE == isGroupStarter())
		{
//			if (FALSE == m_plnkTabCarrier->IsAbsorbTabIC())
//			{	
//				m_plnkTabCarrier->ReleaseTabIC(TRUE);
//				m_bTabICReceiveAbleToTF = TRUE;
//			}

			//@______________
			if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetSupplyTabCnt(m_iWorkGroup) > 0)
			//@______________
				m_bTabICReceiveAbleToTFRequest = TRUE;
		}
		//_______________________

		*/

		/*
		iResult = m_plnkCtrlTabCarrier->MoveReadyPos();
		if (iResult)
		{
			SetErrorLevel(_T("TAB CARRIER move to Ready Pos"), 0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}
		*/
		m_bLoadMoveComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;
		setStep(TABCARRIER_CHECK_SUPPLY_COMPLETE);

	case TABCARRIER_CHECK_SUPPLY_COMPLETE:

		if (TRUE == isGroupStarter())
		{
			if (TRUE == isAllLoadMoveComplete())
				m_bAllLoadMoveComplete[m_iWorkGroup] = TRUE;
			else
				m_bAllLoadMoveComplete[m_iWorkGroup] = FALSE;
		}
		if (FALSE == m_bAllLoadMoveComplete[m_iWorkGroup])
			break;

		m_iWorkToolNo = -1;

		//m_bTabCarrierVacOn = FALSE; //170724 JSH
		
		setStep(TABCARRIER_WAIT_JOB);
		break;

		//NSMC KJS
	case TABCARRIER_MODEL_CHANGE: simulLog("TABCARRIER_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;
		
		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;
		
//		m_plnkTabCarrier->ReleaseTabIC(DEF_VAC_TAB_CARRIER_ALL);
		m_plnkTabCarrier->ReleaseTabIC();

		m_bModelChangeReady = FALSE;
		
		//setStep(TABCARRIER_LOADING_MOVE);
		setStep(TABCARRIER_WAIT_JOB);
		break;

	case TABCARRIER_READY_MOVE_FOR_FEEDERREADY:
		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();
		iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_READY_POS);
		if (iResult)
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				PROCESS_ALARM(generateErrorCode(417102));
			}
			break;
		}
		m_TimerError.StopTimer();

		iResult = m_plnkCtrlTabCarrier->MoveReadyPos();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		m_b1FeederAllMountInitReady = FALSE;
		setStep(TABCARRIER_WAIT_1FEEDERALLMOUNT_INIT);
		break;

	case TABCARRIER_WAIT_1FEEDERALLMOUNT_INIT:
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
					break;
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
					break;
			}
			m_bMountAutoChangeReady[1] = FALSE;
		}
		//___________________

		initializeITI();
		freeReload();
		m_uiRetryCount = 0;
		m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bAllLoadReady[m_iWorkGroup] = FALSE;
		m_bAllUnloadComplete[m_iWorkGroup] = FALSE;
		m_bLoadMoveComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_bAllLoadMoveComplete[m_iWorkGroup] = FALSE;
		m_iWorkTabNo = -1;

		if(m_plnkSystemData->m_bUse1FeederAllMount == TRUE)
			m_b1FeederAllMountInitReady = FALSE;

		Sleep(1000);

		if (TRUE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
			setStep(TABCARRIER_WAIT_JOB);
		else
			setStep(TABCARRIER_WAIT_FEEDERREADY);
		break;

	//TabIC를 다 소진한 Group일 경우 기다림...
	case TABCARRIER_WAIT_FEEDERREADY:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABCARRIER_MODEL_CHANGE);
			break;
		}

		//120116.kms_________
		if(FALSE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
			break;
		//___________________

		if(m_plnkSystemData->m_bUse1FeederAllMount == TRUE)
		{
			if (FALSE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
			{
				break;
			}
			m_b1FeederAllMountInitReady = FALSE;
		}
		m_bMountAutoChangeReady[0] = FALSE;
		m_bMountAutoChangeReady[0] = FALSE;
		setStep(TABCARRIER_READY_MOVE_FOR_FEEDERREADY);
		Sleep(100);
		break;
	default:
		break;
	}
}

/** Thread간 Interface 초기화 */
void MTrsTabCarrier::initializeITI()
{

	m_bTabICSendAbleToTM = FALSE;
	m_bTabICSendStartToTM = FALSE;
	m_bLoadingMoveCompleteToTM = FALSE;
	m_bLoadingCompleteToTM = FALSE;
	m_bUnloadingMoveCompleteToTM = FALSE;

	m_bTabICTrash_ReceiveAbleToTM = FALSE;
	m_bTabICTrash_ReceiveCompleteToTM = FALSE;
	m_bTabICUnloadToTabMounter[m_iWorkGroup][m_iWorkInstance] = FALSE; //170830 JSH
}
//SJ_YYK 150913 
void MTrsTabCarrier::initializeMelsecAOCIF()
{
	m_plnkMelsecIF2->SetUpperEqBit(eReceiveAble, FALSE, m_iAOCUnitNo, TRUE);
	m_plnkMelsecIF2->SetUpperEqBit(eReceiveStart, FALSE, m_iAOCUnitNo, TRUE);
	m_plnkMelsecIF2->SetUpperEqBit(eArm1FoldComplete, FALSE, m_iAOCUnitNo, TRUE);
	m_plnkMelsecIF2->SetUpperEqBit(eReceiveComplete, FALSE, m_iAOCUnitNo, TRUE);
	//SJ_YYK 151105 Add..
	if(m_iWorkInstance % 2 == 0)
		m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, FALSE, m_iAOCUnitNo, TRUE);
	else
		m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, FALSE, m_iAOCUnitNo - 1, TRUE);
}
//_______________________*/	
void MTrsTabCarrier::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepTabCarrier)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepTabCarrier)0;	//  현재 Step Clear
}

/** Step Thread Step */
void MTrsTabCarrier::setStep(EStepTabCarrier estepTabCarrier)
{
	m_estepPrevious	= m_estepCurrent;	// 이전 Step 저장
	m_estepCurrent	= estepTabCarrier;	// 새로 Step으로 전환
}


/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

/**
* Assign component linkage
* 
* @param	listRefComponents	: MTrsTabCarrier 참조 Component list
*/
int MTrsTabCarrier::AssignComponents(STrsTabCarrierRefCompList listRefComponents)
{
	m_plnkTrsAutoManager	= listRefComponents.m_plnkTrsAutoManager;
	m_plnkCtrlTabCarrier	= listRefComponents.m_plnkCtrlTabCarrier;
	m_plnkTabCarrier		= listRefComponents.m_plnkTabCarrier;
	m_plnkPanelAligner		= listRefComponents.m_plnkPanelAligner;
	m_plnkMelsecIF2			= listRefComponents.m_plnkMelsecIF2;

	return ERR_TRS_TABCARRIER_SUCCESS;
}

void MTrsTabCarrier::AssignProcess(STrsTabCarrierRefProcess listRefTrs)
{
	m_plnkCtrlTabFeeder[DEF_TABFEEDER1] = listRefTrs.m_plnkCtrlTabFeeder[DEF_TABFEEDER1];
	m_plnkCtrlTabFeeder[DEF_TABFEEDER2] = listRefTrs.m_plnkCtrlTabFeeder[DEF_TABFEEDER2];
}

/** 
* Component Data 처리 
*
* @param	pData : SMMTrsTabCarrierData 구조체에 대한 포인터
* @return	Error : 0 = Success, 그외=기타
*/
int MTrsTabCarrier::SetData(STrsTabCarrierData datComponent)
{
	m_plnkSystemData	= datComponent.m_plnkSystemData;
	m_plnkProcessData	= datComponent.m_plnkProcessData;
	m_plnkPreBonderData	= datComponent.m_plnkPreBonderData;//2009.04.04 SESL_CYJ
//@	m_plnkWorkSchedule	= datComponent.m_plnkWorkSchedule;
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST] = datComponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND] = datComponent.m_plnkWorkScheduleSecond;
#endif

	m_plnkDBTabLoss		= datComponent.m_plnkDBTabLoss;

	m_eWorkSide			= datComponent.m_eWorkSide;
	return ERR_TRS_TABCARRIER_SUCCESS;
}

/** 
 * TrsTabCarrier Component의 자동 운전 초기화를 시작한다.
 * @return	Error Code : 0 = SUCCESS, 그 외 = Error
 */
int MTrsTabCarrier::Initialize()
{
	initializeStep();	// 모든 Step을 초기 Step으로 지정한다.
	initializeITI();
	//SJ_YYK 150913 Add...
	initializeMelsecAOCIF();
	//SJ_YYK 151105 Del...
	/*
	if(m_iWorkInstance % 2 == 0)
	{
		m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo1, FALSE, m_iAOCUnitNo);
		m_plnkMelsecIF2->SetUpperEqBit(ePanelExistenceInfo2, FALSE, m_iAOCUnitNo);
	}
	*/
	//__________________________________*/
	m_bAutoInitialized  = FALSE;
	m_b1FeederAllMountInitReady = FALSE;
	//m_bTabCarrierVacOn = FALSE;
	//120116.KMS_________
	m_bMountAutoChangeReady[0] = FALSE;
	m_bMountAutoChangeReady[1] = FALSE;

	/*/
	//120106.kms____________
	//Feeder 사용이 All 이 아니고 Front 사용 이 아닐 경우 
#ifdef DEF_SOURCE_SYSTEM
	if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if(m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_1 && m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_ALL)
		{
			if(m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_bAutoInitialized  = TRUE;
				return ERR_TRS_TABCARRIER_SUCCESS;
			}
		}
		//Feeder 사용이 All 이 아니고 Rear 사용 이 아닐 경우 
		else if(m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_2 && m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_ALL)
		{
			if(m_iWorkGroup == DEF_REAR_GROUP)
			{
				m_bAutoInitialized  = TRUE;
				return ERR_TRS_TABCARRIER_SUCCESS;
			}
		}
	}
#endif
	//______________________
	/*/

	if(m_iWorkGroup == DEF_REAR_GROUP)
	{
		m_bAutoInitialized  = TRUE;
		return ERR_TRS_TABCARRIER_SUCCESS;
	}

#ifndef SIMULATION
	int iResult = ERR_TRS_TABCARRIER_SUCCESS;

	if (FALSE == m_plnkTabCarrier->IsAbsorbTabIC())
		m_plnkTabCarrier->ReleaseTabIC();

	//161221 SJ_YSH
// 	if(m_plnkTabCarrier->IsAbsorbTabIC() == TRUE)
// 		m_bTabCarrierVacOn = TRUE;
// 	else
 	if(m_plnkTabCarrier->IsAbsorbTabIC() == FALSE)
	{
		iResult = m_plnkTabCarrier->AbsorbTabIC();
		if (iResult == 0)
		{
			if(m_iWorkGroup == DEF_FRONT_GROUP)
				return generateErrorCode(417109);
			else
				return generateErrorCode(417110);
		}	
	}
	//_____________

	iResult = m_plnkTabCarrier->Down();
	if (iResult)
	{
		SetErrorLevel(_T("TAB CARRIER initialize"), 0, __FILE__, __LINE__);
		return iResult;
	}

	//170116 SJ_YSH
	iResult = m_plnkTabCarrier->Down2();
	if (iResult)
	{
		SetErrorLevel(_T("TAB CARRIER initialize"), 0, __FILE__, __LINE__);
		return iResult;
	}
	//_____________

	m_plnkTabCarrier->InitAlignMarkOffset();

	if (m_plnkTabCarrier->IsShortPitchTab())
		iResult = m_plnkTabCarrier->Forward();
	else
		iResult = m_plnkTabCarrier->Backward();
	if (iResult)
		return iResult;
	
	iResult = m_plnkCtrlTabCarrier->MoveReadyPos();
	if (iResult)
	{
		SetErrorLevel(_T("TAB CARRIER initialize"), 0, __FILE__, __LINE__);
		return iResult;
	}

#endif /* SIMULATION */

	//170719_KDH 작업할꺼 없으면 Skip Test중.
	m_iScheduleNo[m_iWorkGroup] = 0;	//@
	BOOL bLastWorkGroup = TRUE;
	int i = 0;
	for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
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

	freeReload();
	m_uiRetryCount = 0;
	m_bLoadReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
	m_bAllLoadReady[m_iWorkGroup] = FALSE;
	m_bAllUnloadComplete[m_iWorkGroup] = FALSE;
	//110223_____
	m_bLoadMoveComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	m_bAllLoadMoveComplete[m_iWorkGroup] = FALSE;
	//___________
	//SJ_YYK 150109 Add..
	m_bUnloadingReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
	m_bAllUnloadingReady[m_iWorkGroup] = FALSE;
	m_bPrealignComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	//_____________________________*/
	m_bLoadingComplete[m_iWorkGroup][m_iWorkInstance] = FALSE; //SJ_YYK 150908 Add..
	m_iWorkTabNo = -1;
	m_TimerError.StopTimer();
	m_bAutoInitialized  = TRUE;

	return ERR_TRS_TABCARRIER_SUCCESS;
}

/**
 * MTrsTabCarrier Initial 완료 여부를 해제한다.
 *
 * @return void
 */
void MTrsTabCarrier::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/
void MTrsTabCarrier::AssignITI(STabCarrierRefITIList sITIList)
{
	m_plnkITITabFeeder = sITIList.m_plnkITITabFeeder;
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		m_plnkITITabMounter[i] = sITIList.m_plnkITITabMounter[i];
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/

void MTrsTabCarrier::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsTabCarrier::ThreadStop()
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

/***********************************************************************************************
/*
/* ITI관련 함수
/*
/***********************************************************************************************/

/**
 * Initial 완료 여부 확인
 *
 * @return BOOL (TRUE : Initial 완료, FALSE : 미완료)
 */
BOOL MTrsTabCarrier::IsInitialized(void) 
{
	return m_bAutoInitialized;
}

//111229 Add_____________
//Unloading위치에서 Loading위치로이동시 미리 타발하도록
BOOL MTrsTabCarrier::IsTabICReceiveAbleToTFRequest()	
{ 
	return m_bTabICReceiveAbleToTFRequest;
}
//_______________________

BOOL MTrsTabCarrier::IsTabICReceiveAbleToTF()	
{ 
	return m_bTabICReceiveAbleToTF;
}

BOOL MTrsTabCarrier::IsTabICReceiveStartToTF()	
{ 
	return m_bTabICReceiveStartToTF;
}

BOOL MTrsTabCarrier::IsTabICReceiveCompleteToTF()	
{ 
	return m_bTabICReceiveCompleteToTF;	
}

BOOL MTrsTabCarrier::IsTabICSendAbleToTM() 
{ 
	return m_bTabICSendAbleToTM;
}

BOOL MTrsTabCarrier::IsTabICSendStartToTM() 
{ 
	return m_bTabICSendStartToTM;
}

BOOL MTrsTabCarrier::IsLoadingMoveCompleteToTM()
{
	return m_bLoadingMoveCompleteToTM;
}

BOOL MTrsTabCarrier::IsLoadingCompleteToTM()
{
	return m_bLoadingCompleteToTM;
}

BOOL MTrsTabCarrier::IsUnloadingMoveCompleteToTM()
{
	return m_bUnloadingMoveCompleteToTM;
}

BOOL MTrsTabCarrier::IsTabICTrash_ReceiveAbleToTM()
{
	return m_bTabICTrash_ReceiveAbleToTM;
}

BOOL MTrsTabCarrier::IsTabICTrash_ReceiveCompleteToTM()
{
	return m_bTabICTrash_ReceiveCompleteToTM;
}

//TabMounter가 Carrier에서 TabLoading전 Tab번호를 지정하면 TabCarrier가 TabOffset만큼 이동
void MTrsTabCarrier::SetTabNo(int iRealTabNo)
{
	m_iWorkTabNo = iRealTabNo;
}

// NSMC
BOOL MTrsTabCarrier::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsTabCarrier::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

void MTrsTabCarrier::registerReload()
{
	m_csLock[m_iWorkGroup].Lock();
	if (!m_sReloadInfo[m_iWorkGroup].bRegistered)
	{
		m_sReloadInfo[m_iWorkGroup].iInstanceNo = m_iWorkInstance;
		m_sReloadInfo[m_iWorkGroup].bRegistered = TRUE;
	}
	m_csLock[m_iWorkGroup].Unlock();
}

void MTrsTabCarrier::freeReload()
{
	m_csLock[m_iWorkGroup].Lock();
	m_sReloadInfo[m_iWorkGroup].bRegistered = FALSE;
	m_sReloadInfo[m_iWorkGroup].iInstanceNo = -1;
	m_csLock[m_iWorkGroup].Unlock();
}

BOOL MTrsTabCarrier::isExistReload()
{
	m_csLock[m_iWorkGroup].Lock();
	BOOL bRegistered = m_sReloadInfo[m_iWorkGroup].bRegistered;
	if (m_sReloadInfo[m_iWorkGroup].iInstanceNo <= m_iWorkInstance)
		bRegistered = FALSE;
	m_csLock[m_iWorkGroup].Unlock();
	return bRegistered;
}

int MTrsTabCarrier::escapeForReload()
{
/*********************************************************
	Next(-)<-----			  ----->	Prev(+)										  (+)Prev <-----			  -----> Next(-)
	Carrier1	Carrier2	Carrier3	Carrier4	||	Tool1	Tool2 ... Tool8    ||  Carrier4		Carrier3	Carrier2	Carrier1

*********************************************************/
	int iResult = ERR_TRS_TABCARRIER_SUCCESS;
	if (FALSE == isExistReload())
		return ERR_TRS_TABCARRIER_SUCCESS;
	int MyInstanceNo = m_iWorkInstance;
	if (m_sReloadInfo[m_iWorkGroup].iInstanceNo > MyInstanceNo)	//PrevUnit
	{
		if (FALSE == m_TimerError.IsTimerStarted())
			m_TimerError.StartTimer();
		if (m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_READY_POS) == 0)
		{
			m_TimerError.StopTimer();
			iResult = m_plnkCtrlTabCarrier->MoveReadyPos();
			if (iResult)
				return iResult;
		}
		else
		{
			if (m_TimerError.MoreThan(30.0))
			{
				m_TimerError.StopTimer();
				return generateErrorCode(417102);
			}
		}
	}
	/*
	else if (m_sReloadInfo.iInstanceNo < MyInstanceNo)	//NextUnit
	{
		if (GetStep() == TABCARRIER_WAIT_JOB || GetStep() == TABCARRIER_MOVE_READY || GetStep() == TABCARRIER_WAIT_CYCLE_JOB_END)
		{
			if (m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_UNLOAD_POS) == 0)
			{
				iResult = m_plnkCtrlTabCarrier->MoveUnloadingPos();
				if (iResult)
					return iResult;
			}
		}
	}
	*/
	return ERR_TRS_TABCARRIER_SUCCESS;
}

int MTrsTabCarrier::GetTabMounterNo()
{
	int iInstanceNo = m_iWorkInstance;
	return iInstanceNo;
}

int MTrsTabCarrier::GetWorkTabICNo()
{
	return 0;
}

BOOL MTrsTabCarrier::isGroupStarter()
{
	if (m_iWorkInstance == GetGroupStarter())
		return TRUE;
	return FALSE;
}

BOOL MTrsTabCarrier::isAllLoadReady()
{
	BOOL bReady = TRUE;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		bReady &= m_bLoadReady[m_iWorkGroup][i];
	}
	return bReady;
}

BOOL MTrsTabCarrier::isAllUnloadComplete()
{
	BOOL bComplete = TRUE;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		bComplete &= m_bUnloadComplete[m_iWorkGroup][i];
	}
	return bComplete;
}

BOOL MTrsTabCarrier::isAllSupplyComplete()
{
	BOOL bComplete = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if (m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_ALL )
			bComplete = m_bGroupSupplyComplete[DEF_FRONT_GROUP] && m_bGroupSupplyComplete[DEF_REAR_GROUP];
		else
			bComplete = m_bGroupSupplyComplete[m_plnkSystemData->m_nUseFeeder];
	}
#else
	/*/
	bComplete = m_bGroupSupplyComplete[m_iWorkGroup];
//@#endif
	return bComplete;
}

BOOL MTrsTabCarrier::isAllLoadMoveComplete()
{
	BOOL bComplete = TRUE;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		bComplete &= m_bLoadMoveComplete[m_iWorkGroup][i];
	}
	return bComplete;
}
//SJ_YYK 150109 Add..
BOOL MTrsTabCarrier::isAllUnloadingReady()
{
	BOOL bReady = TRUE;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		bReady &= m_bUnloadingReady[m_iWorkGroup][i];
	}
	return bReady;
}

BOOL MTrsTabCarrier::is1FeederAllMounterInitStart()
{
	BOOL bInitStart = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
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

void MTrsTabCarrier::InitialWorkStep()
{
	if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount && FALSE == m_plnkCtrlTabFeeder[m_iWorkGroup]->GetFeederReady())
		setStep(TABCARRIER_WAIT_FEEDERREADY);
	else
		initializeStep();
}

int MTrsTabCarrier::GetGroupStarter()
{
	int iStarter = 0;
	iStarter = 0;
	return iStarter;
}

int MTrsTabCarrier::GetNextGroupStarter()
{
	int iNextStarter = GetGroupStarter();
	iNextStarter++;
	return iNextStarter;
}

//120116.kms__________
BOOL MTrsTabCarrier::IsMountAutoChangeReady1()
{
	return m_bMountAutoChangeReady[0];
}

BOOL MTrsTabCarrier::IsMountAutoChangeReady2()
{
	return m_bMountAutoChangeReady[1];
}
//@140311.KKY___________________________
int MTrsTabCarrier::moveAll_Unload_To_LoadReady()
{
	int iResult = ERR_TRS_TABCARRIER_SUCCESS;
//#ifdef DEF_GATE_SYSTEM
	MPos_XYT tgPos[DEF_MAX_TABCARRIER];
	MPos_XYT alignPos[DEF_MAX_TABCARRIER];
	double tgPosY[DEF_MAX_TABCARRIER];
	MTabCarrier* pTabCarrier[DEF_MAX_TABCARRIER];
	/*/
	ISercosMultiAxis* pMultiAxisY = NULL;
	if (m_iWorkGroup == DEF_FRONT_GROUP)
		pMultiAxisY = MOlbSystemPre.GetSercosMultiMotionComponent(DEF_AXIS_SERCOS_MULTI_F_TABCARRIER_Y);
	else
		pMultiAxisY = MOlbSystemPre.GetSercosMultiMotionComponent(DEF_AXIS_SERCOS_MULTI_R_TABCARRIER_Y);
	/*/

	int i = 0;
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
			pTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
		}
		else
		{
			pTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
		}

		//pTabCarrier[i]->InitAlignMarkOffset();

		tgPos[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
		tgPosY[i] = tgPos[i].dY;
		alignPos[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS);
	}

//@	/*/
	/*/
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->StartMove(tgPos[i].dX);
		if (iResult)
			return iResult;
	}

	//20140313 SJ_HJG MultiAxis로 이동시 이동속도 느려서 수정....
//H	iResult = pMultiAxisY->StartMove(tgPosY);
//H	if (iResult)
//H		return iResult;

	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		Sleep(100);
		iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->StartMove(tgPos[i].dY);
		if (iResult)
			return iResult;
	}
	//_____________________
	/*/
	
	//170203 SJ_YSH
	/*/
	for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
	{
		Sleep(50);
		//iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->StartMove(tgPos[i].dT);
		iResult = pTabCarrier[i]->MoveXYTPos(tgPos[i], DEF_TABCARRIER_READY_POS, FALSE, TRUE);
		if (iResult)
			return iResult;
	}
	/*/
	for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
	{
		Sleep(10);

		iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->StartMove(tgPos[i].dT);
		if (iResult)
			return iResult;

		iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->StartMove(tgPos[i].dY);
		if (iResult)
			return iResult;

		iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->StartMove(tgPos[i].dX);
		if (iResult)
			return iResult;
	}
	//_____________

	Sleep(50);
	
	/*/
	MPos_XYT CurPos;
	BOOL bMoveT[DEF_MAX_TABCARRIER] = {FALSE, FALSE, FALSE, FALSE};
	while(1)
	{
		Sleep(10);
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			if (TRUE == bMoveT[i]) 
				continue;
			CurPos = pTabCarrier[i]->GetCurrentXYTPos();
			if ((CurPos.dY < alignPos[i].dY - 10.0) || (fabs(CurPos.dX-tgPos[i].dX) < 1.0) ||
				pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsMotionDone())
			{
				iResult = pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->StartMove(tgPos[i].dT);
				if (iResult)
				{
					return iResult;
				}
				bMoveT[i] = TRUE;
			}
		}
		BOOL bAllMoveT = TRUE;
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			bAllMoveT &= bMoveT[i];
		}
		if (TRUE == bAllMoveT)
			break;
	}
	/*/

	/*/
	iResult = pTabCarrier[DEF_MAX_TABCARRIER-1]->Wait4DoneXYT();
	if (iResult)
	{
		return iResult;
	}

	//170203 SJ_YSH
	iResult = pTabCarrier[DEF_TABCARRIER3]->Wait4DoneXYT();
	if (iResult)
	{
		return iResult;
	}

	iResult = pTabCarrier[DEF_TABCARRIER2]->Wait4DoneXYT();
	if (iResult)
	{
		return iResult;
	}
	/*/

	iResult = pTabCarrier[DEF_TABCARRIER1]->Wait4DoneXYT();
	if (iResult)
	{
	
		return iResult;
	}
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		pTabCarrier[i]->InitAlignMarkOffset();
	//_____________
	
//#endif
	return ERR_TRS_TABCARRIER_SUCCESS;
}

void MTrsTabCarrier::SetTabICUnloadToTabMounter(int iGroupNo, int iWorkNo, BOOL Data)
{
	m_bTabICUnloadToTabMounter[iGroupNo][iWorkNo] = Data;
}
//@_____________________________________