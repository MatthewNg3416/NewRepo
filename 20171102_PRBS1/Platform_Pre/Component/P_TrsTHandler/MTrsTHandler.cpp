/* 
 * TrsTHandler Component
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
 * MTrsTHandler.cpp : Implementation of MTrsTHandler component.
 */

#include "Stdafx.h"
#include "MTrsTHandler.h"
#include "MTrsAutoManager.h"
#include "MCtrlHandler.h"
#include "MHandler.h"
#include "IStatePanelTransfer.h"
#include "DefSystem.h"
#include "MNetH.H"
#include "Common.h"
#include "MPlatformOlbSystem.h"

#include "DefIOAddrPreBonder.h"
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

extern MPlatformOlbSystem	MOlbSystemPre;

void MTrsTHandler::simulLog(CString Msg, int iStep,	int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	/*/수정요
	CString strLog;
	strLog.Format("%s[%d] : %s(%d)\n",GetObjectName(),GetInstanceNo(),Msg,iStep);

	FILE *fp = fopen( ".\\StepLog.txt", "a" );
//	fprintf(fp, "%s\n", strLog);
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
	/*/

#endif
}

/** RUN Step을 수행한다 */
void MTrsTHandler::doRunStep()
{
	int iResult;

	switch (m_estepCurrent)
	{
	/** 0. Loading 위치로 이동한다. */
	case STEP_UNLHANDLER_MOVE_TO_LOAD_POS:
		simulLog("STEP_UNLHANDLER_MOVE_TO_LOAD_POS", m_estepCurrent);
		// X축은 미리 Loading 위치로 가서 기다린다.
		m_LineTactTimeTimer.StartTimer();  // Equip Tact Timer Start

		iResult = m_plnkCtrlHandler->MoveForLoading();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		initializeITI();
		
		setStep(STEP_UNLHANDLER_GET_PANEL);
		break;
		
	/** 1. 워크벤치로부터 Panel을 가져온다. */
	case STEP_UNLHANDLER_GET_PANEL:
		simulLog("STEP_UNLHANDLER_GET_PANEL", m_estepCurrent);

		m_bIsLoadingReady = TRUE;

//		if(!m_plnkITIWorkBench->IsSendAbleToTHandler())
//			break;

		if(!m_plnkITITranferCenter->IsSendStartToTurnHandler())
			break;
		
		iResult = m_plnkHandler->CheckVacuum();
		if (iResult)
		{
			PROCESS_ALARM(generateErrorCode(411031));
			return;
		}

		iResult = m_plnkCtrlHandler->GetPanel();
		if (iResult)
		{
			if (m_plnkHandler->IsAbsorbPanel())
			{
//				m_plnkTrsAutoManager->TransferGlassData(DEF_WORKBENCH_PANEL, DEF_THandler_PANEL);
			}
			PROCESS_ALARM(iResult);
			break;
		}

//		m_plnkTrsAutoManager->TransferGlassData(DEF_WORKBENCH_PANEL, DEF_THandler_PANEL);

		//m_plnkTrsAutoManager->TransferGlassData(DEF_PANEL_TRANSFER_CENTER_PANEL, DEF_THANDLER_PANEL); //170123 SJ_YSH Del.

		m_bIsLoadingComplete = TRUE;

/*		while(1)
		{
			Sleep(10);
			if (FALSE == m_plnkITIWorkBench->IsSendAbleToTHandler())
				break;
		}
		*/

		while(TRUE)
		{
			Sleep(5);
			if(TRUE == m_plnkITITranferCenter->IsSendCompleteToTurnHandler())
				break;
		}

		m_bIsLoadingReady = FALSE;
		m_bIsLoadingComplete = FALSE;

		setStep(STEP_UNLHANDLER_MOVE_TO_UNLOAD_POS);
		break;

	/** 3. Unloading 위치로 이동한다. */
	case STEP_UNLHANDLER_MOVE_TO_UNLOAD_POS:
		iResult = m_plnkHandler->CheckVacuum();
		if (iResult)
		{
			PROCESS_ALARM(generateErrorCode(411032));
			return;
		}

		if(m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			if (TRUE != m_plnkHandler->IsAbsorbPanel())
			{
				PROCESS_ALARM(generateErrorCode(411032));
				return;
			}
		}

//		if (TRUE == m_plnkTrsAutoManager->IsDoorOpenNextMC())
//			break;

		iResult = m_plnkCtrlHandler->MoveForUnloading();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		setStep(STEP_UNLHANDLER_PUT_PANEL);
		break;
		
	/** 4. Panel을 다음설비에 내려놓는다. */
	case STEP_UNLHANDLER_PUT_PANEL:
		simulLog("STEP_UNLHANDLER_PUT_PANEL", m_estepCurrent);

		iResult = m_plnkHandler->CheckVacuum();
		if (iResult)
		{
			PROCESS_ALARM(generateErrorCode(411032));
			return;
		}

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			if (FALSE == m_plnkHandler->IsAbsorbPanel())
			{
				PROCESS_ALARM(generateErrorCode(411033));
				break;
			}
		}

		if (FALSE == m_plnkHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS))
		{
	//		if (TRUE == m_plnkTrsAutoManager->IsDoorOpenNextMC())
	//			break;
			iResult = m_plnkCtrlHandler->MoveForUnloading();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}

		m_bIsUnloadingReady = TRUE;

		if(m_plnkITITranferIn->IsReceiveAbleToTurnHandler() == FALSE)
			break;

		if(m_plnkPanelTransferIn->IsPanelAbsorbed() == TRUE)
			break;

		iResult = m_plnkCtrlHandler->PutPanel();
		if (iResult)
		{
			m_bIsUnloadingReady = FALSE;
			PROCESS_ALARM(iResult);
			break;
		}
		
		m_plnkTrsAutoManager->TransferGlassData(DEF_THANDLER_PANEL, DEF_PANEL_TRANSFER_PANEL);

		m_bIsUnloadingComplete = TRUE;

		while(TRUE)
		{
			Sleep(10);
			if(TRUE == m_plnkITITranferIn->IsReceiveCompleteToTurnHandler())
				break;
		}

		initializeITI();

		setStep(STEP_UNLHANDLER_MOVE_TO_LOAD_POS);
		break;
	}
}

