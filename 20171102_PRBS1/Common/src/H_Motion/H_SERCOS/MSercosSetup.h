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
 *			        �� ������ Sercos Motion Board �ʱ�ȭ�� ���� Class�� �����Ѵ�.
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
 * �� Class�� Sercos motion board �ʱ�ȭ�� ���� Class�� �����Ѵ�.
 * 
 * Sercos Motion Board�� ���� ���� �ٸ� ��ü���� �ߺ��ؼ� �ʱ�ȭ�� �Ѵٰų� ������ �ϴ� ������
 * �����ϱ� ���Ͽ� Sercos ���� Parameter�� �����ϴ� Class�� �����Ǿ��ִ�.
 * 
 * ���� static Member Variable��
 * - ���� �ʱ�ȭ Flag
 * - ���� ���� ����
 * - ������ ����� Sercos Parameter ���� ����
 *
 * @author Hong-Ju Mun (hongju.mun@samsung.com)
 * @version $Revision$
 */

class MSercosSetup  : public ISercosSetup
{
public:

	/**
	 * Constructor : initialize inner object, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */	
	MSercosSetup();

	/**
	 * Destructor : Delete inner opbject
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	~MSercosSetup();

	/**
	 * Board�� ���¸� �ʱ�ȭ �Ѵ�. 
	 *
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	virtual INT32S BoardInit();

	/**
	 * System Error�� �ʱ�ȭ �Ѵ�. 
	 *
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	virtual INT32S ClearSystemError();

	/**
	 * �ʱ�ȭ ���¸� Return�Ѵ�. 
	 *
	 * @return	: TRUE = �ʱ�ȭ �Ϸ�
	 *            FALSE = �ʱ�ȭ �̿Ϸ� 
	 */
	virtual bool	GetInitialized() { return m_bInitialized; }

	/**
	 * ErrorString�� Return�Ѵ�. 
	 *
	 * @return	m_cpErrString 
	 */
	virtual char*	GetErrorString() { return m_cpErrString; }

	/**
	 * Board�� �Ķ���� ���¸� �ʱ�ȭ �Ѵ�. 
	 *
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	virtual INT32S SercosSetup();

	INT32S	CheckRealPhaseNo4(BOOL *pbReturn);

	BOOL	IsAlreadyPhase4();

private:
	INT32S	m_iRingCount;

	INT32S	m_iRingList[MAX_RING_NUM];

	// Board�� Phase4 �������� ���� �Ǵ�. TRUE : Phase 4, False : Phase 4 �ƴ�.
	BOOL	m_bAlreadyPhase4;

    /**  Board �ʱ�ȭ ���� (TRUE:Board �ʱ�ȭ ����) */
	bool	m_bInitialized;

	char*	m_cpErrString;
};

#endif //
