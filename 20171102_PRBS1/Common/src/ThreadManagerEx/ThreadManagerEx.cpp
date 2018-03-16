/***
* ThreadEx.cpp - Kwangil, Shin
*******************************************************************************/

/* ------------------------------------------------------------------------------------- */
// Dependencies
/* ------------------------------------------------------------------------------------- */
#include "stdafx.h"
#include "mplatformolb.h"
#include "ThreadManagerEx.h"

#include <process.h>
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long CThreadManagerEx::m_lInstance = 0;

/* ------------------------------------------------------------------------------------- */
// Construction
/* ------------------------------------------------------------------------------------- */
CThreadManagerEx::CThreadManagerEx()
{
	InitInstance();
}


/* ------------------------------------------------------------------------------------- */
// class Destructor
/* ------------------------------------------------------------------------------------- */
CThreadManagerEx::~CThreadManagerEx()
{
	Destroy();
}


/* ------------------------------------------------------------------------------------- */
// class DoDataExchange
/* ------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------- */
// class Event Map
/* ------------------------------------------------------------------------------------- */


/* ##################################################################################### */

/* ===================================================================================== */
// System Control Service - Public Function start
/* ------------------------------------------------------------------------------------- */

/* Thread create.
---------------------------------------------*/
TrdRetEx::Status CThreadManagerEx::CreateThread(LPVOID lpvParam, _stTrdInfo *pstTrdInfo, LPCTSTR lpszThreadName, pThreadAddress pCreateFunction)
{
	CString strMsg;

	// Thread Syc
	//
	::InterlockedIncrement(&CThreadManagerEx::m_lInstance);

	// System Thread Name
	//
	pstTrdInfo->strTrdName.Format(_T("Thread Name %s %02d"), lpszThreadName, m_lInstance);
	pstTrdInfo->nTrdSyncID	= m_lInstance;

	// Thread Event Alc
	//
	if (!pstTrdInfo->hTerminateEvent)
	{
		pstTrdInfo->hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, pstTrdInfo->strTrdName + _T(" Terminate Event"));
		
		ASSERT(pstTrdInfo->hTerminateEvent != NULL);
		if (!pstTrdInfo->hTerminateEvent)
		{
			pstTrdInfo->enumStatus = TrdRetEx::eErrorEvent;
			
			return pstTrdInfo->enumStatus;
		}
	}
	

	// Send Event Alc
	//
	if (!pstTrdInfo->hSendEvent)
	{
		pstTrdInfo->hSendEvent = CreateEvent(NULL, TRUE, FALSE, pstTrdInfo->strTrdName + _T(" Send Event"));
		
		ASSERT(pstTrdInfo->hSendEvent != NULL);
		if (!pstTrdInfo->hSendEvent)
		{
			pstTrdInfo->enumStatus = TrdRetEx::eErrorEvent;

			return pstTrdInfo->enumStatus;
		}
		else
		{
			ResetEvent(pstTrdInfo->hSendEvent);
		}
	}
	

	// Thread Suspend Type Create
	//
	if (!pstTrdInfo->hThread)
	{
		pstTrdInfo->pCreateFunction = pCreateFunction;
		pstTrdInfo->lpvParam		= lpvParam;

		pstTrdInfo->hThread = (HANDLE)(UINT_PTR)_beginthreadex(NULL, 0, pstTrdInfo->pCreateFunction, (LPVOID)pstTrdInfo, pstTrdInfo->dwCreationFlags, &pstTrdInfo->uThreadID);
	}

	ASSERT(INVALID_HANDLE_VALUE != pstTrdInfo->hThread);
	ASSERT(pstTrdInfo->hThread != NULL);

	if (!pstTrdInfo->hThread || INVALID_HANDLE_VALUE == pstTrdInfo->hThread)
	{
		strMsg.Empty();
		strMsg.Format(_T("Check the create thread status - Not thread create.! : %s \n"), pstTrdInfo->strTrdName);
		TRACE (strMsg);

		pstTrdInfo->enumStatus = TrdRetEx::eErrorCreate;
	}
	else
	{
		if (pstTrdInfo->dwCreationFlags & CREATE_SUSPENDED)
		{
			VERIFY(ResumeThread(pstTrdInfo->hThread) != (DWORD)-1);

			pstTrdInfo->enumStatus = TrdRetEx::eResume;
		}
		else
		{
			pstTrdInfo->enumStatus = TrdRetEx::eSuccess;
		}
	}

