#include "stdafx.h"
#include "UtilManager.h"
#include "math.h"
#include "MDebug.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
const CString MUtilManager::strKeyboardWindowName = _T("ȭ�� Ű����");
const CString MUtilManager::strKeyboardFileName = _T("osk.exe");
const CString MUtilManager::strSystem32FilePath = _T("C:\\WINDOWS\\system32\\");

//170731_KDH PPT ���� ���� �뵵 
const CString MUtilManager::strInfoPanelFileName = _T("Machine_Info.pptx");
const CString MUtilManager::strInfoFilePath = _T("\\Info\\");
//___________________________________

BOOL MUtilManager::GetNumberBox(TCHAR* szNum, int maxchar, TCHAR* getnum, TCHAR* title, BOOL pwd/*=FALSE*/)
{
#ifdef UNICODE
   CCommonGetNumDlg dlg(maxchar, getnum, title, NULL, pwd);
#else
   CGetNumDlg dlg(maxchar, getnum, title, NULL, pwd);
#endif

   if (dlg.DoModal() == IDOK)
   {
	  lstrcpy(szNum, dlg.m_strGetNum);
	  return TRUE;
   }
   else
	   return FALSE;
}

BOOL MUtilManager::GetValueBox(TCHAR* szVal, int maxchar, TCHAR* getval, TCHAR* title, BOOL pwd/*=FALSE*/, BOOL btndisable/*= FALSE*/)
{
#ifdef UNICODE
   CCommonGetValueDlg dlg(maxchar, getval, title, NULL, pwd, btndisable);
#else
	CGetValueDlg dlg(maxchar, getval, title, NULL, pwd, btndisable);
#endif
   
   if (dlg.DoModal() == IDOK)
   {
      lstrcpy(szVal, dlg.m_strGetVal);
      return TRUE;
   }
   else
	   return FALSE;
}	


// ML_hongju �ٱ��� ����
//____________________________________________________________________________________________________________________________________

// MyMessage ǥ�� �� Title�� �׳� �������� ǥ��..
UINT MUtilManager::MyMessageBox(CString strSection, UINT iKey, CString strTitle, UINT type, TCHAR* YesBut, TCHAR* NoBut)
{
	CString strMonitoringLog;
	CString strUserSelect;

	int iSelect;

	CCommonMsgDlg dlg;
	
	BOOL	bReturn = TRUE;
	TCHAR	chMessage[500];
	CString strKoreanMessage = _T("");
	CString	strEnglishMessage = _T("");
	CString	strTempMessage = _T("");

	memset(chMessage, 0, sizeof(chMessage));
	
	// strKey ���� ���ڸ� �޴´�...ó���� �̰����� �Ѵ�.
	CString strKey;
	
	strKey.Format(_T("Message_%d"), iKey);
	MLanguageManager::ReadDisplayMessage(strSection, strKey, &strEnglishMessage);

	// ML_hongju �߰� ����... �׻� �ѱ��� ���� ���°� ������ ����.
	if (eLANGUAGE_KOREAN != MLanguageManager::GetNowLanguage())
		MLanguageManager::ReadDisplayMessage(strSection, strKey, &strKoreanMessage, eLANGUAGE_KOREAN);
	
	// Line �ٲٴ°� ����...
	strTempMessage.Format(_T("%s\r\n%s"), strEnglishMessage, strKoreanMessage);
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
	
#ifdef UNICODE
	wcscpy(dlg.m_message, strTempMessage);
	wcscpy(dlg.m_title, strTitle);
	wcscpy(dlg.m_strYes, YesBut);
	wcscpy(dlg.m_strNo, NoBut);
#else
	strcpy(dlg.m_message, strTempMessage);
	strcpy(dlg.m_title, strTitle);
	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);
#endif
	MLanguageManager::ReadDisplayMessage(strSection, strKey, &strEnglishMessage, eLANGUAGE_ENGLISH);
	strMonitoringLog.Format(_T("MSGBOX [1] %s"), strEnglishMessage);
