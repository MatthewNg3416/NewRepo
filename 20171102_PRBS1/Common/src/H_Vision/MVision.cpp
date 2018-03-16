#include "stdafx.h"
#include "ICommon.h"
#include "MVision.h"
#include "Msystem_meteor.h"
#include "Msystem_solios.h"
#include "Mdisplay.h"
#include "MPlatformOlbSystem.h"	//20121004 SJ_HJG 횡전개
#include <sys/timeb.h>
#include <math.h>

#include "common.h"

int g_iDisplayReferenceLine = FALSE;	// Display Reference Line

MVision::MVision() : m_pICommon(NULL), m_lAppID(M_NULL), m_rgiVSystemType(NULL), m_iCpuCount(-1),
	m_iSystemCount(-1), m_iCameraCount(-1), m_iErrorPrint(FALSE), m_iSaveImage(FALSE), m_lCameraChangeTime(0)
{
	m_vectorpDisp.clear();
	m_vectorCamData.clear();
	m_vectorMarkData.clear();

	struct _timeb sTime;
	_ftime(&sTime);
	m_strOldDay.append(ctime(&sTime.time), 0, 10);

	InitializeCriticalSection(&m_csLog);
	InitializeCriticalSection(&m_csVisionLock);
}

MVision::~MVision()
{
	DeleteCriticalSection(&m_csLog);
	DeleteCriticalSection(&m_csVisionLock);
	std::vector<Msystem*>::iterator iter;
	for (iter = m_vectorpSystem.begin(); iter < m_vectorpSystem.end(); iter++)
		(*iter)->FreeAllDigitizer();

	while (m_vectorpDisp.size() > 0)
	{
		delete m_vectorpDisp.back();
		m_vectorpDisp.pop_back();
	}

	removeModelElement(-1);
	removeCameraData(-1);

	while (m_vectorpSystem.size() > 0)
	{
		delete m_vectorpSystem.back();
		m_vectorpSystem.pop_back();
	}

	if (m_rgiVSystemType != NULL)
		delete[] m_rgiVSystemType;

	if (m_lAppID)
		MappFree(m_lAppID);

	if (m_pICommon)
		delete m_pICommon;

	// Trace 는 모두 없애고, 별도의 추적 기능 사용할 것. : 확인 필요
	TRACE(_T("MVision Class Destruction is Completed.\n"));
}

void MVision::CreateCommonObject(SCommonAttribute& commonData)
{
#ifdef DEF_USE_ICOMMON
	m_pICommon = new ICommon(commonData);
#endif
}
#ifdef DEF_USE_ICOMMON
ICommon* MVision::GetCommonObject()
{
	return m_pICommon;
}
#endif

int MVision::getErrorCode(const int iVisionErrorCode, const int iUserCamNo)
{
	if ((DEF_NONE_CAMERA != iUserCamNo && FALSE == IsValidCam(iUserCamNo))
		|| NULL == m_pICommon)
	{
		return iVisionErrorCode;
	}

	int iReturnCode = iVisionErrorCode;
#ifdef DEF_USE_ICOMMON
	int iObjectID = m_pICommon->GetObjectID();	// Vision Object ID
	if (DEF_NONE_CAMERA == iUserCamNo)
		iReturnCode = iObjectID * 1000000 + iVisionErrorCode * 1000 + m_pICommon->GetInstanceNo();
	else
		iReturnCode = iObjectID * 1000000 + iVisionErrorCode * 1000 + (iUserCamNo + 1);
#endif
	return iReturnCode;
}

int MVision::Initialize(const tstring strVisionDataFilePath, const tstring strModelDataFilePath, const tstring strLogDataFilePath, const tstring strImageDataFilePath)
{
	m_strVisionDataFilePath = getSingleCharacterString(strVisionDataFilePath);
	m_strModelDataFilePath = getSingleCharacterString(strModelDataFilePath);
	m_strLogDataFilePath = getSingleCharacterString(strLogDataFilePath);
	m_strImageDataFilePath = getSingleCharacterString(strImageDataFilePath);
	checkUsingFolders();
	return initialize();
}

int MVision::initialize()
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Application Allocation
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	m_lAppID = MappAlloc(M_DEFAULT, M_NULL);
	if (m_lAppID == M_NULL)
		return getErrorCode(ERR_APPLICATION_ALLOC_FAILURE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Data File Load : System Config, Camera
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	loadConfigData();	// Config Data Load
	loadCameraData();	// Cemera Data Load
	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Apply Config Option
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (!m_iErrorPrint)
	{
		MappControl(M_ERROR, M_PRINT_DISABLE);
		MappControl(M_TRACE, M_PRINT_DISABLE);
		MappControl(M_PARAMETER, M_CHECK_DISABLE);
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Msystem Allocation : System, Digitizer Allocation
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	SSystemData sSystemData;
	sSystemData.m_iCpuCount = m_iCpuCount;
	sSystemData.m_pvectorCamData = &m_vectorCamData;
	sSystemData.m_rgiType = m_rgiVSystemType;

#ifndef VISION_SIMULATION
	Msystem* pSystem = NULL;
	for (int i = 0; i < m_iSystemCount; i++)
	{
		sSystemData.m_iInstanceNo = i;

		switch (m_rgiVSystemType[i])
		{
		case DEF_METEOR_II_MC:	pSystem = new Msystem_meteor;	break;
		case DEF_SOLIOS:		pSystem = new Msystem_solios;	break;
		default:
			SetErrorLevel(_T("MVISION initialize"),3, __FILE__, __LINE__);
			return getErrorCode(ERR_INVALID_SYSTEM_TYPE);
		}
		
		int iResult = pSystem->AllocSystem(&sSystemData);
		if (iResult)
		{
			SetErrorLevel(_T("MVISION initialize"),3, __FILE__, __LINE__);
			return getErrorCode(iResult);
		}

		m_vectorpSystem.push_back(pSystem);
	}

	assignCameraDataAboutSystems();	// Camera Data 에 System 관련 Data Assign
#endif

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Vision Mark Allocation (Restore Model)
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	loadMarkRelationData();	//120119.KKY
	loadModelData();	// Vision Mark Load : (Pre-Condition) System Allocation 완료 후 수행.

	return ERR_VISION_SUCCESS;
}

void MVision::assignCameraDataAboutSystems()
{
#ifndef VISION_SIMULATION
	std::vector<SCameraData>::iterator iter;
	for ( iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++)
	{
		(*iter).m_iConnectedSystemID = -1;
		(*iter).m_lSource = M_NULL;
		(*iter).m_lGrab = M_NULL;
		
		for (int j = 0; j < m_iSystemCount; j++)
		{
			if ((*iter).m_iInternalCameraNo >= m_vectorpSystem[j]->GetStartCamNo()
				&& (*iter).m_iInternalCameraNo < m_vectorpSystem[j]->GetLastCamNo())
			{
				(*iter).m_iConnectedSystemID = j;
				(*iter).m_lSource = m_vectorpSystem[j]->GetSourceBuffer((*iter).m_iUserCameraNo);
				(*iter).m_lGrab = m_vectorpSystem[j]->GetGrabBuffer((*iter).m_iUserCameraNo);
			}
		}
	}
#endif
}

inline BOOL MVision::IsValidCam(const int iCamNo) const
{
	return (iCamNo > -1 && iCamNo < (signed)m_vectorCamData.size()) ? TRUE : FALSE;
}

const tstring MVision::GetCameraName(const int iUserCamNo) const
{
	if (IsValidCam(iUserCamNo) == FALSE)
		return _T("Null");

	tstring strCamName;

#ifdef UNICODE
//	_TCHAR wszCamName[100];
	_TCHAR wszCamName[255];

	size_t count = mbstowcs(wszCamName, m_vectorCamData[iUserCamNo].m_pszCameraName, 255);
	if (-1 == count)	// 확인 필요.
		return _T("Null");

	strCamName = wszCamName;
#else
	strCamName = m_vectorCamData[iUserCamNo].m_pszCameraName;
#endif

	return strCamName;
}

inline const int MVision::GetMaxUseCameraNo() const
{
	return m_vectorCamData.size();
}

inline BOOL MVision::isValidMarkNo(const int iModelNo)
{
	return (iModelNo >= 0 && iModelNo < DEF_MAX_USE_MARK);
}

int MVision::getSystemIndex(int iUserCamNo)
{
	// Precondition : iUserCamNo 의 유효성은 이 함수 호출 전에 검증되어야 한다.
	
	int iID = m_vectorCamData[iUserCamNo].m_iConnectedSystemID;
	if (iID < 0 || iID >= m_iSystemCount)
		return -1;

	return iID;
}

HWND MVision::getViewHandle(int iCamNo)
{
	// 1. display priority 검사 : IMAGE_DISPLAY >> TEMPORARY >> LOCAL >> FIXED
	// 2. Fixed 이면 Cam 번호에 맞는 View 선택.

	int iHighPriority = DEF_FIXED_VIEW_PRIORITY;
	
	std::vector<Mdisplay*>::reverse_iterator work;
	std::vector<Mdisplay*>::reverse_iterator ri;
	for (ri = m_vectorpDisp.rbegin(); ri != m_vectorpDisp.rend(); ri++)
	{
		if ((*ri)->GetDisplayPriority() == DEF_IMAGE_DISPLAY_VIEW_PRIORITY)
			continue;
		else if ((*ri)->GetDisplayPriority() < iHighPriority)
		{
			iHighPriority = (*ri)->GetDisplayPriority();
			work = ri;
		}
	}

	HWND hWnd = NULL;
	if (iHighPriority != DEF_FIXED_VIEW_PRIORITY)
		hWnd = (*work)->GetDispWndHandle();
	else
	{
		int iViewID = m_vectorCamData[iCamNo].m_iFixedViewID;

		std::vector<Mdisplay*>::iterator iter;
		for (iter = m_vectorpDisp.begin(); iter < m_vectorpDisp.end(); iter++)
		{
			if ((*iter)->GetViewID() == iViewID)
			{
				hWnd = (*iter)->GetDispWndHandle();
				break;
			}
		}
	}
	return hWnd;
}

void MVision::LockCamera(int iCamNo)
{
//	EnterCriticalSection(&m_csVisionLock);

	int iSystemIndex = getSystemIndex(iCamNo);
	m_vectorpSystem[iSystemIndex]->EnterCameraCriticalSection(iCamNo);
}

void MVision::UnlockCamera(int iCamNo)
{
	int iSystemIndex = getSystemIndex(iCamNo);
	m_vectorpSystem[iSystemIndex]->LeaveCameraCriticalSection(iCamNo);

//	LeaveCriticalSection(&m_csVisionLock);
}

int MVision::Grab(int iCamNo)
{
#ifdef VISION_SIMULATION
	return ERR_VISION_SUCCESS;
#endif

	if (FALSE == IsValidCam(iCamNo))
		return getErrorCode(ERR_INVALID_CAMERA_NO);

	int iSystemIndex = getSystemIndex(iCamNo);
	m_vectorCamData[iCamNo].m_hWnd = getViewHandle(iCamNo);

	int iResult = m_vectorpSystem[iSystemIndex]->ChangeCamera(iCamNo, m_lCameraChangeTime);
	if (iResult)
		return getErrorCode(iResult, iCamNo);

	iResult = m_vectorpSystem[iSystemIndex]->Grab(&(m_vectorCamData[iCamNo]));
	if (iResult)
		return getErrorCode(iResult, iCamNo);

/* 삭제 예정.
	long lDelay = m_vectorCamData[iCamNo].m_lGrabSettlingTime;
	if (lDelay > 0)
		Sleep(lDelay);
*/
	return ERR_VISION_SUCCESS;
}

int MVision::GrabWait(int iCamNo)
{
#ifdef VISION_SIMULATION
	return ERR_VISION_SUCCESS;
#endif

	if (FALSE == IsValidCam(iCamNo))
		return getErrorCode(ERR_INVALID_CAMERA_NO);

	int iSystemIndex = getSystemIndex(iCamNo);
	int iResult = m_vectorpSystem[iSystemIndex]->WaitGrabComplete(iCamNo);
	if (iResult)
		return getErrorCode(iResult, iCamNo);

	return ERR_VISION_SUCCESS;
}

/* 삭제 예정.
int MVision::GetGrabSettlingTime(int iCamNo)
{
	return m_vectorCamData[iCamNo].m_lGrabSettlingTime;
}

void MVision::SetGrabSettlingTime(int iCamNo, int iGrabSettlingTime)
{
	m_vectorCamData[iCamNo].m_lGrabSettlingTime = iGrabSettlingTime;
}
*/

int MVision::GetCameraChangeTime(int iCamNo)
{
	return m_lCameraChangeTime;
}

void MVision::SetCameraChangeTime(int iCamNo, int iCameraChangeTime)
{
	m_lCameraChangeTime = iCameraChangeTime;
}

int MVision::makeDisplay(int iViewID, int iPriority, RECT* pWndPos, CWnd* pParentWnd)
{
	SDisplayAttributeData sData;
	sData.m_iID = iViewID;
	sData.m_iPriority = iPriority;
	sData.m_rectWndPos.left = pWndPos->left;
	sData.m_rectWndPos.top = pWndPos->top;
	sData.m_rectWndPos.right = pWndPos->right;
	sData.m_rectWndPos.bottom = pWndPos->bottom;
	sData.m_pParentWnd = pParentWnd;

	// 이전에 생성된 Display 중 동일 display (동일 Parent, 동일 Size) 가 있는 경우 그 ID 를 Return!
	std::vector<Mdisplay*>::iterator iter;
	for (iter = m_vectorpDisp.begin(); iter < m_vectorpDisp.end(); iter++)
	{
		SDisplayAttributeData rData = (*iter)->GetDisplayAttrib();
		if (rData.m_pParentWnd == sData.m_pParentWnd)
		{
			if (rData.m_rectWndPos.left == sData.m_rectWndPos.left
				&& rData.m_rectWndPos.top == sData.m_rectWndPos.top
				&& rData.m_rectWndPos.right == sData.m_rectWndPos.right
				&& rData.m_rectWndPos.bottom == sData.m_rectWndPos.bottom)
				return rData.m_iID;
		}
	}

	Mdisplay* pDisp = new Mdisplay;
	pDisp->SetDisplayAttrib(&sData);
	pDisp->CreateDisplay();
	m_vectorpDisp.push_back(pDisp);
	return iViewID;
}

int MVision::CreateFixedView(int iViewID, RECT* pWndPos, CWnd* pParentWnd)
{
	return makeDisplay(iViewID, DEF_FIXED_VIEW_PRIORITY, pWndPos, pParentWnd);
}

int MVision::CreateTemporaryView(RECT* pWndPos, CWnd* pParentWnd)
{
	return makeDisplay(m_vectorpDisp.size(), DEF_TEMPORARY_VIEW_PRIORITY, pWndPos, pParentWnd);

	/* 확인 필요. : 종료 등 시 문제 발생 가능성 높음. 기능 구현 세부 검토 필요.
	// 확인 필요 : Temporary View 생성 전에 Live 되고 있는 Camera 의 영상은
	// 별도 처리 필요. - 별도 처리 하게 되면 Destroy View 에서도 처리 필요. 원래 hWnd 복원...

	int iTemporaryViewID = makeDisplay(m_vectorpDisp.size(), DEF_TEMPORARY_VIEW_PRIORITY, pWndPos, pParentWnd);

	std::vector<SCameraData>::iterator iter;
	for (iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++)
	{
		EnterCriticalSection(&((*iter).m_csSource));
		m_vectorOldWndHandle.push_back((*iter).m_hWnd);

		if ((*iter).m_hWnd != NULL)
			(*iter).m_hWnd = m_vectorpDisp[iTemporaryViewID]->GetDispWndHandle();
		LeaveCriticalSection(&((*iter).m_csSource));
	}

	if (m_vectorOldWndHandle.size() != m_vectorCamData.size())
	{
		return -999;	// 확인 필요.
	}

	return iTemporaryViewID;
	*/
}

int MVision::CreateLocalView(RECT* pWndPos, CWnd* pParentWnd)
{
#ifdef VISION_SIMULATION
	return ERR_VISION_SUCCESS;
#endif

	return makeDisplay(m_vectorpDisp.size(), DEF_LOCAL_VIEW_PRIORITY, pWndPos, pParentWnd);
}

int MVision::DestroyView(int iViewID)
{
	if (m_vectorpDisp.size() < 1)
		return ERR_VISION_SUCCESS;

	std::vector<Mdisplay*>::reverse_iterator ri;
	for (ri = m_vectorpDisp.rbegin(); ri != m_vectorpDisp.rend(); ri++)
	{
		if ((*ri)->GetViewID() == iViewID)
		{
			delete *ri;
			m_vectorpDisp.erase((++ri).base());
			break;
		}
	}
	return ERR_VISION_SUCCESS;
}

void MVision::HaltVideo(int iCamNo)
{
#ifdef VISION_SIMULATION
	return;
#endif

	if (IsValidCam(iCamNo) == FALSE)
		return;

	int iSystemIndex = getSystemIndex(iCamNo);
	m_vectorCamData[iCamNo].m_hWnd = getViewHandle(iCamNo);

	m_vectorpSystem[iSystemIndex]->Halt(&(m_vectorCamData[iCamNo]));
}

void MVision::ClearOverlay(int iCamNo)
{
#ifdef VISION_SIMULATION
	return;
#endif
	m_vectorCamData[iCamNo].m_iRectIndex = -1;
	m_vectorCamData[iCamNo].m_iPointIndex = -1;
	m_vectorCamData[iCamNo].m_iTextIndex = -1;
	m_vectorCamData[iCamNo].m_iPointIndex_Measure = -1;
	m_vectorCamData[iCamNo].m_iLineIndex_Measure= -1;
	redraw(iCamNo);
}

int MVision::LiveVideo(int iCamNo)
{
#ifdef VISION_SIMULATION
	return ERR_VISION_SUCCESS;
#endif
	if (IsValidCam(iCamNo) == FALSE)
		return getErrorCode(ERR_INVALID_CAMERA_NO);

	std::vector<SCameraData>::iterator iter;
	for (iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++)
		(*iter).m_hWnd = NULL;

	int iSystemIndex = getSystemIndex(iCamNo);
	m_vectorCamData[iCamNo].m_hWnd = getViewHandle(iCamNo);

	int iResult = m_vectorpSystem[iSystemIndex]->ChangeCamera(iCamNo, m_lCameraChangeTime);
	if (iResult)
		return getErrorCode(iResult, iCamNo);

	m_vectorpSystem[iSystemIndex]->GrabContinuous(&(m_vectorCamData[iCamNo]));

	return ERR_VISION_SUCCESS;
}

int MVision::DisplayPatternImage(int iCamNo, EMarkType eMarkType, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, eMarkType, iModelNo);
	if (iIndex == -1)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	if (FALSE == m_vectorMarkData[iIndex].m_bIsModel)
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);

	// HaltVideo 처리
	ClearOverlay(iCamNo);
	HaltVideo(iCamNo);

	// Mark Type 별 처리.
	if (eMarkType == DEF_PM_MARK)
	{
		SMarkData* pMark = &(m_vectorMarkData[iIndex]);
		long lWidth = pMark->m_lModelRight - pMark->m_lModelLeft;
		long lHeight = pMark->m_lModelBottom - pMark->m_lModelTop;
		
		// Model Image Copy
		long lSource = m_vectorCamData[iCamNo].m_lSource;
		long lModelLeft = (m_vectorCamData[iCamNo].m_lBufferSizeX - lWidth) / 2L;
		long lModelTop = (m_vectorCamData[iCamNo].m_lBufferSizeY - lHeight) / 2L;
		long lModel;
		MbufChild2d(lSource, lModelLeft, lModelTop, lWidth, lHeight, &lModel);
		if (!lModel)
			return getErrorCode(ERR_MODEL_DISPLAY_BUFFER_ALLOCATION_FAIL);
		
		EnterCriticalSection(&(m_vectorCamData[iCamNo].m_csSource));
		MbufClear(lSource, M_COLOR_GRAY);
		MpatCopy(pMark->m_lModelID, lModel, M_DEFAULT);
		LeaveCriticalSection(&(m_vectorCamData[iCamNo].m_csSource));
		MbufFree(lModel);
		
		// Search Area, Model Area, Reference Point Display
		RECT search = { pMark->m_lSearchAreaLeft, pMark->m_lSearchAreaTop, pMark->m_lSearchAreaRight, pMark->m_lSearchAreaBottom };
		setDrawRectData(iCamNo, search);
		
		RECT model = { lModelLeft, lModelTop, lModelLeft + lWidth, lModelTop + lHeight };
		setDrawRectData(iCamNo, model);
		
		long lRefX = lModelLeft + static_cast<long>(pMark->m_dReferenceX);
		long lRefY = lModelTop + static_cast<long>(pMark->m_dReferenceY);
		setDrawPointData(iCamNo, lRefX, lRefY);
		
		// 확인 필요 : Mark 관련 정보 Text display
				
	}
	else if (eMarkType == DEF_BLOB_MARK)
	{
		RECT blob = { m_vectorMarkData[iIndex].m_lModelLeft, m_vectorMarkData[iIndex].m_lModelTop,
			m_vectorMarkData[iIndex].m_lModelRight, m_vectorMarkData[iIndex].m_lModelBottom };
		setDrawRectData(iCamNo, blob);
		
		// 확인 필요 : Mark 관련 정보 Text display

	}
	else if (eMarkType == DEF_CALIPER_MARK)
	{
	}
	redraw(iCamNo);
	return ERR_VISION_SUCCESS;
}

