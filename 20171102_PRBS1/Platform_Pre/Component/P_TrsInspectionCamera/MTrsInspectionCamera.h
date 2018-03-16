/* 
 * MTrsInspectionCamera Component
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
 * MTrsInspectionCamera.h : Headerfile of MTrsInspectionCamera component.
 */

#ifndef MTRSINSPECTIONCAMERA_H
#define MTRSINSPECTIONCAMERA_H

#include "ICommon.h"
#include "MTickTimer.h"
#include "ITI_InspectionCamera.h"
#include "MProcessData.h"

class MSystemData;
class MProcessData;
class MCameraCarrier;
class IStateCameraCarrier;
class MCtrlInspectionCamera;
class ITI_TabMounter;
class ITI_PanelAligner;
class MTrsAutoManager;
class MPreBonderData;
class MTabMounter;

const int ERR_TRS_CAMERACARRIER_SUCCESS = 0;

const int DEF_INSPECTION_CAMERA_WAIT	= 7;

typedef
enum enumStepInspectionCamera
{
	INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT,
	INSPECTIONCAMERA_MOVE_TO_TAB_MARK_POS,
	INSPECTIONCAMERA_INSPECT_MARK,
	INSPECTIONCAMERA_MOVE_TO_MOUNT_POS,
//	INSPECTIONCAMERA_MOVE_TO_AFTER_INSPECT_TAB_MARK_POS,
	INSPECTIONCAMERA_WAIT_GROUPWORK_DONE,
	INSPECTIONCAMERA_MOUNT_AFTER_VISION_LOG_REDAY,
	INSPECTIONCAMERA_MOUNT_AFTER_VISION_LOG,
	INSPECTIONCAMERA_ALL_WORK_DONE,
	INSPECTIONCAMERA__MODEL_CHANGE,
	INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY,
	INSPECTIONCAMERA_WAIT_1FEEDERALLMOUNT_INIT,
	INSPECTIONCAMERA_INSPECTION_TAB_BOLB_READY, //170612 JSH
	INSPECTIONCAMERA_INSPECTION_TAB_BOLB_PA_WAIT, //170612 JSH
	INSPECTIONCAMERA_INSPECTION_TAB_BOLB, //170612 JSH
	INSPECTIONCAMERA_INSPECTION_TAB_BOLB_AFTER, //170612 JSH
	INSPECTIONCAMERA_INSPECTION_TAB_BOLB_DONE, //170612 JSH
} EStepInspectionCamera;

typedef
struct tagSTrsInspectionCameraRefCompList
{
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MCtrlInspectionCamera*	m_plnkCtrlInspectCam;
	MCameraCarrier*			m_plnkCamCarrier;
	IStateCameraCarrier*	m_rgplnkStateCamCarrier[DEF_MAX_INSPECTION_CAM_PER_GROUP];
	MTabMounter*			m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP];

} STrsInspectionCameraRefCompList, *pSTrsInspectionCameraRefCompList;

typedef
struct tagSInspectionCameraRefITIList
{
	ITI_PanelAligner*				m_plnkITI_PanelAligner;
	ITI_TabMounter*					m_plnkITI_TabMounter[DEF_MAX_WORKER_PER_GROUP];

} SInspectionCameraRefITIList, *pInspectionCameraRefITIList;

/** MTrsInspectionCamera Data */
typedef
struct tagSTrsInspectionCameraData
{
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MPreBonderData*		m_plnkPrebonderData;
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;
} STrsInspectionCameraData, *pSTrsInspectionCameraData;

class MTrsInspectionCamera : public ITI_InspectionCamera, public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	ITI_PanelAligner*	m_plnkITI_PanelAligner;
	ITI_TabMounter*		m_plnkITI_TabMounter[DEF_MAX_WORKER_PER_GROUP];

	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlInspectionCamera* m_plnkCtrlInspectCam;
	MCameraCarrier*		m_plnkCamCarrier;
	IStateCameraCarrier* m_rgplnkStateCamCarrier[DEF_MAX_INSPECTION_CAM_PER_GROUP];
	MTabMounter*		m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP];
/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MPreBonderData*		m_plnkPrebonderData;
//@	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

	int	m_iWorkGroup;
	int	m_iWorkInstance;
