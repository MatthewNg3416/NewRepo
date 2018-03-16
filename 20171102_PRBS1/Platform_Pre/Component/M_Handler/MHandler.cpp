/* 
 * Handler Component
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
 * Handler.cpp : Implementation of Handler component.
 */

// kss 22 WorkBench ���� �ʿ�
#include "stdafx.h"
//@#include "MStage.h"
#include "MHandler.h"
#include "IOpPanel.h"
#include "DefIOAddrPreBonder.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"
#include <math.h>
#include "IStatePanelTransfer.h"
#include "DefPanelTransfer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

extern MPlatformOlbSystem	MOlbSystemPre;

/** 
 * Constructor
 * 
 * @param	SCommonAttribute commonData (���������� ����� Data)
 * @param	SHandlerRefCompList listRefComponent (������ Component ������ ����Ʈ)
 * @param	SHandlerData datComponent (��ü������ ����� Data)
 */
MHandler::MHandler(SCommonAttribute commonData, SHandlerRefCompList listRefComponent, SHandlerData datComponent)
: MHandlerCommon(commonData, listRefComponent)
{
	SetData(datComponent);

	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir,
											m_plnkSystemData->m_strModelFileName);

	LoadXYTFixedPosParameter();
	LoadXYTOffsetPosParameter();
	LoadZFixedPosParameter();
	LoadZOffsetPosParameter();
	calculateHandlerXYTModelPosParameter();	// XYT Model ��ǥ ���
}

/**
 * @stereotype destructor 
 */
MHandler::~MHandler()
{
}

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

/**
 * Handler ���� Data�� �о�´�.
 *
 * @param	SHandlerData* pData (�о�� Handler Data�� ����� ���)
 * @return	void
 */
void MHandler::GetData(SHandlerData* pData)
{
	pData->m_iHandlerType				= m_iHandlerType;
	pData->m_usiIAddrPanelDetect		= m_usiIAddrPanelDetect;
//@	pData->m_usiIAddrArmViolation		= m_usiIAddrArmViolation;
//@	pData->m_usiOAddrArmViolation		= m_usiOAddrArmViolation;
//@	pData->m_usiIAddrMoveInhibit		= m_usiIAddrMoveInhibit;
//@	pData->m_usiOAddrMoveInhibit		= m_usiOAddrMoveInhibit;
	pData->m_strPosFileName				= m_strPosFileName;
	pData->m_plnkSystemData				= m_plnkSystemData;
	pData->m_sXYTTeachingInfo			= m_sXYTTeachingInfo;	
	pData->m_sZTeachingInfo				= m_sZTeachingInfo;
	pData->m_strOffsetPosFileName		= m_strOffsetPosFileName;
	pData->m_strOffsetPosFilePath		= m_strOffsetPosFilePath;
	pData->m_plnkPanelData				= m_plnkPanelData;
//@	pData->m_plnkAcfData				= m_plnkAcfData;	
}

/**
 * Handler ���� Data�� �����Ѵ�.
 *
 * @param	SHandlerData datComponent (�����ϰ��� �ϴ� Handler Data)
 * @return	void
 */
