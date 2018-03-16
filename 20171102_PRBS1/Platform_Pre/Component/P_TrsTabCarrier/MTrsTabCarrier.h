/* 
 * TrsTabCarrier Component
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
 * MTrsTabCarrier.h : Headerfile of MTrsTabCarrier component.
 */

#ifndef TRSTABCARRIER_H
#define TRSTABCARRIER_H

#include "ICommon.h"
#include "MTickTimer.h"
#include "ITI_TabCarrier.h"
#include "DefTabCarrier.h"
#include "MPos_XYT.h"
#include "afxmt.h"
#include "MProcessData.h"

class MSystemData;
class MProcessData;
class MPreBonderData;//2009.04.04 SESL_CYJ
class MTabCarrier;
class IStatePanelAligner;
class MCtrlTabCarrier;
class MTrsAutoManager;
class ITI_TabFeeder;
class ITI_TabMounter;
class MCtrlTabFeeder;
class CDBTabLoss;
class MNetH;

const int ERR_TRS_TABCARRIER_SUCCESS = 0;

typedef
enum enumStepTabCarrier
{
	TABCARRIER_WAIT_JOB,							//0
	TABCARRIER_LOADINGREADY_MOVE,					//1
    TABCARRIER_LOADING_MOVE,						//2
    TABCARRIER_LOADING,								//3
	TABCARRIER_MOVE_PREALIGN_POS,					//4
	TABCARRIER_PREALIGN,							//5
	TABCARRIER_UNLOADING_MOVE_READY,				//6	//Gate에서만
    TABCARRIER_UNLOADING_MOVE,						//7
    TABCARRIER_UNLOADING,							//8
	TABCARRIER_MOVE_ESPACE_POS,						//9
	TABCARRIER_WAIT_UNLOADING_COMPLETE_GROUP,		//10
	TABCARRIER_CHECK_SUPPLY_COMPLETE,				//11
    TABCARRIER_MODEL_CHANGE,						//12
	TABCARRIER_READY_MOVE_FOR_FEEDERREADY,			//13
	TABCARRIER_WAIT_1FEEDERALLMOUNT_INIT,			//14
	TABCARRIER_WAIT_FEEDERREADY,					//15

} EStepTabCarrier;

typedef
struct tagSTrsTabCarrierRefCompList
{
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlTabCarrier*	m_plnkCtrlTabCarrier;
	MTabCarrier*		m_plnkTabCarrier;
	IStatePanelAligner* m_plnkPanelAligner;
	MNetH*				m_plnkMelsecIF2;

} STrsTabCarrierRefCompList, *pSTrsTabCarrierRefCompList;

typedef
struct tagSTabCarrierRefITIList
{
	ITI_TabFeeder*			m_plnkITITabFeeder;
	ITI_TabMounter*			m_plnkITITabMounter[DEF_MAX_TABMOUNTER];
} STabCarrierRefITIList, *pTabCarrierRefITIList;

/** TrsTabCarrier Data */
typedef
struct tagSTrsTabCarrierData
{
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;	
	MPreBonderData*		m_plnkPreBonderData; // 2009.04.04 SESL_CYJ
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;
	CDBTabLoss*			m_plnkDBTabLoss;
	/** 작업변 */
	EWorkingSide		m_eWorkSide;

} STrsTabCarrierData, *pSTrsTabCarrierData;

typedef struct tagSTrsTabCarrierReloadInfo
{	
	BOOL	bRegistered;
	int		iInstanceNo;
} STrsTabCarrierReloadInfo;

/** Process Layer List */
typedef
struct  tagSTrsTabCarrierRefProcess
{
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];
} STrsTabCarrierRefProcess;

class MTrsTabCarrier : public ITI_TabCarrier,  public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	MTrsAutoManager*		m_plnkTrsAutoManager;

	ITI_TabFeeder*			m_plnkITITabFeeder;
	ITI_TabMounter*			m_plnkITITabMounter[DEF_MAX_TABMOUNTER];

	MCtrlTabCarrier*	m_plnkCtrlTabCarrier;
	MTabCarrier*		m_plnkTabCarrier;
	IStatePanelAligner* m_plnkPanelAligner;
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];

	MNetH*				m_plnkMelsecIF2;

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MPreBonderData*		m_plnkPreBonderData; // 2009.04.04 SESL_CYJ
//@	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];
	CDBTabLoss*			m_plnkDBTabLoss;

