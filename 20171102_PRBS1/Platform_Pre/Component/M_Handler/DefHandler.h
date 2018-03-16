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
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
struct {
	/**  Stage의 Pointer		*/
//@	IStateStage*		m_plnkStage;

    //20060503 ysb
	IStateWorkBench*    m_plnkWorkBench;

	//SJ_YYK 161104 Add..
	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransferIn;

	/** OpPanel의 Pointer		*/
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

	// kss 5. 불필요 
	
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

	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;

	/** Panel Data 관리 Class */
	MPanelData*		m_plnkPanelData;
	
	/** 고정좌표 Data가 저장될 File Name */
	CString			m_strPosFileName;

	/** Offset 좌표 Data 가 저장될 File Name */
	CString			m_strOffsetPosFileName;

	/** Offset 좌표 Data가 저장될 File Path */
	CString			m_strOffsetPosFilePath;

	// kss 5. 불필요 

	/** XYT축 Teaching 정보							*/
    STeachingInfo	m_sXYTTeachingInfo;	

	/** FINAL Data									*/	// 
//@    MAcfData*		m_plnkAcfData;			
	
	/** Z축 Teaching 정보							*/
	STeachingInfo	m_sZTeachingInfo;

} SHandlerData;


#endif //DEF_HANDLER_H
