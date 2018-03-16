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

	// Ȯ�� �ʿ� : Log �� ����.
	TRACE(_T("VisionSystem_%d Destruction is Completed.\n"), m_iInstanceNo);
}

int Msystem_meteor::AllocSystem(SSystemData* pSystemData)
{
	initMembers(pSystemData);
	
	//��������������������������������������������������������������������������������������
	// Vision System Type �� ���� System ID Allocation
	//��������������������������������������������������������������������������������������
	m_iMaxCamNum = DEF_METEOR_SYSTEM_MAX_CAM_NUM;
	long lSystemNo = static_cast<long>(m_iInstanceNo);
	m_lSysID = MsysAlloc(M_SYSTEM_METEOR_II, lSystemNo, M_COMPLETE, M_NULL);
	if (m_lSysID == M_NULL)
		return ERR_SYSTEM_ALLOC_FAILURE;

	//��������������������������������������������������������������������������������������
	// Digitizer, Buffer Allocation
	//��������������������������������������������������������������������������������������
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
		if (iUserCamNo == -1)	// �ش� Channel ���� ����ϴ� Camera �� ���� ���.
			continue;
		
		//��������������������������������������������������������������������������������������
		// Digitizer Allocation
		//��������������������������������������������������������������������������������������
		char szDCF[256];
		memset(szDCF, NULL, sizeof(szDCF));
		sprintf(szDCF, "%s\\%s",
			(*m_pvectorCamData)[iUserCamNo].m_pszDCFPath, (*m_pvectorCamData)[iUserCamNo].m_pszDCFName);
		
		// DCF ���� Ȯ��
		if (FALSE == PathFileExistsA(szDCF))
			return ERR_DCF_FILE_DOES_NOT_EXIST;
		
		MdigAlloc(m_lSysID, M_DEV0, szDCF, M_DEFAULT, &m_rglDigID[j]);
		if (m_rglDigID[j] == M_NULL)
			return ERR_DIGITIZER_ALLOC_FAILURE;

		// Grab Mode : SYNCHRONOUS MODE

		//��������������������������������������������������������������������������������������
		// Grab ���� Buffer Allocation
		//��������������������������������������������������������������������������������������
		// �ش� Meteor II m/c System �� �ش� Channel ���� ����ϴ� Camera �� �ػ� ��������
		// Grab Parent Color Buffer Allocation
		int iCamNoChOffset = j * m_iMaxCamPerChannel;	// Channel ���濡 ���� ���� Camera ��ȣ Offset
		long lSizeX = (*m_pvectorCamData)[iUserCamNo].m_lBufferSizeX;
		long lSizeY = (*m_pvectorCamData)[iUserCamNo].m_lBufferSizeY;
		
		MbufAllocColor(m_lSysID, 3L, lSizeX, lSizeY, 8L+M_UNSIGNED,
			M_DISP+M_RGB24+M_PLANAR+M_IMAGE+M_PROC+M_GRAB, &m_rglGrabParent[j]);
		if (m_rglGrabParent[j] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;	// Ȯ�� �ʿ� : ��� �������� �� �ʿ� �ִ�.
		
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
	
	//��������������������������������������������������������������������������������������
	// Source Image Buffer Allocation
	//��������������������������������������������������������������������������������������
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
		
		// Ȯ�� �ʿ� : Size X, Y �� ��ȿ�� ����
		
		MbufAlloc2d(m_lSysID, lSizeX, lSizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+M_DIB, &m_rglSource[i]);
		if (m_rglSource[i] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
		
		MbufClear(m_rglSource[i], 0x0);
	}

	//��������������������������������������������������������������������������������������
	// Digitizer Configuration : Hook Start
	//��������������������������������������������������������������������������������������			
	int iSysCamNo = -1;
	int iUserCamNo = -1;
	int iCamNoChOffset = -1;
	int iWholeCameraNo = (*m_pvectorCamData).size();	// ��ü ��� Camera ����
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
			// Ȯ�� �ʿ� : �Ʒ� Hook Counter �� MdigHookFunction �� �ϳ��� ������ ���� �� ����? ����� ���α׷��Ӱ� �Ǽ��� �� �ִ�.
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
	// Ȯ�� �ʿ� : live �ߴٰ� �����ϸ� MdisplayThread ���� �޸� �� �߻�.


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
		// Ȯ�� �ʿ� : Error ��Ȳ��. 2 �̻��� ���� ���� �� ����.
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

/* Ȯ�� �ʿ� : ���� ����. : Image Display ��� Halt Ȯ�� ���ۿ��� �߰����� Grab �� �Ͼ�� ��ġ �ʴ� ��� �̹��� ������ ��� ����.
	// Live ���� ��, ������ Image �� ���ϴ� ��ġ(�ش� Camera Grab Buffer List ù��° ��ġ)�� ����.
*/

	// Ȯ�� �ʿ� : Hooking �ѵ��ٰ� �� Hooking �ϸ� ��� �ǳ�?
	// Debugging ��� Hooking ��� �ɸ�. Ƚ����ŭ Hook Handler �� �ߺ� ȣ�� ��.
	if (m_rgiDigHookCount[iChNo] > 0)
	{
		// Ȯ�� �ʿ� : Error ��Ȳ��. 1 �̻��� ���� ���� �� ����.
		TRACE2("Halt : m_rgiDigHookCount[%d] = %d\n", iChNo, m_rgiDigHookCount[iChNo]);
		return;
	}

	MdigHookFunction(lDigitizer, M_GRAB_END, hookHandler, &(m_pHookData[iChNo]));
	(m_rgiDigHookCount[iChNo])++;
}

void Msystem_meteor::FreeAllDigitizer()
{
	// Precondition : Mark Data Memory ���� ���� ���� ȣ��Ǿ�� ��.

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
				// Ȯ�� �ʿ� : Error ��Ȳ��. 0, 1 �̿��� ���� ���� �� ����.
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
	int iWholeCameraNo = (*m_pvectorCamData).size();	// ��ü ��� Camera ����
	for (int i = 0; i < iWholeCameraNo; i++)
	{
		int iInternalCamNo = (*m_pvectorCamData)[i].m_iInternalCameraNo;
		if (iInternalCamNo < m_iStartCamNo + iCamNoChOffset
			|| iInternalCamNo >= m_iStartCamNo + iCamNoChOffset + m_iMaxCamPerChannel)
			continue;
		
		// Ȯ�� �ʿ� : Cam ��ȣ
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