int MVision::FindMultiModel(int iCamNo, int* rgiModel, long lNumModel)
{
	int iResult = ERR_VISION_SUCCESS;
	for (long l = 0; l < lNumModel; l++)
	{
		iResult = RecognitionPatternMark(iCamNo, rgiModel[l]);
		if (iResult)
			return iResult;
	}

	return ERR_VISION_SUCCESS;
}

int MVision::findMultiModel(int iCamNo, int* rgiModel, long lNumModel)
{
	// Note, all models must be of the same size and must use the same search region in the target image. 
	
	// ModelID Array, Result ID Array allocatioin
	ASSERT(lNumModel >0 && lNumModel <= DEF_MAX_MARK_NUM_FOR_MULTI_FIND);
	int rgiModelIndex[DEF_MAX_MARK_NUM_FOR_MULTI_FIND];
	memset(rgiModelIndex, -1, sizeof(rgiModelIndex));
	// 확인 필요 : 배열명을 의미가 명확한 이름으로 바꿀 것.
	int rgiModelNo[DEF_MAX_MARK_NUM_FOR_MULTI_FIND];
	memset(rgiModelNo, -1, sizeof(rgiModelNo));
	long rglModelID[DEF_MAX_MARK_NUM_FOR_MULTI_FIND];
	memset(rglModelID, 0, sizeof(rglModelID));
	long rglResultID[DEF_MAX_MARK_NUM_FOR_MULTI_FIND];
	memset(rglResultID, 0, sizeof(rglResultID));
	int iID = 0;
	long lMaxNumModel = lNumModel;
	for (int i = 0; i < lMaxNumModel; i++)
	{
		rgiModelIndex[iID] = getMarkIndex(iCamNo, DEF_PM_MARK, rgiModel[i]);
		if (rgiModelIndex[iID] == -1)
		{
			lNumModel--;
			continue;
		}

		rglModelID[iID] = m_vectorMarkData[rgiModelIndex[iID]].m_lModelID;
		rglResultID[iID] = m_vectorMarkData[rgiModelIndex[iID]].m_lResultID;
		rgiModelNo[iID] = rgiModel[i];
		iID++;
	}

	if (lNumModel < 1)
		return getErrorCode(ERR_INVALID_MARK_ARRAY_FOR_MULTI_SEARCH, iCamNo);


	// Grab 완료 대기 : 인식 작업은 Continuous Grab 중에는 시행해서는 안됨
	int iSysIndex = getSystemIndex(iCamNo);
	int iResult = m_vectorpSystem[iSysIndex]->WaitGrabComplete(iCamNo);
	if (iResult)
		return getErrorCode(iResult, iCamNo);
	
	MpatFindMultipleModel(m_vectorCamData[iCamNo].m_lGrab, rglModelID, rglResultID, lNumModel, M_FIND_ALL_MODELS);

	SMarkData* pMark = NULL;
	for (long l = 0; l < lNumModel; l++)
	{
		pMark = &(m_vectorMarkData[rgiModelIndex[l]]);

		// Search Area Draw : 인식 결과와 상관없이 인식 작업을 수행하면 무조건 표시
		RECT search = { pMark->m_lSearchAreaLeft, pMark->m_lSearchAreaTop, pMark->m_lSearchAreaRight, pMark->m_lSearchAreaBottom };
		setDrawRectData(iCamNo, search);
		
		if (MpatGetNumber(rglResultID[l], M_NULL) != 1L)	// 찾아낸 Model 이 1 개가 아닌 경우 : Error 처리.
		{
			pMark->m_bSuccess = FALSE;
			pMark->m_dResultX = 0.0;
			pMark->m_dResultY = 0.0;
			
			char szErr[DEF_MAX_CHR_LENGTH];
			memset(szErr, 0, sizeof(szErr));
			sprintf(szErr, "Cam%d : Mk%d is Not Found!", iCamNo + 1, rgiModelNo[l]);
			DrawOverlayText(iCamNo, szErr);
			return getErrorCode(ERR_PATTERN_MATCHING_SEARCH_FAIL, iCamNo);
		}
		
		// 인식 결과 얻기 : Reference Position 위치, 인식 Score
		MpatGetResult(rglResultID[l], M_POSITION_X, &pMark->m_dResultX);
		MpatGetResult(rglResultID[l], M_POSITION_Y, &pMark->m_dResultY);
		MpatGetResult(rglResultID[l], M_SCORE, &pMark->m_dScore);
		
		// Draw Objects : 찾아낸 Model Area, Reference Point, Result Text
		long lOffsetX = static_cast<long>(pMark->m_dResultX - pMark->m_dReferenceX);
		long lOffsetY = static_cast<long>(pMark->m_dResultY - pMark->m_dReferenceY);
		RECT result = { lOffsetX, lOffsetY, pMark->m_lModelRight - pMark->m_lModelLeft + lOffsetX, pMark->m_lModelBottom - pMark->m_lModelTop + lOffsetY };
		setDrawRectData(iCamNo, result);
		
		setDrawPointData(iCamNo, (long)pMark->m_dResultX, (long)pMark->m_dResultY);
		
		char szResult[DEF_MAX_CHR_LENGTH];
		memset(szResult, 0, sizeof(szResult));
		if (pMark->m_dScore >= pMark->m_dThreshold)
		{
			pMark->m_bSuccess = TRUE;
			sprintf(szResult, "Ok : C%d, M%d, (x,y)=(%.3lf,%.3lf)[v], Sc=%.1lf%%", iCamNo, rgiModelNo[l], pMark->m_dResultX, pMark->m_dResultY, pMark->m_dScore);
			setDrawTextData(iCamNo, szResult);
		}
		else
		{
			pMark->m_bSuccess = FALSE;
			sprintf(szResult, "NG : C%d, M%d, (x,y)=(%.3lf,%.3lf)[v], Sc=%.1lf%%", iCamNo, rgiModelNo[l], pMark->m_dResultX, pMark->m_dResultY, pMark->m_dScore);
			DrawOverlayText(iCamNo, szResult);
			return getErrorCode(ERR_PATTERN_MATCHING_LOW_SCORE, iCamNo);
		}
	}
	redraw(iCamNo);

	return ERR_VISION_SUCCESS;
}

int MVision::RecognitionPatternMark(int iCamNo, int iModelNo)
{
	// Mark Data Link
	
	int iModelIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (iModelIndex == -1)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	SMarkData* pMark = &(m_vectorMarkData[iModelIndex]);
	if (pMark->m_bIsModel == FALSE)	// Model 정상 등록 사용 여부 확인
	{
		pMark->m_bSuccess = FALSE;
		pMark->m_dResultX = 0.0;
		pMark->m_dResultY = 0.0;
		
		char szErr[DEF_MAX_CHR_LENGTH];
		memset(szErr, 0, sizeof(szErr));
		sprintf(szErr, "Mk:%d is Not Registered!", iModelNo);
		DrawOverlayText(iCamNo, szErr);

		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);
	}

	// Grab 완료 대기 : 인식 작업은 Continuous Grab 중에는 시행해서는 안됨
	int iSysIndex = getSystemIndex(iCamNo);
	int iResult = m_vectorpSystem[iSysIndex]->WaitGrabComplete(iCamNo);
	if (iResult)
		return getErrorCode(iResult, iCamNo);

	// Grab Buffer 에서 Model 찾은 후 결과 저장
	MpatFindModel(m_vectorCamData[iCamNo].m_lGrab, pMark->m_lModelID, pMark->m_lResultID);

	// Search Area Draw : 인식 결과와 상관없이 인식 작업을 수행하면 무조건 표시
	RECT search = { pMark->m_lSearchAreaLeft, pMark->m_lSearchAreaTop, pMark->m_lSearchAreaRight, pMark->m_lSearchAreaBottom };
	setDrawRectData(iCamNo, search);

	if (MpatGetNumber(pMark->m_lResultID, M_NULL) != 1L)	// 찾아낸 Model 이 1 개가 아닌 경우 : Error 처리.
	{
		pMark->m_bSuccess = FALSE;
		pMark->m_dResultX = 0.0;
		pMark->m_dResultY = 0.0;

		char szErr[DEF_MAX_CHR_LENGTH];
		memset(szErr, 0, sizeof(szErr));
		sprintf(szErr, "Mk:%d is Not Found!", iModelNo);
		DrawOverlayText(iCamNo, szErr);
		return getErrorCode(ERR_PATTERN_MATCHING_SEARCH_FAIL, iCamNo);
	}

	// 인식 결과 얻기 : Reference Position 위치, 인식 Score
	MpatGetResult(pMark->m_lResultID, M_POSITION_X, &pMark->m_dResultX);
	MpatGetResult(pMark->m_lResultID, M_POSITION_Y, &pMark->m_dResultY);
	MpatGetResult(pMark->m_lResultID, M_SCORE, &pMark->m_dScore);

	// Draw Objects : 찾아낸 Model Area, Reference Point
	long lOffsetX = static_cast<long>(pMark->m_dResultX - pMark->m_dReferenceX);
	long lOffsetY = static_cast<long>(pMark->m_dResultY - pMark->m_dReferenceY);
	RECT result = { lOffsetX, lOffsetY, pMark->m_lModelRight - pMark->m_lModelLeft + lOffsetX, pMark->m_lModelBottom - pMark->m_lModelTop + lOffsetY };
	setDrawRectData(iCamNo, result);

	setDrawPointData(iCamNo, (long)pMark->m_dResultX, (long)pMark->m_dResultY);

	// Draw Text
	char szResult[DEF_MAX_CHR_LENGTH];
	memset(szResult, 0, sizeof(szResult));
	if (pMark->m_dScore >= pMark->m_dThreshold)
	{
		pMark->m_bSuccess = TRUE;
		sprintf(szResult, "Ok : M%d, Sc=%.1lf%%", iModelNo, pMark->m_dScore);
		setDrawTextData(iCamNo, szResult);
	}
	else
	{
		pMark->m_bSuccess = FALSE;
		sprintf(szResult, "NG : M%d, Sc=%.1lf%%", iModelNo, pMark->m_dScore);
		DrawOverlayText(iCamNo, szResult);

		return getErrorCode(ERR_PATTERN_MATCHING_LOW_SCORE, iCamNo);
	}
	redraw(iCamNo);

	return ERR_VISION_SUCCESS;
}

