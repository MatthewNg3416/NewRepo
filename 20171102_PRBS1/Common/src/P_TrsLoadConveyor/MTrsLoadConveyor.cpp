/* 
 * TrsLoadConveyor Component
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
 * MTrsLoadConveyor.cpp : Implementation of MTrsLoadConveyor component.
 */

#include "Stdafx.h"
#include "MPlatformOlbSystem.h"
#include "MNetH.h"
#include "MLCNet.h"
//#include "MRollerConveyor.h"
#include "MPanelTransfer.h"
#include "MTrsLoadConveyor.h"
#include "MTrsAutoManager.h"
#include "ITI_PanelAligner.h"
#include "ITI_PanelTransfer.h"
#ifdef DEF_TEST
#include "IOpPanel.h"
#endif

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
#include "DefIOAddrPreBonder.h"
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
#include "DefIOAddrAcf.h"
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
#include "DefIOAddrFinal.h"
#endif

MTrsLoadConveyor::MTrsLoadConveyor(	SCommonAttribute			commonData, 
									STrsLoadConveyorRefCompList	listRefComponents, 
									STrsLoadConveyorData		datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	m_bAutoInitialized	= FALSE;

	m_bThreadLife		= FALSE;
	m_pThread			= NULL;
	m_bAutoRun			= FALSE;

	initializeITI();
	initializeStep();

	m_bPanelInStop = FALSE;
}

MTrsLoadConveyor::~MTrsLoadConveyor()
{
}

