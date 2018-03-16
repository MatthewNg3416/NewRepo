/* 
 * OpPanel Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. (_T("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MOpPanel.cpp : This file implements the class of op-panel component.
 *			  이 파일은 op-panel Class에 대한 interface class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#include "stdafx.h"
#include "MOpPanel.h"

#include "common.h"
#include "DefSystemConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MOpPanel::MOpPanel()
{
	intializeData();
}

MOpPanel::MOpPanel(SCommonAttribute commonData, IIO *pIO, SOpPanelIOAddr sPanelIOAddr, SJogTable sJogTable)
 : ICommon(commonData)
{
	/** IO 개체 연결 */
	m_plnkIO = pIO;

	/** IO Address Table 설정 */
	m_IOAddrTable = sPanelIOAddr;

	/** Jog 정보 설정 */
	m_JogTable = sJogTable;

	/** 사용하지 않는 정보에 대한 부분 초기화 */
	for (int i = m_JogTable.m_iListNo; i < DEF_OPPANEL_MAX_JOG_LIST; i++)
	{
		m_JogTable.m_Motion[i].m_XKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_XKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_YKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_YKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_TKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_TKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_ZKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_ZKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
	}
}

/**
* memeber data를 초기화 한다.	
*/
void MOpPanel::intializeData()
{

	/** Touch Panel Select IO Address 초기화 */
//@KKY.130906    m_IOAddrTable.usiTouchSelectAddr = 0;
	for (int i=0; i<DEF_MAX_TOUCH_SELECT; i++)
		m_IOAddrTable.usiTouchSelectAddr[i] = 0;

	/** Door Sensor I Address, Use Flag 초기화 */
	for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
	{
		for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
		{
			m_IOAddrTable.usiSafeDoorAddr[i][j] = 0;
			m_IOAddrTable.rgbSafeDoorFlag[i][j] = TRUE;
			m_IOAddrTable.rgbMaterialChangeFlag[i][j] = FALSE;//110411.Add
		}
	}

	/** 앞 Panel의 Switch 및 LED의 IO Address 초기화 */
	m_IOAddrTable.FrontPanel.usiTPStopInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiResetInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiResetOutputAddr = 0;
	m_IOAddrTable.FrontPanel.usiRunInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiRunOutputAddr = 0;
	m_IOAddrTable.FrontPanel.usiStopInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiStopOutputAddr = 0;
	m_IOAddrTable.FrontPanel.usiTnInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiTpInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiXnInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiXpInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiYnInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiYpInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiZnInputAddr = 0;
	m_IOAddrTable.FrontPanel.usiZpInputAddr = 0;

	/** 뒷 Panel의 Switch 및 LED의 IO Address 초기화 */
	m_IOAddrTable.BackPanel.usiTPStopInputAddr = 0;
	m_IOAddrTable.BackPanel.usiResetInputAddr = 0;
	m_IOAddrTable.BackPanel.usiResetOutputAddr = 0;
	m_IOAddrTable.BackPanel.usiRunInputAddr = 0;
	m_IOAddrTable.BackPanel.usiRunOutputAddr = 0;
	m_IOAddrTable.BackPanel.usiStopInputAddr = 0;
	m_IOAddrTable.BackPanel.usiStopOutputAddr = 0;
	m_IOAddrTable.BackPanel.usiTnInputAddr = 0;
	m_IOAddrTable.BackPanel.usiTpInputAddr = 0;
	m_IOAddrTable.BackPanel.usiXnInputAddr = 0;
	m_IOAddrTable.BackPanel.usiXpInputAddr = 0;
	m_IOAddrTable.BackPanel.usiYnInputAddr = 0;
	m_IOAddrTable.BackPanel.usiYpInputAddr = 0;
	m_IOAddrTable.BackPanel.usiZnInputAddr = 0;
	m_IOAddrTable.BackPanel.usiZpInputAddr = 0;

	/** 앞 Press Panel의 Switch 및 LED의 IO Address 초기화 */
	m_IOAddrTable.FrontPressPanel.usiTPStopInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiResetInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiResetOutputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiRunInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiRunOutputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiStopInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiStopOutputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiTnInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiTpInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiXnInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiXpInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiYnInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiYpInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiZnInputAddr = 0;
	m_IOAddrTable.FrontPressPanel.usiZpInputAddr = 0;

	/** 뒷 Press Panel의 Switch 및 LED의 IO Address 초기화 */
	m_IOAddrTable.BackPressPanel.usiTPStopInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiResetInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiResetOutputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiRunInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiRunOutputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiStopInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiStopOutputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiTnInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiTpInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiXnInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiXpInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiYnInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiYpInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiZnInputAddr = 0;
	m_IOAddrTable.BackPressPanel.usiZpInputAddr = 0;
	
	/** Tower Lamp의 IO Address 초기화 */
	for (int i = 0; i < DEF_OPPANEL_MAX_BUZZER_MODE; i++)
		m_IOAddrTable.TowerLamp.usiBuzzerAddr[i] = 0;

	m_IOAddrTable.TowerLamp.usiGreenLampAddr = 0;
	m_IOAddrTable.TowerLamp.usiRedLampAddr = 0;
	m_IOAddrTable.TowerLamp.usiYellowLampAddr = 0;

	/** E-STOP Switch Status IO Address */
	for (int i = 0; i < DEF_OPPANEL_MAX_ESTOP_RELAY_NO; i++)
		m_IOAddrTable.usiEStopInputAddr[i] = 0;

	/** Main Air Check IO Address */
	for (int i = 0; i < DEF_OPPANEL_MAX_MAIN_AIR_NO; i++)
		m_IOAddrTable.usiMainAirAddr[i] = 0;

	/** Main Vacuum Check IO Address */
	for (int i = 0; i < DEF_OPPANEL_MAX_MAIN_VACUUM_NO; i++)
		m_IOAddrTable.usiMainVacuumAddr[i] = 0;

	/** Jog 정보 초기화 */
	m_JogTable.m_iListNo = 0;

	for (int i = 0; i < DEF_OPPANEL_MAX_JOG_LIST; i++)
	{
		m_JogTable.m_Motion[i].m_XKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_XKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_YKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_YKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_TKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_TKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_ZKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_ZKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
	}

   /** IO Object */
    m_plnkIO = NULL;
}

/**
* 소멸자
*
* @stereotype destructor 
*/
MOpPanel::~MOpPanel()
{
}

