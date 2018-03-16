/* 
 * Sercos Motion Component
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
 * MSercosSetup.h : This file defines the class of Sercos Motion Board Data.
 *			        이 파일은 Sercos Motion Board 초기화를 위한 Class를 정의한다.
 *
 * @author Hong-Ju Mun (hongju.mun@samsung.com)
 * @version $Revision$
 */

#include "DefMotionValue.h"
#include "ISercosSetup.h"

#ifndef MSercosSetup_H
#define MSercosSetup_H

#include "SMMCDEF.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * This class is defined class of Sercos motion board data.
 * 이 Class는 Sercos motion board 초기화를 위한 Class로 정의한다.
 * 
 * Sercos Motion Board에 대해 서로 다른 개체에서 중복해서 초기화를 한다거나 설정을 하는 행위를
 * 방지하기 위하여 Sercos 관련 Parameter를 관리하는 Class로 구성되어있다.
 * 
 * 내부 static Member Variable들
 * - 보드 초기화 Flag
 * - 보드 구성 정보
 * - 보드의 축들을 Sercos Parameter 관련 정보
 *
 * @author Hong-Ju Mun (hongju.mun@samsung.com)
 * @version $Revision$
 */

class MSercosSetup  : public ISercosSetup
{
public:

	/**
	 * Constructor : initialize inner object, 내부 개체들을 초기화한다.
	 * @stereotype constructor
	 */	
	MSercosSetup();

	/**
	 * Destructor : Delete inner opbject
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	~MSercosSetup();

	/**
	 * Board의 상태를 초기화 한다. 
	 *
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	virtual INT32S BoardInit();

	/**
	 * System Error를 초기화 한다. 
	 *
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	virtual INT32S ClearSystemError();

	/**
	 * 초기화 상태를 Return한다. 
	 *
	 * @return	: TRUE = 초기화 완료
	 *            FALSE = 초기화 미완료 
	 */
	virtual bool	GetInitialized() { return m_bInitialized; }

	/**
	 * ErrorString을 Return한다. 
	 *
	 * @return	m_cpErrString 
	 */
	virtual char*	GetErrorString() { return m_cpErrString; }

	/**
	 * Board의 파라미터 상태를 초기화 한다. 
	 *
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	virtual INT32S SercosSetup();

	INT32S	CheckRealPhaseNo4(BOOL *pbReturn);

	BOOL	IsAlreadyPhase4();

private:
	INT32S	m_iRingCount;

	INT32S	m_iRingList[MAX_RING_NUM];

	// Board가 Phase4 상태인지 유무 판단. TRUE : Phase 4, False : Phase 4 아님.
	BOOL	m_bAlreadyPhase4;

    /**  Board 초기화 여부 (TRUE:Board 초기화 했음) */
	bool	m_bInitialized;

	char*	m_cpErrString;
};

#endif //
