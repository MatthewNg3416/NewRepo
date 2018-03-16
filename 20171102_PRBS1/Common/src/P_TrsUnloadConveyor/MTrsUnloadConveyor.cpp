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

// 2009.05.22 hjh ver3 && D�� OutConveyor ����.
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

		// Fall Through : Panel �� Unload Conveyor Panel ���� Sensor �� �������� ���� ��ġ�� ���� ��츦 ���� ���ؼ�.

	case UNLOAD_CONVEYOR_SEND_PANEL_READY: simulLog("UNLOAD_CONVEYOR_SEND_PANEL_READY", m_estepCurrent);

		if (TRUE == m_plnkUnloadConveyor->IsPanelDetected())
		{
			setStep(UNLOAD_CONVEYOR_SEND_PANEL);
			break;
		}
		
		// Ȯ�� �ʿ� : 32inch �� ��� Ʋ���� ���� ������ �ӵ� ���� ��� ��.
		//             46inch ��� ���� �̷��� �� �ʿ������ ����. �̷��� ���� ���� tact time �ս���? - ���� ���� ����.
		iResult = m_plnkUnloadConveyor->MoveForward();
		if (iResult)
		{
			m_plnkUnloadConveyor->StopMoving();
			SetErrorLevel(_T("UNLOAD CONVEYOR ready to Send Panel"),0, __FILE__, __LINE__);
			PROCESS_ALARM(iResult);
			break;
		}
		
		// Panel�� Unload Conveyor�� �����Ǵ��� Ȯ��
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
				// 428008 = Panel Aligner�κ��� Panel�� �Ѱ� ���� �� 30�� ���� ������ Panel�� �������� �ʾҽ��ϴ�.
				PROCESS_ALARM(generateErrorCode(428008));
				return;
			}
			
			Sleep(1);
		}
		setStep(UNLOAD_CONVEYOR_SEND_PANEL);
		break;

	case UNLOAD_CONVEYOR_SEND_PANEL: simulLog("UNLOAD_CONVEYOR_SEND_PANEL", m_estepCurrent);
		//���� ���� Load Conveyor�� Panel �Ѱ��ش�.
		//Final(Gate)
		//0.Out Conveyor�� SendAble ��ȣ ������.
		//1.Out Conveyor�κ��� ReceiveAble ��ȣ ���
		//2.Motor On -> Panel �� ���������� Ȯ��
		//3.Stop Sensor off -> SendCompleteToOutConveyor ��ȣ ������.
		//4.ReceiveCompleteToUnloadConveyor ��ȣ ��� <- TimeOut Check
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

//syr..SESL : ������K �߰� �ڵ�� ��ȫ��D �߰� �ڵ尡 �Բ� �����Ǿ� Process ���� ������ �´��� Ȯ��	���
// 2009.05.22 hjh ver3 && D�� OutConveyor ����.
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

		// hongju_SESL ���޽����� ������ sendAble�� ���� �����޶�� �䱸�ؼ�..Ȯ�� �ʿ�...
#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) && defined(DEF_MECH_VERSION3))
		m_plnkMelsecIF->SetLowerEqBit(eSendAble, TRUE);
#endif
		// hongju ����.0523_SESL.. ������ �������� ReceiveAble �ް� ���� �ϵ��� �����ؾ� �Ѵ�..
		if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble))
		{
			m_plnkUnloadConveyor->StopMoving();
			break;
		}
				
		// �λ� Vac�� Off �϶��� ��ȣ�� �༭ �ٷ� Panel�� �Ѿ���� ����..
		if (TRUE == m_plnkITIPanelAligner->IsPlateVacuumOn()) //2009.06.11
			break;

		m_plnkMelsecIF->SetLowerEqBit(eSendAble, TRUE);

