/* 
 * TrsPanelTransfer Component
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
 * MTrsPanelTransfer.cpp : Implementation of MTrsPanelTransfer component.
 */

#include "Stdafx.h"
#include "MSystemData.h"
#include "MPanelTransfer.h"
#include "IStatePanelAligner.h"
//#include "IStateRollerConveyor.h"
#include "MTrsPanelTransfer.h"
#include "MTrsAutoManager.h"
#include "ITI_LoadConveyor.h"
#include "ITI_PanelAligner.h"
#include "ITI_PanelTransferCenter.h"
#include "ITI_PanelTransferOut.h"
#include "ITI_THandler.h"
#include "common.h"
#include "MTickTimer.h"
#ifdef DEF_PLATFORM_ARP_SYSTEM
#include "ITI_Carrier.h"
#endif

#include "MPanelData.h"
#include "MPanelAligner.h" //17019 JSH

#include "DefPanelAligner.h"

#include "MPlatformOlbSystem.h"
extern MPlatformOlbSystem	MOlbSystemPre;


MTrsPanelTransfer::MTrsPanelTransfer(SCommonAttribute				commonData,
									STrsPanelTransferRefCompList	listRefComponents,
									STrsPanelTransferData			datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	m_bAutoInitialized  = FALSE;

	m_bThreadLife		= FALSE;
	m_pThread			= NULL;	
	m_bAutoRun			= FALSE;
	m_bPanelInStop		= FALSE;

	initializeITI();
	initializeStep();
}

MTrsPanelTransfer::~MTrsPanelTransfer()
{
}

void MTrsPanelTransfer::InitializeMelsecIF()
{
	m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eArm1FoldComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eGlassCheckSensorOn, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eImmediatelyPauseRequest, FALSE);
}

void MTrsPanelTransfer::simulLog(CString strMsg, int iStep, int iSleep)
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
#endif	/* SIMULATION */
}

UINT MTrsPanelTransfer::threadFunction(LPVOID pParam)
{
	// TrsPanelTransfer Object Pointer Assign
	MTrsPanelTransfer* pThis = (MTrsPanelTransfer*) pParam;

	TRACE(_T("MTrsPanelTransfer Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsPanelTransfer Thread Stop\n"));

	return ERR_TRS_PANEL_TRANSFER_SUCCESS;
}

void MTrsPanelTransfer::threadJob()
{
#ifndef DEF_USE_TRANSFER_CENTER	
	//170919 JSH.s
 	double dAlignerTolerance = 300.0;
 	BOOL bTransferDangerState = (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE);
 	BOOL bAlignerDangerState = (m_plnkStatePanelAligner->IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dAlignerTolerance) == TRUE && m_plnkStatePanelAligner->IsPanelAbsorbed() == TRUE);
 
 	if( bTransferDangerState || bAlignerDangerState )
 		m_plnkMelsecIF->SetUpperEqBit(eReserved1_UpperEqBit, FALSE);
 	else
 		m_plnkMelsecIF->SetUpperEqBit(eReserved1_UpperEqBit, TRUE);

	m_plnkMelsecIF->SetUpperEqBit(eReserved2_UpperEqBit, TRUE);
	//170919 JSH.e
