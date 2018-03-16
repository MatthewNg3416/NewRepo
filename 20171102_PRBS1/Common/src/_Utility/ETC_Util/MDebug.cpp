#include "stdafx.h"
#include "MDebug.h"
#include "UtilManager.h"
#include <new.h>
//#include <strsafe.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MStackTrace MDebug::m_CallStack;
MDebug* MDebug::m_pDebug							= NULL;
TCHAR	MDebug::m_strDumpPath[MAX_PATH]				= {0,};
BOOL	MDebug::m_bInitalizeDump					= FALSE;
BOOL	MDebug::m_bUseCreateDump					= FALSE;

MDebug::MDebug()
{
	Initalize();
}

void MDebug::Initalize()
{
#ifndef WriteDebugLog
	InitializeCriticalSection(&m_csError);
	InitializeCriticalSection(&m_csDebugLog);
	
#endif
}


//void MDebug::SetNowStackTrace() { m_CallStack.ReadCallStack();  }
MStackTrace MDebug::GetNowStackTrace()
{
	MStackTrace StackTrace;
	StackTrace.ReadCallStack();
	return StackTrace;
}

#ifndef WriteDebugLog
BOOL MDebug::WriteDebugLog(CString strMsg)
{
	if(m_pDebug == NULL)
		m_pDebug = new MDebug();
	return MDebug::WriteDebugLog(_T("DebugLog"), strMsg);
}
BOOL MDebug::WriteDebugLog(CString strFileName, CString strMsg)
{
	if(m_pDebug == NULL)
		m_pDebug = new MDebug();

	::EnterCriticalSection(&m_pDebug->m_csDebugLog);

	FILE* fsource; 
	CFileFind fn;
	CString fname;

	CTime tmCurr = CTime::GetCurrentTime();
	strMsg = tmCurr.Format(_T("[%m/%d-%H:%M:%S]")) + strMsg;
	fname = _T("..\\Log\\") + strFileName + tmCurr.Format(_T("%m%d")) + _T(".txt");
	
	//LKH_20090130 : Log Unicode Writing
	if ((fsource = _wfopen(fname, _T("ab+,ccs=UNICODE"))) == NULL)
	{
		::LeaveCriticalSection(&m_pDebug->m_csDebugLog);
		return FALSE;
	}
	//���� �߻� �α� ���� ���
	fwprintf(fsource, _T("%s\r\n"), strMsg.GetBuffer());
	fclose(fsource);

	::LeaveCriticalSection(&m_pDebug->m_csDebugLog);

	return TRUE;
}
#endif


//Dump Section
BOOL MDebug::RegisterUserDump(IN LPCTSTR lpszDumpPath/*=NULL*/)
{
	m_bInitalizeDump = FALSE;
	DWORD	dwRtnValue	= 0;
	
	if (NULL != lpszDumpPath)
		_tcscpy(m_strDumpPath, lpszDumpPath);
	else
	{	
		dwRtnValue = MDebug::GetDefaultDumpPath(m_strDumpPath, MAX_PATH);
		if (ERROR_SUCCESS != dwRtnValue)
			return FALSE;
	}
	//�⺻ ����ŷ� userdump.exe�� ����Ѵ�. (ũ�� �ʿ� ������ �ϴ�.. �ش� ��ɻ���ϸ� Visual Studio Attach ��� ��� �Ұ�)
	//MString strCommandMessage = MString::MFormat(_T("userdump -i -d %s"), m_strDumpPath);
	//system(strCommandMessage.GetChar());
	//_____________________________

	::SetUnhandledExceptionFilter(MDebug::UnhandleExceptionFilter); //����,, �̰� App ���� �����ÿ� �ϸ� �ʱ�ȭ��.. 
	::_set_new_handler(MemoryCreateError);
	::_CrtSetReportHook(WhenReportMessageHook);
	m_bInitalizeDump = TRUE;

	return TRUE;
}
BOOL MDebug::CreateUserDump(IN LPCTSTR lpszDumpPath/*=NULL*/, IN LPCTSTR lpszDumpName/*=NULL*/, IN LPCTSTR lpszProcessID/*=NULL*/)
{
	DWORD	dwRtnValue				= 0;
	TCHAR	strDumpPath[MAX_PATH]	= {0, };
	TCHAR	strDumpName[MAX_PATH]	= {0, };
	TCHAR	strProcessID[MAX_PATH]	= {0, };

	if (NULL != lpszDumpPath)
		_tcscpy(strDumpPath, lpszDumpPath);
	else
	{	
		ASSERT(m_bInitalizeDump == TRUE);
		_tcscpy(strDumpPath, m_strDumpPath);
	}

	if (NULL != lpszDumpName)
		_tcscpy(strDumpName, lpszDumpName);
	else
		_tcscpy(strDumpName, DEF_PROGRAM_NAME);

	if (NULL != lpszProcessID)
		_tcscpy(strProcessID, lpszProcessID);
	else
		_tcscpy(strProcessID, DEF_PROGRAM_NAME);


	SYSTEMTIME	stTime = {0,}; ::GetLocalTime(&stTime);

	
	//���ǻ���!!! ���ϰ�ο� ������ �� ���� ��� �۵� �ȵ�!!!! UserDump ���α׷� �̸�, Dump���� �̸�
	CString strCommandMessage = MString::MFormat(_T("%s %s%s_%.4d%.2d%.2d_%.2d%.2d%.2d%.3d.dmp"), 
		strProcessID, strDumpPath, strDumpName, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
	::ShellExecute(NULL, _T("open"), DEF_USERDUMP_PATH, strCommandMessage, _T(","), SW_SHOW); // ���α׷� ����
/*
	CString strCommandMessage = MString::MFormat(_T("userdump %s %s%s_%.4d%.2d%.2d_%.2d%.2d%.2d%.3d.dmp"), 
		strProcessID, strDumpPath, strDumpName, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
	system(MString::ConvertWideCharToChar(strCommandMessage.GetBuffer(0)));
*/	

	m_bUseCreateDump = TRUE; 
	return TRUE;
}
DWORD MDebug::GetDefaultDumpPath(OUT LPTSTR lpszPath, IN DWORD dwCchPath)
{
	if (NULL == lpszPath)
		return ERROR_INVALID_PARAMETER;

	// Clean Up
	ZeroMemory(lpszPath, sizeof(TCHAR) * dwCchPath);

	if (0 == ::GetModuleFileName(NULL, lpszPath, dwCchPath))
		return ::GetLastError(); // Module Handle ���ϴµ� ����
	//170724 JSH.s
	LPTSTR lpszFound;
	for(int i = 0; i<3; i++)
	{
		lpszFound = _tcsrchr(lpszPath, TEXT('\\'));
		if (NULL == lpszFound) // Path ���� \�� ����?
			return ERROR_PATH_NOT_FOUND;
		
		if (lpszFound <= lpszPath)
			return ERROR_PATH_NOT_FOUND;
		
		*lpszFound = TEXT('\0');
	}
	//170724 JSH.e
	// ��� ����, 
	SYSTEMTIME	stTime = {0,};	::GetLocalTime(&stTime);
	_stprintf(lpszPath, _T("%s\\Dump"), lpszPath);
	CreateDirectory(lpszPath, NULL);
	_stprintf(lpszPath, _T("%s\\%.4d%.2d%.2d_%.2d%.2d%.2d\\"), lpszPath, stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond); //170724 JSH
	CreateDirectory(lpszPath, NULL);
	
	_tcscpy(m_strDumpPath, lpszPath);
	return ERROR_SUCCESS;
}
BOOL MDebug::GetCreateDumpStatus(void)
{
	return m_bUseCreateDump;
}
DWORD MDebug::RemoveDumpPath(void)
{
	ASSERT(m_bInitalizeDump == TRUE);
	return ::RemoveDirectory(m_strDumpPath);
}


LONG WINAPI MDebug::UnhandleExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo)
{
	//���ǻ���!!! ���ϰ�ο� ������ �� ���� ��� �۵� �ȵ�!!!! UserDump ���α׷� �̸�, Dump���� �̸�
	CString strFileName = MString::MFormat(_T("%s_Exception"), DEF_PROGRAM_NAME);	
#ifdef _Win64
	CString strProcessID = MString::MFormat(_T("%u*%I64u"), GetCurrentThreadId(), ExceptionInfo);
#else
	CString strProcessID = MString::MFormat(_T("%u*%u"), GetCurrentThreadId(), ExceptionInfo);
#endif
	CreateUserDump(NULL, strFileName, strProcessID); 
	return EXCEPTION_CONTINUE_SEARCH;
}

