/* 
 * TrsLoadConveyor Component
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
 * MTrsLoadConveyor.h : Headerfile of MTrsLoadConveyor component.
 */

#ifndef TRS_LOAD_CONVEYOR_H
#define TRS_LOAD_CONVEYOR_H

#include "ICommon.h"
#include "ITI_LoadConveyor.h"
#include "MTickTimer.h"

class MNetH;
class MLCNet;
class MSystemData;
//class MRollerConveyor;
class IStatePanelTransfer;
class MTrsAutoManager;
class ITI_PanelAligner;
class ITI_PanelTransfer;

#ifdef DEF_TEST
class IOpPanel;
#endif

const int ERR_TRS_LOAD_CONVEYOR_SUCCESS = 0;

typedef
enum enumStepLoadConveyor
{
	LOAD_CONVEYOR_RECEIVE_PANEL_READY,
	LOAD_CONVEYOR_RECEIVE_PANEL,
	LOAD_CONVEYOR_SEND_PANEL_READY,
	LOAD_CONVEYOR_SEND_PANEL,

	// NSMC
	LOAD_CONVEYOR_MODEL_CHANGE,
} EStepLoadConveyor;

typedef
struct tagSTrsLoadConveyorRefCompList
{
	MTrsAutoManager*		m_plnkTrsAutoManager;
//	MRollerConveyor*		m_plnkLoadConveyor;
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
	MNetH*					m_plnkMelsecIF;
	MLCNet*					m_plnkLCNet;

#ifdef DEF_TEST
	IOpPanel*				m_plnkOpPanel;
#endif
} STrsLoadConveyorRefCompList, *pSTrsLoadConveyorRefCompList;

/** TrsLoadConveyor Data */
typedef
struct tagSTrsLoadConveyorData
{
	MSystemData*			m_plnkSystemData;

} STrsLoadConveyorData, *pSTrsLoadConveyorData;

typedef
struct tagSLoadConveyorRefITIList
{
	ITI_PanelAligner*		m_plnkITIPanelAligner;
	ITI_PanelTransfer*		m_plnkITIPanelTransfer;

} SLoadConveyorRefITIList, *pLoadConveyorRefITIList;


class MTrsLoadConveyor : public ITI_LoadConveyor, public virtual ICommon
{
private:

/**********
 *  Reference Components
 */

	MTrsAutoManager*		m_plnkTrsAutoManager;
//	MRollerConveyor*		m_plnkLoadConveyor;
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
	MNetH*					m_plnkMelsecIF;
	MLCNet*					m_plnkLCNet;

#ifdef DEF_TEST
	IOpPanel*				m_plnkOpPanel;
#endif

/**********
 *  Data
 */
	MSystemData*			m_plnkSystemData;

	ITI_PanelAligner*		m_plnkITIPanelAligner;
	ITI_PanelTransfer*		m_plnkITIPanelTransfer;

/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsLoadConveyor의 Run Flag	*/
	BOOL				m_bAutoRun;

	/** MTrsLoadConveyor의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** 현재 Step 저장 */
	EStepLoadConveyor	m_estepCurrent;

	/** 한단계 이전 Step 저장*/
	EStepLoadConveyor	m_estepPrevious;

	/** Log Message	*/
// jdy sesl	char				m_szLogMsg[250];

/**********
 *  ITI (Inter-Thread Interface)
 */
	BOOL				m_bITISendStartToPanelTransfer;
	BOOL				m_bITISendCompleteToPanelTransfer;

	// NSMC
	BOOL				m_bModelChangeReady;

/**********
 *  Etc.
 */

#ifdef SIMULATION
	int					m_iPreStep;
#endif

	MTickTimer			m_timer;

	CString				m_strItiStatus;

	/** Panel 공급 중지 플래그 */
	BOOL				m_bPanelInStop;

private:
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

	void doRunStep();

	static UINT threadFunction(LPVOID pParam);

    void threadJob();

	void setStep(EStepLoadConveyor estepLoadConveyor);

	void initializeStep();

	void simulLog(CString strMsg, int iStep, int iSleep = 500);

public:

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MTrsLoadConveyor(SCommonAttribute			commonData, 
					STrsLoadConveyorRefCompList	listRefComponents, 
					STrsLoadConveyorData		datComponent );

    ~MTrsLoadConveyor();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	int AssignComponents(STrsLoadConveyorRefCompList listRefComponents);

	int SetData(STrsLoadConveyorData datComponent);

	int Initialize();
	
	void ResetInitialized();
	
/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 

	void AssignITI(ITI_PanelAligner* pITI_PanelAligner, ITI_PanelTransfer* pITI_PanelTransfer);

	void initializeITI();

	/**
	 * Initial 완료 여부 확인
	 *
	 * @return BOOL (TRUE : Initial 완료, FALSE : 미완료)
	 */
	BOOL IsInitialized();

	BOOL IsSendStartToPanelTransfer();
	BOOL IsSendCompleteToPanelTransfer();

	// NSMC
	BOOL IsModelChangeReady();
/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 

	void ThreadRun();

	void ThreadStop();

	int	GetStep();

	BOOL IsAutoRun();

	CString GetItiStatus();

	void initializeMelsecIF();

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 

	/**
	 *	LoadConveyor 에 Panel 이 있는지 여부 반환
	 */
	BOOL IsEmptyPanel();

	/**
	 *	Panel 공급 중지 기능 확인
	 */
	BOOL IsPanelInStop();

	/**
	 * Panel 공급 중지 기능 설정
	 */
	void SetPanelInStop(BOOL bSel);

/***********************************************************************************************
/*
/* Etc.
/*
/************************************************************************************************/ 

	int ScrapPanel();
	
};
#endif //TRS_LOAD_CONVEYOR_H