#endif

	switch (m_plnkTrsAutoManager->GetOPMode())
	{
	case MANUAL_MODE:
		m_bAutoRun = FALSE;
		break;

	case AUTORUN_MODE:
		switch (m_plnkTrsAutoManager->GetOPStatus())
		{
		case ERROR_STOP:
		case STEP_STOP:
//@			m_bITIReceiveAbleToPanelTransferCenter = FALSE;
#ifndef DEF_USE_TRANSFER_CENTER	
			InitializeMelsecIF(); //170902 JSH
#endif
			m_bAutoRun = FALSE;
			break;

		case START_RUN:
			InitializeMelsecIF();
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

void MTrsPanelTransfer::doRunStep()
{
	int iResult = ERR_TRS_PANEL_TRANSFER_SUCCESS;
	double dAlignerTolerance = 100.0; //170201 SJ_YSH

	switch (m_estepCurrent)
	{
	case PANEL_TRANSFER_MOVE_TO_LOAD_POS:

		//Panel Receive 준비
		//Release Vacuum -> Down & Stopper Down -> Load Pos 이동

		if (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE)
		{
			// 424000 = Panel Loading 동작 전에 Panel을 흡착하고 있습니다.
			PROCESS_ALARM(generateErrorCode(424000));
			break;
		}

		iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		//170929 KDW Gear
		//iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		iResult = m_plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(PANEL_TRANSFER_RECEIVE_PANEL_READY);
		break;

	case PANEL_TRANSFER_RECEIVE_PANEL_READY:
		//Panel Aligner의 Panel 작업 끝나고 Panel Aligner Uload 대기 위치로 이동 대기
		//1.PanelAligner로부터 동시 동작을 위해 신호를 받아야 한다.
		// - PanelTransferEnabletoStopperUp 신호 대기
		//2.Stopper Up

		setStep(PANEL_TRANSFER_RECEIVE_PANEL);
		break;
		
	case PANEL_TRANSFER_RECEIVE_PANEL:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(PANEL_TRANSFER_MODEL_CHANGE);
			break;
		}

		//170201 SJ_YSH
		/*/
		if(m_plnkITIPanelAligner->IsPanelTransferEnabletoMoveUnloadPos() == FALSE)
			break;
		/*/

		dAlignerTolerance = 1200.0; //170913 JSH
		if(m_plnkStatePanelAligner->IsPanelAbsorbed() == TRUE 
			&& TRUE == m_plnkStatePanelAligner->IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dAlignerTolerance)) //위치 확인하고 수정 가능.
			break;


#ifdef DEF_USE_TURN_HANDLER	
		m_bITIReceiveAbleToTurnHandler = TRUE;
		m_bITIReceiveAbleToPanelTransferCenter = TRUE;

		if(m_plnkITITurnHandler->IsUnloadingReady() == FALSE)
			break;

		while(TRUE)
		{
			Sleep(10);
			if(m_plnkITITurnHandler->IsUnloadingComplete() == TRUE)
				break;

			if(m_plnkITITurnHandler->IsUnloadingReady() == FALSE)
				return;
		}

		m_bITIReceiveCompleteToTurnHandler = TRUE;
	
		while(TRUE)
		{
			Sleep(10);
			if(m_plnkITITurnHandler->IsUnloadingReady() == FALSE)
				break;
		}

		m_bITIReceiveAbleToTurnHandler = FALSE;
		m_bITIReceiveCompleteToTurnHandler = FALSE;
		m_bITIReceiveAbleToPanelTransferCenter = FALSE;
#else
		//SJ_YYK 150318 Add..
		if(m_plnkTrsAutoManager->m_bSetAutoAlignPanelInStop == TRUE)
		{
			m_plnkTrsAutoManager->m_bSetAutoAlignPanelInStop = FALSE;
			SetPanelInStop(TRUE);
		}
		//____________________________*/
		
		//170923 JSh.s
		//Trs동작중 Load위치 안가는 현상으로 인해 추가
		if(m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS) == FALSE)
		{
			//170929 KDW Gear
			//iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
			iResult = m_plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
			if (iResult)
			{
				SetErrorLevel(_T("PANEL TRANSFER ready to Panel Receive"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//170923 JSh.e

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			BOOL bNotReceive = FALSE;

			double dAlignerTolerance = 1200.0;//170913 JSH
			BOOL bTransferDangerState = (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE);
			BOOL bAlignerDangerState = (m_plnkStatePanelAligner->IsInXPos(DEF_PANEL_ALIGNER_LOAD_POS, &dAlignerTolerance) == TRUE && 
										m_plnkStatePanelAligner->IsPanelAbsorbed() == TRUE);
			
			if( bTransferDangerState || bAlignerDangerState )
				bNotReceive = TRUE;
			if (IsPanelInStop())
				bNotReceive = TRUE;

			if (bNotReceive == TRUE)
			{
				m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
				break;
			}

			// 1. ReceiveAble On
			m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, TRUE);
			// 2. SendAble Check
			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble))
				break;	

			if (m_plnkLCNet->m_eEqState != ePM)
			{
				if (m_plnkSystemData->m_bUseProductPlanValidation == TRUE)
				{
					iResult = m_plnkTrsAutoManager->CheckProductValidation();
					if (iResult)
					{
						Sleep(1000);
						iResult = m_plnkTrsAutoManager->CheckProductValidation();
						if (iResult == ERR_DISCORD_DEVICE_ID)
						{
							PROCESS_ALARM(generateErrorCode(437008));
							break;
						}
						else if (iResult == ERR_CNTOVER_PRODUCT_PLAN)
						{
							PROCESS_ALARM(generateErrorCode(437009));
							break;
						}
					}
				}
				if (m_plnkSystemData->m_bUseTXDataCheckMode == TRUE)
				{
					int iValidationCode = m_plnkTrsAutoManager->CheckPanelValidationItem(DEF_PANEL_TRANSFER_PANEL);
					if(iValidationCode)
					{
						iResult = m_plnkTrsAutoManager->ConvertValidationCodeToErrorCode(iValidationCode);
						if(iResult)
						{
							PROCESS_ALARM(iResult)
							break;
						}
					}
				}
				if (m_plnkSystemData->m_bUsePanelValidation == TRUE)
				{
					iResult = m_plnkTrsAutoManager->CheckPanelValidation(DEF_PANEL_TRANSFER_CENTER_PANEL);
					switch (iResult)
					{
					case ERR_NO_BATCHID_DATA:
						SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
						// 427011 = Panel에 Batch ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437004));
						return;
					case ERR_NO_DEVICEID_DATA:
						SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
						// 427012 = Panel에 Device ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437005));
						return;
					case ERR_NO_PANELID_DATA:
						SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
						// 427013 = Panel에 Panel ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437006));
						return;
						///* 확인 필요 : 삭제 할까?
					/*case ERR_SAME_PANELID_IN_LAST_5TIME:
						// 427014 = Panel ID가 최근 생산된 5개의 패널중 하나와 일치합니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(427014));
						return;
						//*/
					case ERR_NO_PANEL_DATA:
						SetErrorLevel(_T("Panel Transfer Center do Panel Validation Check"),0, __FILE__, __LINE__);
						// 427015 = Panel에 이전 설비로부터 넘겨받은 ID Data가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437007));
						return;
					}
				}
			}

			// 3. SendSart On
			m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, TRUE);
			// 4. SendStart 대기	
			m_timer.StartTimer();
			while (TRUE) 
			{
				Sleep(5);
				if (m_plnkMelsecIF->IsOnUpperEqBit(eSendStart)) 
					break;
				
				if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) 
				{
					InitializeMelsecIF();
					return;
				}
				if (m_timer.MoreThan(30.0)) 
				{
					InitializeMelsecIF();
					SetErrorLevel(_T("Panel Transfer Out send Panel"),0, __FILE__, __LINE__);
					// 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 SendStart 신호를 받지 못하였습니다.
					PROCESS_ALARM(generateErrorCode(437000));
					return;
				}
			}

			iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
			if (iResult) {
				SetErrorLevel(_T("PANEL TRANSFER Receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}

			iResult = m_plnkPanelTransfer->AbsorbPanel();
			if (iResult) {
				SetErrorLevel(_T("PANEL TRANSFER Receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
			

			// Doolin kwangilshin 2017. 09. 16.
			//
// 			//110929 sj_kjs 시점변경..
// 			m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_PANEL_TRANSFER_PANEL);
			// End.

			// 6. Arm1FoldComplete On
			m_plnkMelsecIF->SetUpperEqBit(eArm1FoldComplete, TRUE);

			m_timer.StartTimer();
			while (true) {
				Sleep(5);
				if (m_plnkMelsecIF->IsOnUpperEqBit(eGlassCheckSensorOn)) 
					break;

				if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) {
					InitializeMelsecIF();
					return;
				}
				if (m_timer.MoreThan(30.0)) {
					InitializeMelsecIF();
					SetErrorLevel(_T("Panel Transfer Out send Panel"),0, __FILE__, __LINE__);
					// 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 GlassCheckSensorOn 신호를 받지 못하였습니다.
					PROCESS_ALARM(generateErrorCode(437001));
					return;
				}
			}

			// 7. Receive Complete
			m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, TRUE);

			m_timer.StartTimer();
			while (true) {
				Sleep(5);
				if (m_plnkMelsecIF->IsOnUpperEqBit(eSendComplete)) break;

				if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) {
					InitializeMelsecIF();
					return;
				}
				if (m_timer.MoreThan(30.0)) {
					InitializeMelsecIF();
					SetErrorLevel(_T("Panel Transfer Out send Panel"),0, __FILE__, __LINE__);
					// 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 SendComplete 신호를 받지 못하였습니다.
					PROCESS_ALARM(generateErrorCode(437002));
					return;
				}
			}

			// Doolin kwangilshin 2017. 09. 16.
			//
			m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_PANEL_TRANSFER_PANEL);
			// End.


			// 8. Init
			InitializeMelsecIF();

			//100823.KKY.Add_____
			//m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
			//___________________
			//120224.kms________
		//	m_plnkMelsecIF->SetUpperEqBit(eReserved4_UpperEqBit, TRUE);
			//__________________
		}
		else
		{
			// Doolin kwangilshin 2017. 09. 16.
			//
			m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_PANEL_TRANSFER_PANEL);
			// End.
		}
