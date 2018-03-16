/* samsung */
/* 
 * RFID Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * IRFID.h : This file defines the interface class of RFID component.
 *			  이 파일은 RFID Class에 대한 interface class를 정의한다.
 *
 * @author  Minkyoung Jeong(minkyoung.jeong@samsung.com)
 * @version $Revision$
 */
#ifndef IRFID_H
#define IRFID_H

#include "DefRFID.h"

class IRFID 
{
public:
	IRFID(){};
	~IRFID(){};

public:
	virtual CString ReadData(EZigItemType eZigItemType)=0;

	virtual int WriteData(EZigItemType eZigItemType, char *pData)=0;

	/** Battery State 반환 */
	virtual int GetBatteryState() = 0;
	
};
#endif //IRFID_H
