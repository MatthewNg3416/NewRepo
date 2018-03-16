/* 
 * TrsUnloadConveyor Component
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
 * MTrsUnloadConveyor.cpp : Implementation of MTrsUnloadConveyor component.
 */

#include "Stdafx.h"
#include "MPlatformOlbSystem.h"
#include "MNetH.h"
#include "MRollerConveyor.h"
#include "DefPanelAligner.h"
#include "IStatePanelAligner.h"
#include "MTrsUnloadConveyor.h"
#include "MTrsAutoManager.h"
#include "ITI_PanelAligner.h"
#include "ITI_PanelTransfer.h"
#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM))
#include "ITI_OutConveyor.h"
#endif

// 2009.05.22 hjh ver3 && D군 OutConveyor 없음.
#ifdef DEF_TEST
#include "MOpPanel.h"
#endif

MTrsUnloadConveyor::MTrsUnloadConveyor(	SCommonAttribute				commonData, 
										STrsUnloadConveyorRefCompList	listRefComponents, 
										STrsUnloadConveyorData			datComponent)
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
}

MTrsUnloadConveyor::~MTrsUnloadConveyor()
{
}

void MTrsUnloadConveyor::simulLog(CString strMsg, int iStep, int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s[%d] : %s(%d)\n"), GetObjectName(), GetInstanceNo(), strMsg, iStep);

	FILE *fp = fopen( ".\\StepLog.txt", "a");
	fprintf(fp, "%s\n", strLog);
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

UINT MTrsUnloadConveyor::threadFunction(LPVOID pParam)
{
	// TrsUnloadConveyor Object Pointer Assign
	MTrsUnloadConveyor* pThis = (MTrsUnloadConveyor*) pParam;

	TRACE(_T("MTrsUnloadConveyor Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsUnloadConveyor Thread Stop\n"));

	return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS;
}

void MTrsUnloadConveyor::threadJob()
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
			m_bITIReceiveAbleToPanelAligner = FALSE;
#if !(defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM))
			initializeMelsecIF();
#endif
			m_plnkUnloadConveyor->StopMoving();
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

void MTrsUnloadConveyor::doRunStep()
{
	int iResult = ERR_TRS_UNLOAD_CONVEYOR_SUCCESS;

	switch (m_estepCurrent)
	{
	case UNLOAD_CONVEYOR_RECEIVE_PANEL: simulLog("UNLOAD_CONVEYOR_RECEIVE_PANEL_READY", m_estepCurrent);
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(UNLOAD_CONVEYOR_MODEL_CHANGE);
			break;
		}	

		iResult = m_plnkUnloadConveyor->StopMoving();
		if (iResult)
		{
			SetErrorLevel(_T("UNLOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}

		m_bITIReceiveAbleToPanelAligner = TRUE;

		if (FALSE == m_plnkITIPanelAligner->IsSendStartToUnloadConveyor())
			break;

		simulLog("UNLOAD_CONVEYOR_RECEIVE_PANEL", m_estepCurrent);

		m_bITIReceiveStartToPanelAligner = TRUE;

		while (TRUE)
		{
			if (TRUE == m_plnkITIPanelAligner->IsSendCompleteToUnloadConveyor())
				break;

			if (FALSE == m_plnkITIPanelAligner->IsSendStartToUnloadConveyor())
			{
				m_bITIReceiveAbleToPanelAligner = FALSE;
				m_bITIReceiveStartToPanelAligner = FALSE;
				return;
			}
			
			Sleep(1);
		}

		m_bITIReceiveAbleToPanelAligner = FALSE;
		m_bITIReceiveStartToPanelAligner = FALSE;

		// Fall Through : Panel 이 Unload Conveyor Panel 감지 Sensor 에 감지되지 않은 위치에 서는 경우를 막기 위해서.

	case UNLOAD_CONVEYOR_SEND_PANEL_READY: simulLog("UNLOAD_CONVEYOR_SEND_PANEL_READY", m_estepCurrent);

		if (TRUE == m_plnkUnloadConveyor->IsPanelDetected())
		{
			setStep(UNLOAD_CONVEYOR_SEND_PANEL);
			break;
		}
		
		// 확인 필요 : 32inch 인 경우 틀어짐 문제 때문에 속도 낮춰 써야 함.
		//             46inch 라면 굳이 이렇게 쓸 필요까지는 없다. 이렇게 썼을 때의 tact time 손실은? - 수정 이후 없다.
		iResult = m_plnkUnloadConveyor->MoveForward();
		if (iResult)
		{
			m_plnkUnloadConveyor->StopMoving();
			SetErrorLevel(_T("UNLOAD CONVEYOR ready to Send Panel"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}
		
		// Panel이 Unload Conveyor에 감지되는지 확인
		m_timer.StartTimer();
		while (TRUE)
		{
			if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
				break;
			
			if (m_plnkUnloadConveyor->IsPanelDetected())
				break;

			if (m_timer.MoreThan(30.0))
			{
				m_plnkUnloadConveyor->StopMoving();
				SetErrorLevel(_T("UNLOAD CONVEYOR ready to Send Panel"),0, __FILE__, __LINE__);
				// 428008 = Panel Aligner로부터 Panel을 넘겨 받은 후 30초 동안 센서에 Panel이 감지되지 않았습니다.
				PROCESS_ALARM(generateErrorCode(428008));
				return;
			}
			
			Sleep(1);
		}
		setStep(UNLOAD_CONVEYOR_SEND_PANEL);
		break;

	case UNLOAD_CONVEYOR_SEND_PANEL: simulLog("UNLOAD_CONVEYOR_SEND_PANEL", m_estepCurrent);
		//다음 설비 Load Conveyor로 Panel 넘겨준다.
		//Final(Gate)
		//0.Out Conveyor로 SendAble 신호 보낸다.
		//1.Out Conveyor로부터 ReceiveAble 신호 대기
		//2.Motor On -> Panel 이 지나가는지 확인
		//3.Stop Sensor off -> SendCompleteToOutConveyor 신호 보낸다.
		//4.ReceiveCompleteToUnloadConveyor 신호 대기 <- TimeOut Check
		//5.All ITI Signal Reset
		//6.Motor Off

		if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		{
			m_plnkUnloadConveyor->MoveForward();
			Sleep(1000);
			m_plnkUnloadConveyor->StopMoving();
			setStep(UNLOAD_CONVEYOR_RECEIVE_PANEL);
			break;
		}

//syr..SESL : 한진현K 추가 코드와 문홍주D 추가 코드가 함께 머지되어 Process 진행 순서가 맞는지 확인	요망
// 2009.05.22 hjh ver3 && D군 OutConveyor 없음.
#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) && defined(DEF_TEST))
		
		while (TRUE)
		{
			if (!m_plnkUnloadConveyor->IsPanelStopDetected())
				break;
			
			if (m_plnkOpPanel->GetStopButtonStatus())
			{
				initializeMelsecIF();
				return;
			}
			
			Sleep(1);
		}
		
		initializeMelsecIF();
		
		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_UNLOAD_CONVEYOR_PANEL);
		
		setStep(UNLOAD_CONVEYOR_RECEIVE_PANEL);
		break;
#endif

		// hongju_SESL 세메스에서 무조건 sendAble를 먼저 보내달라고 요구해서..확인 필요...
#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) && defined(DEF_MECH_VERSION3))
		m_plnkMelsecIF->SetLowerEqBit(eSendAble, TRUE);
#endif
		// hongju 수정.0523_SESL.. 무조건 다음설비 ReceiveAble 받고 동작 하도록 수정해야 한다..
		if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble))
		{
			m_plnkUnloadConveyor->StopMoving();
			break;
		}
				
		// 부상 Vac가 Off 일때만 신호를 줘서 바로 Panel이 넘어가도록 수정..
		if (TRUE == m_plnkITIPanelAligner->IsPlateVacuumOn()) //2009.06.11
			break;

		m_plnkMelsecIF->SetLowerEqBit(eSendAble, TRUE);

//syr..SESL : 코드 막음
/*		m_plnkMelsecIF->SetLowerEqBit(eSendAble, TRUE);
		
		if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble))
		{
			m_plnkUnloadConveyor->StopMoving();
			break;
		}
*/		
		m_plnkTrsAutoManager->AssignGlassData(DEF_UNLOAD_CONVEYOR_PANEL);

		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart))
				break;
			
			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble))
			{
				m_plnkUnloadConveyor->StopMoving();
				initializeMelsecIF();
				return;
			}
			
			if (m_timer.MoreThan(30.0))
			{
				initializeMelsecIF();
				SetErrorLevel(_T("UNLOAD CONVEYOR send Panel"),0, __FILE__, __LINE__);
				// 428006 = 다음 설비로 Panel을 넘겨주던 중 30초 동안 ReceiveStart 신호를 받지 못하였습니다.
				PROCESS_ALARM(generateErrorCode(428006));
				return;
			}
			
			Sleep(1);
		}
		
		// 다음 설비로 SendStart 신호 보내고
		m_plnkMelsecIF->SetLowerEqBit(eSendStart, TRUE);
		
		// Pre Tab 준비를 위하여...
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, TRUE);
		
		// 다음 설비 Load Conveyor 에서는 MoveForward 하므로, 속도 차 발생으로 Panel 틀어짐.
		iResult = m_plnkUnloadConveyor->MoveForward();
		if (iResult)
		{
			m_plnkUnloadConveyor->StopMoving();
			initializeMelsecIF();

			SetErrorLevel(_T("UNLOAD CONVEYOR send Panel"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}

		m_timer.StartTimer();
		while (TRUE)
		{
			if (FALSE == m_plnkUnloadConveyor->IsPanelDetected())
				break;

			if (m_timer.MoreThan(30.0))
			{
				m_plnkUnloadConveyor->StopMoving();
				initializeMelsecIF();

				// 현재 설비의 Unload Conveyor 가 자력으로 Panel 을 Sensor 감지되지 않는 위치까지 이동 시킬 수 없는 경우.
				// - 정상적인 경우라면 다음 설비 Load Conveyor 의 힘을 빌려서라도 Panel 이 이동 될 것이다.
				//   다음 설비 Load Conveyor 에 문제가 생겼다면, 그로 인해 현재 설비에서 Error 를 발생시킬 필요는 없다.
				//   가능하면 현재 설비의 Unload Conveyor Panel 감지 센서의 위치는 현재 Unload Conveyor 만의 힘으로
				//   Panel 을 Sensor 감지되지 않는 위치까지 이동시킬 수 있는 위치로 설치해야 한다.
				if (TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)
					&& TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart))
				{
					SetErrorLevel(_T("UNLOAD CONVEYOR send Panel"),0, __FILE__, __LINE__);
					// 428005 = 다음 설비로 Panel을 넘겨주던 중 30초 동안 정지센서에 Panel이 계속 감지됩니다.
					PROCESS_ALARM(generateErrorCode(428005));
				}
				return;
			}
			
			Sleep(1);
		}
		m_plnkUnloadConveyor->StopMoving();	// Error 처리 무시 : Conveyor 는 서도 그만, 안서도 그만...

		// 다음 설비로 SendComplete 신호 보낸다.
		m_plnkMelsecIF->SetLowerEqBit(eSendComplete, TRUE);

		// 다음 설비로부터 ReceiveComplete 신호 대기
		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveComplete))
				break;

			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)
				|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart))
			{
				// 다음 설비 Load Conveyor 에서 문제가 발생했거나,
				// Melsec Net 이 끊어지는 문제에 대해서도 대응 가능.
				// => 문제의 원인 조치 후, Interface 중이던 두 설비를 Start 하면
				//    바로 생산 진행
				//    : 자동운전 초기화 필요 없음 - 두 설비 모두 Interface step 에 있고,
				//      Panel 도 두 설비의 Conveyor 사이에 있을 것이므로..

				m_plnkUnloadConveyor->StopMoving();
				initializeMelsecIF();
				return;
			}
			
			// time over 발생 불가 : Melsec Cable 이 끊어져도 바로 위 조건에 의해 처리됨.
			
			// 만약을 대비한 time over check
			// : 원인을 알 수 없는 문제에 의해 ReceiveComplete 신호만 On 되지 않을 경우.(현재 설비의 알 수 없는 문제로 봐야 함.)
			//  => Error 처리가 무의미하다.
			//     다른 Interface 도중 Error 와는 다른 경우로, 별도의 Error 조치가 필요한 경우가 된다.
			//     따라서 이 경우는 Error 처리를 무시하기로 한다. (사실 이런 경우가 발생할 수 있는지도 의문이다.)
			//     이미 Panel 은 정상적으로 다음 설비로 넘어갔을 것이므로, Unload Conveyor 는  정상 step 을 진행한다.
			//    다만, Melsec 에 이상이 발생한 경우이므로 다음 Panel 의 전달에서 알 수 없는 동작을 할 수 있다.
			//    Melsec 이상을 Error Display 하지 않는다는 문제는 남지만, 처리가 애매하다..
			if (m_timer.MoreThan(60.0))	// Load Conveyor 의 limit time 보다 긴 시간을 기다려야 한다.
				break;
			
			Sleep(1);
		}

		// All Interface Signal Reset : WorkStart to Lower Eq 신호 포함.
		initializeMelsecIF();

		// PreBonder Tab 작업 준비 신호 해제.
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, FALSE);

		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_UNLOAD_CONVEYOR_PANEL);



		setStep(UNLOAD_CONVEYOR_RECEIVE_PANEL);
		break;

	//NSMC KJS
	case UNLOAD_CONVEYOR_MODEL_CHANGE: simulLog("UNLOAD_CONVEYOR_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;

		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;

		m_bModelChangeReady = FALSE;

		setStep(UNLOAD_CONVEYOR_RECEIVE_PANEL);
		break;

	default:
		break;
	}
}

