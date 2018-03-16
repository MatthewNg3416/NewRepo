#include "stdafx.h"
#include "Msystem_solios.h"
#include <string>

#include "common.h"

BOOL Msystem_solios::m_bDisplayImg = FALSE;

Msystem_solios::Msystem_solios() : m_rgiDigGrabHookCount(NULL), m_rgiDigLiveHookCount(NULL)
{
	m_iSystemType = DEF_SOLIOS;

	// ���� Input Path �� �����ϴ� �ٸ� Camera ��ȣ ���� Array �ʱ�ȭ
	m_rgiPairCamNo[0] = 4;
	m_rgiPairCamNo[1] = 5;
	m_rgiPairCamNo[2] = 6;
	m_rgiPairCamNo[3] = 7;
	m_rgiPairCamNo[4] = 0;
	m_rgiPairCamNo[5] = 1;
	m_rgiPairCamNo[6] = 2;
	m_rgiPairCamNo[7] = 3;

	m_rglCamCh[0] = M_CH0;
	m_rglCamCh[1] = M_CH0;
	m_rglCamCh[2] = M_CH0;
	m_rglCamCh[3] = M_CH0;
	m_rglCamCh[4] = M_CH1;
	m_rglCamCh[5] = M_CH1;
	m_rglCamCh[6] = M_CH1;
	m_rglCamCh[7] = M_CH1;

	m_rglDeviceNo[0] = M_DEV0;	// DVI0 - 1st Sync ���	: HW Cable �ἱ�� �����ϴ� ������.
	m_rglDeviceNo[1] = M_DEV1;	// DVI0 - 2nd Sync ���
	m_rglDeviceNo[2] = M_DEV2;	// DVI1 - 1st Sync ���
	m_rglDeviceNo[3] = M_DEV3;	// DVI1 - 2nd Sync ���
	m_rglDeviceNo[4] = M_DEV0;	// DVI0 - 1st Sync ���
	m_rglDeviceNo[5] = M_DEV1;	// DVI0 - 2nd Sync ���
	m_rglDeviceNo[6] = M_DEV2;	// DVI1 - 1st Sync ���
	m_rglDeviceNo[7] = M_DEV3;	// DVI1 - 2nd Sync ���

	m_bDisplayImg = FALSE;

	for (int i = 0; i < DEF_MATROX_SOLIOS_INDEPENDENT_INPUT_PATH_NUM; i++)
		InitializeCriticalSection(&m_csDataInput[i]);
}

Msystem_solios::~Msystem_solios()
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

	if (m_lSysID != M_NULL)
		MsysFree(m_lSysID);

	for (int i = 0; i < DEF_MATROX_SOLIOS_INDEPENDENT_INPUT_PATH_NUM; i++)
		DeleteCriticalSection(&m_csDataInput[i]);

	// Ȯ�� �ʿ� : Log �� ����.
	TRACE(_T("VisionSystem_%d Destruction is Completed.\n"), m_iInstanceNo);
}