int MDebug::MemoryCreateError(unsigned int iValue)
{
	CString strCommandMessage = MString::MFormat(_T("%s %s%s_MemoryException.dmp"), DEF_PROGRAM_NAME, m_strDumpPath, DEF_PROGRAM_NAME);
	::ShellExecute(NULL, _T("open"), DEF_USERDUMP_PATH, strCommandMessage, _T(","), SW_SHOW); // ���α׷� ����
	return 0;
}

int MDebug::WhenReportMessageHook( int iReportType, char *strMessage, int *iReturnValue )
{
	int iReturnCode = FALSE;
	if(iReportType != _CRT_WARN)
	{
		CString strFileName = MString::MFormat(_T("%s_ReportHook"), DEF_PROGRAM_NAME);	
		CreateUserDump(NULL, strFileName);

		//20171007 KDW Start //�޽��� �� ���� �ٷ� ������ϵ��� ����
		//MUtilManager::MyMessageBox(_T("���� ���α׷��� �ߴ��� ������ �߻��Ͽ����ϴ�.\n This Program has critical problem."), _T("Error"), M_ICONERROR);
		
		CString strReportType = (iReportType == _CRT_ERROR ? _T("_CRT_ERROR"): _T("_CRT_ASSERT"));
		CString strErrorMessage = strMessage;
		CString strInformation = MString::MFormat(_T("Error : %s\n\rReportType = %s\n\r"), strErrorMessage, strReportType);

		//MUtilManager::MyMessageBox(strInformation, _T("Error"), M_ICONERROR);

		//if (IDYES == MUtilManager::MyMessageBox(_T("��� �����Ϸ��� Yes��, ����׸� �Ͻ÷��� No�� �����ּ���\r\nIf you want to continue, Press Yes but If you want to debug, Press No "), _T("Confirm"), M_ICONERROR|M_YESNO))
		//	iReturnCode = TRUE;
		//20171007 KDW End
	}
	return iReturnCode;
}


/*
void InvalidParameterHandler(PCTSTR expression, PCTSTR?function, PCTSTR file, unsigned int line, uintptr_t pReserved)
{
#ifdef _DEBUG // ����׸�忡���� ���� Ȯ��
	wprintf(L"ca1BCASMFCLIbTk.dll Invalid parameter handler invoked. file = %s\n", file);
	wprintf(L"ca1BCASMFCLIbTk.dll Invalid parameter handler invoked. line = %u\n", line);
	wprintf(L"ca1BCASMFCLIbTk.dll Invalid parameter handler invoked. function = %s\n", function);
	wprintf(L"ca1BCASMFCLIbTk.dll Invalid parameter handler invoked. expression = %s\n", expression);
#endif
}*/