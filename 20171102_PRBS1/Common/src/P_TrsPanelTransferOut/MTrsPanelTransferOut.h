//______________________________________________________________________________

// File Name: MTrsPanelTransferOut.h
// History	:
//		1. 2010.07.29 : Created
//		2.
//______________________________________________________________________________

#ifndef _MTRS_PANEL_TRANSFER_OUT_H_
#define _MTRS_PANEL_TRANSFER_OUT_H_
//______________________________________________________________________________

#include "ICommon.h"
#include "ITI_PanelTransferOut.h"
#include "MTickTimer.h"
//______________________________________________________________________________

class MTrsAutoManager;
class IStatePanelAligner;
class MSystemData;
class MPanelTransfer;
class ITI_PanelAligner;
class MNetH;
//______________________________________________________________________________


const int ERR_TRS_PANEL_TRANSFER_OUT_SUCCESS			= 0;
const int DEF_TRS_PANEL_TRANSFER_OUT_INTERFACE_RETRY	= -1;
//______________________________________________________________________________

typedef enum enumStepPanelTransferOut
{
	PANEL_TRANSFER_OUT_MOVE_TO_LOAD_POS,
	PANEL_TRANSFER_OUT_RECEIVE_PANEL_READY,
	PANEL_TRANSFER_OUT_RECEIVE_PANEL,
	PANEL_TRANSFER_OUT_SEND_PANEL_READY,
	PANEL_TRANSFER_OUT_SEND_PANEL,

	// NSMC
	PANEL_TRANSFER_OUT_MODEL_CHANGE,

} EStepPanelTransferOut;
//______________________________________________________________________________

typedef struct tagSTrsPanelTransferOutRefCompList
{
	MTrsAutoManager*		m_plnkTrsAutoManager;
	MPanelTransfer*			m_plnkPanelTransferOut;
	IStatePanelAligner*		m_plnkStatePanelAligner;
	MNetH*					m_plnkMelsecIF;
	MPanelTransfer*         m_plnkPanelTransferIn;

} STrsPanelTransferOutRefCompList, *pSTrsPanelTransferOutRefCompList;
//______________________________________________________________________________

typedef struct tagSTrsPanelTransferOutData
{
	MSystemData*		m_plnkSystemData;

} STrsPanelTransferOutData, *pSTrsPanelTransferOutData;
//______________________________________________________________________________

typedef struct tagSPanelTransferOutRefITIList
{
	ITI_PanelAligner*	m_plnkITIPanelAligner;

} SPanelTransferOutRefITIList, *pPanelTransferOutRefITIList;
//______________________________________________________________________________

class MTrsPanelTransferOut : public ITI_PanelTransferOut, public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MPanelTransfer*		m_plnkPanelTransferOut;
	IStatePanelAligner*	m_plnkStatePanelAligner;
	MNetH*				m_plnkMelsecIF;
	MPanelTransfer*     m_plnkPanelTransferIn;


/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;

	ITI_PanelAligner*	m_plnkITIPanelAligner;
	
/**********
 *  ITI Flag
 */
	BOOL				m_bITIReceiveAblePanelAligner;
	BOOL				m_bITIReceiveStartToPanelAligner;

	BOOL				m_bITISendAbleToNextMachine;
	BOOL				m_bITISendCompleteToNextMachine;

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
	EStepPanelTransferOut	m_estepCurrent;

	/** �Ѵܰ� ���� Step ���� */
	EStepPanelTransferOut	m_estepPrevious;

/**********
 *  ��Ÿ
 */
	CString				m_strItiStatus;

#ifdef SIMULATION
	int					m_iPreStep;
#endif
	MTickTimer			m_timer;

	//121004.kms____
	double m_dTolerance;

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

	void setStep(EStepPanelTransferOut estepPanelTransferOut);

	void initializeStep();

	void simulLog(CString Msg, int iStep, int iSleep = 0);


public:
/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/
	MTrsPanelTransferOut::MTrsPanelTransferOut(SCommonAttribute			commonData,
										STrsPanelTransferOutRefCompList	listRefComponents,
										STrsPanelTransferOutData		datComponent);

    ~MTrsPanelTransferOut();  

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	/**
	* Assign component linkage
	* 
	* @param	listRefComponents	: MTrsPanelTransferOut ���� Component list
	*/
	int AssignComponents(STrsPanelTransferOutRefCompList listRefComponents);

	/** 
	* Component Data ó�� 
	*
	* @param	pData : SMMTrsPanelTransferOutData ����ü�� ���� ������
	* @return	Error : 0 = Success, �׿�=��Ÿ
	*/
	int SetData(STrsPanelTransferOutData datComponent);

	/** 
	 * TrsPanelTransfer Component�� �ڵ� ���� �ʱ�ȭ�� �����Ѵ�.
	 * @return	Error Code : 0 = SUCCESS, �� �� = Error
	 */
	int Initialize();

	/**
	 * MTrsPanelTransferOut Initial �Ϸ� ���θ� �����Ѵ�.
	 *
	 * @return void
	 */
	void ResetInitialized(void);

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/ 
	void AssignITI(ITI_PanelAligner* pITI_PanelAligner);

	/**
	 * MTrsPanelTransferOut Initial �Ϸ� ���� Ȯ��
	 *
	 * @return BOOL (TRUE : Initial �Ϸ�, FALSE : �̿Ϸ�)
	 */
	BOOL IsInitialized();

	BOOL IsReceiveAbleToPanelAligner();
	BOOL IsReceiveStartToPanelAligner();

	BOOL IsSendAbleToNextMachine();
	BOOL IsSendCompleteToNextMachine();

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

	int ScrapPanel();

};

//______________________________________________________________________________
#endif