void MTrsLoadConveyor::simulLog(CString strMsg, int iStep, int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s[%d] : %s(%d)\n"), GetObjectName(), GetInstanceNo(), strMsg, iStep);

	FILE *fp = fopen(".\\StepLog.txt", "a");
	fprintf(fp, "%s\n", strLog);
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

UINT MTrsLoadConveyor::threadFunction(LPVOID pParam)
{
	// TrsLoadConveyor Object Pointer Assign
	MTrsLoadConveyor* pThis = (MTrsLoadConveyor*) pParam;

	TRACE(_T("MTrsLoadConveyor Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsLoadConveyor Thread Stop\n"));

	return ERR_TRS_LOAD_CONVEYOR_SUCCESS;
}

void MTrsLoadConveyor::threadJob()
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
		case STEP_STOP:
			initializeMelsecIF();
			m_plnkLoadConveyor->StopMoving();
			m_bAutoRun = FALSE;
			break;
			
		case START_RUN:
			initializeMelsecIF();
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

void MTrsLoadConveyor::doRunStep()
{
	int iResult = ERR_TRS_LOAD_CONVEYOR_SUCCESS;

	switch (m_estepCurrent)
	{
	case LOAD_CONVEYOR_RECEIVE_PANEL_READY: simulLog("LOAD_CONVEYOR_RECEIVE_PANEL_READY", m_estepCurrent);

		//이전 설비로 부터 Panel 받을 준비
		//1.PanelAligner로부터 동시 동작을 위해 신호를 받아야 한다.
		// - LoadConveyorEnabletoReceivePanel 신호 대기
		// - Panel Aligner에서 보내는 신호, Tact 단축 필요시 Panel Transfer에서 신호보낸다.
		// - LoadConveyor가 신호 Reset 한다.
		//2.Stopper Up

		if (!m_plnkITIPanelTransfer->IsLoadConveyorEnabletoReceivePanel())
			break;

#if defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
		if (m_plnkStatePanelTransfer->IsPanelAbsorbed())
		{
			if (!m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
				break;
		}
#endif

		// Cassette 하강시 Stopper에 닿음...
#ifndef SIMULATION
		// jdy sesl 090102	: cassette 제거할 때 Stopper Down 필요.
		//                     Panel Prealign 기능 사용할 경우 Stopper Up 안되면 안됨.
		if (TRUE == m_plnkSystemData->m_bUsePanelPreAlign)
		{
			// 만약 하강한 Cassette 와 L/C Stopper 가 간섭 발생하는 설비라면...?
			// 어떤 설비에서 문제 있는지 확인해서 이 부분 코드 정리해야 한다.
			// 일단 SESL 13,14 호기는 간섭 없음.
			iResult = m_plnkLoadConveyor->UpStopperUDCyl();
			if (iResult)
			{
				SetErrorLevel(_T("LOAD CONVEYOR ready to Receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
		else
		{
			// 여기도 간섭 여부 확인해서 코드 불필요할 경우 제거할 것.
#	if	!(defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_SOURCE_SYSTEM))
			if (FALSE == m_plnkITIPanelAligner->IsPanelTransferEnabletoStopperUp())
			{
				iResult = m_plnkLoadConveyor->UpStopperUDCyl();
				if (iResult)
				{
					SetErrorLevel(_T("LOAD CONVEYOR ready to Receive Panel"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					break;
				}
			}
#	endif
		}
/* jdy sesl 090102
#if	!(defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_SOURCE_SYSTEM))
		if (FALSE == m_plnkITIPanelAligner->IsPanelTransferEnabletoStopperUp())
		{
			iResult = m_plnkLoadConveyor->UpStopperUDCyl();
			if (iResult)
			{
				SetErrorLevel(_T("LOAD CONVEYOR ready to Receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
#endif
*/
#endif		
		setStep(LOAD_CONVEYOR_RECEIVE_PANEL);
		break;
		
	case LOAD_CONVEYOR_RECEIVE_PANEL: simulLog("LOAD_CONVEYOR_RECEIVE_PANEL", m_estepCurrent);
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(LOAD_CONVEYOR_MODEL_CHANGE);
			break;
		}

		if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		{
			// Dry Run 에서는 Conveyor 회전 동작 Error 처리는 불필요 : 어차피 Panel 없으므로..
			m_plnkLoadConveyor->MoveForward();
			Sleep(1000);
			m_plnkLoadConveyor->ReduceSpeed();
			Sleep(1000);
			m_plnkLoadConveyor->StopMoving();
			initializeMelsecIF();
			
			setStep(LOAD_CONVEYOR_SEND_PANEL_READY);
			break;
		}

#if defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
		if (m_plnkLoadConveyor->IsPanelDecelDetected() && m_plnkITIPanelTransfer->IsReceiveAbleToLoadConveyor())
		{
			setStep(LOAD_CONVEYOR_SEND_PANEL);
			break;			
		}
#endif

		// Panel 투입중지 버튼 눌러진 경우
		if (TRUE == IsPanelInStop())
		{
			// 이전 설비 I/F Time Over Error 방지.
			m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
			break;
		}

#if (defined(DEF_PLATFORM_ARP_SYSTEM) && defined(DEF_TEST) && defined(DEF_GATE_SYSTEM))
		if (FALSE == m_plnkOpPanel->GetResetButtonStatus() || FALSE == m_plnkLoadConveyor->IsPanelDetected())
			break;

		if (m_plnkSystemData->m_bUsePanelPreAlign)
		{
			iResult = m_plnkLoadConveyor->MoveForward();
			if (iResult)
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}

			// PreAlign 전진 
			iResult = m_plnkLoadConveyor->ForwardPanelPreAlignFBCyl();
			if (iResult)
			{
				m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
			
			//20081110 by kss
			// 안정화 시간 
			Sleep(1000);

			iResult = m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
			if (iResult)
			{
				m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}

			iResult = m_plnkLoadConveyor->StopMoving();
			if (iResult)
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
#else

		m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, TRUE);

		// jdy sesl 090102 : 여기서 Stopper 안올라가 있으면 문제 있을 수 있다.
		//                     Panel Prealign 기능 사용할 경우 Stopper Up 안되면 안됨.
		if (TRUE == m_plnkSystemData->m_bUsePanelPreAlign)
		{
			iResult = m_plnkLoadConveyor->UpStopperUDCyl();
			if (iResult)
			{
				SetErrorLevel(_T("LOAD CONVEYOR ready to Receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
		if (!m_plnkMelsecIF->IsOnUpperEqBit(eSendAble))
			break;
		
		m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, TRUE);

		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkMelsecIF->IsOnUpperEqBit(eSendStart))
				break;

			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble))
			{
				initializeMelsecIF();
				return;
			}

			if (m_timer.MoreThan(30.0))
			{
				initializeMelsecIF();
				SetErrorLevel(_T("LOAD CONVEYOR ready to Receive Panel"),0, __FILE__, __LINE__);
				// 427001 = 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 SendStart 신호를 받지 못하였습니다.
				PROCESS_ALARM(generateErrorCode(427001));
				return;
			}

			Sleep(1);
		}

		// 이전 설비로부터 Panel Data 가져 온다. => Interface 도중 Error 나더라도 Panel Data 유실 방지.
		m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_LOAD_CONVEYOR_PANEL);

#if defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM)
		// Panel Data Reading Complete Signal 로 사용.
		m_plnkMelsecIF->SetUpperEqBit(eGlassCheckSensorOn, TRUE);
#endif

		if (FALSE == m_plnkLoadConveyor->IsPanelDecelDetected())
		{
			iResult = m_plnkLoadConveyor->MoveForward();
			if (iResult)
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}

		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkLoadConveyor->IsPanelDecelDetected())
				break;

			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)
				|| FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendStart))
			{
				// 이 경우는 이전 설비의 Unload Conveyor 에서 Inverter Move Forward 시 Error 가 발생한 것이다.
				// 따라서, Panel 은 아직 현재 설비로 넘어오지 않았다.

				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				return;
			}

			if (m_timer.MoreThan(30.0))
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();

				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427000 = 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 감속센서가 감지되지 않았습니다.
				PROCESS_ALARM(generateErrorCode(427000));
				return;
			}

			Sleep(1);
		}

// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
		//20081110 by kss
		// Panel이 Stopper에 닿을 때까지 전진한다. 
		if (!m_plnkSystemData->m_bUsePanelPreAlign)
		{
			if (FALSE == m_plnkLoadConveyor->IsPanelStopDetected())
			{
				iResult = m_plnkLoadConveyor->ReduceSpeed();
				if (iResult)
				{
					m_plnkLoadConveyor->StopMoving();
					initializeMelsecIF();
					
					SetErrorLevel(_T("LOAD CONVEYOR run Step"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
		else
		{
			iResult = m_plnkLoadConveyor->ReduceSpeed();
			if (iResult)
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
/* jdy sesl 081229	
#else
		if (FALSE == m_plnkLoadConveyor->IsPanelStopDetected())
		{
			iResult = m_plnkLoadConveyor->ReduceSpeed();
			if (iResult)
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
#endif
*/
		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkLoadConveyor->IsPanelStopDetected())
				break;

			if (m_timer.MoreThan(30.0))
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427002 = 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 정지센서가 감지되지 않았습니다.
				PROCESS_ALARM(generateErrorCode(427002));
				return;
			}

			Sleep(1);
		}


		// SendComplete 신호대기
		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkMelsecIF->IsOnUpperEqBit(eSendComplete))
				break;

			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)
				|| FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendStart))
			{
				// 이 경우는 Melsec Cable 끊어진 경우라고 봐야 한다. (전 설비 동작 Error 에 의한 신호 꺼짐 아님.)
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427003 = 이전 설비로부터 Panel을 넘겨받는 중 이전 설비의 SendAble, SendStart 신호가 꺼졌습니다. [Melsec Net 확인 필요]
				PROCESS_ALARM(generateErrorCode(427003));
				return;
			}

			// time over 발생 불가 : Melsec Cable 이 끊어져도 바로 위 조건에 의해 처리됨.
			
			// 만약을 대비한 time over check
			// : 원인을 알 수 없는 문제에 의해 SendComplete 신호만 On 되지 않을 경우, Error Display.
			if (m_timer.MoreThan(30.0))
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427004 = 이전 설비로부터 Panel을 넘겨받은 후 30초 동안 SendComplete 신호를 받지 못하였습니다.
				PROCESS_ALARM(generateErrorCode(427004));
				return;
			}

			Sleep(1);
		}

		// 이전 설비로 ReceiveComplete 신호 보낸다.
		m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, TRUE);

		// SendAble ~ SendComplete 신호 Reset 대기
		m_timer.StartTimer();
		while (TRUE)
		{
			// 원칙 : SendAble, Start, Complete All Off 확인해야 하나,
			//		  편의상 Complete Off 만 확인한다.
			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendComplete))
				break;

			// 이 시점에서 Time Over 는 발생할 수 없다. (Melsec Cable 이 끊어지더라도...)
			// 만약을 대비한 안정 장치로 time over check
			//  => time over 발생 시, 이미 현재 설비는 Panel 받는 동작을 완료했으므로 정상 Step 을 진행한다.
			if (m_timer.MoreThan(30.0))
				break;

			Sleep(1);
		}

