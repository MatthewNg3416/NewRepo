/* 
 * TrsTabFeeder Component
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
 * ITI_TabFeeder.h : ITI InterfaceClass of MTrsTabFeeder component.
 */

#ifndef ITI_TABFEEDER_H
#define ITI_TABFEEDER_H

/** @interface */
class ITI_TabFeeder
{
public:
	virtual BOOL IsSendAbleToTC()	= 0;
	virtual BOOL IsSendStartToTC()	= 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;

	virtual void SetPanelAlignerUnloadingpanel() = 0;//100930

};
#endif //ITI_TABFEEDER1_H
