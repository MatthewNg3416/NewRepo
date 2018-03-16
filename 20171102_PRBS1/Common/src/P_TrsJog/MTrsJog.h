/* 
 * MTrsJog Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MTrsJog.h : Headerfile of MTrsJog component.
 */

#ifndef	__MTRAYJOG_H
#define __MTRAYJOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ICommon.h"

const int ERR_TRS_JOG_SUCCESS = 0;

class MManageOpPanel;
class MTrsAutoManager;

class MTrsJog : public virtual ICommon
{
private:
	/** Thread 구동 Flag*/
	BOOL			m_flgThreadLife;
	BOOL			m_bMngoppanelPanelEmpty; //170920 JSH

	/** Thread Handle */
	CWinThread*		m_pThread;

	MManageOpPanel* m_plnkMngOpPanel;
	MTrsAutoManager* m_plnkTrsAutoManager;
	MTowerLampData* m_plnkTowerLampData;	//20170920 KDW Add
	MTickTimer		m_timer;				//20170920 KDW Add


/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/

	void threadJob();
	static UINT	threadFunction(LPVOID pParam);
	int processTowerLamp();

public:

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
	MTrsJog(SCommonAttribute commonData, MManageOpPanel* pMngOpPanel);

	virtual ~MTrsJog();
	
/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	
	int AssignAutoManager(MTrsAutoManager* pAutoManager);

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 
	void ThreadRun();
	void ThreadStop();
};

#endif // ifndef __MJOGTHREAD_H
