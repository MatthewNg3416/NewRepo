/* 
 * Handler Component
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
 * MHandlerCommonCommon.cpp : Implementation of Handler component.
 */

#include "stdafx.h"
#include "MHandlerCommon.h"
#include "MNeth.h"
#include <math.h>
#include "DefIOAddrPreBonder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

/** 
 * Constructor
 * 
 * @param	SCommonAttribute commonData (���������� ����� Data)
 * @param	SHandlerRefCompList listRefComponent (������ Component ������ ����Ʈ)
 */
MHandlerCommon::MHandlerCommon(SCommonAttribute commonData, SHandlerRefCompList listRefComponent)
: ICommon(commonData)
{
	// Hanlder Vacuum 1,2 �� ��� ������� �ʴ� ��츦 ����ؼ� Null �ʱ�ȭ
	int i = 0;
//@	m_plnkVac1 = NULL;
//@	m_plnkVac2 = NULL;
//@	m_plnkVac3 = NULL;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		m_plnkVac[i]	= NULL;
	}


	SetCommonAttribute(commonData);	
	AssignComponents(listRefComponent);

	for (i = 0; i < DEF_HANDLER_MAX_POS; i++)
	{
		/** ��ǥ�� �ʱ�ȭ */
		m_rgsXYTFixedPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTOffsetPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTModelPos[i].Init(0.0, 0.0, 0.0);
		// kss 5. ���ʿ�
	}

	for (i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		/** ��ǥ�� �ʱ�ȭ */
		m_rgdZFixedPos[i]						= 0.0;	// kss 14 �̸� ���� �ʿ� Y -> Z
		m_rgdZModelPos[i]						= 0.0;	// kss 14
		m_rgdZOffsetPos[i]						= 0.0;	// kss 14
	}

//	m_usOAddrPanelBlow1		= OUT_UHANDLER_BLOW1;
//	m_usOAddrPanelBlow2		= OUT_UHANDLER_BLOW2;
}

/**
 * @stereotype destructor 
 */
MHandlerCommon::~MHandlerCommon()
{
}

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

/**
 * Handler Component���� ������ Component List�� �����Ѵ�.
 *
 * @param	SHandlerRefCompList listRefComponets (�����ϰ��� �ϴ� Component ���� List)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::AssignComponents(SHandlerRefCompList listRefComponents)
{
	m_plnkIO	= listRefComponents.m_plnkIO;
//@	m_plnkVac1	= listRefComponents.m_plnkVac1;
//@	m_plnkVac2	= listRefComponents.m_plnkVac2;
//@	m_plnkVac3	= listRefComponents.m_plnkVac3;
	int i = 0;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		m_plnkVac[i]	= listRefComponents.m_plnkVac[i];
	}

	// jdy 7. ���� ���� �ּ� ���� �ʿ� (Stage => Handler ��)
	// Handler X Motion Object
	m_plnkXAxis = listRefComponents.m_plnkXAxis;

	// Handler Y Motion Object
	m_plnkYAxis = listRefComponents.m_plnkYAxis;
	
	// Handler T Motion Object
	m_plnkTAxis = listRefComponents.m_plnkTAxis;

	// Handler Z Motion Object
	m_plnkZAxis = listRefComponents.m_plnkZAxis;

	m_plnkMelsecIF	= listRefComponents.m_plnkMelsecIF;

	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� Motion Component�� ���� �޴´�.
 * @param nAxisID : �� ID (0: X��, 1:Y�� , 2:T��, 3:Z��)
 * @return	IAixs : �ش��ϴ� �� ID�� �´� Axis 
 */
IAxis*	MHandlerCommon::GetMotionComponent(int nAxisID)
{
	// jdy 9. �鿩���� ����, �Ʒ� �ּ� ���� (STU �� �߸��� �ּ�)
		// Motion Component �� ID�� ����
	switch (nAxisID)
	{
		case DEF_HANDLER_X_AXIS :		// Handler X�� 
			return m_plnkXAxis;

		case DEF_HANDLER_Y_AXIS :		// Handler Y��
			return m_plnkYAxis;

		case DEF_HANDLER_T_AXIS :		// Handler T��
			return m_plnkTAxis;

		case DEF_HANDLER_Z_AXIS :		// Handler Z��
			return m_plnkZAxis;

		default :					// �� ID ����
			return NULL;
	}
}



/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/ 

/**********
 * Vacuum
 */

/**
 * Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::AbsorbPanel(BOOL bSkipSensor)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif

	int iResult = ERR_HANDLER_SUCCESS;

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� - StepDisplay �� ���� ����. �α� �޼��� ���� ���� �ʿ�.
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Absorb] [Start] ");
	WriteTactTimeLog(m_szLogMsg);
	
	/*/