int MHandler::SetData(SHandlerData datComponent)
{
	m_iHandlerType					= datComponent.m_iHandlerType;
	m_usiIAddrPanelDetect			= datComponent.m_usiIAddrPanelDetect;
//@	m_usiIAddrArmViolation			= datComponent.m_usiIAddrArmViolation;
//@	m_usiOAddrArmViolation			= datComponent.m_usiOAddrArmViolation;
//@	m_usiIAddrMoveInhibit			= datComponent.m_usiIAddrMoveInhibit;
//@	m_usiOAddrMoveInhibit			= datComponent.m_usiOAddrMoveInhibit;
	m_usOAddrIFMoveEnable			= datComponent.m_usOAddrIFMoveEnable;
	m_strPosFileName				= datComponent.m_strPosFileName;
	m_plnkSystemData				= datComponent.m_plnkSystemData;
	m_plnkPanelData					= datComponent.m_plnkPanelData;
	m_sXYTTeachingInfo				= datComponent.m_sXYTTeachingInfo;	
	m_sZTeachingInfo				= datComponent.m_sZTeachingInfo;
	m_strOffsetPosFileName			= datComponent.m_strOffsetPosFileName;
	m_strOffsetPosFilePath			= datComponent.m_strOffsetPosFilePath;
//@	m_plnkAcfData					= datComponent.m_plnkAcfData;

	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� ���� üũ�� ���� �ʿ���  Component List�� Assign �Ѵ�. 
 *
 * @param	SStateCheckCompList : Component List
 */
int MHandler::AssignStateCheckComponents(SStateCheckCompListForHandler sCompList)
{
//@	m_plnkStage					= sCompList.m_plnkStage;
    m_plnkWorkBench			    = sCompList.m_plnkWorkBench;
	m_plnkPanelTransferCenter	= sCompList.m_plnkPanelTransferCenter;
	m_plnkPanelTransferIn		= sCompList.m_plnkPanelTransferIn;

	return ERR_HANDLER_SUCCESS;
}

/**
 * Model ����� �۾��� �����Ѵ�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandler::ChangeModel()
{
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir,
											m_plnkSystemData->m_strModelFileName);

	LoadXYTOffsetPosParameter();
	LoadZOffsetPosParameter();
	calculateHandlerXYTModelPosParameter();
	return ERR_HANDLER_SUCCESS;
}

/**
 * Handler�� �ʱ�ȭ�Ѵ�.
 * 1. Vacuum ���� Check
 * 2. Panel ������ ���� Vacuum On/Off
 * 3. X�� �������� ���� Check
 * 4. Cylinder Up
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MHandler::Initialize()
{
	int iResult;

	/** Vaccuum�� ���������� �����ϴ��� Check */
	iResult = CheckVacuum();
	if (iResult)
		return iResult;

	/** Panel Detect I/O�� NULL�� �ƴ� ��� */
	//kjs1 ��ιݼ� �ڵ鷯�� ��� �г� ������ ��������.

	/** �������Ͱ� �Ǿ� ���� ���� ��� Error Code Return */
	iResult = CheckXYTOrigin();
	if (iResult)
		return iResult;

	iResult = CheckZOrigin();
	if (iResult)
		return iResult;

	return ERR_HANDLER_SUCCESS;
}

/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/ 

/**********
 * Move
 */
int MHandler::checkInterlockForXYT(int iPosID,BOOL bUseAuto)
{
	//  c-h-1
	if(!IsInZPos(DEF_HANDLER_Z_UP_POS))
	{
		/** 211112 = Handler Z���� Up�� �ƴϾ �̵��� �� �����ϴ�. */
		return generateErrorCode(211112);
	}
/*
	if (m_iHandlerType == DEF_HANDLER_UNLOADING)
	{
		if (iPosID == DEF_HANDLER_UNLOAD_POS || TRUE == IsInXPos(DEF_HANDLER_UNLOAD_POS))
		{
			if (TRUE == MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpenNextMC())
			{
				;//return generateErrorCode(211113);
			}
		}
	}
*/
	/*/
	if(m_iHandlerType == DEF_HANDLER_UNLOADING)
	{
		//20070205 cho ���� 
		//if( !m_plnkIO->IsOn(m_usiIAddrArmViolation))
		if( m_plnkIO->IsOn(m_usiIAddrArmViolation))
					// if(m_plnkUnloadMNetInterface->IsHandlerDownInhibitFromStage())
		{
			//211130 = ���� ����κ��� �̵� ���� ��ȣ�� �����Ǿ����ϴ�.
			return generateErrorCode(211130);
		}
	}

	if(!bUseAuto)
	{
		if(m_iHandlerType == DEF_HANDLER_UNLOADING)
		{
			if(IsInXYTPos(DEF_HANDLER_UNLOAD_POS))
			{

			}
		}
		else
		{
			if(IsInXYTPos(DEF_HANDLER_LOAD_POS))
			{

			}
		}
	}
	/*/
	return ERR_HANDLER_SUCCESS;
}

