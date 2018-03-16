/* 
 * TrsTabFeeder Component
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
 * MTrsTabFeeder.cpp : Implementation of MTrsTabFeeder component.
 */

#include "Stdafx.h"
#include "MProductInfoData.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MProcessData.h"
#include "MTabFeeder.h"
#include "MCtrlTabFeeder.h"
#include "MTrsTabFeeder.h"
#include "MTrsAutoManager.h"
#include "ITI_TabCarrier.h"
#include "ITI_PanelAligner.h"
#include "CommonGetValueDlg.h"
#include "MPreBonderData.h"
// selsk_jdy
#include "common.h"

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

MTrsTabFeeder::MTrsTabFeeder(SCommonAttribute			commonData, 
							STrsTabFeederRefCompList	listRefComponents, 
							STrsTabFeederData			datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

	m_bAutoInitialized	= FALSE;

	m_bThreadLife		= FALSE;
	m_pThread			= NULL;
	m_bAutoRun			= FALSE;

	m_bRecoveryInit		= FALSE;

	m_iInstanceTabCarrier	= -1;//KKY

	m_bPanelAlignerUnloadingPanel = FALSE;//100930.KKY
	m_b1FeederAllMountInitReady = FALSE;
	//120116.KMS_________
	m_bMountAutoChangeReady[0] = FALSE;
	m_bMountAutoChangeReady[1] = FALSE;

	initializeITI();
	initializeStep();

	m_bCurrentPanelBlowExt = FALSE; //SJ_YYK 150317 Add..
}

MTrsTabFeeder::~MTrsTabFeeder()
{
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
void MTrsTabFeeder::simulLog(CString strMsg, int iStep, int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s : %s(%d)\n"), GetObjectName(), strMsg, iStep);

	FILE *fp = fopen( ".\\StepLog.txt", "a");
//	fprintf(fp, "%s\n", strLog);
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

/** Thread �Լ�*/
UINT MTrsTabFeeder::threadFunction(LPVOID pParam)
{
	MTrsTabFeeder*	pThis = (MTrsTabFeeder*) pParam;

	TRACE(_T("MTrsTabFeeder Thread Run\n"));

	while (pThis->m_bThreadLife)
	{
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsTabFeeder Thread Stop\n"));

	return ERR_TRS_TABFEEDER_SUCCESS;
}

void MTrsTabFeeder::threadJob()
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
			m_bITISendAbleToTC = FALSE;

			m_plnkTabFeeder->BrushMotorOff();

			m_plnkTabFeeder->MoldBlowOff();	//2009.10.27 CYJ

			//@130119.KKY_________
			m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			//@___________________

			m_bAutoRun = FALSE;
			break;

		case START_RUN:
			/* Ȯ�� �ʿ�
			if (m_plnkTabFeeder->IsAbsorbPress() && m_plnkTabFeeder->IsInPressPos(DEF_PRESS_DOWN_POS))
				setStep(TABFEEDER_PRESS_UP);
			else
				setStep(TABFEEDER_FEEDING_READY);
			*/

			m_bAutoRun = TRUE;
			break;

		case CYCLE_STOP:
			break;
		
		case RUN:
			//if (WORKING_SIDE_UNKNOWN == m_plnkProcessData->GetCurrentWorkingSide(m_iInstanceNo))
				//break;

			doRunStep();
			break;			
		}
		break;
	}
}