/**
* OpPanel에서 사용할 각종 IO Address를 설정한다.
*
* @param	opIOAddress : 설정할 각종 IO Address Table
*/
void MOpPanel::SetIOAddress(SOpPanelIOAddr opIOAddress)
{
	/** IO Address Table 설정 */
	m_IOAddrTable = opIOAddress;
}

/**
* OpPanel에서 사용할 각종 IO Address를 읽는다.
*
* @param	*popIOAddress : 설정된 각종 IO Address Table
*/
void MOpPanel::GetIOAddress(SOpPanelIOAddr* popIOAddress)
{
	/** IO Address Table 인수 Pointer 오류 점검 */
	ASSERT(popIOAddress != NULL);

	/** IO Address Table 전달 */
	*popIOAddress = m_IOAddrTable;
}

/**
* OpPanel에서 사용할 Jog로 움직일 Motion에 대한 정보를 설정한다.
*
* @param	sJogTable : 설정할 Jog로 움직일 Motion에 대한 정보
*/
void MOpPanel::SetJogTable(SJogTable sJogTable)
{
	/** Jog 정보 설정 */
	m_JogTable = sJogTable;

	/** 사용하지 않는 정보에 대한 부분 초기화 */
	for (int i = m_JogTable.m_iListNo; i < DEF_OPPANEL_MAX_JOG_LIST; i++)
	{
		m_JogTable.m_Motion[i].m_XKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_XKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_YKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_YKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_TKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_TKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		m_JogTable.m_Motion[i].m_ZKey.m_plnkJog = NULL;
		m_JogTable.m_Motion[i].m_ZKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
	}
}

/**
* OpPanel에서 사용할 Jog로 움직일 Motion에 대한 정보를 읽는다.
*
* @param	*psJogTable : 설정된 Jog로 움직일 Motion에 대한 정보
*/
void MOpPanel::GetJogTable(SJogTable* psJogTable)
{
	ASSERT(psJogTable != NULL);

	/** Jog 정보 설정 */
	*psJogTable = m_JogTable;

	/** 사용하지 않는 정보에 대한 부분 초기화 */
	for (int i = m_JogTable.m_iListNo; i < DEF_OPPANEL_MAX_JOG_LIST; i++)
	{
		psJogTable->m_Motion[i].m_XKey.m_plnkJog = NULL;
		psJogTable->m_Motion[i].m_XKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		psJogTable->m_Motion[i].m_YKey.m_plnkJog = NULL;
		psJogTable->m_Motion[i].m_YKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		psJogTable->m_Motion[i].m_TKey.m_plnkJog = NULL;
		psJogTable->m_Motion[i].m_TKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
		psJogTable->m_Motion[i].m_ZKey.m_plnkJog = NULL;
		psJogTable->m_Motion[i].m_ZKey.m_iAxisIndex = DEF_OPPANEL_NO_JOGKEY;
	}
}

int MOpPanel::GetMotionType(int iUnitIndex, int iKey)
{
	if (0 == iKey)	// DEF_MNGOPPANEL_JOG_X_KEY
	{
		return m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iMotionType;
	}
	else if (1 == iKey)	// DEF_MNGOPPANEL_JOG_Y_KEY
	{
		return m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iMotionType;
	}
	else if (2 == iKey)	// DEF_MNGOPPANEL_JOG_T_KEY
	{
		return m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iMotionType;
	}
	else if (3 == iKey)// DEF_MNGOPPANEL_JOG_Z_KEY
	{
		return m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iMotionType;
	}

	return m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iMotionType;
}

/**
 * Slow 단위의 Jog 이동 동작을 수행한다.
 *
 * @param	iUnitIndex : Jog로 움직일 Motion에 대한 정보 Table의 Index
 * @param	iKey : 이동할 Jog Key 종류 (0:X, 1:Y, 2:T, 3:Z)
 * @param	bDir : 이동할 방향 (TRUE: +, FALSE: -)
 * @return	Error Code : 0=SUCCESS, 그외=Error
 */
