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
 * DefHandlerCommon.h : This file defines the value of Handler component.
 *
 */

#ifndef DEF_HANDLER_COMMON_H
#define DEF_HANDLER_COMMON_H

#include "IIO.h"
// kss 1. cylinder 없음 삭제 

#include "IVacuum.h"
#include "IAxis.h"
//@#include "MLog.h"
#include "MTeachingPosData.h"
#include "MSystemData.h"
#include "MPanelData.h"

class MNetH;


/************************************************************************/
/*                             Value Define                             */
/************************************************************************/

/** Handler Elements Max No. */
const	int DEF_HANDLER_PANEL_OFFSET						= 36;

/** Handler Axis Order */
const	int	DEF_HANDLER_X_AXIS								= 0;
const	int	DEF_HANDLER_Y_AXIS								= 1; 
const	int	DEF_HANDLER_T_AXIS								= 2;
const	int	DEF_HANDLER_Z_AXIS								= 3; 
// kss 2. Z축을 제외한 나머지 축에 대한 Max값을 써야한다. 4->3
const	int DEF_HANLDER_MAX_AXIS							= 3;	

/** Hanlder Type Define */
const	int DEF_HANDLER_LOADING								= 0;
const	int DEF_HANDLER_UNLOADING							= 1;

/************************************************************************/
/*                           Position  Define                           */
/************************************************************************/

/** Handler Working Position Index Define */
const	int	DEF_HANDLER_NO_WORKING_POS						= -1;
const	int	DEF_HANDLER_LOAD_POS							= 0;
const	int	DEF_HANDLER_UNLOAD_POS							= 1;
const	int	DEF_HANDLER_TURN_POS							= 2;
const	int DEF_HANDLER_MAX_POS								= 3;

//Z축
const	int DEF_HANDLER_Z_UP_POS							= 0;
const	int DEF_HANDLER_Z_DOWN_POS							= 1;
//const	int DEF_HANDLER_Z_LOAD_DOWN_POS						= 1;
//const	int DEF_HANDLER_Z_UNLOAD_DOWN_POS					= 2;
const	int DEF_HANDLER_MAX_Z_POS							= 2;

/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/

const	int	ERR_HANDLER_SUCCESS								= 0;

const	int	DEF_HANDLER_TURN_POS_T_TOLERANCE				= 5;

const	double DEF_DISTANCE_USE_VACUUM1						= 720.0;
const	double DEF_DISTANCE_USE_VACUUM2						= 900.0;
const	double DEF_40_INCH_PANEL_SIZE_X						= 900.0;
//90421.KKY_____
const	double DEF_46_INCH_PANEL_SIZE_X						= 1000.0;
//______________


/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/
typedef
/**
 * Referenced Component List by Handler Component
 */
struct tagSHandlerRefCompList
{
	/** I/O Object */
	IIO*			m_plnkIO;
	
	// kss 1. cylinder 없음 삭제 
	
	/** Vacuum Line Object - Panel Vacuum (single : 1) */
//@	IVacuum*		m_plnkVac1;
	/** Vacuum Line Object - Panel Vacuum (single : 2) */
//@	IVacuum*		m_plnkVac2;
	/** Vacuum Line Object - Panel Vacuum (single : 3) */
//@	IVacuum*		m_plnkVac3;

	IVacuum*		m_plnkVac[DEF_MAX_THANDLER_VAC];
	
	/** X Axis Object */
    IAxis*			m_plnkXAxis;

	/** Y Axis Object */
    IAxis*			m_plnkYAxis;

	/** T Axis Object */
    IAxis*			m_plnkTAxis;

	/** Z Axis Object */
    IAxis*			m_plnkZAxis;

	MNetH*			m_plnkMelsecIF;

} SHandlerRefCompList;

#endif //DEF_HANDLER_COMMON_H
