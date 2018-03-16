#include "stdafx.h"
#include "MProcessData.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MTabData.h"
#include "MTrsTabFeeder.h"
#include "Common.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MProcessData::MProcessData(MSystemData* pSystemData, MPanelData* pPanelData, MTabData* pTabData)
{
	m_plnkSystemData = pSystemData;
	m_plnkPanelData = pPanelData;
	m_plnkTabData = pTabData;

	m_prgXuData = NULL;
	m_prgXdData = NULL;
	m_prgYlData = NULL;
	m_prgYrData = NULL;
	
	for (int i = 0; i < DEF_MAX_WORK_SIDE; i++)
	{
		m_rgiStartTabNo[i] = -1;
		m_rgiEndTabNo[i] = -1;
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		InitializeCriticalSection(&m_rgcsCarrierLoadWork[i]);

	for (int i = 0; i < DEF_MAX_WORK_SIDE; i++)
		InitializeCriticalSection(&m_csDataUpdate[i]);

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_rgbGetNextPanelTabIC[i] = FALSE;
		m_rgbNextPanelTabICLoad[i] = NULL;
		m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[i] = FALSE;
	}

	m_bWorkStartAfterUnitInitialization = FALSE;
	m_bInitialUpdateProcessDataComplete = FALSE;
}

MProcessData::~MProcessData()
{
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		DeleteCriticalSection(&m_rgcsCarrierLoadWork[i]);

	for (int i = 0; i < DEF_MAX_WORK_SIDE; i++)
		DeleteCriticalSection(&m_csDataUpdate[i]);

	deletePocessData();
}

void MProcessData::deletePocessData()
{
	if (m_prgXuData)
	{
		delete[] m_prgXuData;
		m_prgXuData = NULL;
	}
	
	if (m_prgXdData)
	{
		delete[] m_prgXdData;
		m_prgXdData = NULL;
	}
	
	if (m_prgYlData)
	{
		delete[] m_prgYlData;
		m_prgYlData = NULL;
	}
	
	if (m_prgYrData)
	{
		delete[] m_prgYrData;
		m_prgYrData = NULL;
	}

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		if (m_rgbNextPanelTabICLoad[i])
		{
			delete[] m_rgbNextPanelTabICLoad[i];
			m_rgbNextPanelTabICLoad[i] = NULL;
		}

		m_rgbGetNextPanelTabIC[i] = FALSE;
		m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[i] = FALSE;
	}
}

void MProcessData::ChangeProcessData()
{
	deletePocessData();
}

SWorkSideData MProcessData::getWorkSideData(EWorkingSide eWorkSide)
{
	SWorkSideData sWsData = { NULL, -1, -1, 0 };
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xu)
			break;

		sWsData.pTab = m_prgXuData;
		sWsData.iStart = m_rgiStartTabNo[WORKING_SIDE_X_UP];
		sWsData.iEnd = m_rgiEndTabNo[WORKING_SIDE_X_UP];
#ifndef DEF_MIRROR_
		sWsData.iGap = -1;
#else
		sWsData.iGap = +1;
#endif
		break;

	case WORKING_SIDE_X_DOWN:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xd)
			break;

		sWsData.pTab = m_prgXdData;
		sWsData.iStart = m_rgiStartTabNo[WORKING_SIDE_X_DOWN];
		sWsData.iEnd = m_rgiEndTabNo[WORKING_SIDE_X_DOWN];
#ifndef DEF_MIRROR_
		sWsData.iGap = +1;		
#else
		sWsData.iGap = -1;
#endif
		break;

	case WORKING_SIDE_Y_LEFT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yl)
			break;

		sWsData.pTab = m_prgYlData;
		sWsData.iStart = m_rgiStartTabNo[WORKING_SIDE_Y_LEFT];
		sWsData.iEnd = m_rgiEndTabNo[WORKING_SIDE_Y_LEFT];
#ifndef DEF_MIRROR_
		sWsData.iGap = -1;
#else
		sWsData.iGap = +1;
#endif
		break;

	case WORKING_SIDE_Y_RIGHT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yr)
			break;

		sWsData.pTab = m_prgYrData;
		sWsData.iStart = m_rgiStartTabNo[WORKING_SIDE_Y_RIGHT];
		sWsData.iEnd = m_rgiEndTabNo[WORKING_SIDE_Y_RIGHT];
#ifndef DEF_MIRROR_
		sWsData.iGap = +1;
#else
		sWsData.iGap = -1;
#endif
		break;

	default:
		break;
	}

	// 사용하지 않는 WorkingSide 가 인자로 넘어올 수도 있다.
	return sWsData;
}

MPos_XY* MProcessData::GetOldInspectionPanelMarkVisionPosStoragePointer(EWorkingSide eWorkSide, int iTabNo)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);
	return ws.pTab[iTabNo].m_rgxyOldInspectionPanelMarkVisionPos;
}

int MProcessData::GetWorkTabMounterNo(EWorkingSide eWorkSide, int iTabNo)
{
	if (-1 == iTabNo)
		return DEF_NONE_TABMOUNTER;

	SWorkSideData ws = getWorkSideData(eWorkSide);

	if (FALSE == ws.pTab[iTabNo].m_bUseTab)
		return DEF_NONE_TABMOUNTER;

	if (REPAIR_RUN_MODE == m_plnkSystemData->m_eRunMode && FALSE == ws.pTab[iTabNo].m_bRepairUseTab)
		return DEF_NONE_TABMOUNTER;

	return ws.pTab[iTabNo].m_iWorkMounterNo;
}

int MProcessData::GetMounterDefaultTabNo(EWorkingSide eWorkSide, const int iTabMounterNo)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);

	int iGroupStart = ws.iStart;
	int iGroupEnd = ws.iEnd;

	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
		getSourceUnit1stCycleOpTabNo(&ws, &iGroupStart, &iGroupEnd);
	
	for (int i = iGroupStart; i != iGroupEnd; i += ws.iGap)
	{
		if (iTabMounterNo == ws.pTab[i].m_iWorkMounterNo)
			return i;
	}

	// Mounter 개수 보다 Use Tab 개수가 작은 경우
	return -1;
}

double MProcessData::GetTabPrealignOffset(EWorkingSide eWorkSide, int iTabNo)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);
	return ws.pTab[iTabNo].m_posTabPrealignResult.dX;
}

int MProcessData::getGroupNo(EWorkingSide eWorkSide)
{
	int iGroupNo = -1;
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
#ifndef DEF_MIRROR_
		iGroupNo = DEF_REAR_GROUP;
#else
		iGroupNo = DEF_FRONT_GROUP;
#endif
	case WORKING_SIDE_X_DOWN:
#ifndef DEF_MIRROR_
		iGroupNo = DEF_FRONT_GROUP;
#else
		iGroupNo = DEF_REAR_GROUP;
#endif
		break;
		
	case WORKING_SIDE_Y_LEFT:
#ifndef DEF_MIRROR_
		iGroupNo = DEF_FRONT_GROUP;
#else
		iGroupNo = DEF_REAR_GROUP;
#endif
		break;
		
	case WORKING_SIDE_Y_RIGHT:
#ifndef DEF_MIRROR_
		iGroupNo = DEF_REAR_GROUP;
#else
		iGroupNo = DEF_FRONT_GROUP;
#endif
		break;
		
	default:
		// Error
		break;
	}
	return iGroupNo;
}

const int MProcessData::GetMaxTabQty(EWorkingSide eWorkSide)
{
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Xu)
			return m_plnkTabData->m_iXuTabQty;
		
	case WORKING_SIDE_X_DOWN:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Xd)
			return m_plnkTabData->m_iXdTabQty;
		
	case WORKING_SIDE_Y_LEFT:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yl)
			return m_plnkTabData->m_iYlTabQty;
		
	case WORKING_SIDE_Y_RIGHT:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yr)
			return m_plnkTabData->m_iYrTabQty;

	default:
		break;
	}
	return -1;
}

const int MProcessData::getWorkRangeNo(EWorkingSide eWorkSide)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);

	int i;
	for (i = ws.iStart; i != ws.iEnd; i += ws.iGap)
	{
		if (FALSE == ws.pTab[i].m_bUseTab)
			continue;

		if (REPAIR_RUN_MODE == m_plnkSystemData->m_eRunMode && FALSE == ws.pTab[i].m_bRepairUseTab)
			continue;

		if (FALSE == ws.pTab[i].m_bMountCompleted)
		{
			if (abs(ws.iStart - i) < DEF_MAX_WORKER_PER_GROUP)	// 1st Work Range
				return DEF_WORK_RANGE_1;
			else
				return DEF_WORK_RANGE_2;
		}
	}

	// 현재 Panel 에 대한 모든 Cycle 이 끝났으면 어차피 다음 Cycle 준비해야 한다.
	// 다음 Cycle 이 항상 1st Work Range 인 것은 아니므로 추가적인 확인 필요.
	int i1stWorkRangeEnd = ws.iStart + ws.iGap * DEF_MAX_WORKER_PER_GROUP;
	if ((ws.iGap > 0 && i1stWorkRangeEnd > ws.iEnd) || (ws.iGap < 0 && i1stWorkRangeEnd < ws.iEnd))
		i1stWorkRangeEnd = ws.iEnd;

	for (i = ws.iStart; i != i1stWorkRangeEnd; i += ws.iGap)
	{
		if (TRUE == ws.pTab[i].m_bUseTab)
			return DEF_WORK_RANGE_1;
	}

	if (i == ws.iEnd)
		return DEF_WORK_RANGE_NONE;

	return DEF_WORK_RANGE_2;
}

void MProcessData::getWorkCycleOpTabNo(const EWorkingSide eWorkSide, const SWorkSideData* const ws, int* piStartNo, int* piEndNo)
{
	// 현재 작업 Cycle 이 수행하고 있는 작업 범위가 1st Work Range 인지 2nd Work Range 인지 구분한다.
	int iWorkRange = getWorkRangeNo(eWorkSide);
	if (DEF_WORK_RANGE_1 == iWorkRange)
	{
		*piStartNo = ws->iStart;
		*piEndNo = ws->iStart + ws->iGap * DEF_MAX_WORKER_PER_GROUP;

		if ((ws->iGap > 0 && *piEndNo > ws->iEnd) || (ws->iGap < 0 && *piEndNo < ws->iEnd))
			*piEndNo = ws->iEnd;
	}
	else if (DEF_WORK_RANGE_2 == iWorkRange)
	{
		*piStartNo = ws->iStart + ws->iGap * DEF_MAX_WORKER_PER_GROUP;
		*piEndNo = ws->iEnd;	// 작업 범위는 2nd Work Rage 를 넘지 못한다.
	}
	else
	{
		*piStartNo = ws->iEnd;
		*piEndNo = ws->iEnd;
	}
}

void MProcessData::getSourceUnit1stCycleOpTabNo(const SWorkSideData* const ws, int* piStartNo, int* piEndNo)
{
	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
		return;

	int iWorkRange = -1;
	for (int i = ws->iStart; i != ws->iEnd; i += ws->iGap)
	{
		if (TRUE == ws->pTab[i].m_bUseTab)
		{
			if (abs(ws->iStart - i) < DEF_MAX_WORKER_PER_GROUP)	// 1st Work Range
				iWorkRange = DEF_WORK_RANGE_1;
			else
				iWorkRange = DEF_WORK_RANGE_2;

			break;
		}
	}

	if (-1 == iWorkRange)
	{
		*piStartNo = 0;
		*piEndNo = 0;
		return;
	}

	if (DEF_WORK_RANGE_1 == iWorkRange)
	{
		*piStartNo = ws->iStart;
		*piEndNo = ws->iStart + ws->iGap * DEF_MAX_WORKER_PER_GROUP;
		
		if ((ws->iGap > 0 && *piEndNo > ws->iEnd) || (ws->iGap < 0 && *piEndNo < ws->iEnd))
			*piEndNo = ws->iEnd;
	}
	else // if (DEF_WORK_RANGE_2 == iWorkRange)
	{
		*piStartNo = ws->iStart + ws->iGap * DEF_MAX_WORKER_PER_GROUP;
		*piEndNo = ws->iEnd;	// 작업 범위는 2nd Work Rage 를 넘지 못한다.
	}
}

const BOOL MProcessData::IsWorkSideTabMounterGroupLeader(EWorkingSide eWorkSide, int iTabMounterNo)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);

	int iGroupStart = -1;
	int iGroupEnd = -1;
	int i;
	
	getWorkCycleOpTabNo(eWorkSide, &ws, &iGroupStart, &iGroupEnd);

	ASSERT(iTabMounterNo != -1);
	for (i = iGroupStart; i != iGroupEnd; i += ws.iGap)
	{
		if (FALSE == ws.pTab[i].m_bUseTab)
			continue;
		
		if (REPAIR_RUN_MODE == m_plnkSystemData->m_eRunMode && FALSE == ws.pTab[i].m_bRepairUseTab)
			continue;

		break;
	}

	// sesl_jdy 	if (ws.pTab[i].m_iWorkMounterNo == iTabMounterNo)
	if (i != iGroupEnd && ws.pTab[i].m_iWorkMounterNo == iTabMounterNo)
		return TRUE;

	return FALSE;
}

CString MProcessData::GetDataLog(EWorkingSide eWorkSide)
{
	CString strData;
	strData.Format(_T("ws(%d):"), (int)eWorkSide);

	SWorkSideData ws = getWorkSideData(eWorkSide);

	CString strTabNo;
	for (int i = ws.iStart; i != ws.iEnd; i += ws.iGap)
	{
		strTabNo.Format(_T("tab(%d)"), i);
		strData += strTabNo;
		strData += (ws.pTab[i].m_bCarrierLoadCompleted) ? "1" : "0";
		strData += (ws.pTab[i].m_bMounterLoadCompleted) ? "1" : "0";
		strData += (ws.pTab[i].m_bTabPrealignCompleted) ? "1" : "0";
		strData += (ws.pTab[i].m_bInspectionCompleted) ? "1" : "0";
		strData += (ws.pTab[i].m_bMountCompleted) ? "1" : "0";
		strData += (ws.pTab[i].m_bAfterInspectionCompleted) ? "1" : "0";
		strData += "_";
		strData += (ws.pTab[i].m_bWorkSkip) ? "1" : "0";
		strData += (ws.pTab[i].m_bRemoveTabIC) ? "1" : "0";
		strData += " ";
	}
	int iGroupNo = getGroupNo(eWorkSide);
	strData += m_rgbGetNextPanelTabIC[iGroupNo] ? "1 " : "0 ";
	if (TRUE == m_rgbGetNextPanelTabIC[iGroupNo])
	{
		for (int i = ws.iStart; i != ws.iEnd; i += ws.iGap)
			strData += m_rgbNextPanelTabICLoad[iGroupNo][i] ? "1" : "0";
	}
	return strData;
}

