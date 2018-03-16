/**
 * TabFeeder Component
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
 * IStateTabFeeder.h : 이 파일은 TabFeeder의 상태 체크를 위한 interface class를 정의한다.
 */

#ifndef IStateTabFeeder_H
#define IStateTabFeeder_H

/**
 * @interface
 */
class IStateTabFeeder
{
public:

//__________________________________________________________________________________________________
//	Pressing Motor 관련 함수

/**********
 * Vacuum
 */
	/**
	 * Press의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	virtual BOOL IsAbsorbPress()	= 0;

	/**
	 * Press의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	virtual BOOL IsReleasePress()	= 0;

/**********
 * Cylinder
 */

//	virtual BOOL IsOpenFeederSprocketCyl() = 0;
//	virtual BOOL IsCloseFeederSprocketCyl() = 0;

//	virtual BOOL IsOpenTensionSprocketCyl() = 0;
//	virtual BOOL IsCloseTensionSprocketCyl() = 0;

	virtual BOOL IsOpenRecoverySprocketCyl() = 0;
	virtual BOOL IsCloseRecoverySprocketCyl() = 0;


	virtual BOOL IsUnlockSupplierChuckCyl() = 0;
	virtual BOOL IsLockSupplierChuckCyl() = 0;

//	virtual BOOL IsUpReelRecoveryCyl() = 0;
//	virtual BOOL IsDownReelRecoveryCyl() = 0;

//	virtual BOOL IsForwardReelRecoveryCyl() = 0;
//	virtual BOOL IsBackwardReelRecoveryCyl() = 0;

/**********
 * Sensor
 */

	/**
	 * BadTab Detect
	 *
	 * @return	BOOL : (TRUE = Panel 있음, FALSE = Panel 없음)
	 */
	virtual BOOL IsBadTabDetected(int iSensorNo = -1) = 0;

	virtual BOOL IsPressMoldDetect(void) = 0;

	// 보호지 감지 센서...
	virtual BOOL IsDownProtectDetect(void) = 0;

	virtual BOOL IsTensionUpperLimitDetected(void) = 0;
	virtual BOOL IsTensionUpperDetected(void) = 0;
	virtual BOOL IsTensionLowerDetected(void) = 0;
	virtual BOOL IsTensionLowerLimitDetected(void) = 0;

	virtual BOOL IsRailUnfoldDetected(void) = 0;
//	virtual BOOL IsRailDiscardReelDtetcted(void) = 0;
	virtual BOOL IsCurrentReelDtetcted(void) = 0;
	virtual BOOL IsNewReelDtetcted(void) = 0;

	virtual BOOL IsSupplierSmallTensionDetected(void) = 0;
	virtual BOOL IsSupplierMiddleTensionDetected(void) = 0;
	virtual BOOL IsSupplierLargeTensionDetected(void) = 0;

/**********
 * Position
 */
	/**
	 * Press 축의 원점복귀 여부를 확인한다.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckPressOrigin()	= 0;
    /**
     * Press축의 현재 위치를 얻어온다.
	 *
     * @return	double : 좌표값
     */
	virtual double GetPressCurrentPos(BOOL bType = FALSE) =0;
    /**
     * Press축의 목표 위치를 얻어온다.
	 * Error 시 dTgtPos 0.0으로 설정한다. 
	 *
     * @param	int iPos (상수로 정의된 TabFeeder Working Position)
     * @return	double : 좌표값
     */
	virtual double GetPressTargetPos(int iPos)	= 0;
	/**
	 * Press축에 대해 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPos (상수로 정의된 TabFeeder Working Position)
	 * @param	double dUserTolerance (비교 허용 오차, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInPressPos(int iPos, double* dUserTolerance = NULL)	= 0;
	/**

	/**
	 * Sprocket 축의 원점복귀 여부를 확인한다.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckSprocket1Origin()	= 0;
	 /**
	 * Sprocket1의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치 (X, Y, T축)
	 */
	virtual double GetSprocket1CurrentPos(BOOL bType = FALSE) = 0;
    /**
     * Sprocket축의 목표 위치를 얻어온다.
	 * Error 시 dTgtPos 0.0으로 설정한다. 
	 *
     * @param	int iPos (상수로 정의된 TabFeeder Working Position)
     * @return	double : 좌표값
     */
	virtual double GetSprocket1TargetPos(int iPos)	= 0;
	/**
	 * Sprocket1축에 대해 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPos (상수로 정의된 TabFeeder Working Position)
	 * @param	double dUserTolerance (비교 허용 오차, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInSprocket1Pos(int iPos, double* dUserTolerance = NULL)	= 0;


	/**
	 * BadTab Z축의 원점복귀 여부를 확인한다.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckBadTabOrigin()	= 0;
	/**
	 * BadTab Z의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabFeeder의 현재 위치 (X, Y, T축)
	 */
	virtual double GetBadTabCurrentPos(BOOL bType = FALSE) = 0;
   /**
     * BadTab Z축의 목표 위치를 얻어온다.
	 * Error 시 dTgtPos 0.0으로 설정한다. 
	 *
     * @param	int iPos (상수로 정의된 TabFeeder Working Position)
     * @return	double : 좌표값
     */
	virtual double GetBadTabTargetPos(int iPos)	= 0;
	/**
	 * BadTab Z축에 대해 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPos (상수로 정의된 TabFeeder Working Position)
	 * @param	double dUserTolerance (비교 허용 오차, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInBadTabPos(int iPos, double* dUserTolerance = NULL)	= 0;

	/**
	 * Supplier 축의 원점복귀 여부를 확인한다.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckSupplierOrigin()	= 0;
    /**
     * Supplier축의 현재 위치를 얻어온다.
	 *
     * @return	double : 좌표값
     */
	virtual double GetSupplierCurrentPos(BOOL bType = FALSE) =0;
    /**
     * Supplier축의 목표 위치를 얻어온다.
	 * Error 시 dTgtPos 0.0으로 설정한다. 
	 *
     * @param	int iPos (상수로 정의된 TabFeeder Working Position)
     * @return	double : 좌표값
     */
	virtual double GetSupplierTargetPos(int iPos)	= 0;
	/**
	 * Supplier축에 대해 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPos (상수로 정의된 TabFeeder Working Position)
	 * @param	double dUserTolerance (비교 허용 오차, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInSupplierPos(int iPos, double* dUserTolerance = NULL)	= 0;
	
	
	virtual int CheckMoldPinOrigin(void) = 0;
	virtual double GetMoldPinCurrentPos(BOOL bType = FALSE) = 0;
	virtual double GetMoldPinTargetPos(int iPosID) = 0;
	virtual BOOL IsInMoldPinPos(int iPosID, double *dTolerance = NULL) = 0;

	/**
	 * Press의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	virtual int ReleasePress(BOOL bSkipSensor = FALSE) = 0;

	virtual int AbsorbPress(BOOL bSkipSensor = FALSE) = 0;

};

#endif //IStateTabFeeder_H
