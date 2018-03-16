#ifndef _COMMON_HEADER
#define _COMMON_HEADER

#include "UtilManager.h"
#include "LanguageManager.h"
#include "MString.h"
#include "MDebug.h"
#include "MyProgressWnd.h"
#include "MLog.h"

class CBtnEnh;

//호환성 유지 함수 및 변수 
const int LANGUAGE_SELECTED = eLANGUAGE_SELECTED;
const int LANGUAGE_KOREAN	= eLANGUAGE_KOREAN;
const int LANGUAGE_ENGLISH	= eLANGUAGE_ENGLISH; 
const int LANGUAGE_CHINESE	= eLANGUAGE_CHINESE; 
const int LANGUAGE_SLOVAK	= eLANGUAGE_SLOVAK;
const int LANGUAGE_MAX_COUNT= eLANGUAGE_MAX_COUNT;


inline BOOL GetNumberBox(TCHAR* szNum, int maxchar = 10, TCHAR* getnum = _T(""), TCHAR* title = _T("Insert Number Only."),	BOOL pwd = FALSE) { return MUtilManager::GetNumberBox(szNum, maxchar, getnum, title, pwd); }
inline BOOL GetValueBox(TCHAR* szVal, int maxchar = 10, TCHAR* getval = _T(""), TCHAR* title = _T("Insert Character"), BOOL pwd = FALSE, BOOL btndisable = FALSE) { return MUtilManager::GetValueBox(szVal, maxchar, getval, title, pwd, btndisable); }

// Normal	Pop-Up Display
inline UINT MyMessageBox(CString strSection, UINT iKey, CString strTitle=_T("Error"), UINT type=M_OK, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"))  { return MUtilManager::MyMessageBox(strSection, iKey, strTitle, type, YesBut, NoBut); }
// Data의 읽음 없이 Section, Title 표시
inline UINT MyMessageBox(CString strSection, CString strTitle, UINT type=M_OK, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No")) { return MUtilManager::MyMessageBox(strSection, strTitle, type, YesBut, NoBut); }
// %s, %d 등 기존꺼 그냥 Display 
inline UINT MyMessageBox(TCHAR* message, TCHAR* title, UINT type=M_OK, TCHAR* YesBut=_T("Yes"), TCHAR* NoBut=_T("No"), TCHAR* UserBut = _T("Cancel")) { return MUtilManager::MyMessageBox(message, title, type, YesBut, NoBut, UserBut); }
// PCB 호환성 유지 함수
inline UINT MyMessageBox(UINT type, TCHAR* title, TCHAR* YesBut, TCHAR* NoBut, TCHAR* message) { return MUtilManager::MyMessageBox(type, title, YesBut, NoBut, message); }
//test중입니다.
//////////////////////////////////////////////////////////////////////////////////////////////////////
//*------------------------------- For Common Function --------------------------------------------*//
//void	SetStatusText(LPCTSTR sText);

// Title Display
inline DWORD HexToInt(char* s) { return MUtilManager::HexToInt(s); }
inline void ChartoDec(char* tmp, int &rval) { MUtilManager::ChartoDec(tmp, rval); }
inline void DectoBin(int tmp,char* rval) { MUtilManager::DectoBin(tmp, rval); }
inline void SetTitleLT(CBtnEnh* pButton, CString strSection, CString strKey) { MUtilManager::SetTitleLT(pButton, strSection, strKey); }
inline void SetTitleCB(CBtnEnh* pButton, CString strSection, CString strKey) { MUtilManager::SetTitleCB(pButton, strSection, strKey); }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//*----------------------------- For Multi Language Support ---------------------------------------*//

// Doolin kwanguilshin 2017. 08. 29.
// ret : FALSE -> TRUE Change.
//
#define DISPLAY_READ_ERROR(sect, itm, ret) do { if (FALSE == DisplayDataReadError(sect, itm, m_strFileName))	{ ret = FALSE; return FALSE; } ret = TRUE; } while(0)
#define DISPLAY_WRITE_ERROR(sect, itm, ret) do { if (FALSE == DisplayDataWriteError(sect, itm, m_strFileName))	{ ret = FALSE; return FALSE; } ret = TRUE; } while(0)
//

inline BOOL DisplayDataReadError(CString strSection, CString strItem, CString strFileName) { return MUtilManager::DisplayDataReadError(strSection, strItem, strFileName); }
inline BOOL DisplayDataWriteError(CString strSection, CString strItem, CString strFileName) { return MUtilManager::DisplayDataWriteError(strSection, strItem, strFileName); }

inline int ReadDisplayMessage(CString strSection, CString strKey, CString* pstrValue) { return MLanguageManager::ReadDisplayMessage(strSection, strKey, pstrValue); }
inline int WriteDisplayMessage(CString strSection, CString strKey, CString strValue) { return MLanguageManager::WriteDisplayMessage(strSection, strKey, strValue); }

inline int ReadErrorMessage(CString strSection, CString strKey, CString* pstrValue) { return MLanguageManager::ReadErrorMessage(strSection, strKey, pstrValue); }
//inline CString ReadErrorMessage(int iErrorCode, int iLanguageType) { CString strTemp = MLanguageManager::ReadErrorMessage((int)iErrorCode, (eLanguage_List)iLanguageType); return strTemp; } //170930 JSH
inline CString ReadErrorMessage(int iErrorCode, int iLanguageType, BOOL bErrorReport = FALSE) { CString strTemp = MLanguageManager::ReadErrorMessage((int)iErrorCode, (eLanguage_List)iLanguageType, bErrorReport); return strTemp; }
inline int WriteErrorMessage(CString strSection, CString strKey, CString strValue) { return MLanguageManager::WriteErrorMessage(strSection, strKey, strValue); }


//strIDD : 다이얼로그 ID 값 
#define CHANGE_LANGUAGE_FUNCTION(strIDD) MLanguageManager::ChangeDialogLanguage(this, strIDD)


#endif	// _COMMON_HEADER