#ifdef _DEBUG
	TCHAR szMessage[MAX_PATH];
	_stprintf(szMessage, _T("The Create %s : Handle = %p, ID = %d\n"), pstTrdInfo->strTrdName, pstTrdInfo->hThread, pstTrdInfo->uThreadID);
	TRACE(szMessage);
#endif

	return pstTrdInfo->enumStatus;
}


/* Thread execute.
---------------------------------------------*/
TrdRetEx::Status CThreadManagerEx::CreateThreadExecute(_stTrdInfo *pstTrdInfo)
{
	DWORD pRetEventState;

	pRetEventState = WaitForSingleObject(pstTrdInfo->hSendEvent, 100);

	if (pRetEventState != WAIT_OBJECT_0)
	{
		if (SetEvent(pstTrdInfo->hSendEvent))
		{
			pstTrdInfo->enumStatus = TrdRetEx::eSuccess;
		}
		else
		{
			pstTrdInfo->enumStatus = TrdRetEx::eError;
		}
	}
	else
	{
		pstTrdInfo->enumStatus = TrdRetEx::eResumeState;
	}

	return pstTrdInfo->enumStatus;
}


/* Thread Suspend.
---------------------------------------------*/
TrdRetEx::Status CThreadManagerEx::CreateThreadSuspend(_stTrdInfo *pstTrdInfo)
{
	static int nRtyCount = 0;

	if (ResetEvent(pstTrdInfo->hSendEvent))
	{
		pstTrdInfo->enumStatus = TrdRetEx::eSuspend;

		do
		{
			DWORD pRetEventState = ::MsgWaitForMultipleObjects(pstTrdInfo->uThreadID, &pstTrdInfo->hSendEvent, FALSE, pstTrdInfo->dwExitTime, QS_ALLINPUT);

			if (pRetEventState == (WAIT_OBJECT_0 + pstTrdInfo->uThreadID))
			{
				break;
			}
			else if (pRetEventState == WAIT_TIMEOUT || pRetEventState == WAIT_ABANDONED)
			{
				ResetEvent(pstTrdInfo->hSendEvent);
			}

			nRtyCount++;

		} while (pstTrdInfo->hThread && nRtyCount < pstTrdInfo->nRtyEvt);
	}
	else
	{
		pstTrdInfo->enumStatus = TrdRetEx::eError;
	}

	return pstTrdInfo->enumStatus;
}


/* Thread stop.
---------------------------------------------*/
TrdRetEx::Status CThreadManagerEx::CreateThreadStop(_stTrdInfo *pstTrdInfo)
{
	pstTrdInfo->enumStatus = TrdRetEx::eDestroying;

	return pstTrdInfo->enumStatus;
}