#endif
		setStep(PANEL_TRANSFER_SEND_PANEL_READY);
		break;
		
	case PANEL_TRANSFER_SEND_PANEL_READY:

		if (!m_plnkITIPanelAligner->IsPanelTransferEnabletoMoveUnloadPos())
			break;

		//SJ_YYK 150318 Add..
		if(m_plnkTrsAutoManager->m_bSetAutoAlignPanelInStop == TRUE)
		{
			m_plnkTrsAutoManager->m_bSetAutoAlignPanelInStop = FALSE;
			SetPanelInStop(TRUE);
		}
		//____________________________*/

		MOlbSystemPre.GetPanelAlignerComponent();
		
		//170929 KDW Gear
		//iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
		iResult = m_plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
		
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(PANEL_TRANSFER_SEND_PANEL);
		break;

	case PANEL_TRANSFER_SEND_PANEL:

		if (m_plnkITIPanelAligner->IsReceiveAbleToPanelTransfer() == FALSE)
			break;

		m_bITISendStartToPanelAligner = TRUE;

		while (TRUE)
		{
			if (TRUE == m_plnkITIPanelAligner->IsReceiveStartToPanelTransfer())
				break;

			if (FALSE == m_plnkITIPanelAligner->IsReceiveAbleToPanelTransfer())
			{
				m_bITISendStartToPanelAligner = FALSE;
				return;
			}
			Sleep(10);
			
		}

		// Doolin kwangilshin 2017. 09. 12.
		//
		if (!m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
		{
			if (m_plnkStatePanelAligner->IsDownPickUpUnit() && m_plnkPanelTransfer->IsPanelAbsorbed())
			{
				//170929 KDW Gear
				//iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
				iResult = m_plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
		// End.

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			m_plnkStatePanelAligner->AbsorbPanel(TRUE);
		}

		iResult = m_plnkStatePanelAligner->UpPickUpUnit();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			iResult = m_plnkStatePanelAligner->AbsorbPanel();
			if (SUCCESS != iResult)
			{
				Sleep(200);
				
				if (m_plnkStatePanelAligner->IsUpPickUpUnit())
				{
					iResult = m_plnkStatePanelAligner->AbsorbPanel();
				}
				
				if (SUCCESS != iResult)
				{
					m_bITISendStartToPanelAligner = FALSE;
					m_plnkStatePanelAligner->ReleasePanel();
					m_plnkStatePanelAligner->DownPickUpUnit();
					
					// 424001 = Send Panel 동작 실패 [Panel Aligner 가 Panel 흡착 못함.]
					PROCESS_ALARM(generateErrorCode(424001));
					break;
				}
			}

			iResult = m_plnkPanelTransfer->ReleasePanel();
			if (iResult)
			{
				Sleep(300);
				
				iResult = m_plnkPanelTransfer->ReleasePanel();
				if (iResult)
				{
					m_plnkPanelTransfer->AbsorbPanel();
					m_plnkStatePanelAligner->ReleasePanel();
					m_plnkStatePanelAligner->DownPickUpUnit(TRUE);
					m_bITISendStartToPanelAligner = FALSE;
					
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}

 		iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
		if (iResult)
		{
			m_plnkPanelTransfer->UpPickUpUDCyl();
			m_plnkPanelTransfer->AbsorbPanel();
			m_plnkStatePanelAligner->ReleasePanel();
			m_plnkStatePanelAligner->DownPickUpUnit(TRUE);
			m_bITISendStartToPanelAligner = FALSE;
			
			PROCESS_ALARM(iResult);
			break;
		}

		m_plnkTrsAutoManager->TransferGlassData(DEF_PANEL_TRANSFER_PANEL, DEF_PANEL_ALIGNER_PANEL);

		m_bITISendCompleteToPanelAligner = TRUE;

		//SJ_YYK 150318 Add..
		if(m_plnkTrsAutoManager->m_bSetAutoAlignPanelInStop == TRUE)
		{
			m_plnkTrsAutoManager->m_bSetAutoAlignPanelInStop = FALSE;
			SetPanelInStop(TRUE);
		}
		//____________________________*/

		while (TRUE)
		{
			Sleep(5);
			if (FALSE == m_plnkITIPanelAligner->IsReceiveStartToPanelTransfer())
				break;
		}

		m_bITISendStartToPanelAligner = FALSE;
		m_bITISendCompleteToPanelAligner = FALSE;

		setStep(PANEL_TRANSFER_MOVE_TO_LOAD_POS);
		break;

	//NSMC KJS
	case PANEL_TRANSFER_MODEL_CHANGE: simulLog("PANEL_TRANSFER_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;

		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;

		m_bModelChangeReady = FALSE;

		setStep(PANEL_TRANSFER_MOVE_TO_LOAD_POS);
		break;

	default	:
		break;
	}
}

void MTrsPanelTransfer::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepPanelTransfer)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepPanelTransfer)0;	//  현재 Step Clear
}

