//______________________________________________________________________________

// File Name: MTrsPanelTransferCenter.cpp
// History	:
//		1. 2010.07.29 : Created
//		2.
//______________________________________________________________________________

#include "StdAfx.h"
#include "MTrsPanelTransferCenter.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPanelTransfer.h"
#include "MPanelAligner.h"
#include "IStatePanelAligner.h"
//#include "IStateRollerConveyor.h"
#include "MTrsPanelTransferCenter.h"
#include "MTrsAutoManager.h"
#include "ITI_LoadConveyor.h"
#include "ITI_PanelAligner.h"
#include "ITI_PanelTransfer.h"
#include "ITI_THandler.h" //SJ_YYK 161104 Add..
#include "IStateHandler.h"
#include "common.h"
#include "MTickTimer.h"
//#include "MAirCleaner.h"
//______________________________________________________________________________

MTrsPanelTransferCenter::MTrsPanelTransferCenter(SCommonAttribute	commonData,
									STrsPanelTransferCenterRefCompList			listRefComponents,
									STrsPanelTransferCenterData					datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	m_bAutoInitialized  = FALSE;

	m_bThreadLife		= FALSE;
	m_pThread			= NULL;
	m_bAutoRun			= FALSE;
	m_bPanelInStop = FALSE;
	m_bModelChangeReady	= FALSE;

	initializeITI();
	initializeStep();
}
//______________________________________________________________________________

MTrsPanelTransferCenter::~MTrsPanelTransferCenter()
{
}
//______________________________________________________________________________

void MTrsPanelTransferCenter::simulLog(CString strMsg, int iStep, int iSleep)
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

UINT MTrsPanelTransferCenter::threadFunction(LPVOID pParam)
{
	// TrsPanelTransfer Object Pointer Assign
	MTrsPanelTransferCenter* pThis = (MTrsPanelTransferCenter*) pParam;

	TRACE(_T("MTrsPanelTransferCenter Thread Run\n"));

	// Thread Loop
	while (pThis->m_bThreadLife)
	{
		Sleep(10);
		pThis->threadJob();
	}

	TRACE(_T("MTrsPanelTransferCenter Thread Stop\n"));

	return ERR_TRS_PANEL_TRANSFER_CENTER_SUCCESS;
}
//______________________________________________________________________________

void MTrsPanelTransferCenter::threadJob()
{
	//170201 SJ_YSH
	if(m_plnkPanelTransferCenter->IsPanelAbsorbed() == TRUE 
		|| m_plnkStatePanelTransfer->IsPanelAbsorbed() == TRUE
#ifdef DEF_USE_TURN_HANDLER
		|| m_plnkStateTurnHandler->IsAbsorbPanel() == TRUE 
#endif
		)
	{
		m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
	}
	else
	{
		m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
	}
	//_____________

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
			m_bAutoRun = FALSE;
			break;

		case START_RUN:
			initializeMelsecIF();//110211.Add
			m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);//110211.Add
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

void MTrsPanelTransferCenter::doRunStep()
{
	int iResult = ERR_TRS_PANEL_TRANSFER_CENTER_SUCCESS;

	switch (m_estepCurrent)
	{
	case PANEL_TRANSFER_CENTER_MOVE_TO_LOAD_POS:

		if (m_plnkPanelTransferCenter->IsPanelAbsorbed() == TRUE)
		{
			// 424000 = Panel Loading 동작 전에 Panel을 흡착하고 있습니다.
			PROCESS_ALARM(generateErrorCode(425000));
			break;
		}
		// Pickup Down
		iResult = m_plnkPanelTransferCenter->DownPickUpUDCyl();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

#ifdef DEF_USE_TRANSFER_DONT_MOVE_SIGNAL
		//100823.KKY.Add.
		//대형일 경우 Transfer가 Panel을 가지고 있을때 앞설비 Unload Transfer가 Unloading위치로 들어오면 충돌발생
		if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_46INCH + 10.0)
			m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		else
		{
			if (m_plnkStatePanelTransfer->IsPanelAbsorbed())
				m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
			else
				m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		}
		//___________________
#endif

		// Loading Pos로 이동
		//170929 KDW Gear
		//iResult = m_plnkPanelTransferCenter->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		iResult = m_plnkPanelTransferCenter->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(PANEL_TRANSFER_CENTER_RECEIVE_PANEL_READY);
		break;

	case PANEL_TRANSFER_CENTER_RECEIVE_PANEL_READY:
	//20100822 sj_ysb	
	/*if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE) {
			setStep(PANEL_TRANSFER_CLEAN);
			break;
		}
*/
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(PANEL_TRANSFER_CENTER_MODEL_CHANGE);
			break;
		}

		if (IsPanelInStop())
		{
			m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
			break;
		}
		
		//SJ_YYK 110323 Add. 60inch 대응///