int MOpPanel::MoveJogSlow(int iUnitIndex, int iKey, BOOL bDir)
{

	int iResult = ERR_OPPANEL_SUCCESS;
	int iCoordID;
	CString strLogMsg;

	/** Unit Index 범위 점검 */
	if ((iUnitIndex < 0) || (iUnitIndex > m_JogTable.m_iListNo))
	{
		//WriteErrorLog("MoveJogSlow() : Invalid Unit Index");
		//return generateErrorCode(0);	// ERR_OPPANEL_INVALID_JOG_UNIT_INDEX
	}

	// Doolin kwangilshin 2017. 08. 25.
 	//
// 	iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
// 	
// 	if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
// 	{
//  		m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->SetPositiveSensorEvent(DEF_E_STOP_EVENT);
//  		m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->SetNegativeSensorEvent(DEF_E_STOP_EVENT);
// 	
// 		if (bDir)
// 		{
// 			if (m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->IsOnPositiveSensor())
// 			{
// 				//MyMessageBox(_T("Test"), _T("Jog Move"), M_ICONINFORMATION);
// 				return 0;
// 			}
// 		}
// 		else
// 		{
// 			if (m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->IsOnNegativeSensor())
// 			{
// 				//MyMessageBox(_T("Test"), _T("Jog Move"), M_ICONINFORMATION);
// 				return 0;
// 			}
// 		}
// 	}
	// End.


	/** 눌려진 Jog Key에 따라 동작 */
	switch (iKey)
	{
		/** X-key (Left/Right) */
	case DEF_OPPANEL_JOG_X_KEY :
		/** X-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** Y-key (For/Back) */
	case DEF_OPPANEL_JOG_Y_KEY :
		/** Y-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** T-key (CW/CCW) */
	case DEF_OPPANEL_JOG_T_KEY :
		/** T-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** Z-key (Up/Down) */
	case DEF_OPPANEL_JOG_Z_KEY :
		/** Z-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				/** Slow 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

	default :
		return generateErrorCode(0);	// ERR_OPPANEL_INVALID_JOG_KEY_TYPE
	}

	return iResult;
}

/**
 * Fast 단위의 Jog 이동 동작을 수행한다.
 *
 * @param	iUnitIndex : Jog로 움직일 Motion에 대한 정보 Table의 Index
 * @param	iKey : 이동할 Jog Key 종류 (0:X, 1:Y, 2:T, 3:Z)
 * @param	bDir : 이동할 방향 (TRUE: +, FALSE: -)
 * @return	Error Code : 0=SUCCESS, 그외=Error
 */
int MOpPanel::MoveJogFast(int iUnitIndex, int iKey, BOOL bDir)
{

	int iResult = ERR_OPPANEL_SUCCESS;
	int iCoordID;
	CString strLogMsg;

	/** Unit Index 범위 점검 */
	if ((iUnitIndex < 0) || (iUnitIndex > m_JogTable.m_iListNo))
	{
		return generateErrorCode(0);	// ERR_OPPANEL_INVALID_JOG_UNIT_INDEX
	}

	/** 눌려진 Jog Key에 따라 동작 */
	switch (iKey)
	{
		/** X-key (Left/Right) */
	case DEF_OPPANEL_JOG_X_KEY :
		/** X-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Fast 단위 이동 */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkMultiJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** Y-key (For/Back) */
	case DEF_OPPANEL_JOG_Y_KEY :
		/** Y-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
		
		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Slow 단위 이동 */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkMultiJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** T-key (CW/CCW) */
	case DEF_OPPANEL_JOG_T_KEY :
		/** T-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Slow 단위 이동 */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkMultiJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** Z-key (Up/Down) */
	case DEF_OPPANEL_JOG_Z_KEY :
		/** Z-key에 설정이 되어 있으면 */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
		
		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Slow 단위 이동 */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast 단위 이동 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkMultiJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->JogMoveFast(bDir))
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

	default :
		return generateErrorCode(0);	// ERR_OPPANEL_INVALID_JOG_KEY_TYPE
	}

	return iResult;
}

/**
 * Jog로 이동한 것에 대한 정지 동작을 수행한다.
 *
 * @param	iUnitIndex : Jog로 움직일 Motion에 대한 정보 Table의 Index
 * @return	Error Code : 0=SUCCESS, 그외=Error
 */
int MOpPanel::StopJog(int iUnitIndex)
{
	int iResult;

	for (int i = 0; i < (DEF_OPPANEL_JOG_Z_KEY + 1); i++)
	{
		iResult = StopJog(iUnitIndex,i);
		if (iResult)
			return iResult;
	}
	return ERR_OPPANEL_SUCCESS;
}

/**
 * Jog로 이동한 것에 대한 정지 동작을 수행한다.
 *
 * @param	iUnitIndex : Jog로 움직일 Motion에 대한 정보 Table의 Index
 * @param	iKey : 정지할 Jog Key 종류 (0:X, 1:Y, 2:T, 3:Z)
 * @return	Error Code : 0=SUCCESS, 그외=Error
 */
int MOpPanel::StopJog(int iUnitIndex, int iKey)
{
#ifdef SIMULATION
	return ERR_OPPANEL_SUCCESS;
#endif

	int		iResult = ERR_OPPANEL_SUCCESS;
	int		iCoordID;
	CString	strLogMsg;

	/** Unit Index 범위 점검 */
	if ((iUnitIndex < 0) || (iUnitIndex > m_JogTable.m_iListNo))
	{
		return generateErrorCode(0);
	}

	if (1 == GetMotionType(iUnitIndex, iKey))
	{// Sercos Motion의 경우.
		/** 눌려진 Jog Key에 따라 동작 */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				// Multi Jog
				if (1 == iCoordID)
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkMultiJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
//				bDir = !(bDir^m_JogTable.m_Motion[iUnitIndex].m_YKey.m_bDirMask);
				
				// Multi Jog
				if (1 == iCoordID)
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkMultiJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
			/** T-key (CW/CCW) */
		case DEF_OPPANEL_JOG_T_KEY :
			/** T-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
//				bDir = !(bDir^m_JogTable.m_Motion[iUnitIndex].m_TKey.m_bDirMask);
				
				// Multi Jog
				if (1 == iCoordID)
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkMultiJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
			/** Z-key (Up/Down) */
		case DEF_OPPANEL_JOG_Z_KEY :
			/** Z-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
//				bDir = !(bDir^m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_bDirMask);
				
				// Multi Jog
				if (1 == iCoordID)
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** 이동 정지 */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
		default :
			return generateErrorCode(0);
		}
	}
	else if (2 == GetMotionType(iUnitIndex, iKey))
	{//ACS Motion (Gantry) 인 경우
		//Jog Button을 누르지 않았을 때만 Stop 한다.

		BOOL bXpStatus, bXnStatus;
		BOOL bYpStatus, bYnStatus;

		/** X(+) Key Read */
		bXpStatus = GetJogXPlusButtonStatus();
		/** X(-) Key Read */
		bXnStatus = GetJogXMinusButtonStatus();
		/** Y(+) Key Read */
		bYpStatus = GetJogYPlusButtonStatus();
		/** Y(-) Key Read */
		bYnStatus = GetJogYMinusButtonStatus();

		/** 눌려진 Jog Key에 따라 동작 */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if ((iCoordID > DEF_OPPANEL_NO_JOGKEY) && !bXpStatus && !bXnStatus)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if ((iCoordID > DEF_OPPANEL_NO_JOGKEY) && !bYpStatus && !bYnStatus)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;

		default :
			return generateErrorCode(0);
		}
	}
	else // MMC Motion인 경우.
	{
		/** 눌려진 Jog Key에 따라 동작 */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** T-key (CW/CCW) */
		case DEF_OPPANEL_JOG_T_KEY :
			/** T-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Z-key (Up/Down) */
		case DEF_OPPANEL_JOG_Z_KEY :
			/** Z-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
		default :
			return generateErrorCode(0);
		}
	}

	return iResult;
}

int MOpPanel::CheckAxisStateForJog(int iUnitIndex, int iKey, BOOL bDir)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	int		iCoordID;
	CString	strLogMsg;

	if (1 == GetMotionType(iUnitIndex, iKey))
	{// Sercos Motion의 경우.
		/** 눌려진 Jog Key에 따라 동작 */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** T-key (CW/CCW) */
		case DEF_OPPANEL_JOG_T_KEY :
			/** T-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Z-key (Up/Down) */
		case DEF_OPPANEL_JOG_Z_KEY :
			/** Z-key에 설정이 되어 있으면 */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** 이동 정지 */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
		default :
			return generateErrorCode(0);
		}
	}

	return iResult;
}

/**
* OpPanel에서 사용할 IO 개체 Pointer를 설정한다. 
*
* @param	*pIO : 설정할 IO Component Pointer
*/
void MOpPanel::AssignComponents(IIO* pIO)
{
	/** IO Object Pointer 오류 점검 */
	ASSERT(pIO != NULL);

	/** IO Object Pointer 연결 */
	m_plnkIO = pIO;
}

