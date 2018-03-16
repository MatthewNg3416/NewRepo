//______________________________________________________________________________

// File Name: MTrsPanelTransferOut.cpp
// History	:
//		1. 2010.07.29 : Created
//		2.
//______________________________________________________________________________

#include "StdAfx.h"
#include "MTrsPanelTransferOut.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPanelTransfer.h"
#include "IStatePanelAligner.h"
//#include "IStateRollerConveyor.h"
#include "MTrsAutoManager.h"
#include "ITI_LoadConveyor.h"
#include "ITI_PanelAligner.h"
#include "common.h"
#include "MTickTimer.h"
#include "MNetH.h"
#include "DefPanelAligner.h"
#include "MPlatformOlbSystem.h"
//______________________________________________________________________________

extern MPlatformOlbSystem	MOlbSystemPre;

MTrsPanelTransferOut::MTrsPanelTransferOut(SCommonAttribute	commonData,
									STrsPanelTransferOutRefCompList			listRefComponents,
									STrsPanelTransferOutData					datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	m_bAutoInitialized  = FALSE;

	m_bThreadLife		= FALSE;
	m_pThread			= NULL;
	m_bAutoRun			= FALSE;

	initializeITI();
	initializeStep();
}
//______________________________________________________________________________

MTrsPanelTransferOut::~MTrsPanelTransferOut()
{
}
//______________________________________________________________________________

void MTrsPanelTransferOut::simulLog(CString strMsg, int iStep, int iSleep)
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
//______________________________________________________________________________