void MTrsPanelTransfer::setStep(EStepPanelTransfer estepPanelTransfer)
{
	m_estepPrevious	= m_estepCurrent;	// 이전 Step 저장
	m_estepCurrent	= estepPanelTransfer;		// 새로 Step으로 전환
}

int MTrsPanelTransfer::AssignComponents(STrsPanelTransferRefCompList listRefComponents)
{
	m_plnkTrsAutoManager	= listRefComponents.m_plnkTrsAutoManager;
	m_plnkPanelTransfer		= listRefComponents.m_plnkPanelTransfer;
	m_plnkStatePanelAligner = listRefComponents.m_plnkStatePanelAligner;
//@	m_plnkStatePanelTransferCenter	= listRefComponents.m_plnkStatePanelTransferCenter;
	m_plnkMelsecIF			= listRefComponents.m_plnkMelsecIF;
	m_plnkLCNet				= listRefComponents.m_plnkLCNet;
	return ERR_TRS_PANEL_TRANSFER_SUCCESS;
}

int MTrsPanelTransfer::SetData(STrsPanelTransferData datComponent)
{
	m_plnkPanelData		= datComponent.m_plnkPanelData;
	m_plnkSystemData	= datComponent.m_plnkSystemData;

	return ERR_TRS_PANEL_TRANSFER_SUCCESS;
}

