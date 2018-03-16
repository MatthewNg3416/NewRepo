// VersionDisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "VersionDisplayDlg.h"
#include "MPlatformOlbSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVersionDisplayDlg dialog


CVersionDisplayDlg::CVersionDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVersionDisplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVersionDisplayDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVersionDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVersionDisplayDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDS_DISPLAY, m_ListDisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVersionDisplayDlg, CDialog)
	//{{AFX_MSG_MAP(CVersionDisplayDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVersionDisplayDlg message handlers

BOOL CVersionDisplayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString	strMsg;

	strMsg = _T("Ver.20140702 -  Test");		
	m_ListDisplay.InsertString(0, strMsg);

	strMsg = _T("Ver.20140707 -  Version Display 추가");
	m_ListDisplay.InsertString(0, strMsg);

	strMsg = _T("Ver.20170220 -  Gantry Clean 위치 이동 동작 / Log Image 폴더 구분");
	m_ListDisplay.InsertString(0, strMsg);

	//나가기
	CString strTemp;
	ReadDisplayMessage(MAIN_AUTO, _T("VersionDisplayDlg_1"), &strTemp);
	GetDlgItem(IDCANCEL)->SetWindowText(strTemp);

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVersionDisplayDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVersionDisplayDlg)
	ON_EVENT(CVersionDisplayDlg, IDCANCEL, -600 /* Click */, OnClickCancel, VTS_NONE)	
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVersionDisplayDlg::OnClickCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}