#include "stdafx.h"
#include <math.h>

#include "Common.h"
#include "CommonMsgDlg.h"
#include "CommonGetNumDlg.h"
#include "CommonGetValueDlg.h"
#include "textdescriptor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

unsigned int g_uiLanguageSelect = 0;

////////////////////////////////////////////////
// SetErrorLevel 관련 변수

#define XOFFSET _T("        ")
#define XARROW _T("     ->")

const int DEF_MAX_ERROR = 5;
const int DEF_ERROR_DEPTH = 4;

int g_iErrorNum[DEF_ERROR_DEPTH] = { 0, 0, 0, 0 };
CString g_Error1[DEF_ERROR_DEPTH][DEF_MAX_ERROR];
CString g_Error2[DEF_ERROR_DEPTH][DEF_MAX_ERROR];
//LKH_090210 : 종료시에는 SetErrorLevel 관련 함수 동작 안 함.(AutoMode 가 아닐때는 동작 안 함)
bool g_bCheckAutoMode=FALSE;

CRITICAL_SECTION g_csError;

#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef DEF_PLATFORM_ARP_SYSTEM
extern MPlatformOlbSystem	MOlbSystemACF;
#elif defined (DEF_PLATFORM_PREBONDER_SYSTEM)
extern MPlatformOlbSystem	MOlbSystemPre;
#elif defined (DEF_PLATFORM_FINALBONDER_SYSTEM)
extern MPlatformOlbSystem	MOlbSystemFinal;
#endif

////////////////////////////////////////////////

//UINT MyMessageBox(char* message, char* title, UINT type, char* YesBut/*="예"*/, char* NoBut/*="아니오"*/)
//UINT MyMessageBox(WCHAR* message, WCHAR* title, UINT type=M_OK, WCHAR* YesBut=_T("Yes"), WCHAR* NoBut=_T("No"));
/*{
	CCommonMsgDlg dlg;
	
	if (type & M_ICONINFORMATION) dlg.m_icontype = M_ICONINFORMATION;
	else if (type & M_ICONQUESTION) dlg.m_icontype = M_ICONQUESTION;
	else if (type & M_ICONSTOP) dlg.m_icontype = M_ICONSTOP;
	else if (type & M_ICONERROR) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;
	
	if (type & M_OK) dlg.m_type = M_OK;
	else if (type & M_OKCANCEL) dlg.m_type = M_OKCANCEL;
	else if (type & M_YESNO) dlg.m_type = M_YESNO;
	else if (type & M_ABORTRETRYIGNORE) dlg.m_type = M_ABORTRETRYIGNORE;
	else if (type & M_RETRYCANCEL) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;
	
	strcpy(dlg.m_message, message);
	strcpy(dlg.m_title, title);
	
	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);

   return (dlg.DoModal());
}

//UINT MyMessageBox(UINT type, char* title,  char* YesBut, char* NoBut, char* message ...)
UINT MyMessageBox(UINT type, char* title,  char* YesBut, char* NoBut, WCHAR* message ...)
{
	CCommonMsgDlg dlg;

	if (type & M_ICONINFORMATION) dlg.m_icontype = M_ICONINFORMATION;
	else if (type & M_ICONQUESTION) dlg.m_icontype = M_ICONQUESTION;
	else if (type & M_ICONSTOP) dlg.m_icontype = M_ICONSTOP;
	else if (type & M_ICONERROR) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;

	if (type & M_OK) dlg.m_type = M_OK;
	else if (type & M_OKCANCEL) dlg.m_type = M_OKCANCEL;
	else if (type & M_YESNO) dlg.m_type = M_YESNO;
	else if (type & M_ABORTRETRYIGNORE) dlg.m_type = M_ABORTRETRYIGNORE;
	else if (type & M_RETRYCANCEL) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;
	
	va_list args;
//	char	cmessage[_MAX_PATH];
	WCHAR	cmessage[_MAX_PATH];

	va_start(args, message);
	_vstprintf(cmessage, message, args);
	va_end(args);

	strcpy(dlg.m_message, cmessage);
	strcpy(dlg.m_title, title);

	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);

	return (dlg.DoModal());
}
*/
//BOOL GetNumberBox(char* szNum, int maxchar, char* getnum, char* title, BOOL pwd/*=FALSE*/)
BOOL GetNumberBox(WCHAR* szNum, int maxchar, WCHAR* getnum, WCHAR* title, BOOL pwd/*=FALSE*/)
{
   CCommonGetNumDlg dlg(maxchar, getnum, title, NULL, pwd);
   if (dlg.DoModal() == IDOK)
   {
	  lstrcpy(szNum, dlg.m_strGetNum);
	  return TRUE;
   }
   else
	   return FALSE;
}