/**
* Start Button의 상태를 읽는다.
*
* @param	*pbStatus : Start Button 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetStartButtonStatus()
{
	/** Start Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Start Button"), 
		                         m_IOAddrTable.FrontPanel.usiRunInputAddr,
								 m_IOAddrTable.BackPanel.usiRunInputAddr,
		                         m_IOAddrTable.FrontPressPanel.usiRunInputAddr,
								 m_IOAddrTable.BackPressPanel.usiRunInputAddr
								 );
}

/**
* Stop Button의 상태를 읽는다.
*
* @param	*pbStatus : Stop Button 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetStopButtonStatus()
{
	/** Stop Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Stop Button"),
		                        m_IOAddrTable.FrontPanel.usiStopInputAddr,
								m_IOAddrTable.BackPanel.usiStopInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiStopInputAddr,
								m_IOAddrTable.BackPressPanel.usiStopInputAddr
								);
}

//20100212.kms____________________
BOOL MOpPanel::GetStopButtonStatus(BOOL* bIsFrontStopBtn)
{
	BOOL bIsOnFrontStopBtn = FALSE, bIsOnBackStopBtn = FALSE;
	
	if(m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiStopInputAddr)) bIsOnFrontStopBtn = TRUE;
	else if(m_plnkIO->IsOn(m_IOAddrTable.BackPanel.usiStopInputAddr)) bIsOnBackStopBtn = TRUE;
	
	if(bIsOnFrontStopBtn || bIsOnBackStopBtn)
	{
		if(bIsOnFrontStopBtn)
		{
			*bIsFrontStopBtn = TRUE;
		}
		else
		{
			*bIsFrontStopBtn = FALSE;
		}
		
		return TRUE;
	}
	else
	{
		*bIsFrontStopBtn = FALSE;
		return FALSE;
	}
}

/**
* Reset Button의 상태를 읽는다.
*
* @param	*pbStatus : E-Stop Button 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetResetButtonStatus()
{
	return getPanelSwitchStatus(_T("Reset Button"), 
		                        m_IOAddrTable.FrontPanel.usiResetInputAddr,
								m_IOAddrTable.BackPanel.usiResetInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiResetInputAddr,
								m_IOAddrTable.BackPressPanel.usiResetInputAddr
								);
}

/**
 * Teaching Pendant Stop Button의 상태를 읽는다.
 *
 * @param	*pbStatus : Teaching Pendant Stop Button 상태 (TRUE : ON, FALSE : OFF)
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
BOOL MOpPanel::GetTPStopButtonStatus()
{
	return getPanelSwitchStatus(_T("TP Stop Button"),
		                        m_IOAddrTable.FrontPanel.usiTPStopInputAddr,
								m_IOAddrTable.BackPanel.usiTPStopInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiTPStopInputAddr,
								m_IOAddrTable.BackPressPanel.usiTPStopInputAddr
								);
}

/**
* E-Stop Button 의 상태를 읽는다.
*
* @return	: E-Stop Button 상태 (TRUE : ON, FALSE : OFF)
*/
int MOpPanel::GetEStopButtonStatus()
{
#ifndef SIMULATION
	for (int i = 0; i < DEF_OPPANEL_MAX_ESTOP_RELAY_NO; i++)
	{
		if (NULL != m_IOAddrTable.usiEStopInputAddr[i] && FALSE == m_plnkIO->IsOn(m_IOAddrTable.usiEStopInputAddr[i]))
			return TRUE;
	}
#endif
	return FALSE;
}

/**
* Teaching Pendant 사용 상태를 읽어온다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::GetTPStatus()
{
#ifdef SIMULATION
	return FALSE;
#endif

	return (m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiTPDetectedInputAddr));	
}

/**
* Jog Key중 Y(+방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogYPlusButtonStatus()
{
	/** Jog Y plus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog Y(+) Button"), 
		                        m_IOAddrTable.FrontPanel.usiYpInputAddr, 
								m_IOAddrTable.BackPanel.usiYpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiYpInputAddr, 
								m_IOAddrTable.BackPressPanel.usiYpInputAddr
								);
}

/**
* Jog Key중 Y(-방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogYMinusButtonStatus()
{
	/** Jog Y minus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog Y(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiYnInputAddr, 
								m_IOAddrTable.BackPanel.usiYnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiYnInputAddr, 
								m_IOAddrTable.BackPressPanel.usiYnInputAddr
								);
}

/**
* Jog Key중 X(+방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogXPlusButtonStatus()
{
	/** Jog X plus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog X(+) Button"), 
		                        m_IOAddrTable.FrontPanel.usiXpInputAddr, 
								m_IOAddrTable.BackPanel.usiXpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiXpInputAddr, 
								m_IOAddrTable.BackPressPanel.usiXpInputAddr
								);
}

/**
* Jog Key중 X(-방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogXMinusButtonStatus()
{
	/** Jog X minus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog X(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiXnInputAddr,
								m_IOAddrTable.BackPanel.usiXnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiXnInputAddr,
								m_IOAddrTable.BackPressPanel.usiXnInputAddr
								);
}

/**
* Jog Key중 Z(+방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogZPlusButtonStatus()
{
	/** Jog Z plus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog Z(+) Button"), 
		                        m_IOAddrTable.FrontPanel.usiZpInputAddr, 
								m_IOAddrTable.BackPanel.usiZpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiZpInputAddr, 
								m_IOAddrTable.BackPressPanel.usiZpInputAddr
								);
}

/**
* Jog Key중 Z(-방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogZMinusButtonStatus()
{
	/** Jog Z minus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog Z(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiZnInputAddr,
								m_IOAddrTable.BackPanel.usiZnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiZnInputAddr,
								m_IOAddrTable.BackPressPanel.usiZnInputAddr
								);
}

/**
* Jog Key중 T(+방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogTPlusButtonStatus()
{
	/** Jog T plus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog T(+) Button"),
		                        m_IOAddrTable.FrontPanel.usiTpInputAddr,
								m_IOAddrTable.BackPanel.usiTpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiTpInputAddr,
								m_IOAddrTable.BackPressPanel.usiTpInputAddr
								);
}

/**
* Jog Key중 T(-방향) Button의 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
BOOL MOpPanel::GetJogTMinusButtonStatus()
{
	/** Jog T minus Button 눌려진 상태 읽기 */
	return getPanelSwitchStatus(_T("Jog T(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiTnInputAddr, 
								m_IOAddrTable.BackPanel.usiTnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiTnInputAddr,
								m_IOAddrTable.BackPressPanel.usiTnInputAddr
								);
}

