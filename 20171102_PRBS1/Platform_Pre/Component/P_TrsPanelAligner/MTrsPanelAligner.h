/* 
 * TrsPanelAligner Component
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
 * MTrsPanelAligner.h : Headerfile of MTrsPanelAligner component.
 */

#ifndef TRS_PANEL_ALIGNER_H
#define TRS_PANEL_ALIGNER_H

#include "ICommon.h"
#include "ITI_PanelAligner.h"
#include "MTickTimer.h"
#include "DefSystem.h"
//#include "ITI_PanelTransferOut.h"

class MSystemData;
class MPanelData;
class MProductInfoData;
class MPreBonderData;
class MProcessData;
class MWorkSchedule;
class IIO;
class MNetH;
class IStateTabMounter;
class IStateRollerConveyor;
class IStatePanelTransfer;
class MPanelAligner;
class MModelChange;
class MCtrlPanelAligner;
class ITI_TabMounter;
class ITI_PanelTransfer;
class ITI_PanelTransferOut;
class ITI_InspectionCamera;
class ITI_UnloadConveyor;
class ITI_TabFeeder;
class MTrsAutoManager;
class MCameraCarrier; // 2009.01.13 CYJ
class MLCNet;

const int ERR_TRS_PANEL_ALIGNER_SUCCESS			= 0;
const int DEF_TRS_PANEL_ALIGNER_INTERFACE_RETRY	= -1;

typedef
enum enumStepPanelAligner
{
	/* 0 */		PANEL_ALIGNER_MOVE_TO_LOAD_POS,
// selsk_jdy ��� ����.	/* 1 */	PANEL_ALIGNER_RECEIVE_PANEL_READY,
	/* 1 */		PANEL_ALIGNER_RECEIVE_PANEL,
	/* 2 */		PANEL_ALIGNER_MOVE_TO_ALIGN_POS,
	/* 3 */		PANEL_ALIGNER_ALIGN_PANEL,
	/* 4 */		PANEL_ALIGNER_MOVE_TO_PRE_INSPECTION_POS,	//PRE
	/* 5 */		PANEL_ALIGNER_WAIT_PRE_INSPECTION_COMPLETE,	//PRE
	/* 6 */		PANEL_ALIGNER_MOVE_TO_WORK_POS,
	/* 7 */		PANEL_ALIGNER_WAIT_WORK_COMPLETE,
	/* 8 */		PANEL_ALIGNER_WAIT_MOVE_TO_UNLOAD_POS,
	/* 9 */		PANEL_ALIGNER_MOVE_TO_UNLOAD_POS,
	/* 10 */	PANEL_ALIGNER_SEND_PANEL,
	/* 11 */	PANEL_ALIGNER_MODEL_CHANGE,
				PANEL_ALIGNER_READY_MOVE_FOR_FEEDERREADY,
				PANEL_ALIGNER_WAIT_1FEEDERALLMOUNT_INIT,
				PANEL_ALIGNER_INSPECTION_TAB_BOLB_READY,//170705 JSH
				PANEL_ALIGNER_INSPECTION_TAB_BOLB,//170705 JSH
				PANEL_ALIGNER_INSPECTION_TAB_BOLB_AFTER,//170705 JSH
				PANEL_ALIGNER_INSPECTION_TAB_BOLB_DONE,//170705 JSH


} EStepPanelAligner;

typedef
struct tagSTrsPanelAlignerRefCompList
{
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MCtrlPanelAligner*		m_plnkCtrlPanelAligner;
	MPanelAligner*			m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
//@	IStatePanelTransfer*	m_plnkStatePanelTransferCenter;
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	IIO*					m_plnkIO;
	MNetH*					m_plnkMelsecIF;
	IStateTabMounter*		m_plnkStateTabMounter1;
	IStateTabMounter*		m_plnkStateTabMounter2;
	MModelChange*			m_plnkModelChange1;
	MModelChange*			m_plnkModelChange2;
	MCameraCarrier*			m_plnkCameraCarrier1;
	MCameraCarrier*			m_plnkCameraCarrier2;

