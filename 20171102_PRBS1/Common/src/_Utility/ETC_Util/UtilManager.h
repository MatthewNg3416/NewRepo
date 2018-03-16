#include "stdafx.h"
#include "MFileManager.h"
#include "MString.h"
#include "MyProgressWnd.h"
#include "CommonMsgDlg.h"
#include "textdescriptor.h"
#include "LanguageManager.h"
#include "windows.h"
#include "winbase.h"
#include "tlhelp32.h"
#ifdef UNICODE
#include "CommonGetNumDlg.h"
#include "CommonGetValueDlg.h"
#else
#include "GetNumDlg.h"
#include "GetValueDlg.h"
#endif
#include <vector>
#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*-------------------------------- For Math. Tool ------------------------------------------------*//
#define	RAD2DEG(x)		((x) * 180.0 / 3.1415926535)
#define	DEG2RAD(x)		((x) * 3.1415926535 / 180.0)
#define rad2deg(x)		RAD2DEG(x)
#define deg2rad(x)		DEG2RAD(x)

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*-------------------------------- For Button Color ----------------------------------------------*//
const long DEF_OFF_COLOR = 0x008A8A8A;
const long DEF_ON_COLOR = 0x0095CAFF;
const long DEF_OFF_COLOR1	= 0x00FF4444;
const long DEF_ON_COLOR1	= 0x000000FF;
const long DEF_OFF_COLOR2	= 0x00FF7777;
const long DEF_ON_COLOR2	= 0x003333FF;

const COLORREF DEF_BTN_ON_COLOR	= RGB(255, 153, 51);


//////////////////////////////////////////////////////////////////////////////////////////////////////
//*------------------------------- For MyMessageBox -----------------------------------------------*//
// Section의 내용.
const CString MAIN_AUTO			= _T("MAIN_AUTO");
const CString MAIN_MANUAL		= _T("MAIN_MANUAL");
const CString MAIN_DATA			= _T("MAIN_DATA");
const CString MAIN_TEACH		= _T("MAIN_TEACH");
const CString SUB_DLG			= _T("SUB_DLG");

const CString MY_MESSAGE_BOX	= _T("MY_MESSAGE_BOX");

const int M_ICONINFORMATION   = 0x01;
const int M_ICONQUESTION      = 0x02; 
const int M_ICONSTOP          = 0x04;
const int M_ICONERROR         = 0x08;	

const int M_OK                = 0x10;
const int M_OKCANCEL          = 0x20;
const int M_YESNO             = 0x40;
const int M_ABORTRETRYIGNORE  = 0x80;
const int M_RETRYCANCEL       = 0x100;
const int M_TRAYSELECT        = 0x110;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MUtilManager
{		
private :
	static BOOL CALLBACK EnumWindowCallBack(HWND hwnd, LPARAM lParam);
	static HWND GetRunningProcessHWND(CString strProcess);

public :
	static BOOL GetNumberBox(TCHAR* szNum, int maxchar = 10, TCHAR* getnum = _T(""), TCHAR* title = _T("Insert Number Only."),	BOOL pwd = FALSE);
	static BOOL GetValueBox(TCHAR* szVal, int maxchar = 10, TCHAR* getval = _T(""), TCHAR* title = _T("Insert Character"), BOOL pwd = FALSE, BOOL btndisable = FALSE);

	// Normal	Pop-Up Display
	static UINT MyMessageBox(CString strSection, UINT iKey, CString strTitle=_T("Error"), UINT type=M_OK, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"));
	// Data의 읽음 없이 Section, Title 표시
	static UINT MyMessageBox(CString strSection, CString strTitle, UINT type=M_OK, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"));
	// %s, %d 등 기존꺼 그냥 Display 
	static UINT MyMessageBox(TCHAR* message, TCHAR* title, UINT type=M_OK, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"), TCHAR* UserBut = _T("Cancel"));
	// PCB 호환성 유지 함수
	static UINT MyMessageBox(UINT type, TCHAR* title, TCHAR* YesBut, TCHAR* NoBut, TCHAR* message);


	static BOOL DisplayDataReadError(CString strSection, CString strItem, CString strFileName);
	static BOOL DisplayDataWriteError(CString strSection, CString strItem, CString strFileName);

	static void SetTitleLT(CBtnEnh* pButton, CString strSection, CString strKey);
	static void SetTitleCB(CBtnEnh* pButton, CString strSection, CString strKey);

	//Math
	static DWORD HexToInt(char* s);
	static void ChartoDec(char* tmp, int &rval);
	static void DectoBin(int tmp,char* rval);

	static const CString strKeyboardFileName;
	static const CString strKeyboardWindowName;
	static const CString strSystem32FilePath;

	static void DisplayWindowsKeyboard();
	//170731_KDH PPT 파일 띄우는 용도 
	static const CString strInfoFilePath;
	static const CString strInfoPanelFileName;

	static void DisplayInfoPanel();
	//_________
};