#ifdef DEF_USE_UNLOAD_HANDLER
	/ ** Vacuum Off�Ǿ� �ִ��� Ȯ�� * /
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		if (!IsAbsorbPanel() || bSkipSensor)
		{
			if (m_plnkPanelData->m_dPanelSize_X < DEF_DISTANCE_USE_VACUUM2)
			{
				ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
				
				if ((iResult = m_plnkVac1->On(TRUE)) != ERR_HANDLER_SUCCESS)
				{
					//���� ���� ����
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				if ( m_plnkVac2 !=NULL)
				{
					if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
					{
						//���� ���� ����
//						m_plnkVac1->On();
//@						WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
						return iResult;
					}
				}
				if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
				{
					//���� ���� ����
//					m_plnkVac2->Off();
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				
			}
			else
			{
				ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL && m_plnkVac3 != NULL);
				
				if ((iResult = m_plnkVac1->On(TRUE)) != ERR_HANDLER_SUCCESS)
				{
					//���� ���� ����
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				
				if (!m_plnkVac2->IsOn())
				{
					if ((iResult = m_plnkVac2->On(TRUE)) != ERR_HANDLER_SUCCESS)
					{
						//���� ���� ����
//@						WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
						return iResult;
					}
				}
				
				if( m_iHandlerType != DEF_HANDLER_LOADING)
				{
					if (!m_plnkVac3->IsOn())
					{
						if ((iResult = m_plnkVac3->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
						{
							//���� ���� ����
	//						m_plnkVac2->Off(TRUE);
//@							WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
							return iResult;
						}
					}
					
				}
				
				if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
				{
					//���� ���� ����
//					m_plnkVac2->Off(TRUE);
//					m_plnkVac3->Off(TRUE);
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
				{
					//���� ���� ����
//					m_plnkVac1->Off(TRUE);
//					m_plnkVac3->Off(TRUE);
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
			}
		}
	}
	else
	{
		ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
		
		//if ((iResult = m_plnkVac1->On()))
		//{
			//���� ���� ����
		//	WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		//	return iResult;
		//}
		m_plnkVac1->On();
		if ( m_plnkVac2 !=NULL)
		{
			if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
			{
				//���� ���� ����
//				m_plnkVac1->Off();
//@				WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
				return iResult;
			}
		}
		if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
		{
			//���� ���� ����
//			m_plnkVac2->Off();
//@			WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
			return iResult;
		}
	}
#else
	ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
	
	if ((iResult = m_plnkVac1->On()))
	{
		//���� ���� ����
//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	if ( m_plnkVac2 !=NULL)
	{
		if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
		{
			//���� ���� ����
//			m_plnkVac1->Off();
//@			WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
			return iResult;
		}
	}
	if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����
//		m_plnkVac2->Off();
//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
#endif
	/*/
	
	int i = 0;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		if (m_plnkPanelData->m_bUseTHandlerVac[i])
		{
			m_plnkVac[i]->On(TRUE);
		}
	}

	if (FALSE == bSkipSensor)
	{
		for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
		{
			if (m_plnkPanelData->m_bUseTHandlerVac[i])
			{
				iResult = m_plnkVac[i]->On();
				if (iResult)
				{
					for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
					{
						iResult = m_plnkVac[i]->Off();
					}
					return iResult;
				}
			}
		}
	}

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Absorb] [End] ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;	
}

/**
 * Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */

int MHandlerCommon::ReleasePanel(BOOL bSkipSensor)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	int iResult = ERR_HANDLER_SUCCESS;

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Release] [Start] ");
	WriteTactTimeLog(m_szLogMsg);

	/** Vacuum Off�Ǿ� �ִ��� Ȯ�� */
/*	ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL && m_plnkVac3 != NULL);

	if (m_plnkVac2 != NULL)
		m_plnkVac2->Off(TRUE);
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		m_plnkVac3->Off(TRUE);
	}
	iResult = m_plnkVac1->Off(bSkipSensor);
	if (iResult != ERR_HANDLER_SUCCESS)
	{
		// ���� ���� ���� Log 
		WriteErrorLog("Vacuum Release Action Error");
		return iResult;
	}
	iResult = m_plnkVac2->Off(bSkipSensor);
	if (iResult != ERR_HANDLER_SUCCESS)
	{
		// ���� ���� ���� Log 
		WriteErrorLog("Vacuum Release Action Error");
		return iResult;
	}
	
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		iResult = m_plnkVac3->Off(bSkipSensor);
		if (iResult != ERR_HANDLER_SUCCESS)
		{
			// ���� ���� ���� Log 
			WriteErrorLog("Vacuum Release Action Error");
			return iResult;
		}
		
	}
*/

	int i = 0;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		m_plnkVac[i]->Off(TRUE);
	}

	if (FALSE == bSkipSensor)
	{
		for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
		{
			iResult = m_plnkVac[i]->Off();
			if (iResult)
			{
				return iResult;
			}
		}
	}

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Release] [End] ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;
}

/**
 * Vacuum�� Check �Ѵ�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::CheckVacuum()
{
	int iResult = ERR_HANDLER_SUCCESS;

	/*/
#ifdef DEF_USE_UNLOAD_HANDLER
	// kss 16. DEF_DISTANCE_USE_VACUUM2������ ���� ��� Panel Size ������ �ٲ������. 
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		if (m_plnkPanelData->m_dPanelSize_X < DEF_DISTANCE_USE_VACUUM2)
		{
			ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
			
			// Vacuum ���� Check 
			iResult = m_plnkVac1->CompareIO();
			if (iResult != ERR_HANDLER_SUCCESS) 
				return iResult;
			iResult = m_plnkVac2->CompareIO();
			if (iResult != ERR_HANDLER_SUCCESS) 
				return iResult;

		}
		else
		{
			ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL && m_plnkVac3 != NULL);
			
			// Vacuum1 ���� Check 
			iResult = m_plnkVac1->CompareIO();
			if (iResult != ERR_HANDLER_SUCCESS) 
				return iResult;
			
			// Vacuum2 ���� Check 
			iResult = m_plnkVac2->CompareIO();
			if (iResult != ERR_HANDLER_SUCCESS) 
				return iResult;

			// Vacuum3 ���� Check 
			if( m_iHandlerType != DEF_HANDLER_LOADING)
			{
				iResult = m_plnkVac3->CompareIO();
				if (iResult != ERR_HANDLER_SUCCESS) 
					return iResult;
			}
		}
	}
	else
	{
		ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
		
		// Vacuum ���� Check 
		iResult = m_plnkVac1->CompareIO();
		if (iResult != ERR_HANDLER_SUCCESS) 
			return iResult;
		iResult = m_plnkVac2->CompareIO();
		if (iResult != ERR_HANDLER_SUCCESS) 
			return iResult;

	}
#else
	ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
		
	// Vacuum ���� Check 
	iResult = m_plnkVac1->CompareIO();
	if (iResult != ERR_HANDLER_SUCCESS) 
		return iResult;
	iResult = m_plnkVac2->CompareIO();
	if (iResult != ERR_HANDLER_SUCCESS) 
		return iResult;