void MTrsUnloadConveyor::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepUnloadConveyor)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepUnloadConveyor)0;	//  현재 Step Clear
}

void MTrsUnloadConveyor::setStep(EStepUnloadConveyor estepUnloadConveyor)
{
	m_estepPrevious	= m_estepCurrent;	// 이전 Step 저장
	m_estepCurrent	= estepUnloadConveyor;	// 새로 Step으로 전환
}

int MTrsUnloadConveyor::AssignComponents(STrsUnloadConveyorRefCompList listRefComponents)
{
// 2009.05.22 hjh ver3 && D군 OutConveyor 없음.
#ifdef DEF_TEST
	m_plnkOpPanel			= listRefComponents.m_plnkOpPanel;
#endif
	m_plnkTrsAutoManager	= listRefComponents.m_plnkTrsAutoManager;
	m_plnkUnloadConveyor	= listRefComponents.m_plnkUnloadConveyor;
	m_plnkStatePanelAligner	= listRefComponents.m_plnkStatePanelAligner;
	m_plnkMelsecIF			= listRefComponents.m_plnkMelsecIF;

	return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS;
}

int MTrsUnloadConveyor::SetData(STrsUnloadConveyorData datComponent)
{
	m_plnkSystemData	= datComponent.m_plnkSystemData;
	return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS;
}