	//SJ_YYK 110421 Add.. Ellas
	MLCNet*					m_plnkLCNet;

} STrsPanelAlignerRefCompList, *pSTrsPanelAlignerRefCompList;

/** TrsPanelAligner Data */
typedef
struct tagSTrsPanelAlignerData
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MProductInfoData*	m_plnkProductInfoData;
	MPreBonderData*		m_plnkPrebonderData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*		m_plnkWorkScheduleSecond[DEF_MAX_WORKER_GROUP];

} STrsPanelAlignerData, *pSTrsPanelAlignerData;

typedef
struct tagSPanelAlignerRefITIList
{
#ifdef DEF_SOURCE_SYSTEM
	ITI_PanelTransfer*		m_plnkITIPanelTransfer;
#endif
	ITI_PanelTransferOut*	m_plnkITIPanelTransferOut;
	ITI_TabMounter*			m_plnkITITabMounter;
	ITI_InspectionCamera*	m_plnkITIInspectionCamera;
	ITI_TabMounter*			m_plnkITIRTabMounter;
	ITI_InspectionCamera*	m_plnkITIRInspectionCamera;
} SPanelAlignerRefITIList, *pPanelAlignerRefITIList;


class MTrsPanelAligner : public ITI_PanelAligner, public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MCtrlPanelAligner*		m_plnkCtrlPanelAligner;
	MPanelAligner*			m_plnkPanelAligner;
//	IStateRollerConveyor*	m_plnkStateUnloadConveyor;
#ifdef DEF_SOURCE_SYSTEM
//@	IStatePanelTransfer*	m_plnkStatePanelTransferCenter;
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	IIO*					m_plnkIO;
	MNetH*					m_plnkMelsecIF;
	IStateTabMounter*		m_plnkStateTabMounter1;
	IStateTabMounter*		m_plnkStateTabMounter2;
	MModelChange*			m_plnkModelChange1;
	MModelChange*			m_plnkModelChange2;

	MCameraCarrier*			m_plnkCameraCarrier1;
	MCameraCarrier*			m_plnkCameraCarrier2;

	//SJ_YYK 110421 Add.. Ellas...
	MLCNet*					m_plnkLCNet;

#ifdef DEF_SOURCE_SYSTEM
	ITI_PanelTransfer*		m_plnkITIPanelTransfer;
#endif
	ITI_PanelTransferOut*	m_plnkITIPanelTransferOut;
	ITI_TabMounter*			m_plnkITITabMounter;
	ITI_InspectionCamera*	m_plnkITIInspectionCamera;
	ITI_TabMounter*			m_plnkITIRTabMounter;
	ITI_InspectionCamera*	m_plnkITIRInspectionCamera;
	ITI_TabFeeder*			m_plnkITITabFeeder[DEF_MAX_TABFEEDER];

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MProductInfoData*	m_plnkProductInfoData;
	MPreBonderData*		m_plnkPrebonderData;
	MProcessData*		m_plnkProcessData;
//@	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE][DEF_MAX_WORKER_GROUP];

/**********
 *  ITI Flag
 */
	BOOL				m_bITIReceiveAbleToPanelTransfer;
	BOOL				m_bITIReceiveStartToPanelTransfer;

	BOOL				m_bITISendStartToPanelTransferOut;
	BOOL				m_bITISendCompleteToPanelTransferOut;

	BOOL				m_bITIInspectorEnabletoInspect;

	BOOL				m_bITIPanelAlignerBeginAlignPanelToWorker;
	// 2009.01.13 CYJ Mech ver3 Source Inspection Cam2�� Panel Align�� �ϸ鼭 �߰�.
	BOOL				m_bITIInspectorEnabletoInspectMove;

	BOOL				m_bITIWorkerEnabletoWork;
	BOOL				m_bITIPanelTransferEnabletoMoveUnloadPos;

	BOOL				m_bPanelAlignerUnloadingPanel; //Pre Online

	// �ش� Panel �� ���� �۾� Skip -> ���� flag
	BOOL				m_bSkipCurrentPanelWork;

	// Repair Run ��, �۾� ��� ���� ����.
	BOOL				m_bRepairRunContinue;

	// NSMC
	BOOL				m_bModelChangeReady;

	BOOL				m_b1FeederAllMountInitReady;

	//120116.kms_________
	BOOL				m_bMountAutoChangeReady;
	
	BOOL				m_bReadyInspectionBolbPos[DEF_MAX_WORK_SCHEDULE]; //170705 JSH
