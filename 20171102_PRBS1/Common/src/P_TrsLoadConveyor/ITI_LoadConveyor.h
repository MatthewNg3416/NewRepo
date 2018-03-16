/* 
 * TrsLoadConveyor Component
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
 * ITI_LoadConveyor.h : ITI InterfaceClass of MTrsLoadConveyor component.
 */

#ifndef ITI_LOAD_CONVEYOR_H
#define ITI_LOAD_CONVEYOR_H

/** @interface */
class ITI_LoadConveyor
{
public:
	virtual BOOL IsInitialized()					= 0;
	virtual BOOL IsSendStartToPanelTransfer()		= 0;
	virtual BOOL IsSendCompleteToPanelTransfer()	= 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
};
#endif //ITI_LOAD_CONVEYOR_H
