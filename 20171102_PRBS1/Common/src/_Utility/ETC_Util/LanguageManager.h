#include "stdafx.h"
#include "MFileManager.h"
#include "MString.h"
#include "MyProgressWnd.h"
#include "CommonMsgDlg.h"
#include "textdescriptor.h"

#ifdef UNICODE
#include "CommonGetNumDlg.h"
#include "CommonGetValueDlg.h"
#else
#include "GetNumDlg.h"
#include "GetValueDlg.h"
#endif

#pragma once



enum eLanguage_List
{
	eLANGUAGE_SELECTED = -1, //Default ���� �о����
	eLANGUAGE_KOREAN = 0,
	eLANGUAGE_ENGLISH = 1,
	eLANGUAGE_CHINESE = 2,
	eLANGUAGE_SLOVAK = 3,
	eLANGUAGE_MAX_COUNT	= 4
};

//Display ���� ���
const CString DEF_FILE_NAME_DISPLAY[eLANGUAGE_MAX_COUNT] = { _T("Language_Korean.dat"),
															 _T("Language_English.dat"),
															 _T("Language_Chinese.dat"),
															 _T("Language_Slovak.dat")	};

// Error Message Language ���� ���.
const CString DEF_FILE_NAME_ERROR[eLANGUAGE_MAX_COUNT] =	{ _T("ErrorMessage_Korean.dat"),
															  _T("ErrorMessage_English.dat"),
															  _T("ErrorMessage_Chinese.dat"),
															  _T("ErrorMessage_Slovak.dat")	};

const CString DEF_DIRECTION_LANGUAGE_FILE = _T("./Info/");

const CString DEF_FILE_NAME_ERRORLIST = _T("ErrorList.csv");
const CString DEF_DIRECTION_ERRORLIST_FILE = _T(".\\data\\");
const int DEF_MAX_ERROR_LIST_COUNT = 500;

class MLanguageManager
{		
private :
	static eLanguage_List eNowLanguage;
public :
	static eLanguage_List GetNowLanguage();
	static void SetNowLanguage(eLanguage_List eLanguage);
	static void SetNowLanguage(unsigned int eLanguage);
	
	// Display Language ������ �о�´�.
	static int ReadDisplayMessage(CString strSection, CString strKey, CString* pstrValue, eLanguage_List eLanguage = eLANGUAGE_SELECTED);   
	////���Ͽ� �ش� ���� ����.  strSection : ���� �̸�, strKey : �̸�, strValue : �̸��� �� 
	static int WriteDisplayMessage(CString strSection, CString strKey, CString strValue, eLanguage_List eLanguage = eLANGUAGE_SELECTED); 
	// Error Language ������ �о�´�.
	static int ReadErrorMessage(CString strSection, CString strKey, CString* pstrValue, eLanguage_List eLanguage = eLANGUAGE_SELECTED);   
	////���Ͽ� �ش� ���� ����.  strSection : ���� �̸�, strKey : �̸�, strValue : �̸��� �� 
	static int WriteErrorMessage(CString strSection, CString strKey, CString strValue, eLanguage_List eLanguage = eLANGUAGE_SELECTED); 

	//Error Language ������ �о�´�. 
	static CString ReadErrorMessage(int iErrorCode, eLanguage_List uiLanguageType = eLANGUAGE_SELECTED, BOOL bErrorReport = FALSE);

	//.rc ���Ͽ��� ��ư�� ID ���� �о�´�. 
	//strName : ���̾�α� ID��, strValue : ��ư ���� ���� String �迭, iCount : �������� ��ư ����, Return ��ư ID �迭 ��
	static int GetRCDescription(CString strName, CString *strValue, int *iCount);
	//resource.h ���Ͽ��� ID ��ȣ�� �о�´�. 
	//strName : ��ư ID ��, return ID ��
	static int GetResourceDescription(CString strName);

	static int ChangeDialogLanguage(CWnd* pCWndDialog, CString strIDD);

	static BOOL WriteErrorReadingFailLog(CString strMsg);//170710 JSH
	static BOOL WriteErrorListMessage(int iErrorCode);
	static BOOL WriteErrorListMessage(CString strErrorMsg);
	static int ReadErrorListMessage(int (*iErrorCode)[DEF_MAX_ERROR_LIST_COUNT], CString (*strErrorTime)[DEF_MAX_ERROR_LIST_COUNT] = NULL, CString (*strErrorMsg)[DEF_MAX_ERROR_LIST_COUNT] = NULL);
};

class MDisplayManager : public MFileManager
{
private:
    MDisplayManager(CString strFileName);    
    MDisplayManager(CString strFileName, CString strPath);
	MDisplayManager();

	static MDisplayManager* m_LM_Display[eLANGUAGE_MAX_COUNT];

public:
	static MDisplayManager* GetInstance(eLanguage_List eLanguage_Number);

	int ReadMessage(CString strSection, CString strKey, CString* pstrValue);   
	int WriteMessage(CString strSection, CString strKey, CString strValue); 
};

class MErrorManager : public MFileManager
{
private:
    MErrorManager(CString strFileName);    
    MErrorManager(CString strFileName, CString strPath);
	MErrorManager();

	static MErrorManager* m_LM_Display[eLANGUAGE_MAX_COUNT];

public:
	static MErrorManager* GetInstance(eLanguage_List eLanguage_Number);

	int ReadMessage(CString strSection, CString strKey, CString* pstrValue);   
	int WriteMessage(CString strSection, CString strKey, CString strValue); 

};