EWorkingSide MProcessData::GetCurrentWorkingSide(int iWorkGroupNo)
{
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		if (DEF_FRONT_GROUP == iWorkGroupNo)
		{
#ifndef DEF_MIRROR_ //SJ_YYK 150907 Add..
			if (TRUE == m_plnkPanelData->m_bUseFlag_Xd)
				return WORKING_SIDE_X_DOWN;
#else
			if (TRUE == m_plnkPanelData->m_bUseFlag_Xu)
				return WORKING_SIDE_X_UP;
#endif
		}
		else
		{
#ifndef DEF_MIRROR_
			if (TRUE == m_plnkPanelData->m_bUseFlag_Xu)
				return WORKING_SIDE_X_DOWN;
#else
			if (TRUE == m_plnkPanelData->m_bUseFlag_Xd)
				return WORKING_SIDE_X_UP;
#endif
		}

		// Xd 변 대응 처리 추가.
	}
	else	// if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (DEF_FRONT_GROUP == iWorkGroupNo)
		{
#ifndef DEF_MIRROR_
			if (TRUE == m_plnkPanelData->m_bUseFlag_Yl)
				return WORKING_SIDE_Y_LEFT;
#else
			if (TRUE == m_plnkPanelData->m_bUseFlag_Yr)
				return WORKING_SIDE_Y_RIGHT;
#endif
		}
		else // if (DEF_REAR_GROUP == iWorkGroupNo)
		{
#ifndef DEF_MIRROR_
			if (TRUE == m_plnkPanelData->m_bUseFlag_Yr)
				return WORKING_SIDE_Y_RIGHT;
#else
			if (TRUE == m_plnkPanelData->m_bUseFlag_Yl)
				return WORKING_SIDE_Y_LEFT;
#endif
		}
	}
	
	return WORKING_SIDE_UNKNOWN;
}

const int MProcessData::getStartTabNo(EWorkingSide eWorkSide)
{
	// jdy2
	//int i = 0;
	int i = -1;
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xu)
			break;

#ifndef DEF_MIRROR_	// Normal Line 인 경우
		for (int i = m_plnkTabData->m_iXuTabQty - 1; i > -1; i--)
#else
		for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
#endif
		{
			if (TRUE == m_plnkTabData->m_bUseXuTab[i])
				return i;
		}
		break;

	case WORKING_SIDE_X_DOWN:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xd)
			break;

#ifndef DEF_MIRROR_	// Normal Line 인 경우
		for (int i = 0; i < m_plnkTabData->m_iXdTabQty; i++)
#else
		for (int i = m_plnkTabData->m_iXdTabQty - 1; i > -1; i--)
#endif
		{
			if (TRUE == m_plnkTabData->m_bUseXdTab[i])
				return i;
		}
		break;

	case WORKING_SIDE_Y_LEFT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yl)
			break;

#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (int i = m_plnkTabData->m_iYlTabQty - 1; i > -1; i--)
#else
		for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
#endif
		{
			if (TRUE == m_plnkTabData->m_bUseYlTab[i])
				return i;
		}
		break;

	case WORKING_SIDE_Y_RIGHT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yr)
			break;

#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
#else
		for (int i = m_plnkTabData->m_iYrTabQty - 1; i > -1; i--)
#endif
		{
			if (TRUE == m_plnkTabData->m_bUseYrTab[i])
				return i;
		}
		break;

	default:
		break;
	}

	return -1;
}

const int MProcessData::getEndTabNo(EWorkingSide eWorkSide)
{
	// jdy2
	//int i = 0;
	int i = -1;
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xu)
			break;

#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
		{
			if (TRUE == m_plnkTabData->m_bUseXuTab[i])
				return i - 1;
		}
#else
		for (int i = m_plnkTabData->m_iXuTabQty - 1; i > -1; i--)
		{
			if (TRUE == m_plnkTabData->m_bUseXuTab[i])
				return i + 1;
		}
#endif
		
		break;

	case WORKING_SIDE_X_DOWN:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xd)
			break;

#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (int i = m_plnkTabData->m_iXdTabQty - 1; i > -1; i--)
		{
			if (TRUE == m_plnkTabData->m_bUseXuTab[i])
				return i + 1;
		}
#else
		for (int i = 0; i < m_plnkTabData->m_iXdTabQty; i++)
		{
			if (TRUE == m_plnkTabData->m_bUseXuTab[i])
				return i - 1;
		}
#endif
		break;

	case WORKING_SIDE_Y_LEFT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yl)
			break;

#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
		{
			if (TRUE == m_plnkTabData->m_bUseYlTab[i])
				return i - 1;
		}
#else
		for (int i = m_plnkTabData->m_iYlTabQty - 1; i > -1; i--)
		{
			if (TRUE == m_plnkTabData->m_bUseYlTab[i])
				return i + 1;
		}
#endif
		break;

	case WORKING_SIDE_Y_RIGHT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yr)
			break;

#ifndef DEF_MIRROR_		// Normal Line 인 경우
		for (int i = m_plnkTabData->m_iYrTabQty - 1; i > -1; i--)
		{
			if (TRUE == m_plnkTabData->m_bUseYrTab[i])
				return i + 1;
		}
#else
		for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
		{
			if (TRUE == m_plnkTabData->m_bUseYrTab[i])
				return i - 1;
		}
#endif
		break;

	default:
		break;
	}

	return -1;
}

const int MProcessData::GetStartTabNo(EWorkingSide eWorkSide)
{
	return m_rgiStartTabNo[eWorkSide];
}

const int MProcessData::calculateInspectorNo(EWorkingSide eWorkSide, int iTabNo)
{
	int iGroupStartNo = m_rgiStartTabNo[eWorkSide];
	int iGroupEndNo = m_rgiEndTabNo[eWorkSide];
	int iGap = 0;
	if (m_rgiStartTabNo[eWorkSide] > m_rgiEndTabNo[eWorkSide])
		iGap = -1;
	else if (m_rgiStartTabNo[eWorkSide] < m_rgiEndTabNo[eWorkSide])
		iGap = 1;

	if (abs(m_rgiStartTabNo[eWorkSide] - iTabNo) < DEF_MAX_WORKER_PER_GROUP)
	{
		if (abs(m_rgiStartTabNo[eWorkSide] - m_rgiEndTabNo[eWorkSide]) >= DEF_MAX_WORKER_PER_GROUP)
			iGroupEndNo = m_rgiStartTabNo[eWorkSide] + iGap * DEF_MAX_WORKER_PER_GROUP;
	}

	int iInspectorNo = -1;
	if (WORKING_SIDE_X_UP == eWorkSide || WORKING_SIDE_X_DOWN == eWorkSide)
	{
		iInspectorNo = DEF_INSPECTION_CARRIER_2;
		for (int l = iGroupEndNo; l != iTabNo; l -= iGap)
			iInspectorNo = (iInspectorNo + 1) % DEF_MAX_INSPECTION_CAM_PER_GROUP;
	}
	// 20081008 jdy
	else
	{
		int iGroupNo = getGroupNo(eWorkSide);

		int iTabQty = m_plnkTabData->m_iYlTabQty;
		if (WORKING_SIDE_Y_RIGHT == eWorkSide)
			iTabQty = m_plnkTabData->m_iYrTabQty;
		
		if (TRUE == m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[iGroupNo]
			|| iTabQty > DEF_MAX_INSPECTION_CAM_PER_GROUP + 1)	// 작업 Tab 이 5개 이상인 경우.
		{
			int iInspectorWorkTabNoGap = DEF_INSPECT_TAB_GAP;
			iInspectorNo = static_cast<int>(iTabNo / iInspectorWorkTabNoGap) + DEF_MAX_INSPECTION_CAM_PER_GROUP * iGroupNo;
			
			// 단, dMinPitch < DEF_CAMERACARRIER_SAFE_MARGIN 이고, 작업 Tab 이 3개 이하인 경우
			// 가운데 Inspection Camera 가 피신할 수 있는 위치가 Define 되어 있지 않으므로 생산 시도 시 충돌 발생.
			// 향후 이런 모델이 등장할 경우 추가적인 S/W 수정 필요.
		}
		else if (iTabQty <= DEF_MAX_INSPECTION_CAM_PER_GROUP)	// 작업 Tab 이 3개 이하인 경우.
		{
			iInspectorNo = iTabNo + DEF_MAX_INSPECTION_CAM_PER_GROUP * iGroupNo;
		}
		else // if (iTabQty == DEF_MAX_INSPECTION_CAM_PER_GROUP + 1)	// 4
		{
			if (iTabNo < 2)
				iInspectorNo = DEF_MAX_INSPECTION_CAM_PER_GROUP * iGroupNo;
			else
				iInspectorNo = (iTabNo - 1) + DEF_MAX_INSPECTION_CAM_PER_GROUP * iGroupNo;
		}
	}

	return iInspectorNo;
}

int MProcessData::GetPairTabAlignWorkTabMounterNo(EWorkingSide eWorkSide, int iTabNo)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);
	return ws.pTab[iTabNo].m_iPairWorkTabMounterNo;
}

const int MProcessData::calculatePairTabMounterNo(EWorkingSide eWorkSide, int iRefTabMounterNo)
{
	if (-1 == iRefTabMounterNo)
		return DEF_NONE_TABMOUNTER;

	int iGroupNo = getGroupNo(eWorkSide);
	BOOL rgbTabAlignerUsage[DEF_MAX_TABALIGNER_PER_GROUP];

	int iSum = 0;
	for (int i = 0; i < DEF_MAX_TABALIGNER_PER_GROUP; i++)
	{
		rgbTabAlignerUsage[i] = m_plnkSystemData->m_rgbUseTabAligner[i + iGroupNo * DEF_MAX_TABALIGNER_PER_GROUP];
		iSum += !!rgbTabAlignerUsage[i];
	}

	if (2 == iSum)	// 해당 작업부에 있는 TabAligner 를 모두 사용하는 경우.
	{
		// 확인 필요 : Error 처리 - 나중에 DEF_MAX_TABALIGNER_PER_GROUP 가 2보다 큰 값을 가질 경우 ...
		
		if (iRefTabMounterNo % DEF_MAX_TABALIGNER_PER_GROUP == 0)	// 2 == DEF_MAX_TABALIGNER_PER_GROUP
			return iRefTabMounterNo + 1;
		else // if (iRefTabMounterNo % DEF_MAX_TABALIGNER_PER_GROUP == 1)
			return iRefTabMounterNo - 1;
	}
	return DEF_NONE_TABMOUNTER;
}

const int MProcessData::calculateWorkTabAlignerNo(EWorkingSide eWorkSide, int iRefTabMounterNo)
{
	if (-1 == iRefTabMounterNo)
		return DEF_NONE_TABALIGNER;

	int iGroupNo = getGroupNo(eWorkSide);
	BOOL rgbTabAlignerUsage[DEF_MAX_TABALIGNER_PER_GROUP];

	int iSum = 0;
	for (int i = 0; i < DEF_MAX_TABALIGNER_PER_GROUP; i++)
	{
		rgbTabAlignerUsage[i] = m_plnkSystemData->m_rgbUseTabAligner[i + iGroupNo * DEF_MAX_TABALIGNER_PER_GROUP];
		iSum += rgbTabAlignerUsage[i];
	}

	if (2 == iSum)	// 해당 작업부에 있는 TabAligner 를 모두 사용하는 경우.
	{
		// DEF_MAX_TABALIGNER_PER_GROUP == 2 이므로
		// return value 는 DEF_TABALIGNER1_PER_GROUP(0) or DEF_TABALIGNER2_PER_GROUP(1).
		return iRefTabMounterNo % DEF_MAX_TABALIGNER_PER_GROUP;
	}
	else if (1 == iSum) // 해당 작업부에 있는 TabAligner 중 하나만 사용하는 경우.
	{
		for (int i = 0; i < DEF_MAX_TABALIGNER_PER_GROUP; i++)
		{
			if (TRUE == rgbTabAlignerUsage[i])
				return i;	// return value 는 DEF_TABALIGNER1_PER_GROUP(0) or DEF_TABALIGNER2_PER_GROUP(1).
		}
	}

	// 확인 필요 : 아래 값으로 return 되는 경우는 있을 수 없다....
	return DEF_NONE_TABALIGNER;
}

const int MProcessData::GetWorkTabAlignerID(EWorkingSide eWorkSide, int iTabNo)
{
	SWorkSideData ws = getWorkSideData(eWorkSide);
	return ws.pTab[iTabNo].m_iWorkTabAlignerNo;
}

// jdy2
const BOOL MProcessData::isTabPitchSmallerThanCameraCarrierSafeMargin(int iTabQty, const double rgdTabPitch[DEF_MAX_TAB])
{
	if (SYSTEM_TYPE_GATE != m_plnkSystemData->m_eSystemType)
		return FALSE;

	ASSERT(iTabQty > 0 && iTabQty < DEF_MAX_TAB && rgdTabPitch != NULL);

	// Tab Pitch 간 최소 거리 측정
	double dMinPitch = 1000000.0;	// 임의의 큰 값.
	for (int k = 1; k < iTabQty; k++)
		dMinPitch = (rgdTabPitch[k] < dMinPitch) ? rgdTabPitch[k] : dMinPitch;
	
	ASSERT(dMinPitch > 0.0);
	
	// Tab Pitch 간 최소 거리가 DEF_CAMERACARRIER_SAFE_MARGIN 보다 작은지 여부 Return
	return (dMinPitch < DEF_CAMERACARRIER_SAFE_MARGIN);
}

BOOL MProcessData::UseTwoInspectionCamera()
{
	//2009.10.06 CYJ
//	if (TRUE == m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[0])
	if ((TRUE == m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[DEF_FRONT_GROUP])
	|| (TRUE == m_rgbIsTabPitchSmallerThanCameraCarrierSafeMargin[DEF_REAR_GROUP]))
		return TRUE;
	return FALSE;
}

/* jdy sesl : 불필요 삭제.
// jdy2
const BOOL MProcessData::ProcessWorkExists()
{
	if (PASS_RUN_MODE == m_plnkSystemData->m_eRunMode
		|| DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		return TRUE;
	}

	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xu)
			return FALSE;

		for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
		{
			if (TRUE == m_plnkTabData->m_bUseXuTab[i])
				return TRUE;
		}
	}
	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yl)
		{
			for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
			{
				if (TRUE == m_plnkTabData->m_bUseYlTab[i])
					return TRUE;
			}
		}
		
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yr)
		{
			for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
			{
				if (TRUE == m_plnkTabData->m_bUseYrTab[i])
					return TRUE;
			}
		}
	}

	return FALSE;
}
*/

//###########################################################################
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <class T>
MVarList<T>::MVarList()
{
	Clear();
}

template <class T>
MVarList<T>::MVarList(MVarList& list)
{
	Clear();
	T info;
	int count = list.GetCount();
	for (int i = 0; i < count; i++)
	{
		info = list.GetInfo(i);
		Append(info);
	}
	m_iWorkGroup = list.m_iWorkGroup;
	m_iKeyValue = list.m_iKeyValue;
	m_bReservedSupply = list.m_bReservedSupply;
	m_bLastGroup = list.m_bLastGroup;
}