/**********
 *  Threading
 */
	/** Thread ���� Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsPanelAligner�� Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsPanelAligner�� Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** AUTO View Window Handle */
// selsk_jdy	HWND				m_hAutoViewWnd;

	/** LogView Handle */
// selsk_jdy	HWND				m_hLogViewWnd;

	/** TitleForm Handle */
// selsk_jdy	HWND				m_hTitleForm;

	/** ���� Step ���� */
	EStepPanelAligner	m_estepCurrent;

	/** �Ѵܰ� ���� Step ���� */
	EStepPanelAligner	m_estepPrevious;

/**********
 *  ��Ÿ
 */
	/** Eq Tact Time Timer */
	// selsk_jdy
	MTickTimer			m_EqTactTimer;
	MTickTimer			m_EqTactWaitTimer;
	MTickTimer			m_EqLineTactTimer;
	MTickTimer			m_timer;
	CString				m_strOutPanelID;

	//120515.KMS________
	MTickTimer			m_Assemtimer;

	double				m_dWaitTactTime;

// jdy sesl	char				m_szLogMsg[250];

	/** �λ����� ���� ���� Check Flag */
	BOOL				m_bIsAirCVVacOnFlag;

	/* selsk_jdy ����.
	// Tact Time
	double				m_dEQTactTime;
	*/

	/* selsk_jdy ����.
	// Panel ���� ���¿��� �ʱ�ȭ ���� �� Tact Timer ������ ����
	BOOL				m_bPanelExistTactStart;
	*/

#ifdef SIMULATION
	int					m_iPreStep;
#endif

	BOOL				m_bScrapPanelReq;
	BOOL				m_bLastWorkGroup;
	BOOL				m_bPanelInStop;
	BOOL				m_bTabInspectionTabBlobCheck; //171004 JSH
	BOOL				m_bDryRunPanelExist;

	CPoint				m_ptManualMarkPos[2];

	static int			m_iScheduleNo;
	static int			m_bReceivedPressCompleteSchedule[DEF_MAX_GROUP];
private:
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
	void initializeMelsecIF();

	void doRunStep();

	void initializeITI();

	static UINT threadFunction(LPVOID pParam);

    void threadJob();

	void setStep(EStepPanelAligner estepPanelAligner);

	void initializeStep();

	void simulLog(CString Msg, int iStep, int iSleep = 0);

	// selsk_jdy
	BOOL isPanelTransferEnabledToMoveUnloadPos(int iCheckPosID = -1);

	// selsk_jdy
	BOOL isPanelAlignerEnabledToMoveWorkPos();

	// selsk_jdy
	int waitBackupDown();

	int waitTabMounterAllUp();

	int waitPlateVacuumOn();
public:
	int moveModelChangeWorkPos(BOOL bWait);
	int waitModelChangeWorkPos();
	int moveModelChangeAlignPos(BOOL bWait);
	int waitModelChangeAlignPos();
private:
	BOOL isUseWorkGroup(int iWorkGroup);
	BOOL is1FeederAllMounterInitStart();
	//@130204.KKY____________
	BOOL isAllTabMounterReadyToPusherDown();
	//@______________________

	//121004.kms____
	double m_dTolerance;
public:

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

	MTrsPanelAligner::MTrsPanelAligner(SCommonAttribute			commonData,
									STrsPanelAlignerRefCompList	listRefComponents,
									STrsPanelAlignerData		datComponent);

    ~MTrsPanelAligner();  

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	/**
	 * Assign component linkage
	 * 
	 * @param	listRefComponents	: MTrsPanelAligner ���� Component list
	 */
	int AssignComponents(STrsPanelAlignerRefCompList listRefComponents);

	/** 
	 * Component Data ó�� 
	 *
	 * @param	pData : SMMTrsPanelAlignerData ����ü�� ���� ������
	 * @return	Error : 0 = Success, �׿�=��Ÿ
	 */
	int SetData(STrsPanelAlignerData datComponent);

	/** 
	 * TrsPanelAligner Component�� �ڵ� ���� �ʱ�ȭ�� �����Ѵ�.
	 * @return	Error Code : 0 = SUCCESS, �� �� = Error
	 */
