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

//WriteDebugLog Define은 PCB에서만 사용합니다. 
//PCB와 OLB간의 DebugLog 남기는 방식이 달라서, PCB의 경우 Log 관련 함수를 사용하실 수 없습니다.
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
	//Exception 상황 [예) ASSERT, NULL 포인터 값 대입 등]에 실행되게 되는 함수
	static LONG WINAPI UnhandleExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo);
	//메모리 생성 실패시 실행되게 되는 함수
	static int MemoryCreateError(unsigned int iValue);
	//Debug Report 창이 나오기 전에 실행되는 함수
	static int WhenReportMessageHook( int reportType, char *message, int *returnValue );

public :
	//	* lpszDumpPath : 덤프파일을 생성할 Path.
	//		ex) 기본값인 경우, C:\ttt\abc.exe는 C:\ttt\Dump\(time)\abc.dmp로 저장된다)
	static BOOL RegisterUserDump(IN LPCTSTR lpszDumpPath = NULL);
	//Dump파일을 생성한다. [lpszDumpPath : Dump파일 경로, lpszDumpName : Dump파일 이름, lpszProcessID : Dump파일을 만들 Process ID(혹은 이름)]
	static BOOL CreateUserDump(IN LPCTSTR lpszDumpPath = NULL, IN LPCTSTR lpszDumpName = NULL, IN LPCTSTR lpszProcessID = NULL);
	//Dump를 한번이라도 생성했는지 확인하는 함수
	static BOOL GetCreateDumpStatus();
	//Dump 파일 경로 삭제 함수
	static DWORD RemoveDumpPath();


	//static void SetNowStackTrace();
	static MStackTrace GetNowStackTrace();
};





