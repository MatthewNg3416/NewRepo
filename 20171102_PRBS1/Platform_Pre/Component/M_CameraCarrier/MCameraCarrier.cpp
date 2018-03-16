/* 
 * CameraCarrier Component
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
#include "MPlatformOlbSystem.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MProcessData.h"
#include "MTabData.h"
#include "IAxis.h"
#include "ICylinder.h"
#include "IStateTabMounter.h"
#include "MCameraCarrier.h"
#include "MTeachingPosData.h"
#include "DefPanelAligner.h"
#include "IStatePanelAligner.h"
#include "DefTabMounter.h"
#include "common.h"

extern MPlatformOlbSystem	MOlbSystemPre;

CCriticalSection MCameraCarrier::m_csMoveLock;

MCameraCarrier::MCameraCarrier(SCommonAttribute commonData, SCameraCarrierRefCompList listRefComponents, SCameraCarrierData datComponent)
: ICommon(commonData)
{
	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		m_rgdFixedPos[i] = 0.0;
		m_rgdOffsetPos[i] = 0.0;
		for (int j=0; j<DEF_MAX_WORK_SCHEDULE; j++)
			m_rgdModelPos[j][i] = 0.0;
	}

	for (int i = 0; i<DEF_CAMERACARRIER_BACKUP_MAX_POS; i++)
	{
		m_rgdFixedPosBackupZ[i] = 0;
		m_rgdModelPosBackupZ[i] = 0;
		m_rgdOffsetPosBackupZ[i] = 0;
	}
	
	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgdFixedPosExpand[i] = 0.0;
		m_rgdModelPosExpand[i] = 0.0;
		m_rgdOffsetPosExpand[i] = 0.0;
	}

	m_plnkPanelAligner = NULL;
	m_bMoving = FALSE;//KKY

	AssignComponents(listRefComponents);
	SetData(datComponent);

	if (m_iInstanceNo < DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkInstance = m_iInstanceNo - DEF_MAX_INSPECTION_CAM_CARRIER;	//@DEF_MAX_TABMOUNTER;
	}
	if (m_iWorkInstance == DEF_MAX_INSPECTION_CAM_CARRIER-1)
		m_isPanelAlignUnit = TRUE;
	else
		m_isPanelAlignUnit = FALSE;
	//@_____________________


	SetFixedOffsetBySystemData();
	///LoadFixedPosParameter();
	LoadOffsetPosParameter();

	calculateModelPosParameter();

	LoadFixedPosParameterBackup();
	LoadOffsetPosParameterBackup();

	LoadFixedPosParameterExpand();
	LoadOffsetPosParameterExpand();
	calculateExpandModelPosParameter();
}

MCameraCarrier::~MCameraCarrier()
{
}

/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

void MCameraCarrier::SetData(SCameraCarrierData datComponent)
{
	m_plnkSystemData		= datComponent.m_plnkSystemData;
	m_plnkPanelData			= datComponent.m_plnkPanelData;
	m_plnkPreBonderData		= datComponent.m_plnkPreBonderData;
	m_plnkProcessData		= datComponent.m_plnkProcessData;
	m_plnkTabData			= datComponent.m_plnkTabData; //SJ_YYK 150109 Add..
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]		= datComponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND]	= datComponent.m_plnkWorkScheduleSecond;
#endif

	m_sTeachingInfo = datComponent.m_sTeachingInfo;
	m_sTeachingInfoBackupZ = datComponent.m_sTeachingInfoBackupZ;
	m_sTeachingInfoExpand = datComponent.m_sTeachingInfoExpand;

	m_strPosFileName = datComponent.m_strPosFileName;
	m_strOffsetPosFileName = datComponent.m_strOffsetPosFileName;
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);

	m_iGroupNo = datComponent.m_iGroupNo;
	m_eWorkSide = datComponent.m_eWorkSide;
//	m_iCamNo = datComponent.m_iCamNo;
	m_iCamNo1 = datComponent.m_iCamNo1;
	m_iCamNo2 = datComponent.m_iCamNo2;
}

void MCameraCarrier::AssignComponents(SCameraCarrierRefCompList CameraCarrierCompList)
{
	m_plnkAxis = CameraCarrierCompList.m_plnkAxis;
	m_plnkAxisBackupZ = CameraCarrierCompList.m_plnkAxisBackupZ;
	m_plnkAxisExpand = CameraCarrierCompList.m_plnkAxisExpand;
	m_plnkESCyl = CameraCarrierCompList.m_plnkESCyl;
}

void MCameraCarrier::AssignStateCheckComponents(SStateCheckCompListForCameraCarrier sCompList)
{
	m_plnkPreCamCarrier = sCompList.m_plnkPreCamCarrier;
	m_plnkNextCamCarrier = sCompList.m_plnkNextCamCarrier;
//	m_plnkTabMounter = sCompList.m_plnkTabMounter;
	for (int i = 0; i<DEF_MAX_TABMOUNTER; i++)
		m_plnkTabMounter[i] = sCompList.m_plnkTabMounter[i];
	m_plnkPanelAligner = sCompList.m_plnkPanelAligner;

	SetFixedOffsetBySystemData();
}

IAxis* MCameraCarrier::GetMotionComponent()
{
	return m_plnkAxis;
}

IAxis* MCameraCarrier::GetMotionComponentBackup()
{
	return m_plnkAxisBackupZ;
}

IAxis* MCameraCarrier::GetMotionComponentExpand()
{
	return m_plnkAxisExpand;
}

int MCameraCarrier::ChangeModel(void)
{
	// Model Path Change
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);

	// Offset 좌표 Load
	int iResult = LoadOffsetPosParameter();
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;

	iResult = LoadOffsetPosParameterBackup();
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;

	SetFixedOffsetBySystemData();
	calculateModelPosParameter();

	iResult = LoadOffsetPosParameterExpand();
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;
	calculateExpandModelPosParameter();

	return ERR_CAMERACARRIER_SUCCESS;
}

/************************************************************************/
/*        Teaching 관련 Interface                                       */
/************************************************************************/
int MCameraCarrier::LoadFixedPosParameter()
{
	MTeachingPosData FixedPosDat(m_sTeachingInfo, m_strPosFileName);

	BOOL bResult = FixedPosDat.ReadData();	
	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		m_rgdFixedPos[i] = FixedPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 214001 = Offset 좌표 파일 읽기 실패
		return generateErrorCode(214012);
	return ERR_CAMERACARRIER_SUCCESS;
}


