// CommonSelectUserGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MLog.h"
#include "MPlatformOlb.h"
#include "CommonSelectUserGroupDlg.h"
#include "MPlatformOlbSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CCommonSelectUserGroupDlg dialog

CCommonSelectUserGroupDlg::CCommonSelectUserGroupDlg(MLog* pLogMng, CWnd* pParent /*=NULL*/)
	: CDialog(CCommonSelectUserGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonSelectUserGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkLogMng = pLogMng;
	m_hTitleViewWnd = (pParent != NULL) ? pParent->m_hWnd : NULL;
}

void CCommonSelectUserGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonSelectUserGroupDlg)
	DDX_Control(pDX, IDB_ENGINEER, m_btnEngineer);
	DDX_Control(pDX, IDB_MAKER, m_btnMaker);
	DDX_Control(pDX, IDB_OPERATOR, m_btnOperator);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonSelectUserGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonSelectUserGroupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonSelectUserGroupDlg message handlers

BOOL CCommonSelectUserGroupDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CCommonSelectUserGroupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonSelectUserGroupDlg)
	ON_EVENT(CCommonSelectUserGroupDlg, IDB_OPERATOR, -600 /* Click */, OnOperator, VTS_NONE)
	ON_EVENT(CCommonSelectUserGroupDlg, IDB_ENGINEER, -600 /* Click */, OnEngineer, VTS_NONE)
	ON_EVENT(CCommonSelectUserGroupDlg, IDB_MAKER, -600 /* Click */, OnMaker, VTS_NONE)
	ON_EVENT(CCommonSelectUserGroupDlg, IDB_CHANGE_PASSWORD, -600 /* Click */, OnChangePassword, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommonSelectUserGroupDlg::OnOperator() 
{
	// TODO: Add your control notification handler code here
	MOlbSystemPre.SetCurrentUserGroup(OPERATOR_GROUP);
	displayLogMessage("Operator");
	EndDialog(OPERATOR_GROUP);
}

void CCommonSelectUserGroupDlg::OnEngineer() 
{
	// TODO: Add your control notification handler code here
	if (checkPassword(ENGINEER_GROUP))
	{
		MOlbSystemPre.SetCurrentUserGroup(ENGINEER_GROUP);
		displayLogMessage("Engineer");
	}
	else
		EndDialog(NONE_GROUP);

	EndDialog(ENGINEER_GROUP);	
}

void CCommonSelectUserGroupDlg::OnMaker() 
{
	// TODO: Add your control notification handler code here
	if (checkPassword(MAKER_GROUP))
	{
		MOlbSystemPre.SetCurrentUserGroup(MAKER_GROUP);
		displayLogMessage("Maker");
	}
	else
		EndDialog(NONE_GROUP);

	EndDialog(MAKER_GROUP);
}

BOOL CCommonSelectUserGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch (MOlbSystemPre.GetCurrentUserGroup())
	{
	case OPERATOR_GROUP:
		m_btnOperator.SetValue(TRUE);
		m_btnEngineer.SetValue(FALSE);
		m_btnMaker.SetValue(FALSE);
		break;
	case ENGINEER_GROUP:
		m_btnOperator.SetValue(FALSE);
		m_btnEngineer.SetValue(TRUE);
		m_btnMaker.SetValue(FALSE);
		break;
	case MAKER_GROUP:
		m_btnOperator.SetValue(FALSE);
		m_btnEngineer.SetValue(FALSE);
		m_btnMaker.SetValue(TRUE);
		break;
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	
	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("CommonSelectUserGroupDlg_1"), &szTextTemp);
	//암호변경 및 저장
	GetDlgItem(IDB_CHANGE_PASSWORD)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("CommonSelectUserGroupDlg_2"), &szTextTemp);
	//Maker
	GetDlgItem(IDB_MAKER)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("CommonSelectUserGroupDlg_3"), &szTextTemp);
	//Engineer
	GetDlgItem(IDB_ENGINEER)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("CommonSelectUserGroupDlg_4"), &szTextTemp);
	//Operator
	GetDlgItem(IDB_OPERATOR)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonSelectUserGroupDlg::OnChangePassword() 
{
	if (changePassword())
	{
		//암호변경을 완료 했습니다.
		MyMessageBox(MY_MESSAGE_BOX,60500, _T("Confirm"));
		EndDialog(-1);
	}
}

