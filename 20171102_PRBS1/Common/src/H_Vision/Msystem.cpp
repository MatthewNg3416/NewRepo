#include "stdafx.h"
#include "Msystem.h"

Msystem::Msystem()
 :	m_lSysID(M_NULL),
	m_rglDigID(NULL),
	m_rglGrabParent(NULL),
	m_rglGrab(NULL),
	m_rglSource(NULL),
	m_iInstanceNo(0),
	m_iMaxCamNum(0),
	m_iStartCamNo(-1)
{
}

Msystem::~Msystem()
{
}

long Msystem::GetSysID()
{
	return m_lSysID;
}

long Msystem::GetSourceBuffer(int iUserCamNo)
{
	int iInternalCamNo = (*m_pvectorCamData)[iUserCamNo].m_iInternalCameraNo;
	if (iInternalCamNo < m_iStartCamNo || iInternalCamNo > m_iStartCamNo + m_iMaxCamNum)
		return M_NULL;

	return m_rglSource[iInternalCamNo - m_iStartCamNo];
}

long Msystem::GetGrabBuffer(int iUserCamNo)
{
	int iInternalCamNo = (*m_pvectorCamData)[iUserCamNo].m_iInternalCameraNo;
	if (iInternalCamNo < m_iStartCamNo || iInternalCamNo > m_iStartCamNo + m_iMaxCamNum)
		return M_NULL;

	return m_rglGrab[iInternalCamNo - m_iStartCamNo];
}

int Msystem::GetStartCamNo()
{
	return m_iStartCamNo;
}

int Msystem::GetLastCamNo()
{
	return m_iStartCamNo + m_iMaxCamNum;
}

void Msystem::initMembers(SSystemData* pSystemData)
{
	m_iInstanceNo = pSystemData->m_iInstanceNo;	// Vision System Instance No 
	m_pvectorCamData = pSystemData->m_pvectorCamData;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// System 별 시작 Camera 번호 계산
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	int iStartCamNo = 0;
	if (m_iInstanceNo != 0)
	{
		for (int i = 0; i < m_iInstanceNo; i++)
		{
			if (pSystemData->m_rgiType[i] == DEF_METEOR_II_MC)
				iStartCamNo += DEF_METEOR_SYSTEM_MAX_CAM_NUM;
			else	// if (pSystemData->m_rgiType[i] == DEF_SOLIOS) : Default
				iStartCamNo += DEF_SOLIOS_SYSTEM_MAX_CAM_NUM;
		}
	}
	m_iStartCamNo = iStartCamNo;
}

long Msystem::getDigitizer(int iSysCamNo)
{
	return m_rglDigID[iSysCamNo];
}

int Msystem::WaitGrabComplete(int iUserCamNo)
{
	// Meteor2 M/C : Synchronous Grab Mode 사용하므로 Wait Grab 불필요.
	if (DEF_METEOR_II_MC == m_iSystemType)
		return ERR_VISION_SUCCESS; 

	if (iUserCamNo >= (int)m_pvectorCamData->size())
		return M_NULL;

	int iSysCamNo = ((*m_pvectorCamData)[iUserCamNo]).m_iInternalCameraNo - m_iStartCamNo;

	/* 확인 필요 : M_GRAB_END 와 M_GRAB_FRAME_END 의 차이.
	M_GRAB_END         Wait for the end of the queued grabs.
    M_GRAB_FRAME_END   Waits for the end of the current grab. 
	*/
	MdigGrabWait(m_rglDigID[iSysCamNo], M_GRAB_END);

	// Error check
	//typedef char                   MIL_TEXT_CHAR; 으로 되어있음
	MIL_TEXT_CHAR szError[M_ERROR_MESSAGE_SIZE];
	//WCHAR szError[M_ERROR_MESSAGE_SIZE];
	long lError = MappGetError(M_CURRENT+M_THREAD_CURRENT+M_MESSAGE, szError);
	if (M_NULL_ERROR != lError)
	{
//100823.KKT.임시Del.ASSERT에러 발생		TRACE(szError);

		return ERR_GRAB_FAIL;
	}
	return ERR_VISION_SUCCESS; 
}

void Msystem::EnterCameraCriticalSection(int iCamNo)
{
	EnterCriticalSection(&((*m_pvectorCamData)[iCamNo].m_csGrab));
}

void Msystem::LeaveCameraCriticalSection(int iCamNo)
{
	LeaveCriticalSection(&((*m_pvectorCamData)[iCamNo].m_csGrab));
}