#	ifdef DEF_SOURCE_SYSTEM
		if(m_plnkStatePanelTransfer->IsPanelAbsorbed() == TRUE)
			break;
		
		//SJ_PJC 20170122 Turn Handler 간섭
#		ifdef DEF_USE_TURN_HANDLER
		if (m_plnkStateTurnHandler->IsAbsorbPanel() == TRUE)
			break;
#		endif
		//_________________________________
		//if(m_plnkPanelData->m_dPanelSize_Y > 705.0)
		if(m_plnkPanelData->m_dPanelSize_Y > 990.0) // 170207 SJ_YSH 78" Panel Y Size 987.0 
		{
			//170206 JSH.s Panel 충돌위험
			if(m_plnkPanelAligner->IsPanelAbsorbed() == TRUE && m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_LOAD_POS) == TRUE)
				break;
			
			//170206 JSH.e
			//m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
			//요거 까지 보고 가는지는  확인 후 추가...
			//일단 Transfer In Vacuum 만 확인하고 Start 하도록....
			//if (m_plnkITIPanelTransfer->IsReceiveAbleToPanelTransferCenter() == FALSE) break;
		}
#	endif
		setStep(PANEL_TRANSFER_CENTER_RECEIVE_PANEL);
		break;

	case PANEL_TRANSFER_CENTER_RECEIVE_PANEL:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(PANEL_TRANSFER_CENTER_MODEL_CHANGE);
			break;
		}