//BOOL GetValueBox(char* szVal, int maxchar, char* getval, char* title, BOOL pwd/*=FALSE*/, BOOL btndisable/*= FALSE*/)
BOOL GetValueBox(WCHAR* szVal, int maxchar, WCHAR* getval, WCHAR* title, BOOL pwd/*=FALSE*/, BOOL btndisable/*= FALSE*/)
{
   CCommonGetValueDlg dlg(maxchar, getval, title, NULL, pwd, btndisable);
   if (dlg.DoModal() == IDOK)
   {
      lstrcpy(szVal, dlg.m_strGetVal);
      return TRUE;
   }
   else
	   return FALSE;
}

DWORD HexToInt(char* s)
{
	char hexch[] = "0123456789ABCDEF";
	int i,j;
	DWORD r,n,k;
	char ch;
	
	k=1; r=0;
	for (i=strlen(s); i>0; i--) 
	{
		ch = s[i-1];
		if (ch > 0x3f)
			ch &= 0xDF;
		n = 0;
		for (j = 0; j<16; j++)
		{
			if (ch == hexch[j])
				n = j;
		}
		r += (n*k);
		k *= 16;
	}
	return r;
}

/*
void SetStatusText(LPCTSTR sText)
{
	CStatusBar* pStatusBar = NULL;

   CWnd *pMainWnd = AfxGetMainWnd();

   if (!pMainWnd) return;

   pStatusBar = (CStatusBar*)pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
    
   // - OR -
   // if (!pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
   // pStatusBar = (CStatusBar*) ((CFrameWnd*) pFrame)->GetMessageBar();

   if (!pStatusBar || !pStatusBar->IsKindOf(RUNTIME_CLASS(CStatusBar))) return;

   pStatusBar->SetWindowText(sText);
}
*/

