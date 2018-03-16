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
/*        Component ���� ���� Interface                                 */
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
/*        Teaching ���� Interface                                       */
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
			// 228000 = Offset ��ǥ ���� �б⸦ ���� �߽��ϴ�.
			return generateErrorCode(228000);
		else if (iAxisType == DEF_MOUNT_Z)
			// 228000 = Offset ��ǥ ���� �б⸦ ���� �߽��ϴ�.
			return generateErrorCode(228105);
		else if (iAxisType == DEF_GANTRY)
			// 228012 = Gantry Offset ��ǥ ���� �б⸦ ���� �߽��ϴ�.
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
			// 228001 = ������ǥ�� Load �ϴµ� ���� �߽��ϴ�. 
			return generateErrorCode(228001);
		else if (DEF_MOUNT_Z == iAxisType)
			// 228001 = ������ǥ�� Load �ϴµ� ���� �߽��ϴ�. 
			return generateErrorCode(228106);
		else if (DEF_GANTRY == iAxisType)
			// 228013 = Gantry ������ǥ�� Load �ϴµ� ���� �߽��ϴ�. 
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
			// 228002 = Offset ��ǥ�� �����ϴµ� ���� �߽��ϴ�.
			return generateErrorCode(228002);
		else if (DEF_MOUNT_Z == iAxisType)
			// 228002 = Offset ��ǥ�� �����ϴµ� ���� �߽��ϴ�.
			return generateErrorCode(228107);
		else if (DEF_GANTRY == iAxisType)
			// 228014 = Gantry Offset ��ǥ�� �����ϴµ� ���� �߽��ϴ�.
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

			// ������ǥ Teaching �Ұ� ��ġ�� ���
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
/// �ӽ�				m_rgdFixedPos[DEF_MOUNT_Y][i] = 0.0;
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
			// 228003 = ���� ��ǥ�� �����ϴµ� ���� �߽��ϴ�. 
			return generateErrorCode(228003);
		else if (DEF_MOUNT_Z == iAxisType)
			// 228003 = ���� ��ǥ�� �����ϴµ� ���� �߽��ϴ�. 
			return generateErrorCode(228108);
		else if (DEF_GANTRY == iAxisType)
			// 228015 = Gantry ���� ��ǥ�� �����ϴµ� ���� �߽��ϴ�. 
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

	// Gantry �� �����ϰ�, Motion Ȱ��ȭ �Ǿ� ������ Gantry �� Pos ID �޾Ƽ� Gantry �൵ �̵� ����
	// �� �� Ȱ��ȭ �÷��� Ȯ�� ����. �Ʒ� TabMounter ���۰� ���õ��� ó�� �� �� �ֵ��� �� ��.
	if (NULL != m_plnkGantryAxis && TRUE == m_bGantryMotionEnable)
	{
		int iGantryPosID = getGantryPosID(iPosID);
		ASSERT(iGantryPosID >= DEF_TABMOUNTER_NONE_POS && iGantryPosID < DEF_TABMOUNTER_GANTRY_MAX_POS);

		if (DEF_TABMOUNTER_NONE_POS != iGantryPosID)
		{
			// 20081007 jdy ���κ��� ��ġ �̵���. : TabMounter Ready ��ġ�� �̵� �ÿ��� Gantry �̵� ���� ������ ��.
			// Gantry �̵� ���� ��, Ȱ��ȭ Flag Clear!
			// Gantry ������ ���ؼ��� �׻� Move �Լ� ȣ�� ���� Gantry �� Ȱ��ȭ ���� �ʿ�
			// : Teaching Dlg ������ Gantry Ȱ��ȭ ���߸��� �ʵ��� ������ ��!
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
	//170327_KDH Test.. ���� 10
	Sleep(30);	// WaitOption == FALSE �� ��츦 ����� Motion ���� ���� �� ����ȭ Delay. 	

	if (FALSE == bWaitOption)
	{
#ifndef SIMULATION
		SetMovingY(FALSE, dTarget);
		m_csMoveLock.Unlock();
		if (TRUE == m_plnkAxis->IsAmpFault() || FALSE == m_plnkAxis->IsAmpEnable())
		{
			strLogMsg.Format(_T("TAB MOUNTER[%d] move Position(Move Pos=%d)"),m_iInstanceNo + 1, iPosID);
			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);
			// 228006 = �̵� ��, Amp Fault Ȥ�� Amp Disable ������.
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

	Sleep(50);	// WaitOption == FALSE �� ��츦 ����� Motion ���� ���� �� ����ȭ Delay. 

	if (FALSE == bWaitOption)
	{
#ifndef SIMULATION
		if (TRUE == m_plnkAxisZ->IsAmpFault() || FALSE == m_plnkAxisZ->IsAmpEnable())
		{
			strLogMsg.Format(_T("TAB MOUNTER[%d] move Z Position(Move Pos=%d)"),m_iInstanceNo + 1, iPosID);
			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);
			// 228006 = �̵� ��, Amp Fault Ȥ�� Amp Disable ������.
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

	// �� �Լ��� �̿��ϸ� Gantry Enable Flag ���¿� ������� Gantry �� ������ �� �ִ�.

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
		// 228005 = TabMounter �� �������Ͱ� �Ǿ����� �ʽ��ϴ�.
		return generateErrorCode(228005);

	if (m_plnkAxisZ->IsOriginReturn() == FALSE)
		// 228005 = TabMounter �� �������Ͱ� �Ǿ����� �ʽ��ϴ�.
		return generateErrorCode(228110);

//@#ifdef DEF_GATE_SYSTEM
	if (m_plnkGantryAxis->IsOriginReturn() == FALSE)
		// 228016 = Gantry �� �������Ͱ� �Ǿ����� �ʽ��ϴ�.
		return generateErrorCode(228016);
//@#endif

	return ERR_TABMOUNTER_SUCCESS;
}

int MTabMounter::CheckOriginZ()
{
	if (m_plnkAxisZ->IsOriginReturn() == FALSE)
		// 228005 = TabMounter �� �������Ͱ� �Ǿ����� �ʽ��ϴ�.
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

	// m_rgdModelPos �� �� �̻� �ǹ� ����. ProcessData ���� �� ������.

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
//@			dInspectionOffset = m_plnkWorkSchedule[iScheduleNo]->GetInspectionOffset(iTabNumber).dY;//Ȯ�ο�
			dInspectionOffset = m_plnkWorkSchedule[iScheduleNo]->GetInspectionOffset(iTabNumber).dX;//Ȯ�ο�
			//@_________________

			//120208.kms_________
//			dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			if(m_iWorkGroup == DEF_FRONT_GROUP)
				dTabOffset = m_plnkTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			else
				dTabOffset = m_plnkRearTabOffsetData->GetTabOffsetMounter(m_eWorkSide, iTabNumber);
			//___________________

			dToolOffset = m_plnkSystemData->m_rgdToolOffsetMounter[m_iInstanceNo];
			
			//20111205 SJ_HJG Auto Align System ����
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
		/*Ȯ�ο�_____
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

//Ȯ�ο�_____
double MTabMounter::CalculateMountPos(double dModelPos)
{
	double dTarget = m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_INSPECTION_POS] + m_rgdOffsetPos[DEF_MOUNT_Y][DEF_TABMOUNTER_INSPECTION_POS] + dModelPos;
	return dTarget;
}
//___________

double MTabMounter::GetCurrentGantryPos(BOOL bType)
{
	// Ȯ�� �ʿ� : DEF_TABMOUNTER_GANTRY_MOUNT_POS �� ��� inspection ����� �ݿ��ؼ� Gantry �� �������� �Ѵ�. - �ϴ� ����

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

// ����! : m_bGantryMotionEnable ���� ���¿� ����, TabMounter Pos ID �� �̿��ؼ� Gantry ��ġ������ Ȯ�� ����
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

// ����! : TabMounter ��ġ�� Ȯ�� ����.
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

// ����! : Gantry ��ġ�� Ȯ�� ����.
//BOOL MTabMounter::IsInGantryPos(int iGantryPosID, int iLoadCarrierID, double* pdTolerance)
BOOL MTabMounter::IsInGantryPos(int iGantryPosID, double* pdTolerance)
{
	if (NULL == m_plnkGantryAxis)
		return FALSE;

//	double dCompare = GetTargetGantryPos(iGantryPosID, iLoadCarrierID);
	double dCompare = GetTargetGantryPos(iGantryPosID);
	return IsInGantryPos(dCompare, pdTolerance);
}

// ����! : Gantry ��ġ�� Ȯ�� ����.
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
//	BOOL bDir = (dTarget > dCurrentPos) ? TRUE /* (+) ���� */ : FALSE /* (-) ���� */;
	
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
			//170327_KDH Test ��.. ���� 10
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
			if (iPosID != -1)	//110123.�ӽ�
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
				//170715_KDH �ӽû���
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

	// Cylinder �׻� �ϰ��Ǿ� �־�� ��.
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
	//Inspection��ġ���� Mount��ġ�� �̵��� 
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

				//170715_KDH �ӽû���
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
				//170715_KDH �ӽû���
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
		if (iPosID != -1)	//110123.�ӽ�
		{
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				if (FALSE == m_plnkTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
				{
					iResult = m_plnkTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
					if (iResult)
						return iResult;
				}
				//170715_KDH �ӽû���
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
	// Gantry �� �浹 ���� : ��� Stopper ����.

	return ERR_TABMOUNTER_SUCCESS;
}

void MTabMounter::calculateFixedPosParameter(int iAxisType)
{
	if (iAxisType == DEF_MOUNT_Z)
		return;
	
	/*/
#ifdef DEF_SOURCE_SYSTEM
	//Tool�߽��� PanelAligner �߽����� �����
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
		//Tool �߽��� Panel Xu���ܿ� �����(���ⷮ ����) -> MarkPos(ex.1.5)�� Model��ǥ�� ����
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
	//170715_KDH �ӽû���
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
	//170715_KDH �ӽû���
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
		// Load ��ġ���� 2�� �ϰ� �Ұ�.
		if (DEF_TABMOUNTER_DOWN_TYPE_DOWN == iDownType)
			// 228028 = 2�� �ϰ� �Ұ�. [Load ��ġ��.]
			return generateErrorCode(228028);
		
		if (TRUE == IsAbsorbTabIC())
			// 228029 = 1�� �ϰ� �Ұ�. [Load ��ġ���� TabIC ���� ������.]
			return generateErrorCode(228029);
	}
	//else if (TRUE == IsInPos(DEF_TABMOUNTER_REMOVE1_POS, &dTolerance))
	else if (TRUE == IsInPos(0, DEF_TABMOUNTER_REMOVE_POS, &dTolerance))
	{
		if (DEF_TABMOUNTER_DOWN_TYPE_DOWN == iDownType)
			// 228030 = 2�� �ϰ� �Ұ�. [Source ���� : Remove ��ġ��.]
			return generateErrorCode(228030);
	}
	else
	{
/*		double dCurrentPos = GetCurrentPos();
		double dRefPos = GetTargetPos(DEF_TABMOUNTER_INSPECTION_POS);
		if (dCurrentPos > dRefPos - 1.0)
		{
			/*KKY.Del
			// Inspection, Mount ��ġ���� Backup up,forward �ƴϸ� 2�� �ϰ� �Ұ�.
			if (DEF_TABMOUNTER_DOWN_TYPE_DOWN == iDownType && (FALSE == IsUpBackupUDCyl() || FALSE == IsForwardBackupFBCyl()))
			{
			SetErrorLevel(_T("TAB MOUNTER check Interlock for Down"),2, __FILE__, __LINE__);
			// 228027 = 2�� �ϰ� �Ұ�. [Backup Up �ƴ�.]
			return generateErrorCode(228027);
			}
			*/
/*			;
		}
		else
		{
			SetErrorLevel(_T("TAB MOUNTER check Interlock for Down"),2, __FILE__, __LINE__);
			// 228034 = �ϰ� �Ұ�. [������ Mounter ��ġ �ƴ�.]
			return generateErrorCode(228034);
		}*/
	}

	// TabMounter �ϰ� ��, �Ʒ��� panel �� �ִ��� Ȯ�� �ʿ��ϴ�.
	// => Ȯ���� ������ ����ġ �����Ƿ� Backup Up ���·� �Ǵ��ϰ� ������ Interlock �� �߰����� �ʴ´�.

	return ERR_TABMOUNTER_SUCCESS;
}

#if FALSE	//==============================================

int MTabMounter::checkInterlockForBackupUp()
{
	if (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed()
		&& FALSE == m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
	{
		SetErrorLevel(_T("TAB MOUNTER check Interlock for Backup Up"),2, __FILE__, __LINE__);
		// 228023 = Backup Up �Ұ�. [Unload ��ġ�� ���� ���� Panel Transfer �� Panel �����ϰ� ����.]
		return generateErrorCode(228023);
	}

	if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed()
//		&& (FALSE == m_plnkPanelAligner->IsInXPos(DEF_PANEL_ALIGNER_PRESS_XU_POS)
//			|| FALSE == m_plnkPanelAligner->IsInYPos(DEF_PANEL_ALIGNER_PRESS_XU_POS)))
		&& FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS))
	{
		SetErrorLevel(_T("TAB MOUNTER check Interlock for Backup Up"),2, __FILE__, __LINE__);
		// 228033 = Backup Up �Ұ�. [PanelAligner �� Press Xu ��ġ �ƴ�.]
		return generateErrorCode(228033);
	}

	// Group ���� ��� TabMounter UD Cylinder Up ���� Ȯ��.
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		if (FALSE == m_plnkTabMounter[i]->IsUpUDCyl()
			&& FALSE == m_plnkTabMounter[i]->IsDownReady())
		{
			SetErrorLevel(_T("TAB MOUNTER check Interlock for Backup Up"),2, __FILE__, __LINE__);
			// 228020 = Backup Up �Ұ�. [Group �� TabMounter �� 2�� �ϰ� ������ TabMounter ����.]
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
	//Front�� ���
	//	Gantry�� Loading��ġ���� �۰ų� ���� ���
	//Rear�� ���
	//	Gantry�� Loading��ġ���� ũ�ų� ���� ���
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
		// 228035 = Gantry Axis �� Amp Enable ���� �ƴ�.
		return generateErrorCode(228035);

	if (TRUE == m_plnkGantryAxis->IsAmpFault())
		// 228036 = Gantry Axis �� Amp Fault ������.
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
			// 228037 = Gantry Axis �� Load ��ġ�� ������ �ʾ���.
			return generateErrorCode(228037);
			
//		case DEF_TABMOUNTER_GANTRY_PREALIGN_POS:
//			strLogMsg.Format(_T("TAB MOUNTER[%d] check Gantry State"), m_iInstanceNo + 1);
//			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);	
//			// 228038 = Gantry Axis �� Prealign ��ġ�� ������ �ʾ���.
//			return generateErrorCode(228038);
			
		case DEF_TABMOUNTER_GANTRY_INSPECTION_POS:
			// 228039 = Gantry Axis �� Inspection ��ġ�� ������ �ʾ���.
			return generateErrorCode(228039);
			
		case DEF_TABMOUNTER_GANTRY_MOUNT_POS:
			// 228040 = Gantry Axis �� Mount ��ġ�� ������ �ʾ���.
			return generateErrorCode(228040);
			
		default:
			// 228041 = ������� �ʴ� Gantry Pos ID ��.
			return generateErrorCode(228041);
		}
	}

	return ERR_TABMOUNTER_SUCCESS;
}

/**
 * �̵��� �α� Carrier�� �浹���� Ȯ���ϴ� �Լ�
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
	BOOL	bDir			= (posTarget > dCurrentPos) ? TRUE /* (+) ���� */ : FALSE /* (-) ���� */;

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

	// Previous TabMounter / Next TabMounter ���� Ȯ��.
	//	 - Process Layer ������ Previous/Next �� ������ ����.
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
					// 228008 = ���� TabMounter�� �浹 ���� ��ġ�� �ֽ��ϴ�.
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
					// 228009 = ���� TabMounter�� �浹 ���� ��ġ�� �ֽ��ϴ�.
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

//Gate������ �����...
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

	//��հ�
	dTabOffsetAverage = m_plnkSystemData->m_dTabOffsetAverage / m_plnkSystemData->m_dTabOffsetdata ;
	
	if(sm_rgdOldAkReceiveData[iTabNo] != dAkReceiveData)
	{
		//-2 ~ 2 ������ ���� ���� �Ѵ�.
		if(dAkReceiveData <= -2.0 || dAkReceiveData >= 2.0)
		{
			//20120126 ����.... OffsetData �ٷ� ���!!
			//���� �� Old�� ����...
			//OldTabOffset = sm_rgdFinalTabOffset[iTabNo];
			if(m_iWorkGroup == DEF_FRONT_GROUP)
			{
				OldTabOffset = m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
				
#ifdef DEF_MIRROR_
				m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo] += ((dAkReceiveData / dTabOffsetAverage) / 1000);
#else
				m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo] -= ((dAkReceiveData / dTabOffsetAverage) / 1000);
#endif				
				
				strAutoAlignLog.Format(_T("Panel ID : %s ,Tab No:[%d], Receive Data:[%0.3f], ��հ�:[%0.3f], ���� Offset��:[%0.3f], ���� Offset��:[%0.3f] "),MOlbSystemPre.GetTrsAutoManagerComponent()->GetGlassID(DEF_PANEL_ALIGNER_PANEL), iTabNo + 1, dAkReceiveData, dTabOffsetAverage, OldTabOffset,m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo]);
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

				strAutoAlignLog.Format(_T("Panel ID : %s ,Tab No:[%d], Receive Data:[%0.3f], ��հ�:[%0.3f], ���� Offset��:[%0.3f], ���� Offset��:[%0.3f] "),MOlbSystemPre.GetTrsAutoManagerComponent()->GetGlassID(DEF_PANEL_ALIGNER_PANEL), iTabNo + 1, dAkReceiveData, dTabOffsetAverage, OldTabOffset,m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo]);
			}
			
			MOlbSystemPre.GetTrsAutoManagerComponent()->WriteAutoAlignLog(strAutoAlignLog);
			sm_rgdOldAkReceiveData[iTabNo] = dAkReceiveData;
		}
	}
	
	//20120126 ����.... OffsetData �ٷ� ���!!
	//return sm_rgdFinalTabOffset[iTabNo];
	if(m_iWorkGroup == DEF_FRONT_GROUP)
		return m_plnkTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
	else
		return m_plnkRearTabOffsetData->m_rgdXuTabOffsetTabMounter[iTabNo];
	
}
