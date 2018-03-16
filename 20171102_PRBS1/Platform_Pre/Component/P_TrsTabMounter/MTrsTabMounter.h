/* 
 * TrsTabMounter Component
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
 * MTrsTabMounter.h : Headerfile of MTrsTabMounter component.
 */

#ifndef TRSTABMOUNTER_H
#define TRSTABMOUNTER_H

#include "ICommon.h"
#include "ITI_TabMounter.h"
#include "MProcessData.h"
#include "MTickTimer.h"

class MSystemData;
class MTabInfo;
class MProcessData;
class IStatePanelTransfer;
class MCtrlTabMounter;
class ITI_PanelAligner;
class ITI_TabCarrier;
class ITI_InspectionCamera;
class MTrsAutoManager;
class MCtrlTabFeeder;
class MNetH;
class MTrsPanelAligner;
class MTrsInspectionCamera;
class MCameraCarrier;

const int ERR_TRS_TABMOUNTER_SUCCESS = 0;

const int MOVE_PERMIT	= 0;
const int MOVE_INHIBIT	= -1;
const int MOVE_SKIP		= -2;

typedef
enum enumStepTabMounter
{
	TABMOUNTER_MOVE_TO_LOAD_POS,				//0
	TABMOUNTER_PICKOUT_WORK,					//1
	TABMOUNTER_MOVE_LOAD_READY,					//2
	TABMOUNTER_MOVE_LOAD,						//3	
	TABMOUNTER_LOAD,							//4
	TABMOUNTER_MOVE_TO_ESCAPE_POS,				//5	//Only Gate			
	TABMOUNTER_ALL_LOAD_COMPLETE_WAIT,			//6	//Only Gate
	TABMOUNTER_WAIT_INSPECTION_READY,			//7
	TABMOUNTER_MOVE_TO_PREALIGN_POS,			//8
	TABMOUNTER_PREALIGN,						//9
	TABMOUNTER_REMOVE_AFTER_PREALIGN,			//10
	TABMOUNTER_MOVE_TO_READY_POS,				//11	// Gate System Only.
	TABMOUNTER_ALL_PREALIGN_COMPLETE_WAIT,		//12	// Gate System Only. : Gantry Move : Group 단위 All Check.
	TABMOUNTER_MOVE_TO_INSPECTION_POS,			//13
	TABMOUNTER_INSPECTION_WAIT,					//14
	TABMOUNTER_MOVE_TO_MOUNT_POS,				//15
	TABMOUNTER_ALL_INSPECTION_COMPLETE_WAIT,	//16	// Gate System : Gantry Move : Group 단위 All Check.
	TABMOUNTER_PREPARE_MOUNT_READY,				//
	TABMOUNTER_PREPARE_MOUNT,					//
	TABMOUNTER_MOUNT,							//
	TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT,			//	// Group 단위 All Check.
	TABMOUNTER_ALL_GROUP_MOUNT_COMPLETE_WAIT,	//	// All Group Check.
	TABMOUNTER_REMOVE_CHECK,					//	// Gate System 필요한 경우 Gantry Move(바로 Load Pos 로 이동)
	TABMOUNTER_MOVE_TO_READY_POS_AFTER_WORKDONE,//
	TABMOUNTER_MOVE_TO_READY_POS_FOR_REMOVE,	//	// Gate System Only.
	TABMOUNTER_REMOVE_WAIT,						//	// Gantry Move
	TABMOUNTER_MODEL_CHANGE,					//
	TABMOUNTER_MOVE_FOR_REMOVE_READY,			//
	TABMOUNTER_REMOVE_TAB,						//
	TABMOUNTER_WAIT_REMOVE_COMPLETE,			//
	TABMOUNTER_READY_MOVE_FOR_FEEDERREADY,		//
	TABMOUNTER_WAIT_1FEEDERALLMOUNT_INIT,		//

} EStepTabMounter;

