/* 
 * CAN VCM Motion Component
 *
 * Copyright 2007 by Mechatronics & Manufacturing Technology Center,
 * Samsung Electronics, Inc., All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MCANSetup.h : This file defines the class of Sercos Motion Board Data.
 *			        이 파일은 Sercos Motion Board 초기화를 위한 Class를 정의한다.
 *
 * @author YouJin Choi (youjinchoi@samsung.com)
 * @version $Revision$
 */

#ifndef MCANSetup_H
#define MCANSetup_H

#include "can_kvaser.h"
#include "CML_CanOpen.h"
#include "ICommon.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

CML_NAMESPACE_USE();

/**
 * This class is defined class of CAN VCM motion board data.
 * 이 Class는 CAN VCM motion board 초기화를 위한 Class로 정의한다.
 * 
 * CAN VCM Motion Board에 대해 서로 다른 개체에서 중복해서 초기화를 한다거나 설정을 하는 행위를
 * 방지하기 위하여 CAN VCM 관련 Parameter를 관리하는 Class로 구성되어있다.
 * 
 * 내부 static Member Variable들
 * - 보드 초기화 Flag
 * - 보드 구성 정보
 * - 보드의 축들 CAN VCM Parameter 관련 정보
 *
 * @author YouJin Choi (youjinchoi@samsung.com)
 * @version $Revision$
 */

class MCANSetup : public virtual ICommon 
{
public:

	/**
	 * Constructor : initialize inner object, 내부 개체들을 초기화한다.
	 * @stereotype constructor
	 */	
	MCANSetup();

	/**
	 * Destructor : Delete inner opbject
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	~MCANSetup();

	/**
	 * Board의 상태를 초기화 한다. 
	 *
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	int BoardInit();

	/**
	 * System Error를 초기화 한다. 
	 *
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	int ClearSystemError();

	CanOpen			m_canOpen;

private:
//	CopleyCAN*		m_can;
	KvaserCAN*		m_can;

    /**  Board 초기화 여부 (TRUE:Board 초기화 했음) */
	bool	m_bInitialized;

};

#endif //
