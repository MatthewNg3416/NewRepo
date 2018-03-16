/* 
 * TrsPanelTransfer Component
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
 * MTrsPanelTransfer.h : Headerfile of MTrsPanelTransfer component.
 */

#ifndef TRS_PANEL_TRANSFER_H
#define TRS_PANEL_TRANSFER_H

#include "ICommon.h"
#include "ITI_PanelTransfer.h"
#include "MTickTimer.h"

class MSystemData;
class MPanelData;
class MPanelTransfer;
class IStatePanelTransfer;
class IStatePanelAligner;
class ITI_PanelTransferCenter;
class ITI_PanelTransferOut;
class ITI_PanelTransfer;
class ITI_PanelAligner;
class ITI_THandler; //SJ_YYK 161104 Add.
class MTrsAutoManager;
class MLCNet;
class MNetH;


#ifdef DEF_PLATFORM_ARP_SYSTEM
class ITI_Carrier;
#endif

const int ERR_TRS_PANEL_TRANSFER_SUCCESS			= 0;
const int DEF_TRS_PANEL_TRANSFER_INTERFACE_RETRY	= -1;

typedef
enum enumStepPanelTransfer
{
	PANEL_TRANSFER_MOVE_TO_LOAD_POS,
	PANEL_TRANSFER_RECEIVE_PANEL_READY,
	PANEL_TRANSFER_RECEIVE_PANEL,
	PANEL_TRANSFER_SEND_PANEL_READY,
	PANEL_TRANSFER_SEND_PANEL,

	// NSMC
	PANEL_TRANSFER_MODEL_CHANGE,

} EStepPanelTransfer;

typedef
struct tagSTrsPanelTransferRefCompList
{
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MPanelTransfer*			m_plnkPanelTransfer;
	IStatePanelAligner*		m_plnkStatePanelAligner;
	MNetH*					m_plnkMelsecIF;
	MLCNet*					m_plnkLCNet;
//@	IStatePanelTransfer*	m_plnkStatePanelTransferCenter;
} STrsPanelTransferRefCompList, *pSTrsPanelTransferRefCompList;

/** TrsPanelTransfer Data */
typedef
struct tagSTrsPanelTransferData
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;

	// Transfer Type
	UINT				m_uiTransferType;

} STrsPanelTransferData, *pSTrsPanelTransferData;

typedef
struct tagSPanelTransferRefITIList
{
	ITI_PanelAligner*	m_plnkITIPanelAligner;
	ITI_THandler*		m_plnkITITurnHandler; //SJ_YYK 161104 Add..

} SPanelTransferRefITIList, *pPanelTransferRefITIList;


class MTrsPanelTransfer : public ITI_PanelTransfer, public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MPanelTransfer*			m_plnkPanelTransfer;
	IStatePanelAligner*		m_plnkStatePanelAligner;
//@	IStatePanelTransfer*	m_plnkStatePanelTransferCenter;
	MNetH*					m_plnkMelsecIF;
	MLCNet*					m_plnkLCNet;

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;

	ITI_PanelAligner*		m_plnkITIPanelAligner;
	ITI_THandler*		m_plnkITITurnHandler; //SJ_YYK 161104 Add..
//@	ITI_PanelTransferOut*	m_plnkITIPanelTransferOut;

/**********
 *  ITI Flag
 */
	BOOL				m_bITIReceiveAbleToPanelTransferCenter;
	BOOL				m_bITIReceiveStartToPanelTransferCenter;

	BOOL				m_bITIReceiveAbleToTurnHandler; //SJ_YYK 161104 Add..
	BOOL				m_bITIReceiveCompleteToTurnHandler; //SJ_YYK 161104 Add..

	BOOL				m_bITISendStartToPanelAligner;
	BOOL				m_bITISendCompleteToPanelAligner;

	// NSMC
	BOOL				m_bModelChangeReady;

	BOOL				m_b1FeederAllMountInitReady;
/**********
 *  Threading
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsPanelTransfer의 Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsPanelTransfer의 Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** 현재 Step 저장 */
	EStepPanelTransfer	m_estepCurrent;

	/** 한단계 이전 Step 저장 */
	EStepPanelTransfer	m_estepPrevious;

/**********
 *  기타
 */
	CString				m_strItiStatus;

#ifdef SIMULATION
	int					m_iPreStep;
#endif

	MTickTimer			m_timer;

	/** Panel 공급 중지 플래그.(KKY) */
	BOOL				m_bPanelInStop;

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

	void setStep(EStepPanelTransfer estepPanelTransfer);

	void initializeStep();

	void simulLog(CString Msg, int iStep, int iSleep = 0);

public:
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
	MTrsPanelTransfer::MTrsPanelTransfer(SCommonAttribute				commonData,
										STrsPanelTransferRefCompList	listRefComponents,
										STrsPanelTransferData			datComponent);

    ~MTrsPanelTransfer();  

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	/**
	* Assign component linkage
	* 
	* @param	listRefComponents	: MTrsPanelTransfer 참조 Component list
	*/
	int AssignComponents(STrsPanelTransferRefCompList listRefComponents);

	/** 
	* Component Data 처리 
	*
	* @param	pData : SMMTrsPanelTransferData 구조체에 대한 포인터
	* @return	Error : 0 = Success, 그외=기타
	*/
	int SetData(STrsPanelTransferData datComponent);

	/** 
	 * TrsPanelTransfer Component의 자동 운전 초기화를 시작한다.
	 * @return	Error Code : 0 = SUCCESS, 그 외 = Error
	 */
	int Initialize();

	/**
	 * MTrsPanelTransfer Initial 완료 여부를 해제한다.
	 *
	 * @return void
	 */
	void ResetInitialized(void);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 

//@	void AssignITI(ITI_PanelTransferCenter* pITI_PanelTransferCenter,/* ITI_PanelTransfer* pITI_PanelTransfer,*/
//@				ITI_PanelTransferOut* pITI_PanelTransferOut, ITI_PanelAligner* pITI_PanelAligner);


	void AssignITI(ITI_THandler* pITI_TurnHandler, ITI_PanelAligner* pITI_PanelAligner);
	//_________________________________
	/**
	 * MTrsPanelTransfer Initial 완료 여부 확인
	 *
	 * @return BOOL (TRUE : Initial 완료, FALSE : 미완료)
	 */
	BOOL IsInitialized();

	BOOL IsReceiveAbleToPanelTransferCenter();
	BOOL IsReceiveStartToPanelTransferCenter();

	BOOL IsReceiveAbleToTurnHandler(); //SJ_YYK 161104 Add.. 
	BOOL IsReceiveCompleteToTurnHandler(); //SJ_YYK 161104 Add..

	BOOL IsSendStartToPanelAligner();
	BOOL IsSendCompleteToPanelAligner();


	// NSMC
	BOOL IsModelChangeReady();

	BOOL Is1FeederAllMountInitReady();
/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/ 
	void ThreadRun();

	void ThreadStop();

	int GetStep();

	BOOL IsAutoRun();

	CString GetItiStatus();

	void InitializeMelsecIF();

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 

	/**
	 *	PanelTransfer 에 Panel 이 있는지 여부 반환
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
#endif //TRS_PANEL_TRANSFER_H