/** RUN Step�� �����Ѵ� */
void MTrsTabFeeder::doRunStep()
{
	int iResult = ERR_TRS_TABFEEDER_SUCCESS;
	int i = 0;
	return;

	/*/
#ifdef DEF_SOURCE_SYSTEM
	//120116.kms_________
	if(m_plnkTrsAutoManager->GetMountAutoChange() == FALSE)
	{
		if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)
		{
			if (FALSE == checkUseFeeder())
				return;
		}
		else
		{
			if (FALSE == m_plnkCtrlTabFeeder->GetFeederReady() && DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
			{
				//Test For "1Feeder 8Mounter Mode" in DryRunning
				//Feeder�غ� �ȵǾ� ������ StartSW or ResetSW�� 1���̻� ������ Feeder�غ� �Ǿ��ٴ� Flag�� TRUE��Ŵ.
				BOOL bSwitchPushed = (DEF_TABFEEDER1 == m_iInstanceNo) ? m_plnkTrsAutoManager->GetStartSWStatus() : m_plnkTrsAutoManager->GetResetSWStatus();
				if (TRUE == bSwitchPushed)
				{
					DWORD sTime = GetTickCount();
					while(1)
					{
						Sleep(10);
						bSwitchPushed = (DEF_TABFEEDER1 == m_iInstanceNo) ? m_plnkTrsAutoManager->GetStartSWStatus() : m_plnkTrsAutoManager->GetResetSWStatus();
						if (FALSE == bSwitchPushed)
							break;
						if (GetTickCount() - sTime > 1000)
						{						
							m_plnkCtrlTabFeeder->SetFeederReady4DryRun(TRUE);
							Sleep(5000);
							break;
						}
					}
				}
			}
		}
	}
	//_______________
#else
	/*/
	if (FALSE == isUseWorkGroup())
		return;
//@#endif

	switch (m_estepCurrent)
	{
	case TABFEEDER_REEL_AUTO_CHANGE_READY: simulLog("TABFEEDER_REEL_AUTO_CHANGE_READY", m_estepCurrent, 100);
		
		m_plnkTabFeeder->WriteAutoChangeLog("AutoChange Start", 0);

		// ��� �Ϸ�� Reel ������ �����ϰ�, ��� ���̴� Reel ������ ����� Reel ������ �̵���Ų��.
		// AutoChange ���� Error �߻��� ��� TabIC Reel ������ �Ѿ�� �ʴ� ���� ������,
		// ���� Data �� �ѱ�� AutoChange �� �����Ѵ�.
		// ��� ���� ��� ���̴� Reel �� ���� �� ���� ���̹Ƿ�, Data �� ���� �ѱ��.
		m_plnkCtrlTabFeeder->ChangeSpareReelInfotoUsingReel();

		iResult = m_plnkCtrlTabFeeder->ExecuteReelAutoChange();
		if (iResult)
		{
			// 20081007 jdy
			// hongju_TJ : Find TabIC�� �ϸ� �ɰ� ����...Homing�� ���� ���� �ʿ� ����.
//			m_plnkCtrlTabFeeder->SetHomeFlagSprocket2(FALSE);
			m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);
			PROCESS_ALARM(iResult);

			m_plnkTabFeeder->WriteAutoChangeLog("AutoChange Error", iResult);
			break;
		}

		setStep(TABFEEDER_REEL_AUTO_CHANGE);
		break;

	case TABFEEDER_REEL_AUTO_CHANGE: simulLog("TABFEEDER_REEL_AUTO_CHANGE", m_estepCurrent, 100);

		{
			CString strMsg;
			strMsg.Format(_T("Feeder%d "), m_iInstanceNo + 1);
			// jdy sesl 081230	CString szTextTemp;
			CString strTextTemp;

			//Sprocket2 AutoHoming ���� ���Դϴ�.
			// jdy sesl 081230	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65500"), &szTextTemp);
			ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65500"), &strTextTemp);

			/* jdy sesl 081230
			strMsg += szTextTemp;
			WCHAR szMsg[512];
			wcscpy(szMsg, strMsg.GetBuffer(strMsg.GetLength()));
			::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MY_MESSAGE, TRUE, (LPARAM)szMsg);
			*/
			strMsg += strTextTemp;
			::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MY_MESSAGE, TRUE, (LPARAM)(strMsg.GetBuffer(strMsg.GetLength())));
		}

		iResult = m_plnkCtrlTabFeeder->ExecuteAutoHoming(TRUE);

		::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MY_MESSAGE, FALSE, NULL);

		if (iResult)
		{
			// 20081007 jdy
			m_plnkCtrlTabFeeder->SetHomeFlagSprocket2(FALSE);
			PROCESS_ALARM(iResult);
			m_plnkTabFeeder->WriteAutoChangeLog("AutoChange Error", iResult);
			break;
		}
		else
		{
			//TabIC �ڵ���ü �Ϸ�.
			CString strMsg;
			ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65501"), &strMsg);
			m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg);
		}
		
		m_plnkTabFeeder->WriteAutoChangeLog("AutoChange End", 0);

		setStep(TABFEEDER_REEL_AUTO_CHANGE_END);
		break;

	case TABFEEDER_REEL_AUTO_CHANGE_END: simulLog("TABFEEDER_REEL_AUTO_CHANGE_END", m_estepCurrent, 100);

		setStep(TABFEEDER_FEEDING_READY);
		break;

    case TABFEEDER_FEEDING_READY: simulLog("TABFEEDER_FEEDING_READY", m_estepCurrent, 100); 

        // ���� ���� Mode ����.
        if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE)
		{
			//NSMC KJS
			if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			{
				m_bModelChangeReady = FALSE;
				setStep(TABFEEDER_MODEL_CHANGE);
				break;
			}
            break;
		}

		if (TRUE == m_plnkSystemData->m_bUseMaterialPrealarm)
		{
			if (TRUE == m_plnkCtrlTabFeeder->m_bConsumeStart[DEF_SPARE_REEL])
			{
				// When TabIC Reel Data is not docked in.
				::PostMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MATERIAL_EXHAUST_PRE_ALARM_REQ, m_iInstanceNo, NULL);
			}
			else
			{
				// �ڵ� ���� ��, New Reel Vacuum Sensor ������ ���� ŭ.
				// ���������� Vacuum �� �������� ��� ������ �� �����Ƿ�, �ܼ��� Vacuum ���¸����� New Reel ���� ���� Ȯ���� ����.
				// ��, AutoChange ������ Tab Press �۾��� �����Ƿ� ���� Vacuum ���� �����ϰ� �����ǹǷ� ���� ����.
				if (FALSE == m_plnkTabFeeder->IsAbsorbNewReel())
					m_plnkTabFeeder->AbsorbNewReel(TRUE);
			}
		}

        // Park, YK - For Online
        m_plnkCtrlTabFeeder->InitAssembleCount();

		if (TRUE == m_plnkSystemData->m_bUseMoldBlow
			&& 0 == m_plnkProductInfoData->m_iProductCount % m_plnkSystemData->m_uiMoldBlowPeriod
			&& m_bCurrentPanelBlowExt == FALSE) //SJ_YYK 150317 Modify..
		{
			m_bCurrentPanelBlowExt = TRUE; //SJ_YYK 150317 Add...
			m_plnkTabFeeder->MoldBlowOn();
			Sleep(m_plnkSystemData->m_uiMoldBlowTime * 1000);
			m_plnkTabFeeder->MoldBlowOff();
		}

		 //SJ_YYK 150317 Modify..		
		if(0 != m_plnkProductInfoData->m_iProductCount % m_plnkSystemData->m_uiMoldBlowPeriod
			&& m_bCurrentPanelBlowExt == TRUE)
		{
			m_bCurrentPanelBlowExt = FALSE;
		}
		//_____________________________*/

		// 080423 ������ �߰�
		// L/C �� ������������� �����ϱ� ���� (�ӽ��ڵ�)
		// selsk_jdy : ������ ��� �߰� �ڵ�
		// 090128 jdy : ���� ������ �����ؼ� ����.
		if (NORMAL_RUN_MODE == m_plnkSystemData->m_eRunMode && TRUE == m_plnkSystemData->m_bUseMaterialDeviceID)
		{
			// 20080416 by kss	: selsk_jdy
			// Error code �߰� �ʿ���.
			if (m_plnkLCNet->m_bConnected)
			{
				if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE != eMatStateEnable
					&& m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE != eMatStateRun)
				{
					// 415002 = ���� TabIC�� Ÿ���� �� �� ���� �����Դϴ�. Reel�� �����ϰ� �� Reel�� �������ּ���
					PROCESS_ALARM(generateErrorCode(415002));   
					break;
				}
				
				// selsk_jdy : ������ D ��û ����.
				// BatchID�� �ִ��� �켱 Ȯ��, BatchID��  Null�̸� .GetLength()���� Access Violation �߻�
				if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_sM_BATCHID)
				{
					if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_sM_BATCHID.GetLength() < 3)
					{
						// 415006 = ���� TabIC�� Ÿ���� �� �� ���� �����Դϴ�. (���� ID ����)
						PROCESS_ALARM(generateErrorCode(415006));   
						break;
					}
				}
				else // BatchID�� ��� ����
				{
					// 415006 = ���� TabIC�� Ÿ���� �� �� ���� �����Դϴ�. (���� ID ����)
					PROCESS_ALARM(generateErrorCode(415006));   
					break;
				}
				
 //2009.06.19 CYJ ������ ���� üũ��.
				if(m_plnkSystemData->m_bUseMoldValidation == TRUE)
				{
					// 20090504 kjw SELSK������ ���� üũ ����(�������� ������->������å�� Ȯ��)
					if (m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE != eMatStateEnable
						&& m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE != eMatStateRun)
					{
						// 415003 = ���� ������ Ÿ���� �� �� ���� �����Դϴ�. ������ �����ϰ� �� ������ �������ּ���
						PROCESS_ALARM(generateErrorCode(415003));   
						break;
					}

					// BatchID �� �ִ��� �켱 Ȯ��, BatchID�� Null �̸� .GetLength()���� Access Violation �߻�
					if (m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_BATCHID)
					{
						if (m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_BATCHID.GetLength() < 3)
						{
							// 415007 = ���� ������ Ÿ���� �� �� ���� �����Դϴ�. (���� ID ����)
							PROCESS_ALARM(generateErrorCode(415007));   
							break;
						}
					}
					else // BatchID�� ��� ����
					{
						// 415007 = ���� ������ Ÿ���� �� �� ���� �����Դϴ�. (���� ID ����)
						PROCESS_ALARM(generateErrorCode(415007));   
						break;
					}
				}
			}
		}

        setStep(TABFEEDER_FEEDING);
        break;
		
	case TABFEEDER_FEEDING: simulLog("TABFEEDER_FEEDING", m_estepCurrent, 100);