//		//100823.Add________
//		if (m_plnkStatePanelTransfer->IsPanelAbsorbed() || 
//			m_plnkStatePanelTransfer->IsAirCVVacPlate1On() || m_plnkStatePanelTransfer->IsAirCVVacPlate2On())
//		{
//			initializeMelsecIF();
//			break;
//		}
		//__________________

		if (IsPanelInStop())
		{	
			m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
			break;
		}

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{

#ifdef DEF_USE_TRANSFER_DONT_MOVE_SIGNAL
			//100823.KKY.Add_____
			if (m_plnkPanelData->m_dPanelSize_X < DEF_PANEL_SIZE_46INCH + 10.0)
				m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
			else
			{
				if (m_plnkStatePanelTransfer->IsPanelAbsorbed())
				{
//					if (FALSE != m_plnkMelsecIF->IsOnUpperEqBit(eReserved5_UpperEqBit))
					m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
					break;
				}
				else
					m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
			}
			//___________________
#else
#endif
			
			//170204 SJ_YSH
			//PanelAligner가 First Align Pos 일때 Trnasfer In Loading 가능
			double dbReceiveAbleTolerance		= 10;
			double dbPanelAlignCurrPos			= m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_X_AXIS)->GetCurrentPosition();
			double dbPanelAlignFirstAlignPos	= m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_FIRST_MARK_POS).dX - dbReceiveAbleTolerance;

			// Doolin kwangilshin 2017. 09. 05.
			//
			if ((m_plnkPanelAligner->IsPanelAbsorbed() && dbPanelAlignCurrPos < dbPanelAlignFirstAlignPos) ||
				(m_plnkPanelTransferCenter->IsPanelAbsorbed() == TRUE) ||
				(m_plnkStatePanelTransfer->IsPanelAbsorbed() == TRUE  || m_plnkStatePanelTransfer->IsPanelDetected())
#ifdef DEF_USE_TURN_HANDLER
				|| (m_plnkStateTurnHandler->IsAbsorbPanel() == TRUE)
#endif	
				)
			{
				m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
				break;
			}
			//_____________

			// 2010.08.20 KBC -----------------------------------
			// 1. ReceiveAble On
			m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, TRUE);
			// 2. SendAble Check
			if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) break;			

			//SJ_YYK 111004 Modify..
			if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE && m_plnkLCNet->m_eEqState != ePM)
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
							return;
						}
						else if (iResult == ERR_CNTOVER_PRODUCT_PLAN)
						{
							PROCESS_ALARM(generateErrorCode(437009));
							return;
						}
					}
				}
				if (m_plnkSystemData->m_bUseTXDataCheckMode == TRUE)
				{
					iResult = m_plnkTrsAutoManager->CheckPanelValidationItem(DEF_PANEL_TRANSFER_CENTER_PANEL);
					switch (iResult)
					{
					case ERR_HPANELID:
						// 427020 = Panel에 HPanelID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437020));
						return;						
					case ERR_EPANELID:		
						// 427021 = Panel에 EPANELID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437021));
						return;
					case ERR_BATCHID:		
						// 427022 = Panel에 BATCHID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437022));
						return;			
					case ERR_PRODTYPE:		
						// 427023 = Panel에 PRODTYPE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437023));
						return;		
					case ERR_PRODKIND:		
						// 427024 = Panel에 PRODKIND가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437024));
						return;		
					case ERR_DEVICEID:		
						// 427025 = Panel에 DEVICEID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437025));
						return;		
					case ERR_STEPID:		
						// 427026 = Panel에 STEPID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437026));
						return;		
					case ERR_PPID:		
						// 427027 = Panel에 PPID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437027));
						return;			
					case ERR_THICKNESS:		
						// 427028 = Panel에 THICKNESS가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437028));
						return;		
					case ERR_INSFLAG:		
						// 427029 = Panel에 INSFLAG가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437029));
						return;
					case ERR_PANELSIZE:		
						// 427030 = Panel에 PANELSIZE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437030));
						return;		
					case ERR_PANELPOSITION:		
						// 427031 = Panel에 PANELPOSITION가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437031));
						return;	
					case ERR_COUNT1:		
						// 427032 = Panel에 COUNT1가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437032));
						return;			
					case ERR_COUNT2:		
						// 427033 = Panel에 COUNT2가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437033));
						return;			
					case ERR_GRADE:		
						// 427034 = Panel에 GRADE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437034));
						return;			
					case ERR_COMMENT:		
						// 427035 = Panel에 COMMENT가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437035));
						return;			
					case ERR_COMPSIZE:		
						// 427036 = Panel에 COMPSIZE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437036));
						return;		
					case ERR_READINGFLAG:		
						// 427037 = Panel에 READINGFLAG가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437037));
						return;		
					case ERR_PANELSTATE:		
						// 427038 = Panel에 PANELSTATE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437038));
						return;		
					case ERR_JUDGEMENT:		
						// 427039 = Panel에 JUDGEMENT가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437039));
						return;		
					case ERR_CODE:		
						// 427040 = Panel에 CODE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437040));
						return;			
					case ERR_RUNLINE:		
						// 427041 = Panel에 RUNLINE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437041));
						return;			
					case ERR_UNIQUEID:		
						// 427042 = Panel에 UNIQUEID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437042));
						return;		
					case ERR_PAIRHPANELID:		
						// 427043 = Panel에 PAIRHPANELID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437043));
						return;	
					case ERR_PAIREPANELID:		
						// 427044 = Panel에 PAIREPANELID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437044));
						return;	
					case ERR_PAIRGRADE:		
						// 427045 = Panel에 PAIRGRADE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437045));
						return;		
					case ERR_PAIRUNIQUEID:		
						// 427046 = Panel에 PAIRUNIQUEID가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437046));
						return;	
					case ERR_FLOWRECIPE:		
						// 427047 = Panel에 FLOWRECIPE가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437047));
						return;		
					case ERR_RESERVED0:		
						// 427048 = Panel에 RESERVED0가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437048));
						return;		
					case ERR_BITSSIGNALS:		
						// 427049 = Panel에 BITSSIGNALS가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437049));
						return;		
					case ERR_REFERDATA:		
						// 427050 = Panel에 REFERDATA가 없습니다. Clean Stage 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437050));
						return;		
					}
				}
			}

			//SJ_YYK 110401 Add.. Panel Validation 기능 추가..
