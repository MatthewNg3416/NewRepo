/* 
 * TabMounter Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include "StdAfx.h"
#include "MSystemData.h"
#include "MPlatformOlbSystem.h"
#include "MPanelData.h"
#include "MProcessData.h"
#include "MTabOffsetData.h"
#include "MAxisData.h"
#include "IAxis.h"
#include "MVacuum.h"
#include "MCylinder.h"
#include "MTabMounter.h"
#include "MTeachingPosData.h"
#include "DefTabAligner.h"
#include "IStateTabAligner.h"
#include "IStatePanelAligner.h"
#include "IStatePanelTransfer.h"
#include "DefPanelTransfer.h"
#include "IStateCameraCarrier.h"
#include <math.h>
#include "DefPanelAligner.h"
#include "MProcessData.h"
#include "MTrsAutoManager.h"
#include "MTabCarrier.h"
#include "MPreBonderData.h"
#include "common.h"

extern MPlatformOlbSystem	MOlbSystemPre;

CCriticalSection MTabMounter::m_csMoveLock;

double MTabMounter::sm_rgdFixedGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];	
double MTabMounter::sm_rgdModelGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];	
double MTabMounter::sm_rgdOffsetGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];

double MTabMounter::sm_rgdFixedGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];	
double MTabMounter::sm_rgdModelGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];	
double MTabMounter::sm_rgdOffsetGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];

//20111205 SJ_HJG
//double MTabMounter::sm_rgdFinalTabOffset[20];	
double MTabMounter::sm_rgdOldAkReceiveData[20];

MTabMounter::MTabMounter(SCommonAttribute commonData, STabMounterRefCompList listRefComponents, STabMounterData datComponent)
: ICommon(commonData)
{
	AssignComponents(listRefComponents);
	SetData(datComponent);

//	m_iWorkGroup = m_iGroupNo;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (m_iInstanceNo < DEF_MOUNTER_WORK_PER_GROUP)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = DEF_MAX_TABMOUNTER - 1 - m_iInstanceNo;
	}
#else
	/*/
	if (m_iInstanceNo < DEF_MOUNTER_WORK_PER_GROUP)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo - DEF_MAX_TABMOUNTER;
	}
//@#endif

	m_plnkPanelAligner = NULL;

	m_iMaxPos[DEF_MOUNT_Y] = DEF_TABMOUNTER_MAX_POS;
	m_iMaxPos[DEF_MOUNT_Z] = DEF_TABMOUNTER_Z_MAX_POS;
	m_iMaxPos[DEF_GANTRY] = DEF_TABMOUNTER_GANTRY_MAX_POS;

	m_rgdFixedPos[DEF_MOUNT_Y] = m_rgdFixedPosMounter;
	m_rgdModelPos[DEF_MOUNT_Y] = m_rgdModelPosMounter;
	m_rgdOffsetPos[DEF_MOUNT_Y] = m_rgdOffsetPosMounter;

	m_rgdFixedPos[DEF_MOUNT_Z] = m_rgdFixedPosMounterZ;
	m_rgdModelPos[DEF_MOUNT_Z] = m_rgdModelPosMounterZ;
	m_rgdOffsetPos[DEF_MOUNT_Z] = m_rgdOffsetPosMounterZ;

	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		m_rgdFixedPos[DEF_GANTRY] = sm_rgdFixedGantryPos1;
		m_rgdModelPos[DEF_GANTRY] = sm_rgdModelGantryPos1;
		m_rgdOffsetPos[DEF_GANTRY] = sm_rgdOffsetGantryPos1;
	}
	else
	{
		m_rgdFixedPos[DEF_GANTRY] = sm_rgdFixedGantryPos2;
		m_rgdModelPos[DEF_GANTRY] = sm_rgdModelGantryPos2;
		m_rgdOffsetPos[DEF_GANTRY] = sm_rgdOffsetGantryPos2;
	}

	int i = 0;
	for (int i = 0; i < DEF_TABMOUNTER_AXIS_TYPE; i++)
	{
		if (i == DEF_GANTRY && m_iInstanceNo != DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo])
			break;
		for (int j = 0; j < m_iMaxPos[i]; j++)
		{
			m_rgdFixedPos[i][j] = 0.0;
			m_rgdOffsetPos[i][j] = 0.0;
			m_rgdModelPos[i][j] = 0.0;

			if (i == DEF_MOUNT_Y)
			{
				m_rgdCalculatedFixedPosMounter[j] = 0.0;
			}
		}

		LoadOffsetPosParameter(i);
		LoadFixedPosParameter(i);
		calculateFixedPosParameter(i);
		calculateModelPosParameter(i);
	}

	m_bGantryMotionEnable = FALSE;

	m_bMoving = FALSE;//KKY

	//20111205 SJ_HJG
#ifdef DEF_SOURCE_SYSTEM
	CString strAkReceiveData;
	double dAkReceiveData = 0.0;
	MGlassSummarys* pGs;
	pGs = new MGlassSummarys;
	MOlbSystemPre.GetMelsecComponent()->GetGlassSummaryWord(pGs,9);

	for (int i = 0; i < 20; i++)
	{
		if (m_iWorkGroup == DEF_FRONT_GROUP)
			strAkReceiveData = pGs->m_strReceiveTabOffsetDataA[i];
		else
			strAkReceiveData = pGs->m_strReceiveTabOffsetDataB[i];

		sm_rgdOldAkReceiveData[i] = _wtoi(strAkReceiveData) / 10;

	//	sm_rgdFinalTabOffset[i] = m_plnkTabOffsetData->GetTabOffsetMounter(m_eWorkSide, i);
	}
#endif
	//______________________-

//	m_iGantryLoadCarrierID = FALSE;
}

MTabMounter::~MTabMounter()
{
}

/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

void MTabMounter::SetData(STabMounterData datComponent)
{
	m_plnkSystemData				= datComponent.m_plnkSystemData;
	m_plnkPanelData					= datComponent.m_plnkPanelData;
	m_plnkPreBonderData				= datComponent.m_plnkPreBonderData;
	m_plnkProcessData				= datComponent.m_plnkProcessData;
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]	= datComponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND] = datComponent.m_plnkWorkScheduleSecond;
#endif
	m_plnkTabOffsetData				= datComponent.m_plnkTabOffsetData;
	//120208.kms_______
	m_plnkRearTabOffsetData         = datComponent.m_plnkRearTabOffsetData;
	//_________________
	m_sTeachingInfo[DEF_MOUNT_Y]	= datComponent.m_sTeachingInfoMounter;
	m_sTeachingInfo[DEF_MOUNT_Z]	= datComponent.m_sTeachingInfoMounterZ;
	m_sTeachingInfo[DEF_GANTRY]		= datComponent.m_sTeachingInfoGantry;
	m_strPosFileName				= datComponent.m_strPosFileName;
	m_strOffsetPosFileName			= datComponent.m_strOffsetPosFileName;
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);

	m_iGroupNo						= datComponent.m_iGroupNo;
	m_eWorkSide						= datComponent.m_eWorkSide;
	m_iPreTabMounterNo				= datComponent.m_iPreTabMounterNo;
	m_iNextTabMounterNo				= datComponent.m_iNextTabMounterNo;
}

void MTabMounter::AssignComponents(STabMounterRefCompList TabMounterCompList)
{
	m_plnkAxis			= TabMounterCompList.m_plnkAxis;
	m_plnkAxisZ			= TabMounterCompList.m_plnkAxisZ;
	m_plnkGantryAxis	= TabMounterCompList.m_plnkGantryAxis;

	m_plnk1stUDCyl		= TabMounterCompList.m_plnk1stUDCyl;
	m_plnkVac			= TabMounterCompList.m_plnkVac;
	m_plnkPusherUDCyl	= TabMounterCompList.m_plnkPusherUDCyl;
}

void MTabMounter::AssignStateCheckComponents(SStateCheckCompListForTabMounter sCompList)
{
	int i = 0;
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		m_plnkCamCarrier[i] = sCompList.m_plnkCamCarrier[i];

	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		m_plnkTabMounter[i]	= sCompList.m_plnkTabMounter[i];

	m_plnkPanelAligner		= sCompList.m_plnkPanelAligner;

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		m_plnkTabCarrier[i] = sCompList.m_plnkTabCarrier[i];

	m_plnkPanelTransferOut	= sCompList.m_plnkPanelTransferOut;

	calculateFixedPosParameter(DEF_MOUNT_Y);
}

IAxis* MTabMounter::GetMotionComponent()
{
	return m_plnkAxis;
}

IAxis* MTabMounter::GetMotionComponentZ()
{
	return m_plnkAxisZ;
}

// Doolin kwangilshin 2017. 09. 05.
//
IAxis* MTabMounter::GetMotionComponentGantry()
{
	return m_plnkGantryAxis;
}
// End.

int MTabMounter::ChangeModel(void)
{
	// Model Path Change
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);

	for (int i = 0; i < DEF_TABMOUNTER_AXIS_TYPE; i++)
	{
		int iResult = LoadOffsetPosParameter(i);
		if (iResult != ERR_TABMOUNTER_SUCCESS)
			return iResult;
		
		calculateFixedPosParameter(i);
		calculateModelPosParameter(i);
	}

	return ERR_TABMOUNTER_SUCCESS;
}

/************************************************************************/
/*        Teaching 관련 Interface                                       */
/************************************************************************/

int MTabMounter::LoadOffsetPosParameter(int iAxisType)
{
	if (iAxisType == DEF_GANTRY && m_iInstanceNo != DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo])
		return ERR_TABMOUNTER_SUCCESS;

	if (iAxisType == DEF_GANTRY && NULL == m_plnkGantryAxis)
		return ERR_TABMOUNTER_SUCCESS;

	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(iAxisType != DEF_GANTRY);
