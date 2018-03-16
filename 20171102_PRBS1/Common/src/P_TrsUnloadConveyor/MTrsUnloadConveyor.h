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

// 2009.05.22 hjh ver3 && D�� OutConveyor ����.
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
// 2009.05.22 hjh ver3 && D�� OutConveyor ����.
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

// 2009.05.22 hjh ver3 && D�� OutConveyor ����.
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
	/** Thread ���� Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	/** MTrsUnloadConveyor�� Run Flag	*/
	BOOL				m_bAutoRun;

	/** MTrsUnloadConveyor�� Initialize Flag */
	BOOL				m_bAutoInitialized;

	/** ���� Step ���� */
	EStepUnloadConveyor	m_estepCurrent;

	/** �Ѵܰ� ���� Step ����*/
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
/* ������ & �Ҹ���
/*
/************************************************************************************************/

	MTrsUnloadConveyor(SCommonAttribute				commonData, 
					STrsUnloadConveyorRefCompList	listRefComponents, 
					STrsUnloadConveyorData			datComponent );

    ~MTrsUnloadConveyor();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	/**
	 * Assign component linkage
	 * 
	 * @param	listRefComponents	: MTrsUnloadConveyor ���� Component list
	 */
	int AssignComponents(STrsUnloadConveyorRefCompList listRefComponents);

	/** 
	 * Component Data ó�� 
	 *
	 * @param	pData : SMMTrsUnloadConveyorData ����ü�� ���� ������
	 * @return	Error : 0 = Success, �׿�=��Ÿ
	 */
	int SetData(STrsUnloadConveyorData datComponent);

	/** 
	 * MTrsUnloadConveyor Component�� �ڵ� ���� �ʱ�ȭ�� �����Ѵ�.
	 * @return	Error Code : 0 = SUCCESS, �� �� = Error
	 */
	int Initialize();
	
	/**
	 * MTrsUnloadConveyor Initial �Ϸ� ���θ� �����Ѵ�.
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
	 * Initial �Ϸ� ���� Ȯ��
	 *
	 * @return BOOL (TRUE : Initial �Ϸ�, FALSE : �̿Ϸ�)
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
	 *	UnloadConveyor �� Panel �� �ִ��� ���� ��ȯ
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