int MHandler::checkInterlockForZ(int iPosID)
{

	/*
	if (iPosID == DEF_HANDLER_Z_LOAD_DOWN_POS)
	{
		if (FALSE == IsInXYTPos(DEF_HANDLER_LOAD_POS))
		{
			Sleep(300);
			if (FALSE == IsInXYTPos(DEF_HANDLER_LOAD_POS))
			{
				return generateErrorCode(211150);
			}
		}
	}
	else if (iPosID == DEF_HANDLER_Z_UNLOAD_DOWN_POS)
	{
		if (FALSE == IsInXYTPos(DEF_HANDLER_UNLOAD_POS))
		{	
			Sleep(300);
			if (FALSE == IsInXYTPos(DEF_HANDLER_UNLOAD_POS))
			{
				return generateErrorCode(211151);
			}
		}
		
	}
	*/	

	if (iPosID == DEF_HANDLER_Z_UP_POS && TRUE == IsAbsorbPanelOneMore())
	{
		/*
		if (TRUE == m_plnkWorkBench->IsAbsorbPanelOneMore() 
			&& TRUE == m_plnkWorkBench->IsInArmPos(DEF_WORKBENCH_ARM_UNLOAD_POS)
			&& TRUE == m_plnkWorkBench->IsInZPos(DEF_WORKBENCH_Z_UP_POS)
			&& FALSE == IsInZPos(DEF_HANDLER_Z_UP_POS))
		{
			//Retry..
			Sleep(300);
			if (TRUE == m_plnkWorkBench->IsAbsorbPanelOneMore() 
			&& TRUE == m_plnkWorkBench->IsInArmPos(DEF_WORKBENCH_ARM_UNLOAD_POS)
			&& TRUE == m_plnkWorkBench->IsInZPos(DEF_WORKBENCH_Z_UP_POS)
			&& FALSE == IsInZPos(DEF_HANDLER_Z_UP_POS))
			{
				return generateErrorCode(211152);
			}
		}
		*/

		if(TRUE == m_plnkPanelTransferIn->IsPanelAbsorbed()
			&& TRUE == m_plnkPanelTransferIn->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS)
			&&  FALSE == IsInZPos(DEF_HANDLER_Z_UP_POS))
		{
			Sleep(300);
			if(TRUE == m_plnkPanelTransferIn->IsPanelAbsorbed()
			&& TRUE == m_plnkPanelTransferIn->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS)
			&&  FALSE == IsInZPos(DEF_HANDLER_Z_UP_POS))
			{
				return generateErrorCode(211152);
			}
		}

		if(TRUE == m_plnkPanelTransferCenter->IsPanelAbsorbed()
			&& TRUE == m_plnkPanelTransferCenter->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS)
			&&  FALSE == IsInZPos(DEF_HANDLER_Z_UP_POS))
		{
			Sleep(300);
			if(TRUE == m_plnkPanelTransferCenter->IsPanelAbsorbed()
			&& TRUE == m_plnkPanelTransferCenter->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS)
			&&  FALSE == IsInZPos(DEF_HANDLER_Z_UP_POS))
			{
				return generateErrorCode(211152);
			}
		}
	}

	return ERR_HANDLER_SUCCESS;
}

/** Handler XYT Move : Interlock check X, Z_Height check X
 */