BOOL CCommonSelectUserGroupDlg::checkPassword(EUserGroup eLoginGroup)
{
	WCHAR getbuf[128];
	CString szTemp;

	if (!GetValueBox(getbuf, 10, _T(""), _T("Input Password."), TRUE))
		return FALSE;

	if (eLoginGroup == OPERATOR_GROUP)
	{
		// jdy sesl		szTemp = decodePassword(m_plnkSystemData->m_strOperatorPassWord);
		szTemp = DecodePassword(m_plnkSystemData->m_strOperatorPassWord);
		if (wcscmp(szTemp.GetBuffer(szTemp.GetLength()), getbuf))
		{
			CString strMsg;
			strMsg.Format(_T("Operator Group Login Fail(Input Password : %s)"), getbuf);
			m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);

			//Operator Password 가 일치 하지 않습니다.
			MyMessageBox(MY_MESSAGE_BOX,60501, _T("Password Error"), M_ICONERROR);
		}
		else
			return TRUE;
	}
	else if (eLoginGroup == ENGINEER_GROUP)
	{
		// jdy sesl		szTemp = decodePassword(m_plnkSystemData->m_strEngineerPassWord);
		szTemp = DecodePassword(m_plnkSystemData->m_strEngineerPassWord);
		if (wcscmp(szTemp.GetBuffer(szTemp.GetLength()), getbuf))
		{
			CString strMsg;
			strMsg.Format(_T("Engineer Group Login Fail(Input Password : %s)"), getbuf);
			m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);

			//Engineer Password 가 일치 하지 않습니다.
			MyMessageBox(MY_MESSAGE_BOX,60502, _T("Password Error"), M_ICONERROR);
		}
		else
			return TRUE;
	}
	else
	{
		// jdy sesl		szTemp = decodePassword(m_plnkSystemData->m_strMakerPassWord);
		szTemp = DecodePassword(m_plnkSystemData->m_strMakerPassWord);
		//if (strcmp(decodePassword(m_plnkSystemData->m_strMakerPassWord), getbuf))
		if (wcscmp(szTemp.GetBuffer(szTemp.GetLength()), getbuf))
		{
			CString strMsg;
			strMsg.Format(_T("Maker Group Login Fail(Input Password : %s)"), getbuf);
			m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);

			//Maker Password 가 일치 하지 않습니다.
			MyMessageBox(MY_MESSAGE_BOX,60503, _T("Password Error"), M_ICONERROR);
		}
		else
			return TRUE;
	}

	return FALSE;
}

BOOL CCommonSelectUserGroupDlg::changePassword()
{
	CString strMsg;
	//현재 암호를 입력 하세요
	MyMessageBox(MY_MESSAGE_BOX,60504, _T("Confirm"));
	if (!checkPassword(MOlbSystemPre.GetCurrentUserGroup()))
		return FALSE;

	WCHAR rgszFirst[128];
	WCHAR rgszSecond[128];
	CString strNewPassword;

	//새로 변경할 입력 하세요
	MyMessageBox(MY_MESSAGE_BOX,60505, _T("Confirm"));

	if (!GetValueBox(rgszFirst, 10, _T(""), _T("Input New Password. (Max 10)"), TRUE))
		return FALSE;

	if (!GetValueBox(rgszSecond, 10, _T(""), _T("Input Agian New Password. (Max 10)"), TRUE))
		return FALSE;
	
	if (wcscmp(rgszFirst, rgszSecond))
	{
		//입력한 암호들이 일치하지 않습니다.
		MyMessageBox(MY_MESSAGE_BOX,60506, _T("Password Changing Confirm Error"), M_ICONERROR);
		return FALSE;
	}

	strNewPassword.Format(_T("%s"), rgszFirst);

	switch (MOlbSystemPre.GetCurrentUserGroup())
	{
	case OPERATOR_GROUP:
		m_plnkSystemData->m_strOperatorPassWord = encodePassword(strNewPassword);
		strMsg.Format(_T("Operator Group Password is changed(New Password : %s)"), m_plnkSystemData->m_strOperatorPassWord);
		m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);
		break;
	case ENGINEER_GROUP:
		m_plnkSystemData->m_strEngineerPassWord = encodePassword(strNewPassword);
		strMsg.Format(_T("Engineer Group Password is changed(New Password : %s)"), m_plnkSystemData->m_strEngineerPassWord);
		m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);
		break;
	case MAKER_GROUP:
		m_plnkSystemData->m_strMakerPassWord = encodePassword(strNewPassword);
		strMsg.Format(_T("Maker Group Password is changed(New Password : %s)"), m_plnkSystemData->m_strMakerPassWord);
		m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);
		break;		
	}

	m_plnkSystemData->WriteData();
	return TRUE;
}