template <class T>
MVarList<T>::~MVarList()
{
	Clear();
}

template <class T>
void MVarList<T>::Clear()
{
	m_list.RemoveAll();
	m_iWorkGroup = -1;
	m_iKeyValue = -1;
	m_bReservedSupply = FALSE;
	m_bLastGroup = FALSE;
}

template <class T>
int MVarList<T>::GetCount()
{
	int cnt = m_list.GetCount();
	return cnt;
}

template <class T>
void MVarList<T>::AddHead(T& info)
{
	m_list.AddHead(info);
}

template <class T>
void MVarList<T>::Append(T& info)
{
	m_list.AddTail(info);
}

template <class T>
void MVarList<T>::Delete(int no)
{
	if (no < 0 || no >= GetCount())
		return;
	POSITION pos = m_list.FindIndex(no);
	if (pos == NULL)
	{
		return;
	}
	m_list.RemoveAt(pos);
}

template <class T>
T MVarList<T>::GetInfo(int no)
{
	T info;	
	if (no < 0 || no >= GetCount())
		return info;
	POSITION pos = m_list.FindIndex(no);
	if (pos == NULL)
	{
		return info;
	}
	info = m_list.GetAt(pos);
	return info;
}

template <class T>
void MVarList<T>::SetInfo(int no, T& info)
{
	if (no < 0 || no >= GetCount())
		return;
	POSITION pos = m_list.FindIndex(no);
	if (pos == NULL)
	{
		return;
	}
	m_list.SetAt(pos, info);
}

template <class T>
int MVarList<T>::GetWorkGroup()
{
	return m_iWorkGroup;
}

template <class T>
void MVarList<T>::SetWorkGroup(int iGroup)
{
	m_iWorkGroup = iGroup;
}

template <class T>
int MVarList<T>::GetKeyValue()
{
	return m_iKeyValue;
}

template <class T>
void MVarList<T>::SetKeyValue(int iValue)
{
	m_iKeyValue = iValue;
}

template <class T>
MVarList<T>& MVarList<T>::operator=(MVarList& list)
{
	if (this == &list) return *this;
	Clear();
	T info;
	int count = list.GetCount();
	for (int i = 0; i < count; i++)
	{
		info = list.GetInfo(i);
		Append(info);
	}
	m_iWorkGroup = list.m_iWorkGroup;
	m_iKeyValue = list.m_iKeyValue;
	m_bReservedSupply = list.m_bReservedSupply;
	return *this;
}

template <class T>
BOOL MVarList<T>::GetReservedSupply()
{
	return m_bReservedSupply;
}

template <class T>
void MVarList<T>::SetReservedSupply(BOOL bReserve)
{
	m_bReservedSupply = bReserve;
}

template <class T>
void MVarList<T>::SetLastGroup(BOOL bLast)
{
	m_bLastGroup = bLast;
}