UINT MTrsTHandler::threadFunction(LPVOID pParam)
{
	MTrsTHandler* pThis;
	pThis = (MTrsTHandler*) pParam;

	TRACE(_T("MTrsTHandler Thread Run\n"));

	while (pThis->m_flgThreadLife)
	{
		pThis->threadJob();
		Sleep(20);
	}

	TRACE(_T("MTrsTHandler Thread Stop\n"));

	return 0;		
}

void MTrsTHandler::initializeITI()
{
	m_bIsLoadingReady	= FALSE;
	m_bIsLoadingComplete	= FALSE;

	m_bIsUnloadingReady	= FALSE;
	m_bIsUnloadingComplete = FALSE;
}

/** Step Thread Step */
void MTrsTHandler::setStep(EStepTHandler EStepTHandler)
{
	m_estepPrevious = m_estepCurrent;	// 이전 Step 저장
	m_estepCurrent = EStepTHandler;		// 새로 Step으로 전환
}

void MTrsTHandler::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (EStepTHandler)0;	//  이전 Step Clear
	m_estepCurrent	= (EStepTHandler)0;	//  현재 Step Clear
}

/**
 * 동시 동작을 위해 StartMove 시킨다.
 *
 * @return	int (0 = Success, Error Code = 그 외)
 */
/*
int MTrsTHandler::startMoveXPos(int iTargetPos)
{
	/*20060509 ysb
#ifdef SIMULATION
	return ERR_TRSTHandler_SUCCESS;
#endif

	IAxis* plnkMotion_X = m_plnkHandler->GetMotionComponent(DEF_HANDLER_X_AXIS);

	double dTaretPos = m_plnkHandler->GetXTargetPos(iTargetPos);

	int iResult = plnkMotion_X->StartMove(dTaretPos, NULL, NULL, NULL, DEF_SMOVE_POSITION);
	if (iResult)
	{
		WriteErrorLog("Move Handler X Axis Fail");	
		return iResult;
	}

	Sleep(50);

	if (plnkMotion_X->GetAmpFault())
	{
		WriteErrorLog("Handler X AMP Fault");	
		return generateErrorCode(411003);	// ERR_TRSLHANDLER_X_AMP_FAULT
	}


	return ERR_TRSTHandler_SUCCESS;
}
*/
/** 이동 완료 확인 */
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

/** 생성자
 * @param commonData		: Component의 CommonData
 * @param listRefComponent	: 참조할 Component 포인터 리스트
 * @param datComponent		: Component 설정 데이타
 */
MTrsTHandler::MTrsTHandler(SCommonAttribute commonData,
						   STrsTHandlerRefCompList listRefComponents,
						   STrsTHandlerData datComponent )
: ICommon(commonData)
{
	m_flgThreadLife				= FALSE;
	m_pThread					= NULL;
	m_flgAutoRun				= FALSE;
	m_bAutoInitialized			= FALSE;
	m_bNextjob					= FALSE;
	m_iCurrentJobID				= NULL;
	initializeITI();

	SetData(datComponent);
	AssignComponents(listRefComponents);

	initializeStep();

	m_strOutPanelID.Empty();

#ifdef SIMULATION
	m_iPreStep					= -1;
#endif
}

MTrsTHandler::~MTrsTHandler()
{
}

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