//		if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
//		{
//			setStep(TABFEEDER_PRESS_DOWN);
//			Sleep(500);
//			break;
//		}

		// �̹� Pressing ������ �Ϸ�Ǿ����� Ȯ��
		if (TRUE == m_plnkTabFeeder->IsAbsorbPress())
		{
			setStep(TABFEEDER_PRESS_DOWN);
			break;
		}

		if(TRUE)
		{
			DWORD stime = GetTickCount();

			iResult = m_plnkCtrlTabFeeder->FeedTabIC();
			
			TRACE(_T("[%d] Feeder Feeding Elaspe Time = %d\n"), m_iInstanceNo, int(GetTickCount() - stime));
		}
		if (iResult)
		{
			/*/
			if (315001 == (iResult/1000)) // �ڵ���ü�ؾ� �� �ð�...
			{
				//8Mount��� �ƴҰ�� �˶��߻�
				if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)
				{
					//120116.kms_______________
					/ *
					if (m_plnkTabFeeder->IsAbsorbNewReel())
						setStep(TABFEEDER_REEL_AUTO_CHANGE_READY);
					else
					{
						SetErrorLevel(_T("TAB FEEDER feeding"), 0, __FILE__, __LINE__);						
						PROCESS_ALARM(iResult);
					}
					* /
					if(TRUE == m_plnkSystemData->m_bUse4MountAll)
					{
						if(m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_ALL)
						{
							if (FALSE == m_plnkCtrlOtherTabFeeder->GetFeederReady())
							{
								SetErrorLevel(_T("TAB FEEDER feeding"), 0, __FILE__, __LINE__);
								PROCESS_ALARM(iResult);
							}
							else
							{
								CString strMsg;
								if (m_iInstanceNo == DEF_TABFEEDER1)
								{
									m_plnkTrsAutoManager->SetMountAutoChange(TRUE);
									ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65503"), &strMsg);
								}
								else
								{
									m_plnkTrsAutoManager->SetMountAutoChange(TRUE);
									ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65504"), &strMsg);
								}

								m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg);
								
								//120116.KMS_________
								m_bMountAutoChangeReady[0] = FALSE;
								m_bMountAutoChangeReady[1] = FALSE;

							DWORD stime = GetTickCount();
								while(1)
								{
									Sleep(10);
									if (TRUE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
										break;
									if (GetTickCount() - stime > 10000)
									{
										break;//�˶� �ڵ� �߰���
									}
								}

								setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
								if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
									Sleep(5000);
								else
									Sleep(5000);
							}
						}
						else
						{
							SetErrorLevel(_T("TAB FEEDER feeding"), 0, __FILE__, __LINE__);						
							PROCESS_ALARM(iResult);
						}
					}
					else
					{
						SetErrorLevel(_T("TAB FEEDER feeding"), 0, __FILE__, __LINE__);						
						PROCESS_ALARM(iResult);
					}
					//_________________________
				}
				//8Mount����� ��� �Ѵ� ������ �˶�...�ƴϸ� OpCall
				else				
				{
					//if (FALSE == m_plnkCtrlOtherTabFeeder->GetHomeFlagSprocket2() || FALSE == m_plnkCtrlOtherTabFeeder->GetFindTabICFlag())
					if (FALSE == m_plnkCtrlOtherTabFeeder->GetFeederReady())
					{
						SetErrorLevel(_T("TAB FEEDER feeding"), 0, __FILE__, __LINE__);
						PROCESS_ALARM(iResult);
					}
					else
					{
						CString strMsg;
						if (m_iInstanceNo == DEF_TABFEEDER1)
							ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65503"), &strMsg);
						else
							ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65504"), &strMsg);
						m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg);
						
						m_b1FeederAllMountInitReady = FALSE;
						DWORD stime = GetTickCount();
						while(1)
						{
							Sleep(10);
							if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
								break;
							if (GetTickCount() - stime > 10000)
							{
								break;//�˶� �ڵ� �߰���
							}
						}
						setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
						if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
							Sleep(5000);
					}
				}
			}
			else
			{
				SetErrorLevel(_T("TAB FEEDER feeding"), 0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
			}
			/*/
			PROCESS_ALARM(iResult);
			break;
		}

		setStep(TABFEEDER_PRESS_DOWN); 
		break;
		
	case TABFEEDER_PRESS_DOWN: simulLog("TABFEEDER_PRESS_DOWN", m_estepCurrent, 100);
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABFEEDER_MODEL_CHANGE);
			break;
		}

		//120116.kms__________
		if (TRUE == m_plnkTrsAutoManager->GetMountAutoChange())
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;

			setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
			break;
		}
		//____________________

		if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
			break;
		}

		if (!m_plnkITIPanelAligner->IsPanelAlignerUnloadingPanel())
			m_bRecoveryInit = FALSE;

		// TabFeeder ��� ������.
		if (!m_bRecoveryInit	// �� cycle �� �ѹ��� ���� ��Ų��.
			//100930.KKY_____
			//&& m_plnkITIPanelAligner->IsPanelAlignerUnloadingPanel())
			&& m_bPanelAlignerUnloadingPanel)
			//_______________
		{
			m_bPanelAlignerUnloadingPanel = FALSE;//100930.KKY

			// Recovery ���� ȸ��
			m_bRecoveryInit	= TRUE;
			
			// ���� ���¸� Run���� ����
			if (m_plnkLCNet->m_bConnected)
			{
				if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE != eMatStateRun)
				{
					m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE = eMatStateRun;
				}
				
				if (m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE != eMatStateRun)
				{
					m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE = eMatStateRun;
				}
			}
			
			// TabIC Event ó��
			m_plnkProductInfoData->m_matTabICInfo[GetInstanceNo()][DEF_USING_REEL] 
				= m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL];

			// Gate ������ ��쵵 Feeder1 ������ File Save Operation �����Ѵ�.
			// - Feeder1 ������ �����ص� ��� Tab ���� ���̰� ũ�� ���� �ʴ´�. (Panel �� �� �з�.)
			// - WriteMaterialData() �� ����ȭ ó���� ���� �ʾ�, TrsTabFeeder 2 ������ ���ÿ� �����ϸ�
			//   ��� ���� �𸥴�. (���α׷� ������ ���� ���� ���� - ?)
			if (DEF_TABFEEDER1 == m_iInstanceNo)
				m_plnkProductInfoData->WriteMaterialData();
			
			// selsk_jdy : �߰� Ȯ�� �ʿ� - �Ʒ� �ڵ� ���� �߻� ���� ����.
			//             Ÿ�̹� �� �幰�� �ϰ�����, PanelAligner �� Unload Conveyor �� Panel �ѱ�� ���� �� �κ� ����Ǹ�
			//             Panel ID ���� �� ����.
			m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_sEQP_STATE
				= m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_ALIGNER_PANEL]->m_strEPanelID;
			//100930.KKY_____
			if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_sEQP_STATE == _T(""))
			{
				m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_sEQP_STATE
					= m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_OUT_PANEL]->m_strEPanelID;
			}
			//_______________

			if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE
				|| m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
			{
				m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nA_QTY = getAssembleQty();
				int iNGCount = m_plnkCtrlTabFeeder->m_iSprocketFeedingCount
					- m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nA_QTY;
				if (iNGCount > 0)
					m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nN_QTY += iNGCount;
				m_plnkCtrlTabFeeder->m_iSprocketFeedingCount = 0;
				m_plnkLCNet->MaterialReport(eMatAssemble, &m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL]);

				
				//eMatNG 12.08.14 Assemble ���� TabFeeder �������� ����
				int iTemp = (m_plnkCtrlTabFeeder->m_iNG_CurTab - m_plnkCtrlTabFeeder->m_iNG_OldTab) - m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nA_QTY;
				m_plnkCtrlTabFeeder->m_iNG_Count += iTemp;
				m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nN_QTY = m_plnkCtrlTabFeeder->m_iNG_Count;
				if(m_plnkCtrlTabFeeder->m_iNG_Count > 0){
					m_plnkLCNet->MaterialReport(eMatNG, &m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL]);
					m_plnkCtrlTabFeeder->m_iNG_Count = 0;
				}
				m_plnkCtrlTabFeeder->m_iNG_OldTab = 0;
				m_plnkCtrlTabFeeder->m_iNG_CurTab = 0;
				
				/* selsk_jdy �ڵ� ����.
			}
			
			// Zig Event ó��
			if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE
				|| m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
			{
			*/
				// selsk_jdy
				// Zig Event ó��
				m_plnkCtrlTabFeeder->m_matZigInfo.m_nA_QTY = getAssembleQty();
				m_plnkCtrlTabFeeder->m_matZigInfo.m_sEQP_STATE
					= m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_ALIGNER_PANEL]->m_strEPanelID;
				//100930.KKY_____
				if (m_plnkCtrlTabFeeder->m_matZigInfo.m_sEQP_STATE == _T(""))
				{
					m_plnkCtrlTabFeeder->m_matZigInfo.m_sEQP_STATE
						= m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_OUT_PANEL]->m_strEPanelID;
				}
				//_______________

				m_plnkCtrlTabFeeder->m_matZigInfo.m_nU_QTY += m_plnkCtrlTabFeeder->GetMoldCount();
				m_plnkCtrlTabFeeder->m_iZigUsedQtyEquip += m_plnkCtrlTabFeeder->GetMoldCount();

				m_plnkCtrlTabFeeder->m_matZigInfo.m_nMAP_STIF = m_plnkCtrlTabFeeder->m_matZigInfo.m_nMAP_STIF - m_plnkCtrlTabFeeder->m_matZigInfo.m_nA_QTY;
				m_plnkCtrlTabFeeder->m_matZigInfo.m_nCUR_STIF = m_plnkCtrlTabFeeder->m_matZigInfo.m_nA_QTY;

				
				CString strTotal;
				strTotal.Format(_T("%d"),m_plnkCtrlTabFeeder->m_iZigUsedQtyEquip);
				m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORT_TYPE = strTotal;
				
				m_plnkCtrlTabFeeder->SetMoldCount(0);
				m_plnkLCNet->MaterialReport(eMatAssemble, &m_plnkCtrlTabFeeder->m_matZigInfo);
				
				if (0 != m_plnkSystemData->m_uiMoldMaxCount
					&& 0 == m_plnkProductInfoData->m_iProductCount % m_plnkSystemData->m_uiMoldMaxCount)
				{
					/* selsk_jdy : SELSK ���� ������ ���ؼ� ������ ���� �����ߴ��� ������ �������� ������ ���� ��õ�.
					               ��, ���� ���� �� OP Call ��ſ� Title Bar Alarm Message �̿�.
					if (m_plnkCtrlTabFeeder->GetZigRFIDState())
					{
						iResult = m_plnkCtrlTabFeeder->WriteZigInfo();
						if (iResult != SUCCESS)
						{
							CString strRFID = "���� RFID Data�� ���µ� �����Ͽ����ϴ�. ���� ���¸� Ȯ���� �ּ���";
							::SendMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_OPCALL_MSG, (WPARAM)(&strRFID), NULL);
							// �ڵ� Ȯ�� �ʿ� : ���� ���� �ʿ�.
							m_plnkTrsAutoManager->SetOperatorCall(TRUE, strRFID);
						}
					}
					*/
					// selsk_jdy
					//100825.KMS ���� �¶������� �������� Ÿ�߼� ���� �ϱ� ���� ���� ���� RFID�� �о ���� �� ����
/*					iResult = m_plnkCtrlTabFeeder->WriteZigInfo();
					if (iResult)
					{
						CString strAlarm;
						//���� Data �� ���µ� �����Ͽ����ϴ�.
						ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_65502"), &strAlarm);
						/* jdy sesl
						WCHAR szAlarm[256];
						memset(szAlarm, 0, sizeof(szAlarm));
						wcscpy(szAlarm, strAlarm.GetBuffer(strAlarm.GetLength()));
						::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_ALARM_MSG, NULL, (LPARAM)szAlarm);
	
						::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_ALARM_MSG, NULL, (LPARAM)(strAlarm.GetBuffer(strAlarm.GetLength())));
					}
*/
				}
			}

			//120515.KMS______________
			m_plnkTrsAutoManager->SetAssemReport(FALSE);
		}

		m_iInstanceTabCarrier = -1;

		//111229 Add_____________
		//Unloading��ġ���� Loading��ġ���̵��� �̸� Ÿ���ϵ���
		//SJ_YYK 150227 Modify...
		//if (m_plnkITITabCarrier[DEF_MAX_TABCARRIER-1]->IsTabICReceiveAbleToTFRequest())
		if (m_plnkITITabCarrier[0]->IsTabICReceiveAbleToTFRequest())
		{
			m_iInstanceTabCarrier = -2;
		}
		else
		{
			for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
			{
				if (m_plnkITITabCarrier[i]->IsTabICReceiveAbleToTF())
				{
					m_iInstanceTabCarrier = i;
					break;
				}
			}
		}
		//________________________

		//@TacIC ��Ÿ�� ��� ��������� ���� ����________
		if (FALSE == m_plnkSystemData->m_bUse4MountAll)
		{
			if (m_iInstanceTabCarrier == -1)
				break;
		}
		//_______________________________________________
		if(TRUE)
		{
			DWORD stime = GetTickCount();
			iResult = m_plnkCtrlTabFeeder->PressTapeStep1();
			if (iResult != ERR_CTRL_TABFEEDER_SUCCESS)
			{
				PROCESS_ALARM(iResult);			
				setStep(TABFEEDER_FEEDING_READY);
				break;
			}
			TRACE(_T("[%d] FEEDER PUNCH Elaspe Time = %d\n"), m_iInstanceNo, int(GetTickCount() - stime));
		}

		setStep(TABFEEDER_PRESS_UP); 
		break;
		
	case TABFEEDER_PRESS_UP: simulLog("TABFEEDER_PRESS_UP", m_estepCurrent, 100);
		//Carrier Loading��ġ�� �̵��� �̸� m_bTabICReceiveAbleToTF = TRUE��Ű�Ƿ�,
		//Press�ϰ����·� ����Ұ�� ���
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABFEEDER_MODEL_CHANGE);
			break;
		}
		
		/*/
#ifdef DEF_SOURCE_SYSTEM
		if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
		{
			if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
			{
				m_b1FeederAllMountInitReady = FALSE;
				setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
				break;
			}
		}

		//120116.kms__________
		if (TRUE == m_plnkTrsAutoManager->GetMountAutoChange())
		{
			m_bMountAutoChangeReady[0] = FALSE;
			m_bMountAutoChangeReady[1] = FALSE;
			setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
			break;
		}
		//____________________
#endif
		/*/

		//@TabIC ��Ÿ�� ��ɽ� TabCarrier�� RequestȮ��
		if (m_iInstanceTabCarrier == -1)
		{
			//SJ_YYK 150227 Modify..
			//if (m_plnkITITabCarrier[DEF_MAX_TABCARRIER-1]->IsTabICReceiveAbleToTFRequest())
			if (m_plnkITITabCarrier[0]->IsTabICReceiveAbleToTFRequest())
			{
				m_iInstanceTabCarrier = -2;
			}
			else
			{
				for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
				{
					if (m_plnkITITabCarrier[i]->IsTabICReceiveAbleToTF())
					{
						m_iInstanceTabCarrier = i;
						break;
					}
				}
			}
			if (m_iInstanceTabCarrier == -1)
				break;
		}

		//111229 Add_____________
		//Unloading��ġ���� Loading��ġ���̵��� �̸� Ÿ���ϵ���
		if (m_iInstanceTabCarrier == -2)
		{
			for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
			{
				if (m_plnkITITabCarrier[i]->IsTabICReceiveAbleToTF())
				{
					m_iInstanceTabCarrier = i;
					break;
				}
			}
			if (m_iInstanceTabCarrier == -2)
				break;
		}
		//_______________________


		//Carrier Loading��ġ�� �̵��� �̸� m_bTabICReceiveAbleToTF = TRUE��Ű�Ƿ�,
		//Press�ϰ����·� ����Ұ�� ���
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABFEEDER_MODEL_CHANGE);
			break;
		}

		if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode
			&& FALSE == m_plnkTabFeeder->IsAbsorbPress())
		{
			setStep(TABFEEDER_PRESS_END); 
			break;
		}

		m_bITISendAbleToTC = TRUE;

		//if (FALSE == m_plnkITITabCarrier->IsTabICReceiveStartToTF())
		if (FALSE == m_plnkITITabCarrier[m_iInstanceTabCarrier]->IsTabICReceiveStartToTF())
			break;

		m_bITISendStartToTC = TRUE;

		while (TRUE)
		{
			//if (TRUE == m_plnkITITabCarrier->IsTabICReceiveCompleteToTF())
			if (TRUE == m_plnkITITabCarrier[m_iInstanceTabCarrier]->IsTabICReceiveCompleteToTF())
				break;

//			if (FALSE == m_plnkITITabCarrier->IsTabICReceiveStartToTF())
			if (FALSE == m_plnkITITabCarrier[m_iInstanceTabCarrier]->IsTabICReceiveStartToTF())
			{
				m_bITISendAbleToTC = FALSE;
				m_bITISendStartToTC = FALSE;
				return;
			}

			/*/
			//120116.kms__________
#ifdef DEF_SOURCE_SYSTEM
			if (TRUE == m_plnkTrsAutoManager->GetMountAutoChange())
			{
				m_bMountAutoChangeReady[0] = FALSE;
				m_bMountAutoChangeReady[1] = FALSE;
				setStep(TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT);
				break;
			}
			//____________________
#endif
			/*/
			Sleep(10);
		}

		m_bITISendAbleToTC = FALSE;
		m_bITISendStartToTC = FALSE;

		setStep(TABFEEDER_PRESS_END); 
		break;

	case TABFEEDER_PRESS_END: simulLog("TABFEEDER_PRESS_END", m_estepCurrent, 100);
		
		iResult = m_plnkCtrlTabFeeder->PressTapeStep2();
		if (iResult != ERR_CTRL_TABFEEDER_SUCCESS)
		{
			PROCESS_ALARM(iResult)
			break;
		}	

		setStep(TABFEEDER_FEEDING_READY); 
		break;

		//NSMC KJS
	case TABFEEDER_MODEL_CHANGE: simulLog("TABFEEDER_MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change �ϷḦ TrsAutoManager ���� �뺸.
		m_bModelChangeReady = TRUE;
		
		// Ȯ�� �ʿ� : TrsAutoManager ���� ��ȣ�� �ϳ��� ����� ���ΰ�? �ΰ��� ����� ���ΰ�?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;
		
		m_bModelChangeReady = FALSE;

		if (PASS_RUN_MODE == m_plnkSystemData->m_eRunMode)
		{
			m_plnkTabFeeder->RecoveryIndMotorOff();

			m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);

			setStep(TABFEEDER_FEEDING_READY);
			break;
		}

		if (TRUE == m_plnkSystemData->m_bUseMaterialDeviceID)
		{
			if (TRUE == m_plnkTrsAutoManager->IsWaitingCheckMaterial())
				break;
		}
			
		if (TRUE == m_plnkSystemData->m_bUseMaterialDeviceID)
		{
			if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE == eMatStateEnable
				|| m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE == eMatStateRun)
			{
				setStep(TABFEEDER_FEEDING_READY);
			}
			/*
			//AutoChange ���� ���Ƿ� ���� ����
			else if (m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_SPARE_REEL].m_nM_STATE == eMatStateEnable
				|| m_plnkCtrlTabFeeder->m_matTabICInfo[DEF_SPARE_REEL].m_nM_STATE == eMatStateRun)
			{
				setStep(TABFEEDER_REEL_AUTO_CHANGE_READY);
			}
			*/
			else
			{
				setStep(TABFEEDER_FEEDING_READY);
			}
		}
		else 
		{
			if (TRUE == m_plnkTrsAutoManager->GetNSMCTabICChange())
			{
				setStep(TABFEEDER_REEL_AUTO_CHANGE_READY);
			}
			else
			{
				setStep(TABFEEDER_FEEDING_READY);
			}
		}

		break;

	case TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT:

		//120116.KMS________
		if (m_iInstanceNo == DEF_TABFEEDER1)
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

		m_b1FeederAllMountInitReady = FALSE;
		if (TRUE == m_plnkCtrlTabFeeder->GetFeederReady())
			setStep(TABFEEDER_FEEDING_READY);
		else
			setStep(TABFEEDER_WAIT_FEEDER_READY);
		break;

	case TABFEEDER_WAIT_FEEDER_READY:
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(TABFEEDER_MODEL_CHANGE);
			break;
		}

		if (TRUE == m_plnkCtrlTabFeeder->GetFeederReady())
			setStep(TABFEEDER_FEEDING_READY);
		break;
	default	:
		break;
	}
}

