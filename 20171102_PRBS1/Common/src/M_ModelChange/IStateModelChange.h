/* 
 * ModelChange Component
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

#ifndef ISTATE_MODEL_CHANGE_H
#define ISTATE_MODEL_CHANGE_H

/**
 * IStateModelChange.h : This file defines the state interface class of Transfer component.
 *
 * @interface
 */
class IStateModelChange 
{
public:
/************************************************************************/
/*                           Cylinder                                   */
/************************************************************************/
	virtual BOOL IsExpand(void) = 0;
	virtual BOOL IsShrink(void) = 0;

	virtual BOOL IsUp(void)		= 0;
	virtual BOOL IsDown(void)	= 0;

	virtual int Up(BOOL bSkipSensor = FALSE) = 0;
	virtual int Down(BOOL bSkipSensor = FALSE) = 0;

	virtual int ForAirRailFB1(BOOL bSkipSensor = FALSE) = 0;
	virtual int ForAirRailFB2(BOOL bSkipSensor = FALSE) = 0;

	virtual int BackAirRailFB1(BOOL bSkipSensor = FALSE) = 0;
	virtual int BackAirRailFB2(BOOL bSkipSensor = FALSE) = 0;

	virtual BOOL IsForAirRailFB1() = 0;
	virtual BOOL IsForAirRailFB2() = 0;

	virtual BOOL IsBackAirRailFB1() = 0;
	virtual BOOL IsBackAirRailFB2() = 0;

/************************************************************************/
/*		I/O Operation Interface											*/
/************************************************************************/
	virtual BOOL IsLockRail(void) = 0;
	virtual BOOL IsUnlockRail(void) = 0;

/************************************************************************/
/*                            Position                                  */
/************************************************************************/
	virtual double GetYCurrentPos(BOOL bType = FALSE) = 0;

	virtual double GetYTargetPos(int iPosID) = 0;

	virtual	double GetTargetWidth(int iPosID) = 0;
	virtual double GetCurrentWidth(BOOL bType = FALSE) = 0;

	virtual BOOL IsInYPos(int iPosID, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL) = 0;

	virtual double GetModelPosOffset() = 0;

};
#endif //ISTATE_MODEL_CHANGE_H