/**********
 *  ITI Flag
 */
	BOOL				m_rgbITIInspectionStartToTM;
	BOOL				m_rgbITIInspectionCompleteToTM;
	BOOL				m_rgbITIMountReadyToTM; //170905 JSH

/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsInspectionCamera의 Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsInspectionCamera의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** AUTO View Window Handle */
// selsk_jdy	HWND				m_hAutoViewWnd;

	/** 현재 Step 저장 */
	EStepInspectionCamera m_estepCurrent;

	/** 한단계 이전 Step 저장 */
	EStepInspectionCamera m_estepPrevious;

	/** ITI Time Check Timer */
	MTickTimer			m_Timer4ITI;

	MTickTimer			m_TimerError;

	// NSMC
	BOOL				m_bModelChangeReady;

	BOOL				m_b1FeederAllMountInitReady;

	//120116.KMS______
	BOOL				m_bMountAutoChangeReady;
/**********
 *  기타
 */
	/** Tact Time Log Message	*/
// jdy sesl	char				m_szLogMsg[250];

#ifdef SIMULATION
	int					m_iPreStep;
#endif

	/** 작업 Group 번호 */
	int					m_iGroupNo;

	/** 작업변 정보 */
	EWorkingSide		m_eWorkSide;
	
	/** 결정된 작업변 상에서 InspectionCamera 가 작업하는 Tab 번호 */