/*
CString GetDisplayString(char* szKorean, char* szEnglish)
{
	ASSERT(NULL != szKorean);
	ASSERT(NULL != szEnglish);

	switch (g_uiLanguageSelect)
	{
	default:
	case 0:	// LANGUAGE_KOREAN
		return CString(szKorean);

	case 1:	// LANGUAGE_ENGLISH
		return CString(szEnglish);

	case 2:	// LANGUAGE_KOR_ENG:
		return CString(szKorean) + "\r\n" + CString(szEnglish);
	}
}
*/
/*
void SetTitleLT(CBtnEnh* pButton, WCHAR* szKorean, WCHAR* szEnglish)
{
	CTextDescriptor text;
	text = pButton->GetTextDescrLT();
	switch (g_uiLanguageSelect)
	{
	case 0:	// LANGUAGE_KOREAN
		if (NULL == szKorean)
			return;

		text.SetText(szKorean);
		break;

	default:
	case 1: // LANGUAGE_ENGLISH
		if (NULL == szEnglish)
			return;

		text.SetText(szEnglish);
		break;
	}
}
*/
/*
void SetTitleCB(CBtnEnh* pButton, WCHAR* szKorean, WCHAR* szEnglish)
{
	CTextDescriptor text;
	text = pButton->GetTextDescrCB();
	switch (g_uiLanguageSelect)
	{
	case 0:	// LANGUAGE_KOREAN
		if (NULL == szKorean)
			return;

		text.SetText(szKorean);
		break;

	default:
	case 1:	// LANGUAGE_ENGLISH
		if (NULL == szEnglish)
			return;

		text.SetText(szEnglish);
		break;
	}
}
*/
/*WCHAR* GetSelectedLanguageString(WCHAR* szKorean, WCHAR* szEnglish)
{
	ASSERT(NULL != szKorean);
	ASSERT(NULL != szEnglish);

	switch (g_uiLanguageSelect)
	{
	case 0:	// LANGUAGE_KOREAN
		return szKorean;

	default:
	case 1:	// LANGUAGE_ENGLISH
		return szEnglish;
	}
}
*/
BOOL DisplayDataReadError(CString strSection, CString strItem, CString strFileName)
{
	CString strTemp;
	strTemp.Format(_T("[%s] %s"), strSection, strItem);

	CString strMsg = strTemp + _T(" item was not read. Do you go ahead reading next item?");

	CString strTitle;
	strTitle.Format(_T("%s File Read Error"), strFileName);

	if (IDNO == MyMessageBox(strMsg, strTitle, M_ICONERROR|M_YESNO))
		return FALSE;

	return TRUE;
}

BOOL DisplayDataWriteError(CString strSection, CString strItem, CString strFileName)
{
	CString strTemp;
	strTemp.Format(_T("[%s] %s"), strSection, strItem);

	CString strMsg = strTemp + _T(" item was not written. Do you go ahead writing next item?");

	CString strTitle;
	strTitle.Format(_T("%s File Write Error"), strFileName);

//	if (IDNO == MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), strTitle.GetBuffer(strTitle.GetLength()), M_ICONERROR|M_YESNO))
//		return FALSE;
	if (IDNO == MyMessageBox(strMsg, strTitle, M_ICONERROR|M_YESNO))
		return FALSE;

	return TRUE;
}

// ML_hongju 다국어 지원
//____________________________________________________________________________________________________________________________________