UINT MTrsPanelTransferOut::threadFunction(LPVOID pParam)
{
	// TrsPanelTransfer Object Pointer Assign
	MTrsPanelTransferOut* pThis = (MTrsPanelTransferOut*) pParam;

	TRACE(_T("MTrsPanelTransferOut Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		Sleep(10);
		pThis->threadJob();
	}

	TRACE(_T("MTrsPanelTransferOut Thread Stop\n"));

	return ERR_TRS_PANEL_TRANSFER_OUT_SUCCESS;
}
//______________________________________________________________________________

void MTrsPanelTransferOut::threadJob()
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
			//110130.Add_____
			/*
			if (TRUE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS)
				&& TRUE == m_plnkPanelTransferOut->IsPanelAbsorbed())
			{
				Sleep(1000);//PanelAligner와 IF중 StepStop시 Panel흡착 풀어버리는 현상 때문
			}
			if (TRUE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS)
				&& TRUE == m_plnkPanelTransferOut->IsPanelAbsorbed()
				&& TRUE == m_plnkStatePanelAligner->IsPanelAbsorbed()
#ifdef DEF_GATE_SYSTEM
				&& TRUE == m_plnkStatePanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_1_POS)
#else
				&& TRUE == m_plnkStatePanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_2_POS)
#endif
				)
			{
				m_plnkPanelTransferOut->ReleasePanel();
				Sleep(100);
				m_plnkPanelTransferOut->DownPickUpUDCyl();
			}
			//_______________*/
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
//______________________________________________________________________________

void MTrsPanelTransferOut::doRunStep()
{
	int iResult = ERR_TRS_PANEL_TRANSFER_OUT_SUCCESS;

	switch (m_estepCurrent)
	{
	case PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS:

		//Panel Receive 준비
		//Release Vacuum -> Down & Stopper Down -> Load Pos 이동

		if (m_plnkPanelTransferOut->IsPanelAbsorbed() == TRUE)
		{
			// 437000 = Panel Loading 동작 전에 Panel을 흡착하고 있습니다.
			PROCESS_ALARM(generateErrorCode(437000));
			break;
		}

		iResult = m_plnkPanelTransferOut->DownPickUpUDCyl();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		/*
		//100823.KKY.뒷설비 Panel이 빠져나갔는지 확인_____
		if (TRUE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
		{
			if (TRUE == m_plnkPanelTransferOut->IsPanelDetected())
				break;
		}
		//________________________________________________
		*/

		//170928 JSh Gear
//		iResult = m_plnkPanelTransferOut->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		iResult = m_plnkPanelTransferOut->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(PANEL_TRANSFER_OUT_RECEIVE_PANEL_READY);
		break;

	case PANEL_TRANSFER_OUT_RECEIVE_PANEL_READY:

		setStep(PANEL_TRANSFER_OUT_RECEIVE_PANEL);
		break;

	case PANEL_TRANSFER_OUT_RECEIVE_PANEL:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(PANEL_TRANSFER_OUT_MODEL_CHANGE);
			break;
		}

//170919 JSH.s
// #ifdef DEF_SOURCE_SYSTEM
// //@		if (FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad())
// #	ifndef DEF_SETUP	//@확인요
// 		/*/@21호기 수정___________
// 		if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved4_UpperEqBit)		//@뒷설비 TransferIn에 Panel이 있다
// 			|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved5_UpperEqBit))	//@뒷설비 Alinger가 Panel 받을수 있다
// 		{
// 			initializeITI();
// 			break;
// 		}
// 		/*/
// 		if (FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad()
// 			|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved1_UpperEqBit)
// 			)
// 		{
// 			initializeITI();
// 			break;
// 		}
// 		//@______________________
// #	endif
// #endif
//170919 JSH.e

		m_bITIReceiveAblePanelAligner = TRUE;

		//170928 JSh.s
		if(FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
		{
			//170928 JSH Gear
			//iResult = m_plnkPanelTransferOut->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
			iResult = m_plnkPanelTransferOut->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//170928 JSh.e

		if (FALSE == m_plnkITIPanelAligner->IsSendStartToPanelTransferOut()) break;

		//170928 JSH.s
		if(FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
		{
			Sleep(100);
			if(FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
			{
				PROCESS_ALARM(generateErrorCode(438006));
				break;
			}
		}
		//170928 JSH.e

		//100930.KKY_____
		iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
		{
			iResult = m_plnkPanelTransferOut->AbsorbPanel();
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//_______________

		m_bITIReceiveStartToPanelAligner = TRUE;

		while (TRUE)
		{
			Sleep(5);
			if (TRUE == m_plnkITIPanelAligner->IsSendCompleteToPanelTransferOut()) break;

			if (FALSE == m_plnkITIPanelAligner->IsSendStartToPanelTransferOut()) {
				//100930.KKY_____
				m_plnkPanelTransferOut->ReleasePanel();
				Sleep(100);
				m_plnkPanelTransferOut->DownPickUpUDCyl();
				//_______________
				m_bITIReceiveAblePanelAligner = FALSE;
				m_bITIReceiveStartToPanelAligner = FALSE;
				return;
			}
		}

		m_bITIReceiveAblePanelAligner = FALSE;
		m_bITIReceiveStartToPanelAligner = FALSE;

		setStep(PANEL_TRANSFER_OUT_SEND_PANEL_READY);
		break;

	case PANEL_TRANSFER_OUT_SEND_PANEL_READY:



		//@___________________
		m_dTolerance = 10.0;
		if(FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, &m_dTolerance))
		{
			if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
			{
#	ifndef DEF_SETUP	//@확인요
				//@130913. 21호기 수정________
				if (FALSE == m_plnkTrsAutoManager->IsPanelEmpty_NextMCLoad()
#		ifdef DEF_SOURCE_SYSTEM
					|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved1_UpperEqBit)	//@21호기 수정
					|| m_plnkTrsAutoManager->IsOffPIO_NextMC(eMachineRun) //170928 JSh
					|| m_plnkTrsAutoManager->IsOffPIO_NextMC(eEmergency) //170928 JSh				
#		endif
					)
					break;
#	endif

//SJ_YYK 150330 Modify..
//#	ifdef DEG_GATE_SYSTEM
// 				if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble))
// 					break;
//#	endif
			}
		}
		//@___________________

		//170928 JSh Gear
		//iResult = m_plnkPanelTransferOut->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
		iResult = m_plnkPanelTransferOut->SafeMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(PANEL_TRANSFER_OUT_SEND_PANEL);
		break;

	case PANEL_TRANSFER_OUT_SEND_PANEL:
		if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
		{
			Sleep(3000);

			// Doolin kwangilshin 2017. 09. 15.
			//
			m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_PANEL_TRANSFER_OUT_PANEL);
			// End.

			setStep(PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS);
			break;
		}

		// 2010.08.20 KBC -------------------------
	/*	if (!m_plnkPanelTransferOut->IsPanelAbsorbed() && !m_plnkPanelTransferOut->IsPanelDetected()) {
			initializeMelsecIF();
	//		setStep(PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS);
			break;
		}*/
		//-----------------------------------------
		if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)) break;

		m_plnkMelsecIF->SetLowerEqBit(eSendAble, TRUE);

		m_plnkTrsAutoManager->AssignGlassData(DEF_PANEL_TRANSFER_OUT_PANEL);

		m_timer.StartTimer();
		while (TRUE) {
			Sleep(5);
			if (m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart)) break;
			
			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)) {
				initializeMelsecIF();
				return;
			}	
			if (m_timer.MoreThan(30.0)) {
				initializeMelsecIF();
				// 438001 = 다음 설비로 Panel을 넘겨주던 중 30초 동안 ReceiveStart 신호를 받지 못하였습니다.
				PROCESS_ALARM(generateErrorCode(438001));
				return;
			}
		}
		
		// 다음 설비로 SendStart 신호 보내고
		m_plnkMelsecIF->SetLowerEqBit(eSendStart, TRUE);

		m_timer.StartTimer();
		while (TRUE) {
			Sleep(5);
			if (m_plnkMelsecIF->IsOnLowerEqBit(eArm1FoldComplete)) break;
			
			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)) {
				initializeMelsecIF();
				return;
			}	
			if (m_timer.MoreThan(30.0)) {
				initializeMelsecIF();
				// 428002 = 다음 설비로 Panel을 넘겨주던 중 30초 동안 eArm1FoldComplete 신호를 받지 못하였습니다.
				PROCESS_ALARM(generateErrorCode(438002));
				return;
			}
		}


		//130125.kms______
		//시점 변경....
		//100928.KKY.시점변경_____
