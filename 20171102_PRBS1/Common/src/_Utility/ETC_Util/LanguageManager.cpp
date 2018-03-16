	#include "stdafx.h"
#include "LanguageManager.h"




eLanguage_List MLanguageManager::eNowLanguage = eLANGUAGE_KOREAN;
eLanguage_List MLanguageManager::GetNowLanguage() { return eNowLanguage; }
void MLanguageManager::SetNowLanguage(eLanguage_List eLanguage) { eNowLanguage = eLanguage; }
void MLanguageManager::SetNowLanguage(unsigned int eLanguage) { eNowLanguage = (eLanguage_List)eLanguage; }

int MLanguageManager::ReadDisplayMessage(CString strSection, CString strKey, CString* pstrValue, eLanguage_List eLanguage )
{
	int iReturn = 0;

	eLanguage_List eNowLanguage = eLanguage;
	if(eLanguage == eLANGUAGE_SELECTED)
		eNowLanguage = MLanguageManager::GetNowLanguage();

	MDisplayManager* pNowDisplayManager = MDisplayManager::GetInstance(eNowLanguage);
	iReturn = pNowDisplayManager->ReadMessage(strSection, strKey, pstrValue);
	pstrValue->Replace(_T("@@"), _T("\n\r"));

	return iReturn;
}
int MLanguageManager::WriteDisplayMessage(CString strSection, CString strKey, CString strValue, eLanguage_List eLanguage)
{
	int iReturn = 0;
	
	eLanguage_List eNowLanguage = eLanguage;
	if(eLanguage == eLANGUAGE_SELECTED)
		eNowLanguage = MLanguageManager::GetNowLanguage();

	strValue.Replace(_T("\r\n"), _T("@@"));
	strValue.Replace(_T("\n\r"), _T("@@"));
	strValue.Replace(_T("\n"), _T("@@"));
	MDisplayManager* pNowDisplayManager = MDisplayManager::GetInstance(eNowLanguage);
	iReturn = pNowDisplayManager->WriteMessage(strSection, strKey, strValue);

	return iReturn;
}
int MLanguageManager::ReadErrorMessage(CString strSection, CString strKey, CString* pstrValue, eLanguage_List eLanguage )
{
	int iReturn = 0;

	eLanguage_List eNowLanguage = eLanguage;
	if(eLanguage == eLANGUAGE_SELECTED)
		eNowLanguage = MLanguageManager::GetNowLanguage();
	
	MErrorManager* pNowDisplayManager = MErrorManager::GetInstance(eNowLanguage);
	iReturn = pNowDisplayManager->ReadMessage(strSection, strKey, pstrValue);
	//170710 JSH.s
	//ErrorCode Reading Fail
	if(iReturn == 0)
	{
		//Log Error Code, Section, eLangug
		CString strErrorFailLog;

		strErrorFailLog.Format(_T("Language[%d] "), eLanguage);
		strErrorFailLog += strSection + _T(" ") + strKey + _T("\n");

		WriteErrorReadingFailLog(strErrorFailLog);
	}
	//170710 JSH.e
	pstrValue->Replace(_T("@@"), _T("\n\r"));

	return iReturn;
}
BOOL MLanguageManager::WriteErrorReadingFailLog(CString strMsg)
{
//	::EnterCriticalSection(&m_csErrorReadingFailLog);
	
	FILE* fsource;
	CFileFind fn;
	CString fname;

	CTime tmCurr = CTime::GetCurrentTime();
	strMsg = tmCurr.Format(_T("[%m/%d-%H:%M:%S]")) + strMsg;
	fname = _T("..\\Log\\ErrorReadingFailLog");
	
//	if (FALSE == fn.FindFile(fname, 0))
		CreateDirectory(fname, NULL);

	fname = _T("..\\Log\\ErrorReadingFailLog\\ErrorReadingFailLog") + tmCurr.Format(_T("%m%d")) + _T(".txt");

	if ((fsource = _wfopen(fname, _T("ab+,ccs=UNICODE"))) == NULL)
	{
//		::LeaveCriticalSection(&m_csErrorReadingFailLog);
		return FALSE;
	}
	//현재 발생 로그 내용 기록
	fwprintf(fsource, _T("%s\r\n"), strMsg.GetBuffer());

	fclose(fsource);

//	::LeaveCriticalSection(&m_csErrorReadingFailLog);
	return TRUE;
}
int MLanguageManager::WriteErrorMessage(CString strSection, CString strKey, CString strValue, eLanguage_List eLanguage)
{
	int iReturn = 0;
	
	eLanguage_List eNowLanguage = eLanguage;
	if(eLanguage == eLANGUAGE_SELECTED)
		eNowLanguage = MLanguageManager::GetNowLanguage();

	strValue.Replace(_T("\r\n"), _T("@@"));
	strValue.Replace(_T("\n\r"), _T("@@"));
	strValue.Replace(_T("\n"), _T("@@"));
	MErrorManager* pNowDisplayManager = MErrorManager::GetInstance(eNowLanguage);
	iReturn = pNowDisplayManager->WriteMessage(strSection, strKey, strValue);

	return iReturn;
}
CString MLanguageManager::ReadErrorMessage(int iErrorCode, eLanguage_List eLanguage, BOOL bErrorReport)
{
	CString strReturnValue = _T("");
	CString strSection = _T("Error");
	CString strKoreanValue = _T("");
	// iResult의 뒤 세자리 000을 지우지 않고 넣었을 경우에 대비함
	if (iErrorCode >= 100000000)
		iErrorCode = iErrorCode / 1000;
	
	CString strErrorCode = MString::MFormat(_T("%d"), iErrorCode);
	CString strErrorNotDefined = MString::MFormat(_T("[%d] Not Defined"), iErrorCode);

	ReadErrorMessage(strSection, strErrorCode, &strReturnValue, eLanguage);
	if(eLANGUAGE_KOREAN !=  MLanguageManager::GetNowLanguage() && FALSE == bErrorReport)
	{
		ReadErrorMessage(strSection, strErrorCode, &strKoreanValue, eLANGUAGE_KOREAN);
		strReturnValue += strKoreanValue;
	}

	strReturnValue.Replace(_T("@@"), _T("\n\r"));
	return strReturnValue;
}