// MyMessage 표시 시 Title은 그냥 영문으로 표시..
UINT MyMessageBox(CString strSection, UINT iKey, CString strTitle, UINT type, WCHAR* YesBut, WCHAR* NoBut)
{
	CString strMonitoringLog;
	CString strUserSelect;

	int iSelect;
	MPlatformOlbSystem* lnkSystem;
	
#ifdef DEF_PLATFORM_ARP_SYSTEM
	lnkSystem = &MOlbSystemACF;
#elif defined (DEF_PLATFORM_PREBONDER_SYSTEM) 
	lnkSystem = &MOlbSystemPre;
#elif defined (DEF_PLATFORM_FINALBONDER_SYSTEM)
	lnkSystem = &MOlbSystemFinal;
#endif

	CCommonMsgDlg dlg;
	
	BOOL	bReturn = TRUE;
	WCHAR	chMessage[500];
	WCHAR	chrKoreanMessage[500];
	WCHAR	chrEnglishMessage[500];
	CString	strTempMessage;

	memset(chMessage, 0, sizeof(chMessage));
	
	// strKey 값은 숫자만 받는다...처리는 이곳에서 한다.
	CString strKey;
	
	strKey.Format(_T("Message_%d"), iKey);
	GetPrivateProfileString(strSection, strKey, _T("CAN NOT READ"), chMessage, 256, getFilePath());

	// ML_hongju 추가 내용... 항상 한글은 같이 띄우는게 좋은거 같음.
	if (LANGUAGE_KOREAN != g_uiLanguageSelect)
	{
		GetPrivateProfileString(strSection, strKey, _T("CAN NOT READ"), chrKoreanMessage, 256, DISPLAY_KOREAN);
		swprintf(chMessage,_T("%s\r\n%s"), chMessage, chrKoreanMessage);
	}
	
	// Line 바꾸는것 적용...
	strTempMessage.Format(_T("%s"), chMessage);
	strTempMessage.Replace(_T("@@"), _T("\n\r"));
	
	if (type & M_ICONINFORMATION) dlg.m_icontype = M_ICONINFORMATION;
	else if (type & M_ICONQUESTION) dlg.m_icontype = M_ICONQUESTION;
	else if (type & M_ICONSTOP) dlg.m_icontype = M_ICONSTOP;
	else if (type & M_ICONERROR) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;
	
	if (type & M_OK) dlg.m_type = M_OK;
	else if (type & M_OKCANCEL) dlg.m_type = M_OKCANCEL;
	else if (type & M_YESNO) dlg.m_type = M_YESNO;
	else if (type & M_ABORTRETRYIGNORE) dlg.m_type = M_ABORTRETRYIGNORE;
	else if (type & M_RETRYCANCEL) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;
	
	wcscpy(dlg.m_message, strTempMessage);
	//	wcscpy(dlg.m_message, chMessage);
	wcscpy(dlg.m_title, strTitle);
	wcscpy(dlg.m_strYes, YesBut);
	wcscpy(dlg.m_strNo, NoBut);
	
	if (TRUE == lnkSystem->m_bSystemInitialized)
	{
		GetPrivateProfileString(strSection, strKey, _T("CAN NOT READ"), chrEnglishMessage, 256, DISPLAY_ENGLISH);
		strMonitoringLog.Format(_T("MSGBOX [1] %s"), chrEnglishMessage);
		lnkSystem->GetTrsAutoManagerComponent()->WriteMonitoringLog(strMonitoringLog);
	}
	
	iSelect = dlg.DoModal();
	
	switch (iSelect)
	{
	case IDABORT: strUserSelect.Format(_T("IDABORT")); break;
	case IDOK: strUserSelect.Format(_T("IDOK")); break;
	case IDYES: strUserSelect.Format(_T("IDYES")); break;
	case IDRETRY: strUserSelect.Format(_T("IDRETRY")); break;
	case IDCANCEL: strUserSelect.Format(_T("IDCANCEL")); break;
	case IDNO: strUserSelect.Format(_T("IDNO")); break;
	case IDIGNORE: strUserSelect.Format(_T("IDIGNORE")); break;
	default: strUserSelect.Format(_T("UNKNOWN")); break;
	}
	
	if (TRUE == lnkSystem->m_bSystemInitialized)
	{
		strMonitoringLog.Format(_T("MSGBOX [0] [Select] %s"), strUserSelect);
		lnkSystem->GetTrsAutoManagerComponent()->WriteMonitoringLog(strMonitoringLog);
	}
	
	return iSelect;
}

// MyMessage 표시시 Section, Title 영문으로 표시..
UINT MyMessageBox(CString strSection, CString strTitle, UINT type, WCHAR* YesBut, WCHAR* NoBut)
{
	CCommonMsgDlg dlg;
	
	BOOL	bReturn = TRUE;
	
	// strKey 값은 숫자만 받는다...처리는 이곳에서 한다.
	CString strKey;
	
	if (type & M_ICONINFORMATION) dlg.m_icontype = M_ICONINFORMATION;
	else if (type & M_ICONQUESTION) dlg.m_icontype = M_ICONQUESTION;
	else if (type & M_ICONSTOP) dlg.m_icontype = M_ICONSTOP;
	else if (type & M_ICONERROR) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;
	
	if (type & M_OK) dlg.m_type = M_OK;
	else if (type & M_OKCANCEL) dlg.m_type = M_OKCANCEL;
	else if (type & M_YESNO) dlg.m_type = M_YESNO;
	else if (type & M_ABORTRETRYIGNORE) dlg.m_type = M_ABORTRETRYIGNORE;
	else if (type & M_RETRYCANCEL) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;
	
	// Line 바꾸는것 적용...
	strSection.Replace(_T("@@"), _T("\n\r"));
	
	wcscpy(dlg.m_message, strSection);
	wcscpy(dlg.m_title, strTitle);
	wcscpy(dlg.m_strYes, YesBut);
	wcscpy(dlg.m_strNo, NoBut);
	
	return (dlg.DoModal());
}