/** Thread�� Interface �ʱ�ȭ */
void MTrsTabFeeder::initializeITI()
{	
	m_bITISendAbleToTC	= FALSE;
	m_bITISendStartToTC	= FALSE;
}

void MTrsTabFeeder::initializeStep()
{
	// �ڵ����� STEP �ʱ�ȭ
	m_estepPrevious	= (enumStepTabFeeder)TABFEEDER_FEEDING_READY;	//  ���� Step Clear
	m_estepCurrent	= (enumStepTabFeeder)TABFEEDER_FEEDING_READY;	//  ���� Step Clear
}

/** Step Thread Step */
void MTrsTabFeeder::setStep(EStepTabFeeder estepTabFeeder)
{
	m_estepPrevious	= m_estepCurrent;	// ���� Step ����
	m_estepCurrent	= estepTabFeeder;	// ���� Step���� ��ȯ
}

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

/**
* Assign component linkage
* 
* @param	listRefComponents	: MTrsTabFeeder ���� Component list
*/
int MTrsTabFeeder::AssignComponents(STrsTabFeederRefCompList listRefComponents)
{
	m_plnkTrsAutoManager		= listRefComponents.m_plnkTrsAutoManager;
	m_plnkCtrlTabFeeder			= listRefComponents.m_plnkCtrlTabFeeder;
	m_plnkCtrlOtherTabFeeder	= listRefComponents.m_plnkCtrlOtherTabFeeder;
	m_plnkTabFeeder				= listRefComponents.m_plnkTabFeeder;
	m_plnkLCNet					= listRefComponents.m_plnkLCNet;
	return ERR_TRS_TABFEEDER_SUCCESS;
}