int MTrsPanelTransfer::Initialize()
{
	m_bAutoInitialized = FALSE;
	initializeITI();
	initializeStep();

	//@	m_plnkPanelTransfer->AirCVFloatingOn();

#ifndef SIMULATION
	int iResult = m_plnkPanelTransfer->CheckOrigin();
	if (iResult)
		return iResult;

	if (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE) {
		setStep(PANEL_TRANSFER_SEND_PANEL_READY);
	}
	else {
		// 부상 유닛에 Panel 있는지 확인
		// Panel 감지 센서의 오감지에 대한 확인 작업임. => 센서 오감지의 경우는 에러 처리 무시.
	/// 2010.08.11 KBC DryRunTest -------------------------------------------------
		if (TRUE == m_plnkPanelTransfer->IsPanelDetected()
			&& FALSE == m_plnkStatePanelAligner->IsPanelAbsorbed())
		{
			//110420.Add_____
//@			if (FALSE == m_plnkStatePanelTransferCenter->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS)
//@				|| FALSE == m_plnkStatePanelTransferCenter->IsPanelAbsorbed())
			{
			//_______________
				iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
				if (iResult)
					return iResult;

				iResult = m_plnkPanelTransfer->AbsorbPanel();
				m_plnkPanelTransfer->ReleasePanel();	// Error 처리하면 안됨.
				m_plnkPanelTransfer->DownPickUpUDCyl();	// Error 처리하면 안됨.

				// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
				if (SUCCESS == iResult)
				{
					// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
					return generateErrorCode(424006);
				}
			}
		}
	/// 2010.08.11 KBC DryRunTest -------------------------------------------------


		if (m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_PANEL]->m_strEPanelID.GetLength() > 1)		
		{
//			if (MyMessageBox(_T("PanelTransferIn has a Panel ID but released a vacuum. Scrap(delete) the Panel ID?\r\nPanel Scrap"), _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
			if (MyMessageBox(_T("PanelTransfer In이 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?\r\nPanelTransfer In is Vacuum Off state. but there is Panel ID. Do you want Panel Scrap?"), _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_PANEL);	
		}

		iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
		if (iResult)
			return iResult;
	}
#endif

	m_bAutoInitialized  = TRUE;
	return ERR_TRS_PANEL_TRANSFER_SUCCESS; 
}