//		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_PANEL_TRANSFER_OUT_PANEL);
		//________________________

		// 흡착 해제
		iResult = m_plnkPanelTransferOut->ReleasePanel();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		m_plnkMelsecIF->SetLowerEqBit(eGlassCheckSensorOn, TRUE);

		// 다음 설비로부터 ReceiveComplete 신호 대기
		m_timer.StartTimer();
		while (TRUE) {
			Sleep(5);
			if (TRUE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveComplete)) break;

			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveAble)
				|| FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart)) {
				initializeMelsecIF();
				return;
			}
			if (m_timer.MoreThan(30.0)) {
				initializeMelsecIF();
				// 437003 = 다음 설비로 Panel을 넘겨주던 중 30초 동안 ReceiveComplete 신호를 받지 못하였습니다.
				PROCESS_ALARM(generateErrorCode(438003));
				return;
			}
			
		}

		iResult = m_plnkPanelTransferOut->DownPickUpUDCyl();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		// Doolin kwangilshin 2017. 09. 15.
		//
//  		//130125.kms______
//  		//시점 변경....
//  		//100928.KKY.시점변경_____
//  		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_PANEL_TRANSFER_OUT_PANEL);
//  		//________________________
 		// End.

		// 다음 설비로 SendComplete 신호 보낸다.
		m_plnkMelsecIF->SetLowerEqBit(eSendComplete, TRUE);
		
		m_timer.StartTimer();
		while (TRUE) {
			Sleep(5);
			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReceiveStart))
				break;
			
			if (m_timer.MoreThan(30.0)) {
				initializeMelsecIF();
				// 437004 = 다음 설비로 Panel을 넘겨주던 중 30초 동안 eReceiveStart 신호가 초기화 되지 않았습니다.
				PROCESS_ALARM(generateErrorCode(437004));
				return;
			}
		}

		// Doolin kwangilshin 2017. 09. 15.
		//
		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_PANEL_TRANSFER_OUT_PANEL);
 		// End.


		// All Interface Signal Reset : WorkStart to Lower Eq 신호 포함.
		initializeMelsecIF();
		m_plnkMelsecIF->SetLowerEqBit(eWorkStart_LowerEq, FALSE);

		//120111.kms__________
		//ModelChange 시 상위 에러 보고 안함
		//Panel Out 할때 False 시켜 알람 보고 하도록 한다.
		m_plnkTrsAutoManager->SetModelChangeAlarmReport(FALSE);
		//____________________

		//100928.KKY.시점변경_____
