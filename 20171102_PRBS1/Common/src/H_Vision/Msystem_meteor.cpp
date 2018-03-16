#include "stdafx.h"
#include "Msystem_meteor.h"
#include <string>

Msystem_meteor::Msystem_meteor() : m_iOldChNo(0), m_rgiDigHookCount(NULL)
{
	m_iSystemType = DEF_METEOR_II_MC;
}

Msystem_meteor::~Msystem_meteor()
{
	if (m_rglGrab != NULL)
	{
		for (int i = 0; i < m_iMaxCamNum; i++)
		{
			if (m_rglGrab[i] != M_NULL)
				MbufFree(m_rglGrab[i]);
		}
		delete[] m_rglGrab;
	}
	
	if (m_rglSource != NULL)
	{
		for (int i = 0; i < m_iMaxCamNum; i++)
		{
			if (m_rglSource[i] != M_NULL)
				MbufFree(m_rglSource[i]);
		}
		delete[] m_rglSource;
	}

	if (m_rglGrabParent != NULL)
	{
		for (int i = 0; i < DEF_MATROX_SYSTEM_CHANNEL_NUM; i++)
		{
			if (m_rglGrabParent[i] != NULL)
				MbufFree(m_rglGrabParent[i]);
		}
		delete[] m_rglGrabParent;
	}

	if (m_lSysID != M_NULL)
		MsysFree(m_lSysID);

	// 확인 필요 : Log 로 변경.
	TRACE(_T("VisionSystem_%d Destruction is Completed.\n"), m_iInstanceNo);
}