/**
* 안전센서 (Door)의 상태를 읽는다.
*
* @param	*pbStatus : 안전센서 (Door)의 상태 (TRUE : ON, FALSE : OFF)
* @param	iGroup : (OPTION = -1) 몇번째 그룹인지 번호 (-1 이면 전체 센서를 확인하여 하나라도 ON 이면 결과를 ON 으로 알린다.)
* @param	iIndex : (OPTION = -1) 몇번째 센서인지 번호 (-1 이면 전체 센서를 확인하여 하나라도 ON 이면 결과를 ON 으로 알린다.)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::GetSafeDoorStatus(int iGroup, int iIndex)
{
#ifdef SIMULATION
	return FALSE;
#endif

	int bStatus;

	/** 센서 그룹  전체 확인 */
	if (iGroup == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
		{
			if (iIndex == -1)
			{
				for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
				{
					/** Sensor Address가 할당되어 있어야만 확인 */
					if (m_IOAddrTable.usiSafeDoorAddr[i][j] != 0)
					{
						//if (m_IOAddrTable.rgbSafeDoorFlag[i][j] == TRUE)
						if (m_IOAddrTable.rgbSafeDoorFlag[i][j] == TRUE && m_IOAddrTable.rgbMaterialChangeFlag[i][j] == FALSE)	//110411.Modify
						{
							if (m_plnkIO->IsOn(m_IOAddrTable.usiSafeDoorAddr[i][j]) == TRUE)
							{
								return TRUE;
							}
						}
					}
					/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
					else
						j = DEF_OPPANEL_MAX_DOOR_SENSOR;
				}
			}
			else
			{
				/** Sensor Address가 할당되어 있어야만 확인 */
				if (m_IOAddrTable.usiSafeDoorAddr[i][iIndex] != 0)
				{
//					if (m_IOAddrTable.rgbSafeDoorFlag[i][iIndex] == TRUE)
					if (m_IOAddrTable.rgbSafeDoorFlag[i][iIndex] == TRUE && m_IOAddrTable.rgbMaterialChangeFlag[i][iIndex] == FALSE)	//110411.Modify
						bStatus = (m_plnkIO->IsOn(m_IOAddrTable.usiSafeDoorAddr[i][iIndex]));
					else
						bStatus = FALSE;
				}
				else
					bStatus = FALSE;
			}
		}
		bStatus = FALSE;
	}
	/** 센서 그룹 하나만 확인 */
	else
	{
		if (iIndex == -1)
		{
			for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
			{
				/** Sensor Address가 할당되어 있어야만 확인 */
				if (m_IOAddrTable.usiSafeDoorAddr[iGroup][j] != 0)
				{
					//if (m_IOAddrTable.rgbSafeDoorFlag[iGroup][j] == TRUE)
					if (m_IOAddrTable.rgbSafeDoorFlag[iGroup][j] == TRUE && m_IOAddrTable.rgbMaterialChangeFlag[iGroup][j] == FALSE)	//110411.Modify
					{
						if (m_plnkIO->IsOn(m_IOAddrTable.usiSafeDoorAddr[iGroup][j]) == TRUE)
						{
							return TRUE;
						}
					}
				}
				/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
				else
					j = DEF_OPPANEL_MAX_DOOR_SENSOR;
			}
		}
		else
		{
			/** Sensor Address가 할당되어 있어야만 확인 */
			if (m_IOAddrTable.usiSafeDoorAddr[iGroup][iIndex] != 0)
			{
				//if (m_IOAddrTable.rgbSafeDoorFlag[iGroup][iIndex] == TRUE)
				if (m_IOAddrTable.rgbSafeDoorFlag[iGroup][iIndex] == TRUE && m_IOAddrTable.rgbMaterialChangeFlag[iGroup][iIndex] == FALSE)	//110411.Modify)
					bStatus = (m_plnkIO->IsOn(m_IOAddrTable.usiSafeDoorAddr[iGroup][iIndex]));
				else
					bStatus = FALSE;
			}
			else
				bStatus = FALSE;
		}
		bStatus = FALSE;
	}
	return bStatus;
}

/**
 * Air Error의 상태를 읽는다.
 *
 * @param	iIndex : (OPTION=-1) 몇번째 센서인지 번호 (-1이면 전체 센서를 확인하여 하나라도 ON이면 결과를 ON으로 알린다.)
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
BOOL MOpPanel::GetAirErrorStatus(int iIndex)
{
#ifdef SIMULATION
	return FALSE;
#endif

	BOOL bSts = FALSE;
	BOOL bResult;

	/** 센서 전체 확인 */
	if (iIndex == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_MAIN_AIR_NO; i++)
		{
			/** Sensor Address가 할당되어 있어야만 확인 */
			if (m_IOAddrTable.usiMainAirAddr[i] != 0)
			{
				bResult = m_plnkIO->IsOff(m_IOAddrTable.usiMainAirAddr[i]);

				bSts = bSts || bResult;
			}
			/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
			else
				break;
		}

		bResult = bSts;
	}
	/** 센서 하나만 확인 */
	else
	{
		ASSERT(iIndex >= 0 && iIndex < DEF_OPPANEL_MAX_MAIN_AIR_NO);
		/** Sensor Address가 할당되어 있어야만 확인 */
		if (m_IOAddrTable.usiMainAirAddr[iIndex] != 0)
		{
			bResult = m_plnkIO->IsOff(m_IOAddrTable.usiMainAirAddr[iIndex]);
		}
		else
			bResult = FALSE;
	}

	return bResult;
}

/**
 * Vacuum Error의 상태를 읽는다.
 *
 * @param	iIndex : (OPTION=-1) 몇번째 센서인지 번호 (-1이면 전체 센서를 확인하여 하나라도 ON이면 결과를 ON으로 알린다.)
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
BOOL MOpPanel::GetVacuumErrorStatus(int iIndex)
{
#ifdef SIMULATION
	return FALSE;
#endif

	BOOL bSts = FALSE;
	BOOL bResult;

	/** 센서 전체 확인 */
	if (iIndex == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_MAIN_VACUUM_NO; i++)
		{
			/** Sensor Address가 할당되어 있어야만 확인 */
			if (m_IOAddrTable.usiMainVacuumAddr[i] != 0)
			{
				bResult = m_plnkIO->IsOff(m_IOAddrTable.usiMainVacuumAddr[i]);

				bSts = bSts || bResult;
			}
			/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
			else
				break;
		}

		bResult = bSts;
	}
	/** 센서 하나만 확인 */
	else
	{
		ASSERT(iIndex >= 0 && iIndex < DEF_OPPANEL_MAX_MAIN_VACUUM_NO);

		/** Sensor Address가 할당되어 있어야만 확인 */
		if (m_IOAddrTable.usiMainVacuumAddr[iIndex] != 0)
		{
			bResult = m_plnkIO->IsOff(m_IOAddrTable.usiMainVacuumAddr[iIndex]);
		}
		else
			bResult = FALSE;
	}

	return bResult;
}


