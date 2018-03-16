/* 
 * TrsPanelTransfer Component
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
 * ITI_PanelTransfer.h : ITI InterfaceClass of MTrsPanelTransfer component.
 */

#ifndef ITI_PANEL_TRANSFER_H
#define ITI_PANEL_TRANSFER_H

/** @interface */
class ITI_PanelTransfer
{
public:  
	virtual BOOL IsInitialized() = 0;

	virtual BOOL IsReceiveAbleToPanelTransferCenter()			= 0;
	virtual BOOL IsReceiveStartToPanelTransferCenter()			= 0;

	virtual BOOL IsReceiveAbleToTurnHandler()				= 0;
	virtual BOOL IsReceiveCompleteToTurnHandler()			= 0;

	virtual BOOL IsSendStartToPanelAligner()			= 0;
	virtual BOOL IsSendCompleteToPanelAligner()			= 0;

//	virtual BOOL IsLoadConveyorEnabletoReceivePanel()	= 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
};
#endif //ITI_PANEL_TRANSFER_H