int MTrsUnloadConveyor::Initialize()
{
	m_bAutoInitialized = FALSE;
#if !(defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM))
	initializeMelsecIF();
#endif
	initializeITI();
	initializeStep();

#ifndef SIMULATION
	int iResult = m_plnkUnloadConveyor->StopMoving();
	if (iResult)
		return iResult;

	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		m_bAutoInitialized = TRUE;
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, FALSE);
		return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS;
	}

	if (TRUE == m_plnkStatePanelAligner->IsPanelAbsorbed()
		&& TRUE == m_plnkStatePanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_UNLOAD_POS))
	{
		m_bAutoInitialized = TRUE;	
		return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS;
	}

	if (FALSE == m_plnkUnloadConveyor->IsPanelDetected())	// Panel 감지되지 않을 때
	{
		if (FALSE == m_plnkStatePanelAligner->IsPanelAbsorbed())
		{
			// Panel 유무 확인
			iResult = m_plnkUnloadConveyor->MoveForwardReducedSpeed();
			if (iResult)
			{
				m_plnkUnloadConveyor->StopMoving();
				return iResult;
			}

			m_timer.StartTimer();
			while (TRUE)
			{
				if (TRUE == m_plnkUnloadConveyor->IsPanelDetected())
				{
					iResult = m_plnkUnloadConveyor->StopMoving();
					if (iResult)
					{
						m_plnkUnloadConveyor->StopMoving();
						return iResult;
					}

					setStep(UNLOAD_CONVEYOR_SEND_PANEL);
					break;
				}

				if (m_timer.MoreThan(10.0))
				{
					iResult = m_plnkUnloadConveyor->StopMoving();
					if (iResult)
					{
						m_plnkUnloadConveyor->StopMoving();
						return iResult;
					}
					break;
				}

				Sleep(1);
			}
		}

		// hongju_SESL UNLoad Conveyor에 Panel이 감지되지 않으면 무조건 Scrap 진행??
/*		if (m_plnkTrsAutoManager->m_pGlassData[DEF_UNLOAD_CONVEYOR_PANEL]->m_strEPanelID.GetLength() > 1)		
		{
		//	if (MyMessageBox(MY_MESSAGE_BOX,65500, _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
			m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_UNLOAD_CONVEYOR_PANEL);	
		}
*/
		// PreBonder Tab 작업 준비 신호 해제.
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, FALSE);
	}
	else // if (TRUE == m_plnkUnloadConveyor->IsPanelDetected())
	{
		setStep(UNLOAD_CONVEYOR_SEND_PANEL);
	}