#endif
	/*/

	int i = 0;

	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		iResult = m_plnkVac[i]->CompareIO();
		if (iResult)
			return iResult;
	}

	return ERR_HANDLER_SUCCESS;
}

int MHandlerCommon::AbsorbPanel(int iVacID, BOOL bSkipSensor)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	ASSERT(iVacID >= 0 && iVacID < DEF_MAX_THANDLER_VAC);

	int iResult = ERR_HANDLER_SUCCESS;

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Absorb] [Start] ");
	WriteTactTimeLog(m_szLogMsg);

	//170121 SJ_YSH Del.
	/*/
	if (iVacID == 0)
		m_plnkIO->OutputOff(m_usOAddrPanelBlow1);
	else
		m_plnkIO->OutputOff(m_usOAddrPanelBlow2);
	/*/

	iResult = m_plnkVac[iVacID]->On(bSkipSensor);
	if (iResult)
	{
		return iResult;
	}

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Absorb] [End] ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;
}

int MHandlerCommon::ReleasePanel(int iVacID, BOOL bSkipSensor)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	ASSERT(iVacID >= 0 && iVacID < DEF_MAX_THANDLER_VAC);

	int iResult = ERR_HANDLER_SUCCESS;

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Release] [Start] ");
	WriteTactTimeLog(m_szLogMsg);

	//170121 SJ_YSH Del.
	/*/
	if (iVacID == 0)
		m_plnkIO->OutputOn(m_usOAddrPanelBlow1);
	else
		m_plnkIO->OutputOn(m_usOAddrPanelBlow2);
	/*/

	iResult = m_plnkVac[iVacID]->Off(bSkipSensor);
	if (iResult)
	{
		//170121 SJ_YSH Del.
		/*/
		if (iVacID == 0)
			m_plnkIO->OutputOff(m_usOAddrPanelBlow1);
		else
			m_plnkIO->OutputOff(m_usOAddrPanelBlow2);
		/*/
		return iResult;
	}
	
	//170121 SJ_YSH Del.
	/*/
	if (iVacID == 0)
		m_plnkIO->OutputOff(m_usOAddrPanelBlow1);
	else
		m_plnkIO->OutputOff(m_usOAddrPanelBlow2);
	/*/

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Release] [End] ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;
}

/***********************************************************************************************
/*
/* Operation State Interface
/*
/************************************************************************************************/

/**********
 * Vacuum
 */

/**
 * Handler�� Vacuum�� ���۵� �������� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = Vac On, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsAbsorbPanel()
{
	/*/
	// kss 16. DEF_DISTANCE_USE_VACUUM2������ ���� ��� Panel Size ������ �ٲ������. 
	if( m_iHandlerType == DEF_HANDLER_UNLOADING)
	{
		ASSERT(m_plnkVac1 && m_plnkVac2 && m_plnkVac3);
		if (m_plnkPanelData->m_dPanelSize_X < DEF_DISTANCE_USE_VACUUM2)
		{
			return m_plnkVac1->IsOn() && m_plnkVac2->IsOn();
		}
		else
		{
			return m_plnkVac1->IsOn() && m_plnkVac2->IsOn() && m_plnkVac3->IsOn();
		}
	}
	else
	{
		ASSERT(m_plnkVac1 && m_plnkVac2);
		return m_plnkVac1->IsOn() && m_plnkVac2->IsOn();
	}
	/*/

	int i=0;
	BOOL bAbsorb = TRUE;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{

		if (m_plnkPanelData->m_bUseTHandlerVac[i])
		{
			bAbsorb &= m_plnkVac[i]->IsOn();
		}
	}
	return bAbsorb;
}

/**
 * Handler�� Vacuum�� ���� ������ �������� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = Vac Off, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsReleasePanel()
{
	/*/
	// kss 16. DEF_DISTANCE_USE_VACUUM2������ ���� ��� Panel Size ������ �ٲ������. 
	// 060824 kjw
	if( m_iHandlerType == DEF_HANDLER_UNLOADING)
	{
		ASSERT(m_plnkVac1 && m_plnkVac2 && m_plnkVac3);
		if (m_plnkPanelData->m_dPanelSize_X < DEF_DISTANCE_USE_VACUUM2)
		{
			return m_plnkVac1->IsOff() && m_plnkVac2->IsOff();
		}
		else
		{
			return m_plnkVac1->IsOff() && m_plnkVac2->IsOff() && m_plnkVac3->IsOff();
		}
	}
	else
	{
		ASSERT(m_plnkVac1 && m_plnkVac2);
		return m_plnkVac1->IsOff() && m_plnkVac2->IsOff();
	}
	/*/
	int i=0;
	BOOL bRelease = TRUE;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		bRelease &= m_plnkVac[i]->IsOff();
	}
	return bRelease;
}

BOOL MHandlerCommon::IsAbsorbPanel(int iVacID)
{
	ASSERT(iVacID >= 0 && iVacID < DEF_MAX_THANDLER_VAC);

	return m_plnkVac[iVacID]->IsOn();
}

BOOL MHandlerCommon::IsReleasePanel(int iVacID)
{
	ASSERT(iVacID >= 0 && iVacID < DEF_MAX_THANDLER_VAC);

	return m_plnkVac[iVacID]->IsOff();
}

BOOL MHandlerCommon::IsAbsorbPanelOneMore()
{
	int i=0;
	BOOL bAbsorb = FALSE;
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		if (m_plnkPanelData->m_bUseTHandlerVac[i])
		{
			bAbsorb |= m_plnkVac[i]->IsOn();
		}
	}
	return bAbsorb;
}

/**********
 * Sensor
 */

/**
 * Handler�� Vacuum ���¸� Ȯ���Ѵ�.
 * Error �� pbState�� FALSE�� �����Ѵ�. 
 *
 * @return	BOOL (TRUE = Detected Ok, FALSE = Detected Fail)
 */
BOOL MHandlerCommon::IsPanelDetected()
{
	ASSERT(m_plnkIO);
	return m_plnkIO->IsOn(m_usiIAddrPanelDetect);
}


