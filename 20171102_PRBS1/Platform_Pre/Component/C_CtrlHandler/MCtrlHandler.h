/* 
 * Control Handler Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef MCTRLHANDLER_H
#define MCTRLHANDLER_H

#include "ICommon.h"
//@#include "IStateStage.h"
//@#include "IStateWorkBench.h"
//@#include "IStateHandler.h"
//@#include "DefCleaner.h"
//@#include "MCleaner.h"
//@#include "MInspectCamera.h"
//@#include "MHandler.h"
//@#include "DefSystem.h"
class MHandler;
class MWorkBench;
class MNetH;
class MSystemData;
class MPanelData;
class IStatePanelTransfer;

/************************************************************************/
/*                          Error Code Define                           */
/************************************************************************/
//20050517 ysb
const	int ERR_CTRLHANDLER_SUCCESS								= 0;


/************************************************************************/
/*                        Structure Declaration                         */
/************************************************************************/

typedef
/**
 * Referenced Component List by Unload Handler Component
 */
struct tagSCtrlHandlerRefCompList
{
    /** Handler Object */
    MHandler*		m_plnkHandler;
	/** Cleaner Object */
//@    MCleaner*		m_plnkCleaner;
    /** Stage Object */
//@    IStateStage*	m_plnkStage;	
	/** WorkBench Object */
    MWorkBench*		m_plnkWorkBench;	
//@	IStateInspectCamera* m_plnkInspectCameraCarrier;
//@	IVision*				m_plnkVision;
	MNetH*			m_plnkMelsecNet;

	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransfer;

} SCtrlHandlerRefCompList, *pSCtrlHandlerRefCompList;

typedef
/**
 * Unload Handler Component Data	
 */ 
struct tagSCtrlHandlerData
{
	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
//@	MAcfData*		m_plnkAcfData;

} SCtrlHandlerData, *pSCtrlHandlerData;


typedef
/**
 * Load HandOffPanel function의 SubStep을 정의한다.
 */
enum enumStepLoadHandOffPanel
{
	STEP_LOAD_HANDOFF_INIT,
	STEP_LOAD_HANDOFF_SET_RECEIVE_POSSIBLE,
	STEP_LOAD_HANDOFF_RECEIVE_START_READY,
	STEP_LOAD_HANDOFF_RECEIVE_START,
	STEP_LOAD_HANDOFF_RECEIVE_COMPLETE,
} EStepLoadHandOffPanel;

typedef
/**
 * UnloadHandOffPanel function의 SubStep을 정의한다.
 */
enum enumStepUnLoadHandOffPanel
{
	STEP_UNLOAD_HANDOFF_INIT,
	STEP_UNLOAD_HANDOFF_CHECK_SEND_POSSIBLE,
	STEP_UNLOAD_HANDOFF_SEND,
	STEP_UNLOAD_HANDOFF_SEND_COMPLETE,
	STEP_UNLOAD_HANDOFF_WAIT_LOADING_MOVE_READY,
	STEP_UNLOAD_HANDOFF_WAIT_LOADING_MOVE_COMPLETE,
} EStepUnLoadHandOffPanel;

/************************************************************************/
/*                          Class Declaration                           */
/************************************************************************/

class MCtrlHandler : public virtual ICommon
{
private:
    /** Handler Object */
    MHandler*				m_plnkHandler;

	/** Cleaner Object */
//@    MCleaner*				m_plnkCleaner;
    /** Stage Object */
//@    IStateStage*			m_plnkStage;
	/** Stage Object */
    MWorkBench*				m_plnkWorkBench;	
	/** System Data 관리 Class */
	MSystemData*			m_plnkSystemData;
	MPanelData*				m_plnkPanelData;
//@	MAcfData*		m_plnkAcfData;
//@	IStateInspectCamera*    m_plnkInspectCameraCarrier;
//@	IVision*				m_plnkVision;
	MNetH*					m_plnkMelsecNet;

	IStatePanelTransfer*	m_plnkPanelTransferCenter;
	IStatePanelTransfer*	m_plnkPanelTransfer;


	EStepLoadHandOffPanel	m_estepLoadHandOff;
	EStepLoadHandOffPanel	m_estepLoadHandOffPrevious;

	EStepUnLoadHandOffPanel	m_estepUnloadHandOff;
	EStepUnLoadHandOffPanel	m_estepUnloadHandOffPrevious;

public:
	MCtrlHandler(SCommonAttribute commonData,
				SCtrlHandlerRefCompList listRefComponent, 
				SCtrlHandlerData datComponent);

	virtual ~MCtrlHandler();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
public:

	int Initialize();

	int AssignComponents(SCtrlHandlerRefCompList listRefComponents);

	int SetData(SCtrlHandlerData datComponent);

	int InitializeMelsecIF();
/***********************************************************************************************
/*
/* Handler Operation
/*
/************************************************************************************************/

	int GetPanel();

	int PutPanel();

	/*/
	int UpHandler();

	int PutPanelReady();

	int LoadPanel();
	/*/

	int MoveForLoading();

	int MoveForWaiting();

	int MoveForUnloading();

	/*/
	int GetCameraPosForCurrentJob(int iCurrentJobID);

	int GetHandlerPosForCurrentJob(int iCurrentJobID);

	int	InspectPanel(int iCurrentJobID);
	/*/

/***********************************************************************************************
/*
/* Internal Operation
/*
/************************************************************************************************/
private:
	
	/*/
	void	InitLoadStep();
	
	void	SetLoadStep(EStepLoadHandOffPanel stepVal);
	
	void	initUnloadStep();

	void	setUnloadStep(EStepUnLoadHandOffPanel stepVal);
	/*/
};

#endif //MCTRLHANDLER_H
