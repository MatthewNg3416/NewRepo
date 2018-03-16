#include "StdAfx.h"
#include "MSystemData.h"
#include "IAxis.h"
#include "ICylinder.h"
#include "MTabAligner.h"
#include "MTeachingPosData.h"

#include "common.h"

MTabAligner::MTabAligner(SCommonAttribute commonData, STabAlignerRefCompList listRefComponents, STabAlignerData datComponent)
: ICommon(commonData)
{
	AssignComponents(listRefComponents);
	SetData(datComponent);

	LoadFixedPosParameter();
	LoadOffsetPosParameter();
	calculateModelPosParameter();
}

MTabAligner::~MTabAligner()
{
}

void MTabAligner::SetData(STabAlignerData datComponent)
{
	m_plnkSystemData		= datComponent.m_plnkSystemData;
	m_sTeachingInfo			= datComponent.m_sTeachingInfo;
	m_strPosFileName		= datComponent.m_strPosFileName;
	m_strOffsetPosFileName	= datComponent.m_strOffsetPosFileName;
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);
	m_iPrealignCameraNo		= datComponent.m_iUseCameraNo;
}

void MTabAligner::AssignComponents(STabAlignerRefCompList TabAlignerCompList)
{
	m_plnkAxis1 = TabAlignerCompList.m_plnkAxis1;
	m_plnkAxis2 = TabAlignerCompList.m_plnkAxis2;
	m_plnkESCyl = TabAlignerCompList.m_plnkESCyl;
}

void MTabAligner::AssignStateCheckComponents(SStateCheckCompListForTabAligner sCompList)
{
}

IAxis* MTabAligner::GetMotionComponent(int iAxisID)
{
	switch (iAxisID)
	{
	case DEF_TABALIGNER_X1_AXIS:
		return m_plnkAxis1;

	case DEF_TABALIGNER_X2_AXIS:
		return m_plnkAxis2;

	default :					// 축 ID 오류
		return NULL;
	}
}