/**********
 *  ITI Flag
 */
	//111229 Add_____________
	//Unloading위치에서 Loading위치로이동시 미리 타발하도록
	BOOL m_bTabICReceiveAbleToTFRequest;
	//_______________________

	BOOL m_bTabICReceiveAbleToTF;
	BOOL m_bTabICReceiveStartToTF;
	BOOL m_bTabICReceiveCompleteToTF;

	BOOL m_bTabICSendAbleToTM;
	BOOL m_bTabICSendStartToTM;
	BOOL m_bLoadingMoveCompleteToTM;//110415.Add
	BOOL m_bLoadingCompleteToTM;	//@130114.Add
	BOOL m_bUnloadingMoveCompleteToTM;

	BOOL m_bTabICTrash_ReceiveAbleToTM;
	BOOL m_bTabICTrash_ReceiveCompleteToTM;

	// NSMC
	BOOL				m_bModelChangeReady;

	BOOL				m_b1FeederAllMountInitReady;

	//120116.kms_______
	BOOL m_bMountAutoChangeReady[2];
/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsTabCarrier의 Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsTabCarrier의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	BOOL				m_bTabCarrierVacOn; //161221 SJ_YSH

	/** 현재 Step 저장 */
	EStepTabCarrier		m_estepCurrent;

	/** 한단계 이전 Step 저장*/
	EStepTabCarrier		m_estepPrevious;

	//100728.충돌확인 이전 step저장_____
	MPos_XYT			m_posAlignOffset;
	static STrsTabCarrierReloadInfo	m_sReloadInfo[DEF_MAX_GROUP];
	static CCriticalSection	m_csLock[DEF_MAX_GROUP];
	//__________________________________

	static BOOL			m_bLoadReady[DEF_MAX_GROUP][DEF_MAX_TABCARRIER];
	static BOOL			m_bAllLoadReady[DEF_MAX_GROUP];
	static BOOL			m_bUnloadComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER];
	static BOOL			m_bAllUnloadComplete[DEF_MAX_GROUP];
	static BOOL			m_bGroupSupplyComplete[DEF_MAX_GROUP];
	//110223_____
	static BOOL			m_bLoadMoveComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER];
	static BOOL			m_bAllLoadMoveComplete[DEF_MAX_GROUP];
	//___________
	//SJ_YYK 150109 Add....
	static BOOL			m_bUnloadingReady[DEF_MAX_GROUP][DEF_MAX_TABCARRIER];
	static BOOL			m_bAllUnloadingReady[DEF_MAX_GROUP];
	static BOOL			m_bPrealignComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER];
	//______________________________________*/
	static BOOL			m_bLoadingComplete[DEF_MAX_GROUP][DEF_MAX_TABCARRIER]; //SJ_YYK 150908 Add..
	static BOOL			m_bTabICUnloadToTabMounter[DEF_MAX_GROUP][DEF_MAX_TABCARRIER]; //170830 JSH TabIC를 진짜 전달하였는지 확인하는 Bit

	int					m_iWorkTabNo;
	int					m_iWorkInstance;
	int					m_iWorkGroup;//Front, Rear
	static int			m_iWorkGroupMounter[DEF_MAX_GROUP];

	int					m_iAOCUnitNo; //SJ_YYK 150817 Add...

	int					m_iOtherWorkGroup;
	int					m_iWorkToolNo;
	BOOL				m_bSupplyTab;

	/** 1 Cycle 동안 Load 하는 Tab 들에 대한 Tab 번호 저장 배열 (for display) */
//	int					m_rgiWorkTabICNo[DEF_MAX_WORKER_PER_GROUP];

/**********
 *  기타
 */
	/** retry timer */
	MTickTimer			m_Timer4Retry;

	MTickTimer			m_TimerError;

	MTickTimer			m_timer;

	UINT				m_uiRetryCount;

	/** Tact Time Log Message	*/
// jdy sesl	char				m_szLogMsg[250];

#ifdef SIMULATION
	int					m_iPreStep;
