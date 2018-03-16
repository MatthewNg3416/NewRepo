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

		//���� ����� ���� Panel ���� �غ�
		//1.PanelAligner�κ��� ���� ������ ���� ��ȣ�� �޾ƾ� �Ѵ�.
		// - LoadConveyorEnabletoReceivePanel ��ȣ ���
		// - Panel Aligner���� ������ ��ȣ, Tact ���� �ʿ�� Panel Transfer���� ��ȣ������.
		// - LoadConveyor�� ��ȣ Reset �Ѵ�.
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

		// Cassette �ϰ��� Stopper�� ����...
#ifndef SIMULATION
		// jdy sesl 090102	: cassette ������ �� Stopper Down �ʿ�.
		//                     Panel Prealign ��� ����� ��� Stopper Up �ȵǸ� �ȵ�.
		if (TRUE == m_plnkSystemData->m_bUsePanelPreAlign)
		{
			// ���� �ϰ��� Cassette �� L/C Stopper �� ���� �߻��ϴ� ������...?
			// � ���񿡼� ���� �ִ��� Ȯ���ؼ� �� �κ� �ڵ� �����ؾ� �Ѵ�.
			// �ϴ� SESL 13,14 ȣ��� ���� ����.
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
			// ���⵵ ���� ���� Ȯ���ؼ� �ڵ� ���ʿ��� ��� ������ ��.
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
			// Dry Run ������ Conveyor ȸ�� ���� Error ó���� ���ʿ� : ������ Panel �����Ƿ�..
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

		// Panel �������� ��ư ������ ���
		if (TRUE == IsPanelInStop())
		{
			// ���� ���� I/F Time Over Error ����.
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

			// PreAlign ���� 
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
			// ����ȭ �ð� 
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

		// jdy sesl 090102 : ���⼭ Stopper �ȿö� ������ ���� ���� �� �ִ�.
		//                     Panel Prealign ��� ����� ��� Stopper Up �ȵǸ� �ȵ�.
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
				// 427001 = ���� ����κ��� Panel�� �Ѱܹ޴� �� 30�� ���� SendStart ��ȣ�� ���� ���Ͽ����ϴ�.
				PROCESS_ALARM(generateErrorCode(427001));
				return;
			}

			Sleep(1);
		}

		// ���� ����κ��� Panel Data ���� �´�. => Interface ���� Error ������ Panel Data ���� ����.
		m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_LOAD_CONVEYOR_PANEL);

#if defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM)
		// Panel Data Reading Complete Signal �� ���.
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
				// �� ���� ���� ������ Unload Conveyor ���� Inverter Move Forward �� Error �� �߻��� ���̴�.
				// ����, Panel �� ���� ���� ����� �Ѿ���� �ʾҴ�.

				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				return;
			}

			if (m_timer.MoreThan(30.0))
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();

				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427000 = ���� ����κ��� Panel�� �Ѱܹ޴� �� 30�� ���� ���Ӽ����� �������� �ʾҽ��ϴ�.
				PROCESS_ALARM(generateErrorCode(427000));
				return;
			}

			Sleep(1);
		}

// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
		//20081110 by kss
		// Panel�� Stopper�� ���� ������ �����Ѵ�. 
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
				// 427002 = ���� ����κ��� Panel�� �Ѱܹ޴� �� 30�� ���� ���������� �������� �ʾҽ��ϴ�.
				PROCESS_ALARM(generateErrorCode(427002));
				return;
			}

			Sleep(1);
		}


		// SendComplete ��ȣ���
		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkMelsecIF->IsOnUpperEqBit(eSendComplete))
				break;

			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)
				|| FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendStart))
			{
				// �� ���� Melsec Cable ������ ����� ���� �Ѵ�. (�� ���� ���� Error �� ���� ��ȣ ���� �ƴ�.)
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427003 = ���� ����κ��� Panel�� �Ѱܹ޴� �� ���� ������ SendAble, SendStart ��ȣ�� �������ϴ�. [Melsec Net Ȯ�� �ʿ�]
				PROCESS_ALARM(generateErrorCode(427003));
				return;
			}

			// time over �߻� �Ұ� : Melsec Cable �� �������� �ٷ� �� ���ǿ� ���� ó����.
			
			// ������ ����� time over check
			// : ������ �� �� ���� ������ ���� SendComplete ��ȣ�� On ���� ���� ���, Error Display.
			if (m_timer.MoreThan(30.0))
			{
				m_plnkLoadConveyor->StopMoving();
				initializeMelsecIF();
				
				SetErrorLevel(_T("LOAD CONVEYOR receive Panel"),0, __FILE__, __LINE__);
				// 427004 = ���� ����κ��� Panel�� �Ѱܹ��� �� 30�� ���� SendComplete ��ȣ�� ���� ���Ͽ����ϴ�.
				PROCESS_ALARM(generateErrorCode(427004));
				return;
			}

			Sleep(1);
		}

		// ���� ����� ReceiveComplete ��ȣ ������.
		m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, TRUE);

		// SendAble ~ SendComplete ��ȣ Reset ���
		m_timer.StartTimer();
		while (TRUE)
		{
			// ��Ģ : SendAble, Start, Complete All Off Ȯ���ؾ� �ϳ�,
			//		  ���ǻ� Complete Off �� Ȯ���Ѵ�.
			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendComplete))
				break;

			// �� �������� Time Over �� �߻��� �� ����. (Melsec Cable �� ����������...)
			// ������ ����� ���� ��ġ�� time over check
			//  => time over �߻� ��, �̹� ���� ����� Panel �޴� ������ �Ϸ������Ƿ� ���� Step �� �����Ѵ�.
			if (m_timer.MoreThan(30.0))
				break;

			Sleep(1);
		}

///////////////////////////////////////////////////////////////////////////////////
		// 2009.04.22 SESL_CYJ �� ������� I/F �ð� ���� 
		// ���� ���� : Panel Prealign ������ ������ I/F �� �Ϸ��ؼ� 2~3�� Delay �߻�
		// All Interface Signal Reset
		initializeMelsecIF();

// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
		if (m_plnkSystemData->m_bUsePanelPreAlign)
		{
			//20081110 by kss
			// ����ȭ �ð� 
			Sleep(1000);
			
			//20081110 by kss
			// PreAlign ���� 
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
			// ����ȭ �ð� 
			//Sleep(1000);
			// Data ���� �ð� ��ŭ Stopper�� �����ϵ��� ����Ѵ�. syr_090311
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
			// ����ȭ �ð� 
			Sleep(1000);
			
			//20081110 by kss
			// PreAlign ���� 
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
					// 427011 = Panel�� Batch ID�� �����ϴ�. Load Conveyor �κ��� Panel �� ������ �ּ���.
					PROCESS_ALARM(generateErrorCode(427011));
					return;
				case ERR_NO_DEVICEID_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427012 = Panel�� Device ID�� �����ϴ�. Load Conveyor �κ��� Panel �� ������ �ּ���.
					PROCESS_ALARM(generateErrorCode(427012));
					return;
				case ERR_NO_PANELID_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427013 = Panel�� Panel ID�� �����ϴ�. Load Conveyor �κ��� Panel �� ������ �ּ���.
					PROCESS_ALARM(generateErrorCode(427013));
					return;
/* Ȯ�� �ʿ� : ���� �ұ�?
				case ERR_SAME_PANELID_IN_LAST_5TIME:
					// 427014 = Panel ID�� �ֱ� ����� 5���� �г��� �ϳ��� ��ġ�մϴ�. Load Conveyor �κ��� Panel �� ������ �ּ���.
					PROCESS_ALARM(generateErrorCode(427014));
					return;
*/
				case ERR_NO_PANEL_DATA:
					SetErrorLevel(_T("LOAD CONVEYOR do Panel Validation Check"),0, __FILE__, __LINE__);
					// 427015 = Panel�� ���� ����κ��� �Ѱܹ��� ID Data�� �����ϴ�. Load Conveyor �κ��� Panel �� ������ �ּ���.
					PROCESS_ALARM(generateErrorCode(427015));
					return;
				}
			}
		}

		setStep(LOAD_CONVEYOR_SEND_PANEL);
		break;

	case LOAD_CONVEYOR_SEND_PANEL: simulLog("LOAD_CONVEYOR_SEND_PANEL", m_estepCurrent);
		//PanelTransfer�� Panel�� ������.

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

		// PanelTransfer�κ��� ReceiveAble ��ȣ�� ��ٸ���.
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

		// PanelTransfer�� �����ִ� �ӵ��� ���� ����
		iResult = m_plnkLoadConveyor->MoveForwardTransferSpeed();
		if (iResult)
		{
			m_bITISendStartToPanelTransfer = FALSE;
			m_plnkLoadConveyor->StopMoving();
			
			SetErrorLevel(_T("LOAD CONVEYOR send Panel to Panel Transfer"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}

		// hongju ����..Test �ʿ�
		// Panel ������ �ϴ� ���� ���� ���Ͽ� Sleep()�� �ְ� Panel ���� ���� ���� ��Ŵ
//		Sleep(300);
		//2009.11.11 CYJ Panel Aligner Unload & Panel Transfer Receive Panel ���� ����
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
#ifdef DEF_PLATFORM_ARP_SYSTEM // 2009.04.14 SESL_CYJ ARP�� ����ǵ��� ����.
				// 2009.04.07 SESL_CYJ, SESL_HJH Carrier �� Panel �� ������ �߻��Ͽ� Carrier �̵��ð� ��� �� ���ν� Ȯ�� �� ����.
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
				// 427005 = Panel Transfer�� Panel�� �Ѱ��ִ� �� 30�� ���� Panel Transfer�� ���Ӽ����� �������� �ʾҽ��ϴ�.
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

		// Data ���� �ð� ��ŭ Stopper�� �����ϵ��� ����Ѵ�.
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
		// Model Change �ϷḦ TrsAutoManager ���� �뺸.
		m_bModelChangeReady = TRUE;

		// Ȯ�� �ʿ� : TrsAutoManager ���� ��ȣ�� �ϳ��� ����� ���ΰ�? �ΰ��� ����� ���ΰ�?
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
	// �ڵ����� STEP �ʱ�ȭ
	m_estepPrevious	= (enumStepLoadConveyor)0;	//  ���� Step Clear
	m_estepCurrent	= (enumStepLoadConveyor)0;	//  ���� Step Clear
}

void MTrsLoadConveyor::setStep(EStepLoadConveyor estepLoadConveyor)
{
	m_estepPrevious	= m_estepCurrent;		// ���� Step ����
	m_estepCurrent	= estepLoadConveyor;	// ���� Step���� ��ȯ
}

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

/**
* Assign component linkage
* 
* @param	listRefComponents	: MTrsLoadConveyor ���� Component list
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
* Component Data ó�� 
*
* @param	pData : SMMTrsLoadConveyorData ����ü�� ���� ������
* @return	Error : 0 = Success, �׿�=��Ÿ
*/
int MTrsLoadConveyor::SetData(STrsLoadConveyorData datComponent)
{
	m_plnkSystemData = datComponent.m_plnkSystemData;
	return ERR_TRS_LOAD_CONVEYOR_SUCCESS;
}

/** 
 * MTrsLoadConveyor Component�� �ڵ� ���� �ʱ�ȭ�� �����Ѵ�.
 * @return	Error Code : 0 = SUCCESS, �� �� = Error
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
		// 427006 = Conveyor �κ��� Cylinder ���� ��ȣ�� ���� ����. [Conveyor �и� ���� Ȯ�� �ʿ�.]
		return generateErrorCode(427006);
	}

#	endif

	// jdy sesl 081229	#ifdef DEF_PANEL_PRE_ALIGN
	if (m_plnkSystemData->m_bUsePanelPreAlign)
	{
		//20081110 by kss
		// PreAlign ���� 
		iResult = m_plnkLoadConveyor->BackwardPanelPreAlignFBCyl();
		if (iResult)
			return iResult;
	}
// jdy sesl 081229#endif

	// 080722 ������ ���� -> ����Test�� �ʱ�ȭ �Ҷ� Stopper up �߰� (�����Ͼ� ��û����)
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
		// �ʱ�ȭ�ÿ� ���� Panel�� ������Ű�� ���� Stopper�� Up���ѳ��´�. 
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

	// Panel ���������� Panel ���� ���� ���� ��
	if (FALSE == m_plnkLoadConveyor->IsPanelDetected())
	{
		// Panel ���� Ȯ��
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

		// hongju_SESL Load Conveyor�� Panel�� �������� ������ ������ Scrap ����??
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
 * MTrsLoadConveyor Initial �Ϸ� ���θ� �����Ѵ�.
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
 * Initial �Ϸ� ���� Ȯ��
 *
 * @return BOOL (TRUE : Initial �Ϸ�, FALSE : �̿Ϸ�)
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