int Msystem_solios::AllocSystem(SSystemData* pSystemData)
{
	initMembers(pSystemData);

	//��������������������������������������������������������������������������������������
	// Vision System Type �� ���� System ID Allocation
	//��������������������������������������������������������������������������������������
	m_iMaxCamNum = DEF_SOLIOS_SYSTEM_MAX_CAM_NUM;
	long lSystemNo = static_cast<long>(m_iInstanceNo);
	m_lSysID = MsysAlloc(M_SYSTEM_SOLIOS, lSystemNo, M_COMPLETE, M_NULL);
	if (m_lSysID == M_NULL)
		return ERR_SYSTEM_ALLOC_FAILURE;

	//��������������������������������������������������������������������������������������
	// System Configuration
	//��������������������������������������������������������������������������������������
	// Solios System Type ������ ��ȿ.
	if (pSystemData->m_iCpuCount > 1)
		MsysControl(m_lSysID, M_MODIFIED_BUFFER_HOOK_MODE, M_MULTI_THREAD + pSystemData->m_iCpuCount);

	//��������������������������������������������������������������������������������������
	// Digitizer, Buffer Allocation
	//��������������������������������������������������������������������������������������
	int iWholeCameraNo = (*m_pvectorCamData).size();	// ��ü ��� Camera ����
	int iInternalCamNoPerSystem = -1;
	int iInternalCamNo = -1;
	
	char szDCF[256];
	m_rglDigID = new long[m_iMaxCamNum];
	m_rgiDigGrabHookCount = new int[m_iMaxCamNum];
	m_rgiDigLiveHookCount = new int[m_iMaxCamNum];
	m_rglSource = new long[m_iMaxCamNum];
	m_rglGrab = new long[m_iMaxCamNum];
	
	for (int i = 0; i < m_iMaxCamNum; i++)
	{
		m_rglDigID[i] = M_NULL;
		m_rgiDigGrabHookCount[i] = 0;
		m_rgiDigLiveHookCount[i] = 0;
		m_rglSource[i] = M_NULL;
		m_rglGrab[i] = M_NULL;
		
		int j;
		for (j = 0; j < iWholeCameraNo; j++)
		{
			iInternalCamNo = (*m_pvectorCamData)[j].m_iInternalCameraNo;
			if (iInternalCamNo < m_iStartCamNo || iInternalCamNo >= m_iStartCamNo + m_iMaxCamNum)
				continue;
			
			iInternalCamNoPerSystem = iInternalCamNo - m_iStartCamNo;
			if (iInternalCamNoPerSystem == i)
				break;
		}

		if (j == iWholeCameraNo)
			continue;
		
		int iUserCamNo = (*m_pvectorCamData)[j].m_iUserCameraNo;
		ASSERT(iUserCamNo == j);
		
		//��������������������������������������������������������������������������������������
		// Digitizer Allocation
		//��������������������������������������������������������������������������������������
		memset(szDCF, NULL, sizeof(szDCF));
		sprintf(szDCF, "%s\\%s",
			(*m_pvectorCamData)[iUserCamNo].m_pszDCFPath, (*m_pvectorCamData)[iUserCamNo].m_pszDCFName);
		
		// DCF ���� Ȯ��
		if (FALSE == PathFileExistsA(szDCF))
			return ERR_DCF_FILE_DOES_NOT_EXIST;
		
		MdigAlloc(m_lSysID, m_rglDeviceNo[i], szDCF, M_DEFAULT, &m_rglDigID[i]);
		if (m_rglDigID[i] == M_NULL)
		{
			// Ȯ�� �ʿ�
			char szError[M_ERROR_MESSAGE_SIZE];
			long lError = MappGetError(M_CURRENT+M_MESSAGE, szError);
			return ERR_DIGITIZER_ALLOC_FAILURE;
		}

		// Grab Time Out �ð� ���� : 3��.
		MdigControl(m_rglDigID[i], M_GRAB_TIMEOUT, 3000);

		// Sync �� ���� Camera Lock ���� : Sync �ϳ��� �� ���� �����ؾ� ��.
		if (i < DEF_MATROX_SOLIOS_INDEPENDENT_INPUT_PATH_NUM)
			MdigControl(m_rglDigID[i], M_CAMERA_LOCK, M_ENABLE);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
		//��������������������������������������������������������������������������������������
		// Digitizer Configuration
		//��������������������������������������������������������������������������������������			
		// Hook Start
		MdigHookFunction(m_rglDigID[i], M_GRAB_END, hookHandler, &((*m_pvectorCamData)[iUserCamNo]));
		(m_rgiDigGrabHookCount[i])++;

		// Grab Mode ���� : ASYNCHRONOUS MODE
		MdigControl(m_rglDigID[i], M_GRAB_MODE, M_ASYNCHRONOUS);
		
		//��������������������������������������������������������������������������������������
		// Grab ���� Buffer Allocation
		//��������������������������������������������������������������������������������������			
		long lSizeX = (*m_pvectorCamData)[iUserCamNo].m_lBufferSizeX;
		long lSizeY = (*m_pvectorCamData)[iUserCamNo].m_lBufferSizeY;
		
		MbufAlloc2d(m_lSysID, lSizeX, lSizeY, 8L+M_UNSIGNED,
			M_IMAGE+M_GRAB+M_PROC+M_DISP, &m_rglGrab[iInternalCamNoPerSystem]);
		if (m_rglGrab[iInternalCamNoPerSystem] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
		
		MbufClear(m_rglGrab[iInternalCamNoPerSystem], 0x0);
		
		//��������������������������������������������������������������������������������������
		// Source Image Buffer ID Initialize
		//��������������������������������������������������������������������������������������
		MbufAlloc2d(m_lSysID, lSizeX, lSizeY, 8L+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+M_DIB, &m_rglSource[i]);
		if (m_rglSource[i] == M_NULL)
			return ERR_BUFFER_ALLOC_FAILURE;
		
		MbufClear(m_rglSource[i], 0x0);
	}
	
	return ERR_VISION_SUCCESS;
}

int Msystem_solios::ChangeCamera(int iCamNo, long lCameraChangeTime)
{
	// Ȯ�� �ʿ� : �Ʒ� �ڵ� ���� ���� �ϴ��� Ȯ�� �� ��.

	int iSysCamNo = ((*m_pvectorCamData)[iCamNo]).m_iInternalCameraNo - m_iStartCamNo;

	int iWholeCamNo = (*m_pvectorCamData).size();
	const int iPairInternalCamNo = m_rgiPairCamNo[iSysCamNo] + m_iStartCamNo;
	int iPairUserCamNo = -1;
	for (int i = 0; i < iWholeCamNo; i++)
	{
		if (iPairInternalCamNo == ((*m_pvectorCamData)[i]).m_iInternalCameraNo)
		{
			iPairUserCamNo = ((*m_pvectorCamData)[i]).m_iUserCameraNo;
		}
	}

	if (-1 == iPairUserCamNo)
		return ERR_VISION_SUCCESS;
	
	if (m_rgiDigLiveHookCount[m_rgiPairCamNo[iSysCamNo]] == 1)	// Pair Camera Live ������ ���.
		Halt(&(*m_pvectorCamData)[iPairUserCamNo]);
	else
	{
		int iResult = WaitGrabComplete(iPairUserCamNo);
		if (iResult)
			return iResult;
	}

	long lDigitizer = getDigitizer(iSysCamNo);

	MdigChannel(lDigitizer, m_rglCamCh[iSysCamNo] + M_SIGNAL);
	MdigChannel(lDigitizer, m_rglCamCh[iSysCamNo] + M_SYNC);
	return ERR_VISION_SUCCESS;
}

int Msystem_solios::Grab(SCameraData* pCamData)
{
	int iResult = WaitGrabComplete(pCamData->m_iUserCameraNo);
	if (iResult)
		return iResult;

	int iSysCamNo = pCamData->m_iInternalCameraNo - m_iStartCamNo;
	long lDigitizer = getDigitizer(iSysCamNo);

	MdigGrab(lDigitizer, m_rglGrab[iSysCamNo]);
	return ERR_VISION_SUCCESS;
}

void Msystem_solios::GrabContinuous(SCameraData* pCamData)
{
	int iSysCamNo = pCamData->m_iInternalCameraNo - m_iStartCamNo;
	long lDigitizer = getDigitizer(iSysCamNo);

	EnterCriticalSection(&(pCamData->m_csSource));
	LeaveCriticalSection(&(pCamData->m_csSource));

	if (m_rgiDigGrabHookCount[iSysCamNo] == 1)
	{
		MdigHookFunction(lDigitizer, M_GRAB_END+M_UNHOOK, hookHandler, pCamData);
		(m_rgiDigGrabHookCount[iSysCamNo])--;
	}
	else
	{
		// Ȯ�� �ʿ� : Error ó�� �ʿ�.
		; ///TRACE2("GrabContinuous : m_rgiDigGrabHookCount[%d] = %d\n", iSysCamNo, m_rgiDigGrabHookCount[iSysCamNo]);
	}

	if (m_rgiDigLiveHookCount[iSysCamNo] == 0)
	{
		MdigHookFunction(lDigitizer, M_GRAB_FRAME_END, hookLiveHandler, pCamData);
		(m_rgiDigLiveHookCount[iSysCamNo])++;

		if (m_rgiDigGrabHookCount[iSysCamNo] == 0)
			MdigGrabContinuous(lDigitizer, m_rglGrab[iSysCamNo]);
		//  Ȯ�� �ʿ� : �ӽ� ó��.
		else
			TRACE3("Unusual Hook Count [CamNo : %d] [SysCamNo : %d] [count : %d]\n", pCamData->m_iUserCameraNo, iSysCamNo, m_rgiDigGrabHookCount[iSysCamNo]);
	}
	else
	{
		// Ȯ�� �ʿ� : Error ó�� �ʿ�.
		; //TRACE2("GrabContinuous : m_rgiDigLiveHookCount[%d] = %d\n", iSysCamNo, m_rgiDigLiveHookCount[iSysCamNo]);
	}
}

void Msystem_solios::Halt(SCameraData* pCamData)
{
	int iSysCamNo = pCamData->m_iInternalCameraNo - m_iStartCamNo;
	long lDigitizer = getDigitizer(iSysCamNo);
	MdigHalt(lDigitizer);

	// Ȯ�� �ʿ� : ���̺� ���� ���� ���¿��� Halt �ɸ� ���� �ȳ��� ó�� �ʿ�.
	//EnterCriticalSection(&(pCamData->m_csSource));

	if (m_rgiDigLiveHookCount[iSysCamNo] == 1)
	{
		MdigHookFunction(lDigitizer, M_GRAB_FRAME_END+M_UNHOOK, hookLiveHandler, pCamData);
		(m_rgiDigLiveHookCount[iSysCamNo])--;
	}
	else
	{
		// Ȯ�� �ʿ� : Error ó�� �ʿ�.
		; ///TRACE2("Halt : m_rgiDigLiveHookCount[%d] = %d\n", iSysCamNo, m_rgiDigLiveHookCount[iSysCamNo]);
	}

	// Ȯ�� �ʿ� : �̹� ������ �޼��� flush ó�� �ʿ�.
	// Ȯ�� �ʿ� : ���� ���� �������� Ȯ��.
	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, pCamData->m_hWnd, 0, 0, PM_REMOVE))
		{
			// Ȯ�� �ʿ� : WM_QUIT �� �Ծ������ ���� �ȵǴ� �� �ƴѰ�?
			if (msg.message == WM_QUIT)
			{
				SendMessage(pCamData->m_hWnd, WM_QUIT, 0, 0);
				break;
			}
		}
		else
			break;
	}

	if (m_rgiDigGrabHookCount[iSysCamNo] == 0)
	{
		MdigHookFunction(lDigitizer, M_GRAB_END, hookHandler, pCamData);
		(m_rgiDigGrabHookCount[iSysCamNo])++;
	}
	else
	{
		// Ȯ�� �ʿ� : Error ó�� �ʿ�.
		; ///TRACE2("Halt : m_rgiDigGrabHookCount[%d] = %d\n", iSysCamNo, m_rgiDigGrabHookCount[iSysCamNo]);
	}

	//LeaveCriticalSection(&(pCamData->m_csSource));

	// Ȯ�� �ʿ� : Live ���� Ȯ���� �� �ִ� ��� ã�ƾ� ��.
}