int MLanguageManager::GetRCDescription(CString strName, CString *strValue, int *iCount)
{
	FILE *fp = NULL;
	fp = fopen("./MPlatformOlb.rc", "rt");
	if (!fp)	
		return -1;

	char szLine[1024], strTemp[1024];
	char *pLine = NULL, *szDescrition = NULL;
	BOOL bFindStr = FALSE;

	strName += _T(" DIALOG"); //IDD_AUTO_MAIN_FORM DIALOG 부터 데이터가 시작된다.
	*iCount = 0;
	while (!feof(fp)) 
	{
		pLine = fgets(szLine, 1024, fp); //파일을 읽어온다.
		if (pLine == NULL)	//더이상 읽을게 없을경우 나가기
			break;

		if (bFindStr == FALSE)
		{
#ifdef UNICODE
			if(strstr(szLine, MString::ConvertWideCharToChar(strName.GetBuffer(0), strTemp)) != NULL) //다이얼로그를 찾았을경우 Flag 살리기
				bFindStr = TRUE;	
#else
			if(strstr(szLine, strName.GetBuffer(0)) != NULL) //다이얼로그를 찾았을경우 Flag 살리기
				bFindStr = TRUE;	
#endif

		}
		
		if(bFindStr == TRUE) //다이얼로그를 찾았을 경우
		{
			if(strstr(szLine, "CONTROL") != NULL) //버튼을 찾는다.
			{
				szDescrition = strtok(szLine, ",\t");
				szDescrition = strtok(NULL, ",\t");
				strValue[*iCount] = szDescrition;
				*iCount = *iCount + 1;
			}
			if(strncmp(szLine, "END", 3) == NULL) //끝났을 경우 나가기
				break;
		}
	}
	fclose(fp);
	return 0;
}
int MLanguageManager::GetResourceDescription(CString strName)
{
	FILE *fp = NULL;
	fp = fopen("./resource.h", "rt");
	if (!fp)	
		return -1;

	char szLine[300], strTemp[50];
	char *token, *pLine;
	char seps[] = " \t";
	int index, no;
	//
	while (!feof(fp)) 
	{
		pLine = fgets(szLine, 300, fp);
		if (pLine == NULL)	
			break;

		index = 0;
		token = strtok(szLine, seps);
		while (token != NULL) 
		{
			if (index == 0 && strcmp(token, "#define") != 0)	
				break;
#ifdef UNICODE
			if (index == 1 && strcmp(token, MString::ConvertWideCharToChar(strName.GetBuffer(0), strTemp)) != NULL)
				break;
#else
			if (index == 1 && strcmp(token, strName.GetBuffer(0)) != NULL)
				break;
#endif
			if (index == 2) 
			{
				char strNo[10];
				strcpy(strNo, token);
				sscanf(strNo, "%d", &no);
				fclose(fp);
				return no;
			}
			token = strtok(NULL, seps);
			index++;
		}
	}
	fclose(fp);

	return -1;
}
int MLanguageManager::ChangeDialogLanguage(CWnd* pCWndDialog, CString strIDD)
{
	CString strResourceName[1000];
	int iCount = 0, iResult = 0;
	
	iResult = MLanguageManager::GetRCDescription(strIDD, strResourceName, &iCount);
	if(iResult)
		return iResult;


	for(int i = 0; i < iCount; i++)
	{
		if(strResourceName[i] != "")
		{
			int iDlgID = MLanguageManager::GetResourceDescription(strResourceName[i]);
			if(iDlgID != -1)
			{
				CWnd* CWndBtn = pCWndDialog->GetDlgItem(iDlgID);
				if(CWndBtn != NULL)
				{
					CString strValue;
					int iResult = ReadDisplayMessage(strIDD, strResourceName[i], &strValue);
					if(strValue != _T("CAN NOT READ") && iResult != 0)
						CWndBtn->SetWindowText(strValue);
					else if( 
#ifdef UNICODE
						 wcsncmp(strResourceName[i], _T("IDC"), 3) != FALSE
						 && wcsncmp(strResourceName[i], _T("IDS"), 3) != FALSE)
#else
						 strncmp(strResourceName[i], _T("IDC"), 3) != FALSE
						 && strncmp(strResourceName[i], _T("IDS"), 3) != FALSE)
#endif
					{
						CString strBtnText;
						CWndBtn->GetWindowText(strBtnText);
						WriteDisplayMessage(strIDD, strResourceName[i], strBtnText);
					}
				}
			}
		}
	}

	return iResult;
}
BOOL MLanguageManager::WriteErrorListMessage(int iErrorCode)
{
	CString strMessage = ReadErrorMessage(iErrorCode);
	return WriteErrorListMessage(strMessage);
}
BOOL MLanguageManager::WriteErrorListMessage(CString strErrorMsg)
{
	//LKH_20090112 : Errorlist Unicode대응 수정
	FILE *fpWrite, *fpRead;
	CFileFind fn;

	CString strFileName = DEF_DIRECTION_ERRORLIST_FILE + DEF_FILE_NAME_ERRORLIST;
	CString strFileNameBak = DEF_DIRECTION_ERRORLIST_FILE + DEF_FILE_NAME_ERRORLIST + _T(".bak");

	CTime tmCurr = CTime::GetCurrentTime();
	CString strWriteMessage = _T("");
	strErrorMsg.Replace(_T("\r\n"), _T("@@")); strErrorMsg.Replace(_T("\n\r"), _T("@@"));
	strErrorMsg.Replace(_T("\n"), _T("@@"));
	strWriteMessage.Format(_T("%s, %s"), tmCurr.Format(_T("[%m/%d-%H:%M:%S]")), strErrorMsg);

	if (fn.FindFile(strFileName, 0))
	{
		if (!CopyFile(strFileName, strFileNameBak, FALSE))
			TRACE(_T("Error Data Backup File 생성 실패\n"));

		if (!DeleteFile(strFileName))
			TRACE(_T("Error Data 파일 삭제 실패\n"));
	}
	if ((fpWrite = _wfopen(strFileName, _T("wb+,ccs=UNICODE"))) == NULL)
	{
		TRACE(_T("Error Data File Open 실패\n"));
		return FALSE;
	}

	int count = 0;
	//Errorlist.Dat 파일을 Unicode 로 Incoding 하기 위한 2Byte 기록
	fwprintf(fpWrite, _T("%c"), 0xFEFF);
	//현재 발생한 Error 기록
	fwrite(strWriteMessage, sizeof(WCHAR), strWriteMessage.GetLength(), fpWrite);
	//기존 로그 내용이 있을 때에만 개행문자 필요
	fwprintf(fpWrite, _T("\r\n"));

	if ((fpRead = _wfopen(strFileNameBak, _T("rb"))) == NULL)
	{
		TRACE(_T("Backup File Open 실패\n"));
		fclose(fpWrite);
		return FALSE;
	}

	while (!feof(fpRead))
	{
		if (count > DEF_MAX_ERROR_LIST_COUNT)
			break;
		wchar_t buf[500];
		fgetws(buf, 500, fpRead);
		fwrite(buf, sizeof(WCHAR), wcslen(buf), fpWrite);

		count += 1;
	}
	
	fclose(fpWrite);
	fclose(fpRead);

	return TRUE;
}
//return = 총 갯수, ErrorCode, 
int MLanguageManager::ReadErrorListMessage(int (*iErrorCode)[DEF_MAX_ERROR_LIST_COUNT], CString (*strErrorTime)[DEF_MAX_ERROR_LIST_COUNT], CString (*strErrorMsg)[DEF_MAX_ERROR_LIST_COUNT])
{
	//LKH_20090112 : Errorlist Unicode대응 수정
	FILE *fpRead;
	CFileFind fn;

	CString strFileName = DEF_DIRECTION_ERRORLIST_FILE + DEF_FILE_NAME_ERRORLIST;
	CString strFileNameBak = DEF_DIRECTION_ERRORLIST_FILE + DEF_FILE_NAME_ERRORLIST + _T(".bak");

	CString strWriteMessage = _T("");
	if ((fpRead = _wfopen(strFileName, _T("rb"))) == NULL)
	{
		TRACE(_T("Error Data File Open 실패\n"));
		return 0;
	}

	int i = 0;
	for (i = 0; i < DEF_MAX_ERROR_LIST_COUNT; i++)
	{
		if (feof(fpRead))
			break;
		TCHAR buf[1000];
		fgetws(buf, 1000, fpRead);
		CString strMsg = CString(buf);
		strMsg.Replace(_T("@@"), _T("\r\n"));
		
		int iTimeIndex = strMsg.Find(_T(","));
		if(strErrorTime != NULL)
			(*strErrorTime)[i] = strMsg.Left(iTimeIndex);
		strMsg = strMsg.Mid(iTimeIndex + 1);
		
		int iCodeIndex = strMsg.Find(_T(","));
		if(iErrorCode != NULL)
			(*iErrorCode)[i] = _ttoi(strMsg.Left(iCodeIndex));
		strMsg = strMsg.Mid(iCodeIndex + 1);

		if(strErrorMsg != NULL)
			(*strErrorMsg)[i] = strMsg;
	}

	fclose(fpRead);

	return i;
}