/**
 * Door Sensor 점검여부를 설정한다.
 *
 * @param	bFlag : 점검 여부 (TRUE:점검, FALSE:무시)
 * @param	iGroup : (OPTION=-1) Door Sensor Group 번호 (-1이면 모든 Group내 설정)
 * @param	iIndex : (OPTION=-1) Door Snesor Group 내 Index 번호 (-1이면 Group내 모든 Index 설정)
 */
void MOpPanel::SetDoorCheckFlag(BOOL bFlag, int iGroup, int iIndex)
{
	int iResult = ERR_OPPANEL_SUCCESS;

	/** 센서 그룹  전체 확인 */
	if (iGroup == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
		{
			if (iIndex == -1)
			{
				for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
				{
					/** Sensor Address가 할당되어 있어야만 확인 */
					if (m_IOAddrTable.usiSafeDoorAddr[i][j] != 0)
						m_IOAddrTable.rgbSafeDoorFlag[i][j] = bFlag;
					/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
					else
						j = DEF_OPPANEL_MAX_DOOR_SENSOR;
				}
			}
			else
			{
				/** Sensor Address가 할당되어 있어야만 확인 */
				if (m_IOAddrTable.usiSafeDoorAddr[i][iIndex] != 0)
					m_IOAddrTable.rgbSafeDoorFlag[i][iIndex] = bFlag;
			}
		}
	}
	/** 센서 그룹 하나만 확인 */
	else
	{
		if (iIndex == -1)
		{
			for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
			{
				/** Sensor Address가 할당되어 있어야만 확인 */
				if (m_IOAddrTable.usiSafeDoorAddr[iGroup][j] != 0)
					m_IOAddrTable.rgbSafeDoorFlag[iGroup][j] = bFlag;
				/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
				else
					j = DEF_OPPANEL_MAX_DOOR_SENSOR;
			}
		}
		else
		{
			/** Sensor Address가 할당되어 있어야만 확인 */
			if (m_IOAddrTable.usiSafeDoorAddr[iGroup][iIndex] != 0)
				m_IOAddrTable.rgbSafeDoorFlag[iGroup][iIndex] = bFlag;
		}
	}
}

void MOpPanel::SetMaterialChangeFlag(BOOL bFlag, int iGroup, int iIndex)
{
	int iResult = ERR_OPPANEL_SUCCESS;

	/** 센서 그룹  전체 확인 */
	if (iGroup == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
		{
			if (iIndex == -1)
			{
				for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
				{
					/** Sensor Address가 할당되어 있어야만 확인 */
					if (m_IOAddrTable.usiSafeDoorAddr[i][j] != 0)
						m_IOAddrTable.rgbMaterialChangeFlag[i][j] = bFlag;
					/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
					else
						j = DEF_OPPANEL_MAX_DOOR_SENSOR;
				}
			}
			else
			{
				/** Sensor Address가 할당되어 있어야만 확인 */
				if (m_IOAddrTable.usiSafeDoorAddr[i][iIndex] != 0)
					m_IOAddrTable.rgbMaterialChangeFlag[i][iIndex] = bFlag;
			}
		}
	}
	/** 센서 그룹 하나만 확인 */
	else
	{
		if (iIndex == -1)
		{
			for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
			{
				/** Sensor Address가 할당되어 있어야만 확인 */
				if (m_IOAddrTable.usiSafeDoorAddr[iGroup][j] != 0)
					m_IOAddrTable.rgbMaterialChangeFlag[iGroup][j] = bFlag;
				/** Sensor Address가 할당되어 있지 않으면 확인 중단 */
				else
					j = DEF_OPPANEL_MAX_DOOR_SENSOR;
			}
		}
		else
		{
			/** Sensor Address가 할당되어 있어야만 확인 */
			if (m_IOAddrTable.usiSafeDoorAddr[iGroup][iIndex] != 0)
				m_IOAddrTable.rgbMaterialChangeFlag[iGroup][iIndex] = bFlag;
		}
	}
}

/**
* Start Button의 LED 상태를 변경한다.
*
* @param	bStatus : 변경할 Start Button LED 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetStartLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Start Button의 Led 동작하기 */
		iResult = setPanelLedStatus(_T("Start LED"), 
			                        m_IOAddrTable.FrontPanel.usiRunOutputAddr,
									m_IOAddrTable.BackPanel.usiRunOutputAddr,
									bStatus);
	}

	return iResult;
}

/**
* Stop Button의 LED 상태를 변경한다.
*
* @param	bStatus : 변경할 Stop Button LED 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetStopLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Stop Button의 Led 동작하기 */
		iResult = setPanelLedStatus(_T("Stop LED"),
			                        m_IOAddrTable.FrontPanel.usiStopOutputAddr, 
									m_IOAddrTable.BackPanel.usiStopOutputAddr, 
									bStatus);
	}

	return iResult;
}

/**
* Reset Button의 LED 상태를 변경한다.
*
* @param	bStatus : 변경할 Reset Button LED 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetResetLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Reset Button의 Led 동작하기 */
		iResult = setPanelLedStatus(_T("Reset LED"), 
			                        m_IOAddrTable.FrontPanel.usiResetOutputAddr,
									m_IOAddrTable.BackPanel.usiResetOutputAddr,
									bStatus);
	}

	return iResult;
}

/**
* Tower Lamp의 Red Lamp 상태를 변경한다.
*
* @param	bStatus : 변경할 Red Lamp 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetTowerRedLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Tower Lamp의 Red Lamp 동작하기 */
		iResult = setTowerLampStatus(_T("RED Lamp"), m_IOAddrTable.TowerLamp.usiRedLampAddr, bStatus);
	}

	return iResult;
}

