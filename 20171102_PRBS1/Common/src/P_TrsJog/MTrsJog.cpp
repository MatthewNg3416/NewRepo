/* 
 * MTrsJog Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MTrsJog.cpp : Implementation of MTrsJog component.
 */

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MManageOpPanel.h"
#include "MTrsJog.h"
#include "MTrsAutoManager.h"
#include "MPlatformOlbSystem.h"
#include "MLCNet.h"

#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern	MPlatformOlbSystem	MOlbSystemPre;

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

MTrsJog::MTrsJog(SCommonAttribute commonData, MManageOpPanel* pMngOpPanel)
: ICommon(commonData)
{
	m_plnkMngOpPanel	= pMngOpPanel;
	m_flgThreadLife		= FALSE;
	m_pThread			= NULL;
	m_plnkTowerLampData	= MOlbSystemPre.GetTowerLampData();
}

MTrsJog::~MTrsJog()
{
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

void MTrsJog::ThreadRun()
{
	if (m_flgThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}
	m_flgThreadLife = TRUE;	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsJog::ThreadStop()
{
	m_flgThreadLife = FALSE;

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
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	
/**
 * Auto Manager를 Assign 한다.
 *
 * @param	pAutoManager : 설정할 AutoManager
 * @return	int (0 = Success, Error Code = 그 외)
 */
int MTrsJog::AssignAutoManager(MTrsAutoManager* pAutoManager)
{
	m_plnkTrsAutoManager = pAutoManager;
	return ERR_TRS_JOG_SUCCESS;
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/

UINT MTrsJog::threadFunction(LPVOID pParam)
{
	MTrsJog* pThis = (MTrsJog*) pParam;

	while (pThis->m_flgThreadLife)
	{
		pThis->threadJob();
		Sleep(5);
	}

	TRACE(_T("MTrsJog Thread Stop\n"));

	return ERR_TRS_JOG_SUCCESS;		
}

void MTrsJog::threadJob()
{
	int iResult = m_plnkMngOpPanel->MoveJog();
	if (iResult != ERR_TRS_JOG_SUCCESS)
		Sleep(100);

	processTowerLamp();
}

/** 
 * Tower Lamp를 관리 하는 함수
 * @param : iErrorCode = Error Code
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MTrsJog::processTowerLamp()
{
	int iTowerLampSts;
		
	switch (m_plnkTrsAutoManager->GetOPStatus())
	{
	case ERROR_STOP:
		iTowerLampSts = DEF_MNGOPPANEL_ERRORSTOP_ING;
		break;

	case STEP_STOP:
		if (m_plnkTrsAutoManager->GetOPMode() == AUTORUN_MODE)
			iTowerLampSts = DEF_MNGOPPANEL_STEPSTOP_ING;
		else
			iTowerLampSts = DEF_MNGOPPANEL_STEPSTOP;
		break;

	case START_RUN:
		iTowerLampSts = DEF_MNGOPPANEL_START;
		break;

	case CYCLE_STOP:
		iTowerLampSts = DEF_MNGOPPANEL_CYCLESTOP_ING;
		break;

	case RUN:
		iTowerLampSts = DEF_MNGOPPANEL_RUN;

		// 자동운전 중 Panel 유무 처리
		//20170920 KDW Start
		if (TRUE == m_plnkTrsAutoManager->IsPanelEmpty())
		{
			if(m_bMngoppanelPanelEmpty == FALSE)
			{
				if(!m_timer.IsTimerStarted())
					m_timer.StartTimer();
				
				if(m_timer.MoreThan(m_plnkTowerLampData->m_iDelayTime))
				{
					m_bMngoppanelPanelEmpty = TRUE;
					//iTowerLampSts = DEF_MNGOPPANEL_PANELEMPTY;
					m_timer.StopTimer();
				}
			}
		}
		else
		{
			m_bMngoppanelPanelEmpty = FALSE;
			m_timer.StopTimer();
		}
		if(m_bMngoppanelPanelEmpty)
			iTowerLampSts = DEF_MNGOPPANEL_PANELEMPTY;
		//20170920 KDW End

		if (TRUE == m_plnkTrsAutoManager->isFullPanelInEquipment()
			&& MOlbSystemPre.GetLCNetComponent()->m_eEqProcState == eIdle)
			iTowerLampSts = DEF_MNGOPPANEL_PANELFULL;

		/** 자재 관련 Alarm 상태인 경우 (Spec. : Auto/Manual 에 상관없이 Buzzer Alarm On!) but Auto 에서만 처리.
		 * ex) 자재교체 사전 Alarm, ACF 수동 박리 Alarm etc.
		 */
		if (m_plnkTrsAutoManager->IsMaterialExchange())
			iTowerLampSts = DEF_MNGOPPANEL_PARTSEMPTY;

		// 자동운전 중, Error Image Display 가 필요한 경우
		if (m_plnkTrsAutoManager->GetOPMode() == AUTORUN_MODE
			&& TRUE == m_plnkTrsAutoManager->IsDisplayErrorImage())
		{
			iTowerLampSts = DEF_MNGOPPANEL_ERRORSTOP_ING;
		}

		// hongju_SESL
#ifdef DEF_SESL_SITE
		// 자동운전 중, Pane In Stop이 눌러진 경우
		if (m_plnkTrsAutoManager->GetOPMode() == AUTORUN_MODE
			&& TRUE == m_plnkTrsAutoManager->IsPanelInStopLoadConveyor())
		{
			iTowerLampSts = DEF_MNGOPPANEL_PANEL_IN_STOP;
		}
#endif

		break;

	default:
		break;
	}

	if (m_plnkTrsAutoManager->IsOperatorCall())
	{
		iTowerLampSts = DEF_MNGOPPANEL_OP_CALL;
	}

	// NSMC
	if (DEF_MNGOPPANEL_ERRORSTOP_ING != iTowerLampSts
		&& RUN == m_plnkTrsAutoManager->GetOPStatus()
		&& m_plnkTrsAutoManager->IsStartModelChange())
	{
		iTowerLampSts = DEF_MNGOPPANEL_AUTO_MODEL_CHANGE;
	}

	/** 현재 상태에 따라 OpPanel 을 제어한다. */
	int iResult = m_plnkMngOpPanel->SetOpPanel(iTowerLampSts);
	if (iResult != ERR_MNGOPPANEL_SUCCESS)
		return iResult;

	return ERR_TRS_JOG_SUCCESS;
}
// End Of Code