void Msystem_solios::FreeAllDigitizer()
{
	// Precondition : Mark Data Memory ���� ���� ���� ȣ��Ǿ�� ��.

	for (int i = 0; i < m_iMaxCamNum; i++)
	{
		if (m_rglDigID[i] != M_NULL)
		{
			int iUserCamNo = -1;
			int iWholeCamNo = (*m_pvectorCamData).size();
			for (int j = 0; j < iWholeCamNo; j++)
			{
				if ((*m_pvectorCamData)[j].m_iInternalCameraNo - m_iStartCamNo == i)
				{
					iUserCamNo = (*m_pvectorCamData)[j].m_iUserCameraNo;
					break;
				}
			}
			ASSERT(iUserCamNo != -1);
			
			if (m_rgiDigGrabHookCount[i] == 1)
				MdigHookFunction(m_rglDigID[i], M_GRAB_END+M_UNHOOK, hookHandler, &((*m_pvectorCamData)[iUserCamNo]));
			else if (m_rgiDigGrabHookCount[i] > 1)
			{
				// Ȯ�� �ʿ� : Error ��Ȳ��. 1 �̿��� ���� ���� �� ����.
				TRACE2("FreeAllDigitizer : m_rgiDigGrabHookCount[%d] = %d\n", i, m_rgiDigGrabHookCount[i]);
				return;	// Error Case
			}
			
			if (m_rgiDigLiveHookCount[i] == 1)
			{
				MdigHookFunction(m_rglDigID[i], M_GRAB_FRAME_END+M_UNHOOK, hookLiveHandler, &((*m_pvectorCamData)[iUserCamNo]));
				MdigHalt(m_rglDigID[i]);
			}
			else if (m_rgiDigLiveHookCount[i] > 1)
			{
				// Ȯ�� �ʿ� : Error ��Ȳ��. 1 �̿��� ���� ���� �� ����.
				///TRACE2("FreeAllDigitizer : m_rgiDigLiveHookCount[%d] = %d\n", i, m_rgiDigLiveHookCount[i]);
				return;	// Error Case
			}
			MdigFree(m_rglDigID[i]);
		}
	}
	delete[] m_rglDigID;

	if (m_rgiDigGrabHookCount != NULL)
		delete[] m_rgiDigGrabHookCount;

	if (m_rgiDigLiveHookCount != NULL)
		delete[] m_rgiDigLiveHookCount;
}

