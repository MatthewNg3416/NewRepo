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
/* 생성자 & 소멸자
/*
/************************************************************************************************/

/** 
 * Constructor
 * 
 * @param	SCommonAttribute commonData (공통적으로 사용할 Data)
 * @param	SHandlerRefCompList listRefComponent (참조할 Component 포인터 리스트)
 */
MHandlerCommon::MHandlerCommon(SCommonAttribute commonData, SHandlerRefCompList listRefComponent)
: ICommon(commonData)
{
	// Hanlder Vacuum 1,2 를 모두 사용하지 않는 경우를 대비해서 Null 초기화
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
		/** 좌표계 초기화 */
		m_rgsXYTFixedPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTOffsetPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTModelPos[i].Init(0.0, 0.0, 0.0);
		// kss 5. 불필요
	}

	for (i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		/** 좌표계 초기화 */
		m_rgdZFixedPos[i]						= 0.0;	// kss 14 이름 변경 필요 Y -> Z
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
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

/**
 * Handler Component에서 참조할 Component List를 설정한다.
 *
 * @param	SHandlerRefCompList listRefComponets (설정하고자 하는 Component 참조 List)
 * @return	int : (0 = Success, Error Code = 그 외)
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

	// jdy 7. 파일 별로 주석 정리 필요 (Stage => Handler 등)
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
 * Handler의 Motion Component를 돌려 받는다.
 * @param nAxisID : 축 ID (0: X축, 1:Y축 , 2:T축, 3:Z축)
 * @return	IAixs : 해당하는 축 ID에 맞는 Axis 
 */
IAxis*	MHandlerCommon::GetMotionComponent(int nAxisID)
{
	// jdy 9. 들여쓰기 정리, 아래 주석 정리 (STU 등 잘못된 주석)
		// Motion Component 축 ID에 따라
	switch (nAxisID)
	{
		case DEF_HANDLER_X_AXIS :		// Handler X축 
			return m_plnkXAxis;

		case DEF_HANDLER_Y_AXIS :		// Handler Y축
			return m_plnkYAxis;

		case DEF_HANDLER_T_AXIS :		// Handler T축
			return m_plnkTAxis;

		case DEF_HANDLER_Z_AXIS :		// Handler Z축
			return m_plnkZAxis;

		default :					// 축 ID 오류
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
 * Vacuum을 흡착한다.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::AbsorbPanel(BOOL bSkipSensor)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif

	int iResult = ERR_HANDLER_SUCCESS;

	// kss 15. Log 형식에 맞게 변경 필요 - StepDisplay 로 설정 안함. 로그 메세지 형식 수정 필요.
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Absorb] [Start] ");
	WriteTactTimeLog(m_szLogMsg);
	
	/*/
#ifdef DEF_USE_UNLOAD_HANDLER
	/ ** Vacuum Off되어 있는지 확인 * /
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		if (!IsAbsorbPanel() || bSkipSensor)
		{
			if (m_plnkPanelData->m_dPanelSize_X < DEF_DISTANCE_USE_VACUUM2)
			{
				ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
				
				if ((iResult = m_plnkVac1->On(TRUE)) != ERR_HANDLER_SUCCESS)
				{
					//흡착 동작 오류
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				if ( m_plnkVac2 !=NULL)
				{
					if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
					{
						//흡착 동작 오류
//						m_plnkVac1->On();
//@						WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
						return iResult;
					}
				}
				if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
				{
					//흡착 동작 오류
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
					//흡착 동작 오류
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				
				if (!m_plnkVac2->IsOn())
				{
					if ((iResult = m_plnkVac2->On(TRUE)) != ERR_HANDLER_SUCCESS)
					{
						//흡착 동작 오류
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
							//흡착 동작 오류
	//						m_plnkVac2->Off(TRUE);
//@							WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
							return iResult;
						}
					}
					
				}
				
				if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
				{
					//흡착 동작 오류
//					m_plnkVac2->Off(TRUE);
//					m_plnkVac3->Off(TRUE);
//@					WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
					return iResult;
				}
				if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
				{
					//흡착 동작 오류
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
			//흡착 동작 오류
		//	WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		//	return iResult;
		//}
		m_plnkVac1->On();
		if ( m_plnkVac2 !=NULL)
		{
			if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
			{
				//흡착 동작 오류
//				m_plnkVac1->Off();
//@				WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
				return iResult;
			}
		}
		if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
		{
			//흡착 동작 오류
//			m_plnkVac2->Off();
//@			WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
			return iResult;
		}
	}
#else
	ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
	
	if ((iResult = m_plnkVac1->On()))
	{
		//흡착 동작 오류
//@		WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
		return iResult;
	}
	if ( m_plnkVac2 !=NULL)
	{
		if ((iResult = m_plnkVac2->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
		{
			//흡착 동작 오류
//			m_plnkVac1->Off();
//@			WriteErrorLog("AbsorbPanel() : Vacuum Absorb action error");
			return iResult;
		}
	}
	if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//흡착 동작 오류
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

	// kss 15. Log 형식에 맞게 변경 필요 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Absorb] [End] ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;	
}

/**
 * Vacuum을 해제한다.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
 * @return	int : (0 = Success, Error Code = 그 외)
 */

int MHandlerCommon::ReleasePanel(BOOL bSkipSensor)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	int iResult = ERR_HANDLER_SUCCESS;

	// kss 15. Log 형식에 맞게 변경 필요 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Release] [Start] ");
	WriteTactTimeLog(m_szLogMsg);

	/** Vacuum Off되어 있는지 확인 */
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
		// 흡착 동작 오류 Log 
		WriteErrorLog("Vacuum Release Action Error");
		return iResult;
	}
	iResult = m_plnkVac2->Off(bSkipSensor);
	if (iResult != ERR_HANDLER_SUCCESS)
	{
		// 흡착 동작 오류 Log 
		WriteErrorLog("Vacuum Release Action Error");
		return iResult;
	}
	
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		iResult = m_plnkVac3->Off(bSkipSensor);
		if (iResult != ERR_HANDLER_SUCCESS)
		{
			// 흡착 동작 오류 Log 
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

	// kss 15. Log 형식에 맞게 변경 필요 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [MHandler] [M] [Panel Release] [End] ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;
}

/**
 * Vacuum을 Check 한다.
 *
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::CheckVacuum()
{
	int iResult = ERR_HANDLER_SUCCESS;

	/*/
#ifdef DEF_USE_UNLOAD_HANDLER
	// kss 16. DEF_DISTANCE_USE_VACUUM2변수를 흡착 사용 Panel Size 변수로 바꿔줘야함. 
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		if (m_plnkPanelData->m_dPanelSize_X < DEF_DISTANCE_USE_VACUUM2)
		{
			ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
			
			// Vacuum 상태 Check 
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
			
			// Vacuum1 상태 Check 
			iResult = m_plnkVac1->CompareIO();
			if (iResult != ERR_HANDLER_SUCCESS) 
				return iResult;
			
			// Vacuum2 상태 Check 
			iResult = m_plnkVac2->CompareIO();
			if (iResult != ERR_HANDLER_SUCCESS) 
				return iResult;

			// Vacuum3 상태 Check 
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
		
		// Vacuum 상태 Check 
		iResult = m_plnkVac1->CompareIO();
		if (iResult != ERR_HANDLER_SUCCESS) 
			return iResult;
		iResult = m_plnkVac2->CompareIO();
		if (iResult != ERR_HANDLER_SUCCESS) 
			return iResult;

	}
#else
	ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL);
		
	// Vacuum 상태 Check 
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

	// kss 15. Log 형식에 맞게 변경 필요 
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

	// kss 15. Log 형식에 맞게 변경 필요 
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

	// kss 15. Log 형식에 맞게 변경 필요 
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

	// kss 15. Log 형식에 맞게 변경 필요 
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
 * Handler의 Vacuum이 동작된 상태인지 확인한다.
 *
 * @return	BOOL (TRUE = Vac On, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsAbsorbPanel()
{
	/*/
	// kss 16. DEF_DISTANCE_USE_VACUUM2변수를 흡착 사용 Panel Size 변수로 바꿔줘야함. 
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
 * Handler의 Vacuum이 동작 해제된 상태인지 확인한다.
 *
 * @return	BOOL (TRUE = Vac Off, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsReleasePanel()
{
	/*/
	// kss 16. DEF_DISTANCE_USE_VACUUM2변수를 흡착 사용 Panel Size 변수로 바꿔줘야함. 
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
 * Handler의 Vacuum 상태를 확인한다.
 * Error 시 pbState는 FALSE로 설정한다. 
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
 * Handler X,Y,Z,T 축의 원점복귀 여부를 확인한다.
 *
 * @return	int (TRUE = Shrink, FALSE = 그 외)
 */
int MHandlerCommon::CheckXYTOrigin()
{
#if FALSE
	if (!m_plnkXAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : X축 Not Origin Return");
		return generateErrorCode(211006);	// ERR_HANDLER_X_AXIS_NOT_ORIGIN
	}


	if (!m_plnkYAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : Y축 Not Origin Return");
		return generateErrorCode(211007);	// ERR_HANDLER_Y_AXIS_NOT_ORIGIN
	}
#endif

	if (!m_plnkTAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : T축 Not Origin Return");
		return generateErrorCode(211067);	// ERR_HANDLER_T_AXIS_NOT_ORIGIN
	}

	return ERR_HANDLER_SUCCESS;
}

// jdy 4. CheckOrigin() 에서 XYT 축을 담당하는지 표시해 주던가(CheckOriginXYT()), 아니면 Z 도 CheckOrigin() 안으로 넣고, 선택에 따라 원점복귀 여부를 보는 것이 좋을 듯. (어차피 XYT 만 움직여도 Z 원점복귀 여부는 확인 해야하므로 CheckOrigin() 안에 넣는게 좋을 듯.)	
/**
 * HandlerZ 축의 원점복귀 여부를 확인한다.
 *
 * @return	int (TRUE = Shrink, FALSE = 그 외)
 */
BOOL MHandlerCommon::CheckZOrigin() 
{

	if (!m_plnkZAxis->IsOriginReturn())
	{
//@		WriteErrorLog("CheckOrigin() : Z축 Not Origin Return");
		// jdy 11. 다음 배포 시, 추가 및 수정된 에러 메세지에 대한 배포도 포함되어야 함. (ErrorMessage.dat 영문 에러 메세지 포함.)
		return generateErrorCode(211068);	// ERR_HANDLER_Z_AXIS_NOT_ORIGIN
	}
	return ERR_HANDLER_SUCCESS;
}

// kss 11. 다른 축에 대한 AmpFault/AmpEnable함수도 필요함. 
/**
 * X축 Amp Fault 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsXAxisAmpFault()
{
	return m_plnkXAxis->IsAmpFault();
}

/**
 * X축 Amp Enable 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsXAxisAmpEnable()
{
	return m_plnkXAxis->IsAmpEnable();
}

/**
 * Y축 Amp Fault 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsYAxisAmpFault()
{
//@	return m_plnkYAxis->GetAmpFault();
	return TRUE;
}

/**
 * Y축 Amp Enable 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsYAxisAmpEnable()
{
//@	return m_plnkYAxis->GetAmpEnable();
	return TRUE;
}

/**
 * T축 Amp Fault 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsTAxisAmpFault()
{
	return m_plnkTAxis->IsAmpFault();
}

/**
 * X축 Amp Enable 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = 그 외)
 */
BOOL MHandlerCommon::IsTAxisAmpEnable()
{
	return m_plnkTAxis->IsAmpEnable();
}

/**
 * Z축 Amp Fault 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP FAULT, FALSE = 그 외)
 */

BOOL MHandlerCommon::IsZAxisAmpFault()
{
	return m_plnkZAxis->IsAmpFault();
}

/**
 * Z축 Amp Enable 여부를 확인한다.
 *
 * @return	BOOL (TRUE = AMP Enable, FALSE = 그 외)
 */

BOOL MHandlerCommon::IsZAxisAmpEnable()
{
	return m_plnkZAxis->IsAmpEnable();
}

/**
 * Handler(X, Y, T축)의 현재 위치를 읽는다.
 *
 * @param	bType		: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
 * @return	MPos_XYT : Handler의 현재 위치 (X, Y, T축)
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
 * Handler(Z축)의 현재 위치를 읽는다.
 *
 * @param	bType		: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
 * @return	double : Handler의 현재 위치 (Z축)
 */
double MHandlerCommon::GetZCurrentPos(BOOL bType)
{
	ASSERT(m_plnkZAxis != NULL);

	double dCurPos = 0.0;

	dCurPos = m_plnkZAxis->GetCurrentPosition(bType);

	return dCurPos;
}

/**
 * Handler(Z축)의 목표 위치를 읽는다.
 *
 * @param	iPosID : Position Index ID
 * @return	double : Handler의 목표 위치 (Z축)
 */
double MHandlerCommon::GetZTargetPos(int iPosID)
{

	ASSERT((iPosID >= 0) && (iPosID < DEF_HANDLER_MAX_POS));

	double dTarget = 0.0;

	// jdy 11. 주석 처리된 부분 작업 필요.
//20060504 ysb
	// 목표좌표 = 고정 좌표	+ Offset 좌표
	dTarget = m_rgdZFixedPos[iPosID] + m_rgdZModelPos[iPosID] + m_rgdZOffsetPos[iPosID];
    
	return dTarget;
}

/**
 * Handler(X, Y, T축)의 현재위치와 Index위치를 비교한다.
 *
 * @param	iPosID : Position Index ID
 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
 */
	// kss 17. 불필요 아래에 각 축별로 확인하는 코드가 있음. 
BOOL MHandlerCommon::IsInXYTPos(int iPosID, MPos_XYT* pTolerance)
{
	MPos_XYT posCompare;
//20060509 YSB
	posCompare = GetXYTTargetPos(iPosID);

	return IsInXYTPos(posCompare, pTolerance);
}

/**
 * Handler(X, Y, T축)의 현재위치와 전달위치를 비교한다.
 *
 * @param	sTargetPos : 현재위치와 비교할 전달위치
 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
 */
BOOL MHandlerCommon::IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance)
{
	// jdy 10. 여백 정리 
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
 * Handler X축의 현재위치와 Index위치를 비교한다.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL)  비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
 */
BOOL MHandlerCommon::IsInXPos(int iPosID, double* pdTolerance)
{
	// jdy 12. MPos_XYT posCompare = GetXYTTargetPos(iPosID); 이렇게 한 줄로 표시하는 것이 더 좋음.  유사한 코드에서 같은 방식 적용 필요.
	MPos_XYT posCompare;
//20060509 YSB
	posCompare = GetXYTTargetPos(iPosID);

	return IsInXPos(posCompare.dX, pdTolerance);
}

/**
 * Handler X축의 현재위치와 전달위치를 비교한다.
 *
 * @param	dTargetPos : 현재위치와 비교할 전달위치
 * @param	pdTolerance : 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
 */
BOOL MHandlerCommon::IsInXPos(double dTargetPos, double* pdTolerance)
{
	// jdy 13. 아래 코드들은 다음과 같다. : return m_plnkX->ComparePosition(FALSE , dTargetPos, pdTolerance); 유사한 코드에서 같은 방식 적용 필요.
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
 * Handler Y축의 현재위치와 Index위치를 비교한다.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
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
 * Handler Y축의 현재위치와 전달위치를 비교한다.
 *
 * @param	dTargetPos : 현재위치와 비교할 전달위치
 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
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
 * Handler T축의 현재위치와 Index위치를 비교한다.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
 */
BOOL MHandlerCommon::IsInTPos(int iPosID, double* pdTolerance)
{
	MPos_XYT posCompare;

	posCompare = GetXYTTargetPos(iPosID);

	return IsInTPos(posCompare.dT, pdTolerance);
}

/**
 * Handler T축의 현재위치와 전달위치를 비교한다.
 *
 * @param	dTargetPos : 현재위치와 비교할 전달위치
 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
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
 * Handler(Z축)의 현재위치와 Index위치를 비교한다.
 *
 * @param	iPosID : Position Index ID
 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
 */
BOOL MHandlerCommon::IsInZPos(int iPosID, double* pdTolerance)
{
	double dPosCompare;

	dPosCompare = GetZTargetPos(iPosID);

	return IsInZPos(dPosCompare, pdTolerance);
}

/**
 * Handler(Z축)의 현재위치와 전달위치를 비교한다.
 *
 * @param	dTargetPos : 현재위치와 비교할 전달위치
 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
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
 *	Handler XYT Motion 중인지 확인한다.
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

/* Handler의 XYT 축 관련 고정좌표 Data를 File에서 읽어온다.
 *
 * @return	int : (0 = Success, Error Code = 그 외)
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
		// 210001 = XYT 축 고정좌표를 읽어오지 못했습니다.
//@		WriteErrorLog("LoadXYTFixedPosParameter() : \
//@			           Loading Handler XYT Fixed Pos Parameter Error : Reading Failure");
		return generateErrorCode(211001);	// ERR_Handler_FAIL_IN_LOADING_XYT_FIXPOS_PARAM
	}
	
	return ERR_HANDLER_SUCCESS;
}


/**
 * Handler의 XYT 축 관련 Offset좌표 Data를 File에서 읽어온다.
 *
 * @return	int : (0 = Success, Error Code = 그 외)
 */

int MHandlerCommon::LoadXYTOffsetPosParameter(void)
{
//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.
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
		// 210003 = XYT 축 Offset 좌표를 읽어오지 못했습니다.
//@		WriteErrorLog("LoadXYTOffsetPosParameter() : \
//@					   Loading Handler XYT Offset Pos Parameter Error : Reading Failure");
		return generateErrorCode(211003);	// ERR_Handler_FAIL_IN_LOADING_XYT_OFFSETPOS_PARAM
	}
	
	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler의 Z 축 관련 고정좌표 Data를 File에서 읽어온다.
 *
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::LoadZFixedPosParameter(void)
{
//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.
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
		// 210005 = Z축 축 고정좌표를 읽어오지 못했습니다.
//@		WriteErrorLog("LoadZFixedPosParameter() : \
//@			           Loading Handler Z Fixed Pos Parameter Error : Reading Failure");
		return generateErrorCode(210005);	// ERR_Handler_FAIL_IN_LOADING_Z_FIXPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler의 Z 축 관련 Offset좌표 Data를 File에서 읽어온다.
 *
 * @return	int : (0 = Success, Error Code = 그 외)
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
		// 210207 = Z축 축 Offset 좌표를 읽어오지 못했습니다.
//@		WriteErrorLog("LoadZOffsetPosParameter() : \
//@			           Loading Handler Z Offset Pos Parameter Error : Reading Failure");
		return generateErrorCode(210207);
	}
	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler의 XYT 축 관련 고정좌표 Data를 File에 저장한다.
 *
 * @param	rgsFixedPos : 저장할 고정좌표 배열 (X, Y, T)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.

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
		// 210002 = XYT 축 고정좌표를 File 에 기록할 수 없습니다.
//@		WriteErrorLog("SaveXYTFixedPosParameter() : \
//@			           Saving Handler XYT Fixed Pos Parameter Error : Writing Failure");
		return generateErrorCode(211002);	// ERR_Handler_FAIL_IN_SAVING_XYT_FIXPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;

}


/**
 * Handler의 XYT 축 관련 Offset좌표 Data를 File에 저장한다.
 *
 * @param	rgsOffsetPos : 저장할 Offset좌표 배열 (X, Y, T)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.
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
		// 210004 = XYT 축 Offset 좌표를 File 에 기록할 수 없습니다.
//@		WriteErrorLog("SaveXYTOffsetPosParameter() : \
//@					   Saving Handler XYT Offset Pos Parameter Error : Writing Failure");
		return generateErrorCode(211004);	// ERR_Handler_FAIL_IN_SAVING_XYT_OFFSETPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}



/**
 * Handler의 Z 축 관련 고정좌표 Data를 File에 저장한다.
 *
 * @param	rgdFixedPos : 저장할 고정좌표 배열 (Z)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::SaveZFixedPosParameter(double* rgdFixedPos)
{
	ASSERT(rgdFixedPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.
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
		// 210006 = Z축 축 고정좌표를 File 에 기록할 수 없습니다.
//@		WriteErrorLog("SaveZFixedPosParameter() : \
//@			           Saving Handler Z Fixed Pos Parameter Error : Writing Failure");
		return generateErrorCode(210006);	// ERR_Handler_FAIL_IN_SAVING_Z_FIXPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}


/**
 * Handler의 Z 축 관련 Offset좌표 Data를 File에 저장한다.
 *
 * @param	rgdOffsetPos : 저장할 Offset좌표 배열 (Z)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::SaveZOffsetPosParameter(double* rgdOffsetPos)
{
	ASSERT(rgdOffsetPos != NULL);

//	int iResult = ERR_HANDLER_SUCCESS;	// jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.
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
		// 210008 = Z축 축 Offset 좌표를 File 에 기록할 수 없습니다.
//@		WriteErrorLog("SaveZOffsetPosParameter() : \
//@			           Saving Handler Z Offset Pos Parameter Error : Writing Failure");
		return generateErrorCode(210008);	// ERR_Handler_FAIL_IN_SAVING_Z_OFFSETPOS_PARAM
	}

	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler의 XYT 축 좌표 관련 모든 Data를 반환 한다.
 *
 * @param	rgsFixedPos : 고정좌표 배열 (X, Y, T)
 * @param	rgsOffsetPos : Offset좌표 배열 (X, Y, T)
 * @param	rgsModelPos : Model좌표 배열 (X, Y, T)
 * @return	int : (0 = Success, Error Code = 그 외)
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
 * Handler의 XYT 축 좌표 관련 Model Data를 반환한다. 
 *
 * @param	iPos = Position ID
 * @return	int : (0 = Success, Error Code = 그 외)
 */
MPos_XYT MHandlerCommon::GetXYTModelPosParameter(int iPos)
{
	ASSERT(iPos < DEF_HANDLER_MAX_POS);
	return m_rgsXYTModelPos[iPos];
}
	
/**
 * Handler의 XYT 축 좌표 관련 Offset Data를 반환한다. 
 *
 * @param	iPos = Position ID
 * @return	int : (0 = Success, Error Code = 그 외)
 */
MPos_XYT MHandlerCommon::GetXYTOffsetPosParameter(int iPos) 
{
	ASSERT(iPos < DEF_HANDLER_MAX_POS);
	return m_rgsXYTOffsetPos[iPos];
}
	


/**
 * Handler의 Z 축 좌표 관련 모든 Data를 반환 한다.
 *
 * @param	rgdFixedPos : 고정좌표 배열 (Z)
 * @param	rgdOffsetPos : Offset좌표 배열 (Z)
 * @param	rgdModelPos : Model좌표 배열 (Z)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::GetZPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos)
{
	// ASSERT((rgdFixedPos != NULL) && (rgdOffsetPos != NULL));

//	int iResult = ERR_HANDLER_SUCCESS; // jdy 14. 사용하지 않는 변수임. 단 아래 return ERR_HANDLER_SUCCESS; 로 수정해야함.

	for (int i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		if (rgdFixedPos != NULL)	rgdFixedPos[i]  = m_rgdZFixedPos[i];		
		if (rgdOffsetPos != NULL)	rgdOffsetPos[i] = m_rgdZOffsetPos[i];		
		if (rgdModelPos != NULL)	rgdModelPos[i]	= m_rgdZModelPos[i];
	}

	return ERR_HANDLER_SUCCESS;
}
	

/**
 * Handler Z Teaching 정보를 읽는다.
 *
 * @return	STeachingInfo : Teaching 정보
 */
STeachingInfo MHandlerCommon::GetZTeachingInfo()
{
	return m_sZTeachingInfo;
}

/**
 * Handler XYT Teaching 정보를 읽는다.
 *
 * @return	STeachingInfo : Teaching 정보
 */
STeachingInfo MHandlerCommon::GetXYTTeachingInfo()
{
	return m_sXYTTeachingInfo;
}

/**
 * Handler(X,Y,T축)를 이동시킨다. Interlock을 확인한다.
 *
 * @param	iPosID : Position Index ID
 * @param	bUseAuto : Manual Mode로 사용 할 것인지 않할 것인지 확인 (FLASE : Manual Mode, FALSE : Auto Mode)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::SafeMoveXYTPos(int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	MPos_XYT posTarget;	

	// 목표 위치 읽기
	posTarget = GetXYTTargetPos(iPosID);

	// 해당 위치로 이동
	return MoveXYTPos(posTarget, iPosID, bUseAuto);
}

int MHandlerCommon::SafeMoveXPos(int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	double dTarget;	

	// 목표 위치 읽기
	dTarget = GetXYTTargetPos(iPosID).dX;

	// 해당 위치로 이동
	return MoveXPos(dTarget, iPosID, bUseAuto);
}

/**
 * Handler(Z축)를 Index위치로 이동시킨다. Interlock을 확인한다.
 *
 * @param	iPosID : Position Index ID
 * @param	bUseAuto : Manual Mode로 사용 할 것인지 않할 것인지 확인 (FLASE : Manual Mode, TRUE : Auto Mode)
 * @return	int : (0 = Success, Error Code = 그 외)
 */
int MHandlerCommon::SafeMoveZPos(int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	return ERR_HANDLER_SUCCESS;
#endif
	
	// 목표 위치 읽기
	double dTarget = GetZTargetPos(iPosID);

	// 해당 위치로 이동
	return MoveZPos(dTarget, iPosID, bUseAuto);
}

/*/
int MHandlerCommon::VacuumOn1(BOOL bSkipSensor)
{
	int iResult;
	m_plnkVac1->On(TRUE);
		
	if ((iResult = m_plnkVac1->On(bSkipSensor)) != ERR_HANDLER_SUCCESS)
	{
		//흡착 동작 오류

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
		//흡착 동작 오류

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
		//흡착 동작 오류

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
		//흡착 동작 오류

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
		//흡착 동작 오류

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
		//흡착 동작 오류
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