typedef
struct tagSTrsTabMounterRefCompList
{
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlTabMounter*	m_plnkCtrlTabMounter;
	MTabMounter*		m_plnkTabMounter;
	MNetH*				m_plnkMelsecIF;
	MCameraCarrier*		m_plnkCameraCarrier;

} STrsTabMounterRefCompList, *pSTrsTabMounterRefCompList;

typedef
struct tagSTabMounterRefITIList
{
	ITI_PanelAligner*		m_plnkITI_PanelAligner;
	ITI_TabCarrier*			m_plnkITI_TabCarrier[DEF_MAX_TABCARRIER];
	ITI_InspectionCamera*	m_rgplnkITI_InspectionCamera[DEF_MAX_INSPECTION_CAM_PER_GROUP];
} STabMounterRefITIList, *pTabMounterRefITIList;

/** TrsTabMounter Data */
typedef
struct tagSTrsTabMounterData
{
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;

} STrsTabMounterData, *pSTrsTabMounterData;

/** Process Layer List */
typedef
struct  tagSTrsTabMounterRefProcess
{
	MCtrlTabFeeder*					m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];
	MTrsPanelAligner*				m_plnkTrsPanelAligner;
	MTrsInspectionCamera*			m_plnkTrsInspectionCarrier;
} STrsTabMounterRefProcess;

class MTrsTabMounter : public ITI_TabMounter, public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	ITI_PanelAligner*		m_plnkITI_PanelAligner;
	ITI_TabCarrier*			m_plnkITI_TabCarrier[DEF_MAX_TABCARRIER];
	ITI_InspectionCamera*	m_rgplnkITI_InspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];

	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlTabMounter*	m_plnkCtrlTabMounter;
	MTabMounter*		m_plnkTabMounter;
	MNetH*				m_plnkMelsecIF;
	MCameraCarrier*		m_plnkCameraCarrier;

	MCtrlTabFeeder*			m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];
	MTrsPanelAligner*		m_plnkTrsPanelAligner;
	MTrsInspectionCamera*	m_plnkTrsInspectionCarrier;
/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
//@	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

/**********
 *  ITI Flag
 */
	//===========================================
	static BOOL			m_bITIExecuteWorking[DEF_MAX_GROUP];	//다른 Unit이 작업중인지...
	//===========================================


	BOOL				m_bITITabICReceiveStartToTC;
	BOOL				m_bITITabICReceiveCompleteToTC;
	BOOL				m_bITIInspectionAbleToIC;
	BOOL				m_bITIInspectionWaitStartToIC;
	BOOL				m_bITIInspectionMountReadyWaitToIC; //170905 JSH
	static BOOL			sm_bITIAllInspectionCompleteToPA[DEF_MAX_GROUP];
	static BOOL			sm_bITIAllMountCompleteToPA[DEF_MAX_GROUP];
	static BOOL			sm_bITIAllInspectionCompleteToPASecond[DEF_MAX_GROUP];
	static BOOL			sm_bITIAllMountCompleteToPASecond[DEF_MAX_GROUP];
	
	BOOL				m_bITITabMounterReadyPusherDownToPA;	//@130204.KKY

	//TrsCarrier Remove_______________
	BOOL m_bITITabICRemoveRequestToTC;
	BOOL m_bITITabICRemoveReadyToTC;
	BOOL m_bITITabICRemoveCompleteToTC;

	//TrsCarrier TabIC Reload
	BOOL m_bITITabICReloadRequestToTC;


	// 작업하는 Tab 번호
	//   :	TabMounter 가 해당 Work Cycle 작업 후, TABMOUNTER_MOVE_TO_LOAD_POS step 으로 돌아갈 때는
	//		항상 DEF_NO_WORK 으로 Clear 되어야 한다.
	int					m_iMounterTabNo;

/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsTabMounter의 Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsTabMounter의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** AUTO View Window Handle */
// selsk_jdy	HWND				m_hAutoViewWnd;

	/** LogView Handle */