int MCameraCarrier::LoadFixedPosParameterBackup()
{
	MTeachingPosData BackupFixedPosDat(m_sTeachingInfoBackupZ, m_strPosFileName);

	BOOL bResult = BackupFixedPosDat.ReadData();	
	for (int i = 0; i < DEF_CAMERACARRIER_BACKUP_MAX_POS; i++)
	{
		m_rgdFixedPosBackupZ[i] = BackupFixedPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 214001 = Offset 좌표 파일 읽기 실패
		return generateErrorCode(214012);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::LoadFixedPosParameterExpand()
{
	MTeachingPosData ExpandFixedPosDat(m_sTeachingInfoExpand, m_strPosFileName);

	BOOL bResult = TRUE;

//@	BOOL bResult = ExpandFixedPosDat.ReadData();	
	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgdFixedPosExpand[i] = m_plnkSystemData->m_dData_CameraExpand[m_iGroupNo][m_iWorkInstance];	//@ExpandFixedPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 214001 = Offset 좌표 파일 읽기 실패
		return generateErrorCode(214012);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::LoadOffsetPosParameter(void)
{
	MTeachingPosData CameraCarrierOffsetPosDat(m_sTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	BOOL bResult = CameraCarrierOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		m_rgdOffsetPos[i] = CameraCarrierOffsetPosDat.m_TeachingPos[i][DEF_CAMERACARRIER_AXIS];
	}

	if (!bResult)
		// 214001 = Offset 좌표 파일 읽기 실패
		return generateErrorCode(214001);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::LoadOffsetPosParameterBackup()
{
	MTeachingPosData BackupOffsetPosDat(m_sTeachingInfoBackupZ, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	BOOL bResult = BackupOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_CAMERACARRIER_BACKUP_MAX_POS; i++)
	{
		m_rgdOffsetPosBackupZ[i] = BackupOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 214001 = Offset 좌표 파일 읽기 실패
		return generateErrorCode(214013);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::LoadOffsetPosParameterExpand(void)
{
	MTeachingPosData ExpandOffsetPosDat(m_sTeachingInfoExpand, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	BOOL bResult = ExpandOffsetPosDat.ReadData();
	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgdOffsetPosExpand[i] = ExpandOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		// 214001 = Offset 좌표 파일 읽기 실패
		return generateErrorCode(214001);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::SaveFixPosParameter(double* pdFixPos)
{
	ASSERT(pdFixPos != NULL);

	MTeachingPosData CameraCarrierFixPosDat(m_sTeachingInfo, m_strPosFileName);

	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		m_rgdFixedPos[i] = pdFixPos[i];

		CameraCarrierFixPosDat.m_TeachingPos[i][DEF_CAMERACARRIER_AXIS] = m_rgdFixedPos[i];
	}

	BOOL bResult = CameraCarrierFixPosDat.WriteData();
	if (!bResult)
		// 214002 = Offset 좌표 파일 저장 실패
		return generateErrorCode(214002);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::SaveFixedPosParameterBackup(double* dFixedPos)
{
	ASSERT(dFixedPos != NULL);

	MTeachingPosData BackupFixedPosDat(m_sTeachingInfoBackupZ, m_strPosFileName);

	for (int i = 0; i < DEF_CAMERACARRIER_BACKUP_MAX_POS; i++)
	{
		m_rgdFixedPosBackupZ[i] = dFixedPos[i];

		BackupFixedPosDat.m_TeachingPos[i][0] = m_rgdFixedPosBackupZ[i];
	}

	BOOL bResult = BackupFixedPosDat.WriteData();
	if (!bResult)
		// 214002 = Offset 좌표 파일 저장 실패
		return generateErrorCode(214014);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::SaveFixedPosParameterExpand(double* pdFixPos)
{
	ASSERT(pdFixPos != NULL);

	MTeachingPosData ExpandFixPosDat(m_sTeachingInfoExpand, m_strPosFileName);

	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgdFixedPosExpand[i] = pdFixPos[i];

		ExpandFixPosDat.m_TeachingPos[i][0] = m_rgdFixedPosExpand[i];
	}

	BOOL bResult = ExpandFixPosDat.WriteData();
	if (!bResult)
		// 214002 = Offset 좌표 파일 저장 실패
		return generateErrorCode(214002);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::SaveOffsetPosParameter(double* pdOffsetPos)
{
	ASSERT(pdOffsetPos != NULL);

	MTeachingPosData CameraCarrierOffsetPosDat(m_sTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		m_rgdOffsetPos[i] = pdOffsetPos[i];

		CameraCarrierOffsetPosDat.m_TeachingPos[i][DEF_CAMERACARRIER_AXIS] = m_rgdOffsetPos[i];
	}

	BOOL bResult = CameraCarrierOffsetPosDat.WriteData();
	if (!bResult)
		// 214002 = Offset 좌표 파일 저장 실패
		return generateErrorCode(214002);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::SaveOffsetPosParameterBackup(double* pdOffsetPos)
{
	ASSERT(pdOffsetPos != NULL);

	MTeachingPosData BackupOffsetPosDat(m_sTeachingInfoBackupZ, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	for (int i = 0; i < DEF_CAMERACARRIER_BACKUP_MAX_POS; i++)
	{
		m_rgdOffsetPosBackupZ[i] = pdOffsetPos[i];

		BackupOffsetPosDat.m_TeachingPos[i][0] = m_rgdOffsetPosBackupZ[i];
	}

	BOOL bResult = BackupOffsetPosDat.WriteData();
	if (!bResult)
		// 214002 = Offset 좌표 파일 저장 실패
		return generateErrorCode(214015);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::SaveOffsetPosParameterExpand(double* pdOffsetPos)
{
	ASSERT(pdOffsetPos != NULL);

	MTeachingPosData ExpandOffsetPosDat(m_sTeachingInfoExpand, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgdOffsetPosExpand[i] = pdOffsetPos[i];

		ExpandOffsetPosDat.m_TeachingPos[i][0] = m_rgdOffsetPosExpand[i];
	}

//	return ERR_CAMERACARRIER_SUCCESS;

	BOOL bResult = ExpandOffsetPosDat.WriteData();
	if (!bResult)
		// 214002 = Offset 좌표 파일 저장 실패
		return generateErrorCode(214002);
	return ERR_CAMERACARRIER_SUCCESS;
}


int MCameraCarrier::GetPosParameter(int iScheduleNo, double* pdFixedPos, double* pdOffsetPos, double* pdModelPos)
{
	ASSERT(iScheduleNo >= 0 && iScheduleNo < DEF_MAX_WORK_SCHEDULE);
	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		if (pdFixedPos != NULL)
			pdFixedPos[i] = m_rgdFixedPos[i];

		if (pdModelPos != NULL)
//@			pdModelPos[i] = m_rgdModelPos[i];
			pdModelPos[i] = m_rgdModelPos[iScheduleNo][i];

		if (pdOffsetPos != NULL)
			pdOffsetPos[i] = m_rgdOffsetPos[i];
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::GetPosParameterBackup(double *pdFixedPos, double *pdOffsetPos, double *pdModelPos)
{
	for (int i = 0; i < DEF_CAMERACARRIER_BACKUP_MAX_POS; i++)
	{
		if (pdFixedPos != NULL)
			pdFixedPos[i] = m_rgdFixedPosBackupZ[i];

		if (pdModelPos != NULL)
			pdModelPos[i] = m_rgdModelPosBackupZ[i];

		if (pdOffsetPos != NULL)
			pdOffsetPos[i] = m_rgdOffsetPosBackupZ[i];
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::GetPosParameterExpand(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos)
{
	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		if (pdFixedPos != NULL)
			pdFixedPos[i] = m_rgdFixedPosExpand[i];

		if (pdModelPos != NULL)
			pdModelPos[i] = m_rgdModelPosExpand[i];

		if (pdOffsetPos != NULL)
			pdOffsetPos[i] = m_rgdOffsetPosExpand[i];
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

STeachingInfo MCameraCarrier::GetTeachingInfo()
{
	return m_sTeachingInfo;
}

STeachingInfo MCameraCarrier::GetTeachingInfoBackup()
{
	return m_sTeachingInfoBackupZ;
}

STeachingInfo MCameraCarrier::GetTeachingInfoExpand()
{
	return m_sTeachingInfoExpand;
}

void MCameraCarrier::SetFixedOffsetBySystemData()
{
	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		//카메라간 중심을 Fiducial Mark중심에 맞춘다
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
			double dVarPos = 0.0;
			m_rgdFixedPos[i] = m_plnkSystemData->m_dCamCarrier1_FixedPosX;
			dVarPos = m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0;
			m_rgdFixedPos[i] += dVarPos;
//170826 JSh.s
			if(i == DEF_CAMERACARRIER_TAB_MOUNT_POS)
				m_rgdFixedPos[i] += DEF_INSPECTIOM_BONDING_CENTER_TO_ALIGN_CENTER;
//170826 JSh.e
		}
		else
		{
			double dVarPos = 0.0;
			m_rgdFixedPos[i] = m_plnkSystemData->m_dCamCarrier2_FixedPosX;
			//dVarPos =  -m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0; //IC 길이 변화시 확인..
			dVarPos =  m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0; //IC 길이 변화시 확인.. //170916 JSH
			m_rgdFixedPos[i] += dVarPos;
//170826 JSh.s
			if(i == DEF_CAMERACARRIER_TAB_MOUNT_POS)
				m_rgdFixedPos[i] += DEF_INSPECTIOM_BONDING_CENTER_TO_ALIGN_CENTER;
//170826 JSh.e
		}
	}
}

int MCameraCarrier::SafeMovePos(int iScheduleNo, int iPosID, BOOL bWaitOption, int iTabNumber, BOOL bIgnoreInterlock)
{
	double dTarget = GetTargetPos(iScheduleNo, iPosID, iTabNumber);
	return MovePos(dTarget, iPosID, bWaitOption, bIgnoreInterlock);
}

int MCameraCarrier::MovePos(double dTarget, int iPosID, BOOL bWaitOption, BOOL bIgnoreInterlock)
{
#ifdef SIMULATION
	return ERR_CAMERACARRIER_SUCCESS;
#endif

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCameraCarrier%d] [M] [MovePos to %d, WaitOption(%d)] [Start]", m_iInstanceNo + 1, iPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

	if(m_iInstanceNo >= DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		if(m_iWorkInstance > 0)
			return ERR_CAMERACARRIER_SUCCESS; 
	}

	int iResult = CheckOrigin();
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;

//170826 JSh.s
// 	if (FALSE == IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
// 	{
// 		iResult = SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
// 		if (iResult != ERR_CAMERACARRIER_SUCCESS)
// 		{
// 			CString strLogMsg;
// 
// 			strLogMsg.Format(_T("CAMERA CARRIER[%d] move Position(PositionID=%d)"),m_iInstanceNo + 1, iPosID);
// 			SetErrorLevel(strLogMsg,2, __FILE__, __LINE__);
// 			return iResult;
// 		}
// 	}
//170826 JSh.e

	if (TRUE == IsInPos(dTarget))
		return ERR_CAMERACARRIER_SUCCESS;

	if (FALSE == bIgnoreInterlock)
	{
		iResult = checkInterlock(dTarget);
		if (iResult != ERR_CAMERACARRIER_SUCCESS)
			return iResult;
	}

	m_csMoveLock.Lock();
	SetMovingY(TRUE, dTarget);

	iResult = m_plnkAxis->StartMove(dTarget);
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
	{
		SetMovingY(FALSE, dTarget);
		m_csMoveLock.Unlock();
		return iResult;
	}

	Sleep(50);	// WaitOption == FALSE 일 경우를 대비한 Motion 동작 시작 후 안정화 Delay. 
	m_csMoveLock.Unlock();

	if (TRUE == bWaitOption)
	{
		iResult = m_plnkAxis->Wait4Done();
		if (iResult)
		{
			SetMovingY(FALSE, dTarget);
			return iResult;
		}
	}

	SetMovingY(FALSE, dTarget);
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MCameraCarrier%d] [M] [MovePos to %d, WaitOption(%d)] [End]", m_iInstanceNo + 1, iPosID, bWaitOption);
	WriteTactTimeLog(szLogMsg);

	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::Wait4Done(BOOL bSkipMode)
{
	return m_plnkAxis->Wait4Done(bSkipMode);
}

int MCameraCarrier::SafeMoveBackupPos(int iBackupPosID, BOOL bWaitOption, BOOL bAuto)
{
	if(m_iInstanceNo >= DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		return ERR_CAMERACARRIER_SUCCESS;
	}
	double dTarget = GetTargetPosBackup(iBackupPosID);
	return MoveBackupPos(dTarget, iBackupPosID, bWaitOption, bAuto);
}

int MCameraCarrier::MoveBackupPos(double dBackupTargetPos, int iBackupPosID, BOOL bWaitOption, BOOL bAuto)
{
	int iResult = checkInterlockForBackup(iBackupPosID, dBackupTargetPos, bAuto);
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;
	
	//100928.KKY_____________
	if (m_plnkAxisBackupZ->IsOriginReturn() == FALSE)
		// 214003 = 원점 복귀 안됨.
		return generateErrorCode(214016);
	//_______________________

	iResult = m_plnkAxisBackupZ->StartMove(dBackupTargetPos);
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;

	if (TRUE == bWaitOption)
	{
		iResult = Wait4DoneBackup();
		if (iResult)
			return iResult;
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::Wait4DoneBackup(BOOL bSkipMode)
{
	return m_plnkAxisBackupZ->Wait4Done(bSkipMode);
}

int MCameraCarrier::SafeMoveExpand(int iExpandPosID, BOOL bWaitOption, BOOL bAuto)
{
	if(m_iInstanceNo >= DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		return ERR_CAMERACARRIER_SUCCESS;
	}
	double dTarget = GetTargetPosExpand(iExpandPosID);
	return MoveExpandPos(dTarget, iExpandPosID, bWaitOption, bAuto);
}

int MCameraCarrier::MoveExpandPos(double dExpandTargetPos, int iExpandPosID, BOOL bWaitOption, BOOL bAuto)
{
	if (m_plnkAxisExpand->IsOriginReturn() == FALSE)
		// 214003 = 원점 복귀 안됨.
		return generateErrorCode(214017);

	int iResult = m_plnkAxisExpand->StartMove(dExpandTargetPos);
	if (iResult != ERR_CAMERACARRIER_SUCCESS)
		return iResult;

	if (TRUE == bWaitOption)
	{
		iResult = Wait4DoneExpand();
		if (iResult)
			return iResult;
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::Wait4DoneExpand(BOOL bSkipMode)
{
	return m_plnkAxisExpand->Wait4Done(bSkipMode);
}

int MCameraCarrier::CheckOrigin(void)
{
	if (m_plnkAxis->IsOriginReturn() == FALSE)
		// 214003 = 원점 복귀 안됨.
		return generateErrorCode(214003);
	if(m_iInstanceNo < DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		if (m_plnkAxisBackupZ->IsOriginReturn() == FALSE)
			// 214003 = 원점 복귀 안됨.
			return generateErrorCode(214016);
		if (m_plnkAxisExpand->IsOriginReturn() == FALSE)
			// 214003 = 원점 복귀 안됨.
			return generateErrorCode(214017);
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::CheckOriginBackup()
{
	if (m_plnkAxisBackupZ->IsOriginReturn() == FALSE)
		// 214003 = 원점 복귀 안됨.
		return generateErrorCode(214016);
	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::CheckOriginExpand()
{
	if(m_iInstanceNo < DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		if (m_plnkAxisExpand->IsOriginReturn() == FALSE)
			// 214003 = 원점 복귀 안됨.
			return generateErrorCode(214017);
	}

	return ERR_CAMERACARRIER_SUCCESS;
}

double MCameraCarrier::GetCurrentPos(BOOL bType)
{
	double dCurPos = m_plnkAxis->GetCurrentPosition(bType);
	return dCurPos;
}

double MCameraCarrier::GetCurrentPosBackup(BOOL bType)
{
	double dCurPos = m_plnkAxisBackupZ->GetCurrentPosition(bType);
	return dCurPos;
}

double MCameraCarrier::GetCurrentPosExpand(BOOL bType)
{
	double dCurPos = m_plnkAxisExpand->GetCurrentPosition(bType);
	return dCurPos;
}

//@double MCameraCarrier::GetTargetPos(int iPosID, int iTabNumber)
double MCameraCarrier::GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_CAMERACARRIER_MAX_POS);
	ASSERT(iScheduleNo >= 0 && iScheduleNo < DEF_MAX_WORK_SCHEDULE);

	double dTarget = m_rgdFixedPos[iPosID] + m_rgdOffsetPos[iPosID] + m_rgdModelPos[iScheduleNo][iPosID];

	if (iTabNumber != -1)
	{
		double dVariableLength = 0.0;
		if (iPosID == DEF_CAMERACARRIER_PANEL_MARK_POS && m_iWorkInstance == 0)
		{
	#ifdef DEF_SOURCE_SYSTEM
			dVariableLength = m_plnkPreBonderData->m_dLead_Width_Xtab - m_plnkSystemData->m_dInspectCamFixedPosWidth;
	#else
			dVariableLength = m_plnkPreBonderData->m_dLead_Width_Ytab - m_plnkSystemData->m_dInspectCamFixedPosWidth;
	#endif
			dTarget = dVariableLength/2.0;
		}
		// 확인 필요 : Teaching Dlg 에서 dPanelPressPosPos, dVariableLength 값을 디스플레이해 줄 것.
		dTarget += m_plnkWorkSchedule[iScheduleNo]->GetTabPos(iTabNumber);
//110124.del(CalculateModel에서 적용함)_____		dTarget += dVariableLength;
	}

	return dTarget;
}

double MCameraCarrier::GetTargetPosBackup(int iPosID, int iTabNumber)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_CAMERACARRIER_BACKUP_MAX_POS);

	double dTarget = m_rgdFixedPosBackupZ[iPosID] + m_rgdOffsetPosBackupZ[iPosID] + m_rgdModelPosBackupZ[iPosID];
	return dTarget;
}

double MCameraCarrier::GetTargetPosExpand(int iPosID, int iTabNumber)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_CAMERACARRIER_EXPAND_MAX_POS);

	double dTarget = m_rgdFixedPosExpand[iPosID] + m_rgdModelPosExpand[iPosID] + m_rgdOffsetPosExpand[iPosID];
	return dTarget;
}

//@double MCameraCarrier::CalculateInspectPos(double dInspectModelPos)
double MCameraCarrier::CalculateInspectPos(int iScheduleNo, double dInspectModelPos)
{
	ASSERT(iScheduleNo >= 0 && iScheduleNo < DEF_MAX_WORK_SCHEDULE);
	
	double dTarget = m_rgdFixedPos[DEF_CAMERACARRIER_TAB_MARK_POS] + m_rgdOffsetPos[DEF_CAMERACARRIER_TAB_MARK_POS] + dInspectModelPos;//m_rgdModelPos[iPosID];
	dTarget += m_rgdModelPos[iScheduleNo][DEF_CAMERACARRIER_TAB_MARK_POS];//110124.Add
	return dTarget;
}
//170826 JSH.s
double MCameraCarrier::CalculateMountPos(int iScheduleNo, double dModelPos)
{
	ASSERT(iScheduleNo >= 0 && iScheduleNo < DEF_MAX_WORK_SCHEDULE);
	
	double dTarget = m_rgdFixedPos[DEF_CAMERACARRIER_TAB_MOUNT_POS] + m_rgdOffsetPos[DEF_CAMERACARRIER_TAB_MOUNT_POS] + dModelPos;//m_rgdModelPos[iPosID];
	dTarget += m_rgdModelPos[iScheduleNo][DEF_CAMERACARRIER_TAB_MOUNT_POS];//110124.Add
	return dTarget;
}
//170826 JSH.e


//@BOOL MCameraCarrier::IsInPos(int iPosID, double* pdTolerance)
BOOL MCameraCarrier::IsInPos(int iScheduleNo, int iPosID, double* pdTolerance)
{
	double dCompare = GetTargetPos(iScheduleNo, iPosID);
	return IsInPos(dCompare, pdTolerance);
}

BOOL MCameraCarrier::IsInPos(double dTargetPos, double* pdTolerance)
{
	return m_plnkAxis->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

BOOL MCameraCarrier::IsInPosBackup(int iPosID, double* pdTolerance)
{
	double dCompare = GetTargetPosBackup(iPosID);
	return IsInPosBackup(dCompare, pdTolerance);
}

BOOL MCameraCarrier::IsInPosBackup(double dTargetPos, double* pdTolerance)
{
	return m_plnkAxisBackupZ->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

BOOL MCameraCarrier::IsInPosExpand(int iPosID, double* pdTolerance)
{
	double dCompare = GetTargetPosExpand(iPosID);
	return IsInPosExpand(dCompare, pdTolerance);
}

BOOL MCameraCarrier::IsInPosExpand(double dTargetPos, double* pdTolerance)
{
	return m_plnkAxisExpand->ComparePosition(FALSE, dTargetPos, pdTolerance);
}

int MCameraCarrier::checkInterlock(double dTarget)
{
	int iResult = ERR_CAMERACARRIER_SUCCESS;
	
	iResult = CheckCollisionOtherD(dTarget);
	if (iResult)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(15);
			iResult = CheckCollisionOtherD(dTarget);
			if (iResult == 0)
				break;
			if (GetTickCount() - sTime > 3000)
				break;
		}
		if (iResult)
			return iResult;
	}

	if(m_iInstanceNo >= DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		return ERR_CAMERACARRIER_SUCCESS; 
	}

	//BackupZ축 위치가 Down위치여야 한다
	if (FALSE == IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
		return generateErrorCode(214010);

	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::checkInterlockForBackup(int iPosID, double dTarget, BOOL bAuto)
{
	if( (DEF_CAMERACARRIER_BACKUP_VISION_POS == iPosID) || (DEF_CAMERACARRIER_BACKUP_UP_POS == iPosID) )
	{
#if FALSE
		if (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed()
			&& FALSE == m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
		{
			// 228023 = Backup Up 불가. [Unload 위치에 있지 않은 Panel Transfer 가 Panel 흡착하고 있음.]
			return generateErrorCode(228023);
		}
#endif
		if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed()
//			&& (FALSE == m_plnkPanelAligner->IsInXPos(DEF_PANEL_ALIGNER_PRESS_XU_POS)
//			|| FALSE == m_plnkPanelAligner->IsInYPos(DEF_PANEL_ALIGNER_INSPECT_XU_POS))) // 2009.01.21 CYJ  임시 추가.
			&& FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_1_POS)
			&& FALSE == m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_2_POS))
		{
			BOOL isInPos = FALSE;
			for (int i=0; i<10; i++)
			{
				Sleep(100);
				isInPos = m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_1_POS)
						|| m_plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_INSPECT_2_POS);
				if (isInPos) break;
			}
			if (FALSE == isInPos)
			{
				// 228033 = Backup Up 불가. [PanelAligner 가 Press Xu 위치 아님.]
				return generateErrorCode(228033);
			}
		}

		/*100821.KKY.임시삭제___
		if(!bAuto)
		{
			// Group 내의 모든 TabMounter UD Cylinder Up 상태 확인.
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				if (FALSE == m_plnkTabMounter[i]->IsInSafePosTabMounterFromAligner(i))
				{
					double dComparePos = m_plnkTabMounter[i]->GetTargetPosZ(DEF_TABMOUNTER_Z_MOUNT_POS);
					double dCurPos = m_plnkTabMounter[i]->GetCurrentPosZ();
					if (dCurPos < dComparePos + 5.0)
					{
						return generateErrorCode(214011);
					}
				}
			}
		}
		*/
	}
	return ERR_TABMOUNTER_SUCCESS;
}

int MCameraCarrier::calculateModelPosParameter()
{
	int i = 0;
	for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	{
		for (int j=0; j<DEF_MAX_WORK_SCHEDULE; j++)
			m_rgdModelPos[j][i] = 0.0;
	}

	double TabWidth = m_plnkPreBonderData->m_dLead_Width_Xtab;

	//SJ_YYK 150109 Modify...
	// Align은 Camera Carrier 4번으로 하고 
	// Pos 은 마지막 Tab 위치에서 Fidual Mark 간 거리를 이용
	// 나머지 Carrier는 Standard Gap으로 밀어 놓기  (-) 방향으로 ....
	// 바꾼 이유는 물류 방향 쪽 작업을 먼저하도록 변경, 기존 WorkSchedule, WorkScheduleSecond 서로 순서 변경 
	// 계산은 Source라서.,. Xu Tab 만 계산하도록... 하였음. Xu, Xd Tab Data 다를 시 수정 요..

	if (DEF_INSPECTION_CARRIER_4 == m_iWorkInstance || m_iInstanceNo == DEF_INSPECTION_CARRIER_4 + 1)
	{
	//170916 JSH.s
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		if(m_iInstanceNo == DEF_INSPECTION_CARRIER_4 + 1)
		{
			m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = -m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0;
		}
		else
		{
			m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = -TabWidth/2.0; //??
		}
#else
	//170916 JSH.e
		
		int MaxTab = m_plnkTabData->m_iXuTabQty;
		double dTabPitchSum = 0.0;
		
		for(int j=0; j<MaxTab; j++)
		{
			dTabPitchSum += m_plnkTabData->m_dXuTabPitch[j];
		}
		
		double dDiffDist = m_plnkPanelData->m_dDistance_FiduMark_Xu - dTabPitchSum - (TabWidth / 2.0);
		
		if(m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]->GetUseWorkSide() == TRUE)
		{
			MVarList<MInspectCarrierInfo> iList = m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]->GetFirstInspectionCarrierList();
			MInspectCarrierInfo myInfo = iList.GetInfo(m_iWorkInstance);
			
			// Last Tab 인지 확인..
			if(myInfo.m_nRealTabNo != MaxTab -1)
			{
				double dTabDist = 0.0;
				
				for(int k= myInfo.m_nRealTabNo +1; k<MaxTab; k++)
				{
					dTabDist +=  m_plnkTabData->m_dXuTabPitch[k]; 
				}
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = myInfo.m_dInspectPos + dDiffDist + dTabDist;
				MOlbSystemPre.SetInsCamFrontWorkFiduPos(m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS]);
			}
			else
			{
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = myInfo.m_dInspectPos + dDiffDist;				
				MOlbSystemPre.SetInsCamFrontWorkFiduPos(m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS]);
			}			
		}
		else
		{
			m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
			m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = MOlbSystemPre.GetInsCamFrontWorkFiduPos();

			if(m_iInstanceNo == DEF_INSPECTION_CARRIER_4 + 1) //SJ_YYK 161104 Add.
			{
				double dCalPos = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][0];
				dCalPos = dCalPos - TabWidth;
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
				m_rgdModelPos[DEF_WORK_SCHEDULE_FIRST][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = MOlbSystemPre.GetInsCamFrontWorkFiduPos() - dCalPos;
			}
		}
#endif		
	}
	else
	{
		/*
		for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
		{
			m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
			m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = m_plnkSystemData->m_dDistNegInspectCamera[i] - ( m_rgdFixedPos[DEF_CAMERACARRIER_PANEL_MARK_POS] ) + (m_iWorkInstance * (DEF_INSPECTION_STANDARD_GAP+20.0)+10.0);
		}
		*/
		for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
		{
			//사용하지 않는 작업변이면...
			if (FALSE == m_plnkWorkSchedule[i]->GetUseWorkSide())
			{
				m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_MARK_POS] = m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = TabWidth/2.0
																			//@130710.KKY.미사용 작업변일 경우 충분한 거리를 둔다
																			//@+ (m_iWorkInstance * DEF_INSPECTION_STANDARD_GAP+10.0);
																			+ m_plnkSystemData->m_dDistNegInspectCamera[i] - ( m_rgdFixedPos[DEF_CAMERACARRIER_PANEL_MARK_POS] ) + (m_iWorkInstance * (DEF_INSPECTION_STANDARD_GAP+20.0)+10.0);
			}
			else
			{
				MVarList<MInspectCarrierInfo> iList = m_plnkWorkSchedule[i]->GetFirstInspectionCarrierList();
				MInspectCarrierInfo myInfo = iList.GetInfo(m_iWorkInstance);
				int iUseCnt = 0;
				for (int j=0; j<iList.GetCount(); j++)
				{
					if (iList.GetInfo(j).m_nWorkTabNo != -1)
						iUseCnt++;
				}
				//170913 JSH.s
				//JSH.Note PanelAligner의 MarkPos는 1개.. WorkSchedule에 관계없이 동일
				if (iUseCnt == DEF_MAX_INSPECTION_CAM_CARRIER)
				{
					m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
						m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = myInfo.m_dInspectPos;
				}
				else
				{
					m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_MARK_POS] = 
						m_rgdModelPos[i][DEF_CAMERACARRIER_PANEL_SUB_MARK_POS] = TabWidth/2.0
						+ m_plnkSystemData->m_dDistNegInspectCamera[i] - ( m_rgdFixedPos[DEF_CAMERACARRIER_PANEL_MARK_POS] ) + (m_iWorkInstance * (DEF_INSPECTION_STANDARD_GAP+20.0)+10.0); //170719_KDH Test중,, 20 -> 10
				}
			}
		}
	}


	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::calculateExpandModelPosParameter()
{
	for (int i = 0; i < DEF_CAMERACARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgdModelPosExpand[i] = 0.0;
		m_rgdFixedPosExpand[i] = m_plnkSystemData->m_dData_CameraExpand[m_iGroupNo][m_iWorkInstance];

		if (i == DEF_CAMERACARRIER_EXPAND_WORKING_POS)
		{
#	ifdef DEF_SOURCE_SYSTEM
			m_rgdModelPosExpand[i] = m_plnkPreBonderData->m_dLead_Width_Xtab - m_plnkSystemData->m_dData_CameraExpand[m_iGroupNo][m_iWorkInstance];
#	else
			m_rgdModelPosExpand[i] = m_plnkPreBonderData->m_dLead_Width_Ytab - m_plnkSystemData->m_dData_CameraExpand[m_iGroupNo][m_iWorkInstance];
#	endif
		}
	}
	return ERR_CAMERACARRIER_SUCCESS;
}

const int MCameraCarrier::GetPreCamCarrierNo()
{
	if (m_plnkPreCamCarrier == NULL)
		return DEF_NONE_INSPECTION_CAMERA;

	return m_plnkPreCamCarrier->GetInspectCamCarrierNo();
}

const int MCameraCarrier::GetNextCamCarrierNo()
{
	if (m_plnkNextCamCarrier == NULL)
		return DEF_NONE_INSPECTION_CAMERA;

	return m_plnkNextCamCarrier->GetInspectCamCarrierNo();
}

const int MCameraCarrier::GetInspectCamCarrierNo() const
{
#if FALSE
	if (m_iGroupNo == DEF_INSPECTION_CAMERA_NONE_GROUP)
		return DEF_NONE_INSPECTION_CAMERA;

	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		ASSERT(DEF_MAX_INSPECTION_CAM_CARRIER == 3);

		int rgiInspectCamNo[3]		// 위의 ASSERT 로 배열 크기 정상 여부 확인.
			= { DEF_INSPECTION_CAMERA1,
				DEF_NONE_INSPECTION_CAMERA,
				DEF_INSPECTION_CAMERA2
			};	
		return rgiInspectCamNo[m_iInstanceNo];
	}
	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		ASSERT(DEF_MAX_INSPECTION_CAM_CARRIER == 8);

		int rgiInspectCamNo[8]		// 위의 ASSERT 로 배열 크기 정상 여부 확인.
			= { DEF_NONE_INSPECTION_CAMERA,
				DEF_NONE_INSPECTION_CAMERA,
				DEF_INSPECTION_CAMERA1,
				DEF_INSPECTION_CAMERA2,
				DEF_INSPECTION_CAMERA3,
				DEF_INSPECTION_CAMERA4,
				DEF_INSPECTION_CAMERA5,
				DEF_INSPECTION_CAMERA6
			};	
		return rgiInspectCamNo[m_iInstanceNo];
	}
	return DEF_NONE_INSPECTION_CAMERA;
#endif

//	return m_iInstanceNo;
	return m_iWorkInstance;
}

int MCameraCarrier::Expand(BOOL bSkipSensor)
{
	if(m_iInstanceNo >= DEF_MAX_INSPECTION_CAM_CARRIER)
		return ERR_CAMERACARRIER_SUCCESS;
	int iResult = SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS, !bSkipSensor);
	if (iResult)
		return iResult;

	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::Shrink(BOOL bSkipSensor)
{
	int iResult = SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_INIT_POS, !bSkipSensor);
	if (iResult)
		return iResult;

	return ERR_CAMERACARRIER_SUCCESS;
}

int MCameraCarrier::ExpandShrinkForTabAlign(BOOL bSkipSensor)
{
	int iResult = ERR_CAMERACARRIER_SUCCESS;
	if (FALSE == IsInPosExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS))
		iResult = SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS, !bSkipSensor);

	return iResult;
}

int MCameraCarrier::ExpandShrinkForPanelAlign(BOOL bSkipSensor)
{
	int iResult = ERR_CAMERACARRIER_SUCCESS;
	if (FALSE == IsInPosExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS))
		iResult = SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS, !bSkipSensor);

	return iResult;
}