#ifndef WriteDebugLog
	MDebug::WriteDebugLog(strMonitoringLog);
#endif
	
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
	
#ifndef WriteDebugLog
	strMonitoringLog.Format(_T("MSGBOX [0] [Select] %s"), strUserSelect);
	MDebug::WriteDebugLog(strMonitoringLog);
#endif

	return iSelect;
}

// MyMessage ǥ�ý� Section, Title �������� ǥ��..
UINT MUtilManager::MyMessageBox(CString strSection, CString strTitle, UINT type, TCHAR* YesBut, TCHAR* NoBut)
{
	CCommonMsgDlg dlg;
	
	BOOL	bReturn = TRUE;
	
	// strKey ���� ���ڸ� �޴´�...ó���� �̰����� �Ѵ�.
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
	
	// Line �ٲٴ°� ����...
	strSection.Replace(_T("@@"), _T("\n\r"));
#ifdef UNICODE
	wcscpy(dlg.m_message, strSection);
	wcscpy(dlg.m_title, strTitle);
	wcscpy(dlg.m_strYes, YesBut);
	wcscpy(dlg.m_strNo, NoBut);
#else
	strcpy(dlg.m_message, strSection);
	strcpy(dlg.m_title, strTitle);
	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);
#endif
	return (dlg.DoModal());
}


// Data���� �ҷ����� �� �̿ܿ� %s, %d �� Display
UINT MUtilManager::MyMessageBox(TCHAR* message, TCHAR* title, UINT type, TCHAR* YesBut, TCHAR* NoBut, TCHAR* UserBut)
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
	
#ifdef UNICODE
	wcscpy(dlg.m_message, strMessage.GetBuffer(0));
	wcscpy(dlg.m_title, title);
	wcscpy(dlg.m_strYes, YesBut);
	wcscpy(dlg.m_strNo, NoBut);
//	wcscpy(dlg.m_strUser, UserBut); //���⼭ ���� �߻��� ���� �ʿ�
#else
	strcpy(dlg.m_message, strMessage.GetBuffer(0));
	strcpy(dlg.m_title, title);
	strcpy(dlg.m_strYes, YesBut);
	strcpy(dlg.m_strNo, NoBut);
	strcpy(dlg.m_strUser, UserBut);
#endif

	return (dlg.DoModal());

}
UINT MUtilManager::MyMessageBox(UINT type, TCHAR* title, TCHAR* YesBut, TCHAR* NoBut, TCHAR* message)
{
	return MUtilManager::MyMessageBox(message, title, type, YesBut, NoBut);
}


BOOL MUtilManager::DisplayDataReadError(CString strSection, CString strItem, CString strFileName)
{
	CString strTemp = MString::MFormat(_T("[%s] %s"), strSection, strItem);
	CString strMsg = strTemp + _T(" item was not read. Do you go ahead reading next item?");
	CString strTitle = MString::MFormat(_T("%s File Read Error"), strFileName);

	if (IDNO == MUtilManager::MyMessageBox(strMsg, strTitle, M_ICONERROR|M_YESNO))
		return FALSE;

	return TRUE;
}

BOOL MUtilManager::DisplayDataWriteError(CString strSection, CString strItem, CString strFileName)
{
	CString strTemp = MString::MFormat(_T("[%s] %s"), strSection, strItem);
	CString strMsg = strTemp + _T(" item was not written. Do you go ahead writing next item?");
	CString strTitle = MString::MFormat(_T("%s File Write Error"), strFileName);

	if (IDNO == MUtilManager::MyMessageBox(strMsg, strTitle, M_ICONERROR|M_YESNO))
		return FALSE;

	return TRUE;
}

void MUtilManager::SetTitleLT(CBtnEnh* pButton, CString strSection, CString strKey)
{
	CTextDescriptor text = pButton->GetTextDescrLT();

	CString szTemp;
	MLanguageManager::ReadDisplayMessage(strSection, strKey, &szTemp);
	text.SetText((LPCTSTR)szTemp);
}