int Msystem_meteor::AllocSystem(SSystemData* pSystemData)
{
	initMembers(pSystemData);
	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Vision System Type 에 따른 System ID Allocation
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	m_iMaxCamNum = DEF_METEOR_SYSTEM_MAX_CAM_NUM;
	long lSystemNo = static_cast<long>(m_iInstanceNo);
	m_lSysID = MsysAlloc(M_SYSTEM_METEOR_II, lSystemNo, M_COMPLETE, M_NULL);
	if (m_lSysID == M_NULL)
		return ERR_SYSTEM_ALLOC_FAILURE;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Digitizer, Buffer Allocation
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	m_rglDigID = new long[DEF_MATROX_SYSTEM_CHANNEL_NUM];
	m_rgiDigHookCount = new int[DEF_MATROX_SYSTEM_CHANNEL_NUM];
	m_pHookData = new SMeteorHookData[DEF_MATROX_SYSTEM_CHANNEL_NUM];
	for (int k = 0; k < DEF_MATROX_SYSTEM_CHANNEL_NUM; k++)
	{
		m_rglDigID[k] = M_NULL;
		m_rgiDigHookCount[k] = 0;
		m_pHookData[k].plGrabR = NULL;
		m_pHookData[k].plGrabG = NULL;
		m_pHookData[k].plGrabB = NULL;
		m_pHookData[k].lSourceR = M_NULL;
		m_pHookData[k].lSourceG = M_NULL;
		m_pHookData[k].lSourceB = M_NULL;
		m_pHookData[k].pDataR = NULL;
		m_pHookData[k].pDataG = NULL;
		m_pHookData[k].pDataB = NULL;
	}
	
	// Grab Parent (color) : 2 EA
	m_rglGrabParent = new long[DEF_MATROX_SYSTEM_CHANNEL_NUM];
	for (int i = 0; i < DEF_MATROX_SYSTEM_CHANNEL_NUM; i++)
		m_rglGrabParent[i] = M_NULL;
	
	// Grab Child (monochrome) : 6 (= 2 * 3) EA
	m_rglGrab = new long[DEF_METEOR_SYSTEM_MAX_CAM_NUM];
	for (int i = 0; i < DEF_METEOR_SYSTEM_MAX_CAM_NUM; i++)
		m_rglGrab[i] = M_NULL;

	m_iMaxCamPerChannel = m_iMaxCamNum / DEF_MATROX_SYSTEM_CHANNEL_NUM;
	for (int j = 0; j < DEF_MATROX_SYSTEM_CHANNEL_NUM; j++)
	{
		int iUserCamNo = getFirstCamNoPerChannel(j);
		if (iUserCamNo == -1)	// 해당 Channel 에서 사용하는 Camera 가 없는 경우.
			continue;
		
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		// Digitizer Allocation
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		char szDCF[256];
		memset(szDCF, NULL, sizeof(szDCF));
		sprintf(szDCF, "%s\\%s",
			(*m_pvectorCamData)[iUserCamNo].m_pszDCFPath, (*m_pvectorCamData)[iUserCamNo].m_pszDCFName);
		
		// DCF 유무 확인
		if (FALSE == PathFileExistsA(szDCF))
			return ERR_DCF_FILE_DOES_NOT_EXIST;
		
		MdigAlloc(m_lSysID, M_DEV0, szDCF, M_DEFAULT, &m_rglDigID[j]);
		if (m_rglDigID[j] == M_NULL)
			return ERR_DIGITIZER_ALLOC_FAILURE;

		// Grab Mode : SYNCHRONOUS MODE

		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		// Grab 관련 Buffer Allocation
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		// 해당 Meteor II m/c System 의 해당 Channel 에서 사용하는 Camera 의 해상도 기준으로
		// Grab Parent Color Buffer Allocation
		int iCamNoChOffset = j * m_iMaxCamPerChannel;	// Channel 변경에 따른 내부 Camera 번호 Offset
		long lSizeX = (*m_pvectorCamData)[iUserCamNo].m_lBufferSizeX;
		long lSizeY = (*m_pvectorCamData)[iUserCamNo].m_lBufferSizeY;
		
		MbufAllocColor(m_lSysID, 3L, lSizeX, lSizeY, 8L+M_UNSIGNED,
			M_DISP+M_RGB24+M_PLANAR+M_IMAGE+M_PROC+M_GRAB, &m_rglGrabParent[j]);
		if (m_rglGrabParent[j] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;	// 확인 필요 : 어느 버퍼인지 알 필요 있다.
		
		// Grab Buffer Allocation : Child Buffer
		MbufChildColor(m_rglGrabParent[j], M_RED, &m_rglGrab[0 + iCamNoChOffset]);
		if (m_rglGrab[0 + j * m_iMaxCamPerChannel] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
		
		MbufChildColor(m_rglGrabParent[j], M_GREEN, &m_rglGrab[1 + iCamNoChOffset]);
		if (m_rglGrab[1 + j * m_iMaxCamPerChannel] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
		
		MbufChildColor(m_rglGrabParent[j], M_BLUE, &m_rglGrab[2 + iCamNoChOffset]);
		if (m_rglGrab[2 + j * m_iMaxCamPerChannel] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
	}
	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Source Image Buffer Allocation
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	m_rglSource = new long[m_iMaxCamNum];
	
	for (int i = 0; i < m_iMaxCamNum; i++)
	{
		m_rglSource[i] = M_NULL;
		
		long lSizeX = 0;
		long lSizeY = 0;
		
		if (m_rglGrab[i] == M_NULL)
			continue;
		
		MbufInquire(m_rglGrab[i], M_SIZE_X, &lSizeX);
		MbufInquire(m_rglGrab[i], M_SIZE_Y, &lSizeY);
		
		// 확인 필요 : Size X, Y 값 유효성 검토
		
		MbufAlloc2d(m_lSysID, lSizeX, lSizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+M_DIB, &m_rglSource[i]);
		if (m_rglSource[i] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
		
		MbufClear(m_rglSource[i], 0x0);
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Digitizer Configuration : Hook Start
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━			
	int iSysCamNo = -1;
	int iUserCamNo = -1;
	int iCamNoChOffset = -1;
	int iWholeCameraNo = (*m_pvectorCamData).size();	// 전체 사용 Camera 개수
	for (int j = 0; j < DEF_MATROX_SYSTEM_CHANNEL_NUM; j++)
	{
		m_pHookData[j].plGrabR = &(m_rglGrab[0 + j * m_iMaxCamPerChannel]);
		m_pHookData[j].plGrabG = &(m_rglGrab[1 + j * m_iMaxCamPerChannel]);
		m_pHookData[j].plGrabB = &(m_rglGrab[2 + j * m_iMaxCamPerChannel]);
		m_pHookData[j].lSourceR = m_rglSource[0 + j * m_iMaxCamPerChannel];
		m_pHookData[j].lSourceG = m_rglSource[1 + j * m_iMaxCamPerChannel];
		m_pHookData[j].lSourceB = m_rglSource[2 + j * m_iMaxCamPerChannel];
		
		iCamNoChOffset = j * m_iMaxCamPerChannel;
		for (int i = 0; i < iWholeCameraNo; i++)
		{
			int iInternalCamNo = (*m_pvectorCamData)[i].m_iInternalCameraNo;
			if (iInternalCamNo < m_iStartCamNo + iCamNoChOffset
				|| iInternalCamNo >= m_iStartCamNo + iCamNoChOffset + m_iMaxCamPerChannel)
				continue;
			
			iSysCamNo = iInternalCamNo - m_iStartCamNo;
			iUserCamNo = (*m_pvectorCamData)[i].m_iUserCameraNo;
			switch (iSysCamNo % m_iMaxCamPerChannel)
			{
			case 0:
				m_pHookData[j].pDataR = &((*m_pvectorCamData)[iUserCamNo]);
				break;
			case 1:
				m_pHookData[j].pDataG = &((*m_pvectorCamData)[iUserCamNo]);
				break;
			case 2:
				m_pHookData[j].pDataB = &((*m_pvectorCamData)[iUserCamNo]);
				break;
			}
		}

		if (m_rglDigID[j] != M_NULL)
		{
			MdigHookFunction(m_rglDigID[j], M_GRAB_END, hookHandler, &(m_pHookData[j]));
			// 확인 필요 : 아래 Hook Counter 를 MdigHookFunction 와 하나로 완전히 묶을 수 없나? 현재는 프로그래머가 실수할 수 있다.
			(m_rgiDigHookCount[j])++;
		}
	}
	
	return ERR_VISION_SUCCESS;
}

long Msystem_meteor::getDigitizer(int iSysCamNo)
{
	if (iSysCamNo < m_iMaxCamPerChannel)
		return m_rglDigID[0];
	
	return m_rglDigID[1];
}

int Msystem_meteor::ChangeCamera(int iCamNo, long lCameraChangeTime)
{
	int iSysCamNo = (*m_pvectorCamData)[iCamNo].m_iInternalCameraNo - m_iStartCamNo;
	int iChNo = (iSysCamNo < m_iMaxCamPerChannel) ? 0 : 1;
	if (m_iOldChNo == iChNo)
		return ERR_VISION_SUCCESS;

	m_iOldChNo = iChNo;
	long lCh = (iChNo == 0) ? M_CH0 : M_CH1;
	MdigChannel(m_rglDigID[iChNo], lCh);

	if (lCameraChangeTime > 0)
		Sleep(lCameraChangeTime);

	return ERR_VISION_SUCCESS;
}

int Msystem_meteor::Grab(SCameraData* pCamData)
{
	int iSysCamNo = pCamData->m_iInternalCameraNo - m_iStartCamNo;
	long lDigitizer = getDigitizer(iSysCamNo);
	int iChNo = (iSysCamNo < m_iMaxCamPerChannel) ? 0 : 1;

	MdigGrab(lDigitizer, m_rglGrabParent[iChNo]);
	return ERR_VISION_SUCCESS;
}

void Msystem_meteor::GrabContinuous(SCameraData* pCamData)
{
	// 확인 필요 : live 했다가 종료하면 MdisplayThread 에서 메모리 릭 발생.


	int iSysCamNo = pCamData->m_iInternalCameraNo - m_iStartCamNo;
	long lDigitizer = getDigitizer(iSysCamNo);
	int iChNo = (iSysCamNo < m_iMaxCamPerChannel) ? 0 : 1;

	if (m_rgiDigHookCount[iChNo] == 1)
	{
		MdigHookFunction(lDigitizer, M_GRAB_END+M_UNHOOK, hookHandler, &(m_pHookData[iChNo]));
		(m_rgiDigHookCount[iChNo])--;
	}
	else
	{
		// 확인 필요 : Error 상황임. 2 이상의 값을 가질 수 없음.
		TRACE2("GrabContinuous : m_rgiDigHookCount[%d] = %d\n", iChNo, m_rgiDigHookCount[iChNo]);
	}

	MdigHookFunction(lDigitizer, M_GRAB_FRAME_END, hookLiveHandler, &(m_pHookData[iChNo]));

	MdigGrabContinuous(lDigitizer, m_rglGrabParent[iChNo]);
}

void Msystem_meteor::Halt(SCameraData* pCamData)
{
	int iSysCamNo = pCamData->m_iInternalCameraNo - m_iStartCamNo;
	long lDigitizer = getDigitizer(iSysCamNo);
	int iChNo = (iSysCamNo < m_iMaxCamPerChannel) ? 0 : 1;

	MdigHookFunction(lDigitizer, M_GRAB_FRAME_END+M_UNHOOK, hookLiveHandler, &(m_pHookData[iChNo]));

	MdigHalt(lDigitizer);

/* 확인 필요 : 삭제 예정. : Image Display 등에서 Halt 확인 동작에서 추가적인 Grab 이 일어나서 원치 않는 배경 이미지 나오는 경우 있음.
	// Live 종료 시, 마지막 Image 를 원하는 위치(해당 Camera Grab Buffer List 첫번째 위치)에 저장.
*/

	// 확인 필요 : Hooking 한데다가 또 Hooking 하면 어떻게 되나?
	// Debugging 결과 Hooking 계속 걸림. 횟수만큼 Hook Handler 를 중복 호출 함.
	if (m_rgiDigHookCount[iChNo] > 0)
	{
		// 확인 필요 : Error 상황임. 1 이상의 값을 가질 수 없음.
		TRACE2("Halt : m_rgiDigHookCount[%d] = %d\n", iChNo, m_rgiDigHookCount[iChNo]);
		return;
	}

	MdigHookFunction(lDigitizer, M_GRAB_END, hookHandler, &(m_pHookData[iChNo]));
	(m_rgiDigHookCount[iChNo])++;
}

void Msystem_meteor::FreeAllDigitizer()
{
	// Precondition : Mark Data Memory 영역 해제 전에 호출되어야 함.

	for (int i = 0; i < DEF_MATROX_SYSTEM_CHANNEL_NUM; i++)
	{
		if (m_rglDigID[i] != M_NULL)
		{
			if (m_rgiDigHookCount[i] == 1)	// Grab Mode
			{
				MdigHookFunction(m_rglDigID[i], M_GRAB_END+M_UNHOOK, hookHandler, &(m_pHookData[i]));
				(m_rgiDigHookCount[i])--;
			}
			else if (m_rgiDigHookCount[i] == 0)	// Continuous Grab Mode
			{
				MdigHookFunction(m_rglDigID[i], M_GRAB_FRAME_END+M_UNHOOK, hookLiveHandler, &(m_pHookData[i]));
				MdigHalt(m_rglDigID[i]);
			}
			else
			{
				// 확인 필요 : Error 상황임. 0, 1 이외의 값을 가질 수 없음.
				TRACE2("FreeAllDigitizer : m_rgiDigHookCount[%d] = %d\n", i, m_rgiDigHookCount[i]);
				return;	// Error Case
			}
			MdigFree(m_rglDigID[i]);
		}
	}
	delete[] m_rglDigID;
	delete[] m_pHookData;
	
	if (m_rgiDigHookCount != NULL)
		delete[] m_rgiDigHookCount;
}

int Msystem_meteor::getFirstCamNoPerChannel(int iChID)
{
	int iUserCamNo = -1;
	int iCamNoChOffset = iChID * m_iMaxCamPerChannel;
	int iWholeCameraNo = (*m_pvectorCamData).size();	// 전체 사용 Camera 개수
	for (int i = 0; i < iWholeCameraNo; i++)
	{
		int iInternalCamNo = (*m_pvectorCamData)[i].m_iInternalCameraNo;
		if (iInternalCamNo < m_iStartCamNo + iCamNoChOffset
			|| iInternalCamNo >= m_iStartCamNo + iCamNoChOffset + m_iMaxCamPerChannel)
			continue;
		
		// 확인 필요 : Cam 번호
		iUserCamNo = (*m_pvectorCamData)[i].m_iUserCameraNo;
		break;
	}
	return iUserCamNo;
}

long MFTYPE Msystem_meteor::hookHandler(long lHookType, long lEventID, void MPTYPE *pUserData)
{
	SMeteorHookData* pHookData = (SMeteorHookData*)pUserData;

	if (pHookData->pDataR)
	{
		EnterCriticalSection(&(pHookData->pDataR->m_csSource));
		MbufCopy(*(pHookData->plGrabR), pHookData->lSourceR);
		LeaveCriticalSection(&(pHookData->pDataR->m_csSource));

		if (NULL != pHookData->pDataR->m_hWnd)
			::PostMessage(pHookData->pDataR->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData->pDataR, NULL);
	}

	if (pHookData->pDataG)
	{
		EnterCriticalSection(&(pHookData->pDataG->m_csSource));
		MbufCopy(*(pHookData->plGrabG), pHookData->lSourceG);
		LeaveCriticalSection(&(pHookData->pDataG->m_csSource));

		if (NULL != pHookData->pDataG->m_hWnd)
			::PostMessage(pHookData->pDataG->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData->pDataG, NULL);
	}

	if (pHookData->pDataB)
	{
		EnterCriticalSection(&(pHookData->pDataB->m_csSource));
		MbufCopy(*(pHookData->plGrabB), pHookData->lSourceB);
		LeaveCriticalSection(&(pHookData->pDataB->m_csSource));

		if (NULL != pHookData->pDataB->m_hWnd)
			::PostMessage(pHookData->pDataB->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData->pDataB, NULL);
	}
	return 0;
}

long MFTYPE Msystem_meteor::hookLiveHandler(long lHookType, long lEventID, void MPTYPE *pUserData)
{
	SMeteorHookData* pHookData = (SMeteorHookData*)pUserData;

	long lModifiedIndex = 0;

	// Retrieve the MIL_ID of the grabbed buffer.
//	MdigGetHookInfo(lEventID, M_MODIFIED_BUFFER+M_BUFFER_INDEX, &lModifiedIndex);

	if (pHookData->pDataR && pHookData->pDataR->m_hWnd)
	{
		EnterCriticalSection(&(pHookData->pDataR->m_csSource));
		MbufCopy((pHookData->plGrabR)[lModifiedIndex], pHookData->lSourceR);
		LeaveCriticalSection(&(pHookData->pDataR->m_csSource));

		if (NULL != pHookData->pDataR->m_hWnd)
			::PostMessage(pHookData->pDataR->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData->pDataR, NULL);
	}

	if (pHookData->pDataG && pHookData->pDataG->m_hWnd)
	{
		EnterCriticalSection(&(pHookData->pDataG->m_csSource));
		MbufCopy((pHookData->plGrabG)[lModifiedIndex], pHookData->lSourceG);
		LeaveCriticalSection(&(pHookData->pDataG->m_csSource));

		if (NULL != pHookData->pDataG->m_hWnd)
			::PostMessage(pHookData->pDataG->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData->pDataG, NULL);
	}

	if (pHookData->pDataB && pHookData->pDataB->m_hWnd)
	{
		EnterCriticalSection(&(pHookData->pDataB->m_csSource));
		MbufCopy((pHookData->plGrabB)[lModifiedIndex], pHookData->lSourceB);
		LeaveCriticalSection(&(pHookData->pDataB->m_csSource));

		if (NULL != pHookData->pDataB->m_hWnd)
			::PostMessage(pHookData->pDataB->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData->pDataB, NULL);
	}
	return 0;
}