//KKY.Del	int					m_iInspectTabNo;

	/** m_iInspectTabNo 에 해당하는 TabIC 를 흡착하고 있는 TabMounter 번호
	 *	주의 : TabMounter 가 속한 Group 내에서의 TabMounter Index 값임.
	 */
	int					m_iWorkTabMounterNoInGroup;

	/** CameraCarrier 이동에 대한 상태와 정보를 일치시키기 위한 보호 장치 */
	static CRITICAL_SECTION	sm_rgcsSetMoving[DEF_MAX_WORKER_GROUP];

	int					m_iPreInspectCamNo;
	int					m_iNextInspectCamNo;

	//===========================================
	MInspectCarrierInfo	m_InspectInfo;
	static	BOOL		sm_bGroupWorkComplete[DEF_MAX_GROUP];
	static	BOOL		sm_bAllWorkComplete[DEF_MAX_GROUP];
	static	BOOL		sm_bAlignPosMoveComplete[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	static	BOOL		sm_bITIAllMountCompleteToPA[DEF_MAX_GROUP];
	static	BOOL		sm_bITIAllMountCompleteToPASecond[DEF_MAX_GROUP];
	static	BOOL		sm_bRemoveTab;
	static	BOOL		sm_bRecogRemoveTab[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	//===========================================

	static	int			m_iScheduleNo[DEF_MAX_GROUP];
	static	int			m_bReceivedPressCompleteSchedule[DEF_MAX_GROUP];
	static	BOOL		m_bMovetoAlignPos[DEF_MAX_GROUP];

	//@130206.KKY___________
	int		m_iRetryCnt;
	static BOOL		sm_bRequestUsePanelPusher[DEF_MAX_GROUP];
	static BOOL		sm_bInspectionComplete[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	static BOOL		sm_bPusherDown[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	//@_____________________
	
	//170612 JSh.s
	//170612 JSH 이중부착 방지 TabIC Bolb검사 유무 Flag
	static	BOOL		sm_bInspectionTabBolb[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];

	static	BOOL		sm_bTabBlobReady[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	static	BOOL		sm_bTabBlobStart[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	static	BOOL		sm_bTabBlobAfterReady[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];//171004 JSH
	static	BOOL		sm_bTabBlobAfterStart[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER]; //171004 JSH
	static	BOOL		sm_bTabBlobPAWaitEnd[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];//171004 JSH
	static	BOOL		sm_bTabBlobWorkingToPA[DEF_MAX_GROUP];
	static	BOOL		sm_bTabBlobWorkingToTM[DEF_MAX_GROUP];
	//170612 JSh.e
	static	BOOL		sm_bForceUnitAll_Initialize; //170616 JSh
	static BOOL		sm_MountAfterVisionLogComplete[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	static BOOL		sm_MountAfterVisionLogEnd[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];
	BOOL m_FlgMountAfterVisionLog;
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

	void setStep(EStepInspectionCamera EStepInspectionCamera);

	void initializeStep();

	void simulLog(CString Msg, int iStep, int iSleep = 200);

	// 현재 Inspection Camera 의 이동 목표 위치와 이웃 CameraCarrier 의 이동 상태를 비교하여
	// 이동 가능 여부를 확인하고, 그에 걸맞게 현재 Camera Carrier 의 Moving Flag 를 설정한다.
//KKY.Del	int checkNeighborAndMove();

	//===========================================
	BOOL isGroupStarter();
	BOOL IsReadyToWork();
	//===========================================

	BOOL is1FeederAllMounterInitStart();

	//@130206.KKY_____________
	BOOL isAllInspectionCompleteGroup();
	BOOL isAllPusherDownGroup();
	//@_______________________
	//170612 JSH.s

public:
	void SetInspectionTabBolb();

	BOOL IsTabBlobWorkingToPA(int iWorkGroup)
	{
		return sm_bTabBlobWorkingToPA[iWorkGroup];
	}
	
	BOOL IsTabBlobWorkingToTM(int iWorkGroup)
	{
		return sm_bTabBlobWorkingToTM[iWorkGroup];
	}
	//170612 JSH.e
	
	void ResetInspectionTabBolb(); //170616 JSH

	//170616 JSH.s
public:
	BOOL IsForceUnitAll_Initialize()
	{
		return sm_bForceUnitAll_Initialize;
	}

	void ResetForceUnitAll_Initialize()
	{
		sm_bForceUnitAll_Initialize = FALSE;
	}
	//170616 JSH.e
public:

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MTrsInspectionCamera(SCommonAttribute commonData, STrsInspectionCameraRefCompList listRefComponents, STrsInspectionCameraData datComponent);

    ~MTrsInspectionCamera();  

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	void AssignComponents(STrsInspectionCameraRefCompList listRefComponents);

	void SetData(STrsInspectionCameraData datComponent);

	int Initialize();

	void ResetInitialized(void)
	{
		m_bAutoInitialized = FALSE;
	}

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 
	void AssignITI(SInspectionCameraRefITIList sITIList);

	BOOL IsInspectionStartToTM(int iTabMounterNoInGroup)
	{
		return m_rgbITIInspectionStartToTM;
	}

	BOOL IsInspectionCompleteToTM(int iTabMounterNoInGroup)
	{
		return m_rgbITIInspectionCompleteToTM;
	}
	
	BOOL IsMountReadyToTM(int iTabMounterNoInGroup)
	{
		return m_rgbITIMountReadyToTM;
	}

	int GetInspectTabNo(void)
	{
		//return m_iInspectTabNo;
		return m_InspectInfo.m_nWorkTabNo;
	}
	BOOL IsAllMountCompleteToPA(int iWorkGroup)
	{
		return sm_bITIAllMountCompleteToPA[iWorkGroup];
	}
	BOOL IsAllMountCompleteToPASecond(int iWorkGroup)
	{
		return sm_bITIAllMountCompleteToPASecond[iWorkGroup];
	}

	BOOL IsReceivedPressCompleteSchedule(int iWorkGroup) { return m_bReceivedPressCompleteSchedule[iWorkGroup]; }
	void ResetReceivedPressCompleteSchedule(int iWorkGroup)	{ m_bReceivedPressCompleteSchedule[iWorkGroup] = FALSE; }

	// NSMC
	BOOL IsModelChangeReady();

	BOOL Is1FeederAllMountInitReady();

	//120116.kms__________
	BOOL IsMountAutoChangeReady();
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

	BOOL IsInitialized(void)
	{
		return m_bAutoInitialized;
	}

/***********************************************************************************************
/*
/* ETC
/*
/************************************************************************************************/
	//@___________________________
	static int GetWorkScheduleNo(int iGroupNo) { return m_iScheduleNo[iGroupNo]; }
	static void SetWorkScheduleNo(int iGroupNo, int iWorkNo) { m_iScheduleNo[iGroupNo] = iWorkNo; }
	static void AddendWorkSchedule(int iGroupNo) { m_iScheduleNo[iGroupNo]++; }
	//@___________________________

};
#endif //MTRSINSPECTIONCAMERA_H