///////////////////////////////////////////////////////////////////////////////////
		// 2009.04.22 SESL_CYJ 앞 설비와의 I/F 시간 단축 
		// 수정 이유 : Panel Prealign 동작이 끝나야 I/F 를 완료해서 2~3초 Delay 발생
		// All Interface Signal Reset
		initializeMelsecIF();

// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
		if (m_plnkSystemData->m_bUsePanelPreAlign)
		{
			//20081110 by kss
			// 안정화 시간 
			Sleep(1000);
			
			//20081110 by kss
			// PreAlign 전진 
			iResult = m_plnkLoadConveyor->ForwardPanelPreAlignFBCyl();
			if (iResult)
			{
				m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
			
			//20081110 by kss
			// 안정화 시간 
			//Sleep(1000);
			// Data 설정 시간 만큼 Stopper에 도달하도록 대기한다. syr_090311
			Sleep((ULONG)(m_plnkSystemData->m_dPanelTabDist) * 1000);
		}
// jdy sesl 081229	#endif

		iResult = m_plnkLoadConveyor->StopMoving();
		if (iResult)
		{
			m_plnkLoadConveyor->StopMoving();
			initializeMelsecIF();
			
			SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			setStep(LOAD_CONVEYOR_SEND_PANEL_READY);
			break;
		}
///////////////////////////////////////////////////////////////////////////////////////

// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
		if (m_plnkSystemData->m_bUsePanelPreAlign)
		{
			//20081110 by kss
			// 안정화 시간 
			Sleep(1000);
			
			//20081110 by kss
			// PreAlign 후진 
			iResult = m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
			if (iResult)
			{
				initializeMelsecIF();
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
// jdy sesl 081229	#endif


		if (FALSE == m_plnkLoadConveyor->IsDownStopperUDCyl())
			m_plnkLoadConveyor->DownStopperUDCyl(TRUE);

#endif
		setStep(LOAD_CONVEYOR_SEND_PANEL_READY);
		break;

	case LOAD_CONVEYOR_SEND_PANEL_READY: simulLog("LOAD_CONVEYOR_SEND_PANEL_READY", m_estepCurrent);

		// Panel Validation Check
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE && m_plnkLCNet->m_eEqState != ePM)
		{
			if (m_plnkSystemData->m_bUsePanelValidation == TRUE)
			{
				iResult = m_plnkTrsAutoManager->CheckPanelValidation(DEF_LOAD_CONVEYOR_PANEL);
				switch (iResult)
				{
				case ERR_NO_BATCHID_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427011 = Panel에 Batch ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427011));
					return;
				case ERR_NO_DEVICEID_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427012 = Panel에 Device ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427012));
					return;
				case ERR_NO_PANELID_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427013 = Panel에 Panel ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427013));
					return;
