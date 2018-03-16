/* 
 * Motion Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * CMotionBoard.h : This file defines the class of Motion Board Data.
 *			        이 파일은 Motion Board Data를 위한 Class를 정의한다.
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef MMOTIONBOARD_H
#define MMOTIONBOARD_H

#include "ICommon.h"
#include "DefMotionStruct.h"
//#include "DefMotionError.h"

/**
 * This class is defined class of motion board data.
 * 이 Class는 motion board data를 위한 Class로 정의한다.
 * 
 * Motion Board에 대해 서로 다른 개체에서 중복해서 초기화를 한다거나 설정을 하는 행위를
 * 방지하기 위하여 Motion Board 관련 Data를 관리하는 Class로 내부 Member Variable들을
 * static으로 처리하여 본 Class의 개체를 갖는 모든 개체들의 Motion Board 관련 Data가 공유될 수
 * 있도록 하였다.
 * 
 * 내부 static Member Variable들
 * - 보드 초기화 Flag
 * - 보드 구성 정보
 * - 보드의 축들을 개체에서 사용 여부
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */
class MMotionBoard : public virtual ICommon
{

public:	// attribute

    /**
     * Motion Board 구성 정보
	 * (static 처리 - 모든 객체에서 동일한 값 유지)
	 * @link aggregationByValue 
     * @supplierCardinality 1
     */
	static SMotionBoard	m_BoardConfig;

    /**
     * Motion Board 초기화 수행 여부를 기록하는 Flag
     *  TRUE  : 초기화 실행했음
     *  FALSE : 초기화 실행안했음
     * (static 처리 - 모든 객체에서 Board 초기화 한번만 수행)
     */
	static BOOL	m_bBoardInitSuccessFlag;

    /**
     * MMC PC10 Option Board 초기화 수행 여부를 기록하는 Flag
     *  TRUE  : 초기화 실행했음
     *  FALSE : 초기화 실행안했음
     * (static 처리 - 모든 객체에서 Board 초기화 한번만 수행)
     */
	static BOOL	m_rgbPC10BoardInitSuccessFlag[DEF_MAX_MOTION_BD];

    /**
     * Motion Board중 사용하는 축을 기록하는 Flag
     *  TRUE  : 축 사용함
     *  FALSE : 축 사용 안함
     * (static 처리 - 모든 객체에서 축을 이중으로 사용하는 것을 방지)
     */
	static BOOL	m_rgbAxisUseFlag[DEF_MAX_AXIS_NO];

public:		// Constructor & Destructor

	/**
	 * Constructor
	 * @stereotype constructor
	 */
	MMotionBoard();

	/**
	 * Constructor
	 *
	 * @stereotype constructor
	 */
	MMotionBoard(SCommonAttribute commonData, SMotionBoard mbdConfig);

	/**
	 * Destructor
	 * @stereotype destructor
	 */
	~MMotionBoard();

public:		// operation

	/**
	 * Motion Board 구성 정보 설정
	 *
	 * @param	mbdConfig		: Motion Board 구성 정보
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD TYPE(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER PER MOTION BOARD(MOTIONLIB)
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID PC10 INDECSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID ORIGIN WAIT TIME(<=0.0)(MOTIONLIB)
	 */
	int SetBoardConfig(SMotionBoard mbdConfig);

	/**
	 * Motion Board 구성 정보 읽기
	 *
	 * @param	* pmbdConfig	: Motion Board 구성 정보
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD TYPE(MOTIONLIB)
	 *							  xx = INVALID AXIS NUMBER PER MOTION BOARD(MOTIONLIB)
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID PC10 INDECSEL NUMBER(MOTIONLIB)
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  xx = INVALID ORIGIN WAIT TIME(<=0.0)(MOTIONLIB)
	 */
	int GetBoardConfig(SMotionBoard * pmbdConfig);

	/**
	 * Motion Board 초기화 상태 읽기
	 *
	 * @return	초기화 상태		: TRUE = 초기화했음, FALSE = 초기화안했음
	 */
	BOOL GetBoardInitSuccess(void);

	/**
	 * Motion Board 초기화 상태 설정하기
	 *
	 * @param	bSet			: Flag 설정값
	 * @return	초기화 상태		: TRUE = 초기화했음, FALSE = 초기화안했음
	 */
	BOOL SetBoardInitSuccess(BOOL bSet);

	/**
	 * PC10 Option Board 초기화 상태 읽기
	 *
	 * @param	siBdID			: Board ID(0 ~ 7)
	 * @return	초기화 상태		: TRUE = 초기화했음, FALSE = 초기화안했음
	 */
	BOOL GetPC10BoardInitSuccess(short siBdID);

	/**
	 * PC10 Option 초기화 상태 설정하기
	 *
	 * @param	siBdID			: Board ID(0 ~ 7)
	 * @param	bSet			: Flag 설정값
	 * @return	초기화 상태		: TRUE = 초기화했음, FALSE = 초기화안했음
	 */
	BOOL SetPC10BoardInitSuccess(short siBdID, BOOL bSet);

	/**
	 * 축 사용 여부 설정
	 *
	 * @param	siAxisID		: 축 ID(0 ~ 63)
	 * @param	bState			: 사용 여부(TRUE=사용, FALSE=미사용)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 */
	int SetUseAxis(short siAxisID, BOOL bState);

	/**
	 * 축 사용 여부 읽기
	 *
	 * @param	siAxisID		: 축 ID(0 ~ 63)
	 * @param	*pbState		: 사용 여부(TRUE=사용, FALSE=미사용)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	int GetUseAxis(short siAxisID, BOOL *pbState);

	/**
	 * 구성된 Board 개수 읽기
	 *
	 * @param	*psiBdNum		: Board 구성 개수
	 * @param	*plAddress		:(OPTION=NULL) DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	int GetBoardNum(short *psiBdNum, long *plAddress = NULL);

	/**
	 * Board 개수, DPRAM Address 설정하기
	 *
	 * @param	siBdNum			: Board 구성 개수
	 * @param	*plAddress		: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	int SetBoardNum(short siBdNum, long *plAddress);

};
#endif // MMOTIONBOARD_H

// End of File MMotionBoard.h
