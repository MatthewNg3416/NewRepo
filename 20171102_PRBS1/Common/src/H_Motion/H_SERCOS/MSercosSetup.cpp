/* 
 * Sercos Motion Component
 *
 * Copyright 2006 by Mechatronics & Manufacturing Technology Center,
 * Samsung Electronics, Inc., All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include "stdafx.h"
#include "DefMotionValue.h"
#include "MTickTimer.h"
#include "MSercosSetup.h"

#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * MSercosSetup.cpp : This file defines the class of Motion Board Data.
 *			        이 파일은 Motion Board 초기화를 위한 Class를 정의한다.
 *
 * @author Hong-Ju Mun (hongju.mun@samsung.com)
 * @version $Revision$
 */


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructor : initialize inner object, 내부 개체들을 초기화한다.
 * @stereotype constructor
 */	
MSercosSetup::MSercosSetup()
{
	m_bInitialized		= FALSE;
	m_bAlreadyPhase4	= FALSE;
}


/**
 * Destructor : Delete inner opbject
 *				내부 개체를 소멸한다.
 * @stereotype destructor
 */
MSercosSetup::~MSercosSetup()
{

}

/**
 * Board의 상태를 초기화 한다. 
 *
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
INT32S MSercosSetup::BoardInit()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	int iResult = smmc_init();
	if (iResult != ERR_MOTION_SUCCESS)
	{
		m_cpErrString = smmc_get_error_string();
		return 107920000;	// MMC2 보드를 사용할 수 없습니다.
	}

	m_bInitialized = true;

	return ERR_MOTION_SUCCESS;
}

/**
 * System Error를 초기화 한다. 
 *
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
INT32S MSercosSetup::ClearSystemError()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;
	INT32S	iRingNum;

	for (iRingNum = 0; iRingNum < m_iRingCount; iRingNum++)
	{
		iResult = smmc_clear_system_error(m_iRingList[iRingNum]);
		
		if(iResult != ERR_MOTION_SUCCESS)
		{
			return 107921000; // MMC2 System Error를 Clear 할 수 없습니다.
//			return generateErrorCode(107921); // MMC2 System Error를 Clear 할 수 없습니다.
//			m_cpErrString = smmc_get_error_string();
//			return iResult;
		}
	}
	return ERR_MOTION_SUCCESS;
}

/**
 * Board의 파라미터 상태를 초기화 한다. 
 *
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
INT32S MSercosSetup::SercosSetup()
{
	int iResult = ERR_MOTION_SUCCESS;
	int iRingList[8],nPhaseNo[8]={0}, bAlreadyPhase4[8];
	int iRingCount;
	int i = 0;

	iResult = smmc_get_ring_list(iRingList, &iRingCount);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		m_cpErrString = smmc_get_error_string();
		return 107922000; // Sercos Ring 정보를 읽을 수 없습니다.
	}

	int*	nPhase = new int[iRingCount];
	for (int i=0; i<iRingCount; i++)
	{
		nPhaseNo[i]=4;

		iResult = smmc_get_phase(iRingList[i], &nPhase[i]);
		if(iResult != ERR_MOTION_SUCCESS)
		{
			m_cpErrString = smmc_get_error_string();
			return 107923000; // Sercos Phase Number를 읽을 수 없습니다.
		}

		if (4 != nPhase[i])
		{
			bAlreadyPhase4[i] = FALSE;
			m_bAlreadyPhase4 = FALSE;
		}
		else
		{
			bAlreadyPhase4[i] = TRUE;
		}
	}
	
	for (int i=0; i<iRingCount; i++)
	{
		if (FALSE == bAlreadyPhase4[i])
		{
			m_bAlreadyPhase4 = FALSE;
			break;
		}
		else
		{
			m_bAlreadyPhase4 = TRUE;
		}
	}

		// all of the board phase set to CP4 
	iResult = smmc_set_phasen(iRingCount, iRingList, nPhaseNo);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		iResult = smmc_set_phasen(iRingCount, iRingList, nPhaseNo);
		if(iResult != ERR_MOTION_SUCCESS)
		{
			m_cpErrString = smmc_get_error_string();
			return 107924000; // Sercos Phase Number를 바꿀 수 없습니다.
		}
	}
	else
	{
		for (int i=0; i<iRingCount; i++)
		{
			bAlreadyPhase4[i] = TRUE;
			m_bAlreadyPhase4 = TRUE;
		}
	}

	delete []nPhase;

	return iResult;
}


INT32S MSercosSetup::CheckRealPhaseNo4(BOOL *pbReturn)
{
	int iResult = ERR_MOTION_SUCCESS;
	int iRingList[8];
	int iRingCount;

	iResult = smmc_get_ring_list(iRingList, &iRingCount);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		m_cpErrString = smmc_get_error_string();
		return 107922000; // Sercos Ring 정보를 읽을 수 없습니다.
	}

	int* nPhase = new int[iRingCount];
	for(int i=0; i<iRingCount; i++)
	{
		iResult = smmc_get_phase(iRingList[i], &nPhase[i]);
		if(iResult != ERR_MOTION_SUCCESS)
		{
			delete[] nPhase;
			m_cpErrString = smmc_get_error_string();
			return 107923000; // Sercos Phase Number를 읽을 수 없습니다.
		}

		if (4 != nPhase[i])
		{
			*pbReturn = FALSE;
			break;
		}
		else
		{
			*pbReturn = TRUE;
		}
	}
//	*pbReturn = TRUE;

	delete[] nPhase;
	return ERR_MOTION_SUCCESS;
}

BOOL MSercosSetup::IsAlreadyPhase4()
{
	return m_bAlreadyPhase4;
}