#endif
	*/

	MTeachingPosData TabMounterOffsetPosDat(m_sTeachingInfo[iAxisType], m_strOffsetPosFileName, m_strOffsetPosFilePath);

	BOOL bResult = TabMounterOffsetPosDat.ReadData();	
	for (int i = 0; i < m_iMaxPos[iAxisType]; i++)
	{
		m_rgdOffsetPos[iAxisType][i] = TabMounterOffsetPosDat.m_TeachingPos[i][0];
	}
	if (!bResult)
	{
		if (DEF_MOUNT_Y == iAxisType)
			// 228000 = Offset 좌표 파일 읽기를 실패 했습니다.
			return generateErrorCode(228000);
		else if (iAxisType == DEF_MOUNT_Z)
			// 228000 = Offset 좌표 파일 읽기를 실패 했습니다.
			return generateErrorCode(228105);
		else if (iAxisType == DEF_GANTRY)
			// 228012 = Gantry Offset 좌표 파일 읽기를 실패 했습니다.
			return generateErrorCode(228012);
	}

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::LoadFixedPosParameter(int iAxisType)
{
	if (iAxisType == DEF_GANTRY && m_iInstanceNo != DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo])
		return ERR_TABMOUNTER_SUCCESS;

	if (iAxisType == DEF_GANTRY && NULL == m_plnkGantryAxis)
		return ERR_TABMOUNTER_SUCCESS;

	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(iAxisType != DEF_GANTRY);
#endif
	*/
	MTeachingPosData TabMounterFixPosDat(m_sTeachingInfo[iAxisType], m_strPosFileName);

	BOOL bResult = TabMounterFixPosDat.ReadData();
	for (int i = 0; i < m_iMaxPos[iAxisType]; i++)
	{
		m_rgdFixedPos[iAxisType][i] = TabMounterFixPosDat.m_TeachingPos[i][0];
	}
	if (!bResult)
	{
		if (DEF_MOUNT_Y == iAxisType)
			// 228001 = 고정좌표를 Load 하는데 실패 했습니다. 
			return generateErrorCode(228001);
		else if (DEF_MOUNT_Z == iAxisType)
			// 228001 = 고정좌표를 Load 하는데 실패 했습니다. 
			return generateErrorCode(228106);
		else if (DEF_GANTRY == iAxisType)
			// 228013 = Gantry 고정좌표를 Load 하는데 실패 했습니다. 
			return generateErrorCode(228013);
	}
	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::SaveOffsetPosParameter(int iAxisType, const double* const pdOffsetPos)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(iAxisType != DEF_GANTRY);
#endif
	*/
	MTeachingPosData TabMounterOffsetPosDat(m_sTeachingInfo[iAxisType], m_strOffsetPosFileName, m_strOffsetPosFilePath);

	for (int i = 0; i < m_iMaxPos[iAxisType]; i++)
	{
		TabMounterOffsetPosDat.m_TeachingPos[i][0] = m_rgdOffsetPos[iAxisType][i] = pdOffsetPos[i];
	}
	BOOL bResult = TabMounterOffsetPosDat.WriteData();
	if (!bResult)
	{
		if (DEF_MOUNT_Y == iAxisType)
			// 228002 = Offset 좌표를 저장하는데 실패 했습니다.
			return generateErrorCode(228002);
		else if (DEF_MOUNT_Z == iAxisType)
			// 228002 = Offset 좌표를 저장하는데 실패 했습니다.
			return generateErrorCode(228107);
		else if (DEF_GANTRY == iAxisType)
			// 228014 = Gantry Offset 좌표를 저장하는데 실패 했습니다.
			return generateErrorCode(228014);
	}
	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::SaveFixedPosParameter(int iAxisType, const double* const dFixedPos)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(iAxisType != DEF_GANTRY);
#endif
	*/

	MTeachingPosData TabMounterFixPosDat(m_sTeachingInfo[iAxisType], m_strPosFileName);
	
	for (int i = 0; i < m_iMaxPos[iAxisType]; i++)
	{
		m_rgdFixedPos[iAxisType][i] = dFixedPos[i];

		if (DEF_MOUNT_Y == iAxisType)
		{
			m_rgdFixedPos[iAxisType][i] -= m_rgdCalculatedFixedPosMounter[i];

			// 고정좌표 Teaching 불가 위치인 경우
			// Source : Align1, Align2, Inspection, Mount, Remove1
			// Gate : Align1, Align2, Inspection, Mount
			switch (i)
			{
			//case DEF_TABMOUNTER_REMOVE1_POS:
			case DEF_TABMOUNTER_REMOVE_POS:
				if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
					break;
			case DEF_TABMOUNTER_INSPECTION_POS:
			case DEF_TABMOUNTER_MOUNT_POS:
/// 임시				m_rgdFixedPos[DEF_MOUNT_Y][i] = 0.0;
				break;

			default:
				break;
			}
		}

		TabMounterFixPosDat.m_TeachingPos[i][DEF_TABMOUNTER_AXIS] = m_rgdFixedPos[iAxisType][i];
	}
	BOOL bResult = TabMounterFixPosDat.WriteData();
	if (FALSE == bResult)
	{
		if (DEF_MOUNT_Y == iAxisType)
			// 228003 = 고정 좌표를 저장하는데 실패 했습니다. 
			return generateErrorCode(228003);
		else if (DEF_MOUNT_Z == iAxisType)
			// 228003 = 고정 좌표를 저장하는데 실패 했습니다. 
			return generateErrorCode(228108);
		else if (DEF_GANTRY == iAxisType)
			// 228015 = Gantry 고정 좌표를 저장하는데 실패 했습니다. 
			return generateErrorCode(228015);
	}
	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::GetPosParameter(int iAxisType, double* pdFixedPos, double* pdOffsetPos, double* pdModelPos)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(iAxisType != DEF_GANTRY);
#endif
	*/
	for (int i = 0; i < m_iMaxPos[iAxisType]; i++)
	{
		if (pdFixedPos != NULL)
		{
			if (DEF_MOUNT_Y == iAxisType)
			{
				if (i == DEF_TABMOUNTER_INSPECTION_POS || i == DEF_TABMOUNTER_MOUNT_POS)
					pdFixedPos[i] = m_rgdCalculatedFixedPosMounter[i];
				else
					pdFixedPos[i] = m_rgdFixedPos[iAxisType][i];
			}
			else
			{
				pdFixedPos[i] = m_rgdFixedPos[iAxisType][i];
			}
		}

		if (pdModelPos != NULL)
			pdModelPos[i] = m_rgdModelPos[iAxisType][i];

		if (pdOffsetPos != NULL)
			pdOffsetPos[i] = m_rgdOffsetPos[iAxisType][i];
	}
	return ERR_TABMOUNTER_SUCCESS;
}
	
STeachingInfo MTabMounter::GetTeachingInfo(int iAxisType)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(iAxisType != DEF_GANTRY);
#endif
	*/
	return m_sTeachingInfo[iAxisType];
}

int MTabMounter::SafeMovePos(int iScheduleNo, int iPosID, BOOL bWaitOption, int iTabNumber, BOOL bAuto)
{
	double dTarget = GetTargetPos(iScheduleNo, iPosID, iTabNumber);
	return MovePos(dTarget, iPosID, bWaitOption, bAuto);
}

int MTabMounter::MovePos(double dTarget, int iPosID, BOOL bWaitOption, BOOL bAuto)
{
	CString strLogMsg;
	ASSERT(iPosID >= DEF_TABMOUNTER_NONE_POS && iPosID < DEF_TABMOUNTER_MAX_POS);

#ifdef SIMULATION
	Sleep(500);
	return ERR_TABMOUNTER_SUCCESS;
#endif
	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Move Pos(%d), WaitOption(%d)] [Start]", m_iInstanceNo + 1, iPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

//170826 JSh.s
	if(IsInPos(dTarget) == TRUE)
		return ERR_TABMOUNTER_SUCCESS;
//170826 JSh.s

	int iResult = checkInterlock(iPosID, dTarget, bAuto);
	if (iResult != ERR_TABMOUNTER_SUCCESS)
	{
		m_bGantryMotionEnable = FALSE;
		return iResult;
	}


//@#ifdef DEF_GATE_SYSTEM

	// Gantry 축 존재하고, Motion 활성화 되어 있으면 Gantry 축 Pos ID 받아서 Gantry 축도 이동 시작
	// 이 때 활성화 플래그 확인 주의. 아래 TabMounter 동작과 동시동작 처리 될 수 있도록 할 것.
	if (NULL != m_plnkGantryAxis && TRUE == m_bGantryMotionEnable)
	{
		int iGantryPosID = getGantryPosID(iPosID);
		ASSERT(iGantryPosID >= DEF_TABMOUNTER_NONE_POS && iGantryPosID < DEF_TABMOUNTER_GANTRY_MAX_POS);

		if (DEF_TABMOUNTER_NONE_POS != iGantryPosID)
		{
			// 20081007 jdy 위로부터 위치 이동함. : TabMounter Ready 위치로 이동 시에는 Gantry 이동 하지 않으려 함.
			// Gantry 이동 시작 후, 활성화 Flag Clear!
			// Gantry 동작을 위해서는 항상 Move 함수 호출 전에 Gantry 축 활성화 설정 필요
			// : Teaching Dlg 에서도 Gantry 활성화 빠뜨리지 않도록 주의할 것!
			m_bGantryMotionEnable = FALSE;
			
//			double dGantryTargetPos = GetTargetGantryPos(iGantryPosID, m_iGantryLoadCarrierID);
			double dGantryTargetPos = GetTargetGantryPos(iGantryPosID);
			
			iResult = moveGantryPos(dGantryTargetPos, iGantryPosID, FALSE, bAuto);
			if (iResult)
				return iResult;
		}
	}

//@#endif

	m_csMoveLock.Lock();
	SetMovingY(TRUE, dTarget);

	double posDiff = dTarget - m_plnkAxis->GetCurrentPosition(FALSE);
	if (fabs(posDiff) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		double dNormalVel = m_plnkAxis->GetMovingVelocity();
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkAxis->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / DEF_DIVIDER_FOR_LOW_SPEED;

		iResult = m_plnkAxis->StartMove(dTarget, dLowVel);
	}
	else
		iResult = m_plnkAxis->StartMove(dTarget);

	if (iResult != ERR_TABMOUNTER_SUCCESS)
	{
		SetMovingY(FALSE, dTarget);
		m_csMoveLock.Unlock();
		return iResult;
	}
	//170327_KDH Test.. 기존 10
	Sleep(30);	// WaitOption == FALSE 일 경우를 대비한 Motion 동작 시작 후 안정화 Delay. 	

	if (FALSE == bWaitOption)
	{
#ifndef SIMULATION
		SetMovingY(FALSE, dTarget);
		m_csMoveLock.Unlock();
		if (TRUE == m_plnkAxis->IsAmpFault() || FALSE == m_plnkAxis->IsAmpEnable())
		{
			strLogMsg.Format(_T("TAB MOUNTER[%d] move Position(Move Pos=%d)"),m_iInstanceNo + 1, iPosID);
			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);
			// 228006 = 이동 전, Amp Fault 혹은 Amp Disable 상태임.
			return generateErrorCode(228006);
		}
