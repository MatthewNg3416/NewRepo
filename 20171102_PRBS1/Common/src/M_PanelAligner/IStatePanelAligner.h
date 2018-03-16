/* 
 * PanelAligner Component
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

#ifndef ISTATE_PANEL_ALIGNER_H
#define ISTATE_PANEL_ALIGNER_H

#include "MPos_XYT.h"
#include "MPos_XYTZ.h"

/**
 * IStatePanelAligner.h : This file defines the state interface class of PanelAligner component.
 *
 * @interface
 */
class IStatePanelAligner 
{
public:

/************************************************************************/
/*                            Vacuum                                    */
/************************************************************************/
	virtual int AbsorbPanel(BOOL bSkipSensor = FALSE) = 0;
	virtual int ReleasePanel(BOOL bSkipSensor = FALSE) = 0;

	virtual const BOOL IsPanelAbsorbed(void) = 0;
	virtual const BOOL IsPanelReleased(void) = 0;
		
//@	virtual int AbsorbPanelRotator(BOOL bSkipSensor = FALSE) = 0;
//@	virtual int ReleasePanelRotator(BOOL bSkipSensor = FALSE) = 0;

//@	virtual const BOOL IsPanelAbsorbedRotator(void) = 0;
//@	virtual const BOOL IsPanelReleasedRotator(void) = 0;

	virtual int AirCVBlowOn() = 0;
	virtual int AirCVBlowOff() = 0;
	virtual BOOL IsAirCVBlowOn() = 0;
	virtual BOOL IsAirCVBlowOff() = 0;
	
	virtual int AirCVVacAllOn(int iPanelWorkNo) = 0;
	virtual int AirCVVacAllOff() = 0;
	virtual BOOL IsAirCVVacAllOn(int iPanelWorkNo) = 0;
	virtual BOOL IsAirCVVacAllOff() = 0;

	virtual int AirCVVacPlate1On() = 0;
	virtual int AirCVVacPlate1Off() = 0;
	virtual BOOL IsAirCVVacPlate1On() = 0;
	virtual BOOL IsAirCVVacPlate1Off() = 0;

	virtual int AirCVVacPlate2On() = 0;
	virtual int AirCVVacPlate2Off() = 0;
	virtual BOOL IsAirCVVacPlate2On() = 0;
	virtual BOOL IsAirCVVacPlate2Off() = 0;

	virtual int AirCVVacPlate3On() = 0;
	virtual int AirCVVacPlate3Off() = 0;
	virtual BOOL IsAirCVVacPlate3On() = 0;
	virtual BOOL IsAirCVVacPlate3Off() = 0;

	virtual int AirCVVacPlate4On() = 0;
	virtual int AirCVVacPlate4Off() = 0;
	virtual BOOL IsAirCVVacPlate4On() = 0;
	virtual BOOL IsAirCVVacPlate4Off() = 0;
/************************************************************************/
/*                            Cylinder                                  */
/************************************************************************/
	virtual int UpPickUpUnit(BOOL bWaitOption = TRUE) = 0;
	virtual int DownPickUpUnit(BOOL bWaitOption = TRUE) = 0;

	virtual BOOL IsUpPickUpUnit(void) = 0;
	virtual BOOL IsDownPickUpUnit(void) = 0;

//@	virtual int UpRotatorUnit(BOOL bSkipSensor = FALSE) = 0;
//@	virtual int DownRotatorUnit(BOOL bSkipSensor = FALSE) = 0;

//@	virtual BOOL IsUpRotatorUnit(void) = 0;
//@	virtual BOOL IsDownRotatorUnit(void) = 0;

//@	virtual int TurnRotatorUnit(BOOL bSkipSensor = FALSE) = 0;
//@	virtual int ReturnRotatorUnit(BOOL bSkipSensor = FALSE) = 0;

//@	virtual BOOL IsTurnRotatorUnit(void) = 0;
//@	virtual BOOL IsReturnRotatorUnit(void) = 0;

#ifdef DEF_USE_PANEL_PUSHER
	virtual int UpPanelPusherUDCyl(BOOL bSkipSensor = FALSE) = 0;
	virtual int DownPanelPusherUDCyl(BOOL bSkipSensor = FALSE) = 0;
	virtual BOOL IsUpPanelPusherUDCyl() = 0;
	virtual BOOL IsDownPanelPusherUDCyl() = 0;
#endif
/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	virtual int CheckOrigin(void) = 0;
	virtual int CheckOriginZ(void) = 0;
//@	virtual int CheckOriginBackupMoveX(void) = 0;

	virtual MPos_XYT GetXYTCurrentPos(BOOL bType = FALSE) = 0;
	virtual MPos_XYT GetXYTTargetPos(int iPosID) = 0;

	virtual double	GetZCurrentPos(BOOL bType = FALSE) = 0;
	virtual double	GetZTargetPos(int iPosID) = 0;

//@	virtual double	GetBackupMoveXCurrentPos(BOOL bType = FALSE) = 0;
//@	virtual double	GetBackupMoveXTargetPos(int iPosID) = 0;

	
	virtual BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL) = 0;
	virtual BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL) = 0;

	virtual BOOL IsInZPos(int iPosID, double* pTolerance = NULL) = 0;
	virtual BOOL IsInZPos(double dTargetPos, double* pTolerance = NULL) = 0;

//@	virtual BOOL IsInBackupMoveXPos(int iPosID, double* pTolerance = NULL) = 0;
//@	virtual BOOL IsInBackupMoveXPos(double dTargetPos, double* pTolerance = NULL) = 0;
	
	virtual BOOL IsInXPos(int iPosID, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInYPos(int iPosID, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInTPos(int iPosID, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInTPos(double dTargetPos, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInXYTMotion() = 0;

	virtual int GetXYTPosParameter(MPos_XYT* rgsFixedPos = NULL, MPos_XYT* rgsOffsetPos = NULL, MPos_XYT* rgsModelPos = NULL) = 0;

	virtual int GetZPosParameter(double* rgdFixedPos = NULL, double* rgdOffsetPos = NULL, double* rgdModelPos = NULL) = 0;

//@	virtual int GetBackupMoveXPosParameter(double* rgdFixedPos = NULL, double* rgdOffsetPos = NULL, double* rgdModelPos = NULL) = 0;

#ifdef DEF_GATE_SYSTEM
	virtual BOOL IsPanelLoadingDetected() = 0;
#endif
	virtual BOOL IsPanelWorkingDetected() = 0;

	virtual int	GetPanelWorkNoFromCurPos() = 0;

};
#endif //ISTATE_PANEL_ALIGNER_H
