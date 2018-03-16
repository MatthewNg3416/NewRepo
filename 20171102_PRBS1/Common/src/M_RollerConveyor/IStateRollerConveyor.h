/* 
 * RollerConveyor Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef ISTATE_ROLLER_CONVEYOR_H
#define ISTATE_ROLLER_CONVEYOR_H

/**
 * IStateRollerConveyor.h : This file defines the state interface class of RollerConveyor component.
 *
 * @interface
 */
class IStateRollerConveyor 
{
public:

	virtual BOOL IsUpStopperUDCyl(void) = 0;
	virtual BOOL IsDownStopperUDCyl(void) = 0;

	//20081110 by kss
	virtual BOOL IsForwardPanelPreAlignFBCyl(void) = 0;
	virtual BOOL IsBackwardPanelPreAlignFBCyl(void) = 0;

	virtual BOOL IsInMovingForward(void) = 0;
	virtual BOOL IsInMovingBackward(void) = 0;

	virtual BOOL IsNormalSpeed(void) = 0;
	virtual BOOL IsReducedSpeed(void) = 0;
	virtual BOOL IsTransferSpeed(void) = 0;

	virtual BOOL IsStopMoving(void) = 0;

	virtual BOOL IsInverterReady() = 0;
	virtual BOOL IsInverterFault() = 0;
	virtual int ResetInverterFault() = 0;

	virtual BOOL IsPanelDecelDetected() = 0;
	virtual BOOL IsPanelStopDetected() = 0;
	virtual BOOL IsPanelDetected() = 0;

	virtual double GetSpeed(int iSpeedType) = 0;

};
#endif //ISTATE_ROLLER_CONVEYOR_H