#endif
	}
	else
	{
		m_csMoveLock.Unlock();

		iResult = Wait4Done();
		if (iResult)
			return iResult;

		SetMovingY(FALSE, dTarget);
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Move Pos(%d), WaitOption(%d)] [End]", m_iInstanceNo + 1, iPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::Wait4Done(BOOL bSkipMode)
{
	int iResult = m_plnkAxis->Wait4Done(bSkipMode);
	if (iResult)
		return iResult;

	/*
#ifdef DEF_SOURCE_SYSTEM
	return ERR_TABMOUNTER_SUCCESS;
#endif
	*/
	
	return m_plnkGantryAxis->Wait4Done(bSkipMode);
}

int MTabMounter::SafeMoveZPos(int iPosID, BOOL bWaitOption, BOOL bAuto)
{
	double dTarget = GetTargetPosZ(iPosID);
	return MoveZPos(dTarget, iPosID, bWaitOption, bAuto);
}

int MTabMounter::MoveZPos(double dTarget, int iPosID, BOOL bWaitOption, BOOL bAuto)
{
	CString strLogMsg;
	ASSERT(iPosID >= DEF_TABMOUNTER_Z_NONE_POS && iPosID < DEF_TABMOUNTER_Z_MAX_POS);

#ifdef SIMULATION
	Sleep(500);
	return ERR_TABMOUNTER_SUCCESS;
#endif

	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [MoveZ Pos(%d), WaitOption(%d)] [Start]", m_iInstanceNo + 1, iPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);
	
	if (TRUE == IsInPosZ(dTarget))
		return ERR_TABMOUNTER_SUCCESS;

	int iResult = checkInterlockZ(iPosID, dTarget, bAuto);
	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;

	double posDiff = dTarget - m_plnkAxisZ->GetCurrentPosition(FALSE);
	if (fabs(posDiff) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		if (DEF_TABMOUNTER_Z_MOUNT_POS != iPosID)
		{
			double dNormalVel = m_plnkAxisZ->GetMovingVelocity();
			if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
				dNormalVel = m_plnkAxisZ->GetSlowMovingVelocity();
			
			//100930.KKY_____
			//		double dLowVel = dNormalVel / DEF_DIVIDER_FOR_LOW_SPEED;
			//		iResult = m_plnkAxisZ->StartMove(dTarget, dLowVel);
			iResult = m_plnkAxisZ->StartMove(dTarget, dNormalVel);
			//_______________
		}
		else
		{
			double dDownVel = m_plnkSystemData->m_dMountMoveSpeed;
			iResult = m_plnkAxisZ->StartMove(dTarget, dDownVel);
		}
	}
	else
	{
		double vel = 0.0;
		if (DEF_TABMOUNTER_Z_LOAD_POS == iPosID)
		{
			vel = 100.0;
			iResult = m_plnkAxisZ->StartMove(dTarget, vel);
			//iResult = m_plnkAxisZ->StartMove(dTarget, 100.0, 100, 100);
		}
		else if (DEF_TABMOUNTER_Z_MOUNT_POS == iPosID)
		{
			double dDownVel = m_plnkSystemData->m_dMountMoveSpeed;
			iResult = m_plnkAxisZ->StartMove(dTarget, dDownVel);
		}
		else
		{
			double dNormalVel = m_plnkAxisZ->GetMovingVelocity();
			iResult = m_plnkAxisZ->StartMove(dTarget, dNormalVel);
		}
	}

	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;

	Sleep(50);	// WaitOption == FALSE 일 경우를 대비한 Motion 동작 시작 후 안정화 Delay. 

	if (FALSE == bWaitOption)
	{
#ifndef SIMULATION
		if (TRUE == m_plnkAxisZ->IsAmpFault() || FALSE == m_plnkAxisZ->IsAmpEnable())
		{
			strLogMsg.Format(_T("TAB MOUNTER[%d] move Z Position(Move Pos=%d)"),m_iInstanceNo + 1, iPosID);
			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);
			// 228006 = 이동 전, Amp Fault 혹은 Amp Disable 상태임.
			return generateErrorCode(228109);
		}
#endif
	}
	else
	{
		iResult = Wait4DoneZ();
		if (iResult)
			return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [MoveZ Pos(%d), WaitOption(%d)] [End]", m_iInstanceNo + 1, iPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::Wait4DoneZ(BOOL bSkipMode)
{
	return m_plnkAxisZ->Wait4Done(bSkipMode);
}	

//int MTabMounter::SafeMoveGantryPos(int iGantryPosID, int iLoadCarrierID, BOOL bWaitOption, BOOL bAuto)
int MTabMounter::SafeMoveGantryPos(int iGantryPosID, BOOL bWaitOption, BOOL bAuto)
{
	if (NULL == m_plnkGantryAxis)
		return ERR_TABMOUNTER_SUCCESS;

//	double dTarget = GetTargetGantryPos(iGantryPosID, iLoadCarrierID);
	double dTarget = GetTargetGantryPos(iGantryPosID);
	return MoveGantryPos(dTarget, iGantryPosID, bWaitOption, bAuto);
}

int MTabMounter::MoveGantryPos(double dGantryTargetPos, int iGantryPosID, BOOL bWaitOption, BOOL bAuto)
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_TABMOUNTER_SUCCESS;
#endif

	if (NULL == m_plnkGantryAxis)
		return ERR_TABMOUNTER_SUCCESS;

	// 이 함수를 이용하면 Gantry Enable Flag 상태에 관계없이 Gantry 를 움직일 수 있다.

	int iResult = checkInterlockForGantry(iGantryPosID, dGantryTargetPos, bAuto);
	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;
	
	iResult = moveGantryPos(dGantryTargetPos, iGantryPosID, bWaitOption, bAuto);
	if (iResult)
		return iResult;
	
	if (TRUE == bWaitOption)
	{
		iResult = Wait4Done();
		if (iResult)
			return iResult;
	}
	
	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::moveGantryPos(double dGantryTargetPos, int iGantryPosID, BOOL bWaitOption, BOOL bAuto)
{
#ifdef SIMULATION
	Sleep(500);
	return ERR_TABMOUNTER_SUCCESS;
#endif

	CString strLogMsg;
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Gantry Move Pos(%d), WaitOption(%d)] [Start]", m_iInstanceNo + 1, iGantryPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

	int iResult = ERR_TABMOUNTER_SUCCESS;

	double posDiff = dGantryTargetPos - m_plnkGantryAxis->GetCurrentPosition(FALSE);
	if (fabs(posDiff) < (double)m_plnkSystemData->m_iDistance4DifVel)
	{
		double dNormalVel = m_plnkGantryAxis->GetMovingVelocity();
		if (VELOCITY_MODE_SLOW == MAxisData::eVelocityMode)
			dNormalVel = m_plnkGantryAxis->GetSlowMovingVelocity();
		
		double dLowVel = dNormalVel / DEF_DIVIDER_FOR_LOW_SPEED;

		iResult = m_plnkGantryAxis->StartMove(dGantryTargetPos, dLowVel);
	}
	else
		iResult = m_plnkGantryAxis->StartMove(dGantryTargetPos);

	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;

	if (TRUE == bWaitOption)
	{
		iResult = m_plnkGantryAxis->Wait4Done();
		if (iResult)
			return iResult;
	}

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Gantry Move Pos(%d), WaitOption(%d)] [End]", m_iInstanceNo + 1, iGantryPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::CheckOrigin(void)
{
	if (m_plnkAxis->IsOriginReturn() == FALSE)
		// 228005 = TabMounter 축 원점복귀가 되어있지 않습니다.
		return generateErrorCode(228005);

	if (m_plnkAxisZ->IsOriginReturn() == FALSE)
		// 228005 = TabMounter 축 원점복귀가 되어있지 않습니다.
		return generateErrorCode(228110);

//@#ifdef DEF_GATE_SYSTEM
	if (m_plnkGantryAxis->IsOriginReturn() == FALSE)
		// 228016 = Gantry 축 원점복귀가 되어있지 않습니다.
		return generateErrorCode(228016);
//@#endif

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::CheckOriginZ()
{
	if (m_plnkAxisZ->IsOriginReturn() == FALSE)
		// 228005 = TabMounter 축 원점복귀가 되어있지 않습니다.
		return generateErrorCode(228110);
	return 	ERR_TABMOUNTER_SUCCESS;
}

double MTabMounter::GetCurrentPos(BOOL bType)
{
	double dCurPos = m_plnkAxis->GetCurrentPosition(bType);
	return dCurPos;
}

double MTabMounter::GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_TABMOUNTER_MAX_POS);

	//20111205 SJ_HJG
	CString strAutoAlignLog;

	// m_rgdModelPos 는 더 이상 의미 없음. ProcessData 에서 값 가져옴.