/**********
 * Position
 */

/**
 * Handler X,Y,Z,T ���� �������� ���θ� Ȯ���Ѵ�.
 *
 * @return	int (TRUE = Shrink, FALSE = �� ��)
 */
int MHandlerCommon::CheckXYTOrigin()
{
#if FALSE
	if (!m_plnkXAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : X�� Not Origin Return");
		return generateErrorCode(211006);	// ERR_HANDLER_X_AXIS_NOT_ORIGIN
	}


	if (!m_plnkYAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : Y�� Not Origin Return");
		return generateErrorCode(211007);	// ERR_HANDLER_Y_AXIS_NOT_ORIGIN
	}
#endif

	if (!m_plnkTAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : T�� Not Origin Return");
		return generateErrorCode(211067);	// ERR_HANDLER_T_AXIS_NOT_ORIGIN
	}

	return ERR_HANDLER_SUCCESS;
}

// jdy 4. CheckOrigin() ���� XYT ���� ����ϴ��� ǥ���� �ִ���(CheckOriginXYT()), �ƴϸ� Z �� CheckOrigin() ������ �ְ�, ���ÿ� ���� �������� ���θ� ���� ���� ���� ��. (������ XYT �� �������� Z �������� ���δ� Ȯ�� �ؾ��ϹǷ� CheckOrigin() �ȿ� �ִ°� ���� ��.)	
/**
 * HandlerZ ���� �������� ���θ� Ȯ���Ѵ�.
 *
 * @return	int (TRUE = Shrink, FALSE = �� ��)
 */
BOOL MHandlerCommon::CheckZOrigin() 
{

	if (!m_plnkZAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : Z�� Not Origin Return");
		// jdy 11. ���� ���� ��, �߰� �� ������ ���� �޼����� ���� ������ ���ԵǾ�� ��. (ErrorMessage.dat ���� ���� �޼��� ����.)
		return generateErrorCode(211068);	// ERR_HANDLER_Z_AXIS_NOT_ORIGIN
	}
	return ERR_HANDLER_SUCCESS;
}

// kss 11. �ٸ� �࿡ ���� AmpFault/AmpEnable�Լ��� �ʿ���. 
/**
 * X�� Amp Fault ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsXAxisAmpFault()
{
	return m_plnkXAxis->IsAmpFault();
}

/**
 * X�� Amp Enable ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsXAxisAmpEnable()
{
	return m_plnkXAxis->IsAmpEnable();
}

/**
 * Y�� Amp Fault ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsYAxisAmpFault()
{
//@	return m_plnkYAxis->GetAmpFault();
	return TRUE;
}

/**
 * Y�� Amp Enable ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsYAxisAmpEnable()
{
//@	return m_plnkYAxis->GetAmpEnable();
	return TRUE;
}

/**
 * T�� Amp Fault ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsTAxisAmpFault()
{
	return m_plnkTAxis->IsAmpFault();
}

/**
 * X�� Amp Enable ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = �� ��)
 */
BOOL MHandlerCommon::IsTAxisAmpEnable()
{
	return m_plnkTAxis->IsAmpEnable();
}

/**
 * Z�� Amp Fault ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = �� ��)
 */

BOOL MHandlerCommon::IsZAxisAmpFault()
{
	return m_plnkZAxis->IsAmpFault();
}

/**
 * Z�� Amp Enable ���θ� Ȯ���Ѵ�.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = �� ��)
 */

BOOL MHandlerCommon::IsZAxisAmpEnable()
{
	return m_plnkZAxis->IsAmpEnable();
}

/**
 * Handler(X, Y, T��)�� ���� ��ġ�� �д´�.
 *
 * @param	bType		: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
 * @return	MPos_XYT : Handler�� ���� ��ġ (X, Y, T��)
 */
MPos_XYT MHandlerCommon::GetXYTCurrentPos(BOOL bType)
{
//@	ASSERT((m_plnkXAxis != NULL) && (m_plnkYAxis != NULL) && (m_plnkTAxis != NULL));
//@	ASSERT((m_plnkXAxis != NULL) && (m_plnkTAxis != NULL));
	ASSERT(m_plnkTAxis != NULL);
	MPos_XYT sCurPos;

	sCurPos.dT=0.0;

	//sCurPos.dX = m_plnkXAxis->GetCurrentPosition(bType);
	sCurPos.dX = 0.0;
	sCurPos.dY = 0.0;	//@m_plnkYAxis->GetCurrentPosition(bType);
	sCurPos.dT = m_plnkTAxis->GetCurrentPosition(bType);
	return sCurPos;
}

/**
 * Handler(Z��)�� ���� ��ġ�� �д´�.
 *
 * @param	bType		: (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
 * @return	double : Handler�� ���� ��ġ (Z��)
 */
double MHandlerCommon::GetZCurrentPos(BOOL bType)
{
	ASSERT(m_plnkZAxis != NULL);

	double dCurPos = 0.0;

	dCurPos = m_plnkZAxis->GetCurrentPosition(bType);

	return dCurPos;
}

/**
 * Handler(Z��)�� ��ǥ ��ġ�� �д´�.
 *
 * @param	iPosID : Position Index ID
 * @return	double : Handler�� ��ǥ ��ġ (Z��)
 */
double MHandlerCommon::GetZTargetPos(int iPosID)
{

	ASSERT((iPosID >= 0) && (iPosID < DEF_HANDLER_MAX_POS));

	double dTarget = 0.0;

	// jdy 11. �ּ� ó���� �κ� �۾� �ʿ�.
//20060504 ysb
	// ��ǥ��ǥ = ���� ��ǥ	+ Offset ��ǥ
	dTarget = m_rgdZFixedPos[iPosID] + m_rgdZModelPos[iPosID] + m_rgdZOffsetPos[iPosID];
    
	return dTarget;
}

