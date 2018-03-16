/* 
 * TrsPanelAligner Component
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
 * MTrsPanelAligner.cpp : Implementation of MTrsPanelAligner component.
 */

#include "Stdafx.h"
#include "IIO.h"
#include "MNetH.h"
#include "MProductInfoData.h"
#include "MPanelData.h"
#include "MSystemData.h"
//#include "IStateRollerConveyor.h"
#include "IStatePanelTransfer.h"
#include "DefPanelTransfer.h"
#include "MPanelAligner.h"
#include "MModelChange.h"
#include "MCtrlPanelAligner.h"
#include "MTrsPanelAligner.h"
#include "MTrsAutoManager.h"
#include "ITI_UnloadConveyor.h"
#include "ITI_PanelTransfer.h"
#include "ITI_PanelTransferOut.h"
#include "ITI_InspectionCamera.h"
#include "ITI_TabFeeder.h"
#include <math.h>
#include "common.h"
#include "IVision.h"

#include "MProcessData.h"
#include "ITI_TabMounter.h"
#include "DefIOAddrPreBonder.h"
#include "IStateTabMounter.h"
#include "MPreBonderData.h"
#include "MCameraCarrier.h" // 2009.01.13 CYJ
#include "DefTabMounter.h" // 2009.02.19 CYJ

#include "MManageOpPanel.h"
#include "MPlatformOlbSystem.h"
#include "MPanelTransfer.h"

extern MPlatformOlbSystem	MOlbSystemPre;
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
int MTrsPanelAligner::m_iScheduleNo = 0;
int MTrsPanelAligner::m_bReceivedPressCompleteSchedule[] = {FALSE, FALSE};

MTrsPanelAligner::MTrsPanelAligner(SCommonAttribute	commonData, STrsPanelAlignerRefCompList listRefComponents,
								 STrsPanelAlignerData datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	m_bAutoInitialized = FALSE;

	m_bThreadLife = FALSE;
	m_pThread = NULL;
	m_bAutoRun = FALSE;

	m_dWaitTactTime = 0.0;
	m_strOutPanelID.Empty();

	initializeITI();
	initializeStep();

	m_bIsAirCVVacOnFlag = FALSE;

	m_bSkipCurrentPanelWork = FALSE;

	m_bScrapPanelReq = FALSE;
	m_bLastWorkGroup = FALSE;//100929.KKY
	m_bPanelInStop = FALSE;
	m_bTabInspectionTabBlobCheck = FALSE; //171004 JSh

	m_iScheduleNo = 0;

	m_ptManualMarkPos[0].x = 0;
	m_ptManualMarkPos[0].y = 0;
	m_ptManualMarkPos[1].x = 0;
	m_ptManualMarkPos[1].y = 0;
}

