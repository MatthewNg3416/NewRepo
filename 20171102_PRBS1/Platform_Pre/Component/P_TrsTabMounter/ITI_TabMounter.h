/* 
 * TrsTabMounter Component
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
 * ITI_TabMounter.h : ITI InterfaceClass of MTrsTabMounter component.
 */

#ifndef ITI_TABMOUNTER_H
#define ITI_TABMOUNTER_H

/** @interface */
class ITI_TabMounter
{
public:
	virtual BOOL IsTabICReceiveStartToTC(void) = 0;
	virtual BOOL IsTabICReceiveCompleteToTC(void) = 0;

	virtual BOOL IsInspectionAbleToIC(void) = 0;
	virtual BOOL IsInspectionWaitStartToIC(void) = 0;
	
	virtual BOOL IsAllInspectionCompleteToPA(int iWorkGroup) = 0;
	virtual BOOL IsAllMountCompleteToPA(int iWorkGroup) = 0;

	virtual BOOL IsAllInspectionCompleteToPASecond(int iWorkGroup) = 0;
	virtual BOOL IsAllMountCompleteToPASecond(int iWorkGroup) = 0;

	virtual BOOL IsTabMounterReadyPusherDownToPA() = 0;//@130204.KKY

	//TrsCarrier Remove_______________
	virtual BOOL IsTabICRemoveRequestToTC() = 0;
	virtual BOOL IsTabICRemoveReadyToTC() = 0;
	virtual BOOL IsTabICRemoveCompleteToTC() = 0;

	virtual BOOL IsTabICReloadRequestToTC() =  0;
	//________________________________

	virtual int GetMountTabNo(void) = 0;
	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;

	virtual BOOL IsPressCompleteSchedule(int iWorkGroup) = 0;
	
	virtual BOOL IsInspectionMountReadyWaitToIC(void) = 0; //170918 JSH
};
#endif //ITI_TABMOUNTER_H
