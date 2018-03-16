/*
 * TrsTHandler Component
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
 * ITI_THandler.h : ITI InterfaceClass of TrsTHandler component.
 */
#ifndef ITI_THANDLER_H
#define ITI_THANDLER_H

/** @interface */
class ITI_THandler {
public:    
	virtual BOOL IsInitialized()				= 0;
	virtual BOOL IsLoadingReady()			= 0;
	virtual BOOL IsLoadingComplete()			= 0;

	virtual BOOL IsUnloadingReady()				= 0;
	virtual BOOL IsUnloadingComplete()			= 0;
	
};
#endif //ITI_THandler_H