int MVision::RecognitionBLOBPattern(int iCamNo, int iModelNo)
{
	int iModelIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);	// Mark Data Link
	if (iModelIndex == -1)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	SMarkData* pMark = &(m_vectorMarkData[iModelIndex]);
	if (pMark->m_bIsModel == FALSE)	// Model 정상 등록 사용 여부 확인
	{
		pMark->m_bSuccess = FALSE;

		char szErr[DEF_MAX_CHR_LENGTH];
		memset(szErr, 0, sizeof(szErr));
		sprintf(szErr, "Mk:%d is Not Registered!", iModelNo);
		DrawOverlayText(iCamNo, szErr);
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);
	}

	// Grab 완료 대기 : 인식 작업은 Continuous Grab 중에는 시행해서는 안됨
	int iSysIndex = getSystemIndex(iCamNo);
	int iResult = m_vectorpSystem[iSysIndex]->WaitGrabComplete(iCamNo);
	if (iResult)
		return getErrorCode(iResult, iCamNo);
	
	// Blob 영역을 복사할 Buffer 생성 : Grab Buffer 사용
	long lBlob;
	MbufChild2d(m_vectorCamData[iCamNo].m_lGrab, pMark->m_lModelLeft, pMark->m_lModelTop,
		pMark->m_lModelRight - pMark->m_lModelLeft, pMark->m_lModelBottom - pMark->m_lModelTop, &lBlob);

	// Allocate a feature list.
	long lFeatureList;
	long lSysID = m_vectorpSystem[iSysIndex]->GetSysID();
	MblobAllocFeatureList(lSysID, &lFeatureList); 
	MblobSelectFeature(lFeatureList, M_AREA);
	
	// Allocate a blob result buffer.
	MblobAllocResult(lSysID, &(pMark->m_lResultID)); 
	
	// Polarity 설정
	if (pMark->m_dPolarity == FALSE)
		MblobControl(pMark->m_lResultID, M_FOREGROUND_VALUE, M_ZERO);
	else	// if (pMark->m_dPolarity == TRUE)
		MblobControl(pMark->m_lResultID, M_FOREGROUND_VALUE, M_NONZERO);
	
	// All blobs are grouped together
	MblobControl(pMark->m_lResultID, M_BLOB_IDENTIFICATION, M_WHOLE_IMAGE);

	// Blob Image 이진화 처리 : lSource 관련 Critical Section 처리
	MimBinarize(lBlob, lBlob, M_GREATER_OR_EQUAL, pMark->m_dThreshold, M_NULL);

	EnterCriticalSection(&(m_vectorCamData[iCamNo].m_csSource));
	MbufCopy(m_vectorCamData[iCamNo].m_lGrab, m_vectorCamData[iCamNo].m_lSource);
	LeaveCriticalSection(&m_vectorCamData[iCamNo].m_csSource);

	// Calculate selected features for each blob.
	MblobCalculate(lBlob, M_NULL, lFeatureList, pMark->m_lResultID);

	double dFindPixelsInBlob = 0.0;
	MblobGetResult(pMark->m_lResultID, M_AREA, &dFindPixelsInBlob);
	//MblobGetResult(pMark->m_lResultID, M_PERIMETER, &dPerimeter);	// Get perimeter
	//MblobGetResult(pMark->m_lResultID, M_CENTER_OF_GRAVITY_X+M_TYPE_SHORT, &CenterOfMassX);	// Get Center of gravity
	//MblobGetResult(pMark->m_lResultID, M_CENTER_OF_GRAVITY_Y+M_TYPE_SHORT, &CenterOfMassY);

	// Result Score 계산
	pMark->m_dScore = dFindPixelsInBlob / (pMark->m_lModelRight - pMark->m_lModelLeft) / (pMark->m_lModelBottom - pMark->m_lModelTop) * 100.0;
	
	MblobFree(pMark->m_lResultID); 
	MblobFree(lFeatureList); 
	MbufFree(lBlob);

	// Draw Blob Area
	RECT rectBlob = { pMark->m_lModelLeft, pMark->m_lModelTop, pMark->m_lModelRight, pMark->m_lModelBottom };
	setDrawRectData(iCamNo, rectBlob);

	// Draw Result Text
	char szResult[DEF_MAX_CHR_LENGTH];
	memset(szResult, 0, sizeof(szResult));
	if (pMark->m_dScore < pMark->m_dMinLimit || pMark->m_dScore > pMark->m_dMaxLimit)
	{
		pMark->m_bSuccess = FALSE;
		sprintf(szResult, "NG : M%d, Sc=%.1lf%%", iModelNo, pMark->m_dScore);
		DrawOverlayText(iCamNo, szResult);
		return getErrorCode(ERR_BLOB_ANALYSIS_FAIL, iCamNo);
	}
	else
	{
		pMark->m_bSuccess = TRUE;
		sprintf(szResult, "Ok : M%d, Sc=%.1lf%%", iModelNo, pMark->m_dScore);
		setDrawTextData(iCamNo, szResult);
	}
	redraw(iCamNo);
	return ERR_VISION_SUCCESS;
}
	//20121004 SJ_HJG 횡전개

//@130119.KKY__________________________
int MVision::RecognitionEdgeLineY(int iCameraNo, int iModelNo, BOOL bSettingMode, double dRefVX, double dRefVY)
{
	int iModelIndex = getMarkIndex(iCameraNo, DEF_BLOB_MARK, iModelNo);	// Mark Data Link
	if (iModelIndex == -1)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCameraNo);

	SMarkData* pMark = &(m_vectorMarkData[iModelIndex]);

	if (pMark->m_bIsModel == FALSE)	// Model 정상 등록 사용 여부 확인
	{
		pMark->m_bSuccess = FALSE;

		char szErr[DEF_MAX_CHR_LENGTH];
		memset(szErr, 0, sizeof(szErr));
		sprintf(szErr, "Mk:%d is Not Registered!", iModelNo);
		DrawOverlayText(iCameraNo, szErr);
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCameraNo);
	}
	
	// Blob 영역을 복사할 Buffer 생성 : Grab Buffer 사용
	long lBlob[2];
	long lProjection[2];	// Projection buffer identifier.

	CRect rcModel(pMark->m_lModelLeft, pMark->m_lModelTop, pMark->m_lModelRight, pMark->m_lModelBottom);
	CRect rcTmp = rcModel;
	long lWidth = rcModel.Width();
	long lHeight = rcModel.Height();
	if (dRefVX != 0)
	{
		rcModel.left = (long)(dRefVX + pMark->m_dReferenceX);
		rcModel.right = (long)(dRefVX + pMark->m_dReferenceX + rcTmp.Width());
	}
	if (dRefVY != 0)
	{
		rcModel.top = (long)(dRefVY + pMark->m_dReferenceY);
		rcModel.bottom = (long)(dRefVY + pMark->m_dReferenceY + rcTmp.Height());
	}


	int iSysIndex = getSystemIndex(iCameraNo);
	long lSysID = m_vectorpSystem[iSysIndex]->GetSysID();
	
	long lDepth = 20;
	long lOffset = 5;
	if (iModelNo == DEF_BLOB_TAB_END_MARK1 || iModelNo == DEF_BLOB_TAB_END_MARK2)
	{
		lDepth = 30;
	}

	if (rcModel.Width() < (lDepth+lOffset)*2)
		return getErrorCode(ERR_BLOB_MARK_SIZE_INVALID, iCameraNo);

	POINT vSize = GetDistribution(iCameraNo);
	//if (rcModel.right >= vSize.x-1 || rcModel.top >= vSize.y-1)
	if (rcModel.right >= vSize.x-1 || rcModel.bottom >= vSize.y-1)
		return getErrorCode(ERR_BLOB_MARK_SIZE_INVALID, iCameraNo);
	if (rcModel.left <= 0 || rcModel.top <= 0)
		return getErrorCode(ERR_BLOB_MARK_SIZE_INVALID, iCameraNo);

	MbufChild2d(m_vectorCamData[iCameraNo].m_lGrab, rcModel.left+lOffset, rcModel.top, lDepth, lHeight, &lBlob[0]);
	MbufChild2d(m_vectorCamData[iCameraNo].m_lGrab, rcModel.right-(lOffset+lDepth), rcModel.top, lDepth, lHeight, &lBlob[1]);

	//@MbufChild로 할당했기 때문에 필요 없음.MbufAlloc2d로 할당했을 경우 이미지 Copy를 위해 필요함
//@	MbufCopyClip(m_vectorCamData[iCameraNo].m_lGrab, lBlob[0], -(rcModel.left+lOffset), -rcModel.top);
//@	MbufCopyClip(m_vectorCamData[iCameraNo].m_lGrab, lBlob[1], -(rcModel.Width()-(lOffset+lDepth)), -rcModel.top);
	
	// Blob Image 이진화 처리 : lSource 관련 Critical Section 처리
	MimBinarize(lBlob[0], lBlob[0], M_GREATER_OR_EQUAL, M_DEFAULT, M_DEFAULT);
	MimBinarize(lBlob[1], lBlob[1], M_GREATER_OR_EQUAL, M_DEFAULT, M_DEFAULT);

	if (TRUE == bSettingMode)
	{
		EnterCriticalSection(&(m_vectorCamData[iCameraNo].m_csSource));
		MbufCopy(m_vectorCamData[iCameraNo].m_lGrab, m_vectorCamData[iCameraNo].m_lSource);
		LeaveCriticalSection(&m_vectorCamData[iCameraNo].m_csSource);
	}

	long *lResult = new long[lHeight];
	int iPos[2] = {0, 0};
	double dMax = -1.0;
	for (int i=0; i<2; i++)
	{
		// Allocate a blob result buffer.
		MimAllocResult(lSysID, lHeight, M_PROJ_LIST, &lProjection[i]);
		MimProject(lBlob[i], lProjection[i], M_90_DEGREE);
		memset(lResult, 0, sizeof(lResult));
		MimGetResult(lProjection[i], M_VALUE, lResult);

		dMax = -1.0;
		double dVal = 0.0;
		for (int j=lHeight-3; j>=2; j--)
		{
			dVal = (lResult[j]-lResult[j-2])/(lDepth*2.0);
			if (fabs(dVal) > dMax)
			{
				dMax = fabs(dVal);
				iPos[i] = j;
			}
		}
	}

	for (int i=0; i<2; i++)
	{
		MimFree(lProjection[i]);
		MbufFree(lBlob[i]);
	}
	delete [] lResult;

	pMark->m_dResultX = rcModel.CenterPoint().x;
	pMark->m_dResultY = rcModel.top + ((iPos[0] + iPos[1])/2 + .5);
	pMark->m_bSuccess = TRUE;
		
	setDrawPointData(iCameraNo, (long)pMark->m_dResultX, (long)pMark->m_dResultY);
	redraw(iCameraNo);
	return ERR_VISION_SUCCESS;
}
//@____________________________________

inline void MVision::redraw(int iCamNo)
{
	if (NULL == m_vectorCamData[iCamNo].m_hWnd)
		return;

	::PostMessage(m_vectorCamData[iCamNo].m_hWnd, WM_DISPLAY_SOURCE_IMAGE, (WPARAM)&(m_vectorCamData[iCamNo]), NULL);
}

void MVision::RedrawView(int iViewID)
{
	// 확인 필요 : 정확하지 않은 핸들로 메세지 보내는 부분 있는지 점검 필요.
	// 0 에게 메세지 보내면 문제...

	// Vision 전체 코드에 걸쳐서, message queue 에 message 쌓이는 경우 발생하지 않도록 별도 처리 필요...

	std::vector<Mdisplay*>::iterator j;
	for (j = m_vectorpDisp.begin(); j < m_vectorpDisp.end(); j++)
	{
		if ((*j)->GetViewID() == iViewID)
		{
			::InvalidateRect((*j)->GetDispWndHandle(), NULL, FALSE);
			return;
		}
	}
}

void MVision::setDrawRectData(int iCamNo, RECT& rect, int iObjectType)
{
	int index = -1;
	if (iObjectType == DEF_FIXED_OBJECT)
	{
		(m_vectorCamData[iCamNo].m_iRectIndex)++;
		index = m_vectorCamData[iCamNo].m_iRectIndex;
	}
	else	// if (iObjectType == DEF_FLOATING_OBJECT)
	{
		int i;
		for (i = m_vectorCamData[iCamNo].m_iRectIndex; i > -1; i--)
		{
			if (m_vectorCamData[iCamNo].m_rgiRectType[i] == DEF_FLOATING_OBJECT)
				break;
		}
		
		if (i == -1)	// Floating Object 가 하나도 없는 경우.
		{
			(m_vectorCamData[iCamNo].m_iRectIndex)++;
			index = m_vectorCamData[iCamNo].m_iRectIndex;
		}
		else	// 하나의 Camera 에는 요소 별로 하나의 Floating Object 만 표시될 수 있다.
			index = i;
	}

	if (m_vectorCamData[iCamNo].m_iRectIndex < DEF_MAX_RECT_OBJECT)
	{
		m_vectorCamData[iCamNo].m_rgiRectType[index] = iObjectType;
		m_vectorCamData[iCamNo].m_rgdRectLeft[index] = rect.left;
		m_vectorCamData[iCamNo].m_rgdRectTop[index] = rect.top;
		m_vectorCamData[iCamNo].m_rgdRectRight[index] = rect.right;
		m_vectorCamData[iCamNo].m_rgdRectBottom[index] = rect.bottom;
	}
	else
	{
		(m_vectorCamData[iCamNo].m_iRectIndex)--;
	}
}

void MVision::setDrawPointData(int iCamNo, long lX, long lY, int iObjectType)
{
	int index = -1;
	if (iObjectType == DEF_FIXED_OBJECT)
	{
		(m_vectorCamData[iCamNo].m_iPointIndex)++;
		index = m_vectorCamData[iCamNo].m_iPointIndex;
	}
	else	// if (iObjectType == DEF_FLOATING_OBJECT)
	{
		int i;
		for (i = m_vectorCamData[iCamNo].m_iPointIndex; i > -1; i--)
		{
			if (m_vectorCamData[iCamNo].m_rgiPointType[i] == DEF_FLOATING_OBJECT)
				break;
		}
		
		if (i == -1)	// Floating Object 가 하나도 없는 경우.
		{
			(m_vectorCamData[iCamNo].m_iPointIndex)++;
			index = m_vectorCamData[iCamNo].m_iPointIndex;
		}
		else	// 하나의 Camera 에는 요소 별로 하나의 Floating Object 만 표시될 수 있다.
			index = i;
	}

	if (m_vectorCamData[iCamNo].m_iPointIndex < DEF_MAX_POINT_OBJECT)
	{
		m_vectorCamData[iCamNo].m_rgiPointType[index] = iObjectType;
		m_vectorCamData[iCamNo].m_rgdPointX[index] = lX;
		m_vectorCamData[iCamNo].m_rgdPointY[index] = lY;
	}
	else
	{
		(m_vectorCamData[iCamNo].m_iPointIndex)--;
	}
}

void MVision::setDrawTextData(int iCamNo, char* pszText, int iObjectType)
{
	int index = -1;
	int i;
	if (iObjectType == DEF_FIXED_OBJECT)
	{
		(m_vectorCamData[iCamNo].m_iTextIndex)++;
		index = m_vectorCamData[iCamNo].m_iTextIndex;
	}
	else	// if (iObjectType == DEF_FLOATING_OBJECT)
	{
		for (i = m_vectorCamData[iCamNo].m_iTextIndex; i > -1; i--)
		{
			if (m_vectorCamData[iCamNo].m_rgiTextType[i] == DEF_FLOATING_OBJECT)
				break;
		}
		
		if (i == -1)	// Floating Object 가 하나도 없는 경우.
		{
			(m_vectorCamData[iCamNo].m_iTextIndex)++;
			index = m_vectorCamData[iCamNo].m_iTextIndex;
		}
		else	// 하나의 Camera 에는 요소 별로 하나의 Floating Object 만 표시될 수 있다.
			index = i;
	}
	
	if (m_vectorCamData[iCamNo].m_iTextIndex < DEF_MAX_TEXT_OBJECT)
	{
		m_vectorCamData[iCamNo].m_rgiTextType[index] = iObjectType;
		memset(m_vectorCamData[iCamNo].m_rgszText[index], 0, DEF_MAX_TEXT_LENGTH);
		strcpy(m_vectorCamData[iCamNo].m_rgszText[index], pszText);
	}
	else
	{
		(m_vectorCamData[iCamNo].m_iTextIndex)--;
	}
}

//110214.Add_____
void MVision::setDrawPointData_Measure(int iCamNo, long lX, long lY, int iObjectType/* = DEF_FLOATING_OBJECT*/)
{
	int index = -1;
	if (iObjectType == DEF_FIXED_OBJECT)
	{
		(m_vectorCamData[iCamNo].m_iPointIndex_Measure)++;
		index = m_vectorCamData[iCamNo].m_iPointIndex_Measure;
	}
	else	// if (iObjectType == DEF_FLOATING_OBJECT)
	{
		int i;
		for (i = m_vectorCamData[iCamNo].m_iPointIndex_Measure; i > -1; i--)
		{
			if (m_vectorCamData[iCamNo].m_rgiPointType_Measure[i] == DEF_FLOATING_OBJECT)
				break;
		}
		
		if (i == -1)	// Floating Object 가 하나도 없는 경우.
		{
			(m_vectorCamData[iCamNo].m_iPointIndex_Measure)++;
			index = m_vectorCamData[iCamNo].m_iPointIndex_Measure;
		}
		else	// 하나의 Camera 에는 요소 별로 하나의 Floating Object 만 표시될 수 있다.
			index = i;
	}

	if (m_vectorCamData[iCamNo].m_iPointIndex_Measure < DEF_MAX_POINT_OBJECT)
	{
		m_vectorCamData[iCamNo].m_rgiPointType_Measure[index] = iObjectType;
		m_vectorCamData[iCamNo].m_rgdPointX_Measure[index] = lX;
		m_vectorCamData[iCamNo].m_rgdPointY_Measure[index] = lY;
	}
	else
	{
		(m_vectorCamData[iCamNo].m_iPointIndex_Measure)--;
	}
}