// Data에서 불러오는 것 이외에 %s, %d 등 Display
UINT MyMessageBox(char* message, WCHAR* title, UINT type, WCHAR* YesBut, WCHAR* NoBut)
{
	CCommonMsgDlg dlg;
	
	CString strMessage(message);
	
	if (type & M_ICONINFORMATION) dlg.m_icontype = M_ICONINFORMATION;
	else if (type & M_ICONQUESTION) dlg.m_icontype = M_ICONQUESTION;
	else if (type & M_ICONSTOP) dlg.m_icontype = M_ICONSTOP;
	else if (type & M_ICONERROR) dlg.m_icontype = M_ICONERROR;
	else dlg.m_icontype = M_ICONINFORMATION;
	
	if (type & M_OK) dlg.m_type = M_OK;
	else if (type & M_OKCANCEL) dlg.m_type = M_OKCANCEL;
	else if (type & M_YESNO) dlg.m_type = M_YESNO;
	else if (type & M_ABORTRETRYIGNORE) dlg.m_type = M_ABORTRETRYIGNORE;
	else if (type & M_RETRYCANCEL) dlg.m_type = M_RETRYCANCEL;
	else dlg.m_type = M_OK;
	
	wcscpy(dlg.m_message, strMessage.GetBuffer(strMessage.GetLength()));
	
	wcscpy(dlg.m_title, title);
	
	wcscpy(dlg.m_strYes, YesBut);
	wcscpy(dlg.m_strNo, NoBut);
	
	return (dlg.DoModal());
}

void SetTitleLT(CBtnEnh* pButton, CString strSection, CString strKey)
{
	CTextDescriptor text;
	text = pButton->GetTextDescrLT();

	WCHAR chrTemp[256];

	GetPrivateProfileString(strSection, strKey, _T("CAN NOT READ"), chrTemp, 256, getFilePath());

	CString szTemp;
	szTemp = chrTemp;
	szTemp.Replace(_T("@@"),_T("\r\n"));
	text.SetText((LPCTSTR)szTemp);
//	text.SetText(szTemp.GetBuffer(szTemp.GetLength()));
	//text.SetText(chrTemp);
}

void SetTitleCB(CBtnEnh* pButton, CString strSection, CString strKey)
{
	CTextDescriptor text;
	text = pButton->GetTextDescrCB();
	WCHAR chrTemp[256];

	GetPrivateProfileString(strSection, strKey, _T("CAN NOT READ"), chrTemp, 256, getFilePath());

	CString szTemp;
	szTemp = chrTemp;
	szTemp.Replace(_T("@@"),_T("\r\n"));
	text.SetText((LPCTSTR)szTemp);
//	text.SetText(szTemp.GetBuffer(szTemp.GetLength()));
	//text.SetText(chrTemp);
}

int ReadDisplayMessage(CString strSection, CString strKey, CString* pstrValue)
{
	BOOL	bReturn = TRUE;
	WCHAR chrTemp[512];
	WCHAR chrKoreanMessage[256];

	GetPrivateProfileString(strSection, strKey, _T("CAN NOT READ"), chrTemp, 256, getFilePath());

	//SESL_LKH_090709 : Button이외에 Message만 한글 병행 표기
	if (LANGUAGE_KOREAN != g_uiLanguageSelect && MY_MESSAGE_BOX == strSection)
	{
		GetPrivateProfileString(strSection, strKey, _T("ERROR"), chrKoreanMessage, 256, DISPLAY_KOREAN);
		pstrValue->Format(_T("%s\r\n%s"), chrTemp, chrKoreanMessage);
	}
	else
	{
		pstrValue->Format(_T("%s"), chrTemp);
	}
//	pstrValue->Format(_T("%s"), chrTemp);
	pstrValue->Replace(_T("@@"), _T("\n\r"));

	return bReturn;
}