//@	int Initialize(BOOL bSkipCurrentPanelWork);
	int Initialize(BOOL bResetPanelWorkNo, BOOL bSkipCurrentPanelWork);

	/**
	 * MTrsPanelAligner Initial �Ϸ� ���θ� �����Ѵ�.
	 */
	void ResetInitialized(void);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 

	void AssignITI(ITI_PanelTransfer* pITI_PanelTransfer, ITI_PanelTransferOut* pITI_PanelTransferOut, ITI_TabMounter* pITI_TabMounter, ITI_TabMounter* pITI_RTabMounter, 
					ITI_InspectionCamera* pITI_InspectionCamera, ITI_InspectionCamera* pITI_RInspectionCamera, 
					ITI_TabFeeder* pITI_TabFeeder1, ITI_TabFeeder* pITI_TabFeeder2);
	/**
	 * MTrsPanelAligner Initial �Ϸ� ���� Ȯ��
	 *
	 * @return BOOL (TRUE : Initial �Ϸ�, FALSE : �̿Ϸ�)
	 */
	BOOL IsInitialized();

	BOOL IsReceiveAbleToPanelTransfer();
	BOOL IsReceiveStartToPanelTransfer();

	BOOL IsSendStartToPanelTransferOut();
	BOOL IsSendCompleteToPanelTransferOut();

	BOOL IsPanelAlignerBeginAlignPanelToWorker();
	BOOL IsWorkerEnabletoWork();
	BOOL IsInspectorEnabletoInspect();

// 2009.01.13 CYJ Mech ver3 Source Inspection Cam2�� Panel Align�� �ϸ鼭 �߰�.
	BOOL IsInspectorEnabletoInspectMove();

	BOOL IsPanelTransferEnabletoMoveUnloadPos();

	BOOL IsPanelAlignerUnloadingPanel();

	void SetRepairRunContinue();
	BOOL IsRepairRunContinue();

	// NSMC
	BOOL IsModelChangeReady();

	BOOL Is1FeederAllMountInitReady();

	BOOL IsReceivedPressCompleteSchedule(int iWorkGroup)	{ return m_bReceivedPressCompleteSchedule[iWorkGroup]; }
	void ResetReceivedPressCompleteSchedule(int iWorkGroup)	{ m_bReceivedPressCompleteSchedule[iWorkGroup] = FALSE; }

	//120116.kms__________
	BOOL IsMountAutoChangeReady();
	
	BOOL IsReadyInspectionBolbPos(int iWorkGroup); //170705 JSH
/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

	void ThreadRun();

	void ThreadStop();

	int GetStep();

	BOOL IsAutoRun();

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 

	/**
	 *	PanelAligner �� Panel �� �ִ��� ���� ��ȯ
	 */
	BOOL IsEmptyPanel();

	// syr..SESL: hongju ���� 0523_SESL
	// ���� Plate�� VacuumOn �������� üũ
	BOOL IsPlateVacuumOn();

/***********************************************************************************************
/*
/* Etc.
/*
/************************************************************************************************/ 
	/**
	 *	Panel ���� ���� ��� Ȯ��
	 */
	BOOL IsPanelInStop();

	/**
	 * Panel ���� ���� ��� ����
	 */
	void SetPanelInStop(BOOL bSel);

	int ScrapPanel();

	BOOL IsDryRunPanelExist() { return m_bDryRunPanelExist; }
	void SetManualMarkPos(int iCamNo, CPoint ptMark) { m_ptManualMarkPos[iCamNo] = ptMark; }

	static void	SetWorkScheduleNo(int iNo) { m_iScheduleNo = iNo; }
	static void AppendWorkSchedule() { m_iScheduleNo++; }
	static int	GetWorkScheduleNo() { return m_iScheduleNo; }
};
#endif //TRS_PANEL_ALIGNER_H