//	double dTarget = m_rgdFixedPos[DEF_MOUNT_Y][iPosID] + m_rgdCalculatedFixedPosMounter[iPosID] + m_rgdOffsetPos[DEF_MOUNT_Y][iPosID] + m_rgdModelPos[DEF_MOUNT_Y][iPosID];
	double dTarget = m_rgdOffsetPos[DEF_MOUNT_Y][iPosID] + m_rgdModelPos[DEF_MOUNT_Y][iPosID];

	if (iPosID == DEF_TABMOUNTER_INSPECTION_POS || iPosID == DEF_TABMOUNTER_MOUNT_POS)
	{
		dTarget += m_rgdCalculatedFixedPosMounter[iPosID];
	}
	else
		dTarget += m_rgdFixedPos[DEF_MOUNT_Y][iPosID];

	if ((DEF_TABMOUNTER_INSPECTION_POS == iPosID || DEF_TABMOUNTER_MOUNT_POS == iPosID) && -1 != iTabNumber)
	{
		double dModelPosByTabData = m_plnkWorkSchedule[iScheduleNo]->GetTabPos(iTabNumber);
		double dTabOffset = 0.0;
		double dToolOffset = 0.0;
//110113.Del		double dTabPrealignOffset = m_plnkProcessData->GetTabPrealignOffset(m_eWorkSide, iTabNumber);
		double dTabPrealignOffset = 0.0;
		double dInspectionOffset = 0.0;


		if (DEF_TABMOUNTER_MOUNT_POS == iPosID)
		{
			//if (m_plnkSystemData->m_bUseInspectCamera == TRUE)
			//@130106.KKY_______
//@			dInspectionOffset = m_plnkWorkSchedule[iScheduleNo]->GetInspectionOffset(iTabNumber).dY;//확인요
			dInspectionOffset = m_plnkWorkSchedule[iScheduleNo]->GetInspectionOffset(iTabNumber).dX;//확인요
			//@_________________

			//120208.kms_________
//			dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			if(m_iWorkGroup == DEF_FRONT_GROUP)
				dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			else
				dTabOffset = m_plnkRearTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			//___________________

			dToolOffset = m_plnkSystemData->m_rgdToolOffsetMounter[m_iInstanceNo];
			
			//20111205 SJ_HJG Auto Align System 적용
#ifdef DEF_SOURCE_SYSTEM
			if(m_plnkSystemData->m_bUseAutoAlignSystem == TRUE)
			{	
				dTabOffset = GetReceiveAkdata(m_eWorkSide, iTabNumber);
			}
			else
#endif
			{
				dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			}
			//_________________________________
		}
		
		double dPanelPressPosPos = 0.0;
		/*확인요_____
		if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
		{
			MPos_XYT rgsFixedPos[DEF_PANEL_ALIGNER_MAX_POSITION];
			m_plnkPanelAligner->GetXYTPosParameter(rgsFixedPos);
			dPanelPressPosPos = rgsFixedPos[DEF_PANEL_ALIGNER_PRESS_XU_POS].dX;
		}
		*/
		
		dTarget += dModelPosByTabData + dTabPrealignOffset + dInspectionOffset + dTabOffset + dToolOffset + dPanelPressPosPos;		
	}
	//SJ_YYK 150109 Add...
	if(DEF_TABMOUNTER_LOAD_POS == iPosID)
	{
		double dLoadOffset = 0.0;
	}
	//___________________________________*/
	return dTarget;
}

double MTabMounter::GetCurrentPosZ(BOOL bType)
{
	double dCurPos = m_plnkAxisZ->GetCurrentPosition(bType);
	return dCurPos;
}

double MTabMounter::GetTargetPosZ(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_TABMOUNTER_Z_MAX_POS);
	double dTarget = m_rgdFixedPos[DEF_MOUNT_Z][iPosID] + m_rgdOffsetPos[DEF_MOUNT_Z][iPosID] + m_rgdModelPos[DEF_MOUNT_Z][iPosID];
	return dTarget;
}

//확인요_____
double MTabMounter::CalculateMountPos(double dModelPos)
{
	double dTarget = m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] + m_rgdOffsetPos[DEF_MOUNT_Y][DEF_TABMOUNTER_INSPECTION_POS] + dModelPos;
	return dTarget;
}
//___________

double MTabMounter::GetCurrentGantryPos(BOOL bType)
{
	// 확인 필요 : DEF_TABMOUNTER_GANTRY_MOUNT_POS 인 경우 inspection 결과를 반영해서 Gantry 가 움직여야 한다. - 일단 보류

	if (NULL == m_plnkGantryAxis)
		return 0.0;

	double dCurPos = m_plnkGantryAxis->GetCurrentPosition(bType);
	return dCurPos;
}

//double MTabMounter::GetTargetGantryPos(int iGantryPosID, int iCarrierID)
double MTabMounter::GetTargetGantryPos(int iGantryPosID)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
#endif
	*/
	/*
	ASSERT(iGantryPosID < DEF_TABMOUNTER_GANTRY_LOAD2_POS || iGantryPosID > DEF_TABMOUNTER_GANTRY_LOAD4_POS);
	if (iGantryPosID == DEF_TABMOUNTER_GANTRY_LOAD1_POS)
		iGantryPosID += iCarrierID;
	*/
	ASSERT(iGantryPosID >= DEF_TABMOUNTER_NONE_POS && iGantryPosID < DEF_TABMOUNTER_GANTRY_MAX_POS);
	double dTarget = m_rgdFixedPos[DEF_GANTRY][iGantryPosID] + m_rgdOffsetPos[DEF_GANTRY][iGantryPosID] + m_rgdModelPos[DEF_GANTRY][iGantryPosID];
	return dTarget;
}

// 주의! : m_bGantryMotionEnable 설정 상태에 따라, TabMounter Pos ID 를 이용해서 Gantry 위치까지도 확인 가능
BOOL MTabMounter::IsInPos(int iScheduleNo, int iPosID, double* pdTolerance)
{
	double dMounterAxisCompare = GetTargetPos(iScheduleNo, iPosID);
	BOOL bMounterAxisSts = IsInPos(dMounterAxisCompare, pdTolerance);

	BOOL bGantryAxisSts = TRUE;
//@#ifdef DEF_GATE_SYSTEM
	if (NULL != m_plnkGantryAxis && TRUE == m_bGantryMotionEnable)
	{
		int iGantryPosID = getGantryPosID(iPosID);
//		double dGantryAxisCompare = GetTargetGantryPos(iGantryPosID, m_iGantryLoadCarrierID);
		double dGantryAxisCompare = GetTargetGantryPos(iGantryPosID);
		bGantryAxisSts = IsInGantryPos(dGantryAxisCompare, pdTolerance);
	}
//@#endif
	return bMounterAxisSts && bGantryAxisSts;
}

// 주의! : TabMounter 위치만 확인 가능.
BOOL MTabMounter::IsInPos(double dTargetPos, double* pdTolerance)
{
	return m_plnkAxis->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

BOOL MTabMounter::IsInPosZ(int iPosID, double* pdTolerance)
{
	double dTargetPos = GetTargetPosZ(iPosID);
	return IsInPosZ(dTargetPos, pdTolerance);
}

BOOL MTabMounter::IsInPosZ(double dTargetPos, double* pdTolerance)
{
	return m_plnkAxisZ->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

// 주의! : Gantry 위치만 확인 가능.
//BOOL MTabMounter::IsInGantryPos(int iGantryPosID, int iLoadCarrierID, double* pdTolerance)
BOOL MTabMounter::IsInGantryPos(int iGantryPosID, double* pdTolerance)
{
	if (NULL == m_plnkGantryAxis)
		return FALSE;

//	double dCompare = GetTargetGantryPos(iGantryPosID, iLoadCarrierID);
	double dCompare = GetTargetGantryPos(iGantryPosID);
	return IsInGantryPos(dCompare, pdTolerance);
}

// 주의! : Gantry 위치만 확인 가능.
BOOL MTabMounter::IsInGantryPos(double dGantryTargetPos, double* pdTolerance)
{
	if (NULL == m_plnkGantryAxis)
		return FALSE;

	return m_plnkGantryAxis->ComparePosition(FALSE, dGantryTargetPos, pdTolerance);
}

int MTabMounter::checkInterlock(int iPosID, double dTarget, BOOL bAuto)
{
#ifdef SIMULATION
	return ERR_TABMOUNTER_SUCCESS;
#endif

	int iResult = ERR_TABMOUNTER_SUCCESS;
	double dCurrentPos = m_plnkAxis->GetCurrentPosition();
//	BOOL bDir = (dTarget > dCurrentPos) ? TRUE /* (+) 방향 */ : FALSE /* (-) 방향 */;
	
	iResult = CheckCollisionOtherD(dTarget);
	if (iResult)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			iResult = CheckCollisionOtherD(dTarget);
			if (iResult == 0)
				break;
			if (GetTickCount() - sTime > 3000)
				break;
			//170327_KDH Test 중.. 기존 10
			Sleep(30);
		}
		if (iResult)
			return iResult;
	}

	//110414.Add_____
	BOOL bMoveShort = FALSE;
	if (fabs(dTarget-dCurrentPos) <= 5.0)
		bMoveShort = TRUE;
	BOOL bZMoveUp = FALSE;
		bZMoveUp = TRUE;

	if (fabs(dTarget-dCurrentPos) <= 1)
		bZMoveUp = FALSE;
	//_______________

	iResult = ERR_TABMOUNTER_SUCCESS;
	double dPos = GetTargetPosZ(DEF_TABMOUNTER_Z_INSPECTION_POS) + DEF_TABMOUNTER_SAFE_HEIGHT;

	if(bZMoveUp == TRUE) //171005 JSH
	{
		if (CheckOriginZ() != ERR_TABMOUNTER_SUCCESS)
		{
			iResult = SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
		}
		else
		{
			if (iPosID != -1)	//110123.임시
			{
				double dTolerance = 0.1;
				if (TRUE == IsInGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS, &dTolerance) || TRUE == IsInGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS, &dTolerance))
				{
					if (FALSE == IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance) && FALSE == IsInPosZ(dPos, &dTolerance))
					{
						iResult = SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
					}
				}
				else
				{
					if (FALSE == IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
					{
						iResult = SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
					}
				}
				//170715_KDH 임시삭제
				/*
				if (FALSE == IsUpPusherCyl())
				{
				iResult = UpPusherCyl();
				if (iResult)
				{
				SetErrorLevel(_T("TAB MOUNTER check Interlock for Gantry"),2, __FILE__, __LINE__);
				return iResult;
				}
				}
				*/
				//______________
			}
		}
		if (iResult)
			return iResult;
	}
//@#endif