BOOL MCameraCarrier::IsExpand(void)
{
	return IsInPosExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS);
}

BOOL MCameraCarrier::IsShrink(void)
{
	return IsInPosExpand(DEF_CAMERACARRIER_EXPAND_INIT_POS);
}

/**
 * 이동시 인근 Carrier와 충돌여부 확인하는 함수
 **/

void MCameraCarrier::SetMovingY(BOOL bMove, double posTarget)
{
	m_posTargetMove = posTarget;
	m_bMoving = bMove;
}

int	MCameraCarrier::CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber)
{
	double tgPos = GetTargetPos(iScheduleNo, iPosID, iTabNumber);
	return CheckCollisionOtherD(tgPos);
}

int MCameraCarrier::CheckCollisionOtherD(double posTarget)
{
	int iResult = ERR_CAMERACARRIER_SUCCESS;

	if(m_iInstanceNo >= DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		return ERR_CAMERACARRIER_SUCCESS; 
	}
	m_csMoveLock.Lock();

	double dCurrentPos = GetCurrentPos();
	BOOL bDir = (posTarget > dCurrentPos) ? TRUE /* (+) 방향 */ : FALSE /* (-) 방향 */;

	if (m_plnkPreCamCarrier != NULL)
	{
		if (TRUE == bDir)
		{
			iResult = m_plnkPreCamCarrier->CheckOrigin();
			if (iResult)
			{
				m_csMoveLock.Unlock();
				return iResult;
			}
			double dComparePos = 0.0;
			if (m_plnkPreCamCarrier->IsMovingY())
				dComparePos = m_plnkPreCamCarrier->GetMovingTgPos();
			else
				dComparePos = m_plnkPreCamCarrier->GetCurrentPos();

			if (posTarget > dComparePos - (DEF_CAMERACARRIER_SAFE_MARGIN - 1.5))
			{
				m_csMoveLock.Unlock();
				// 214006 = 이동 불가 [이동 시, Pre Camera Carrier 와 간섭 발생 : (+) 이동]
				return generateErrorCode(214006);
			}
		}
	}
	
	if (m_plnkNextCamCarrier != NULL)
	{
		if (FALSE == bDir)
		{
			iResult = m_plnkNextCamCarrier->CheckOrigin();
			if (iResult)
			{
				m_csMoveLock.Unlock();
				return iResult;
			}
			double dComparePos = 0.0;
			if (m_plnkNextCamCarrier->IsMovingY())
				dComparePos = m_plnkNextCamCarrier->GetMovingTgPos();
			else
				dComparePos = m_plnkNextCamCarrier->GetCurrentPos();

			if (posTarget < dComparePos + (DEF_CAMERACARRIER_SAFE_MARGIN - 1.5))
			{
				m_csMoveLock.Unlock();
				// 214009 = 이동 불가 [이동 시, Next Camera Carrier 와 간섭 발생 : (-) 이동]
				return generateErrorCode(214009);
			}
		}
	}
	m_csMoveLock.Unlock();
	return  ERR_CAMERACARRIER_SUCCESS;
}

BOOL MCameraCarrier::IsMovingY()
{
	m_csMoveLock.Lock();
	BOOL bMove = (m_bMoving || m_plnkAxis->IsInMotion());
	m_csMoveLock.Unlock();
	return bMove;
}

double MCameraCarrier::GetMovingTgPos()
{
	return m_posTargetMove;
}

int MCameraCarrier::GetCameraNo(int iCameraId)
{
	int iCamNo = 0;
	(iCameraId == 0) ? iCamNo = m_iCamNo1 : iCamNo = m_iCamNo2;
	return iCamNo;
}
