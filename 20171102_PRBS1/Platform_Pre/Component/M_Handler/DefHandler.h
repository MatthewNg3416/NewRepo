/* 
 * Handler Component
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
 * DefHandler.h : This file defines the value of Handler component.
 *
 */

#ifndef DEF_HANDLER_H
#define DEF_HANDLER_H

#include "IIO.h"
#include "IVacuum.h"
#include "IAxis.h"
//@#include "MLog.h"
//@#include "MAcfData.h"
#include "MTeachingPosData.h"
#include "MSystemData.h"
#include "MPanelData.h"


//@class MAcfData;
class IOpPanel;
//@class IStateStage;
class IStateWorkBench;
class IStatePanelTransfer; //SJ_YYK 161104 Add.

/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/

typedef
/**
 * �� ����ü�� Mechanical Layer�� ��ȣ ������ ���� ���ο� �����
 * Component List�� �����ϴ� ����ü�̴�.
 *
 * @stereotype struct 
 */
struct {
	/**  Stage�� Pointer		*/
//@	IStateStage*		m_plnkStage;

    //20060503 ysb
	IStateWorkBench*    m_plnkWorkBench;

	//SJ_YYK 161104 Add..
	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransferIn;

	/** OpPanel�� Pointer		*/
	IOpPanel*			m_plnkOpPanel;

} SStateCheckCompListForHandler;

typedef
/**
 * Handler Component Data	
 */ 
struct SHandlerData
{
	// Handler Type
	int				m_iHandlerType;

	// kss 5. ���ʿ� 
	
	/** Panel Detect Sensor Address */
	unsigned short	m_usiIAddrPanelDetect;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiIAddrArmViolation;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiOAddrArmViolation;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiIAddrMoveInhibit;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiOAddrMoveInhibit;

	unsigned short	m_usOAddrIFMoveEnable;

	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;

	/** Panel Data ���� Class */
	MPanelData*		m_plnkPanelData;
	
	/** ������ǥ Data�� ����� File Name */
	CString			m_strPosFileName;

	/** Offset ��ǥ Data �� ����� File Name */
	CString			m_strOffsetPosFileName;

	/** Offset ��ǥ Data�� ����� File Path */
	CString			m_strOffsetPosFilePath;

	// kss 5. ���ʿ� 

	/** XYT�� Teaching ����							*/
    STeachingInfo	m_sXYTTeachingInfo;	

	/** FINAL Data									*/	// 
//@    MAcfData*		m_plnkAcfData;			
	
	/** Z�� Teaching ����							*/
	STeachingInfo	m_sZTeachingInfo;

} SHandlerData;


#endif //DEF_HANDLER_H
