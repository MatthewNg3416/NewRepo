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
 *			        �� ������ Sercos Motion Board �ʱ�ȭ�� ���� Class�� �����Ѵ�.
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
 * �� Class�� CAN VCM motion board �ʱ�ȭ�� ���� Class�� �����Ѵ�.
 * 
 * CAN VCM Motion Board�� ���� ���� �ٸ� ��ü���� �ߺ��ؼ� �ʱ�ȭ�� �Ѵٰų� ������ �ϴ� ������
 * �����ϱ� ���Ͽ� CAN VCM ���� Parameter�� �����ϴ� Class�� �����Ǿ��ִ�.
 * 
 * ���� static Member Variable��
 * - ���� �ʱ�ȭ Flag
 * - ���� ���� ����
 * - ������ ��� CAN VCM Parameter ���� ����
 *
 * @author YouJin Choi (youjinchoi@samsung.com)
 * @version $Revision$
 */

class MCANSetup : public virtual ICommon 
{
public:

	/**
	 * Constructor : initialize inner object, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */	
	MCANSetup();

	/**
	 * Destructor : Delete inner opbject
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	~MCANSetup();

	/**
	 * Board�� ���¸� �ʱ�ȭ �Ѵ�. 
	 *
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	int BoardInit();

	/**
	 * System Error�� �ʱ�ȭ �Ѵ�. 
	 *
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	int ClearSystemError();

	CanOpen			m_canOpen;

private:
//	CopleyCAN*		m_can;
	KvaserCAN*		m_can;

    /**  Board �ʱ�ȭ ���� (TRUE:Board �ʱ�ȭ ����) */
	bool	m_bInitialized;

};

#endif //