template <class T>
BOOL MVarList<T>::GetLastGroup()
{
	return m_bLastGroup;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MJobInfo::MJobInfo()
{
	Init();
}

MJobInfo::MJobInfo(MJobInfo& info)
{
	m_nWorkedGroup = info.m_nWorkedGroup;
	m_nWorkedToolID = info.m_nWorkedToolID;
	m_nWorkedInspectionID = info.m_nWorkedInspectionID;
	m_bToolLoadComplete = info.m_bToolLoadComplete;
	m_bInspectionComplete = info.m_bInspectionComplete;
	m_bPressComplete = info.m_bPressComplete;
	m_bSkip = info.m_bSkip;

	m_OffsetPrealign = info.m_OffsetPrealign;
	m_OffsetInspection = info.m_OffsetInspection;
}

MJobInfo::~MJobInfo()
{
}

MJobInfo& MJobInfo::operator=(MJobInfo& info)
{
	if (this == &info) return *this;
	m_nWorkedGroup = info.m_nWorkedGroup;
	m_nWorkedToolID = info.m_nWorkedToolID;
	m_nWorkedInspectionID = info.m_nWorkedInspectionID;
	m_bToolLoadComplete = info.m_bToolLoadComplete;
	m_bInspectionComplete = info.m_bInspectionComplete;
	m_bPressComplete = info.m_bPressComplete;
	m_bSkip = info.m_bSkip;

	m_OffsetPrealign = info.m_OffsetPrealign;
	m_OffsetInspection = info.m_OffsetInspection;

	return *this;
}

int MJobInfo::Init()
{
	m_nWorkedGroup = 0;
	m_nWorkedToolID = 0;
	m_nWorkedInspectionID = 0;
	m_bToolLoadComplete = FALSE;
	m_bInspectionComplete = FALSE;
	m_bPressComplete = FALSE;
	m_bSkip = FALSE;

	m_OffsetPrealign.Init(0.0, 0.0, 0.0);
	m_OffsetInspection.Init(0.0, 0.0, 0.0);
	return 0;
}

void MJobInfo::SetPressComplete()
{
	m_bToolLoadComplete = TRUE;
	m_bInspectionComplete = TRUE;
	m_bPressComplete = TRUE;
	m_bSkip = FALSE;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MTabInfo::MTabInfo()
{
	Init();
}

MTabInfo::MTabInfo(MTabInfo& info)
{
	m_nTabNo = info.m_nTabNo;
	m_nRealTabNo = info.m_nRealTabNo;
	m_dTabPitch = info.m_dTabPitch;
	m_dModelOffset = info.m_dModelOffset;
	m_jobInfo = info.m_jobInfo;
//	m_dTabPitch = info.m_dTabPitch;
//	m_nDefaultTabCarrierNo = info.m_nDefaultTabCarrierNo;
}

MTabInfo::~MTabInfo()
{
}

MTabInfo& MTabInfo::operator=(MTabInfo& info)
{
	if (this == &info) return *this;
	m_nTabNo = info.m_nTabNo;
	m_nRealTabNo = info.m_nRealTabNo;
	m_dTabPitch = info.m_dTabPitch;
	m_dModelOffset = info.m_dModelOffset;
	m_jobInfo = info.m_jobInfo;
//	m_nDefaultTabCarrierNo = info.m_nDefaultTabCarrierNo;
//	m_dTabPitch = info.m_dTabPitch;
	return *this;
}


void MTabInfo::Init()
{
	m_nTabNo = -1;
	m_nRealTabNo = -1;
	m_dTabPitch = 0.0;
	m_dModelOffset = 0.0;
	m_jobInfo.Init();
//	m_nDefaultTabCarrierNo = -1;
//	m_dTabPitch = 0.0;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MListWorkTab::MListWorkTab()
{
	m_nTotalWorkTab = 0;
	m_nLastKeyValue = 0;
}

MListWorkTab::~MListWorkTab()
{
	Clear();
}

void MListWorkTab::Clear()
{
	m_nTotalWorkTab = 0;
	m_nLastKeyValue = 0;
	POSITION pos;
	MVarList<MTabInfo> *plist;
	int count = GetCount();
	for (int i = 0; i < count; i++)
	{
		pos = m_list.FindIndex(i);
		plist = m_list.GetAt(pos);
		plist->Clear();
		delete plist;
	}
	m_list.RemoveAll();
}

int MListWorkTab::GetCount()
{
	int cnt = m_list.GetCount();
	return cnt;
}

int MListWorkTab::GetTotalWorkTabCount()
{
	return m_nTotalWorkTab;
}

void MListWorkTab::AddHead(MVarList<MTabInfo>* pl)
{
	m_nTotalWorkTab += pl->GetCount();
	pl->SetKeyValue(m_nLastKeyValue++);
	m_list.AddHead(pl);
}

void MListWorkTab::Append(MVarList<MTabInfo>* pl)
{
	m_nTotalWorkTab += pl->GetCount();
	pl->SetKeyValue(m_nLastKeyValue++);
	m_list.AddTail(pl);
}

void MListWorkTab::Insert(int no, MVarList<MTabInfo>* pl)
{
	if (GetCount() == 0)
	{
		Append(pl);
		return;
	}
	if (no < 0 || no >= GetCount())
		return;
	POSITION pos = m_list.FindIndex(no);
	if (pos == NULL)
	{
		return;
	}
	pl->SetKeyValue(m_nLastKeyValue++);
	m_list.InsertBefore(pos, pl);
	m_nTotalWorkTab += pl->GetCount();
}

void MListWorkTab::Delete(int no)
{
	if (no < 0 || no >= GetCount())
		return;
	POSITION pos = m_list.FindIndex(no);
	if (pos == NULL)
	{
		return;
	}
	MVarList<MTabInfo>* plist = m_list.GetAt(pos);
	m_nTotalWorkTab -= plist->GetCount();
	m_list.RemoveAt(pos);
	plist->Clear();
	delete plist;
}

MVarList<MTabInfo>* MListWorkTab::GetSubList(int no)
{
	if (no < 0 || no >= GetCount())
		return NULL;
	POSITION pos = m_list.FindIndex(no);
	if (pos == NULL)
	{
		return NULL;
	}
	return m_list.GetAt(pos);
}

//MListWorkTab& MListWorkTab::operator=(MListWorkTab& list)
MListWorkTab& MListWorkTab::CopyData(MListWorkTab& list)
{
	if (this == &list) return *this;
	Clear();
	MVarList<MTabInfo> *pnode;
	int cnt = list.GetCount();
	for (int i = 0; i < cnt; i++)
	{
		pnode = new MVarList<MTabInfo>;
		*pnode = *list.GetSubList(i);
		Append(pnode);
	}
	return *this;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MInspectCarrierInfo::MInspectCarrierInfo()
{
	Init();
}

MInspectCarrierInfo::~MInspectCarrierInfo()
{
}

MInspectCarrierInfo& MInspectCarrierInfo::operator=(MInspectCarrierInfo& info)
{
	if (this == &info)	return *this;
	m_nWorkTabNo = info.m_nWorkTabNo;
	m_nRealTabNo = info.m_nRealTabNo;
	m_dInspectPos = info.m_dInspectPos;
	m_nCarrierNo = info.m_nCarrierNo;
	return *this;
}

void MInspectCarrierInfo::Init()
{
	m_nWorkTabNo = -1;
	m_nRealTabNo = -1;
	m_dInspectPos = 0.0;
	m_bWorkSkipAfterInspect = FALSE;
	m_nCarrierNo = 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*=========================================================
[CAutoMainForm::OnUnitInitial()에서 초기화]
	[WorkSchedule초기화]
	투입된 패널이 없거나 PanelAligner에만 Panel이 있고 Mounter작업 완료한 상태이면
		- ResetWorkSchedule()로 Clear시킴.(m_bInitializedWorkData = FALSE)
	PanelAligner에 Panel이 있을경우
		처음부터 작업
			- ResetWorkSchedule()로 Clear시킴.(m_bInitializedWorkData = FALSE)
		현재 작업 유지
			- GenerateWorkScheduleByPanel()로 Press완료 상태에 따라서 작업상태 변경.
		패널 Pass
			- ResetWorkSchedule()로 Clear시킴.(m_bInitializedWorkData = FALSE)
	다른 Unit에 Panel이 있을경우 
		- ResetWorkSchedule()로 Clear시킴.(m_bInitializedWorkData = FALSE)
	[WorkSchedule Update]
	Mounter흡착상태, Carrier흡착상태에 따라서 Update시킴.(m_bInitializedWorkData = TRUE)
	

=========================================================*/

int MWorkSchedule::m_nInstanceCnt = 0;
BOOL MWorkSchedule::m_bWorkPanelExist = FALSE;

MWorkSchedule::MWorkSchedule(const EWorkingSide eWSide, const int iWorkGroup, const int iScheduleNo)
	:m_eWorkSide(eWSide), m_iWorkGroup(iWorkGroup), m_iWorkScheduleNo(iScheduleNo)
{
	m_nInstanceCnt++;
	ASSERT(m_nInstanceCnt <= DEF_MAX_WORK_SIDE);

	m_bUseWorkSide = FALSE;
	m_bWorkPanelExist = FALSE;

	m_OrgListWorkTab.Clear();
	m_CurWorkSchedule.Clear();
	
	int i = 0;

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		m_pCurrentSupplyList[i] = NULL;
		m_pFirstSupplyList[i] = NULL;
		m_bFeederReady[i] = FALSE;
	}

	m_pCurrentWorkList = NULL;

	for (int i = 0; i < DEF_MAX_SUB_LIST; i++)
		m_bCompleteSubWorkList[i] = FALSE;

	m_bExistWorkList = FALSE;
	m_bCompleteCurWorkList = FALSE;
	m_bExistSupplyList[DEF_FRONT_GROUP] = FALSE;
	m_bExistSupplyList[DEF_REAR_GROUP] = FALSE;
	m_bLock = FALSE;
}

MWorkSchedule::~MWorkSchedule()
{
	m_OrgListWorkTab.Clear();
	m_CurWorkSchedule.Clear();
}

void MWorkSchedule::AssignData(SWorkScheduleRefData RefData)
{
	m_plnkSystemData	= RefData.m_plnkSystemData;
	m_plnkPanelData		= RefData.m_plnkPanelData;
	m_plnkTabData		= RefData.m_plnkTabData;
	m_plnkPreBonderData = RefData.m_plnkPreBonderData;
	m_iFirstTabNo		= RefData.m_iFirstTabNo;
	m_iLastTabNo		= RefData.m_iLastTabNo;
}

void MWorkSchedule::AssignProcess(SWorkScheduleRefProcess listRefTrs)
{
	m_RefProcess.m_plnkTrsPanelAligner = listRefTrs.m_plnkTrsPanelAligner;
	m_RefProcess.m_plnkTrsTabFeeder[DEF_TABFEEDER1] = listRefTrs.m_plnkTrsTabFeeder[DEF_TABFEEDER1];
	m_RefProcess.m_plnkTrsTabFeeder[DEF_TABFEEDER2] = listRefTrs.m_plnkTrsTabFeeder[DEF_TABFEEDER2];
}

BOOL MWorkSchedule::GetUseWorkSide()
{
	return m_bUseWorkSide;
}

int MWorkSchedule::ChangeModel()
{
	SetLock();

	m_OrgListWorkTab.Clear();
	m_bUseWorkSide = FALSE;
	
	int i = 0, j = 0;
	int		nTabQty;
	BOOL	bUseFlag[DEF_MAX_TAB] =  { FALSE, };
	double	dTabPitch[DEF_MAX_TAB] = { 0.0, };
	double	dTabPitchReverse[DEF_MAX_TAB] = { 0.0, };
	double	dTempSum = 0.0;
	double	dMarkDistance = 0.0;

	switch (m_eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (m_plnkPanelData->m_bUseFlag_Xu)
		{
			m_bUseWorkSide = TRUE;
			nTabQty = m_plnkTabData->m_iXuTabQty;
			dMarkDistance = m_plnkPanelData->m_dDistance_FiduMark_Xu;
			for (int i = 0; i < nTabQty; i++)
			{
				bUseFlag[i] = m_plnkTabData->m_bUseXuTab[i];
				dTabPitch[i] = m_plnkTabData->m_dXuTabPitch[i];
				dTempSum += dTabPitch[i];
				if (i != 0)
					dTabPitchReverse[i] = m_plnkTabData->m_dXuTabPitch[nTabQty-i];
			}
			dTabPitchReverse[0] = dMarkDistance - dTempSum;
		}
		break;
	case WORKING_SIDE_X_DOWN:
		if (m_plnkPanelData->m_bUseFlag_Xd)
		{
			m_bUseWorkSide = TRUE;
			nTabQty = m_plnkTabData->m_iXdTabQty;
			dMarkDistance = m_plnkPanelData->m_dDistance_FiduMark_Xd;
			for (int i = 0; i < nTabQty; i++)
			{
				bUseFlag[i] = m_plnkTabData->m_bUseXdTab[i];
				dTabPitch[i] = m_plnkTabData->m_dXdTabPitch[i];
				dTempSum += dTabPitch[i];
				if (i != 0)
					dTabPitchReverse[i] = m_plnkTabData->m_dXdTabPitch[nTabQty-i];
			}
			dTabPitchReverse[0] = dMarkDistance - dTempSum;
		}
		break;
	case WORKING_SIDE_Y_LEFT:
		if (m_plnkPanelData->m_bUseFlag_Yl)
		{
			m_bUseWorkSide = TRUE;
			nTabQty = m_plnkTabData->m_iYlTabQty;
			dMarkDistance = m_plnkPanelData->m_dDistance_FiduMark_Yl;
			for (int i = 0; i < nTabQty; i++)
			{
				bUseFlag[i] = m_plnkTabData->m_bUseYlTab[i];
				dTabPitch[i] = m_plnkTabData->m_dYlTabPitch[i];
				dTempSum += dTabPitch[i];
				if (i != 0)
					dTabPitchReverse[i] = m_plnkTabData->m_dYlTabPitch[nTabQty-i];
			}
			dTabPitchReverse[0] = dMarkDistance - dTempSum;
		}
		break;
	case WORKING_SIDE_Y_RIGHT:
		if (m_plnkPanelData->m_bUseFlag_Yr)
		{
			m_bUseWorkSide = TRUE;
			nTabQty = m_plnkTabData->m_iYrTabQty;
			dMarkDistance = m_plnkPanelData->m_dDistance_FiduMark_Yr;
			for (int i = 0; i < nTabQty; i++)
			{
				bUseFlag[i] = m_plnkTabData->m_bUseYrTab[i];
				dTabPitch[i] = m_plnkTabData->m_dYrTabPitch[i];
				dTempSum += dTabPitch[i];
				if (i != 0)
					dTabPitchReverse[i] = m_plnkTabData->m_dYrTabPitch[nTabQty-i];
			}
			dTabPitchReverse[0] = dMarkDistance - dTempSum;
		}
		break;
	}

	if (FALSE == m_bUseWorkSide)
	{
		SetUnlock();
		return 0;
	}

	MTabInfo tabinfo;
	MVarList<MTabInfo>* plistnew = NULL;

	int nUsedCnt = 0;
	int nWorkGroupCnt = 0;
	int nLastTabNo = 0;
	for (int i = 0 ; i < nTabQty; i++)
	{
		if (bUseFlag[i])
		{
			nUsedCnt++;
			nLastTabNo = i;
		}
	}

	BOOL IsCreatedTab[DEF_MAX_TAB];
	memset(IsCreatedTab, 0, sizeof(BOOL)*DEF_MAX_TAB);
	
//	int iMethod = 1;//0=4Tab,4Tab,2Tab 1=2Tab,4Tab,4Tab
	int iMethod = 0;
	//170923 JSh 임시 65" 전용
	if(nTabQty == 6)
		iMethod = 2;

	double dMaxGap = DEF_CAMERACARRIER_SAFE_MARGIN;
	int nCreatedTabCnt = 0;
	int ncnt = 0;
	while(1)
	{
		double dSum = 0.0;			//마지막으로 만들어진 Tab부터 더한 거리
		double dTotalSum = 0.0;		//첫번째 Tab부터 더한 거리
		plistnew = new MVarList<MTabInfo>;
		if (iMethod == 0)
		{
			for (int i = 0; i < nTabQty; i++)
			{
				dSum += dTabPitch[i];
				dTotalSum += dTabPitch[i];
				if (!bUseFlag[i] || IsCreatedTab[i])
					continue;
				if (dSum >= dMaxGap || plistnew->GetCount() == 0)	//첫번째 Tab은 무조건 작업할수 있으므로...
				{
					tabinfo.m_nTabNo = i;
					tabinfo.m_nRealTabNo = i + m_iFirstTabNo;
					tabinfo.m_dTabPitch = dTotalSum;
					//Gate : Fiducial Mark중심에서 위치
	#	ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Add.
					tabinfo.m_dModelOffset = -tabinfo.m_dTabPitch;
	#	else
					tabinfo.m_dModelOffset = tabinfo.m_dTabPitch;
	#	endif
					plistnew->Append(tabinfo);
					IsCreatedTab[i] = TRUE;
					nCreatedTabCnt++;
					dSum = 0;
					ncnt = plistnew->GetCount();
					if (ncnt >= DEF_MAX_WORK_PER_UNIT)
						break;
				}
			}
			if (plistnew->GetCount() > 0)
				m_OrgListWorkTab.Append(plistnew);
		}
		else if (iMethod == 1)	//iMethod == 1
		{
			for (int i = nTabQty-1; i >= 0; i--)
			{
				dSum += dTabPitchReverse[nTabQty-1-i];
				dTotalSum += dTabPitchReverse[nTabQty-1-i];
				if (!bUseFlag[i] || IsCreatedTab[i])
					continue;
				if (dSum >= dMaxGap || plistnew->GetCount() == 0)	//첫번째 Tab은 무조건 작업할수 있으므로...
				{
					tabinfo.m_nTabNo = i;
					tabinfo.m_nRealTabNo = i+m_iFirstTabNo;
					tabinfo.m_dTabPitch = dMarkDistance - dTotalSum;
					
					//Gate : Fiducial Mark중심에서 위치
	#	ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Add.
					tabinfo.m_dModelOffset = -tabinfo.m_dTabPitch;
	#	else
					tabinfo.m_dModelOffset = tabinfo.m_dTabPitch;
	#	endif
					plistnew->AddHead(tabinfo);
					IsCreatedTab[i] = TRUE;
					nCreatedTabCnt++;
					dSum = 0;
					ncnt = plistnew->GetCount();
					if (ncnt >= DEF_MAX_WORK_PER_UNIT)
						break;
				}
			}
			if (plistnew->GetCount() > 0)
				m_OrgListWorkTab.AddHead(plistnew);
		}
		//170923 JSh.s
		else
		{
			dMaxGap = dTabPitch[1] * 2 +10;
			for (int i = 0; i < nTabQty; i++)
			{
				dSum += dTabPitch[i];
				dTotalSum += dTabPitch[i];
				if (!bUseFlag[i] || IsCreatedTab[i])
					continue;
				if (dSum >= dMaxGap || plistnew->GetCount() == 0)	//첫번째 Tab은 무조건 작업할수 있으므로...
				{
					tabinfo.m_nTabNo = i;
					tabinfo.m_nRealTabNo = i + m_iFirstTabNo;
					tabinfo.m_dTabPitch = dTotalSum;
					//Gate : Fiducial Mark중심에서 위치
#	ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Add.
					tabinfo.m_dModelOffset = -tabinfo.m_dTabPitch;
#	else
					tabinfo.m_dModelOffset = tabinfo.m_dTabPitch;
#	endif
					plistnew->Append(tabinfo);
					IsCreatedTab[i] = TRUE;
					nCreatedTabCnt++;
					dSum = 0;
					ncnt = plistnew->GetCount();
					if (ncnt >= DEF_MAX_WORK_PER_UNIT)
						break;
				}
			}
			if (plistnew->GetCount() > 0)
				m_OrgListWorkTab.Append(plistnew);
		}
		//170923 JSh.e

		if (nCreatedTabCnt >= nUsedCnt)
			break;
	}

#ifdef DEF_SOURCE_SYSTEM
	nWorkGroupCnt = nUsedCnt / DEF_MAX_WORK_PER_UNIT;
	int irest = nUsedCnt - nWorkGroupCnt*DEF_MAX_WORK_PER_UNIT;
	if (irest > 0) nWorkGroupCnt++;

//		WorkTab이 9개 이상일 경우 다음 case 대로 workschedule을 작성한다.
//		case 9:		//0,3,6			1,4,7			2,5,8
//		case 10:	//0,3,6,9		1,4,7			2,5,8
//		case 11:	//0,3,6,9		1,4,7,10		2,5,8
//		case 12:	//0,3,6,9		1,4,7,10		2,5,8,11
//		case 13:	//0,4,8,12		1,5,9			2,6,10		3,7,11
//		case 14:	//0,4,8,12		1,5,9,13		2,6,10		3,7,11
//		case 15:	//0,4,8,12		1,5,9,13		2,6,10,14	3,7,11
//		case 16:	//0,4,8,12		1,5,9,13		2,6,10,14	3,7,11,15		
	if (nWorkGroupCnt >= 3)
	{
		int nstartno = 0;
		tabinfo.Init();
		m_OrgListWorkTab.Clear();
		for (int i=0; i<nWorkGroupCnt; i++)
		{
			plistnew = new MVarList<MTabInfo>;
			for (int j=nstartno; j<nUsedCnt; j+=nWorkGroupCnt)
			{
				tabinfo.m_nTabNo = j;
				tabinfo.m_nRealTabNo = j+m_iFirstTabNo;
				tabinfo.m_dTabPitch = GetPitchSum(j);
				//@Fiducial Mark중심에서 위치
#	ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Add.
				tabinfo.m_dModelOffset = -tabinfo.m_dTabPitch;
#else
				tabinfo.m_dModelOffset = tabinfo.m_dTabPitch;
#endif
				plistnew->Append(tabinfo);
			}
			m_OrgListWorkTab.Append(plistnew);
			nstartno++;
		}
	}
#endif
	//_______________________

	for (int i=0; i<DEF_MAX_GROUP; i++)
	{
		m_pFirstSupplyList[i] = NULL;
	}

	MVarList<MTabInfo>* ptmplistmount = NULL;
	CString strLog;
	TRACE(_T("=====Total:%d=====\n"), m_OrgListWorkTab.GetTotalWorkTabCount());
	ncnt = m_OrgListWorkTab.GetCount();
	int iOldWorkGroup = DEF_NONE_GROUP;
	int iWorkGroup = DEF_FRONT_GROUP;

	for (int i = 0; i < ncnt; i++)
	{
		iWorkGroup = m_iWorkGroup;
		ptmplistmount = m_OrgListWorkTab.GetSubList(i);
		ptmplistmount->SetWorkGroup(iWorkGroup);
		int listcnt = ptmplistmount->GetCount();
		for (int j = 0; j < listcnt; j++)
		{
			tabinfo = ptmplistmount->GetInfo(j);
//			strLog.Format(_T("=====No:%02d [TabNo:%d, Carrier:%d, Pitch:%.3lf=====]\n"), i, tabinfo.m_nTabNo, tabinfo.m_nDefaultTabCarrierNo, tabinfo.m_dTabPitch);
			strLog.Format(_T("=====No:%02d [WorkGroup:%d, TabNo:%d, RealTabNo:%d, Pitch:%.3lf]=======\n"), i, ptmplistmount->GetWorkGroup(), tabinfo.m_nTabNo, tabinfo.m_nRealTabNo, tabinfo.m_dTabPitch);
			TRACE(strLog);
		}
		if (i <= 1 && iWorkGroup != iOldWorkGroup)
		{
			m_pFirstSupplyList[iWorkGroup] = ptmplistmount;
		}
		iOldWorkGroup = iWorkGroup;
	}

	//First Inspection Carrier List를 구한다--->Align할때 사용
	MVarList<MTabInfo> *plistmount;
	ncnt = m_OrgListWorkTab.GetCount();
	if (ncnt > 0)
	{
		for (int i = 0; i < ncnt; i++)
		{
			plistmount= m_OrgListWorkTab.GetSubList(i);
			if (i == 0)
				m_FirstInspectCarrierList = CreateInspectionCarrierInfo(*plistmount);
			else
				CreateInspectionCarrierInfo(*plistmount);	//For Debugging

//			m_iTabSupplyCnt[i] = plistmount->GetCount();
		}
		TRACE(_T("\n"));
	}

	SetUnlock();

	//For Debugging Test____________________
	GenerateWorkSchedule();
	SetExistWorkPanel(TRUE);

	iWorkGroup = m_iWorkGroup;
	BOOL IsSupplyCarrier[4] = {FALSE,};
	int iWorkToolNo[4] = {-1,-1,-1,-1};
	int iWorkCarrierNo[4] = {-1,-1,-1,-1};
	int iInspectCarrierNo[4] = {-1,-1,-1,-1};
	int iWorkTabMountNo[4] = {-1,-1,-1,-1};
	int iSupplyKeyValue = -1;
	int iWorkKeyValue = -1;
	MTabInfo SupplyTabInfo[4];
	MTabInfo WorkTabInfo[4];
	MInspectCarrierInfo InspectInfo[4];
	BOOL IsLoadComplete = FALSE;
	BOOL IsInspectComplete = FALSE;
	BOOL IsPressComplete = FALSE;
	BOOL IsPressCompleteAll = FALSE;

	int totcnt = m_CurWorkSchedule.GetCount();
	for (int k = 0; k < totcnt; k++)
	{
		PickOut_SupplyWorkList(iWorkGroup);							//MTrsTabMounter에서 공급할 WorkList을 구함
		PickOut_CurrentWorkList();									//MTrsInspectionCamera에서 작업할 WorkList를 구함

		iSupplyKeyValue = GetKeyValue_SupplyWorkList(iWorkGroup);	//MTrsTabMounter에서 공급할 Key값
		iWorkKeyValue = GetKeyValue_CurrentWorkList();				//MTrsInspectionCamera에서 작업할 Key값

		for (int i = 0; i < 4; i++)
		{
			IsSupplyCarrier[i] = IsSupplyTab(iWorkGroup, i);							//MTrsTabCarrier에서 작업할 Carrier인지 판단
			iWorkToolNo[i] = GetCarrierToTool_FromCurrentSupplyList(iWorkGroup, iWorkGroup, i);		//MTrsTabCarrier에서 넘겨줄 TabMounter번호 구함
		}

		for (int i = 0; i < 4; i++)
		{
			iWorkCarrierNo[i] = -1;
			iInspectCarrierNo[i] = -1;
			iWorkTabMountNo[i] = -1;

			//MTrsTabMounter에서 공급할 TabInfo를 구함
			SupplyTabInfo[i] = GetTabInfo_SupplyWorkList(iWorkGroup, i);

			//공급할 Tab이면
			if (SupplyTabInfo[i].m_nTabNo != -1)
			{
				//넘겨받을 TabCarrier번호를 구함
				iWorkCarrierNo[i] = GetCarrierNo_FromCurrentSupplyList(iWorkGroup, SupplyTabInfo[i].m_nTabNo);
				//TabIC Loading후 LoadingComplete set
				SetMounterTabLoadComplete_SupplyWorkList(iWorkGroup, i);
			}

			//MTrsTabMounter에서 현재 작업할 List의 TabIC Loading을 완료했는지 판단
			IsLoadComplete = IsLoadComplete_SupplyList(iSupplyKeyValue);

			//MTrsTabMounter의 Key와 MTrsInspectionCamera의 Key가 일치하는지 판단. 일치하면 TabInfo를 다시 구함
			if (iSupplyKeyValue == iWorkKeyValue)
				WorkTabInfo[i] = GetTabInfo_CurrentWorkList(i);
			
			//MTrsTabMounter와 작업하는 MTrsInspectionCamera번호를 구함
			if (WorkTabInfo[i].m_nTabNo != -1)
			{
				iInspectCarrierNo[i] = GetInspectionNoFromCurrentWorkList(WorkTabInfo[i].m_nTabNo);
			}

			//Inspection Carrier에서 현재 작업할 List의 InspectionInfo를 구함____
			InspectInfo[i] = GetInspectCarrierInfo_FromCurrentWorkList(i);
			if (InspectInfo[i].m_nWorkTabNo != -1)
			{
				//Inspection완료 Set
				iWorkTabMountNo[i] = GetTabMounterNoFromCurrentWorkList(InspectInfo[i].m_nWorkTabNo);
				SetInspectionComplete(InspectInfo[i].m_nWorkTabNo);
			}

			//MTrsTabMounter에서 현재 작업할 List의 Inspection을 완료했는지 판단
			IsInspectComplete = IsInspectionCompleteGroup_CurrentWorkList();
			if (IsInspectComplete)
				break;
		}

		
		//현재 작업할 List의 Press완료 판단
		for (int i = 0; i < 4; i++)
		{
			//TabMounter_____
			if (WorkTabInfo[i].m_nTabNo != -1)
			{
				SetPressComplete(WorkTabInfo[i].m_nTabNo);
			}

			//TabMounter & Inspection Carrier_____
			IsPressComplete = IsPressComplete_CurrentWorkList();
		}

		//모든 List을 Press완료 판단
		IsPressCompleteAll = IsPressComplete_AllWorkList();
		if (IsPressCompleteAll)
			break;
	}


	SetExistWorkPanel(FALSE);
	ClearWorkSchedule();
	//____________________________

	return 0;
}

void MWorkSchedule::GetOrgWorkList(MListWorkTab *pList)
{
	pList->CopyData(m_OrgListWorkTab);
}

double MWorkSchedule::GetPitchSum(int nTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	double dsum = 0.0;
	for (int i=0; i<=nTabNo; i++)
	{
		switch (m_eWorkSide)
		{
		case WORKING_SIDE_X_UP:
			dsum += m_plnkTabData->m_dXuTabPitch[i];
			break;
		case WORKING_SIDE_X_DOWN:
			dsum += m_plnkTabData->m_dXdTabPitch[i];
			break;
		case WORKING_SIDE_Y_LEFT:
			dsum += m_plnkTabData->m_dYlTabPitch[i];
			break;
		case WORKING_SIDE_Y_RIGHT:
			dsum += m_plnkTabData->m_dYrTabPitch[i];
			break;
		}
	}
	return dsum;
}

//@KKY.130824.Carrier X방향 공급시 작업해야 하는 Carrier인지 판단.
//@	반드시 -----> BOOL MWorkSchedule::IsSupplyTab(int iWorkGroup, int iCarrierNo) 에서만 호출해야 한다.
MVarList<MTabInfo>*	MWorkSchedule::getCurCarrierSupplyList(int iWorkGroup)
{

	int i=0, j=0;
	MVarList<MTabInfo> *tfirstlist = m_pFirstSupplyList[iWorkGroup];
	MVarList<MTabInfo> *tlist = NULL;
	//작업할 Schedule이 생성되지 않았을 경우 미리 만들어 놓은 첫번째 List로 구한다
	int iScheduleCnt = m_CurWorkSchedule.GetCount();
	for (int i=0; i<iScheduleCnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		if (tlist->GetWorkGroup() != iWorkGroup)
			continue;
		int iListCnt = tlist->GetCount();
		MTabInfo tInfo;
		for (j=0; j<iListCnt; j++)
		{
			tInfo = tlist->GetInfo(j);
			if (FALSE == tInfo.m_jobInfo.m_bToolLoadComplete)
				return tlist;
		}
	}
	return tfirstlist;
}
//@___________________

BOOL MWorkSchedule::IsGeneratedWorkSchedule()
{
	SetLock();
	int cnt = m_CurWorkSchedule.GetCount();
	SetUnlock();
	if (cnt <= 0)
		return FALSE;
	return TRUE;
}

//작업패널이 들어와서 처음 WorkSchedule Data를 만들때
int	MWorkSchedule::GenerateWorkSchedule()
{
	SetLock();
	m_bExistWorkList = FALSE;
	m_bCompleteCurWorkList = FALSE;
	m_bExistSupplyList[DEF_FRONT_GROUP] = FALSE;
	m_bExistSupplyList[DEF_REAR_GROUP] = FALSE;
	m_CurWorkSchedule.Clear();
	m_pCurrentWorkList = NULL;
	m_InspectCarrierList.Clear();
	m_TabMounterListSupply.Clear();
	
	int i = 0;
	for (int i = 0; i < DEF_MAX_GROUP; i++)
		m_pCurrentSupplyList[i] = NULL;

	for (int i = 0; i < DEF_MAX_SUB_LIST; i++)
		m_bCompleteSubWorkList[i] = FALSE;

	if (TRUE == m_bUseWorkSide)
		m_CurWorkSchedule.CopyData(m_OrgListWorkTab);


	SetUnlock();
	return 0;
}

//모든 작업 List를 Clear시킨다.
int MWorkSchedule::ClearWorkSchedule()
{
	SetLock();
	m_bExistWorkList = FALSE;
	m_bCompleteCurWorkList = FALSE;
	m_bExistSupplyList[DEF_FRONT_GROUP] = FALSE;
	m_bExistSupplyList[DEF_REAR_GROUP] = FALSE;
	m_bWorkPanelExist = FALSE;
	m_CurWorkSchedule.Clear();
	int i = 0;
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		m_pCurrentSupplyList[i] = NULL;		
	}
	m_pCurrentWorkList = NULL;

	for (int i = 0; i < DEF_MAX_SUB_LIST; i++)
		m_bCompleteSubWorkList[i] = FALSE;

	SetUnlock();
	return 0;
}

//PanelAligner에 Panel이 있을 경우 MJobInfo의 m_bPressComplete 여부에 따라 WorkSchedule을 초기화 한다
int MWorkSchedule::GenerateWorkScheduleByPanel()
{	
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	
	int cnt = m_CurWorkSchedule.GetCount();
	int i = 0, j = 0;

	int subcnt = 0;
	MVarList<MTabInfo>* plist;
	MVarList<MTabInfo> pPressCompleteList;
	MTabInfo tabinfo;
	for (int i = 0; i < cnt; i++)
	{
		plist = m_CurWorkSchedule.GetSubList(i);
		subcnt = plist->GetCount();
		for (j = 0; j < subcnt; j++)
		{
			tabinfo = plist->GetInfo(j);
			if (tabinfo.m_jobInfo.m_bPressComplete)
			{
				tabinfo.m_jobInfo.SetPressComplete();
				pPressCompleteList.Append(tabinfo);
			}
		}
	}

	m_bExistWorkList = FALSE;
	m_bCompleteCurWorkList = FALSE;
	m_bExistSupplyList[DEF_FRONT_GROUP] = FALSE;
	m_bExistSupplyList[DEF_REAR_GROUP] = FALSE;
	m_CurWorkSchedule.Clear();
	m_pCurrentWorkList = NULL;
	m_InspectCarrierList.Clear();
	m_TabMounterListSupply.Clear();
	for (int i = 0; i < DEF_MAX_GROUP; i++)
		m_pCurrentSupplyList[i] = NULL;

	for (int i = 0; i < DEF_MAX_SUB_LIST; i++)
		m_bCompleteSubWorkList[i] = FALSE;

	int completecnt = pPressCompleteList.GetCount();
	m_CurWorkSchedule.CopyData(m_OrgListWorkTab);

	if (completecnt > 0)
	{
		cnt = m_CurWorkSchedule.GetCount();
		for (int i=0; i<cnt; i++)
		{
			plist = m_CurWorkSchedule.GetSubList(i);
			subcnt = plist->GetCount();
			int nSubCompleteCnt = 0;
			for (j=0; j<subcnt; j++)
			{
				tabinfo = plist->GetInfo(j);
				for (int k=0; k<completecnt; k++)
				{
					MTabInfo tInfo = pPressCompleteList.GetInfo(k);
					if (tabinfo.m_nTabNo == tInfo.m_nTabNo)
					{
						tabinfo.m_jobInfo.SetPressComplete();
						plist->SetInfo(j, tabinfo);
						nSubCompleteCnt++;
						break;
					}
				}
			}
			if (nSubCompleteCnt == subcnt)
			{
				m_bCompleteSubWorkList[i] = TRUE;
				plist->SetReservedSupply(TRUE);
			}
		}
	}


	SetUnlock();
	return 0;
}


//Tab정보를 변경한다. 고유한 번호는 Tab번호이므로, List에 접근시 Tab번호를 사용한다
int MWorkSchedule::UpdateWorkTabInfo(MTabInfo info)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int i = 0, j = 0;
	MTabInfo tInfo;
	MVarList<MTabInfo>* pl;
	int ncntsub = 0;
	int ncnt = m_CurWorkSchedule.GetCount();
	for (int i = 0; i < ncnt; i++)
	{
		pl = m_CurWorkSchedule.GetSubList(i);
		ncntsub = pl->GetCount();
		for (j = 0; j < ncntsub; j++)
		{
			tInfo = pl->GetInfo(j);
			if (tInfo.m_nTabNo == info.m_nTabNo)
			{
				pl->SetInfo(j, info);
				SetUnlock();
				return DEF_PROCESS_DATA_SUCCESS;
			}
		}
	}
	SetUnlock();
	return -1;
}

MVarList<MInspectCarrierInfo> MWorkSchedule::GetFirstInspectionCarrierList()
{
	ASSERT(m_bUseWorkSide == TRUE);
	return m_FirstInspectCarrierList;
}

//작업할 TabList로 InspectionCarrier의 정보를 구한다
MVarList<MInspectCarrierInfo> MWorkSchedule::CreateInspectionCarrierInfo(MVarList<MTabInfo> varlist, BOOL bTrace)
{
	ASSERT(m_bUseWorkSide == TRUE);
	//MWorkSchedule에서만 호출되므로 Lock필요 없음
//	SetLock();

	MVarList<MInspectCarrierInfo>	InspCarrierList;

	MInspectCarrierInfo				icinfo;
	int i = 0, j = 0;
	int ncnt = varlist.GetCount();
	double dInspectPos[DEF_MAX_WORK_PER_UNIT] = {0, };

	for (int i = 0; i < ncnt; i++)
	{		
		icinfo.m_nWorkTabNo = varlist.GetInfo(i).m_nTabNo;
		icinfo.m_nRealTabNo = varlist.GetInfo(i).m_nRealTabNo;
		icinfo.m_dInspectPos = varlist.GetInfo(i).m_dModelOffset;
		InspCarrierList.Append(icinfo);
		dInspectPos[i] = icinfo.m_dInspectPos;
	}
	
#ifdef DEF_SOURCE_SYSTEM
	double TabWidth = m_plnkPreBonderData->m_dLead_Width_Xtab;
#else
	double TabWidth = m_plnkPreBonderData->m_dLead_Width_Ytab;
#endif

	//Inspection Carrier 4개의 위치가 결정되었으면 빠져나간다
	if (ncnt == DEF_MAX_WORK_PER_UNIT)
	{
//		SetUnlock();
		sortingInspectCarrierPos(&InspCarrierList);
	}
	else
	{
		double dNLimit = 0.0, dPLimit = 0.0;
		//첫번째 Inspection Carrier가 움직일수 있는 최대 -위치(패널 Xu Fiducial Mark중심에서)
		//마지막 Inspection Carrier가 움직일수 있는 최대 +위치(패널 Xu Fiducial Mark중심에서)
		if (varlist.GetWorkGroup() == DEF_FRONT_GROUP)
		{
			/*/
			dNLimit = m_plnkSystemData->m_dDistNegInspectCamera[DEF_FRONT_GROUP] //@ + m_plnkSystemData->m_dAlignCam1_FixedPos_X
						+ m_plnkSystemData->m_dCamCarrier1_FixedPosX
						+ m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[DEF_FRONT_GROUP][DEF_INSPECTION_CARRIER_1]/2.0
						+ (DEF_PANELALIGNER_DIST_CENTERTOEDGE + DEF_PANELALIGNER_DIST_PROJECT - m_plnkPanelData->m_dY_FiduMark_Xu);

			dPLimit = m_plnkSystemData->m_dDistPosInspectCamera[DEF_FRONT_GROUP] //@+ m_plnkSystemData->m_dAlignCam1_FixedPos_X
						+ m_plnkSystemData->m_dCamCarrier1_FixedPosX
						+ m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[DEF_FRONT_GROUP][DEF_INSPECTION_CARRIER_4]/2.0
						+ (DEF_PANELALIGNER_DIST_CENTERTOEDGE + DEF_PANELALIGNER_DIST_PROJECT - m_plnkPanelData->m_dY_FiduMark_Xu);
			/*/
			dNLimit = m_plnkSystemData->m_dDistNegInspectCamera[DEF_FRONT_GROUP]
					- m_plnkSystemData->m_dCamCarrier1_FixedPosX
					+ m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0;	//@KKY.130909.
//@			dPLimit = m_plnkSystemData->m_dDistPosInspectCamera[DEF_FRONT_GROUP] - dNLimit;
			dPLimit = m_plnkSystemData->m_dDistPosInspectCamera[DEF_FRONT_GROUP]
					- m_plnkSystemData->m_dCamCarrier1_FixedPosX
					+ m_plnkSystemData->m_dInspectCamFixedPosWidth/2.0;	//@KKY.130909.
		}
		else
		{
			/*/
			dNLimit = m_plnkSystemData->m_dDistNegInspectCamera[DEF_REAR_GROUP] //@+ m_plnkSystemData->m_dAlignCam2_FixedPos_X
						+ m_plnkSystemData->m_dCamCarrier2_FixedPosX
						+ m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[DEF_REAR_GROUP][DEF_INSPECTION_CARRIER_1]/2.0
						+ (DEF_PANELALIGNER_DIST_CENTERTOEDGE + DEF_PANELALIGNER_DIST_PROJECT - m_plnkPanelData->m_dY_FiduMark_Xu);

			dPLimit = m_plnkSystemData->m_dDistPosInspectCamera[DEF_REAR_GROUP] //@+ m_plnkSystemData->m_dAlignCam2_FixedPos_X
						+ m_plnkSystemData->m_dCamCarrier2_FixedPosX
						+ m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[DEF_REAR_GROUP][DEF_INSPECTION_CARRIER_4]/2.0
						+ (DEF_PANELALIGNER_DIST_CENTERTOEDGE + DEF_PANELALIGNER_DIST_PROJECT - m_plnkPanelData->m_dY_FiduMark_Xu);
			/*/
			dNLimit = m_plnkSystemData->m_dDistNegInspectCamera[DEF_REAR_GROUP]
					- m_plnkSystemData->m_dCamCarrier2_FixedPosX;
//@			dPLimit = m_plnkSystemData->m_dDistPosInspectCamera[DEF_REAR_GROUP] - dNLimit;
			dPLimit = m_plnkSystemData->m_dDistPosInspectCamera[DEF_REAR_GROUP]
					- m_plnkSystemData->m_dCamCarrier2_FixedPosX;
		}

		int		nMaxInspCnt = DEF_MAX_WORK_PER_UNIT;//최대로 위치를 결정해야하는 갯수
		int		nCreatedInspCnt = ncnt;//이미 위치가 결정된 Inspection Carrier
		double	dMaxGap = DEF_CAMERACARRIER_SAFE_MARGIN;
		
		int nRetryCnt = -1;
GENERATE_POS:
		while(nCreatedInspCnt != nMaxInspCnt)
		{
			nRetryCnt++;
			if (nRetryCnt > 100)
			{
				//ASSERT(0);
				MyMessageBox(MY_MESSAGE_BOX, 62502, _T("Schedule Error"), M_ICONERROR);
				return InspCarrierList;			
			}
			for (int i=0; i<nCreatedInspCnt; i++)
			{
				//-Limit부터 첫번째 Inspection위치까지 임의로 위치를 만든다
				if (i == 0)
				{
					//생성시킬수 있는 갯수를 구한다
					int ntmpCnt = int( (dInspectPos[i] - dNLimit) / dMaxGap);

					if (ntmpCnt > 0)
					{
						for (j=0; j<ntmpCnt; j++)
						{
							if (nCreatedInspCnt >= nMaxInspCnt) break;
							dInspectPos[nCreatedInspCnt] = dNLimit + dMaxGap*j;
							icinfo.m_dInspectPos = dInspectPos[nCreatedInspCnt];
							icinfo.m_nWorkTabNo = -1;
							icinfo.m_nRealTabNo = -1;
							InspCarrierList.Append(icinfo);
							nCreatedInspCnt++;
						}
						sortingInspectCarrierPos(&InspCarrierList);
						for (j=0; j<InspCarrierList.GetCount(); j++)
						{
							dInspectPos[j] = InspCarrierList.GetInfo(j).m_dInspectPos;
						}
						goto GENERATE_POS;
					}
				}
				//+Limit부터 마지막 Inspection위치까지 임의로 위치를 만든다
				if (i == nCreatedInspCnt-1)
				{
					int ntmpCnt = int( (dPLimit - dInspectPos[i]) / dMaxGap);
					if (ntmpCnt > 0)
					{
						for (j=0; j<ntmpCnt; j++)
						{
							if (nCreatedInspCnt >= nMaxInspCnt) break;
							dInspectPos[nCreatedInspCnt] = dPLimit - dMaxGap*j;
							icinfo.m_dInspectPos = dInspectPos[nCreatedInspCnt];
							icinfo.m_nWorkTabNo = -1;
							icinfo.m_nRealTabNo = -1;
							InspCarrierList.Append(icinfo);
							nCreatedInspCnt++;
						}
						sortingInspectCarrierPos(&InspCarrierList);
						for (j=0; j<InspCarrierList.GetCount(); j++)
						{
							dInspectPos[j] = InspCarrierList.GetInfo(j).m_dInspectPos;
						}
						goto GENERATE_POS;
					}				
				}
				//기 위치결정된 Insepction사이에 임의로 위치를 만든다
				if (nCreatedInspCnt >= 2)
				{
					int ntmpCnt = int ( (dInspectPos[i+1] - dInspectPos[i] - dMaxGap) / dMaxGap);
					if (ntmpCnt > 0)
					{
						for (j=0; j<ntmpCnt; j++)
						{
							if (nCreatedInspCnt >= nMaxInspCnt) break;
							dInspectPos[nCreatedInspCnt] = dInspectPos[i] + dMaxGap*(j+1);
							icinfo.m_dInspectPos = dInspectPos[nCreatedInspCnt];
							icinfo.m_nWorkTabNo = -1;
							icinfo.m_nRealTabNo = -1;
							InspCarrierList.Append(icinfo);
							nCreatedInspCnt++;
						}
						sortingInspectCarrierPos(&InspCarrierList);
						for (j=0; j<InspCarrierList.GetCount(); j++)
						{
							dInspectPos[j] = InspCarrierList.GetInfo(j).m_dInspectPos;
						}
						goto GENERATE_POS;
					}
				}
			}	
		}
	}
	
	sortingInspectCarrierPos(&InspCarrierList, TRUE);

	ncnt = 	InspCarrierList.GetCount();
	for (int i=0; i<ncnt; i++)
	{
		icinfo = InspCarrierList.GetInfo(i);
		if (bTrace)
			TRACE(_T("[No:%d, RealTabNo:%d, Pos:%.3lf] "), icinfo.m_nWorkTabNo, icinfo.m_nRealTabNo, icinfo.m_dInspectPos);
	}
	if (bTrace)
		TRACE(_T("\n"));
	//__________________________

//	SetUnlock();
	return InspCarrierList;
}

//TRUE시키기전 반드시 WorkList를 만든다
//FALSE시킨후 반드시 WorkList를 Clear시킨다.
void MWorkSchedule::SetExistWorkPanel(BOOL bExist)
{
	SetLock();
	if (TRUE == bExist && m_bUseWorkSide)
	{
		//SJ_YYK 110318 Add... WorkTab 0 일땐 workschedule 무시..(임시수정. 추가수정요)
		if(m_OrgListWorkTab.GetCount() != 0)

		ASSERT(m_CurWorkSchedule.GetCount() != 0);
	}
	m_bWorkPanelExist = bExist;
	SetUnlock();
}

BOOL MWorkSchedule::GetExistWorkPanel()
{
	return m_bWorkPanelExist;
}

void MWorkSchedule::SetFeederReady(int iWorkGroup, BOOL bReady)
{
	m_bFeederReady[iWorkGroup] = bReady;
}

BOOL MWorkSchedule::isAllFeederReady()
{
	return m_bFeederReady[DEF_FRONT_GROUP] && m_bFeederReady[DEF_REAR_GROUP];
}

void MWorkSchedule::sortingInspectCarrierPos(MVarList<MInspectCarrierInfo> *pList, BOOL bLast)
{
	ASSERT(m_bUseWorkSide == TRUE);

	MInspectCarrierInfo	 info1, info2;
	for (int i = 0; i < pList->GetCount(); i++)
	{
		for (int j = 1; j < pList->GetCount(); j++)
		{
			info1 = pList->GetInfo(j-1);
			info2 = pList->GetInfo(j);
			if (bLast)
			{
				if (info1.m_dInspectPos > info2.m_dInspectPos)				
				{
					pList->SetInfo(j-1, info2);
					pList->SetInfo(j, info1);
				}
			}
			else
			{
				if (info1.m_dInspectPos > info2.m_dInspectPos)				
				{
					pList->SetInfo(j-1, info2);
					pList->SetInfo(j, info1);
				}
			}
		}
	}
	for (int i=0; i<pList->GetCount(); i++)
	{
		info1 = pList->GetInfo(i);
		info1.m_nCarrierNo = i;
		pList->SetInfo(i, info1);
	}
}

//TrsTabCarrier관련 함수=====================================================================
int	MWorkSchedule::IsAllSupplyComplete(BOOL bCheckGroup, int iWorkGroup)
{
	SetLock();
	int i=0, j=0;
	int iScheduleCnt = m_CurWorkSchedule.GetCount();
	if (iScheduleCnt == 0)
	{
		SetUnlock();
		return TRUE;
	}

	int iTabCnt = 0;
	MVarList<MTabInfo> *tlist;	
	for (int i=0; i<iScheduleCnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		if (TRUE == bCheckGroup && (tlist->GetWorkGroup() != iWorkGroup))
			continue;
		int iListCnt = tlist->GetCount();
		MTabInfo tInfo;
		for (j=0; j<iListCnt; j++)
		{
			tInfo = tlist->GetInfo(j);
			if (FALSE == tInfo.m_jobInfo.m_bToolLoadComplete)
			{
				SetUnlock();
				return FALSE;
			}
		}
	}
	SetUnlock();
	return TRUE;
}

int	MWorkSchedule::GetSupplyTabCnt(int iWorkGroup)
{
	SetLock();
	int i=0, j=0;
	int iFirstTabCnt = 0;


	if (m_pFirstSupplyList[iWorkGroup] != NULL)
	{
		iFirstTabCnt = m_pFirstSupplyList[iWorkGroup]->GetCount();
	}

	//작업할 Schedule이 생성되지 않았을 경우 미리 만들어 놓은 첫번째 List로 구한다
	int iScheduleCnt = m_CurWorkSchedule.GetCount();
	if (iScheduleCnt == 0)
	{
		SetUnlock();
		//@130106.KKY______
//@		return iFirstTabCnt;
		return 0;
		//@________________
	}

	BOOL bChkGroup = TRUE;

	int iTabCnt = 0;
	MVarList<MTabInfo> *tlist;	
	for (int i=0; i<iScheduleCnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		if (TRUE == bChkGroup)
		{
			if (tlist->GetWorkGroup() != iWorkGroup)
				continue;
		}
		int iListCnt = tlist->GetCount();
		MTabInfo tInfo;
		iTabCnt = 0;
		for (j=0; j<iListCnt; j++)
		{
			tInfo = tlist->GetInfo(j);
			if (FALSE == tInfo.m_jobInfo.m_bToolLoadComplete)
				iTabCnt++;
		}
		if (iTabCnt != 0)
			break;
	}
	//@130114.KKY______
	if (iTabCnt == 0)
		iTabCnt = iFirstTabCnt;
	//@________________
	SetUnlock();
	return iTabCnt;
}

int	MWorkSchedule::PickOut_SupplyWorkList(int iWorkGroup)
{
//	ASSERT(m_bUseWorkSide == TRUE);
	if (FALSE == m_bUseWorkSide)
		return -1;
	iWorkGroup = m_iWorkGroup;

	SetLock();

	if (FALSE == m_bWorkPanelExist)
	{
		SetUnlock();
		return -1;
	}

	//101223.Modify________________
	///Job은 남아 있는데 픽아웃 하려고 하는 경우.
	if (TRUE == m_bExistSupplyList[iWorkGroup] || m_pCurrentSupplyList[iWorkGroup] != NULL)
	{
		SetUnlock();
		return -1;
	}
	//____________________________

	int i = 0, j = 0;
	MVarList<MTabInfo> *tlist;	
	MTabInfo tInfo;
	int ncntsub = 0;
	int ncnt = m_CurWorkSchedule.GetCount();
	for (int i=0; i<ncnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		if (FALSE == tlist->GetReservedSupply())
		{
			ncntsub = tlist->GetCount();
			for (j=0; j<ncntsub; j++)
			{			
				tInfo = tlist->GetInfo(j);
				if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
				{
//					m_CurWorkSchedule.GetSubList(i)->SetReservedSupply(TRUE);
//					m_CurWorkSchedule.GetSubList(i)->SetWorkGroup(iWorkGroup);
//					tlist = m_CurWorkSchedule.GetSubList(i);
					tlist->SetReservedSupply(TRUE);
					tlist->SetWorkGroup(iWorkGroup);
//					setWorkGroup(iWorkGroup, tlist->GetKeyValue());
					FittingTabNoToMountNo(tlist, &m_TabMounterListSupply);
//					m_pCurrentSupplyList[iWorkGroup] = m_CurWorkSchedule.GetSubList(i);
					m_pCurrentSupplyList[iWorkGroup] = tlist;
					m_bExistSupplyList[iWorkGroup] = TRUE;
					SetUnlock();
					return 0;
				}
			}
		}
	}

	SetUnlock();
	return -1;
}

int	MWorkSchedule::GetFirstWorkToolNo_FromCurrentSupplyList(int iWorkGroup)
{
	SetLock();
	int iToolNo = -1;
	if (TRUE == IsExist_SupplyWorkList(iWorkGroup))
	{
		MTabInfo tInfo;
		MVarList<MTabInfo> *pList;
		pList = m_pCurrentSupplyList[iWorkGroup];
		int cnt = pList->GetCount();
		for (int i = 0; i < cnt; i++)
		{
			tInfo = pList->GetInfo(i);
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete)
			{
				iToolNo = tInfo.m_jobInfo.m_nWorkedToolID;
				break;
			}
		}
	}
	SetUnlock();
	return iToolNo;
}

//Front, Rear에 따라서 작업할 Tool및 TabCarrier의 번호를 구한다
void MWorkSchedule::FittingTabNoToMountNo(MVarList<MTabInfo> *tList, MVarList<MInspectCarrierInfo> *cList)
{
	ASSERT(m_bUseWorkSide == TRUE);
	TRACE(_T("PickoutSupplyWorkList[WorkGroup:%d]===>"), tList->GetWorkGroup());
//	m_TabMounterListSupply = CreateInspectionCarrierInfo(*tList);
	*cList = CreateInspectionCarrierInfo(*tList);
	int i = 0, j = 0;
	MTabInfo tinfo;
	MInspectCarrierInfo cinfo;
	int nWorkTabCnt = tList->GetCount();
//	int nMountCnt = m_TabMounterListSupply.GetCount();
	int nMountCnt = cList->GetCount();
	for (int i = 0; i < nWorkTabCnt; i++)
	{
		tinfo = tList->GetInfo(i);
		for (j = 0; j < nMountCnt; j++)
		{
			//cinfo = m_TabMounterListSupply.GetInfo(j);
			cinfo = cList->GetInfo(j);
			if (tinfo.m_nTabNo == cinfo.m_nWorkTabNo)
			{
				tinfo.m_jobInfo.m_nWorkedToolID = j;
				tList->SetInfo(i, tinfo);
				break;
			}
		}
	}


}

BOOL MWorkSchedule::IsExist_SupplyWorkList(int iWorkGroup)
{
	ASSERT(m_bUseWorkSide == TRUE);
	iWorkGroup = m_iWorkGroup;
//	SetLock();
	BOOL bexist = FALSE;
	bexist = m_bExistSupplyList[iWorkGroup];
//	SetUnlock();
	return bexist;
}

BOOL MWorkSchedule::isLoadComplete_SupplyWorkList(int iWorkGroup)
{
	ASSERT(m_bUseWorkSide == TRUE);
	iWorkGroup = m_iWorkGroup;

//	SetLock();
	int i = 0;
	MTabInfo tInfo;
	MVarList<MTabInfo> *pList;

	pList = m_pCurrentSupplyList[iWorkGroup];
	if (pList == NULL)
		return TRUE;

	int ncnt = pList->GetCount();
	if (ncnt == 0)
	{
//		SetUnlock();
		return FALSE;
	}
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = pList->GetInfo(i);
		if (FALSE == tInfo.m_jobInfo.m_bToolLoadComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
		{
//			SetUnlock();
			return FALSE;
		}
	}
//	SetUnlock();
	return TRUE;
}

int	MWorkSchedule::GetKeyValue_SupplyWorkList(int iWorkGroup)
{
	ASSERT(m_bUseWorkSide == TRUE);
	iWorkGroup = m_iWorkGroup;

	SetLock();
	MVarList<MTabInfo> *pList;
	pList = m_pCurrentSupplyList[iWorkGroup];
	if (pList->GetCount() == 0)
	{
		SetUnlock();
		return -1;
	}
	int iKeyValue = -1;
	iKeyValue = pList->GetKeyValue();
	SetUnlock();
	return iKeyValue;
}

int	MWorkSchedule::GetCarrierToTool_FromCurrentSupplyList(int iMountGroup, int iCarrierGroup, int iCarrierNo)
{
	ASSERT(m_bUseWorkSide == TRUE);
	SetLock();
	iMountGroup = m_iWorkGroup;

	if (FALSE == IsExist_SupplyWorkList(iMountGroup))
	{
		SetUnlock();
		return -1;
	}

	int iToolID = -1;
	//확인요_____
//	int istartToolNo = DEF_MAX_TABCARRIER - iCarrierNo - 1;
	iToolID = iCarrierNo;	
	//__________
	SetUnlock();
	return iToolID;	
}

BOOL MWorkSchedule::IsSupplyTab(int iWorkGroup, int iCarrierNo)
{
	int iSupplyTabCnt = GetSupplyTabCnt(iWorkGroup);
	if (iSupplyTabCnt <= 0)
		return FALSE;

	//@TabCarrier X방향 공급일때 공급하는 Carrier인지 확인하는 코드________
	SetLock();
	MVarList<MTabInfo> *tlist = getCurCarrierSupplyList(iWorkGroup);
	MVarList<MInspectCarrierInfo> InspInfo = CreateInspectionCarrierInfo(*tlist, FALSE);
	MInspectCarrierInfo info;
	int iCnt = InspInfo.GetCount();
	//Debugging..........
	for (int i=0; i<iCnt; i++)
	{
		info = InspInfo.GetInfo(i);
	}
	//...................

	if (iCarrierNo < iCnt)
	{
		info = InspInfo.GetInfo(iCarrierNo);
		if (info.m_nWorkTabNo != -1)
		{
			SetUnlock();
			return TRUE;
		}
	}
	SetUnlock();
	return FALSE;
	//@____________________________________________________________________



}

//Carrier에서 현재 공급하는 Tab의 작업 그룹. 8Mounter Mode에서 사용
int	MWorkSchedule::GetWorkGroup_CarrierSupply(int iCarrierWorkGroup)
{
	int iWorkGroup = DEF_NONE_GROUP;
	iWorkGroup = iCarrierWorkGroup;

	return iWorkGroup;
}

//@Carrier가 Ready위치 대기중 TabLoading할 다음 Carrier가 있는지 확인
BOOL MWorkSchedule::IsExistNextSupplyCarrier(int iWorkGroup, int iCarrierNo)
{
	MVarList<MTabInfo> *tlist = getCurCarrierSupplyList(iWorkGroup);
	MVarList<MInspectCarrierInfo> InspInfo = CreateInspectionCarrierInfo(*tlist, FALSE);
	MInspectCarrierInfo info;
	int iCnt = InspInfo.GetCount();
	for (int i=iCarrierNo+1; i<DEF_MAX_TABCARRIER; i++)
	{
		info = InspInfo.GetInfo(i);
		if (info.m_nWorkTabNo != -1)
			return TRUE;
	}
	return FALSE;
}

//===========================================================================================

//TrsTabMounter관련 함수=====================================================================
MTabInfo MWorkSchedule::GetTabInfo_SupplyWorkList(int iWorkGroup, int iToolID)
{
	ASSERT(m_bUseWorkSide == TRUE);

	iWorkGroup = m_iWorkGroup;

	MTabInfo tInfo;
	if (FALSE == IsExist_SupplyWorkList(iWorkGroup))
	{
		return tInfo;
	}
	SetLock();

	MVarList<MTabInfo> *pList;
	pList = m_pCurrentSupplyList[iWorkGroup];
	int cnt = pList->GetCount();
	for (int i = 0; i < cnt; i++)
	{
		tInfo = pList->GetInfo(i);
		if (tInfo.m_jobInfo.m_nWorkedToolID == iToolID)
		{
			SetUnlock();
			return tInfo;
		}
	}
	tInfo.Init();
	SetUnlock();
	return tInfo;
}

int	MWorkSchedule::SetMounterTabLoadComplete_SupplyWorkList(int iWorkGroup, int iToolID)
{
	ASSERT(m_bUseWorkSide == TRUE);

	iWorkGroup = m_iWorkGroup;

	SetLock();
	MVarList<MTabInfo> *pList;

	pList = m_pCurrentSupplyList[iWorkGroup];

	if (pList == NULL)
		return -1;

	int ncnt = pList->GetCount();
	int i = 0;
	MTabInfo tInfo;
	if (ncnt == 0)
	{
		SetUnlock();
		return -1;
	}
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = pList->GetInfo(i);
		if (tInfo.m_jobInfo.m_nWorkedToolID == iToolID)
		{
			tInfo.m_jobInfo.m_bToolLoadComplete = TRUE;
			pList->SetInfo(i, tInfo);
			break;
		}
	}
	if (isLoadComplete_SupplyWorkList(iWorkGroup))
	{
//		m_iSupplyListNo++;
//		if (m_iSupplyListNo >= m_CurWorkSchedule.GetCount())
//			m_iSupplyListNo = 0;
		m_bExistSupplyList[iWorkGroup] = FALSE;
		m_pCurrentSupplyList[iWorkGroup] = NULL;
	}
	SetUnlock();
	return 0;
}