/**
 * Handler(X, Y, T��)�� ������ġ�� Index��ġ�� ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
	// kss 17. ���ʿ� �Ʒ��� �� �ະ�� Ȯ���ϴ� �ڵ尡 ����. 
BOOL MHandlerCommon::IsInXYTPos(int iPosID, MPos_XYT* pTolerance)
{
	MPos_XYT posCompare;
//20060509 YSB
	posCompare = GetXYTTargetPos(iPosID);

	return IsInXYTPos(posCompare, pTolerance);
}

/**
 * Handler(X, Y, T��)�� ������ġ�� ������ġ�� ���Ѵ�.
 *
 * @param	sTargetPos : ������ġ�� ���� ������ġ
 * @param	pTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance)
{
	// jdy 10. ���� ���� 
	BOOL bState[DEF_HANLDER_MAX_AXIS];
	bState[DEF_HANDLER_T_AXIS]=TRUE;
	if (pTolerance != NULL)
	{
		//bState[DEF_HANDLER_X_AXIS] = m_plnkXAxis->ComparePosition(FALSE , sTargetPos.dX,  &pTolerance->dX);
		bState[DEF_HANDLER_X_AXIS] = TRUE;
		bState[DEF_HANDLER_Y_AXIS] = TRUE;	//@m_plnkYAxis->ComparePosition(FALSE , sTargetPos.dY,  &pTolerance->dY);
		bState[DEF_HANDLER_T_AXIS] = m_plnkTAxis->ComparePosition(FALSE , sTargetPos.dT,  &pTolerance->dT);
	}
	else
	{
		//bState[DEF_HANDLER_X_AXIS] = m_plnkXAxis->ComparePosition(FALSE , sTargetPos.dX,  NULL);
		bState[DEF_HANDLER_X_AXIS] = TRUE;
		bState[DEF_HANDLER_Y_AXIS] = TRUE;	//@m_plnkYAxis->ComparePosition(FALSE , sTargetPos.dY,  NULL);
		bState[DEF_HANDLER_T_AXIS] = m_plnkTAxis->ComparePosition(FALSE , sTargetPos.dT,  NULL);
	}

	return bState[DEF_HANDLER_X_AXIS] && bState[DEF_HANDLER_T_AXIS];
}

/**
 * Handler X���� ������ġ�� Index��ġ�� ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL)  �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInXPos(int iPosID, double* pdTolerance)
{
	// jdy 12. MPos_XYT posCompare = GetXYTTargetPos(iPosID); �̷��� �� �ٷ� ǥ���ϴ� ���� �� ����.  ������ �ڵ忡�� ���� ��� ���� �ʿ�.
	MPos_XYT posCompare;
//20060509 YSB
	posCompare = GetXYTTargetPos(iPosID);

	return IsInXPos(posCompare.dX, pdTolerance);
}

/**
 * Handler X���� ������ġ�� ������ġ�� ���Ѵ�.
 *
 * @param	dTargetPos : ������ġ�� ���� ������ġ
 * @param	pdTolerance : �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInXPos(double dTargetPos, double* pdTolerance)
{
	// jdy 13. �Ʒ� �ڵ���� ������ ����. : return m_plnkX->ComparePosition(FALSE , dTargetPos, pdTolerance); ������ �ڵ忡�� ���� ��� ���� �ʿ�.
//	BOOL bState;

	if (pdTolerance != NULL)
	{
		 return m_plnkXAxis->ComparePosition(FALSE , dTargetPos, pdTolerance);
	}
	else
	{
		return m_plnkXAxis->ComparePosition(FALSE , dTargetPos, NULL);
	}

//	return bState;
}


/**
 * Handler Y���� ������ġ�� Index��ġ�� ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */

 /*/
BOOL MHandlerCommon::IsInYPos(int iPosID, double* pdTolerance)
{
	MPos_XYT posCompare;

	posCompare = GetXYTTargetPos(iPosID);

	return IsInYPos(posCompare.dY, pdTolerance);
	return TRUE;
}
/*/

/**
 * Handler Y���� ������ġ�� ������ġ�� ���Ѵ�.
 *
 * @param	dTargetPos : ������ġ�� ���� ������ġ
 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */

/*/
BOOL MHandlerCommon::IsInYPos(double dTargetPos, double* pdTolerance)
{
//	BOOL bState;
	if (pdTolerance != NULL)
	{
		 return m_plnkYAxis->ComparePosition(FALSE , dTargetPos, pdTolerance);
	}
	else
	{
		return m_plnkYAxis->ComparePosition(FALSE , dTargetPos, NULL);
	}
	return TRUE;
}
/*/

/**
 * Handler T���� ������ġ�� Index��ġ�� ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInTPos(int iPosID, double* pdTolerance)
{
	MPos_XYT posCompare;

	posCompare = GetXYTTargetPos(iPosID);

	return IsInTPos(posCompare.dT, pdTolerance);
}

/**
 * Handler T���� ������ġ�� ������ġ�� ���Ѵ�.
 *
 * @param	dTargetPos : ������ġ�� ���� ������ġ
 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInTPos(double dTargetPos, double* pdTolerance)
{
	BOOL bState=TRUE;
	if (pdTolerance != NULL)
	{
		return m_plnkTAxis->ComparePosition(FALSE , dTargetPos, pdTolerance);
	}
	else
	{
		return m_plnkTAxis->ComparePosition(FALSE , dTargetPos,  NULL);
	}
	return bState;
}

/**
 * Handler(Z��)�� ������ġ�� Index��ġ�� ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInZPos(int iPosID, double* pdTolerance)
{
	double dPosCompare;

	dPosCompare = GetZTargetPos(iPosID);

	return IsInZPos(dPosCompare, pdTolerance);
}

/**
 * Handler(Z��)�� ������ġ�� ������ġ�� ���Ѵ�.
 *
 * @param	dTargetPos : ������ġ�� ���� ������ġ
 * @param	pdTolerance : (OPTION=NULL) �� �� ����� ������ (NULL�̸� Motion�� Tolerance ���)
 * @return	BOOL : (TRUE = ��ġ, FALSE = ����ġ)
 */
