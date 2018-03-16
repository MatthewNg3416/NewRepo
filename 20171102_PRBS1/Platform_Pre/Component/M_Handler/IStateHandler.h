// jdy 2. 전체넉으로 코드 들여쓰기 주의.
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
 * IStateHandler.h : 이 파일은 Handler의 상태 체크를 위한 interface class를 정의한다.
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

	// jdy 4. CheckOrigin() 에서 XYT 축을 담당하는지 표시해 주던가(CheckOriginXYT()), 아니면 Z 도 CheckOrigin() 안으로 넣고, 선택에 따라 원점복귀 여부를 보는 것이 좋을 듯. (어차피 XYT 만 움직여도 Z 원점복귀 여부는 확인 해야하므로 CheckOrigin() 안에 넣는게 좋을 듯.)
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