/* 확인 필요 : 삭제 할까?
				case ERR_SAME_PANELID_IN_LAST_5TIME:
					// 427014 = Panel ID가 최근 생산된 5개의 패널중 하나와 일치합니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427014));
					return;
*/
				case ERR_NO_PANEL_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427015 = Panel에 이전 설비로부터 넘겨받은 ID Data가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427015));
					return;
				}
			}
		}

		setStep(LOAD_CONVEYOR_SEND_PANEL);
		break;

	case LOAD_CONVEYOR_SEND_PANEL: simulLog("LOAD_CONVEYOR_SEND_PANEL", m_estepCurrent);
		//PanelTransfer로 Panel을 보낸다.

		if (FALSE == m_plnkLoadConveyor->IsDownStopperUDCyl())
		{
			iResult = m_plnkLoadConveyor->DownStopperUDCyl();
			if (iResult)
			{
				SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}

		// PanelTransfer로부터 ReceiveAble 신호를 기다린다.
		if (!m_plnkITIPanelTransfer->IsReceiveAbleToLoadConveyor())
		{
			if (FALSE == m_plnkLoadConveyor->IsStopMoving())
				m_plnkLoadConveyor->StopMoving();

			break;
		}

		m_bITISendStartToPanelTransfer = TRUE;

		while (TRUE)
		{
			if (m_plnkITIPanelTransfer->IsReceiveStartToLoadConveyor())
				break;

			if (FALSE == m_plnkITIPanelTransfer->IsReceiveAbleToLoadConveyor())
			{
				if (FALSE == m_plnkLoadConveyor->IsStopMoving())
					m_plnkLoadConveyor->StopMoving();

				m_bITISendStartToPanelTransfer = FALSE;
				return;
			}
			
			Sleep(1);
		}

		// PanelTransfer로 전해주는 속도가 따로 있음
		iResult = m_plnkLoadConveyor->MoveForwardTransferSpeed();
		if (iResult)
		{
			m_bITISendStartToPanelTransfer = FALSE;
			m_plnkLoadConveyor->StopMoving();
			
			SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}

		// hongju 수정..Test 필요
		// Panel 오감지 하는 것을 막기 위하여 Sleep()을 주고 Panel 감지 센서 동작 시킴
//		Sleep(300);
		//2009.11.11 CYJ Panel Aligner Unload & Panel Transfer Receive Panel 동시 동작
		Sleep(1000);
		/*KKY.Del
		iResult = m_plnkStatePanelTransfer->UpStopperUDCyl();
		if (iResult)
		{
			m_bITISendStartToPanelTransfer = FALSE;
			m_plnkLoadConveyor->StopMoving();

			SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}
		*/
		m_timer.StartTimer();
		while (TRUE)
		{
			if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
			{
				Sleep(1000);
				break;
			}

			if (m_plnkStatePanelTransfer->IsPanelDetected())
			{
#ifdef DEF_PLATFORM_ARP_SYSTEM // 2009.04.14 SESL_CYJ ARP만 적용되도록 수정.
				// 2009.04.07 SESL_CYJ, SESL_HJH Carrier 를 Panel 로 오감지 발생하여 Carrier 이동시간 대기 후 재인식 확인 후 진행.
				Sleep(300);
				if (m_plnkStatePanelTransfer->IsPanelDetected())
#endif
					break;
			}
			
			if (m_timer.MoreThan(30.0))
			{
				m_bITISendStartToPanelTransfer = FALSE;
				m_plnkLoadConveyor->StopMoving();

				SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
				// 427005 = Panel Transfer로 Panel을 넘겨주는 중 30초 동안 Panel Transfer의 감속센서가 감지되지 않았습니다.
				PROCESS_ALARM(generateErrorCode(427005));
				return;
			}
			
			Sleep(1);
		}

		iResult = m_plnkLoadConveyor->ReduceSpeed();
		if (iResult)
		{
			m_bITISendStartToPanelTransfer = FALSE;
			m_plnkLoadConveyor->StopMoving();

			SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}

		// Data 설정 시간 만큼 Stopper에 도달하도록 대기한다.
		Sleep((ULONG)(m_plnkSystemData->m_dStopperArrivingDelay) * 1000);

		iResult = m_plnkLoadConveyor->StopMoving();
		if (iResult)
		{
			m_bITISendStartToPanelTransfer = FALSE;
			m_plnkLoadConveyor->StopMoving();

			SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{		
			m_plnkStatePanelTransfer->AbsorbPanel(TRUE);
			
			iResult = m_plnkStatePanelTransfer->UpPickUpUDCyl();
			if (iResult)
			{
				m_bITISendStartToPanelTransfer = FALSE;

				SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
			
			iResult = m_plnkStatePanelTransfer->AbsorbPanel();
			if (iResult)
			{
				iResult = m_plnkStatePanelTransfer->AbsorbPanel();
				if (iResult)
				{
					m_bITISendStartToPanelTransfer = FALSE;

					SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}

		m_plnkTrsAutoManager->TransferGlassData(DEF_LOAD_CONVEYOR_PANEL, DEF_PANEL_TRANSFER_PANEL);

		m_bITISendCompleteToPanelTransfer = TRUE;

		while (TRUE)
		{
			if (FALSE == m_plnkITIPanelTransfer->IsReceiveStartToLoadConveyor())
				break;
			
			Sleep(1);
		}

		m_bITISendStartToPanelTransfer = FALSE;
		m_bITISendCompleteToPanelTransfer = FALSE;

		setStep(LOAD_CONVEYOR_RECEIVE_PANEL_READY);
		break;

	//NSMC KJS
	case LOAD_CONVEYOR_MODEL_CHANGE: simulLog("LOAD_CONVEYOR_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;

		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;

		m_bModelChangeReady = FALSE;

		setStep(LOAD_CONVEYOR_RECEIVE_PANEL_READY);
		break;

	default:
		break;
	}
}

void MTrsLoadConveyor::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepLoadConveyor)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepLoadConveyor)0;	//  현재 Step Clear
}

void MTrsLoadConveyor::setStep(EStepLoadConveyor estepLoadConveyor)
{
	m_estepPrevious	= m_estepCurrent;		// 이전 Step 저장
	m_estepCurrent	= estepLoadConveyor;	// 새로 Step으로 전환
}

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

/**
* Assign component linkage
* 
* @param	listRefComponents	: MTrsLoadConveyor 참조 Component list
*/
int MTrsLoadConveyor::AssignComponents(STrsLoadConveyorRefCompList listRefComponents)
{
	m_plnkTrsAutoManager		= listRefComponents.m_plnkTrsAutoManager;
	m_plnkLoadConveyor			= listRefComponents.m_plnkLoadConveyor;
	m_plnkStatePanelTransfer	= listRefComponents.m_plnkStatePanelTransfer;
	m_plnkMelsecIF				= listRefComponents.m_plnkMelsecIF;
	m_plnkLCNet					= listRefComponents.m_plnkLCNet;

#ifdef DEF_TEST
	m_plnkOpPanel				= listRefComponents.m_plnkOpPanel;
#endif

	return ERR_TRS_LOAD_CONVEYOR_SUCCESS;
}

/** 
* Component Data 처리 
*
* @param	pData : SMMTrsLoadConveyorData 구조체에 대한 포인터
* @return	Error : 0 = Success, 그외=기타
*/
int MTrsLoadConveyor::SetData(STrsLoadConveyorData datComponent)
{
	m_plnkSystemData = datComponent.m_plnkSystemData;
	return ERR_TRS_LOAD_CONVEYOR_SUCCESS;
}

/** 
 * MTrsLoadConveyor Component의 자동 운전 초기화를 시작한다.
 * @return	Error Code : 0 = SUCCESS, 그 외 = Error
 */
int MTrsLoadConveyor::Initialize()
{
	m_bAutoInitialized = FALSE;
	initializeMelsecIF();
	initializeITI();
	initializeStep();

#ifndef SIMULATION
	int iResult = ERR_TRS_LOAD_CONVEYOR_SUCCESS;

#	ifndef DEF_SETUP
	if (FALSE == m_plnkLoadConveyor->IsUpStopperUDCyl()
		&& FALSE == m_plnkLoadConveyor->IsDownStopperUDCyl())
	{
		// 427006 = Conveyor 로부터 Cylinder 상태 신호를 받지 못함. [Conveyor 분리 여부 확인 필요.]
		return generateErrorCode(427006);
	}

#	endif

	// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
	if (m_plnkSystemData->m_bUsePanelPreAlign)
	{
		//20081110 by kss
		// PreAlign 후진 
		iResult = m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
		if (iResult)
			return iResult;
	}
// jdy sesl 081229#endif

	// 080722 이정용 수정 -> 물류Test시 초기화 할때 Stopper up 추가 (문학일씨 요청사항)
	if(m_plnkLoadConveyor->IsPanelDetected())
	{
		
#	if (defined(DEF_PLATFORM_ARP_SYSTEM) && defined(DEF_TEST) && defined(DEF_GATE_SYSTEM))
		if (m_plnkLoadConveyor->IsDownStopperUDCyl())
			iResult = m_plnkLoadConveyor->UpStopperUDCyl();
#	else
		iResult = m_plnkLoadConveyor->DownStopperUDCyl();
#	endif
		if (iResult)
			return iResult;
	}
	else
	{
		//20081110 by kss
		// 초기화시에 다음 Panel을 정지시키기 위해 Stopper를 Up시켜놓는다. 
		iResult = m_plnkLoadConveyor->UpStopperUDCyl();
		if (iResult)
			return iResult;
	}

	iResult = m_plnkLoadConveyor->StopMoving();
	if (iResult)
	{
		m_plnkLoadConveyor->StopMoving();
		return iResult;
	}

	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		m_bAutoInitialized = TRUE;
		return ERR_TRS_LOAD_CONVEYOR_SUCCESS;
	}

	if (TRUE == m_plnkStatePanelTransfer->IsPanelAbsorbed()
		&& TRUE == m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
	{
		m_bAutoInitialized = TRUE;	
		return ERR_TRS_LOAD_CONVEYOR_SUCCESS; 
	}

	// Panel 감지센서에 Panel 감지 되지 않을 때
	if (FALSE == m_plnkLoadConveyor->IsPanelDetected())
	{
		// Panel 유무 확인
		iResult = m_plnkLoadConveyor->MoveForwardReducedSpeed();
		if (iResult)
		{
			m_plnkLoadConveyor->StopMoving();
			return iResult;
		}

		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkLoadConveyor->IsPanelDetected())
			{
				iResult = m_plnkLoadConveyor->StopMoving();
				if (iResult)
				{
					m_plnkLoadConveyor->StopMoving();
					return iResult;
				}

				iResult = m_plnkLoadConveyor->DownStopperUDCyl();
				if (iResult)
					return iResult;

				break;
			}

			if (m_timer.MoreThan(10.0))
			{
				iResult = m_plnkLoadConveyor->StopMoving();
				if (iResult)
				{
					m_plnkLoadConveyor->StopMoving();
					return iResult;
				}
				break;
			}

			Sleep(1);
		}

		// hongju_SESL Load Conveyor에 Panel이 감지되지 않으면 무조건 Scrap 진행??
/*		if (m_plnkTrsAutoManager->m_pGlassData[DEF_LOAD_CONVEYOR_PANEL]->m_strEPanelID.GetLength() > 1)		
		{
		//	if (MyMessageBox(MY_MESSAGE_BOX,65500, _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
			m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_LOAD_CONVEYOR_PANEL);	
		}
*/
	}

	if (TRUE == m_plnkLoadConveyor->IsPanelStopDetected()
		|| TRUE == m_plnkLoadConveyor->IsPanelDecelDetected())
	{
		setStep(LOAD_CONVEYOR_SEND_PANEL);
	}