//#ifdef DEF_SOURCE_SYSTEM
			// Panel Validation Check
			if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
			{
				if (m_plnkSystemData->m_bUsePanelValidation == TRUE)
				{
					iResult = m_plnkTrsAutoManager->CheckPanelValidation(DEF_PANEL_TRANSFER_CENTER_PANEL);
					switch (iResult)
					{
					case ERR_NO_BATCHID_DATA:
						// 427011 = Panel에 Batch ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437004));
						return;
					case ERR_NO_DEVICEID_DATA:
						// 427012 = Panel에 Device ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437005));
						return;
					case ERR_NO_PANELID_DATA:
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
						// 427015 = Panel에 이전 설비로부터 넘겨받은 ID Data가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
						PROCESS_ALARM(generateErrorCode(437007));
						return;
					}
				}
			}
//#endif

			// 3. SendSart On
			m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, TRUE);
			// 4. SendStart 대기	
			m_timer.StartTimer();
			while (TRUE) {
				Sleep(5);
				//20100822 sj_ysb
				/*if (m_plnkMelsecIF->IsOnLowerEqBit(eSendStart)) 
					break;
				
				if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eSendAble)) {
					initializeMelsecIF();
					return;
				}
				*/
				if (m_plnkMelsecIF->IsOnUpperEqBit(eSendStart)) 
					break;
				
				if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) {
					initializeMelsecIF();
					return;
				}
				if (m_timer.MoreThan(30.0)) {
					initializeMelsecIF();
					// 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 SendStart 신호를 받지 못하였습니다.
					PROCESS_ALARM(generateErrorCode(437000));
					return;
				}
			}
		}

		// 5. Cylinder Up & Absorb
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			iResult = m_plnkPanelTransferCenter->UpPickUpUDCyl();
			if (iResult) {
				PROCESS_ALARM(iResult);
				break;
			}
		}

		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			iResult = m_plnkPanelTransferCenter->AbsorbPanel();
			if (iResult) {
				PROCESS_ALARM(iResult);
				break;
			}
			

			//110929 sj_kjs 시점변경..
			m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_PANEL_TRANSFER_CENTER_PANEL);

			// 6. Arm1FoldComplete On
			m_plnkMelsecIF->SetUpperEqBit(eArm1FoldComplete, TRUE);

			m_timer.StartTimer();
			while (true) {
				Sleep(5);
				/*if (m_plnkMelsecIF->IsOnLowerEqBit(eGlassCheckSensorOn)) 
					break;

				if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eSendAble)) {
					initializeMelsecIF();
					return;
				}
				*/
				if (m_plnkMelsecIF->IsOnUpperEqBit(eGlassCheckSensorOn)) 
					break;

				if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) {
					initializeMelsecIF();
					return;
				}
				if (m_timer.MoreThan(30.0)) {
					initializeMelsecIF();
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
				/*if (m_plnkMelsecIF->IsOnLowerEqBit(eSendComplete)) break;

				if (FALSE == m_plnkMelsecIF->IsOnLowerEqBit(eSendAble)) {
					initializeMelsecIF();
					return;
				}
				*/
				if (m_plnkMelsecIF->IsOnUpperEqBit(eSendComplete)) break;

				if (FALSE == m_plnkMelsecIF->IsOnUpperEqBit(eSendAble)) {
					initializeMelsecIF();
					return;
				}
				if (m_timer.MoreThan(30.0)) {
					initializeMelsecIF();
					// 이전 설비로부터 Panel을 넘겨받는 중 30초 동안 SendComplete 신호를 받지 못하였습니다.
					PROCESS_ALARM(generateErrorCode(437002));
					return;
				}
			}
		}

		// 8. Init
		initializeMelsecIF();

		//100823.KKY.Add_____
		m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
		//___________________
		//120224.kms________
		m_plnkMelsecIF->SetUpperEqBit(eReserved4_UpperEqBit, TRUE);
		//__________________

		// Doolin kwangilshin 2017. 09. 15.
		//
		m_plnkTrsAutoManager->ManageGlassData(ePanelIn, DEF_PANEL_TRANSFER_CENTER_PANEL);
		// End.


		setStep(PANEL_TRANSFER_CLEAN);
		break;

	case PANEL_TRANSFER_CLEAN:
		setStep(PANEL_TRANSFER_CENTER_SEND_PANEL_READY);
		break;

	case PANEL_TRANSFER_CENTER_SEND_PANEL_READY:

		//SJ_YYK 110401 Add.. Panel Validation 기능 추가..