//syr..SESL : �ڵ� ����
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
				// 428006 = ���� ����� Panel�� �Ѱ��ִ� �� 30�� ���� ReceiveStart ��ȣ�� ���� ���Ͽ����ϴ�.
				PROCESS_ALARM(generateErrorCode(428006));
				return;
			}
			
			Sleep(1);
		}
		
		// ���� ����� SendStart ��ȣ ������
		m_plnkMelsecIF->SetLowerEqBit(eSendStart, TRUE);
		
		// Pre Tab �غ� ���Ͽ�...
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, TRUE);
		
		// ���� ���� Load Conveyor ������ MoveForward �ϹǷ�, �ӵ� �� �߻����� Panel Ʋ����.
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

				// ���� ������ Unload Conveyor �� �ڷ����� Panel �� Sensor �������� �ʴ� ��ġ���� �̵� ��ų �� ���� ���.
				// - �������� ����� ���� ���� Load Conveyor �� ���� �������� Panel �� �̵� �� ���̴�.
				//   ���� ���� Load Conveyor �� ������ ����ٸ�, �׷� ���� ���� ���񿡼� Error �� �߻���ų �ʿ�� ����.
				//   �����ϸ� ���� ������ Unload Conveyor Panel ���� ������ ��ġ�� ���� Unload Conveyor ���� ������
				//   Panel �� Sensor �������� �ʴ� ��ġ���� �̵���ų �� �ִ� ��ġ�� ��ġ�ؾ� �Ѵ�.
				if (TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)
					&& TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart))
				{
					SetErrorLevel(_T("UNLOAD CONVEYOR send Panel"),0, __FILE__, __LINE__);
					// 428005 = ���� ����� Panel�� �Ѱ��ִ� �� 30�� ���� ���������� Panel�� ��� �����˴ϴ�.
					PROCESS_ALARM(generateErrorCode(428005));
				}
				return;
			}
			
			Sleep(1);
		}
		m_plnkUnloadConveyor->StopMoving();	// Error ó�� ���� : Conveyor �� ���� �׸�, �ȼ��� �׸�...

		// ���� ����� SendComplete ��ȣ ������.
		m_plnkMelsecIF->SetLowerEqBit(eSendComplete, TRUE);

		// ���� ����κ��� ReceiveComplete ��ȣ ���
		m_timer.StartTimer();
		while (TRUE)
		{
			if (TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveComplete))
				break;

			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)
				|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart))
			{
				// ���� ���� Load Conveyor ���� ������ �߻��߰ų�,
				// Melsec Net �� �������� ������ ���ؼ��� ���� ����.
				// => ������ ���� ��ġ ��, Interface ���̴� �� ���� Start �ϸ�
				//    �ٷ� ���� ����
				//    : �ڵ����� �ʱ�ȭ �ʿ� ���� - �� ���� ��� Interface step �� �ְ�,
				//      Panel �� �� ������ Conveyor ���̿� ���� ���̹Ƿ�..

				m_plnkUnloadConveyor->StopMoving();
				initializeMelsecIF();
				return;
			}
			
			// time over �߻� �Ұ� : Melsec Cable �� �������� �ٷ� �� ���ǿ� ���� ó����.
			
			// ������ ����� time over check
			// : ������ �� �� ���� ������ ���� ReceiveComplete ��ȣ�� On ���� ���� ���.(���� ������ �� �� ���� ������ ���� ��.)
			//  => Error ó���� ���ǹ��ϴ�.
			//     �ٸ� Interface ���� Error �ʹ� �ٸ� ����, ������ Error ��ġ�� �ʿ��� ��찡 �ȴ�.
			//     ���� �� ���� Error ó���� �����ϱ�� �Ѵ�. (��� �̷� ��찡 �߻��� �� �ִ����� �ǹ��̴�.)
			//     �̹� Panel �� ���������� ���� ����� �Ѿ�� ���̹Ƿ�, Unload Conveyor ��  ���� step �� �����Ѵ�.
			//    �ٸ�, Melsec �� �̻��� �߻��� ����̹Ƿ� ���� Panel �� ���޿��� �� �� ���� ������ �� �� �ִ�.
			//    Melsec �̻��� Error Display ���� �ʴ´ٴ� ������ ������, ó���� �ָ��ϴ�..
			if (m_timer.MoreThan(60.0))	// Load Conveyor �� limit time ���� �� �ð��� ��ٷ��� �Ѵ�.
				break;
			
			Sleep(1);
		}

		// All Interface Signal Reset : WorkStart to Lower Eq ��ȣ ����.
		initializeMelsecIF();

		// PreBonder Tab �۾� �غ� ��ȣ ����.
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, FALSE);

		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_UNLOAD_CONVEYOR_PANEL);



		setStep(UNLOAD_CONVEYOR_RECEIVE_PANEL);
		break;

	//NSMC KJS
	case UNLOAD_CONVEYOR_MODEL_CHANGE: simulLog("UNLOAD_CONVEYOR_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change �ϷḦ TrsAutoManager ���� �뺸.
		m_bModelChangeReady = TRUE;

		// Ȯ�� �ʿ� : TrsAutoManager ���� ��ȣ�� �ϳ��� ����� ���ΰ�? �ΰ��� ����� ���ΰ�?
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
	// �ڵ����� STEP �ʱ�ȭ
	m_estepPrevious	= (enumStepUnloadConveyor)0;	//  ���� Step Clear
	m_estepCurrent	= (enumStepUnloadConveyor)0;	//  ���� Step Clear
}

void MTrsUnloadConveyor::setStep(EStepUnloadConveyor estepUnloadConveyor)
{
	m_estepPrevious	= m_estepCurrent;	// ���� Step ����
	m_estepCurrent	= estepUnloadConveyor;	// ���� Step���� ��ȯ
}

int MTrsUnloadConveyor::AssignComponents(STrsUnloadConveyorRefCompList listRefComponents)
{
// 2009.05.22 hjh ver3 && D�� OutConveyor ����.
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

	if (FALSE == m_plnkUnloadConveyor->IsPanelDetected())	// Panel �������� ���� ��
	{
		if (FALSE == m_plnkStatePanelAligner->IsPanelAbsorbed())
		{
			// Panel ���� Ȯ��
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

		// hongju_SESL UNLoad Conveyor�� Panel�� �������� ������ ������ Scrap ����??
/*		if (m_plnkTrsAutoManager->m_pGlassData[DEF_UNLOAD_CONVEYOR_PANEL]->m_strEPanelID.GetLength() > 1)		
		{
		//	if (MyMessageBox(MY_MESSAGE_BOX,65500, _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
			m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_UNLOAD_CONVEYOR_PANEL);	
		}
*/
		// PreBonder Tab �۾� �غ� ��ȣ ����.
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