/** 
* Component Data ó�� 
*
* @param	pData : SMMTrsTabFeederData ����ü�� ���� ������
* @return	Error : 0 = Success, �׿�=��Ÿ
*/
int MTrsTabFeeder::SetData(STrsTabFeederData datComponent)
{
	m_plnkProcessData		= datComponent.m_plnkProcessData;
	m_plnkPanelData			= datComponent.m_plnkPanelData;
	m_plnkSystemData		= datComponent.m_plnkSystemData;
	m_plnkPreBonderData		= datComponent.m_plnkPreBonderData;
	m_plnkProductInfoData	= datComponent.m_plnkProductInfoData;
	//eMatNG 12.08.14 yh
	m_plnkWorkSchedule		= datComponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkScheduleSecond = datComponent.m_plnkWorkScheduleSecond;
#endif

	return ERR_TRS_TABFEEDER_SUCCESS;
}

/** 
 * TrsTabFeeder Component�� �ڵ� ���� �ʱ�ȭ�� �����Ѵ�.
 * @return	Error Code : 0 = SUCCESS, �� �� = Error
 */
int MTrsTabFeeder::Initialize()
{
	int iResult = ERR_TRS_TABFEEDER_SUCCESS;

	initializeStep();	// ��� Step�� �ʱ� Step���� �����Ѵ�.
	initializeITI();
	m_bAutoInitialized  = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)	//8Mount��� �ƴҰ��
	{
		if (FALSE == checkUseFeeder())	//�̻���� Feeder�� ��� ������ �ʱ�ȭ �Ϸ� ����
		{
			m_bAutoInitialized  = TRUE;
			m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			m_plnkTabFeeder->RecoveryIndMotorOff();
			m_plnkTabFeeder->PReelMotorOff();
			return ERR_TRS_TABFEEDER_SUCCESS; 
		}
	}
