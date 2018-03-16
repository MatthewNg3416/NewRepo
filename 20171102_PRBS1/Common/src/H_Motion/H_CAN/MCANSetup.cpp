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
 *			        �� ������ Motion Board �ʱ�ȭ�� ���� Class�� �����Ѵ�.
 *
 * @author YouJin Choi (youjinchoi@samsung.com)
 * @version $Revision$
 */


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructor : initialize inner object, ���� ��ü���� �ʱ�ȭ�Ѵ�.
 * @stereotype constructor
 */	
MCANSetup::MCANSetup() : m_can(NULL)
{
	m_bInitialized = false;
	BoardInit();
}


/**
 * Destructor : Delete inner opbject
 *				���� ��ü�� �Ҹ��Ѵ�.
 * @stereotype destructor
 */
MCANSetup::~MCANSetup()
{
	if (NULL != m_can)
		delete m_can;
}

/**
 * Board�� ���¸� �ʱ�ȭ �Ѵ�. 
 *
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
int MCANSetup::BoardInit()
{
#ifdef SIMULATION
	return NO_ERROR;
#endif
	
	// Can Board ����  
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
		// Error ó��
//		WriteErrorLog("VMove() : VMove Start failed");
//		return generateErrorCode(117024);
	}

	m_bInitialized = true;

	return NO_ERROR;
}

/**
 * System Error�� �ʱ�ȭ �Ѵ�. 
 *
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
int MCANSetup::ClearSystemError()
{
#ifdef SIMULATION
	return NO_ERROR;
#endif

	return NO_ERROR;
}