// selsk_jdy	HWND				m_hLogViewWnd;

	/** 현재 Step 저장 */
	EStepTabMounter		m_estepCurrent;

	/** 한단계 이전 Step 저장 */
	EStepTabMounter		m_estepPrevious;

	// NSMC
	BOOL				m_bModelChangeReady;

	BOOL				m_b1FeederAllMountInitReady;

	//120116.kms__________
	BOOL				m_bMountAutoChangeReady[2];

#ifdef SIMULATION
	int					m_iPreStep;
#endif
	
	BOOL				m_bTabMounterVacOn; //161221 SJ_YSH

	/** 쓰레기통 점유권 : Critical Section
	 *	주의 : Source System 에서만 사용한다. 
	 */
	static CRITICAL_SECTION	sm_csTrashBin;

	/** 쓰레기통 점유 중 Error 발생 시 비상 정지 Flag */
	static BOOL			sm_bEmergencyStopForTrashBin;

/**********
 *  TabMounter 정보 : TabMounter 간 상태 정보 공유, TabMounter 역할 저장
 */
	/** **************************************************************************
	 *	기구적으로 결정되는 TabMounter 정보
	 */

	/** TabMounter 가 속한 Group 번호 */
	int					m_iGroupNo;

	/** 작업변 정보 */
	EWorkingSide		m_eWorkSide;

	/** **************************************************************************
	 *	이전/다음 TabMounter 번호 설정
	 *  ⇒ 이전/다음 구분 기준
	 *		- 자신보다 먼저 Prealign 위치로 이동하는 TabMounter 가 이전 TabMounter 임.
	 *		- Gate 의 경우 TabCarrier Unload 위치에 따라 Front/Rear Group 의 TabMounter 출발 위치가 달라짐.
	 */

	/** 이전 TabMounter 번호 : 1번 TabMounter 의 [이전 TabMounter 번호]는 -1 이다.
	 *	: TabMounter 가 Mount 위치로 이동하기 전에, Previous TabMounter 의 위치를 확인해야 한다.
	 */
	int					m_iPreTabMounterNo;

	/** 다음 TabMounter 번호
	 *	: 각 Group 의 마지막 TabMounter 의 [다음 TabMounter 번호]는 DEF_LAST_PER_GROUP 이다.
	 *	: TabMounter 가 Load 위치로 이동하기 전에, Next TabMounter 의 위치를 확인해야 한다.
	 */
	int					m_iNextTabMounterNo;
	/** **************************************************************************
	 *	TabMounter 작업 상태 설정 및 공유를 위한 Flag 배열
	 *
	 *	** 사용 원칙 :	TabMounter 들 작업 상태 공유 목적이므로,
	 *					해당 관련 작업(Error 처리 포함) 완료 후에만 설정하고,
	 *					All XX Complete Wait Step 에서만 확인한다.
	 */

	/** TabMounter 의 해당 Cycle 에 대한 작업 여부 저장
	 *	- 여기서 작업을 한다는 의미는 TabMounter 가 Panel 생산 중에 이동을 한다는 의미이다.
	 *	  따라서, 특정 TabMounter 가 직접 TabIC 를 압착하는 역할을 수행하지 않아도
	 *	  다른 TabMounter 의 이동을 위해서 이동을 해야 한다면, 그 TabMounter 도 작업을 하는
	 *	  TabMounter 로 간주되어야 한다.
	 */
	// 아래 플래그들 struct 로 관리? - 매 step 별 완료 flag 추가 관리.

	/** TabMounter 가 Mount 동작을 완료했는지 여부 저장 */
//	static BOOL			sm_rgbMountComplete[DEF_MAX_TABMOUNTER];

/**********
 *  기타
 */
	/** Tab 번호에 따라 함께 작업하는 Inspection Unit 번호
	 *	주의: Group 내에서의 Index 여야 함.
	 *	임시 저장 공간으로 사용.
	 */
	int					m_iInspectorUnitNoInGroup;

	/**	현재 TabMounter 가 속한 Group 내에서의 TabMounter Index
	 */
