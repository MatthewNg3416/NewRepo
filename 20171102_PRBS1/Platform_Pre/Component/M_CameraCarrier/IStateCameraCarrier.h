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
	/** ���� �������� ���θ� Ȯ���Ѵ�.
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	virtual int	CheckOrigin(void) = 0;
	virtual int	CheckOriginBackup(void) = 0;	


 	/**
	 * CameraCarrier �� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : CameraCarrier�� ���� ��ġ
	 */
	virtual double GetCurrentPos(BOOL bType = FALSE) = 0;
	virtual double GetCurrentPosBackup(BOOL bType = FALSE) = 0;
	
	/**
	 * CameraCarrier �� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : CameraCarrier�� ��ǥ ��ġ
	 */
	virtual double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1) = 0;
 	virtual double GetTargetPosBackup(int iPosID, int iTabNumber = -1) = 0;
	/**
	 * CameraCarrier �࿡ ���� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPos (����� ���ǵ� CameraCarrier Working Position)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInPos(int iScheduleNo, int iPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPosBackup(int iPos, double* pdTolerance = NULL) = 0;

	/**
	 * CameraCarrier �࿡ ���� ���� ��ġ�� Parameter�� ���޵� ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
     * @param	double dTgtPos (���ϰ��� �ϴ� ��ġ�� ��ǥ)
     * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual BOOL IsInPos(double dTgtPos, double* pdTolerance = NULL) = 0;
	virtual BOOL IsInPosBackup(double dTgtPos, double* pdTolerance = NULL) = 0;

/**********
 * Cylinder.
 */
	/**
	 * CameraCarrier Expand/Shrink Cylinder �� Expand ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * CameraCarrier Expand/Shrink Cylinder �� Shrink ���¸� Ȯ���Ѵ�.
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
	virtual BOOL IsMovingY() = 0;//Y���� �̵�������...
	virtual double GetMovingTgPos() = 0;

	virtual int GetCameraNo(int iCameraId) = 0;
};
#endif //ISTATE_CAMERA_CARRIER_H