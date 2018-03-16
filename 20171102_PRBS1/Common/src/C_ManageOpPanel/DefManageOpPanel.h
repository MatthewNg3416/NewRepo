/* 
 * ManageOpPanel Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * DefManageOpPanel.h : This file defines the structure.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef DEFMANAGEOPPANEL_H
#define DEFMANAGEOPPANEL_H

/**
 * Manage Op Panel Value Define
 */

/** Touch Panel ¾Õ/µÞ¸é ID define */
const	int DEF_MNGOPPANEL_NONE_PANEL_ID	= 0;

const	int	DEF_MNGOPPANEL_FRONT_PANEL_ID	= 1;
const	int DEF_MNGOPPANEL_BACK_PANEL_ID	= 2;

/** Jog °ü·Ã define */
const	int	DEF_MNGOPPANEL_JOG_NO_USE		= -1;

const	double	DEF_MNGOPPANEL_BLINK_RATE	= 0.5;

const	int	DEF_MNGOPPANEL_JOG_X_KEY		= 0;
const	int	DEF_MNGOPPANEL_JOG_Y_KEY		= 1;
const	int	DEF_MNGOPPANEL_JOG_T_KEY		= 2;
const	int	DEF_MNGOPPANEL_JOG_Z_KEY		= 3;

const	int DEF_MNGOPPANEL_NON_DIR			= -1;
const	int DEF_MNGOPPANEL_NEGATIVE_DIR		= 0;	// FALSE
const	int DEF_MNGOPPANEL_POSITIVE_DIR		= 1;	// TRUE

const	int	DEF_MNGOPPANEL_NON_KEY			= 0;
const	int	DEF_MNGOPPANEL_POS_KEY			= 1;
const	int	DEF_MNGOPPANEL_NEG_KEY			= 2;
const	int	DEF_MNGOPPANEL_ALL_KEY			= 3;

const	int	DEF_MNGOPPANEL_STEPSTOP			= 1;
const	int	DEF_MNGOPPANEL_START			= 2;
const	int	DEF_MNGOPPANEL_RUN				= 3;
const	int	DEF_MNGOPPANEL_STEPSTOP_ING		= 4;
const	int	DEF_MNGOPPANEL_ERRORSTOP_ING	= 5;
const	int	DEF_MNGOPPANEL_CYCLESTOP_ING	= 7;
const	int	DEF_MNGOPPANEL_OP_CALL			= 8;
const	int	DEF_MNGOPPANEL_PARTSEMPTY		= 9;
const	int	DEF_MNGOPPANEL_PANELEMPTY		= 10;

// NSMC
const	int DEF_MNGOPPANEL_AUTO_MODEL_CHANGE= 11;

// hongju_SESL
const	int DEF_MNGOPPANEL_PANEL_IN_STOP	= 12;

const	int	DEF_MNGOPPANEL_PANELFULL		= 13;
/**
 * Error Code Define
 */
const	int	ERR_MNGOPPANEL_SUCCESS			= 0;

#endif //DEFMANAGEOPPANEL_H

// End of File DefManageOpPanel.h