//		m_plnkTrsAutoManager->ManageGlassData(ePanelOut, DEF_PANEL_TRANSFER_OUT_PANEL);
		//________________________
		setStep(PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS);
		break;

	// NSMC
	case PANEL_TRANSFER_OUT_MODEL_CHANGE:
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;

		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;

		m_bModelChangeReady = FALSE;

		setStep(PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS);
		break;
	}
}
//______________________________________________________________________________

void MTrsPanelTransferOut::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepPanelTransferOut)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepPanelTransferOut)0;	//  현재 Step Clear
}
//______________________________________________________________________________

void MTrsPanelTransferOut::setStep(EStepPanelTransferOut estepPanelTransferOut)
{
	m_estepPrevious	= m_estepCurrent;				// 이전 Step 저장
	m_estepCurrent	= estepPanelTransferOut;		// 새로 Step으로 전환
}
//______________________________________________________________________________

int MTrsPanelTransferOut::AssignComponents(STrsPanelTransferOutRefCompList listRefComponents)
{
	m_plnkTrsAutoManager		= listRefComponents.m_plnkTrsAutoManager;
	m_plnkPanelTransferOut		= listRefComponents.m_plnkPanelTransferOut;
	m_plnkStatePanelAligner		= listRefComponents.m_plnkStatePanelAligner;
	m_plnkMelsecIF				= listRefComponents.m_plnkMelsecIF;
	m_plnkPanelTransferIn       = listRefComponents.m_plnkPanelTransferIn;

	return ERR_TRS_PANEL_TRANSFER_OUT_SUCCESS;
}
//______________________________________________________________________________

int MTrsPanelTransferOut::SetData(STrsPanelTransferOutData datComponent)
{
	m_plnkSystemData	= datComponent.m_plnkSystemData;

	return ERR_TRS_PANEL_TRANSFER_OUT_SUCCESS;
}
//______________________________________________________________________________

