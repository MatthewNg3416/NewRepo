#ifndef _COMMON_HEADER
#define _COMMON_HEADER

#include "MyProgressWnd.h"
#include "MDebug.h"
class CBtnEnh;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*-------------------------------- For Button Color ----------------------------------------------*//
const long DEF_OFF_COLOR = 0x008A8A8A;
const long DEF_ON_COLOR = 0x0095CAFF;
const COLORREF DEF_BTN_ON_COLOR	= RGB(255, 153, 51);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*-------------------------------- For Math. Tool ------------------------------------------------*//
#ifndef RAD2DEG
#define	RAD2DEG(x)		((x) * 180.0 / 3.1415926535)
#endif

#ifndef DEG2RAD
#define	DEG2RAD(x)		((x) * 3.1415926535 / 180.0)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*------------------------------- For MyMessageBox -----------------------------------------------*//
#define  M_ICONINFORMATION    0x01 
#define  M_ICONQUESTION       0x02 
#define  M_ICONSTOP           0x04 
#define  M_ICONERROR          0x08 	

#define  M_OK                 0x10
#define  M_OKCANCEL           0x20
#define  M_YESNO              0x40
#define  M_ABORTRETRYIGNORE   0x80
#define  M_RETRYCANCEL        0x100

BOOL	GetNumberBox(WCHAR* szNum, int maxchar = 10, WCHAR* getnum = _T(""), WCHAR* title = _T("Insert Number Only."),	BOOL pwd = FALSE);
BOOL	GetValueBox(WCHAR* szVal, int maxchar = 10, WCHAR* getval = _T(""), WCHAR* title = _T("Insert Character"), BOOL pwd = FALSE, BOOL btndisable = FALSE);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*------------------------------- For Common Function --------------------------------------------*//
//void	SetStatusText(LPCTSTR sText);

DWORD	HexToInt(char* s);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*----------------------------- For Multi Language Support ---------------------------------------*//

#define DISPLAY_READ_ERROR(sect, itm, ret) do { if (FALSE == DisplayDataReadError(sect, itm, m_strFileName)) return FALSE; ret = FALSE; } while (0)
#define DISPLAY_WRITE_ERROR(sect, itm, ret) do { if (FALSE == DisplayDataWriteError(sect, itm, m_strFileName)) return FALSE; ret = FALSE; } while (0)

BOOL DisplayDataReadError(CString strSection, CString strItem, CString strFileName);
BOOL DisplayDataWriteError(CString strSection, CString strItem, CString strFileName);

// ML_hongju 추가.. 다국어 지원을 위한 코드 삽입
//________________________________________________________________________________________

// 전체 Section name
// 1. Main View
// 2. Manual View
// 3. Data View
// 4. Teach View
// 5. Sub Dlg Box
// 6. 그외 
// 7. Message Box로 뜨는 모든것..
//________________________________________________________________________________________

// Section의 내용.
#define MAIN_AUTO				_T("MAIN_AUTO")
#define MAIN_MANUAL				_T("MAIN_MANUAL")
#define MAIN_DATA				_T("MAIN_DATA")
#define MAIN_TEACH				_T("MAIN_TEACH")
#define SUB_DLG					_T("SUB_DLG")
//LKH_20081111 : ETC를 SUB_DLG로 통합
//#define ETC						_T("ETC")

// MyMessageBox
#define MY_MESSAGE_BOX			_T("MY_MESSAGE_BOX")

//________________________________________________________________________________________


// Display Language 파일 경로.
//________________________________________________________________________________________
#define DISPLAY_KOREAN			_T("./Info/Language_Korean.dat")
#define DISPLAY_ENGLISH			_T("./Info/Language_English.dat")
#define DISPLAY_CHINESE			_T("./Info/Language_Chinese.dat")
#define DISPLAY_SLOVAK			_T("./Info/Language_Slovak.dat")
//________________________________________________________________________________________

// Error Message Language 파일 경로.
//________________________________________________________________________________________
#define ERRORMESSAGE_KOREAN		_T("./Info/ErrorMessage_Korean.dat")
#define ERRORMESSAGE_ENGLISH	_T("./Info/ErrorMessage_English.dat")
#define ERRORMESSAGE_CHINESE	_T("./Info/ErrorMessage_Chinese.dat")
#define ERRORMESSAGE_SLOVAK		_T("./Info/ErrorMessage_Slovak.dat")
//________________________________________________________________________________________

const UINT LANGUAGE_SELECTED	= -1;
const UINT LANGUAGE_KOREAN		= 0;
const UINT LANGUAGE_ENGLISH		= 1;
const UINT LANGUAGE_CHINESE		= 2;
const UINT LANGUAGE_SLOVAK		= 3;

// Title Display
void SetTitleLT(CBtnEnh* pButton, CString strSection, CString strKey);
void SetTitleCB(CBtnEnh* pButton, CString strSection, CString strKey);

// Main View 관련 Display
int ReadDisplayMessage(CString strSection, CString strKey, CString* pstrValue);    

// Normal Pop-Up Display
UINT MyMessageBox(CString strSection, UINT iKey, CString strTitle=_T("Error"), UINT type=M_OK, WCHAR* YesBut=_T("Yes"), WCHAR* NoBut=_T("No"));
// Data의 읽음 없이 Section, Title 표시
UINT MyMessageBox(CString strSection, CString strTitle, UINT type=M_OK, WCHAR* YesBut=_T("Yes"), WCHAR* NoBut=_T("No"));

// %s, %d 등 기존꺼 그냥 Display 
UINT MyMessageBox(char* message, WCHAR* title, UINT type=M_OK, WCHAR* YesBut=_T("Yes"), WCHAR* NoBut=_T("No"));

CString ReadErrorMessage(int iErrorCode, UINT uiLanguageType = LANGUAGE_SELECTED);

// Display 파일 Path
CString getFilePath();

// Error Message 파일 Path
CString getErrorFilePath();

int SetErrorLevel(CString Error,int iErrorLevel, char* szFileName, int iLine);

CString GetErrorLevel();
int ResetErrorLevel();

//LKH_090210 : SetErrorLevel 함수 Autorun 때만 사용하게 함
void SetAutoRunLevel();
void SetManualLevel();


#endif	// _COMMON_HEADER