//현재 Supply작업중인 List에서 iTabNo를 넘겨주고, 작업할 Carrier번호를 얻는다(확인요)
int	MWorkSchedule::GetCarrierNo_FromCurrentSupplyList(int iWorkGroup, int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	if (FALSE == IsExist_SupplyWorkList(iWorkGroup))
	{
		SetUnlock();
		return -1;
	}

	int iCarrierID = -1;
	MVarList<MTabInfo>* tList = m_pCurrentSupplyList[iWorkGroup];

	MTabInfo tInfo;
	int ncnt = tList->GetCount();
	int nstartno = 0;
	int workgroup = tList->GetWorkGroup();

	for (int i = 0; i < ncnt; i++)
	{
		tInfo = tList->GetInfo(i);
		if (tInfo.m_nTabNo == iTabNo)
		{
			iCarrierID = i;
			break;
		}
	}

	SetUnlock();
	return iCarrierID;
}

BOOL MWorkSchedule::IsLoadComplete_SupplyList(int iKeyValue)
{
	SetLock();
	BOOL bComplete = TRUE;
	int ncnt = m_CurWorkSchedule.GetCount();
	MVarList<MTabInfo> *tlist;	
	for (int i=0; i<ncnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		if (tlist->GetKeyValue() == iKeyValue)
		{
			ncnt = tlist->GetCount();
			int j;
			for (j = 0; j < ncnt; j++)
			{
				if (FALSE == tlist->GetInfo(j).m_jobInfo.m_bToolLoadComplete)
					break;
			}
			if (j < ncnt)
				bComplete = FALSE;
			break;//110106.Add
		}
	}
	SetUnlock();
	return bComplete;
}

