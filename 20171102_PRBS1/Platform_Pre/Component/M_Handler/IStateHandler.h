// jdy 2. ��ü������ �ڵ� �鿩���� ����.
	/**
 * Handler Component
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
 * IStateHandler.h : �� ������ Handler�� ���� üũ�� ���� interface class�� �����Ѵ�.
 */

#ifndef IStateHandler_H
#define IStateHandler_H

#include "DefHandlerCommon.h"
#include "MPos_XYT.h"

/**
 * @interface
 */
class IStateHandler
{
public:
/**********
 * Vacuum
 */
	virtual BOOL IsAbsorbPanel(void)	= 0;

	virtual BOOL IsReleasePanel(void)	= 0;

	virtual BOOL IsAbsorbPanel(int iVacID)	= 0;

	virtual BOOL IsReleasePanel(int iVacID)	= 0;

/**********
 * Sensor
 */
	virtual BOOL IsPanelDetected(void)	= 0;

/**********
 * Position
 */
	virtual int CheckXYTOrigin() = 0;

	// jdy 4. CheckOrigin() ���� XYT ���� ����ϴ��� ǥ���� �ִ���(CheckOriginXYT()), �ƴϸ� Z �� CheckOrigin() ������ �ְ�, ���ÿ� ���� �������� ���θ� ���� ���� ���� ��. (������ XYT �� �������� Z �������� ���δ� Ȯ�� �ؾ��ϹǷ� CheckOrigin() �ȿ� �ִ°� ���� ��.)
	virtual BOOL CheckZOrigin() = 0;

	virtual BOOL IsXAxisAmpFault() = 0;

	virtual BOOL IsXAxisAmpEnable() = 0;

//@	virtual BOOL IsYAxisAmpFault() = 0;

//@	virtual BOOL IsYAxisAmpEnable() = 0;

	virtual BOOL IsTAxisAmpFault() = 0;

	virtual BOOL IsTAxisAmpEnable() = 0;

	virtual BOOL IsZAxisAmpFault() = 0;

	virtual BOOL IsZAxisAmpEnable() = 0;

	virtual int AbsorbPanel(BOOL bSkipSensor = FALSE) =0;

	virtual int ReleasePanel(BOOL bSkipSensor = FALSE) =0;

	virtual int CheckVacuum() = 0;

	//20060504 ysb
    virtual MPos_XYT GetXYTCurrentPos(BOOL bType = FALSE) = 0;

	virtual double GetZCurrentPos(BOOL bType = FALSE) = 0;

	virtual MPos_XYT GetXYTTargetPos(int iPosID, int iTabNum = -1) = 0;

	virtual double GetZTargetPos(int iPosID) = 0;

	virtual BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL) = 0;

	virtual BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL) = 0;

	virtual BOOL IsInXPos(int iPosID, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL) = 0;

//@	virtual BOOL IsInYPos(int iPosID, double* pdTolerance = NULL) = 0;

//@	virtual BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInTPos(int iPosID, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInTPos(double dTargetPos, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInZPos(int iPosID, double* pdTolerance = NULL) = 0;

	virtual BOOL IsInZPos(double dTargetPos, double* pdTolerance = NULL) = 0;


	 
};

#endif //IStateHandler_H