//#ifdef DEF_SOURCE_SYSTEM
		// Panel Validation Check
		if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			if (m_plnkSystemData->m_bUsePanelValidation == TRUE)
			{
				iResult = m_plnkTrsAutoManager->CheckPanelValidation(DEF_PANEL_TRANSFER_CENTER_PANEL);
				switch (iResult)
				{
				case ERR_NO_BATCHID_DATA:
					// 427011 = Panel에 Batch ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(437004));
					return;
				case ERR_NO_DEVICEID_DATA:
					// 427012 = Panel에 Device ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(437005));
					return;
				case ERR_NO_PANELID_DATA:
					// 427013 = Panel에 Panel ID가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(437006));
					return;
					/* 확인 필요 : 삭제 할까?
					case ERR_SAME_PANELID_IN_LAST_5TIME:
					// 427014 = Panel ID가 최근 생산된 5개의 패널중 하나와 일치합니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(427014));
					return;
					*/
				case ERR_NO_PANEL_DATA:
					// 427015 = Panel에 이전 설비로부터 넘겨받은 ID Data가 없습니다. Load Conveyor 로부터 Panel 을 제거해 주세요.
					PROCESS_ALARM(generateErrorCode(437007));
					return;
				}
			}
		}
//#endif

		// 2010.08.02 KBC -----------------------------
		// InterLock 
		// 1. Receivable 이 아니면 이동할 수 없다
		// 2. 판넬이 흡착되어 있고 Unloading Position이 아니면 이동할 수 없다
		if (m_plnkITIPanelTransfer->IsReceiveAbleToPanelTransferCenter() == FALSE) 
			break;
		/*if (!m_plnkStatePanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS) &&
			 m_plnkStatePanelTransfer->IsPanelAbsorbed()) break;
		//---------------------------------------------
		*/
//		if (m_plnkITIPanelTransfer->IsReceiveAbleToPanelTransferCenter() == FALSE) break;
		if (m_plnkITITHandler->IsLoadingReady() == FALSE) 
			break;

#ifdef DEF_USE_TURN_HANDLER
		if (m_plnkStateTurnHandler->IsAbsorbPanel() == TRUE)
			break;