CString	CCommonSelectUserGroupDlg::encodePassword(CString strCode)
{
	/*
// jdy sesl	int iLength;
// jdy sesl	int iTemp;
// jdy sesl	WCHAR szCode[128];
	CString strResult = "";
	CString strTemp;

	int iLength = strCode.GetLength();
	if (iLength <= 0 || iLength >= 128)
		return "";

// jdy sesl	wcscpy(szCode, strCode.GetBuffer(strCode.GetLength()));

	for (int i = 0; i < iLength; i++)
	{
		// jdy sesl		iTemp = szCode[i];
		//iTemp = strCode.GetAt[i];
		// jdy sesl	strTemp.Format(_T("%02X"), iTemp);
		strTemp.Format(_T("%02X"), (int)strCode.GetAt(i));
		strResult += strTemp;
	}
	*/
	int iLength = strCode.GetLength();
	if (iLength >= 128) return _T("");

	CString strResult, strTemp;
	for (int i = 0; i < iLength; i++)
	{
		strTemp.Format(_T("%02X"), (int)strCode.GetAt(i));
		strResult += strTemp;
	}
	return strResult;
}

// jdy sesl		CString	CCommonSelectUserGroupDlg::decodePassword(CString strCode)	
CString	CCommonSelectUserGroupDlg::DecodePassword(CString strCode)	
{
	int iLength;
	int iTemp;
	int i, j;
	char szCode[128];
// jdy sesl 081229	CString strResult = "";
// jdy sesl 081229	CString strTemp;

	iLength = strCode.GetLength();
	if (iLength <= 0 || iLength >= 128)
		return "";

	if (iLength%2 != 0)
		return "";

	for (i=0, j=0; i<iLength; i+=2, j++)
	{
		iTemp = _wtoi(strCode.Mid(i, i+2));
		swscanf(strCode.Mid(i,2), _T("%X"), &iTemp);
		szCode[j] = (char)iTemp;
	}

	szCode[j] = NULL;

	/* jdy sesl 081229
	strResult.Format(_T("%s"), szCode);

	return strResult;
	*/
	return CString(szCode);
}

void CCommonSelectUserGroupDlg::displayLogMessage(CString strUserGroup)
{
	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("CommonSelectUserGroupDlg_5"), &szTextTemp);
	// 로 Log in 되었습니다.
	// jdy sesl		CString strMsg = strUserGroup + szTextTemp;
	CString strMsg = strUserGroup + _T(" ") + szTextTemp;
	
	/* jdy sesl
	WCHAR szMsg[256];
	memset(szMsg, 0, sizeof(szMsg));
	wcscpy(szMsg, strMsg.GetBuffer(strMsg.GetLength()));
	if (NULL != m_hTitleViewWnd)
		::SendMessage(m_hTitleViewWnd, WM_ALARM_MSG, NULL, (LPARAM)szMsg);
	*/
	if (NULL != m_hTitleViewWnd)
		::SendMessage(m_hTitleViewWnd, WM_ALARM_MSG, NULL, (LPARAM)(strMsg.GetBuffer(strMsg.GetLength())));

	m_plnkLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strMsg, __FILE__, __LINE__);
}