BOOL MWorkSchedule::IsInspectionSkip_CurrentWorkList(int iWorkInstance)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	if (FALSE == m_bExistWorkList)
	{
		SetUnlock();
		return TRUE;
	}

	int ncnt = m_pCurrentWorkList->GetCount();
	int i = 0;
	MTabInfo tInfo;
	if (ncnt == 0)
	{
		SetUnlock();
		return TRUE;
	}
	BOOL bSkip = FALSE;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (tInfo.m_jobInfo.m_nWorkedToolID == iWorkInstance)
		{
			bSkip = tInfo.m_jobInfo.m_bSkip;
			break;
		}
	}
	SetUnlock();
	return bSkip;
}

BOOL MWorkSchedule::IsInspectionCompleteGroup_CurrentWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	int i = 0;
	MTabInfo tInfo;
	if (ncnt == 0)
	{
		SetUnlock();
		return FALSE;
	}
	BOOL bComplete = TRUE;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		bComplete &= (tInfo.m_jobInfo.m_bInspectionComplete || tInfo.m_jobInfo.m_bSkip);
	}
	SetUnlock();
	return bComplete;
}

//마지막에 Pickout한 Group을 찾는다.
int	MWorkSchedule::GetLastPickoutGroup()
{
	SetLock();
	int iPickOutGroup = -1;
	if (m_CurWorkSchedule.GetCount() == 0)
	{
		SetUnlock();
		return -1;
	}
	
	int i = 0, j = 0;
	MVarList<MTabInfo> *tlist;	
	MTabInfo tInfo;
	int iListcnt = m_CurWorkSchedule.GetCount();
	for (int i=0; i<iListcnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		int itabcnt = tlist->GetCount();
		for (j=0; j<itabcnt; j++)
		{
			tInfo = tlist->GetInfo(j);
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete)
				break;
		}
		if (j < itabcnt)
			break;
		else
			iPickOutGroup = tlist->GetWorkGroup();
	}
	SetUnlock();
	return iPickOutGroup;
}
//===========================================================================================