void Msystem_solios::EnterCameraCriticalSection(int iCamNo)
{
	int iSysCamNo = ((*m_pvectorCamData)[iCamNo]).m_iInternalCameraNo - m_iStartCamNo;
	EnterCriticalSection(&m_csDataInput[m_rglDeviceNo[iSysCamNo]]);

	EnterCriticalSection(&((*m_pvectorCamData)[iCamNo].m_csGrab));
}

void Msystem_solios::LeaveCameraCriticalSection(int iCamNo)
{
	LeaveCriticalSection(&((*m_pvectorCamData)[iCamNo].m_csGrab));

	int iSysCamNo = ((*m_pvectorCamData)[iCamNo]).m_iInternalCameraNo - m_iStartCamNo;
	LeaveCriticalSection(&m_csDataInput[m_rglDeviceNo[iSysCamNo]]);
}

long MFTYPE Msystem_solios::hookHandler(long lHookType, long lEventID, void MPTYPE *pUserData)
{
	SCameraData* pHookData = (SCameraData*)pUserData;

	EnterCriticalSection(&pHookData->m_csSource);
	MbufCopy(pHookData->m_lGrab, pHookData->m_lSource);
	LeaveCriticalSection(&pHookData->m_csSource);

	if (NULL != pHookData->m_hWnd)
		::PostMessage(pHookData->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData, NULL);

	return 0;
}

long MFTYPE Msystem_solios::hookLiveHandler(long lHookType, long lEventID, void MPTYPE *pUserData)
{
	SCameraData* pHookData = (SCameraData*)pUserData;

	EnterCriticalSection(&pHookData->m_csSource);
	MbufCopy(pHookData->m_lGrab, pHookData->m_lSource);
	LeaveCriticalSection(&pHookData->m_csSource);

	if (GetTickCount() % 100 > 50)//100901.KKY.�ӽ�(DisplayȽ�� ����)
	{
		//if (NULL != pHookData->m_hWnd)
		if (NULL != pHookData->m_hWnd && FALSE == m_bDisplayImg)
		{
			::PostMessage(pHookData->m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)pHookData, NULL);
			m_bDisplayImg = TRUE;			
		}
	}
	else
		m_bDisplayImg = FALSE;
	return 0;
}