/**
* Assign component linkage
* 
* @param	listRefComponents	: MTrsTHandler 참조 Component list
*/
int MTrsTHandler::AssignComponents(STrsTHandlerRefCompList listRefComponents)
{
	m_plnkCtrlHandler			= listRefComponents.m_plnkCtrlHandler;
	m_plnkHandler				= listRefComponents.m_plnkHandler;
	m_plnkMelsecIF				= listRefComponents.m_plnkMelsecIF;
	m_plnkPanelTransferCenter	= listRefComponents.m_plnkPanelTransferCenter;
	m_plnkPanelTransferIn		= listRefComponents.m_plnkPanelTransferIn;
	return ERR_TRSTHandler_SUCCESS;
}

/** 
* Component Data 처리 
*
* @param	pData : SMMTrsTHandlerData 구조체에 대한 포인터
* @return	Error : 0 = Success, 그외=기타
*/
int MTrsTHandler::SetData(STrsTHandlerData datComponent)
{
	m_plnkSystemData		= datComponent.m_plnkSystemData;
	m_plnkPanelData			= datComponent.m_plnkPanelData;
	m_plnkProductInfoData	= datComponent.m_plnkProductInfoData;
	return ERR_TRSTHandler_SUCCESS;
}

/** 
* Component Data 처리 
*
* @param	pData : SDrawerData 구조체에 대한 포인터
* @return	Error : 0 = Success, 그외=기타
*/
void MTrsTHandler::GetData(STrsTHandlerData *datComponent)
{
	datComponent->m_plnkSystemData		= m_plnkSystemData;
	datComponent->m_plnkPanelData		= m_plnkPanelData;
	datComponent->m_plnkProductInfoData	= m_plnkProductInfoData;
}

int MTrsTHandler::Initialize()
{
	m_bAutoInitialized				= FALSE;

// 1. 항상 실행해야 하는 초기화
	initializeStep();
	initializeITI();

#ifndef SIMULATION
	/** Handler Object가 할당되어 있지 않은 경우 Error Code Return */
	if (m_plnkHandler == NULL)
	{
		// 411004 = 자동운전 초기화에 실패 했습니다. [Mechanical Handler Object 가 할당되지 않음.]
//@		WriteErrorLog("Initialize(): 자동운전 초기화에 실패 했습니다. [Mechanical Handler Object 가 할당되지 않음.]");
		return generateErrorCode(411004);
	}

	/** 원점복귀가 되어 있지 않은 경우 Error Code Return */
	int iResult = m_plnkHandler->CheckXYTOrigin();
	if (iResult)
		return iResult;

// 2. Vacuum 상태 체크
/*	if (m_plnkHandler->IsPanelDetected() && !m_plnkHandler->IsAbsorbPanel())
	{
		// 411003 = Panel 감지되나, 흡착하고 있지 않습니다.
		WriteErrorLog("Initialize():Handler 에 Panel 감지되나, 흡착하고 있지 않습니다.");
		return generateErrorCode(411003);
	}
*/
	/** Vaccuum 이 정상적으로 동작하는지 Check */
	//iResult = m_plnkHandler->CheckVacuum();
//@___________________________________________________
	//170425 임시로 수정 
	if(!m_plnkHandler->IsAbsorbPanel())
	{
		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_TURN_HANDLER_VACUUM1);
		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_TURN_HANDLER_VACUUM2);
		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_TURN_HANDLER_VACUUM3);
	}
//@___________________________________________________	
	//if (iResult)
	//{
		
		/*/
		iResult = MyMessageBox("Vacuum operation is abnormal. Release the vacuum?\r\n"
			"Vacuum 동작이 이상 합니다.Vacuum을 해제 하시겠습니까?","Check Vacuum",M_ICONQUESTION|M_YESNO);
		if (iResult == IDYES)
		{
			m_plnkHandler->ReleasePanel(FALSE);  // Vacuum Compare Error 방지
			Sleep(100);
			iResult = m_plnkHandler->CheckVacuum();
			if (iResult)
				return iResult;
			
		}
		else
		{			
			m_plnkHandler->AbsorbPanel(FALSE);  // Vacuum Compare Error 방지
			Sleep(100);
			iResult = m_plnkHandler->CheckVacuum();
			if (iResult)
				return iResult;
		}
		/*/
