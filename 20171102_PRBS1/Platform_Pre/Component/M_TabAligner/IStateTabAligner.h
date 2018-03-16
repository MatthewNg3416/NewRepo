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
	/** 축의 원점복귀 여부를 확인한다.
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	virtual int CheckOrigin(void)	= 0;

 	/**
	 * TabAligner의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabAligner의 현재 위치
	 */
	virtual MPos_XYT GetCurrentPos(BOOL bType = FALSE) = 0;
	
	/**
	 * TabAligner의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabAligner의 목표 위치 
	 */
	virtual MPos_XYT GetTargetPos(int iPosID) = 0;

 	/**
	 * TabAligner 축에 대해 현재 위치와 목표 위치가 일치하는지 확인한다.
	 *
	 * @param	int iPos (상수로 정의된 TabAligner Working Position)
	 * @return	TRUE : OK, FALSE : NOT OK
	 */
	virtual BOOL IsInPos(int iPos, MPos_XYT* pTolerance = NULL) = 0;

	/**
	 * TabAligner X축에 대해 현재 위치와 Parameter로 전달된 위치가 일치하는지 확인한다.
	 * Error 시 pbResult는 FALSE로 설정한다. 
	 *
     * @param	double dTgtPos (비교하고자 하는 위치의 좌표)
     * @return	TRUE : OK, FALSE : FAIL
	 */
	virtual BOOL IsInPos(MPos_XYT pTgtPos, MPos_XYT* pTolerance = NULL) = 0;

/**********
 * Cylinder.
 */
	/**
	 * TabAligner Expand/Shrink Cylinder 의 Expand 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * TabAligner Expand/Shrink Cylinder 의 Shrink 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

};
#endif //ISTATE_ALIGNER_H