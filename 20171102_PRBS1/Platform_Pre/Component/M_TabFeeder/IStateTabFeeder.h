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
 * IStateTabFeeder.h : �� ������ TabFeeder�� ���� üũ�� ���� interface class�� �����Ѵ�.
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
//	Pressing Motor ���� �Լ�

/**********
 * Vacuum
 */
	/**
	 * Press�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
	virtual BOOL IsAbsorbPress()	= 0;

	/**
	 * Press�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
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
	 * @return	BOOL : (TRUE = Panel ����, FALSE = Panel ����)
	 */
	virtual BOOL IsBadTabDetected(int iSensorNo = -1) = 0;

	virtual BOOL IsPressMoldDetect(void) = 0;

	// ��ȣ�� ���� ����...
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
	 * Press ���� �������� ���θ� Ȯ���Ѵ�.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckPressOrigin()	= 0;
    /**
     * Press���� ���� ��ġ�� ���´�.
	 *
     * @return	double : ��ǥ��
     */
	virtual double GetPressCurrentPos(BOOL bType = FALSE) =0;
    /**
     * Press���� ��ǥ ��ġ�� ���´�.
	 * Error �� dTgtPos 0.0���� �����Ѵ�. 
	 *
     * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
     * @return	double : ��ǥ��
     */
	virtual double GetPressTargetPos(int iPos)	= 0;
	/**
	 * Press�࿡ ���� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
	 * @param	double dUserTolerance (�� ��� ����, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInPressPos(int iPos, double* dUserTolerance = NULL)	= 0;
	/**

	/**
	 * Sprocket ���� �������� ���θ� Ȯ���Ѵ�.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckSprocket1Origin()	= 0;
	 /**
	 * Sprocket1�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ (X, Y, T��)
	 */
	virtual double GetSprocket1CurrentPos(BOOL bType = FALSE) = 0;
    /**
     * Sprocket���� ��ǥ ��ġ�� ���´�.
	 * Error �� dTgtPos 0.0���� �����Ѵ�. 
	 *
     * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
     * @return	double : ��ǥ��
     */
	virtual double GetSprocket1TargetPos(int iPos)	= 0;
	/**
	 * Sprocket1�࿡ ���� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
	 * @param	double dUserTolerance (�� ��� ����, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInSprocket1Pos(int iPos, double* dUserTolerance = NULL)	= 0;


	/**
	 * BadTab Z���� �������� ���θ� Ȯ���Ѵ�.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckBadTabOrigin()	= 0;
	/**
	 * BadTab Z�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabFeeder�� ���� ��ġ (X, Y, T��)
	 */
	virtual double GetBadTabCurrentPos(BOOL bType = FALSE) = 0;
   /**
     * BadTab Z���� ��ǥ ��ġ�� ���´�.
	 * Error �� dTgtPos 0.0���� �����Ѵ�. 
	 *
     * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
     * @return	double : ��ǥ��
     */
	virtual double GetBadTabTargetPos(int iPos)	= 0;
	/**
	 * BadTab Z�࿡ ���� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
	 * @param	double dUserTolerance (�� ��� ����, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInBadTabPos(int iPos, double* dUserTolerance = NULL)	= 0;

	/**
	 * Supplier ���� �������� ���θ� Ȯ���Ѵ�.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckSupplierOrigin()	= 0;
    /**
     * Supplier���� ���� ��ġ�� ���´�.
	 *
     * @return	double : ��ǥ��
     */
	virtual double GetSupplierCurrentPos(BOOL bType = FALSE) =0;
    /**
     * Supplier���� ��ǥ ��ġ�� ���´�.
	 * Error �� dTgtPos 0.0���� �����Ѵ�. 
	 *
     * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
     * @return	double : ��ǥ��
     */
	virtual double GetSupplierTargetPos(int iPos)	= 0;
	/**
	 * Supplier�࿡ ���� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPos (����� ���ǵ� TabFeeder Working Position)
	 * @param	double dUserTolerance (�� ��� ����, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInSupplierPos(int iPos, double* dUserTolerance = NULL)	= 0;
	
	
	virtual int CheckMoldPinOrigin(void) = 0;
	virtual double GetMoldPinCurrentPos(BOOL bType = FALSE) = 0;
	virtual double GetMoldPinTargetPos(int iPosID) = 0;
	virtual BOOL IsInMoldPinPos(int iPosID, double *dTolerance = NULL) = 0;

	/**
	 * Press�� Vacuum�� �����Ѵ�.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
	virtual int ReleasePress(BOOL bSkipSensor = FALSE) = 0;

	virtual int AbsorbPress(BOOL bSkipSensor = FALSE) = 0;

};

#endif //IStateTabFeeder_H
