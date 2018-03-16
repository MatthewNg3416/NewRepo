/* 
 * Transfer Component
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

#ifndef ISTATE_PANEL_TRANSFER_H
#define ISTATE_PANEL_TRANSFER_H

/**
 * IStatePanelTransfer.h : This file defines the state interface class of Transfer component.
 *
 * @interface
 */
class IStatePanelTransfer 
{
public:
/************************************************************************/
/*                           Cylinder                                   */
/************************************************************************/
	virtual int UpPickUpUDCyl(BOOL bSkipSensor = FALSE) = 0;
	virtual int DownPickUpUDCyl(BOOL bSkipSensor = FALSE) = 0;
	//virtual int UpStopperUDCyl(BOOL bSkipSensor = FALSE) = 0;
	//virtual int DownStopperUDCyl(BOOL bSkipSensor = FALSE) = 0;

	virtual BOOL IsUpPickUpUDCyl(void) = 0;
	virtual BOOL IsDownPickUpUDCyl(void) = 0;

	///virtual BOOL IsUpStopperUDCyl(void) = 0;
	///virtual BOOL IsDownStopperUDCyl(void) = 0;

	virtual int ForAirRailFB1(BOOL bSkipSensor = FALSE) = 0;
	virtual int ForAirRailFB2(BOOL bSkipSensor = FALSE) = 0;

	virtual int BackAirRailFB1(BOOL bSkipSensor = FALSE) = 0;
	virtual int BackAirRailFB2(BOOL bSkipSensor = FALSE) = 0;

	virtual BOOL IsForAirRailFB1() = 0;
	virtual BOOL IsForAirRailFB2() = 0;

	virtual BOOL IsBackAirRailFB1() = 0;
	virtual BOOL IsBackAirRailFB2() = 0;

/************************************************************************/
/*                            Vacuum                                    */
/************************************************************************/
	virtual int ReleasePanel(BOOL bSkipSensor = FALSE) = 0;
	virtual int AbsorbPanel(BOOL bSkipSensor = FALSE) = 0;

	virtual BOOL IsPanelAbsorbed(void) = 0;
	virtual BOOL IsPanelReleased(void) = 0;
	virtual BOOL IsPanelDetected(void) = 0; 
/************************************************************************/
/*                            Position                                  */
/************************************************************************/
	virtual double GetXCurrentPos(BOOL bType = FALSE) = 0;
	virtual double GetX2CurrentPos(BOOL bType = FALSE) = 0;

	virtual double GetXTargetPos(int iPosID) = 0;

	virtual BOOL IsInXPos(int iPosID, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL) = 0;

/************************************************************************/
/*                            Sensor                                    */
/************************************************************************/
	virtual BOOL IsPanelLoadingDetected() = 0;
//@	virtual BOOL IsPanelUnLoadingDetected() = 0;

	/*/
	virtual BOOL IsAirCVBlowOff() = 0;
	virtual int AirCVVacAllOn() = 0;
	virtual int AirCVVacAllOff() = 0;
	virtual BOOL IsAirCVVacAllOn() = 0;
	virtual BOOL IsAirCVVacAllOff() = 0;
	virtual int AirCVVacPlate1On() = 0;
	virtual int AirCVVacPlate1Off() = 0;
	virtual BOOL IsAirCVVacPlate1On() = 0;
	virtual BOOL IsAirCVVacPlate1Off() = 0;
	virtual int AirCVVacPlate2On() = 0;
	virtual int AirCVVacPlate2Off() = 0;
	virtual BOOL IsAirCVVacPlate2On() = 0;
	virtual BOOL IsAirCVVacPlate2Off() = 0;
	virtual int AirCVFloatingOn() = 0;
	virtual int AirCVFloatingOff() = 0;

	virtual BOOL IsPanelDetected() = 0;
	/*/
};
#endif //ISTATE_PANEL_TRANSFER_H
