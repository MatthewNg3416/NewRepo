/* 
 * TabAligner Component
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
 * IStateTabAligner.h : This file defines the interface class of TabAligner Component
 *					for interlock check by other Mechanical Layer Components.
 */

#ifndef ISTATE_ALIGNER_H
#define ISTATE_ALIGNER_H

#include "MPos_XYT.h"

/**
 * @interface
 */
class IStateTabAligner
{
public:

/**********
 * Position
 */
	/** ���� �������� ���θ� Ȯ���Ѵ�.
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	virtual int CheckOrigin(void)	= 0;

 	/**
	 * TabAligner�� ���� ��ġ�� �д´�.
	 *
	 * @param	bType : (OPTION=FALSE) Motion�� ��ġ ���� (FALSE=Encoder��, TRUE=Command��)
	 * @return	double : TabAligner�� ���� ��ġ
	 */
	virtual MPos_XYT GetCurrentPos(BOOL bType = FALSE) = 0;
	
	/**
	 * TabAligner�� ��ǥ ��ġ�� �д´�.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabAligner�� ��ǥ ��ġ 
	 */
	virtual MPos_XYT GetTargetPos(int iPosID) = 0;

 	/**
	 * TabAligner �࿡ ���� ���� ��ġ�� ��ǥ ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 *
	 * @param	int iPos (����� ���ǵ� TabAligner Working Position)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInPos(int iPos, MPos_XYT* pTolerance = NULL) = 0;

	/**
	 * TabAligner X�࿡ ���� ���� ��ġ�� Parameter�� ���޵� ��ġ�� ��ġ�ϴ��� Ȯ���Ѵ�.
	 * Error �� pbResult�� FALSE�� �����Ѵ�. 
	 *
     * @param	double dTgtPos (���ϰ��� �ϴ� ��ġ�� ��ǥ)
     * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual BOOL IsInPos(MPos_XYT pTgtPos, MPos_XYT* pTolerance = NULL) = 0;

/**********
 * Cylinder.
 */
	/**
	 * TabAligner Expand/Shrink Cylinder �� Expand ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * TabAligner Expand/Shrink Cylinder �� Shrink ���¸� Ȯ���Ѵ�.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

};
#endif //ISTATE_ALIGNER_H