#endif
		
		//170207 SJ_YSH
		// Doolin kwangilshin 2017. 09. 05.
		//
		if(m_plnkStatePanelTransfer->IsPanelDetected() == TRUE && m_plnkStatePanelTransfer->IsPanelAbsorbed() == TRUE)
			break;
		//_____________

		double dPanelAlingerMarkposX;
		double dPanelAlignerCurPosX;

		dPanelAlingerMarkposX = m_plnkPanelAligner->GetXYTTargetPos(DEF_PANEL_ALIGNER_FIRST_MARK_POS).dX - 100.0;
		dPanelAlignerCurPosX = m_plnkPanelAligner->GetXYTCurrentPos().dX;

		//if(m_plnkPanelAligner->IsPanelAbsorbed() == TRUE && m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_LOAD_POS) == TRUE)
		if(m_plnkPanelAligner->IsPanelAbsorbed() == TRUE && dPanelAlignerCurPosX < dPanelAlingerMarkposX)
			break;
		//_____________

		//170929 KDW Gear
		//iResult = m_plnkPanelTransferCenter->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
		iResult = m_plnkPanelTransferCenter->SafeMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		//100823.KKY.Add_____
//		m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		//___________________
		//SJ_PJC 20170122 Turn Handler 간섭
		//m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		if(m_plnkStatePanelTransfer->IsPanelAbsorbed()
#ifdef DEF_USE_TURN_HANDLER
		|| m_plnkStateTurnHandler->IsAbsorbPanel() == TRUE
#endif
		)
		{
			m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
		}
		else
		{
			m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		}
		//_________________________________

		setStep(PANEL_TRANSFER_CENTER_SEND_PANEL);
		break;

	case PANEL_TRANSFER_CENTER_SEND_PANEL:


		if(m_plnkITITHandler->IsLoadingReady() == FALSE) 
			break;
		m_bITISendStartToTHandler = TRUE;

		while (TRUE)
		{
			Sleep(5);
			if (TRUE == m_plnkITITHandler->IsLoadingComplete()) 
				break;
		}

		m_bITISendCompleteToTHandler = TRUE;

		while (TRUE)
		{
			Sleep(5);
			if (FALSE == m_plnkITITHandler->IsLoadingComplete()
				|| FALSE == m_plnkITITHandler->IsLoadingReady())
				break;
		}
		initializeITI();

		//SJ_PJC 20170122 Turn Handler 간섭
		//m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		if(m_plnkStatePanelTransfer->IsPanelAbsorbed()
#ifdef DEF_USE_TURN_HANDLER
			|| m_plnkStateTurnHandler->IsAbsorbPanel() == TRUE
#endif
			)
		{
			m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
		}
		else
		{
			m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, TRUE);
		}
		//_________________________________

		m_plnkTrsAutoManager->TransferGlassData(DEF_PANEL_TRANSFER_CENTER_PANEL, DEF_THANDLER_PANEL); //170123 SJ_YSH Add.


		setStep(PANEL_TRANSFER_CENTER_MOVE_TO_LOAD_POS);
		break;

	//NSMC KJS
	case PANEL_TRANSFER_CENTER_MODEL_CHANGE: simulLog("PANEL_TRANSFER_CENTER_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;

		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;

		m_bModelChangeReady = FALSE;

		setStep(PANEL_TRANSFER_CENTER_MOVE_TO_LOAD_POS);
		break;

	default	:
		break;

	}
}
//______________________________________________________________________________

void MTrsPanelTransferCenter::initializeStep()
{
	// 자동운전 STEP 초기화
	m_estepPrevious	= (enumStepPanelTransferCenter)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepPanelTransferCenter)0;	//  현재 Step Clear
}
//______________________________________________________________________________

void MTrsPanelTransferCenter::setStep(EStepPanelTransferCenter estepPanelTransfer)
{
	m_estepPrevious	= m_estepCurrent;	// 이전 Step 저장
	m_estepCurrent	= estepPanelTransfer;		// 새로 Step으로 전환
}
//______________________________________________________________________________