//	int					m_iTabMounterNoInGroup;

	/** **************************************************************************
	 *	Panel 흡착으로 인해 발생하는 Offset 반영 관련.
	 */

	// Panel 이 Inspection 위치에 있을 때, Panel 의 기준 Fidu Mark 위치 Y 값.
	double				m_dPanelInspectPosFiduMarkPosY;
	// Panel 이 Mount 위치에 있을 때, Panel 의 기준 Fidu Mark 위치 Y 값.
	double				m_dPanelMountPosFiduMarkPosY;


	//===========================================
	MTickTimer			m_TimerError;
	MTabInfo			m_TabInfo;
	//Front -> 3번 Tool, Rear -> 4번 Tool
	//TabIC Loading후 Inspection작업시작시 반대편 Unit이 작업중이면 대기하고,
	//작업중이 아니면 m_bITIExecuteWorking 플래그를 셋팅하기 위해
	UINT				m_uiRetryCount;
	int					m_iWorkInstance;
	int					m_iWorkGroup;//Front, Rear
	int					m_iOtherWorkGroup;
	int					m_iWorkCarrierNo;

	static BOOL			sm_bRemoveTab[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];//물고 있는 TabIC를 버릴지...
	static BOOL			m_bLoadReady[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];
	static BOOL			m_bAllLoadReady[DEF_MAX_GROUP];
	static BOOL			m_bGroupInspectionComplete[DEF_MAX_GROUP];
	static BOOL			m_bGroupMountStart[DEF_MAX_GROUP];
	static BOOL			m_bGroupMountComplete[DEF_MAX_GROUP];
	static int			m_iFirstWorkToolNo[DEF_MAX_GROUP];
	static int			m_iKeyValue[DEF_MAX_GROUP];
	static int			sm_iPickoutGroup;//-1->초기화, 0->Front, 1->Rear
	static int			sm_iPressGroup;//-1->초기화, 0->Front, 1->Rear
	static int			sm_iWorkTabNo[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];//-1이면 Press작업하지 않음
	static BOOL			sm_bCycleWorkComplete[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];//작업하지 않는 Tab일경우 TABMOUNTER_LOAD step이 완료되지 않았을때 꼬이는 현상 때문에...
	static BOOL			sm_bGroupLoadComplete[DEF_MAX_GROUP];
	static BOOL			m_bNextPosCheckComplete[DEF_MAX_GROUP];

	//===========================================

	static BOOL			m_bPressCompleteSchedule[DEF_MAX_GROUP];
	static int			m_iScheduleNo[DEF_MAX_GROUP];

private:
	BOOL	isGroupStarter();
	BOOL	isAllLoadReady();
	BOOL	isAllCycleWorkComplete();
	BOOL	isExistRemoveTab();
	BOOL	isLoadCompleteOtherGroup();
	BOOL	isPressCompleteOtherGroup();
	BOOL	isExistNextLoadTab();
	BOOL	is1FeederAllMounterInitStart();
	BOOL	isTabCarrierOtherGroup();
	int		getLoadPosID();
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

	void doRunStep(void);

	void initializeITI(void);

	static UINT threadFunction(LPVOID pParam);

    void threadJob(void);

	void setStep(EStepTabMounter estepTabMounter);

	void initializeStep(void);

	void simulLog(CString Msg, int iStep, int iSleep = 200);

	int checkGantryState(int iGantryPosID);

	// sesl_jdy
	int prepareMountWork();

	int checkBackupState();
