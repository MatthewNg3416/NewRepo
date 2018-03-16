// DataTabICImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataTabICImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataTabICImageDlg dialog


CDataTabICImageDlg::CDataTabICImageDlg(int iSel, CWnd* pParent /*=NULL*/)
	: CDialog(CDataTabICImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataTabICImageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iWorkSide = iSel;
}


void CDataTabICImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTabICImageDlg)
	DDX_Control(pDX, IDC_TAB_IMAGE, m_sTabImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataTabICImageDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTabICImageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTabICImageDlg message handlers

BOOL CDataTabICImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	WCHAR szCurrentDirectory[255];
	::GetCurrentDirectory(255, szCurrentDirectory);

	if (m_iWorkSide == 0 || m_iWorkSide == 1)
		lstrcat(szCurrentDirectory, _T("\\res\\Xº¯.jpg"));
	else
		lstrcat(szCurrentDirectory, _T("\\res\\Yº¯.jpg"));
	
	m_sTabImage.SetPicture(szCurrentDirectory);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataTabICImageDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataTabICImageDlg)
	ON_EVENT(CDataTabICImageDlg, IDB_CANCEL, -600 /* Click */, OnClickTabDataCancel, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataTabICImageDlg::OnClickTabDataCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}