int MTrsPanelTransferCenter::AssignComponents(STrsPanelTransferCenterRefCompList listRefComponents)
{
	m_plnkTrsAutoManager		= listRefComponents.m_plnkTrsAutoManager;
	m_plnkPanelTransferCenter	= listRefComponents.m_plnkPanelTransferCenter;
	m_plnkPanelAligner			= listRefComponents.m_plnkPanelAligner; //170205 SJ_YSH
	m_plnkStatePanelTransfer	= listRefComponents.m_plnkStatePanelTransfer;
#ifdef DEF_USE_TURN_HANDLER
	m_plnkStateTurnHandler		= listRefComponents.m_plnkStateTurnHandler; //SJ_YYK 161104 Add.
#endif
	m_plnkMelsecIF				= listRefComponents.m_plnkMelsecIF;
	//SJ_YYK 110421 Add. Ellas..
	m_plnkLCNet					= listRefComponents.m_plnkLCNet;

	return ERR_TRS_PANEL_TRANSFER_CENTER_SUCCESS;
}
//______________________________________________________________________________

int MTrsPanelTransferCenter::SetData(STrsPanelTransferCenterData datComponent)
{
	m_plnkPanelData		= datComponent.m_plnkPanelData;
	m_plnkSystemData	= datComponent.m_plnkSystemData;

	return ERR_TRS_PANEL_TRANSFER_CENTER_SUCCESS;
}
//______________________________________________________________________________

int MTrsPanelTransferCenter::Initialize()
{
	m_bAutoInitialized = FALSE;
	initializeITI();
	initializeStep();

	//100823.KKY.Add_____
	m_plnkMelsecIF->SetUpperEqBit(eReserved5_UpperEqBit, FALSE);
	//___________________

	//임시 20100823 sj_ysb
//@	m_plnkPanelTransferCenter->AirCVBlowOn();

#ifndef SIMULATION
	int iResult = m_plnkPanelTransferCenter->CheckOrigin();
	if (iResult) return iResult;

	if (m_plnkPanelTransferCenter->IsPanelAbsorbed()) {
		setStep(PANEL_TRANSFER_CENTER_SEND_PANEL_READY);
	}
	else 
	{
		if (m_plnkPanelTransferCenter->IsPanelDetected() && !m_plnkStatePanelTransfer->IsPanelAbsorbed())
		{
			if (FALSE == m_plnkPanelTransferCenter->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
			{
				iResult = m_plnkPanelTransferCenter->UpPickUpUDCyl();
				if (iResult) return iResult;

				iResult = m_plnkPanelTransferCenter->AbsorbPanel();
				m_plnkPanelTransferCenter->ReleasePanel();
				m_plnkPanelTransferCenter->DownPickUpUDCyl();

				// 흡착 성공인 경우, 실제로 흡착되지 않은 Panel 이 있다는 Message 표시 필요.
				if (SUCCESS == iResult) {
					// 424006 = 부상유닛에 Panel이 감지되지만 흡착상태가 아닙니다..
					return generateErrorCode(424006);
				}
			}
		}

		// Panel ID가 있을때
		if (m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_CENTER_PANEL]->m_strEPanelID.GetLength() > 1) {
			CString strMsg = 
//				_T("PanelTransferCenter 가 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?\r\nPanelTransferCenter has a Panel ID but released a vacuum. Scrap(delete) the Panel ID?\r\nPanel Scrap");
				_T("PanelTransfer Center 가 Vacuum Off 상태인데, Panel ID 가 존재합니다. Scrap을 진행할까요?\r\nPanelTransfer Center is Vacuum Off state. but there is Panel ID. Do you want Panel Scrap?");
			if (MyMessageBox(strMsg, _T("Question"), M_YESNO|M_ICONQUESTION) == IDYES)
				m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_CENTER_PANEL);	
		}

		iResult = m_plnkPanelTransferCenter->DownPickUpUDCyl();
		if (iResult) return iResult;

		// Panel Transfer에 Panel 없을때 ITI
	}
#endif

	m_bAutoInitialized  = TRUE;
	return ERR_TRS_PANEL_TRANSFER_CENTER_SUCCESS; 
}
//______________________________________________________________________________

void MTrsPanelTransferCenter::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsPanelInStop()
{
	return (m_bPanelInStop || /*m_plnkLCNet->m_eEqState == ePM ||*/ m_plnkTrsAutoManager->IsPanelInStopNSMC());
}
//______________________________________________________________________________

