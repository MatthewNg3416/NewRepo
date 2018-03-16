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
 *			        �� ������ Motion Board Data�� ���� Class�� �����Ѵ�.
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
 * �� Class�� motion board data�� ���� Class�� �����Ѵ�.
 * 
 * Motion Board�� ���� ���� �ٸ� ��ü���� �ߺ��ؼ� �ʱ�ȭ�� �Ѵٰų� ������ �ϴ� ������
 * �����ϱ� ���Ͽ� Motion Board ���� Data�� �����ϴ� Class�� ���� Member Variable����
 * static���� ó���Ͽ� �� Class�� ��ü�� ���� ��� ��ü���� Motion Board ���� Data�� ������ ��
 * �ֵ��� �Ͽ���.
 * 
 * ���� static Member Variable��
 * - ���� �ʱ�ȭ Flag
 * - ���� ���� ����
 * - ������ ����� ��ü���� ��� ����
 *
 * @author Yong-Seog Pahn(dragon.pahn@samsung.com)
 * @version $Revision$
 */
class MMotionBoard : public virtual ICommon
{

public:	// attribute

    /**
     * Motion Board ���� ����
	 * (static ó�� - ��� ��ü���� ������ �� ����)
	 * @link aggregationByValue 
     * @supplierCardinality 1
     */
	static SMotionBoard	m_BoardConfig;

    /**
     * Motion Board �ʱ�ȭ ���� ���θ� ����ϴ� Flag
     *  TRUE  : �ʱ�ȭ ��������
     *  FALSE : �ʱ�ȭ ���������
     * (static ó�� - ��� ��ü���� Board �ʱ�ȭ �ѹ��� ����)
     */
	static BOOL	m_bBoardInitSuccessFlag;

    /**
     * MMC PC10 Option Board �ʱ�ȭ ���� ���θ� ����ϴ� Flag
     *  TRUE  : �ʱ�ȭ ��������
     *  FALSE : �ʱ�ȭ ���������
     * (static ó�� - ��� ��ü���� Board �ʱ�ȭ �ѹ��� ����)
     */
	static BOOL	m_rgbPC10BoardInitSuccessFlag[DEF_MAX_MOTION_BD];

    /**
     * Motion Board�� ����ϴ� ���� ����ϴ� Flag
     *  TRUE  : �� �����
     *  FALSE : �� ��� ����
     * (static ó�� - ��� ��ü���� ���� �������� ����ϴ� ���� ����)
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
	 * Motion Board ���� ���� ����
	 *
	 * @param	mbdConfig		: Motion Board ���� ����
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
	 * Motion Board ���� ���� �б�
	 *
	 * @param	* pmbdConfig	: Motion Board ���� ����
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
	 * Motion Board �ʱ�ȭ ���� �б�
	 *
	 * @return	�ʱ�ȭ ����		: TRUE = �ʱ�ȭ����, FALSE = �ʱ�ȭ������
	 */
	BOOL GetBoardInitSuccess(void);

	/**
	 * Motion Board �ʱ�ȭ ���� �����ϱ�
	 *
	 * @param	bSet			: Flag ������
	 * @return	�ʱ�ȭ ����		: TRUE = �ʱ�ȭ����, FALSE = �ʱ�ȭ������
	 */
	BOOL SetBoardInitSuccess(BOOL bSet);

	/**
	 * PC10 Option Board �ʱ�ȭ ���� �б�
	 *
	 * @param	siBdID			: Board ID(0 ~ 7)
	 * @return	�ʱ�ȭ ����		: TRUE = �ʱ�ȭ����, FALSE = �ʱ�ȭ������
	 */
	BOOL GetPC10BoardInitSuccess(short siBdID);

	/**
	 * PC10 Option �ʱ�ȭ ���� �����ϱ�
	 *
	 * @param	siBdID			: Board ID(0 ~ 7)
	 * @param	bSet			: Flag ������
	 * @return	�ʱ�ȭ ����		: TRUE = �ʱ�ȭ����, FALSE = �ʱ�ȭ������
	 */
	BOOL SetPC10BoardInitSuccess(short siBdID, BOOL bSet);

	/**
	 * �� ��� ���� ����
	 *
	 * @param	siAxisID		: �� ID(0 ~ 63)
	 * @param	bState			: ��� ����(TRUE=���, FALSE=�̻��)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 */
	int SetUseAxis(short siAxisID, BOOL bState);

	/**
	 * �� ��� ���� �б�
	 *
	 * @param	siAxisID		: �� ID(0 ~ 63)
	 * @param	*pbState		: ��� ����(TRUE=���, FALSE=�̻��)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = USED AXIS ID(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	int GetUseAxis(short siAxisID, BOOL *pbState);

	/**
	 * ������ Board ���� �б�
	 *
	 * @param	*psiBdNum		: Board ���� ����
	 * @param	*plAddress		:(OPTION=NULL) DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	int GetBoardNum(short *psiBdNum, long *plAddress = NULL);

	/**
	 * Board ����, DPRAM Address �����ϱ�
	 *
	 * @param	siBdNum			: Board ���� ����
	 * @param	*plAddress		: DPRAM Address
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD NUMBER(MOTIONLIB)
	 *							  xx = INVALID POINTER(MOTIONLIB)
	 */
	int SetBoardNum(short siBdNum, long *plAddress);

};
#endif // MMOTIONBOARD_H

// End of File MMotionBoard.h