//TrsInspectionCamera관련 함수=====================================================================
//List에서 PressComplete되지않고, Skip도 아닌 Tab이 있으면 넘겨준다
int	MWorkSchedule::PickOut_CurrentWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

	int i = 0, j = 0;

//	if (FALSE == m_bWorkPanelExist)
	if (FALSE == m_bWorkPanelExist || TRUE == m_bExistWorkList)
		return -1;
	
	SetLock();

	MVarList<MTabInfo> *tlist;	
	MTabInfo tInfo;
	int ncntsub = 0;
	int ncnt = m_CurWorkSchedule.GetCount();
	for (int i=0; i<ncnt; i++)
	{
		tlist = m_CurWorkSchedule.GetSubList(i);
		ncntsub = tlist->GetCount();
		for (j=0; j<ncntsub; j++)
		{
			tInfo = tlist->GetInfo(j);
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
			{
				if (i == ncnt-1)
					tlist->SetLastGroup(TRUE);
				else
					tlist->SetLastGroup(FALSE);
				TRACE(_T("PickoutCurrentWorkList[WorkGroup:%d]===>"), tlist->GetWorkGroup());
				m_InspectCarrierList = CreateInspectionCarrierInfo(*tlist);
				FittingTabNoToInspectionNo(tlist, &m_InspectCarrierList);
				m_pCurrentWorkList = tlist;
				m_bCompleteCurWorkList = FALSE;
				m_bExistWorkList = TRUE;
				SetUnlock();
				return 0;
			}
		}
	}
	SetUnlock();
	return -1;
}

int MWorkSchedule::FittingTabNoToInspectionNo(MVarList<MTabInfo> *tList, MVarList<MInspectCarrierInfo> *cList)
{
	ASSERT(m_bUseWorkSide == TRUE);

//하면안됨...	SetLock();
	int i = 0, j = 0;
	MTabInfo tinfo;
	MInspectCarrierInfo cinfo;
	int nWorkTabCnt = tList->GetCount();
//	int nInspectCnt = m_InspectCarrierList.GetCount();
	*cList = CreateInspectionCarrierInfo(*tList);
	int nInspectCnt = cList->GetCount();
	for (int i = 0; i < nWorkTabCnt; i++)
	{
		tinfo = tList->GetInfo(i);
		for (j = 0; j < nInspectCnt; j++)
		{
			cinfo = cList->GetInfo(j);
			if (tinfo.m_nTabNo == cinfo.m_nWorkTabNo)
			{
				tinfo.m_jobInfo.m_nWorkedInspectionID = j;
				tList->SetInfo(i, tinfo);
				break;
			}
		}
	}
//	SetUnlock();
	return 0;
}

BOOL MWorkSchedule::IsExist_CurrentWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

//	SetLock();
	BOOL bexist = FALSE;
//	if (m_pCurrentWorkList.GetCount() != 0 && m_InspectCarrierList.GetCount() != 0)
//		bexist = TRUE;
	bexist = m_bExistWorkList;
//	SetUnlock();
	return bexist;
}

MInspectCarrierInfo MWorkSchedule::GetInspectCarrierInfo_FromCurrentWorkList(int iInstanceNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	MInspectCarrierInfo info = m_InspectCarrierList.GetInfo(iInstanceNo);
	SetUnlock();
	return info;
}

MInspectCarrierInfo	MWorkSchedule::GetInspectCarrierInfo_FromFirstWorkList(int iInstanceNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

//	SetLock();
	MInspectCarrierInfo info = m_FirstInspectCarrierList.GetInfo(iInstanceNo);
//	SetUnlock();
	return info;
}

MInspectCarrierInfo	MWorkSchedule::GetFirstWorkTabCarrierInfo_FromFirstWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

//	SetLock();
	MInspectCarrierInfo info;
	int cnt = m_FirstInspectCarrierList.GetCount();
	for (int i=0; i<cnt; i++)
	{
		info = m_FirstInspectCarrierList.GetInfo(i);
		if (info.m_nWorkTabNo != -1)
		{
//			SetUnlock();
			return info;
		}
	}
//	SetUnlock();
	return info;
}

MInspectCarrierInfo	MWorkSchedule::GetLastWorkTabCarrierInfo_FromFirstWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

//	SetLock();
	MInspectCarrierInfo info;
	int cnt = m_FirstInspectCarrierList.GetCount();
	for (int i=cnt-1; i>=0; i--)
	{
		info = m_FirstInspectCarrierList.GetInfo(i);
		if (info.m_nWorkTabNo != -1)
		{
//			SetUnlock();
			return info;
		}
	}
//	SetUnlock();
	return info;
}

