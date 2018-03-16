/* 
 * CAN VCM Motion Component
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
#include "MCANSetup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * MCANSetup.cpp : This file defines the class of Motion Board Data.
 *			        이 파일은 Motion Board 초기화를 위한 Class를 정의한다.
 *
 * @author YouJin Choi (youjinchoi@samsung.com)
 * @version $Revision$
 */


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructor : initialize inner object, 내부 개체들을 초기화한다.
 * @stereotype constructor
 */	
MCANSetup::MCANSetup() : m_can(NULL)
{
	m_bInitialized = false;
	BoardInit();
}


/**
 * Destructor : Delete inner opbject
 *				내부 개체를 소멸한다.
 * @stereotype destructor
 */
MCANSetup::~MCANSetup()
{
	if (NULL != m_can)
		delete m_can;
}

/**
 * Board의 상태를 초기화 한다. 
 *
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
int MCANSetup::BoardInit()
{
#ifdef SIMULATION
	return NO_ERROR;
#endif
	
	// Can Board 설정  
	int canBPS = 1000000;         // CAN network bit rate
//	char *canDevice = "CAN0";     // Identifies the CAN device, if necessary
	char *canDevice = "KVASER0";  // Identifies the CAN device, if necessary

	// Board Open
//	m_can = new CopleyCAN(canDevice);
//	m_can->SetBaud(canBPS);

	m_can = new KvaserCAN(canDevice);

	// Open the CANopen network object
	const Error *err = m_canOpen.Open(*m_can);
	if (err)
	{
		// Error 처리
//		WriteErrorLog("VMove() : VMove Start failed");
//		return generateErrorCode(117024);
	}

	m_bInitialized = true;

	return NO_ERROR;
}

/**
 * System Error를 초기화 한다. 
 *
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
int MCANSetup::ClearSystemError()
{
#ifdef SIMULATION
	return NO_ERROR;
#endif

	return NO_ERROR;
}


