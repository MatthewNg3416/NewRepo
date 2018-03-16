/* 
 * TrsTHandler Component
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
 * TrsTHandler.h : Headerfile of TrsTHandler component.
 */

#ifndef MTRSTHANDLER_H
#define MTRSTHANDLER_H

#include "ITI_THandler.h"
#include "ITI_PanelTransferCenter.h"
#include "ITI_PanelTransfer.h"

#include "DefSystem.h"
#include "MProductInfoData.h"
#include "MTickTimer.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "ICommon.h"

class	MTrsAutoManager;
class	MNetH;
class	MCtrlHandler;
class	MHandler;
class	IStatePanelTransfer;

const	int		ERR_TRSTHandler_SUCCESS					= 0;

const	int		MAX_QUEUE_LINE_TACT	= 10;

typedef
/**
 * Unlaod THandler의 STEP을 정의한다.
 * Stage Step은 Stage의 이동점을 기준으로 설계 했으며
 * 단위 Step에 들어갈때 해야할 일의 내용을 Step 이름으로 한다.
 */
enum enumStepTHandler
{
	/** 0. Loading 위치로 이동한다. */
	STEP_UNLHANDLER_MOVE_TO_LOAD_POS,

	/** 1. 워크벤치로부터 Panel을 가져온다. */
	STEP_UNLHANDLER_GET_PANEL,

	/** 2. Inspect위치로 이동한다. */
	STEP_UNLHANDLER_MOVE_INSPECT_POS,

	/** 2. Inspect 위치로 이동해서 ACF 부착상태를 검사한다.. */
	STEP_UNLHANDLER_INSPECT_PANEL,

	/** 3. Unloading 위치로 이동한다. */
	STEP_UNLHANDLER_MOVE_TO_UNLOAD_POS,

	/** 4. Panel을 다음설비에 내려놓는다. */
	STEP_UNLHANDLER_PUT_PANEL,

} EStepTHandler;

typedef
/** TrsTHandler Component가 가지는 Cylinder Component List */
 struct tagSTrsTHandlerRefCompList
{
	MCtrlHandler*			m_plnkCtrlHandler;
	MHandler*				m_plnkHandler;
	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransferIn;
	MNetH*					m_plnkMelsecIF;
	MTrsAutoManager*	    m_plnkTrsAutoManager;
} STrsTHandlerRefCompList, *pSTrsTHandlerRefCompList;

typedef
/** TrsTHandler Data */
struct tagSTrsTHandlerData
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MProductInfoData*	m_plnkProductInfoData;
} STrsTHandlerData, *pSTrsTHandlerData;

class MTrsTHandler : public ITI_THandler, public virtual ICommon 
{    
private:

/**********
 *  Reference Components
 */
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MCtrlHandler*		m_plnkCtrlHandler;
	MHandler*			m_plnkHandler;
	IStatePanelTransfer*		m_plnkPanelTransferCenter;
	IStatePanelTransfer*		m_plnkPanelTransferIn;
	MNetH*				m_plnkMelsecIF;

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MProductInfoData*	m_plnkProductInfoData;

/**********
 *  ITI Link
 */
	ITI_PanelTransferCenter*		m_plnkITITranferCenter;
	ITI_PanelTransfer*				m_plnkITITranferIn;

/**********
 *  ITI Flag
 */
//	BOOL				m_bIsPanelTakeReady;
//  BOOL				m_bIsPanelTakeComplete;
	BOOL				m_bIsLoadingReady;
	BOOL				m_bIsLoadingComplete;

	BOOL				m_bIsUnloadingReady;
	BOOL				m_bIsUnloadingComplete;


/**********
 *  Threading
 */
	/** Thread 구동 Flag*/
	BOOL				m_flgThreadLife;
	/** Thread Handle */
	CWinThread*			m_pThread;

	BOOL				m_flgAutoRun;

	BOOL				m_bAutoInitialized;

	BOOL				m_bNextjob;

	int					m_iCurrentJobID;