void MVision::setDrawLineData_Measure(int iCamNo, long lStartX, long lStartY, long lEndX, long lEndY, int iObjectType/* = DEF_FLOATING_OBJECT*/)
{
	int index = -1;
	if (iObjectType == DEF_FIXED_OBJECT)
	{
		(m_vectorCamData[iCamNo].m_iLineIndex_Measure)++;
		index = m_vectorCamData[iCamNo].m_iLineIndex_Measure;
	}
	else	// if (iObjectType == DEF_FLOATING_OBJECT)
	{
		int i;
		for ( i = m_vectorCamData[iCamNo].m_iLineIndex_Measure; i > -1; i--)
		{
			if (m_vectorCamData[iCamNo].m_rgiLineType_Measure[i] == DEF_FLOATING_OBJECT)
				break;
		}
		
		if (i == -1)	// Floating Object 가 하나도 없는 경우.
		{
			(m_vectorCamData[iCamNo].m_iLineIndex_Measure)++;
			index = m_vectorCamData[iCamNo].m_iLineIndex_Measure;
		}
		else	// 하나의 Camera 에는 요소 별로 하나의 Floating Object 만 표시될 수 있다.
			index = i;
	}

	if (m_vectorCamData[iCamNo].m_iLineIndex_Measure < DEF_MAX_LINE_OBJECT)
	{
		m_vectorCamData[iCamNo].m_rgiLineType_Measure[index] = iObjectType;
		m_vectorCamData[iCamNo].m_rgdLineStartX_Measure[index] = lStartX;
		m_vectorCamData[iCamNo].m_rgdLineStartY_Measure[index] = lStartY;
		m_vectorCamData[iCamNo].m_rgdLineEndX_Measure[index] = lEndX;
		m_vectorCamData[iCamNo].m_rgdLineEndY_Measure[index] = lEndY;
	}
	else
	{
		(m_vectorCamData[iCamNo].m_iLineIndex_Measure)--;
	}
}
//_______________

void MVision::DrawOverlayAreaRect(int iCamNo, RECT& rect, int iObjectType/*= DEF_FIXED_OBJECT*/)
{
	setDrawRectData(iCamNo, rect, iObjectType);
	redraw(iCamNo);
}

void MVision::DrawOverlayCrossMark(int iCamNo, long lX, long lY, int iObjectType/*= DEF_FIXED_OBJECT*/)
{
	setDrawPointData(iCamNo, lX, lY, iObjectType);
	redraw(iCamNo);
}

void MVision::DrawOverlayText(int iCamNo, char* pszText, int iObjectType/*= DEF_FIXED_OBJECT*/)
{
	setDrawTextData(iCamNo, pszText, iObjectType);
	redraw(iCamNo);
}

//110214.Add_____
void MVision::DrawOverlayLine_Measure(int iCamNo, long lStartX, long lStartY, long lEndX, long lEndY, int iObjectType /*= DEF_FLOATING_OBJECT*/)
{
	setDrawLineData_Measure(iCamNo, lStartX, lStartY, lEndX, lEndY, iObjectType);
	redraw(iCamNo);
}

void MVision::DrawOverlayCross_Measure(int iCamNo, long lX, long lY, int iObjectType /*= DEF_FLOATING_OBJECT*/)
{
	setDrawPointData_Measure(iCamNo, lX, lY, iObjectType);
	redraw(iCamNo);
}
//_______________

void MVision::LoadImage(int iDisplayCamNo, tstring strPathName)
{
	ClearOverlay(iDisplayCamNo);
	HaltVideo(iDisplayCamNo);

	long lSource = m_vectorCamData[iDisplayCamNo].m_lSource;
	int iSysIndex = m_vectorCamData[iDisplayCamNo].m_iConnectedSystemID;
	long lSysID = m_vectorpSystem[iSysIndex]->GetSysID();

	EnterCriticalSection(&(m_vectorCamData[iDisplayCamNo].m_csSource));

	long lRestore;
	std::string strFileName = getSingleCharacterString(strPathName);
	MbufImport((char*)strFileName.c_str(), M_DEFAULT, M_RESTORE, lSysID, &lRestore);

	long lRestoreWidth, lRestoreHeight;
	MbufInquire(lRestore, M_SIZE_X, &lRestoreWidth);
	MbufInquire(lRestore, M_SIZE_Y, &lRestoreHeight);

	long lSourceWidth, lSourceHeight;
	MbufInquire(lSource, M_SIZE_X, &lSourceWidth);
	MbufInquire(lSource, M_SIZE_Y, &lSourceHeight);

	if (lRestoreWidth == lSourceWidth && lRestoreHeight == lSourceHeight)
		MbufCopy(lRestore, lSource);
	else
	{
		long lSizeBand;
		MbufInquire(lRestore, M_SIZE_BAND, &lSizeBand);

		long lRestoreView;
		MbufAllocColor(lSysID, lSizeBand, lRestoreWidth, lRestoreHeight, 8L+M_UNSIGNED, M_IMAGE+M_PROC+M_DISP+M_DIB+M_GDI, &lRestoreView);

		MbufCopy(lRestore, lRestoreView);

		HDC hRestoreDC;
		MbufControl(lRestoreView, M_WINDOW_DC_ALLOC, M_DEFAULT);
		#ifndef DEF_MIL_VER_90
		MbufInquire(lRestoreView, M_WINDOW_DC, &hRestoreDC);
		#else
		hRestoreDC = ((HDC)MbufInquire(lRestoreView, M_WINDOW_DC, M_NULL));
		#endif
		
		HDC hSourceDC;
		MbufControl(lSource, M_WINDOW_DC_ALLOC, M_DEFAULT);
		#ifndef DEF_MIL_VER_90
		MbufInquire(lSource, M_WINDOW_DC, &hSourceDC);
		#else
		hSourceDC = ((HDC)MbufInquire(lSource, M_WINDOW_DC, M_NULL));
		#endif
		
		::SetStretchBltMode(hSourceDC, HALFTONE);
		::StretchBlt(hSourceDC, 0, 0, lSourceWidth, lSourceHeight, hRestoreDC, 0, 0, lRestoreWidth, lRestoreHeight, SRCCOPY);
		
		MbufControl(lRestoreView, M_WINDOW_DC_FREE, M_DEFAULT);
		MbufControl(lSource, M_WINDOW_DC_FREE, M_DEFAULT);
		MbufFree(lRestoreView);
	}
	MbufFree(lRestore);
	LeaveCriticalSection(&(m_vectorCamData[iDisplayCamNo].m_csSource));

	m_vectorCamData[iDisplayCamNo].m_hWnd = getViewHandle(iDisplayCamNo);
	redraw(iDisplayCamNo);
}

void MVision::EnableSaveImage(BOOL bFlag)
{
	m_iSaveImage = bFlag;
}

int MVision::SaveImage(int iCamNo, const tstring& strPath, const tstring& strFile)
{
	if (m_iSaveImage == FALSE)
		return ERR_VISION_SUCCESS;

	std::string strPath_SC = getSingleCharacterString(strPath);
	std::string strFile_SC = getSingleCharacterString(strFile);

	char szName[DEF_MAX_CHR_LENGTH];
	time_t lTime;
	time(&lTime);
	struct tm *tmTime = localtime(&lTime);
	struct _timeb sTime;
	_ftime(&sTime);
	sprintf(szName, "%s\\[%04d %02d %02d %02d.%02d.%02d.%03d] %s.bmp",
		strPath_SC.c_str(), tmTime->tm_year + 1900, tmTime->tm_mon + 1, tmTime->tm_mday,
		tmTime->tm_hour, tmTime->tm_min, tmTime->tm_sec, sTime.millitm, strFile_SC.c_str());

	int iSysIndex = getSystemIndex(iCamNo);
	int iResult = m_vectorpSystem[iSysIndex]->WaitGrabComplete(iCamNo);	// 동일 thread 에서 호출된 Grab 완료 대기
	if (iResult)
		return getErrorCode(iResult, iCamNo);

	// 위의 WaitGrabComplete() 와 아래의 EnterCriticalSection() 수행 사이에,
	// 다른 Thread 에서 동일 Camera 에 대해 Grab 동작을 수행하지 못하도록 하는 것은
	// 이 함수 호출부에서 Critical Section(m_csGrab) 을 이용해서 처리해야 한다.

	// 확인 필요 : 외부에서 Critical Section 걸어줘야 함...
	MbufExport(szName, M_BMP, m_vectorCamData[iCamNo].m_lGrab);

	return ERR_VISION_SUCCESS;
}

int MVision::SaveImage(int iCamNo, const tstring& strFile)
{
	tstring strImageDataFilePath;

#ifdef UNICODE
	_TCHAR wszFilePath[100];
	
	//170214 SJ_YSH
	//Camera별 Image 저장 Folder 구분
	//SDC 최웅일 과장 요청 사항
	if(iCamNo >= DEF_F_TABALIGNER_CAMERA_1 && iCamNo <= DEF_F_TABALIGNER_CAMERA_4)
		m_strImageDataFilePath = getSingleCharacterString(_T("..\\LogImage\\F_TabAlignImage"));

	else if(iCamNo >= DEF_INSPECTION_CAMERA_1 && iCamNo <= DEF_INSPECTION_CAMERA_8)
		m_strImageDataFilePath = getSingleCharacterString(_T("..\\LogImage\\F_InspectionImage"));

	else if(iCamNo >= DEF_R_TABALIGNER_CAMERA_1 && iCamNo <= DEF_R_TABALIGNER_CAMERA_4)
		m_strImageDataFilePath = getSingleCharacterString(_T("..\\LogImage\\R_TabAlignImage"));
	
	else if(iCamNo >= DEF_R_INSPECTION_CAMERA_1 && iCamNo <= DEF_R_INSPECTION_CAMERA_8)
		m_strImageDataFilePath = getSingleCharacterString(_T("..\\LogImage\\R_InspectionImage"));
	//_____________

	size_t count = mbstowcs(wszFilePath, m_strImageDataFilePath.c_str(), 100);
	if (-1 == count)	// 확인 필요.
		return getErrorCode(-9999);

	strImageDataFilePath = wszFilePath;
#else
	strImageDataFilePath = m_strImageDataFilePath;
#endif

	return SaveImage(iCamNo, strImageDataFilePath, strFile);
}

int MVision::SaveLastErrorImage(int iCamNo, const tstring& strPath)
{
	return SaveImage(iCamNo, strPath, _T("LastErrorImage"));
}

int MVision::DeleteOldImageFiles(const tstring& strPath/* = _T("")*/)
{
    time_t ltime;
    struct tm* tmTime;
	
    time(&ltime);                 
    tmTime = localtime(&ltime); 
	
	std::string strDirPath = (strPath.empty() == TRUE) ? m_strImageDataFilePath : getSingleCharacterString(strPath);
    char szWildCardFileName[DEF_MAX_CHR_LENGTH];
    sprintf(szWildCardFileName, "%s\\*.bmp", strDirPath.c_str());
	WIN32_FIND_DATAA finddataFile;
    HANDLE handleFirstFile = FindFirstFileA(szWildCardFileName, &finddataFile);
    if (handleFirstFile == INVALID_HANDLE_VALUE)
        return ERR_VISION_SUCCESS;

	std::string strCurrentFile = finddataFile.cFileName;
	deleteOldFile(tmTime, strDirPath, strCurrentFile);

    while (FindNextFileA(handleFirstFile, &finddataFile))
    {
        strCurrentFile = finddataFile.cFileName;
		deleteOldFile(tmTime, strDirPath, strCurrentFile);
    }
    return ERR_VISION_SUCCESS;
}

void MVision::deleteOldFile(struct tm* tmTime, const std::string& strPath, std::string& currentFile)
{
	int iDElapsedDay = 0;
    int iLogKeepingDays = 5;
	int pDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int iLeapYear = tmTime->tm_year + 1900;
    if ((!(iLeapYear % 4) && (iLeapYear % 100)) || !(iLeapYear % 400))
        pDays[1] = 29;

	char szLogFile[DEF_MAX_CHR_LENGTH], szDeleteFile[DEF_MAX_CHR_LENGTH];

	int iDYear = atoi(currentFile.substr(1, 4).c_str());
	int iDMonth = atoi(currentFile.substr(6, 2).c_str());
	int iDDay = atoi(currentFile.substr(9, 2).c_str());
	
	int iDiffYear = tmTime->tm_year + 1900 - iDYear;
	if (iDiffYear == 0)
	{
		for (int i = iDMonth; i < tmTime->tm_mon + 2; i++)
		{
			if (iDMonth == tmTime->tm_mon + 1)
			{
				iDElapsedDay = tmTime->tm_mday - iDDay;
				break;
			}
			
			if (i == iDMonth)
				iDElapsedDay += (pDays[iDMonth - 1] - iDDay);
			else if (i == tmTime->tm_mon + 1)
				iDElapsedDay += tmTime->tm_mday;
			else
				iDElapsedDay += pDays[i - 1];
		}
	}
	else
	{
		for (int i = iDMonth; i < 13; i++)
		{
			if (i == iDMonth) 
				iDElapsedDay += (pDays[iDMonth - 1] - iDDay);
			else
				iDElapsedDay += pDays[i - 1];
		}
		
		for (int i = 1; i < tmTime->tm_mon + 2; i++)
		{
			if (i == tmTime->tm_mon + 1)
				iDElapsedDay += tmTime->tm_mday;
			else
				iDElapsedDay += pDays[i];
		}
		iDElapsedDay += (iDiffYear - 1) * 365;
	}

	if (iDElapsedDay > iLogKeepingDays)
	{
		sprintf(szLogFile, "%s\\", strPath.c_str());
		sprintf(szDeleteFile, "%s%s", szLogFile, currentFile.c_str());
		DeleteFileA(szDeleteFile);
	}
}

int MVision::getMarkIndex(int iCamNo, EMarkType eMarkType, const int iModelNo)
{
	if (IsValidCam(iCamNo) == FALSE
		|| (eMarkType <= DEF_NONE_MARK || eMarkType >= DEF_MAX_MARK_TYPE)
		|| isValidMarkNo(iModelNo) == FALSE
		|| -1 == m_vectorCamData[iCamNo].m_rgiMarkRelation[eMarkType][iModelNo])
		return -1;

	int iVectorIndex = 0;
	std::vector<SMarkData>::iterator iter;
	for ( iter = m_vectorMarkData.begin(); iter < m_vectorMarkData.end(); iter++, iVectorIndex++)
	{
		if (m_vectorCamData[iCamNo].m_rgiMarkRelation[eMarkType][iModelNo] == (*iter).m_iMarkIndex)
			return iVectorIndex;
	}

	return -1;
}

int MVision::SetSearchArea(int iCamNo, int iModelNo, const RECT& sRect)
{
	int iIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (iIndex == -1)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	RECT rectLimit = { 0, 0, m_vectorCamData[iCamNo].m_lBufferSizeX, m_vectorCamData[iCamNo].m_lBufferSizeY };
	int iResult = checkAreaSize(sRect, rectLimit);
	if (iResult)
		return iResult;

	SMarkData* pMark = &(m_vectorMarkData[getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo)]);
	if (pMark->m_bIsModel == FALSE)
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);

	pMark->m_lSearchAreaLeft = sRect.left;
	pMark->m_lSearchAreaTop = sRect.top;
	pMark->m_lSearchAreaRight = sRect.right;
	pMark->m_lSearchAreaBottom = sRect.bottom;

	MpatSetPosition(pMark->m_lModelID, sRect.left, sRect.top, sRect.right - sRect.left, sRect.bottom - sRect.top);
	MpatPreprocModel(M_NULL, pMark->m_lModelID, M_DEFAULT);

	return WriteModelData(iCamNo, DEF_PM_MARK, iModelNo);
}