CString getFilePath()
{
	CString strFilePath;

	switch (g_uiLanguageSelect)
	{
	case LANGUAGE_KOREAN:
		strFilePath = DISPLAY_KOREAN;
		break;
	case LANGUAGE_ENGLISH:
		strFilePath = DISPLAY_ENGLISH;
		break;
	case LANGUAGE_CHINESE:
		strFilePath = DISPLAY_CHINESE;
		break;
	case LANGUAGE_SLOVAK:
		strFilePath = DISPLAY_SLOVAK;
		break;
	}

	return strFilePath;
}

// Get Error Path
CString getErrorFilePath()
{
	CString strFilePath;

	switch (g_uiLanguageSelect)
	{
	case LANGUAGE_KOREAN:
		strFilePath = ERRORMESSAGE_KOREAN;
		break;
	case LANGUAGE_ENGLISH:
		strFilePath = ERRORMESSAGE_ENGLISH;
		break;
	case LANGUAGE_CHINESE:
		strFilePath = ERRORMESSAGE_CHINESE;
		break;
	case LANGUAGE_SLOVAK:
		strFilePath = ERRORMESSAGE_SLOVAK;
		break;
	}

	return strFilePath;
}

CString ReadErrorMessage(int iErrorCode, UINT uiLanguageType)
{
   	CString strMsg;
	CString strErrorCode;
	CString strErrorNotDefined;
	WCHAR chMessage[256];
	WCHAR chrKoreanMessage[256];

	CString strSection = _T("Error");

	// iResult의 뒤 세자리 000을 지우지 않고 넣었을 경우에 대비함
	if (iErrorCode >= 100000000)
		iErrorCode = iErrorCode / 1000;

	strErrorCode.Format(_T("%d"), iErrorCode);
	strErrorNotDefined.Format(_T("[%d] Not Defined"), iErrorCode);

	CString strErrorFilePath;
	switch (uiLanguageType)
	{
	case LANGUAGE_KOREAN:
		strErrorFilePath = ERRORMESSAGE_KOREAN;
		break;
	case LANGUAGE_ENGLISH:
		strErrorFilePath = ERRORMESSAGE_ENGLISH;
		break;
	case LANGUAGE_CHINESE:
		strErrorFilePath = ERRORMESSAGE_CHINESE;
		break;
	case LANGUAGE_SLOVAK:
		strErrorFilePath = ERRORMESSAGE_SLOVAK;
		break;
		
	default:
		strErrorFilePath = getErrorFilePath();	
		break;
	}

	GetPrivateProfileString(strSection, strErrorCode, strErrorNotDefined, chMessage, 256, strErrorFilePath);

	//SESL_LKH_090709 : Button이외에 Message만 한글 병행 표기

	if (LANGUAGE_KOREAN != g_uiLanguageSelect)
	{
		GetPrivateProfileString(strSection, strErrorCode, strErrorNotDefined, chrKoreanMessage, 256, ERRORMESSAGE_KOREAN);
		strMsg.Format(_T("%s\r\n%s"), chMessage, chrKoreanMessage);
	}
	else
	{
		strMsg.Format(_T("%s"), chMessage);
	}

//	strMsg.Format(_T("%s"), chMessage);
	// Line 바꾸는것 적용...
	strMsg.Replace(_T("@@"), _T("\n\r"));
	return strMsg;
}

