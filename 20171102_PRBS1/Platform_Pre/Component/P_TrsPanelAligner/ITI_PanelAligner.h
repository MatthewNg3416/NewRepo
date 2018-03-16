/* 
 * TrsPanelAligner Component
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
 * ITI_PanelAligner.h : ITI InterfaceClass of MTrsPanelAligner component.
 */

#ifndef ITI_PANEL_ALIGNER_H
#define ITI_PANEL_ALIGNER_H

/** @interface */
class ITI_PanelAligner
{
public:  
	virtual BOOL IsInitialized() = 0;

	virtual BOOL IsReceiveAbleToPanelTransfer()			= 0;
	virtual BOOL IsReceiveStartToPanelTransfer()		= 0;

	virtual BOOL IsSendStartToPanelTransferOut()		= 0;
	virtual BOOL IsSendCompleteToPanelTransferOut()		= 0;

	virtual BOOL IsPanelAlignerBeginAlignPanelToWorker()= 0;

	virtual BOOL IsWorkerEnabletoWork()					= 0;

	virtual BOOL IsInspectorEnabletoInspect()			= 0;

	virtual BOOL IsPanelTransferEnabletoMoveUnloadPos()	= 0;

	virtual BOOL IsPanelAlignerUnloadingPanel()			= 0;

	virtual void SetRepairRunContinue()					= 0;
	virtual BOOL IsRepairRunContinue()					= 0;

	virtual BOOL IsInspectorEnabletoInspectMove()		= 0;

	//syr..SESL: hongju 수정 0523_SESL
	// 현재 Plate가 VacuumOn 상태인지 체크
	virtual BOOL IsPlateVacuumOn()						= 0;

	virtual BOOL IsReceivedPressCompleteSchedule(int iWorkGroup)	= 0;
	virtual void ResetReceivedPressCompleteSchedule(int iWorkGroup)	= 0;


	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
	
	virtual BOOL IsReadyInspectionBolbPos(int iWorkGroup)	= 0; //170705 JSH
};
#endif //ITI_PANEL_ALIGNER_H
