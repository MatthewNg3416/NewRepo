/* 
 * CameraCarrier Component
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
 * IStateCameraCarrier.h : This file defines the interface class of CameraCarrier Component
 *					for interlock check by other Mechanical Layer Components.
 */

#ifndef ISTATE_CAMERA_CARRIER_H
#define ISTATE_CAMERA_CARRIER_H

/**
 * @interface
 */
class IStateCameraCarrier
{
public:

/**********
 * Position
 */
	/** 축의 원점복귀 여부를 확인한다.
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	virtual int	CheckOrigin(void) = 0;
	virtual int	CheckOriginBackup(void) = 0;	


 	/**
	 * CameraCarrier 의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : CameraCarrier의 현재 위치
	 */
	virtual double GetCurrentPos(BOOL bType = FALSE) = 0;
	virtual double GetCurrentPosBackup(BOOL bType = FALSE) = 0;
	
	/**
	 * CameraCarrier 의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : CameraCarrier의 목표 위치
	 */
	virtual double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1) = 0;
 	virtual double GetTargetPosBackup(int iPosID, int iTabNumber = -1) = 0;
	/**
	 * CameraCarrier 축에 대해 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPos (상수로 정의된 CameraCarrier Working Position)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInPos(int iScheduleNo, int iPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPosBackup(int iPos, double* pdTolerance = NULL) = 0;

	/**
	 * CameraCarrier 축에 대해 현재 위치와 Parameter로 전달된 위치가 일치하는지 확인한다.
	 *
     * @param	double dTgtPos (비교하고자 하는 위치의 좌표)
     * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual BOOL IsInPos(double dTgtPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPosBackup(double dTgtPos, double* pdTolerance = NULL) = 0;

/**********
 * Cylinder.
 */
	/**
	 * CameraCarrier Expand/Shrink Cylinder 의 Expand 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * CameraCarrier Expand/Shrink Cylinder 의 Shrink 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

/**********
 * etc.
 */
	virtual const int GetInspectCamCarrierNo(void) const = 0;

	virtual int	CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber) = 0;
	virtual int CheckCollisionOtherD(double dTargetPos) = 0;
	virtual BOOL IsMovingY() = 0;//Y축이 이동중인지...
	virtual double GetMovingTgPos() = 0;

	virtual int GetCameraNo(int iCameraId) = 0;
};
#endif //ISTATE_CAMERA_CARRIER_H