#endif
	m_bAutoInitialized = TRUE;
	return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS; 
}

void MTrsUnloadConveyor::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

void MTrsUnloadConveyor::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsUnloadConveyor::ThreadStop()
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

#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM))
void MTrsUnloadConveyor::AssignITI(ITI_PanelAligner* pITI_PanelAligner,
								   ITI_PanelTransfer* pITI_PanelTransfer,
								   ITI_OutConveyor* pITI_OutConveyor)
{
	m_plnkITIPanelAligner = pITI_PanelAligner;
	m_plnkITIPanelTransfer = pITI_PanelTransfer;
	m_plnkITIOutConveyor = pITI_OutConveyor;
}
#else
void MTrsUnloadConveyor::AssignITI(ITI_PanelAligner* pITI_PanelAligner, ITI_PanelTransfer* pITI_PanelTransfer)
{
	m_plnkITIPanelAligner = pITI_PanelAligner;
	m_plnkITIPanelTransfer = pITI_PanelTransfer;
}
#endif

void MTrsUnloadConveyor::initializeITI()
{
	m_bITIReceiveAbleToPanelAligner			= FALSE;
	m_bITIReceiveStartToPanelAligner		= FALSE;
	m_bITISendAbleToOutConveyor				= FALSE;
	m_bITISendCompleteToOutConveyor			= FALSE;
}