//@		CString strError;
//@		strError = MOlbSystemPre.GetErrorMessage(iResult);
//@		MyMessageBox(strError.GetBuffer(strError.GetLength()), "Door Open Error", M_ICONERROR);
		//return iResult;
	//}

	/** 3-1. Actuator 초기화 */
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if (iResult)
		return iResult;

	iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_LOAD_POS);
	if (iResult)
		return iResult;
	
	//170123 SJ_YSH
	// Panel ID가 있을때
	if (m_plnkTrsAutoManager->m_pGlassData[DEF_THANDLER_PANEL]->m_strEPanelID.GetLength() > 1)		
	{
		if(m_plnkHandler->IsAbsorbPanel() == FALSE)
		{
//			if (MyMessageBox(_T("PanelTransferIn has a Panel ID but released a vacuum. Scrap(delete) the Panel ID?\r\nPanel Scrap"), _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
			if (MyMessageBox(_T("Turn Handler is Vacuum Off, Panel ID is present. Do you want Scrap?\r\nTurn Handler가 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?\r\nPanel Scrap"), _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_THANDLER_PANEL);	
		}
	}
	//_____________

	/** 3-3. Step 초기화 */
	// 060824 kjw Vacuum 2개중 1개가 Off일 경우 Error 처리
	if (m_plnkHandler->IsReleasePanel())
	{
		setStep(STEP_UNLHANDLER_MOVE_TO_LOAD_POS);
	}
	else
	{
		iResult = m_plnkHandler->AbsorbPanel();
		if (iResult)
			return iResult;

		setStep(STEP_UNLHANDLER_MOVE_TO_UNLOAD_POS);
	}

	
#endif

	m_bAutoInitialized = TRUE;
	return ERR_TRSTHandler_SUCCESS; 
}

void MTrsTHandler::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 


void MTrsTHandler::AssignITI(ITI_PanelTransferCenter* plnkITITranferCenter, ITI_PanelTransfer* plnkITITranferIn)
{
	m_plnkITITranferCenter = plnkITITranferCenter;
	m_plnkITITranferIn	= plnkITITranferIn;
}

/**
* Auto Manager를 Assign 한다.
* @param	pAutoManager : 설정할 AutoManager
*/
int MTrsTHandler::AssignAutoManager(MTrsAutoManager* pAutoManager)
{
	m_plnkTrsAutoManager = pAutoManager;

	return ERR_TRSTHandler_SUCCESS;
}

/**
 *	AutoView Handle 을 Assign 한다.
 */
void MTrsTHandler::SetWindowHandle(HWND hWnd)
{
	m_hAutoViewWnd = hWnd;
}

void MTrsTHandler::SetLogViewHandle(HWND hWnd)
{
	m_hLogViewWnd = hWnd;
}

void MTrsTHandler::SetTitleViewHandle(HWND hWnd)
{
	m_hTitleForm = hWnd;
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

void MTrsTHandler::ThreadRun()
{
	if (m_flgThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_flgThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsTHandler::ThreadStop()
{
	DWORD exitcode;

	m_flgThreadLife = FALSE;
	Sleep(50);

	if (m_pThread != NULL)
	{
		exitcode = ::WaitForSingleObject(m_pThread->m_hThread, 10000);

		if (exitcode == WAIT_TIMEOUT)
			::TerminateThread(m_pThread->m_hThread, exitcode);

		m_pThread = NULL;
	}
}

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 

/**
 *	Unload Handler 에 Panel 이 있는지 여부 반환
 */
BOOL MTrsTHandler::IsEmptyPanel()
{
	return m_plnkHandler->IsReleasePanel();
}

void MTrsTHandler::threadJob()
{
	switch (m_plnkTrsAutoManager->GetOPMode())
	{
	case MANUAL_MODE:
		m_flgAutoRun = FALSE;
		break;
	case AUTORUN_MODE:
		switch (m_plnkTrsAutoManager->GetOPStatus())
		{
		case STEP_STOP:
		case ERROR_STOP:			
			m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);

//			initializeITI();
			m_LineTactTimeTimer.StopTimer();

			m_flgAutoRun = FALSE;
			break;
		case START_RUN:
			m_LineTactTimeTimer.StopTimer();
			m_flgAutoRun = TRUE;
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

// NSMC
BOOL MTrsTHandler::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}
//170123 SJ_YSH
int MTrsTHandler::ScrapPanel()
{
	int iResult = SUCCESS;

	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_THANDLER_PANEL);

	if (TRUE == m_plnkHandler->IsAbsorbPanel())
	{
		CString strMsg = _T("There is a panel on Turn Handler. Remove it.");
		
		do
		{
			iResult = m_plnkHandler->ReleasePanel();
			if (iResult)
				return iResult;
			
			Sleep(500);

			if(MyMessageBox(MY_MESSAGE_BOX, 62600, _T("Question Scrap"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) ==IDNO)
				return FALSE;

//			MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Remove Panel Request", M_ICONINFORMATION);
			MyMessageBox(MY_MESSAGE_BOX, 62100, "Remove Panel Request", M_ICONINFORMATION);

			m_plnkHandler->AbsorbPanel(TRUE);
			Sleep(300);

		}while (TRUE == m_plnkHandler->IsAbsorbPanel());
	}
	
	m_plnkHandler->ReleasePanel(TRUE);

	return Initialize();
}
//_____________
/* End Of Code */
