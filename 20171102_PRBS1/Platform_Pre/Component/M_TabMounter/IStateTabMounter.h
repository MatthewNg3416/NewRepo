/* 
 * TabMounter Component
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
 * IStateTabMounter.h : This file defines the interface class of TabMounter Component
 *					for interlock check by other Mechanical Layer Components.
 */

#ifndef ISTATE_TABMOUNTER_H
#define ISTATE_TABMOUNTER_H

/**
 * @interface
 */
class IStateTabMounter
{
public:

/**********
 * Position
 */
	virtual int	CheckOrigin(void) = 0;
	virtual int CheckOriginZ(void) = 0;
	virtual double GetCurrentPos(BOOL bType = FALSE) = 0;
	virtual double GetCurrentPosZ(BOOL bType = FALSE) = 0;
//@	virtual double GetTargetPos(int iPosID, int iTabNumber = -1) = 0;
	virtual double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1) = 0;
	virtual double GetTargetPosZ(int iPosID) = 0;
//@	virtual BOOL IsInPos(int iPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPos(int iScheduleNo, int iPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPos(double dTgtPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPosZ(int iPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPosZ(double dTgtPos, double* pdTolerance = NULL) = 0;

//@#if FALSE	//==============================================

	virtual double GetCurrentGantryPos(BOOL bType = FALSE) = 0;
	virtual double GetTargetGantryPos(int iGantryPosID) = 0;
	virtual BOOL IsInGantryPos(int iGantryPosID, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInGantryPos(double dGantryTargetPos, double* pdTolerance = NULL) = 0;

//@#endif		//#if FALSE	//==================================
/**********
 * Cylinder
 */

	virtual BOOL IsUpCyl(void) = 0;
	virtual BOOL IsDownCyl(void) = 0;

	virtual int	 UpPusherCyl(BOOL bSkipSensor = FALSE) = 0;
	virtual int	 DownPusherCyl(BOOL bSkipSensor = FALSE) = 0;
	virtual BOOL IsUpPusherCyl(void) = 0;
	virtual BOOL IsDownPusherCyl(void) = 0;

#if FALSE	//==============================================
	virtual BOOL IsUpUDCyl(void) = 0;
	virtual BOOL IsDownReady(void) = 0;
	virtual BOOL IsDownPress(void) = 0;

	virtual BOOL IsUpBackupUDCyl(void) = 0;
	virtual BOOL IsDownBackupUDCyl(void) = 0;

	virtual BOOL IsForwardBackupFBCyl(void) = 0;
	virtual BOOL IsBackwardBackupFBCyl(void) = 0;

	virtual int Up(BOOL bSkipSensor = FALSE) = 0;

#endif		//#if FALSE	//==================================

/**********
 * etc.
 */
	virtual const int GetTabMounterNo(void) const = 0;

	virtual const BOOL IsUpAllTabMounterInGroup(void) = 0;

	virtual const BOOL IsInSafePosTabMounterFromAligner(int nToolID) = 0;

//@	virtual const BOOL IsIn_FrontTabLoadPos_TabMounter(int nToolID) = 0;
	virtual const BOOL IsIn_FrontTabLoadPos_TabMounter(int iScheduleNo, int nToolID) = 0;
	
//@	virtual const BOOL IsIn_RearTabLoadPos_TabMounter(int nToolID) = 0;
	virtual const BOOL IsIn_RearTabLoadPos_TabMounter(int iScheduleNo, int nToolID) = 0;

	virtual BOOL IsInDangerPosWithTabCarrier() = 0;


//@	virtual int	CheckCollisionOther(int iPosID, int iTabNumber) = 0;
	virtual int	CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber) = 0;
	virtual int CheckCollisionOtherD(double dTargetPos) = 0;
	virtual BOOL IsMovingY() = 0;//Y축이 이동중인지...
	virtual double GetMovingTgPos() = 0;

	virtual int	GetWorkGroup() = 0;
	virtual	int	GetWorkInstance() = 0;

	virtual int SafeMoveZPos(int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE) = 0;
};
#endif //ISTATE_TABMOUNTER_H