//@#ifdef DEF_GATE_SYSTEM
	if (NULL != m_plnkGantryAxis && TRUE == m_bGantryMotionEnable)
	{
		int iGantryPosID = getGantryPosID(iPosID);
//		double dGantryTargetPos = GetTargetGantryPos(iGantryPosID, m_iGantryLoadCarrierID);
		double dGantryTargetPos = GetTargetGantryPos(iGantryPosID);

		iResult = checkInterlockForGantry(iGantryPosID, dGantryTargetPos, bAuto);
		if (iResult)
			return iResult;
	}
//@#endif

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::checkInterlockZ(int iPosID, double dTarget, BOOL bAuto)
{
	int iResult = ERR_TABMOUNTER_SUCCESS;

	// Cylinder 항상 하강되어 있어야 함.
	if (FALSE == IsDownCyl())
	{
		iResult = DownCyl(TRUE);
		if (iResult)
			return iResult;
	}
	
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
		return iResult;

	int iDownType = DEF_TABMOUNTER_DOWN_TYPE_MIDDLE;
	MPos_XYT posTolerance(1.0, 2.0, 2.0);
	switch (iPosID)
	{
///	case DEF_TABMOUNTER_Z_MOVE_POS:
///		break;
	case DEF_TABMOUNTER_Z_LOAD_POS:
		if (FALSE == IsInPos(0, DEF_TABMOUNTER_LOAD_POS) && FALSE == IsInPos(0, DEF_TABMOUNTER_R_LOAD_POS))
		{
			Sleep(500);
			if (FALSE == IsInPos(0, DEF_TABMOUNTER_LOAD_POS) && FALSE == IsInPos(0, DEF_TABMOUNTER_R_LOAD_POS))
			{
				return generateErrorCode(228100);
			}
		}
		/*/
#ifdef DEF_SOURCE_SYSTEM
		if (TRUE == IsInPos(DEF_TABMOUNTER_LOAD_POS))
		{
			if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS, &posTolerance))
			{
				return generateErrorCode(228111);
			}
		}
		else if (TRUE == IsInPos(DEF_TABMOUNTER_R_LOAD_POS))
		{
			if (FALSE == m_plnkTabCarrierOther->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS, &posTolerance))
			{
				return generateErrorCode(228111);
			}
		}
#endif
		/*/
		iDownType = DEF_TABMOUNTER_DOWN_TYPE_MIDDLE;
		break;
///	case DEF_TABMOUNTER_Z_R_LOAD_POS:
///		if (FALSE == IsInPos(DEF_TABMOUNTER_R_LOAD_POS))
///		{
///			return generateErrorCode(228101);
///		}
///#ifdef DEF_SOURCE_SYSTEM
///		if (FALSE == m_plnkTabCarrierOther->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS, &posTolerance))
///		{
///			return generateErrorCode(228112);
//		}
///#endif
///		iDownType = DEF_TABMOUNTER_DOWN_TYPE_MIDDLE;
///		break;
///	case DEF_TABMOUNTER_Z_INSPECTION_POS:
///		if (FALSE == IsInPos(DEF_TABMOUNTER_INSPECTION_POS))
///		{
///			return generateErrorCode(228102);
///		}
///		iDownType = DEF_TABMOUNTER_DOWN_TYPE_DOWN;
///		break;
///	case DEF_TABMOUNTER_Z_MOUNT_POS:
///		if (FALSE == IsInPos(DEF_TABMOUNTER_MOUNT_POS))
///		{
///			return generateErrorCode(228103);
///		}
///		iDownType = DEF_TABMOUNTER_DOWN_TYPE_DOWN;
///		break;
	case DEF_TABMOUNTER_Z_REMOVE_POS:
		//if (FALSE == IsInPos(DEF_TABMOUNTER_REMOVE1_POS))
		if (FALSE == IsInPos(0, DEF_TABMOUNTER_REMOVE_POS))
		{
			return generateErrorCode(228104);
		}
		iDownType = DEF_TABMOUNTER_DOWN_TYPE_MIDDLE;
		break;
	}
///	iResult = checkInterlockForDown(iDownType);
	return iResult;
}


int MTabMounter::checkInterlockForGantry(int iPosID, double dTarget, BOOL bAuto)
{	
	int iResult = ERR_TABMOUNTER_SUCCESS;
	//Inspection위치에서 Mount위치로 이동시 
	double dTolerance = 0.1;
	if (TRUE == IsInGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS, &dTolerance))
	{
		if (iPosID == DEF_TABMOUNTER_GANTRY_MOUNT_POS)
		{
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				dTolerance = 0.1;
				double dPos = m_plnkTabMounter[i]->GetTargetPosZ(DEF_TABMOUNTER_Z_INSPECTION_POS) + DEF_TABMOUNTER_SAFE_HEIGHT;
				if (FALSE == m_plnkTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance) && FALSE == m_plnkTabMounter[i]->IsInPosZ(dPos, &dTolerance))
				{
					iResult = m_plnkTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
					if (iResult)
						return iResult;
				}

				//170715_KDH 임시삭제
/*
				if (FALSE == m_plnkTabMounter[i]->IsUpPusherCyl())
				{
					iResult = m_plnkTabMounter[i]->UpPusherCyl();
					if (iResult)
					{
						SetErrorLevel(_T("TAB MOUNTER check Interlock for Gantry"),2, __FILE__, __LINE__);
						return iResult;
					}
				}
				*/
				//________
			}
		}
		else
		{
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				if (FALSE == m_plnkTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
				{
					iResult = m_plnkTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
					if (iResult)
						return iResult;
				}
				//170715_KDH 임시삭제
	/*
				if (FALSE == m_plnkTabMounter[i]->IsUpPusherCyl())
				{
					iResult = m_plnkTabMounter[i]->UpPusherCyl();
					if (iResult)
					{
						SetErrorLevel(_T("TAB MOUNTER check Interlock for Gantry"),2, __FILE__, __LINE__);
						return iResult;
					}
				}
				*/
				//____________
			}
		}
	}
	else
	{
		if (iPosID != -1)	//110123.임시
		{
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				if (FALSE == m_plnkTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
				{
					iResult = m_plnkTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
					if (iResult)
						return iResult;
				}
				//170715_KDH 임시삭제
	/*
				if (FALSE == m_plnkTabMounter[i]->IsUpPusherCyl())
				{
					iResult = m_plnkTabMounter[i]->UpPusherCyl();
					if (iResult)
					{
						SetErrorLevel(_T("TAB MOUNTER check Interlock for Gantry"),2, __FILE__, __LINE__);
						return iResult;
					}
				}
				*/
				//__________________
			}
		}
	}
	// Gantry 간 충돌 방지 : 가운데 Stopper 있음.

	return ERR_TABMOUNTER_SUCCESS;
}

void MTabMounter::calculateFixedPosParameter(int iAxisType)
{
	if (iAxisType == DEF_MOUNT_Z)
		return;
	
	/*/
#ifdef DEF_SOURCE_SYSTEM
	//Tool중심을 PanelAligner 중심으로 맞춘다
	m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = -m_plnkSystemData->m_dAlignCam1_FixedPos_Y
					- m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[DEF_FRONT_GROUP][DEF_INSPECTION_CARRIER_1]/2.0;
	m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MOUNT_POS] = -m_plnkSystemData->m_dAlignCam1_FixedPos_Y
					- m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[DEF_FRONT_GROUP][DEF_INSPECTION_CARRIER_1]/2.0;

#else
	/*/
#ifdef DEF_SOURCE_SYSTEM
	double TabWidth = m_plnkPreBonderData->m_dLead_Width_Xtab;
#else
	double TabWidth = m_plnkPreBonderData->m_dLead_Width_Ytab;
#endif

	if (iAxisType == DEF_MOUNT_Y)
	{
		//Tool 중심을 Panel Xu끝단에 맞춘다(돌출량 일정) -> MarkPos(ex.1.5)는 Model좌표에 적용
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
//			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = -m_plnkSystemData->m_dAlignCam1_FixedPos_X
//							- DEF_PANELALIGNER_DIST_CENTERTOEDGE - DEF_PANELALIGNER_DIST_PROJECT // + m_plnkPanelData->m_dY_FiduMark_Xu
//							- m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[m_iGroupNo][DEF_INSPECTION_CARRIER_1]/2.0;

			/*/
			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = -m_plnkSystemData->m_dCamCarrier1_FixedPosX
							- MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X()
							- m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[m_iGroupNo][DEF_INSPECTION_CARRIER_1]/2.0
#ifdef DEF_GATE_SYSTEM
							+ m_plnkPanelData->m_dY_FiduMark_Xu;
#else
							+ m_plnkPanelData->m_dX_FiduMark_Xu;
#endif
			/*/
			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = m_plnkSystemData->m_dCamCarrier1_FixedPosX
								//@- TabWidth/2.0;
								- m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0;

			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MOUNT_POS] = m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS];
		}
		else
		{
//			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = -m_plnkSystemData->m_dAlignCam2_FixedPos_X
//							- DEF_PANELALIGNER_DIST_CENTERTOEDGE - DEF_PANELALIGNER_DIST_PROJECT //+ m_plnkPanelData->m_dY_FiduMark_Xu
//							- m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[m_iGroupNo][DEF_INSPECTION_CARRIER_1]/2.0;
			/*/
			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = -m_plnkSystemData->m_dCamCarrier2_FixedPosX
							- MOlbSystemPre.GetDistance_StageCenterToPanelEdge_X()
							- m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[m_iGroupNo][DEF_INSPECTION_CARRIER_1]/2.0
#ifdef DEF_GATE_SYSTEM
							+ m_plnkPanelData->m_dY_FiduMark_Xu;
#else
							+ m_plnkPanelData->m_dX_FiduMark_Xu;
#endif
			/*/
			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] = m_plnkSystemData->m_dCamCarrier2_FixedPosX
								//@- TabWidth/2.0;
								- m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0;

			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MOUNT_POS] = m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS];
		}

		/*
		if (m_plnkPanelAligner != NULL)
		{
			MPos_XYT rgsFixedPos[DEF_PANEL_ALIGNER_MAX_POSITION];
			MPos_XYT rgsModelPos[DEF_PANEL_ALIGNER_MAX_POSITION];
			m_plnkPanelAligner->GetXYTPosParameter(rgsFixedPos, NULL, rgsModelPos);
			double dPanelPressPos = rgsFixedPos[DEF_PANEL_ALIGNER_PRESS_XU_POS].dX + rgsModelPos[DEF_PANEL_ALIGNER_PRESS_XU_POS].dX;
			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] =
			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MOUNT_POS] += dPanelPressPos;
		}
		*/
	}
	/*/
	else if (iAxisType == DEF_GANTRY)
	{
		m_rgdFixedPos[DEF_GANTRY][DEF_TABMOUNTER_GANTRY_INSPECTION_POS] = 
		m_rgdFixedPos[DEF_GANTRY][DEF_TABMOUNTER_GANTRY_MOUNT_POS] = 0.0;

	}
	/*/