int MTabAligner::ChangeModel(void)
{
	// Model Path Change
	m_strOffsetPosFilePath.Format(_T("%s\\%s"), m_plnkSystemData->m_strModelFileDir, m_plnkSystemData->m_strModelFileName);

	// X  Offset 좌표 Load
	int iResult = LoadOffsetPosParameter();
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	calculateModelPosParameter();

	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::LoadOffsetPosParameter(void)
{
	MTeachingPosData TabAlignerOffsetPosDat(
		m_sTeachingInfo,
		m_strOffsetPosFileName,
		m_strOffsetPosFilePath);

	BOOL bResult = TabAlignerOffsetPosDat.ReadData();	
	for (int i = 0; i < DEF_TABALIGNER_MAX_POS; i++)
	{
		m_rgsOffsetPos[i].dX = TabAlignerOffsetPosDat.m_TeachingPos[i][DEF_TABALIGNER_X1_AXIS];
		m_rgsOffsetPos[i].dT = TabAlignerOffsetPosDat.m_TeachingPos[i][DEF_TABALIGNER_X2_AXIS];
	}

	if (!bResult)
		// X축 Offset 좌표 파일 읽기를 실패 했습니다.
		return generateErrorCode(229000);
	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::LoadFixedPosParameter(void)
{
	int iResult = ERR_TABALIGNER_SUCCESS;
	BOOL bResult;

	MTeachingPosData TabAlignerFixPosDat(m_sTeachingInfo, m_strPosFileName);

	bResult = TabAlignerFixPosDat.ReadData();
	
	for (int i = 0; i < DEF_TABALIGNER_MAX_POS; i++)
	{
		m_rgsFixedPos[i].dX = TabAlignerFixPosDat.m_TeachingPos[i][DEF_TABALIGNER_X1_AXIS];
		m_rgsFixedPos[i].dT = TabAlignerFixPosDat.m_TeachingPos[i][DEF_TABALIGNER_X2_AXIS];
	}

	if (!bResult)
		return generateErrorCode(229001);

	return iResult;
}

int MTabAligner::SaveOffsetPosParameter(MPos_XYT* pOffsetPos)
{
	ASSERT(pOffsetPos != NULL);

	MTeachingPosData TabAlignerOffsetPosDat(m_sTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	for (int i = 0; i < DEF_TABALIGNER_MAX_POS; i++)
	{
		m_rgsOffsetPos[i] = pOffsetPos[i];

		TabAlignerOffsetPosDat.m_TeachingPos[i][DEF_TABALIGNER_X1_AXIS] = m_rgsOffsetPos[i].dX;
		TabAlignerOffsetPosDat.m_TeachingPos[i][DEF_TABALIGNER_X2_AXIS] = m_rgsOffsetPos[i].dT;
	}

	BOOL bResult = TabAlignerOffsetPosDat.WriteData();
	if (!bResult)
		return generateErrorCode(229002);
	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::SaveFixedPosParameter(MPos_XYT* pFixedPos)
{
	ASSERT(pFixedPos != NULL);

	int iResult = ERR_TABALIGNER_SUCCESS;
	BOOL bResult;
	
	MTeachingPosData TabAlignerFixedPosDat(m_sTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_TABALIGNER_MAX_POS; i++)
	{
		m_rgsFixedPos[i] = pFixedPos[i];

		TabAlignerFixedPosDat.m_TeachingPos[i][DEF_TABALIGNER_X1_AXIS] = m_rgsFixedPos[i].dX;
		TabAlignerFixedPosDat.m_TeachingPos[i][DEF_TABALIGNER_X2_AXIS] = m_rgsFixedPos[i].dT;
	}

	bResult = TabAlignerFixedPosDat.WriteData();
	
	if (!bResult)
		//WriteErrorLog("Saving TabAligner Y Fixed Pos Parameter Error : Writing Failure");
		return generateErrorCode(229003);

	return iResult;
}

int MTabAligner::GetPosParameter(MPos_XYT* pFixedPos, MPos_XYT* pOffsetPos, MPos_XYT* pModelPos)
{
	for (int i = 0; i < DEF_TABALIGNER_MAX_POS; i++)
	{
		if (pFixedPos != NULL)
			pFixedPos[i] = m_rgsFixedPos[i];

		if (pModelPos != NULL)
			pModelPos[i] = m_rgsModelPos[i];

		if (pOffsetPos != NULL)
			pOffsetPos[i] = m_rgsOffsetPos[i];
	}
	return ERR_TABALIGNER_SUCCESS;
}
	
STeachingInfo MTabAligner::GetTeachingInfo()
{
	return m_sTeachingInfo;
}

int MTabAligner::MovePos(MPos_XYT posTarget, int iPosID, BOOL bWaitOption)
{
#ifdef SIMULATION
	return ERR_TABALIGNER_SUCCESS;
#endif

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabAligner%d] [M] [Move XT Pos : %d] [Start]", m_iInstanceNo + 1, iPosID);
	WriteTactTimeLog(szLogMsg);

	int iResult = CheckOrigin();
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	// Interlock 확인 - Check 할 사항 없음.
/*	iResult = checkInterlock(iPosID);
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;
*/

	iResult = m_plnkAxis1->StartMove(posTarget.dX);
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	iResult = m_plnkAxis2->StartMove(posTarget.dT);
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	if (!bWaitOption)
		return ERR_TABALIGNER_SUCCESS;

	iResult = m_plnkAxis1->Wait4Done();
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	iResult = m_plnkAxis2->Wait4Done();
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabAligner%d] [M] [Move XT Pos : %d] [End]", m_iInstanceNo + 1, iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::Wait4Done()
{
	int iResult = m_plnkAxis1->Wait4Done(TRUE);
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	iResult = m_plnkAxis2->Wait4Done(TRUE);
	if (iResult != ERR_TABALIGNER_SUCCESS)
		return iResult;

	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::SafeMovePos(int iPosID, BOOL bWaitOption)
{
	MPos_XYT posTarget = GetTargetPos(iPosID);
	return MovePos(posTarget, iPosID, bWaitOption);
}

int MTabAligner::CheckOrigin(void)
{
	if (m_plnkAxis1->IsOriginReturn() == FALSE)
		return generateErrorCode(229004);
	if (m_plnkAxis2->IsOriginReturn() == FALSE)
		return generateErrorCode(229005);
	return ERR_TABALIGNER_SUCCESS;
}

MPos_XYT MTabAligner::GetCurrentPos(BOOL bType)
{
	ASSERT(m_plnkAxis1 != NULL);
	MPos_XYT posCurrent;
	posCurrent.dX = m_plnkAxis1->GetCurrentPosition(bType);
	posCurrent.dT = m_plnkAxis2->GetCurrentPosition(bType);

	return posCurrent;
}

MPos_XYT MTabAligner::GetTargetPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_TABALIGNER_MAX_POS);
	MPos_XYT posTarget = m_rgsFixedPos[iPosID] + m_rgsOffsetPos[iPosID] + m_rgsModelPos[iPosID];
	return posTarget;
}

BOOL MTabAligner::IsInPos(int iPosID, MPos_XYT* pTolerance)
{
	MPos_XYT posCompare = GetTargetPos(iPosID);
	return IsInPos(posCompare, pTolerance);
}

BOOL MTabAligner::IsInPos(MPos_XYT posTarget, MPos_XYT* pTolerance)
{
	BOOL bState[DEF_TABALIGNER_AXIS_MAX_NO];

	if (pTolerance != NULL)
	{
		bState[DEF_TABALIGNER_X1_AXIS] = m_plnkAxis1->ComparePosition(FALSE , posTarget.dX,	&pTolerance->dX);
		bState[DEF_TABALIGNER_X2_AXIS] = m_plnkAxis2->ComparePosition(FALSE , posTarget.dT,  &pTolerance->dT);
	}
	else
	{
		bState[DEF_TABALIGNER_X1_AXIS] = m_plnkAxis1->ComparePosition(FALSE , posTarget.dX, NULL);
		bState[DEF_TABALIGNER_X2_AXIS] = m_plnkAxis2->ComparePosition(FALSE , posTarget.dT,NULL);
	}

	return bState[DEF_TABALIGNER_X1_AXIS] && bState[DEF_TABALIGNER_X2_AXIS];
}

int MTabAligner::GetPrealignCamNo()
{
	return m_iPrealignCameraNo;
}

int MTabAligner::checkInterlock(int iPosID)
{
	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::calculateModelPosParameter()
{
	m_rgsModelPos[DEF_TABALIGNER_READY_POS].dX	= 0.0;
	m_rgsModelPos[DEF_TABALIGNER_READY_POS].dT	= 0.0;

	m_rgsModelPos[DEF_TABALIGNER_ALIGN_POS].dX	= 0.0;
	m_rgsModelPos[DEF_TABALIGNER_ALIGN_POS].dT	= 0.0;

	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::Expand(BOOL bSkipSensor)
{
	if (NULL == m_plnkESCyl)
		return ERR_TABALIGNER_SUCCESS;

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabAligner] [M] [TabAligner ES Cyl Expand (SkipMode:%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	
	int iResult = m_plnkESCyl->Expand(bSkipSensor);
	if (iResult)
		return iResult;
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabAligner] [M] [TabAligner ES Cyl Expand (SkipMode:%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	return ERR_TABALIGNER_SUCCESS;
}

int MTabAligner::Shrink(BOOL bSkipSensor)
{
	if (NULL == m_plnkESCyl)
		return ERR_TABALIGNER_SUCCESS;
	
	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabAligner] [M] [TabAligner ES Cyl Shrink (SkipMode:%d)] [Start]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	
	int iResult = m_plnkESCyl->Shrink(bSkipSensor);
	if (iResult)
		return iResult;
	
	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabAligner] [M] [TabAligner ES Cyl Shrink (SkipMode:%d)] [End]", bSkipSensor);
	WriteTactTimeLog(szLogMsg);
	return ERR_TABALIGNER_SUCCESS;
}

BOOL MTabAligner::IsExpand(void)
{
	if (NULL == m_plnkESCyl)
		return TRUE;

	return m_plnkESCyl->IsExpand();
}

BOOL MTabAligner::IsShrink(void)
{
	if (NULL == m_plnkESCyl)
		return TRUE;

	return m_plnkESCyl->IsShrink();	
}