#include "stdafx.h"
#include "MFileManager.h"
#include "MString.h"
#include <xstring>
#include "MLog.h"
#include "CRTDBG.h"
#include "windows.h"
#include "WinBase.h"
#include <DbgHelp.h>
#include "MStackTrace.h"
#pragma comment(lib, "dbghelp.lib")
#pragma once


#define XOFFSET _T("        ")
#define XARROW _T("     ->")

const int DEF_MAX_ERROR = 5;
const int DEF_ERROR_DEPTH = 4;
#define DEF_PROGRAM_NAME _T("MPlatformOlb.exe")
#define DEF_USERDUMP_PATH _T("..\\Common\\LIB\\userdump\\x86\\userdump.exe")

//WriteDebugLog Define�� PCB������ ����մϴ�. 
//PCB�� OLB���� DebugLog ����� ����� �޶�, PCB�� ��� Log ���� �Լ��� ����Ͻ� �� �����ϴ�.
class MDebug
{
	
#ifndef WriteDebugLog 
private :
	CRITICAL_SECTION m_csError;
	CRITICAL_SECTION m_csDebugLog;
public :
	MDebug();
//	static MDebug* GetInstance
	static BOOL WriteDebugLog(CString strMsg);
	static BOOL WriteDebugLog(CString strFileName, CString strMsg);
#endif
	//	CaptureStackBackTrace
	
private :
	void Initalize();

	static MStackTrace m_CallStack;

	static TCHAR	m_strDumpPath[MAX_PATH];
	static BOOL		m_bInitalizeDump;
	static BOOL		m_bUseCreateDump;
	static MDebug*	m_pDebug;

	static DWORD GetDefaultDumpPath(OUT LPTSTR lpszPath, IN DWORD dwCchPath);
	//Exception ��Ȳ [��) ASSERT, NULL ������ �� ���� ��]�� ����ǰ� �Ǵ� �Լ�
	static LONG WINAPI UnhandleExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo);
	//�޸� ���� ���н� ����ǰ� �Ǵ� �Լ�
	static int MemoryCreateError(unsigned int iValue);
	//Debug Report â�� ������ ���� ����Ǵ� �Լ�
	static int WhenReportMessageHook( int reportType, char *message, int *returnValue );

public :
	//	* lpszDumpPath : ���������� ������ Path.
	//		ex) �⺻���� ���, C:\ttt\abc.exe�� C:\ttt\Dump\(time)\abc.dmp�� ����ȴ�)
	static BOOL RegisterUserDump(IN LPCTSTR lpszDumpPath = NULL);
	//Dump������ �����Ѵ�. [lpszDumpPath : Dump���� ���, lpszDumpName : Dump���� �̸�, lpszProcessID : Dump������ ���� Process ID(Ȥ�� �̸�)]
	static BOOL CreateUserDump(IN LPCTSTR lpszDumpPath = NULL, IN LPCTSTR lpszDumpName = NULL, IN LPCTSTR lpszProcessID = NULL);
	//Dump�� �ѹ��̶� �����ߴ��� Ȯ���ϴ� �Լ�
	static BOOL GetCreateDumpStatus();
	//Dump ���� ��� ���� �Լ�
	static DWORD RemoveDumpPath();


	//static void SetNowStackTrace();
	static MStackTrace GetNowStackTrace();
};