BOOL MTrsUnloadConveyor::IsInitialized(void)
{
	return m_bAutoInitialized;
}

BOOL MTrsUnloadConveyor::IsReceiveAbleToPanelAligner()
{
	return m_bITIReceiveAbleToPanelAligner;
}

BOOL MTrsUnloadConveyor::IsReceiveStartToPanelAligner()
{
	return m_bITIReceiveStartToPanelAligner;
}

BOOL MTrsUnloadConveyor::IsSendAbleToOutConveyor()
{
	return m_bITISendAbleToOutConveyor;
}

BOOL MTrsUnloadConveyor::IsSendCompleteToOutConveyor()
{
	return m_bITISendCompleteToOutConveyor;
}

BOOL MTrsUnloadConveyor::IsEmptyPanel()
{
	BOOL bPanelDetected = m_plnkUnloadConveyor->IsPanelDetected();
	return !bPanelDetected;
}

int	MTrsUnloadConveyor::GetStep()
{
	return m_estepCurrent;
}

BOOL MTrsUnloadConveyor::IsAutoRun()
{
	return m_bAutoRun;
}

CString MTrsUnloadConveyor::GetItiStatus()
{
	#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM))
	m_strItiStatus.Format(_T("ReceiveA PA: %d, ReceiveS PA: %d, SendA OutConveyor: %d, SendC OutConveyor: %d"),
		m_bITIReceiveAbleToPanelAligner, m_bITIReceiveStartToPanelAligner,
		m_bITISendAbleToOutConveyor, m_bITISendCompleteToOutConveyor);
#else
	m_strItiStatus.Format(_T("ReceiveA PA: %d, ReceiveS PA: %d"),
		m_bITIReceiveAbleToPanelAligner, m_bITIReceiveStartToPanelAligner);
#endif
	return m_strItiStatus;
}

void MTrsUnloadConveyor::initializeMelsecIF()
{
	m_plnkMelsecIF->SetLowerEqBit(eSendAble, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eSendStart, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eSendComplete, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eImmediatelyPauseRequest, FALSE);
}

int MTrsUnloadConveyor::ScrapPanel()
{
	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_UNLOAD_CONVEYOR_PANEL);
	
	m_bAutoInitialized = FALSE;
	initializeMelsecIF();
	initializeITI();
	initializeStep();

	int iResult = m_plnkUnloadConveyor->StopMoving();
	if (iResult)
		return iResult;

	if (TRUE == m_plnkUnloadConveyor->IsPanelDetected())
	{
		if (TRUE == m_plnkStatePanelAligner->IsPanelAbsorbed()
			&& TRUE == m_plnkStatePanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_UNLOAD_POS))
		{
			return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS; 
		}
		
		CString strMsg = _T("There is a panel on Unload Conveyor. Remove it.");
		do
		{
			Sleep(1);
			MyMessageBox(MY_MESSAGE_BOX, 62300, "Remove Panel Request", M_ICONINFORMATION);
		} while (TRUE == m_plnkUnloadConveyor->IsPanelDetected());
	}

	m_bAutoInitialized = TRUE;
	return ERR_TRS_UNLOAD_CONVEYOR_SUCCESS; 
}

// NSMC
BOOL MTrsUnloadConveyor::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}
// End Of Code