public:
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MTrsTabMounter(SCommonAttribute commonData, STrsTabMounterRefCompList listRefComponents, STrsTabMounterData datComponent);

    ~MTrsTabMounter(void);  

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	void AssignComponents(STrsTabMounterRefCompList listRefComponents);

	void SetData(STrsTabMounterData datComponent);

	int Initialize();

	void ResetInitialized(void)
	{
		m_bAutoInitialized = FALSE;
	}

	void AssignProcess(STrsTabMounterRefProcess listRefTrs);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 

	void AssignITI(STabMounterRefITIList sITIList);

	BOOL IsTabICReceiveStartToTC(void)
	{
		return m_bITITabICReceiveStartToTC;
	}

	BOOL IsTabICReceiveCompleteToTC(void)
	{
		return m_bITITabICReceiveCompleteToTC;
	}

	BOOL IsInspectionAbleToIC(void)
	{
		return m_bITIInspectionAbleToIC;
	}

	BOOL IsInspectionWaitStartToIC(void)
	{
		return m_bITIInspectionWaitStartToIC;
	}

	//170905 JSH
	BOOL IsInspectionMountReadyWaitToIC(void)
	{
		return m_bITIInspectionMountReadyWaitToIC;
	}

	BOOL IsAllInspectionCompleteToPA(int iWorkGroup)
	{
		return sm_bITIAllInspectionCompleteToPA[iWorkGroup];
	}
	BOOL IsAllMountCompleteToPA(int iWorkGroup)
	{
		return sm_bITIAllMountCompleteToPA[iWorkGroup];
	}

	BOOL IsAllInspectionCompleteToPASecond(int iWorkGroup)
	{
		return sm_bITIAllInspectionCompleteToPASecond[iWorkGroup];
	}
	BOOL IsAllMountCompleteToPASecond(int iWorkGroup)
	{
		return sm_bITIAllMountCompleteToPASecond[iWorkGroup];
	}

	//@130204.KKY____________
	BOOL IsTabMounterReadyPusherDownToPA()
	{
		return m_bITITabMounterReadyPusherDownToPA;
	}
	//@______________________

	int GetMountTabNo(void)
	{
		//return m_iMounterTabNo;
		return m_TabInfo.m_nTabNo;
	}

	//TrsCarrier Remove_______________
	BOOL IsTabICRemoveRequestToTC()
	{
		return m_bITITabICRemoveRequestToTC;
	}
	BOOL IsTabICRemoveReadyToTC()
	{
		return m_bITITabICRemoveReadyToTC;
	}
	BOOL IsTabICRemoveCompleteToTC()
	{
		return m_bITITabICRemoveCompleteToTC;
	}
	//TrsCarrier TabIC Reload
	BOOL IsTabICReloadRequestToTC()
	{
		return m_bITITabICReloadRequestToTC;
	}

	BOOL IsPressCompleteSchedule(int iWorkGroup)
	{
		return m_bPressCompleteSchedule[iWorkGroup];
	}


	// NSMC
	BOOL IsModelChangeReady();

	BOOL Is1FeederAllMountInitReady();

	//120116.kms_________
	BOOL IsMountAutoChangeReady1();
	BOOL IsMountAutoChangeReady2();

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

	void ThreadRun(void);

	void ThreadStop(void);

	int GetStep(void)
	{
		return m_estepCurrent;
	}

	BOOL IsAutoRun(void)
	{
		return m_bAutoRun;
	}

	BOOL IsInitialized(void)
	{
		return m_bAutoInitialized;
	}

/***********************************************************************************************
/*										ETC														*/
/************************************************************************************************/ 
	int GetTabCarrierNo();

	static BOOL IsLoadComplete(int iWorkGroup);
	
	//20131115 SJ_HJG
	double			m_dMountInspectionDown;
	double			m_dMountInspectionTime;
	double			m_dMountInspectionUp;
	double			m_dMountMove;
	double			m_dMountBondingDown;
	double			m_dMountBondingTime;
	double			m_dMountBondingUp;

	MTickTimer		m_TMountParametertimer;

	//@___________________________
	int GetWorkScheduleNo(int iGroupNo) { return m_iScheduleNo[iGroupNo]; }
	void SetWorkScheduleNo(int iGroupNo, int iWorkNo) { m_iScheduleNo[iGroupNo] = iWorkNo; }
	//@___________________________
};
#endif //TRSTABMOUNTER_H