BOOL MWorkSchedule::IsSpecifiedWorkGroup()
{
	ASSERT(m_bUseWorkSide == TRUE);
	ASSERT (m_pCurrentWorkList);

	SetLock();
	BOOL bSpecified = FALSE;
	if (m_pCurrentWorkList->GetWorkGroup() == DEF_FRONT_GROUP || m_pCurrentWorkList->GetWorkGroup() == DEF_REAR_GROUP)
		bSpecified = TRUE;
	bSpecified &= m_bExistWorkList;
	SetUnlock();
	return bSpecified;
}

int MWorkSchedule::GetTabMounterNoFromCurrentWorkList(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	if (FALSE == IsSpecifiedWorkGroup())
		return -1;
	SetLock();
	int iToolID = -1;
	MTabInfo tinfo;
	int ncnt = m_pCurrentWorkList->GetCount();
	for (int i = 0; i < ncnt; i++)
	{
		tinfo = m_pCurrentWorkList->GetInfo(i);
		if (tinfo.m_nTabNo == iTabNo)
		{
			iToolID = tinfo.m_jobInfo.m_nWorkedToolID;
			break;
		}
	}
	SetUnlock();
	return iToolID;
}

int	MWorkSchedule::GetInspectionNoFromCurrentWorkList(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	if (FALSE == IsSpecifiedWorkGroup())
		return -1;
	SetLock();
	int iInspectionID = -1;
	MInspectCarrierInfo info;
	int ncnt = m_InspectCarrierList.GetCount();
	for (int i = 0; i < ncnt; i++)
	{
		info = m_InspectCarrierList.GetInfo(i);
		if (info.m_nWorkTabNo == iTabNo)
		{
			iInspectionID = i;			
			break;
		}
	}
	SetUnlock();
	return iInspectionID;
}

//Inspection중 인식에러로 인해 TabSkip해야할 경우
void MWorkSchedule::SetTabSkipToCurrentWorkList(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	MTabInfo tInfo;
	BOOL bGroupWorkComplete = FALSE;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (tInfo.m_nTabNo == iTabNo)
		{
			tInfo.m_jobInfo.m_bSkip = TRUE;
			m_pCurrentWorkList->SetInfo(i, tInfo);
			break;
		}
	}

	/*TabSkip지정할 경우 PressCompleteFlag는 변경하지 않는다...
	//남아있는 Tab이 없을경우 Skip한 Tab을 Append할지 결정한다
	if (i != ncnt)
	{
		for (int i=0; i<ncnt; i++)
		{
			tInfo = m_pCurrentWorkList->GetInfo(i);
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
				break;//작업할 Tab이 남아 있다...
		}
		//현재 List작업을 완료 했는데, skip한 Tab이 있으면 Append한다
		if (i == ncnt)
		{
			bGroupWorkComplete = TRUE;
		}
	}

	if (TRUE == bGroupWorkComplete)//IsPressComplete_CurrentWorkList())
	{
		m_bExistWorkList = FALSE;
		m_bCompleteCurWorkList = TRUE;
		m_bCompleteSubWorkList[m_pCurrentWorkList->GetKeyValue()] = TRUE;
//		m_pCurrentWorkList = NULL;
	}
	else
		m_bCompleteCurWorkList = FALSE;
	*/
	SetUnlock();
}

void MWorkSchedule::SetInspectionComplete(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	MTabInfo tInfo;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (tInfo.m_nTabNo == iTabNo)
		{
			tInfo.m_jobInfo.m_bInspectionComplete = TRUE;
			m_pCurrentWorkList->SetInfo(i, tInfo);
			break;
		}
	}
	SetUnlock();
}

void MWorkSchedule::ResetInspectionComplete_CurWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	MTabInfo tInfo;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		tInfo.m_jobInfo.m_bInspectionComplete = FALSE;
		tInfo.m_jobInfo.m_bPressComplete = FALSE;
		tInfo.m_jobInfo.m_bSkip = FALSE;
		tInfo.m_jobInfo.m_bToolLoadComplete = FALSE;
		m_pCurrentWorkList->SetInfo(i, tInfo);
	}
	SetUnlock();
}

void MWorkSchedule::SetPressComplete(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	MTabInfo tInfo;
	BOOL bGroupWorkComplete = FALSE;
	int i;
	for (i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (tInfo.m_nTabNo == iTabNo)
		{
			//tInfo.m_jobInfo.m_bPressComplete = TRUE;
			tInfo.m_jobInfo.SetPressComplete();
			m_pCurrentWorkList->SetInfo(i, tInfo);
			break;
		}
	}
	//남아있는 Tab이 있는지 없는지 확인
	if (i != ncnt)
	{
		int i;
		for (i=0; i<ncnt; i++)
		{
			tInfo = m_pCurrentWorkList->GetInfo(i);
			//if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete)
				break;//작업할 Tab이 남아 있다...
		}
		if (i == ncnt)
		{
			bGroupWorkComplete = TRUE;
		}
	}

	if (TRUE == bGroupWorkComplete)//IsPressComplete_CurrentWorkList())
	{
		m_bExistWorkList = FALSE;
		m_bCompleteCurWorkList = TRUE;
		m_bCompleteSubWorkList[m_pCurrentWorkList->GetKeyValue()] = TRUE;
//		m_pCurrentWorkList = NULL;
	}
	else
		m_bCompleteCurWorkList = FALSE;

	SetUnlock();
}

BOOL MWorkSchedule::IsPressComplete_CurrentWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

	/*
	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	MTabInfo tInfo;
	BOOL bComplete = TRUE;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
		{
			bComplete = FALSE;
			break;
		}
	}
	//m_csLock.Unlock();
	SetUnlock();
	return bComplete;
	*/
	return m_bCompleteCurWorkList;
}

void MWorkSchedule::AppendSkipTabAfterInspection()
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_pCurrentWorkList->GetCount();
	MTabInfo tInfo;
	MVarList<MTabInfo> *WorkList = new MVarList<MTabInfo>;
	BOOL bSkip = FALSE;
	for (int i = 0; i < ncnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (TRUE == tInfo.m_jobInfo.m_bSkip)
		{
			bSkip = TRUE;
			tInfo.m_jobInfo.Init();
			WorkList->Append(tInfo);
		}
	}
	if (WorkList->GetCount() == 0)
		delete WorkList;
	else
	{
		m_CurWorkSchedule.Append(WorkList);
	}

	SetUnlock();
}

BOOL MWorkSchedule::IsPressComplete_AllWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_CurWorkSchedule.GetCount();
	int nsubcnt = 0;
	MVarList<MTabInfo> *tList;
	MTabInfo tInfo;
	BOOL bComplete = TRUE;
	for (int i = 0; i < ncnt; i++)
	{
		tList = m_CurWorkSchedule.GetSubList(i);
		nsubcnt = tList->GetCount();
		for (int j = 0; j < nsubcnt; j++)
		{
			tInfo = tList->GetInfo(j);
			//if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete)
			{
				bComplete = FALSE;
				break;
			}
		}
		if (FALSE == bComplete)
			break;
	}
	//171004 JSh.s
	if(ncnt == 0)
	{
		bComplete = FALSE;
	}
	//171004 JSh.e
	SetUnlock();
	return bComplete;
}

BOOL MWorkSchedule::IsPressCompleteSelectWorkList(int iKeyValue)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_CurWorkSchedule.GetCount();
	int nsubcnt = 0;
	MVarList<MTabInfo> *tList;
	MTabInfo tInfo;
	BOOL bComplete = FALSE;
	for (int i = 0; i < ncnt; i++)
	{
		tList = m_CurWorkSchedule.GetSubList(i);
		if (tList->GetKeyValue() == iKeyValue)
		{
			nsubcnt = tList->GetCount();
			int j;
			for (j = 0; j < nsubcnt; j++)
			{
				tInfo = tList->GetInfo(j);
				if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
					break;
			}
			if (j == nsubcnt)
				bComplete = TRUE;
		}
	}
	SetUnlock();
	return bComplete;
}

int MWorkSchedule::GetKeyValue_CurrentWorkList()
{
	ASSERT(m_bUseWorkSide == TRUE);

//	if (m_pCurrentWorkList.GetCount() == 0)
	if (FALSE == m_bExistWorkList)
		return -1;
	int iKeyValue = -1;
	iKeyValue = m_pCurrentWorkList->GetKeyValue();
	return iKeyValue;
}

MTabInfo MWorkSchedule::GetTabInfo_CurrentWorkList(int iWorkInstance)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	MTabInfo tInfo;
	int cnt = m_pCurrentWorkList->GetCount();
	for (int i = 0; i < cnt; i++)
	{
		tInfo = m_pCurrentWorkList->GetInfo(i);
		if (tInfo.m_jobInfo.m_nWorkedToolID == iWorkInstance)
		{
			SetUnlock();
			return tInfo;
		}
	}
	tInfo.Init();
	SetUnlock();
	return tInfo;
}

double MWorkSchedule::GetTabPos(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	MTabInfo tabinfo;
	MVarList<MTabInfo>* plistmount = NULL;
	CString strLog;
	int ncnt = m_OrgListWorkTab.GetCount();
	for (int i = 0; i < ncnt; i++)
	{
		plistmount = m_OrgListWorkTab.GetSubList(i);
		int listcnt = plistmount->GetCount();
		for (int j = 0; j < listcnt; j++)
		{
			tabinfo = plistmount->GetInfo(j);
			if (tabinfo.m_nTabNo == iTabNo)
			{
				SetUnlock();
				return tabinfo.m_dModelOffset;
			}
		}
	}
	SetUnlock();
	return 0;
}

void MWorkSchedule::SetInspectionOffset(int iTabNo, MPos_XYT posoffset)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	int ncnt = m_CurWorkSchedule.GetCount();
	int nsubcnt = 0;
	MVarList<MTabInfo> *tList;
	MTabInfo tInfo;
	BOOL bComplete = FALSE;
	for (int i = 0; i < ncnt; i++)
	{
		tList = m_CurWorkSchedule.GetSubList(i);
		nsubcnt = tList->GetCount();
		for (int j = 0; j<nsubcnt; j++)
		{
			tInfo = tList->GetInfo(j);
//@			if (tInfo.m_nTabNo == iTabNo)
			if (tInfo.m_nRealTabNo == iTabNo)
			{
				tInfo.m_jobInfo.m_OffsetInspection = posoffset;
				tList->SetInfo(j, tInfo);
			}
		}
	}
	SetUnlock();	
}

MPos_XYT MWorkSchedule::GetInspectionOffset(int iTabNo)
{
	ASSERT(m_bUseWorkSide == TRUE);

	SetLock();
	MPos_XYT Offset;
	int ncnt = m_CurWorkSchedule.GetCount();
	int nsubcnt = 0;
	MVarList<MTabInfo> *tList;
	MTabInfo tInfo;
	BOOL bComplete = FALSE;
	for (int i = 0; i < ncnt; i++)
	{
		tList = m_CurWorkSchedule.GetSubList(i);
		nsubcnt = tList->GetCount();
		for (int j = 0; j<nsubcnt; j++)
		{
			tInfo = tList->GetInfo(j);
//@			if (tInfo.m_nTabNo == iTabNo)
			if (tInfo.m_nRealTabNo == iTabNo)
			{
				Offset = tInfo.m_jobInfo.m_OffsetInspection;
			}
		}
	}
	SetUnlock();
	return Offset;
}
//=================================================================================================

int MWorkSchedule::GetMaxTabQty(EWorkingSide eWorkSide)
{
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Xu)
			return m_plnkTabData->m_iXuTabQty;
		
	case WORKING_SIDE_X_DOWN:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Xd)
			return m_plnkTabData->m_iXdTabQty;
		
	case WORKING_SIDE_Y_LEFT:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yl)
			return m_plnkTabData->m_iYlTabQty;
		
	case WORKING_SIDE_Y_RIGHT:
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yr)
			return m_plnkTabData->m_iYrTabQty;

	default:
		break;
	}
	return -1;
}

BOOL MWorkSchedule::IsMoreWorkForCurrentPanel()
{
	/*
	SetLock();
	int ncnt = m_CurWorkSchedule.GetCount();
	if (ncnt == 0)
	{
		SetUnlock();
		return FALSE;
	}
	MVarList<MTabInfo> *plist;
	for (int i=0; i<ncnt; i++)
	{
		plist = m_CurWorkSchedule.GetSubList(i);
		int subcnt = plist->GetCount();
		for (int j=0; i<subcnt; j++)
		{
			MTabInfo tInfo = plist->GetInfo(i);
			if (FALSE == tInfo.m_jobInfo.m_bPressComplete && FALSE == tInfo.m_jobInfo.m_bSkip)
			{
				SetUnlock();
				return TRUE;
			}
		}
	}
	SetUnlock();
	return FALSE;	
	*/
	if (FALSE == m_bUseWorkSide)
		return FALSE;

	SetLock();
	int nSubListCnt = m_CurWorkSchedule.GetCount();
	BOOL bExistWorkTab = FALSE;
	for (int i=0; i<nSubListCnt; i++)
	{
		if (!m_bCompleteSubWorkList[i])
		{
			bExistWorkTab = TRUE;
			break;
		}
	}
	SetUnlock();
	return bExistWorkTab;
}

//현재 Press작업중인 WorkGroup이 마지막인지
BOOL MWorkSchedule::IsLastWorkGroup()
{
	ASSERT(m_bUseWorkSide == TRUE);
	
	if (m_pCurrentWorkList != NULL
		&& m_pCurrentWorkList->GetLastGroup())
		return TRUE;
	return FALSE;
}

void MWorkSchedule::SetLock()
{
	/*
	DWORD sTime = GetTickCount();
	while(m_bLock)
	{
		if (GetTickCount() - sTime > 500)
		{
			m_bLock = FALSE;
		}
//		DoEvents();
		Sleep(1);
	}
	m_bLock = TRUE;
	*/
	//m_csLock.Lock(); //171004 JSH
	m_csLock.Lock();
}

void MWorkSchedule::SetUnlock()
{
//	m_bLock = FALSE;
	m_csLock.Unlock();
}

void MWorkSchedule::DoEvents()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
		if (!AfxGetApp()->PumpMessage()){
			::PostQuitMessage(0);
			return;
		}
	}
	Sleep(1);
}

//미사용__________
int MWorkSchedule::setWorkGroup(int iWorkGroup, int iKeyValue)
{
	ASSERT(m_bUseWorkSide == TRUE);

	//하면안됨	SetLock();
	int ncnt = m_CurWorkSchedule.GetCount();
	if (ncnt == 0)
	{
//		SetUnlock();
		return -1;
	}
	MVarList<MTabInfo> *plist;
	for (int i=0; i<ncnt; i++)
	{
		plist = m_CurWorkSchedule.GetSubList(i);
		if (plist->GetKeyValue() == iKeyValue)
		{
			plist->SetWorkGroup(iWorkGroup);
			FittingTabNoToMountNo(plist, &m_TabMounterListSupply);
//			SetUnlock();
			return 0;
		}
	}
//	SetUnlock();
	return -1;
}
//###########################################################################