int MTrsPanelTransferOut::Initialize()
{
	m_bAutoInitialized = FALSE;
	initializeITI();
	initializeStep();

	////임시 20100823 sj_ysb
	//m_plnkPanelTransferOut->AirCVFloatingOn();
//@	m_plnkPanelTransferOut->AirCVBlowOn();

#ifndef SIMULATION
	int iResult = m_plnkPanelTransferOut->CheckOrigin();
	if (iResult) return iResult;

	if (m_plnkPanelTransferOut->IsPanelAbsorbed()) {
		setStep(PANEL_TRANSFER_OUT_SEND_PANEL_READY);
	}
	else {
		//120223.kms___________
		//Transfer Out  이 Loading  위치도 Unloading 위치도 아닐 경우 Loading 
		//위치로 이동 한다.
		if(FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS)
		&& FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
		{
			iResult = m_plnkPanelTransferOut->DownPickUpUDCyl();
			if (iResult)
				return iResult;
			iResult = m_plnkPanelTransferOut->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
			if (iResult)
				return iResult;
		}
		//____________________

//		#pragma message(__LOC__ "MTrsPanelTransferOut::Initialize() 초기화 부분 수정요")
		/*/
		//120213.kms___________
#ifdef DEF_SOURCE_SYSTEM
		//120213.kms___________
		//초기화 부분 에 대해 수정
		if(TRUE == m_plnkPanelTransferOut->IsPanelDetected() &&
		   FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
		{
			iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
			if (iResult)
				return iResult;

			iResult = m_plnkPanelTransferOut->AbsorbPanel();
			m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
			m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.

			// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
			if (SUCCESS == iResult)
			{
				// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
				return generateErrorCode(424007);
			}
		}
		else if(TRUE == m_plnkPanelTransferOut->IsPanelDetected() &&
		        FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS) &&
				FALSE == m_plnkStatePanelAligner->IsPanelAbsorbed())
		{
			if(m_plnkPanelTransferIn->IsPanelAbsorbed())
			{
				if(m_plnkPanelTransferIn->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS) == FALSE)
				{
					iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
					if (iResult)
						return iResult;
					
					iResult = m_plnkPanelTransferOut->AbsorbPanel();
					m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
					m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.
					
					// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
					if (SUCCESS == iResult)
					{
						// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
						return generateErrorCode(424007);
					}
				}
			}
			else
			{
				iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
				if (iResult)
					return iResult;

				iResult = m_plnkPanelTransferOut->AbsorbPanel();
				m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
				m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.

				// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
				if (SUCCESS == iResult)
				{
					// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
					return generateErrorCode(424007);
				}

			}
		}
		//______________________________________________

#else
		/*/

		//초기화 부분 에 대해 수정
		if(TRUE == m_plnkPanelTransferOut->IsPanelDetected() &&
		   FALSE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
		{
			iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
			if (iResult)
				return iResult;

			iResult = m_plnkPanelTransferOut->AbsorbPanel();
			m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
			m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.

			// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
			if (SUCCESS == iResult)
			{
				// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
				return generateErrorCode(424007);
			}
		}
		else if(TRUE == m_plnkPanelTransferOut->IsPanelDetected() &&
		        TRUE == m_plnkPanelTransferOut->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
		{
			if(TRUE == m_plnkStatePanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_LOAD_POS))
			{
				iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
				if (iResult)
					return iResult;

				iResult = m_plnkPanelTransferOut->AbsorbPanel();
				m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
				m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.

				// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
				if (SUCCESS == iResult)
				{
					// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
					return generateErrorCode(424007);
				}
			}
			else   //Panel Aligner Unload Pos
			{
				if(FALSE == m_plnkStatePanelAligner->IsPanelAbsorbed())
				{
					iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
					if (iResult)
						return iResult;
					
					iResult = m_plnkPanelTransferOut->AbsorbPanel();
					m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
					m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.
					
					// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
					if (SUCCESS == iResult)
					{
						// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
						return generateErrorCode(424007);
					}
				}
			}
		}
//@#endif
		//______________________________________________


		//120224.KMS____________
		/*/
		if(TRUE == m_plnkPanelTransferOut->IsPanelUnLoadingDetected())
		{
			if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eReserved6_LowerEqBit))
			{
				//426005 = Panel Transfer Out 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다.
				return generateErrorCode(426005);
			}
		}
		/*/
		//______________________

		if (m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_OUT_PANEL]->m_strEPanelID.GetLength() > 1) {
			CString strMsg = 
//				_T("PanelTransferOut 가 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?\r\nPanelTransferOut has a Panel ID but released a vacuum. Scrap(delete) the Panel ID?\r\nPanel Scrap");
				_T("PanelTransferOut 이 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?\r\nPanelTransfer Out is Vacuum Off state. but there is Panel ID. Do you want Panel Scrap?");
			if (MyMessageBox(strMsg, _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_OUT_PANEL);	
		}



		iResult = m_plnkPanelTransferOut->DownPickUpUDCyl();
		if (iResult)
			return iResult;
		setStep(PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS);
	}
	/*/// 2010.08.02 KBC -----------------------------------
	if (m_plnkPanelTransferOut->IsPanelAbsorbed() == TRUE)
	{
		setStep(PANEL_TRANSFER_OUT_SEND_PANEL_READY);
	}
	else // Panel 흡착하고 있지 않음
	{
		// 부상 유닛에 Panel 있는지 확인
		// Panel 감지 센서의 오감지에 대한 확인 작업임. => 센서 오감지의 경우는 에러 처리 무시.
		if (TRUE == m_plnkPanelTransferOut->IsPanelDetected()
			&& FALSE == m_plnkStatePanelAligner->IsPanelAbsorbed())
		{
			iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
			if (iResult)
				return iResult;

			iResult = m_plnkPanelTransferOut->AbsorbPanel();
			m_plnkPanelTransferOut->ReleasePanel();	// Error 처리하면 안됨.
			m_plnkPanelTransferOut->DownPickUpUDCyl();	// Error 처리하면 안됨.

			// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
			if (SUCCESS == iResult)
			{
				// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
				return generateErrorCode(424007);
			}
		}

		if (m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_PANEL]->m_strEPanelID.GetLength() > 1)		
		{
			// ML_hongju
//			CString strMsg = ML_MSG("PanelTransfer 가 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?",
//				"PanelTransfer has a Panel ID but released a vacuum. Scrap(delete) the Panel ID?\r\nPanel Scrap");
//			if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
//				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_PANEL);	
			if (MyMessageBox(_T("PanelTransfer has a Panel ID but released a vacuum. Scrap(delete) the Panel ID?\r\nPanel Scrap"), _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_PANEL);	
		}

		iResult = m_plnkPanelTransferOut->DownPickUpUDCyl();
		if (iResult)
			return iResult;

		// Panel Transfer에 Panel 없을때 ITI
// 2010.07.29 KBC		m_bITILoadConveyorEnabletoReceivePanel = TRUE;
	}
#else
// 2010.07.29 KBC	m_bITILoadConveyorEnabletoReceivePanel = TRUE;
#endif
	/*/// 2010.08.02 KBC -----------------------------------
#endif

	m_bAutoInitialized  = TRUE;
	return ERR_TRS_PANEL_TRANSFER_OUT_SUCCESS; 
}
//______________________________________________________________________________

void MTrsPanelTransferOut::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}
//______________________________________________________________________________

