#include "stdafx.h"
#include "MsystemData.h"
#include "MErrorMsg.h"
#include "MyProgressWnd.h"
#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MErrorMsg::MErrorMsg(CString strFileName) : MFileManager(strFileName)
{
}

MErrorMsg::MErrorMsg(CString strFileName, CString strPath) : MFileManager(strFileName, strPath)
{
}

MErrorMsg::~MErrorMsg()
{
}

CString MErrorMsg::GetErrorMessage(int iErrorCode, UINT uiLanguageType)
{
	CString strMsg;
	CString strTemp;
	CString strErrorCode;

	strErrorCode.Format(_T("%d"), iErrorCode);

	// 이기호 수정
	CString szTemp;
	switch (uiLanguageType)
	{
	case LANGUAGE_KOREAN:
	case LANGUAGE_KOR_ENG:
		if (GetValue("ERROR KOREAN", strErrorCode, &strTemp) == FALSE)
		{
			ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_66000"), &szTemp);
			//가 정의되어 있지 않습니다.
			strTemp.Format(_T("Error code [%d] %s"), iErrorCode,szTemp);
			return strTemp;
		}

		if (LANGUAGE_KOREAN == uiLanguageType)
			break;
		
		strMsg += strTemp + "\r\n";
		/* Fall Through */
	default:
	case LANGUAGE_ENGLISH:
		if (GetValue("ERROR ENGLISH", strErrorCode, &strTemp) == FALSE)
		{
			ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_66000"), &szTemp);
			//가 정의되어 있지 않습니다.
			strTemp.Format(_T("Error code [%d] %s"), iErrorCode,szTemp);
			return strTemp;
		}
		break;
	}

	strMsg += strTemp;
	return strMsg;
}