int MVision::checkAreaSize(const RECT& rectObject, const RECT& rectLimit)
{
	// 위치 조건 확인
	if (rectObject.left < rectLimit.left)
		return getErrorCode(ERR_MARK_POSITION_LIMIT_LEFT);
	else if (rectObject.top < rectLimit.top)
		return getErrorCode(ERR_MARK_POSITION_LIMIT_TOP);
	else if (rectObject.right > rectLimit.right)
		return getErrorCode(ERR_MARK_POSITION_LIMIT_RIGHT);
	else if (rectObject.bottom > rectLimit.bottom)
		return getErrorCode(ERR_MARK_POSITION_LIMIT_BOTTOM);

	// 크기 조건 확인 제한 해제.

	return ERR_VISION_SUCCESS;
}

int MVision::getNewMarkStorageIndex()
{
	// Camera Data 로부터 사용 Model Index Max 값을 얻는다.
	int iMaxMarkIndex = -1;
	std::vector<SCameraData>::iterator l;
	for (l = m_vectorCamData.begin(); l < m_vectorCamData.end(); l++)
	{
		for (int j = 0; j < DEF_MAX_MARK_TYPE; j++)
		{
			for (int k = 0; k < DEF_MAX_USE_MARK; k++)
			{
				if ((*l).m_rgiMarkRelation[j][k] > iMaxMarkIndex)
					iMaxMarkIndex = (*l).m_rgiMarkRelation[j][k];
			}
		}
	}
	iMaxMarkIndex++;
	if (0 == iMaxMarkIndex)
		return 0;

	int* rgbMarkIndex = new int[iMaxMarkIndex];
	int k;
	for (k = 0; k < iMaxMarkIndex; k++)
		rgbMarkIndex[k] = -1;

	std::vector<SMarkData>::iterator iter;
	for (iter = m_vectorMarkData.begin(); iter < m_vectorMarkData.end(); iter++)
	{
		if (-1 != (*iter).m_iMarkIndex)
			rgbMarkIndex[(*iter).m_iMarkIndex] = (*iter).m_iMarkIndex;
	}

	for (k = 0; k < iMaxMarkIndex; k++)
	{
		if (-1 == rgbMarkIndex[k])
			break;
	}

	delete[] rgbMarkIndex;
	return k;
}

int MVision::RegisterPatternMark(int iCamNo, int iModelNo, RECT& SearchArea,
								 RECT& ModelArea, double dReferenceX, double dReferenceY)
{
#ifdef VISION_SIMULATION
	return ERR_VISION_SUCCESS;
#endif

	if (FALSE == IsValidCam(iCamNo))
		return getErrorCode(ERR_INVALID_CAMERA_NO);
		
	if (FALSE == isValidMarkNo(iModelNo))
		return getErrorCode(ERR_MARK_NO_RANGE_OVER, iCamNo);

	RECT rectLimit = { 0, 0, m_vectorCamData[iCamNo].m_lBufferSizeX, m_vectorCamData[iCamNo].m_lBufferSizeY };
	int iResult = checkAreaSize(SearchArea, rectLimit);
	if (iResult)
		return iResult;

	iResult = checkAreaSize(ModelArea, SearchArea);
	if (iResult)
		return iResult;

	SMarkData sMark;
	memset(&sMark, 0, sizeof(SMarkData));
	sMark.m_iSystemIndex = getSystemIndex(iCamNo);
	sMark.m_iMarkIndex = getNewMarkStorageIndex();	// Model Storage Index
	sMark.m_dCertaintyThreshold = 90.0;	// Certainty Threshold
	sMark.m_dThreshold	= 70.0;	// Acceptance Threshold
	sMark.m_eMarkType = DEF_PM_MARK;	// Mark Type
	sMark.m_lSearchAreaLeft = SearchArea.left;	// Search Area
	sMark.m_lSearchAreaTop = SearchArea.top;
	sMark.m_lSearchAreaRight = SearchArea.right;
	sMark.m_lSearchAreaBottom = SearchArea.bottom;
	sMark.m_lModelLeft = ModelArea.left;	// Model Area
	sMark.m_lModelTop = ModelArea.top;
	sMark.m_lModelRight = ModelArea.right;
	sMark.m_lModelBottom = ModelArea.bottom;
	sMark.m_dReferenceX = dReferenceX - ModelArea.left;	// Reference Point
	sMark.m_dReferenceY = dReferenceY - ModelArea.top;

	char szTemp[DEF_MAX_CHR_LENGTH];	
	memset(szTemp, NULL, sizeof(szTemp));
	sprintf(szTemp, "%s\\PM_mark%d.mmo", m_strModelDataFilePath.c_str(), sMark.m_iMarkIndex);
	
	if (sMark.m_pszFileName != NULL)
		delete[] sMark.m_pszFileName;
	
//	sMark.m_pszFileName = new char[strlen(szTemp) + 1];
	sMark.m_pszFileName = new char[DEF_MAX_CHR_LENGTH];
	sprintf(sMark.m_pszFileName, "%s", szTemp);

	// 이미 Camera 에 등록된 동일 Model 번호가 있는지 확인 필요.

	long lSysID = m_vectorpSystem[sMark.m_iSystemIndex]->GetSysID();
	long lSource = m_vectorCamData[iCamNo].m_lSource;
	long lModelWidth = sMark.m_lModelRight - sMark.m_lModelLeft;
	long lModelHeight = sMark.m_lModelBottom - sMark.m_lModelTop;

	// Model ID Allocation
	MpatAllocModel(lSysID, lSource, sMark.m_lModelLeft, sMark.m_lModelTop, lModelWidth, lModelHeight, M_NORMALIZED, &(sMark.m_lModelID));
	if (sMark.m_lModelID == M_NULL)
	{
		return -999;	// Model 생성 실패.
	}

	MpatSetAccuracy(sMark.m_lModelID, M_HIGH);
	MpatSetSpeed(sMark.m_lModelID, M_HIGH);
	MpatSetAcceptance(sMark.m_lModelID, sMark.m_dThreshold);			// Acceptance Threshold Setting
	MpatSetCertainty(sMark.m_lModelID, sMark.m_dCertaintyThreshold);	// Set Certainty Threshold
	MpatSetCenter(sMark.m_lModelID, sMark.m_dReferenceX, sMark.m_dReferenceY);
	MpatSetNumber(sMark.m_lModelID, 1L);
	MpatSetPosition(sMark.m_lModelID, SearchArea.left, SearchArea.top, SearchArea.right - SearchArea.left, SearchArea.bottom - SearchArea.top);
	MpatPreprocModel(lSource, sMark.m_lModelID, M_DEFAULT);
	MpatAllocResult(lSysID, M_DEFAULT, &(sMark.m_lResultID));	// Result ID Allocation
	sMark.m_bIsModel = TRUE;

	std::vector<SMarkData>::iterator iter = m_vectorMarkData.begin();
	for (int j = 0; j < sMark.m_iMarkIndex; j++)
		iter++;

	m_vectorMarkData.insert(iter, sMark);
	m_vectorCamData[iCamNo].m_rgiMarkRelation[DEF_PM_MARK][iModelNo] = sMark.m_iMarkIndex;
	return ERR_VISION_SUCCESS;
}

//@int MVision::RegisterBLOBPattern(int iCamNo, int iModelNo, RECT& SearchArea, RECT& BlobArea)
int MVision::RegisterBLOBPattern(int iCamNo, int iModelNo, RECT& SearchArea, RECT& BlobArea, double dRefX, double dRefY)
{
#ifdef VISION_SIMULATION
	return ERR_VISION_SUCCESS;
#endif

	if (FALSE == IsValidCam(iCamNo))
		return getErrorCode(ERR_INVALID_CAMERA_NO);

	if (FALSE == isValidMarkNo(iModelNo))
		return getErrorCode(ERR_MARK_NO_RANGE_OVER, iCamNo);

//171004 JSH.s
// 	int iResult = checkAreaSize(BlobArea, SearchArea);
// 	if (iResult)
// 	{
// 		SetErrorLevel(_T("MVISION register Blob Pattern"),3, __FILE__, __LINE__);
// 		return iResult;
// 	}
//171004 JSH.e

	SMarkData sMark;
	memset(&sMark, 0, sizeof(SMarkData));
	sMark.m_iMarkIndex = getNewMarkStorageIndex();	// Model Storage Index
	sMark.m_eMarkType = DEF_BLOB_MARK;	// Mark Type
	sMark.m_dMaxLimit = 90.0;	// Score Max Limit : default = 90.0
	sMark.m_dMinLimit = 50.0;	// Score Min Limit : default = 50.0
	sMark.m_dPolarity = 0.0;	// Polarity : default = Black
	sMark.m_lModelLeft = BlobArea.left;	// Blob Area
	sMark.m_lModelTop = BlobArea.top;
	sMark.m_lModelRight = BlobArea.right;
	sMark.m_lModelBottom = BlobArea.bottom;
	//20121004 SJ_HJG 횡전개
	sMark.m_lSearchAreaLeft = SearchArea.left;	// Search Area
	sMark.m_lSearchAreaTop = SearchArea.top;
	sMark.m_lSearchAreaRight = SearchArea.right;
	sMark.m_lSearchAreaBottom = SearchArea.bottom;
	//______________________

	//@130119.KKY___________
	sMark.m_dReferenceX = dRefX;	//@모델Box의 Left가 Refrence마크 중심에서 떨어진 거리 X
	sMark.m_dReferenceY = dRefY;	//@모델Box의 Top이 Refrence마크 중심에서 떨어진 거리  Y
	//@_____________________

	sMark.m_dThreshold = 50.0;	// Threshold : default = 50
	sMark.m_bIsModel = TRUE;

	m_vectorMarkData.push_back(sMark);
	m_vectorCamData[iCamNo].m_rgiMarkRelation[DEF_BLOB_MARK][iModelNo] = sMark.m_iMarkIndex;
	return ERR_VISION_SUCCESS;
}

int MVision::cutModelLink(int iCamNo, EMarkType eMarkType, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, eMarkType, iModelNo);
	if (iIndex > -1)
	{
		m_vectorCamData[iCamNo].m_rgiMarkRelation[eMarkType][iModelNo] = -1;
		//120119.KKY_______
		//saveCameraData();
		saveMarkRelationData();
		//_________________
	}
	return iIndex;
}

void MVision::removeModelFile(int iModelStorageIndex)
{
	// Delete Pattern Matching Model Format Data File
	if (m_vectorMarkData[iModelStorageIndex].m_pszFileName != NULL)
		// 확인 필요 : File 이 존재하지 않으면 Fail ⇒ FALSE Return.
		DeleteFileA(m_vectorMarkData[iModelStorageIndex].m_pszFileName);

	// Delete Model Data Text
	std::string strFileName = m_strModelDataFilePath + "\\VisionModel.dat";
	makeBackupFile(strFileName);
	/* 확인 필요 : 계속 진행 가능하도록 처리 필요?
	if (makeBackupFile(strFileName) == FALSE)
		return;
	*/

	char szSection[DEF_MAX_CHR_LENGTH];
	memset(szSection, NULL, sizeof(szSection));
	sprintf(szSection, "Mark%d", m_vectorMarkData[iModelStorageIndex].m_iMarkIndex);
	WritePrivateProfileSectionA(szSection, "", strFileName.c_str());
}

void MVision::removeCameraData(int iCamIndex)
{
	CString strTemp;

	//20110825. SJ_YTH. Add..
	Sleep(50);
	//________

	if (-1 == iCamIndex)
	{
		int cnt = 0;
		while (m_vectorCamData.size() > 0)
		{
			Sleep(10);
			if (NULL != m_vectorCamData.back().m_pszDCFPath)
			{
				strTemp.Format(_T("%s"), m_vectorCamData.back().m_pszDCFPath);
				delete[] m_vectorCamData.back().m_pszDCFPath;
			}
			if (NULL != m_vectorCamData.back().m_pszDCFName)
			{
				strTemp.Format(_T("%s"), m_vectorCamData.back().m_pszDCFName);
				delete[] m_vectorCamData.back().m_pszDCFName;
			}
			
			if (NULL != m_vectorCamData.back().m_pszCameraName)
			{
				strTemp.Format(_T("%s"), m_vectorCamData.back().m_pszCameraName);
				delete[] m_vectorCamData.back().m_pszCameraName;
			}
			
			DeleteCriticalSection(&(m_vectorCamData.back().m_csGrab));
			DeleteCriticalSection(&(m_vectorCamData.back().m_csSource));
			m_vectorCamData.pop_back();
			cnt++;
		}

///		cnt = GetMaxUseCameraNo();
///		for (int i = 0; i < cnt; i++)
///			removeCameraData(i);
		return;
	}
	
	// 해당 Camera 를 Storage 에서 제거
	std::vector<SCameraData>::iterator k;
	for (k = m_vectorCamData.begin(); k < m_vectorCamData.end(); k++)
	{
		if ((*k).m_iUserCameraNo == iCamIndex)
		{
			if (NULL != (*k).m_pszDCFPath)
				delete[] (*k).m_pszDCFPath;
			
			if (NULL != (*k).m_pszDCFName)
				delete[] (*k).m_pszDCFName;
			
			if (NULL != (*k).m_pszCameraName)
				delete[] (*k).m_pszCameraName;
			
			DeleteCriticalSection(&((*k).m_csGrab));
			DeleteCriticalSection(&((*k).m_csSource));
			m_vectorCamData.erase(k);
			return;
		}
	}
}

void MVision::removeModelElement(int iModelStorageIndex)
{

	//110825. SJ_YTH. Add..
	Sleep(50);
	//________
	if (iModelStorageIndex == -1)	// iModelStorageIndex == -1 이면 All Model Remove
	{
		while (m_vectorMarkData.size() > 0)
		{
			Sleep(10);

			if (NULL != m_vectorMarkData.back().m_pszModelName)
				delete[] m_vectorMarkData.back().m_pszModelName;

			if (NULL != m_vectorMarkData.back().m_pszFileName)
				delete[] m_vectorMarkData.back().m_pszFileName;

			if (m_vectorMarkData.back().m_eMarkType == DEF_PM_MARK)
			{
				if (m_vectorMarkData.back().m_lModelID != M_NULL)
					MpatFree(m_vectorMarkData.back().m_lModelID);

				if (m_vectorMarkData.back().m_lResultID != M_NULL)
					MpatFree(m_vectorMarkData.back().m_lResultID);
			}

			m_vectorMarkData.pop_back();
		}
		return;
	}

	// 해당 Mark 를 Storage 에서 제거
	SMarkData* pMark = &(m_vectorMarkData[iModelStorageIndex]);
	if (NULL != pMark->m_pszModelName)
		delete[] pMark->m_pszModelName;
	
	if (NULL != pMark->m_pszFileName)
		delete[] pMark->m_pszFileName;
	
	if (pMark->m_eMarkType == DEF_PM_MARK)
	{
		if (pMark->m_lModelID != M_NULL)
			MpatFree(pMark->m_lModelID);
		
		if (pMark->m_lResultID != M_NULL)
			MpatFree(pMark->m_lResultID);
	}
	
	int iCount = 0;
	std::vector<SMarkData>::iterator k;
	for (k = m_vectorMarkData.begin(); k < m_vectorMarkData.end(); k++, iCount++)
	{
		if (iCount == iModelStorageIndex)
			break;
	}
	m_vectorMarkData.erase(k);
}

void MVision::DeleteMark(int iCamNo, EMarkType eMarkType, int iModelNo)
{
	// Cut Model Storage Link
	int iIndex = cutModelLink(iCamNo, eMarkType, iModelNo);
	if (iIndex == -1)
		return;

	if (m_vectorMarkData.size() <= 0)	// Mark Storage 에 저장된 내용이 없으면 작업 완료
		return;

	// 모든 Camera 에 대해서 iIndex 에 대한 Link 가 있는지 조사 => 없으면 Mark Storage 에서 삭제.
	int iTargetIndex = m_vectorMarkData[iIndex].m_iMarkIndex;
	std::vector<SCameraData>::iterator iter;
	for (iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++)
	{
		for (int j = 0; j < DEF_MAX_USE_MARK; j++)
		{
			if ((*iter).m_rgiMarkRelation[eMarkType][j] == iTargetIndex)
				return;
		}
	}

	removeModelFile(iIndex);	// 해당 Mark 에 대한 Model File, Model Data Text 제거
	removeModelElement(iIndex);	// 해당 Mark 를 Storage 에서 제거
}