/**
* Tower Lamp의 Yellow Lamp 상태를 변경한다.
*
* @param	bStatus : 변경할 Yellow Lamp 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetTowerYellowLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Tower Lamp의 Yellow Lamp 동작하기 */
		iResult = setTowerLampStatus(_T("YELLOW Lamp"), m_IOAddrTable.TowerLamp.usiYellowLampAddr, bStatus);
	}

	return iResult;
}

/**
* Tower Lamp의 Green Lamp 상태를 변경한다.
*
* @param	bStatus : 변경할 Green Lamp 상태 (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetTowerGreenLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Tower Lamp의 Green Lamp 동작하기 */
		iResult = setTowerLampStatus(_T("GREEN Lamp"), m_IOAddrTable.TowerLamp.usiGreenLampAddr, bStatus);
	}

	return iResult;
}

/**
* Buzzer 상태를 변경한다.
*
* @param	iMode : 변경할 Buzzer 출력 (0=, 1=, 2=, 3=)
* @param	bStatus : 출력 상태 (TRUE=ON, FALSE=OFF)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::SetBuzzerStatus(int iMode, BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus[DEF_OPPANEL_MAX_BUZZER_MODE]
		= {DEF_INITIAL_STATUS, DEF_INITIAL_STATUS, DEF_INITIAL_STATUS, DEF_INITIAL_STATUS};

	/** Tower Lamp의 Buzzer에 대한 전체 출력 모드가 선택된 경우 */
	if (iMode == DEF_OPPANEL_BUZZER_ALL)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_BUZZER_MODE; i++)
		{
			if (bOldStatus[i] != bStatus)
			{
				bOldStatus[i] = bStatus;
				if ((iResult = setTowerLampStatus(_T("Buzzer"), 
					                              m_IOAddrTable.TowerLamp.usiBuzzerAddr[i], 
												  bStatus)) 
				   != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
	}
	/** Tower Lamp의 Buzzer에 대한 하나의 출력 모드가 선택된 경우 */
	else
	{
		if (bOldStatus[iMode] != bStatus)
		{
			bOldStatus[iMode] = bStatus;
			if ((iResult = setTowerLampStatus(_T("Buzzer"), 
				                              m_IOAddrTable.TowerLamp.usiBuzzerAddr[iMode],
											  bStatus)) 
			    != ERR_OPPANEL_SUCCESS)
				return iResult;
		}
	}

	return iResult;
}

/**
* 활성화된 Touch Panel의 ID를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::GetEnabledOpPanelID()
{
	/** IO가 FALSE이면 앞 Touch Panel */
	//@KKY.130906_____________
//@	if (FALSE == m_plnkIO->IsOn(m_IOAddrTable.usiTouchSelectAddr))
//@		return DEF_OPPANEL_FRONT_PANEL_ID;	
	/** IO가 TRUE이면 뒷 Touch Panel */

	if (FALSE == m_plnkIO->IsOn(m_IOAddrTable.usiTouchSelectAddr[0]))
		return DEF_OPPANEL_FRONT_PANEL_ID;

	return DEF_OPPANEL_BACK_PANEL_ID;
}

/**
* 해당 ID의 Touch Panel를 활성화한다.
*
* @param	iOpPanelID : 활성화할 Touch Panel의 ID (1 : 앞면, 2 : 뒷면)
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::ChangeOpPanel(int iOpPanelID)
{
	/** 전환할 Touch Panel이 현재 활성화된 Touch Panel과 일치하면 Pass */
	if (iOpPanelID == GetEnabledOpPanelID())
		return ERR_OPPANEL_SUCCESS;

	/*/KKY.130906___________
	if (iOpPanelID == DEF_OPPANEL_FRONT_PANEL_ID)
	{
		int iResult = m_plnkIO->OutputOff(m_IOAddrTable.usiTouchSelectAddr);
		if (iResult)
		{
			SetErrorLevel(_T("OPPANEL change Touch Panel"),2, __FILE__, __LINE__);
			return iResult;
		}
	}
	else if (iOpPanelID == DEF_OPPANEL_BACK_PANEL_ID)
	{
		int iResult = m_plnkIO->OutputOn(m_IOAddrTable.usiTouchSelectAddr);
		if (iResult)
		{
			SetErrorLevel(_T("OPPANEL change Touch Panel"),2, __FILE__, __LINE__);
			return iResult;
		}
	}
	/*/
	if (iOpPanelID == DEF_OPPANEL_FRONT_PANEL_ID)
	{
		int iResult = m_plnkIO->OutputOff(m_IOAddrTable.usiTouchSelectAddr[0]);
		if (iResult)
			return iResult;
	}
	else if (iOpPanelID == DEF_OPPANEL_BACK_PANEL_ID)
	{
		int iResult = m_plnkIO->OutputOn(m_IOAddrTable.usiTouchSelectAddr[0]);
		if (iResult)
			return iResult;
	}

	return ERR_OPPANEL_SUCCESS;
}