MDisplayManager* MDisplayManager::m_LM_Display[eLANGUAGE_MAX_COUNT] = {NULL, NULL, NULL, NULL};

MDisplayManager::MDisplayManager(CString strFileName) : MFileManager(strFileName) { }    
MDisplayManager::MDisplayManager(CString strFileName, CString strPath) : MFileManager(strFileName, strPath) { }
MDisplayManager::MDisplayManager() : MFileManager(){ }

MDisplayManager* MDisplayManager::GetInstance(eLanguage_List eLanguage_Number)
{
	if(m_LM_Display[eLANGUAGE_KOREAN] == NULL)
		m_LM_Display[eLANGUAGE_KOREAN] = new MDisplayManager(DEF_FILE_NAME_DISPLAY[eLANGUAGE_KOREAN], DEF_DIRECTION_LANGUAGE_FILE);
	if(m_LM_Display[eLANGUAGE_ENGLISH] == NULL)
		m_LM_Display[eLANGUAGE_ENGLISH] = new MDisplayManager(DEF_FILE_NAME_DISPLAY[eLANGUAGE_ENGLISH], DEF_DIRECTION_LANGUAGE_FILE);
	if(m_LM_Display[eLANGUAGE_CHINESE] == NULL)
		m_LM_Display[eLANGUAGE_CHINESE] = new MDisplayManager(DEF_FILE_NAME_DISPLAY[eLANGUAGE_CHINESE], DEF_DIRECTION_LANGUAGE_FILE);
	if(m_LM_Display[eLANGUAGE_SLOVAK] == NULL)
		m_LM_Display[eLANGUAGE_SLOVAK] = new MDisplayManager(DEF_FILE_NAME_DISPLAY[eLANGUAGE_SLOVAK], DEF_DIRECTION_LANGUAGE_FILE);

	return m_LM_Display[eLanguage_Number];
}

