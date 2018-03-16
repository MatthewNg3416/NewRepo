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
	TABMOUNTER_ALL_PREALIGN_COMPLETE_WAIT,		//12	// Gate System Only. : Gantry Move : Group ���� All Check.
	TABMOUNTER_MOVE_TO_INSPECTION_POS,			//13
	TABMOUNTER_INSPECTION_WAIT,					//14
	TABMOUNTER_MOVE_TO_MOUNT_POS,				//15
	TABMOUNTER_ALL_INSPECTION_COMPLETE_WAIT,	//16	// Gate System : Gantry Move : Group ���� All Check.
	TABMOUNTER_PREPARE_MOUNT_READY,				//
	TABMOUNTER_PREPARE_MOUNT,					//
	TABMOUNTER_MOUNT,							//
	TABMOUNTER_ALL_MOUNT_COMPLETE_WAIT,			//	// Group ���� All Check.
	TABMOUNTER_ALL_GROUP_MOUNT_COMPLETE_WAIT,	//	// All Group Check.
	TABMOUNTER_REMOVE_CHECK,					//	// Gate System �ʿ��� ��� Gantry Move(�ٷ� Load Pos �� �̵�)
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
	static BOOL			m_bITIExecuteWorking[DEF_MAX_GROUP];	//�ٸ� Unit�� �۾�������...
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


	// �۾��ϴ� Tab ��ȣ
	//   :	TabMounter �� �ش� Work Cycle �۾� ��, TABMOUNTER_MOVE_TO_LOAD_POS step ���� ���ư� ����
	//		�׻� DEF_NO_WORK ���� Clear �Ǿ�� �Ѵ�.
	int					m_iMounterTabNo;

/**********
 *  Threading
 */
	/** Thread ���� Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsTabMounter�� Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsTabMounter�� Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** AUTO View Window Handle */
// selsk_jdy	HWND				m_hAutoViewWnd;

	/** LogView Handle */
// selsk_jdy	HWND				m_hLogViewWnd;

	/** ���� Step ���� */
	EStepTabMounter		m_estepCurrent;

	/** �Ѵܰ� ���� Step ���� */
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

	/** �������� ������ : Critical Section
	 *	���� : Source System ������ ����Ѵ�. 
	 */
	static CRITICAL_SECTION	sm_csTrashBin;

	/** �������� ���� �� Error �߻� �� ��� ���� Flag */
	static BOOL			sm_bEmergencyStopForTrashBin;

/**********
 *  TabMounter ���� : TabMounter �� ���� ���� ����, TabMounter ���� ����
 */
	/** **************************************************************************
	 *	�ⱸ������ �����Ǵ� TabMounter ����
	 */

	/** TabMounter �� ���� Group ��ȣ */
	int					m_iGroupNo;

	/** �۾��� ���� */
	EWorkingSide		m_eWorkSide;

	/** **************************************************************************
	 *	����/���� TabMounter ��ȣ ����
	 *  �� ����/���� ���� ����
	 *		- �ڽź��� ���� Prealign ��ġ�� �̵��ϴ� TabMounter �� ���� TabMounter ��.
	 *		- Gate �� ��� TabCarrier Unload ��ġ�� ���� Front/Rear Group �� TabMounter ��� ��ġ�� �޶���.
	 */

	/** ���� TabMounter ��ȣ : 1�� TabMounter �� [���� TabMounter ��ȣ]�� -1 �̴�.
	 *	: TabMounter �� Mount ��ġ�� �̵��ϱ� ����, Previous TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int					m_iPreTabMounterNo;

	/** ���� TabMounter ��ȣ
	 *	: �� Group �� ������ TabMounter �� [���� TabMounter ��ȣ]�� DEF_LAST_PER_GROUP �̴�.
	 *	: TabMounter �� Load ��ġ�� �̵��ϱ� ����, Next TabMounter �� ��ġ�� Ȯ���ؾ� �Ѵ�.
	 */
	int					m_iNextTabMounterNo;
	/** **************************************************************************
	 *	TabMounter �۾� ���� ���� �� ������ ���� Flag �迭
	 *
	 *	** ��� ��Ģ :	TabMounter �� �۾� ���� ���� �����̹Ƿ�,
	 *					�ش� ���� �۾�(Error ó�� ����) �Ϸ� �Ŀ��� �����ϰ�,
	 *					All XX Complete Wait Step ������ Ȯ���Ѵ�.
	 */

	/** TabMounter �� �ش� Cycle �� ���� �۾� ���� ����
	 *	- ���⼭ �۾��� �Ѵٴ� �ǹ̴� TabMounter �� Panel ���� �߿� �̵��� �Ѵٴ� �ǹ��̴�.
	 *	  ����, Ư�� TabMounter �� ���� TabIC �� �����ϴ� ������ �������� �ʾƵ�
	 *	  �ٸ� TabMounter �� �̵��� ���ؼ� �̵��� �ؾ� �Ѵٸ�, �� TabMounter �� �۾��� �ϴ�
	 *	  TabMounter �� ���ֵǾ�� �Ѵ�.
	 */
	// �Ʒ� �÷��׵� struct �� ����? - �� step �� �Ϸ� flag �߰� ����.

	/** TabMounter �� Mount ������ �Ϸ��ߴ��� ���� ���� */