//@#endif

}

void MTabMounter::calculateModelPosParameter(int iAxisType)
{
	if (iAxisType == DEF_GANTRY && m_iInstanceNo != DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo])
		return;

	if (iAxisType == DEF_GANTRY && NULL == m_plnkGantryAxis)
		return;

	for (int i = 0; i < m_iMaxPos[iAxisType]; i++)
		m_rgdModelPos[iAxisType][i] = 0.0;

//@#ifdef DEF_GATE_SYSTEM
	if (iAxisType == DEF_MOUNT_Y)
	{
//		m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] =
//		m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MOUNT_POS] = m_plnkPanelData->m_dY_FiduMark_Xu;
		;
	}
	else if (iAxisType == DEF_GANTRY)
	{
#ifdef DEF_GATE_SYSTEM
		double dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Xu/2.0;
#else
//		double dHalfFiduDist = m_plnkPanelData->m_dDistance_FiduMark_Yl/2.0;
		double dHalfFiduDist = m_plnkPanelData->m_dPanelSize_Y/2.0;
#endif
		double dMarkDistPanelToTab = 0.0;
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;
	#else
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;
	#endif
#else
			//SJ_YYK 150907 Modify...
	#ifndef DEF_MIRROR_
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xd;
	#else
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xu;
	#endif
#endif
			m_rgdModelPos[DEF_GANTRY][DEF_TABMOUNTER_GANTRY_INSPECTION_POS]
				= m_rgdModelPos[DEF_GANTRY][DEF_TABMOUNTER_GANTRY_MOUNT_POS]
				= -m_plnkSystemData->m_dGantryOriginDist[m_iGroupNo] - (dHalfFiduDist + dMarkDistPanelToTab + DEF_TABMARK_VISION_CENTER_OFFSET - m_plnkPanelData->m_dX_FiduMark_Yl);//171003 JSH
		}
		else
		{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;
	#else
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;
	#endif
#else
		//SJ_YYK 150907 Modify...
	#ifndef DEF_MIRROR_
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xu;
	#else
			dMarkDistPanelToTab = m_plnkPanelData->m_dMarkDist_PanelToTab_Xd;
	#endif
#endif
			m_rgdModelPos[DEF_GANTRY][DEF_TABMOUNTER_GANTRY_INSPECTION_POS]
				= m_rgdModelPos[DEF_GANTRY][DEF_TABMOUNTER_GANTRY_MOUNT_POS]
				= -m_plnkSystemData->m_dGantryOriginDist[m_iGroupNo]
					+ (dHalfFiduDist + dMarkDistPanelToTab + DEF_TABMARK_VISION_CENTER_OFFSET - m_plnkPanelData->m_dY_FiduMark_Yl); //171003 JSH
		}
	}
//@#endif
}

int MTabMounter::getGantryPosID(int iTabMounterPosID)
{
	switch (iTabMounterPosID)
	{
	case DEF_TABMOUNTER_READY_POS:
	case DEF_TABMOUNTER_LOAD_POS:
		return DEF_TABMOUNTER_GANTRY_LOAD_POS;
//		return DEF_TABMOUNTER_GANTRY_LOAD1_POS;

	case DEF_TABMOUNTER_INSPECTION_POS:
		return DEF_TABMOUNTER_GANTRY_INSPECTION_POS;

	case DEF_TABMOUNTER_MOUNT_POS:
		return DEF_TABMOUNTER_GANTRY_MOUNT_POS;

//	case DEF_TABMOUNTER_R_LOAD_POS://Spare
//	case DEF_TABMOUNTER_REMOVE_POS:
	default:
		break;		
	}
	return DEF_TABMOUNTER_NONE_POS;
}

int MTabMounter::UpCyl(BOOL bSkipSensor)
{
	int iResult = m_plnk1stUDCyl->Up(bSkipSensor);
	if (iResult)
		return iResult;
	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::DownCyl(BOOL bSkipSensor)
{
	int iResult = m_plnk1stUDCyl->Down(bSkipSensor);
	if (iResult)
		return iResult;
	return ERR_TABMOUNTER_SUCCESS;
}

BOOL MTabMounter::IsUpCyl(void)
{
	return m_plnk1stUDCyl->IsUp();
}

BOOL MTabMounter::IsDownCyl(void)
{
	return m_plnk1stUDCyl->IsDown();
}

//@_________________________________________
int MTabMounter::UpPusherCyl(BOOL bSkipSensor)
{
	//170715_KDH 임시삭제
	//int iResult = m_plnkPusherUDCyl->Up(bSkipSensor);
	//if (iResult)
	//	return iResult;
	//____________
	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::DownPusherCyl(BOOL bSkipSensor)
{
	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Pusher Down]", m_iInstanceNo + 1);
	WriteNormalLog(szLogMsg);
	//170715_KDH 임시삭제
///	int iResult = m_plnkPusherUDCyl->Down(bSkipSensor);
//	if (iResult)
//		return iResult;
	//__________
	return ERR_TABMOUNTER_SUCCESS;
}

BOOL MTabMounter::IsUpPusherCyl(void)
{
	return m_plnkPusherUDCyl->IsUp();
}

BOOL MTabMounter::IsDownPusherCyl(void)
{
	return m_plnkPusherUDCyl->IsDown();
}
//@_________________________________________

int MTabMounter::CheckVacuum(void)
{
	return m_plnkVac->CompareIO();
}

int MTabMounter::AbsorbTabIC(BOOL bSkipSensor)
{
	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Vacuum Absorb] [Start]", m_iInstanceNo + 1);
	WriteTactTimeLog(szLogMsg);

	CString strLogMsg;

	int iResult = m_plnkVac->On(bSkipSensor);
	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Vacuum Absorb] [End]", m_iInstanceNo + 1);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::ReleaseTabIC(BOOL bSkipSensor)
{
	char szLogMsg[200];
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Vacuum Release] [Start]", m_iInstanceNo + 1);
	WriteTactTimeLog(szLogMsg);

	CString strLogMsg;

	int iResult = m_plnkVac->Off(bSkipSensor);
	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabMounter%d] [M] [Vacuum Release] [End]", m_iInstanceNo + 1);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABMOUNTER_SUCCESS;
}

BOOL MTabMounter::IsAbsorbTabIC(void)
{
	return m_plnkVac->IsOn();
}

BOOL MTabMounter::IsReleaseTabIC(void)
{
	return m_plnkVac->IsOff();
}


int MTabMounter::checkInterlockForDown(int iDownType)
{
#ifdef SIMULATION
	return ERR_TABMOUNTER_SUCCESS;
#endif

	int iResult = CheckOrigin();
	if (iResult != ERR_TABMOUNTER_SUCCESS)
		return iResult;

	double dTolerance = 1.5;
	
	if (TRUE == IsInPos(0, DEF_TABMOUNTER_LOAD_POS, &dTolerance))
	{
		// Load 위치에서 2차 하강 불가.
		if (DEF_TABMOUNTER_DOWN_TYPE_DOWN == iDownType)
			// 228028 = 2차 하강 불가. [Load 위치임.]
			return generateErrorCode(228028);
		
		if (TRUE == IsAbsorbTabIC())
			// 228029 = 1차 하강 불가. [Load 위치에서 TabIC 흡착 상태임.]
			return generateErrorCode(228029);
	}
	//else if (TRUE == IsInPos(DEF_TABMOUNTER_REMOVE1_POS, &dTolerance))
	else if (TRUE == IsInPos(0, DEF_TABMOUNTER_REMOVE_POS, &dTolerance))
	{
		if (DEF_TABMOUNTER_DOWN_TYPE_DOWN == iDownType)
			// 228030 = 2차 하강 불가. [Source 설비 : Remove 위치임.]
			return generateErrorCode(228030);
	}
	else
	{
/*		double dCurrentPos = GetCurrentPos();
		double dRefPos = GetTargetPos(DEF_TABMOUNTER_INSPECTION_POS);
		if (dCurrentPos > dRefPos - 1.0)
		{
			/*KKY.Del
			// Inspection, Mount 위치에서 Backup up,forward 아니면 2차 하강 불가.
			if (DEF_TABMOUNTER_DOWN_TYPE_DOWN == iDownType && (FALSE == IsUpBackupUDCyl() || FALSE == IsForwardBackupFBCyl()))
			{
			SetErrorLevel(_T("TAB MOUNTER check Interlock for Down"),2, __FILE__, __LINE__);
			// 228027 = 2차 하강 불가. [Backup Up 아님.]
			return generateErrorCode(228027);
			}
			*/
/*			;
		}
		else
		{
			SetErrorLevel(_T("TAB MOUNTER check Interlock for Down"),2, __FILE__, __LINE__);
			// 228034 = 하강 불가. [정해진 Mounter 위치 아님.]
			return generateErrorCode(228034);
		}*/
	}

	// TabMounter 하강 시, 아래에 panel 이 있는지 확인 필요하다.
	// => 확인할 조건이 적절치 않으므로 Backup Up 상태로 판단하고 별도의 Interlock 은 추가하지 않는다.

	return ERR_TABMOUNTER_SUCCESS;
}

#if FALSE	//==============================================

