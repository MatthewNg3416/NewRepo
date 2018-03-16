/* 
 * TrsInspectionCamera Component
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
 * ITI_InspectionCamera.h : ITI InterfaceClass of MTrsInspectionCamera component.
 */

#ifndef ITI_INSPECTIONCAMERA_H
#define ITI_INSPECTIONCAMERA_H

/** @interface */
class ITI_InspectionCamera
{
public:  
	virtual BOOL IsInspectionStartToTM(int iTabMounterNoInGroup) = 0;
	virtual BOOL IsInspectionCompleteToTM(int iTabMounterNoInGroup) = 0;
	virtual BOOL IsAllMountCompleteToPA(int iWorkGroup) = 0;
	virtual BOOL IsAllMountCompleteToPASecond(int iWorkGroup) = 0;
	virtual int GetInspectTabNo(void) = 0;

	virtual BOOL IsReceivedPressCompleteSchedule(int iWorkGroup) = 0;
	virtual void ResetReceivedPressCompleteSchedule(int iWorkGroup) = 0;

	// NSMC
	virtual BOOL IsModelChangeReady()	= 0;
	virtual BOOL IsMountReadyToTM(int iTabMounterNoInGroup) = 0; //170918 JSH

	//170612 JSH.s
	virtual BOOL IsTabBlobWorkingToPA(int iTabMounterNoInGroup) = 0;
	virtual BOOL IsTabBlobWorkingToTM(int iTabMounterNoInGroup) = 0;
	//170612 JSH.e
};
#endif //ITI_INSPECTIONCAMERA_H