//	static BOOL			sm_rgbMountComplete[DEF_MAX_TABMOUNTER];

/**********
 *  ��Ÿ
 */
	/** Tab ��ȣ�� ���� �Բ� �۾��ϴ� Inspection Unit ��ȣ
	 *	����: Group �������� Index ���� ��.
	 *	�ӽ� ���� �������� ���.
	 */
	int					m_iInspectorUnitNoInGroup;

	/**	���� TabMounter �� ���� Group �������� TabMounter Index
	 */
//	int					m_iTabMounterNoInGroup;

	/** **************************************************************************
	 *	Panel �������� ���� �߻��ϴ� Offset �ݿ� ����.
	 */

	// Panel �� Inspection ��ġ�� ���� ��, Panel �� ���� Fidu Mark ��ġ Y ��.
	double				m_dPanelInspectPosFiduMarkPosY;
	// Panel �� Mount ��ġ�� ���� ��, Panel �� ���� Fidu Mark ��ġ Y ��.
	double				m_dPanelMountPosFiduMarkPosY;


	//===========================================
	MTickTimer			m_TimerError;
	MTabInfo			m_TabInfo;
	//Front -> 3�� Tool, Rear -> 4�� Tool
	//TabIC Loading�� Inspection�۾����۽� �ݴ��� Unit�� �۾����̸� ����ϰ�,
	//�۾����� �ƴϸ� m_bITIExecuteWorking �÷��׸� �����ϱ� ����
	UINT				m_uiRetryCount;
	int					m_iWorkInstance;
	int					m_iWorkGroup;//Front, Rear
	int					m_iOtherWorkGroup;
	int					m_iWorkCarrierNo;

	static BOOL			sm_bRemoveTab[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];//���� �ִ� TabIC�� ������...
	static BOOL			m_bLoadReady[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];
	static BOOL			m_bAllLoadReady[DEF_MAX_GROUP];
	static BOOL			m_bGroupInspectionComplete[DEF_MAX_GROUP];
	static BOOL			m_bGroupMountStart[DEF_MAX_GROUP];
	static BOOL			m_bGroupMountComplete[DEF_MAX_GROUP];
	static int			m_iFirstWorkToolNo[DEF_MAX_GROUP];
	static int			m_iKeyValue[DEF_MAX_GROUP];
	static int			sm_iPickoutGroup;//-1->�ʱ�ȭ, 0->Front, 1->Rear
	static int			sm_iPressGroup;//-1->�ʱ�ȭ, 0->Front, 1->Rear
	static int			sm_iWorkTabNo[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];//-1�̸� Press�۾����� ����
	static BOOL			sm_bCycleWorkComplete[DEF_MAX_GROUP][DEF_MOUNTER_WORK_PER_GROUP];//�۾����� �ʴ� Tab�ϰ�� TABMOUNTER_LOAD step�� �Ϸ���� �ʾ����� ���̴� ���� ������...
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
/* ������ & �Ҹ���
/*
/************************************************************************************************/

	MTrsTabMounter(SCommonAttribute commonData, STrsTabMounterRefCompList listRefComponents, STrsTabMounterData datComponent);

    ~MTrsTabMounter(void);  

/***********************************************************************************************
/*
/* Component ���� ���� Interface
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