int MTabMounter::checkInterlockForBackupUp()
{
	if (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed()
		&& FALSE == m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
	{
		SetErrorLevel(_T("TAB MOUNTER check Interlock for Backup Up"),2, __FILE__, __LINE__);
		// 228023 = Backup Up 불가. [Unload 위치에 있지 않은 Panel Transfer 가 Panel 흡착하고 있음.]
		return generateErrorCode(228023);
	}

	if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed()
//		&& (FALSE == m_plnkPanelAligner->IsInXPos(DEF_PANEL_ALIGNER_PRESS_XU_POS)
//			|| FALSE == m_plnkPanelAligner->IsInYPos(DEF_PANEL_ALIGNER_PRESS_XU_POS)))
		&& FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS))
	{
		SetErrorLevel(_T("TAB MOUNTER check Interlock for Backup Up"),2, __FILE__, __LINE__);
		// 228033 = Backup Up 불가. [PanelAligner 가 Press Xu 위치 아님.]
		return generateErrorCode(228033);
	}

	// Group 내의 모든 TabMounter UD Cylinder Up 상태 확인.
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		if (FALSE == m_plnkTabMounter[i]->IsUpUDCyl()
			&& FALSE == m_plnkTabMounter[i]->IsDownReady())
		{
			SetErrorLevel(_T("TAB MOUNTER check Interlock for Backup Up"),2, __FILE__, __LINE__);
			// 228020 = Backup Up 불가. [Group 내 TabMounter 중 2차 하강 상태인 TabMounter 있음.]
			return generateErrorCode(228020);
		}
	}

	return ERR_TABMOUNTER_SUCCESS;
}

#endif		//#if FALSE	//==================================

void MTabMounter::SetFixedOffsetBySystemData()
{
	for (int i = 0; i < DEF_TABMOUNTER_AXIS_TYPE; i++)
	{
//KKY.Del		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType && DEF_GANTRY == i)
//KKY.Del			break;
		
		calculateFixedPosParameter(i);
	}
}

const BOOL MTabMounter::IsUpAllTabMounterInGroup()
{
	double dTolerance = 0.1;
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		if (TRUE == IsInSafePosTabMounterFromAligner(i))
			continue;
		//100930.KKY_____
		double dPos = m_plnkTabMounter[i]->GetTargetPosZ(DEF_TABMOUNTER_Z_INSPECTION_POS) + DEF_TABMOUNTER_SAFE_HEIGHT;
		if (FALSE == m_plnkTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance)
			&& FALSE == m_plnkTabMounter[i]->IsInPosZ(dPos, &dTolerance))
		//_______________
			return FALSE;
	}

	return TRUE;
}

const BOOL MTabMounter::IsInSafePosTabMounterFromAligner(int nToolID)
{
	/*/
#ifdef DEF_SOURCE_SYSTEM
	double dNRef = m_plnkTabMounter[3]->GetTargetPos(DEF_TABMOUNTER_LOAD_POS) + DEF_TABMOUNTER_TOOL_SIZE;
	double dPRef = m_plnkTabMounter[4]->GetTargetPos(DEF_TABMOUNTER_LOAD_POS) - DEF_TABMOUNTER_TOOL_SIZE;
	if (m_plnkTabMounter[nToolID]->GetCurrentPos() <= dNRef || m_plnkTabMounter[nToolID]->GetCurrentPos() >= dPRef)
		return TRUE;
	return FALSE;
#else
	/*/
	//Front일 경우
	//	Gantry가 Loading위치보다 작거나 같을 경우
	//Rear일 경우
	//	Gantry가 Loading위치보다 크거나 같을 경우
//	double dGantryLoad1Pos = GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_LOAD1_POS, 0);
	double dGantryLoadPos = GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_LOAD_POS);
	double dGantryCurPos = GetCurrentGantryPos();
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
//		if (dGantryCurPos < dGantryLoad1Pos + 1.0)
		if (dGantryCurPos < dGantryLoadPos + 1.0)
			return TRUE;
	}
	else
	{
//		if (dGantryCurPos > dGantryLoad1Pos - 1.0)
		if (dGantryCurPos > dGantryLoadPos - 1.0)
			return TRUE;
	}
//@#endif
	return TRUE;
}

const BOOL MTabMounter::IsIn_FrontTabLoadPos_TabMounter(int iScheduleNo, int nToolID)
{
	double dRef = m_plnkTabMounter[3]->GetTargetPos(iScheduleNo, DEF_TABMOUNTER_LOAD_POS) + DEF_TABMOUNTER_TOOL_SIZE*1.5;
	if (m_plnkTabMounter[nToolID]->GetCurrentPos() <= dRef)
		return TRUE;
	return FALSE;
}

const BOOL MTabMounter::IsIn_RearTabLoadPos_TabMounter(int iScheduleNo, int nToolID)
{
	double dRef = m_plnkTabMounter[4]->GetTargetPos(iScheduleNo, DEF_TABMOUNTER_LOAD_POS) - DEF_TABMOUNTER_TOOL_SIZE*1.5;
	if (m_plnkTabMounter[nToolID]->GetCurrentPos() >= dRef)
		return TRUE;
	return FALSE;
}

BOOL MTabMounter::IsInDangerPosWithTabCarrier()
{
	double dLoadPos = GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS);
	double dCurPos = GetCurrentPos();
//	double dGantryLoad4Pos = GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_LOAD1_POS, DEF_MAX_TABCARRIER-1);
	double dGantryDangerPos = GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_LOAD_POS);
	double dCurPosGantry = GetCurrentGantryPos();

	BOOL bToolDanger = FALSE;
	BOOL bGantryDanger = FALSE;
	if ((dCurPos > dLoadPos - DEF_TABMOUNTER_TOOL_SIZE*1.5) && (dCurPos < dLoadPos + DEF_TABMOUNTER_TOOL_SIZE*1.5) )
		bToolDanger = TRUE;
//@	if (m_iWorkInstance == DEF_FRONT_GROUP)
	if (m_iGroupNo == DEF_FRONT_GROUP)
	{
		dGantryDangerPos += ((DEF_TABCARRIER_STANDARD_GAP+2.0)*(DEF_MAX_TABCARRIER-1));
		if (dCurPosGantry <= dGantryDangerPos)
			bGantryDanger = TRUE;
	}
//@	else if (m_iWorkInstance == DEF_REAR_GROUP)
	else
	{
		dGantryDangerPos -= ((DEF_TABCARRIER_STANDARD_GAP+2.0)*(DEF_MAX_TABCARRIER-1));
		if (dCurPosGantry >= dGantryDangerPos)
			bGantryDanger = TRUE;
	}

	return (bToolDanger && bGantryDanger);
}

int MTabMounter::CheckGantryState(int iGantryPosID)
{
	if (NULL == m_plnkGantryAxis)
		return ERR_TABMOUNTER_SUCCESS;

//	ASSERT(iGantryPosID < DEF_TABMOUNTER_GANTRY_LOAD2_POS || iGantryPosID > DEF_TABMOUNTER_GANTRY_LOAD4_POS);
//	if (iGantryPosID == DEF_TABMOUNTER_GANTRY_LOAD1_POS)
//		iGantryPosID += m_iWorkInstance;

	CString strLogMsg;
	if (FALSE == m_plnkGantryAxis->IsAmpEnable())
		// 228035 = Gantry Axis 가 Amp Enable 상태 아님.
		return generateErrorCode(228035);

	if (TRUE == m_plnkGantryAxis->IsAmpFault())
		// 228036 = Gantry Axis 가 Amp Fault 상태임.
		return generateErrorCode(228036);
	
//	if (TRUE == m_plnkGantryAxis->IsAxisDone() && FALSE == IsInGantryPos(iGantryPosID, m_iGantryLoadCarrierID))
	if (TRUE == m_plnkGantryAxis->IsAxisDone() && FALSE == IsInGantryPos(iGantryPosID))
	{
		switch (iGantryPosID)
		{
//		case DEF_TABMOUNTER_GANTRY_LOAD1_POS:
//		case DEF_TABMOUNTER_GANTRY_LOAD2_POS:
//		case DEF_TABMOUNTER_GANTRY_LOAD3_POS:
//		case DEF_TABMOUNTER_GANTRY_LOAD4_POS:
		case DEF_TABMOUNTER_GANTRY_LOAD_POS:
			// 228037 = Gantry Axis 가 Load 위치에 멈추지 않았음.
			return generateErrorCode(228037);
			
//		case DEF_TABMOUNTER_GANTRY_PREALIGN_POS:
//			strLogMsg.Format(_T("TAB MOUNTER[%d] check Gantry State"), m_iInstanceNo + 1);
//			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);	
//			// 228038 = Gantry Axis 가 Prealign 위치에 멈추지 않았음.
//			return generateErrorCode(228038);
			
		case DEF_TABMOUNTER_GANTRY_INSPECTION_POS:
			// 228039 = Gantry Axis 가 Inspection 위치에 멈추지 않았음.
			return generateErrorCode(228039);
			
		case DEF_TABMOUNTER_GANTRY_MOUNT_POS:
			// 228040 = Gantry Axis 가 Mount 위치에 멈추지 않았음.
			return generateErrorCode(228040);
			
		default:
			// 228041 = 사용하지 않는 Gantry Pos ID 임.
			return generateErrorCode(228041);
		}
	}

	return ERR_TABMOUNTER_SUCCESS;
}

/**
 * 이동시 인근 Carrier와 충돌여부 확인하는 함수
 **/

void MTabMounter::SetMovingY(BOOL bMove, double posTarget)
{
	m_posTargetMove = posTarget;
	m_bMoving = bMove;
}

int	MTabMounter::CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber)
{
	double tgPos = GetTargetPos(iScheduleNo, iPosID, iTabNumber);
	return CheckCollisionOtherD(tgPos);
}

