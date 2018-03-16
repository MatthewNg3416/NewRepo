/***
* CThreadManagerEx - Debug Thread Controller 
*
*
*       Copyright (c) Doolin. All rights reserved.
*
* Created	: Kwangilshiin, Kwangilshin@Naver.com
*			: 2017. 08. 02.
*
* Purpose	: Control method using thread.
*
*
*		Base	2017. 08. 03. Kwangilshin.
*
*******************************************************************************/

#if !defined(AFX_THREADMANAGEREX_H__F8AE630C_3568_4BB6_98AF_D30897AB4011__INCLUDED_)
#define AFX_THREADMANAGEREX_H__F8AE630C_3568_4BB6_98AF_D30897AB4011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxmt.h>


/* ------------------------------------------------------------------------------------- */
// Class Definitions 
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Namespace Definitions 
/* ------------------------------------------------------------------------------------- */
// Thread Status - Cmd and Ret
//
namespace TrdRetEx
{
	enum Status
	{
		eSuccess			= 0,	
		eTerminate,
		eSuspend,
		eResume,
		eDestroying,
		eDestroyed,
		eResumeState,
		eSuspendState,
		eErrorCreate,
		eErrorEvent,
		eError,
		eNone,
	};
};

/* ------------------------------------------------------------------------------------- */
// Module Group definitions. 
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Definitions 
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// enum definitions
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Typedef 
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// Event ID
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// STD Class
/* ------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------- */
// LIB Class - Motion Infomation. 
/* ------------------------------------------------------------------------------------- */
class CThreadManagerEx
{
public:

	/* --------------------------------------------------------------------------------- */
	// Construction
	/* --------------------------------------------------------------------------------- */
	CThreadManagerEx();

	/* --------------------------------------------------------------------------------- */
	// Destruction
	/* --------------------------------------------------------------------------------- */
	virtual					~CThreadManagerEx();

	/* ================================================================================= */
	// Typedef definitions
	/* ================================================================================= */

public:	/* Typedef definitions */

	// Thread sync
	//
	static long				m_lInstance;

	// Thread Flg.
	enum
	{
		WaitsSend		= 0,
		WaitsTerminate	= 1,
		WaitsEnd		= 2
	};


	// Thread Function
	//
	typedef UINT(WINAPI *pThreadAddress) (LPVOID lpParam);				// Thread Function Addr
	
	
	// Thread Info Flg
	//
	typedef struct TrdInfoEx
	{
		int					nUniqueID;
		int					nTrdSyncID;				// Thread Sync ID.
		LPVOID				lpvParam;				// Call Class Hnd.
		HANDLE				hThread;				// Thread Handle 
		UINT				uThreadID;				// Thread ID 
		DWORD				dwExitCode;				// Exit Flg
		TrdRetEx::Status	enumStatus;				// Thread state
		DWORD				dwCreationFlags;		// Initial start position of the thread
		
		BOOL				bExitFlag;				// Thread Exit values 
		DWORD				dwExitTime;				// Thread Exit Wite Time
		DWORD				dwWaitTime;				// Thread while wiat time.
		
		HANDLE				hSendEvent;				// Thread Run Msg
		HANDLE				hTerminateEvent;		// Thread Exit Msg
		CCriticalSection	csTrd;
		
		pThreadAddress		pCreateFunction;		// Thread Fucntion
		CString				strTrdName;				// Thread Name
		
		DWORD				dwTerminateTimeOut;		// Terminate Time Out
		int					nRtyEvt;				// Terminate Retry Count
		
		void Destroy()
		{
			nUniqueID				= -1;
			nTrdSyncID				= -1;

			lpvParam				= NULL;

			hThread					= NULL;
			hThread					= 0;
			uThreadID				= -1;
			dwExitCode				= 0;
			
			enumStatus				= TrdRetEx::eNone;
			
			dwCreationFlags			= CREATE_SUSPENDED;
			
			bExitFlag				= FALSE;
			dwExitTime				= 500;
			dwWaitTime				= 1;
			
			hSendEvent				= NULL;
			hTerminateEvent			= NULL;
			
			strTrdName.Empty();
			
			dwTerminateTimeOut		= 0;
			nRtyEvt					= 3;
		}
		
		void memDelete()
		{
			// Therad
			//
			if (hThread)			{ CloseHandle(hThread);				hThread				= NULL; }
			
			// Event
			//
			if (hSendEvent)			{ CloseHandle(hSendEvent);			hSendEvent			= NULL; }
			if (hTerminateEvent)	{ CloseHandle(hTerminateEvent);		hTerminateEvent		= NULL; }
		}
		
		TrdInfoEx()
		{
			Destroy();
		}
		
	} _stTrdInfo;

protected:	
private:	



	/* ================================================================================= */
	// Variable definitions
	/* ================================================================================= */
public:
protected:
private:


	/* ================================================================================= */
	// Function definitions
	/* ================================================================================= */
public:

	/* Thread create.
	---------------------------------------------*/
	TrdRetEx::Status			CreateThread(LPVOID lpvParam, _stTrdInfo *pstTrdInfo, LPCTSTR lpszThreadName, pThreadAddress pCreateFunction);

	/* Thread execute.
	---------------------------------------------*/
	TrdRetEx::Status			CreateThreadExecute(_stTrdInfo *pstTrdInfo);

	/* Thread Suspend.
	---------------------------------------------*/
	TrdRetEx::Status			CreateThreadSuspend(_stTrdInfo *pstTrdInfo);

	/* Thread stop.
	---------------------------------------------*/
	TrdRetEx::Status			CreateThreadStop(_stTrdInfo *pstTrdInfo);
		
	/* Thread terminate.
	---------------------------------------------*/
	TrdRetEx::Status			CreateThreadTerminate(_stTrdInfo *pstTrdInfo);

	/* Thread make state check.
	---------------------------------------------*/
	BOOL						Check_CreateThread(_stTrdInfo *pstTrdInfo);

	/* Thread state check - stop.
	---------------------------------------------*/
	BOOL						Check_CreateThreadExiting(_stTrdInfo *pstTrdInfo);

	/* Thread state check - Exit.
	---------------------------------------------*/
	BOOL						Check_CreateThreadExit(_stTrdInfo *pstTrdInfo);

	/* Thread state check.
	---------------------------------------------*/
	TrdRetEx::Status			Check_CreateThreadState(_stTrdInfo *pstTrdInfo);

	/* PeekMessage Wait Time - Sleep.
	--------------------------------------------- */
	DWORD						WaitTime(DWORD dwMsec);

protected:
private:
	
	/* LIB initialization.
	--------------------------------------------- */
	BOOL						InitInstance();
	
	/* LIB destroy.
	--------------------------------------------- */
	void						Destroy();
	
	
	/* ================================================================================= */
	// Event Message definitions
	/* ================================================================================= */
};

#endif // !defined(AFX_THREADMANAGEREX_H__F8AE630C_3568_4BB6_98AF_D30897AB4011__INCLUDED_)