BOOL MHandlerCommon::IsInZPos(double dTargetPos, double* pdTolerance)
{
//	BOOL bState;

	if (pdTolerance != NULL)
	{
		return m_plnkZAxis->ComparePosition(FALSE , dTargetPos,  pdTolerance);
	}
	else
	{
		return m_plnkZAxis->ComparePosition(FALSE , dTargetPos, NULL);
	}

//	return bState;
}


/**
 *	Handler XYT Motion ������ Ȯ���Ѵ�.
 */
BOOL MHandlerCommon::IsInXYTMotion()
{
#ifdef SIMULATION
	return TRUE;
#endif

	if (//m_plnkXAxis->IsInMotion()
//@		|| m_plnkYAxis->IsInMotion()
		//|| 
		m_plnkTAxis->IsInMotion()
		)
	{
		return TRUE;
	}

	return FALSE;
}

/* Handler�� XYT �� ���� ������ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::LoadXYTFixedPosParameter(void)
{
//	int iResult = ERR_HANDLER_SUCCESS;
	BOOL bResult;

	MTeachingPosData HandlerXYTFixPosDat(m_sXYTTeachingInfo, m_strPosFileName);

	bResult = HandlerXYTFixPosDat.ReadData();
	for (int i = 0; i < DEF_HANDLER_MAX_POS; i++)
	{
		m_rgsXYTFixedPos[i].dX = HandlerXYTFixPosDat.m_TeachingPos[i][DEF_HANDLER_X_AXIS];
		m_rgsXYTFixedPos[i].dY = 0.0;	//@HandlerXYTFixPosDat.m_TeachingPos[i][DEF_HANDLER_Y_AXIS];
		m_rgsXYTFixedPos[i].dT = HandlerXYTFixPosDat.m_TeachingPos[i][DEF_HANDLER_T_AXIS];
	}
	if (!bResult)
	{
		// 210001 = XYT �� ������ǥ�� �о���� ���߽��ϴ�.
//@		WriteErrorLog("LoadXYTFixedPosParameter() : \
//@			           Loading Handler XYT Fixed Pos Parameter Error : Reading Failure");
		return generateErrorCode(211001);	// ERR_Handler_FAIL_IN_LOADING_XYT_FIXPOS_PARAM
	}
	
	return ERR_HANDLER_SUCCESS;
}


/**
 * Handler�� XYT �� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */

