/* 
 * TrsTabFeeder Component
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
 * MTrsTabFeeder.h : Headerfile of MTrsTabFeeder component.
 */

#ifndef TRSTABFEEDER_H
#define TRSTABFEEDER_H

#include "ICommon.h"
#include "MTickTimer.h"
#include "ITI_TabFeeder.h"

class MSystemData;
class MPanelData;
class MProcessData;
class MLCNet;
class MPreBonderData;
class MProductInfoData;
class MTabFeeder;
class MCtrlTabFeeder;
class ITI_TabCarrier;
class ITI_PanelAligner;
class MTrsAutoManager;

const int ERR_TRS_TABFEEDER_SUCCESS	= 0;

typedef
enum enumStepTabFeeder
{
	/* 0 */	TABFEEDER_REEL_AUTO_CHANGE_READY,
	/* 1 */	TABFEEDER_REEL_AUTO_CHANGE,
	/* 2 */	TABFEEDER_REEL_AUTO_CHANGE_END,
	/* 3 */	TABFEEDER_FEEDING_READY,
	/* 4 */	TABFEEDER_FEEDING,
	/* 5 */	TABFEEDER_FEEDING_END,
// selsk_jdy	/* 6 */	TABFEEDER_PRESSING_READY,
	/* 6 */	TABFEEDER_PRESS_DOWN,
	/* 7 */	TABFEEDER_PRESS_UP,
	/* 8 */	TABFEEDER_PRESS_END,
	/* 9 */ TABFEEDER_MODEL_CHANGE,

			TABFEEDER_WAIT_1FEEDERALLMOUNT_INIT,
			TABFEEDER_WAIT_FEEDER_READY,
} EStepTabFeeder;

typedef
struct tagSTrsTabFeederRefCompList
{
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder;
	MCtrlTabFeeder*		m_plnkCtrlOtherTabFeeder;
	MTabFeeder*			m_plnkTabFeeder;
	MLCNet*				m_plnkLCNet;
} STrsTabFeederRefCompList, *pSTrsTabFeederRefCompList;

typedef
struct tagSTabFeederRefITIList
{
//	ITI_TabCarrier*		m_plnkITITabCarrier;
	ITI_TabCarrier*		m_plnkITITabCarrier[DEF_MAX_TABCARRIER];
	ITI_PanelAligner*	m_plnkITIPanelAligner;
} STabFeederRefITIList, *pTabFeederRefITIList;

/** TrsTabFeeder Data */
typedef
struct tagSTrsTabFeederData
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MProcessData*		m_plnkProcessData;
	MPreBonderData*		m_plnkPreBonderData;
	MProductInfoData*	m_plnkProductInfoData;
	//eMatNG 12.08.14 yh
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;
} STrsTabFeederData, *pSTrsTabFeederData;


class MTrsTabFeeder : public ITI_TabFeeder, public virtual ICommon
{
private:

/**********
 *  Reference Components
 */
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder;
	MCtrlTabFeeder*		m_plnkCtrlOtherTabFeeder;
	MTabFeeder*			m_plnkTabFeeder;
	MLCNet*				m_plnkLCNet;

//	ITI_TabCarrier*		m_plnkITITabCarrier;
	ITI_TabCarrier*		m_plnkITITabCarrier[DEF_MAX_TABCARRIER];
	ITI_PanelAligner*	m_plnkITIPanelAligner;

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MProcessData*		m_plnkProcessData;
	MPreBonderData*		m_plnkPreBonderData;
	MProductInfoData*	m_plnkProductInfoData;
	//eMatNG 12.08.14 yh 
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;

/**********
 *  ITI Flag
 */
	BOOL				m_bITISendAbleToTC;
	BOOL				m_bITISendStartToTC;

/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsTabFeeder의 Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsTabFeeder의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** AUTO View Window Handle */
// selsk_jdy	HWND				m_hAutoViewWnd;

	/** 현재 Step 저장*/
	EStepTabFeeder		m_estepCurrent;

	/** 한단계 이전 Step 저장 */
	EStepTabFeeder		m_estepPrevious;

	/** ITI Time Check Timer */
	MTickTimer			m_Timer4ITI;

	// NSMC
	BOOL				m_bModelChangeReady;
	MTickTimer			tmMaterialCheck;

	BOOL				m_b1FeederAllMountInitReady;

	//120116.KMS___________
	BOOL                m_bMountAutoChangeReady[2];
/**********
 *  기타
 */
	/** Panel Aligner Unloading시 Recovery Control을 한번만 동작 시키기 위해 */
	BOOL				m_bRecoveryInit;

	int					m_iInstanceTabCarrier;//TabIC를 요구한 Carrier의 Instance번호

	BOOL				m_bPanelAlignerUnloadingPanel;//100930.KKY

	BOOL				m_bCurrentPanelBlowExt; //SJ_YYK 150317 Add..

#ifdef SIMULATION
	int					m_iPreStep;
#endif

private:
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

	void doRunStep();

	void initializeITI();

	static UINT threadFunction(LPVOID pParam);
	
    void threadJob();

	void setStep(EStepTabFeeder estepTabFeeder);

	void initializeStep();

	void simulLog(CString Msg, int iStep,	int iSleep = 0);

	BOOL isUseWorkGroup();

public:

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MTrsTabFeeder::MTrsTabFeeder(	SCommonAttribute			commonData, 
									STrsTabFeederRefCompList	listRefComponents, 
									STrsTabFeederData			datComponent );

    ~MTrsTabFeeder();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	int AssignComponents(STrsTabFeederRefCompList listRefComponents);

	int SetData(STrsTabFeederData datComponent);

	int Initialize();

	void ResetInitialized(void);
	void SetInitializedOK();//110322.Add

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 
//	void AssignITI(ITI_TabCarrier* pITI_TabCarrier, ITI_PanelAligner* pITI_PanelAligner);
	void AssignITI(tagSTabFeederRefITIList sITIList);

	BOOL IsInitialized(void);

	BOOL IsSendAbleToTC();
	BOOL IsSendStartToTC();

	// NSMC
	BOOL IsModelChangeReady();

	void SetPanelAlignerUnloadingpanel();//100930.KKY

	BOOL Is1FeederAllMountInitReady();

	//120116.KMS_________
	BOOL IsMountAutoChangeReady1();
	BOOL IsMountAutoChangeReady2();
/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

	void ThreadRun();
	void ThreadStop();

	int	GetStep();

	BOOL IsAutoRun();

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 
private:

	int getAssembleQty();

	BOOL checkUseFeeder();
};
#endif //TRSTABFEEDER_H