	// 현재 Step 저장
	EStepTHandler		m_estepCurrent;
	// 한단계 이전 Step 저장
	EStepTHandler		m_estepPrevious;

	// NSMC
	BOOL				m_bModelChangeReady;
	
/**********
 *  기타
 */	
	/** Line Tact Time Timer */
	MTickTimer			m_LineTactTimeTimer;

	/** Simulation 로깅을 위한 변수 */
	int					m_iPreStep;

	/** Melsec Timer */
	MTickTimer			m_tMNetTimer;

	/** AutoView Handle */
	HWND				m_hAutoViewWnd;

	/** LogView Handle */
	HWND				m_hLogViewWnd;

	/** TitleForm Handle */
	HWND				m_hTitleForm;

	/** 배출 Panel ID (ProductInfoData 로 전달) */
	CString				m_strOutPanelID;

	/** Eq Tact Time */
	double				m_dEqTact;

	//070205 cho iti msg========
	CString     m_strItiStatus;
	//=========================

private:
/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 

	/** RUN Step을 수행한다 */
	void		doRunStep();

	/** Thread간 Interface 초기화 */
	void		initializeITI();

	/** Thread 함수*/
	static UINT	threadFunction(LPVOID pParam);
    void		threadJob();

	/** Step Thread Step */
	void		setStep(EStepTHandler estepTHandler);

	void		initializeStep();

	void		simulLog(CString Msg, int iStep,	int iSleep = 0);


public: 
	
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	/** 생성자
	 * @param commonData		: Component의 CommonData
	 * @param listRefComponent	: 참조할 Component 포인터 리스트
	 * @param datComponent		: Component 설정 데이타
	 */
	MTrsTHandler::MTrsTHandler(	SCommonAttribute		commonData,
								STrsTHandlerRefCompList	listRefComponents, 
								STrsTHandlerData		datComponent );

	/** 소멸자
	* @stereotype	destructor
	*/
	virtual	~MTrsTHandler();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	int		AssignComponents(STrsTHandlerRefCompList listRefComponents);
	int		SetData(STrsTHandlerData datComponent);
	void	GetData(STrsTHandlerData *pData);
	int		Initialize();
	void	ResetInitialized(void);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 

    void    AssignITI(ITI_PanelTransferCenter* plnkITITranferCenter, ITI_PanelTransfer* plnkITITranferIn);

	/**
	* Auto Manager를 Assign 한다.
	* @param	pAutoManager : 설정할 AutoManager
	*/
	int		AssignAutoManager(MTrsAutoManager* pAutoManager);

	BOOL	IsInitialized()						
	{ 
		return m_bAutoInitialized;					
	}
	BOOL	IsLoadingReady()
	{
		return m_bIsLoadingReady;
	}
	BOOL	IsLoadingComplete()		
	{ 
		return m_bIsLoadingComplete;			
	}

	BOOL	IsUnloadingReady()
	{
		return m_bIsUnloadingReady;
	}

	BOOL	IsUnloadingComplete()
	{
		return m_bIsUnloadingComplete;
	}
/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 
	void	ThreadRun();
	void	ThreadStop();
	int		GetStep()	
	{
		return m_estepCurrent;
	}
	BOOL	IsAutoRun()		
	{
		return m_flgAutoRun;	
	}
	// 070206   cho iti Get ===========================================================================
	CString        GetItiStatus()
	{
//@		m_strItiStatus.Format("LDR: %d, LDC: %d",	m_bIsLoadingReady,m_bIsLoadingComplete);
		return m_strItiStatus;
	}
	//==================================================================================================

	BOOL IsModelChangeReady();

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 

	/**
	 *	Unload Handler 에 Panel 이 있는지 여부 반환
	 */
	BOOL IsEmptyPanel();

/***********************************************************************************************
/*
/* etc.
/*
/************************************************************************************************/ 
	void	SetWindowHandle(HWND hWnd);

	void	SetLogViewHandle(HWND hWnd);

	void	SetTitleViewHandle(HWND hWnd);
	
	int ScrapPanel(); //170123 SJ_YSH
};
#endif //MTRSTHandler_H