/**
* Touch Panel의 활성화할 상태를 읽는다.
*
* @return	Error Code : 0 = SUCCESS, 그외 = Error
*/
int MOpPanel::GetOpPanelSelectSW()
{
	BOOL bFrontStatus, bBackStatus;
	//@KKY.130906___________
	BOOL bFrontPressStatus, bBackPressStatus;
	//@_____________________

	/** 앞 Panel의 Button의 상태 읽기 */
	if (m_IOAddrTable.FrontPanel.usiRunInputAddr != NULL)
		bFrontStatus = m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiRunInputAddr);
	else
		bFrontStatus = FALSE;

	if (m_IOAddrTable.FrontPanel.usiStopInputAddr != NULL)
		bFrontStatus |= m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiStopInputAddr);
	else
		bFrontStatus |= FALSE;

	/** 뒷 Panel의 Button의 상태 읽기 */
	if (m_IOAddrTable.BackPanel.usiRunInputAddr != NULL)
		bBackStatus = m_plnkIO->IsOn(m_IOAddrTable.BackPanel.usiRunInputAddr);
	else
		bBackStatus = FALSE;

	if (m_IOAddrTable.BackPanel.usiStopInputAddr != NULL)
		bBackStatus |= m_plnkIO->IsOn(m_IOAddrTable.BackPanel.usiStopInputAddr);
	else
		bBackStatus |= FALSE;

	//@KKY.130906_____________
	if (m_IOAddrTable.FrontPressPanel.usiRunInputAddr != NULL)
		bFrontPressStatus = m_plnkIO->IsOn(m_IOAddrTable.FrontPressPanel.usiRunInputAddr);
	else
		bFrontPressStatus = FALSE;

	if (m_IOAddrTable.FrontPressPanel.usiStopInputAddr != NULL)
		bFrontPressStatus |= m_plnkIO->IsOn(m_IOAddrTable.FrontPressPanel.usiStopInputAddr);
	else
		bFrontPressStatus |= FALSE;

	if (m_IOAddrTable.BackPressPanel.usiRunInputAddr != NULL)
		bBackPressStatus = m_plnkIO->IsOn(m_IOAddrTable.BackPressPanel.usiRunInputAddr);
	else
		bBackPressStatus = FALSE;

	if (m_IOAddrTable.BackPressPanel.usiStopInputAddr != NULL)
		bBackPressStatus |= m_plnkIO->IsOn(m_IOAddrTable.BackPressPanel.usiStopInputAddr);
	else
		bBackPressStatus |= FALSE;
	//@_______________________

	/** Front Stop Switch가 눌려졌으면 - 앞면으로 전환 */
	if (bFrontStatus == TRUE)
	{
		return DEF_OPPANEL_FRONT_PANEL_ID;
	}
	/** Back Stop Switch가 눌려졌으면 - 뒷면으로 전환 */
	else if (bBackStatus == TRUE)
	{
		return DEF_OPPANEL_BACK_PANEL_ID;
	}
	//@KKY.130906___________
	else if (bFrontPressStatus == TRUE)
	{
		return DEF_OPPANEL_FRONT_PANEL_ID_PRESS;
	}
	else if (bBackPressStatus == TRUE)
	{
		return DEF_OPPANEL_BACK_PANEL_ID_PRESS;
	}
	//@_____________________
	/** 해당 사항 없을 때 */
	else
	{
		return DEF_OPPANEL_NONE_PANEL_ID;
	}

	return ERR_OPPANEL_SUCCESS;
}


/**
 * 앞, 뒷 Panel의 특정 Switch의 눌려진 상태를 확인한다.
 *
 * @param	strLogName : Log할 때 사용할 Switch 이름
 * @param	usiFrontSWAddr : 앞 Panel의 Switch IO Address
 * @param	usiBackSWAddr : 뒷 Panel의 Switch IO Address
 * @param	*pbStatus : 앞, 뒷 Panel의 Switch 눌려진 상태 (둘 중 하나라도 눌리면 TRUE)
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
BOOL MOpPanel::getPanelSwitchStatus(CString strLogName, 
									unsigned short usiFrontSWAddr, 
									unsigned short usiBackSWAddr,
									unsigned short usiFrontPressSWAddr, 
									unsigned short usiBackPressSWAddr)
{
	BOOL bFrontStatus = FALSE;
	BOOL bBackStatus = FALSE;
	BOOL bFrontPressStatus = FALSE;
	BOOL bBackPressStatus = FALSE;

	/** 앞 Panel의 Button의 상태 읽기 */
	if (usiFrontSWAddr != NULL)
		bFrontStatus = m_plnkIO->IsOn(usiFrontSWAddr);
	else
		bFrontStatus = FALSE;

	/** 뒷 Panel의 Button의 상태 읽기 */
	if (usiBackSWAddr != NULL)
		bBackStatus = m_plnkIO->IsOn(usiBackSWAddr);
	else
		bBackStatus = FALSE;

	//@KKY.130906__________
	if (usiFrontPressSWAddr != NULL)
		bFrontPressStatus = m_plnkIO->IsOn(usiFrontPressSWAddr);
	else
		bFrontPressStatus = FALSE;

	if (usiFrontPressSWAddr != NULL)
		bBackPressStatus = m_plnkIO->IsOn(usiBackPressSWAddr);
	else
		bBackPressStatus = FALSE;
	//@____________________

	/** 앞/뒷 Panel 중 하나라도 눌려졌으면 TRUE Return */
//@	return (bFrontStatus || bBackStatus);
	return (bFrontStatus || bBackStatus || bFrontPressStatus || bBackPressStatus);

	return ERR_OPPANEL_SUCCESS;
}

/**
 * 앞, 뒷 Panel의 특정 LED의 동작을 설정한다.
 *
 * @param	strLogName : Log할 때 사용할 LED 이름
 * @param	usiFrontSWAddr : 앞 Panel의 LED IO Address
 * @param	usiBackSWAddr : 뒷 Panel의 LED IO Address
 * @param	bStatus : 설정할 앞, 뒷 Panel의 LED 동작상태 (앞, 뒷면 둘다 동작)
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
int MOpPanel::setPanelLedStatus(CString strLogName, 
								unsigned short usiFrontLedAddr, 
								unsigned short usiBackLedAddr, 
								BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;

	/** LED를 ON 동작하기 */
	if (bStatus == TRUE)
	{
		if (usiFrontLedAddr != NULL)
		{
			iResult = m_plnkIO->OutputOn(usiFrontLedAddr);
			if (iResult != ERR_OPPANEL_SUCCESS)
				return iResult;
		}

		if (usiBackLedAddr != NULL)
		{
			iResult = m_plnkIO->OutputOn(usiBackLedAddr);
			if (iResult != ERR_OPPANEL_SUCCESS)
				return iResult;
		}
	}
	/** LED를 OFF 동작하기 */
	else
	{
		if (usiFrontLedAddr != NULL)
		{
			iResult = m_plnkIO->OutputOff(usiFrontLedAddr);
			if (iResult != ERR_OPPANEL_SUCCESS)
				return iResult;
		}

		if (usiBackLedAddr != NULL)
		{
			iResult = m_plnkIO->OutputOff(usiBackLedAddr);
			if (iResult != ERR_OPPANEL_SUCCESS)
				return iResult;
		}
	}

	return ERR_OPPANEL_SUCCESS;
}

/**
 * Tower Lamp의 Lamp, Buzzer의 동작을 설정한다.
 *
 * @param	strLogName : Log할 때 사용할 Tower Lamp 동작 요소 이름
 * @param	usiTowerAddr : 앞 Panel의 LED IO Address
 * @param	bStatus : 설정할 Tower Lamp의 Lamp, Buzzer의 동작상태
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
int MOpPanel::setTowerLampStatus(CString strLogName, unsigned short usiTowerAddr, BOOL bStatus)
{
	/** 동작 요소를 ON 동작하기 */
	if (bStatus == TRUE)
		return m_plnkIO->OutputOn(usiTowerAddr);

	/** 동작 요소를 OFF 동작하기 */
	return m_plnkIO->OutputOff(usiTowerAddr);
}