int MHandler::moveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bUseAuto)
{
	int iResult = ERR_HANDLER_SUCCESS;
	BOOL bBacklaskUse = FALSE;
	
	double dPosT = 0.0;
	MPos_XYT posCurr = GetXYTCurrentPos();  // ���� ��ġ �б�
	if (((posTarget.dT - posCurr.dT) > 0.001) && iPosID != -1)  // Backlash ���� ����̸�
	{
		dPosT = posTarget.dT;
		posTarget.dT += m_plnkSystemData->m_dBacklash ;  // T�� ��ǥ ��ġ + Backlash ������ ����
		bBacklaskUse = TRUE;
	}

	/** T�� ȸ������ 5�� �̻� �϶� Turn ��ġ�� �̵��� T�� ȸ��
	 * �ϰ� ��ǥ ��ġ�� �̵� �Ѵ�.
	 */
	if (fabs(posTarget.dT - posCurr.dT) > 5.0 || bUseAuto == TRUE)
	{
		MPos_XYT posTurn;
		
		posTurn = GetXYTTargetPos(DEF_HANDLER_TURN_POS);

		iResult = m_plnkTAxis->Move(posTarget.dT);  // T�� ��ǥ ��ġ�� �̵�
		if (iResult)
			return iResult;
	}
		
	posCurr = GetXYTCurrentPos();  // ���� ��ġ �б�
	
/*
	iResult = m_plnkXAxis->Move(posTarget.dX); // T���� Turn ��ġ���� �̵��� �����̸� 
	if (iResult)                                //X,Y,T�� ���� ������ �����ص� ������ ����.
		return iResult;
		*/
	
//@	iResult = m_plnkYAxis->Move(posTarget.dY);
//@	if (iResult)
//@		return iResult;

	if (bBacklaskUse == TRUE)  // Backlash ���� ����̸�
	{
		iResult = m_plnkTAxis->StartMove(dPosT);  // T�� ��ǥ ��ġ ���� ��ǥ ��ġ�� �̵�
		if (iResult)
			return iResult;
		
		iResult = m_plnkTAxis->Wait4Done();  // T�� ��ǥ ��ġ �̵� �Ϸ� ���
		if (iResult)
			return iResult;
	}

	return ERR_HANDLER_SUCCESS;
}


/**
 * Handler(X, Y, T��)�� ��ǥ ��ġ�� �д´�.
 *
 * @param	iPosID : Position Index ID
 * @return	MPos_XYT : Handler�� ��ǥ ��ġ (X, Y, T��)
 */
MPos_XYT MHandler::GetXYTTargetPos(int iPosID, int iTabNum)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_HANDLER_MAX_POS);

	MPos_XYT posTarget;

	// ��ǥ��ǥ = ���� ��ǥ	+ ����ǥ + Offset ��ǥ + Etc Offset ��ǥ
	posTarget.dX = m_rgsXYTFixedPos[iPosID].dX
					+ m_rgsXYTModelPos[iPosID].dX
					+ m_rgsXYTOffsetPos[iPosID].dX;
				
	posTarget.dY = m_rgsXYTFixedPos[iPosID].dY
					+ m_rgsXYTModelPos[iPosID].dY
					+ m_rgsXYTOffsetPos[iPosID].dY;
				
	posTarget.dT = m_rgsXYTFixedPos[iPosID].dT
					+ m_rgsXYTModelPos[iPosID].dT
					+ m_rgsXYTOffsetPos[iPosID].dT;
				
	return posTarget;
}

/**
 * Handler(X,Y,T��)�� Index��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @param	posTarget	: Target ��ǥ
 * @param	iPosID		: Position Index ID
 * @param	bUseAuto	: Auto mode ��� ���θ� ���� (default = TRUE)
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandler::MoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_HANDLER_SUCCESS;
#endif
	
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	MPos_XYT posTemp = GetXYTCurrentPos();
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move XYT to PosID : %d] [Start]", GetInstanceNo(), iPosID);
	WriteTactTimeLog(m_szLogMsg);

	int	iResult = ERR_HANDLER_SUCCESS;
	
	iResult	= checkInterlockForXYT(iPosID,bUseAuto);
	if (iResult)
		return iResult;
	
	// Handler XYT Move : Interlock check X, Z_Height check X
	iResult = moveXYTPos(posTarget, iPosID, bUseAuto);
	if (iResult)
		return iResult;

	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	posTemp = GetXYTCurrentPos();
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move XYT to PosID : %d] [End]", GetInstanceNo(), iPosID);
	WriteTactTimeLog(m_szLogMsg);

	return iResult;
}

int MHandler::MoveXPos(double dTarget, int iPosID, BOOL bUseAuto)
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_HANDLER_SUCCESS;
#endif
	
//@	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
//@	MPos_XYT posTemp = GetXYTCurrentPos();
//@	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move XYT to PosID : %d] [Start]", GetInstanceNo(), iPosID);
//@	WriteTactTimeLog(m_szLogMsg);

	int	iResult = ERR_HANDLER_SUCCESS;

	iResult	= checkInterlockForXYT(iPosID,bUseAuto);
	if (iResult)
		return iResult;

	iResult = m_plnkXAxis->Move(dTarget); // T���� Turn ��ġ���� �̵��� �����̸� 
	if (iResult)                                //X,Y,T�� ���� ������ �����ص� ������ ����.
		return iResult;

//@	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
//@	posTemp = GetXYTCurrentPos();
//@	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move XYT to PosID : %d] [End]", GetInstanceNo(), iPosID);
//@	WriteTactTimeLog(m_szLogMsg);

	return iResult;
}

/**
 * Handler(X,Y)�� Turn ��ġ�� �̵���Ų��. Interlock�� Ȯ���Ѵ�.
 *
 * @return	int : (0 = Success, Error Code = �� ��)
 */
