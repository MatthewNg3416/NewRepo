//______________________________________________________________________________

// File Name: ITI_PanelTransferOut.h
// History	:
//		1. 2010.07.29 : Created
//		2.
//______________________________________________________________________________

#ifndef _ITI_PANEL_TRANSFER_OUT_H_
#define _ITI_PANEL_TRANSFER_OUT_H_
//______________________________________________________________________________

//______________________________________________________________________________

class ITI_PanelTransferOut
{
public:  
	virtual BOOL IsInitialized() = 0;

	virtual BOOL IsReceiveAbleToPanelAligner()		= 0;
	virtual BOOL IsReceiveStartToPanelAligner()		= 0;
	virtual BOOL IsSendAbleToNextMachine()			= 0;
	virtual BOOL IsSendCompleteToNextMachine()		= 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
};
//______________________________________________________________________________

#endif