void MTrsPanelTransferCenter::SetPanelInStop(BOOL bSel)
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
//______________________________________________________________________________
void MTrsPanelTransferCenter::AssignITI(ITI_THandler* pITI_THandler, ITI_PanelTransfer* pITI_PanelTransfer)
{
	m_plnkITITHandler		= pITI_THandler;
	m_plnkITIPanelTransfer	= pITI_PanelTransfer;
}

//______________________________________________________________________________

void MTrsPanelTransferCenter::ThreadRun()
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

void MTrsPanelTransferCenter::ThreadStop()
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

void MTrsPanelTransferCenter::initializeITI()
{
	m_bITISendStartToPanelTransfer			= FALSE;
	m_bITISendCompleteToPanelTransfer		= FALSE;

	m_bITISendStartToTHandler				= FALSE;
	m_bITISendCompleteToTHandler			= FALSE;
	// NSMC
	m_bModelChangeReady					= FALSE;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsInitialized()
{
	return m_bAutoInitialized;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsSendStartToPanelTransfer()
{
	return m_bITISendStartToPanelTransfer;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsSendCompleteToPanelTransfer()
{
	return m_bITISendCompleteToPanelTransfer;
}
//SJ_YYK 161104 Add..
BOOL MTrsPanelTransferCenter::IsSendStartToTurnHandler()
{
	return m_bITISendStartToTHandler;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsSendCompleteToTurnHandler()
{
	return m_bITISendCompleteToTHandler;
}
//___________________________________*/

//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsEmptyPanel()
{
	BOOL bResult = m_plnkPanelTransferCenter->IsPanelReleased();
	return bResult;
}
//______________________________________________________________________________

int MTrsPanelTransferCenter::GetStep()
{
	return m_estepCurrent;
}
//______________________________________________________________________________

BOOL MTrsPanelTransferCenter::IsAutoRun()
{
	return m_bAutoRun;
}
//______________________________________________________________________________

CString MTrsPanelTransferCenter::GetItiStatus()
{
	m_strItiStatus.Format(_T("SendS: %d, SendC: %d"), 
		m_bITISendStartToPanelTransfer, m_bITISendCompleteToPanelTransfer);

	return m_strItiStatus;
}
//______________________________________________________________________________

int MTrsPanelTransferCenter::ScrapPanel()
{
	int iResult = SUCCESS;

	m_plnkTrsAutoManager->ManageGlassData(ePanelRemove, DEF_PANEL_TRANSFER_CENTER_PANEL);

	if (TRUE == m_plnkPanelTransferCenter->IsPanelAbsorbed())
	{
		CString strMsg = _T("There is a panel on Panel Transfer Center. Remove it.");
		
		do
		{
			iResult = m_plnkPanelTransferCenter->ReleasePanel();
			if (iResult)
				return iResult;
			
			Sleep(500);

			if(MyMessageBox(MY_MESSAGE_BOX, 62600, _T("Question Scrap"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) ==IDNO)
				return FALSE;

//			MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Remove Panel Request", M_ICONINFORMATION);
			MyMessageBox(MY_MESSAGE_BOX, 62200, "Remove Panel Request", M_ICONINFORMATION);

			m_plnkPanelTransferCenter->AbsorbPanel(TRUE);
			Sleep(300);

		} while (TRUE == m_plnkPanelTransferCenter->IsPanelAbsorbed());
	}
	
	m_plnkPanelTransferCenter->ReleasePanel(TRUE);

	return Initialize();
}
//______________________________________________________________________________

// NSMC
BOOL MTrsPanelTransferCenter::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsPanelTransferCenter::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

//______________________________________________________________________________

void MTrsPanelTransferCenter::initializeMelsecIF()
{
	m_plnkMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveStart, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eArm1FoldComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eReceiveComplete, FALSE);
	m_plnkMelsecIF->SetUpperEqBit(eImmediatelyPauseRequest, FALSE);
}
//______________________________________________________________________________

// End Of Code