int MHandler::MoveXYTurnPos()
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_HANDLER_SUCCESS;
#endif
	
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	MPos_XYT posTemp = GetXYTCurrentPos();
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move XY to Turn Pos] [Start]", GetInstanceNo());
	WriteTactTimeLog(m_szLogMsg);

	int	iResult = ERR_HANDLER_SUCCESS;
	
	iResult	= checkInterlockForXYT(0);
	if (iResult)
		return iResult;
	
	// Handler XYT Move : Interlock check X, Z_Height check X
	MPos_XYT posTurn = GetXYTTargetPos(DEF_HANDLER_TURN_POS);

//@	iResult = m_plnkYAxis->Move(posTurn.dY);
//@	if (iResult)
//@		return iResult;
/*	
	iResult = m_plnkXAxis->Move(posTurn.dX);  
	if (iResult)
		return iResult;
*/
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	posTemp = GetXYTCurrentPos();
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move XY to Turn Pos] [End]", GetInstanceNo());
	WriteTactTimeLog(m_szLogMsg);

	return iResult;
}

int MHandler::MoveZPos(double dTarget, int iPosID, BOOL bUseAuto)
{
	int iResult;

	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move Z to PosID : %d] [Start]", GetInstanceNo(), iPosID);
	WriteTactTimeLog(m_szLogMsg);

	double dTolerance = 1.0;
	if (FALSE == IsInZPos(iPosID, &dTolerance))
	{
		iResult	= checkInterlockForZ(iPosID);
		if (iResult)	
			return iResult;
	}

	dTolerance = 100.0;

	//161229 SJ_YSH Del.
	/*/
	if (FALSE == IsInXPos(DEF_HANDLER_LOAD_POS, &dTolerance))
	{
		if (iPosID != DEF_HANDLER_Z_UP_POS)
//@			m_plnkIO->OutputOff(OUT_O_INTERFACE_15);
			m_plnkIO->OutputOff(m_usOAddrIFMoveEnable);
	}
	/*/

	// �� �̵� ���� 
	iResult = m_plnkZAxis->StartMove(dTarget);
	if (iResult)
	{
		/** MoveZPos() : Handler Z Axis StartMove() Fail */
		return iResult;
	}
 
	// �̵� �Ϸ� ���� 
	iResult = m_plnkZAxis->Wait4Done();
	if (iResult)
	{
		/** MoveZPos() : Handler Z Axis Wait4Done() Fail */
		return iResult;
	}

	if (iPosID == DEF_HANDLER_Z_UP_POS)
	{
//@		m_plnkIO->OutputOn(OUT_O_INTERFACE_15);
		m_plnkIO->OutputOn(m_usOAddrIFMoveEnable);
	}

	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Move Z to PosID : %d] [End]", GetInstanceNo(), iPosID);
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;
}


int MHandler::checkInterlockForVacuumOff()
{

	if (TRUE == IsAbsorbPanel())
	{
	}

	return ERR_HANDLER_SUCCESS;
}