void MUtilManager::SetTitleCB(CBtnEnh* pButton, CString strSection, CString strKey)
{
	CTextDescriptor text = pButton->GetTextDescrCB();

	CString szTemp;
	MLanguageManager::ReadDisplayMessage(strSection, strKey, &szTemp);
	text.SetText(szTemp.GetBuffer(0));
}

DWORD MUtilManager::HexToInt(char* s)
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




void MUtilManager::ChartoDec(char* tmp, int &rval)
{
	// 2���� char[13]�� 10������...
	int aa, i, itmp;

	aa = 0;

	for(i=0;i<12;i++)
	{
		if(tmp[i] == '0') itmp = 0;
		else itmp = 1;
		
		aa += (int)(itmp)*(int)pow(2.0,(double)(i));
	}

	rval = aa;
}

void MUtilManager::DectoBin(int tmp,char* rval)
{
	int i;
	// 10������ 2���� char��....

	for( i = 0; i < 12; i++)
	{
		if(tmp%2 == 0) rval[i] = '0';
		else rval[i] = '1';

		tmp = (int)((double)tmp/2.0);
	}
}


BOOL CALLBACK MUtilManager::EnumWindowCallBack(HWND hwnd, LPARAM lParam)
{
	vector<HWND>* VarList = (vector<HWND>*)lParam;
	if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0)
		VarList->push_back(hwnd);		

	return TRUE;
}


HWND MUtilManager::GetRunningProcessHWND(CString strProcess)
{
	vector<HWND> VarList;
	EnumWindows(EnumWindowCallBack, (LPARAM)&VarList);
	for(unsigned int i = 0; i < VarList.size(); i++)
	{
		TCHAR strProcessName[255];
		GetWindowText(VarList[i], strProcessName, 255);
		if(strProcessName == strProcess)
			return VarList[i];
	}
	return NULL;
}

void MUtilManager::DisplayWindowsKeyboard()
{

	//170914 JSH.s Win7 ȭ��Ű���� //Win7�� �ٸ� ȯ�濡�� Test �ʿ�
	::ShellExecute(NULL, _T("open"), _T("..\\Common\\src\\_Utility\\ETC_Util\\TabTip.exe"), NULL, _T(","), SW_SHOW); // ���α׷� ����
	/*/
	HWND KeyHWND = GetRunningProcessByHWND(strKeyboardWindowName);
	if(KeyHWND != NULL) //���α׷��� �������ϰ��
		ShowWindow(KeyHWND, SW_SHOWDEFAULT); //�ּ�ȭ�� ����
	else
	::ShellExecute(NULL, _T("open"), strSystem32FilePath + strKeyboardFileName, NULL, _T(","), SW_SHOW); // ���α׷� ����
	/*/
	//170914 JSH.e
}

void MUtilManager::DisplayInfoPanel()
{
	TCHAR lpszPath[MAX_PATH]; 
	DWORD dwCchPath = MAX_PATH;

	ZeroMemory(lpszPath, sizeof(TCHAR) * dwCchPath);
	if (0 == ::GetModuleFileName(NULL, lpszPath, dwCchPath))
		return; // Module Handle ���ϴµ� ����
	
	for(int i = 0; i < 2; i++)
	{
		LPTSTR lpszFound = _tcsrchr(lpszPath, TEXT('\\'));
		if (NULL == lpszFound) // Path ���� \�� ����?
			return;
		
		if (lpszFound <= lpszPath)
			return;
		*lpszFound = TEXT('\0');
	}
	wsprintf(lpszPath, _T("%s%s%s"), lpszPath, strInfoFilePath, strInfoPanelFileName);
//	::ShellExecute(NULL, _T("open"), _T("") + strInfoPanelFileName, NULL, _T(","), SW_SHOW); // ���α׷� ����
	::ShellExecute(NULL, _T("open"), lpszPath, NULL, _T(","), SW_SHOW); // ���α׷� ����
}