int MVision::CheckModel(int iCamNo, EMarkType eModelType, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, eModelType, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	if (FALSE == m_vectorMarkData[iIndex].m_bIsModel)
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);

	return ERR_VISION_SUCCESS;
}

double MVision::GetSearchResultX(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dResultX;
}
	//20121004 SJ_HJG 횡전개
double MVision::GetSearchBlobResultX(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dResultX;
}
double MVision::GetSearchResultY(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dResultY;
}
	//20121004 SJ_HJG 횡전개
double MVision::GetSearchBlobResultY(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dResultY;
}

RECT MVision::GetSearchAreaRect(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (iIndex == -1)
	{
		RECT rect = { 0, 0, 0, 0 };
		return rect;
	}

	RECT rect = { m_vectorMarkData[iIndex].m_lSearchAreaLeft, m_vectorMarkData[iIndex].m_lSearchAreaTop,
		m_vectorMarkData[iIndex].m_lSearchAreaRight, m_vectorMarkData[iIndex].m_lSearchAreaBottom };
	return rect;
}

int MVision::SetModelName(int iCamNo, EMarkType eMarkType, int iModelNo, tstring strName)
{
	int iIndex = getMarkIndex(iCamNo, eMarkType, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);


	SMarkData *pMark = &(m_vectorMarkData[iIndex]);
	if (pMark->m_lModelID == M_NULL)
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);

	if (NULL != pMark->m_pszModelName)
		delete[] pMark->m_pszModelName;

//	pMark->m_pszModelName = new char[_tcslen(strName.c_str()) + 1];
	pMark->m_pszModelName = new char[DEF_MAX_CHR_LENGTH];
	strcpy(pMark->m_pszModelName, getSingleCharacterString(strName).c_str());
	return ERR_VISION_SUCCESS;
}

int MVision::SetSearchAcceptanceThreshold(int iCamNo, int iModelNo, double dValue)
{
	if (dValue < 0.0 || dValue > 100.0)
		return getErrorCode(ERR_INVALID_PARAMETER_VALUE, iCamNo);

	int iIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	SMarkData *pMark = &(m_vectorMarkData[iIndex]);
	if (pMark->m_lModelID == M_NULL)
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);

	pMark->m_dThreshold = dValue;
	MpatSetAcceptance(pMark->m_lModelID, dValue);
	MpatPreprocModel(m_vectorCamData[iCamNo].m_lSource, pMark->m_lModelID, M_DEFAULT);
	return ERR_VISION_SUCCESS;
}

double MVision::GetSearchAcceptanceThreshold(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_PM_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dThreshold;
}

int MVision::SetBlobThreshold(int iCamNo, int iModelNo, double dThreshold)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dThreshold = dThreshold;
	return ERR_VISION_SUCCESS;
}

double MVision::GetBlobThreshold(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dThreshold;
}

int MVision::SetBlobJudgeMaxPixelPercent(int iCamNo, int iModelNo, int iMaxPixelPercent)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dMaxLimit = iMaxPixelPercent;
	return ERR_VISION_SUCCESS;
}

int MVision::SetBlobJudgeMinPixelPercent(int iCamNo, int iModelNo, int iMinPixelPercent)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dMinLimit = iMinPixelPercent;
	return ERR_VISION_SUCCESS;
}

int MVision::GetBlobJudgeMinPixelPercent(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0;

	return (int)m_vectorMarkData[iIndex].m_dMinLimit;
}

int MVision::GetBlobJudgeMaxPixelPercent(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0;

	return (int)m_vectorMarkData[iIndex].m_dMaxLimit;
}

int MVision::SetBlobPolarity(int iCamNo, int iModelNo, double dPolarity)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dPolarity = dPolarity;
	return ERR_VISION_SUCCESS;
}

double MVision::GetBlobPolarity(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dPolarity;
}

double MVision::GetBlobResultScore(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dScore;
}

long MVision::GetBlobPixelsInModelArea(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (iIndex == -1)
		return (long)0.0;

	return (m_vectorMarkData[iIndex].m_lModelRight - m_vectorMarkData[iIndex].m_lModelLeft) * (m_vectorMarkData[iIndex].m_lModelBottom - m_vectorMarkData[iIndex].m_lModelTop);
}
	//20121004 SJ_HJG 횡전개
int MVision::GetBlobHistogram(int iCamNo, int iModelNo, unsigned char* rgucHist)
//int MVision::GetBlobHistogram(int iCamNo, int iModelNo, unsigned char* rgucHist, int* rglHistVals1)
{
#ifdef VISION_SIMULATION
	for (int j = 0; j < 256; j++)
		rgucHist[j] = (j < 128) ? j : 256 - j;

	return ERR_VISION_SUCCESS;
#endif

	int iIndex = getMarkIndex(iCamNo, DEF_BLOB_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	if (FALSE == m_vectorMarkData[iIndex].m_bIsModel)
		return getErrorCode(ERR_NOT_REGISTERED_MARK, iCamNo);

	ClearOverlay(iCamNo);	// Halt Camera 처리
	HaltVideo(iCamNo);

	RECT blob = { m_vectorMarkData[iIndex].m_lModelLeft, m_vectorMarkData[iIndex].m_lModelTop, m_vectorMarkData[iIndex].m_lModelRight, m_vectorMarkData[iIndex].m_lModelBottom };
	setDrawRectData(iCamNo, blob);

	int iSysIndex = getSystemIndex(iCamNo);
	long lBlob;
	MbufChild2d(m_vectorpSystem[iSysIndex]->GetSourceBuffer(iCamNo), blob.left, blob.top, blob.right - blob.left, blob.bottom - blob.top, &lBlob);
	if (lBlob == M_NULL)
		// 확인 필요.
		return -999;

	long lHistogram;	// Histogram buffer identifier.
	long rglHistVals[256];	// Histogram values.
	MimAllocResult(m_vectorpSystem[iSysIndex]->GetSysID(), 256L, M_HIST_LIST, &lHistogram);	// Allocate a histogram result buffer.
	MimHistogram(lBlob, lHistogram);	// Perform the histogram.
	MimGetResult(lHistogram, M_VALUE, rglHistVals);	// Get the results.
	for (int i = 0; i < 256; i++)
	{
		rgucHist[i] = static_cast<unsigned char>(rglHistVals[i]);
	//	rglHistVals1[i] = rglHistVals[i];	//20121004 SJ_HJG 횡전개
	}
	
	MimFree(lHistogram);
	MbufFree(lBlob);
	return ERR_VISION_SUCCESS;
}

int MVision::RegisterEdgeModel(int CameraNo, int MarkNo, RECT rectMeasurementBox)
{
	return ERR_VISION_SUCCESS;
}

int MVision::FindEdge(int iCameraNo, int iModelNo)
{
	return ERR_VISION_SUCCESS;
}

double MVision::GetEdgeFinderPolarity(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_CALIPER_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dPolarity;
}

double MVision::GetEdgeFinderThreshold(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_CALIPER_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dThreshold;
}

double MVision::GetEdgeFinderDirection(int iCamNo, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, DEF_CALIPER_MARK, iModelNo);
	if (iIndex == -1)
		return 0.0;

	return m_vectorMarkData[iIndex].m_dSearchDirection;
}