int MHandler::calculateHandlerXYTModelPosParameter()
{
	for (int i=0; i<DEF_HANDLER_MAX_POS; i++)  // Model ��ǥ�� �ʱ�ȭ
	{
		m_rgsXYTModelPos[i].dX = 0.0;
		m_rgsXYTModelPos[i].dY = 0.0;
		m_rgsXYTModelPos[i].dT = 0.0;
	}
	m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dT = 90.0;

#if FALSE	//@SSM

	double dHalfPanelSizeX = m_plnkPanelData->m_dPanelSize_X / 2.0;
	double dHalfPanelSizeY = m_plnkPanelData->m_dPanelSize_Y / 2.0;
	/**********************
	 * Loading Handler Pos
	 **********************/
    
	/*LOADING HANDLER(Source)*/
	//HANDLER UNLOAD POS
	//���� ��ġ:  Stage �߽ɰ� Handler �߽� ��ġ
	//X�� �̵���: ���� (Panel �߽� ����)
	//Y�� �̵���: 36mm - Panel size Y/2
    //Y�� �̵���: ����
    
	//HANDLER LOAD POS
	//���� ��ġ: WorkBench ���� ���ܰ� Handler �߽� ��ġ 
	//X�� �̵���: - Panel size Y/2
	//Y�� �̵���:  -(Panel size Y/2 - 36mm )
    //Y�� �̵���: ����

	/*LOADING HANDLER(gate)*/
	//HANDLER UNLOAD POS
	//���� ��ġ:  Stage �߽ɰ� Handler �߽� ��ġ
	//X�� �̵���: ���� (Panel �߽� ����)
	//Y�� �̵���: DEF_WORKBENCH_PITCH
    //Y�� �̵���: ����    
	
	
#ifndef DEF_USE_TURN_HANDLER
	// 20060717 Load Pos �� ��ǥ �߰�.
	m_rgsXYTModelPos[DEF_HANDLER_LOAD_POS].dX = -(m_plnkPanelData->m_dPanelSize_X / 2.0);

	m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dX = 0.0;
	//32��ġ
	//90421.KKY.Comment : ���ⷮ�� �����ϰ� ����(Stageȸ���߽ɿ��� Panel���ܱ��� �Ÿ� - HalfPanelSizeY ==> ��ɰŸ�)
	if (m_plnkPanelData->m_dPanelSize_X < DEF_40_INCH_PANEL_SIZE_X)
	{
		m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dY = (m_plnkSystemData->m_dPanelOffestData -dHalfPanelSizeY) 
													+ STAGE_HALF_SIZE_Y;
													//- STAGE_32PANEL_GET_OFFSET;
	}
	else
	{
		m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dY = (m_plnkSystemData->m_dPanelOffestData -dHalfPanelSizeY) 
													+ STAGE_HALF_SIZE_Y;
	}
	m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dT = 0.0;
#else	
	if( m_iHandlerType == DEF_HANDLER_LOADING)
	{	
		m_rgsXYTModelPos[DEF_HANDLER_LOAD_POS].dX = 0.0;//-dHalfPanelSizeY;
		m_rgsXYTModelPos[DEF_HANDLER_LOAD_POS].dY = (m_plnkSystemData->m_dPanelOffestData -dHalfPanelSizeY) 
														+ STAGE_HALF_SIZE_Y;
		if (m_plnkPanelData->m_dPanelSize_X >= DEF_46_INCH_PANEL_SIZE_X)
		{		
			m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dX = 0.0;
			m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dY = DEF_ECCENTRIC_OFFSET_Y;
			m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dT = 90;
		}
		//__________________________________________
#	ifdef DEF_MIRROR
			m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dT = 90.0;		
#	else
			m_rgsXYTModelPos[DEF_HANDLER_UNLOAD_POS].dT = -90.0;		
#	endif
	}
	else
	{
		for (int i = 0; i < DEF_HANDLER_MAX_POS ; i++)
		{
			switch (i)
			{
			case DEF_HANDLER_LOAD_POS:
				if (m_plnkPanelData->m_dPanelSize_X < DEF_40_INCH_PANEL_SIZE_X)
				{
					m_rgsXYTModelPos[i].dY = 0.0;//DEF_WORKBENCH_PITCH;
					
				}
				// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
				m_rgsXYTModelPos[i].dT = 90;
#			else
				m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				break; 
			case DEF_HANDLER_UNLOAD_POS:
				// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
				m_rgsXYTModelPos[i].dT = 90;
#			else
				m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif


				break; 
			
			case DEF_HANDLER_INSPECT_XU_POS:
				if (m_plnkPanelData->m_bUseFlag_Yl && m_plnkPanelData->m_bUseFlag_Yr)
				{
					m_rgsXYTModelPos[i].dX =  m_plnkAcfData->m_dY_AcfCheckOffset_Xu[DEF_FIRST_JOB];
			    	// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				}
				else if (m_plnkPanelData->m_bUseFlag_Yl)
				{
					m_rgsXYTModelPos[i].dX = m_plnkAcfData->m_dY_AcfCheckOffset_Xu[DEF_FIRST_JOB];
										// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				}
				break; 
			case DEF_HANDLER_INSPECT_XD_POS:
				if (m_plnkPanelData->m_bUseFlag_Yl && m_plnkPanelData->m_bUseFlag_Yr)
				{
					m_rgsXYTModelPos[i].dX = - m_plnkAcfData->m_dY2_AcfCheckOffset_Xd[DEF_FIRST_JOB];
										// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				}
				else if (m_plnkPanelData->m_bUseFlag_Yl)
				{
					m_rgsXYTModelPos[i].dX = - m_plnkAcfData->m_dY2_AcfCheckOffset_Xd[DEF_FIRST_JOB];
				// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				}
				break;
			case DEF_HANDLER_INSPECT_YF1_POS:
				m_rgsXYTModelPos[i].dX = - dHalfPanelSizeY
									   - m_plnkAcfData->m_dX_AcfCheckOffset_Yr
									   + (m_plnkAcfData->m_AcfCheckOffset_A_Yr
									   - (m_plnkAcfData->m_Acf_TabLength_Yr/2));
				// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				break;
			case DEF_HANDLER_INSPECT_YR1_POS:
				m_rgsXYTModelPos[i].dX = - dHalfPanelSizeY
									   - m_plnkAcfData->m_dX2_AcfCheckOffset_Yr
									   + (m_plnkAcfData->m_AcfCheckOffset_C_Yl
									   - (m_plnkAcfData->m_Acf_TabLength_Yl/2));
				// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				break;
			case DEF_HANDLER_INSPECT_YF2_POS:
				m_rgsXYTModelPos[i].dX =  dHalfPanelSizeY
										+ m_plnkAcfData->m_dX2_AcfCheckOffset_Yl
										- (m_plnkAcfData->m_AcfCheckOffset_B_Yr
									    - (m_plnkAcfData->m_Acf_TabLength_Yr/2));
										
				// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				break;
			case DEF_HANDLER_INSPECT_YR2_POS:
				m_rgsXYTModelPos[i].dX = dHalfPanelSizeY
									   + m_plnkAcfData->m_dX_AcfCheckOffset_Yr
									   - (m_plnkAcfData->m_AcfCheckOffset_D_Yl
									   - (m_plnkAcfData->m_Acf_TabLength_Yl/2));
									// 20060722
#	ifdef DEF_USE_TURN_HANDLER
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;
#			endif
#		endif
#	endif
				break;
#	ifdef DEF_USE_TURN_HANDLER
			case DEF_HANDLER_REMOVE_POS:
#		ifndef DEF_USE_UHANDLER	
#			ifdef DEF_MIRROR
					m_rgsXYTModelPos[i].dT = 90;
#			else
					m_rgsXYTModelPos[i].dT = -90;			
#			endif
#		endif
				break;
#	endif
			default:
				break;
			}

			m_rgsXYTModelPos[i].dT = 0;
		}
//CHO ADD ======================================
		m_rgsXYTModelPos[i].dT = 0.0;
//==============================================
	}

#endif

#endif	//@#if FALSE

  return ERR_HANDLER_SUCCESS;
}

/**
 * Vacuum�� �����Ѵ�.
 *
 * @param	bSkipSensor : (OPTION = FALSE) Vacuum ���� �� ���� üũ ���� (FALSE=Ȯ��, TRUE=����)
 * @return	int : (0 = Success, Error Code = �� ��)
 */

/*/
int MHandler::ReleasePanel(BOOL bSkipSensor)
{
//#ifdef SIMULATION
//	return ERR_HANDLER_SUCCESS;
//#endif
	
	int iResult = ERR_HANDLER_SUCCESS;

    iResult = checkInterlockForVacuumOff();
	if (iResult)
		return iResult;

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Panel Release] [Start], GetInstanceNo() ");
	WriteTactTimeLog(m_szLogMsg);

	/ ** Vacuum Off�Ǿ� �ִ��� Ȯ�� * /
	ASSERT(m_plnkVac1 != NULL && m_plnkVac2 != NULL && m_plnkVac3 != NULL);

	if (m_plnkVac2 != NULL)
		m_plnkVac2->Off(FALSE);
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		m_plnkVac3->Off(FALSE);
	}
	iResult = m_plnkVac1->Off(FALSE);
	if (iResult != ERR_HANDLER_SUCCESS)
	{
		// ���� ���� ���� Log 
//@		WriteErrorLog("Vacuum Release Action Error");
		return iResult;
	}
	iResult = m_plnkVac2->Off(FALSE);
	if (iResult != ERR_HANDLER_SUCCESS)
	{
		// ���� ���� ���� Log 
//@		WriteErrorLog("Vacuum Release Action Error");
		return iResult;
	}
	
	if( m_iHandlerType != DEF_HANDLER_LOADING)
	{
		iResult = m_plnkVac3->Off(FALSE);
		if (iResult != ERR_HANDLER_SUCCESS)
		{
			// ���� ���� ���� Log 
//@			WriteErrorLog("Vacuum Release Action Error");
			return iResult;
		}
		
	}

	// kss 15. Log ���Ŀ� �°� ���� �ʿ� 
	memset(m_szLogMsg, 0x00, sizeof(m_szLogMsg));
	sprintf(m_szLogMsg, " [%d MHandler] [M] [Panel Release] [End], GetInstanceNo() ");
	WriteTactTimeLog(m_szLogMsg);

	return ERR_HANDLER_SUCCESS;
}
/*/

/*
int MHandler::LoadXYTOffsetPosParameter()
{

	/** Stage Offset Loading */
/*	MHandlerCommon::LoadXYTOffsetPosParameter();

	double dPanelSizeX = m_plnkPanelData->m_dPanelSize_X;
	double dPanelSizeY = m_plnkPanelData->m_dPanelSize_Y;

	/** Stage Etc offset Loading */
/*	for (int i = 0; i < DEF_HANDLER_MAX_POS ; i++)
	{
		switch (i)
		{

	    case DEF_HANDLER_INSPECT_XU_POS:
			m_rgsXYTOffsetPos[i].dX = m_plnkAcfData->m_dY_AcfCheckOffset_Xu[DEF_FIRST_JOB];
			break; 
		case DEF_HANDLER_INSPECT_XD_POS:
			m_rgsXYTOffsetPos[i].dX = m_plnkAcfData->m_dY2_AcfCheckOffset_Xd[DEF_FIRST_JOB];
			break;
		case DEF_HANDLER_INSPECT_YF1_POS:
			m_rgsXYTOffsetPos[i].dX = m_plnkAcfData->m_dY2_AcfCheckOffset_Yl;
			break;
		case DEF_HANDLER_INSPECT_YR1_POS:
			m_rgsXYTOffsetPos[i].dX = m_plnkAcfData->m_dY_AcfCheckOffset_Yr;
			break;
		case DEF_HANDLER_INSPECT_YF2_POS:
			m_rgsXYTOffsetPos[i].dX = m_plnkAcfData->m_dY_AcfCheckOffset_Yl;
			break;
		case DEF_HANDLER_INSPECT_YR2_POS:
			m_rgsXYTOffsetPos[i].dX = m_plnkAcfData->m_dX2_AcfCheckOffset_Yr;
		default:
			break;
		}
	}

	return ERR_HANDLER_SUCCESS;
}
*/
// End Of Code