#else
	/*/
	m_bAutoInitialized = TRUE;
	return ERR_TRS_TABFEEDER_SUCCESS; 
	if (FALSE == isUseWorkGroup())
	{
		m_bAutoInitialized = TRUE;
		return ERR_TRS_TABFEEDER_SUCCESS; 
	}
//@#endif

	m_plnkTabFeeder->MoldBlowOff();

#ifndef SIMULATION
	// ���� ���� ���� �� �ڵ����� ���� �� ��ġ �ȸ´� ���� ����. �۾� ��ġ�� �����ش�.
	if (FALSE == m_plnkTabFeeder->IsInSupplierPos(DEF_SUPPLIER_WORK_POS))
	{
		iResult = m_plnkTabFeeder->SafeMoveSupplierPos(DEF_SUPPLIER_WORK_POS);
		if (iResult)
		{
			SetErrorLevel(_T("TAB FEEDER initialize"), 0, __FILE__, __LINE__);
			return iResult;
		}
	}

	//80507.KKY_____
	m_plnkTabFeeder->LockSupplierChuckCyl(TRUE);
	Sleep(500);
	//______________

	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode && FALSE == m_plnkCtrlTabFeeder->GetFeederReady())
	{
		iResult = m_plnkTabFeeder->PReelMotorOff();
		if (iResult)
		{
			SetErrorLevel(_T("TAB FEEDER initialize"), 0, __FILE__, __LINE__);
			return iResult;
		}

		setStep(TABFEEDER_WAIT_FEEDER_READY);
		m_bAutoInitialized  = TRUE;
		return ERR_TRS_TABFEEDER_SUCCESS; 
	}
#endif
	/*/

	if (m_plnkSystemData->m_eRunMode == NORMAL_RUN_MODE
		|| m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
	{
		m_plnkTabFeeder->ServoOnOffFeederReel(TRUE);
		m_plnkTabFeeder->RecoveryIndMotorOn();

		iResult = m_plnkCtrlTabFeeder->InitializeTabFeeder();
		if (iResult)
		{
//@#ifdef DEF_SOURCE_SYSTEM
//@			if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
//@				setStep(TABFEEDER_WAIT_FEEDER_READY);
//@#endif
			SetErrorLevel(_T("TAB FEEDER initialize"), 0, __FILE__, __LINE__);
			return iResult;
		}
	}
	// jdy sesl 081231
	else
	{

		// hongju_TJ : Pass Run �϶��� �Ѵ� ���� �� ��Ŵ..
		iResult = m_plnkTabFeeder->RecoveryIndMotorOff();
		if (iResult)
		{
			SetErrorLevel(_T("TAB FEEDER initialize"), 0, __FILE__, __LINE__);
			return iResult;
		}
		iResult = m_plnkTabFeeder->PReelMotorOff();
		if (iResult)
		{
			SetErrorLevel(_T("TAB FEEDER initialize"), 0, __FILE__, __LINE__);
			return iResult;
		}
	}
#endif /* SIMULATION */

	// SJ_YYK 110415 Test Add....
	/*
	if(m_plnkTabFeeder->IsAbsorbPress())
	{
		setStep(TABFEEDER_PRESS_UP);		
	}
	//_____________________________*/
	
	m_b1FeederAllMountInitReady = FALSE;

	//120116.KMS_________
	m_bMountAutoChangeReady[0] = FALSE;
	m_bMountAutoChangeReady[1] = FALSE;

	m_bAutoInitialized  = TRUE;
	return ERR_TRS_TABFEEDER_SUCCESS; 
}