#endif /* SIMULATION */
	
	m_bAutoInitialized = TRUE;
	return ERR_TRS_LOAD_CONVEYOR_SUCCESS; 
}

/**
 * MTrsLoadConveyor Initial 완료 여부를 해제한다.
 *
 * @return void
 */
void MTrsLoadConveyor::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

BOOL MTrsLoadConveyor::IsPanelInStop()
{
	return (m_bPanelInStop || m_plnkLCNet->m_eEqState == ePM || m_plnkTrsAutoManager->IsPanelInStopNSMC());
}

void MTrsLoadConveyor::SetPanelInStop(BOOL bSel)
{
	m_bPanelInStop = bSel;
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/

void MTrsLoadConveyor::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsLoadConveyor::ThreadStop()
{
	DWORD exitcode;

	m_bThreadLife = FALSE;

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
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/

void MTrsLoadConveyor::AssignITI(ITI_PanelAligner* pITI_PanelAligner, ITI_PanelTransfer* pITI_PanelTransfer)
{
	m_plnkITIPanelAligner	= pITI_PanelAligner;
	m_plnkITIPanelTransfer	= pITI_PanelTransfer;
}

void MTrsLoadConveyor::initializeITI()
{
	m_bITISendStartToPanelTransfer		= FALSE;
	m_bITISendCompleteToPanelTransfer	= FALSE;
}

/**
 * Initial 완료 여부 확인
 *
 * @return BOOL (TRUE : Initial 완료, FALSE : 미완료)
 */
BOOL MTrsLoadConveyor::IsInitialized()
{
	return m_bAutoInitialized;
}

BOOL MTrsLoadConveyor::IsSendStartToPanelTransfer()
{
	return m_bITISendStartToPanelTransfer;
}

BOOL MTrsLoadConveyor::IsSendCompleteToPanelTransfer()
{
	return m_bITISendCompleteToPanelTransfer;
}

BOOL MTrsLoadConveyor::IsEmptyPanel()
{
	BOOL bPanelDetected = m_plnkLoadConveyor->IsPanelDetected();
	return !bPanelDetected;
}

int	MTrsLoadConveyor::GetStep()
{
	return m_estepCurrent;
}

BOOL MTrsLoadConveyor::IsAutoRun()
{
	return m_bAutoRun;
}

CString MTrsLoadConveyor::GetItiStatus()
{
	m_strItiStatus.Format(_T("SendS PanelTransfer: %d, SendC PanelTransfer: %d"),
		m_bITISendStartToPanelTransfer, m_bITISendCompleteToPanelTransfer);

	return m_strItiStatus;
}

void MTrsLoadConveyor::initializeMelsecIF()
{
	m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eImmediatelyPauseRequest, FALSE);

#if defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM)
	m_plnkMelsecIF->SetUpperEqBit(eGlassCheckSensorOn, FALSE);
#endif
}

int MTrsLoadConveyor::ScrapPanel()
{
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
		return ERR_TRS_LOAD_CONVEYOR_SUCCESS;

	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_LOAD_CONVEYOR_PANEL);

	m_bAutoInitialized = FALSE;
	initializeMelsecIF();
	initializeITI();
	initializeStep();

	int iResult = m_plnkLoadConveyor->StopMoving();
	if (iResult)
	{
		m_plnkLoadConveyor->StopMoving();
		return iResult;
	}

	if (TRUE == m_plnkLoadConveyor->IsPanelDetected())
	{
		if (TRUE == m_plnkStatePanelTransfer->IsPanelAbsorbed()
			&& TRUE == m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
		{
			return ERR_TRS_LOAD_CONVEYOR_SUCCESS; 
		}
		
		CString strMsg = _T("There is a panel on Load Conveyor. Remove it.");
		do
		{
			Sleep(1);
//			MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Remove Panel Request", M_ICONINFORMATION);
			MyMessageBox(MY_MESSAGE_BOX, 62100, "Remove Panel Request", M_ICONINFORMATION);
		} while (TRUE == m_plnkLoadConveyor->IsPanelDetected());
	}

	m_bAutoInitialized = TRUE;
	return ERR_TRS_LOAD_CONVEYOR_SUCCESS; 
}

// NSMC
BOOL MTrsLoadConveyor::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}
// End Of Code
