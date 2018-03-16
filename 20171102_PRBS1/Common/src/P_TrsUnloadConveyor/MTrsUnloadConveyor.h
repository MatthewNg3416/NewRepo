/* 
 * TrsUnloadConveyor Component
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
 * MTrsUnloadConveyor.h : Headerfile of MTrsUnloadConveyor component.
 */

#ifndef TRS_UNLOAD_CONVEYOR_H
#define TRS_UNLOAD_CONVEYOR_H

#include "ICommon.h"
#include "ITI_UnloadConveyor.h"
#include "MTickTimer.h"

class MNetH;
class MSystemData;
//class MRollerConveyor;
class IStatePanelAligner;
class MTrsAutoManager;
class ITI_PanelAligner;
class ITI_PanelTransfer;
#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) )
class ITI_OutConveyor;
#endif

// 2009.05.22 hjh ver3 && D군 OutConveyor 없음.
#ifdef DEF_TEST
class MOpPanel;
#endif

const int ERR_TRS_UNLOAD_CONVEYOR_SUCCESS = 0;

typedef
enum enumStepUnloadConveyor
{
	UNLOAD_CONVEYOR_RECEIVE_PANEL,
	UNLOAD_CONVEYOR_SEND_PANEL_READY,
	UNLOAD_CONVEYOR_SEND_PANEL,

	// NSMC
	UNLOAD_CONVEYOR_MODEL_CHANGE,
} EStepUnloadConveyor;

typedef
struct tagSTrsUnloadConveyorRefCompList
{
// 2009.05.22 hjh ver3 && D군 OutConveyor 없음.
#ifdef DEF_TEST
	IOpPanel*			m_plnkOpPanel;
#endif

	MTrsAutoManager*	m_plnkTrsAutoManager;
//	MRollerConveyor*	m_plnkUnloadConveyor;
	IStatePanelAligner*	m_plnkStatePanelAligner;
	MNetH*				m_plnkMelsecIF;

} STrsUnloadConveyorRefCompList, *pSTrsUnloadConveyorRefCompList;

/** TrsUnloadConveyor Data */
typedef
struct tagSTrsUnloadConveyorData
{
	MSystemData*		m_plnkSystemData;

} STrsUnloadConveyorData, *pSTrsUnloadConveyorData;

typedef
struct tagSUnloadConveyorRefITIList
{
	ITI_PanelAligner*	m_plnkITIPanelAligner;
	ITI_PanelTransfer*	m_plnkITIPanelTransfer;

#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) )
	ITI_OutConveyor*	m_plnkITIOutConveyor;
#endif

} SUnloadConveyorRefITIList, *pUnloadConveyorRefITIList;


class MTrsUnloadConveyor : public ITI_UnloadConveyor, public virtual ICommon
{
private:

/**********
 *  Reference Components
 */
	MTrsAutoManager*	m_plnkTrsAutoManager;
//	MRollerConveyor*	m_plnkUnloadConveyor;
	IStatePanelAligner*	m_plnkStatePanelAligner;
	MNetH*				m_plnkMelsecIF;

// 2009.05.22 hjh ver3 && D군 OutConveyor 없음.
#ifdef DEF_TEST
	IOpPanel*			m_plnkOpPanel;
#endif

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;

	ITI_PanelAligner*	m_plnkITIPanelAligner;
	ITI_PanelTransfer*	m_plnkITIPanelTransfer;

#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) )
	ITI_OutConveyor*	m_plnkITIOutConveyor;
#endif

/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsUnloadConveyor의 Run Flag	*/
	BOOL				m_bAutoRun;

	/** MTrsUnloadConveyor의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** 현재 Step 저장 */
	EStepUnloadConveyor	m_estepCurrent;

	/** 한단계 이전 Step 저장*/
	EStepUnloadConveyor	m_estepPrevious;

	/** Log Message	*/
// jdy sesl	char				m_szLogMsg[250];

/**********
 *  ITI (Inter-Thread Interface)
 */
	BOOL				m_bITIReceiveAbleToPanelAligner;
	BOOL				m_bITIReceiveStartToPanelAligner;
	BOOL				m_bITISendAbleToOutConveyor;
	BOOL				m_bITISendCompleteToOutConveyor;

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


private:
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

	void doRunStep();

	static UINT threadFunction(LPVOID pParam);

    void threadJob();

	void setStep(EStepUnloadConveyor estepUnloadConveyor);

	void initializeStep();

	void simulLog(CString strMsg, int iStep, int iSleep = 500);

public:

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	MTrsUnloadConveyor(SCommonAttribute				commonData, 
					STrsUnloadConveyorRefCompList	listRefComponents, 
					STrsUnloadConveyorData			datComponent );

    ~MTrsUnloadConveyor();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	/**
	 * Assign component linkage
	 * 
	 * @param	listRefComponents	: MTrsUnloadConveyor 참조 Component list
	 */
	int AssignComponents(STrsUnloadConveyorRefCompList listRefComponents);

	/** 
	 * Component Data 처리 
	 *
	 * @param	pData : SMMTrsUnloadConveyorData 구조체에 대한 포인터
	 * @return	Error : 0 = Success, 그외=기타
	 */
	int SetData(STrsUnloadConveyorData datComponent);

	/** 
	 * MTrsUnloadConveyor Component의 자동 운전 초기화를 시작한다.
	 * @return	Error Code : 0 = SUCCESS, 그 외 = Error
	 */
	int Initialize();
	
	/**
	 * MTrsUnloadConveyor Initial 완료 여부를 해제한다.
	 *
	 * @return void
	 */
	void ResetInitialized(void);
	
/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 

#if (defined(DEF_PLATFORM_FINALBONDER_SYSTEM) && defined(DEF_GATE_SYSTEM) )
	void AssignITI(ITI_PanelAligner* pITI_PanelAligner, ITI_PanelTransfer* pITI_PanelTransfer,
					ITI_OutConveyor* pITI_OutConveyor);
#else
	void AssignITI(ITI_PanelAligner* pITI_PanelAligner, ITI_PanelTransfer* pITI_PanelTransfer);
#endif

	void initializeITI();

	/**
	 * Initial 완료 여부 확인
	 *
	 * @return BOOL (TRUE : Initial 완료, FALSE : 미완료)
	 */
	BOOL IsInitialized();

	BOOL IsReceiveAbleToPanelAligner();
	BOOL IsReceiveStartToPanelAligner();

	BOOL IsSendAbleToOutConveyor();
	BOOL IsSendCompleteToOutConveyor();

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
	 *	UnloadConveyor 에 Panel 이 있는지 여부 반환
	 */
	BOOL IsEmptyPanel();

/***********************************************************************************************
/*
/* Etc.
/*
/************************************************************************************************/ 

	int ScrapPanel();
};
#endif //TRS_UNLOAD_CONVEYOR_H