MTrsPanelAligner::~MTrsPanelAligner()
{
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
void MTrsPanelAligner::simulLog(CString strMsg, int iStep,	int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s[%d] : %s(%d)\n"), GetObjectName(), GetInstanceNo(), strMsg, iStep);

	FILE *fp = fopen( ".\\StepLog.txt", "a");
	fprintf(fp, "%s\n", LPSTR(LPCTSTR(strLog)));
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

/** Thread 함수*/
UINT MTrsPanelAligner::threadFunction(LPVOID pParam)
{
	// TrsPanelAligner Object Pointer Assign
	MTrsPanelAligner* pThis = (MTrsPanelAligner*) pParam;

	TRACE(_T("MTrsPanelAligner Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsPanelAligner Thread Stop\n"));

	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

void MTrsPanelAligner::threadJob()
{
	switch (m_plnkTrsAutoManager->GetOPMode())
	{
	case MANUAL_MODE:
		m_bAutoRun = FALSE;

		if (TRUE == m_bScrapPanelReq)
			ScrapPanel();

		break;

	case AUTORUN_MODE:
		switch (m_plnkTrsAutoManager->GetOPStatus())
		{
		case ERROR_STOP:
		case STEP_STOP:
			m_bITIReceiveAbleToPanelTransfer = FALSE;
			m_bITISendStartToPanelTransferOut = FALSE;
	#ifdef DEF_GATE_SYSTEM
			initializeMelsecIF();
	#endif
			m_bAutoRun = FALSE;
			break;

		case START_RUN:
			//m_EqLineTactTimer.StopTimer();
			m_EqTactWaitTimer.StopTimer();
			m_dWaitTactTime = 0.0;
	#ifdef DEF_GATE_SYSTEM
			initializeMelsecIF();
	#endif
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

void MTrsPanelAligner::doRunStep()
{
	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;
	BOOL bPreBonderMoreWork = FALSE;	
	int iPosID = DEF_PANEL_ALIGNER_PRESS_1_POS;
	int i = 0;
	BOOL bUsePusher = FALSE;

	switch (m_estepCurrent)
	{
	case PANEL_ALIGNER_MOVE_TO_LOAD_POS: simulLog("PANEL_ALIGNER_MOVE_TO_LOAD_POS", m_estepCurrent, 500);
		WriteTactTimeLog(" [MPanelAligner] [M] [New Cycle]");
		
		if (m_plnkPanelAligner->IsPanelAbsorbed())
		{
			PROCESS_ALARM(generateErrorCode(410011));
			break;
		}

		m_bDryRunPanelExist = FALSE;

		/*/
		m_bITIPanelTransferEnabletoMoveUnloadPos = FALSE;
		//TransferOut에 Panel이 없거나, TransferOut이 Unloading위치에 있으면 TransferIn이 들어와도 좋다...
		if (!m_plnkStatePanelTransferOut->IsPanelAbsorbed()
//			|| FALSE == m_plnkStatePanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
			|| (m_plnkStatePanelTransferOut->GetXCurrentPos() > m_plnkStatePanelTransferOut->GetXTargetPos(DEF_PANEL_TRANSFER_LOAD_POS)+400.0))
		{
			m_bITIPanelTransferEnabletoMoveUnloadPos = TRUE;
		}
		//________
		/*/
//170913 JSH.s Del...
// 		if (FALSE == m_plnkStatePanelTransferOut->IsPanelAbsorbed() && TRUE == m_plnkITIPanelTransferOut->IsReceiveAbleToPanelAligner())
// 		{
// 			m_bITIPanelTransferEnabletoMoveUnloadPos = TRUE;
// 		}
// 		else
// 			m_bITIPanelTransferEnabletoMoveUnloadPos = FALSE;
//170913 JSH.e

		//판넬 ID 없어지는 문제로 인해 17 보고 먼저 되어 ...
		//시점 이동
		m_bPanelAlignerUnloadingPanel = FALSE;

		if (FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_LOAD_POS))
		{
			iResult = m_plnkCtrlPanelAligner->MovetoLoadPos();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		// Pre Online
		//100826.KMS
//		m_bPanelAlignerUnloadingPanel = FALSE;

		setStep(PANEL_ALIGNER_RECEIVE_PANEL);
		break;

	case PANEL_ALIGNER_RECEIVE_PANEL:

		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(PANEL_ALIGNER_MODEL_CHANGE);
			break;
		}

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________

		if (m_plnkPanelAligner->IsPanelAbsorbed())
		{
			PROCESS_ALARM(generateErrorCode(410027));
			break;
		}


		//170913 JSh
		m_bITIPanelTransferEnabletoMoveUnloadPos = TRUE;
		m_bITIReceiveAbleToPanelTransfer = TRUE;


		if (FALSE == m_plnkITIPanelTransfer->IsSendStartToPanelAligner())
		{
			// 정체 상태일 때의 대기시간 EQ Tact Time 에서 제외
			if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
				m_EqTactWaitTimer.StartTimer();
			break;
		}

		if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
		{
			m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
			m_EqTactWaitTimer.StopTimer();
		}

		// 확인 필요...
		ASSERT(FALSE == m_EqTactWaitTimer.IsTimerStarted());

		simulLog("PANEL_ALIGNER_RECEIVE_PANEL", m_estepCurrent, 500);

		m_bITIReceiveStartToPanelTransfer = TRUE;

		// selsk_jdy : Panel 물류 동작 동작 시점 명확히 하기 위해서 아래에서 위치 이동.
		m_bITIPanelTransferEnabletoMoveUnloadPos = FALSE;

		while (TRUE)
		{
			Sleep(10);
			if (TRUE == m_plnkITIPanelTransfer->IsSendCompleteToPanelAligner())
				break;

			if (FALSE == m_plnkITIPanelTransfer->IsSendStartToPanelAligner())
			{
				m_bITIReceiveAbleToPanelTransfer = FALSE;
				m_bITIReceiveStartToPanelTransfer = FALSE;
				return;
			}
		}

		m_bITIReceiveAbleToPanelTransfer = FALSE;
		m_bITIReceiveStartToPanelTransfer = FALSE;


		if ((m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
			|| (m_plnkSystemData->m_eRunMode != PASS_RUN_MODE && FALSE == m_bSkipCurrentPanelWork))
		{
			for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
			{
				//@130114________________
				/*/
				if (FALSE == m_plnkWorkSchedule[i][0]->GetExistWorkPanel())
				{
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
					m_plnkWorkSchedule[i][0]->SetExistWorkPanel(TRUE);
					m_plnkWorkSchedule[i][1]->SetExistWorkPanel(TRUE);
				}
				/*/
				if (FALSE == m_plnkWorkSchedule[i][0]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
				if (FALSE == m_plnkWorkSchedule[i][1]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
				//@______________________
			}
			m_plnkWorkSchedule[0][0]->SetExistWorkPanel(TRUE);	//@
		}		
	

//		SetWorkScheduleNo(0); //170719 JSH Del...s
		m_bDryRunPanelExist = TRUE;
		//SJ_YYK 150318 Add...

		if(m_plnkTrsAutoManager->m_bAutoAlignDataUpdate == TRUE)
		{
			m_plnkCtrlPanelAligner->UpdateAlignData();
			m_plnkTrsAutoManager->m_bAutoAlignDataUpdate = FALSE;
		}
		m_EqTactTimer.StartTimer();
		//__________________________________*/
		setStep(PANEL_ALIGNER_MOVE_TO_ALIGN_POS);
		break;

	case PANEL_ALIGNER_MOVE_TO_ALIGN_POS: simulLog("PANEL_ALIGNER_MOVE_TO_ALIGN_POS", m_estepCurrent, 500);


 		iResult = m_plnkPanelAligner->SpotCleanerOn();
 		if (iResult)
 		{
 			PROCESS_ALARM(iResult);
 			return;
 		}

		if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
		{
			m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
			m_EqTactWaitTimer.StopTimer();
		}
		//20170427 JSH.e
		//_______________H
		

		//SJ_YYK 110401 Add.. Panel Validation 기능 추가.. 
#if defined DEF_GATE_SYSTEM  //SJ_YYK 150109 Add..
//#ifdef DEF_GATE_SYSTEM
		// Panel Validation Check
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			if (m_plnkSystemData->m_bUsePanelValidation == TRUE)
			{
				iResult = m_plnkTrsAutoManager->CheckPanelValidation(DEF_PANEL_ALIGNER_PANEL);
				switch (iResult)
				{
				case ERR_NO_BATCHID_DATA:
					SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427011 = Panel에 Batch ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(410023));
					return;
				case ERR_NO_DEVICEID_DATA:
					SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427012 = Panel에 Device ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(410024));
					return;
				case ERR_NO_PANELID_DATA:
					SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427013 = Panel에 Panel ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(410025));
					return;
					/* 확인 필요 : 삭제 할까?
					case ERR_SAME_PANELID_IN_LAST_5TIME:
					// 427014 = Panel ID가 최근 생산된 5개의 패널중 하나와 일치합니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427014));
					return;
					*/
				case ERR_NO_PANEL_DATA:
					SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427015 = Panel에 이전 설비로부터 넘겨받은 ID Data가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(410026));
					return;
				}
			}
		}
#endif
		

		if (REPAIR_RUN_MODE == m_plnkSystemData->m_eRunMode && FALSE == m_bRepairRunContinue)
			break;

		if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE && FALSE == m_bSkipCurrentPanelWork)
		{

			for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
			{
				if (FALSE == m_plnkWorkSchedule[i][0]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
				if (FALSE == m_plnkWorkSchedule[i][1]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
				//@______________________________
			}			
			m_plnkWorkSchedule[0][0]->SetExistWorkPanel(TRUE);
		}



		

		//20170427 JSH.s
		if (FALSE == m_plnkStatePanelTransferOut->IsPanelReleased())
		{			
			if (FALSE == m_plnkStatePanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
			{
				// 정체 상태일 때의 대기시간 EQ Tact Time 에서 제외
				if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
					m_EqTactWaitTimer.StartTimer();
				break;
			}
		}

		if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
		{
			m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
			m_EqTactWaitTimer.StopTimer();
		}
		//20170427 JSH.e

//@#ifdef DEF_GATE_SYSTEM
		if (TRUE == m_plnkPrebonderData->m_bUseTabPanelAlign)
			iResult = moveModelChangeWorkPos(FALSE);
		else
			iResult = moveModelChangeAlignPos(FALSE);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
//@#endif

		iResult = m_plnkCtrlPanelAligner->MovetoAlignPos();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

//@#ifdef DEF_GATE_SYSTEM
		if (TRUE == m_plnkPrebonderData->m_bUseTabPanelAlign)
			iResult = waitModelChangeWorkPos();
		else
			iResult = waitModelChangeAlignPos();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
//@#endif
		
// 		//170724_KDH SpotCleaner 추가
 		iResult = m_plnkPanelAligner->SpotCleanerOff();
 		if (iResult)
 		{
 			PROCESS_ALARM(iResult);
 			return;
 		}
// 		//__________________

		//SJ_YYK 110414 Add.... Align Pass Mode
		if(m_plnkSystemData->m_bUseAlignPass == TRUE)
		{
			setStep(PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS);
			break;
		}
		//_________________________________________________*/
		
		setStep(PANEL_ALIGNER_ALIGN_PANEL);
		break;

	case PANEL_ALIGNER_ALIGN_PANEL: simulLog("PANEL_ALIGNER_ALIGN_PANEL", m_estepCurrent, 500);

		WriteTactTimeLog("[MTrsPanelAligner] [P] Thread [Start]");
		
		//121204.kms_______________
		//Melsec Word Data 에서 Comment 값이 OS 이면 Pass 물류 로 변환
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			if(m_plnkTrsAutoManager->GetAssignPassData(DEF_PANEL_ALIGNER_PANEL))
			{
				//130113.KMS___________ 
				//WM_Message 생성 하여 OpCall 동작 한다. Unload 동작 할때 "OS" 확인 하여 Buzzer OFF 한다.
				CString strEqCmd;
//				MOlbSystemPre.GetManageOpPanelComponent()->SetBuzzerMode(TRUE);
				strEqCmd.Format(_T("전 설비 에서 Comment 에 OLB PASS 가 적힌 Panel이 들어왔습니다. PASS 합니다..\nIt is income Panel written OLB PASS from Previous Equipment"));				
				m_plnkTrsAutoManager->SetOperatorCall(TRUE, strEqCmd);
//				m_plnkTrsAutoManager->SetOperatorCall(TRUE);
//				::SendMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_OPCALL_MSG, (WPARAM)(&strEqCmd), NULL);
				//_____________________
				setStep(PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS);
				break;
			}
		}
		//_________________________

		//120515.KMS______________
		m_plnkTrsAutoManager->SetAssemReport(TRUE);

		{ // 170913 변수사용위해 {}처리
			EMarkErrorTreat eReturn = eMarkErrorNone;
			
			iResult = m_plnkCtrlPanelAligner->AlignPanel();
			if (iResult != ERR_TRS_PANEL_ALIGNER_SUCCESS)
			{
				if (m_plnkSystemData->m_bUsePanelPusher != 0)
				{
					//Error발생시 Panel Pusher동작 후 재검사
					bUsePusher = FALSE;
					DWORD sTime = GetTickCount();
					while(1)
					{
						Sleep(20);
						//170913 JSh.s
						//if (GetTickCount() - sTime > 10000)
						//	break;
						if(m_plnkTrsAutoManager->GetOPStatus() != RUN)
							return;
						//170913 JSh.e
						if (TRUE == isAllTabMounterReadyToPusherDown())
						{
							bUsePusher = TRUE;
							break;
						}
					}
					if (TRUE == bUsePusher)
					{
						m_plnkStateTabMounter1->DownPusherCyl(TRUE);
						m_plnkStateTabMounter2->DownPusherCyl();
						Sleep(1000);
						iResult = m_plnkCtrlPanelAligner->AlignPanel();
						m_plnkStateTabMounter1->UpPusherCyl(TRUE);
						m_plnkStateTabMounter2->UpPusherCyl();
						Sleep(200);
					}
				}
			}
			
			if (iResult != ERR_TRS_PANEL_ALIGNER_SUCCESS)
			{
				// sesl_jdy
				// 310005 = Panel Fiducial Mark 인식에 실패했습니다.
				// 310006 = Panel Mark Auto Search에 실패했습니다.
				// 310008 = Panel Mark 를 인식할 수 없습니다. [PanelAlign Cam1 : Panel 깨짐, 이물 확인 필요] - Mark 오인식 대비해서 일단 포함.
				// 310009 = Panel Mark 를 인식할 수 없습니다. [PanelAlign Cam2 : Panel 깨짐, 이물 확인 필요] - Mark 오인식 대비해서 일단 포함.
				// 310010 = Panel Fiducial Mark 인식 실패 [PanelAlign Cam1]
				// 310011 = Panel Fiducial Mark 인식 실패 [PanelAlign Cam2]
				int iErrorCode = iResult / 1000;
				if (310005 == iErrorCode ||
					310006 == iErrorCode ||
					310008 == iErrorCode ||
					310009 == iErrorCode ||
					310010 == iErrorCode ||
					310011 == iErrorCode
					)
				{
					// 임시로 PanelMarkManualSearch 기능으로 사용.
					if (TRUE == m_plnkSystemData->m_bUseTabReelAutoChange)
					{
						::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MANUAL_SEARCH_REQUEST, (WPARAM)(&eReturn), NULL);
						if (eReturn == eMarkStop)
						{
							m_plnkTrsAutoManager->SetOPStatus(STEP_STOP);
							return;
						}
						else if (eReturn == eMarkRunManual)
						{
							m_plnkCtrlPanelAligner->AlignPanelSetPoint(m_ptManualMarkPos[0], m_ptManualMarkPos[1]);
						}
						else if (eReturn == eMarkRunRetry)
						{
							m_plnkPanelAligner->MoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
							// 								m_plnkPanelAligner->AirCVVacAllOn(DEF_NONE_WORK_SCHEDULE);
							Sleep(800);							
							m_plnkPanelAligner->ReleasePanel();
							m_plnkPanelAligner->DownPickUpUnit();
							m_plnkPanelAligner->InitAlignMarkOffset();
							m_plnkCtrlPanelAligner->MovetoAlignPos();
							m_plnkPanelAligner->UpPickUpUnit();							
							iResult = m_plnkPanelAligner->AbsorbPanel();
							if (iResult)
							{
								PROCESS_ALARM(iResult);
								break;
							}							
							m_plnkPanelAligner->AirCVBlowOn();
							Sleep(1000);
							return;
						}
						else if (eReturn == eMarkOS)
						{
							m_plnkTrsAutoManager->AssignPassData(DEF_PANEL_ALIGNER_PANEL);
							m_plnkTrsAutoManager->WriteMonitoringLog("Panel Mark Manual Search : Pass");							
							//Pass 시 Step 저 뒤로 보낸다.........
							//setStep(PANEL_ALIGNER_MOVE_TO_UNLOAD_POS);							
							setStep(PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS);
							return;
						}
					}
				}
				else
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
/*
			for (int i = 1; ; i++)
			{
		 		iResult = m_plnkCtrlPanelAligner->MovetoAlignPos();
				if (iResult != ERR_TRS_PANEL_ALIGNER_SUCCESS)
				{
					SetErrorLevel(_T("PANEL ALIGNER align Panel"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					return;
				}

				if (eReturn == eMarkRunManual)
				{
					MOlbSystemPre.GetVisionComponent()->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);
					MOlbSystemPre.GetVisionComponent()->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_2);
					MOlbSystemPre.GetVisionComponent()->Grab(DEF_PANEL_ALIGN_CAMERA_1);
					MOlbSystemPre.GetVisionComponent()->Grab(DEF_PANEL_ALIGN_CAMERA_2);
					break;
				}
				
				//@130204___________________
				if (TRUE == bUsePusher)
				{
					m_plnkStateTabMounter1->DownPusherCyl(TRUE);
					m_plnkStateTabMounter2->DownPusherCyl();
					Sleep(1000);
				}
				//@_________________________

				iResult = m_plnkCtrlPanelAligner->AlignPanel(FALSE);
				
				//@130204___________________
				if (TRUE == bUsePusher)
				{
					m_plnkStateTabMounter1->UpPusherCyl(TRUE);
					m_plnkStateTabMounter2->UpPusherCyl();
					Sleep(200);
				}
				//@_________________________
				
				if (iResult != ERR_TRS_PANEL_ALIGNER_SUCCESS)
				{
					SetErrorLevel(_T("PANEL ALIGNER align Panel"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					return;
				}
				
				if (i > m_plnkSystemData->m_iPanelAlignRetryLimit)
				{
					SetErrorLevel(_T("PANEL ALIGNER align Panel"),0, __FILE__, __LINE__);
					// 410005 = Align Panel 동작 제한 횟수 초과.
					PROCESS_ALARM(generateErrorCode(410005));
					return;
				}
				if (TRUE == m_plnkCtrlPanelAligner->CheckFiduMarkPos())
					break;
					
			}
			*/
		}

		if(m_bTabInspectionTabBlobCheck == TRUE)
		{
			m_bTabInspectionTabBlobCheck = FALSE;
			setStep(PANEL_ALIGNER_INSPECTION_TAB_BOLB_READY);
			break;
		}

		if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE
			|| TRUE == m_bSkipCurrentPanelWork)
		{
#ifdef DEF_GATE_SYSTEM
			iPosID = DEF_PANEL_ALIGNER_INSPECT_1_POS;
#else
			//iPosID = DEF_PANEL_ALIGNER_INSPECT_2_POS;
			iPosID = DEF_PANEL_ALIGNER_INSPECT_1_POS; //SJ_YYK 150109 Add..
#endif
			iResult = m_plnkCtrlPanelAligner->MovetoInspectPos(iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			setStep(PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS);
			break;
		}

//@#ifdef DEF_GATE_SYSTEM
		iResult = moveModelChangeWorkPos(FALSE);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			setStep(PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS);
			break;
		}
//@#endif
		
		setStep(PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS);
		break;

	case PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS:
		//PreBonder Inspection 위치로 이동
		simulLog("PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS", m_estepCurrent, 500);
		
//@#ifdef DEF_GATE_SYSTEM
		iResult = moveModelChangeWorkPos(FALSE);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
//@#endif

		//SJ_YYK 150109 Modify..
#ifdef DEF_USE_REVERSE_TAB_SIDE
		if (m_iScheduleNo == 0)
			iPosID = DEF_PANEL_ALIGNER_INSPECT_1_POS;
		else
			iPosID = DEF_PANEL_ALIGNER_INSPECT_2_POS;
#else
		if (m_iScheduleNo == 0)
			iPosID = DEF_PANEL_ALIGNER_INSPECT_2_POS;
		else
			iPosID = DEF_PANEL_ALIGNER_INSPECT_1_POS;
#endif

		//170920 JSH.s
		if (m_iScheduleNo != 0)
		{
			if (FALSE == m_plnkStatePanelTransferOut->IsPanelReleased()
				|| FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad()
				)
			{
				if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
					m_EqTactWaitTimer.StartTimer();
				break;
			}
			if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
			{
				m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
				m_EqTactWaitTimer.StopTimer();
			}
		}
		//170920 JSH.e

//@		if (FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS))
		if (FALSE == m_plnkPanelAligner->IsInXYTPos(iPosID))
		{
//			bPreBonderMoreWork = m_plnkProcessData->IsMoreWorkForCurrentPanel();
//			if (TRUE == bPreBonderMoreWork)
			{
				//Air부상 여부
				if (TRUE == m_bIsAirCVVacOnFlag || FALSE == m_plnkPanelAligner->IsAirCVVacAllOff())
				{
					m_plnkCtrlPanelAligner->CheckPlateBlowOn();

					m_bIsAirCVVacOnFlag = FALSE;
				}
				//작업구간에 있는 TabMounters상승 여부
				iResult = waitTabMounterAllUp();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}

//@			iResult = m_plnkCtrlPanelAligner->MovetoInspectPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS);
			iResult = m_plnkCtrlPanelAligner->MovetoInspectPos(iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
			
			//170316 SJ_YSH Del.
			/*/
			//170202 SJ_YSH
			if(iPosID == DEF_PANEL_ALIGNER_INSPECT_2_POS)
			{
				m_bITIPanelTransferEnabletoMoveUnloadPos = TRUE;
			}
			/*/

			/*/

			/*/
//____________________________
		}
		
//@#ifdef DEF_GATE_SYSTEM
		iResult = waitModelChangeWorkPos();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
//@#endif
		
		//Backup상승해도 좋다
		m_bITIInspectorEnabletoInspectMove = TRUE;

		if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE)
		{
			m_plnkPanelAligner->AirCVBlowOff();
			m_plnkPanelAligner->AirCVVacAllOn(m_iScheduleNo);
		}

		if (FALSE == m_plnkPanelAligner->IsInZPos(DEF_PANEL_ALIGNER_Z_WORK_POS))
		{
			iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
		// Inpection 실시 전에 Panel을 Plate가 잡도록 한다. 
		if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE)
		{
			if (FALSE == m_bIsAirCVVacOnFlag) // 2009.05.22 CYJ
			{
				iResult = waitPlateVacuumOn();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
				m_bIsAirCVVacOnFlag = TRUE;
			}
		}	


//		TRACE(_T("<<<<<TrsPanelAligner:Move To InspectionPos Complete>>>>>\n"));

		setStep(PANEL_ALIGNER_WAIT_PRE_INSPECTION_COMPLETE);
		break;

	case PANEL_ALIGNER_WAIT_PRE_INSPECTION_COMPLETE:
		//PreBonder Inspection 완료 대기
		//1.InspectionCamera에게 InspectorEnabletoInspect 신호보낸다.
		//2.TabMounter로부터 AllInspectCompleteToPA 신호대기
		//3.InspectorEnabletoInspect 신호 해제

		simulLog("PANEL_ALIGNER_WAIT_PRE_INSPECTION_COMPLETE", m_estepCurrent, 500);

		//110420.Add_____
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(PANEL_ALIGNER_MODEL_CHANGE);
			break;
		}
		//_______________

		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________

		m_bITIWorkerEnabletoWork = FALSE;
		//PanelAligner Inspection위치로 이동후 Inspection해도 좋다는 신호
		m_bITIInspectorEnabletoInspect = TRUE;
		
		if (isUseWorkGroup(DEF_FRONT_GROUP) && !m_plnkITITabMounter->IsAllInspectionCompleteToPA(DEF_FRONT_GROUP))
			break;
		if (isUseWorkGroup(DEF_REAR_GROUP) && !m_plnkITIRTabMounter->IsAllInspectionCompleteToPA(DEF_REAR_GROUP))
			break;
		
		m_bITIInspectorEnabletoInspect = FALSE;
		m_bITIInspectorEnabletoInspectMove = FALSE;

//@#ifdef DEF_SOURCE_SYSTEM
//@		m_bLastWorkGroup = m_plnkWorkSchedule[0]->IsLastWorkGroup();
//@#else
		m_bLastWorkGroup = TRUE;
		if (isUseWorkGroup(DEF_FRONT_GROUP))
			m_bLastWorkGroup &= m_plnkWorkSchedule[m_iScheduleNo][DEF_FRONT_GROUP]->IsLastWorkGroup();
		if (isUseWorkGroup(DEF_REAR_GROUP))
			m_bLastWorkGroup &= m_plnkWorkSchedule[m_iScheduleNo][DEF_REAR_GROUP]->IsLastWorkGroup();
//@#endif

		setStep(PANEL_ALIGNER_MOVE_TO_WORK_POS);
		break;

	case PANEL_ALIGNER_MOVE_TO_WORK_POS: simulLog("PANEL_ALIGNER_MOVE_TO_WORK_POS", m_estepCurrent, 500);
		//Worker (Mounter) 작업 위치로 이동
		// - Source : Press_Xu Pos
		// - Gate : Press_Yl(Yr) Pos

		// Test Mode : TabMounter 가 Inspection Offset 을 반영해서 Mount 위치로 이동한 후
		//             그 위치 확인을 위해서 자동운전을 진행하지 않고 대기한다.
//		if (TRUE == m_plnkSystemData->m_bUseTestStop) // 2009.01.23 CYJ UseTestStop 기능을 UsePanelPusher로 변경.
//			break;

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//작업구간에 있는 TabMounters상승 여부
		iResult = waitTabMounterAllUp();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		if (FALSE == m_plnkPanelAligner->IsInBackupMoveXPos(DEF_BACKUP_MOVE_X_WORK_POS))
		{
			iResult = m_plnkPanelAligner->SafeMoveBackupMoveXPos(DEF_BACKUP_MOVE_X_WORK_POS);
			if (iResult)
			{
				SetErrorLevel(_T("PANEL ALIGNER move to Work Position"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}

//		TRACE(_T("<<<<<TrsPanelAligner:Move To WorkPos Complete>>>>>\n"));
#endif
		/*/

		//Inspection 완료후 BackupMoveX축 작업위치로 이동후 Inspection Camera이동불가 신호
		m_bITIInspectorEnabletoInspectMove = FALSE;
		m_bITIWorkerEnabletoWork = TRUE;

		setStep(PANEL_ALIGNER_WAIT_WORK_COMPLETE);
		break;

	case PANEL_ALIGNER_WAIT_WORK_COMPLETE: simulLog("PANEL_ALIGNER_WAIT_WORK_COMPLETE", m_estepCurrent, 500);
		//Mounter 작업 완료 대기
		//1.Worker (Mounter)에게 WorkerEnableToWork 신호보낸다.
		//2.Worker (Mounter)로부터 Complete 신호대기 <- TimeOut Check
		//3.WorkerEnableToWork 신호 해제
		
		m_bITIWorkerEnabletoWork = TRUE;
		if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE)
		{
			if (FALSE == m_bIsAirCVVacOnFlag)
			{
				iResult = waitPlateVacuumOn();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}

				m_bIsAirCVVacOnFlag = TRUE;
			}
		}

		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY);
			break;
		}

		// 현재 작업에 대한 Data Update 전 상황을 검사해야 한다.
//		bPreBonderMoreWork = m_plnkProcessData->IsMoreWorkForCurrentPanel();
		if (isUseWorkGroup(DEF_FRONT_GROUP))
		{
			if (FALSE == m_plnkITITabMounter->IsAllMountCompleteToPA(DEF_FRONT_GROUP) || FALSE == m_plnkITIInspectionCamera->IsAllMountCompleteToPA(DEF_FRONT_GROUP))
				break;
		}
		if (isUseWorkGroup(DEF_REAR_GROUP))
		{
			if (FALSE == m_plnkITIRTabMounter->IsAllMountCompleteToPA(DEF_REAR_GROUP) || FALSE == m_plnkITIRInspectionCamera->IsAllMountCompleteToPA(DEF_REAR_GROUP))
				break;
		}
		
		//작업구간에 있는 TabMounters상승 여부
		iResult = waitTabMounterAllUp();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		//___________________

		m_bITIInspectorEnabletoInspect = FALSE;
		m_bITIWorkerEnabletoWork = FALSE;


		//@________________________________________________
//@		/*/
		bPreBonderMoreWork = TRUE;
		if (isUseWorkGroup(DEF_FRONT_GROUP))
			bPreBonderMoreWork &= m_plnkWorkSchedule[m_iScheduleNo][DEF_FRONT_GROUP]->IsMoreWorkForCurrentPanel();
		if (isUseWorkGroup(DEF_REAR_GROUP))
			bPreBonderMoreWork &= m_plnkWorkSchedule[m_iScheduleNo][DEF_REAR_GROUP]->IsMoreWorkForCurrentPanel();

		// Source	: 현재 Panel 에 대해 부착해야할 Tab 이 더 있는 경우, Inspection Step 으로 돌아간다.
		// Gate		: 현재 Panel 에 대해 부착해야할 Tab 이 더 있는 경우, 현재 Step 에서 계속 대기.
		if (TRUE == bPreBonderMoreWork)
		{
			//1개 Group MountPress후 Inspection Camera이동해도 좋다는 신호
			m_bITIInspectorEnabletoInspectMove = TRUE;
			setStep(PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS);
			break;
		}
		
		//@_________________________
//@		/*/

		/*/
		bPreBonderMoreWork = FALSE;
		if (TRUE == m_plnkWorkSchedule[m_iScheduleNo-1][DEF_FRONT_GROUP]->GetUseWorkSide())
		{
			if (FALSE == m_plnkITITabMounter->IsPressCompleteSchedule(DEF_FRONT_GROUP))
			{
				bPreBonderMoreWork = TRUE;
			}
		}
		if (TRUE == m_plnkWorkSchedule[m_iScheduleNo-1][DEF_REAR_GROUP]->GetUseWorkSide())
		{
			if (FALSE == m_plnkITITabMounter->IsPressCompleteSchedule(DEF_REAR_GROUP))
			{
				bPreBonderMoreWork |= TRUE;
			}
		}

		if (TRUE == bPreBonderMoreWork)
		{
			//1개 Group MountPress후 Inspection Camera이동해도 좋다는 신호
			m_bITIInspectorEnabletoInspectMove = TRUE;
			setStep(PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS);
			break;
		}
		/*/
		m_bReceivedPressCompleteSchedule[DEF_FRONT_GROUP] = 
		m_bReceivedPressCompleteSchedule[DEF_REAR_GROUP] = TRUE;
		//@________________________________________________
		m_iScheduleNo++;

		//170719_KDH 작업할꺼 없으면 Skip Test중.
		m_bLastWorkGroup = TRUE;
		for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
		{
			MListWorkTab ListWorkTab;
			if (isUseWorkGroup(DEF_FRONT_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if (isUseWorkGroup(DEF_REAR_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if(m_bLastWorkGroup == FALSE)
				break;
		}
		m_iScheduleNo = i;
		//_______________________


		if (m_iScheduleNo < DEF_MAX_WORK_SCHEDULE) 
		{
			//@130114
//			if (FALSE == m_plnkWorkSchedule[m_iScheduleNo][0]->GetExistWorkPanel()
//				|| FALSE == m_plnkWorkSchedule[m_iScheduleNo][1]->GetExistWorkPanel())
//			{
//				for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
//				{
//					m_plnkWorkSchedule[m_iScheduleNo][i]->GenerateWorkSchedule();
//					m_plnkWorkSchedule[m_iScheduleNo][i]->SetExistWorkPanel(TRUE);
//				}
//			}
			
			setStep(PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS);
			break;
			//_______________________
		}
		
		for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
		{
			m_plnkWorkSchedule[i][0]->ClearWorkSchedule();
			m_plnkWorkSchedule[i][1]->ClearWorkSchedule();
//@			m_plnkWorkSchedule[i][0]->SetExistWorkPanel(FALSE);
//@			m_plnkWorkSchedule[i][1]->SetExistWorkPanel(FALSE);
		}
		m_plnkWorkSchedule[0][0]->SetExistWorkPanel(FALSE);
	//	m_iScheduleNo = 0; //170719 JSH
		
		//170719_KDH 작업할꺼 없으면 Skip Test중.
		m_iScheduleNo = 0;	//@
		m_bLastWorkGroup = TRUE;
		for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
		{
			MListWorkTab ListWorkTab;
			if (isUseWorkGroup(DEF_FRONT_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if (isUseWorkGroup(DEF_REAR_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if(m_bLastWorkGroup == FALSE)
				break;
		}
		m_iScheduleNo = i;
		//_______________________
		//@_________________________

		//전체 MountPress후 Inspection Camera이동불가 신호
		m_bITIInspectorEnabletoInspectMove = FALSE;

		setStep(PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS);
		break;

	case PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS: simulLog("PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS", m_estepCurrent, 500);
		// Unload 위치로 이동 대기
		// 다음설비 Load Conveyor에 Panel 있으면
		// - Unload 또는 Unload Wait 위치로 이동하면 충돌
		//1.다음설비(Load Conveyor)로부터 EnableToMoveToUnloadPos 신호 대기

	

	//	m_iScheduleNo = 0;//@ //170719 JSH

		// PanelTransferOut에 Panel 없는지 확인
//#ifdef DEF_GATE_SYSTEM
		if (FALSE == m_plnkITIPanelTransferOut->IsReceiveAbleToPanelAligner())
		{
			// selsk_jdy
			// 정체 상태일 때의 대기시간 EQ Tact Time 에서 제외
			if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
				m_EqTactWaitTimer.StartTimer();
			break;
		}
		if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
		{
			m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
			m_EqTactWaitTimer.StopTimer();
		}
//#endif

		if (FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_UNLOAD_POS))
		{
#ifdef DEF_SOURCE_SYSTEM
#	ifndef DEF_SETUP	//@확인요
			if (FALSE == m_plnkStatePanelTransferOut->IsPanelReleased()
				|| FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad()  //170916 JSH

//@				|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved4_UpperEqBit)
//@				|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved5_UpperEqBit)
				|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved1_UpperEqBit)	//@PreLoading위치에 패널감지된다
				)
			{
				if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
					m_EqTactWaitTimer.StartTimer();
				break;
			}
#	endif
			if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
			{
				m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
				m_EqTactWaitTimer.StopTimer();
			}
#endif
			m_plnkPanelAligner->AirCVVacAllOff();
			m_plnkPanelAligner->AirCVBlowOn();
			m_plnkCtrlPanelAligner->CheckPlateBlowOn();
			if (FALSE == m_plnkPanelAligner->IsAirCVBlowOn())
			{
				PROCESS_ALARM(generateErrorCode(410013));
				break;
			}
			DWORD sTime = GetTickCount();
			while(FALSE == m_plnkPanelAligner->IsAirCVVacAllOff())
			{
				Sleep(10);
				if (GetTickCount() - sTime > 3000)
					break;
			}

			m_bIsAirCVVacOnFlag = FALSE;

			iResult = m_plnkCtrlPanelAligner->MovetoUnloadPos();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
	

		//100930.KKY_____
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			//20170427 JSH.s
			if (FALSE == m_plnkStatePanelTransferOut->IsPanelAbsorbed())
			{
				m_bITISendStartToPanelTransferOut = TRUE;
				//20170909 KDW Start
				// 정체 상태일 때의 대기시간 EQ Tact Time 에서 제외
				if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
					m_EqTactWaitTimer.StartTimer();
				//20170909 KDW End
				break;
			}
			//20170427 JSH.e
			else
			{
				if (FALSE == m_plnkPanelAligner->IsPanelReleased())
					m_plnkPanelAligner->ReleasePanel();

				m_plnkPanelAligner->AirCVBlowOn();

				if (FALSE == m_plnkPanelAligner->IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
					m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);

				m_plnkCtrlPanelAligner->CheckPlateBlowOn();
				if (FALSE == m_plnkPanelAligner->IsAirCVBlowOn())
				{
					PROCESS_ALARM(generateErrorCode(410013));
					break;
				}
				m_bIsAirCVVacOnFlag = FALSE;
			}
			//20170427 JSH.s
			if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
			{
				m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
				m_EqTactWaitTimer.StopTimer();
			}
			//20170427 JSH.e
		}
		else
		{
			if (FALSE == m_plnkStatePanelTransferOut->IsUpPickUpUDCyl())
			{
				m_bITISendStartToPanelTransferOut = TRUE;
				//20170909 KDW Start
				// 정체 상태일 때의 대기시간 EQ Tact Time 에서 제외
				if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
					m_EqTactWaitTimer.StartTimer();
				//20170909 KDW End
				break;
			}
			else
			{
				if (FALSE == m_plnkPanelAligner->IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
				{
					Sleep(100);
					m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);
				}
			}
			//20170427 JSH.s
			if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
			{
				m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
				m_EqTactWaitTimer.StopTimer();
			}
			//20170427 JSH.e
		}
		
		//170913 JSH.s Del...
		//170316 SJ_YSH
		//Panel Aligner Unload 위치 이동 후 Panel Transfer In이 Unload로 진입 하도록.
		//m_bITIPanelTransferEnabletoMoveUnloadPos = TRUE;
		//170913 JSH.e
		

//@		/*/
		for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
		{
			m_plnkWorkSchedule[i][0]->ClearWorkSchedule();
			m_plnkWorkSchedule[i][1]->ClearWorkSchedule();
//@			m_plnkWorkSchedule[i][0]->SetExistWorkPanel(FALSE);
//@			m_plnkWorkSchedule[i][1]->SetExistWorkPanel(FALSE);
		}
		m_plnkWorkSchedule[0][0]->SetExistWorkPanel(FALSE);

#ifdef DEF_GATE_SYSTEM
		if (m_plnkTrsAutoManager->IsPanelExistInPrevMachine())
#else
		if (m_plnkTrsAutoManager->IsPanelExistInTransfer())
#endif
		{
			for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
			{
				//@_______________________
				/*/
				if (FALSE == m_plnkWorkSchedule[i][0]->GetExistWorkPanel()
					|| FALSE == m_plnkWorkSchedule[i][1]->GetExistWorkPanel())
				{
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
					m_plnkWorkSchedule[i][0]->SetExistWorkPanel(TRUE);
					m_plnkWorkSchedule[i][1]->SetExistWorkPanel(TRUE);
				}
				/*/
				if (FALSE == m_plnkWorkSchedule[i][0]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
				if (FALSE == m_plnkWorkSchedule[i][1]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
				//@_______________________
			}
			m_plnkWorkSchedule[0][0]->SetExistWorkPanel(TRUE);
		}

//@		/*/
//170427 JSH.s del..
/*
		if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
		{
			m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
			m_EqTactWaitTimer.StopTimer();
		}
*/
//170427 JSH.e

		//121204.kms_______________
		//Melsec Word Data 에서 Comment 값이 OS 이면 Pass 물류 로 변환
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			if(m_plnkTrsAutoManager->GetAssignPassData(DEF_PANEL_ALIGNER_PANEL))
			{
				//130113.KMS___________ 
				//WM_Message 생성 하여 OpCall 동작 한다. Unload 동작 할때 "OS" 확인 하여 Buzzer OFf 한다.
				CString strEqCmd;
//				MOlbSystemPre.GetManageOpPanelComponent()->SetBuzzerMode(FALSE);
				strEqCmd.Format(_T("PASS Complete 되었습니다. .\n PASS Complete."));				
				m_plnkTrsAutoManager->SetOperatorCall(FALSE, strEqCmd);
//				m_plnkTrsAutoManager->SetOperatorCall(FALSE);
//				::SendMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_OPCALL_MSG, (WPARAM)(&strEqCmd), NULL);
			}
		}
		//_________________________


//		m_plnkCtrlPanelAligner->LightingAlign(FALSE);
		setStep(PANEL_ALIGNER_MOVE_TO_UNLOAD_POS);
		break;

	case PANEL_ALIGNER_MOVE_TO_UNLOAD_POS: simulLog("PANEL_ALIGNER_MOVE_TO_UNLOAD_POS", m_estepCurrent, 500);
		//다음설비(Load Conveyor)로부터 EnableToMoveToUnloadPos 신호 받고 Unload 위치로 이동
		//1.Unload Pos로 이동

		/* selsk_jdy
		// 정체 상태일 때의 대기시간 EQ Tact Time 에서 제외
		m_dWaitTactTime = m_EqTactWaitTimer.GetElapseTime();
		m_EqTactWaitTimer.StopTimer();
		*/

		/*100930.KKY.Del_____
		//2009.11.11 CYJ Panel Aligner Unload & Panel Transfer Receive Panel 동시 동작
		iResult = m_plnkCtrlPanelAligner->MovetoUnloadPos();
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER move to Unload Position"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}
//		TRACE(_T("<<<<<TrsPanelAligner:Move To UnloadPos Complete>>>>>\n"));
		*/

		//100826.KMS
		m_bPanelAlignerUnloadingPanel = TRUE;


		//120324.kms__________
//@		if (m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_1)
		if (MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->GetUseWorkSide())
			m_plnkITITabFeeder[DEF_TABFEEDER1]->SetPanelAlignerUnloadingpanel();
//@		else if (m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_2)
		if (MOlbSystemPre.GetWorkSchedule(DEF_REAR_GROUP)->GetUseWorkSide())
			m_plnkITITabFeeder[DEF_TABFEEDER2]->SetPanelAlignerUnloadingpanel();
//@		else
//@		{
//@			m_plnkITITabFeeder[DEF_TABFEEDER1]->SetPanelAlignerUnloadingpanel();
//@			m_plnkITITabFeeder[DEF_TABFEEDER2]->SetPanelAlignerUnloadingpanel();
//@		}
		//____________________


		//SJ_YYK 150911 Add...
		if(MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->GetUseWorkSide())
			m_plnkTrsAutoManager->MatAOCAssembleReport(DEF_FRONT_GROUP);
		if(MOlbSystemPre.GetWorkSchedule(DEF_REAR_GROUP)->GetUseWorkSide())
			m_plnkTrsAutoManager->MatAOCAssembleReport(DEF_FRONT_GROUP);
		//_________________________________________*/

		// 091028 공정 Key Parameter 측정 완료 확인
		if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE
			|| m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
		{
//			if (FALSE == m_plnkTrsAutoManager->IsMeasureProcessData())
			{
				// 공정 Key Parameter 보고
				m_plnkTrsAutoManager->ProcessParamReport();
			//	TRACE(_T("Parameter Report"));
			}
		}
		//-----------------------------------------------------------//

//		m_bITIPanelTransferEnabletoMoveUnloadPos = isPanelTransferEnabledToMoveUnloadPos();

//		m_plnkWorkSchedule->ClearWorkSchedule();
//		m_plnkWorkSchedule->SetExistWorkPanel(FALSE);
//		TRACE(_T("<<<<<TrsPanelAligner:Clear WorkSchedule>>>>>\n"));

		setStep(PANEL_ALIGNER_SEND_PANEL);
		break;

	case PANEL_ALIGNER_SEND_PANEL: simulLog("PANEL_ALIGNER_SEND_PANEL", m_estepCurrent, 500);
		
#ifdef DEF_GATE_SYSTEM
		if (m_plnkTrsAutoManager->IsPanelExistInPrevMachine())
#else
		if (m_plnkTrsAutoManager->IsPanelExistInTransfer())
#endif
		{
			for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
			{
				/*/
				if (FALSE == m_plnkWorkSchedule[i][0]->GetExistWorkPanel())
				{
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
					m_plnkWorkSchedule[i][0]->SetExistWorkPanel(TRUE);
					m_plnkWorkSchedule[i][1]->SetExistWorkPanel(TRUE);
				}
				/*/
				if (FALSE == m_plnkWorkSchedule[i][0]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][0]->GenerateWorkSchedule();
				if (FALSE == m_plnkWorkSchedule[i][1]->IsGeneratedWorkSchedule())
					m_plnkWorkSchedule[i][1]->GenerateWorkSchedule();
			}
			m_plnkWorkSchedule[0][0]->SetExistWorkPanel(TRUE);
		}
		
		//120515.KMS______________
		//305 보고 하고 나서 Panel Out 되도록....
		//5sec 이상 대기 하면 잘못된 것으로 판단 
//@#ifdef DEF_SOURCE_SYSTEM
		/*
		if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE)
		{
			m_Assemtimer.StartTimer();
			while(TRUE)
			{
				if(m_plnkTrsAutoManager->GetAssemReport() == FALSE)
					break;

				if(m_Assemtimer.MoreThan(5.0))
				{
					TRACE(_T("Assem 보고 문제가 있어서 Break 했어요.\n"));
					break;
				}
			}
		}
//@#endif
		//________________________
		*/
		
		if (m_plnkITIPanelTransferOut->IsReceiveAbleToPanelAligner() == FALSE)
		{
			if (FALSE == m_EqTactWaitTimer.IsTimerStarted())
				m_EqTactWaitTimer.StartTimer();
			break;
		}

		if (TRUE == m_EqTactWaitTimer.IsTimerStarted())
		{
			m_dWaitTactTime += m_EqTactWaitTimer.GetElapseTime();
			m_EqTactWaitTimer.StopTimer();
		}

		m_bITISendStartToPanelTransferOut = TRUE;
		//171007 JSH.s
		{
			MTickTimer dTimeOut; 
			dTimeOut.StopTimer();
			while (TRUE)
			{
				if (TRUE == m_plnkITIPanelTransferOut->IsReceiveStartToPanelAligner())
				{				
					break;
				}

				if (FALSE == m_plnkITIPanelTransferOut->IsReceiveAbleToPanelAligner())
				{
					m_bITISendStartToPanelTransferOut = FALSE;
					return;
				}

				if (m_plnkTrsAutoManager->GetOPStatus() == ERROR_STOP
					|| m_plnkTrsAutoManager->GetOPStatus() == STEP_STOP)
				{
					if(dTimeOut.IsTimerStarted() == FALSE)
						dTimeOut.StartTimer();
					
					if(dTimeOut.MoreThan(30))
					{
						m_bITISendStartToPanelTransferOut = FALSE;
						return;
					}
				}
				Sleep(10);
			}
		}
		//171007 JSH.e

		m_strOutPanelID = m_plnkTrsAutoManager->GetGlassID(DEF_PANEL_ALIGNER_PANEL);

		if (FALSE == m_plnkPanelAligner->IsDownPickUpUnit())
		{
			iResult = m_plnkPanelAligner->DownPickUpUnit();
			if (iResult)
			{	
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
		//170913 JSh
		m_bITIPanelTransferEnabletoMoveUnloadPos = TRUE;
		
		m_plnkTrsAutoManager->TransferGlassData(DEF_PANEL_ALIGNER_PANEL, DEF_PANEL_TRANSFER_OUT_PANEL);

		m_bITISendCompleteToPanelTransferOut = TRUE;

		while (TRUE)
		{
			Sleep(10);
			if (FALSE == m_plnkITIPanelTransferOut->IsReceiveStartToPanelAligner())
				break;
		}
		m_bITISendStartToPanelTransferOut = FALSE;
		m_bITISendCompleteToPanelTransferOut = FALSE;

		// Line Tact Timer 종료
		if (m_EqLineTactTimer.IsTimerStarted())
		{
			double dElapseTime = m_EqLineTactTimer.GetElapseTime();
			// selsk_jdy
			if (dElapseTime < 0.0)
				dElapseTime = 0.0;

			//20170909 KDW Start
			m_strOutPanelID = m_plnkTrsAutoManager->GetGlassID(DEF_PANEL_ALIGNER_PANEL);

			m_plnkProductInfoData->SetPanelInfo(dElapseTime, m_strOutPanelID, CTime(m_EqTactTimer.GetStartTime()), CTime::GetCurrentTime());
			m_strOutPanelID.Empty();

			double dEquipTact = dElapseTime - m_dWaitTactTime;
			m_EqTactWaitTimer.StopTimer();//170909 JSH
			m_EqTactTimer.StopTimer();
			m_EqLineTactTimer.StopTimer();
			m_dWaitTactTime = 0.0; //cd17-026

			if (dEquipTact < 0.0)
				dEquipTact = 0.0;
			
			m_plnkProductInfoData->m_dEquipTact = dEquipTact;			
			//20170909 KDW End

			//170427 JSH.s
			//line, mech, waite, mount wait
			CString strLogMsg;
			strLogMsg.Format(_T(",Cell ID,%s,LineTact, %0.3lf, EQTact, %0.3lf, Wait, %0.3lf,"), m_strOutPanelID, dElapseTime, m_plnkProductInfoData->m_dEquipTact, m_dWaitTactTime);
			WriteEqTactTimeLog(strLogMsg);

			::PostMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_LINE_TACK_TIME, NULL, NULL);
			::PostMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_EQ_TACK_TIME, NULL, NULL);
			::PostMessage(m_plnkTrsAutoManager->GetLogViewHandle(), WM_REFRESH_LOG_MAIN_WINDOW, DEF_LINE_TACT_LOG, NULL);
			::PostMessage(m_plnkTrsAutoManager->GetLogViewHandle(), WM_REFRESH_LOG_MAIN_WINDOW, DEF_MACHINE_TACT_LOG, NULL);
			//170427 JSh.e
		}

		// selsk_jdy
		// Line Tact Timer Start!
		m_dWaitTactTime = 0.0; //170427 JSH
		m_EqLineTactTimer.StartTimer();

		// Doolin kwangilshin 2017. 09. 15.
		//
// 		m_plnkProductInfoData->m_iProductCount++;
// 		::PostMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_PRODUCT_QTY, NULL, NULL);
		// End.

		m_bSkipCurrentPanelWork = FALSE;

		//PanelTransfer에 Panel이 있을경우 Aligner가 Loading위치로 이동하면서 m_bITIPanelTransferEnabletoMoveUnloadPos을 TRUE로 주면,
		//PanelTransfer와 PanelTransferOut이 동시에 이동한다. 따라서 시간차를 두기 위해 Sleep을 준다
//		if (m_plnkStatePanelTransfer->IsPanelAbsorbed())
//			Sleep(500);
		m_bDryRunPanelExist = FALSE;
		setStep(PANEL_ALIGNER_MOVE_TO_LOAD_POS);
		break;

		//NSMC KJS
	case PANEL_ALIGNER_MODEL_CHANGE: simulLog("PANEL_ALIGNER_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;
		
		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;
		
		m_bModelChangeReady = FALSE;

		setStep(PANEL_ALIGNER_MOVE_TO_LOAD_POS);
		break;
		
	case PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY:
		m_bModelChangeReady = FALSE;
		if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed() || TRUE == m_bDryRunPanelExist)
		{
			iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS);
			if (iResult)
				PROCESS_ALARM(iResult);
		}
		else
		{
			if (FALSE == m_plnkPanelAligner->IsDownPickUpUnit())
			{
				iResult = m_plnkPanelAligner->DownPickUpUnit();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
		/*/
		iResult = m_plnkPanelAligner->SafeMoveBackupMoveXPos(DEF_BACKUP_MOVE_X_READY_POS);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}
		/*/
		setStep(PANEL_ALIGNER_WAIT_1FEEDERALLMOUNT_INIT);
		break;

	case PANEL_ALIGNER_WAIT_1FEEDERALLMOUNT_INIT:
		//120116.KMS________
		m_bMountAutoChangeReady = TRUE;
		if(TRUE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
			break;

		if(m_plnkSystemData->m_bUse1FeederAllMount == TRUE)
		{
			m_b1FeederAllMountInitReady = TRUE;
			if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
				break;
		}
		m_bMountAutoChangeReady = FALSE;
		//__________________

		initializeITI();
		m_bLastWorkGroup = FALSE;
		m_plnkPanelAligner->AirCVBlowOn();
		m_bIsAirCVVacOnFlag = FALSE;
		m_b1FeederAllMountInitReady = FALSE;
		Sleep(1000);
		if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed() || TRUE == m_bDryRunPanelExist)
		{
			if (FALSE == m_plnkPanelAligner->IsAligned())
			{
				setStep(PANEL_ALIGNER_MOVE_TO_ALIGN_POS);
			}
			else
			{
				setStep(PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS);
			}
		}
		else
		{
			setStep(PANEL_ALIGNER_MOVE_TO_LOAD_POS);
		}
		break;
		//170705 JSH.s
	case PANEL_ALIGNER_INSPECTION_TAB_BOLB_READY: 
		//171004 JSh.s
		//170719_KDH 작업할꺼 없으면 Skip Test중.
		m_iScheduleNo = 0;	//@
		m_bLastWorkGroup = TRUE;
		for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
		{
			MListWorkTab ListWorkTab;
			if (isUseWorkGroup(DEF_FRONT_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if (isUseWorkGroup(DEF_REAR_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if(m_bLastWorkGroup == FALSE)
				break;
		}
		m_iScheduleNo = i;
		//171004 JSh.e
		
		setStep(PANEL_ALIGNER_INSPECTION_TAB_BOLB);
		break;
	case PANEL_ALIGNER_INSPECTION_TAB_BOLB:
#ifdef DEF_USE_REVERSE_TAB_SIDE
		if (m_iScheduleNo == 0)
			iPosID = DEF_PANEL_ALIGNER_INSPECT_1_POS;
		else
			iPosID = DEF_PANEL_ALIGNER_INSPECT_2_POS;
#else
		if (m_iScheduleNo == 0)
			iPosID = DEF_PANEL_ALIGNER_INSPECT_2_POS;
		else
			iPosID = DEF_PANEL_ALIGNER_INSPECT_1_POS;
#endif
		
		//170920 JSH.s
		if (m_iScheduleNo != 0)
		{
			if (FALSE == m_plnkStatePanelTransferOut->IsPanelReleased()
				|| FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad()
				)
			{
				break;
			}
		}
		//170920 JSH.e
		
		if (FALSE == m_plnkPanelAligner->IsInXYTPos(iPosID))
		{
			//			bPreBonderMoreWork = m_plnkProcessData->IsMoreWorkForCurrentPanel();
			//			if (TRUE == bPreBonderMoreWork)
			{
				//Air부상 여부
				if (TRUE == m_bIsAirCVVacOnFlag || FALSE == m_plnkPanelAligner->IsAirCVVacAllOff())
				{
					m_plnkCtrlPanelAligner->CheckPlateBlowOn();
					
					m_bIsAirCVVacOnFlag = FALSE;
				}
				//작업구간에 있는 TabMounters상승 여부
				iResult = waitTabMounterAllUp();
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
			
			iResult = m_plnkCtrlPanelAligner->MovetoInspectPos(iPosID);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
		setStep(PANEL_ALIGNER_INSPECTION_TAB_BOLB_AFTER);
		break;
	case PANEL_ALIGNER_INSPECTION_TAB_BOLB_AFTER: 
		m_bReadyInspectionBolbPos[m_iScheduleNo] = TRUE;
		
		bPreBonderMoreWork = TRUE;
		if (isUseWorkGroup(DEF_FRONT_GROUP))
			bPreBonderMoreWork &= m_plnkWorkSchedule[m_iScheduleNo][DEF_FRONT_GROUP]->IsMoreWorkForCurrentPanel();
		if (isUseWorkGroup(DEF_REAR_GROUP))
			bPreBonderMoreWork &= m_plnkWorkSchedule[m_iScheduleNo][DEF_REAR_GROUP]->IsMoreWorkForCurrentPanel();
		
		if (TRUE == bPreBonderMoreWork)
		{
			setStep(PANEL_ALIGNER_INSPECTION_TAB_BOLB);
			break;
		}
		
		m_iScheduleNo++;
		//170719_KDH 작업할꺼 없으면 Skip Test중.
		m_bLastWorkGroup = TRUE;
		for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
		{
			MListWorkTab ListWorkTab;
			if (isUseWorkGroup(DEF_FRONT_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if (isUseWorkGroup(DEF_REAR_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if(m_bLastWorkGroup == FALSE)
				break;
		}
		m_iScheduleNo = i;
		//_______________________
		
		if (m_iScheduleNo < DEF_MAX_WORK_SCHEDULE)
		{
			setStep(PANEL_ALIGNER_INSPECTION_TAB_BOLB);
			break;
		}
		
		//171004 JSh.s
		//170719_KDH 작업할꺼 없으면 Skip Test중.
		m_iScheduleNo = 0;	//@
		m_bLastWorkGroup = TRUE;
		for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
		{
			MListWorkTab ListWorkTab;
			if (isUseWorkGroup(DEF_FRONT_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if (isUseWorkGroup(DEF_REAR_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if(m_bLastWorkGroup == FALSE)
				break;
		}
		m_iScheduleNo = i;
		//171004 JSh.e
		
		setStep(PANEL_ALIGNER_INSPECTION_TAB_BOLB_DONE);
		break;
	case PANEL_ALIGNER_INSPECTION_TAB_BOLB_DONE: 
		//170612 JSH.s
		//스텝 관계없이 Inspection Camea TabIC Blob 검사중에는 정지...
#ifdef DEF_SOURCE_SYSTEM
		if (m_plnkITIInspectionCamera->IsTabBlobWorkingToPA(DEF_FRONT_GROUP) == TRUE)
			break;
#else
		if (isUseWorkGroup(DEF_FRONT_GROUP))
		{
			if (m_plnkITIInspectionCamera->IsTabBlobWorkingToPA(DEF_FRONT_GROUP) == TRUE)
				break;
		}
		if (isUseWorkGroup(DEF_REAR_GROUP))
		{
			if (m_plnkITIInspectionCamera->IsTabBlobWorkingToPA(DEF_REAR_GROUP) == TRUE)
				break;
		}
#endif
		//170612 JSH.e
		
		setStep(PANEL_ALIGNER_MOVE_TO_ALIGN_POS);
		break;
		//170705 JSH.e
	default:
		break;
	}
}

void MTrsPanelAligner::initializeITI()
{	
	m_bITIReceiveAbleToPanelTransfer			= FALSE;
	m_bITIReceiveStartToPanelTransfer			= FALSE;

	m_bITISendStartToPanelTransferOut				= FALSE;
	m_bITISendCompleteToPanelTransferOut			= FALSE;

	m_bITIInspectorEnabletoInspect				= FALSE;

	// 2009.01.13 CYJ Mech ver3 Source Inspection Cam2가 Panel Align도 하면서 추가.
	m_bITIInspectorEnabletoInspectMove			= FALSE;

	m_bITIWorkerEnabletoWork					= FALSE;
	m_bITIPanelTransferEnabletoMoveUnloadPos	= FALSE;

	m_bPanelAlignerUnloadingPanel				= FALSE;

	m_bRepairRunContinue						= FALSE;

	m_bReceivedPressCompleteSchedule[DEF_FRONT_GROUP] = FALSE;
	m_bReceivedPressCompleteSchedule[DEF_REAR_GROUP] = FALSE;

	//170705 JSH.s
	for(int i =0; i<DEF_MAX_WORK_SCHEDULE; i++)
	{
		m_bReadyInspectionBolbPos[i] = FALSE;
	}
	//170705 JSH.e
}

void MTrsPanelAligner::initializeMelsecIF()
{
	m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eArm1FoldComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eImmediatelyPauseRequest, FALSE);
}

void MTrsPanelAligner::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepPanelAligner)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepPanelAligner)0;	//  현재 Step Clear
}

void MTrsPanelAligner::setStep(EStepPanelAligner estepPanelAligner)
{
	m_estepPrevious	= m_estepCurrent;		// 이전 Step 저장
	m_estepCurrent	= estepPanelAligner;	// 새로 Step으로 전환
}

int MTrsPanelAligner::AssignComponents(STrsPanelAlignerRefCompList listRefComponents)
{
	m_plnkTrsAutoManager		= listRefComponents.m_plnkTrsAutoManager;
	m_plnkCtrlPanelAligner		= listRefComponents.m_plnkCtrlPanelAligner;
	m_plnkPanelAligner			= listRefComponents.m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
//@	m_plnkStatePanelTransferCenter = listRefComponents.m_plnkStatePanelTransferCenter;
	m_plnkStatePanelTransfer	= listRefComponents.m_plnkStatePanelTransfer;
#endif
	m_plnkStatePanelTransferOut	= listRefComponents.m_plnkStatePanelTransferOut;
	m_plnkIO					= listRefComponents.m_plnkIO;
	m_plnkMelsecIF				= listRefComponents.m_plnkMelsecIF;
	m_plnkStateTabMounter1		= listRefComponents.m_plnkStateTabMounter1;
	m_plnkStateTabMounter2		= listRefComponents.m_plnkStateTabMounter2;
	m_plnkModelChange1			= listRefComponents.m_plnkModelChange1;
	m_plnkModelChange2			= listRefComponents.m_plnkModelChange2;

	m_plnkCameraCarrier1		= listRefComponents.m_plnkCameraCarrier1;
	m_plnkCameraCarrier2		= listRefComponents.m_plnkCameraCarrier2;

	//SJ_YYK 110421 ... Ellas...
	m_plnkLCNet					= listRefComponents.m_plnkLCNet;

	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::SetData(STrsPanelAlignerData datComponent)
{
	m_plnkPanelData			= datComponent.m_plnkPanelData;
	m_plnkSystemData		= datComponent.m_plnkSystemData;
	m_plnkPrebonderData		= datComponent.m_plnkPrebonderData;
	m_plnkProductInfoData	= datComponent.m_plnkProductInfoData;
	m_plnkProcessData		= datComponent.m_plnkProcessData;
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_plnkWorkSchedule[0][i]	= datComponent.m_plnkWorkSchedule[i];
#ifdef DEF_SOURCE_SYSTEM
		m_plnkWorkSchedule[1][i]	= datComponent.m_plnkWorkScheduleSecond[i];
#endif
	}

	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

//@int MTrsPanelAligner::Initialize(BOOL bSkipCurrentPanelWork)
int MTrsPanelAligner::Initialize(BOOL bResetPanelWorkNo, BOOL bSkipCurrentPanelWork)
{
	m_bTabInspectionTabBlobCheck = FALSE; //171004 JSh
	m_bAutoInitialized = FALSE;
	m_bSkipCurrentPanelWork = bSkipCurrentPanelWork;

	initializeITI();
	initializeStep();

	m_plnkPanelAligner->AirCVBlowOn();
	m_bIsAirCVVacOnFlag = FALSE;
	int i = 0;

	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;
#ifndef SIMULATION
	iResult = m_plnkPanelAligner->CheckVacuum();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
		m_plnkPanelAligner->ReleasePanel();
		return iResult;
	}

	iResult = m_plnkPanelAligner->CheckOrigin();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
		return iResult;
	}
	
	//170724_KDH SpotCleaner 추가
	iResult = m_plnkPanelAligner->SpotCleanerOff();
	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
		return iResult;
	}
	//__________________
	
	//150527 SJ_YSH
#	ifdef DEF_SOURCE_SYSTEM
	if(m_plnkPanelData->m_dPanelSize_X > (DEF_PANEL_SIZE_78INCH + 5))
	{
		iResult = MOlbSystemPre.GetPanelTransferComponent()->ForAirRailFB1();
		if(iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}

		iResult = MOlbSystemPre.GetPanelTransferComponent()->ForAirRailFB2();
		if(iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}
	}
	else
	{
		iResult = MOlbSystemPre.GetPanelTransferComponent()->BackAirRailFB1();
		if(iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}

		iResult = MOlbSystemPre.GetPanelTransferComponent()->BackAirRailFB2();
		if(iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}
	}
#	endif
	//_____________


	if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed())
	{
		/* selsk_jdy 삭제.
		// 설비 Tact Timer 시작 - 종료 : Panel Aligner가 Unloading 할 때
		// Panel 가진 상태에서 Tact 측정 시작을 위해
		m_bPanelExistTactStart = TRUE;
		*/
		//@_______________________________
		if (TRUE == bResetPanelWorkNo)
		{
			//170719_KDH 작업할꺼 없으면 Skip Test중.
			m_iScheduleNo = 0;	//@
			m_bLastWorkGroup = TRUE;
			for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
			{
				MListWorkTab ListWorkTab;
				if (isUseWorkGroup(DEF_FRONT_GROUP))
				{
					m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
					m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
				}
				if (isUseWorkGroup(DEF_REAR_GROUP))
				{
					m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
					m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
				}
				if(m_bLastWorkGroup == FALSE)
					break;
			}
			m_iScheduleNo = i;
			//_______________________
//			m_iScheduleNo = 0;
			m_bITIPanelTransferEnabletoMoveUnloadPos = FALSE; //170216 SJ_YSH
		}
		//@_______________________________

		iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}

		if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE || TRUE == m_bSkipCurrentPanelWork)
		{
			setStep(PANEL_ALIGNER_MOVE_TO_ALIGN_POS);
		}
		else //All Initiolize
		{
			//170705 JSH.s
			if(m_plnkSystemData->m_bUseInspectionTabBolb == TRUE && TRUE == bResetPanelWorkNo)
				m_bTabInspectionTabBlobCheck = TRUE; //171004 JSh

			setStep(PANEL_ALIGNER_MOVE_TO_ALIGN_POS);
			//170705 JSH.e
		}
		
	}
	else	// Panel 흡착하지 않은 경우.
	{
		//170719_KDH 작업할꺼 없으면 Skip Test중.
		m_iScheduleNo = 0;	//@
		m_bLastWorkGroup = TRUE;
		for (int i = m_iScheduleNo; i < DEF_MAX_WORK_SCHEDULE; i++)
		{
			MListWorkTab ListWorkTab;
			if (isUseWorkGroup(DEF_FRONT_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_FRONT_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if (isUseWorkGroup(DEF_REAR_GROUP))
			{
				m_plnkWorkSchedule[i][DEF_REAR_GROUP]->GetOrgWorkList(&ListWorkTab);
				m_bLastWorkGroup &= (ListWorkTab.GetTotalWorkTabCount() == 0);
			}
			if(m_bLastWorkGroup == FALSE)
				break;
		}
		m_iScheduleNo = i;
		//_______________________


		// selsk_jdy
		if (m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_ALIGNER_PANEL]->m_strEPanelID.GetLength() > 1)		
		{
			//PanelAligner 가 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?
			if (MyMessageBox(MY_MESSAGE_BOX, 62501, _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_ALIGNER_PANEL);	
		}

		//120224.KMS____________
#	ifdef DEF_SOURCE_SYSTEM
		if(TRUE == m_plnkPanelAligner->IsPanelWorkingDetected())
		{
			if(FALSE == m_plnkStatePanelTransferOut->IsPanelAbsorbed())
			{
				if(m_plnkStatePanelTransfer->IsPanelAbsorbed() == FALSE &&
				   m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS) == TRUE)
				{
					// 210081 = Panel Aligner 부상유닛에 Panel이 감지되지만 Aligner가 흡착상태가 아닙니다.
					return generateErrorCode(210081);
				}
			}
			else
			{
				if(FALSE == m_plnkStatePanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
				{
					if(m_plnkStatePanelTransfer->IsPanelAbsorbed() == FALSE &&
						m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS) == TRUE)
					{
						// 210081 = Panel Aligner 부상유닛에 Panel이 감지되지만 Aligner가 흡착상태가 아닙니다.
						return generateErrorCode(210081);
					}
				}
			}
		}
#	else
		if(TRUE == m_plnkPanelAligner->IsPanelWorkingDetected())
		{
			if(FALSE == m_plnkStatePanelTransferOut->IsPanelAbsorbed())
			{
				// 210070 = Panel Aligner 부상유닛에 Panel이 감지되지만 Aligner가 흡착상태가 아닙니다.
				return generateErrorCode(210076);
			}
			else
			{
				if(FALSE == m_plnkStatePanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
				{
					// 210070 = Panel Aligner 부상유닛에 Panel이 감지되지만 Aligner가 흡착상태가 아닙니다.
					return generateErrorCode(210076);
				}
			}
		}
#	endif

		iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS, FALSE, TRUE);
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}

		iResult = m_plnkCtrlPanelAligner->MovetoLoadPos();
		if (iResult)
		{
			SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
			return iResult;
		}

		setStep(PANEL_ALIGNER_MOVE_TO_LOAD_POS);
	}

	if (iResult)
	{
		SetErrorLevel(_T("PANEL ALIGNER initialize"),0, __FILE__, __LINE__);
		return iResult;
	}
#endif

	if (TRUE == m_plnkPrebonderData->m_bUseTabPanelAlign)
	{
		iResult = moveModelChangeWorkPos(TRUE);
		if (iResult)
			return iResult;
	}
	else
	{
		iResult = moveModelChangeAlignPos(TRUE);
		if (iResult)
			return iResult;
	}

	m_bLastWorkGroup = FALSE;//100929.KKY
	m_bDryRunPanelExist = FALSE;
	m_bAutoInitialized  = TRUE;

	//120116.KMS________
	m_bMountAutoChangeReady = FALSE;

	//120515.KMS______________
	m_plnkTrsAutoManager->SetAssemReport(FALSE);
	//________________________

	return ERR_TRS_PANEL_ALIGNER_SUCCESS; 
}

void MTrsPanelAligner::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

void MTrsPanelAligner::AssignITI(ITI_PanelTransfer* pITI_PanelTransfer, 
								ITI_PanelTransferOut* pITI_PanelTransferOut,
								ITI_TabMounter* pITI_TabMounter,
								ITI_TabMounter* pITI_RTabMounter, 
								ITI_InspectionCamera* pITI_InspectionCamera,
								ITI_InspectionCamera* pITI_RInspectionCamera, 
								ITI_TabFeeder* pITI_TabFeeder1,
								ITI_TabFeeder* pITI_TabFeeder2)
{
#ifdef DEF_SOURCE_SYSTEM
	m_plnkITIPanelTransfer				= pITI_PanelTransfer;
#endif
	m_plnkITIPanelTransferOut			= pITI_PanelTransferOut;
	m_plnkITITabMounter					= pITI_TabMounter;
	m_plnkITIRTabMounter				= pITI_RTabMounter;
	m_plnkITIInspectionCamera			= pITI_InspectionCamera;
	m_plnkITIRInspectionCamera			= pITI_RInspectionCamera;
	m_plnkITITabFeeder[DEF_TABFEEDER1]	= pITI_TabFeeder1;
	m_plnkITITabFeeder[DEF_TABFEEDER2]	= pITI_TabFeeder2;
}

void MTrsPanelAligner::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);

}

void MTrsPanelAligner::ThreadStop()
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

BOOL MTrsPanelAligner::IsInitialized(void) 
{
	return m_bAutoInitialized;
}

BOOL MTrsPanelAligner::IsReceiveAbleToPanelTransfer()	
{
	return m_bITIReceiveAbleToPanelTransfer;
}

BOOL MTrsPanelAligner::IsReceiveStartToPanelTransfer()	
{
	return m_bITIReceiveStartToPanelTransfer;
}

BOOL MTrsPanelAligner::IsSendStartToPanelTransferOut()	
{
	return m_bITISendStartToPanelTransferOut;
}

BOOL MTrsPanelAligner::IsSendCompleteToPanelTransferOut()	
{
	return m_bITISendCompleteToPanelTransferOut;
}

BOOL MTrsPanelAligner::IsPanelAlignerBeginAlignPanelToWorker()	
{
	return m_bITIPanelAlignerBeginAlignPanelToWorker;
}

BOOL MTrsPanelAligner::IsWorkerEnabletoWork()	
{
	return m_bITIWorkerEnabletoWork;
}

BOOL MTrsPanelAligner::IsInspectorEnabletoInspect()
{
	return m_bITIInspectorEnabletoInspect;
}

BOOL MTrsPanelAligner::IsInspectorEnabletoInspectMove()
{
	return m_bITIInspectorEnabletoInspectMove;
}

BOOL MTrsPanelAligner::IsPanelTransferEnabletoMoveUnloadPos()	
{
	return m_bITIPanelTransferEnabletoMoveUnloadPos;
}

BOOL MTrsPanelAligner::IsPanelAlignerUnloadingPanel()
{
	return m_bPanelAlignerUnloadingPanel;
}

BOOL MTrsPanelAligner::IsEmptyPanel()
{
	// selsk_jdy
	return m_plnkPanelAligner->IsPanelReleased();

	// selsk_jdy 아래 조건처럼 볼 필요 없다.
#if 0	// 아래 삭제 할 것.
	BOOL bResult1, bResult2, bResult3, bResult4;

	bResult1 = m_plnkPanelAligner->IsPanelReleased();
	bResult2 = m_plnkStatePanelTransfer->IsPanelDetected();
	bResult3 = m_plnkPanelAligner->IsAirCVVacAllOff();
	bResult4 = m_plnkPanelAligner->IsAirCVBlowOn();

	return bResult1 && (/*!bResult2 && */bResult3 && bResult4);
#endif
}

int MTrsPanelAligner::GetStep()
{
	return m_estepCurrent;
}

BOOL MTrsPanelAligner::IsAutoRun()
{
	return m_bAutoRun;
}

void MTrsPanelAligner::SetRepairRunContinue()
{
	m_bRepairRunContinue = TRUE;
}

BOOL MTrsPanelAligner::IsRepairRunContinue()
{
	return m_bRepairRunContinue;
}

int MTrsPanelAligner::ScrapPanel()
{
	// sesl_jdy
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
	{
		// 410010 = Panel Scrap 작업 불가. [Auto Run Mode 임.]
		return generateErrorCode(410010);
	}

	// sesl_jdy
	m_bScrapPanelReq = FALSE;

	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;

	m_plnkPanelAligner->AirCVVacAllOff();
	m_plnkPanelAligner->AirCVBlowOn();

	// sesl_jdy : 100 -> 50
	Sleep(50);

	m_plnkPanelAligner->AirCVBlowOff();

	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_ALIGNER_PANEL);

	if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed())
	{
		do
		{
			iResult = m_plnkPanelAligner->ReleasePanel();
			if (iResult)
				return iResult;
			
			Sleep(500);


			if(MyMessageBox(MY_MESSAGE_BOX, 62600, _T("Question Scrap"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) ==IDNO)
				return FALSE;

			//Panel Aligner 에 Panel 이 있습니다. 제거해 주세요.
//			MyMessageBox(MY_MESSAGE_BOX, 62500, _T("Remove Panel Request"), M_ICONINFORMATION);
			MyMessageBox(MY_MESSAGE_BOX, 62400, _T("Remove Panel Request"), M_ICONINFORMATION);


			m_plnkPanelAligner->AbsorbPanel(TRUE);
//KKY.Del			m_plnkPanelAligner->UpPickUpUnit();
			Sleep(500);

		} while (TRUE == m_plnkPanelAligner->IsPanelAbsorbed());
	}

	m_plnkPanelAligner->ReleasePanel(TRUE);

	return Initialize(TRUE, FALSE);
}

// selsk_jdy
BOOL MTrsPanelAligner::isPanelTransferEnabledToMoveUnloadPos(int iCheckPosID/*= -1*/)	//@---> 미사용 함수
{
#ifdef SIMULATION
	return FALSE;
#endif

	ASSERT(iCheckPosID >= -1 && iCheckPosID < DEF_PANEL_ALIGNER_MAX_POSITION);

	double dCheckPosX = (-1 == iCheckPosID)
		? m_plnkPanelAligner->GetXYTCurrentPos().dX
		: m_plnkPanelAligner->GetXYTTargetPos(iCheckPosID).dX;
	
	double dLoadPosX = m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_LOAD_POS).dX;
	
	double dSafeDist = dCheckPosX - dLoadPosX - 58.0; // 58.0 = margin : Tab 길이 반영. (사양 50mm) : 설비별로 다른 값을 가질 수 있다.
	ASSERT(dSafeDist >= 0.0);

	if (m_plnkPanelData->m_dPanelSize_Y < dSafeDist)
		return TRUE;
	
	return FALSE;
}

// selsk_jdy
BOOL MTrsPanelAligner::isPanelAlignerEnabledToMoveWorkPos()	//@---> 미사용 함수
{
	double dUnloadPosX = m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_UNLOAD_POS).dX;
	double dWorkPosX = m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_PRESS_1_POS).dX;
	
	double dSafeDist = dUnloadPosX - dWorkPosX - 58.0; // 58.0 = margin : Tab 길이 반영. (사양 50mm) : 설비별로 다른 값을 가질 수 있다.
	ASSERT(dSafeDist >= 0.0);

	if (m_plnkPanelData->m_dPanelSize_Y < dSafeDist)
		return TRUE;
	
	return FALSE;
}

int MTrsPanelAligner::waitBackupDown()
{
	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::waitTabMounterAllUp()
{
	MTickTimer timerUpCheck;
	timerUpCheck.StartTimer();

	int i = 0;
	while (TRUE)
	{
#ifdef SIMULATION
		break;
#endif
		Sleep(5);
		if (TRUE == m_plnkStateTabMounter1->IsUpAllTabMounterInGroup() && TRUE == m_plnkStateTabMounter2->IsUpAllTabMounterInGroup())
		{
			break;
		}

		if (TRUE == timerUpCheck.MoreThan(10.0))
		{
			// 410004 = Interlock Check : TabMounter Up 안됨.
			return generateErrorCode(410004);
		}
		
	}

	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::waitPlateVacuumOn()
{
	m_plnkPanelAligner->AirCVBlowOff();	//100822.임시추가
	//100906_____
//	Sleep(200);
	Sleep(10);
	//___________
	m_plnkPanelAligner->AirCVVacAllOn(m_iScheduleNo);
	Sleep(10);
	
	MTickTimer timerVacuum;
	timerVacuum.StartTimer();
	
	while (FALSE == m_plnkPanelAligner->IsAirCVVacAllOn(m_iScheduleNo))
	{
		if (TRUE == timerVacuum.MoreThan(10.0))
		{
			// 410000 = 부상 Plate 가 Panel 을 흡착하지 못함.
			return generateErrorCode(410000);
		}
		
		Sleep(10);
	}
	
	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::moveModelChangeWorkPos(BOOL bWait)
{
	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;
//@#ifdef DEF_GATE_SYSTEM
	BOOL bMove1 = FALSE, bMove2 = FALSE;
	if (FALSE == m_plnkModelChange1->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
	{
		bMove1 = TRUE;
		iResult = m_plnkModelChange1->SafeMoveYPos(DEF_MODEL_CHANGE_MODEL_POS, TRUE, FALSE);
		if (iResult)
			return iResult;
	}
	if (FALSE == m_plnkModelChange2->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
	{
		bMove2 = TRUE;
		iResult = m_plnkModelChange2->SafeMoveYPos(DEF_MODEL_CHANGE_MODEL_POS, TRUE, FALSE);
		if (iResult)
			return iResult;
	}
	if (bWait)
	{
		if (bMove1)
		{
			iResult = m_plnkModelChange1->Wait4DoneY(TRUE);
			if (iResult)
				return iResult;
			
			//150527 SJ_YS Del..
			/*
			MOlbSystemPre.GetPanelTransferComponent()->BackAirRailFB1();

			MOlbSystemPre.GetPanelTransferComponent()->ForAirRailFB1();
			*/

		}
		if (bMove2)
		{
			iResult = m_plnkModelChange2->Wait4DoneY();
			if (iResult)
				return iResult;
			
			//150527 SJ_YS Del..
			/*
			MOlbSystemPre.GetPanelTransferComponent()->BackAirRailFB2();

			MOlbSystemPre.GetPanelTransferComponent()->ForAirRailFB2();
			*/
		}
	}
//@#endif
	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::waitModelChangeWorkPos()
{
	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;
//@#ifdef DEF_GATE_SYSTEM
	if (FALSE == m_plnkModelChange1->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
	{
		iResult = m_plnkModelChange1->Wait4DoneY(TRUE);
		if (iResult)
			return iResult;
	}
	if (FALSE == m_plnkModelChange2->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
	{
		iResult = m_plnkModelChange2->Wait4DoneY();
		if (iResult)
			return iResult;
	}
//@#endif
	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::moveModelChangeAlignPos(BOOL bWait)
{
	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;
//@#ifdef DEF_GATE_SYSTEM
	BOOL bMove1 = FALSE, bMove2 = FALSE;
	if (FALSE == m_plnkModelChange1->IsInYPos(DEF_MODEL_CHANGE_PANELALIGN_POS))
	{
		bMove1 = TRUE;
		iResult = m_plnkModelChange1->SafeMoveYPos(DEF_MODEL_CHANGE_PANELALIGN_POS, TRUE, FALSE);
		if (iResult)
			return iResult;
	}
	if (FALSE == m_plnkModelChange2->IsInYPos(DEF_MODEL_CHANGE_PANELALIGN_POS))
	{
		bMove2 = TRUE;
		iResult = m_plnkModelChange2->SafeMoveYPos(DEF_MODEL_CHANGE_PANELALIGN_POS, TRUE, FALSE);
		if (iResult)
			return iResult;
	}
	if (bWait)
	{
		if (bMove1)
		{
			iResult = m_plnkModelChange1->Wait4DoneY(TRUE);
			if (iResult)
				return iResult;
			//150527 SJ_YS Del..
			/*
			MOlbSystemPre.GetPanelTransferComponent()->BackAirRailFB1();

			MOlbSystemPre.GetPanelTransferComponent()->ForAirRailFB1();
			*/
		}
		if (bMove2)
		{
			iResult = m_plnkModelChange2->Wait4DoneY();
			if (iResult)
				return iResult;
			//150527 SJ_YS Del..
			/*
			MOlbSystemPre.GetPanelTransferComponent()->BackAirRailFB2();

			MOlbSystemPre.GetPanelTransferComponent()->ForAirRailFB2();
			*/
		}
	}
//@#endif
	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

int MTrsPanelAligner::waitModelChangeAlignPos()
{
	int iResult = ERR_TRS_PANEL_ALIGNER_SUCCESS;
//@#ifdef DEF_GATE_SYSTEM
	if (FALSE == m_plnkModelChange1->IsInYPos(DEF_MODEL_CHANGE_PANELALIGN_POS))
	{
		iResult = m_plnkModelChange1->Wait4DoneY(TRUE);
		if (iResult)
			return iResult;
	}
	if (FALSE == m_plnkModelChange2->IsInYPos(DEF_MODEL_CHANGE_PANELALIGN_POS))
	{
		iResult = m_plnkModelChange2->Wait4DoneY();
		if (iResult)
			return iResult;
	}
//@#endif
	return ERR_TRS_PANEL_ALIGNER_SUCCESS;
}

BOOL MTrsPanelAligner::isUseWorkGroup(int iWorkGroup)
{
	BOOL bUse = FALSE;
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
#	ifdef DEF_GATE_SYSTEM
	#	ifndef DEF_MIRROR_
		bUse = m_plnkPanelData->m_bUseFlag_Yl;
	#	else
		bUse = m_plnkPanelData->m_bUseFlag_Yr;
	#	endif
#	else
	bUse = m_plnkPanelData->m_bUseFlag_Xu;
#	endif
	}
	else
	{
#	ifdef DEF_GATE_SYSTEM
	#	ifndef DEF_MIRROR_
		bUse = m_plnkPanelData->m_bUseFlag_Yr;
	#	else
		bUse = m_plnkPanelData->m_bUseFlag_Yl;
	#	endif
#	else
	return bUse;
#	endif
		//SJ_YYK 150907 Modify...
#ifndef DEF_MIRROR_
		bUse = m_plnkPanelData->m_bUseFlag_Xu;
#else
		bUse = m_plnkPanelData->m_bUseFlag_Xd;
#endif
	}
	return bUse;
}

BOOL MTrsPanelAligner::is1FeederAllMounterInitStart()
{
	BOOL bInitStart = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart() && TRUE == m_plnkTrsAutoManager->IsAll1FeederAllMount_MountInitReady())
		{
			bInitStart = TRUE;
		}
	}
#endif
	/*/
	return bInitStart;
}


//syr..SESL: hongju 추가 0523_SESL..Interface 문제 해결...뒷설비로 Panel 넘길시 부상이 잡고 있으면 센서 벗어나지 않는 에러 발생..
BOOL MTrsPanelAligner::IsPlateVacuumOn()
{
	return m_plnkPanelAligner->IsAirCVVacAllOn(m_iScheduleNo);
}

// NSMC
BOOL MTrsPanelAligner::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsPanelAligner::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

BOOL MTrsPanelAligner::IsPanelInStop()
{
	return (m_bPanelInStop || /*m_plnkLCNet->m_eEqState == ePM ||*/ m_plnkTrsAutoManager->IsPanelInStopNSMC());
}

void MTrsPanelAligner::SetPanelInStop(BOOL bSel)
{
	m_bPanelInStop = bSel;
	// 2011.11.05 Park, YK - //ELLAS_111103
	if(m_bPanelInStop == TRUE)
	{
		m_plnkLCNet->EqSpecialStateReport(ePanelSupplyStopStart);
	}
	else
	{
		m_plnkLCNet->EqSpecialStateReport(ePanelSupplyStopEnd);
	}
}

//@130204.KKY___________
BOOL MTrsPanelAligner::isAllTabMounterReadyToPusherDown()
{
	return (m_plnkITITabMounter->IsTabMounterReadyPusherDownToPA() && m_plnkITIRTabMounter->IsTabMounterReadyPusherDownToPA());
}
//@_____________________

//120116.kms__________
BOOL MTrsPanelAligner::IsMountAutoChangeReady()
{
	return m_bMountAutoChangeReady;
}

//170705 JSH.s
BOOL MTrsPanelAligner::IsReadyInspectionBolbPos(int iWorkGroup)
{
	return m_bReadyInspectionBolbPos[iWorkGroup]; //m_iScheduleNo
}
//170705 JSH.e

// End Of Code