void MTrsPanelTransferOut::AssignITI(ITI_PanelAligner* pITI_PanelAligner)
{
	m_plnkITIPanelAligner	= pITI_PanelAligner;
}
//______________________________________________________________________________

void MTrsPanelTransferOut::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}
//______________________________________________________________________________

void MTrsPanelTransferOut::ThreadStop()
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
//______________________________________________________________________________

void MTrsPanelTransferOut::initializeITI()
{
 	m_bITIReceiveAblePanelAligner		= FALSE;
	m_bITIReceiveStartToPanelAligner	= FALSE;

	m_bITISendAbleToNextMachine			= FALSE;
	m_bITISendCompleteToNextMachine		= FALSE;

	// NSMC
	m_bModelChangeReady					= FALSE;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsInitialized()
{
	return m_bAutoInitialized;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsReceiveAbleToPanelAligner()
{
	return m_bITIReceiveAblePanelAligner;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsReceiveStartToPanelAligner()
{
	return m_bITIReceiveStartToPanelAligner;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsSendAbleToNextMachine()
{
	return m_bITISendAbleToNextMachine;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsSendCompleteToNextMachine()
{
	return m_bITISendCompleteToNextMachine;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsEmptyPanel()
{
	BOOL bResult = m_plnkPanelTransferOut->IsPanelReleased();
	return bResult;
}
//______________________________________________________________________________

int MTrsPanelTransferOut::GetStep()
{
	return m_estepCurrent;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferOut::IsAutoRun()
{
	return m_bAutoRun;
}
//______________________________________________________________________________

CString MTrsPanelTransferOut::GetItiStatus()
{
	m_strItiStatus.Format(_T("ReceiveA: %d, ReceiveS: %d, SendS: %d, SendC: %d"), 
		m_bITIReceiveAblePanelAligner, m_bITIReceiveStartToPanelAligner,
		m_bITISendAbleToNextMachine, m_bITISendCompleteToNextMachine);

	return m_strItiStatus;
}
//______________________________________________________________________________

void MTrsPanelTransferOut::initializeMelsecIF()
{
	m_plnkMelsecIF->SetLowerEqBit(eSendAble, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eSendStart, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eSendComplete, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eGlassCheckSensorOn, FALSE);
	m_plnkMelsecIF->SetLowerEqBit(eImmediatelyPauseRequest, FALSE);
}
//______________________________________________________________________________

int MTrsPanelTransferOut::ScrapPanel()
{
	int iResult = SUCCESS;

	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_OUT_PANEL);

	if (TRUE == m_plnkPanelTransferOut->IsPanelAbsorbed())
	{
		CString strMsg = _T("There is a panel on Panel Transfer. Remove it.");
		
		do
		{
			iResult = m_plnkPanelTransferOut->ReleasePanel();
			if (iResult)
				return iResult;
			
			Sleep(500);

			if(MyMessageBox(MY_MESSAGE_BOX, 62600, _T("Question Scrap"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) ==IDNO)
				return FALSE;

//			MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Remove Panel Request", M_ICONINFORMATION);
			MyMessageBox(MY_MESSAGE_BOX, 62300, "Remove Panel Request", M_ICONINFORMATION);

			m_plnkPanelTransferOut->AbsorbPanel(TRUE);
			Sleep(300);

		} while (TRUE == m_plnkPanelTransferOut->IsPanelAbsorbed());
	}
	
	m_plnkPanelTransferOut->ReleasePanel(TRUE);

	return Initialize();
}
//______________________________________________________________________________

// NSMC
BOOL MTrsPanelTransferOut::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsPanelTransferOut::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

//______________________________________________________________________________

// End Of Code