#endif

	/** 작업변 */
	EWorkingSide		m_eWorkSide;

	static	int			m_iScheduleNo[DEF_MAX_GROUP];

private:
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

	void doRunStep();

	void initializeITI();

	void initializeMelsecAOCIF(); //SJ_YYK 150913 Add...

	static UINT threadFunction(LPVOID pParam);

    void threadJob();

	void setStep(EStepTabCarrier estepTabCarrier);

	void initializeStep();

	void simulLog(CString Msg, int iStep, int iSleep = 0);

	int getVacPosID(int iTabNo);

	//===============================================================
	//Prealign Error 또는 Unloading위치 이동후 TabIC미흡착시 등록하여 준다
	void registerReload();
	void freeReload();
	BOOL isExistReload();
	//Carrier가 동작중 Prealign마크 불량 또는 Unloading위치에서 TabIC없을경우
	//다시 TabLoading하기 위해 다른 Carrier들이 비켜주는 동작
	int	 escapeForReload();

	BOOL	isGroupStarter();
	BOOL	isAllLoadReady();
	BOOL	isAllUnloadComplete();
	BOOL	isAllSupplyComplete();
	BOOL	isAllLoadMoveComplete();
	BOOL	is1FeederAllMounterInitStart();
	BOOL	isAllUnloadingReady(); //SJ_YYK 150109 Add..
	//===============================================================

	//@140311.KKY_____________
	int		moveAll_Unload_To_LoadReady();
	//@_______________________

public:

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MTrsTabCarrier::MTrsTabCarrier(	SCommonAttribute			commonData, 
									STrsTabCarrierRefCompList	listRefComponents, 
									STrsTabCarrierData			datComponent );

    ~MTrsTabCarrier();  

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	int AssignComponents(STrsTabCarrierRefCompList listRefComponents);

	int SetData(STrsTabCarrierData datComponent);

	int Initialize();

	void ResetInitialized();

	void AssignProcess(STrsTabCarrierRefProcess listRefTrs);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 
	void AssignITI(STabCarrierRefITIList sITIList);

	BOOL IsInitialized(void);

	//111229 Add_____________
	//Unloading위치에서 Loading위치로이동시 미리 타발하도록
	BOOL IsTabICReceiveAbleToTFRequest();
	//_______________________
	BOOL IsTabICReceiveAbleToTF();
	BOOL IsTabICReceiveStartToTF();
	BOOL IsTabICReceiveCompleteToTF();

	//TabMounter
	BOOL IsTabICSendAbleToTM();
	BOOL IsTabICSendStartToTM();
	BOOL IsLoadingMoveCompleteToTM();
	BOOL IsLoadingCompleteToTM();
	BOOL IsUnloadingMoveCompleteToTM();
	void SetTabNo(int iRealTabNo);//TabMounter가 Carrier에서 TabLoading전 Tab번호를 지정하면 TabCarrier가 TabOffset만큼 이동

	BOOL IsTabICTrash_ReceiveAbleToTM();
	BOOL IsTabICTrash_ReceiveCompleteToTM();

	// NSMC
	BOOL IsModelChangeReady();

	BOOL Is1FeederAllMountInitReady();

	//120116.kms__________
	BOOL IsMountAutoChangeReady1();
	BOOL IsMountAutoChangeReady2();
/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

	void ThreadRun();

	void ThreadStop();

	int GetStep()
	{
		return m_estepCurrent;
	}

	BOOL IsAutoRun()
	{
		return m_bAutoRun;
	}

/***********************************************************************************************
/*										ETC														*/
/************************************************************************************************/ 
	int GetTabMounterNo();
	int	GetWorkTabICNo();
	void InitialWorkStep();

	//@___________________________
	int GetWorkScheduleNo(int iGroupNo) { return m_iScheduleNo[iGroupNo]; }
	void SetWorkScheduleNo(int iGroupNo, int iWorkNo) { m_iScheduleNo[iGroupNo] = iWorkNo; }
	//@___________________________

	int GetGroupStarter();
	int GetNextGroupStarter();

	void SetTabICUnloadToTabMounter(int iGroupNo, int iWorkNo, BOOL Data);
};
#endif //TRSTABCARRIER_H