int MTabMounter::CheckCollisionOtherD(double posTarget)
{
	int		iResult			= ERR_TABMOUNTER_SUCCESS;
	double	dComparePos		= 0.0;
	IAxis*	pclsPreAxis		= NULL;

	
	double	dCurrentPos		= m_plnkAxis->GetCurrentPosition();
	BOOL	bDir			= (posTarget > dCurrentPos) ? TRUE /* (+) 방향 */ : FALSE /* (-) 방향 */;

//170826 JSh.s

	// Doolin kwangilshin 2017. 08. 28.
	//
	/*
	int iTempTabMounterNoInGroup = 0;

	if (bDir)
	{
		iTempTabMounterNoInGroup = m_iPreTabMounterNo;
	}
	else
	{
		iTempTabMounterNoInGroup = m_iNextTabMounterNo;
	}

	if (DEF_NONE_TABMOUNTER == iTempTabMounterNoInGroup)
	{
		return ERR_TABMOUNTER_SUCCESS;
	}

	iResult = m_plnkTabMounter[iTempTabMounterNoInGroup]->CheckOrigin();
	
	if (iResult)
	{
		SetErrorLevel(_T("TAB MOUNTER check Interlock"),2, __FILE__, __LINE__);
		return iResult;
	}
	
	dComparePos = m_plnkTabMounter[iTempTabMounterNoInGroup]->GetCurrentPos();
	pclsPreAxis	= MOlbSystemPre.GetTabMounterComponent(iTempTabMounterNoInGroup)->GetMotionComponent();
	if (fabs(dCurrentPos - dComparePos) >= DEF_TABMOUNTER_COLLISION_GAP)	
	{	
		if (pclsPreAxis->IsMotionDone())
		{
			if ((bDir && (posTarget + DEF_TABMOUNTER_COLLISION_GAP) >= dComparePos) ||
				(!bDir && (posTarget - DEF_TABMOUNTER_COLLISION_GAP) <= dComparePos))
			{
				return ERR_TABMOUNTER_SUCCESS;
			}
			else
			{
				return ERR_TABMOUNTER_WAIT;
			}
		}
	}
	else
	{
		return ERR_TABMOUNTER_WAIT;	
	}

	return ERR_TABMOUNTER_SUCCESS;
*/
	// End.

//170826 JSh.s

	// Previous TabMounter / Next TabMounter 간섭 확인.
	//	 - Process Layer 에서의 Previous/Next 와 개념이 같다.
	if (DEF_NONE_TABMOUNTER != m_iPreTabMounterNo)
	{
		if (TRUE == bDir)
		{
//@#ifdef DEF_SOURCE_SYSTEM
//@			int iTempTabMounterNoInGroup = m_iPreTabMounterNo - m_iGroupNo * DEF_MAX_WORKER_PER_GROUP;
//@#else
			int iTempTabMounterNoInGroup = m_iPreTabMounterNo;
//@#endif
			iResult = m_plnkTabMounter[iTempTabMounterNoInGroup]->CheckOrigin();
			if (iResult)
				return iResult;
			/*
			if (!bAuto)
			{
				if (dTarget > m_plnkTabMounter[iTempTabMounterNoInGroup]->GetCurrentPos() - DEF_TABMOUNTER_COLLISION_GAP)
				{
					SetErrorLevel(_T("TAB MOUNTER check Interlock"),2, __FILE__, __LINE__);
					// 228008 = 이전 TabMounter가 충돌 가능 위치에 있습니다.
					return generateErrorCode(228008);
				}
			}
			*/
			double dComparePos = 0.0;
			if (m_plnkTabMounter[iTempTabMounterNoInGroup]->IsMovingY())
				dComparePos = m_plnkTabMounter[iTempTabMounterNoInGroup]->GetMovingTgPos();
			else
				dComparePos = m_plnkTabMounter[iTempTabMounterNoInGroup]->GetCurrentPos();
//			if (posTarget > dComparePos - DEF_TABMOUNTER_COLLISION_GAP)
			if (posTarget > dComparePos - (DEF_TABMOUNTER_COLLISION_GAP - 2.0))
				return generateErrorCode(228008);
		}
	}
	
	if (DEF_NONE_TABMOUNTER != m_iNextTabMounterNo)
	{
		if (FALSE == bDir)
		{
//@#ifdef DEF_SOURCE_SYSTEM
//@			int iTempTabMounterNoInGroup = m_iNextTabMounterNo - m_iGroupNo * DEF_MAX_WORKER_PER_GROUP;
//@#else
			int iTempTabMounterNoInGroup = m_iNextTabMounterNo;
//@#endif
			iResult = m_plnkTabMounter[iTempTabMounterNoInGroup]->CheckOrigin();
			if (iResult)
				return iResult;
			/*
			if (!bAuto)
			{
				if (dTarget < m_plnkTabMounter[iTempTabMounterNoInGroup]->GetCurrentPos() + DEF_TABMOUNTER_COLLISION_GAP)
				{
					SetErrorLevel(_T("TAB MOUNTER check Interlock"),2, __FILE__, __LINE__);
					// 228009 = 이후 TabMounter가 충돌 가능 위치에 있습니다.
					return generateErrorCode(228009);
				}
			}
			*/
			double dComparePos = 0.0;
			if (m_plnkTabMounter[iTempTabMounterNoInGroup]->IsMovingY())
				dComparePos = m_plnkTabMounter[iTempTabMounterNoInGroup]->GetMovingTgPos();
			else
				dComparePos = m_plnkTabMounter[iTempTabMounterNoInGroup]->GetCurrentPos();
//			if (posTarget < dComparePos + DEF_TABMOUNTER_COLLISION_GAP)
			if (posTarget < dComparePos + (DEF_TABMOUNTER_COLLISION_GAP - 2.0))
				return generateErrorCode(228009);
		}
	}
	return  ERR_TABMOUNTER_SUCCESS;
}

BOOL MTabMounter::IsMovingY()
{
	m_csMoveLock.Lock();
	BOOL bMove = (m_bMoving || m_plnkAxis->IsInMotion());
	m_csMoveLock.Unlock();
	return bMove;
}

double MTabMounter::GetMovingTgPos()
{
	return m_posTargetMove;
}

void MTabMounter::SetTabInfo(MTabInfo tabinfo)
{
	m_TabInfo = tabinfo;
}

MTabInfo MTabMounter::GetTabInfo()
{
	return m_TabInfo;
}

//void MTabMounter::SetGantryMotionEnable(BOOL bState, int iLoadCarrierID)
void MTabMounter::SetGantryMotionEnable(BOOL bState)
{
//	ASSERT(iLoadCarrierID >= 0 && iLoadCarrierID < DEF_MAX_TABCARRIER);

	m_bGantryMotionEnable = bState;
//	if (bState)
//		m_iGantryLoadCarrierID = iLoadCarrierID;
//	else
//		m_iGantryLoadCarrierID = 0;
}

//Gate에서만 사용함...
double MTabMounter::GetLoadReadyPos()
{
	double dLoadReadyPos = GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS);
//@#ifdef DEF_GATE_SYSTEM
//@	dLoadReadyPos += (DEF_TABMOUNTER_STANDARD_GAP+0.5);	
	dLoadReadyPos += (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL+0.5);
//@#endif
	return dLoadReadyPos;
}

//20111205 SJ_HJG
double MTabMounter::GetReceiveAkdata(EWorkingSide eWorkSide, int iTabNo)
{
	//20111205 SJ_HJG
	double dTabOffsetAverage = 0.0;
	CString strAKReceivedata,strAutoAlignLog;
	double OldTabOffset = 0.0;
	double dAkReceiveData = 0.0;

	MGlassSummarys* pGs;
	pGs = new MGlassSummarys;

	MOlbSystemPre.GetMelsecComponent()->GetGlassSummaryWord(pGs,9);//Ak Local No. 9


	if (m_iWorkGroup == DEF_FRONT_GROUP)
		strAKReceivedata= pGs->m_strReceiveTabOffsetDataA[iTabNo]; //Front
	else
		strAKReceivedata= pGs->m_strReceiveTabOffsetDataB[iTabNo]; //Rear

	dAkReceiveData = (wcstod(strAKReceivedata, NULL) / 10); 

	//평균값
	dTabOffsetAverage = m_plnkSystemData->m_dTabOffsetAverage / m_plnkSystemData->m_dTabOffsetdata ;
	
	if(sm_rgdOldAkReceiveData[iTabNo] != dAkReceiveData)
	{
		//-2 ~ 2 사이의 값을 무시 한다.
		if(dAkReceiveData <= -2.0 || dAkReceiveData >= 2.0)
		{
			//20120126 수정.... OffsetData 바로 사용!!
			//현재 값 Old로 저장...
			//OldTabOffset = sm_rgdFinalTabOffset[iTabNo];
			if(m_iWorkGroup == DEF_FRONT_GROUP)
			{
				OldTabOffset = m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
				
#ifdef DEF_MIRROR_
				m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo] += ((dAkReceiveData / dTabOffsetAverage) / 1000);
#else
				m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo] -= ((dAkReceiveData / dTabOffsetAverage) / 1000);
#endif				
				
				strAutoAlignLog.Format(_T("Panel ID : %s ,Tab No:[%d], Receive Data:[%0.3f], 평균값:[%0.3f], 이전 Offset값:[%0.3f], 최종 Offset값:[%0.3f] "),MOlbSystemPre.GetTrsAutoManagerComponent()->GetGlassID(DEF_PANEL_ALIGNER_PANEL), iTabNo + 1, dAkReceiveData, dTabOffsetAverage, OldTabOffset,m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo]);
			}
			else
			{
				OldTabOffset = m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
				
#ifdef DEF_MIRROR_
				m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo] += ((dAkReceiveData / dTabOffsetAverage) / 1000);
#else
				m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo] -= ((dAkReceiveData / dTabOffsetAverage) / 1000);
#endif
				
				sm_rgdOldAkReceiveData[iTabNo] = dAkReceiveData;

				strAutoAlignLog.Format(_T("Panel ID : %s ,Tab No:[%d], Receive Data:[%0.3f], 평균값:[%0.3f], 이전 Offset값:[%0.3f], 최종 Offset값:[%0.3f] "),MOlbSystemPre.GetTrsAutoManagerComponent()->GetGlassID(DEF_PANEL_ALIGNER_PANEL), iTabNo + 1, dAkReceiveData, dTabOffsetAverage, OldTabOffset,m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo]);
			}
			
			MOlbSystemPre.GetTrsAutoManagerComponent()->WriteAutoAlignLog(strAutoAlignLog);
			sm_rgdOldAkReceiveData[iTabNo] = dAkReceiveData;
		}
	}
	
	//20120126 수정.... OffsetData 바로 사용!!
	//return sm_rgdFinalTabOffset[iTabNo];
	if(m_iWorkGroup == DEF_FRONT_GROUP)
		return m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
	else
		return m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
	
}