/**
 * MTrsTabFeeder Initial �Ϸ� ���θ� �����Ѵ�.
 *
 * @return void
 */
void MTrsTabFeeder::ResetInitialized(void)
{
	m_bAutoInitialized = FALSE;
}

//110322.Add_____
void MTrsTabFeeder::SetInitializedOK()
{
	m_bAutoInitialized = TRUE;
}
//_______________

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/

//void MTrsTabFeeder::AssignITI(ITI_TabCarrier* pITI_TabCarrier, ITI_PanelAligner* pITI_PanelAligner)
void MTrsTabFeeder::AssignITI(tagSTabFeederRefITIList sITIList)
{
	//m_plnkITITabCarrier		= pITI_TabCarrier;
	//m_plnkITIPanelAligner	= pITI_PanelAligner;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		m_plnkITITabCarrier[i] = sITIList.m_plnkITITabCarrier[i];
	m_plnkITIPanelAligner	= sITIList.m_plnkITIPanelAligner;
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/

void MTrsTabFeeder::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_bThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsTabFeeder::ThreadStop()
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
/* ITI���� �Լ�
/*
/************************************************************************************************/

BOOL MTrsTabFeeder::IsInitialized(void) 
{
	return m_bAutoInitialized;
}

BOOL MTrsTabFeeder::IsSendAbleToTC()	
{ 
	return m_bITISendAbleToTC;	
}

BOOL MTrsTabFeeder::IsSendStartToTC()	
{ 
	return m_bITISendStartToTC;	
}

int MTrsTabFeeder::getAssembleQty()
{
	/*/
	//eMatNG 12.08.14 TabFeeder �Ѱ��� ����� ��� ���� �׷� �Ҵ� 
#ifdef DEF_SOURCE_SYSTEM
	int iForcedGroup = 2;
	if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if (m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_ALL)
		{
			iForcedGroup = m_plnkSystemData->m_nUseFeeder;
		}
	}
#endif 
	/*/

	//eMatNG 12.08.14 yh 
	int iAssembleCount = 0; 

	MVarList<MTabInfo>* ptmplistmount = NULL;
	MListWorkTab tmpListWorkTab;
	
	int ncnt = 0;
	int iWorkGroup;
	int i = 0;

	m_plnkWorkSchedule->GetOrgWorkList(&tmpListWorkTab);
	ncnt = tmpListWorkTab.GetCount();
	for (int i = 0; i < ncnt; i++)
	{
		ptmplistmount = tmpListWorkTab.GetSubList(i);
		iWorkGroup = ptmplistmount->GetWorkGroup();
		int listcnt = ptmplistmount->GetCount();

		/*/
#ifdef DEF_SOURCE_SYSTEM
		//eMatNG 12.08.14 TabFeeder �Ѱ��� ����� ��� ���� �׷� �Ҵ�
		if (iForcedGroup != 2)
		{
			iWorkGroup = iForcedGroup;
		}
#endif
		/*/

		if (iWorkGroup == m_iInstanceNo)
			iAssembleCount += listcnt;
	}

#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkScheduleSecond->GetOrgWorkList(&tmpListWorkTab);
	ncnt = tmpListWorkTab.GetCount();
	for (int i = 0; i < ncnt; i++)
	{
		ptmplistmount = tmpListWorkTab.GetSubList(i);
		iWorkGroup = ptmplistmount->GetWorkGroup();
		int listcnt = ptmplistmount->GetCount();
		if (iWorkGroup == m_iInstanceNo)
			iAssembleCount += listcnt;
	}	
#endif

	return iAssembleCount;

//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
//		return m_plnkCtrlTabFeeder->m_iXAssemble;

//	return m_plnkCtrlTabFeeder->m_iYAssemble;
}

int	MTrsTabFeeder::GetStep()
{
	return m_estepCurrent;
}

BOOL MTrsTabFeeder::IsAutoRun()
{
	return m_bAutoRun;
}

// NSMC
BOOL MTrsTabFeeder::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsTabFeeder::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

//100930.KKY_____
void MTrsTabFeeder::SetPanelAlignerUnloadingpanel()
{
	m_bPanelAlignerUnloadingPanel = FALSE;
	/*/
	if (m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_1)
	{
		if(m_iInstanceNo == 1 )
			return;
	}
	else if (m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_2)
	{
		if(m_iInstanceNo == 0 )
			return;
	}
	/*/
	if (FALSE == m_plnkWorkSchedule->GetUseWorkSide())
		return;
	m_bPanelAlignerUnloadingPanel = TRUE;
}
//_______________

BOOL MTrsTabFeeder::isUseWorkGroup()
{
	BOOL bUse = FALSE;
	if (m_iInstanceNo == 0)
	{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
		bUse = m_plnkPanelData->m_bUseFlag_Yl;
	#else
		bUse = m_plnkPanelData->m_bUseFlag_Yr;
	#endif
#else
		bUse = m_plnkPanelData->m_bUseFlag_Xu;
#endif
	}
	else
	{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
		bUse = m_plnkPanelData->m_bUseFlag_Yr;
	#else
		bUse = m_plnkPanelData->m_bUseFlag_Yl;
	#endif
#else
		bUse = m_plnkPanelData->m_bUseFlag_Xd;
#endif
	}
	return bUse;
}

BOOL MTrsTabFeeder::checkUseFeeder()
{
//@	if (m_plnkSystemData->m_nUseFeeder == UINT(m_iInstanceNo) || m_plnkSystemData->m_nUseFeeder == DEF_TABFEEDER_ALL)
	if (TRUE == m_plnkWorkSchedule->GetUseWorkSide())
		return TRUE;
	else
		return FALSE;
}

//120116.kms__________
BOOL MTrsTabFeeder::IsMountAutoChangeReady1()
{
	return m_bMountAutoChangeReady[0];
}

BOOL MTrsTabFeeder::IsMountAutoChangeReady2()
{
	return m_bMountAutoChangeReady[1];
}

// End Of Code
