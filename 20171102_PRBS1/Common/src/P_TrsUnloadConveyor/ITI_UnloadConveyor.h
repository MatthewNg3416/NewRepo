/* 
 * TrsUnloadConveyor Component
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
 * ITI_UnloadConveyor.h : ITI InterfaceClass of MTrsUnloadConveyor component.
 */

#ifndef ITI_UNLOAD_CONVEYOR_H
#define ITI_UNLOAD_CONVEYOR_H

/** @interface */
class ITI_UnloadConveyor
{
public:
	virtual BOOL IsInitialized() = 0;

	virtual BOOL IsReceiveAbleToPanelAligner()		= 0;
	virtual BOOL IsReceiveStartToPanelAligner()		= 0;
	virtual BOOL IsSendAbleToOutConveyor()			= 0;
	virtual BOOL IsSendCompleteToOutConveyor()		= 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
};
#endif //ITI_UNLOAD_CONVEYOR_H