int MVision::SetEdgeFinderPolarity(int iCamNo, int iModelNo, double dPolarity)
{
	int iIndex = getMarkIndex(iCamNo, DEF_CALIPER_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dPolarity = dPolarity;
	return ERR_VISION_SUCCESS;
}

int MVision::SetEdgeFinderThreshold(int iCamNo, int iModelNo, double dThreshold)
{
	int iIndex = getMarkIndex(iCamNo, DEF_CALIPER_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dThreshold = dThreshold;
	return ERR_VISION_SUCCESS;
}

int MVision::SetEdgeFinderDirection(int iCamNo, int iModelNo, double dSearchDirection)
{
	int iIndex = getMarkIndex(iCamNo, DEF_CALIPER_MARK, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

	m_vectorMarkData[iIndex].m_dSearchDirection = dSearchDirection;
	return ERR_VISION_SUCCESS;
}

int MVision::WriteLog(const tstring& strFile, const tstring& strLog)
{
	EnterCriticalSection(&m_csLog);

	struct _timeb sTime;
	_ftime(&sTime);
	std::string strText = ctime(&sTime.time);

	std::string strDay;
	strDay.append(strText, 0, 10);

	std::string strNewFileName = getSingleCharacterString(strFile);
	int iExtIndex = strNewFileName.find_last_of(".");
	
	std::string strDate;
	strDate = "_" + strDay;
	strNewFileName.insert(iExtIndex, strDate);

	char szText[20];
	sprintf(szText, ".%03d] ", sTime.millitm);
	int iIndex = strText.length();
	strText.replace(iIndex - 6, iIndex, szText);
	strText = "[" + strText + getSingleCharacterString(strLog) + "\n";

	FILE* fp = fopen(strNewFileName.c_str(), "at");
	if (fp == NULL)
	{
		LeaveCriticalSection(&m_csLog);
		return FALSE;
	}

	fprintf(fp, "%s", strText.c_str());
	fclose(fp);
	LeaveCriticalSection(&m_csLog);
	return TRUE;
}

POINT MVision::GetDistribution(int iCamNo)
{
	POINT point	= { m_vectorCamData[iCamNo].m_lBufferSizeX,
					m_vectorCamData[iCamNo].m_lBufferSizeY };
	return point;
}

int MVision::ChangeModel(const tstring strModelDataFilePath)
{
//#ifdef VISION_SIMULATION
//	return ERR_VISION_SUCCESS;
//#endif

	// 확인 필요 : Delete Camera, Mark Data 추가.
	// 모델 체인지 시에 System Config Data 에 있는 Camera 개수와 Camera Data 내용이 안맞을 경우는 어떻게 되나?


	m_strModelDataFilePath = getSingleCharacterString(strModelDataFilePath);
	checkUsingFolders();

	removeModelElement(-1);
//120119.KKY.Del	removeCameraData(-1);
//120119.KKY.Del	loadCameraData();
//120119.KKY.Del	assignCameraDataAboutSystems();

	loadMarkRelationData();	//120119.KKY
	loadModelData();	// Vision Mark Load
	
	return ERR_VISION_SUCCESS;
}

void MVision::checkUsingFolders()
{
	// Config Data Folder, Camera Data Folder
	if (FALSE == PathIsDirectoryA(m_strVisionDataFilePath.c_str()))
		::CreateDirectoryA(m_strVisionDataFilePath.c_str(), NULL);

	// Model Data Folder
	if (FALSE == PathIsDirectoryA(m_strModelDataFilePath.c_str()))
		::CreateDirectoryA(m_strModelDataFilePath.c_str(), NULL);

	// Log Data Folder
	if (FALSE == PathIsDirectoryA(m_strLogDataFilePath.c_str()))
		::CreateDirectoryA(m_strLogDataFilePath.c_str(), NULL);

	// Image Data Folder
	if (FALSE == PathIsDirectoryA(m_strImageDataFilePath.c_str()))
		::CreateDirectoryA(m_strImageDataFilePath.c_str(), NULL);
}

/* 참고용 Code.
int* MVision::SetNumOfUseCam(int* rgiCamNum, int iNumOfUseCam, ...)
{
	if (rgiCamNum)
	{
		delete []rgiCamNum;
		rgiCamNum = NULL;
	}

	rgiCamNum = new int[iNumOfUseCam+1];
	long int	*pdata;
	__asm	MOV		pdata,	EBP
	pdata += 4;		// 2+1
	for (int i=0 ; i<iNumOfUseCam+1 ; i++)
		rgiCamNum[i] = *(pdata+i);

	return rgiCamNum;
}
*/

std::string MVision::getConsoleInput(const char* szOutMsg)
{
	if (AllocConsole() == NULL)
		return std::string("0");

	DWORD dwOut;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(hOut, szOutMsg, lstrlenA(szOutMsg), &dwOut, NULL);

	DWORD dwIn;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	char szIn[20];
	memset(szIn, 0, sizeof(szIn));
	ReadConsoleA(hIn, szIn, sizeof(szIn), &dwIn, NULL);
	szIn[dwIn - 2] = '\0';

	FreeConsole();
	return std::string(szIn);
}

void MVision::loadConfigData()
{
	// Vision System(Board) Type : Meteor2 m/c, Solios
	char szData[DEF_MAX_CHR_LENGTH];
	char szKeyName[64];
	memset(szKeyName, NULL, sizeof(szKeyName));
	BOOL bSaveOption = FALSE;
	
	std::vector<int> vector_iSystmeType;
	vector_iSystmeType.clear();

	std::string strFileName = m_strVisionDataFilePath + "\\vision.dat";

	m_iCpuCount = GetPrivateProfileIntA("CONFIG", "CPU_Count", -99, strFileName.c_str());
	// Data 유효성 검사
	if (m_iCpuCount == -99)
	{
		std::string strIn = getConsoleInput("CPU_Count : ");
		m_iCpuCount = atoi(strIn.c_str());
		bSaveOption = TRUE;
	}

	m_iSystemCount = 0;	// Vision System (Board) 개수
	for (int i = 0; ; i++)
	{
		sprintf(szKeyName, "System%dType", i);
		memset(szData, NULL, sizeof(szData));
		GetPrivateProfileStringA("CONFIG", szKeyName, "none", szData, 256, strFileName.c_str());
		_strlwr(szData);

		if (strcmp(szData, "meteor2mc") == 0)
		{
			vector_iSystmeType.push_back(DEF_METEOR_II_MC);
			m_iSystemCount++;
		}
		else if (strcmp(szData, "solios") == 0)
		{
			vector_iSystmeType.push_back(DEF_SOLIOS);
			m_iSystemCount++;
		}
		else
			break;
	}

	if (m_iSystemCount == 0)	// System Count Data 유효성 검사
	{
		std::string strIn = getConsoleInput("SystemCount : ");
		m_iSystemCount = atoi(strIn.c_str());

		// 설정된 값의 유효성 검토 : 확인 필요

		// 여기서 다시 System Type 입력 받아야 함.
		for (int i = 0; i < m_iSystemCount; i++)
		{
			sprintf(szKeyName, "System%dType : ", i);
			std::string strIn = getConsoleInput(szKeyName);
			_strlwr((char*)strIn.c_str());

			if (strcmp(strIn.c_str(), "meteor2mc") == 0)
				vector_iSystmeType.push_back(DEF_METEOR_II_MC);
			else //if (strcmp(strIn.c_str(), "solios") == 0)
				vector_iSystmeType.push_back(DEF_SOLIOS);
		}

		bSaveOption = TRUE;
	}

	// Vision System Type Array Initialize
	m_rgiVSystemType = new int[m_iSystemCount];
	for (int i = 0; i < m_iSystemCount; i++)
	{
		m_rgiVSystemType[i] = vector_iSystmeType[i];
	}

	m_iCameraCount = GetPrivateProfileIntA("CONFIG", "CameraCount", -99, strFileName.c_str());
	// Data 유효성 검사
	if (m_iCameraCount == -99)
	{
		std::string strIn = getConsoleInput("CameraCount : ");
		m_iCameraCount = atoi(strIn.c_str());
		bSaveOption = TRUE;
	}

	m_iErrorPrint = GetPrivateProfileIntA("CONFIG", "ErrorPrint", TRUE, strFileName.c_str());
	// Data 유효성 검사
	if (m_iErrorPrint != FALSE && m_iErrorPrint != TRUE)
	{
	}

	m_iSaveImage = GetPrivateProfileIntA("CONFIG", "SaveImage", FALSE, strFileName.c_str());
	// Data 유효성 검사
	if (m_iSaveImage != FALSE && m_iSaveImage != TRUE)
	{
	}

	m_lCameraChangeTime = GetPrivateProfileIntA("CONFIG", "CameraChangeTime", FALSE, strFileName.c_str());
	// Data 유효성 검사
	if (m_lCameraChangeTime < 0)
	{
		m_lCameraChangeTime = 0;
	}

	if (TRUE == bSaveOption)
	{
		saveConfigData();
	}
}

void MVision::loadCameraData()
{
	m_vectorCamData.clear();

	//120119.KKY_________
//	std::string strFileName = m_strModelDataFilePath + "\\Camera.dat";
	std::string strFileName = m_strVisionDataFilePath + "\\Camera.dat";
	//___________________

	BOOL bSaveOption = FALSE;
	SCameraData sCamData;
	char szSection[20];
//	char szData[64];
	char szData[DEF_MAX_CHR_LENGTH];
	for (int i = 0; i < m_iCameraCount; i++)
	{
		memset(&sCamData, 0, sizeof(SCameraData));

		memset(szSection, 0, sizeof(szSection));
		sprintf(szSection, "Camera%d", i);

		// Initialize Draw Objects Index
		sCamData.m_iRectIndex = -1;
		sCamData.m_iPointIndex = -1;
		sCamData.m_iTextIndex = -1;

		sCamData.m_iFixedViewID = GetPrivateProfileIntA(szSection, "FixedViewID", -99, strFileName.c_str());
		if (sCamData.m_iFixedViewID == -99)
		{
			std::string strIn = getConsoleInput("FixedViewID : ");

			// 확인 필요 : 값 유효성 검토 필요.
			sCamData.m_iFixedViewID = atoi(strIn.c_str());
			bSaveOption = TRUE;
		}

		sCamData.m_iUserCameraNo = i;

		sCamData.m_iInternalCameraNo = GetPrivateProfileIntA(szSection, "InternalCameraNo", -99, strFileName.c_str());
		if (sCamData.m_iInternalCameraNo == -99)
		{
			std::string strIn = getConsoleInput("InternalCameraNo : ");
			sCamData.m_iInternalCameraNo = atoi(strIn.c_str());
			bSaveOption = TRUE;
		}

/* 삭제 예정.
		sCamData.m_lGrabSettlingTime = GetPrivateProfileIntA(szSection, "GrabSettlingTime", -99, strFileName.c_str());
		if (sCamData.m_lGrabSettlingTime == -99)
		{
			std::string strIn = getConsoleInput("GrabSettlingTime : ");
			sCamData.m_lGrabSettlingTime = atoi(strIn.c_str());
			bSaveOption = TRUE;
		}
*/
		sCamData.m_lBufferSizeX = GetPrivateProfileIntA(szSection, "ResolutionX", -99, strFileName.c_str());
		if (sCamData.m_lBufferSizeX == -99)
		{
			std::string strIn = getConsoleInput("ResolutionX : ");
			sCamData.m_lBufferSizeX = atoi(strIn.c_str());
			bSaveOption = TRUE;
		}

		sCamData.m_lBufferSizeY = GetPrivateProfileIntA(szSection, "ResolutionY", -99, strFileName.c_str());
		if (sCamData.m_lBufferSizeY == -99)
		{
			std::string strIn = getConsoleInput("ResolutionY : ");
			sCamData.m_lBufferSizeY = atoi(strIn.c_str());
			bSaveOption = TRUE;
		}

		GetPrivateProfileStringA(szSection, "CameraName", "ERROR", szData, sizeof(szData), strFileName.c_str());
		if (strcmp(szData, "ERROR") == 0)
		{
			std::string strIn = getConsoleInput("CameraName : ");
			sCamData.m_pszCameraName = new char[strIn.length() + 1];
			strcpy(sCamData.m_pszCameraName, strIn.c_str());
			bSaveOption = TRUE;
		}
		else
		{
//			sCamData.m_pszCameraName = new char[lstrlenA(szData) + 1];
			sCamData.m_pszCameraName = new char[DEF_MAX_CHR_LENGTH];
			strcpy(sCamData.m_pszCameraName, szData);
		}

		GetPrivateProfileStringA(szSection, "DCFName", "ERROR", szData, sizeof(szData), strFileName.c_str());
		if (strcmp(szData, "ERROR") == 0)
		{
			std::string strIn = getConsoleInput("DCFName : ");
			sCamData.m_pszDCFName = new char[strIn.length() + 1];
			strcpy(sCamData.m_pszDCFName, strIn.c_str());
			bSaveOption = TRUE;
		}
		else
		{
//			sCamData.m_pszDCFName = new char[lstrlenA(szData) + 1];
			sCamData.m_pszDCFName = new char[DEF_MAX_CHR_LENGTH];
			strcpy(sCamData.m_pszDCFName, szData);
		}

		GetPrivateProfileStringA(szSection, "DCFPath", "ERROR", szData, sizeof(szData), strFileName.c_str());
		if (strcmp(szData, "ERROR") == 0)
		{
			std::string strIn = getConsoleInput("DCFPath : ");
			sCamData.m_pszDCFPath = new char[strIn.length() + 1];
			strcpy(sCamData.m_pszDCFPath, strIn.c_str());
			bSaveOption = TRUE;
		}
		else
		{
//			sCamData.m_pszDCFPath = new char[lstrlenA(szData) + 1];
			sCamData.m_pszDCFPath = new char[DEF_MAX_CHR_LENGTH];
			strcpy(sCamData.m_pszDCFPath, szData);
		}
/*
		// 확인 필요 : Mark 인덱스 저장 배열 초기화 추가.
		char szItem[32];
		for (int j = 0; j < DEF_MAX_MARK_TYPE; j++)
		{
			for (int k = 0; k < DEF_MAX_USE_MARK; k++)
			{
				memset(szItem, 0, sizeof(szItem));
				sprintf(szItem, "MarkRelation[%d][%d]", j, k);

#ifdef VISION_SIMULATION
				sCamData.m_rgiMarkRelation[j][k] = -1;				
#else
				sCamData.m_rgiMarkRelation[j][k] = GetPrivateProfileIntA(szSection, szItem, -1, strFileName.c_str());
#endif
				// 확인 필요 : 에러 처리. 유효성.
			}
		}
*/
		InitializeCriticalSection(&sCamData.m_csGrab);
		InitializeCriticalSection(&sCamData.m_csSource);
		m_vectorCamData.push_back(sCamData);		
	}

	if (TRUE == bSaveOption)
	{
		saveCameraData();
	}
}

void MVision::loadModelData()
{
	m_vectorMarkData.clear();

	std::string strFileName = m_strModelDataFilePath + "\\VisionModel.dat";

	// Camera Data 로부터 사용 Model Index Max 값을 얻는다.
	int iMax = -1;
	std::vector<SCameraData>::iterator iter;
	for (iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++)
	{
		for (int j = 0; j < DEF_MAX_MARK_TYPE; j++)
		{
			for (int k = 0; k < DEF_MAX_USE_MARK; k++)
			{
				if ((*iter).m_rgiMarkRelation[j][k] > iMax)
					iMax = (*iter).m_rgiMarkRelation[j][k];
			}
		}
	}
	iMax++;

	SMarkData sMark;
	char szSection[20];
	char szData[DEF_MAX_CHR_LENGTH];
	for (int j = 0; j < iMax; j++)
	{
		memset(&sMark, 0, sizeof(SMarkData));

		memset(szSection, 0, sizeof(szSection));
		sprintf(szSection, "Mark%d", j);

		sMark.m_bSuccess = FALSE;
		sMark.m_dScore = 0.0;
		sMark.m_dResultX = -99.0;
		sMark.m_dResultY = -99.0;

		// Mark Type 이 없으면 Skip! : Mark 는 나중에 다시 등록할 수 있으므로 값이 없다고 해서 Console 입력을 받지는 않는다.
		GetPrivateProfileStringA(szSection, "MarkType", "ERROR", szData, sizeof(szData), strFileName.c_str());
		if (strcmp(szData, "ERROR") == 0)
		{
			sMark.m_iMarkIndex = -1;
			m_vectorMarkData.push_back(sMark);	// Null Mark 를 m_vectorMarkData 에 추가.
			continue;
		}

		if (strcmp(szData, "PatternMatching") == 0)
			sMark.m_eMarkType = DEF_PM_MARK;
		else if (strcmp(szData, "BLOB") == 0)
			sMark.m_eMarkType = DEF_BLOB_MARK;
		else if (strcmp(szData, "Caliper") == 0)
			sMark.m_eMarkType = DEF_CALIPER_MARK;
		else
			sMark.m_eMarkType = DEF_NONE_MARK;

		sMark.m_iMarkIndex = j;	// Mark Array Index

		/* 확인 필요 - 현재 불필요. 일단 사용 안함.
		GetPrivateProfileStringA(szSection, "ModelName", "ERROR", szData, sizeof(szData), strFileName.c_str());
		if (strcmp(szData, "ERROR") == 0)
		{
			std::string strIn = getConsoleInput("ModelName : ");

			sMark.m_pszModelName = new char[strIn.length() + 1];
			strcpy(sMark.m_pszModelName, strIn.c_str());
		}
		else
		{
			sMark.m_pszModelName = new char[lstrlenA(szData) + 1];
			strcpy(sMark.m_pszModelName, szData);
		}
		*/

		if (sMark.m_eMarkType == DEF_PM_MARK)
		{
			sMark.m_iSystemIndex = GetPrivateProfileIntA(szSection, "SystemIndex", -99, strFileName.c_str());
			if (sMark.m_iSystemIndex == -99)
			{
				return;
			}
			sMark.m_lModelLeft = GetPrivateProfileIntA(szSection, "ModelLeft", -99, strFileName.c_str());
			if (sMark.m_lModelLeft == -99)
			{
				return;
			}
			sMark.m_lModelTop = GetPrivateProfileIntA(szSection, "ModelTop", -99, strFileName.c_str());
			if (sMark.m_lModelTop == -99)
			{
				return;
			}
			sMark.m_lModelRight = GetPrivateProfileIntA(szSection, "ModelRight", -99, strFileName.c_str());
			if (sMark.m_lModelRight == -99)
			{
				return;
			}
			sMark.m_lModelBottom = GetPrivateProfileIntA(szSection, "ModelBottom", -99, strFileName.c_str());
			if (sMark.m_lModelBottom == -99)
			{
				return;
			}
			sMark.m_dThreshold = GetPrivateProfileIntA(szSection, "Threshold", -99, strFileName.c_str());
			if (sMark.m_dThreshold == -99)
			{
				return;
			}
			sMark.m_dCertaintyThreshold = GetPrivateProfileIntA(szSection, "CertaintyThreshold", -99, strFileName.c_str());
			if (sMark.m_dCertaintyThreshold == -99)
			{
				return;
			}
			
			sMark.m_lSearchAreaLeft = GetPrivateProfileIntA(szSection, "SearchAreaLeft", -99, strFileName.c_str());
			if (sMark.m_lSearchAreaLeft == -99)
			{
				return;
			}
			sMark.m_lSearchAreaTop = GetPrivateProfileIntA(szSection, "SearchAreaTop", -99, strFileName.c_str());
			if (sMark.m_lSearchAreaTop == -99)
			{
				return;
			}
			sMark.m_lSearchAreaRight = GetPrivateProfileIntA(szSection, "SearchAreaRight", -99, strFileName.c_str());
			if (sMark.m_lSearchAreaRight == -99)
			{
				return;
			}
			sMark.m_lSearchAreaBottom = GetPrivateProfileIntA(szSection, "SearchAreaBottom", -99, strFileName.c_str());
			if (sMark.m_lSearchAreaBottom == -99)
			{
				return;
			}
			sMark.m_dReferenceX = GetPrivateProfileIntA(szSection, "ReferenceX", -99, strFileName.c_str());
			if (sMark.m_dReferenceX == -99)
			{
				return;
			}
			sMark.m_dReferenceY = GetPrivateProfileIntA(szSection, "ReferenceY", -99, strFileName.c_str());
			if (sMark.m_dReferenceY == -99)
			{
				return;
			}
			
			restoreSearchMark(sMark);
		}
		else if (sMark.m_eMarkType == DEF_BLOB_MARK)
		{
			sMark.m_lModelLeft = GetPrivateProfileIntA(szSection, "ModelLeft", -99, strFileName.c_str());
			if (sMark.m_lModelLeft == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_lModelTop = GetPrivateProfileIntA(szSection, "ModelTop", -99, strFileName.c_str());
			if (sMark.m_lModelTop == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_lModelRight = GetPrivateProfileIntA(szSection, "ModelRight", -99, strFileName.c_str());
			if (sMark.m_lModelRight == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_lModelBottom = GetPrivateProfileIntA(szSection, "ModelBottom", -99, strFileName.c_str());
			if (sMark.m_lModelBottom == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_dThreshold = GetPrivateProfileIntA(szSection, "Threshold", -99, strFileName.c_str());
			if (sMark.m_dThreshold == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_dPolarity = GetPrivateProfileIntA(szSection, "Polarity", -99, strFileName.c_str());
			if (sMark.m_dPolarity == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_dMaxLimit = GetPrivateProfileIntA(szSection, "MaxLimit", -99, strFileName.c_str());
			if (sMark.m_dMaxLimit == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}
			sMark.m_dMinLimit = GetPrivateProfileIntA(szSection, "MinLimit", -99, strFileName.c_str());
			if (sMark.m_dMinLimit == -99)
			{
				// 확인 필요 : Error 처리
				return;
			}

			//@KKY_____________________
			sMark.m_dReferenceX = GetPrivateProfileIntA(szSection, "ReferenceX", -99, strFileName.c_str());
			if (sMark.m_dReferenceX == -99)
			{
				return;
			}
			sMark.m_dReferenceY = GetPrivateProfileIntA(szSection, "ReferenceY", -99, strFileName.c_str());
			if (sMark.m_dReferenceY == -99)
			{
				return;
			}
			//@________________________
		}
		else if (sMark.m_eMarkType == DEF_CALIPER_MARK)
		{
/*			m_pszFileName;	// Model File Name
			m_lModelLeft;	// Model Area : PM - Model, BLOB - Blob Area, CALIPER - Measurement Box, OCR - OCR Reading Area
			m_lModelTop;
			m_lModelRight;
			m_lModelBottom;
			m_dThreshold;	// Threshold : PM - Acceptance Threshold, BLOB - Binary Threshold, CALIPER - Edge Threshold
			m_dPolarity;	// BLOB - 목표 Pixel 의 극성 (Black : 0 / White : 1), CALIPER - whether an edge is rising or falling (0 : M_NEGATIVE, 1 : M_POSITIVE)
			m_lModelID;		// Vision Model ID
			m_lResultID;	// Result Buffer Identifier
			m_dSearchDirection;	// Search Direction
*/
		}
		else // if (sMark.m_eMarkType == DEF_NONE_MARK)
			continue;

		sMark.m_bIsModel = TRUE;

		m_vectorMarkData.push_back(sMark);
	}
}

int MVision::restoreSearchMark(SMarkData& sMark)
{
	// 확인 필요 : 동일 마크를 시스템만 바꿔서 새로 생성할 때 문제 되는 것 없나?

	char szFileName[DEF_MAX_CHR_LENGTH];
	memset(szFileName, 0, sizeof(szFileName));
	sprintf(szFileName, "%s\\PM_mark%d.mmo", m_strModelDataFilePath.c_str(), sMark.m_iMarkIndex);

	if (sMark.m_pszFileName != NULL)
		delete[] sMark.m_pszFileName;

//	sMark.m_pszFileName = new char[lstrlenA(szFileName) + 1];
	sMark.m_pszFileName = new char[DEF_MAX_CHR_LENGTH];
	sprintf(sMark.m_pszFileName, "%s", szFileName);

	// 확인 필요 : File 존재 여부확인 작업 필요

	long lSysID = m_vectorpSystem[sMark.m_iSystemIndex]->GetSysID();

#ifndef VISION_SIMULATION
	MpatRestore(lSysID, szFileName, &sMark.m_lModelID);
	if (sMark.m_lModelID == M_NULL)
		// 확인 필요.
		return -999;//ERR_FILE_READ_FAILURE;

	MpatSetPosition(sMark.m_lModelID, sMark.m_lSearchAreaLeft, sMark.m_lSearchAreaTop, sMark.m_lSearchAreaRight - sMark.m_lSearchAreaLeft, sMark.m_lSearchAreaBottom - sMark.m_lSearchAreaTop);
	MpatPreprocModel(M_NULL, sMark.m_lModelID, M_DEFAULT);

	// Result ID Allocation
	MpatAllocResult(lSysID, M_DEFAULT, &(sMark.m_lResultID));
#endif
	
	sMark.m_bIsModel = TRUE;
	return ERR_VISION_SUCCESS;
}

int MVision::WriteModelData(int iCamNo, EMarkType eMarkType, int iModelNo)
{
	int iIndex = getMarkIndex(iCamNo, eMarkType, iModelNo);
	if (-1 == iIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iCamNo);

//120119.KKY.Del	saveCameraData();	// Camera 의 Mark Relation Array 를 File 에 저장.	
	saveMarkRelationData();	//120119.KKY

	SMarkData sMark = m_vectorMarkData[iIndex];
	return saveModelData(sMark);
}

int MVision::saveConfigData()
{
	char szData[DEF_MAX_CHR_LENGTH];
	char szKeyName[64];
	memset(szKeyName, NULL, sizeof(szKeyName));

	std::string strFileName = m_strVisionDataFilePath + "\\vision.dat";
	makeBackupFile(strFileName);

	/* 확인 필요 : 계속 진행 가능하도록 처리 필요?
	if (makeBackupFile(strFileName) == FALSE)
		// 확인 필요 : 계속 진행 가능하도록 처리 필요.
		return -999;
	*/

	// 확인 필요 : History 기능은 ?

	sprintf(szData, "%d", m_iCpuCount);
	if (WritePrivateProfileStringA("CONFIG", "CPU_Count", szData, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	for (int i = 0; i < m_iSystemCount; i++)
	{
		sprintf(szKeyName, "System%dType", i);
		memset(szData, NULL, sizeof(szData));
		if (m_rgiVSystemType[i] == DEF_METEOR_II_MC)
			sprintf(szData, "meteor2mc");
		else // if (m_rgiVSystemType[i] == DEF_SOLIOS)
			sprintf(szData, "solios");

		if (WritePrivateProfileStringA("CONFIG", szKeyName, szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;
	}

	sprintf(szData, "%d", m_iCameraCount);
	if (WritePrivateProfileStringA("CONFIG", "CameraCount", szData, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	sprintf(szData, "%d", m_iErrorPrint);
	if (WritePrivateProfileStringA("CONFIG", "ErrorPrint", szData, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	sprintf(szData, "%d", m_iSaveImage);
	if (WritePrivateProfileStringA("CONFIG", "SaveImage", szData, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	sprintf(szData, "%d", m_lCameraChangeTime);
	if (WritePrivateProfileStringA("CONFIG", "CameraChangeTime", szData, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	return ERR_VISION_SUCCESS;
}

int MVision::saveCameraData()
{
	//120119.KKY___________
//	std::string strFileName = m_strModelDataFilePath + "\\Camera.dat";
	std::string strFileName = m_strVisionDataFilePath + "\\Camera.dat";
	//_____________________
	makeBackupFile(strFileName);

	/* 확인 필요 : 계속 진행 가능하도록 처리 필요?
	if (makeBackupFile(strFileName) == FALSE)
		// 확인 필요 : 계속 진행 가능하도록 처리 필요.
		return -999;
	*/

	char szSection[20];
//	char szData[64];
	char szData[DEF_MAX_CHR_LENGTH];

	SCameraData* pCam = NULL;
	for (int i = 0; i < m_iCameraCount; i++)
	{
		pCam = &(m_vectorCamData[i]);
		memset(szSection, 0, sizeof(szSection));
		sprintf(szSection, "Camera%d", i);

		sprintf(szData, "%d", pCam->m_iFixedViewID);
		if (WritePrivateProfileStringA(szSection, "FixedViewID", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", pCam->m_iInternalCameraNo);
		if (WritePrivateProfileStringA(szSection, "InternalCameraNo", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

/* 삭제 예정.
		sprintf(szData, "%d", pCam->m_lGrabSettlingTime);
		if (WritePrivateProfileStringA(szSection, "GrabSettlingTime", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;
*/
		sprintf(szData, "%d", pCam->m_lBufferSizeX);
		if (WritePrivateProfileStringA(szSection, "ResolutionX", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", pCam->m_lBufferSizeY);
		if (WritePrivateProfileStringA(szSection, "ResolutionY", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		if (WritePrivateProfileStringA(szSection, "CameraName", pCam->m_pszCameraName, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		if (WritePrivateProfileStringA(szSection, "DCFName", pCam->m_pszDCFName, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		if (WritePrivateProfileStringA(szSection, "DCFPath", pCam->m_pszDCFPath, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		/*120119.KKY.Del
#ifndef VISION_SIMULATION
		char szKeyName[64];
		for (int j = 0; j < DEF_MAX_MARK_TYPE; j++)
		{
			for (int k = 0; k < DEF_MAX_USE_MARK; k++)
			{
				memset(szKeyName, 0, sizeof(szKeyName));
				sprintf(szKeyName, "MarkRelation[%d][%d]", j, k);

				sprintf(szData, "%d", pCam->m_rgiMarkRelation[j][k]);
				if (WritePrivateProfileStringA(szSection, szKeyName, szData, strFileName.c_str()) == FALSE)
					// 확인 필요 : RETURN VALUE
					return -999;
			}
		}
#endif
		*/
	}
	return ERR_VISION_SUCCESS;
}

int MVision::saveAllModelData()
{
	std::vector<SMarkData>::iterator iter;
	for (iter = m_vectorMarkData.begin(); iter < m_vectorMarkData.end(); iter++)
	{
		int iResult = saveModelData(*iter);
		if (iResult)
			return iResult;
	}
	return ERR_VISION_SUCCESS;
}

int MVision::saveModelData(SMarkData sMark)
{
	std::string strFileName = m_strModelDataFilePath + "\\VisionModel.dat";
	makeBackupFile(strFileName);

	/* 확인 필요 : 계속 진행 가능하도록 처리 필요?
	if (makeBackupFile(strFileName) == FALSE)
		// 확인 필요 : 계속 진행 가능하도록 처리 필요.
		return -999;
	*/
	
	char szData[DEF_MAX_CHR_LENGTH];
	char szSection[20];
	memset(szSection, 0, sizeof(szSection));
	sprintf(szSection, "Mark%d", sMark.m_iMarkIndex);
	
	if (sMark.m_eMarkType == DEF_PM_MARK)
		sprintf(szData, "PatternMatching");
	else if (sMark.m_eMarkType == DEF_BLOB_MARK)
		sprintf(szData, "BLOB");
	else if (sMark.m_eMarkType == DEF_CALIPER_MARK)
		sprintf(szData, "Caliper");
	else
		// 확인 필요 :
		return -999;

	if (WritePrivateProfileStringA(szSection, "MarkType", szData, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	if (WritePrivateProfileStringA(szSection, "ModelName", sMark.m_pszModelName, strFileName.c_str()) == FALSE)
		// 확인 필요 : RETURN VALUE
		return -999;

	if (sMark.m_eMarkType == DEF_PM_MARK)
	{
		sprintf(szData, "%d", sMark.m_iSystemIndex);
		if (WritePrivateProfileStringA(szSection, "SystemIndex", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelLeft);
		if (WritePrivateProfileStringA(szSection, "ModelLeft", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelTop);
		if (WritePrivateProfileStringA(szSection, "ModelTop", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelRight);
		if (WritePrivateProfileStringA(szSection, "ModelRight", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelBottom);
		if (WritePrivateProfileStringA(szSection, "ModelBottom", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.1lf", sMark.m_dThreshold);
		if (WritePrivateProfileStringA(szSection, "Threshold", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.1lf", sMark.m_dCertaintyThreshold);
		if (WritePrivateProfileStringA(szSection, "CertaintyThreshold", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lSearchAreaLeft);
		if (WritePrivateProfileStringA(szSection, "SearchAreaLeft", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lSearchAreaTop);
		if (WritePrivateProfileStringA(szSection, "SearchAreaTop", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lSearchAreaRight);
		if (WritePrivateProfileStringA(szSection, "SearchAreaRight", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lSearchAreaBottom);
		if (WritePrivateProfileStringA(szSection, "SearchAreaBottom", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.3lf", sMark.m_dReferenceX);
		if (WritePrivateProfileStringA(szSection, "ReferenceX", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.3lf", sMark.m_dReferenceY);
		if (WritePrivateProfileStringA(szSection, "ReferenceY", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		MpatSave(sMark.m_pszFileName, sMark.m_lModelID);
	}
	else if (sMark.m_eMarkType == DEF_BLOB_MARK)
	{
		sprintf(szData, "%d", sMark.m_lModelLeft);
		if (WritePrivateProfileStringA(szSection, "ModelLeft", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelTop);
		if (WritePrivateProfileStringA(szSection, "ModelTop", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelRight);
		if (WritePrivateProfileStringA(szSection, "ModelRight", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%d", sMark.m_lModelBottom);
		if (WritePrivateProfileStringA(szSection, "ModelBottom", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.1lf", sMark.m_dThreshold);
		if (WritePrivateProfileStringA(szSection, "Threshold", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.1lf", sMark.m_dPolarity);
		if (WritePrivateProfileStringA(szSection, "Polarity", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.1lf", sMark.m_dMaxLimit);
		if (WritePrivateProfileStringA(szSection, "MaxLimit", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.1lf", sMark.m_dMinLimit);
		if (WritePrivateProfileStringA(szSection, "MinLimit", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		//@130119.KKY__________________
		sprintf(szData, "%.3lf", sMark.m_dReferenceX);
		if (WritePrivateProfileStringA(szSection, "ReferenceX", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;

		sprintf(szData, "%.3lf", sMark.m_dReferenceY);
		if (WritePrivateProfileStringA(szSection, "ReferenceY", szData, strFileName.c_str()) == FALSE)
			// 확인 필요 : RETURN VALUE
			return -999;
		//@____________________________
	}
	else if (sMark.m_eMarkType == DEF_CALIPER_MARK)
	{
		/*			m_pszFileName;	// Model File Name
		m_lModelLeft;	// Model Area : PM - Model, BLOB - Blob Area, CALIPER - Measurement Box, OCR - OCR Reading Area
		m_lModelTop;
		m_lModelRight;
		m_lModelBottom;
		m_dThreshold;	// Threshold : PM - Acceptance Threshold, BLOB - Binary Threshold, CALIPER - Edge Threshold
		m_dPolarity;	// BLOB - 목표 Pixel 의 극성 (Black : 0 / White : 1), CALIPER - whether an edge is rising or falling (0 : M_NEGATIVE, 1 : M_POSITIVE)
		m_lModelID;		// Vision Model ID
		m_lResultID;	// Result Buffer Identifier
		m_dSearchDirection;	// Search Direction
		*/
	}
	return ERR_VISION_SUCCESS;
}

int	MVision::ImportModel(EMarkType eMarkType, int iSrcCamNo, int iSrcMarkNo, int iDstCamNo, int iDstMarkNo)
{
	int iSrcIndex = getMarkIndex(iSrcCamNo, eMarkType, iSrcMarkNo);
	if (-1 == iSrcIndex)
		return getErrorCode(ERR_INVALID_MARK_INDEX, iSrcCamNo);

	if (m_vectorMarkData[iSrcIndex].m_bIsModel == FALSE)
		// 확인 필요.
		return -999;

	SMarkData* pMark = &(m_vectorMarkData[iSrcIndex]);
	SMarkData sMark;
	memcpy(&sMark, pMark, sizeof(SMarkData));

	int iDstIndex = getMarkIndex(iDstCamNo, eMarkType, iDstMarkNo);
	if (iDstIndex != -1)
		DeleteMark(iDstCamNo, eMarkType, iDstMarkNo);

	if (eMarkType == DEF_PM_MARK)
	{
		int iDstSysIndex = getSystemIndex(iDstCamNo);
		sMark.m_iSystemIndex = iDstSysIndex;

		// Model Name 은 외부에서 Setting 한다.
		sMark.m_pszModelName = NULL;

		sMark.m_pszFileName = NULL;
		restoreSearchMark(sMark);

		sMark.m_iMarkIndex = getNewMarkStorageIndex();
		
		char szFileName[DEF_MAX_CHR_LENGTH];
		memset(szFileName, 0, sizeof(szFileName));
		sprintf(szFileName, "%s\\PM_mark%d.mmo", m_strModelDataFilePath.c_str(), sMark.m_iMarkIndex);
		
		if (sMark.m_pszFileName != NULL)
			delete[] sMark.m_pszFileName;

//		sMark.m_pszFileName = new char[strlen(szFileName) + 1];
		sMark.m_pszFileName = new char[DEF_MAX_CHR_LENGTH];
		strcpy(sMark.m_pszFileName, szFileName);

		m_vectorMarkData.push_back(sMark);
		
		saveModelData(sMark);
	}
	else if (eMarkType == DEF_BLOB_MARK)
	{
		sMark.m_iMarkIndex = getNewMarkStorageIndex();

		CRect rectSearch;
		rectSearch.left = sMark.m_lSearchAreaLeft;
		rectSearch.top = sMark.m_lSearchAreaTop;
		rectSearch.right = sMark.m_lSearchAreaRight;
		rectSearch.bottom = sMark.m_lSearchAreaBottom;

		CRect rectBlob;
//171004 JSH.s
		rectBlob.left = sMark.m_lModelLeft;
		rectBlob.top = sMark.m_lModelTop;
		rectBlob.right = sMark.m_lModelRight;
		rectBlob.bottom = sMark.m_lModelBottom;
//171004 JSH.e

		int iResult = RegisterBLOBPattern(iDstCamNo, iDstMarkNo, rectSearch, rectBlob);
		if (iResult)
			return iResult;
	}
	else
	{
		return getErrorCode(ERR_NOT_SUPPORTED_OPERATION);
	}

	m_vectorCamData[iDstCamNo].m_rgiMarkRelation[eMarkType][iDstMarkNo] = sMark.m_iMarkIndex;	
	return ERR_VISION_SUCCESS;
}

int MVision::makeBackupFile(std::string strSrc)
{
	int iIndex = strSrc.find(".dat");
	if (iIndex < 1)
		return FALSE;

	std::string strDst = strSrc;
	strDst.replace(iIndex, iIndex + 3, ".bak");

	if (::CopyFileA(strSrc.c_str(), strDst.c_str(), FALSE) == FALSE)
	{
		DWORD dwordErrorCode = GetLastError();
		return FALSE;
	}
	return TRUE;
}

void MVision::DrawSourceImage(int iCamNo)
{
	m_vectorCamData[iCamNo].m_hWnd = getViewHandle(iCamNo);
	redraw(iCamNo);
}

std::string MVision::getSingleCharacterString(tstring str)
{
#ifndef UNICODE
	return str;
#endif

	int iLength = str.length() + 1;
//	char* pszSC = new char[iLength];
	char* pszSC = new char[DEF_MAX_CHR_LENGTH];
	size_t count = wcstombs(pszSC, str.c_str(), (size_t)iLength);
	if (-1 == count)
		return "";

	std::string strSC = pszSC;
	delete[] pszSC;

	return strSC;
}
//120119.KKY________________________________
void MVision::loadMarkRelationData()
{
	std::string strFileName = m_strModelDataFilePath + "\\Camera.dat";

	char szSection[20];
	
	std::vector<SCameraData>::iterator iter;
	int iCamNo = 0;
	for ( iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++, iCamNo++)
	{
		memset(szSection, 0, sizeof(szSection));
		sprintf(szSection, "Camera%d", iCamNo);

		char szItem[32];
		for (int j = 0; j < DEF_MAX_MARK_TYPE; j++)
		{
			for (int k = 0; k < DEF_MAX_USE_MARK; k++)
			{
				memset(szItem, 0, sizeof(szItem));
				sprintf(szItem, "MarkRelation[%d][%d]", j, k);						
#ifdef VISION_SIMULATION
				(*iter).m_rgiMarkRelation[j][k] = -1;				
#else
				(*iter).m_rgiMarkRelation[j][k] = GetPrivateProfileIntA(szSection, szItem, -1, strFileName.c_str());
#endif
			}
		}		
	}
}

int MVision::saveMarkRelationData()
{
	std::string strFileName = m_strModelDataFilePath + "\\Camera.dat";

	char szSection[20];
	char szData[DEF_MAX_CHR_LENGTH];
	int iCamNo = 0;
	
	std::vector<SCameraData>::iterator iter;
	for ( iter = m_vectorCamData.begin(); iter < m_vectorCamData.end(); iter++, iCamNo++)
	{
		memset(szSection, 0, sizeof(szSection));
		sprintf(szSection, "Camera%d", iCamNo);

		char szItem[32];
		for (int j = 0; j < DEF_MAX_MARK_TYPE; j++)
		{
			for (int k = 0; k < DEF_MAX_USE_MARK; k++)
			{
				memset(szItem, 0, sizeof(szItem));
				sprintf(szItem, "MarkRelation[%d][%d]", j, k);				
				sprintf(szData, "%d", (*iter).m_rgiMarkRelation[j][k]);
				if (WritePrivateProfileStringA(szSection, szItem, szData, strFileName.c_str()) == FALSE)
					// 확인 필요 : RETURN VALUE
					return -999;
			}
		}
	}

	return ERR_VISION_SUCCESS;
}
//__________________________________________