int MDisplayManager::ReadMessage(CString strSection, CString strKey, CString* pstrValue)
{
	return GetValue(strSection, strKey, pstrValue);
}
int MDisplayManager::WriteMessage(CString strSection, CString strKey, CString strValue)
{
	return SetValue(strSection, strKey, strValue);
}


MErrorManager* MErrorManager::m_LM_Display[eLANGUAGE_MAX_COUNT] = {NULL, NULL, NULL, NULL};

MErrorManager::MErrorManager(CString strFileName) : MFileManager(strFileName) { }    
MErrorManager::MErrorManager(CString strFileName, CString strPath) : MFileManager(strFileName, strPath) { }
MErrorManager::MErrorManager() : MFileManager(){ }

MErrorManager* MErrorManager::GetInstance(eLanguage_List eLanguage_Number)
{
	if(m_LM_Display[eLANGUAGE_KOREAN] == NULL)
		m_LM_Display[eLANGUAGE_KOREAN] = new MErrorManager(DEF_FILE_NAME_ERROR[eLANGUAGE_KOREAN], DEF_DIRECTION_LANGUAGE_FILE);
	if(m_LM_Display[eLANGUAGE_ENGLISH] == NULL)
		m_LM_Display[eLANGUAGE_ENGLISH] = new MErrorManager(DEF_FILE_NAME_ERROR[eLANGUAGE_ENGLISH], DEF_DIRECTION_LANGUAGE_FILE);
	if(m_LM_Display[eLANGUAGE_CHINESE] == NULL)
		m_LM_Display[eLANGUAGE_CHINESE] = new MErrorManager(DEF_FILE_NAME_ERROR[eLANGUAGE_CHINESE], DEF_DIRECTION_LANGUAGE_FILE);
	if(m_LM_Display[eLANGUAGE_SLOVAK] == NULL)
		m_LM_Display[eLANGUAGE_SLOVAK] = new MErrorManager(DEF_FILE_NAME_ERROR[eLANGUAGE_SLOVAK], DEF_DIRECTION_LANGUAGE_FILE);

	return m_LM_Display[eLanguage_Number];
}
	
int MErrorManager::ReadMessage(CString strSection, CString strKey, CString* pstrValue)
{
	return GetValue(strSection, strKey, pstrValue);
}
int MErrorManager::WriteMessage(CString strSection, CString strKey, CString strValue)
{
	return SetValue(strSection, strKey, strValue);
}