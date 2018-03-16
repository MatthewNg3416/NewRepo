//______________________________________________________________________________

// File Name: MTrsPanelTransferCenter.h
// History	:
//		1. 2010.07.29 : Created
//		2.
//______________________________________________________________________________

#ifndef _MTRS_PANEL_TRANSFER_CENTER_H_
#define _MTRS_PANEL_TRANSFER_CENTER_H_
//______________________________________________________________________________

#include "ICommon.h"
#include "ITI_PanelTransferCenter.h"
#include "MTickTimer.h"
#include "DefSystemConfig.h"
//______________________________________________________________________________

class IStateRollerConveyor;
class IStatePanelTransfer;
class IStateHandler; //SJ_YYK 161104 Add.
class MTrsAutoManager;
class MSystemData;
class MPanelData;
class MPanelTransfer;
class MPanelAligner; //170205 SJ_YSH
class MNetH;
class ITI_LoadConveyor;
class ITI_PanelTransfer;
class ITI_THandler; //SJ_YYK 161104 Add..
class MLCNet;
//______________________________________________________________________________


const int ERR_TRS_PANEL_TRANSFER_CENTER_SUCCESS			= 0;
const int DEF_TRS_PANEL_TRANSFER_CETNER_INTERFACE_RETRY	= -1;
//______________________________________________________________________________

typedef enum enumStepPanelTransferCenter
{
	PANEL_TRANSFER_CENTER_MOVE_TO_LOAD_POS,
	PANEL_TRANSFER_CENTER_RECEIVE_PANEL_READY,
	PANEL_TRANSFER_CENTER_RECEIVE_PANEL,
	PANEL_TRANSFER_CLEAN,
	PANEL_TRANSFER_CENTER_SEND_PANEL_READY,
	PANEL_TRANSFER_CENTER_SEND_PANEL,

	// NSMC
	PANEL_TRANSFER_CENTER_MODEL_CHANGE,

} EStepPanelTransferCenter;
//______________________________________________________________________________

typedef struct tagSTrsPanelTransferCenterRefCompList
{
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MPanelTransfer*			m_plnkPanelTransferCenter;
	MPanelAligner*			m_plnkPanelAligner; //170205 SJ_YSH
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
	IStateHandler*			m_plnkStateTurnHandler; //SJ_YYK 161104 Add..

	MNetH*					m_plnkMelsecIF;
	//SJ_YYK 110421 Add.. Ellas
	MLCNet*					m_plnkLCNet;

} STrsPanelTransferCenterRefCompList, *pSTrsPanelTransferCenterRefCompList;
//______________________________________________________________________________

typedef struct tagSTrsPanelTransferCenterData
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;

} STrsPanelTransferCenterData, *pSTrsPanelTransferCenterData;
//______________________________________________________________________________

typedef struct tagSPanelTransferCenterRefITIList
{
	ITI_LoadConveyor*	m_plnkITILoadConveyor;
	ITI_PanelTransfer*	m_plnkITIPanelTransfer;
	ITI_THandler*		m_plnkITITHandler; //SJ_YYK 161104 Add..

} SPanelTransferCentrerRefITIList, *pPanelTransferCenterRefITIList;
//______________________________________________________________________________

class MTrsPanelTransferCenter : public ITI_PanelTransferCenter, public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MPanelTransfer*			m_plnkPanelTransferCenter;
	MPanelAligner*			m_plnkPanelAligner; //170205 SJ_YSH
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
	IStatePanelTransfer*	m_plnkStatePanelTransferCenter;
#ifdef DEF_USE_TURN_HANDLER
	IStateHandler*			m_plnkStateTurnHandler; //SJ_YYK 161104 Add..
#endif
	MNetH*					m_plnkMelsecIF;
	//SJ_YYK 110421 Add.. Ellas
	MLCNet*					m_plnkLCNet;

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;

	ITI_LoadConveyor*	m_plnkITILoadConveyor;
	ITI_PanelTransfer*	m_plnkITIPanelTransfer;
	ITI_THandler*		m_plnkITITHandler; //SJ_YYK 161104 Add..
	
/**********
 *  ITI Flag
 */
	BOOL				m_bITISendStartToPanelTransfer;
	BOOL				m_bITISendCompleteToPanelTransfer;

	BOOL				m_bITISendStartToTHandler;//SJ_YYK 161104 Add..
	BOOL				m_bITISendCompleteToTHandler; //SJ_YYK 161104 Add..

	// NSMC
	BOOL				m_bModelChangeReady;

	BOOL				m_b1FeederAllMountInitReady;
/**********
 *  Threading
 */
	/** Thread ���� Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsPanelTransfer�� Run Flag */	
	BOOL				m_bAutoRun;

	/** MTrsPanelTransfer�� Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** ���� Step ���� */
	EStepPanelTransferCenter	m_estepCurrent;

	/** �Ѵܰ� ���� Step ���� */
	EStepPanelTransferCenter	m_estepPrevious;

/**********
 *  ��Ÿ
 */
	CString				m_strItiStatus;
	MTickTimer			m_timer;

	/** Panel ���� ���� �÷��� */
	BOOL				m_bPanelInStop;

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

	void setStep(EStepPanelTransferCenter estepPanelTransfer);

	void initializeStep();

	void simulLog(CString Msg, int iStep, int iSleep = 0);


public:
/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/
	MTrsPanelTransferCenter::MTrsPanelTransferCenter(SCommonAttribute		commonData,
										STrsPanelTransferCenterRefCompList	listRefComponents,
										STrsPanelTransferCenterData			datComponent);

    ~MTrsPanelTransferCenter();  

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	/**
	* Assign component linkage
	* 
	* @param	listRefComponents	: MTrsPanelTransferCenter ���� Component list
	*/
	int AssignComponents(STrsPanelTransferCenterRefCompList listRefComponents);

	/** 
	* Component Data ó�� 
	*
	* @param	pData : SMMTrsPanelTransferCenterData ����ü�� ���� ������
	* @return	Error : 0 = Success, �׿�=��Ÿ
	*/
	int SetData(STrsPanelTransferCenterData datComponent);

	/** 
	 * TrsPanelTransfer Component�� �ڵ� ���� �ʱ�ȭ�� �����Ѵ�.
	 * @return	Error Code : 0 = SUCCESS, �� �� = Error
	 */
	int Initialize();

	/**
	 * MTrsPanelTransferCenter Initial �Ϸ� ���θ� �����Ѵ�.
	 *
	 * @return void
	 */
	void ResetInitialized(void);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 
	void AssignITI(ITI_THandler* pITI_THandler, ITI_PanelTransfer* pITI_PanelTransfer);
	/**
	 * MTrsPanelTransferCenter Initial �Ϸ� ���� Ȯ��
	 *
	 * @return BOOL (TRUE : Initial �Ϸ�, FALSE : �̿Ϸ�)
	 */
	BOOL IsInitialized();


	BOOL IsSendStartToPanelTransfer();
	BOOL IsSendCompleteToPanelTransfer();

	//SJ_YYK 161104 Add..
	BOOL IsSendStartToTurnHandler();
	BOOL IsSendCompleteToTurnHandler();

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
	void initializeMelsecIF();

/***********************************************************************************************
/*
/* Process Time Information
/*
/************************************************************************************************/ 

	/**
	 *	PanelTransfer �� Panel �� �ִ��� ���� ��ȯ
	 */
	BOOL IsEmptyPanel();

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

private:

};

//______________________________________________________________________________

#endif