void MTrsPanelTransfer::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

BOOL MTrsPanelTransfer::IsPanelInStop()
{
	//@130202.ePM삭제가 맞나요???
	return (m_bPanelInStop /*|| m_plnkLCNet->m_eEqState == ePM*/ || m_plnkTrsAutoManager->IsPanelInStopNSMC());
}

void MTrsPanelTransfer::SetPanelInStop(BOOL bSel)
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



void MTrsPanelTransfer::AssignITI(ITI_THandler* pITI_TurnHandler, 
								ITI_PanelAligner* pITI_PanelAligner)
{
	m_plnkITITurnHandler			= pITI_TurnHandler;
	m_plnkITIPanelAligner			= pITI_PanelAligner;
}


void MTrsPanelTransfer::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsPanelTransfer::ThreadStop()
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

void MTrsPanelTransfer::initializeITI()
{
	m_bITIReceiveAbleToPanelTransferCenter	= FALSE;
	m_bITIReceiveStartToPanelTransferCenter	= FALSE;

	m_bITIReceiveAbleToTurnHandler = FALSE; //SJ_YYK 161104 Add..
	m_bITIReceiveCompleteToTurnHandler	= FALSE; //SJ_YYK 161104 Add.

	m_bITISendStartToPanelAligner			= FALSE;
	m_bITISendCompleteToPanelAligner		= FALSE;

	// NSMC
	m_bModelChangeReady					= FALSE;
}

BOOL MTrsPanelTransfer::IsInitialized()
{
	return m_bAutoInitialized;
}

BOOL MTrsPanelTransfer::IsReceiveAbleToPanelTransferCenter()
{
	return m_bITIReceiveAbleToPanelTransferCenter;
}

BOOL MTrsPanelTransfer::IsReceiveStartToPanelTransferCenter()
{
	return m_bITIReceiveStartToPanelTransferCenter;
}
//SJ_YYK 161104 Add.
BOOL MTrsPanelTransfer::IsReceiveAbleToTurnHandler()
{
	return m_bITIReceiveAbleToTurnHandler;
}

BOOL MTrsPanelTransfer::IsReceiveCompleteToTurnHandler()
{
	return m_bITIReceiveCompleteToTurnHandler;
}
//________________________________*/

BOOL MTrsPanelTransfer::IsSendStartToPanelAligner()
{
	return m_bITISendStartToPanelAligner;
}

BOOL MTrsPanelTransfer::IsSendCompleteToPanelAligner()
{
	return m_bITISendCompleteToPanelAligner;
}

BOOL MTrsPanelTransfer::IsEmptyPanel()
{
	BOOL bResult = m_plnkPanelTransfer->IsPanelReleased();
	return bResult;
}

int MTrsPanelTransfer::GetStep()
{
	return m_estepCurrent;
}

BOOL MTrsPanelTransfer::IsAutoRun()
{
	return m_bAutoRun;
}

CString MTrsPanelTransfer::GetItiStatus()
{
	m_strItiStatus.Format(_T("ReceiveA: %d, ReceiveS: %d, SendS: %d, SendC: %d"), 
		m_bITIReceiveAbleToPanelTransferCenter, m_bITIReceiveStartToPanelTransferCenter,
		m_bITISendStartToPanelAligner, m_bITISendCompleteToPanelAligner);

	return m_strItiStatus;
}

int MTrsPanelTransfer::ScrapPanel()
{
	int iResult = SUCCESS;

	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_PANEL);

	if (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed())
	{
		CString strMsg = _T("There is a panel on Panel Transfer. Remove it.");
		
		do
		{
			iResult = m_plnkPanelTransfer->ReleasePanel();
			if (iResult)
				return iResult;
			
			Sleep(500);

			if(MyMessageBox(MY_MESSAGE_BOX, 62600, _T("Question Scrap"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) ==IDNO)
				return FALSE;

//			MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Remove Panel Request", M_ICONINFORMATION);
			MyMessageBox(MY_MESSAGE_BOX, 62100, "Remove Panel Request", M_ICONINFORMATION);

			m_plnkPanelTransfer->AbsorbPanel(TRUE);
			Sleep(300);

		}while (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed());
	}
	
	m_plnkPanelTransfer->ReleasePanel(TRUE);

	return Initialize();
}

// NSMC
BOOL MTrsPanelTransfer::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsPanelTransfer::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

// End Of Code

