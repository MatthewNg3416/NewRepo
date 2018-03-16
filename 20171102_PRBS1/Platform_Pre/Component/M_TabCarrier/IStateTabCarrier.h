/**
 * TabCarrier Component
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
 * IStateTabCarrier.h : 이 파일은 TabCarrier의 상태 체크를 위한 interface class를 정의한다.
 */

#ifndef IStateTabCarrier_H
#define IStateTabCarrier_H

#include "MPos_XYT.h"

/**
 * @interface
 */
class IStateTabCarrier
{
public:
/**********
 * Vacuum
 */
	/**
	 * TabCarrier의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	virtual int CheckVacuum(int iVacID) = 0;
	virtual int CheckVacuum() = 0;

	/**
	 * TabCarrier의 Vacuum 압축상태를 확인한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
//	virtual BOOL IsAbsorbTabIC(int iVacID = -1)	= 0;
	virtual BOOL IsAbsorbTabIC()	= 0;
	
	/**
	 * TabCarrier의 Vacuum 해제상태를 확인한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
//	virtual BOOL IsReleaseTabIC(int iVacID = -1) = 0;
	virtual BOOL IsReleaseTabIC() = 0;

	/**
	 * TabCarrier의 Vacuum을 압축한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	virtual int AbsorbTabIC(int iVacID, BOOL bSkipSensor = FALSE) = 0;
	virtual int AbsorbTabIC(BOOL bSkipSensor = FALSE) = 0;
	
	/**
	 * TabCarrier의 Vacuum을 해제한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	virtual int ReleaseTabIC(int iVacID, BOOL bSkipSensor = FALSE) = 0;
	virtual int ReleaseTabIC(BOOL bSkipSensor = FALSE) = 0;

/**********
 * Cylinder
 */
	virtual BOOL IsShrinkCamera() = 0;
	virtual BOOL IsExpandCamera() = 0;

	/**
	 * TabCarrier가 상승된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = 그 외)
	 */
	virtual BOOL IsUp()	= 0;

	/**
	 * TabCarrier가 하강된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = 그 외)
	 */
	virtual BOOL IsDown()	= 0;

	virtual BOOL IsForward()	= 0;

	virtual BOOL IsBackward()	= 0;

	virtual BOOL IsForwardDumpBasket() = 0;
	virtual BOOL IsBackwardDumpBasket() = 0;

/**********
 * Sensor
 */

/**********
 * Position
 */
	/**
	 * TabCarrier X,Y축의 원점복귀 여부를 확인한다.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckOrigin()	= 0;
	virtual int CheckOriginX()	= 0;
	virtual int CheckOriginY()	= 0;
 
    /**
     * TabCarrier X,Y축의 현재 위치를 얻어온다.
	 *
	 * @return	MPos_XY : 좌표값
     */
	virtual MPos_XYT GetCurrentXYTPos(BOOL bType = FALSE) = 0;


    /**
     * TabCarrier X,Y축의 목표 위치를 얻어온다.
	 * Error 시 dTgtPos 0.0으로 설정한다. 
	 *
     * @param	int iPos (상수로 정의된 TabCarrier Working Position)
     * @return	MPos_XY : 좌표값
     */
	virtual MPos_XYT GetTargetXYTPos(int iPos)	= 0;

	/**
	 * TabCarrier X,Y의 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPosID (상수로 정의된 TabCarrier Working Position)
	 * @param	double pTolerance (비교 허용 오차, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL) = 0;
	virtual BOOL IsInXPos(int iPosID, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInYPos(int iPosID, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInTPos(int iPosID, double* pTolerance = NULL) = 0;//KKY

	/**
	 * TabCarrier X,Y의 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	double sTargetPos (비교하고자 하는 위치의 좌표)
	 * @param	double pTolerance (비교 허용 오차, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL) = 0;
	virtual BOOL IsInXPos(double dTargetPos, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInYPos(double dTargetPos, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInTPos(double dTargetPos, double* pTolerance = NULL) = 0;//KKY

	
	virtual int	CheckCollisionOther(int iPosID) = 0;
	virtual int CheckCollisionOther(MPos_XYT posTarget) = 0;
	virtual BOOL IsMovingY() = 0;//Y축이 이동중인지...
	virtual MPos_XYT GetMovingTgPos() = 0;


	virtual int	GetWorkGroup() = 0;
	virtual	int	GetWorkInstance() = 0;

	virtual BOOL IsShortPitchTab() = 0;	//5Pitch이하 Short, 5Pitch초과 Long


	virtual int		CheckOriginExpand() = 0;
	virtual double	GetCurrentExpandPos(BOOL bType = FALSE) = 0;
	virtual double	GetTargetExpandPos(int iPos) = 0;
	virtual BOOL	IsInExpandPos(int iPosID, double* pTolerance = NULL) = 0;
	virtual BOOL	IsInExpandPos(double dTarget, double* pTolerance = NULL) = 0;

	virtual int		Wait4DoneXYT() = 0;
};

#endif //IStateTabCarrier_H