/* Thread terminate.
---------------------------------------------*/
TrdRetEx::Status CThreadManagerEx::CreateThreadTerminate(_stTrdInfo *pstTrdInfo)
{
	TrdRetEx::Status	enumResult	= TrdRetEx::eError;
	DWORD				dwRunTime	= 0;


	::InterlockedDecrement(&CThreadManagerEx::m_lInstance);

	pstTrdInfo->enumStatus	= TrdRetEx::eDestroying;
	pstTrdInfo->bExitFlag	= TRUE;

	ResetEvent(pstTrdInfo->hSendEvent);
	SetEvent(pstTrdInfo->hTerminateEvent);

	if (!Check_CreateThreadExiting(pstTrdInfo))
	{
		pstTrdInfo->dwTerminateTimeOut = ::MsgWaitForMultipleObjects(pstTrdInfo->uThreadID, &pstTrdInfo->hTerminateEvent, FALSE, pstTrdInfo->dwExitTime, QS_ALLINPUT);

		while (pstTrdInfo->dwExitTime >= dwRunTime)
		{
			if (Check_CreateThreadExit(pstTrdInfo))
			{
#ifdef _DEBUG
				TCHAR szMessage[MAX_PATH];
				_stprintf(szMessage, _T("The %s exit : Handle = %p, ID = %d\n"), pstTrdInfo->strTrdName, pstTrdInfo->hThread, pstTrdInfo->uThreadID);
				TRACE(szMessage);
#endif
				enumResult = TrdRetEx::eDestroyed;

				break;
			}

			dwRunTime += WaitTime(1);
		}

		if (enumResult != TrdRetEx::eDestroyed)
		{
			TerminateThread(pstTrdInfo->hThread, 10L);

#ifdef _DEBUG
			TCHAR szMessage[MAX_PATH];
			_stprintf(szMessage, _T("The %s time out forcing exit : Handle = %p, ID = %d\n"), pstTrdInfo->strTrdName, pstTrdInfo->hThread, pstTrdInfo->uThreadID);
			TRACE(szMessage);
#endif
			enumResult = TrdRetEx::eDestroyed;
		}
	}
	else
	{
#ifdef _DEBUG
		TCHAR szMessage[MAX_PATH];
		_stprintf(szMessage, _T("The %s exit : Handle = %p, ID = %d\n"), pstTrdInfo->strTrdName, pstTrdInfo->hThread, pstTrdInfo->uThreadID);
		TRACE(szMessage);
#endif
		enumResult = TrdRetEx::eDestroyed;
	}

	return enumResult;
}


/* Thread state check.
---------------------------------------------*/
BOOL CThreadManagerEx::Check_CreateThread(_stTrdInfo *pstTrdInfo)
{
	if (pstTrdInfo->hThread)	return TRUE;

	return FALSE;
}


/* Thread state check - stop.
---------------------------------------------*/
BOOL CThreadManagerEx::Check_CreateThreadExiting(_stTrdInfo *pstTrdInfo)
{
	return (pstTrdInfo->enumStatus == TrdRetEx::eDestroying || pstTrdInfo->enumStatus == TrdRetEx::eDestroyed);
}


/* Thread state check - Exit.
---------------------------------------------*/
BOOL CThreadManagerEx::Check_CreateThreadExit(_stTrdInfo *pstTrdInfo)
{
	return (pstTrdInfo->enumStatus == TrdRetEx::eDestroyed);
}


/* Thread state check.
---------------------------------------------*/
TrdRetEx::Status CThreadManagerEx::Check_CreateThreadState(_stTrdInfo *pstTrdInfo)
{
	return pstTrdInfo->enumStatus;
}


/* PeekMessage Wait Time - Sleep.
--------------------------------------------- */
DWORD CThreadManagerEx::WaitTime(DWORD dwMsec)
{
	MSG		msg;
	DWORD	dwStart = 0;
	DWORD	dwEnd	= 0;

	dwStart			= GetTickCount();
	dwEnd			= GetTickCount();

	while (dwEnd - dwStart <= dwMsec)
	{
		dwEnd = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(1);
	}

	return (dwEnd - dwStart);
}

/* ------------------------------------------------------------------------------------- */
// System Control Service - Public Function End
/* ===================================================================================== */


/* ===================================================================================== */
// System Control Service - Protected Function start
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// System Control Service - Protected Function End
/* ===================================================================================== */



/* ===================================================================================== */
// System Control Service - private Function start
/* ------------------------------------------------------------------------------------- */

/* LIB initialization.
--------------------------------------------- */
BOOL CThreadManagerEx::InitInstance()
{

	return TRUE;
}


/* LIB destroy.
* [01]	 		: None.
* Result		: None.
--------------------------------------------- */
void CThreadManagerEx::Destroy()
{

}

/* ------------------------------------------------------------------------------------- */
// System Control Service - private Function End
/* ===================================================================================== */


/* ##################################################################################### */