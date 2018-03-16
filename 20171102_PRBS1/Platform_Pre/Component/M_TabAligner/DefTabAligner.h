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
 * DefTabAligner.h : This file defines the value of TabAligner component.
 *
 */

#ifndef DEF_TABALIGNER_H
#define DEF_TABALIGNER_H

#include "MTeachingInfo.h"

class IAxis;
class ICylinder;
class MSystemData;

/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/

/**
 * Referenced Component List by TabAligner Component
 */
typedef struct tagSTabAlignerRefCompList
{
    IAxis*		m_plnkAxis1;
    IAxis*		m_plnkAxis2;
	ICylinder*	m_plnkESCyl;	// Only Use for Mech ver2.0

} STabAlignerRefCompList;

/**
 * 이 구조체는 Mechanical Layer간 상호 참조를 위해 내부에 연결될
 * Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{

} SStateCheckCompListForTabAligner;

typedef
/**
 * TabAligner Component Data	
 */ 
struct STabAlignerData
{
	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;

	/** PositionInfo.inf File 정보 구조체 */
	STeachingInfo	m_sTeachingInfo;
	
	/** Offset Data가 저장될 File Name */
	CString			m_strOffsetPosFileName;
	
	/** Parameter Data 가 저장될 File Name */
	CString			m_strPosFileName;

	/** TabAlign 시 사용하는 Camera 번호 */
	int				m_iUseCameraNo;

} STabAlignerData;

/************************************************************************/
/*                           Position  Define                           */
/************************************************************************/
const	int DEF_TABALIGNER_AXIS_MAX_NO		= 2;
const	int	DEF_TABALIGNER_X1_AXIS			= 0;
const	int	DEF_TABALIGNER_X2_AXIS			= 1;

const	int	DEF_TABALIGNER_MAX_POS			= 3;
const	int	DEF_TABALIGNER_NO_WORKING_POS	= -1;
const	int	DEF_TABALIGNER_READY_POS		= 0;
const	int	DEF_TABALIGNER_ALIGN_POS		= 1;
const	int	DEF_TABALIGNER_RESERVED_POS		= 2;

/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

const	int	ERR_TABALIGNER_SUCCESS	= 0;

#endif //DEF_TABALIGNER_H
