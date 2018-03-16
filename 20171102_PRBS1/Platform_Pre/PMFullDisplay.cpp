// PMFullDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "Mplatformolb.h"
#include "PMFullDisplay.h"

#include "MPlatformOlbSystem.h"
#include "MLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPMFullDisplay dialog

#ifdef DEF_PLATFORM_ARP_SYSTEM
extern MPlatformOlbSystem	MOlbSystemACF;
#elif defined (DEF_PLATFORM_PREBONDER_SYSTEM)
extern MPlatformOlbSystem	MOlbSystemPre;
#elif defined (DEF_PLATFORM_FINALBONDER_SYSTEM)
extern MPlatformOlbSystem	MOlbSystemFinal;
#endif

CPMFullDisplay::CPMFullDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CPMFullDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPMFullDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CPMFullDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMFullDisplay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPMFullDisplay, CDialog)
	//{{AFX_MSG_MAP(CPMFullDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPMFullDisplay message handlers

BOOL CPMFullDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTemp;
	
	// TODO: Add extra initialization here
	SetWindowText(_T("**  전체 화면 PM Mode 입니다.  **"));

	//나가기
	ReadDisplayMessage(MAIN_MANUAL, _T("PMFullDisplay_1"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);
		

	SetWindowPos(&(CWnd::wndTopMost), 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CPMFullDisplay, CDialog)
    //{{AFX_EVENTSINK_MAP(CPMFullDisplay)
	ON_EVENT(CPMFullDisplay, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

CString	CPMFullDisplay::DecodePassword(CString strCode)	
{
	int iLength;
	int iTemp;
	int i,j;
	char szCode[128];

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

	return CString(szCode);
}

BOOL CPMFullDisplay::checkPassword()
{
	WCHAR getbuf[128];
	CString szTemp;

	if (!GetValueBox(getbuf, 10, _T(""), _T("Input Password."), TRUE))
		return FALSE;

	szTemp = DecodePassword(m_plnkSystemData->m_strMakerPassWord);

	if (wcscmp(szTemp.GetBuffer(szTemp.GetLength()), getbuf))
	{
		CString strMsg;
		strMsg.Format(_T("Maker Group Login Fail(Input Password : %s)"), getbuf);

		//Maker Password 가 일치 하지 않습니다.
		MyMessageBox(MY_MESSAGE_BOX,60503, _T("Password Error"), M_ICONERROR);
	}
	else
		return TRUE;

	return FALSE;
}

void CPMFullDisplay::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (checkPassword())
	{
		OnCancel();
		EndDialog(MAKER_GROUP);
	}
}
