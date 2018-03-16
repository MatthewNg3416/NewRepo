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
 * IStateTabCarrier.h : �� ������ TabCarrier�� ���� üũ�� ���� interface class�� �����Ѵ�.
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
	 * TabCarrier�� Vacuum�� �������� Ȯ���Ѵ�. �����̸� Success Return.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	virtual int CheckVacuum(int iVacID) = 0;
	virtual int CheckVacuum() = 0;

	/**
	 * TabCarrier�� Vacuum ������¸� Ȯ���Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//	virtual BOOL IsAbsorbTabIC(int iVacID = -1)	= 0;
	virtual BOOL IsAbsorbTabIC()	= 0;
	
	/**
	 * TabCarrier�� Vacuum �������¸� Ȯ���Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = ���� ����, FALSE = ���� ����)
	 */
//	virtual BOOL IsReleaseTabIC(int iVacID = -1) = 0;
	virtual BOOL IsReleaseTabIC() = 0;

	/**
	 * TabCarrier�� Vacuum�� �����Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	virtual int AbsorbTabIC(int iVacID, BOOL bSkipSensor = FALSE) = 0;
	virtual int AbsorbTabIC(BOOL bSkipSensor = FALSE) = 0;
	
	/**
	 * TabCarrier�� Vacuum�� �����Ѵ�.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
	 * @return	int : (0 = Success, Error Code = �� ��)
	 */
//	virtual int ReleaseTabIC(int iVacID, BOOL bSkipSensor = FALSE) = 0;
	virtual int ReleaseTabIC(BOOL bSkipSensor = FALSE) = 0;

/**********
 * Cylinder
 */
	virtual BOOL IsShrinkCamera() = 0;
	virtual BOOL IsExpandCamera() = 0;

	/**
	 * TabCarrier�� ��µ� �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = �� ��)
	 */
	virtual BOOL IsUp()	= 0;

	/**
	 * TabCarrier�� �ϰ��� �������� Ȯ���Ѵ�.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = �� ��)
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
	 * TabCarrier X,Y���� �������� ���θ� Ȯ���Ѵ�.
	 *
      * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual int CheckOrigin()	= 0;
	virtual int CheckOriginX()	= 0;
	virtual int CheckOriginY()	= 0;
 
    /**
     * TabCarrier X,Y���� ���� ��ġ�� ���´�.
	 *
	 * @return	MPos_XY : ��ǥ��
     */
	virtual MPos_XYT GetCurrentXYTPos(BOOL bType = FALSE) = 0;


    /**
     * TabCarrier X,Y���� ��ǥ ��ġ�� ���´�.
	 * Error �� dTgtPos 0.0���� �����Ѵ�. 
	 *
     * @param	int iPos (����� ���ǵ� TabCarrier Working Position)
     * @return	MPos_XY : ��ǥ��
     */
	virtual MPos_XYT GetTargetXYTPos(int iPos)	= 0;

	/**
	 * TabCarrier X,Y�� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPosID (����� ���ǵ� TabCarrier Working Position)
	 * @param	double pTolerance (�� ��� ����, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL) = 0;
	virtual BOOL IsInXPos(int iPosID, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInYPos(int iPosID, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInTPos(int iPosID, double* pTolerance = NULL) = 0;//KKY

	/**
	 * TabCarrier X,Y�� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	double sTargetPos (���ϰ��� �ϴ� ��ġ�� ��ǥ)
	 * @param	double pTolerance (�� ��� ����, default = 0.0)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL) = 0;
	virtual BOOL IsInXPos(double dTargetPos, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInYPos(double dTargetPos, double* pTolerance = NULL) = 0;//KKY
	virtual BOOL IsInTPos(double dTargetPos, double* pTolerance = NULL) = 0;//KKY

	
	virtual int	CheckCollisionOther(int iPosID) = 0;
	virtual int CheckCollisionOther(MPos_XYT posTarget) = 0;
	virtual BOOL IsMovingY() = 0;//Y���� �̵�������...
	virtual MPos_XYT GetMovingTgPos() = 0;


	virtual int	GetWorkGroup() = 0;
	virtual	int	GetWorkInstance() = 0;

	virtual BOOL IsShortPitchTab() = 0;	//5Pitch���� Short, 5Pitch�ʰ� Long


	virtual int		CheckOriginExpand() = 0;
	virtual double	GetCurrentExpandPos(BOOL bType = FALSE) = 0;
	virtual double	GetTargetExpandPos(int iPos) = 0;
	virtual BOOL	IsInExpandPos(int iPosID, double* pTolerance = NULL) = 0;
	virtual BOOL	IsInExpandPos(double dTarget, double* pTolerance = NULL) = 0;

	virtual int		Wait4DoneXYT() = 0;
};

#endif //IStateTabCarrier_H