int MHandlerCommon::LoadXYTOffsetPosParameter(void)
{
//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.
	BOOL bResult;

	MTeachingPosData HandlerXYTOffsetPosDat(m_sXYTTeachingInfo, 
										  m_strOffsetPosFileName,
										  m_strOffsetPosFilePath);

	// Offset Pos
	bResult = HandlerXYTOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_HANDLER_MAX_POS; i++)
	{
		m_rgsXYTOffsetPos[i].dX = HandlerXYTOffsetPosDat.m_TeachingPos[i][DEF_HANDLER_X_AXIS];
		m_rgsXYTOffsetPos[i].dY = 0.0;	//@HandlerXYTOffsetPosDat.m_TeachingPos[i][DEF_HANDLER_Y_AXIS];
		m_rgsXYTOffsetPos[i].dT = HandlerXYTOffsetPosDat.m_TeachingPos[i][DEF_HANDLER_T_AXIS];
	}

	if (!bResult)
	{
		// 210003 = XYT �� Offset ��ǥ�� �о���� ���߽��ϴ�.
//@		WriteErrorLog("LoadXYTOffsetPosParameter() : \
//@					   Loading Handler XYT Offset Pos Parameter Error : Reading Failure");
		return generateErrorCode(211003);	// ERR_Handler_FAIL_IN_LOADING_XYT_OFFSETPOS_PARAM
	}
	
	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� Z �� ���� ������ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::LoadZFixedPosParameter(void)
{
//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.
	BOOL bResult;
	CString strLogMsg;

	MTeachingPosData HandlerZFixedPosDat(m_sZTeachingInfo,m_strPosFileName);
	
	// Fixed Pos 
	bResult = HandlerZFixedPosDat.ReadData();
	
	for (int i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		m_rgdZFixedPos[i] = HandlerZFixedPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
	{
		// 210005 = Z�� �� ������ǥ�� �о���� ���߽��ϴ�.
//@		WriteErrorLog("LoadZFixedPosParameter() : \
//@			           Loading Handler Z Fixed Pos Parameter Error : Reading Failure");
		return generateErrorCode(210005);	// ERR_Handler_FAIL_IN_LOADING_Z_FIXPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� Z �� ���� Offset��ǥ Data�� File���� �о�´�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::LoadZOffsetPosParameter(void)
{
	MTeachingPosData HandlerZOffsetPosDat(m_sZTeachingInfo,
										m_strOffsetPosFileName, 
										m_strOffsetPosFilePath);
	
	// Offset Pos 
	BOOL bResult = HandlerZOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		m_rgdZOffsetPos[i] = HandlerZOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
	{
		// 210207 = Z�� �� Offset ��ǥ�� �о���� ���߽��ϴ�.
//@		WriteErrorLog("LoadZOffsetPosParameter() : \
//@			           Loading Handler Z Offset Pos Parameter Error : Reading Failure");
		return generateErrorCode(210207);
	}
	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� XYT �� ���� ������ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsFixedPos : ������ ������ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.

	int i;
	BOOL bResult;
	
	MTeachingPosData HandlerXYTFixPosDat(m_sXYTTeachingInfo, m_strPosFileName);
	
	for (i = 0; i < DEF_HANDLER_MAX_POS; i++)
	{
		//m_rgsXYTFixedPos[i].dX = rgsFixedPos[i].dX;
		m_rgsXYTFixedPos[i].dX = 0.0;
		m_rgsXYTFixedPos[i].dY = 0.0;	//@rgsFixedPos[i].dY; 
		m_rgsXYTFixedPos[i].dT = rgsFixedPos[i].dT;
		
		HandlerXYTFixPosDat.m_TeachingPos[i][DEF_HANDLER_X_AXIS] = m_rgsXYTFixedPos[i].dX;
		HandlerXYTFixPosDat.m_TeachingPos[i][DEF_HANDLER_Y_AXIS] = m_rgsXYTFixedPos[i].dY;
		HandlerXYTFixPosDat.m_TeachingPos[i][DEF_HANDLER_T_AXIS] = m_rgsXYTFixedPos[i].dT;
	}

	bResult = HandlerXYTFixPosDat.WriteData();
	
	if (!bResult)
	{
		// 210002 = XYT �� ������ǥ�� File �� ����� �� �����ϴ�.
//@		WriteErrorLog("SaveXYTFixedPosParameter() : \
//@			           Saving Handler XYT Fixed Pos Parameter Error : Writing Failure");
		return generateErrorCode(211002);	// ERR_Handler_FAIL_IN_SAVING_XYT_FIXPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;

}


/**
 * Handler�� XYT �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgsOffsetPos : ������ Offset��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.
	int i;
	BOOL bResult;

	MTeachingPosData HandlerXYTOffsetPosDat(m_sXYTTeachingInfo, 
										  m_strOffsetPosFileName, 
										  m_strOffsetPosFilePath);

	// Offset Pos 
	for (i = 0; i < DEF_HANDLER_MAX_POS; i++)
	{
		//m_rgsXYTOffsetPos[i].dX = rgsOffsetPos[i].dX;
		m_rgsXYTOffsetPos[i].dX = 0.0;
		m_rgsXYTOffsetPos[i].dY = 0.0;	//@rgsOffsetPos[i].dY;
		m_rgsXYTOffsetPos[i].dT = rgsOffsetPos[i].dT;

		HandlerXYTOffsetPosDat.m_TeachingPos[i][DEF_HANDLER_X_AXIS] = m_rgsXYTOffsetPos[i].dX;
		HandlerXYTOffsetPosDat.m_TeachingPos[i][DEF_HANDLER_Y_AXIS] = m_rgsXYTOffsetPos[i].dY;
		HandlerXYTOffsetPosDat.m_TeachingPos[i][DEF_HANDLER_T_AXIS] = m_rgsXYTOffsetPos[i].dT;
	}

	bResult = HandlerXYTOffsetPosDat.WriteData();

	if (!bResult)
	{
		// 210004 = XYT �� Offset ��ǥ�� File �� ����� �� �����ϴ�.
//@		WriteErrorLog("SaveXYTOffsetPosParameter() : \
//@					   Saving Handler XYT Offset Pos Parameter Error : Writing Failure");
		return generateErrorCode(211004);	// ERR_Handler_FAIL_IN_SAVING_XYT_OFFSETPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}



/**
 * Handler�� Z �� ���� ������ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgdFixedPos : ������ ������ǥ �迭 (Z)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::SaveZFixedPosParameter(double* rgdFixedPos)
{
	ASSERT(rgdFixedPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.
	int i;
	BOOL bResult;
	
	MTeachingPosData HandlerZFixPosDat(m_sZTeachingInfo, m_strPosFileName);
	
	for (i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		m_rgdZFixedPos[i] = rgdFixedPos[i];

		HandlerZFixPosDat.m_TeachingPos[i][0] = m_rgdZFixedPos[i];
	}

	bResult = HandlerZFixPosDat.WriteData();
	if (!bResult)
	{
		// 210006 = Z�� �� ������ǥ�� File �� ����� �� �����ϴ�.
//@		WriteErrorLog("SaveZFixedPosParameter() : \
//@			           Saving Handler Z Fixed Pos Parameter Error : Writing Failure");
		return generateErrorCode(210006);	// ERR_Handler_FAIL_IN_SAVING_Z_FIXPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}


/**
 * Handler�� Z �� ���� Offset��ǥ Data�� File�� �����Ѵ�.
 *
 * @param	rgdOffsetPos : ������ Offset��ǥ �迭 (Z)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::SaveZOffsetPosParameter(double* rgdOffsetPos)
{
	ASSERT(rgdOffsetPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.
	int i;
	BOOL bResult;

	MTeachingPosData HandlerZOffsetPosDat(m_sZTeachingInfo, 
										m_strOffsetPosFileName, 
										m_strOffsetPosFilePath);

	// Offset Pos 
	for (i = 0; i < DEF_HANDLER_MAX_Z_POS ; i++)
	{
		m_rgdZOffsetPos[i] = rgdOffsetPos[i];

		HandlerZOffsetPosDat.m_TeachingPos[i][0] = m_rgdZOffsetPos[i];	
	}
	bResult = HandlerZOffsetPosDat.WriteData();
	if (!bResult)
	{
		// 210008 = Z�� �� Offset ��ǥ�� File �� ����� �� �����ϴ�.
//@		WriteErrorLog("SaveZOffsetPosParameter() : \
//@			           Saving Handler Z Offset Pos Parameter Error : Writing Failure");
		return generateErrorCode(210008);	// ERR_Handler_FAIL_IN_SAVING_Z_OFFSETPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� XYT �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgsFixedPos : ������ǥ �迭 (X, Y, T)
 * @param	rgsOffsetPos : Offset��ǥ �迭 (X, Y, T)
 * @param	rgsModelPos : Model��ǥ �迭 (X, Y, T)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::GetXYTPosParameter(MPos_XYT* rgsFixedPos,
									   MPos_XYT* rgsOffsetPos,
									   MPos_XYT* rgsModelPos)
{
	for (int i = 0; i < DEF_HANDLER_MAX_POS; i++)
	{
		if (rgsFixedPos != NULL)
			rgsFixedPos[i]	= m_rgsXYTFixedPos[i];
		if (rgsOffsetPos != NULL)
			rgsOffsetPos[i]	= m_rgsXYTOffsetPos[i];
		if (rgsModelPos != NULL)
			rgsModelPos[i]	= m_rgsXYTModelPos[i];
		
	}
	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� XYT �� ��ǥ ���� Model Data�� ��ȯ�Ѵ�. 
 *
 * @param	iPos = Position ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
MPos_XYT MHandlerCommon::GetXYTModelPosParameter(int iPos)
{
	ASSERT(iPos < DEF_HANDLER_MAX_POS);
	return m_rgsXYTModelPos[iPos];
}
	
/**
 * Handler�� XYT �� ��ǥ ���� Offset Data�� ��ȯ�Ѵ�. 
 *
 * @param	iPos = Position ID
 * @return	int : (0 = Success, Error Code = �� ��)
 */
MPos_XYT MHandlerCommon::GetXYTOffsetPosParameter(int iPos) 
{
	ASSERT(iPos < DEF_HANDLER_MAX_POS);
	return m_rgsXYTOffsetPos[iPos];
}
	


/**
 * Handler�� Z �� ��ǥ ���� ��� Data�� ��ȯ �Ѵ�.
 *
 * @param	rgdFixedPos : ������ǥ �迭 (Z)
 * @param	rgdOffsetPos : Offset��ǥ �迭 (Z)
 * @param	rgdModelPos : Model��ǥ �迭 (Z)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::GetZPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos)
{
	// ASSERT((rgdFixedPos != NULL) && (rgdOffsetPos != NULL));

//	int iResult = ERR_HANDLER_SUCCESS; // jdy 14. ������� �ʴ� ������. �� �Ʒ� return ERR_HANDLER_SUCCESS; �� �����ؾ���.

	for (int i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		if (rgdFixedPos != NULL)	rgdFixedPos[i]  = m_rgdZFixedPos[i];		
		if (rgdOffsetPos != NULL)	rgdOffsetPos[i] = m_rgdZOffsetPos[i];		
		if (rgdModelPos != NULL)	rgdModelPos[i]	= m_rgdZModelPos[i];
	}

	return ERR_HANDLER_SUCCESS;
}
	

/**
 * Handler Z Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MHandlerCommon::GetZTeachingInfo()
{
	return m_sZTeachingInfo;
}

/**
 * Handler XYT Teaching ������ �д´�.
 *
 * @return	STeachingInfo : Teaching ����
 */
STeachingInfo MHandlerCommon::GetXYTTeachingInfo()
{
	return m_sXYTTeachingInfo;
}

/**
 * Handler(X,Y,T��)�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	bUseAuto : Manual Mode�� ��� �� ������ ���� ������ Ȯ�� (FLASE : Manual Mode, FALSE : Auto Mode)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::SafeMoveXYTPos(int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	MPos_XYT posTarget;	

	// ��ǥ ��ġ �б�
	posTarget = GetXYTTargetPos(iPosID);

	// �ش� ��ġ�� �̵�
	return MoveXYTPos(posTarget, iPosID, bUseAuto);
}

int MHandlerCommon::SafeMoveXPos(int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	double dTarget;	

	// ��ǥ ��ġ �б�
	dTarget = GetXYTTargetPos(iPosID).dX;

	// �ش� ��ġ�� �̵�
	return MoveXPos(dTarget, iPosID, bUseAuto);
}

/**
 * Handler(Z��)�� Index��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	iPosID : Position Index ID
 * @param	bUseAuto : Manual Mode�� ��� �� ������ ���� ������ Ȯ�� (FLASE : Manual Mode, TRUE : Auto Mode)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandlerCommon::SafeMoveZPos(int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	// ��ǥ ��ġ �б�
	double dTarget = GetZTargetPos(iPosID);

	// �ش� ��ġ�� �̵�
	return MoveZPos(dTarget, iPosID, bUseAuto);
}

/*/
int MHandlerCommon::VacuumOn1(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac1->On(TRUE);
		
	if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����

//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	return ERR_HANDLER_SUCCESS;
}

int MHandlerCommon::VacuumOn2(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac2->Off(TRUE);
		
	if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����

//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	return ERR_HANDLER_SUCCESS;
}
int MHandlerCommon::VacuumOn3(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac3->On(TRUE);
		
	if ((iResult = m_plnkVac3->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����

//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	return ERR_HANDLER_SUCCESS;
}

int MHandlerCommon::VacuumOff1(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac1->Off(TRUE);
		
	if ((iResult = m_plnkVac1->Off(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����

//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	return ERR_HANDLER_SUCCESS;
}

int MHandlerCommon::VacuumOff2(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac2->On(TRUE);
		
	if ((iResult = m_plnkVac2->Off(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����

//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	return ERR_HANDLER_SUCCESS;
}

int MHandlerCommon::VacuumOff3(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac3->Off(TRUE);
		
	if ((iResult = m_plnkVac3->Off(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//���� ���� ����
//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	return ERR_HANDLER_SUCCESS;
}

BOOL MHandlerCommon::IsVacOn1(void)
{
	ASSERT(m_plnkVac1 != NULL);
	return m_plnkVac1->IsOn();
}

BOOL MHandlerCommon::IsVacOn2(void)
{
	ASSERT(m_plnkVac2 != NULL);
	return m_plnkVac2->IsOn();
}

BOOL MHandlerCommon::IsVacOn3(void)
{
	ASSERT(m_plnkVac3 != NULL);
	return m_plnkVac3->IsOn();
}

BOOL MHandlerCommon::IsVacOff1(void)
{
	ASSERT(m_plnkVac1 != NULL);
	return m_plnkVac1->IsOff();
}

BOOL MHandlerCommon::IsVacOff2(void)
{
	ASSERT(m_plnkVac2 != NULL);
	return m_plnkVac2->IsOff();
}

BOOL MHandlerCommon::IsVacOff3(void)
{
	ASSERT(m_plnkVac3 != NULL);
	return m_plnkVac3->IsOff();
}
/*/
// End Of Code
