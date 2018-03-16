//______________________________________________________________________________

// File Name: ITI_PanelTransferCenter.h
// History	:
//		1. 2010.07.29 : Created
//		2.
//______________________________________________________________________________

#ifndef _ITI_PANEL_TRANSFER_CENTER_H_
#define _ITI_PANEL_TRANSFER_CENTER_H_
//______________________________________________________________________________

//______________________________________________________________________________

class ITI_PanelTransferCenter
{
public:  
	virtual BOOL IsInitialized() = 0;

	virtual BOOL IsSendStartToPanelTransfer()			= 0;
	virtual BOOL IsSendCompleteToPanelTransfer()		= 0;

	virtual BOOL IsSendStartToTurnHandler()				= 0;
	virtual BOOL IsSendCompleteToTurnHandler()			= 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
};
//______________________________________________________________________________

#endif
