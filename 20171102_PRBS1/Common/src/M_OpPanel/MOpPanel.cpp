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
 *			  �� ������ op-panel Class�� ���� interface class�� �����Ѵ�.
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
	/** IO ��ü ���� */
	m_plnkIO = pIO;

	/** IO Address Table ���� */
	m_IOAddrTable = sPanelIOAddr;

	/** Jog ���� ���� */
	m_JogTable = sJogTable;

	/** ������� �ʴ� ������ ���� �κ� �ʱ�ȭ */
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
* memeber data�� �ʱ�ȭ �Ѵ�.	
*/
void MOpPanel::intializeData()
{

	/** Touch Panel Select IO Address �ʱ�ȭ */
//@KKY.130906    m_IOAddrTable.usiTouchSelectAddr = 0;
	for (int i=0; i<DEF_MAX_TOUCH_SELECT; i++)
		m_IOAddrTable.usiTouchSelectAddr[i] = 0;

	/** Door Sensor I Address, Use Flag �ʱ�ȭ */
	for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
	{
		for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
		{
			m_IOAddrTable.usiSafeDoorAddr[i][j] = 0;
			m_IOAddrTable.rgbSafeDoorFlag[i][j] = TRUE;
			m_IOAddrTable.rgbMaterialChangeFlag[i][j] = FALSE;//110411.Add
		}
	}

	/** �� Panel�� Switch �� LED�� IO Address �ʱ�ȭ */
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

	/** �� Panel�� Switch �� LED�� IO Address �ʱ�ȭ */
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

	/** �� Press Panel�� Switch �� LED�� IO Address �ʱ�ȭ */
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

	/** �� Press Panel�� Switch �� LED�� IO Address �ʱ�ȭ */
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
	
	/** Tower Lamp�� IO Address �ʱ�ȭ */
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

	/** Jog ���� �ʱ�ȭ */
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
* �Ҹ���
*
* @stereotype destructor 
*/
MOpPanel::~MOpPanel()
{
}

/**
* OpPanel���� ����� ���� IO Address�� �����Ѵ�.
*
* @param	opIOAddress : ������ ���� IO Address Table
*/
void MOpPanel::SetIOAddress(SOpPanelIOAddr opIOAddress)
{
	/** IO Address Table ���� */
	m_IOAddrTable = opIOAddress;
}

/**
* OpPanel���� ����� ���� IO Address�� �д´�.
*
* @param	*popIOAddress : ������ ���� IO Address Table
*/
void MOpPanel::GetIOAddress(SOpPanelIOAddr* popIOAddress)
{
	/** IO Address Table �μ� Pointer ���� ���� */
	ASSERT(popIOAddress != NULL);

	/** IO Address Table ���� */
	*popIOAddress = m_IOAddrTable;
}

/**
* OpPanel���� ����� Jog�� ������ Motion�� ���� ������ �����Ѵ�.
*
* @param	sJogTable : ������ Jog�� ������ Motion�� ���� ����
*/
void MOpPanel::SetJogTable(SJogTable sJogTable)
{
	/** Jog ���� ���� */
	m_JogTable = sJogTable;

	/** ������� �ʴ� ������ ���� �κ� �ʱ�ȭ */
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
* OpPanel���� ����� Jog�� ������ Motion�� ���� ������ �д´�.
*
* @param	*psJogTable : ������ Jog�� ������ Motion�� ���� ����
*/
void MOpPanel::GetJogTable(SJogTable* psJogTable)
{
	ASSERT(psJogTable != NULL);

	/** Jog ���� ���� */
	*psJogTable = m_JogTable;

	/** ������� �ʴ� ������ ���� �κ� �ʱ�ȭ */
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
 * Slow ������ Jog �̵� ������ �����Ѵ�.
 *
 * @param	iUnitIndex : Jog�� ������ Motion�� ���� ���� Table�� Index
 * @param	iKey : �̵��� Jog Key ���� (0:X, 1:Y, 2:T, 3:Z)
 * @param	bDir : �̵��� ���� (TRUE: +, FALSE: -)
 * @return	Error Code : 0=SUCCESS, �׿�=Error
 */
int MOpPanel::MoveJogSlow(int iUnitIndex, int iKey, BOOL bDir)
{

	int iResult = ERR_OPPANEL_SUCCESS;
	int iCoordID;
	CString strLogMsg;

	/** Unit Index ���� ���� */
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


	/** ������ Jog Key�� ���� ���� */
	switch (iKey)
	{
		/** X-key (Left/Right) */
	case DEF_OPPANEL_JOG_X_KEY :
		/** X-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow ���� �̵� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				/** Slow ���� �̵� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** Y-key (For/Back) */
	case DEF_OPPANEL_JOG_Y_KEY :
		/** Y-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow ���� �̵� */
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
		/** T-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow ���� �̵� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				/** Slow ���� �̵� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
		}
		break;

		/** Z-key (Up/Down) */
	case DEF_OPPANEL_JOG_Z_KEY :
		/** Z-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Slow ���� �̵� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkMultiJog->JogMoveSlow(bDir)) 
					!= ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			else
			{
				/** Slow ���� �̵� */
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
 * Fast ������ Jog �̵� ������ �����Ѵ�.
 *
 * @param	iUnitIndex : Jog�� ������ Motion�� ���� ���� Table�� Index
 * @param	iKey : �̵��� Jog Key ���� (0:X, 1:Y, 2:T, 3:Z)
 * @param	bDir : �̵��� ���� (TRUE: +, FALSE: -)
 * @return	Error Code : 0=SUCCESS, �׿�=Error
 */
int MOpPanel::MoveJogFast(int iUnitIndex, int iKey, BOOL bDir)
{

	int iResult = ERR_OPPANEL_SUCCESS;
	int iCoordID;
	CString strLogMsg;

	/** Unit Index ���� ���� */
	if ((iUnitIndex < 0) || (iUnitIndex > m_JogTable.m_iListNo))
	{
		return generateErrorCode(0);	// ERR_OPPANEL_INVALID_JOG_UNIT_INDEX
	}

	/** ������ Jog Key�� ���� ���� */
	switch (iKey)
	{
		/** X-key (Left/Right) */
	case DEF_OPPANEL_JOG_X_KEY :
		/** X-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Fast ���� �̵� */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast ���� �̵� */
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
		/** Y-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
		
		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Slow ���� �̵� */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast ���� �̵� */
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
		/** T-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;

		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Slow ���� �̵� */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast ���� �̵� */
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
		/** Z-key�� ������ �Ǿ� ������ */
		iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
		
		if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
		{
			/** Slow ���� �̵� */
			// Multi Jog
			if (1 == iCoordID)
			{
				/** Fast ���� �̵� */
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
 * Jog�� �̵��� �Ϳ� ���� ���� ������ �����Ѵ�.
 *
 * @param	iUnitIndex : Jog�� ������ Motion�� ���� ���� Table�� Index
 * @return	Error Code : 0=SUCCESS, �׿�=Error
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
 * Jog�� �̵��� �Ϳ� ���� ���� ������ �����Ѵ�.
 *
 * @param	iUnitIndex : Jog�� ������ Motion�� ���� ���� Table�� Index
 * @param	iKey : ������ Jog Key ���� (0:X, 1:Y, 2:T, 3:Z)
 * @return	Error Code : 0=SUCCESS, �׿�=Error
 */
int MOpPanel::StopJog(int iUnitIndex, int iKey)
{
#ifdef SIMULATION
	return ERR_OPPANEL_SUCCESS;
#endif

	int		iResult = ERR_OPPANEL_SUCCESS;
	int		iCoordID;
	CString	strLogMsg;

	/** Unit Index ���� ���� */
	if ((iUnitIndex < 0) || (iUnitIndex > m_JogTable.m_iListNo))
	{
		return generateErrorCode(0);
	}

	if (1 == GetMotionType(iUnitIndex, iKey))
	{// Sercos Motion�� ���.
		/** ������ Jog Key�� ���� ���� */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				// Multi Jog
				if (1 == iCoordID)
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkMultiJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
//				bDir = !(bDir^m_JogTable.m_Motion[iUnitIndex].m_YKey.m_bDirMask);
				
				// Multi Jog
				if (1 == iCoordID)
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkMultiJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
			/** T-key (CW/CCW) */
		case DEF_OPPANEL_JOG_T_KEY :
			/** T-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
//				bDir = !(bDir^m_JogTable.m_Motion[iUnitIndex].m_TKey.m_bDirMask);
				
				// Multi Jog
				if (1 == iCoordID)
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkMultiJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
			}
			break;
			
			/** Z-key (Up/Down) */
		case DEF_OPPANEL_JOG_Z_KEY :
			/** Z-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
//				bDir = !(bDir^m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_bDirMask);
				
				// Multi Jog
				if (1 == iCoordID)
				{
					/** �̵� ���� */
					if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
						return iResult;
				}
				else
				{
					/** �̵� ���� */
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
	{//ACS Motion (Gantry) �� ���
		//Jog Button�� ������ �ʾ��� ���� Stop �Ѵ�.

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

		/** ������ Jog Key�� ���� ���� */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if ((iCoordID > DEF_OPPANEL_NO_JOGKEY) && !bXpStatus && !bXnStatus)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if ((iCoordID > DEF_OPPANEL_NO_JOGKEY) && !bYpStatus && !bYnStatus)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;

		default :
			return generateErrorCode(0);
		}
	}
	else // MMC Motion�� ���.
	{
		/** ������ Jog Key�� ���� ���� */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** T-key (CW/CCW) */
		case DEF_OPPANEL_JOG_T_KEY :
			/** T-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->JogStop()) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Z-key (Up/Down) */
		case DEF_OPPANEL_JOG_Z_KEY :
			/** Z-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
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
	{// Sercos Motion�� ���.
		/** ������ Jog Key�� ���� ���� */
		switch (iKey)
		{
			/** X-key (Left/Right) */
		case DEF_OPPANEL_JOG_X_KEY :
			/** X-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_XKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Y-key (For/Back) */
		case DEF_OPPANEL_JOG_Y_KEY :
			/** Y-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_YKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** T-key (CW/CCW) */
		case DEF_OPPANEL_JOG_T_KEY :
			/** T-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
				if ((iResult = m_JogTable.m_Motion[iUnitIndex].m_TKey.m_plnkJog->CheckAxisStateForJog(bDir)) != ERR_OPPANEL_SUCCESS)
					return iResult;
			}
			break;
			
			/** Z-key (Up/Down) */
		case DEF_OPPANEL_JOG_Z_KEY :
			/** Z-key�� ������ �Ǿ� ������ */
			iCoordID = m_JogTable.m_Motion[iUnitIndex].m_ZKey.m_iAxisIndex;
			
			if (iCoordID > DEF_OPPANEL_NO_JOGKEY)
			{
				/** �̵� ���� */
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
* OpPanel���� ����� IO ��ü Pointer�� �����Ѵ�. 
*
* @param	*pIO : ������ IO Component Pointer
*/
void MOpPanel::AssignComponents(IIO* pIO)
{
	/** IO Object Pointer ���� ���� */
	ASSERT(pIO != NULL);

	/** IO Object Pointer ���� */
	m_plnkIO = pIO;
}

/**
* Start Button�� ���¸� �д´�.
*
* @param	*pbStatus : Start Button ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetStartButtonStatus()
{
	/** Start Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Start Button"), 
		                         m_IOAddrTable.FrontPanel.usiRunInputAddr,
								 m_IOAddrTable.BackPanel.usiRunInputAddr,
		                         m_IOAddrTable.FrontPressPanel.usiRunInputAddr,
								 m_IOAddrTable.BackPressPanel.usiRunInputAddr
								 );
}

/**
* Stop Button�� ���¸� �д´�.
*
* @param	*pbStatus : Stop Button ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetStopButtonStatus()
{
	/** Stop Button ������ ���� �б� */
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
* Reset Button�� ���¸� �д´�.
*
* @param	*pbStatus : E-Stop Button ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
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
 * Teaching Pendant Stop Button�� ���¸� �д´�.
 *
 * @param	*pbStatus : Teaching Pendant Stop Button ���� (TRUE : ON, FALSE : OFF)
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
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
* E-Stop Button �� ���¸� �д´�.
*
* @return	: E-Stop Button ���� (TRUE : ON, FALSE : OFF)
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
* Teaching Pendant ��� ���¸� �о�´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::GetTPStatus()
{
#ifdef SIMULATION
	return FALSE;
#endif

	return (m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiTPDetectedInputAddr));	
}

/**
* Jog Key�� Y(+����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogYPlusButtonStatus()
{
	/** Jog Y plus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog Y(+) Button"), 
		                        m_IOAddrTable.FrontPanel.usiYpInputAddr, 
								m_IOAddrTable.BackPanel.usiYpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiYpInputAddr, 
								m_IOAddrTable.BackPressPanel.usiYpInputAddr
								);
}

/**
* Jog Key�� Y(-����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogYMinusButtonStatus()
{
	/** Jog Y minus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog Y(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiYnInputAddr, 
								m_IOAddrTable.BackPanel.usiYnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiYnInputAddr, 
								m_IOAddrTable.BackPressPanel.usiYnInputAddr
								);
}

/**
* Jog Key�� X(+����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogXPlusButtonStatus()
{
	/** Jog X plus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog X(+) Button"), 
		                        m_IOAddrTable.FrontPanel.usiXpInputAddr, 
								m_IOAddrTable.BackPanel.usiXpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiXpInputAddr, 
								m_IOAddrTable.BackPressPanel.usiXpInputAddr
								);
}

/**
* Jog Key�� X(-����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogXMinusButtonStatus()
{
	/** Jog X minus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog X(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiXnInputAddr,
								m_IOAddrTable.BackPanel.usiXnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiXnInputAddr,
								m_IOAddrTable.BackPressPanel.usiXnInputAddr
								);
}

/**
* Jog Key�� Z(+����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogZPlusButtonStatus()
{
	/** Jog Z plus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog Z(+) Button"), 
		                        m_IOAddrTable.FrontPanel.usiZpInputAddr, 
								m_IOAddrTable.BackPanel.usiZpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiZpInputAddr, 
								m_IOAddrTable.BackPressPanel.usiZpInputAddr
								);
}

/**
* Jog Key�� Z(-����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogZMinusButtonStatus()
{
	/** Jog Z minus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog Z(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiZnInputAddr,
								m_IOAddrTable.BackPanel.usiZnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiZnInputAddr,
								m_IOAddrTable.BackPressPanel.usiZnInputAddr
								);
}

/**
* Jog Key�� T(+����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogTPlusButtonStatus()
{
	/** Jog T plus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog T(+) Button"),
		                        m_IOAddrTable.FrontPanel.usiTpInputAddr,
								m_IOAddrTable.BackPanel.usiTpInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiTpInputAddr,
								m_IOAddrTable.BackPressPanel.usiTpInputAddr
								);
}

/**
* Jog Key�� T(-����) Button�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
BOOL MOpPanel::GetJogTMinusButtonStatus()
{
	/** Jog T minus Button ������ ���� �б� */
	return getPanelSwitchStatus(_T("Jog T(-) Button"), 
		                        m_IOAddrTable.FrontPanel.usiTnInputAddr, 
								m_IOAddrTable.BackPanel.usiTnInputAddr,
		                        m_IOAddrTable.FrontPressPanel.usiTnInputAddr,
								m_IOAddrTable.BackPressPanel.usiTnInputAddr
								);
}

/**
* �������� (Door)�� ���¸� �д´�.
*
* @param	*pbStatus : �������� (Door)�� ���� (TRUE : ON, FALSE : OFF)
* @param	iGroup : (OPTION = -1) ���° �׷����� ��ȣ (-1 �̸� ��ü ������ Ȯ���Ͽ� �ϳ��� ON �̸� ����� ON ���� �˸���.)
* @param	iIndex : (OPTION = -1) ���° �������� ��ȣ (-1 �̸� ��ü ������ Ȯ���Ͽ� �ϳ��� ON �̸� ����� ON ���� �˸���.)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::GetSafeDoorStatus(int iGroup, int iIndex)
{
#ifdef SIMULATION
	return FALSE;
#endif

	int bStatus;

	/** ���� �׷�  ��ü Ȯ�� */
	if (iGroup == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
		{
			if (iIndex == -1)
			{
				for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
				{
					/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
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
					/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
					else
						j = DEF_OPPANEL_MAX_DOOR_SENSOR;
				}
			}
			else
			{
				/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
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
	/** ���� �׷� �ϳ��� Ȯ�� */
	else
	{
		if (iIndex == -1)
		{
			for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
			{
				/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
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
				/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
				else
					j = DEF_OPPANEL_MAX_DOOR_SENSOR;
			}
		}
		else
		{
			/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
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
 * Air Error�� ���¸� �д´�.
 *
 * @param	iIndex : (OPTION=-1) ���° �������� ��ȣ (-1�̸� ��ü ������ Ȯ���Ͽ� �ϳ��� ON�̸� ����� ON���� �˸���.)
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
BOOL MOpPanel::GetAirErrorStatus(int iIndex)
{
#ifdef SIMULATION
	return FALSE;
#endif

	BOOL bSts = FALSE;
	BOOL bResult;

	/** ���� ��ü Ȯ�� */
	if (iIndex == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_MAIN_AIR_NO; i++)
		{
			/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
			if (m_IOAddrTable.usiMainAirAddr[i] != 0)
			{
				bResult = m_plnkIO->IsOff(m_IOAddrTable.usiMainAirAddr[i]);

				bSts = bSts || bResult;
			}
			/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
			else
				break;
		}

		bResult = bSts;
	}
	/** ���� �ϳ��� Ȯ�� */
	else
	{
		ASSERT(iIndex >= 0 && iIndex < DEF_OPPANEL_MAX_MAIN_AIR_NO);
		/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
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
 * Vacuum Error�� ���¸� �д´�.
 *
 * @param	iIndex : (OPTION=-1) ���° �������� ��ȣ (-1�̸� ��ü ������ Ȯ���Ͽ� �ϳ��� ON�̸� ����� ON���� �˸���.)
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
BOOL MOpPanel::GetVacuumErrorStatus(int iIndex)
{
#ifdef SIMULATION
	return FALSE;
#endif

	BOOL bSts = FALSE;
	BOOL bResult;

	/** ���� ��ü Ȯ�� */
	if (iIndex == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_MAIN_VACUUM_NO; i++)
		{
			/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
			if (m_IOAddrTable.usiMainVacuumAddr[i] != 0)
			{
				bResult = m_plnkIO->IsOff(m_IOAddrTable.usiMainVacuumAddr[i]);

				bSts = bSts || bResult;
			}
			/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
			else
				break;
		}

		bResult = bSts;
	}
	/** ���� �ϳ��� Ȯ�� */
	else
	{
		ASSERT(iIndex >= 0 && iIndex < DEF_OPPANEL_MAX_MAIN_VACUUM_NO);

		/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
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
 * Door Sensor ���˿��θ� �����Ѵ�.
 *
 * @param	bFlag : ���� ���� (TRUE:����, FALSE:����)
 * @param	iGroup : (OPTION=-1) Door Sensor Group ��ȣ (-1�̸� ��� Group�� ����)
 * @param	iIndex : (OPTION=-1) Door Snesor Group �� Index ��ȣ (-1�̸� Group�� ��� Index ����)
 */
void MOpPanel::SetDoorCheckFlag(BOOL bFlag, int iGroup, int iIndex)
{
	int iResult = ERR_OPPANEL_SUCCESS;

	/** ���� �׷�  ��ü Ȯ�� */
	if (iGroup == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
		{
			if (iIndex == -1)
			{
				for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
				{
					/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
					if (m_IOAddrTable.usiSafeDoorAddr[i][j] != 0)
						m_IOAddrTable.rgbSafeDoorFlag[i][j] = bFlag;
					/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
					else
						j = DEF_OPPANEL_MAX_DOOR_SENSOR;
				}
			}
			else
			{
				/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
				if (m_IOAddrTable.usiSafeDoorAddr[i][iIndex] != 0)
					m_IOAddrTable.rgbSafeDoorFlag[i][iIndex] = bFlag;
			}
		}
	}
	/** ���� �׷� �ϳ��� Ȯ�� */
	else
	{
		if (iIndex == -1)
		{
			for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
			{
				/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
				if (m_IOAddrTable.usiSafeDoorAddr[iGroup][j] != 0)
					m_IOAddrTable.rgbSafeDoorFlag[iGroup][j] = bFlag;
				/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
				else
					j = DEF_OPPANEL_MAX_DOOR_SENSOR;
			}
		}
		else
		{
			/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
			if (m_IOAddrTable.usiSafeDoorAddr[iGroup][iIndex] != 0)
				m_IOAddrTable.rgbSafeDoorFlag[iGroup][iIndex] = bFlag;
		}
	}
}

void MOpPanel::SetMaterialChangeFlag(BOOL bFlag, int iGroup, int iIndex)
{
	int iResult = ERR_OPPANEL_SUCCESS;

	/** ���� �׷�  ��ü Ȯ�� */
	if (iGroup == -1)
	{
		for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
		{
			if (iIndex == -1)
			{
				for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
				{
					/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
					if (m_IOAddrTable.usiSafeDoorAddr[i][j] != 0)
						m_IOAddrTable.rgbMaterialChangeFlag[i][j] = bFlag;
					/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
					else
						j = DEF_OPPANEL_MAX_DOOR_SENSOR;
				}
			}
			else
			{
				/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
				if (m_IOAddrTable.usiSafeDoorAddr[i][iIndex] != 0)
					m_IOAddrTable.rgbMaterialChangeFlag[i][iIndex] = bFlag;
			}
		}
	}
	/** ���� �׷� �ϳ��� Ȯ�� */
	else
	{
		if (iIndex == -1)
		{
			for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
			{
				/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
				if (m_IOAddrTable.usiSafeDoorAddr[iGroup][j] != 0)
					m_IOAddrTable.rgbMaterialChangeFlag[iGroup][j] = bFlag;
				/** Sensor Address�� �Ҵ�Ǿ� ���� ������ Ȯ�� �ߴ� */
				else
					j = DEF_OPPANEL_MAX_DOOR_SENSOR;
			}
		}
		else
		{
			/** Sensor Address�� �Ҵ�Ǿ� �־�߸� Ȯ�� */
			if (m_IOAddrTable.usiSafeDoorAddr[iGroup][iIndex] != 0)
				m_IOAddrTable.rgbMaterialChangeFlag[iGroup][iIndex] = bFlag;
		}
	}
}

/**
* Start Button�� LED ���¸� �����Ѵ�.
*
* @param	bStatus : ������ Start Button LED ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetStartLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Start Button�� Led �����ϱ� */
		iResult = setPanelLedStatus(_T("Start LED"), 
			                        m_IOAddrTable.FrontPanel.usiRunOutputAddr,
									m_IOAddrTable.BackPanel.usiRunOutputAddr,
									bStatus);
	}

	return iResult;
}

/**
* Stop Button�� LED ���¸� �����Ѵ�.
*
* @param	bStatus : ������ Stop Button LED ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetStopLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Stop Button�� Led �����ϱ� */
		iResult = setPanelLedStatus(_T("Stop LED"),
			                        m_IOAddrTable.FrontPanel.usiStopOutputAddr, 
									m_IOAddrTable.BackPanel.usiStopOutputAddr, 
									bStatus);
	}

	return iResult;
}

/**
* Reset Button�� LED ���¸� �����Ѵ�.
*
* @param	bStatus : ������ Reset Button LED ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetResetLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Reset Button�� Led �����ϱ� */
		iResult = setPanelLedStatus(_T("Reset LED"), 
			                        m_IOAddrTable.FrontPanel.usiResetOutputAddr,
									m_IOAddrTable.BackPanel.usiResetOutputAddr,
									bStatus);
	}

	return iResult;
}

/**
* Tower Lamp�� Red Lamp ���¸� �����Ѵ�.
*
* @param	bStatus : ������ Red Lamp ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetTowerRedLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Tower Lamp�� Red Lamp �����ϱ� */
		iResult = setTowerLampStatus(_T("RED Lamp"), m_IOAddrTable.TowerLamp.usiRedLampAddr, bStatus);
	}

	return iResult;
}

/**
* Tower Lamp�� Yellow Lamp ���¸� �����Ѵ�.
*
* @param	bStatus : ������ Yellow Lamp ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetTowerYellowLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Tower Lamp�� Yellow Lamp �����ϱ� */
		iResult = setTowerLampStatus(_T("YELLOW Lamp"), m_IOAddrTable.TowerLamp.usiYellowLampAddr, bStatus);
	}

	return iResult;
}

/**
* Tower Lamp�� Green Lamp ���¸� �����Ѵ�.
*
* @param	bStatus : ������ Green Lamp ���� (TRUE : ON, FALSE : OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetTowerGreenLamp(BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus = DEF_INITIAL_STATUS;

	if (bOldStatus != bStatus)
	{
		bOldStatus = bStatus;
		/** Tower Lamp�� Green Lamp �����ϱ� */
		iResult = setTowerLampStatus(_T("GREEN Lamp"), m_IOAddrTable.TowerLamp.usiGreenLampAddr, bStatus);
	}

	return iResult;
}

/**
* Buzzer ���¸� �����Ѵ�.
*
* @param	iMode : ������ Buzzer ��� (0=, 1=, 2=, 3=)
* @param	bStatus : ��� ���� (TRUE=ON, FALSE=OFF)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::SetBuzzerStatus(int iMode, BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;
	static BOOL bOldStatus[DEF_OPPANEL_MAX_BUZZER_MODE]
		= {DEF_INITIAL_STATUS, DEF_INITIAL_STATUS, DEF_INITIAL_STATUS, DEF_INITIAL_STATUS};

	/** Tower Lamp�� Buzzer�� ���� ��ü ��� ��尡 ���õ� ��� */
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
	/** Tower Lamp�� Buzzer�� ���� �ϳ��� ��� ��尡 ���õ� ��� */
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
* Ȱ��ȭ�� Touch Panel�� ID�� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::GetEnabledOpPanelID()
{
	/** IO�� FALSE�̸� �� Touch Panel */
	//@KKY.130906_____________
//@	if (FALSE == m_plnkIO->IsOn(m_IOAddrTable.usiTouchSelectAddr))
//@		return DEF_OPPANEL_FRONT_PANEL_ID;	
	/** IO�� TRUE�̸� �� Touch Panel */

	if (FALSE == m_plnkIO->IsOn(m_IOAddrTable.usiTouchSelectAddr[0]))
		return DEF_OPPANEL_FRONT_PANEL_ID;

	return DEF_OPPANEL_BACK_PANEL_ID;
}

/**
* �ش� ID�� Touch Panel�� Ȱ��ȭ�Ѵ�.
*
* @param	iOpPanelID : Ȱ��ȭ�� Touch Panel�� ID (1 : �ո�, 2 : �޸�)
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::ChangeOpPanel(int iOpPanelID)
{
	/** ��ȯ�� Touch Panel�� ���� Ȱ��ȭ�� Touch Panel�� ��ġ�ϸ� Pass */
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
* Touch Panel�� Ȱ��ȭ�� ���¸� �д´�.
*
* @return	Error Code : 0 = SUCCESS, �׿� = Error
*/
int MOpPanel::GetOpPanelSelectSW()
{
	BOOL bFrontStatus, bBackStatus;
	//@KKY.130906___________
	BOOL bFrontPressStatus, bBackPressStatus;
	//@_____________________

	/** �� Panel�� Button�� ���� �б� */
	if (m_IOAddrTable.FrontPanel.usiRunInputAddr != NULL)
		bFrontStatus = m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiRunInputAddr);
	else
		bFrontStatus = FALSE;

	if (m_IOAddrTable.FrontPanel.usiStopInputAddr != NULL)
		bFrontStatus |= m_plnkIO->IsOn(m_IOAddrTable.FrontPanel.usiStopInputAddr);
	else
		bFrontStatus |= FALSE;

	/** �� Panel�� Button�� ���� �б� */
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

	/** Front Stop Switch�� ���������� - �ո����� ��ȯ */
	if (bFrontStatus == TRUE)
	{
		return DEF_OPPANEL_FRONT_PANEL_ID;
	}
	/** Back Stop Switch�� ���������� - �޸����� ��ȯ */
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
	/** �ش� ���� ���� �� */
	else
	{
		return DEF_OPPANEL_NONE_PANEL_ID;
	}

	return ERR_OPPANEL_SUCCESS;
}


/**
 * ��, �� Panel�� Ư�� Switch�� ������ ���¸� Ȯ���Ѵ�.
 *
 * @param	strLogName : Log�� �� ����� Switch �̸�
 * @param	usiFrontSWAddr : �� Panel�� Switch IO Address
 * @param	usiBackSWAddr : �� Panel�� Switch IO Address
 * @param	*pbStatus : ��, �� Panel�� Switch ������ ���� (�� �� �ϳ��� ������ TRUE)
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
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

	/** �� Panel�� Button�� ���� �б� */
	if (usiFrontSWAddr != NULL)
		bFrontStatus = m_plnkIO->IsOn(usiFrontSWAddr);
	else
		bFrontStatus = FALSE;

	/** �� Panel�� Button�� ���� �б� */
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

	/** ��/�� Panel �� �ϳ��� ���������� TRUE Return */
//@	return (bFrontStatus || bBackStatus);
	return (bFrontStatus || bBackStatus || bFrontPressStatus || bBackPressStatus);

	return ERR_OPPANEL_SUCCESS;
}

/**
 * ��, �� Panel�� Ư�� LED�� ������ �����Ѵ�.
 *
 * @param	strLogName : Log�� �� ����� LED �̸�
 * @param	usiFrontSWAddr : �� Panel�� LED IO Address
 * @param	usiBackSWAddr : �� Panel�� LED IO Address
 * @param	bStatus : ������ ��, �� Panel�� LED ���ۻ��� (��, �޸� �Ѵ� ����)
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
int MOpPanel::setPanelLedStatus(CString strLogName, 
								unsigned short usiFrontLedAddr, 
								unsigned short usiBackLedAddr, 
								BOOL bStatus)
{
	int iResult = ERR_OPPANEL_SUCCESS;

	/** LED�� ON �����ϱ� */
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
	/** LED�� OFF �����ϱ� */
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
 * Tower Lamp�� Lamp, Buzzer�� ������ �����Ѵ�.
 *
 * @param	strLogName : Log�� �� ����� Tower Lamp ���� ��� �̸�
 * @param	usiTowerAddr : �� Panel�� LED IO Address
 * @param	bStatus : ������ Tower Lamp�� Lamp, Buzzer�� ���ۻ���
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
int MOpPanel::setTowerLampStatus(CString strLogName, unsigned short usiTowerAddr, BOOL bStatus)
{
	/** ���� ��Ҹ� ON �����ϱ� */
	if (bStatus == TRUE)
		return m_plnkIO->OutputOn(usiTowerAddr);

	/** ���� ��Ҹ� OFF �����ϱ� */
	return m_plnkIO->OutputOff(usiTowerAddr);
}
