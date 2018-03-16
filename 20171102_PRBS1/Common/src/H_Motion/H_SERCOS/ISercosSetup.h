/* 
 * Motion Component
 *
 * Copyright 2006 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef ISercosSetup_H
#define ISercosSetup_H

#include "SMMCDEF.h"

#include "ICommon.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ISercosSetup   : public virtual ICommon
{
public:

	virtual INT32S	BoardInit()						= 0;

	virtual INT32S	ClearSystemError()				= 0;

	virtual bool	GetInitialized()				= 0;

	virtual char*	GetErrorString()				= 0;

	virtual INT32S	SercosSetup()					= 0;

	virtual INT32S	CheckRealPhaseNo4(BOOL *pbReturn) = 0;
	
	virtual BOOL	IsAlreadyPhase4()				= 0;
};

#endif //