int SetErrorLevel(CString Error, int iErrorLevel, char* szFileName, int iLine)
{
	//LKH_090210 : SetErrorLevel 함수 Autorun 때만 사용하게 함
	if (g_bCheckAutoMode == FALSE)
		return 0;

	EnterCriticalSection(&g_csError);

//LKH_20081118 : 각 Layer에 따라 Error Message를 기록하는 함수(문자열 직접 설정)
	CString FileName = szFileName;
	int iFound = FileName.ReverseFind('/');
	if (iFound == -1)
		iFound = FileName.ReverseFind('\\');

	FileName = FileName.Mid(iFound + 1);

	if (g_iErrorNum[iErrorLevel] >= DEF_MAX_ERROR)
	{
		for (int i = 0; i < DEF_MAX_ERROR - 1; i++)
		{
			g_Error1[iErrorLevel][i] = g_Error1[iErrorLevel][i+1];
			g_Error2[iErrorLevel][i] = g_Error2[iErrorLevel][i+1];
		}
		g_iErrorNum[iErrorLevel] = DEF_MAX_ERROR - 1;
	}
	g_Error1[iErrorLevel][g_iErrorNum[iErrorLevel]].Format(_T("%s"), Error);
	g_Error2[iErrorLevel][g_iErrorNum[iErrorLevel]].Format(_T("(%s, Line : %d)"), FileName, iLine);
	g_iErrorNum[iErrorLevel] += 1;

	LeaveCriticalSection(&g_csError);

	return 0;
}

CString GetErrorLevel()
{
	//LKH_090210 : SetErrorLevel 함수 Autorun 때만 사용하게 함
	if (g_bCheckAutoMode == FALSE)
		return _T("Manual Mode");

	EnterCriticalSection(&g_csError);

	//LKH_20081118 : 저장된 각 Layer의 Error Message를 합쳐서 반환하는 함수
	//현재 같은 Layer에서 최대 5개까지 Error Message 누적 기록 중.
	CString strReturn = _T("\r\n\r\n");
	CString szTemp;
	int iOffsetCnt = 1;

	for (int i = 0; i < DEF_ERROR_DEPTH; i++)
	{
		if (!g_Error1[i][0].IsEmpty())
		{
			szTemp.Empty();

			for (int j = g_iErrorNum[i] - 1; j >= 0; j--)
			{
				if (i > 0)
				{
					for (int k = 0; k < iOffsetCnt - 1; k++)
						szTemp += XOFFSET;

					if (iOffsetCnt > 1)
						szTemp += XARROW;
					else
						szTemp += XOFFSET;
				}
				else
				{
					for (int k = 0; k < iOffsetCnt; k++)
						szTemp += XOFFSET;
				}

				szTemp += g_Error1[i][j] + _T("\r\n");

				for (int k = 0; k < iOffsetCnt; k++)
					szTemp += XOFFSET;

				szTemp += g_Error2[i][j] + _T("\r\n");
			}
			strReturn = strReturn + szTemp + _T("\r\n");
			iOffsetCnt++;
		}
	}

	LeaveCriticalSection(&g_csError);
	
	return strReturn;
}

int ResetErrorLevel()
{
	//LKH_090210 : SetErrorLevel 함수 Autorun 때만 사용하게 함
	if (g_bCheckAutoMode == FALSE)
		return 0;

	EnterCriticalSection(&g_csError);

//LKH_20081118 : 저장된 각 Layer의 Error Message 삭제(Error Message Dialog 종료시 호출)
	for (int i = 0; i < DEF_ERROR_DEPTH; i++)
	{
		for (int j = 0; j < DEF_MAX_ERROR; j++)
		{
			g_Error1[i][j].Empty();
			g_Error2[i][j].Empty();
		}
		g_iErrorNum[i] = 0;
	}

	LeaveCriticalSection(&g_csError);
	return 1;
}

//LKH_090210 : SetErrorLevel 함수 Autorun 때만 사용하게 함
void SetAutoRunLevel()
{
	g_bCheckAutoMode=TRUE;
}
void SetManualLevel()
{
	g_bCheckAutoMode=FALSE;
}